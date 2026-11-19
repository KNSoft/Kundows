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

foreach ($input in @($FirstInputFile, $SecondInputFile)) {
    if (-not (Test-Path -LiteralPath $input -PathType Leaf)) {
        throw "Concatenation input was not found: $input"
    }
}

New-Item -ItemType Directory -Force -Path (Split-Path -Parent $OutputFile) | Out-Null
$output = [IO.File]::Open(
    $OutputFile,
    [IO.FileMode]::Create,
    [IO.FileAccess]::Write,
    [IO.FileShare]::None
)
try {
    foreach ($input in @($FirstInputFile, $SecondInputFile)) {
        $stream = [IO.File]::OpenRead($input)
        try {
            $stream.CopyTo($output)
        }
        finally {
            $stream.Dispose()
        }
    }
}
finally {
    $output.Dispose()
}
