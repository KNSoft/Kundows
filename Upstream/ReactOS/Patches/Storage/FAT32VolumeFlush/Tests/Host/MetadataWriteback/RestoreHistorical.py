"""Recreate only the two source files used in the historical negative control."""
import argparse
from hashlib import sha256
from pathlib import Path
import subprocess

root = Path(__file__).resolve().parents[8]
pin = "4fbbefd614b218afd3f0cbf09aa32998ea3ad7a6"
queue = "79db84f092e0ebc1e5aa93029e921bc2cd75b671"
files = ["ntoskrnl/cc/pin.c", "ntoskrnl/cc/view.c"]
p = argparse.ArgumentParser(description=__doc__)
p.add_argument("output", type=Path, help="a new directory to pass to Run.cmd --overlay")
p.add_argument("--stage2", type=Path, help="directory containing the two archived deltas")
args = p.parse_args()
out = args.output.resolve()
out.mkdir()  # Never modify an existing checkout or overlay.


def blob(repo, revision, name):
    return subprocess.check_output(["git", "-C", repo, "show", f"{revision}:{name}"])


def apply(data):
    subprocess.run(["git", "-C", out, "apply", *["--include=" + f for f in files], "-"],
                   input=data, check=True)


subprocess.run(["git", "init", "-q", out], check=True)
for name in files:
    dest = out / name
    dest.parent.mkdir(exist_ok=True, parents=True)
    dest.write_bytes(blob(root / "Upstream/ReactOS/Source", pin, name))
for patch in ("mm-flush-status", "cc-bcb-ownership"):
    apply(blob(root, queue, f"Upstream/ReactOS/Patches/Runtime/ReactOS-{patch}.patch"))
if args.stage2:
    for name, expected in (
        ("cc-idle-mapping-delta.patch", "990f1948b30e9d4e904d16ed2a30174f4b9d4d8e73925ad8192fa1d9d297baa2"),
        ("cc-metadata-handoff-delta.patch", "3f2de79edb742cd3a854ea7a5078226f78dc5d6f8739fe360a4f20fcf10ea7e7"),
    ):
        data = (args.stage2 / name).read_bytes()
        if sha256(data).hexdigest() != expected:
            raise ValueError(f"Wrong historical delta: {name}")
        apply(data)
for name in files:
    print(sha256((out / name).read_bytes()).hexdigest(), name)
