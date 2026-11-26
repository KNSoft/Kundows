"""Run from a native x64 or x86 MSVC developer prompt."""
import argparse
import hashlib
import os
from pathlib import Path
import re
import subprocess as sp
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[5]
QUEUE = "79db84f092e0ebc1e5aa93029e921bc2cd75b671"
PIN = "4fbbefd614b218afd3f0cbf09aa32998ea3ad7a6"
FILES = ["ntoskrnl/cc/pin.c", "ntoskrnl/cc/view.c"]


def blob(repo, rev, path):
    return sp.check_output(["git", "-C", repo, "show", f"{rev}:{path}"])


def functions(text, names):
    # Same column-zero definition convention as ../run.py.
    parts = []
    for name, result in names:
        matches = re.findall(r"^" + name + r"[ \t]*\([^;{}]*?\)\s*(?:/\*.*?\*/\s*)?\{.*?^\}",
                             text, re.M | re.S)
        if len(matches) != 1:
            raise ValueError(f"expected one definition of {name}, got {len(matches)}")
        parts.append(result + "\n" + matches[0])
    return "\n".join(parts)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    group = parser.add_mutually_exclusive_group()
    group.add_argument("--candidate", type=Path, help="quiescent ReactOS checkout or snapshot")
    group.add_argument("--stage2", action="store_true", help="reconstruct the delivered two deltas")
    args = parser.parse_args()
    (ROOT / "OutDir").mkdir(exist_ok=True)
    out = Path(tempfile.mkdtemp(prefix="metadata-writeback-", dir=ROOT / "OutDir"))
    print(out, flush=True)
    for path in FILES:
        dest = out / path
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_bytes((args.candidate / path).read_bytes() if args.candidate else
                         blob(ROOT / "Upstream/ReactOS/Source", PIN, path))
    if not args.candidate:
        sp.run(["git", "init", "-q", out], check=True)
        # QUEUE is a historical commit: git show needs the filenames at that revision.
        for patch in ["mm-flush-status", "cc-bcb-ownership"]:
            sp.run(["git", "apply", *["--include="+p for p in FILES], "-"], cwd=out, check=True,
                   input=blob(ROOT, QUEUE, f"Upstream/ReactOS/Patches/Runtime/ReactOS-{patch}.patch"))
    manifest = [f"candidate: {args.candidate.resolve()}" if args.candidate else f"pin: {PIN} queue: {QUEUE}"]
    if args.stage2:
        for name, expected in [
                ("cc-idle-mapping-delta.patch","990f1948b30e9d4e904d16ed2a30174f4b9d4d8e73925ad8192fa1d9d297baa2"),
                ("cc-metadata-handoff-delta.patch","3f2de79edb742cd3a854ea7a5078226f78dc5d6f8739fe360a4f20fcf10ea7e7")]:
            data = (HERE.parents[1] / name).read_bytes()
            if hashlib.sha256(data).hexdigest() != expected:
                raise ValueError("delta identity mismatch: " + name)
            sp.run(["git", "apply", *["--include="+p for p in FILES], "-"], cwd=out, check=True,input=data)
            manifest.append(f"{expected}  {name}")
    for path in FILES:
        manifest.append(f"{hashlib.sha256((out / path).read_bytes()).hexdigest()}  {path}")
    (out / "inputs.txt").write_text("\n".join(manifest) + "\n", encoding="utf-8")
    print("\n".join(manifest))
    pin, view = [(out / path).read_text(encoding="utf-8") for path in FILES]
    page = bool(re.search(r"^CcpWriteMetadataPage\(", pin, re.M))
    if page:
        owners = re.findall(r"^typedef struct _CC_PAGE_OWNER\s*\{[^}]*\} CC_PAGE_OWNER;",pin,re.M)
        if len(owners) != 1:
            raise ValueError("expected one CC_PAGE_OWNER type")
        code = owners[0] + "\n" + functions(pin, [
            ("CcpFreeBcb","static VOID"), ("CcpFindBcb","static PINTERNAL_BCB"),
            ("CcpDereferenceBcb","static VOID"), ("CcpReferenceBcb","static NTSTATUS"),
            ("CcpGetAppropriateBcb","static PVOID"), ("CcpWriteMetadataPage","static NTSTATUS"),
            ("CcpFlushMetadata","NTSTATUS")])
    else:
        code = functions(pin, [("CcpFreeBcb", "static VOID"), ("CcpFreeBcbList", "static VOID"),
        ("CcpBeginBcbFlush", "BOOLEAN"), ("CcpCompleteBcbFlush", "BOOLEAN"),
        ("CcpFindBcb", "PINTERNAL_BCB"), ("CcpDereferenceBcb", "VOID"),
        ("CcpGetAppropriateBcb", "PVOID")])
    code += "\n" + functions(pin, [("CcSetDirtyPinnedData", "VOID NTAPI"),
        ("CcUnpinDataForThread", "VOID NTAPI"), ("CcUnpinData", "VOID NTAPI"),
        ("CcpPinData", "BOOLEAN"), ("CcPinRead", "BOOLEAN NTAPI"),
        ("CcRepinBcb", "VOID NTAPI"), ("CcUnpinRepinnedBcb", "VOID NTAPI")])
    code += "\n" + functions(view, [("CcRosFlushVacb", "NTSTATUS"), ("CcFlushCache", "VOID NTAPI")])
    (out / "production.inc").write_text(code, encoding="utf-8")
    env = dict(os.environ, TMP=str(out), TEMP=str(out))
    def run(cmd, log):
        with (out / "commands.txt").open("a",encoding="utf-8") as f:
            print(sp.list2cmdline(cmd), file=f)
        with (out / log).open("wb") as f:
            result = sp.run(cmd, cwd=out, env=env, stdout=f, stderr=sp.STDOUT, timeout=35)
        print((out / log).read_bytes().decode(errors="replace"))
        with (out / "commands.txt").open("a",encoding="utf-8") as f:
            print(f"{log} rc={result.returncode}", file=f)
        return result.returncode
    if run(["cl", "/nologo", "/TC", "/MT", "/W3", "/Od", *(["/DPAGE_HANDOFF"] if page else []),
            f"/I{out}", HERE / "contract.c"], "compile.log"):
        print("COMPILE ERROR")
        return 2
    results = [run([out / "contract.exe", case], case + ".log") for case in ["pin", "retry"]]
    return max(results) if all(rc in (0,1) for rc in results) else 2


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (ValueError, OSError, sp.SubprocessError) as exc:
        print(f"ASSEMBLY / EXECUTION ERROR: {exc}")
        raise SystemExit(2)
