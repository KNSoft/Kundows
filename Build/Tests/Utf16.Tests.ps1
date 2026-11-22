[CmdletBinding()]
param([string]$MSBuild = "MSBuild.exe")
$ErrorActionPreference = "Stop"
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot "..\.."))
& "$root\Build\ImportReactOSGenerators.ps1" -Generator Utf16 -Check
$temporary = Join-Path $env:TEMP ("kundows-utf16-" + [guid]::NewGuid())
$null = New-Item -ItemType Directory -Path $temporary
$cases = 0
try {
    foreach ($project in Get-ChildItem "$root\Upstream\ReactOS\Projects" -Recurse -Filter "*.vcxproj" -File) {
        $xml = [xml](Get-Content -LiteralPath $project.FullName -Raw)
        foreach ($group in @($xml.Project.ItemGroup | Where-Object { $_.Label -eq "ReactOSUtf16" })) {
            $null = $group.Condition -match "=='(?<configuration>[^|]+)\|(?<platform>[^']+)'"
            $platform = $Matches.platform
            $configuration = $Matches.configuration
            $options = @("/nologo", "/v:quiet", "/t:GenerateReactOSUtf16ProjectFiles", "/p:BuildProjectReferences=false", "/p:Platform=$platform", "/p:Configuration=$configuration")
            & $MSBuild $project.FullName @options
            if ($LASTEXITCODE) { throw "UTF-16 generation failed: $($project.Name) $platform $configuration" }
            $hostPlatform = if ($platform -eq "ARM64") { "x64" } else { $platform }
            $outputs = @($group.ReactOSUtf16 | ForEach-Object {
                $source = $_.Include.Replace('$(KundowsRoot)', "$root\").Replace('$(Platform)', $platform).Replace('$(Configuration)', $configuration)
                $output = $_.Outputs.Replace('$(KundowsRoot)', "$root\").Replace('$(Platform)', $platform).Replace('$(Configuration)', $configuration)
                & "$root\OutDir\HostTools\$hostPlatform\$configuration\utf16le.exe" $source "$temporary\reference"
                if ($LASTEXITCODE -or (Get-FileHash $output).Hash -ne (Get-FileHash "$temporary\reference").Hash) {
                    throw "Native UTF-16 output differs from upstream: $output"
                }
                $cases++
                $output
            })
            $before = @($outputs | ForEach-Object { (Get-Item -LiteralPath $_).LastWriteTimeUtc.Ticks })
            & $MSBuild $project.FullName @options
            if ($LASTEXITCODE) { throw "Incremental UTF-16 generation failed" }
            $after = @($outputs | ForEach-Object { (Get-Item -LiteralPath $_).LastWriteTimeUtc.Ticks })
            if (($before -join "|") -cne ($after -join "|")) { throw "UTF-16 generation rewrote unchanged outputs" }
            Remove-Item -LiteralPath $outputs[0]
            & $MSBuild $project.FullName @options
            if ($LASTEXITCODE -or -not (Test-Path -LiteralPath $outputs[0])) { throw "UTF-16 missing-output regeneration failed" }
        }
    }
    $options = @("/nologo", "/v:quiet", "/p:FixtureRoot=$temporary")
    foreach ($value in @("first", "second")) {
        [IO.File]::WriteAllText("$temporary\input.INI", $value, [Text.UTF8Encoding]::new($false))
        & $MSBuild "$PSScriptRoot\Utf16.Tests.proj" @options
        if ($LASTEXITCODE -or [IO.File]::ReadAllText("$temporary\explicit.destination") -cne $value) { throw "UTF-16 generation left stale output after an input change" }
    }
    foreach ($encoding in @([Text.Encoding]::UTF8, [Text.Encoding]::Unicode, [Text.Encoding]::BigEndianUnicode, [Text.Encoding]::UTF32, [Text.UTF32Encoding]::new($true, $true))) {
        [IO.File]::WriteAllText("$temporary\input.INI", "ReactOS text", $encoding)
        & $MSBuild "$PSScriptRoot\Utf16.Tests.proj" @options /p:BomOption=nobom
        if ($LASTEXITCODE) { throw "UTF-16 input encoding test failed" }
        & "$root\OutDir\HostTools\x64\Debug\utf16le.exe" "$temporary\input.INI" "$temporary\reference" nobom
        if ($LASTEXITCODE -or (Get-FileHash "$temporary\explicit.destination").Hash -ne (Get-FileHash "$temporary\reference").Hash) { throw "UTF-16 BOM or input encoding changed upstream behavior" }
    }
    # Exercise the packaging consumer's actual production functions too.
    $KundowsRoot = $root
    $Platform = "x64"
    $Configuration = "Debug"
    $tokens = $null
    $errors = $null
    $ast = [Management.Automation.Language.Parser]::ParseFile("$root\Build\BuildReactOSIso.ps1", [ref]$tokens, [ref]$errors)
    if ($errors.Count) { throw "ISO script parse failure" }
    foreach ($name in @("Assert-File", "Get-HostToolPath", "Invoke-HostTool", "Convert-ToUtf16File")) {
        $definition = $ast.Find({ param($node) $node -is [Management.Automation.Language.FunctionDefinitionAst] -and $node.Name -eq $name }, $true)
        . ([scriptblock]::Create($definition.Extent.Text))
    }
    Convert-ToUtf16File "$temporary\input.INI" "$temporary\packaging"
    & "$root\OutDir\HostTools\x64\Debug\utf16le.exe" "$temporary\input.INI" "$temporary\reference"
    if ($LASTEXITCODE -or (Get-FileHash "$temporary\packaging").Hash -ne (Get-FileHash "$temporary\reference").Hash) { throw "Packaging UTF-16 differs from upstream" }
    Remove-Item -LiteralPath "$temporary\input.INI"
    & $MSBuild "$PSScriptRoot\Utf16.Tests.proj" @options > "$temporary\failure.log"
    if ($LASTEXITCODE -eq 0) { throw "UTF-16 generation concealed a missing input" }
}
finally { Remove-Item -LiteralPath $temporary -Recurse -Force }
"UTF-16: $cases configuration recipes match upstream; changed-input, BOM/encoding, incremental, missing-output, packaging, and failure tests passed"
exit 0
