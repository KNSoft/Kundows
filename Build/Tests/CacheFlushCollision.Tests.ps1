[CmdletBinding()]
param([string]$MSBuild = "MSBuild.exe")
$ErrorActionPreference = "Stop"
$MSBuild = (Get-Command $MSBuild -ErrorAction Stop).Source
$project = Join-Path $PSScriptRoot "CacheFlushCollisionTests.vcxproj"
foreach ($platform in @("Win32","x64")) {
    & $MSBuild $project /nologo /m:2 /v:minimal "/p:Platform=$platform" /p:Configuration=Release
    if ($LASTEXITCODE) { throw "Native cache flush-collision build failed: $platform" }
    $target = & $MSBuild $project /nologo /v:quiet "/p:Platform=$platform" /p:Configuration=Release -getProperty:TargetPath
    if ($LASTEXITCODE) { throw "Cannot evaluate the native flush test output" }
    & ($target -join "`n").Trim()
    if ($LASTEXITCODE) { throw "Cache flush-collision regression failed: $platform" }
}
