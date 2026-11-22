# Native ReactOS generators

The Windows build uses the original ReactOS host-tool sources through ordinary
MSVC projects in `Upstream/ReactOS/Projects/HostTools` and `HostTools.slnx`.
`bin2c`, `obj2bin`, `utf16le`, and `gendib` must not be replaced with PowerShell
implementations or compiled ad hoc by their consumers.

Validation:

```powershell
Build/Tests/HostTools.Tests.ps1 -MSBuild <path-to-MSBuild.exe>
```

This builds the four tools for Win32, x64, and ARM64 in Debug and Release.
On an x64 host, executable tests cover Win32/x64 only, including all bin2c
formats, array attributes, UTF-16 BOM options, gendib outputs, and obj2bin
failure propagation. Cross-compilation is not an ARM64 execution test.

## DIB generation

Win32k declares the upstream `gendib <output-directory>` command, its three
outputs, and a native GenDib project reference. Generation depends on the tool
executable, not on a success stamp. The compiler and Windows SDK are selected
by MSBuild for the host-tool project, never rediscovered inside the generator.

`Build/Tests/GenDib.Tests.ps1` compares all six configurations with direct
upstream output and checks no-op rebuilds and missing-output regeneration.

## Raw 16-bit binaries

`ReactOSBootSector.targets` executes the recorded MSVC preprocessing, x86 ML,
and upstream obj2bin stages. Boot-sector and Frldr16 projects own their outputs;
FreeLdr consumes Frldr16 rather than generating a second copy. The DOS command
binary has its own native utility project instead of being discovered and built
inside bin2c.

Maintenance-only import and validation:

```powershell
Build/ImportReactOSGenerators.ps1 -Generator BootSector -Check
Build/Tests/BootSector.Tests.ps1 -MSBuild <path-to-MSBuild.exe>
```

The importer uses evaluated commands from the migration records, not a CMake
parser. Native builds read only committed project items. Tests exercise all
recorded configurations and compare the binary stage with direct upstream
obj2bin output, including incremental and deleted-output behavior.

## Binary-to-C generation

Bin2c executes the original tool with every recorded argument, including
`BINSTR`, array attributes, and the attribute header. PciData owns the shared
HAL tables and is referenced by its three HAL consumers. InstallFreeLdr
references the boot-sector projects; NTVDM references the DOS Command project.
No source-tree lookup or implicit assembler invocation remains in bin2c.

```powershell
Build/ImportReactOSGenerators.ps1 -Generator Bin2c -Check
Build/Tests/Bin2c.Tests.ps1 -MSBuild <path-to-MSBuild.exe>
```

## UTF-16 conversion

Theme and URLMON projects preserve each evaluated input, destination, and BOM
option. Their native targets depend on the actual input and output files;
there is no filename guessing, existing-output skip, or project success stamp.
ISO packaging also invokes the original utf16le tool rather than a second
managed-text conversion implementation.

```powershell
Build/ImportReactOSGenerators.ps1 -Generator Utf16 -Check
Build/Tests/Utf16.Tests.ps1 -MSBuild <path-to-MSBuild.exe>
```

## IID failures

WIDL is the only producer of IID definitions. Invalid IDL must fail with WIDL's
exit status; there is no UUID-regex fallback or generated-header rewriting.
`Build/Tests/UuidIids.Tests.ps1` compares valid and invalid IID/header generation
with direct WIDL across all six configurations.

## WIDL commands and dependencies

Native WIDL builds consume only explicit project items: original arguments,
working directories, declared input/output files, and the optional second
upstream header pass. They do not inspect CMake or reverse-resolve IDLs from
generated filenames. The old WIDL and UUID generation scripts are removed.
Guards, RPC interfaces, proxy symbols, and IDL contents are never rewritten.

Preserve forward slashes in WIDL arguments. Upstream `sdk/tools/tools.h` uses
`strrchr(file, '/')` in `get_basename`; replacing slashes with backslashes changes
the generated names. Correct arguments remove the need for compatibility
preambles and proxy-symbol normalization.

The old migration records truncate many multiline commands at `cd`. Therefore
WIDL's maintenance importer reads the complete original custom commands from
isolated reference VS projects, not those truncated summaries. CMake is used
only to configure/generate those reference projects, never to build ReactOS or
as part of the native build. Keep `DLL_EXPORT_VERSION=0x502`,
`ENABLE_ROSTESTS=0`, and separate Debug/Release configurations when regenerating
the current baseline. `DLL_EXPORT_VERSION=0x600` is still supported upstream,
but changes the target/export set and is not part of this migration.

`ImportReactOSWidl.ps1 -ReferenceRoot <directory> [-Check]` expects six generated
reference trees named `reference-{x86,x64,arm64}-{Debug,Release}`. It preserves
their commands and dependencies, consolidates shared SDK generation in
ReactOSPrepare, and gives shared Advapi32 RPC files an explicit utility producer.
Only actual WIDL producers reference the WIDL host project.

```powershell
Build/Tests/Widl.Tests.ps1 -MSBuild <path-to-MSBuild.exe>
```

Optionally supply `-ReferenceRoot` to verify metadata parity as well. Reference
trees and their CMake cache files are not native build inputs.

The scope remains the existing default-system migration. This work does not
enable NT6 exports, rostests, or new guest functionality, and does not replace
the existing disconnected Debug/Release boot records with a new boot claim.

## Validation (2026-11-22)

- Four upstream host tools: 24 MSVC builds and 28 executable smoke cases.
- Raw binaries: 50 configuration chains; existing 28 x64 Debug/Release binary
  hashes unchanged.
- Bin2c: 18 configuration recipes; native Win32 HAL legacy sources compile,
  and both PCI table objects have the original `INITDATA` section.
- UTF-16: 96 configuration recipes, including stale-input, encoding/BOM,
  packaging-consumer, and failure checks.
- WIDL: 3018 configuration recipes across all six configurations, 108 direct
  output comparisons, and 24 valid/invalid IID/header cases. Incremental,
  missing-output, and second-pass failure checks pass.
- WIDL import is unchanged on recheck. Spec2def, raw-binary, bin2c, and UTF-16
  imports also remain unchanged; importing one family does not reorder another.
- Rebuilt x64 Debug/Release UUID libraries and Comcat/Advapi32 DLLs. Exact PE
  validation against their audited provider closures passes: 63 images in each
  configuration, 9773 Debug / 9454 Release imports, zero errors.
- Existing spec2def tests (62 native cases), KD protocol tests (10), ISO-name
  tests (17), and packaging audits (3273 Debug / 3267 Release declarations,
  zero unresolved inputs) still pass.

Logs and isolated reference projects are under the session temporary
`native-tools-1122` directory. No new ISO or VM boot validation is claimed.

## x86 MSVC C++ stub exports

The four-ISO validation found that x86 LINK truncates decorated C++ exports
when a DEF entry aliases them directly to a C stub. Fix this in the upstream
`spec2def` producer: emit a COFF `/alternatename` directive and leave the full
decorated DEF name unaliased. The original stub calling convention and export
ordinal are retained. No generated DEF file or linked PE is rewritten.

`Spec2Def.vcxproj` applies `ReactOS-spec2def-cpp-exports.patch` before compiling
the upstream tool. The patch is scoped to this producer, not every consumer.

```powershell
Build/Tests/Spec2Def.CppExports.Tests.ps1 -MSBuild <path-to-MSBuild.exe>
```

The regression builds actual Win32 Debug/Release `msvcrt20`, `msvcrt40`, and
`vssapi` DLLs, then checks 1976 complete C++ export names and ordinals against
the original specs and generated definitions. All six tool configurations and
the existing 62 spec2def cases also pass. Unaffected generator modes remain
byte-identical; a typed-stub check preserves both cdecl and stdcall symbols.
