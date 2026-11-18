param(
    [Parameter(Mandatory = $true)]
    [string]$WidlTool,
    [Parameter(Mandatory = $true)]
    [string]$SourceRoot,
    [Parameter(Mandatory = $true)]
    [string]$GeneratedRoot,
    [string]$OutputFile = "",
    [ValidateSet("Header", "Iids", "RpcClient", "RpcServer", "Resource", "TypeLib", "WinrtResource", "SharedHeaders", "ProjectHeaders")]
    [string]$Mode = "Header",
    [string]$IdlFile = "",
    [string]$HeaderFile = "",
    [string]$IncludeDirectoryList = "",
    [string]$LibraryDirectoryList = "",
    [string]$DefineList = "",
    [string]$CMakeListFile = "",
    [string]$SourceDirectory = "",
    [string]$OutputDirectory = "",
    [ValidateSet("Win32", "x64", "ARM64")]
    [string]$Architecture = "x64"
)

$ErrorActionPreference = "Stop"

function Get-IncludeDirectories([string]$List) {
    return @(
        $List -split ";" |
            ForEach-Object { $_.Trim() } |
            Where-Object { $_.Length -gt 0 }
    )
}

function Get-IdlStem([string]$Name, [string]$GenerationMode) {
    $stem = [IO.Path]::GetFileNameWithoutExtension($Name)
    switch ($GenerationMode) {
        "Iids" { return $stem -replace "_i$", "" }
        "RpcClient" { return $stem -replace "_c$", "" }
        "RpcServer" { return $stem -replace "_s$", "" }
        "Resource" { return $stem -replace "_r$", "" }
        "TypeLib" { return $stem -replace "_t$", "" }
        "WinrtResource" { return $stem -replace "_w$", "" }
        default { return $stem }
    }
}

function Get-CMakeIdlNames([string]$Path, [bool]$AllowEmpty = $false) {
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) {
        throw "CMake source list was not found: $Path"
    }

    $contents = Get-Content -LiteralPath $Path -Raw
    $bodies = @(
        [regex]::Matches($contents, "(?is)list\s*\(\s*APPEND\s+SOURCE(?<body>.*?)\)") |
            ForEach-Object { $_.Groups["body"].Value }
        [regex]::Matches($contents, "(?is)add_idl_headers\s*\((?<body>.*?)\)") |
            ForEach-Object { $_.Groups["body"].Value }
    )

    $names = New-Object System.Collections.Generic.List[string]
    foreach ($body in $bodies) {
        $withoutComments = [regex]::Replace($body, "(?m)#.*$", "")
        foreach ($token in ($withoutComments -split "\s+")) {
            $name = $token.Trim()
            if ($name -match "(?i)\.idl$" -and -not $names.Contains($name)) {
                $names.Add($name)
            }
        }
    }

    if ($names.Count -eq 0 -and -not $AllowEmpty) {
        throw "No IDL files were found in CMake source list: $Path"
    }
    return @($names)
}

function Resolve-CMakeSourcePath(
    [string]$Token,
    [string]$CurrentSourceDirectory,
    [string]$SourceRoot,
    [string]$OutputDirectory
) {
    $value = $Token.Trim().Trim('"')
    $value = $value.Replace('${REACTOS_SOURCE_DIR}', [IO.Path]::GetFullPath($SourceRoot))
    $value = $value.Replace('${CMAKE_CURRENT_SOURCE_DIR}', [IO.Path]::GetFullPath($CurrentSourceDirectory))
    $value = $value.Replace('${CMAKE_CURRENT_BINARY_DIR}', [IO.Path]::GetFullPath($OutputDirectory))

    if ($value -match '\$\{') {
        throw "Unsupported CMake path token '$Token'."
    }
    if ([IO.Path]::IsPathRooted($value)) {
        return [IO.Path]::GetFullPath($value)
    }
    return [IO.Path]::GetFullPath((Join-Path $CurrentSourceDirectory $value))
}

function Get-CMakeRpcFiles([string]$Path) {
    $contents = Get-Content -LiteralPath $Path -Raw
    $files = New-Object System.Collections.Generic.List[psobject]
    foreach ($match in [regex]::Matches(
        $contents,
        "(?is)add_rpc_files?\s*\(\s*(?<type>client|server)\s+(?<body>.*?)\)"
    )) {
        $body = [regex]::Replace($match.Groups["body"].Value, "(?m)#.*$", "")
        $suffix = if ($match.Groups["type"].Value -ieq "client") { "_c" } else { "_s" }
        foreach ($token in ($body -split "\s+")) {
            $name = $token.Trim().Trim('"')
            if ($name -notmatch "(?i)\.idl$") {
                continue
            }
            $files.Add([pscustomobject]@{
                Type = $match.Groups["type"].Value.ToLowerInvariant()
                Name = $name
                OutputName = ([IO.Path]::GetFileNameWithoutExtension($name) + $suffix)
            })
        }
    }
    return @($files)
}

function Get-CMakeIdlArguments([string]$Path) {
    $contents = Get-Content -LiteralPath $Path -Raw
    $arguments = New-Object System.Collections.Generic.List[string]
    foreach ($match in [regex]::Matches($contents, "(?is)set\s*\(\s*IDL_FLAGS\s+(?<body>.*?)\)")) {
        $body = [regex]::Replace($match.Groups["body"].Value, "(?m)#.*$", "")
        foreach ($token in ($body -split "\s+")) {
            $value = $token.Trim()
            if ($value -match "^--[A-Za-z0-9-]+(?:=[^\s\)]+)?$" -and
                -not $arguments.Contains($value)) {
                $arguments.Add($value)
            }
        }
    }
    return @($arguments)
}

function Get-CMakeIncludeDirectories(
    [string]$Path,
    [string]$CurrentSourceDirectory,
    [string]$SourceRoot,
    [string]$OutputDirectory
) {
    $contents = Get-Content -LiteralPath $Path -Raw
    $variables = @{}
    $variables["REACTOS_SOURCE_DIR"] = [IO.Path]::GetFullPath($SourceRoot)
    $variables["CMAKE_CURRENT_SOURCE_DIR"] = [IO.Path]::GetFullPath($CurrentSourceDirectory)
    $variables["CMAKE_CURRENT_BINARY_DIR"] = [IO.Path]::GetFullPath($OutputDirectory)

    foreach ($match in [regex]::Matches($contents, "(?is)set\s*\(\s*(?<name>[A-Za-z_][A-Za-z0-9_]*)\s+(?<value>.*?)\)")) {
        $variables[$match.Groups["name"].Value] = $match.Groups["value"].Value.Trim()
    }

    $directories = New-Object System.Collections.Generic.List[string]
    foreach ($match in [regex]::Matches($contents, "(?is)(?:include_directories|target_include_directories)\s*\((?<body>.*?)\)")) {
        $body = [regex]::Replace($match.Groups["body"].Value, "(?m)#.*$", "")
        foreach ($token in ($body -split "\s+")) {
            $value = $token.Trim()
            if (-not $value -or $value -match "^(?i:BEFORE|AFTER|SYSTEM|PUBLIC|PRIVATE|INTERFACE)$") {
                continue
            }

            for ($pass = 0; $pass -lt 8; $pass++) {
                $expanded = [regex]::Replace($value, "\$\{(?<name>[A-Za-z_][A-Za-z0-9_]*)\}", {
                    param($nested)
                    $name = $nested.Groups["name"].Value
                    if ($variables.ContainsKey($name)) { return $variables[$name] }
                    return $nested.Value
                })
                if ($expanded -eq $value) { break }
                $value = $expanded
            }

            if ($value -match "\$\{") {
                continue
            }
            if ($value -match "\$<") {
                continue
            }

            $resolved = if ([IO.Path]::IsPathRooted($value)) {
                [IO.Path]::GetFullPath($value)
            } else {
                [IO.Path]::GetFullPath((Join-Path $CurrentSourceDirectory $value))
            }
            if (-not $directories.Contains($resolved)) {
                $directories.Add($resolved)
            }
        }
    }
    return @($directories)
}

function Get-CMakeTypeLibNames([string]$Path) {
    $contents = Get-Content -LiteralPath $Path -Raw
    $names = New-Object System.Collections.Generic.List[string]
    foreach ($match in [regex]::Matches($contents, "(?is)add_typelib\s*\((?<body>.*?)\)")) {
        $body = [regex]::Replace($match.Groups["body"].Value, "(?m)#.*$", "")
        foreach ($token in ($body -split "\s+")) {
            $name = $token.Trim()
            if ($name -match "(?i)\.idl$" -and -not $names.Contains($name)) {
                $names.Add($name)
            }
        }
    }
    return @($names)
}

function Get-CMakeRpcProxyNames([string]$Path) {
    $contents = Get-Content -LiteralPath $Path -Raw
    $names = New-Object System.Collections.Generic.List[string]
    foreach ($match in [regex]::Matches($contents, "(?is)add_rpcproxy_files\s*\((?<body>.*?)\)")) {
        $body = [regex]::Replace($match.Groups["body"].Value, "(?m)#.*$", "")
        foreach ($token in ($body -split "\s+")) {
            $name = $token.Trim()
            if ($name -match "(?i)\.idl$" -and -not $names.Contains($name)) {
                $names.Add($name)
            }
        }
    }
    return @($names)
}

function Resolve-IdlFile {
    if ($IdlFile) {
        $resolved = [IO.Path]::GetFullPath($IdlFile)
        if (Test-Path -LiteralPath $resolved -PathType Leaf) {
            return $resolved
        }
        throw "IDL file was not found: $resolved"
    }

    $outputFullPath = [IO.Path]::GetFullPath($OutputFile)
    $generatedFullPath = [IO.Path]::GetFullPath($GeneratedRoot)
    $stem = Get-IdlStem ([IO.Path]::GetFileName($outputFullPath)) $Mode
    $relativeDirectory = ""
    if ($outputFullPath.StartsWith($generatedFullPath, [StringComparison]::OrdinalIgnoreCase)) {
        $relativeOutput = [IO.Path]::GetRelativePath($generatedFullPath, $outputFullPath)
        $relativeDirectory = [IO.Path]::GetDirectoryName($relativeOutput)
    }

    if ($relativeDirectory) {
        $candidate = Join-Path (Join-Path $SourceRoot $relativeDirectory) "$stem.idl"
        if (Test-Path -LiteralPath $candidate -PathType Leaf) {
            return [IO.Path]::GetFullPath($candidate)
        }
    }

    $matches = @(
        Get-ChildItem -LiteralPath $SourceRoot -Recurse -Filter "$stem.idl" -File |
            ForEach-Object { $_.FullName }
    )
    if ($matches.Count -eq 1) {
        return $matches[0]
    }
    if ($matches.Count -eq 0) {
        throw "Unable to locate an IDL source for '$OutputFile' (stem '$stem')."
    }

    if ($relativeDirectory) {
        $suffix = $relativeDirectory.Replace("/", [IO.Path]::DirectorySeparatorChar).Replace("\", [IO.Path]::DirectorySeparatorChar)
        $preferred = @($matches | Where-Object {
            $_.StartsWith((Join-Path $SourceRoot $suffix), [StringComparison]::OrdinalIgnoreCase)
        })
        if ($preferred.Count -eq 1) {
            return $preferred[0]
        }
    }

    throw "Multiple IDL sources match '$stem.idl' for '$OutputFile': $($matches -join ', ')"
}

function Add-HeaderCompatibilityGuard([string]$Path) {
    if ([IO.Path]::GetFileName($Path) -ieq "uiautomationclient.h") {
        if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) {
            return
        }
        $headerText = [IO.File]::ReadAllText($Path)
        $compatibilityGuard = "#ifndef __uiautomationclient_h__`r`n#define __uiautomationclient_h__`r`n#endif`r`n`r`n"
        if ($headerText -notmatch "(?m)^\s*#define\s+__uiautomationclient_h__\b") {
            [IO.File]::WriteAllText(
                $Path,
                $compatibilityGuard + $headerText,
                [Text.UTF8Encoding]::new($false))
        }
        return
    }

    $compatibilityMacro = switch ([IO.Path]::GetFileName($Path).ToLowerInvariant()) {
        "oaidl.h" { "__oaidl_h__" }
        "hstring.h" { "__hstring_h__" }
        default { return }
    }
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) {
        return
    }

    $headerText = [IO.File]::ReadAllText($Path)
    if ($headerText -notmatch "(?m)^\s*#define\s+$([regex]::Escape($compatibilityMacro))\b") {
        $compatibilityGuard = "#ifndef $compatibilityMacro`r`n#define $compatibilityMacro`r`n#endif`r`n`r`n"
        [IO.File]::WriteAllText(
            $Path,
            $compatibilityGuard + $headerText,
            [Text.UTF8Encoding]::new($false))
    }
}

function Set-RpcHeaderInterfaceGuard([string]$Path, [string]$Suffix) {
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) {
        return
    }

    $headerText = [IO.File]::ReadAllText($Path)
    $guardMatches = [regex]::Matches(
        $headerText,
        "(?m)^(?<indent>\s*#(?:ifndef|define)\s+)(?<guard>[A-Za-z_][A-Za-z0-9_]*_INTERFACE_DEFINED__)\s*$")
    if ($guardMatches.Count -eq 0) {
        return
    }

    $guards = New-Object System.Collections.Generic.List[string]
    foreach ($match in $guardMatches) {
        $guard = $match.Groups["guard"].Value
        if (-not $guards.Contains($guard)) {
            $guards.Add($guard)
        }
    }
    foreach ($guard in $guards) {
        $newGuard = "${guard}_$Suffix"
        $headerText = $headerText -replace (
            "(?m)^(\s*#(?:ifndef|define)\s+)" +
            [regex]::Escape($guard) +
            "(\s*)$"
        ), ('$1' + $newGuard + '$2')
    }
    [IO.File]::WriteAllText(
        $Path,
        $headerText,
        [Text.UTF8Encoding]::new($false))
}

function Normalize-RpcProxySymbols([string]$Path, [string]$ProxyStem) {
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) {
        return
    }

    $sourceText = [IO.File]::ReadAllText($Path)
    $proxyFileInfoPattern =
        "(?<prefix>[A-Za-z_][A-Za-z0-9_]*)" +
        [regex]::Escape($ProxyStem) +
        "_ProxyFileInfo\b"
    $match = [regex]::Match($sourceText, $proxyFileInfoPattern)
    if (-not $match.Success) {
        return
    }

    $prefix = $match.Groups["prefix"].Value
    if ($prefix) {
        $sourceText = $sourceText.Replace($prefix, "")
        [IO.File]::WriteAllText(
            $Path,
            $sourceText,
            [Text.UTF8Encoding]::new($false))
    }
}

function Get-RpcFileArguments([string]$ResolvedIdl, [string]$RpcType = "") {
    if (-not $RpcType) {
        if ($Mode -ne "RpcClient" -and $Mode -ne "RpcServer") {
            return @()
        }
        $RpcType = if ($Mode -eq "RpcClient") { "client" } else { "server" }
    }

    $rpcType = $RpcType.ToLowerInvariant()
    $idlName = [IO.Path]::GetFileName($ResolvedIdl)
    $cmakeFiles = if ($CMakeListFile) {
        @([IO.Path]::GetFullPath($CMakeListFile))
    } else {
        @(Get-ChildItem -LiteralPath $SourceRoot -Recurse -Filter "CMakeLists.txt" -File |
            ForEach-Object { $_.FullName })
    }

    foreach ($cmakePath in $cmakeFiles) {
        $contents = Get-Content -LiteralPath $cmakePath -Raw
        $arguments = New-Object System.Collections.Generic.List[string]
        $rpcMatch = $false
        foreach ($match in [regex]::Matches($contents, "(?is)add_rpc_files?\s*\(\s*$rpcType\s+(?<body>.*?)\)")) {
            $body = [regex]::Replace($match.Groups["body"].Value, "(?m)#.*$", "")
            if ($body -notmatch [regex]::Escape($idlName)) {
                continue
            }
            $rpcMatch = $true
            $prefixAll = [regex]::Match($body, "(?is)\bPREFIX_ALL\s+(?<value>[^\s\)]+)")
            if ($prefixAll.Success) {
                $arguments.Add("--prefix-all=$($prefixAll.Groups['value'].Value)")
                break
            }
            foreach ($name in @("PREFIX_CLIENT", "PREFIX_SERVER")) {
                $prefix = [regex]::Match(
                    $body,
                    "(?is)\b$name\s+(?<value>[^\s\)]+)")
                if ($prefix.Success) {
                    $arguments.Add("--$($name.ToLowerInvariant().Replace('_', '-'))=$($prefix.Groups['value'].Value)")
                }
            }
        }

        if ($rpcMatch) {
            foreach ($match in [regex]::Matches(
                $contents,
                "(?im)(?:^|\s)--prefix-(?:all|client|server)=[^\s\)]+")) {
                $argument = $match.Value.Trim()
                if (-not $arguments.Contains($argument)) {
                    $arguments.Add($argument)
                }
            }
            if ($arguments.Count -gt 0) {
                return @($arguments)
            }
        }
    }
    return @()
}

if (-not (Test-Path -LiteralPath $WidlTool -PathType Leaf)) {
    throw "WIDL was not found: $WidlTool"
}

$includes = Get-IncludeDirectories $IncludeDirectoryList
$arguments = @("-nostdinc", "-D__REACTOS__")
foreach ($define in (Get-IncludeDirectories $DefineList)) {
    $arguments += "-D$define"
}
switch ($Architecture) {
    "Win32" { $arguments += @("-m32", "--win32", "-b", "i386-x-y") }
    "x64" { $arguments += @("-m64", "--win64", "-b", "amd64-x-y") }
    "ARM64" { $arguments += @("-b", "arm64-x-y") }
}
foreach ($include in $includes) {
    $arguments += "-I$include"
}
$libraryArguments = @()
foreach ($library in (Get-IncludeDirectories $LibraryDirectoryList)) {
    $libraryArguments += "-L$library"
}

function Invoke-Widl([string[]]$WidlArguments, [string]$WorkingDirectory) {
    Push-Location -LiteralPath $WorkingDirectory
    try {
        & $WidlTool @WidlArguments
        if ($LASTEXITCODE -ne 0) {
            throw "WIDL exited with code $LASTEXITCODE for '$resolvedIdl'."
        }
    }
    finally {
        Pop-Location
    }
}

function Get-WidlInputFile([string]$Path) {
    if ([IO.Path]::GetFileName($Path) -ine "netdfs.idl") {
        return $Path
    }

    $compatibilityDirectory = Join-Path $GeneratedRoot "ReactOSWidl\Compatibility"
    $compatibilityPath = Join-Path $compatibilityDirectory "netdfs.idl"
    New-Item -ItemType Directory -Force -Path $compatibilityDirectory | Out-Null
    $contents = [IO.File]::ReadAllText($Path)
    $contents = $contents -replace `
        "(?m)^\s*typedef\s+DWORD\s+NET_API_STATUS\s*;\s*$", `
        "/* NET_API_STATUS is provided by ms-dtyp.idl. */"
    [IO.File]::WriteAllText(
        $compatibilityPath,
        $contents,
        [Text.UTF8Encoding]::new($false))
    return $compatibilityPath
}

if ($Mode -eq "SharedHeaders" -or $Mode -eq "ProjectHeaders") {
    if (-not $CMakeListFile -or -not $SourceDirectory) {
        throw "$Mode generation requires CMakeListFile and SourceDirectory."
    }
    if ($Mode -eq "SharedHeaders" -and -not $OutputDirectory) {
        throw "SharedHeaders generation requires OutputDirectory."
    }
    if ($Mode -eq "ProjectHeaders" -and -not $OutputDirectory) {
        $resolvedSourceDirectory = [IO.Path]::GetFullPath($SourceDirectory)
        $relativeDirectory = [IO.Path]::GetRelativePath(
            [IO.Path]::GetFullPath($SourceRoot),
            $resolvedSourceDirectory)
        $OutputDirectory = Join-Path $GeneratedRoot $relativeDirectory
    }

    $resolvedSourceDirectory = [IO.Path]::GetFullPath($SourceDirectory)
    $resolvedOutputDirectory = [IO.Path]::GetFullPath($OutputDirectory)
    $idlNames = Get-CMakeIdlNames $CMakeListFile ($Mode -eq "ProjectHeaders")
    New-Item -ItemType Directory -Force -Path $resolvedOutputDirectory | Out-Null
    $projectArguments = $arguments + @(
        "-I$resolvedSourceDirectory"
        "-I$resolvedOutputDirectory"
        (Get-CMakeIncludeDirectories $CMakeListFile $resolvedSourceDirectory $SourceRoot $resolvedOutputDirectory |
            ForEach-Object { "-I$_" })
    )
    $projectLibraryArguments = @($libraryArguments)
    $projectIdlArguments = @(Get-CMakeIdlArguments $CMakeListFile)
    $projectWidlArguments = $projectArguments + $projectLibraryArguments + $projectIdlArguments

    foreach ($idlName in $idlNames) {
        $resolvedIdl = Resolve-CMakeSourcePath `
            $idlName `
            $resolvedSourceDirectory `
            $SourceRoot `
            $resolvedOutputDirectory
        if (-not (Test-Path -LiteralPath $resolvedIdl -PathType Leaf)) {
            throw "IDL file from '$CMakeListFile' was not found: $resolvedIdl"
        }
        $widlInput = Get-WidlInputFile $resolvedIdl

        $resolvedOutput = [IO.Path]::ChangeExtension(
            (Join-Path $resolvedOutputDirectory $idlName),
            ".h")
        New-Item -ItemType Directory -Force -Path (Split-Path -Parent $resolvedOutput) | Out-Null
        Invoke-Widl ($projectWidlArguments + @("-Oicf", "-h", "-o", $resolvedOutput, $widlInput)) $resolvedSourceDirectory
        Add-HeaderCompatibilityGuard $resolvedOutput
    }
    foreach ($rpcFile in (Get-CMakeRpcFiles $CMakeListFile)) {
        $resolvedIdl = Resolve-CMakeSourcePath `
            $rpcFile.Name `
            $resolvedSourceDirectory `
            $SourceRoot `
            $resolvedOutputDirectory
        if (-not (Test-Path -LiteralPath $resolvedIdl -PathType Leaf)) {
            throw "RPC IDL file from '$CMakeListFile' was not found: $resolvedIdl"
        }

        $resolvedOutput = Join-Path $resolvedOutputDirectory "$($rpcFile.OutputName).h"
        New-Item -ItemType Directory -Force -Path (Split-Path -Parent $resolvedOutput) | Out-Null
        $rpcArguments = Get-RpcFileArguments $resolvedIdl $rpcFile.Type
        $widlInput = Get-WidlInputFile $resolvedIdl
        Invoke-Widl (
            $projectWidlArguments + $rpcArguments + @(
                "-Oif", "-h",
                "-o", $resolvedOutput,
                $widlInput
            )
        ) (Split-Path -Parent $resolvedIdl)
        Set-RpcHeaderInterfaceGuard $resolvedOutput $rpcFile.Type.ToUpperInvariant()
    }
    foreach ($typeLibName in (Get-CMakeTypeLibNames $CMakeListFile)) {
        $resolvedIdl = Resolve-CMakeSourcePath `
            $typeLibName `
            $resolvedSourceDirectory `
            $SourceRoot `
            $resolvedOutputDirectory
        if (-not (Test-Path -LiteralPath $resolvedIdl -PathType Leaf)) {
            throw "Type library IDL from '$CMakeListFile' was not found: $resolvedIdl"
        }
        $widlInput = Get-WidlInputFile $resolvedIdl

        # add_typelib() writes the typelib to CMAKE_CURRENT_BINARY_DIR using
        # only the IDL basename, even when the IDL is in a subdirectory.
        $resolvedOutput = [IO.Path]::ChangeExtension(
            (Join-Path $resolvedOutputDirectory ([IO.Path]::GetFileName($typeLibName))),
            ".tlb")
        New-Item -ItemType Directory -Force -Path (Split-Path -Parent $resolvedOutput) | Out-Null
        Invoke-Widl ($projectWidlArguments + @("-t", "-o", $resolvedOutput, $widlInput)) $resolvedSourceDirectory
    }

    $rpcProxyIdlFiles = New-Object System.Collections.Generic.List[string]
    $rpcProxyIdlNames = New-Object System.Collections.Generic.List[string]
    foreach ($rpcProxyName in (Get-CMakeRpcProxyNames $CMakeListFile)) {
        $resolvedIdl = [IO.Path]::GetFullPath((Join-Path $resolvedSourceDirectory $rpcProxyName))
        if (-not (Test-Path -LiteralPath $resolvedIdl -PathType Leaf)) {
            throw "RPC proxy IDL from '$CMakeListFile' was not found: $resolvedIdl"
        }

        $proxyStem = [IO.Path]::GetFileNameWithoutExtension($rpcProxyName)
        $proxyC = Join-Path $resolvedOutputDirectory "${proxyStem}_p.c"
        $proxyH = Join-Path $resolvedOutputDirectory "${proxyStem}_p.h"
        $proxyHeaderName = [IO.Path]::GetFileName($proxyH)
        New-Item -ItemType Directory -Force -Path $resolvedOutputDirectory | Out-Null
        Invoke-Widl (
            $projectWidlArguments + @(
                "-Oicf", "-p",
                "-o", $proxyC,
                "-H", $proxyHeaderName,
                $rpcProxyName
            )
        ) $resolvedSourceDirectory
        Normalize-RpcProxySymbols $proxyC $proxyStem
        Invoke-Widl (
            $projectWidlArguments + @(
                "-Oicf", "-h",
                "-o", $proxyH,
                $rpcProxyName
            )
        ) $resolvedSourceDirectory
        $rpcProxyIdlFiles.Add($resolvedIdl)
        $rpcProxyIdlNames.Add($rpcProxyName)
    }

    if ($rpcProxyIdlFiles.Count -gt 0) {
        $dlldata = Join-Path $resolvedOutputDirectory "proxy.dlldata.c"
        Invoke-Widl (
            $projectWidlArguments + @(
                "-Oicf", "--dlldata-only",
                "-o", $dlldata
            ) + @($rpcProxyIdlNames)
        ) $resolvedSourceDirectory
    }
    exit 0
}

$resolvedOutput = [IO.Path]::GetFullPath($OutputFile)
$outputDirectory = Split-Path -Parent $resolvedOutput
New-Item -ItemType Directory -Force -Path $outputDirectory | Out-Null
$resolvedIdl = Resolve-IdlFile
$widlInput = Get-WidlInputFile $resolvedIdl
$rpcFileArguments = Get-RpcFileArguments $resolvedIdl

switch ($Mode) {
    "Header" {
        Invoke-Widl ($arguments + @("-Oicf", "-h", "-o", $resolvedOutput, $widlInput)) (Split-Path -Parent $resolvedIdl)
    }
    "Iids" {
        $uuidScript = Join-Path $PSScriptRoot "GenerateReactOSUuidIids.ps1"
        & pwsh.exe -NoLogo -NoProfile -ExecutionPolicy Bypass -File $uuidScript `
            -WidlTool $WidlTool -IdlFile $widlInput -OutputFile $resolvedOutput `
            -Architecture $Architecture -Mode Iids -IncludeDirectoryList $IncludeDirectoryList
        if ($LASTEXITCODE -ne 0) {
            throw "UUID generation failed for '$resolvedIdl'."
        }
    }
    "RpcClient" {
        if (-not $HeaderFile) {
            throw "RpcClient generation requires HeaderFile."
        }
        $resolvedHeader = [IO.Path]::GetFullPath($HeaderFile)
        New-Item -ItemType Directory -Force -Path (Split-Path -Parent $resolvedHeader) | Out-Null
        Invoke-Widl ($arguments + $rpcFileArguments + @("-Oif", "-c", "-o", $resolvedOutput, "-H", $resolvedHeader, $widlInput)) (Split-Path -Parent $resolvedIdl)
        Invoke-Widl ($arguments + $rpcFileArguments + @("-Oif", "-h", "-o", $resolvedHeader, $widlInput)) (Split-Path -Parent $resolvedIdl)
        Set-RpcHeaderInterfaceGuard $resolvedHeader "CLIENT"
    }
    "RpcServer" {
        if (-not $HeaderFile) {
            throw "RpcServer generation requires HeaderFile."
        }
        $resolvedHeader = [IO.Path]::GetFullPath($HeaderFile)
        New-Item -ItemType Directory -Force -Path (Split-Path -Parent $resolvedHeader) | Out-Null
        Invoke-Widl ($arguments + $rpcFileArguments + @("-Oif", "-s", "-o", $resolvedOutput, "-H", $resolvedHeader, $widlInput)) (Split-Path -Parent $resolvedIdl)
        Invoke-Widl ($arguments + $rpcFileArguments + @("-Oif", "-h", "-o", $resolvedHeader, $widlInput)) (Split-Path -Parent $resolvedIdl)
        Set-RpcHeaderInterfaceGuard $resolvedHeader "SERVER"
    }
    "Resource" {
        Invoke-Widl ($arguments + @("-Oicf", "-r", "-o", $resolvedOutput, $widlInput)) (Split-Path -Parent $resolvedIdl)
    }
    "WinrtResource" {
        Invoke-Widl ($arguments + @("-Oicf", "-r", "-winrt", "-o", $resolvedOutput, $widlInput)) (Split-Path -Parent $resolvedIdl)
    }
    "TypeLib" {
        Invoke-Widl ($arguments + $libraryArguments + @("-t", "-o", $resolvedOutput, $widlInput)) (Split-Path -Parent $resolvedIdl)
    }
}
