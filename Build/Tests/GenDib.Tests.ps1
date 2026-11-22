[CmdletBinding()]
param([string]$MSBuild = "MSBuild.exe")
$ErrorActionPreference = "Stop"
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot "..\.."))
$project = Join-Path $root "Upstream\ReactOS\Projects\Win32ss\Win32k.vcxproj"
$temporary = Join-Path $env:TEMP ("kundows-gendib-" + [guid]::NewGuid())
$null = New-Item -ItemType Directory -Path $temporary
try {
    foreach ($platform in @("Win32", "x64", "ARM64")) {
        foreach ($configuration in @("Debug", "Release")) {
            $options = @("/nologo", "/v:minimal", "/t:GenerateReactOSGenDib", "/p:BuildProjectReferences=false", "/p:Platform=$platform", "/p:Configuration=$configuration")
            & $MSBuild $project @options
            if ($LASTEXITCODE) { throw "Native gendib $platform $configuration failed" }
            $hostPlatform = if ($platform -eq "ARM64") { "x64" } else { $platform }
            & "$root\OutDir\HostTools\$hostPlatform\$configuration\gendib.exe" $temporary
            if ($LASTEXITCODE) { throw "Reference gendib failed" }
            $directory = "$root\OutDir\Build\Generated\$platform\$configuration\win32ss\gdi\dib"
            $names = @("dib8gen.c", "dib16gen.c", "dib32gen.c")
            foreach ($name in $names) {
                if ((Get-FileHash "$directory\$name").Hash -ne (Get-FileHash "$temporary\$name").Hash) { throw "gendib output differs: $name" }
            }
            $before = @($names | ForEach-Object { (Get-Item "$directory\$_").LastWriteTimeUtc.Ticks })
            & $MSBuild $project @options
            if ($LASTEXITCODE) { throw "Incremental gendib failed" }
            $after = @($names | ForEach-Object { (Get-Item "$directory\$_").LastWriteTimeUtc.Ticks })
            if (($before -join "|") -cne ($after -join "|")) { throw "gendib rewrote unchanged outputs" }
            Remove-Item -LiteralPath "$directory\dib16gen.c"
            & $MSBuild $project @options
            if ($LASTEXITCODE -or -not (Test-Path "$directory\dib16gen.c")) { throw "gendib did not restore a missing output" }
        }
    }
}
finally { Remove-Item -LiteralPath $temporary -Recurse -Force }
"gendib: six configurations match upstream; incremental and missing-output tests passed"
