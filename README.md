# Kundows

Kundows builds on [ReactOS](https://reactos.org/) to match the binary interfaces and observable behavior of Windows.

## Build

On x64 Windows with Git, RosBE and Visual Studio 2026 installed, open `cmd.exe`
at the root of a checkout whose path has no spaces:

```bat
Configure.cmd
Build.cmd
MakeISO.cmd
```

The default is **x64 Debug**. Open `OutDir\amd64\Debug\REACTOS.slnx` to build in Visual Studio. The ISO is `OutDir\bootcd-x64-Debug.iso`.

[Build guide](Docs/Building.md) · [Documentation](Docs/README.md)
