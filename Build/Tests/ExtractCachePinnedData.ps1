param(
    [Parameter(Mandatory=$true)][string]$Source,
    [Parameter(Mandatory=$true)][string]$Output,
    [string[]]$FunctionNames = @("CcpDereferenceBcb", "CcSetDirtyPinnedData", "CcUnpinDataForThread", "CcUnpinRepinnedBcb")
)
$ErrorActionPreference = "Stop"
$text = [IO.File]::ReadAllText($Source)
$functions = foreach ($name in $FunctionNames) {
    $pattern = '(?m)^(?:static\s+)?(?:VOID|BOOLEAN|NTSTATUS)\s+(?:NTAPI\s+)?' + [regex]::Escape($name) + '\s*\('
    $matches = [regex]::Matches($text, $pattern)
    if ($matches.Count -ne 1) { throw "Expected one production definition of $name" }
    $start = $matches[0].Index
    $open = $text.IndexOf('{', $start)
    if ($open -lt 0) { throw "Missing function body: $name" }
    $depth = 0
    $state = "Code"
    $end = -1
    :Scan for ($i = $open; $i -lt $text.Length; $i++) {
        $c = $text[$i]
        $next = if ($i+1 -lt $text.Length) { $text[$i+1] } else { [char]0 }
        switch ($state) {
            "Line" { if ($c -eq "`n") { $state = "Code" }; continue Scan }
            "Block" { if ($c -eq '*' -and $next -eq '/') { $state = "Code"; $i++ }; continue Scan }
            "String" {
                if ($c -eq '\') { $i++ } elseif ($c -eq '"') { $state = "Code" }
                continue Scan
            }
            "Character" {
                if ($c -eq '\') { $i++ } elseif ($c -eq "'") { $state = "Code" }
                continue Scan
            }
        }
        if ($c -eq '/' -and $next -eq '/') { $state = "Line"; $i++; continue }
        if ($c -eq '/' -and $next -eq '*') { $state = "Block"; $i++; continue }
        if ($c -eq '"') { $state = "String"; continue }
        if ($c -eq "'") { $state = "Character"; continue }
        if ($c -eq '{') { $depth++ }
        if ($c -eq '}') {
            $depth--
            if ($depth -eq 0) { $end = $i+1; break }
        }
    }
    if ($end -lt 0) { throw "Unterminated function: $name" }
    $text.Substring($start, $end-$start)
}
$content = "/* Actual production functions; only kernel/MM boundaries are mocked by the fixture. */`r`n" +
    ($functions -join "`r`n`r`n") + "`r`n"
$null = New-Item -ItemType Directory -Force -Path (Split-Path -Parent $Output)
if (-not (Test-Path $Output) -or [IO.File]::ReadAllText($Output) -cne $content) {
    [IO.File]::WriteAllText($Output, $content, [Text.UTF8Encoding]::new($false))
}
