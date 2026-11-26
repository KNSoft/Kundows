# Documentation

- [Build guide](Building.md)
- [Windows reimplementation and Hyper-V verification](windows-reimplementation.md)
- [Running host C tests](../Upstream/ReactOS/Patches/Shared/README.md)
- [Commit scopes](Scopes.json)

## Notes

The [ReactOS patch directory](../Upstream/ReactOS/Patches/) is grouped by **BuildSystem**, **ARM64**, **Kernel**, **Graphics**, **Storage** and **Applications**. Each problem below links to its own directory; `Shared/` holds the common host test tools.

### Build and toolchain

- [Choosing the Windows DLL export level](../Upstream/ReactOS/Patches/BuildSystem/DLLExportVersion/README.md)
- [Decorated C++ exports on Win32](../Upstream/ReactOS/Patches/BuildSystem/Spec2DefCppExports/README.md)
- [FreeLdr missing the configuration-specific PE](../Upstream/ReactOS/Patches/BuildSystem/FreeLdrTargetFile/README.md)
- [Generated headers missing during a parallel build](../Upstream/ReactOS/Patches/BuildSystem/GeneratedHeaderDeps/README.md)
- [Registry INF sharing violations during media assembly](../Upstream/ReactOS/Patches/BuildSystem/VSMediaDeps/README.md)
- [Missing ARM64 NTDLL/RTL objects](../Upstream/ReactOS/Patches/ARM64/ARM64Ntdll/README.md)
- [ARM64 solution built the wrong host-tool architecture](../Upstream/ReactOS/Patches/ARM64/ARM64VSHostTools/README.md)

### Storage and cache

| Problem | Status |
|---|---|
| [FAT32 volume flush returned success with both allocation tables still stale](../Upstream/ReactOS/Patches/Storage/FAT32VolumeFlush/README.md) | Fixed in CC; verified at flush return and after remount. |
| [One FAT copy on the original installation disk lost 29 entries](../Upstream/ReactOS/Patches/Storage/FAT32InstallMirrorDivergence/README.md) | Disk and boot symptoms confirmed; first divergent write not captured. |
| [A failed paging write was counted as successful](../Upstream/ReactOS/Patches/Storage/MMFlushWriteErrors/README.md) | Fixed on the tested writeback and retry paths. |
| [A duplicate pin could lock the wrong buffer record](../Upstream/ReactOS/Patches/Storage/CcDuplicateBcb/README.md) | Fixed; covered by the ownership regression. |
| [An in-flight page write collided with a second flush](../Upstream/ReactOS/Patches/Storage/MMFlushCollision/README.md) | Optional; verified by a controlled two-writer case. |
| [First boot found a short HAL file chain](../Upstream/ReactOS/Patches/Storage/FAT32SetupHalChain/README.md) | Broken chain observed on an earlier install; first wrong write unknown. |

### Display and startup

- [EFI text Setup lost the boot framebuffer on VBoxVGA](../Upstream/ReactOS/Patches/Graphics/VBoxVGATextMode/README.md) — VirtualBox EFI and Hyper-V BIOS verified; Hyper-V UEFI stops before Setup
- [Late file-information completion during Setup](../Upstream/ReactOS/Patches/Kernel/IoSyncStatus/README.md)
- [Debugger discovery waited on a disconnected port](../Upstream/ReactOS/Patches/Kernel/KdAutoProbe/README.md)
- [TSC calibration stopped waiting for RTC samples](../Upstream/ReactOS/Patches/Kernel/RtcPeriodic/README.md)

- [win32k read the target relationship before it was delivered](../Upstream/ReactOS/Patches/Graphics/MonitorTargetRelation/README.md)
- [Monitor list query returned before its buffered output was delivered](../Upstream/ReactOS/Patches/Graphics/MonitorListCompletion/README.md)
- [Video port returned the adapter FDO for a target-device relation](../Upstream/ReactOS/Patches/Graphics/VideoTargetPDO/README.md) — open
- [GDI initialization failure left the USER lock held](../Upstream/ReactOS/Patches/Graphics/UserInitUnlock/README.md)

### Applications

- [SORT dropped the last character of an unterminated line](../Upstream/ReactOS/Patches/Applications/SortEof/README.md)
- [WHERE searched the current directory outside an explicit scope](../Upstream/ReactOS/Patches/Applications/WhereScopedSearch/README.md)
- [Notepad chose CR when LF was the most frequent ending](../Upstream/ReactOS/Patches/Applications/NotepadMixedLineEndings/README.md)
- [FINDSTR ignored line anchors](../Upstream/ReactOS/Patches/Applications/FindStrLineAnchors/README.md) — candidate rejected by long-line controls

Start in the problem directory. Its `README.md` links the reproduction, investigation, fix or open question, and verification limits. Keep the main patch beside it; put the issue's inputs, results, screenshots and single-file cases in `Assets/`. Use `Tests/` for multi-file suites. Keep full dumps and disk images outside Git.

## Commit messages

Use `[SCOPE] Summary` or `[SCOPE1][SCOPE2] Summary`. Write scopes in uppercase;
use `:` for a sub-scope such as `[NTDLL:LDR]`. Keep one change per commit,
use an imperative summary without a period, and explain why in the body.
Add new scopes to [Scopes.json](Scopes.json).
