# Kundows

## Build

Install Git, [RosBE for Windows](https://reactos.org/wiki/Build_Environment)
and Visual Studio 2026 with the C++ desktop workload, **C++ CMake tools for
Windows**, and a Windows SDK. From a regular **cmd.exe** on x64 Windows, in
a checkout whose path has no spaces:

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

The default patches fix `spec2def` exports, the PC BIOS FreeLdr PE path, and
two clean-build issues:

- [Generated headers](Build/Patches/ReactOS-generated-header-deps.patch):
  lwIP must wait for `wdm.h` and the static PathCch library for `wtypes.h`.
- [Media targets](Build/Patches/ReactOS-vs-media-deps.patch): **Build
  Solution** leaves image assembly to explicit targets. When building the
  BootCD, the hive projects wait for the shared registry INF producer.

`ARM64` requires the ARM64 MSVC tools. It applies the experimental NTDLL port
and [builds the host tools for Win32](Build/Patches/ReactOS-arm64-vs-host-tools.patch)
so they run on the x64 build machine. `Configure.cmd x64 Debug --image-fixes`
adds the KD, RTC, CC and MM changes investigated for particular runtime scenarios.
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
See [`boot/boot_images.cmake`](Upstream/ReactOS/Source/boot/boot_images.cmake)
for `bootcd` and [`create_iso_lists`](Upstream/ReactOS/Source/sdk/cmake/CMakeMacros.cmake)
for how the lists and CAB dependencies are created.

The earlier Kundows pipeline built its own `.slnx` projects, then its
`Build/BuildReactOSIso.ps1` prepared hives, a CAB and the EFI image, wrote its
own graft list, and ran `mkisofs` and `isohybrid`. Its successful boot test
exercised that packaging path. It was not a test of `REACTOS.sln /t:bootcd`.
The old packager and its validation record are available in commit `26617bf`.

A concrete x64 **BIOS** difference is visible on the same ReactOS commit
(`1103f43a0c0`), configuration (Release) and compiler: the upstream
[FreeLdr rule](Upstream/ReactOS/Source/boot/freeldr/freeldr/pcat.cmake)
concatenates `frldr16.bin` with `${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:freeldr_pe>`.
That expression supplies a filename, but the VS generator puts the PE in
`Release/freeldr_pe.exe`. In a Windows `copy /B` check with a present first
file and a missing second file, the command returned success and wrote only
the first file. In the retained upstream build, `freeldr.sys` was
byte-identical to the 2,048-byte prefix alone. In the old Kundows build it
was byte-identical to the prefix plus the 173,568-byte PE (175,616 bytes).
The unmodified rule is unchanged at `4fbbefd614b`. The
[FreeLdr patch](Build/Patches/ReactOS-freeldr-target-file.patch) uses
`$<TARGET_FILE:freeldr_pe>` to get the linked PE for the active configuration.
The generated x64 Debug and Win32 Release projects both reference that PE
as an input to the concatenation rule. The UEFI loader uses a separate rule.

With Visual Studio 2026, a clean x64 Debug **Build Solution** completed
with 1,366 projects built and no failures. `MakeISO.cmd` then produced a
256,901,120-byte BootCD containing both El Torito entries. Its BIOS loader
matched `frldr16.bin` (2,048 bytes) plus the linked PE (372,224 bytes); its
EFI loader matched `uefildr.efi` (488,448 bytes). In a diskless VirtualBox
BIOS guest, FreeLdr displayed its menu; selecting text-mode Setup led to a
COM1 debugger prompt. A UEFI guest reached Setup language selection, with
visible text-mode display corruption. Win32 Release was checked through
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

## Commit message

Use:

```text
[SCOPE] Summary
[SCOPE1][SCOPE2] Summary
```

Rules:

- Use uppercase scopes; use `:` for sub-scopes, such as `[NTDLL:LDR]`.
- Use one commit for one change.
- Use an imperative summary without a period; explain why in the commit body.
- Add a new scope to [docs/Scopes.json](docs/Scopes.json) when needed.
