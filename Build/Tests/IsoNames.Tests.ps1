$ErrorActionPreference = "Stop"
$scriptPath = Join-Path $PSScriptRoot "..\BuildReactOSIso.ps1"
$tokens = $null
$errors = $null
$ast = [Management.Automation.Language.Parser]::ParseFile(
    [IO.Path]::GetFullPath($scriptPath), [ref]$tokens, [ref]$errors)
if ($errors.Count) { throw ($errors | Out-String) }
foreach ($name in @("Resolve-CMakeTargetFileNames", "Test-IsoRelativePath")) {
    $definition = $ast.Find({
        param($node)
        $node -is [Management.Automation.Language.FunctionDefinitionAst] -and
        $node.Name -eq $name
    }, $true)
    if (-not $definition) { throw "Missing production function: $name" }
    . ([scriptblock]::Create($definition.Extent.Text))
}

# Mock the MSBuild output catalog, not the production name/path handling.
function Resolve-OutputForTarget {
    param([string]$Target, [object[]]$ProjectMap)
    switch ($Target) {
        "kdcom" { return "E:\build\Kdcom\kdcom.dll" }
        "kdgdb" { return "E:\build\Kdgdb\kdgdb.dll" }
        default { return $null }
    }
}
$map = @([pscustomobject]@{ Target = "mock" })
$cases = 0
foreach ($entry in @(
    @('$<TARGET_FILE_NAME:kdcom>', "kdcom.dll"),
    @('$<TARGET_FILE_NAME:kdgdb>', "kdgdb.dll"),
    @('prefix-$<TARGET_FILE_NAME:kdcom>', "prefix-kdcom.dll"),
    @('ntdll.dll', "ntdll.dll"),
    @('$<TARGET_FILE_NAME:missing>', '$<TARGET_FILE_NAME:missing>')
)) {
    $actual = Resolve-CMakeTargetFileNames -Value $entry[0] -ProjectMap $map
    if ($actual -cne $entry[1]) { throw "Name expansion failed: $($entry[0]) -> $actual" }
    $cases++
}
foreach ($path in @(
    'amd64/system32/$<TARGET_FILE_NAME:kdcom>',
    'reactos/${UNRESOLVED}/file.dll',
    'reactos/$(UNRESOLVED)/file.dll',
    'amd64/system32/file:name.dll',
    'amd64/system32/bad<name>.dll',
    '/absolute/file.dll',
    'a/../file.dll',
    'a//file.dll',
    'a\file.dll'
)) {
    if (Test-IsoRelativePath -Path $path) { throw "Invalid ISO path accepted: $path" }
    $cases++
}
foreach ($path in @("amd64/system32/kdcom.dll", "freeldr.ini", "Users/Default/Start Menu/file.lnk")) {
    if (-not (Test-IsoRelativePath -Path $path)) { throw "Valid ISO path rejected: $path" }
    $cases++
}
"ISO names: $cases cases passed"
