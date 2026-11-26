"""Read-only x64 Debug BootCD identity checks. Run from the checkout root."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
from Series import MANIFEST, ROOT, selected

OUT = Path("OutDir/amd64/Debug")
PUB = Path("OutDir/bootcd-x64-Debug.iso")
OLD = "fe13b2ac52af5ae6bd121ef39032744f77fbb33894c3bfc423e4fac51111bc09"
PE = {
    "system32/ntoskrnl.exe": "ntoskrnl/Debug/ntoskrnl.exe",
    "system32/ntkrnlmp.exe": "ntoskrnl/ntkrnlmp/Debug/ntkrnlmp.exe",
    "system32/drivers/fastfat.sys": "drivers/filesystems/fastfat/Debug/fastfat.sys",
}
TAR = str(Path(os.environ["SystemRoot"]) / "System32/tar.exe")


def require(test, message):
    if not test:
        raise ValueError(message)


def digest(data):
    return hashlib.sha256(data).hexdigest()


def filehash(path):
    with open(path, "rb") as f:
        h = hashlib.sha256()
        for block in iter(lambda: f.read(1 << 20), b""):
            h.update(block)
        return h.hexdigest()


def git(root, *args):
    return subprocess.check_output(["git", "-C", root, *args])


def candidate(image_fixes=False):
    src = Path("Upstream/ReactOS/Source")
    head = git(src, "rev-parse", "HEAD").decode().strip()
    require(head == git(".", "rev-parse", ":Upstream/ReactOS/Source").decode().strip(), "gitlink")
    diff = git(src, "diff", "--no-ext-diff", "--no-textconv", "--binary", "HEAD", "--")
    extra = git(src, "ls-files", "--others", "--exclude-standard", "-z").split(b"\0")
    inputs = [*(p.relative_to(ROOT) for p in selected(image_fixes=image_fixes)),
              MANIFEST.relative_to(ROOT), Path("Build/ReactOS.cmd"), Path("Build/ReactOSSeries.cmd"),
              *(src / os.fsdecode(k) for k in extra if k)]
    return dict(pin=head, diff=digest(diff), inputs={p.as_posix(): filehash(p) for p in sorted(inputs)})


def inspect(iso, expected):
    listing = subprocess.check_output([TAR, "-tvf", iso]).splitlines()
    require(set(expected) == set(PE), "incomplete PE identities")
    for name in PE:
        target = "amd64/" + name
        alias = f" reactos/{name} link to {target}".encode()
        require(sum(line.endswith(alias) for line in listing) == 1, f"ambiguous alias: {name}")
        b = subprocess.check_output([TAR, "-xOf", iso, target])
        require(b.startswith(b"MZ") and digest(b) == expected[name], f"payload mismatch: {target}")
    cab = subprocess.check_output([TAR, "-xOf", iso, "amd64/reactos.cab"])
    members = subprocess.check_output([TAR, "-tf", "-"], input=cab).decode(errors="replace").splitlines()
    require(members and not ({Path(m).name.lower() for m in members} &
                             {Path(n).name for n in PE}), "CAB layout changed; inspect duplicate payloads")


def main():
    p = argparse.ArgumentParser()
    p.add_argument("mode", choices=["identity", "links", "media", "baseline"])
    p.add_argument("--image-fixes", action="store_true", help="identify the optional x64 queue")
    p.add_argument("files", nargs="*")
    a = p.parse_args()
    if a.mode == "identity":
        result = candidate(a.image_fixes)
    elif a.mode == "baseline":
        iso, ids = map(Path, a.files)
        require(filehash(iso) == OLD, "not the recorded baseline")
        inspect(iso, json.loads(ids.read_bytes()))
        result = "BASELINE ONLY"
    else:
        identity = Path(a.files[0])
        require(candidate(a.image_fixes) == json.loads(identity.read_bytes()), "candidate identity changed")
        state = dict(candidate=filehash(identity), linked={n: filehash(OUT / v) for n, v in PE.items()})
        if a.mode == "links":
            result = state
        else:
            _, linkfile, makelog, tested = a.files
            require(state == json.loads(Path(linkfile).read_bytes()), "linked build changed")
            # Logs written by cmd.exe redirection (not PowerShell transcoding).
            last = Path(makelog).read_bytes().splitlines()[-1].strip().lower()
            published = os.fsencode(str(PUB.resolve())).lower()
            require(last in (b'bootcd: "' + published + b'"', b"bootcd: " + published),
                    "no completed publication")
            h = filehash(PUB)
            require(h != OLD and filehash(tested) == h, "old or different test ISO")
            inspect(tested, state["linked"])
            result = dict(**state, iso=h, tested=str(Path(tested).resolve()), makeLog=filehash(makelog))
    print(json.dumps(result, indent=2, sort_keys=True))


if __name__ == "__main__":
    try:
        main()
    except (ValueError, OSError, subprocess.CalledProcessError, IndexError) as exc:
        raise SystemExit(f"FAIL: {exc}")
