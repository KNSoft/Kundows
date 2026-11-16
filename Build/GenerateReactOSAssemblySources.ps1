param(
    [Parameter(Mandatory = $true)]
    [string]$Compiler,
    [Parameter(Mandatory = $true)]
    [string]$SourceListFile,
    [Parameter(Mandatory = $true)]
    [string]$OutputListFile,
    [Parameter(Mandatory = $true)]
    [string]$IncludeListFile,
    [Parameter(Mandatory = $true)]
    [string]$DefinitionListFile
)

$ErrorActionPreference = "Stop"

function Read-ListFile([string]$Path) {
    if (-not (Test-Path -LiteralPath $Path)) {
        return @()
    }

    return @(
        Get-Content -LiteralPath $Path |
            ForEach-Object { $_ -split ";" } |
            ForEach-Object { $_.Trim() } |
            Where-Object { $_.Length -gt 0 }
    )
}

if (-not (Test-Path -LiteralPath $Compiler)) {
    throw "MSVC compiler was not found: $Compiler"
}

$sourceFiles = Read-ListFile $SourceListFile |
    Where-Object { [IO.Path]::GetExtension($_) -in @(".s", ".S") }
$outputFiles = Read-ListFile $OutputListFile |
    Where-Object { [IO.Path]::GetExtension($_) -ieq ".asm" }
$includeDirectories = Read-ListFile $IncludeListFile
$definitions = Read-ListFile $DefinitionListFile

foreach ($sourceFile in $sourceFiles) {
    $sourceName = [IO.Path]::GetFileNameWithoutExtension($sourceFile)
    $outputCandidates = @(
        $outputFiles | Where-Object {
            $outputName = [IO.Path]::GetFileNameWithoutExtension($_)
            $outputName.StartsWith("${sourceName}_", [StringComparison]::OrdinalIgnoreCase)
        }
    )

    if ($outputCandidates.Count -eq 0) {
        continue
    }

    if ($outputCandidates.Count -gt 1) {
        # Some frozen ReactOS source names are prefixes of another source
        # (seh.s and seh_prolog.s).  The generated name for the exact source
        # is the shortest matching candidate; retain the hard failure for
        # genuinely ambiguous equal-length mappings.
        $shortestLength = ($outputCandidates | ForEach-Object {
            [IO.Path]::GetFileNameWithoutExtension($_).Length
        } | Measure-Object -Minimum).Minimum
        $outputCandidates = @(
            $outputCandidates | Where-Object {
                [IO.Path]::GetFileNameWithoutExtension($_).Length -eq $shortestLength
            }
        )
    }

    if ($outputCandidates.Count -ne 1) {
        throw "Unable to uniquely map assembly source '$sourceFile': $($outputCandidates -join ', ')"
    }

    $outputFile = $outputCandidates[0]
    $outputDirectory = Split-Path -Parent $outputFile
    New-Item -ItemType Directory -Force -Path $outputDirectory | Out-Null

    $arguments = @("/nologo", "/X")
    foreach ($includeDirectory in $includeDirectories) {
        $arguments += "/I$includeDirectory"
    }
    foreach ($definition in $definitions) {
        $arguments += "/D$definition"
    }
    $arguments += @("/D__ASM__", "/D_USE_ML", "/EP", "/c", $sourceFile)

    & $Compiler @arguments > $outputFile
    if ($LASTEXITCODE -ne 0) {
        throw "cl.exe failed with exit code $LASTEXITCODE while preprocessing '$sourceFile'"
    }
}
