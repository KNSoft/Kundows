[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$ReferenceRoot,
    [switch]$Check
)
$ErrorActionPreference = "Stop"
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot ".."))
$projectsRoot = Join-Path $root "Upstream\ReactOS\Projects"
$sourceRoot = Join-Path $root "Upstream\ReactOS\Source"
$namespace = "http://schemas.microsoft.com/developer/msbuild/2003"
$catalog = Get-Content "$projectsRoot\Migration.json" -Raw | ConvertFrom-Json
$infrastructure = Get-Content "$projectsRoot\Infrastructure.json" -Raw | ConvertFrom-Json -AsHashtable
$sdkTargets = @("psdk", "dxsdk", "wineheaders", "reactos_idls", "stdole2", "d3d_idl_headers")
$sharedPath = "$projectsRoot\Dll\Win32\Advapi32\Advapi32RpcHeaders.vcxproj"
$sharedGuid = "{881DECD7-A58F-42ED-A24C-B1180BFEA092}"
$documents = @{}
$recipes = 0

# The original project files are read only at maintenance time. Do not add
# this importer, its reference trees, or CMake to native build dependencies.
function Get-Document([string]$Path) {
    if (-not $documents.ContainsKey($Path)) { $documents[$Path] = [xml]"<Project xmlns='$namespace'/>" }
    return $documents[$Path]
}
function New-Group([xml]$Document, [string]$Key) {
    $group = $Document.CreateElement("ItemGroup", $namespace)
    $group.SetAttribute("Label", "ReactOSWidl")
    $group.SetAttribute("Condition", "'`$(Configuration)|`$(Platform)'=='$Key'")
    return $group
}
function Add-Item([xml]$Document, [Xml.XmlElement]$Group, [string]$Kind, [string]$Identity, [hashtable]$Metadata) {
    $item = $Document.CreateElement($Kind, $namespace)
    $item.SetAttribute("Include", $Identity)
    foreach ($name in ($Metadata.Keys | Sort-Object)) {
        $node = $Document.CreateElement($name, $namespace)
        $node.InnerText = $Metadata[$name]
        $null = $item.AppendChild($node)
    }
    $null = $Group.AppendChild($item)
}
function Convert-ReferencePath([string]$Path) {
    $path = [IO.Path]::GetFullPath($Path).Replace("\", "/")
    if ($path.StartsWith("$referenceDirectory/", [StringComparison]::OrdinalIgnoreCase)) {
        return '$(ReactOSWidlGeneratedRoot)/' + $path.Substring($referenceDirectory.Length + 1)
    }
    if ($path.StartsWith("$referenceSource/", [StringComparison]::OrdinalIgnoreCase)) {
        return '$(ReactOSWidlSourceRoot)/' + $path.Substring($referenceSource.Length + 1)
    }
    throw "Reference path is outside the source/binary trees: $Path"
}
function Convert-Arguments([string]$Arguments) {
    # Keep the original forward slashes: upstream WIDL's get_basename only
    # recognizes '/', so changing these alters guards and proxy symbol names.
    $value = $Arguments.Replace($referenceSource + "/", '$(ReactOSWidlSourceRoot)/').
        Replace($referenceDirectory + "/", '$(ReactOSWidlGeneratedRoot)/')
    if ($value.Contains($referenceDirectory.Replace("/", "\")) -or $value.Contains($referenceSource.Replace("/", "\"))) {
        throw "Review a nonstandard backslash WIDL command before importing it"
    }
    return $value
}
function Add-Commands([xml]$Document, [Xml.XmlElement]$Group, [string]$ReferenceProject, [Collections.Generic.HashSet[string]]$Seen) {
    $project = [xml](Get-Content -LiteralPath $ReferenceProject -Raw)
    foreach ($custom in @($project.Project.ItemGroup.CustomBuild)) {
        $command = @($custom.Command | Where-Object { $_.Condition -eq "'`$(Configuration)|`$(Platform)'=='$key'" })[0]
        if (-not $command -or $command.InnerText -notmatch 'widl\.exe') { continue }
        $arguments = @()
        $workingDirectory = [IO.Path]::GetDirectoryName($ReferenceProject)
        foreach ($line in $command.InnerText -split "\r?\n") {
            if ($line -match '^cd (?:/D )?"?(?<path>.+?)"?$') { $workingDirectory = $Matches.path }
            if ($line -match '^(?:"[^"]*[\\/]widl\.exe"|[^\s"]*[\\/]widl\.exe)\s+(?<arguments>.*)$') {
                $arguments += Convert-Arguments $Matches.arguments
            }
        }
        if ($arguments.Count -lt 1 -or $arguments.Count -gt 2) { throw "Unexpected WIDL command count in $ReferenceProject" }
        $outputs = @($custom.Outputs | Where-Object { $_.Condition -eq $command.Condition })[0].InnerText
        $outputs = @($outputs.Split(";") | ForEach-Object { Convert-ReferencePath $_ })
        $outputKey = $outputs -join ";"
        if (-not $Seen.Add($outputKey)) {
            if (-not [object]::ReferenceEquals($owners[$outputKey], $Document)) { throw "WIDL output requires an explicit shared producer: $outputKey" }
            continue
        }
        $owners[$outputKey] = $Document
        $inputs = @($custom.AdditionalInputs | Where-Object { $_.Condition -eq $command.Condition })[0].InnerText
        $inputs = @($inputs.Split(";") | Where-Object { $_ -and $_ -ne "%(AdditionalInputs)" -and $_ -notmatch '[\\/]widl\.exe$' } |
            ForEach-Object { Convert-ReferencePath $_ })
        if (-not $inputs.Count) { throw "Missing declared WIDL inputs in $ReferenceProject" }
        $metadata = @{
            Arguments = $arguments[0]
            Inputs = $inputs -join ";"
            Outputs = $outputs -join ";"
            OutputDirectory = $outputs[0].Substring(0, $outputs[0].LastIndexOf("/"))
            WorkingDirectory = Convert-ReferencePath $workingDirectory
        }
        if ($arguments.Count -eq 2) { $metadata.HeaderArguments = $arguments[1] }
        Add-Item $Document $Group "ReactOSWidl" $outputs[0] $metadata
        $script:recipes++
    }
    return $project
}
function Add-Reference([xml]$Document, [Xml.XmlElement]$Group, [string]$Path, [string]$Guid, [switch]$HostTool) {
    $metadata = @{ Project = $Guid; ReferenceOutputAssembly = "false"; LinkLibraryDependencies = "false" }
    if ($HostTool) { $metadata.SetPlatform = 'Platform=$(ReactOSWidlHostPlatform)' }
    Add-Item $Document $Group "ProjectReference" $Path $metadata
}
function Finish-Group([xml]$Document, [Xml.XmlElement]$Group) {
    if (@($Group.ChildNodes | Where-Object { $_.LocalName -eq "ReactOSWidl" }).Count) {
        Add-Reference $Document $Group '$(KundowsRoot)Upstream\ReactOS\Projects\HostTools\Widl\Widl.vcxproj' "{0BD1B80C-7B4E-5D30-9B52-4FD9AE9ED79C}" -HostTool
    }
    if ($Group.ChildNodes.Count) { $null = $Document.DocumentElement.AppendChild($Group) }
}

foreach ($platform in @("Win32", "x64", "ARM64")) {
    $architecture = switch ($platform) { "Win32" { "x86" } "x64" { "x64" } "ARM64" { "arm64" } }
    foreach ($configuration in @("Debug", "Release")) {
        $key = "$configuration|$platform"
        $referenceDirectory = [IO.Path]::GetFullPath((Join-Path $ReferenceRoot "reference-$architecture-$configuration")).Replace("\", "/")
        $cache = Get-Content "$referenceDirectory/CMakeCache.txt" -Raw
        foreach ($setting in @("DLL_EXPORT_VERSION=0x502", "CMAKE_BUILD_TYPE=$configuration", "ENABLE_ROSTESTS=0")) {
            $name, $value = $setting.Split("=")
            if ($cache -notmatch "(?m)^${name}:[^=]+=$([regex]::Escape($value))\r?$") { throw "Reference configuration mismatch: $setting in $referenceDirectory" }
        }
        $referenceSource = [regex]::Match($cache, '(?m)^CMAKE_HOME_DIRECTORY:INTERNAL=(.+)\r?$').Groups[1].Value.TrimEnd("`r").Replace("\", "/")
        if ([IO.Path]::GetFullPath($referenceSource) -ine $sourceRoot) { throw "Reference source tree differs from this checkout" }
        $sharedDocument = Get-Document $sharedPath
        $sharedGroup = New-Group $sharedDocument $key
        $seen = [Collections.Generic.HashSet[string]]::new([StringComparer]::OrdinalIgnoreCase)
        $owners = @{}
        $null = Add-Commands $sharedDocument $sharedGroup "$referenceDirectory/dll/win32/advapi32/advapi32_rpc_headers.vcxproj" $seen
        Finish-Group $sharedDocument $sharedGroup
        $prepare = Get-Document "$projectsRoot\Prepare\ReactOSPrepare.vcxproj"
        $prepareGroup = New-Group $prepare $key
        foreach ($target in $sdkTargets) {
            $directory = $infrastructure.Targets[$target].Configurations[$key].SourceDirectory
            $null = Add-Commands $prepare $prepareGroup "$referenceDirectory/$directory/$target.vcxproj" $seen
        }
        Finish-Group $prepare $prepareGroup
        foreach ($entry in $catalog.Projects) {
            $record = Get-Content (Join-Path $root $entry.Record) -Raw | ConvertFrom-Json
            $config = $record.Configurations.$key
            if (-not $config.AvailableInBaseline) { continue }
            $original = "$referenceDirectory/$($config.SourceDirectory)/$($entry.Target).vcxproj"
            if (-not (Test-Path -LiteralPath $original)) { throw "Reference project missing: $original" }
            $document = Get-Document (Join-Path $root $entry.Project)
            $group = New-Group $document $key
            $upstream = Add-Commands $document $group $original $seen
            foreach ($reference in @($upstream.Project.ItemGroup.ProjectReference)) {
                $target = [IO.Path]::GetFileNameWithoutExtension($reference.Include)
                if ($target -eq "advapi32_rpc_headers") {
                    Add-Reference $document $group '$(KundowsRoot)Upstream\ReactOS\Projects\Dll\Win32\Advapi32\Advapi32RpcHeaders.vcxproj' $sharedGuid
                } elseif ($target -notin $sdkTargets -and $infrastructure.Targets.ContainsKey($target)) {
                    $steps = @($infrastructure.Targets[$target].Configurations[$key].Steps)
                    if (@($steps | Where-Object { $_.HostTools -contains "widl" }).Count) {
                        $null = Add-Commands $document $group $reference.Include $seen
                    }
                }
            }
            Finish-Group $document $group
        }
    }
}

$changed = 0
$producers = 0
foreach ($path in $documents.Keys | Sort-Object) {
    $document = $documents[$path]
    if ($document.DocumentElement.ChildNodes.Count) { $producers++ }
    $settings = [Xml.XmlWriterSettings]::new()
    $settings.Indent = $true
    $settings.OmitXmlDeclaration = $true
    $settings.NewLineChars = "`n"
    $text = [Text.StringBuilder]::new()
    $writer = [Xml.XmlWriter]::Create($text, $settings)
    $document.Save($writer)
    $writer.Dispose()
    $block = if ($document.DocumentElement.ChildNodes.Count) { (($text.ToString() -split "`n") | Select-Object -Skip 1 | Select-Object -SkipLast 1) -join "`n" } else { "" }
    $contents = [IO.File]::ReadAllText($path)
    $updated = $contents
    if ($path -like "*\Sdk\Lib\Uuid\Uuid.vcxproj") {
        $updated = [regex]::Replace($updated, '(?ms)^  <PropertyGroup>\r?\n    <ReactOSWidlTool .*?^  </PropertyGroup>\r?\n', "")
        $updated = [regex]::Replace($updated, '(?ms)^  <ItemGroup>\r?\n    <ReactOSUuidIdl .*?^  </ItemGroup>\r?\n', "")
        $updated = [regex]::Replace($updated, '(?ms)^  <ItemGroup>\r?\n    <ProjectReference Include="\.\.\\\.\.\\\.\.\\HostTools\\Widl\\Widl.vcxproj">.*?^  </ItemGroup>\r?\n', "")
        $updated = [regex]::Replace($updated, '(?ms)^  <Target Name="GenerateReactOSUuidInterfaceIdentifiers".*?^  </Target>\r?\n', "")
        $updated = $updated.Replace('$(Platform)\$(Configuration)\Uuid', '$(Platform)\$(Configuration)\sdk\lib\uuid')
    }
    $pattern = '(?ms)^  <ItemGroup Label="ReactOSWidl"[^>]*>.*?^  </ItemGroup>\r?\n'
    $existing = [regex]::Match($updated, $pattern)
    $updated = [regex]::Replace($updated, $pattern, "")
    if ($block) {
        $newline = if ($contents.Contains("`r`n")) { "`r`n" } else { "`n" }
        $anchor = '  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />'
        if (-not $updated.Contains($anchor)) { throw "Native C++ targets import missing: $path" }
        $block = $block.Replace("`n", $newline) + $newline
        $updated = if ($existing.Success) { $updated.Insert($existing.Index, $block) } else { $updated.Replace($anchor, $block + $anchor) }
    }
    if ($contents -cne $updated) {
        $changed++
        if (-not $Check) { [IO.File]::WriteAllText($path, $updated, [Text.UTF8Encoding]::new($false)) }
    }
}
if ($Check -and $changed) { throw "$changed native projects differ from their complete reference WIDL recipes" }
"WIDL import: $recipes configuration recipes; $producers owners/consumers; $changed projects updated"
