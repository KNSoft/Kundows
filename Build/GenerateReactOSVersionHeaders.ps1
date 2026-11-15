[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$SourceRoot,
    [Parameter(Mandatory = $true)]
    [string]$CommonIncludeDirectory,
    [Parameter(Mandatory = $true)]
    [string]$PlatformIncludeDirectory,
    [Parameter(Mandatory = $true)]
    [ValidateSet('Win32', 'x64', 'ARM64')]
    [string]$Platform,
    [Parameter(Mandatory = $true)]
    [string]$Configuration
)

$ErrorActionPreference = 'Stop'

function Read-ReactOSVersionValue {
    param([string]$Name)

    $line = Get-Content -LiteralPath (Join-Path $SourceRoot 'sdk\include\reactos\version.cmake') |
        Where-Object { $_ -match "^\s*set\($Name\s+(.+?)\)\s*$" } |
        Select-Object -First 1
    if (-not $line) {
        throw "ReactOS version variable '$Name' was not found."
    }

    $value = $Matches[1].Trim()
    if ($value.StartsWith('"') -and $value.EndsWith('"')) {
        return $value.Substring(1, $value.Length - 2)
    }
    return $value
}

function Get-GitValue {
    param([string[]]$Arguments)

    $value = & git -C $SourceRoot @Arguments 2>$null
    if ($LASTEXITCODE -ne 0 -or -not $value) {
        return 'unknown'
    }
    return ([string]$value).Trim()
}

function Write-Utf8File {
    param(
        [string]$Path,
        [string]$Content
    )

    $directory = Split-Path -Parent $Path
    [void][IO.Directory]::CreateDirectory($directory)
    [IO.File]::WriteAllText($Path, $Content, [Text.UTF8Encoding]::new($false))
}

$major = Read-ReactOSVersionValue 'KERNEL_VERSION_MAJOR'
$minor = Read-ReactOSVersionValue 'KERNEL_VERSION_MINOR'
$patch = Read-ReactOSVersionValue 'KERNEL_VERSION_PATCH_LEVEL'
$copyrightYear = Read-ReactOSVersionValue 'COPYRIGHT_YEAR'
$buildType = Read-ReactOSVersionValue 'KERNEL_VERSION_BUILD_TYPE'
$winArch = switch ($Platform) {
    'Win32' { 'x86' }
    'x64' { 'x64' }
    'ARM64' { 'ARM64' }
}

$build = [DateTime]::UtcNow.ToString('yyyyMMdd')
$commit = Get-GitValue @('rev-parse', 'HEAD')
$revision = Get-GitValue @('describe', '--abbrev=7', '--long', '--always')
$version = "$major.$minor.$patch-$winArch"
if ($buildType) {
    $version = "$version-$buildType"
}
$dllMajor = [int]$major + 42
$dllVersion = "$dllMajor.$minor.$patch"
if ($buildType) {
    $dllVersion = "$dllVersion-$buildType"
}

$compilerName = 'MSVC'
$compilerVersion = 'unknown'
$compiler = Get-Command cl.exe -ErrorAction SilentlyContinue
if ($compiler) {
    $compilerVersion = [Diagnostics.FileVersionInfo]::GetVersionInfo($compiler.Source).FileVersion
}

$versionTemplate = Get-Content -LiteralPath (Join-Path $SourceRoot 'sdk\include\reactos\version.h.cmake') -Raw
$versionHeader = $versionTemplate.
    Replace('@KERNEL_VERSION_MAJOR@', $major).
    Replace('@KERNEL_VERSION_MINOR@', $minor).
    Replace('@KERNEL_VERSION_PATCH_LEVEL@', $patch).
    Replace('@COPYRIGHT_YEAR@', $copyrightYear)

$buildTemplate = Get-Content -LiteralPath (Join-Path $SourceRoot 'sdk\include\reactos\buildno.h.cmake') -Raw
$buildHeader = $buildTemplate.
    Replace('@KERNEL_VERSION_BUILD@', $build).
    Replace('@REVISION@', $revision).
    Replace('@KERNEL_VERSION@', $version).
    Replace('@COMMIT_HASH@', $commit).
    Replace('@REACTOS_DLL_VERSION_MAJOR@', [string]$dllMajor).
    Replace('@DLL_VERSION_STR@', $dllVersion).
    Replace('@CMAKE_C_COMPILER_ID@', $compilerName).
    Replace('@CMAKE_C_COMPILER_VERSION@', $compilerVersion)

$commonReactOS = Join-Path $CommonIncludeDirectory 'reactos'
$platformReactOS = Join-Path $PlatformIncludeDirectory 'reactos'
Write-Utf8File (Join-Path $commonReactOS 'version.h') $versionHeader
Write-Utf8File (Join-Path $platformReactOS 'buildno.h') $buildHeader

Write-Host "Generated ReactOS version headers for $Platform|$Configuration"
