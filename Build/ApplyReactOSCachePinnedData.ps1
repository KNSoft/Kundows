param(
    [Parameter(Mandatory=$true)][string]$SourceDirectory,
    [Parameter(Mandatory=$true)][string]$StateFile
)
$ErrorActionPreference = "Stop"
$source = (Resolve-Path -LiteralPath $SourceDirectory).Path
$sha = [Security.Cryptography.SHA256]::Create()
try {
    $identity = [BitConverter]::ToString($sha.ComputeHash(
        [Text.Encoding]::UTF8.GetBytes($source.ToUpperInvariant()))).Replace("-", "")
} finally { $sha.Dispose() }

# UP and MP kernel projects can prepare concurrently on a clean source tree.
$mutex = [Threading.Mutex]::new($false, "Local\Kundows.CachePinnedData.$identity")
$acquired = $false
try {
    try { $acquired = $mutex.WaitOne(120000) }
    catch [Threading.AbandonedMutexException] { $acquired = $true }
    if (-not $acquired) { throw "Timed out waiting for the cache-pinning source patch lock" }
    & "$PSScriptRoot\ApplyReactOSPatches.ps1" -SourceDirectory $source `
        -PatchFile "$PSScriptRoot\Patches\ReactOS-cc-pinned-dirty.patch" -StateFile $StateFile
} finally {
    if ($acquired) { $mutex.ReleaseMutex() }
    $mutex.Dispose()
}
