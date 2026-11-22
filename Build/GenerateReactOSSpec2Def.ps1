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
    [string]$Version = "0x502",
    [switch]$ResolveModuleName,
    [string]$ModuleNameOutput = "",
    [string]$ModuleNameOverride = ""
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

function Get-CMakeProjectFiles {
    $files = [Collections.Generic.List[string]]::new()
    foreach ($item in @($xml.Project.ItemGroup.None)) {
        $include = [string]$item.Include
        if (-not $include) {
            continue
        }

        $isCMake = [IO.Path]::GetFileName($include) -ieq "CMakeLists.txt" -or
            [IO.Path]::GetExtension($include) -ieq ".cmake"
        if (-not $isCMake) {
            continue
        }

        $candidate = if ([IO.Path]::IsPathRooted($include)) {
            $include
        } else {
            Join-Path $projectDirectory $include
        }
        if (Test-Path -LiteralPath $candidate -PathType Leaf) {
            $fullPath = [IO.Path]::GetFullPath($candidate)
            if (-not $files.Contains($fullPath)) {
                [void]$files.Add($fullPath)
            }
        }
    }
    return @($files)
}

function Get-CMakeModuleName([string]$SpecFile) {
    if ($ModuleNameOverride) {
        return $ModuleNameOverride
    }

    $specLeaf = [IO.Path]::GetFileName($SpecFile)
    $targetStem = [IO.Path]::GetFileNameWithoutExtension("$TargetName$TargetExtension")
    $runtimeTarget = $targetStem -replace "(?i)^lib", ""
    $runtimeTarget = $runtimeTarget -replace "(?i)_pe$", ""
    if (-not $runtimeTarget) {
        $runtimeTarget = [IO.Path]::GetFileNameWithoutExtension($specLeaf)
    }

    $targetIdentityAliases = @(
        $targetStem
        $runtimeTarget
        $targetStem -replace "(?i)_pe$", ""
        $targetStem -replace "(?i)^lib", ""
        ($targetStem -replace "(?i)^lib", "") -replace "(?i)_pe$", ""
    ) | Where-Object { $_ } | Select-Object -Unique

    $targetAliases = @(
        $targetStem
        $runtimeTarget
        $targetStem -replace "(?i)_pe$", ""
        $targetStem -replace "(?i)^lib", ""
        ($targetStem -replace "(?i)^lib", "") -replace "(?i)_pe$", ""
        [IO.Path]::GetFileNameWithoutExtension($specLeaf)
    ) | Where-Object { $_ } | Select-Object -Unique

    function Expand-CMakeToken([string]$Token) {
        $normalized = $Token.Trim('"').Replace("\", "/")
        return [regex]::Replace($normalized, '\$\{[^}]+\}', $runtimeTarget)
    }

    function Test-CMakeSpecToken([string]$Token) {
        $resolved = Expand-CMakeToken $Token
        $resolvedLeaf = [IO.Path]::GetFileName($resolved)
        return $resolvedLeaf.Equals($specLeaf, [StringComparison]::OrdinalIgnoreCase) -or
            $resolved.EndsWith("/$specLeaf", [StringComparison]::OrdinalIgnoreCase)
    }

    function Search-CMakeFiles([System.Collections.IEnumerable]$CMakeFiles) {
        foreach ($cmake in $CMakeFiles) {
            $contents = Get-Content -LiteralPath $cmake -Raw
            foreach ($match in [regex]::Matches($contents, "(?im)^(?!\s*#)\s*spec2def\s*\(\s*(?<module>[^\s\)]+)\s+(?<spec>[^\s\)]+)")) {
                $specToken = $match.Groups["spec"].Value
                if (-not (Test-CMakeSpecToken $specToken)) {
                    continue
                }

                $module = Expand-CMakeToken $match.Groups["module"].Value
                if (-not $candidates.Contains($module)) {
                    [void]$candidates.Add($module)
                }
            }

            foreach ($match in [regex]::Matches($contents, "(?im)^(?!\s*#)\s*generate_import_lib\s*\(\s*(?<target>[^\s\)]+)\s+(?<module>[^\s\)]+)\s+(?<spec>[^\s\)]+)")) {
                $targetToken = Expand-CMakeToken $match.Groups["target"].Value
                $targetStem = [IO.Path]::GetFileNameWithoutExtension($targetToken)
                if ($targetIdentityAliases -notcontains $targetStem) {
                    continue
                }

                $specToken = $match.Groups["spec"].Value
                if (-not (Test-CMakeSpecToken $specToken)) {
                    continue
                }

                # ReactOS's MSVC generate_import_lib wrapper passes its
                # second argument to spec2def as -n=<runtime-module>.
                $module = Expand-CMakeToken $match.Groups["module"].Value
                if (-not $targetSpecificCandidates.Contains($module)) {
                    [void]$targetSpecificCandidates.Add($module)
                }
            }
        }
    }

    $searchFiles = [Collections.Generic.List[string]]::new()
    $pendingFiles = [Collections.Generic.Queue[string]]::new()
    $visitedFiles = [Collections.Generic.HashSet[string]]::new([StringComparer]::OrdinalIgnoreCase)
    $projectCMakeFiles = @(Get-CMakeProjectFiles)
    foreach ($cmake in $projectCMakeFiles) {
        $pendingFiles.Enqueue($cmake)
    }

    while ($pendingFiles.Count -gt 0) {
        $cmake = $pendingFiles.Dequeue()
        $cmake = [IO.Path]::GetFullPath($cmake)
        if (-not $visitedFiles.Add($cmake)) {
            continue
        }
        [void]$searchFiles.Add($cmake)

        $contents = Get-Content -LiteralPath $cmake -Raw
        $cmakeDirectory = Split-Path -Parent $cmake
        foreach ($match in [regex]::Matches($contents, "(?im)^(?!\s*#)\s*include\s*\(\s*(?<path>[^\s\)]+)")) {
            $include = $match.Groups["path"].Value.Trim('"')
            if (-not $include) {
                continue
            }
            $include = $include.Replace('${CMAKE_CURRENT_SOURCE_DIR}', $cmakeDirectory)
            $include = $include.Replace('${CMAKE_CURRENT_LIST_DIR}', $cmakeDirectory)
            if ($include -match "\$\{|\$\(") {
                continue
            }
            $includePath = if ([IO.Path]::IsPathRooted($include)) {
                [IO.Path]::GetFullPath($include)
            } else {
                [IO.Path]::GetFullPath((Join-Path $cmakeDirectory $include))
            }
            foreach ($candidate in @(
                $includePath
                "$includePath.cmake"
                (Join-Path $includePath "CMakeLists.txt")
            )) {
                if (Test-Path -LiteralPath $candidate -PathType Leaf) {
                    $pendingFiles.Enqueue([IO.Path]::GetFullPath($candidate))
                    break
                }
            }
        }
        foreach ($match in [regex]::Matches($contents, "(?im)^(?!\s*#)\s*add_subdirectory\s*\(\s*(?<path>[^\s\)]+)")) {
            $subdirectory = $match.Groups["path"].Value.Trim('"')
            if (-not $subdirectory -or $subdirectory -match "\$\{|\$\(") {
                continue
            }
            $subdirectoryCMake = Join-Path (Join-Path $cmakeDirectory $subdirectory) "CMakeLists.txt"
            if (Test-Path -LiteralPath $subdirectoryCMake -PathType Leaf) {
                $pendingFiles.Enqueue([IO.Path]::GetFullPath($subdirectoryCMake))
            }
        }
    }

    $candidates = [Collections.Generic.List[string]]::new()
    $targetSpecificCandidates = [Collections.Generic.List[string]]::new()
    Search-CMakeFiles $searchFiles
    $allCandidates = @($candidates + $targetSpecificCandidates | Select-Object -Unique)

    if ($candidates.Count -gt 0 -or $targetSpecificCandidates.Count -gt 0) {
        $identityCandidates = @(
            $candidates | Where-Object {
                $candidateStem = [IO.Path]::GetFileNameWithoutExtension($_)
                $targetIdentityAliases -contains $candidateStem
            }
        )
        $identityCandidates = @($identityCandidates + $targetSpecificCandidates | Select-Object -Unique)
        if ($identityCandidates.Count -eq 1) {
            return $identityCandidates[0]
        }
        if ($identityCandidates.Count -gt 1) {
            throw "CMake module resolution is ambiguous for '$SpecFile' and target '$TargetName$TargetExtension': $($identityCandidates -join ', ')"
        }

        $fallbackCandidates = @(
            $candidates | Where-Object {
                $candidateStem = [IO.Path]::GetFileNameWithoutExtension($_)
                $targetAliases -contains $candidateStem
            }
        )
        if ($fallbackCandidates.Count -eq 1) {
            return $fallbackCandidates[0]
        }
        if ($fallbackCandidates.Count -gt 1) {
            throw "CMake module resolution is ambiguous for '$SpecFile' and target '$TargetName$TargetExtension': $($fallbackCandidates -join ', ')"
        }

        throw "CMake module declarations for '$SpecFile' do not match target '$TargetName$TargetExtension': $($allCandidates -join ', ')"
    }

    throw "No CMake module declaration (spec2def or generate_import_lib) matched '$SpecFile' for target '$TargetName$TargetExtension'. Set ReactOSSpec2DefModuleName only when the upstream target has no declaration."
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
if ($ResolveModuleName) {
    if (-not $ModuleNameOutput) {
        throw "ModuleNameOutput is required when ResolveModuleName is specified."
    }

    $moduleName = if ($specFiles.Count -gt 0) {
        Get-CMakeModuleName $specFiles[0]
    } elseif ($TargetName) {
        "$TargetName$TargetExtension"
    } else {
        ""
    }

    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $ModuleNameOutput) | Out-Null
    [IO.File]::WriteAllText(
        [IO.Path]::GetFullPath($ModuleNameOutput),
        "$moduleName`r`n",
        [Text.UTF8Encoding]::new($false))
    exit 0
}

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
