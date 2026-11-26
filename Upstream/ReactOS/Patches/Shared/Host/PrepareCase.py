"""Prepare production function bodies for Run.cmd; the case itself is C."""
import argparse
from pathlib import Path
import re
import runpy
import subprocess

HERE = Path(__file__).resolve().parent
PATCHES = HERE.parents[1]
FAT = PATCHES / "Storage/FAT32VolumeFlush/Tests/Host"
SOURCES = {
    "bcb": FAT / "Bcb.c", "truncate": FAT / "Truncate.c",
    "writeback": PATCHES / "Storage/MMFlushWriteErrors/Tests/Writeback.c",
    "user": PATCHES / "Graphics/UserInitUnlock/Assets/User.c",
    "relation": PATCHES / "Graphics/MonitorTargetRelation/Assets/Relation.c",
    "monitor-list": PATCHES / "Graphics/MonitorListCompletion/Tests/MonitorList.c",
    "ioctl": PATCHES / "Graphics/MonitorListCompletion/Tests/Ioctl.c",
    "video": PATCHES / "Graphics/VideoTargetPDO/Assets/Video.c",
}
PIN = "4fbbefd614b218afd3f0cbf09aa32998ea3ad7a6"
p = argparse.ArgumentParser(description=__doc__)
p.add_argument("output", type=Path)
p.add_argument("suite", choices=(*SOURCES, "bcb-life", "bcb-owner", "fat", "fat-clean-chain",
                                 "fat-volume", "fat-volume-legacy", "collision", "single-page", "short-write"))
p.add_argument("--source", type=Path, default=PATCHES.parent / "Source")
p.add_argument("--stock", action="store_true", help="read the pinned Git blobs")
p.add_argument("--mm-only", action="store_true", help="pair patched MM with stock CC; negative control for writeback")
p.add_argument("--overlay", type=Path, help="read matching private candidate files before the source checkout")
args = p.parse_args()
if args.mm_only:
    if args.stock or args.suite != "writeback":
        p.error("--mm-only is a writeback negative control on a patched source tree")
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
    if suite == "short-write":
        tests = PATCHES / "Storage/FAT32InstallMirrorDivergence/Tests"
        runpy.run_path(str(tests / "CheckShortWrite.py"))["prepare"](out, src, PIN)
        return tests / "ShortWrite.c"
    handoff = "CcpWriteMetadataPage(" in read(pin)
    if suite == "fat-volume" and (handoff or "CcpBeginBcbFlush(" in read(pin)):
        source = generate("bcb", out)
        with (out / "bcb.inc").open("a", encoding="utf-8") as f:
            f.write(defs(fat, [("FatFlushVolume", "NTSTATUS")]))
        return source
    if suite in ("writeback", "collision", "single-page", "short-write"):
        names = []
        if re.search(r"^MiCheckDirtySegment\(", read(mm), re.M):
            names.append(("MiCheckDirtySegment", "static BOOLEAN"))
        names += [("MmCheckDirtySegment", "BOOLEAN NTAPI"), ("MmFlushSegment", "NTSTATUS NTAPI")]
        (out / "cache-flush-functions.inc").write_text(defs(mm, names), encoding="utf-8")
        if suite == "collision":
            return PATCHES / "Storage/MMFlushCollision/Assets/CacheFlushCollisionTests.c"
        if suite == "single-page":
            return PATCHES / "Storage/MMFlushWriteErrors/Tests/SinglePage.c"
        data = defs("ntoskrnl/cc/view.c", [("CcRosFlushVacb", "NTSTATUS"),
                                 ("CcRosFlushDirtyPages", "NTSTATUS"), ("CcFlushCache", "VOID NTAPI")])
    elif suite == "truncate":
        data = defs(pin, [("CcpCanPurgeBcbRange", "BOOLEAN"), ("CcpTruncateBcbs", "BOOLEAN"),
                          ("CcpFreeBcbList", "VOID")])
        data += defs(mm, [("MmPurgeSegment", "BOOLEAN NTAPI")])
        data += defs("ntoskrnl/cc/view.c", [("CcpSignalUninitializeEvents", "static VOID"),
                     ("CcpReuseFileCache", "static PCACHE_UNINITIALIZE_EVENT"),
                     ("CcRosDeleteFileCache", "static NTSTATUS"), ("CcpDereferenceCacheMap", "VOID"),
                     ("CcRosUninitializeFileCache", "NTSTATUS")])
        data += defs("ntoskrnl/cc/fs.c", [("CcpPurgeCacheSection", "static BOOLEAN"),
                     ("CcPurgeCacheSection", "BOOLEAN NTAPI"), ("CcpNoteTruncate", "static VOID"),
                     ("CcpFinishTruncate", "static NTSTATUS"), ("CcSetFileSizesEx", "NTSTATUS NTAPI"),
                     ("CcpApplyCloseSize", "VOID"),
                     ("CcSetFileSizes", "VOID NTAPI"),
                     ("CcUninitializeCacheMap", "BOOLEAN NTAPI"), ("CcGetFileSizes", "BOOLEAN NTAPI")])
    elif suite == "bcb":
        extra = []
        if "CcpBeginBcbFlush(" in read(pin):
            extra = [("CcpFreeBcb","static VOID"), ("CcpFreeBcbList","static VOID"),
                     ("CcpBeginBcbFlush","BOOLEAN"), ("CcpCompleteBcbFlush","BOOLEAN")]
            extra.append(("CcpDeleteBcbs" if "CcpDeleteBcbs(" in read(pin) else "CcpPurgeBcbs","VOID"))
        if handoff:
            extra = [("CcpFreeBcb","static VOID"), ("CcpFreeBcbList","static VOID"), ("CcpDeleteBcbs","NTSTATUS")]
        extra += [("CcpFindBcb", "PINTERNAL_BCB"), ("CcpDereferenceBcb", "VOID")]
        if "CcpReferenceBcb(" in read(pin):
            extra += [("CcpReferenceBcb", "static NTSTATUS"), ("CcpCanPurgeBcbRange", "BOOLEAN")]
        data = defs(pin, extra + [("CcpGetAppropriateBcb", "PVOID"), ("CcSetDirtyPinnedData", "VOID NTAPI"),
                   ("CcUnpinDataForThread", "VOID NTAPI"), ("CcUnpinData", "VOID NTAPI"),
                   ("CcpPinData", "BOOLEAN"), ("CcPinRead", "BOOLEAN NTAPI"),
                   ("CcMapData", "BOOLEAN NTAPI"), ("CcRepinBcb", "VOID NTAPI"),
                    ("CcUnpinRepinnedBcb", "VOID NTAPI")]) + defs(fat, [("FatFlushFat", "NTSTATUS")])
        if "CcpPurgeBcbs(" in read(pin):
            data += "\n#define CcpDeleteBcbs(map) CcpPurgeBcbs(map,0,MAXLONGLONG)\n"
        if handoff:
            text = read(pin)
            data += text[text.index("typedef struct _CC_PAGE_OWNER"):text.index("static\nBOOLEAN\nCcpPinData(")]
    elif suite in ("fat", "fat-clean-chain"):
        text = read(fat)
        whole = definition(text, "FatCommonFlushBuffers", "NTSTATUS")
        # Drop only the unrelated file/directory switch arms; keep the volume
        # path, finally, completion callback and lower-driver call verbatim.
        a = whole.index("case ", whole.index("switch (TypeOfOpen)"))
        b = whole.index("case UserVolumeOpen:", a)
        data = defs(fat, [("FatFlushCompletionRoutine", "NTSTATUS NTAPI")]) + whole[:a] + whole[b:]
        data += defs("drivers/filesystems/fastfat/shutdown.c", [("FatCommonShutdown", "NTSTATUS")])
        if suite == "fat-clean-chain":
            data = definition(text, "FatFlushVolume", "NTSTATUS") + data
        (out / "fat.inc").write_text(data, encoding="utf-8")
        return FAT / "Fat.c"
    elif suite in ("fat-volume", "fat-volume-legacy"):
        data = defs("ntoskrnl/fsrtl/largemcb.c", [("FsRtlGetNextBaseMcbEntry", "BOOLEAN NTAPI"),
                                                ("FsRtlGetNextLargeMcbEntry", "BOOLEAN NTAPI")])
        data += defs("drivers/filesystems/fastfat/fsctrl.c", [("FatGetNextMcbEntry", "BOOLEAN")])
        data += defs(fat, [("FatFlushFat", "NTSTATUS"), ("FatFlushVolume", "NTSTATUS")])
        (out / "fat-volume.inc").write_text(data, encoding="utf-8")
        return FAT / "FatVolumeLegacy.c"
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
    return SOURCES[suite]


if __name__ == "__main__":
    suite, out = args.suite, args.output.resolve()
    source = generate("bcb" if suite in ("bcb-life", "bcb-owner") else suite, out)
    options = [f'/I"{out}"', f'/I"{HERE}"']
    if suite == "single-page":
        options.append(f'/I"{PATCHES / "Storage/MMFlushCollision/Assets"}"')
    if suite == "fat-clean-chain":
        options.append("/DKW_FAT_FLUSH_CHAIN")
    if source == SOURCES["bcb"]:
        options.append(f'/I"{PATCHES / "Storage/CcDuplicateBcb/Assets"}"')
        pin = read("ntoskrnl/cc/pin.c")
        handoff = "CcpWriteMetadataPage(" in pin
        if handoff or "CcpBeginBcbFlush(" in pin:
            options.append("/DKW_CC_OWNERSHIP")
            if handoff:
                options.append("/DKW_CC_HANDOFF")
            if "CcpReferenceBcb(" in pin:
                options.append("/DKW_CC_SPLIT_MAPPING")
            if suite == "fat-volume":
                options.append("/DKW_CC_VOLUME")
    (out / "Case.rsp").write_text("\n".join([*options, f'"{source}"', ""]), encoding="utf-16")
