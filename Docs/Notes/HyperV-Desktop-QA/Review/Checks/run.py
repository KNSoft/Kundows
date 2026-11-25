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
p.add_argument("--overlay", type=Path, help="read matching private candidate files before the source checkout")
p.add_argument("--suite", choices=("writeback", "bcb", "bcb-life", "bcb-owner", "fat", "fat-volume", "fat-volume-legacy", "user", "relation", "ioctl", "monitor-list", "video", "collision"), action="append")
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
    if args.overlay and (args.overlay / path).is_file():
        return (args.overlay / path).read_text(encoding="utf-8")
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
    if suite == "fat-volume" and "CcpBeginBcbFlush(" in read(pin):
        source = generate("bcb", out)
        with (out / "bcb.inc").open("a", encoding="utf-8") as f:
            f.write(defs(fat, [("FatFlushVolume", "NTSTATUS")]))
        return source
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
        extra = []
        if "CcpBeginBcbFlush(" in read(pin):
            extra = [("CcpFreeBcb","static VOID"), ("CcpFreeBcbList","static VOID"),
                     ("CcpBeginBcbFlush","BOOLEAN"), ("CcpCompleteBcbFlush","BOOLEAN")]
            extra.append(("CcpDeleteBcbs" if "CcpDeleteBcbs(" in read(pin) else "CcpPurgeBcbs","VOID"))
        data = defs(pin, extra + [("CcpFindBcb", "PINTERNAL_BCB"), ("CcpDereferenceBcb", "VOID"),
                   ("CcpGetAppropriateBcb", "PVOID"), ("CcSetDirtyPinnedData", "VOID NTAPI"),
                   ("CcUnpinDataForThread", "VOID NTAPI"), ("CcUnpinData", "VOID NTAPI"),
                   ("CcpPinData", "BOOLEAN"), ("CcPinRead", "BOOLEAN NTAPI"),
                   ("CcMapData", "BOOLEAN NTAPI"), ("CcRepinBcb", "VOID NTAPI"),
                    ("CcUnpinRepinnedBcb", "VOID NTAPI")]) + defs(fat, [("FatFlushFat", "NTSTATUS")])
        if "CcpPurgeBcbs(" in read(pin):
            data += "\n#define CcpDeleteBcbs(map) CcpPurgeBcbs(map,0,MAXLONGLONG)\n"
    elif suite == "fat":
        text = read(fat)
        whole = definition(text, "FatCommonFlushBuffers", "NTSTATUS")
        # Drop only the unrelated file/directory switch arms; keep the volume
        # path, finally, completion callback and lower-driver call verbatim.
        a = whole.index("case ", whole.index("switch (TypeOfOpen)"))
        b = whole.index("case UserVolumeOpen:", a)
        data = defs(fat, [("FatFlushCompletionRoutine", "NTSTATUS NTAPI")]) + whole[:a] + whole[b:]
        data += defs("drivers/filesystems/fastfat/shutdown.c", [("FatCommonShutdown", "NTSTATUS")])
    elif suite in ("fat-volume", "fat-volume-legacy"):
        data = defs("ntoskrnl/fsrtl/largemcb.c", [("FsRtlGetNextBaseMcbEntry", "BOOLEAN NTAPI"),
                                                ("FsRtlGetNextLargeMcbEntry", "BOOLEAN NTAPI")])
        data += defs("drivers/filesystems/fastfat/fsctrl.c", [("FatGetNextMcbEntry", "BOOLEAN")])
        data += defs(fat, [("FatFlushFat", "NTSTATUS"), ("FatFlushVolume", "NTSTATUS")])
        (out / "fat-volume.inc").write_text(data, encoding="utf-8")
        return HERE / "fat-volume-legacy.c"
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
    elif suite == "monitor-list":
        data = defs("win32ss/drivers/videoprt/dispatch.c", [("VideoPortEnumMonitorPdo", "NTSTATUS")])
        data += defs("win32ss/gdi/eng/device.c", [("EngpCompleteTargetRelation", "NTSTATUS NTAPI"),
                                                ("EngpQueryMonitorList", "NTSTATUS")])
    (out / (suite + ".inc")).write_text(data, encoding="utf-8")
    return HERE / (suite + ".c")


if __name__ == "__main__":
    failed = 0
    suites = ["writeback", "bcb", "fat", "fat-volume", "user", "relation", "video", "ioctl"]
    if "EngpQueryMonitorList(" in read("win32ss/gdi/eng/device.c"):
        suites.append("monitor-list")
    for suite in args.suite or suites:
        with tempfile.TemporaryDirectory(prefix="Kundows-check-") as tmp:
            out = Path(tmp)
            source = generate("bcb" if suite in ("bcb-life","bcb-owner") else suite, out)
            options = []
            if source.stem == "bcb" and "CcpBeginBcbFlush(" in read("ntoskrnl/cc/pin.c"):
                options.append("/DKW_CC_OWNERSHIP")
                if suite == "fat-volume":
                    options.append("/DKW_CC_VOLUME")
            subprocess.run(["cl", "/nologo", "/W3", "/TC", "/MT", "/Od", "/I" + tmp] + options + [source],
                            cwd=out, check=True)
            cmd = [out / (source.stem + ".exe")]
            if suite == "bcb-life":
                cmd.append("--lifetime")
            if suite == "bcb-owner":
                cmd.append("--ownership")
            if suite == "fat-volume" and source.stem == "bcb":
                cmd.append("--volume")
            r = subprocess.run(cmd, cwd=out, timeout=20)
            failed |= r.returncode != 0
    raise SystemExit(failed)
