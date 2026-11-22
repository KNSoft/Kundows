[CmdletBinding()]
param([string]$MSBuild = "MSBuild.exe")
$ErrorActionPreference = "Stop"
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot "..\.."))
$temporary = Join-Path $env:TEMP ("kundows-iids-" + [guid]::NewGuid())
$null = New-Item -ItemType Directory -Path $temporary
$cases = 0
try {
    foreach ($platform in @("Win32", "x64", "ARM64")) {
        foreach ($configuration in @("Debug", "Release")) {
            $hostPlatform = if ($platform -eq "ARM64") { "x64" } else { $platform }
            $tool = "$root\OutDir\HostTools\$hostPlatform\$configuration\widl.exe"
            $architecture = switch ($platform) {
                "Win32" { @("-m32", "--win32", "-b", "i386-x-y") }
                "x64" { @("-m64", "--win64", "-b", "amd64-x-y") }
                "ARM64" { @("-b", "arm64-x-y") }
            }
            foreach ($mode in @("Iids", "Header")) {
                foreach ($valid in @($true, $false)) {
                    $text = '[uuid(12345678-1234-5678-90ab-1234567890ab)] interface IAuditNative {};'
                    if (-not $valid) { $text += "`nthis is not valid IDL;" }
                    [IO.File]::WriteAllText("$temporary\input.idl", $text)
                    Remove-Item "$temporary\direct", "$temporary\wrapped" -Force -ErrorAction SilentlyContinue
                    $options = if ($mode -eq "Header") { @("-Oicf", "-h") } else { @("-Os", "-u") }
                    $arguments = @("-nostdinc", "-D__REACTOS__", "-D_MIDL_USE_GUIDDEF_") + $architecture + $options
                    Push-Location $temporary
                    try {
                        & $tool @arguments -o "$temporary/output" "$temporary/input.idl" > "$temporary\direct.log" 2>&1
                        $directCode = $LASTEXITCODE
                        $reference = if (Test-Path "$temporary\output") { [IO.File]::ReadAllBytes("$temporary\output") }
                        Remove-Item "$temporary\output" -Force -ErrorAction SilentlyContinue
                        & $MSBuild "$PSScriptRoot\Widl.Tests.proj" /nologo /v:quiet "/p:FixtureRoot=$temporary" "/p:Platform=$platform" "/p:Configuration=$configuration" "/p:ArchitectureFlags=$($architecture -join ' ')" "/p:ModeFlags=$($options -join ' ')" > "$temporary\wrapped.log" 2>&1
                        $wrapperCode = $LASTEXITCODE
                    }
                    finally { Pop-Location }
                    if ($wrapperCode -ne $directCode -or (($directCode -eq 0) -ne $valid)) { throw "IID/header failure propagation differs from upstream: $platform $configuration $mode valid=$valid" }
                    if ($valid -and [Convert]::ToHexString($reference) -cne [Convert]::ToHexString([IO.File]::ReadAllBytes("$temporary\output"))) { throw "IID/header output was rewritten" }
                    if (-not $valid -and (Test-Path "$temporary\output")) { throw "Invalid IDL manufactured an IID/header output" }
                    $cases++
                }
            }
        }
    }
}
finally { Remove-Item -LiteralPath $temporary -Recurse -Force }
"IID/header generation: $cases success/failure cases match native WIDL; no regex fallback or output rewriting"
exit 0
