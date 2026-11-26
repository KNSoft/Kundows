# Building Kundows

## Requirements

Install Git, [RosBE for Windows](https://reactos.org/wiki/Build_Environment)
and Visual Studio 2026 with the C++ desktop workload, **C++ CMake tools for
Windows**, and a Windows SDK. Use a checkout path without spaces on x64 Windows.

## Build commands

From a regular **cmd.exe** at the repository root:

```bat
Configure.cmd
Build.cmd
MakeISO.cmd
```

All three commands default to **x64 Debug**. After `Configure.cmd`, open
`OutDir\amd64\Debug\REACTOS.slnx` and choose **Build Solution**, or run
`Build.cmd`. `MakeISO.cmd` publishes `OutDir\bootcd-x64-Debug.iso`.

Each `Configure.cmd <platform> <configuration>` generates **one independent
solution for that pair**, on demand. For example, `Configure.cmd Win32 Release`
generates `OutDir\i386\Release\REACTOS.slnx`. Pass `Win32 Release` to the
other two commands to use that solution. ARM64 uses `OutDir\arm64`. ReactOS
uses `ARCH` and `CMAKE_BUILD_TYPE` at configure time, including for kernel
debugging and runtime checks. The generated solution exposes only the
configuration matching those settings. Move an old Ninja tree aside before
configuring VS in the same directory.

Configure sets `DLL_EXPORT_VERSION=0x600` for the target system. Re-run
`Configure.cmd` on existing `0x502` trees before using Build or MakeISO.
See the [export-level comparison](../Upstream/ReactOS/Patches/BuildSystem/DLLExportVersion/README.md) for
what this enables and why a larger value is not necessarily more compatible.

- `Configure.cmd` initializes/checks the pinned submodule, applies the
  selected patches, sets up MSVC and RosBE, then runs VS's bundled CMake
  with the `Visual Studio 18 2026` generator and `toolchain-msvc.cmake`.
  Both host and target parser builds use RosBE's Bison, Flex and M4. RosBE
  is located via `ROSBE_ROOT`, the active environment, the installer registry
  entry, `RosBE.cmd` on `PATH`, or `%SystemDrive%\RosBE`.
- `Build.cmd` invokes `devenv.com REACTOS.slnx /Build` with the selected
  configuration and platform. It uses the same solution as the IDE.
- `MakeISO.cmd` builds upstream `bootcd` in **that same VS build tree**. CMake
  and the ReactOS host tools own the file lists, hives, CAB and boot images.
  The finished ISO is moved to `OutDir\bootcd-<platform>-<configuration>.iso`.

### Patch selection

[`Series.tsv`](../Upstream/ReactOS/Patches/Series.tsv) lists one issue patch per line, in apply order. `All` selects every target, `ARM64` the ARM64 target only, and `ImageFixes` adds the optional `--image-fixes` patch. ARM64 requires the ARM64 MSVC tools. To print a selection, run `Build\ReactOSSeries.cmd List amd64` (or `i386` / `arm64`, optionally followed by `--image-fixes`).

Open `Patches/<category>/<issue>/` for the [issue README](README.md#notes) and, when a fix is selected, one main `.patch`. The category groups related problems; it does not determine patch order or platform. Attachments and simple cases go in `Assets/`; multi-file test programs go in `Tests/`. Only paths named in the series enter the build. [CheckSeries.cmd](../Upstream/ReactOS/Patches/Shared/CheckSeries.cmd) tests queue application, recognition and rollback without changing the source checkout.

Patches apply to the shared submodule working tree. `Configure.cmd`
recognizes already-applied patches and stops on conflicts. Use a clean
pinned source tree when switching back to a smaller patch set; configuring
another solution does not remove edits or previously selected patches.

### Visual Studio and the boot image

The [ReactOS build guide, VSSolution section](https://reactos.org/wiki/Building_ReactOS#VSSolution_(Visual_Studio))
recommends Ninja for a bootable ISO. Here a small upstream-source patch
corrects a known defect in the VS-generated PC BIOS loader.
See [`boot/boot_images.cmake`](../Upstream/ReactOS/Source/boot/boot_images.cmake)
for `bootcd` and [`create_iso_lists`](../Upstream/ReactOS/Source/sdk/cmake/CMakeMacros.cmake)
for how the lists and CAB dependencies are created.

The earlier Kundows pipeline built its own `.slnx` projects, then its
`Build/BuildReactOSIso.ps1` prepared hives, a CAB and the EFI image, wrote its
own graft list, and ran `mkisofs` and `isohybrid`. Its successful boot test
exercised that packaging path. It was not a test of `REACTOS.sln /t:bootcd`.
The old packager and its validation record are available in commit `26617bf`.

See the [FreeLdr inputs and byte counts](../Upstream/ReactOS/Patches/BuildSystem/FreeLdrTargetFile/README.md) for the multi-configuration loader failure.

The earlier `0x502` x64 Debug BootCD completed a fresh FAT installation
on Hyper-V Gen 1 and booted from the installed disk to an interactive
desktop. See the [tested ISO hash, first disk boot and Ctrl+Alt+Del
screenshots](../Upstream/ReactOS/Patches/Shared/BootCD/README.md#hyper-v-gen-1).

With Visual Studio 2026, the initial clean x64 Debug **Build Solution** completed
with 1,366 projects built and no failures. `MakeISO.cmd` then produced a
256,901,120-byte BootCD containing both El Torito entries. Its BIOS loader
matched `frldr16.bin` (2,048 bytes) plus the linked PE (372,224 bytes); its
EFI loader matched `uefildr.efi` (488,448 bytes). In the earlier VirtualBox BIOS test,
the COM1 banner appears before kernel entry; the same build subsequently
started copying files with no serial port. On VirtualBox EFI64, the text-mode
fix restored a readable Setup language page. See the [comparison with the
pre-rework ISO and screenshots](../Upstream/ReactOS/Patches/Graphics/VBoxVGATextMode/README.md). Win32 Release was checked through
FreeLdr (2,048 + 143,360 bytes), and ARM64 Debug through solution generation
and the native host-tools build.

The previous **Ninja** x64 Release build at `4fbbefd614b` produced a 241,172,480-byte
BootCD with the upstream `bootcd` target. Its El Torito catalog contains BIOS
and EFI boot entries. The packaged `loader/freeldr.sys` (175,616 bytes) and
`efi/boot/bootx64.efi` (207,360 bytes) matched the expected build outputs
byte for byte. These are image-content checks, not a guest boot test.

## Upstream

- ReactOS is at `Upstream\ReactOS\Source`.
- Its baseline is the Git submodule pointer.
- Keep Kundows changes in `Upstream\ReactOS\Patches`, outside the submodule.
- Keep the ReactOS submodule at an explicit upstream commit and apply the
  Kundows patch set when preparing a build.
- When synchronizing upstream, advance the gitlink, check patch applicability
  and behavior, build the binaries and `bootcd`, then boot-test the image.
- If a Kundows change appears suitable for ReactOS upstream, submitting it is
  optional. Before doing so, review the upstream contribution requirements and
  confirm that the change satisfies its source-provenance, clean-room, and
  other applicable requirements.
