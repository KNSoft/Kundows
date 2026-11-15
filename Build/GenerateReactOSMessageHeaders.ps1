param(
    [Parameter(Mandatory = $true)]
    [string]$WindowsSdkDir,
    [Parameter(Mandatory = $true)]
    [string]$SourceFile,
    [Parameter(Mandatory = $true)]
    [string]$OutputDirectory
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path -LiteralPath $SourceFile)) {
    throw "Message source was not found: $SourceFile"
}

New-Item -ItemType Directory -Force -Path $OutputDirectory | Out-Null

$mc = Get-ChildItem -LiteralPath (Join-Path $WindowsSdkDir "bin") -Filter "mc.exe" -File -Recurse |
    Sort-Object FullName -Descending |
    Select-Object -First 1

if ($null -eq $mc) {
    throw "Unable to locate mc.exe below $WindowsSdkDir"
}

& $mc.FullName -A -b -h $OutputDirectory -r $OutputDirectory $SourceFile
if ($LASTEXITCODE -ne 0) {
    throw "mc.exe failed with exit code $LASTEXITCODE"
}
