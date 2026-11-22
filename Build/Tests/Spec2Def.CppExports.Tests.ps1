[CmdletBinding()]
param([string]$MSBuild = "MSBuild.exe")
$ErrorActionPreference = "Stop"
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot "..\.."))
$MSBuild = (Get-Command $MSBuild -ErrorAction Stop).Source
$checked = 0
foreach ($configuration in @("Debug", "Release")) {
    foreach ($module in @("msvcrt20", "msvcrt40", "vssapi")) {
        $projectName = switch ($module) {
            "msvcrt20" { "Msvcrt20" }
            "msvcrt40" { "Msvcrt40" }
            "vssapi" { "Vssapi" }
        }
        $project = Join-Path $root "Upstream\ReactOS\Projects\Dll\Win32\$projectName\$projectName.vcxproj"
        & $MSBuild $project /nologo /m:4 /verbosity:minimal /p:Platform=Win32 "/p:Configuration=$configuration" /p:CL_MPCount=2
        if ($LASTEXITCODE) { throw "Native $module $configuration build failed" }

        # Ask the evaluated native project for its tool locations; do not
        # discover compilers/SDKs or guess a toolchain installation.
        $properties = & $MSBuild $project /nologo /verbosity:quiet /p:Platform=Win32 "/p:Configuration=$configuration" `
            -getProperty:VCToolsInstallDir,PreferredToolArchitecture,PlatformShortName,TargetPath
        if ($LASTEXITCODE) { throw "Native tool-property evaluation failed" }
        $properties = ($properties -join "`n" | ConvertFrom-Json).Properties
        $dumpbin = Join-Path $properties.VCToolsInstallDir "bin\Host$($properties.PreferredToolArchitecture)\$($properties.PlatformShortName)\dumpbin.exe"
        $exports = & $dumpbin /nologo /exports $properties.TargetPath
        if ($LASTEXITCODE) { throw "Cannot inspect the actual linked $module image" }
        $actual = [Collections.Generic.Dictionary[string,int]]::new([StringComparer]::Ordinal)
        foreach ($line in $exports) {
            if ($line -match '^\s*(\d+)\s+[0-9A-F]+\s+[0-9A-F]+\s+(\S+)') {
                if ($actual.ContainsKey($Matches[2])) { throw "Duplicate PE export: $($Matches[2])" }
                $actual.Add($Matches[2], [int]$Matches[1])
            }
        }

        $spec = [IO.File]::ReadAllText((Join-Path $root "Upstream\ReactOS\Source\dll\win32\$module\$module.spec"))
        $definition = [IO.File]::ReadAllText((Join-Path $root "OutDir\Build\Generated\Win32\$configuration\dll\win32\$module\$module.def"))
        $sourceNames = [Collections.Generic.HashSet[string]]::new([StringComparer]::Ordinal)
        foreach ($match in [regex]::Matches($spec, '(?m)^\s*(?:@|\d+)\s+[a-z]+\s+(?<flags>(?:-\S+\s+)*)(?<name>\?[^\s(]+)')) {
            # The fixtures also declare Win64-specific C++ spellings.
            if ($match.Groups["flags"].Value -match '(?:^|\s)-arch=(?:win64|amd64)(?:\s|$)') { continue }
            $null = $sourceNames.Add($match.Groups["name"].Value)
        }
        $names = [Collections.Generic.HashSet[string]]::new([StringComparer]::Ordinal)
        foreach ($match in [regex]::Matches($definition, '(?m)^\s+(\?[^\s=]+)(?:=[^\s]+)?\s+@(\d+)\b')) {
            $name = $match.Groups[1].Value
            $ordinal = [int]$match.Groups[2].Value
            if (-not $actual.ContainsKey($name) -or $actual[$name] -ne $ordinal) {
                throw "Missing, truncated, or wrongly numbered C++ export: $module $configuration $name @$ordinal"
            }
            $null = $names.Add($name)
            $checked++
        }
        if (-not $names.Count -or -not $names.SetEquals($sourceNames)) {
            throw "Generated C++ export names differ from the original $module spec"
        }
        Write-Output "$module ${configuration}: $($names.Count) exact C++ names/ordinals passed"
    }
}
Write-Output "spec2def C++ exports: six native DLL builds, $checked names/ordinals passed"
