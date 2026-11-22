[CmdletBinding()]
param(
    [ValidateSet("Audit", "Build")]
    [string]$Mode = "Audit",
    [ValidateSet("Win32", "x64", "ARM64")]
    [string]$Platform = "x64",
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Release",
    [string]$SourceRoot = "",
    [string]$KundowsRoot = "",
    [string]$OutputDirectory = ""
)

$ErrorActionPreference = "Stop"

if (-not $KundowsRoot) {
    $KundowsRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot ".."))
}
if (-not $SourceRoot) {
    $SourceRoot = Join-Path $KundowsRoot "Upstream\ReactOS\Source"
}

$KundowsRoot = [IO.Path]::GetFullPath($KundowsRoot)
$SourceRoot = [IO.Path]::GetFullPath($SourceRoot)
$generatedRoot = Join-Path $KundowsRoot "OutDir\Build\Generated\$Platform\$Configuration"
$imageRoot = if ($OutputDirectory) {
    [IO.Path]::GetFullPath($OutputDirectory)
} else {
    Join-Path $KundowsRoot "OutDir\Images\$Platform\$Configuration"
}
$auditRoot = Join-Path $generatedRoot "ISO"

$architecture = switch ($Platform) {
    "Win32" { "i386" }
    "x64" { "amd64" }
    "ARM64" { "arm64" }
}
$winArchitecture = switch ($Platform) {
    "Win32" { "x86" }
    "x64" { "amd64" }
    "ARM64" { "arm64" }
}

# These directories are not part of ReactOS' default system configuration:
# the parent CMake files either comment them out or guard them with
# DLL_EXPORT_VERSION >= 0x600/0x603 (the default is 0x502).
$defaultDisabledCmakeDirectories = @(
    "base/applications/msconfig_new",
    "dll/cpl/liccpa",
    "dll/directx/ksproxy",
    "dll/directx/wine/d2d1",
    "dll/directx/wine/d3d10",
    "dll/directx/wine/d3d10_1",
    "dll/directx/wine/d3d10core",
    "dll/directx/wine/d3d11",
    "dll/directx/wine/d3d12",
    "dll/directx/wine/d3d12core",
    "dll/directx/wine/d3dx10_33",
    "dll/directx/wine/d3dx10_34",
    "dll/directx/wine/d3dx10_35",
    "dll/directx/wine/d3dx10_36",
    "dll/directx/wine/d3dx10_37",
    "dll/directx/wine/d3dx10_38",
    "dll/directx/wine/d3dx10_39",
    "dll/directx/wine/d3dx10_40",
    "dll/directx/wine/d3dx10_41",
    "dll/directx/wine/d3dx10_42",
    "dll/directx/wine/d3dx10_43",
    "dll/directx/wine/d3dx11_42",
    "dll/directx/wine/d3dx11_43",
    "dll/directx/wine/dplay",
    "dll/directx/wine/dpnhpast",
    "dll/directx/wine/dwrite",
    "dll/directx/wine/dxcore",
    "dll/directx/wine/dxgi",
    "dll/directx/wine/dxva2",
    "dll/nls/idndl",
    "dll/win32/avrt",
    "dll/win32/bcp47langs",
    "dll/win32/bcryptprimitives",
    "dll/win32/dcomp",
    "dll/win32/dwmapi",
    "dll/win32/mmdevapi",
    "dll/win32/msnet32",
    "dll/win32/ncrypt",
    "dll/win32/rtworkq",
    "dll/win32/tbs",
    "dll/win32/wevtapi",
    "drivers/storage/class/sfloppy",
    "drivers/usb/usbaudio"
)

function Assert-File([string]$Path, [string]$Description) {
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) {
        throw "$Description was not found: $Path"
    }
}

function Get-CMakeCalls {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Text,
        [Parameter(Mandatory = $true)]
        [string]$CommandName
    )

    $pattern = "(?i)\b" + [regex]::Escape($CommandName) + "\s*\("
    foreach ($match in [regex]::Matches($Text, $pattern)) {
        $lineStart = $Text.LastIndexOf("`n", [Math]::Max(0, $match.Index - 1)) + 1
        $linePrefix = $Text.Substring($lineStart, $match.Index - $lineStart)
        if ($linePrefix.TrimStart().StartsWith("#")) {
            continue
        }
        $prefix = $Text.Substring(0, $match.Index)
        $definitionDepth = 0
        foreach ($definition in [regex]::Matches(
            $prefix,
            "(?im)^\s*(?<command>function|macro|endfunction|endmacro)\s*\("
        )) {
            if ($definition.Groups["command"].Value -ieq "function" -or
                $definition.Groups["command"].Value -ieq "macro") {
                $definitionDepth++
            } elseif ($definitionDepth -gt 0) {
                $definitionDepth--
            }
        }
        if ($definitionDepth -gt 0) {
            continue
        }
        $open = $Text.IndexOf("(", $match.Index)
        $depth = 0
        $quoted = $false
        $escaped = $false
        $comment = $false
        $end = -1

        for ($index = $open; $index -lt $Text.Length; $index++) {
            $character = $Text[$index]
            $next = if ($index + 1 -lt $Text.Length) { $Text[$index + 1] } else { [char]0 }

            if ($comment) {
                if ($character -eq "`n") {
                    $comment = $false
                }
                continue
            }
            if (-not $quoted -and $character -eq "#" ) {
                $comment = $true
                continue
            }
            if ($escaped) {
                $escaped = $false
                continue
            }
            if ($character -eq "\" -and $quoted) {
                $escaped = $true
                continue
            }
            if ($character -eq '"') {
                $quoted = -not $quoted
                continue
            }
            if ($quoted) {
                continue
            }
            if ($character -eq "(") {
                $depth++
            } elseif ($character -eq ")") {
                $depth--
                if ($depth -eq 0) {
                    $end = $index
                    break
                }
            }
        }

        if ($end -lt 0) {
            throw "Unterminated $CommandName call near offset $($match.Index)."
        }

        [pscustomobject]@{
            Text = $Text.Substring($match.Index, $end - $match.Index + 1)
            Body = $Text.Substring($open + 1, $end - $open - 1)
            Offset = $match.Index
        }
    }
}

function Get-CMakeFunctionDefinitions {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Text
    )

    $pattern = "(?ims)(?:^|`n)\s*(?:function|macro)\s*\(\s*(?<name>[A-Za-z_][A-Za-z0-9_]*)\s*(?<parameters>[^)]*)\)\s*(?<body>.*?)^\s*end(?:function|macro)\s*\("
    foreach ($match in [regex]::Matches($Text, $pattern)) {
        $lineStart = $Text.LastIndexOf("`n", [Math]::Max(0, $match.Index - 1)) + 1
        $linePrefix = $Text.Substring($lineStart, $match.Index - $lineStart)
        if ($linePrefix.TrimStart().StartsWith("#")) {
            continue
        }

        [pscustomobject]@{
            Name = $match.Groups["name"].Value
            Parameters = $match.Groups["parameters"].Value
            Body = $match.Groups["body"].Value
        }
    }
}

function Split-CMakeArguments([string]$Body) {
    $arguments = [Collections.Generic.List[string]]::new()
    $current = [Text.StringBuilder]::new()
    $quoted = $false
    $escaped = $false
    $comment = $false

    foreach ($character in $Body.ToCharArray()) {
        if ($comment) {
            if ($character -eq "`n") {
                $comment = $false
            }
            continue
        }
        if (-not $quoted -and $character -eq "#") {
            $comment = $true
            continue
        }
        if ($escaped) {
            [void]$current.Append($character)
            $escaped = $false
            continue
        }
        if ($character -eq "\" -and $quoted) {
            $escaped = $true
            continue
        }
        if ($character -eq '"') {
            $quoted = -not $quoted
            continue
        }
        if (-not $quoted -and [char]::IsWhiteSpace($character)) {
            if ($current.Length -gt 0) {
                $arguments.Add($current.ToString())
                [void]$current.Clear()
            }
            continue
        }
        [void]$current.Append($character)
    }
    if ($current.Length -gt 0) {
        $arguments.Add($current.ToString())
    }
    return @($arguments)
}

function Expand-CMakeValue([string]$Value, [hashtable]$Variables) {
    $expanded = $Value
    for ($pass = 0; $pass -lt 12; $pass++) {
        $next = [regex]::Replace($expanded, "\$\{(?<name>[A-Za-z_][A-Za-z0-9_]*)\}", {
            param($match)
            $name = $match.Groups["name"].Value
            if ($Variables.ContainsKey($name)) {
                return [string]$Variables[$name]
            }
            return $match.Value
        })
        if ($next -eq $expanded) {
            break
        }
        $expanded = $next
    }
    return $expanded
}

function Expand-CMakeValueList {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Value,
        [Parameter(Mandatory = $true)]
        [hashtable]$Variables,
        [int]$Depth = 0
    )

    if ($Depth -gt 16) {
        return @($Value)
    }
    $match = [regex]::Match($Value, "\$\{(?<name>[A-Za-z_][A-Za-z0-9_]*)\}")
    if (-not $match.Success) {
        return @(Expand-CMakeValue $Value $Variables)
    }

    $name = $match.Groups["name"].Value
    if (-not $Variables.ContainsKey($name)) {
        return @($Value)
    }

    $prefix = $Value.Substring(0, $match.Index)
    $suffix = $Value.Substring($match.Index + $match.Length)
    $expanded = [Collections.Generic.List[string]]::new()
    foreach ($item in @($Variables[$name])) {
        $candidateValue = $prefix + [string]$item + $suffix
        if ($candidateValue -eq $Value) {
            $expanded.Add($Value)
            continue
        }
        foreach ($candidate in (Expand-CMakeValueList -Value $candidateValue -Variables $Variables -Depth ($Depth + 1))) {
            $expanded.Add($candidate)
        }
    }
    return @($expanded)
}

function Resolve-CMakeTargetFileNames {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Value,
        [Parameter(Mandatory = $true)]
        [object[]]$ProjectMap
    )

    return [regex]::Replace($Value, '\$<TARGET_FILE_NAME:(?<target>[^>]+)>', {
        param($match)
        $output = Resolve-OutputForTarget -Target $match.Groups["target"].Value -ProjectMap $ProjectMap
        if ($output) {
            return [IO.Path]::GetFileName($output)
        }
        return $match.Value
    })
}

function Test-IsoRelativePath {
    param([Parameter(Mandatory = $true)][string]$Path)

    if ([string]::IsNullOrWhiteSpace($Path) -or
        $Path -match '\$(?:\{|<|\()|[<>:"\\|?*\x00-\x1F]' -or
        $Path.StartsWith("/")) {
        return $false
    }
    foreach ($component in $Path.Split("/")) {
        if ($component -in @("", ".", "..")) {
            return $false
        }
    }
    return $true
}

function Get-ProjectOutputMap {
    param(
        [Parameter(Mandatory = $true)]
        [string]$ProjectRoot
    )

    $result = [Collections.Generic.List[object]]::new()
    foreach ($project in Get-ChildItem -LiteralPath $ProjectRoot -Recurse -Filter "*.vcxproj" -File) {
        try {
            $xml = [xml](Get-Content -LiteralPath $project.FullName -Raw)
            $projectNameNode = $xml.SelectSingleNode("//*[local-name()='ProjectName']")
            if (-not $projectNameNode) {
                continue
            }

            $configurationKey = "$Configuration|$Platform"
            $propertyGroups = @($xml.SelectNodes("//*[local-name()='PropertyGroup']") | Where-Object {
                $condition = $_.Condition
                -not $condition -or $condition -like "*$configurationKey*"
            })
            $targetNameNode = $null
            $targetExtNode = $null
            $typeNode = $null
            foreach ($group in $propertyGroups) {
                $groupTargetName = $group.SelectSingleNode("*[local-name()='TargetName']")
                $groupTargetExt = $group.SelectSingleNode("*[local-name()='TargetExt']")
                $groupType = $group.SelectSingleNode("*[local-name()='ConfigurationType']")
                if (-not $targetNameNode -and $groupTargetName) { $targetNameNode = $groupTargetName }
                if (-not $targetExtNode -and $groupTargetExt) { $targetExtNode = $groupTargetExt }
                if (-not $typeNode -and $groupType) { $typeNode = $groupType }
            }
            if (-not $projectNameNode) {
                continue
            }
            $projectName = $projectNameNode.InnerText
            $targetName = if ($targetNameNode) { $targetNameNode.InnerText } else { $projectName }
            $configurationType = if ($typeNode) { $typeNode.InnerText } else { "" }
            if (-not $targetName) {
                continue
            }
            $targetExt = if ($targetExtNode) { $targetExtNode.InnerText } else {
                switch ($configurationType) {
                    "DynamicLibrary" { ".dll" }
                    "StaticLibrary" { ".lib" }
                    default { ".exe" }
                }
            }
            $result.Add([pscustomobject]@{
                Project = $project.FullName
                ProjectName = $projectName
                TargetName = $targetName
                TargetExt = $targetExt
                ConfigurationType = $configurationType
                Buildable = ($configurationType -ne "Utility")
                Output = Join-Path $KundowsRoot "OutDir\$Platform\$Configuration\$projectName\$targetName$targetExt"
            })
        } catch {
            throw "Unable to read native project '$($project.FullName)': $($_.Exception.Message)"
        }
    }
    return @($result)
}

function Resolve-OutputForTarget {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Target,
        [Parameter(Mandatory = $true)]
        [object[]]$ProjectMap
    )

    $special = @{
        "dosmbr" = "boot\freeldr\bootsect\dosmbr.bin"
        "ext" = "boot\freeldr\bootsect\ext.bin"
        "extldr" = "boot\freeldr\bootsect\extldr.sys"
        "btrfsvbr" = "boot\freeldr\bootsect\btrfs.bin"
        "fat" = "boot\freeldr\bootsect\fat.bin"
        "fat32" = "boot\freeldr\bootsect\fat32.bin"
        "ntfsvbr" = "boot\freeldr\bootsect\ntfs.bin"
        "isoboot" = "boot\freeldr\bootsect\isoboot.bin"
        "isobtrt" = "boot\freeldr\bootsect\isobtrt.bin"
        "isombr" = "boot\freeldr\bootsect\isombr.bin"
        "efisys" = "efisys.bin"
    }
    if ($special.ContainsKey($Target)) {
        return Join-Path $generatedRoot $special[$Target]
    }

    $exactProject = @($ProjectMap | Where-Object {
        $_.Buildable -and
        [string]::Equals($_.ProjectName, $Target, [StringComparison]::OrdinalIgnoreCase)
    })
    if ($exactProject.Count -eq 1) {
        return $exactProject[0].Output
    }

    $candidates = @($ProjectMap | Where-Object {
        $_.Buildable -and
        [string]::Equals($_.TargetName, $Target, [StringComparison]::OrdinalIgnoreCase) -or
        $_.Buildable -and
        [string]::Equals($_.ProjectName, $Target, [StringComparison]::OrdinalIgnoreCase)
    })
    $existing = @($candidates | Where-Object {
        Test-Path -LiteralPath $_.Output -PathType Leaf
    })
    if ($existing.Count -eq 1) {
        return $existing[0].Output
    }
    if ($existing.Count -gt 1) {
        return $null
    }
    if ($candidates.Count -eq 1) {
        return $candidates[0].Output
    }
    return $null
}

function Get-ActiveFunctionCdCalls {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Text,
        [Parameter(Mandatory = $true)]
        [hashtable]$Variables,
        [Parameter(Mandatory = $true)]
        [object[]]$FunctionDefinitions,
        [Parameter(Mandatory = $true)]
        [object[]]$ProjectMap
    )

    $calls = [Collections.Generic.List[object]]::new()
    foreach ($definition in $FunctionDefinitions) {
        foreach ($invocation in @(Get-CMakeCalls -Text $Text -CommandName $definition.Name)) {
            $invocationTokens = [Collections.Generic.List[string]]::new()
            foreach ($token in (Split-CMakeArguments $invocation.Body)) {
                foreach ($expanded in (Expand-CMakeValueList -Value $token -Variables $Variables)) {
                    $invocationTokens.Add([string]$expanded)
                }
            }

            $localVariables = @{}
            foreach ($key in $Variables.Keys) {
                $localVariables[$key] = $Variables[$key]
            }

            $parameters = @(Split-CMakeArguments $definition.Parameters)
            $fixedArgumentIndex = 0
            foreach ($parameter in $parameters) {
                if (-not $parameter) {
                    continue
                }
                if ($parameter -eq "ARGN") {
                    $localVariables[$parameter] = @(
                        if ($fixedArgumentIndex -lt $invocationTokens.Count) {
                            $invocationTokens[$fixedArgumentIndex..($invocationTokens.Count - 1)]
                        }
                    )
                    break
                }
                $localVariables[$parameter] = if ($fixedArgumentIndex -lt $invocationTokens.Count) {
                    $invocationTokens[$fixedArgumentIndex]
                } else {
                    ""
                }
                $fixedArgumentIndex++
            }

            foreach ($parseCall in @(Get-CMakeCalls -Text $definition.Body -CommandName "cmake_parse_arguments")) {
                $parseTokens = @(Split-CMakeArguments $parseCall.Body)
                if ($parseTokens.Count -lt 4) {
                    continue
                }
                $prefix = $parseTokens[0]
                $oneValueKeywords = @(
                    if ($parseTokens[2]) { $parseTokens[2] -split ";" }
                )
                $multiValueKeywords = @(
                    if ($parseTokens[3]) { $parseTokens[3] -split ";" }
                )
                $keywords = @($oneValueKeywords + $multiValueKeywords)
                $extraTokens = @(
                    if ($fixedArgumentIndex -lt $invocationTokens.Count) {
                        $invocationTokens[$fixedArgumentIndex..($invocationTokens.Count - 1)]
                    }
                )
                for ($index = 0; $index -lt $extraTokens.Count; $index++) {
                    $keyword = $extraTokens[$index]
                    if ($oneValueKeywords -contains $keyword) {
                        $localVariables["${prefix}_${keyword}"] = if ($index + 1 -lt $extraTokens.Count) {
                            $extraTokens[$index + 1]
                        } else {
                            ""
                        }
                        $index++
                    } elseif ($multiValueKeywords -contains $keyword) {
                        $values = [Collections.Generic.List[string]]::new()
                        for ($valueIndex = $index + 1; $valueIndex -lt $extraTokens.Count; $valueIndex++) {
                            if ($keywords -contains $extraTokens[$valueIndex]) {
                                $index = $valueIndex - 1
                                break
                            }
                            $values.Add($extraTokens[$valueIndex])
                            $index = $valueIndex
                        }
                        $localVariables["${prefix}_${keyword}"] = @($values)
                    }
                }
            }

            foreach ($setCall in @(Get-CMakeCalls -Text $definition.Body -CommandName "set")) {
                $setTokens = @(Split-CMakeArguments $setCall.Body)
                if ($setTokens.Count -lt 2) {
                    continue
                }
                $setName = $setTokens[0]
                $setValues = [Collections.Generic.List[string]]::new()
                foreach ($setValue in $setTokens[1..($setTokens.Count - 1)]) {
                    foreach ($expanded in (Expand-CMakeValueList -Value $setValue -Variables $localVariables)) {
                        $value = [string]$expanded
                        if ($value -match '^\$<TARGET_FILE_NAME:(?<target>[^>]+)>$') {
                            $targetOutput = Resolve-OutputForTarget -Target $Matches["target"] -ProjectMap $ProjectMap
                            if ($targetOutput) {
                                $value = [IO.Path]::GetFileName($targetOutput)
                            }
                        }
                        $setValues.Add($value)
                    }
                }
                $hasExistingValue = $localVariables.ContainsKey($setName) -and
                    @($localVariables[$setName]).Count -gt 0 -and
                    [string]$localVariables[$setName][0]
                if (-not ($hasExistingValue -and $setName -match "_NAME_ON_CD$")) {
                    $localVariables[$setName] = if ($setValues.Count -eq 1) {
                        $setValues[0]
                    } else {
                        @($setValues)
                    }
                }
            }

            foreach ($cdCall in @(Get-CMakeCalls -Text $definition.Body -CommandName "add_cd_file")) {
                $calls.Add([pscustomobject]@{
                    Text = $cdCall.Text
                    Body = $cdCall.Body
                    Offset = $invocation.Offset
                    Variables = $localVariables
                })
            }
        }
    }
    return @($calls)
}

function Test-ProjectTargetDisabled {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Target,
        [Parameter(Mandatory = $true)]
        [object[]]$ProjectMap
    )

    return @($ProjectMap | Where-Object {
        -not $_.Buildable -and
        ([string]::Equals($_.TargetName, $Target, [StringComparison]::OrdinalIgnoreCase) -or
         [string]::Equals($_.ProjectName, $Target, [StringComparison]::OrdinalIgnoreCase))
    }).Count -gt 0
}

function Find-CMakeTokenIndex {
    param(
        [Parameter(Mandatory = $true)]
        [string[]]$Tokens,
        [Parameter(Mandatory = $true)]
        [string]$Token,
        [int]$Start = 0
    )

    for ($index = $Start; $index -lt $Tokens.Count; $index++) {
        if ([string]::Equals($Tokens[$index], $Token, [StringComparison]::OrdinalIgnoreCase)) {
            return $index
        }
    }
    return -1
}

function Parse-CdArguments {
    param(
        [Parameter(Mandatory = $true)]
        [string[]]$Tokens
    )

    $keywords = @("TARGET", "FILE", "DESTINATION", "NAME_ON_CD", "FOR", "NO_CAB")
    $parsed = [ordered]@{
        Target = ""
        Files = [Collections.Generic.List[string]]::new()
        Destination = ""
        NameOnCd = ""
        Images = [Collections.Generic.List[string]]::new()
        NoCab = $false
        Unknown = [Collections.Generic.List[string]]::new()
    }

    $index = 0
    while ($index -lt $Tokens.Count) {
        $token = $Tokens[$index]
        $upper = $token.ToUpperInvariant()
        switch ($upper) {
            "TARGET" {
                if ($index + 1 -ge $Tokens.Count) {
                    $parsed.Unknown.Add("TARGET")
                } else {
                    $parsed.Target = $Tokens[++$index]
                }
            }
            "FILE" {
                $index++
                while ($index -lt $Tokens.Count -and $keywords -notcontains $Tokens[$index].ToUpperInvariant()) {
                    $parsed.Files.Add($Tokens[$index])
                    $index++
                }
                $index--
            }
            "DESTINATION" {
                if ($index + 1 -ge $Tokens.Count) {
                    $parsed.Unknown.Add("DESTINATION")
                } else {
                    $parsed.Destination = $Tokens[++$index]
                }
            }
            "NAME_ON_CD" {
                if ($index + 1 -ge $Tokens.Count) {
                    $parsed.Unknown.Add("NAME_ON_CD")
                } else {
                    $parsed.NameOnCd = $Tokens[++$index]
                }
            }
            "FOR" {
                $index++
                while ($index -lt $Tokens.Count -and $keywords -notcontains $Tokens[$index].ToUpperInvariant()) {
                    $parsed.Images.Add($Tokens[$index])
                    $index++
                }
                $index--
            }
            "NO_CAB" {
                $parsed.NoCab = $true
            }
            default {
                $parsed.Unknown.Add($token)
            }
        }
        $index++
    }
    return [pscustomobject]$parsed
}

function Test-IsPlannedGeneratedPath {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Path
    )

    if (-not $Path.StartsWith($generatedRoot, [StringComparison]::OrdinalIgnoreCase)) {
        return $false
    }

    $relative = [IO.Path]::GetRelativePath($generatedRoot, $Path).Replace("\", "/")
    $planned = @(
        "boot/bootdata/caroots.inf",
        "boot/bootdata/hivebcd_utf16.inf",
        "boot/bootdata/registry.inf",
        "boot/bootdata/SETUPREG.HIV",
        "boot/bootdata/system",
        "boot/bootdata/software",
        "boot/bootdata/default",
        "boot/bootdata/sam",
        "boot/bootdata/security",
        "boot/bootdata/BCD",
        "boot/bootdata/packages/reactos.inf",
        "boot/bootdata/packages/reactos.cab",
        "efisys.bin",
        "media/sdb/sysmain.sdb",
        "media/sdb/drvmain.sdb",
        "media/vgafonts/vgafonts.cab"
    )
    if ($planned -contains $relative) {
        return $true
    }
    return $relative -match "^media/nls/[^/]+\.nls$"
}

function Get-BootcdIniSource {
    $source = Join-Path $SourceRoot "boot\bootdata\bootcd.ini"
    if ($Configuration -ne "Release") {
        return $source
    }

    $output = Join-Path $generatedRoot "boot\bootdata\bootcd.Release.ini"
    $contents = Get-Content -LiteralPath $source -Raw
    $contents = [regex]::Replace(
        $contents,
        "(?im)^\s*DefaultOS\s*=\s*LiveImg_Debug\s*$",
        "DefaultOS=LiveImg"
    )
    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $output) | Out-Null
    [IO.File]::WriteAllText($output, $contents, [Text.UTF8Encoding]::new($false))
    return $output
}

function Get-HostToolPath {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Name
    )

    $path = Join-Path $KundowsRoot "OutDir\HostTools\$Platform\$Configuration\$Name.exe"
    Assert-File $path "Host tool '$Name'"
    return $path
}

function Invoke-HostTool {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Name,
        [Parameter(Mandatory = $true)]
        [string[]]$Arguments,
        [Parameter(Mandatory = $true)]
        [string]$WorkingDirectory
    )

    $tool = Get-HostToolPath $Name
    New-Item -ItemType Directory -Force -Path $WorkingDirectory | Out-Null
    Write-Host "[$Name] $($Arguments -join ' ')"
    Push-Location $WorkingDirectory
    try {
        & $tool @Arguments
        if ($LASTEXITCODE -ne 0) {
            throw "Host tool '$Name' failed with exit code $LASTEXITCODE."
        }
    }
    finally {
        Pop-Location
    }
}

function Convert-ToUtf16File {
    param(
        [Parameter(Mandatory = $true)]
        [string]$InputFile,
        [Parameter(Mandatory = $true)]
        [string]$OutputFile
    )

    Assert-File $InputFile "UTF-16 input"
    $outputDirectory = Split-Path -Parent $OutputFile
    New-Item -ItemType Directory -Force -Path $outputDirectory | Out-Null
    Invoke-HostTool -Name "utf16le" -WorkingDirectory $outputDirectory -Arguments @($InputFile, $OutputFile)
}

function Write-ConcatenatedFile {
    param(
        [Parameter(Mandatory = $true)]
        [string[]]$InputFiles,
        [Parameter(Mandatory = $true)]
        [string]$OutputFile
    )

    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $OutputFile) | Out-Null
    $output = [IO.File]::Open($OutputFile, [IO.FileMode]::Create, [IO.FileAccess]::Write, [IO.FileShare]::None)
    try {
        foreach ($inputFile in $InputFiles) {
            Assert-File $inputFile "Concatenation input"
            $input = [IO.File]::OpenRead($inputFile)
            try {
                $input.CopyTo($output)
            }
            finally {
                $input.Dispose()
            }
        }
    }
    finally {
        $output.Dispose()
    }
}

function Get-RegistryInfSources {
    $sources = [Collections.Generic.List[string]]::new()
    $projectMap = Get-ProjectOutputMap -ProjectRoot (Join-Path $KundowsRoot "Upstream\ReactOS\Projects")
    foreach ($cmake in Get-ChildItem -LiteralPath $SourceRoot -Recurse -Filter "CMakeLists.txt" -File) {
        $sourceDirectory = Split-Path -Parent $cmake.FullName
        $relativeDirectory = [IO.Path]::GetRelativePath($SourceRoot, $sourceDirectory).Replace("\", "/").ToLowerInvariant()
        if ($defaultDisabledCmakeDirectories -contains $relativeDirectory) {
            continue
        }
        if ($relativeDirectory -eq "modules/rosapps" -or
            $relativeDirectory.StartsWith("modules/rosapps/") -or
            $relativeDirectory -eq "modules/rostests" -or
            $relativeDirectory.StartsWith("modules/rostests/") -or
            $relativeDirectory -eq "modules/wallpapers" -or
            $relativeDirectory.StartsWith("modules/wallpapers/")) {
            continue
        }
        $text = Get-Content -LiteralPath $cmake.FullName -Raw
        foreach ($call in Get-CMakeCalls -Text $text -CommandName "add_registry_inf") {
            foreach ($token in (Split-CMakeArguments $call.Body)) {
                if ($token -match "\$\{|\$\(" -or $token.StartsWith("#")) {
                    continue
                }
                $targetStem = [IO.Path]::GetFileNameWithoutExtension($token)
                $targetNames = @(
                    $targetStem
                    if ($targetStem -match "(?i)_reg$") {
                        $targetStem -replace "(?i)_reg$", ""
                    }
                ) | Where-Object { $_ } | Select-Object -Unique
                $targetMatches = @($projectMap | Where-Object {
                    $targetNames -contains $_.ProjectName -or
                    $targetNames -contains $_.TargetName
                })
                if ($targetMatches.Count -gt 1) {
                    throw "Registry INF target '$($targetNames -join ', ')' is ambiguous for '$($cmake.FullName)'."
                }
                if ($targetMatches.Count -eq 1 -and -not $targetMatches[0].Buildable) {
                    continue
                }
                $path = if ([IO.Path]::IsPathRooted($token)) {
                    [IO.Path]::GetFullPath($token)
                } else {
                    [IO.Path]::GetFullPath((Join-Path $sourceDirectory $token))
                }
                if (Test-Path -LiteralPath $path -PathType Leaf) {
                    $sources.Add($path)
                }
            }
        }
    }
    return @($sources | Sort-Object -Unique)
}

function Prepare-RegistryInputs {
    $converted = [Collections.Generic.List[string]]::new()
    foreach ($source in Get-RegistryInfSources) {
        $relative = [IO.Path]::GetRelativePath($SourceRoot, $source)
        $relativeDirectory = Split-Path -Parent $relative
        $baseName = [IO.Path]::GetFileNameWithoutExtension($relative)
        $output = Join-Path (Join-Path $generatedRoot $relativeDirectory) ($baseName + "_utf16.inf")
        Convert-ToUtf16File -InputFile $source -OutputFile $output
        $converted.Add($output)
    }

    $bootDataRoot = Join-Path $generatedRoot "boot\bootdata"
    $registryInf = Join-Path $bootDataRoot "registry.inf"
    Write-ConcatenatedFile -InputFiles @($converted) -OutputFile $registryInf

    $carootsOutput = Join-Path $bootDataRoot "caroots.inf"
    Convert-ToUtf16File `
        -InputFile (Join-Path $SourceRoot "boot\bootdata\caroots.inf") `
        -OutputFile $carootsOutput
    $hiveBcdOutput = Join-Path $bootDataRoot "hivebcd_utf16.inf"
    Convert-ToUtf16File `
        -InputFile (Join-Path $SourceRoot "boot\bootdata\hivebcd.inf") `
        -OutputFile $hiveBcdOutput

    return [pscustomobject]@{
        BootDataRoot = $bootDataRoot
        RegistryInf = $registryInf
        SetupRegInf = Join-Path $SourceRoot "boot\bootdata\setupreg.inf"
        LiveCdInf = Join-Path $SourceRoot "boot\bootdata\livecd.inf"
        HiveInstInf = Join-Path $SourceRoot "boot\bootdata\hiveinst.inf"
        CarootsInf = $carootsOutput
        HiveBcdInf = $hiveBcdOutput
    }
}

function Build-RegistryHives {
    param(
        [Parameter(Mandatory = $true)]
        [object]$RegistryInputs
    )

    $bootData = $RegistryInputs.BootDataRoot
    New-Item -ItemType Directory -Force -Path $bootData | Out-Null
    Invoke-HostTool -Name "mkhive" -WorkingDirectory $bootData -Arguments @(
        "-h:SETUPREG", "-u", "-d:$bootData",
        $RegistryInputs.RegistryInf, $RegistryInputs.SetupRegInf
    )
    Invoke-HostTool -Name "mkhive" -WorkingDirectory $bootData -Arguments @(
        "-h:SYSTEM,SOFTWARE,DEFAULT,SAM,SECURITY", "-d:$bootData",
        $RegistryInputs.RegistryInf, $RegistryInputs.LiveCdInf,
        $RegistryInputs.CarootsInf, $RegistryInputs.HiveInstInf
    )
    Invoke-HostTool -Name "mkhive" -WorkingDirectory $bootData -Arguments @(
        "-h:BCD", "-u", "-d:$bootData", $RegistryInputs.HiveBcdInf
    )
}

function Build-NlsFiles {
    $sourceDirectory = Join-Path $SourceRoot "media\nls\src"
    $outputDirectory = Join-Path $generatedRoot "media\nls"
    New-Item -ItemType Directory -Force -Path $outputDirectory | Out-Null
    foreach ($source in Get-ChildItem -LiteralPath $sourceDirectory -Filter "*.txt" -File) {
        $output = Join-Path $outputDirectory ($source.BaseName + ".nls")
        Invoke-HostTool -Name "txt2nls" -WorkingDirectory $outputDirectory -Arguments @(
            $source.FullName, $output
        )
    }
}

function Build-SdbFiles {
    $sourceDirectory = Join-Path $SourceRoot "media\sdb"
    $outputDirectory = Join-Path $generatedRoot "media\sdb"
    New-Item -ItemType Directory -Force -Path $outputDirectory | Out-Null
    foreach ($databaseName in @("sysmain", "drvmain")) {
        $input = Join-Path $sourceDirectory ($databaseName + ".xml")
        $output = Join-Path $outputDirectory ($databaseName + ".sdb")
        Assert-File $input "SDB XML input"
        Invoke-HostTool -Name "xml2sdb" -WorkingDirectory $outputDirectory -Arguments @(
            "-p", $architecture, "-i", $input, "-o", $output
        )
        Assert-File $output "Generated SDB database"
    }
}

function Build-VgaFontsCabinet {
    $sourceDirectory = Join-Path $SourceRoot "media\vgafonts"
    $outputDirectory = Join-Path $generatedRoot "media\vgafonts"
    New-Item -ItemType Directory -Force -Path $outputDirectory | Out-Null
    $output = Join-Path $outputDirectory "vgafonts.cab"
    $sourceNames = @(
        "437-8x8.bin",
        "737-8x8.bin",
        "775-8x8.bin",
        "850-8x8.bin",
        "852-8x8.bin",
        "857-8x8.bin",
        "862-8x8.bin",
        "865-8x8.bin",
        "866-8x8.bin",
        "932-8x8.bin",
        "28606-8x8.bin"
    )
    $sources = [Collections.Generic.List[string]]::new()
    foreach ($sourceName in $sourceNames) {
        $source = Join-Path $sourceDirectory $sourceName
        Assert-File $source "VGA font source"
        $sources.Add($source)
    }
    $arguments = @("-M", "raw", "-S", $output) + @($sources)
    Invoke-HostTool -Name "cabman" -WorkingDirectory $outputDirectory -Arguments $arguments
    Assert-File $output "Generated VGA font cabinet"
    return $output
}

function Prepare-GeneratedInfInputs {
    param(
        [Parameter(Mandatory = $true)]
        [object[]]$Conversions
    )

    foreach ($conversion in $Conversions) {
        Convert-ToUtf16File -InputFile $conversion.Source -OutputFile $conversion.Output
    }
}

function Get-CabDirectoryNumbers {
    $dffInput = Join-Path $SourceRoot "boot\bootdata\packages\reactos.dff.in"
    Assert-File $dffInput "ReactOS CAB directive template"
    $numbers = @{}
    $inDirectories = $false
    foreach ($line in Get-Content -LiteralPath $dffInput) {
        if ($line -match "^\s*\[Directories\]\s*$") {
            $inDirectories = $true
            continue
        }
        if ($inDirectories -and $line -match "^\s*\[") {
            break
        }
        if ($inDirectories -and $line -match "^\s*(\d+)\s*=") {
            $number = [int]$Matches[1]
            $value = ($line -replace "^\s*\d+\s*=\s*", "")
            $value = ($value -replace "\s*;.*$", "").Trim().Trim('"')
            $key = $value.Replace("\", "/").Trim("/").ToLowerInvariant()
            if (-not $key) {
                $numbers["reactos"] = $number
            } else {
                $numbers[$key] = $number
                $numbers["reactos/$key"] = $number
            }
        }
    }
    return $numbers
}

function Build-ReactOSCabinet {
    param(
        [Parameter(Mandatory = $true)]
        [object[]]$Declarations
    )

    $packageRoot = Join-Path $generatedRoot "boot\bootdata\packages"
    New-Item -ItemType Directory -Force -Path $packageRoot | Out-Null
    $template = Join-Path $SourceRoot "boot\bootdata\packages\reactos.dff.in"
    $dff = Join-Path $packageRoot "reactos.dff"
    Copy-Item -LiteralPath $template -Destination $dff -Force

    $directoryNumbers = Get-CabDirectoryNumbers
    $dynamicLines = [Collections.Generic.List[string]]::new()
    $seen = @{}
    $directKeys = @{}
    foreach ($directDeclaration in @($Declarations | Where-Object {
        $_.Image -eq "bootcd" -and $_.Placement -eq "Direct"
    })) {
        $directKey = "$($directDeclaration.Source)|$($directDeclaration.Destination)|$($directDeclaration.NameOnCd)"
        $directKeys[$directKey] = $true
    }
    foreach ($declaration in @($Declarations | Where-Object {
        $_.Image -eq "bootcd" -and $_.Placement -eq "Cab"
    })) {
        $directKey = "$($declaration.Source)|$($declaration.Destination)|$($declaration.NameOnCd)"
        if ($directKeys.ContainsKey($directKey)) {
            continue
        }
        $destination = ([string]$declaration.Destination).Replace([string]"\", [string]"/").ToLowerInvariant()
        if (-not $directoryNumbers.ContainsKey($destination)) {
            throw "No upstream CAB directory number for '$($declaration.Destination)'."
        }
        $key = "$($declaration.Source)|$($directoryNumbers[$destination])"
        if ($seen.ContainsKey($key)) { continue }
        $seen[$key] = $true
        $source = $declaration.Source
        $dynamicLines.Add("`"$source`" $($directoryNumbers[$destination])")
    }

    $dynamic = Join-Path $packageRoot "reactos.dff.dyn"
    [IO.File]::WriteAllLines($dynamic, $dynamicLines, [Text.UTF8Encoding]::new($false))
    $dffBytes = [IO.File]::ReadAllBytes($dff)
    $dynamicBytes = [IO.File]::ReadAllBytes($dynamic)
    $extra = if ($dffBytes.Length -gt 0 -and $dffBytes[$dffBytes.Length - 1] -ne 10) { 2 } else { 0 }
    $combined = [byte[]]::new($dffBytes.Length + $dynamicBytes.Length + $extra)
    [Array]::Copy($dffBytes, 0, $combined, 0, $dffBytes.Length)
    $offset = $dffBytes.Length
    if ($extra) {
        $combined[$offset++] = 13
        $combined[$offset++] = 10
    }
    [Array]::Copy($dynamicBytes, 0, $combined, $offset, $dynamicBytes.Length)
    [IO.File]::WriteAllBytes($dff, $combined)

    Invoke-HostTool -Name "cabman" -WorkingDirectory $packageRoot -Arguments @(
        "-C", $dff, "-L", $packageRoot, "-I", "-P", $SourceRoot
    )
    $cabInf = Join-Path $packageRoot "reactos.inf"
    Assert-File $cabInf "Generated ReactOS CAB INF"
    Invoke-HostTool -Name "cabman" -WorkingDirectory $packageRoot -Arguments @(
        "-C", $dff, "-L", $packageRoot, "-N", "-RC", $cabInf, "-P", $SourceRoot
    )
    $cab = Join-Path $packageRoot "reactos.cab"
    Assert-File $cab "Generated ReactOS CAB"
    return [pscustomobject]@{
        Cab = $cab
        Inf = $cabInf
        Dff = $dff
    }
}

function Build-EfiSystemImage {
    $efiPlatformId = switch ($Platform) {
        "Win32" { "ia32" }
        "x64" { "x64" }
        "ARM64" { "aa64" }
    }
    $projectRoot = Join-Path $KundowsRoot "Upstream\ReactOS\Projects"
    $projectMap = Get-ProjectOutputMap -ProjectRoot $projectRoot
    $uefildr = Resolve-OutputForTarget -Target "uefildr" -ProjectMap $projectMap
    Assert-File $uefildr "UEFI loader output"
    $efiImage = Join-Path $generatedRoot "efisys.bin"
    if (Test-Path -LiteralPath $efiImage) {
        Remove-Item -LiteralPath $efiImage -Force
    }
    $arguments = @($efiImage, "-format", "2880", "EFIBOOT")
    if ($Platform -eq "Win32" -or $Platform -eq "x64") {
        $fatBoot = Join-Path $generatedRoot "boot\freeldr\bootsect\fat.bin"
        Assert-File $fatBoot "FAT EFI boot sector"
        $arguments += @("-boot", $fatBoot)
    }
    $arguments += @(
        "-mkdir", "EFI",
        "-mkdir", "EFI/BOOT",
        "-add", $uefildr, "EFI/BOOT/boot$efiPlatformId.efi"
    )
    Invoke-HostTool -Name "fatten" -WorkingDirectory $generatedRoot -Arguments $arguments
    Assert-File $efiImage "Generated EFI system image"
    return $efiImage
}

function Add-IsoGraftLine {
    param(
        [Parameter(Mandatory = $true)]
        [Collections.Generic.List[string]]$Lines,
        [Parameter(Mandatory = $true)]
        [hashtable]$Seen,
        [Parameter(Mandatory = $true)]
        [string]$Destination,
        [Parameter(Mandatory = $true)]
        [string]$Source
    )

    $normalizedDestination = ([string]$Destination).Replace([string]"\", [string]"/").Trim("/")
    $line = if ($normalizedDestination) {
        "$normalizedDestination=$Source"
    } else {
        $Source
    }
    if (-not $Seen.ContainsKey($line)) {
        $Seen[$line] = $true
        $Lines.Add($line)
    }
}

function Write-BootcdIsoInputs {
    param(
        [Parameter(Mandatory = $true)]
        [object[]]$Declarations,
        [Parameter(Mandatory = $true)]
        [object]$Cabinet,
        [Parameter(Mandatory = $true)]
        [string]$EfiImage
    )

    $empty = Join-Path $auditRoot "empty"
    New-Item -ItemType Directory -Force -Path $empty | Out-Null
    $lines = [Collections.Generic.List[string]]::new()
    $seen = @{}
    $lines.Add($empty)

    foreach ($declaration in @($Declarations | Where-Object {
        ($_.Image -eq "bootcd" -and $_.Placement -eq "Direct") -or
        ($_.Image -eq "livecd")
    })) {
        $destination = ([string]$declaration.Destination).Replace([string]"\", [string]"/")
        if ($declaration.Image -eq "bootcd" -and $destination -match "^reactos(?:/|$)") {
            $destination = $architecture + $destination.Substring(7)
        }
        $target = if ($destination) {
            "$destination/$($declaration.NameOnCd)"
        } else {
            $declaration.NameOnCd
        }
        Add-IsoGraftLine -Lines $lines -Seen $seen -Destination $target -Source $declaration.Source
    }

    $bootData = Join-Path $generatedRoot "boot\bootdata"
    Add-IsoGraftLine -Lines $lines -Seen $seen -Destination "$architecture/reactos.cab" -Source $Cabinet.Cab
    Add-IsoGraftLine -Lines $lines -Seen $seen -Destination "$architecture/reactos.inf" -Source $Cabinet.Inf
    Add-IsoGraftLine -Lines $lines -Seen $seen -Destination "$architecture/registry.inf" -Source (Join-Path $bootData "registry.inf")
    Add-IsoGraftLine -Lines $lines -Seen $seen -Destination "$architecture/SETUPREG.HIV" -Source (Join-Path $bootData "SETUPREG.HIV")
    Add-IsoGraftLine -Lines $lines -Seen $seen -Destination "efi/boot/BCD" -Source (Join-Path $bootData "BCD")

    $liveHiveRoot = "reactos/system32/config"
    foreach ($hiveName in @("system", "software", "default", "sam", "security")) {
        Add-IsoGraftLine -Lines $lines -Seen $seen `
            -Destination "$liveHiveRoot/$hiveName" `
            -Source (Join-Path $bootData $hiveName)
    }

    foreach ($emptyDirectory in @(
        "reactos/TEMP",
        "ProgramData/Microsoft/Windows/Start Menu/Programs/Startup",
        "Users/Public/Music",
        "Users/Public/Pictures",
        "Users/Public/Videos",
        "Users/Public/Favorites",
        "Users/Public/Documents",
        "Users/Public/Templates",
        "Users/Default/AppData",
        "Users/Default/AppData/Roaming/Microsoft/Internet Explorer/Quick Launch",
        "Users/Default/AppData/Roaming/Microsoft/Windows/Network Shortcuts",
        "Users/Default/AppData/Roaming/Microsoft/Windows/Printer Shortcuts",
        "Users/Default/AppData/Local/Microsoft/Windows/INetCookies",
        "Users/Default/AppData/Local/Microsoft/Windows/INetCache",
        "Users/Default/Desktop",
        "Users/Default/Favorites",
        "Users/Default/AppData/Local/Microsoft/Windows/History",
        "Users/Default/Music",
        "Users/Default/Pictures",
        "Users/Default/Videos",
        "Users/Default/AppData/Roaming/Microsoft/Windows/Start Menu/Programs",
        "Users/Default/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/Startup",
        "Users/Default/AppData/Roaming/Microsoft/Windows/SendTo",
        "Users/Default/AppData/Roaming/Microsoft/Windows/Recent",
        "Users/Default/Templates"
    )) {
        Add-IsoGraftLine -Lines $lines -Seen $seen `
            -Destination $emptyDirectory `
            -Source $empty
    }

    $efiName = switch ($Platform) {
        "Win32" { "ia32" }
        "x64" { "x64" }
        "ARM64" { "aa64" }
    }
    Add-IsoGraftLine -Lines $lines -Seen $seen -Destination "loader/efisys.bin" -Source $EfiImage
    $projectMap = Get-ProjectOutputMap -ProjectRoot (Join-Path $KundowsRoot "Upstream\ReactOS\Projects")
    $uefildr = Resolve-OutputForTarget -Target "uefildr" -ProjectMap $projectMap
    Add-IsoGraftLine -Lines $lines -Seen $seen -Destination "efi/boot/boot$efiName.efi" -Source $uefildr

    $listPath = Join-Path $auditRoot "bootcd.$Configuration.lst"
    [IO.File]::WriteAllLines($listPath, $lines, [Text.UTF8Encoding]::new($false))

    $sortPath = Join-Path $auditRoot "bootfiles.sort"
    $sortLines = [Collections.Generic.List[string]]::new()
    $sortLines.Add((Join-Path $empty "boot.catalog") + " 4")
    $sortLines.Add((Join-Path $generatedRoot "boot\freeldr\bootsect\isoboot.bin") + " 3")
    $sortLines.Add((Join-Path $generatedRoot "boot\freeldr\bootsect\isobtrt.bin") + " 2")
    $sortLines.Add($EfiImage + " 1")
    [IO.File]::WriteAllLines($sortPath, $sortLines, [Text.UTF8Encoding]::new($false))

    return [pscustomobject]@{
        List = $listPath
        Sort = $sortPath
    }
}

function Build-BootcdIso {
    param(
        [Parameter(Mandatory = $true)]
        [object]$IsoInputs
    )

    New-Item -ItemType Directory -Force -Path $imageRoot | Out-Null
    $iso = Join-Path $imageRoot "bootcd.iso"
    $arguments = @(
        "-quiet", "-o", $iso,
        "-iso-level", "4",
        "-publisher", "ReactOS Project",
        "-preparer", "ReactOS Project",
        "-volid", "ReactOS",
        "-volset", "ReactOS"
    )
    if ($Platform -eq "Win32" -or $Platform -eq "x64") {
        $arguments += @(
            "-eltorito-platform", "x86",
            "-eltorito-boot", "loader/isoboot.bin",
            "-no-emul-boot",
            "-boot-load-size", "4"
        )
    }
    $arguments += @(
        "-eltorito-alt-boot",
        "-eltorito-platform", "efi",
        "-eltorito-boot", "loader/efisys.bin",
        "-no-emul-boot",
        "-hide", "boot.catalog",
        "-sort", $IsoInputs.Sort,
        "-duplicates-once",
        "-no-cache-inodes",
        "-graft-points",
        "-path-list", $IsoInputs.List
    )
    Invoke-HostTool -Name "mkisofs" -WorkingDirectory $auditRoot -Arguments $arguments
    Assert-File $iso "Generated bootcd ISO"

    if ($Platform -eq "Win32" -or $Platform -eq "x64") {
        $mbr = Join-Path $generatedRoot "boot\freeldr\bootsect\isombr.bin"
        Assert-File $mbr "ISO hybrid MBR"
        Invoke-HostTool -Name "isohybrid" -WorkingDirectory $imageRoot -Arguments @(
            "-b", $mbr, "-t", "0x96", $iso
        )
    }
    $layoutValidator = Join-Path $PSScriptRoot "TestReactOSIsoLayout.ps1"
    Assert-File $layoutValidator "ISO layout validator"
    & pwsh.exe -NoLogo -NoProfile -NonInteractive -File $layoutValidator `
        -Iso $iso -OutputJson (Join-Path $auditRoot "iso-layout.json") | Out-Host
    if ($LASTEXITCODE -ne 0) {
        throw "Generated ISO failed raw directory/layout validation: $iso"
    }
    return $iso
}

function Get-CdDeclarations {
    $projectRoot = Join-Path $KundowsRoot "Upstream\ReactOS\Projects"
    $projectMap = Get-ProjectOutputMap -ProjectRoot $projectRoot
    $declarations = [Collections.Generic.List[object]]::new()
    $unresolved = [Collections.Generic.List[object]]::new()
    $generatedInfConversions = [Collections.Generic.List[object]]::new()

    $cmakeFiles = @(
        Get-ChildItem -LiteralPath $SourceRoot -Recurse -Filter "CMakeLists.txt" -File
        Get-ChildItem -LiteralPath $SourceRoot -Recurse -Filter "*.cmake" -File
    ) | Sort-Object FullName -Unique
    $activeFunctionNames = @(
        "add_bootvid",
        "add_d3dcompiler_target",
        "add_d3dx10_forwarder",
        "add_d3dx11_target",
        "add_d3dx9_target",
        "add_hal"
    )
    $functionDefinitions = [Collections.Generic.List[object]]::new()
    foreach ($functionFile in $cmakeFiles) {
        foreach ($definition in @(Get-CMakeFunctionDefinitions -Text (Get-Content -LiteralPath $functionFile.FullName -Raw))) {
            if ($activeFunctionNames -contains $definition.Name) {
                $functionDefinitions.Add($definition)
            }
        }
    }
    foreach ($cmake in $cmakeFiles) {
        $sourceDirectory = Split-Path -Parent $cmake.FullName
        $relativeDirectory = [IO.Path]::GetRelativePath($SourceRoot, $sourceDirectory)
        $normalizedRelativeDirectory = $relativeDirectory.Replace("\", "/").ToLowerInvariant()
        if ($defaultDisabledCmakeDirectories -contains $normalizedRelativeDirectory) {
            continue
        }
        if ($normalizedRelativeDirectory -eq "modules/rosapps" -or
            $normalizedRelativeDirectory.StartsWith("modules/rosapps/") -or
            $normalizedRelativeDirectory -eq "modules/rostests" -or
            $normalizedRelativeDirectory.StartsWith("modules/rostests/") -or
            $normalizedRelativeDirectory -eq "modules/wallpapers" -or
            $normalizedRelativeDirectory.StartsWith("modules/wallpapers/")) {
            continue
        }
        $binaryDirectory = Join-Path $generatedRoot $relativeDirectory
        if ($normalizedRelativeDirectory -eq "boot" -and $cmake.Name -eq "boot_images.cmake") {
            $binaryDirectory = $generatedRoot
        }
        $text = Get-Content -LiteralPath $cmake.FullName -Raw
        $variables = @{
            "ARCH" = $architecture
            "WINARCH" = $winArchitecture
            "REACTOS_SOURCE_DIR" = $SourceRoot
            "CMAKE_SOURCE_DIR" = $SourceRoot
            "CMAKE_BINARY_DIR" = $generatedRoot
            "CMAKE_CURRENT_SOURCE_DIR" = $sourceDirectory
            "CMAKE_CURRENT_BINARY_DIR" = $binaryDirectory
            "EFI_PLATFORM_ID" = switch ($architecture) {
                "i386" { "ia32" }
                "amd64" { "x64" }
                "arm64" { "aa64" }
            }
        }
        if ($relativeDirectory.Replace("\", "/") -eq "hal/halx86" -and $architecture -eq "amd64") {
            $variables["_halname"] = @("hal", "halmp")
        }
        if ($relativeDirectory.Replace("\", "/") -eq "drivers/base/bootvid" -and $architecture -eq "amd64") {
            $variables["_target"] = @("bootvid", "lfbbvid")
        }

        $variableCalls = [Collections.Generic.List[object]]::new()
        foreach ($commandName in @("set", "list", "foreach")) {
            foreach ($variableCall in Get-CMakeCalls -Text $text -CommandName $commandName) {
                $variableCalls.Add([pscustomobject]@{
                    Command = $commandName
                    Body = $variableCall.Body
                    Offset = $variableCall.Offset
                })
            }
        }
        foreach ($variableCall in @($variableCalls | Sort-Object Offset)) {
            $variableTokens = @(Split-CMakeArguments $variableCall.Body)
            if ($variableCall.Command -eq "set") {
                if ($variableTokens.Count -ge 2) {
                    $variables[$variableTokens[0]] = ($variableTokens[1..($variableTokens.Count - 1)] -join " ")
                }
                continue
            }
            if ($variableCall.Command -eq "list") {
                if ($variableTokens.Count -lt 3) {
                    continue
                }
                $listOperation = $variableTokens[0].ToUpperInvariant()
                $listName = $variableTokens[1]
                if ($listOperation -eq "APPEND" -or $listOperation -eq "PREPEND") {
                    $listValues = [Collections.Generic.List[string]]::new()
                    foreach ($listValue in $variableTokens[2..($variableTokens.Count - 1)]) {
                        foreach ($expandedValue in (Expand-CMakeValueList -Value $listValue -Variables $variables)) {
                            $listValues.Add($expandedValue)
                        }
                    }
                    $oldValues = if ($variables.ContainsKey($listName)) {
                        @($variables[$listName])
                    } else {
                        @()
                    }
                    $variables[$listName] = if ($listOperation -eq "PREPEND") {
                        @($listValues) + @($oldValues)
                    } else {
                        @($oldValues) + @($listValues)
                    }
                } elseif ($listOperation -eq "TRANSFORM" -and $variableTokens.Count -ge 4) {
                    $transform = $variableTokens[2].ToUpperInvariant()
                    $transformValue = $variableTokens[3]
                    $oldValues = @($variables[$listName])
                    if ($transform -eq "APPEND") {
                        $variables[$listName] = @($oldValues | ForEach-Object { "$_$transformValue" })
                    } elseif ($transform -eq "PREPEND") {
                        $variables[$listName] = @($oldValues | ForEach-Object { "$transformValue$_" })
                    }
                }
                continue
            }
            if ($variableTokens.Count -lt 2) {
                continue
            }
            $loopName = $variableTokens[0]
            $loopValues = [Collections.Generic.List[string]]::new()
            foreach ($loopValue in $variableTokens[1..($variableTokens.Count - 1)]) {
                foreach ($expandedValue in (Expand-CMakeValueList -Value $loopValue -Variables $variables)) {
                    $loopValues.Add($expandedValue)
                }
            }
            $variables[$loopName] = @($loopValues)
        }

        foreach ($driverInfCall in Get-CMakeCalls -Text $text -CommandName "add_driver_inf") {
            $driverInfTokens = @(Split-CMakeArguments $driverInfCall.Body)
            if ($driverInfTokens.Count -lt 2) {
                continue
            }
            $driverInfNames = [Collections.Generic.List[string]]::new()
            foreach ($infToken in $driverInfTokens[1..($driverInfTokens.Count - 1)]) {
                foreach ($expandedInfName in (Expand-CMakeValueList -Value $infToken -Variables $variables)) {
                    $driverInfNames.Add($expandedInfName)
                }
            }
            foreach ($infName in $driverInfNames) {
                if ($infName -match "\$\{|\$\(") {
                    $unresolved.Add([pscustomobject]@{
                        File = $cmake.FullName
                        Call = $driverInfCall.Text
                        Reason = "Unresolved add_driver_inf input '$infName'"
                    })
                    continue
                }
                $source = [IO.Path]::GetFullPath((Join-Path $sourceDirectory $infName))
                $output = [IO.Path]::GetFullPath((Join-Path $binaryDirectory $infName))
                if (-not (Test-Path -LiteralPath $source -PathType Leaf)) {
                    $unresolved.Add([pscustomobject]@{
                        File = $cmake.FullName
                        Call = $driverInfCall.Text
                        Reason = "Driver INF source is missing: $source"
                    })
                    continue
                }
                $generatedInfConversions.Add([pscustomobject]@{
                    Source = $source
                    Output = $output
                })
                foreach ($image in @("bootcd", "livecd", "regtest")) {
                    $declarations.Add([pscustomobject]@{
                        Image = $image
                        Placement = "Cab"
                        Source = $output
                        Destination = "reactos/inf"
                        NameOnCd = [IO.Path]::GetFileName($output)
                        UpstreamFile = [IO.Path]::GetRelativePath($SourceRoot, $cmake.FullName)
                        Call = $driverInfCall.Text
                    })
                }
            }
        }

        $cdCalls = @(
            Get-CMakeCalls -Text $text -CommandName "add_cd_file"
            Get-ActiveFunctionCdCalls -Text $text -Variables $variables -FunctionDefinitions @($functionDefinitions) -ProjectMap $projectMap
        )
        foreach ($call in $cdCalls) {
            $tokens = @(Split-CMakeArguments $call.Body)
            if ($tokens.Count -eq 0) {
                continue
            }

            $arguments = Parse-CdArguments -Tokens $tokens
            if ($arguments.Unknown.Count -gt 0) {
                $unresolved.Add([pscustomobject]@{
                    File = $cmake.FullName
                    Call = $call.Text
                    Reason = "Unparsed add_cd_file arguments: $($arguments.Unknown -join ', ')"
                })
                continue
            }
            if (-not $arguments.Images.Count -or -not $arguments.Destination) {
                $unresolved.Add([pscustomobject]@{
                    File = $cmake.FullName
                    Call = $call.Text
                    Reason = "Missing DESTINATION or FOR"
                })
                continue
            }
            $callVariables = if ($call.PSObject.Properties.Name -contains "Variables") {
                $call.Variables
            } else {
                $variables
            }
            $images = @($arguments.Images)
            if ($images -contains "all") {
                $images = @("bootcd", "livecd", "regtest")
            }
            $destinationValues = @(Expand-CMakeValueList -Value $arguments.Destination -Variables $callVariables)
            $destination = if ($destinationValues.Count -gt 0) {
                [string]$destinationValues[0]
            } else {
                ""
            }
            $nameOnCd = if ($arguments.NameOnCd) {
                $nameValues = @(Expand-CMakeValueList -Value $arguments.NameOnCd -Variables $callVariables)
                if ($nameValues.Count -gt 0) { [string]$nameValues[0] } else { "" }
            } else {
                ""
            }

            $values = [Collections.Generic.List[object]]::new()
            if ($arguments.Files.Count -gt 0) {
                foreach ($fileValue in $arguments.Files) {
                    $values.Add([pscustomobject]@{ Kind = "FILE"; Value = $fileValue })
                }
            } elseif ($arguments.Target) {
                $values.Add([pscustomobject]@{ Kind = "TARGET"; Value = $arguments.Target })
            } else {
                $unresolved.Add([pscustomobject]@{
                    File = $cmake.FullName
                    Call = $call.Text
                    Reason = "Missing TARGET or FILE"
                })
                continue
            }

            foreach ($image in $images) {
                foreach ($item in $values) {
                    $expandedValues = @(Expand-CMakeValueList -Value $item.Value -Variables $callVariables)
                    $expandedValues = @($expandedValues | ForEach-Object {
                        if ($_ -is [Array]) { $_ } else { [string]$_ }
                    } | ForEach-Object {
                        if ($_ -match ";") { $_ -split ";" } else { $_ }
                    } | Where-Object { $_ })
                    if ($expandedValues.Count -eq 0) {
                        $expandedValues = @($item.Value)
                    }

                    foreach ($value in $expandedValues) {
                    $unexpanded = $value -match "\$\{|\$\("
                    if ($unexpanded) {
                        if ($value -match "(?i)(bootcd_extras|livecd_extras|3rdparty|welcome_config)" -and
                            $value -match "^(?<directory>.*?)/(?:\$\{[^}]+\})") {
                            if (-not (Test-Path -LiteralPath $Matches["directory"] -PathType Container)) {
                                continue
                            }
                        }
                        $unresolved.Add([pscustomobject]@{
                            File = $cmake.FullName
                            Call = $call.Text
                            Reason = "Unresolved variable '$value'"
                        })
                        continue
                    }
                    $source = if ($item.Kind -eq "TARGET") {
                        Resolve-OutputForTarget -Target $value -ProjectMap $projectMap
                    } else {
                        if ([IO.Path]::IsPathRooted($value)) {
                            [IO.Path]::GetFullPath($value)
                        } else {
                            [IO.Path]::GetFullPath((Join-Path $sourceDirectory $value))
                        }
                    }
                    if ($item.Kind -eq "FILE" -and
                        [string]::Equals(
                            [IO.Path]::GetFullPath($source),
                            [IO.Path]::GetFullPath((Join-Path $SourceRoot "boot\bootdata\bootcd.ini")),
                            [StringComparison]::OrdinalIgnoreCase
                        )) {
                        $source = Get-BootcdIniSource
                    }
                    if (-not $source) {
                        if ($item.Kind -eq "TARGET" -and
                            (Test-ProjectTargetDisabled -Target $value -ProjectMap $projectMap)) {
                            continue
                        }
                        $unresolved.Add([pscustomobject]@{
                            File = $cmake.FullName
                            Call = $call.Text
                            Reason = "No MSBuild output mapping for target '$value'"
                        })
                        continue
                    }
                    if (-not (Test-Path -LiteralPath $source -PathType Leaf) -and
                        -not (Test-IsPlannedGeneratedPath -Path $source)) {
                        $sourceRelative = if ($source.StartsWith($SourceRoot, [StringComparison]::OrdinalIgnoreCase)) {
                            [IO.Path]::GetRelativePath($SourceRoot, $source).Replace("\", "/").ToLowerInvariant()
                        } else {
                            ""
                        }
                        if ($sourceRelative.StartsWith("modules/optional/")) {
                            continue
                        }
                        $unresolved.Add([pscustomobject]@{
                            File = $cmake.FullName
                            Call = $call.Text
                            Reason = "Declared source file is missing: $source"
                        })
                        continue
                    }
                    $cdDestination = if ($destination -eq "root") { "" } else { $destination }
                    $cdName = if ($nameOnCd) { $nameOnCd } else { [IO.Path]::GetFileName($source) }
                    $cdName = Resolve-CMakeTargetFileNames -Value $cdName -ProjectMap $projectMap
                    $cdPath = if ($cdDestination) { "$cdDestination/$cdName" } else { $cdName }
                    if ($cdName.Contains("/") -or -not (Test-IsoRelativePath -Path $cdPath)) {
                        $unresolved.Add([pscustomobject]@{
                            File = $cmake.FullName
                            Call = $call.Text
                            Reason = "Unresolved or invalid ISO destination/name: '$cdPath'"
                        })
                        continue
                    }
                    $declarations.Add([pscustomobject]@{
                        Image = $image
                        Placement = if ($arguments.NoCab) { "Direct" } else { "Cab" }
                        Source = $source
                        Destination = $cdDestination
                        NameOnCd = $cdName
                        UpstreamFile = [IO.Path]::GetRelativePath($SourceRoot, $cmake.FullName)
                        Call = $call.Text
                    })
                    }
                }
            }
        }
    }
    $uniqueDeclarations = @(
        $declarations |
            Group-Object Image, Source, Destination, NameOnCd |
            ForEach-Object {
                $direct = $_.Group | Where-Object { $_.Placement -eq "Direct" }
                $cab = $_.Group | Where-Object { $_.Placement -eq "Cab" }
                $isSpecialNls = @("c_437.nls", "c_1252.nls") -contains ([string]$_.Group[0].NameOnCd).ToLowerInvariant()
                if ($direct -and $isSpecialNls) {
                    $direct[0]
                } elseif ($cab) {
                    $cab[0]
                } else {
                    $_.Group[0]
                }
            }
    )
    $uniqueInfConversions = @(
        $generatedInfConversions |
            Group-Object Source, Output |
            ForEach-Object { $_.Group[0] }
    )
    return [pscustomobject]@{
        Declarations = $uniqueDeclarations
        Unresolved = @($unresolved)
        GeneratedInfConversions = $uniqueInfConversions
    }
}

New-Item -ItemType Directory -Force -Path $auditRoot | Out-Null
$result = Get-CdDeclarations
$report = [pscustomobject]@{
    SourceRoot = $SourceRoot
    Platform = $Platform
    Configuration = $Configuration
    Architecture = $architecture
    Declarations = $result.Declarations
    Unresolved = $result.Unresolved
    GeneratedInfConversions = $result.GeneratedInfConversions
    BootcdUnresolved = @($result.Unresolved | Where-Object {
        $_.Call -match "(?i)\bFOR\s+(?:[^\r\n]*\ball\b|[^\r\n]*\bbootcd\b)"
    })
}
$reportPath = Join-Path $auditRoot "cd-audit.json"
$report | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $reportPath -Encoding utf8

Write-Host "ISO declaration audit: declarations=$($result.Declarations.Count), unresolved=$($result.Unresolved.Count)"
Write-Host "Audit report: $reportPath"

if ($Mode -eq "Build") {
    if ($report.BootcdUnresolved.Count -gt 0) {
        throw "ISO build is blocked because the upstream declaration adapter has unresolved BootCD entries. See $reportPath or use -Mode Audit to resolve them."
    }
    Build-NlsFiles
    Build-SdbFiles
    Build-VgaFontsCabinet
    Prepare-GeneratedInfInputs -Conversions $result.GeneratedInfConversions
    $registryInputs = Prepare-RegistryInputs
    Build-RegistryHives -RegistryInputs $registryInputs
    $cabinet = Build-ReactOSCabinet -Declarations $result.Declarations
    $efiImage = Build-EfiSystemImage
    $isoInputs = Write-BootcdIsoInputs `
        -Declarations $result.Declarations `
        -Cabinet $cabinet `
        -EfiImage $efiImage
    $iso = Build-BootcdIso -IsoInputs $isoInputs

    $buildReport = [pscustomobject]@{
        Audit = $reportPath
        ISO = $iso
        Cabinet = $cabinet.Cab
        HiveRoot = $registryInputs.BootDataRoot
        EfiSystemImage = $efiImage
        IsoFileList = $isoInputs.List
        IsoSortFile = $isoInputs.Sort
    }
    $buildReportPath = Join-Path $auditRoot "bootcd-build.json"
    $buildReport | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $buildReportPath -Encoding utf8
    Write-Host "BootCD ISO: $iso"
    Write-Host "Build report: $buildReportPath"
}
