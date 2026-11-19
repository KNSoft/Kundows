[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$OutputFile,
    [Parameter(Mandatory = $true)]
    [string]$FirstInputFile,
    [Parameter(Mandatory = $true)]
    [string]$SecondInputFile
)

$ErrorActionPreference = "Stop"

foreach ($sourceFile in @($FirstInputFile, $SecondInputFile)) {
    if (-not (Test-Path -LiteralPath $sourceFile -PathType Leaf)) {
        throw "Concatenation input was not found: $sourceFile"
    }
}

New-Item -ItemType Directory -Force -Path (Split-Path -Parent $OutputFile) | Out-Null
$outputStream = [IO.File]::Open(
    $OutputFile,
    [IO.FileMode]::Create,
    [IO.FileAccess]::Write,
    [IO.FileShare]::None
)
try {
    foreach ($sourceFile in @($FirstInputFile, $SecondInputFile)) {
        $stream = [IO.File]::OpenRead($sourceFile)
        try {
            $stream.CopyTo($outputStream)
        }
        finally {
            $stream.Dispose()
        }
    }
}
finally {
    $outputStream.Dispose()
}
