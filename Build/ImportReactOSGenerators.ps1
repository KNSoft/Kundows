[CmdletBinding()]
param(
    [ValidateSet("BootSector", "Bin2c")]
    [string]$Generator = "BootSector",
    [switch]$Check
)
$ErrorActionPreference = "Stop"
$namespace = "http://schemas.microsoft.com/developer/msbuild/2003"
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot ".."))
$projectRoot = Join-Path $root "Upstream\ReactOS\Projects"

# Maintenance-time import only. Native builds never read migration records.
function Split-Command([string]$Command) {
    return @([regex]::Matches($Command, '(?:[^\s"]|"[^"]*")+') |
        ForEach-Object { $_.Value.Trim('"') })
}
function Convert-Path([string]$Path, [string]$Directory) {
    $expanded = $Path.Replace('$(ProjectDir)', "$Directory\").Replace("/", "\")
    return '$(KundowsRoot)' + [IO.Path]::GetRelativePath($root, [IO.Path]::GetFullPath($expanded))
}
function Convert-Argument([string]$Argument, [string]$Directory) {
    if ($Argument -match '^(?<option>/I|-I|/Fo)(?<path>\$\(ProjectDir\).+)$') {
        return $Matches.option + '"' + (Convert-Path $Matches.path $Directory) + '"'
    }
    if ($Argument.Contains('$(ProjectDir)')) { return '"' + (Convert-Path $Argument $Directory) + '"' }
    return $Argument
}
function Add-Item([xml]$Document, [Xml.XmlElement]$Group, [string]$Kind, [string]$InputFile, [hashtable]$Metadata) {
    $item = $Document.CreateElement($Kind, $namespace)
    $item.SetAttribute("Include", $InputFile)
    foreach ($name in ($Metadata.Keys | Sort-Object)) {
        $node = $Document.CreateElement($name, $namespace)
        $node.InnerText = $Metadata[$name]
        $null = $item.AppendChild($node)
    }
    $null = $Group.AppendChild($item)
}
function Add-HostReference([xml]$Document, [Xml.XmlElement]$Group, [string]$Tool, [string]$Guid) {
    Add-Item $Document $Group "ProjectReference" "`$(KundowsRoot)Upstream\ReactOS\Projects\HostTools\$Tool\$Tool.vcxproj" @{
        Project = "{$Guid}"
        SetPlatform = "Platform=`$(ReactOS${Generator}HostPlatform)"
        ReferenceOutputAssembly = "false"
        LinkLibraryDependencies = "false"
    }
}
function Get-BootSectorItems([object]$Record, [string]$Directory, [string]$StepsProperty) {
    $document = [xml]"<Project xmlns='$namespace'/>"
    foreach ($configuration in $Record.Configurations.PSObject.Properties) {
        $steps = @($configuration.Value.$StepsProperty)
        $group = $document.CreateElement("ItemGroup", $namespace)
        $group.SetAttribute("Label", "ReactOSBootSector")
        $group.SetAttribute("Condition", "'`$(Configuration)|`$(Platform)'=='$($configuration.Name)'")
        for ($index = 0; $index -lt $steps.Count; $index++) {
            if ($steps[$index].HostTools -notcontains "obj2bin") { continue }
            $conversion = $steps[$index]
            $assembly = $steps[$index - 1]
            $preprocess = $steps[$index - 2]
            $pp = @(Split-Command $preprocess.ReferenceCommand)
            $asm = @(Split-Command $assembly.ReferenceCommand)
            $bin = @(Split-Command $conversion.ReferenceCommand)
            if ($pp[-2] -ne ">" -or $asm[0] -notmatch '(^|\\)ml\.exe$') { throw "Unexpected raw binary chain: $($configuration.Name)" }
            $output = Convert-Path $preprocess.Outputs[0] $Directory
            Add-Item $document $group "ReactOSBootPreprocess" (Convert-Path $pp[-3] $Directory) @{
                Arguments = (@($pp | Select-Object -Skip 1 | ForEach-Object { Convert-Argument $_ $Directory }) -join " ")
                Outputs = $output
                OutputDirectory = $output.Substring(0, $output.LastIndexOf("\"))
            }
            Add-Item $document $group "ReactOSBootAssemble" (Convert-Path $asm[-1] $Directory) @{
                Arguments = (@($asm | Select-Object -Skip 1 | ForEach-Object { Convert-Argument $_ $Directory }) -join " ")
                Outputs = Convert-Path $assembly.Outputs[0] $Directory
            }
            Add-Item $document $group "ReactOSBootConvert" (Convert-Path $bin[1] $Directory) @{
                Arguments = (@($bin | Select-Object -Skip 1 | ForEach-Object { Convert-Argument $_ $Directory }) -join " ")
                Outputs = Convert-Path $conversion.Outputs[0] $Directory
            }
            if ($index -ge 3 -and $steps[$index - 3].ReferenceCommand -match '^cmd\.exe /C "copy /Y /B ') {
                $copy = $steps[$index - 3]
                if ($copy.Inputs.Count -ne 1) { throw "Review multi-input ASM16 concatenation before importing it" }
                Add-Item $document $group "ReactOSAsm16Copy" ("`$(KundowsRoot)Upstream\ReactOS\Source\" + $copy.Inputs[0].Replace("/", "\")) @{
                    Outputs = Convert-Path $copy.Outputs[0] $Directory
                }
            }
        }
        if ($group.ChildNodes.Count) {
            Add-HostReference $document $group "Obj2bin" "AF11E530-6ADF-43CD-A380-4FAEB2A22053"
            $null = $document.DocumentElement.AppendChild($group)
        }
    }
    return $document
}
function Get-Bin2cItems([object]$Record, [string]$Directory, [switch]$ReferenceOnly) {
    $document = [xml]"<Project xmlns='$namespace'/>"
    foreach ($configuration in $Record.Configurations.PSObject.Properties) {
        $steps = @($configuration.Value.PendingGenerationSteps | Where-Object { $_.HostTools -contains "bin2c" })
        if (-not $steps.Count) { continue }
        $group = $document.CreateElement("ItemGroup", $namespace)
        $group.SetAttribute("Label", "ReactOSBin2c")
        $group.SetAttribute("Condition", "'`$(Configuration)|`$(Platform)'=='$($configuration.Name)'")
        if ($ReferenceOnly) {
            Add-Item $document $group "ProjectReference" '$(KundowsRoot)Upstream\ReactOS\Projects\Hal\Halx86\PciData.vcxproj' @{
                Project = "{67C9CA89-A87A-44A8-90B3-FA2814144F72}"
                ReferenceOutputAssembly = "false"
                LinkLibraryDependencies = "false"
            }
        } else {
            foreach ($step in $steps) {
                $arguments = @(Split-Command $step.ReferenceCommand | Select-Object -Skip 1)
                $outputs = @($step.Outputs | ForEach-Object { Convert-Path $_ $Directory })
                Add-Item $document $group "ReactOSBin2c" (Convert-Path $arguments[0] $Directory) @{
                    Arguments = (@($arguments | ForEach-Object { Convert-Argument $_ $Directory }) -join " ")
                    Outputs = $outputs -join ";"
                    OutputDirectory = $outputs[0].Substring(0, $outputs[0].LastIndexOf("\"))
                }
            }
            Add-HostReference $document $group "Bin2c" "68DF970F-540E-4F4B-88BA-0A5477F3F1C6"
            if ($Record.Target -eq "ntvdm") {
                Add-Item $document $group "ProjectReference" '$(KundowsRoot)Upstream\ReactOS\Projects\Subsystems\Mvdm\Dos\Command.vcxproj' @{
                    Project = "{0CDAB3D3-F8A6-48F3-BE91-260047F2E410}"
                    ReferenceOutputAssembly = "false"
                    LinkLibraryDependencies = "false"
                }
            }
        }
        $null = $document.DocumentElement.AppendChild($group)
    }
    return $document
}
function Update-Project([string]$Path, [xml]$Items, [string]$Label) {
    $settings = [Xml.XmlWriterSettings]::new()
    $settings.Indent = $true
    $settings.OmitXmlDeclaration = $true
    $settings.NewLineChars = "`n"
    $text = [Text.StringBuilder]::new()
    $writer = [Xml.XmlWriter]::Create($text, $settings)
    $Items.Save($writer)
    $writer.Dispose()
    $lines = $text.ToString() -split "`n"
    $block = if ($Items.DocumentElement.ChildNodes.Count) { ($lines | Select-Object -Skip 1 | Select-Object -SkipLast 1) -join "`n" } else { "" }
    $contents = [IO.File]::ReadAllText($Path)
    $updated = [regex]::Replace($contents, "(?ms)^  <ItemGroup Label=`"$Label`"[^>]*>.*?^  </ItemGroup>\r?\n", "")
    if ($block) {
        $newline = if ($contents.Contains("`r`n")) { "`r`n" } else { "`n" }
        $anchor = '  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />'
        if (-not $updated.Contains($anchor)) { throw "Native C++ targets import missing: $Path" }
        $updated = $updated.Replace($anchor, $block.Replace("`n", $newline) + $newline + $anchor)
    }
    if ($contents -cne $updated) {
        if ($Check) { throw "$Path differs from its recorded $Label commands" }
        [IO.File]::WriteAllText($Path, $updated, [Text.UTF8Encoding]::new($false))
        return 1
    }
    return 0
}

$changed = 0
$projects = 0
foreach ($project in Get-ChildItem -LiteralPath $projectRoot -Recurse -Filter "*.vcxproj" -File) {
    $recordPath = Join-Path $project.DirectoryName "$($project.BaseName).Migration.json"
    if (-not (Test-Path -LiteralPath $recordPath)) { continue }
    $record = Get-Content -LiteralPath $recordPath -Raw | ConvertFrom-Json
    if ($Generator -eq "BootSector") {
        # freeldr consumes Frldr16.vcxproj; do not repeat its producer.
        if ($record.Target -eq "freeldr") { continue }
        $items = Get-BootSectorItems $record $project.DirectoryName "PendingGenerationSteps"
    } else {
        $items = Get-Bin2cItems $record $project.DirectoryName -ReferenceOnly:($record.Target -in @("lib_hal_legacy", "lib_hal_pc98", "lib_hal_xbox"))
    }
    if ($items.DocumentElement.ChildNodes.Count) { $projects++ }
    $changed += Update-Project $project.FullName $items "ReactOS$Generator"
}
if ($Generator -eq "BootSector") {
    $infrastructure = Get-Content (Join-Path $projectRoot "Infrastructure.json") -Raw | ConvertFrom-Json
    $items = Get-BootSectorItems $infrastructure.Targets.command $projectRoot "Steps"
    $changed += Update-Project (Join-Path $projectRoot "Subsystems\Mvdm\Dos\Command.vcxproj") $items "ReactOSBootSector"
} else {
    $directory = Join-Path $projectRoot "Hal\Halx86"
    $record = Get-Content (Join-Path $directory "Lib_Hal_Legacy.Migration.json") -Raw | ConvertFrom-Json
    $items = Get-Bin2cItems $record $directory
    $changed += Update-Project (Join-Path $directory "PciData.vcxproj") $items "ReactOSBin2c"
}
"$Generator import: $($projects + 1) projects with recipes/references; $changed projects updated"
