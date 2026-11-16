param(
    [Parameter(Mandatory = $true)]
    [string]$SourceDirectory,
    [Parameter(Mandatory = $true)]
    [string]$PatchFile,
    [Parameter(Mandatory = $true)]
    [string]$StateFile
)

$ErrorActionPreference = "Stop"

$sourceDirectory = (Resolve-Path -LiteralPath $SourceDirectory).Path
$patchFile = (Resolve-Path -LiteralPath $PatchFile).Path
$stateDirectory = Split-Path -Parent $StateFile
$patchHash = (Get-FileHash -LiteralPath $patchFile -Algorithm SHA256).Hash

New-Item -ItemType Directory -Force -Path $stateDirectory | Out-Null

$sourceCommit = (& git -C $sourceDirectory rev-parse HEAD).Trim()
if ($LASTEXITCODE -ne 0) {
    throw "Unable to determine the ReactOS source commit in '$sourceDirectory'."
}

$state = "$sourceCommit`n$patchHash"
function Test-Patch([switch]$Reverse) {
    $arguments = @(
        "-C", $sourceDirectory,
        "apply",
        "--check",
        "--whitespace=nowarn"
    )
    if ($Reverse) {
        $arguments += "--reverse"
    }
    $arguments += $patchFile
    & git @arguments *> $null
    return $LASTEXITCODE -eq 0
}

if ((Test-Path -LiteralPath $StateFile) -and
    ((Get-Content -LiteralPath $StateFile -Raw) -eq $state) -and
    (Test-Patch -Reverse)) {
    return
}

if (Test-Patch) {
    & git -C $sourceDirectory apply --whitespace=nowarn $patchFile
    if ($LASTEXITCODE -ne 0) {
        throw "Failed to apply ReactOS patch '$patchFile'."
    }
}
elseif (-not (Test-Patch -Reverse)) {
    throw "ReactOS patch '$patchFile' is neither applicable nor already applied."
}

[IO.File]::WriteAllText($StateFile, $state)
