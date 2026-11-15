param(
    [Parameter(Mandatory = $true)]
    [string]$WindowsSdkDir,
    [Parameter(Mandatory = $true)]
    [string]$SourceFile,
    [Parameter(Mandatory = $true)]
    [string]$OutputDirectory,
    [ValidateSet("ANSI", "UNICODE")]
    [string]$Mode = "ANSI"
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path -LiteralPath $SourceFile)) {
    throw "Message source was not found: $SourceFile"
}

New-Item -ItemType Directory -Force -Path $OutputDirectory | Out-Null

$mc = Get-ChildItem -LiteralPath (Join-Path $WindowsSdkDir.TrimEnd("\", "/") "bin") -Filter "mc.exe" -File -Recurse |
    Sort-Object FullName -Descending |
    Select-Object -First 1

if ($null -eq $mc) {
    throw "Unable to locate mc.exe below $WindowsSdkDir"
}

$sourceName = [IO.Path]::GetFileNameWithoutExtension($SourceFile)
$convertedSource = Join-Path $OutputDirectory "$sourceName.mc"

# ReactOS stores .mc files as UTF-8, while the MSVC message compiler requires
# -u for UTF-16LE input without a BOM. This mirrors CMake's utf16le_convert()
# call with the nobom option.
$reader = [IO.StreamReader]::new((Resolve-Path -LiteralPath $SourceFile), $true)
try {
    $sourceText = $reader.ReadToEnd()
}
finally {
    $reader.Dispose()
}

[IO.File]::WriteAllText(
    $convertedSource,
    $sourceText,
    [Text.UnicodeEncoding]::new($false, $false))

$messageEncoding = if ($Mode -eq "UNICODE") { "-U" } else { "-A" }
& $mc.FullName -u $messageEncoding -b -h $OutputDirectory -r $OutputDirectory $convertedSource
if ($LASTEXITCODE -ne 0) {
    throw "mc.exe failed with exit code $LASTEXITCODE"
}

$header = Join-Path $OutputDirectory "$sourceName.h"
$resource = Join-Path $OutputDirectory "$sourceName.rc"
if (-not (Test-Path -LiteralPath $header) -or -not (Test-Path -LiteralPath $resource)) {
    throw "mc.exe did not generate the expected outputs: $header and $resource"
}
