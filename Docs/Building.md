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
See the [export-level comparison](Notes/DLL-Export-Version/README.md) for
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

The default patches fix `spec2def` exports, the PC BIOS FreeLdr PE path,
and the following issues:

- [Generated headers](../Build/Patches/ReactOS-generated-header-deps.patch):
  lwIP must wait for `wdm.h` and the static PathCch library for `wtypes.h`.
- [Media targets](../Build/Patches/ReactOS-vs-media-deps.patch): **Build
  Solution** leaves image assembly to explicit targets. When building the
  BootCD, the hive projects wait for the shared registry INF producer.
- [VBoxVGA text mode](../Build/Patches/ReactOS-blue-vbox-vbe.patch): on the
  VirtualBox VGA adapter, Setup switches from the firmware framebuffer to
  VGA text mode with the correct row width.
- [File information I/O](../Build/Patches/ReactOS-io-sync-status.patch):
  complete inline query/set requests before returning their stack-based
  status blocks. Debug Setup exercises this path while copying files.
- [Serial debugger discovery](../Build/Patches/ReactOS-kd-auto-probe.patch) and
  [RTC initialization](../Build/Patches/ReactOS-rtc-periodic.patch): bound the
  initial wait for an unattached debugger, and start the APIC HAL's periodic
  clock after the interrupt route is ready. Both waits were observed in
  saved Hyper-V x64 Debug guest CPU states.
- [Pinned cache writes](../Build/Patches/ReactOS-cc-pinned-dirty.patch):
  preserve a dirty buffer's final modifications even if writeback runs
  between the filesystem marking the buffer dirty and releasing its pin.

`ARM64` requires the ARM64 MSVC tools. It applies the experimental NTDLL port
and [builds the host tools for Win32](../Build/Patches/ReactOS-arm64-vs-host-tools.patch)
so they run on the x64 build machine. `Configure.cmd x64 Debug --image-fixes`
also applies the memory-manager flush-collision change.
The evidence and its limits are recorded before the first diff
in each patch. The old parser-snapshot adapter is unnecessary with RosBE.

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

A concrete x64 **BIOS** difference is visible on the same ReactOS commit
(`1103f43a0c0`), configuration (Release) and compiler: the upstream
[FreeLdr rule](../Upstream/ReactOS/Source/boot/freeldr/freeldr/pcat.cmake)
concatenates `frldr16.bin` with `${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:freeldr_pe>`.
That expression supplies a filename, but the VS generator puts the PE in
`Release/freeldr_pe.exe`. In a Windows `copy /B` check with a present first
file and a missing second file, the command returned success and wrote only
the first file. In the retained upstream build, `freeldr.sys` was
byte-identical to the 2,048-byte prefix alone. In the old Kundows build it
was byte-identical to the prefix plus the 173,568-byte PE (175,616 bytes).
The unmodified rule is unchanged at `4fbbefd614b`. The
[FreeLdr patch](../Build/Patches/ReactOS-freeldr-target-file.patch) uses
`$<TARGET_FILE:freeldr_pe>` to get the linked PE for the active configuration.
The generated x64 Debug and Win32 Release projects both reference that PE
as an input to the concatenation rule. The UEFI loader uses a separate rule.

The earlier `0x502` x64 Debug BootCD completed a fresh FAT installation
on Hyper-V Gen 1 and booted from the installed disk to an interactive
desktop. See the [tested ISO hash, first disk boot and Ctrl+Alt+Del
screenshots](Notes/BootCD-x64-Setup/README.md#hyper-v-gen-1).

With Visual Studio 2026, the initial clean x64 Debug **Build Solution** completed
with 1,366 projects built and no failures. `MakeISO.cmd` then produced a
256,901,120-byte BootCD containing both El Torito entries. Its BIOS loader
matched `frldr16.bin` (2,048 bytes) plus the linked PE (372,224 bytes); its
EFI loader matched `uefildr.efi` (488,448 bytes). In the earlier VirtualBox BIOS test,
the COM1 banner appears before kernel entry; the same build subsequently
started copying files with no serial port. On VirtualBox EFI64, the text-mode
fix restored a readable Setup language page. See the [comparison with the
pre-rework ISO and screenshots](Notes/BootCD-x64-Setup/README.md#historical-vboxvga-comparison). Win32 Release was checked through
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
- Do not commit changes to the ReactOS submodule; put Kundows changes in
  `Build\Patches`.
- Keep the ReactOS submodule at an explicit upstream commit and apply the
  Kundows patch set when preparing a build.
- When synchronizing upstream, advance the gitlink, check patch applicability
  and behavior, build the binaries and `bootcd`, then boot-test the image.
- If a Kundows change appears suitable for ReactOS upstream, submitting it is
  optional. Before doing so, review the upstream contribution requirements and
  confirm that the change satisfies its source-provenance, clean-room, and
  other applicable requirements.
