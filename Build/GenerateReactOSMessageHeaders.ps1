param(
    [Parameter(Mandatory = $true)]
    [string]$WindowsSdkDir,
    [string]$SourceFile = "",
    [string]$OutputDirectory = "",
    [ValidateSet("ANSI", "UNICODE", "Project")]
    [string]$Mode = "ANSI",
    [string]$CMakeListFile = "",
    [string]$SourceDirectory = "",
    [string]$SourceRoot = "",
    [string]$GeneratedRoot = ""
)

$ErrorActionPreference = "Stop"

$mc = Get-ChildItem -LiteralPath (Join-Path $WindowsSdkDir.TrimEnd("\", "/") "bin") -Filter "mc.exe" -File -Recurse |
    Sort-Object FullName -Descending |
    Select-Object -First 1

if ($null -eq $mc) {
    throw "Unable to locate mc.exe below $WindowsSdkDir"
}

function Invoke-MessageGeneration([string]$MessageSource, [string]$Destination, [string]$MessageMode) {
    if (-not (Test-Path -LiteralPath $MessageSource -PathType Leaf)) {
        throw "Message source was not found: $MessageSource"
    }

    New-Item -ItemType Directory -Force -Path $Destination | Out-Null
    $sourceName = [IO.Path]::GetFileNameWithoutExtension($MessageSource)
    $convertedSource = Join-Path $Destination "$sourceName.mc"

    # ReactOS stores .mc files as UTF-8, while the MSVC message compiler requires
    # -u for UTF-16LE input without a BOM. This mirrors CMake's utf16le_convert()
    # call with the nobom option.
    $reader = [IO.StreamReader]::new((Resolve-Path -LiteralPath $MessageSource), $true)
    try {
        $sourceText = $reader.ReadToEnd()
    }
    finally {
        $reader.Dispose()
    }

    # The frozen ReactOS sources use semicolon comment lines. Recent mc.exe
    # versions copy a leading semicolon comment into the generated header without
    # turning it into a C comment, so discard those comments before invocation.
    $sourceText = [regex]::Replace($sourceText, "(?m)^\s*;.*(?:\r?\n|$)", "")
    [IO.File]::WriteAllText(
        $convertedSource,
        $sourceText,
        [Text.UnicodeEncoding]::new($false, $false))

    $messageEncoding = if ($MessageMode -eq "UNICODE") { "-U" } else { "-A" }
    & $mc.FullName -u $messageEncoding -b -h $Destination -r $Destination $convertedSource
    if ($LASTEXITCODE -ne 0) {
        throw "mc.exe failed with exit code $LASTEXITCODE"
    }

    $header = Join-Path $Destination "$sourceName.h"
    $resource = Join-Path $Destination "$sourceName.rc"
    if (-not (Test-Path -LiteralPath $header) -or -not (Test-Path -LiteralPath $resource)) {
        throw "mc.exe did not generate the expected outputs: $header and $resource"
    }
}

if ($Mode -eq "Project") {
    if (-not $CMakeListFile -or -not $SourceDirectory -or -not $SourceRoot -or -not $GeneratedRoot) {
        throw "Project message generation requires CMakeListFile, SourceDirectory, SourceRoot and GeneratedRoot."
    }
    if (-not $OutputDirectory) {
        $relativeDirectory = [IO.Path]::GetRelativePath(
            [IO.Path]::GetFullPath($SourceRoot),
            [IO.Path]::GetFullPath($SourceDirectory))
        $OutputDirectory = Join-Path $GeneratedRoot $relativeDirectory
    }

    $contents = Get-Content -LiteralPath $CMakeListFile -Raw
    foreach ($match in [regex]::Matches($contents, "(?is)add_message_headers\s*\((?<body>.*?)\)")) {
        $body = [regex]::Replace($match.Groups["body"].Value, "(?m)#.*$", "")
        $tokens = @($body -split "\s+" | ForEach-Object { $_.Trim() } | Where-Object { $_ })
        $messageMode = "ANSI"
        foreach ($token in $tokens) {
            if ($token -match "^(?i:ANSI|UNICODE)$") {
                $messageMode = $token.ToUpperInvariant()
                continue
            }
            if ($token -match "(?i)\.mc$") {
                $messageSource = [IO.Path]::GetFullPath((Join-Path $SourceDirectory $token))
                Invoke-MessageGeneration $messageSource ([IO.Path]::GetFullPath($OutputDirectory)) $messageMode
            }
        }
    }
    exit 0
}

if (-not $SourceFile -or -not $OutputDirectory) {
    throw "Direct message generation requires SourceFile and OutputDirectory."
}
Invoke-MessageGeneration ([IO.Path]::GetFullPath($SourceFile)) ([IO.Path]::GetFullPath($OutputDirectory)) $Mode
