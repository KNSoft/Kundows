"""Build host checks from the pin or a patched checkout. Use an x64 MSVC prompt."""
import argparse
from pathlib import Path
import re
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent
PIN = "4fbbefd614b218afd3f0cbf09aa32998ea3ad7a6"
p = argparse.ArgumentParser(description=__doc__)
p.add_argument("source", type=Path)
p.add_argument("--stock", action="store_true", help="read the pinned Git blobs")
p.add_argument("--mm-only", action="store_true", help="pair patched MM with stock CC; negative control for writeback")
p.add_argument("--suite", choices=("writeback", "bcb", "fat", "user", "relation", "ioctl", "video", "collision"), action="append")
args = p.parse_args()
if args.mm_only:
    if args.stock or args.suite not in (None, ["writeback"]):
        p.error("--mm-only is a writeback negative control on a patched source tree")
    args.suite = ["writeback"]
src = args.source.resolve()
assert subprocess.check_output(["git", "-C", src, "rev-parse", "HEAD"], text=True).strip() == PIN


def read(path):
    if args.stock or (args.mm_only and path == "ntoskrnl/cc/view.c"):
        return subprocess.check_output(["git", "-C", src, "show", f"{PIN}:{path}"]).decode("utf-8")
    return (src / path).read_text(encoding="utf-8")


def definition(text, name, result):
    # Upstream starts definitions (and closing braces) in column zero. Skip
    # prototypes. Preserve the argument list and the entire production body.
    chunks = re.findall(r"^" + name + r"[ \t]*\([^;{}]*?\)\s*(?:/\*.*?\*/\s*)?\{.*?^\}", text, re.M | re.S)
    if len(chunks) != 1:
        raise ValueError(f"expected one definition for {name}, found {len(chunks)}")
    return result + "\n" + chunks[0] + "\n"


def defs(path, names):
    text = read(path)
    return "\n".join(definition(text, name, result) for name, result in names)


def generate(suite, out):
    mm = "ntoskrnl/mm/section.c"
    pin = "ntoskrnl/cc/pin.c"
    fat = "drivers/filesystems/fastfat/flush.c"
    if suite in ("writeback", "collision"):
        names = []
        if re.search(r"^MiCheckDirtySegment\(", read(mm), re.M):
            names.append(("MiCheckDirtySegment", "static BOOLEAN"))
        names += [("MmCheckDirtySegment", "BOOLEAN NTAPI"), ("MmFlushSegment", "NTSTATUS NTAPI")]
        (out / "cache-flush-functions.inc").write_text(defs(mm, names), encoding="utf-8")
        if suite == "collision":
            # Existing controlled two-writer test, from before the build migration.
            root = HERE.parents[4]
            historical = subprocess.check_output(["git", "-C", root, "show",
                              "26617bf:Build/Tests/CacheFlushCollisionTests.c"])
            (out / "collision.c").write_bytes(historical)
            return out / "collision.c"
        data = defs("ntoskrnl/cc/view.c", [("CcRosFlushVacb", "NTSTATUS"),
                                 ("CcRosFlushDirtyPages", "NTSTATUS"), ("CcFlushCache", "VOID NTAPI")])
    elif suite == "bcb":
        data = defs(pin, [("CcpFindBcb", "PINTERNAL_BCB"), ("CcpDereferenceBcb", "VOID"),
                   ("CcpGetAppropriateBcb", "PVOID"), ("CcSetDirtyPinnedData", "VOID NTAPI"),
                   ("CcUnpinDataForThread", "VOID NTAPI"), ("CcUnpinData", "VOID NTAPI"),
                   ("CcpPinData", "BOOLEAN"), ("CcPinRead", "BOOLEAN NTAPI"),
                   ("CcMapData", "BOOLEAN NTAPI"), ("CcRepinBcb", "VOID NTAPI"),
                   ("CcUnpinRepinnedBcb", "VOID NTAPI")]) + defs(fat, [("FatFlushFat", "NTSTATUS")])
    elif suite == "fat":
        text = read(fat)
        whole = definition(text, "FatCommonFlushBuffers", "NTSTATUS")
        # Drop only the unrelated file/directory switch arms; keep the volume
        # path, finally, completion callback and lower-driver call verbatim.
        a = whole.index("case ", whole.index("switch (TypeOfOpen)"))
        b = whole.index("case UserVolumeOpen:", a)
        data = defs(fat, [("FatFlushCompletionRoutine", "NTSTATUS NTAPI")]) + whole[:a] + whole[b:]
        data += defs("drivers/filesystems/fastfat/shutdown.c", [("FatCommonShutdown", "NTSTATUS")])
    elif suite == "user":
        data = defs("win32ss/user/ntuser/ntuser.c", [("NtUserInitialize", "NTSTATUS APIENTRY")])
    elif suite == "relation":
        path = "win32ss/gdi/eng/device.c"
        names = [("EngpPnPTargetRelationRequest", "NTSTATUS")]
        if "EngpCompleteTargetRelation(" in read(path):
            names.insert(0, ("EngpCompleteTargetRelation", "NTSTATUS NTAPI"))
        data = defs(path, names)
    elif suite == "video":
        data = defs("win32ss/drivers/videoprt/dispatch.c", [("IntVideoPortDispatchFdoPnp", "NTSTATUS NTAPI")])
    elif suite == "ioctl":
        data = defs("win32ss/drivers/videoprt/dispatch.c", [("VideoPortEnumMonitorPdo", "NTSTATUS")])
        data += defs("win32ss/gdi/eng/device.c", [("EngDeviceIoControl", "DWORD APIENTRY")])
    (out / (suite + ".inc")).write_text(data, encoding="utf-8")
    return HERE / (suite + ".c")


if __name__ == "__main__":
    failed = 0
    for suite in args.suite or ("writeback", "bcb", "fat", "user", "relation", "video", "ioctl"):
        with tempfile.TemporaryDirectory(prefix="Kundows-check-") as tmp:
            out = Path(tmp)
            source = generate(suite, out)
            subprocess.run(["cl", "/nologo", "/W3", "/TC", "/MT", "/Od", "/I" + tmp, source],
                           cwd=out, check=True)
            r = subprocess.run([out / (suite + ".exe")], cwd=out, timeout=20)
            failed |= r.returncode != 0
    raise SystemExit(failed)
