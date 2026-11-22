# Native spec2def generation

The native projects preserve ReactOS's evaluated `spec2def` and
`generate_import_lib` commands. There is no build-time CMake scan, target-name
inference, module-name override list, or guessed `.dll` extension.

For example, upstream declares:

```cmake
spec2def(ntoskrnl.exe ntoskrnl.spec ADD_IMPORTLIB)
```

Its `-n=ntoskrnl.exe` argument is retained in
`Upstream/ReactOS/Projects/Ntoskrnl/Libntoskrnl.vcxproj`.
`libntoskrnl` is a build target; `ntoskrnl.exe` is the runtime provider.
The native build runs the same `spec2def`, assembler, and `link /LIB` stages.

## Build inputs

`Build/ReactOSSpec2Def.targets` consumes three explicit native item types:

- `ReactOSSpec2Def`: spec input, original arguments, output directory and files.
- `ReactOSSpec2DefStub`: generated assembly input, original assembler and arguments.
- `ReactOSImportLibrary`: generated definition and object inputs, original librarian arguments.

MSBuild `Inputs`/`Outputs` express their dependencies and incremental behavior.
There are no generation stamps or suffix-based discovery of generated files.
Configuration-specific items preserve the original architecture, `--dbg`,
version, input spec, and output paths. Thus shared specs and target aliases do
not require special handling. NTDLL, HAL and Comcat use the same targets.

The upstream tool's own behavior is unchanged, including its default DLL name
when invoked without `-n`; the recorded ReactOS recipes explicitly supply `-n`.
Errors from the tools propagate normally.

## Maintaining the migration

`*.Migration.json` contains the baseline's already evaluated reference commands.
These are migration records, not runtime build inputs.
After updating the upstream baseline and its records, import their spec2def
recipes into the native projects:

```powershell
pwsh -File Build/ImportReactOSSpec2Def.ps1
pwsh -File Build/ImportReactOSSpec2Def.ps1 -Check
pwsh -File Build/Tests/Spec2Def.Tests.ps1
```

The importer only translates paths and tool locations; it neither evaluates
CMake nor derives module names. It does not rewrite migration/build-history
records. Native builds need neither this importer nor those records.

Tests compare every migrated configuration's module name, flags, spec input and
outputs with its recorded recipe, exercise native generation on Win32, x64 and
ARM64 in Debug and Release, and check incremental and missing-output behavior.
Run them from a Visual Studio developer shell with the Win32 and x64 host
`spec2def` tools built in both configurations. `-MSBuild` accepts an explicit
MSBuild executable path; `-StaticOnly` runs just the recipe comparison.

## Validation (2026-11-22)

- 818 native projects, 4794 configuration recipes: module names, options,
  sources and outputs match the migration records; importer `-Check` is clean.
- 62 native generation cases passed across Win32/x64/ARM64 and Debug/Release.
  Up-to-date outputs were not rewritten; deleting an output regenerated it.
- Rebuilt the native x64 import libraries and relinked real consumers:
  Debug `kdgdb.dll` imports `ntoskrnl.exe`/`hal.dll`; Debug and Release
  `usbccgp.sys` import `ntoskrnl.exe`/`usbd.sys`, not `ntoskrnl.dll`.
- Exact PE symbol validation of those consumers and their native dependency
  closures passed: Debug 7 images/209 imports; Release 6 images/174 imports;
  zero missing modules or exports.
- Logs: `OutDir/ModuleBuildLogs/Spec2Def-Native-Metadata-Final-1122.log` and
  `OutDir/ModuleBuildLogs/Spec2Def-Native-Consumers-1122.json`.

These results validate generation and native linking. Existing ISO hashes and
the earlier VM boot records have not been replaced by a new ISO/boot test.
