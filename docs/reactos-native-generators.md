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
