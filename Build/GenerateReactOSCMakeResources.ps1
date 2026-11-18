param(
    [Parameter(Mandatory = $true)]
    [string]$ProjectFile,
    [Parameter(Mandatory = $true)]
    [string]$SourceRoot,
    [Parameter(Mandatory = $true)]
    [string]$Platform,
    [Parameter(Mandatory = $true)]
    [string]$Configuration
)

$ErrorActionPreference = "Stop"

$projectPath = [IO.Path]::GetFullPath($ProjectFile)
$projectDirectory = Split-Path -Parent $projectPath
$sourceRootPath = [IO.Path]::GetFullPath($SourceRoot)
$xml = [xml](Get-Content -LiteralPath $projectPath -Raw)

function Resolve-ProjectPath([string]$Value) {
    $expanded = $Value
    $expanded = $expanded.Replace('$(ProjectDir)', "$projectDirectory\")
    $expanded = $expanded.Replace('$(Platform)', $Platform)
    $expanded = $expanded.Replace('$(Configuration)', $Configuration)
    $expanded = $expanded.Replace('$(KundowsRoot)', "$sourceRootPath\..\..\..\")
    return [IO.Path]::GetFullPath($expanded)
}

function Get-ResourceDescription([string]$TargetName) {
    switch ($TargetName.ToLowerInvariant()) {
        "bootvid" { return "VGA Boot Driver" }
        "lfbbvid" { return "Generic Linear FrameBuffer Boot Driver" }
        "pc98bvid" { return "PC-98 Boot Driver" }
        "xboxbvid" { return "Xbox Boot Driver" }
        default { return "Boot Video Driver" }
    }
}

$template = Join-Path $sourceRootPath "drivers\base\bootvid\bootvid.rc"
if (-not (Test-Path -LiteralPath $template -PathType Leaf)) {
    throw "Boot video resource template was not found: $template"
}

foreach ($itemGroup in @($xml.Project.ItemGroup)) {
    foreach ($item in @($itemGroup.ResourceCompile)) {
        $include = [string]$item.Include
        if (-not $include) {
            continue
        }

        $output = Resolve-ProjectPath $include
        if ([IO.Path]::GetExtension($output) -ine ".rc" -or
            $output -notmatch [regex]::Escape("OutDir\Build\Generated")) {
            continue
        }

        $targetName = [IO.Path]::GetFileNameWithoutExtension($output)
        if ($targetName -notin @("bootvid", "lfbbvid", "pc98bvid", "xboxbvid")) {
            continue
        }

        $description = Get-ResourceDescription $targetName
        $contents = [IO.File]::ReadAllText($template)
        $contents = $contents -replace `
            '(?m)^\s*#cmakedefine REACTOS_STR_FILE_DESCRIPTION\s+"\$\{REACTOS_STR_FILE_DESCRIPTION\}"\s*$', `
            "#define REACTOS_STR_FILE_DESCRIPTION    `"$description`""
        $contents = $contents -replace `
            '(?m)^\s*#cmakedefine REACTOS_STR_INTERNAL_NAME\s+"\$\{REACTOS_STR_INTERNAL_NAME\}"\s*$', `
            "#define REACTOS_STR_INTERNAL_NAME       `"$targetName`""
        $contents = $contents -replace `
            '(?m)^\s*#cmakedefine REACTOS_STR_ORIGINAL_FILENAME\s+"\$\{REACTOS_STR_ORIGINAL_FILENAME\}"\s*$', `
            "#define REACTOS_STR_ORIGINAL_FILENAME   `"$targetName.dll`""

        New-Item -ItemType Directory -Force -Path (Split-Path -Parent $output) | Out-Null
        [IO.File]::WriteAllText(
            $output,
            $contents,
            [Text.UTF8Encoding]::new($false))
    }
}
