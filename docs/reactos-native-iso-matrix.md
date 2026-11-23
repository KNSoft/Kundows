# Native ISO matrix (2026-11-23)

All images were built with native Windows MSBuild/MSVC. `DLL_EXPORT_VERSION`
remains `0x502`, rostests remain disabled, and packaging/layout/exact PE checks
remain enabled. Reference CMake configuration was not a CMake/Ninja build.

Batch images are retained at
`D:\VM\Kundows-IsoMatrix-20261123\Images\<Platform>\<Configuration>\bootcd.iso`.
Superseded images and failed/diagnostic installation disks were preserved.

## Current corrected images

| Platform | Configuration | Bytes | Packaged PEs | Imports | Exact PE errors |
| --- | --- | ---: | ---: | ---: | ---: |
| Win32 | Debug | 227540992 | 1596 | 111108 | 0 |
| Win32 | Release | 190840832 | 1592 | 105243 | 0 |
| x64 | Debug | 246415360 | 1571 | 107550 | 0 |
| x64 | Release | 202375168 | 1567 | 103270 | 0 |

SHA-256:

```text
Win32 Debug   0A58315084FBDC4B4537E967C389738AA4F00267220C0C6BC72F75481ACFA3DD
Win32 Release CFCBD57E9BB68D55415DF8718B6DFD547F63DEEBF09292637532AE99039ED2E1
x64 Debug     9398249C4C4C2391BBE56FE86CC51B180CD6D57BD77FDFC4659A69499EA86432
x64 Release   D5F4914038C4FDCF6C48ED42B16FE3A0B10AFABFCDB6D03312AB2240F6D5ECCC
```

## Correctness changes and regressions

- x86 decorated C++ exports: the upstream `spec2def` producer emits COFF
  `/alternatename` directives rather than truncated decorated DEF aliases.
  Six native DLL builds check 1976 exact names/ordinals; all six host-tool
  configurations and the existing 62 cases pass.
- KD UART errors return to the existing packet retry policy instead of an
  unbounded packet-leader poll. The actual production transport passes 14
  cases. Debugger discovery, genuine exceptions, prompts, and retry allowances
  are not disabled to make a boot pass.
- Pinned cache writes retain dirty intent through unpin. The original
  production functions reproduce nine failed assertions; the correction passes
  nine cases on each native host platform. Concurrent clean source patch
  application and cached rechecking pass in an isolated pristine fixture.
  See [the cache investigation](reactos-native-cache-pinning.md) for the
  observed FAT corruption and the limits of the causal evidence.
- Colliding MM writeback now serializes each page's writes and waits for
  clean-but-in-flight writes at the synchronous flush boundary. Seven
  event-controlled cases pass on each native host platform; the original
  implementation loses newer data under the controlled older-last I/O
  completion schedule. See [the MM writeback regression](reactos-native-cache-writeback.md).

Both UP and MP kernels were rebuilt in every configuration before repackaging.

## Installed-system validation

Three current images completed fresh, serial-disconnected installation and
reached responsive installed desktops, including guest Ctrl+Alt+Del:

| Image | Fully readable installed files | Exact installed PE matches | Disk checks |
| --- | ---: | ---: | --- |
| Win32 Debug | 1082 | 769 | Passed |
| x64 Debug | 1077 | 764 | Passed |
| x64 Release | 1076 | 763 | Passed |

Read-only clones retained after guest shutdown have matching FAT mirrors,
complete checked critical chains, and the expected rebuilt kernels. The x64
Release comparator initially omitted Setup's legitimate `halmp.dll` to
`hal.dll` rename; exact SHA-256 identity established the alias. The original
failed comparison and its explicit correction are both preserved.

Win32 Release remains failed: the primary SYSTEM hive declares 516096 bytes
(126 clusters) but its chain ends after 55 clusters; both FAT copies agree.
All 769 installed PEs match native outputs. Its first disk boot fails, so
four-image validation is not complete. Diagnostic copies and instrumented
kernels are used only to investigate this failure and never count as passes.

On the preserved writable diagnostic copy, logging immediately before
`KeBugCheckWithTf` captures context revealed an initial `0x7F` with parameter
1 equal to 8 (double fault), followed by another fault in `RtlCaptureContext`.
Three earlier first-chance access violations used address `0xAFD0D850`.
An earlier diagnostic logging `RtlAssert` arguments produced no assertion
message. Thus the context-capture crash alone is not evidence of an assertion,
nor does this boot trace yet explain the on-disk FAT-chain truncation.

Earlier pinned-dirty-only Debug desktop passes belong to superseded images.

The owned Generation-1 Hyper-V VMs use one CPU, fixed 1 GiB RAM, no NIC, and a
new 8 GiB dynamic disk per final installation. Tests select text Setup from
the ISO, use FAT quick format, continue disk-first through graphical Setup and
its final reboot, then verify the installed desktop and guest Ctrl+Alt+Del.
COM1 and COM2 must be disconnected throughout these final tests.

The mistaken FAT full-format selection on the first corrected Win32 Debug
test disk was stopped at 3%, before file copying. That preparation-only disk
was retained and replaced with a new blank disk; it is not recorded as an OS
failure or a successful installation.

Raw logs, image manifests, exact PE reports, VM request records, screenshots,
and read-only disk diagnostics are retained in the session's
`iso-matrix-20261123` evidence directory.

Build performance findings and proposals (not implemented) are in
[the build investigation](reactos-native-build-investigation.md).
