# A case supplies either a source file and C declarations, or a local binding script.
# Compile the selected checkout's bodies; never substitute copies from the test tree.
param([string]$Fixture)
$ErrorActionPreference = 'Stop'
Set-StrictMode -Version 3
$Program = (Resolve-Path -LiteralPath $Fixture).Path
$Source = [IO.Path]::GetFullPath("$PSScriptRoot\..\..\..\Source")
$Out = $env:KW_CASE_DIR
$Overlay = $null
$Stock = $false
$StockFile = $null
$Options = @()
$units = [ordered]@{}
$recipe = $null
$i = 0
while ($i -lt $args.Count) {
    $arg = $args[$i++]
    switch -Wildcard ($arg) {
        '--stock'      { $Stock = $true }
        '--source'     { $Source = (Resolve-Path -LiteralPath $args[$i++]).Path }
        '--overlay'    { $Overlay = (Resolve-Path -LiteralPath $args[$i++]).Path }
        '--stock-file' { $StockFile = $args[$i++] }
        '*.ps1'        { $recipe = (Resolve-Path -LiteralPath $arg).Path }
        '*.c'          { $units[$arg] = $args[$i++] -split ';' }
        default        { throw "Unknown test option: $arg" }
    }
}
$pin = git -C $PSScriptRoot rev-parse :Upstream/ReactOS/Source
if ($LASTEXITCODE -or !$pin) { throw 'Cannot read the ReactOS gitlink' }
$head = git -C $Source rev-parse HEAD
if ($LASTEXITCODE -or $head -ne $pin) { throw "Expected ReactOS $pin at $Source" }

function Read-Unit([string]$path, [switch]$Pinned) {
    if ($Pinned -or $Stock -or $path -eq $StockFile) {
        $lines = git -C $Source show "${pin}:$path"
        if ($LASTEXITCODE) { throw "Cannot read ${pin}:$path" }
        return ($lines -join "`n") + "`n"
    }
    $file = Join-Path $Source $path
    if ($Overlay -and (Test-Path -LiteralPath (Join-Path $Overlay $path) -PathType Leaf)) {
        $file = Join-Path $Overlay $path
    }
    return [IO.File]::ReadAllText($file).Replace("`r`n", "`n")
}

function Functions([string]$text, [string[]]$declarations) {
    foreach ($decl in $declarations) {
        $optional = $decl.StartsWith('?')
        if ($optional) { $decl = $decl.Substring(1) }
        if ($decl -notmatch '^(.+) (\w+)$') { throw "Expected a C return type and function name: $decl" }
        $type, $name = $Matches[1], $Matches[2]
        # Definitions and their closing braces start in column zero in the selected C sources.
        $found = [regex]::Matches($text, "(?ms)^$name[ `t]*\([^;{}]*?\)\s*(?:/\*.*?\*/\s*)?\{.*?^\}")
        if (!$found.Count -and $optional) { continue }
        if ($found.Count -ne 1) { throw "Expected one definition of $name, found $($found.Count)" }
        "$type`n$($found[0].Value)`n"
    }
}

function Write-Include([string]$name, [string[]]$lines) {
    [IO.File]::WriteAllText((Join-Path $Out $name), ($lines -join "`n") + "`n")
}

foreach ($unit in $units.Keys) {
    Write-Include ([IO.Path]::GetFileNameWithoutExtension($unit) + '.inc') (Functions (Read-Unit $unit) $units[$unit])
}
if ($recipe) { . $recipe }
[IO.File]::WriteAllLines((Join-Path $Out 'Case.rsp'),
    [string[]](@("/I`"$Out`"", "/I`"$PSScriptRoot`"") + $Options + "`"$Program`""),
    [Text.Encoding]::Unicode)
