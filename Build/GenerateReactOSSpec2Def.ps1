param(
    [Parameter(Mandatory = $true)]
    [string]$Spec2DefTool,
    [Parameter(Mandatory = $true)]
    [string]$ProjectFile,
    [Parameter(Mandatory = $true)]
    [string]$SourceRoot,
    [Parameter(Mandatory = $true)]
    [string]$OutputStamp,
    [Parameter(Mandatory = $true)]
    [ValidateSet("i386", "x86_64", "arm", "arm64")]
    [string]$Architecture,
    [Parameter(Mandatory = $true)]
    [string]$Platform,
    [Parameter(Mandatory = $true)]
    [string]$Configuration,
    [string]$TargetName = "",
    [string]$TargetExtension = ".dll",
    [string]$Version = "0x502"
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path -LiteralPath $Spec2DefTool -PathType Leaf)) {
    throw "spec2def was not found: $Spec2DefTool"
}
if (-not (Test-Path -LiteralPath $ProjectFile -PathType Leaf)) {
    throw "Project file was not found: $ProjectFile"
}

$projectPath = [IO.Path]::GetFullPath($ProjectFile)
$projectDirectory = Split-Path -Parent $projectPath
$sourceRootPath = [IO.Path]::GetFullPath($SourceRoot)
$xml = [xml](Get-Content -LiteralPath $projectPath -Raw)

function Expand-ProjectPath([string]$Value) {
    $expanded = $Value
    $expanded = $expanded.Replace('$(ProjectDir)', "$projectDirectory\")
    $expanded = $expanded.Replace('$(Platform)', $Platform)
    $expanded = $expanded.Replace('$(Configuration)', $Configuration)
    $expanded = $expanded.Replace('$(TargetName)', $TargetName)
    $expanded = $expanded.Replace('$(TargetExt)', $TargetExtension)
    $expanded = $expanded.Replace('$(KundowsRoot)', "$sourceRootPath\..\..\..\")
    return [IO.Path]::GetFullPath($expanded)
}

function Get-SpecFiles {
    return @(
        $xml.Project.ItemGroup.None |
            ForEach-Object {
                $include = [string]$_.Include
                if ($include -and [IO.Path]::GetExtension($include) -ieq ".spec") {
                    $candidate = if ([IO.Path]::IsPathRooted($include)) {
                        $include
                    } else {
                        Join-Path $projectDirectory $include
                    }
                    if (Test-Path -LiteralPath $candidate -PathType Leaf) {
                        [IO.Path]::GetFullPath($candidate)
                    }
                }
            }
    )
}

function Get-DefinitionFiles {
    $configurationText = "$Configuration|$Platform"
    $values = New-Object System.Collections.Generic.List[string]
    foreach ($group in @($xml.Project.ItemDefinitionGroup)) {
        $condition = [string]$group.Condition
        if ($condition -and $condition -notmatch [regex]::Escape($configurationText)) {
            continue
        }
        foreach ($definition in @($group.Link.ModuleDefinitionFile)) {
            $value = [string]$definition
            if ($value -and -not $values.Contains($value)) {
                $values.Add($value)
            }
        }
    }
    return @($values | ForEach-Object { Expand-ProjectPath $_ } | Select-Object -Unique)
}

function Get-CMakeModuleName([string]$SpecFile) {
    $specLeaf = [IO.Path]::GetFileName($SpecFile)
    $directory = Split-Path -Parent $SpecFile
    while ($directory -and $directory.StartsWith($sourceRootPath, [StringComparison]::OrdinalIgnoreCase)) {
        $cmake = Join-Path $directory "CMakeLists.txt"
        if (Test-Path -LiteralPath $cmake -PathType Leaf) {
            $contents = Get-Content -LiteralPath $cmake -Raw
            foreach ($match in [regex]::Matches($contents, "(?im)spec2def\s*\(\s*(?<module>[^\s\)]+)\s+(?<spec>[^\s\)]+)")) {
                $specToken = $match.Groups["spec"].Value.Trim('"')
                if ($specToken -eq $specLeaf -or $specToken.EndsWith("/$specLeaf", [StringComparison]::OrdinalIgnoreCase) -or $specToken.EndsWith("\$specLeaf", [StringComparison]::OrdinalIgnoreCase)) {
                    $module = $match.Groups["module"].Value.Trim('"')
                    if ($module -match "\$\{[^}]+\}") {
                        $module = "$TargetName$TargetExtension"
                    }
                    return $module
                }
            }
        }
        $parent = Split-Path -Parent $directory
        if ($parent -eq $directory) {
            break
        }
        $directory = $parent
    }

    if ($TargetName) {
        return "$TargetName$TargetExtension"
    }
    return ([IO.Path]::GetFileNameWithoutExtension($specLeaf) + ".dll")
}

function Test-CMakeNoDebugExports([string]$SpecFile) {
    $specLeaf = [IO.Path]::GetFileName($SpecFile)
    $directory = Split-Path -Parent $SpecFile
    while ($directory -and $directory.StartsWith($sourceRootPath, [StringComparison]::OrdinalIgnoreCase)) {
        $cmake = Join-Path $directory "CMakeLists.txt"
        if (Test-Path -LiteralPath $cmake -PathType Leaf) {
            $contents = Get-Content -LiteralPath $cmake -Raw
            foreach ($match in [regex]::Matches($contents, "(?is)spec2def\s*\(\s*(?<body>.*?)\)")) {
                $body = $match.Groups["body"].Value
                if ($body -match [regex]::Escape($specLeaf) -and $body -match "(?i)\bNO_DBG\b") {
                    return $true
                }
            }
        }

        $parent = Split-Path -Parent $directory
        if ($parent -eq $directory) {
            break
        }
        $directory = $parent
    }
    return $false
}

$specFiles = @(Get-SpecFiles)
$definitionFiles = @(Get-DefinitionFiles)
if ($specFiles.Count -eq 0 -or $definitionFiles.Count -eq 0) {
    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $OutputStamp) | Out-Null
    New-Item -ItemType File -Force -Path $OutputStamp | Out-Null
    exit 0
}

$definitionByDirectory = @{}
foreach ($definition in $definitionFiles) {
    $definitionByDirectory[[IO.Path]::GetDirectoryName($definition).ToLowerInvariant()] = $definition
}

foreach ($specFile in $specFiles) {
    $specDirectory = [IO.Path]::GetDirectoryName($specFile).ToLowerInvariant()
    $definition = $definitionByDirectory[$specDirectory]
    if (-not $definition -and $definitionFiles.Count -eq 1) {
        $definition = $definitionFiles[0]
    }
    if (-not $definition) {
        continue
    }

    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $definition) | Out-Null
    $moduleName = Get-CMakeModuleName $specFile
    $debugArguments = @()
    if (-not (Test-CMakeNoDebugExports $specFile)) {
        $debugArguments += "--dbg"
    }
    & $Spec2DefTool --ms "-a=$Architecture" "-n=$moduleName" "-d=$definition" "--version=$Version" @debugArguments $specFile
    if ($LASTEXITCODE -ne 0) {
        throw "spec2def exited with code $LASTEXITCODE for '$specFile'."
    }
}

New-Item -ItemType Directory -Force -Path (Split-Path -Parent $OutputStamp) | Out-Null
New-Item -ItemType File -Force -Path $OutputStamp | Out-Null
