param(
    [Parameter(Mandatory = $true)]
    [string]$SourceRoot,
    [Parameter(Mandatory = $true)]
    [string]$GeneratedRoot,
    [Parameter(Mandatory = $true)]
    [string]$CMakeListFile,
    [Parameter(Mandatory = $true)]
    [string]$SourceDirectory,
    [string]$OutputDirectory = ""
)

$ErrorActionPreference = "Stop"

function Expand-CMakeValue([string]$Value, [hashtable]$Variables) {
    for ($pass = 0; $pass -lt 8; $pass++) {
        $expanded = [regex]::Replace($Value, "\$\{(?<name>[A-Za-z_][A-Za-z0-9_]*)\}", {
            param($match)
            $name = $match.Groups["name"].Value
            if ($Variables.ContainsKey($name)) { return $Variables[$name] }
            return $match.Value
        })
        if ($expanded -eq $Value) { break }
        $Value = $expanded
    }
    return $Value
}

function Resolve-CMakePath([string]$Value, [string]$BaseDirectory, [string]$GeneratedDirectory) {
    $value = $Value.Trim().Trim('"', "'")
    if (-not $value -or $value -match "\$\{") {
        return $null
    }
    if ([IO.Path]::IsPathRooted($value)) {
        return [IO.Path]::GetFullPath($value)
    }
    return [IO.Path]::GetFullPath((Join-Path $BaseDirectory $value))
}

function Convert-ToUtf16([string]$InputFile, [string]$OutputFile, [bool]$WithBom) {
    if (-not (Test-Path -LiteralPath $InputFile -PathType Leaf)) {
        throw "UTF-16 source was not found: $InputFile"
    }
    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $OutputFile) | Out-Null
    $reader = [IO.StreamReader]::new($InputFile, $true)
    try {
        $text = $reader.ReadToEnd()
    }
    finally {
        $reader.Dispose()
    }
    $encoding = [Text.UnicodeEncoding]::new($false, $WithBom)
    [IO.File]::WriteAllText($OutputFile, $text, $encoding)
}

$resolvedSourceRoot = [IO.Path]::GetFullPath($SourceRoot)
$resolvedSourceDirectory = [IO.Path]::GetFullPath($SourceDirectory)
if (-not $OutputDirectory) {
    $relativeDirectory = [IO.Path]::GetRelativePath($resolvedSourceRoot, $resolvedSourceDirectory)
    $OutputDirectory = Join-Path $GeneratedRoot $relativeDirectory
}
$resolvedOutputDirectory = [IO.Path]::GetFullPath($OutputDirectory)

$variables = @{
    "REACTOS_SOURCE_DIR" = $resolvedSourceRoot
    "CMAKE_CURRENT_SOURCE_DIR" = $resolvedSourceDirectory
    "CMAKE_CURRENT_BINARY_DIR" = $resolvedOutputDirectory
}
$contents = Get-Content -LiteralPath $CMakeListFile -Raw
foreach ($match in [regex]::Matches($contents, "(?is)set\s*\(\s*(?<name>[A-Za-z_][A-Za-z0-9_]*)\s+(?<value>.*?)\)")) {
    $variables[$match.Groups["name"].Value] = $match.Groups["value"].Value.Trim()
}

foreach ($match in [regex]::Matches($contents, "(?is)utf16le_convert\s*\((?<body>.*?)\)")) {
    $body = [regex]::Replace($match.Groups["body"].Value, "(?m)#.*$", "")
    $tokens = @($body -split "\s+" | ForEach-Object { $_.Trim() } | Where-Object { $_ })
    if ($tokens.Count -lt 2) {
        continue
    }

    $inputValue = Expand-CMakeValue $tokens[0] $variables
    $outputValue = Expand-CMakeValue $tokens[1] $variables
    $inputFile = Resolve-CMakePath $inputValue $resolvedSourceDirectory $resolvedOutputDirectory
    $outputFile = Resolve-CMakePath $outputValue $resolvedOutputDirectory $resolvedOutputDirectory
    if (-not $inputFile -or -not $outputFile) {
        continue
    }

    if ($outputValue.StartsWith($resolvedOutputDirectory, [StringComparison]::OrdinalIgnoreCase)) {
        $outputFile = [IO.Path]::GetFullPath($outputValue)
    }
    $withBom = $tokens -notcontains "nobom"
    Convert-ToUtf16 $inputFile $outputFile $withBom
}

# The CMake files commonly compute the destination through loop-local
# variables (for example, ${_converted_file}).  Those variables are not
# available to this lightweight parser, but the generated name is always the
# source basename with "_utf16" inserted before the extension.
if ($contents -match "(?i)\butf16le_convert\s*\(") {
    $textFileRoot = Join-Path $resolvedSourceDirectory "textfiles"
    if (Test-Path -LiteralPath $textFileRoot -PathType Container) {
        foreach ($inputFile in (Get-ChildItem -LiteralPath $textFileRoot -Recurse -File -Filter "*.INI")) {
            $relativeFile = [IO.Path]::GetRelativePath($textFileRoot, $inputFile.FullName)
            $relativeDirectory = [IO.Path]::GetDirectoryName($relativeFile)
            $fileName = [IO.Path]::GetFileNameWithoutExtension($relativeFile)
            $outputName = $fileName + "_utf16.INI"
            $outputFile = if ($relativeDirectory) {
                Join-Path (Join-Path $resolvedOutputDirectory "textfiles") (Join-Path $relativeDirectory $outputName)
            } else {
                Join-Path (Join-Path $resolvedOutputDirectory "textfiles") $outputName
            }
            if (-not (Test-Path -LiteralPath $outputFile -PathType Leaf)) {
                Convert-ToUtf16 $inputFile.FullName $outputFile $true
            }
        }
    }
}
