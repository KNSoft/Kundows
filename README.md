# Kundows

## Build

Install Git, [RosBE for Windows](https://reactos.org/wiki/Build_Environment)
and Visual Studio with the MSVC C++ x86/x64 tools and a Windows SDK. From a
regular **cmd.exe** on x64 Windows, in a checkout whose path has no spaces:

```bat
Build.cmd
MakeISO.cmd
```

Both commands default to **x64 Debug**. Binaries and the CMake build tree go
in `OutDir\<arch>\<configuration>` (`OutDir\amd64\Debug` by default). The
final image is `OutDir\bootcd-<platform>-<configuration>.iso`, for example
`OutDir\bootcd-x64-Debug.iso`. Use the same platform and configuration for
both commands. Run `Build.cmd /?` for arguments.

- `Build.cmd` initializes the pinned ReactOS submodule if missing, checks its
  revision, applies the selected patches, and builds upstream `all` using
  CMake, the `Ninja` generator, and `toolchain-msvc.cmake`. Upstream builds
  its host tools and generates parsers with Bison/Flex. Ninja handles
  incremental builds and CMake regeneration.
- `MakeISO.cmd` builds the **upstream `bootcd` target** in the same tree and
  moves the finished `bootcd.iso` to its final name under `OutDir`. Upstream
  owns the hives, CAB, file lists, boot images, `mkisofs` and `isohybrid`
  invocations.
- RosBE is located via `ROSBE_ROOT`, the active RosBE installation, the
  installer registry entry, `RosBE.cmd` on `PATH`, or `%SystemDrive%\RosBE`.
  The scripts use its Ninja, Bison, Flex, M4 and Bison data with the MSVC
  environment set by `vcvarsall`. CMake comes from `RosBE\bin\cmake.exe`
  when installed there; otherwise install **C++ CMake tools for Windows** in
  VS for its bundled CMake. No global environment changes are required.

### Patch selection

`x64` and `Win32` use the `spec2def` export fix. `ARM64` also applies the
experimental NTDLL port and requires the ARM64 MSVC tools. `--image-fixes`
adds the KD, RTC, CC and MM changes investigated for particular runtime
scenarios. The evidence and its limits are recorded before the first diff
in each patch. The old parser-snapshot adapter is unnecessary with RosBE.

Patches apply to the submodule working tree. Repeating a build recognizes
already-applied patches. Conflicts fail the build. The scripts do not remove
edits or previously selected optional patches; use a clean pinned source
tree when switching back to a smaller patch set.

### Why Ninja for a bootable ISO?

The [ReactOS build guide, VSSolution section](https://reactos.org/wiki/Building_ReactOS#VSSolution_(Visual_Studio))
recommends Ninja for a bootable ISO. `VSSolution` is a **CMake generator**;
MSVC is the compiler and works with Ninja. Building `all` only builds binaries.
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
This rule is unchanged at `4fbbefd614b`. It demonstrates one failure mode;
it does not diagnose every configuration or the independent UEFI boot path.

The x64 Release build with the default patch set at `4fbbefd614b` produced a 241,172,480-byte
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
- Add a new scope to `Scopes.json` when needed.
