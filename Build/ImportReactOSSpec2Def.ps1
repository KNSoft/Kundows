[CmdletBinding()]
param(
    [string]$ProjectRoot = (Join-Path $PSScriptRoot "..\Upstream\ReactOS\Projects"),
    [switch]$Check
)
$ErrorActionPreference = "Stop"
$namespace = "http://schemas.microsoft.com/developer/msbuild/2003"
$kundowsRoot = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot ".."))

function Split-ReferenceCommand([string]$Command) {
    return @([regex]::Matches($Command, '(?:[^\s"]|"[^"]*")+') |
        ForEach-Object { $_.Value.Replace('"', "") })
}

function Convert-MigrationPath([string]$Path, [string]$ProjectDirectory) {
    $expanded = $Path.Replace('$(ProjectDir)', "$ProjectDirectory\").Replace("/", "\")
    $fullPath = [IO.Path]::GetFullPath($expanded)
    return '$(KundowsRoot)' + [IO.Path]::GetRelativePath($kundowsRoot, $fullPath)
}

function Convert-ReferenceArgument([string]$Argument, [string]$ProjectDirectory) {
    if ($Argument -match '^(?<option>-[dsli]=|/Fo|/DEF:|/OUT:)(?<path>.+)$') {
        $path = Convert-MigrationPath $Matches.path $ProjectDirectory
        return "$($Matches.option)`"$path`""
    }
    if ($Argument.Contains('$(ProjectDir)')) {
        return '"' + (Convert-MigrationPath $Argument $ProjectDirectory) + '"'
    }
    return $Argument
}

function New-GenerationItem(
    [xml]$Document, [Xml.XmlElement]$Group, [string]$Kind,
    [string]$InputFile, [hashtable]$Metadata
) {
    $item = $Document.CreateElement($Kind, $namespace)
    $item.SetAttribute("Include", $InputFile)
    foreach ($name in ($Metadata.Keys | Sort-Object)) {
        $node = $Document.CreateElement($name, $namespace)
        $node.InnerText = $Metadata[$name]
        $null = $item.AppendChild($node)
    }
    $null = $Group.AppendChild($item)
}

function Get-Spec2DefItems([object]$Record, [string]$ProjectDirectory) {
    $document = [xml]"<Project xmlns='$namespace'/>"
    foreach ($configuration in $Record.Configurations.PSObject.Properties) {
        $steps = @($configuration.Value.PendingGenerationSteps)
        $group = $document.CreateElement("ItemGroup", $namespace)
        $group.SetAttribute("Label", "ReactOSSpec2Def")
        $group.SetAttribute("Condition", "'`$(Configuration)|`$(Platform)'=='$($configuration.Name)'")
        for ($index = 0; $index -lt $steps.Count; $index++) {
            $step = $steps[$index]
            if ($step.HostTools -notcontains "spec2def") { continue }
            $arguments = @(Split-ReferenceCommand $step.ReferenceCommand | Select-Object -Skip 1)
            $specFile = Convert-MigrationPath $arguments[-1] $ProjectDirectory
            $options = @($arguments[0..($arguments.Count - 2)] |
                ForEach-Object { Convert-ReferenceArgument $_ $ProjectDirectory })
            $outputs = @($step.Outputs | ForEach-Object { Convert-MigrationPath $_ $ProjectDirectory })
            New-GenerationItem $document $group "ReactOSSpec2Def" $specFile @{
                Arguments = $options -join " "
                Outputs = $outputs -join ";"
                OutputDirectory = $outputs[0].Substring(0, $outputs[0].LastIndexOf("\"))
            }
            if ($arguments -notcontains "--implib") { continue }

            # These are the two following custom commands emitted by upstream
            # generate_import_lib: assemble the stub, then link /LIB /DEF.
            $assembly = $steps[++$index]
            $library = $steps[++$index]
            $assemblyCommand = @(Split-ReferenceCommand $assembly.ReferenceCommand)
            $libraryCommand = @(Split-ReferenceCommand $library.ReferenceCommand)
            $stubFile = Convert-MigrationPath $assemblyCommand[-1] $ProjectDirectory
            $stubObject = Convert-MigrationPath $assembly.Outputs[0] $ProjectDirectory
            $definition = $arguments | Where-Object { $_.StartsWith("-d=") }
            $definition = Convert-MigrationPath $definition.Substring(3) $ProjectDirectory
            New-GenerationItem $document $group "ReactOSSpec2DefStub" $stubFile @{
                Tool = [IO.Path]::ChangeExtension($assemblyCommand[0], ".exe")
                Arguments = (@($assemblyCommand | Select-Object -Skip 1 |
                    ForEach-Object { Convert-ReferenceArgument $_ $ProjectDirectory }) -join " ")
                Outputs = $stubObject
            }
            New-GenerationItem $document $group "ReactOSImportLibrary" $definition @{
                Arguments = (@($libraryCommand | Select-Object -Skip 1 |
                    ForEach-Object { Convert-ReferenceArgument $_ $ProjectDirectory }) -join " ")
                AdditionalInputs = $stubObject
                Outputs = Convert-MigrationPath $library.Outputs[0] $ProjectDirectory
            }
        }
        if ($group.ChildNodes.Count) { $null = $document.DocumentElement.AppendChild($group) }
    }
    $settings = [Xml.XmlWriterSettings]::new()
    $settings.Indent = $true
    $settings.OmitXmlDeclaration = $true
    $settings.NewLineChars = "`n"
    $text = [Text.StringBuilder]::new()
    $writer = [Xml.XmlWriter]::Create($text, $settings)
    $document.Save($writer)
    $writer.Dispose()
    $lines = $text.ToString() -split "`n"
    return ($lines | Select-Object -Skip 1 | Select-Object -SkipLast 1) -join "`n"
}

$changed = 0
$withRecipes = 0
foreach ($project in Get-ChildItem -LiteralPath $ProjectRoot -Recurse -Filter "*.vcxproj" -File) {
    $recordPath = Join-Path $project.DirectoryName "$($project.BaseName).Migration.json"
    if (-not (Test-Path -LiteralPath $recordPath)) { continue }
    $record = Get-Content -LiteralPath $recordPath -Raw | ConvertFrom-Json
    $items = Get-Spec2DefItems $record $project.DirectoryName
    $contents = [IO.File]::ReadAllText($project.FullName)
    $pattern = '(?ms)^  <ItemGroup Label="ReactOSSpec2Def"[^>]*>.*?^  </ItemGroup>\r?\n'
    $existing = [regex]::Match($contents, $pattern)
    $updated = [regex]::Replace($contents, $pattern, "")
    if ($items) {
        $withRecipes++
        $newline = if ($contents.Contains("`r`n")) { "`r`n" } else { "`n" }
        $items = $items.Replace("`n", $newline) + $newline
        $anchor = '  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />'
        if (-not $updated.Contains($anchor)) { throw "Native C++ targets import missing: $($project.FullName)" }
        $updated = if ($existing.Success) { $updated.Insert($existing.Index, $items) } else { $updated.Replace($anchor, $items + $anchor) }
    }
    if ($updated -cne $contents) {
        $changed++
        if (-not $Check) {
            [IO.File]::WriteAllText($project.FullName, $updated, [Text.UTF8Encoding]::new($false))
        }
    }
}
if ($Check -and $changed) { throw "$changed native projects differ from their recorded spec2def recipes" }
"spec2def migration: $withRecipes projects with recipes; $changed projects $(if ($Check) {'out of date'} else {'updated'})"
