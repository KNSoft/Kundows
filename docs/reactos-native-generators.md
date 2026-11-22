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
