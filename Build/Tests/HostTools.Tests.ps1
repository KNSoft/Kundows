[CmdletBinding()]
param(
    [string]$MSBuild = "MSBuild.exe"
)
$ErrorActionPreference = "Stop"
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot "..\.."))
$temporary = Join-Path $env:TEMP ("kundows-host-tools-" + [guid]::NewGuid())
$null = New-Item -ItemType Directory -Path $temporary
$cases = 0
try {
    [IO.File]::WriteAllBytes((Join-Path $temporary "input.bin"), [byte[]](1, 127, 255))
    [IO.File]::WriteAllText((Join-Path $temporary "input.txt"), "ReactOS", [Text.UTF8Encoding]::new($false))
    foreach ($platform in @("Win32", "x64", "ARM64")) {
        foreach ($configuration in @("Debug", "Release")) {
            foreach ($name in @("Bin2c", "Obj2bin", "Utf16le", "GenDib")) {
                & $MSBuild (Join-Path $root "Upstream\ReactOS\Projects\HostTools\$name\$name.vcxproj") /nologo /v:minimal "/p:Platform=$platform" "/p:Configuration=$configuration"
                if ($LASTEXITCODE) { throw "$name $platform $configuration did not build" }
            }
            # Cross-compiled ARM64 tools cannot run on the x64 build host.
            if ($platform -eq "ARM64") { continue }
            $bin = Join-Path $root "OutDir\HostTools\$platform\$configuration"
            Push-Location $temporary
            try {
                foreach ($format in @("BIN", "BINSTR", "STR")) {
                    & "$bin\bin2c.exe" input.bin output.c output.h $format AuditData 'DATA_SEG("INITDATA")' attribute.h
                    if ($LASTEXITCODE) { throw "bin2c $format failed" }
                    $length = if ($format -eq "BIN") { 3 } else { 4 }
                    if ((Get-Content output.h -Raw) -notmatch "#define AuditData_SIZE $length\b" -or
                        (Get-Content output.c -Raw) -notmatch 'DATA_SEG\("INITDATA"\) unsigned char AuditData' -or
                        (Get-Content output.c -Raw) -notmatch '#include "attribute.h"') {
                        throw "bin2c lost its format or attribute arguments"
                    }
                    $cases++
                }
                foreach ($bom in @("bom", "nobom")) {
                    $options = @(if ($bom -eq "nobom") { "nobom" })
                    & "$bin\utf16le.exe" input.txt output.txt @options
                    if ($LASTEXITCODE) { throw "utf16le $bom failed" }
                    $expected = [Text.Encoding]::Unicode.GetBytes("ReactOS")
                    if ($bom -eq "bom") { $expected = [byte[]](255, 254) + $expected }
                    if ([Convert]::ToHexString([IO.File]::ReadAllBytes("$temporary\output.txt")) -cne [Convert]::ToHexString($expected)) {
                        throw "utf16le $bom output differs"
                    }
                    $cases++
                }
                $null = New-Item -ItemType Directory -Force -Path dib
                & "$bin\gendib.exe" "$temporary\dib"
                if ($LASTEXITCODE) { throw "gendib failed" }
                foreach ($name in @("dib8gen.c", "dib16gen.c", "dib32gen.c")) {
                    if ((Get-Item "dib\$name").Length -eq 0) { throw "gendib output missing: $name" }
                }
                $cases++
                & "$bin\obj2bin.exe" missing.obj output.bin 7c00 2>$null
                if ($LASTEXITCODE -eq 0 -or (Test-Path output.bin)) { throw "obj2bin concealed a missing input" }
                $cases++
            }
            finally { Pop-Location }
        }
    }
}
finally { Remove-Item -LiteralPath $temporary -Recurse -Force }
"host tools: 24 MSVC builds; $cases executable smoke tests passed"
