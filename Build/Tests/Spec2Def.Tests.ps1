[CmdletBinding()]
param(
    [string]$MSBuild = "MSBuild.exe",
    [switch]$StaticOnly
)
$ErrorActionPreference = "Stop"
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot "..\.."))
$projectsRoot = Join-Path $root "Upstream\ReactOS\Projects"
$recipes = 0

function Expand-TestPath([string]$Value, [string]$ProjectDirectory, [string]$ConfigurationKey) {
    $configuration, $platform = $ConfigurationKey.Split("|")
    return [IO.Path]::GetFullPath($Value.Replace('$(ProjectDir)', "$ProjectDirectory\").
        Replace('$(KundowsRoot)', "$root\").Replace('$(Configuration)', $configuration).
        Replace('$(Platform)', $platform).Replace("/", "\"))
}
function Get-Options([string]$Arguments) {
    return @([regex]::Matches($Arguments, '(?:^|\s)(--[^\s]+|-[an]=[^\s]+)') |
        ForEach-Object { $_.Groups[1].Value })
}

# Compare committed native inputs with the original, already evaluated
# commands, including configurations which have no spec2def invocation.
foreach ($project in Get-ChildItem -LiteralPath $projectsRoot -Recurse -Filter "*.vcxproj" -File) {
    $recordPath = Join-Path $project.DirectoryName "$($project.BaseName).Migration.json"
    if (-not (Test-Path -LiteralPath $recordPath)) { continue }
    $record = Get-Content -LiteralPath $recordPath -Raw | ConvertFrom-Json
    $xml = [xml](Get-Content -LiteralPath $project.FullName -Raw)
    if ($xml.SelectNodes("//*[starts-with(local-name(), 'ReactOSSpec2DefModuleName') or local-name()='ReactOSSpec2DefDllName']").Count) {
        throw "Obsolete module-name override in $($project.FullName)"
    }
    foreach ($configuration in $record.Configurations.PSObject.Properties) {
        $key = $configuration.Name
        $expected = @($configuration.Value.PendingGenerationSteps | Where-Object { $_.HostTools -contains "spec2def" })
        $group = @($xml.Project.ItemGroup | Where-Object { $_.Label -eq "ReactOSSpec2Def" -and
            $_.Condition -ceq "'`$(Configuration)|`$(Platform)'=='$key'" })
        $actual = @($group | ForEach-Object { $_.ReactOSSpec2Def } | Where-Object { $_ })
        if ($actual.Count -ne $expected.Count) { throw "$($record.Target) ${key}: recipe count differs" }
        for ($i = 0; $i -lt $actual.Count; $i++) {
            $reference = $expected[$i].ReferenceCommand
            $options = @(Get-Options $reference)
            $nativeOptions = @(Get-Options $actual[$i].Arguments)
            if (($options -join "|") -cne ($nativeOptions -join "|")) {
                throw "$($record.Target) ${key}: module/architecture/export flags differ"
            }
            $source = [regex]::Match($reference, '(\S+\.spec)$').Groups[1].Value
            $originalSource = Expand-TestPath $source $project.DirectoryName $key
            $nativeSource = Expand-TestPath $actual[$i].Include $project.DirectoryName $key
            if ($originalSource -ine $nativeSource) { throw "$($record.Target) ${key}: wrong spec source" }
            $originalOutputs = @($expected[$i].Outputs | ForEach-Object { Expand-TestPath $_ $project.DirectoryName $key })
            $nativeOutputs = @($actual[$i].Outputs.Split(";") | ForEach-Object { Expand-TestPath $_ $project.DirectoryName $key })
            if (($originalOutputs -join "|") -ine ($nativeOutputs -join "|")) {
                throw "$($record.Target) ${key}: generated paths differ"
            }
            $isImport = $options -contains "--implib"
            if (@($group | ForEach-Object { $_.ReactOSSpec2DefStub } | Where-Object { $_ }).Count -ne [int]$isImport -or
                @($group | ForEach-Object { $_.ReactOSImportLibrary } | Where-Object { $_ }).Count -ne [int]$isImport) {
                throw "$($record.Target) ${key}: incomplete import-library dependency chain"
            }
            $recipes++
        }
    }
}
if (Test-Path -LiteralPath (Join-Path $root "Build\GenerateReactOSSpec2Def.ps1")) {
    throw "The build-time module-name inference script must not remain"
}
"spec2def metadata: $recipes upstream configuration recipes matched"
if ($StaticOnly) { exit 0 }

$MSBuild = (Get-Command $MSBuild -ErrorAction Stop).Source
$driver = Join-Path $PSScriptRoot "Spec2Def.Tests.proj"
$driverXml = [xml](Get-Content -LiteralPath $driver -Raw)
$generated = 0
foreach ($platform in @("Win32", "x64", "ARM64")) {
    foreach ($configuration in @("Debug", "Release")) {
        & $MSBuild $driver /nologo /m:4 /verbosity:minimal "/p:Configuration=$configuration" "/p:Platform=$platform"
        if ($LASTEXITCODE -ne 0) { throw "Native $platform $configuration spec2def generation failed" }
        foreach ($entry in $driverXml.Project.ItemGroup.NativeProject) {
            $projectPath = [IO.Path]::GetFullPath($entry.Include.Replace('$(MSBuildThisFileDirectory)', "$PSScriptRoot\"))
            $project = [xml](Get-Content -LiteralPath $projectPath -Raw)
            $group = $project.Project.ItemGroup | Where-Object { $_.Label -eq "ReactOSSpec2Def" -and
                $_.Condition -ceq "'`$(Configuration)|`$(Platform)'=='$configuration|$platform'" }
            foreach ($item in @($group | ForEach-Object { $_.ReactOSSpec2Def } | Where-Object { $_ })) {
                $module = [regex]::Match($item.Arguments, '(?:^|\s)-n=([^\s]+)').Groups[1].Value
                foreach ($output in $item.Outputs.Split(";")) {
                    $path = Expand-TestPath $output ([IO.Path]::GetDirectoryName($projectPath)) "$configuration|$platform"
                    if (-not (Test-Path -LiteralPath $path)) { throw "Missing generated file: $path" }
                    if ([IO.Path]::GetExtension($path) -eq ".def" -and
                        (Get-Content -LiteralPath $path -Raw) -notmatch "(?m)^NAME $([regex]::Escape($module))\r?$") {
                        throw "Wrong runtime module name in $path"
                    }
                }
                $generated++
            }
        }
    }
}

# A second run must not rewrite any output; deleting a declared output must
# regenerate the complete chain without a separate stamp-file mechanism.
$project = Join-Path $projectsRoot "Ntoskrnl\Libntoskrnl.vcxproj"
$directory = Join-Path $root "OutDir\Build\Generated\x64\Debug\ntoskrnl"
$names = @("libntoskrnl_implib.def", "libntoskrnl_stubs.asm", "libntoskrnl_impalias.asm",
    "libntoskrnl_stubs.asm.obj", "libntoskrnl_tmp.lib")
$before = @($names | ForEach-Object { (Get-Item -LiteralPath (Join-Path $directory $_)).LastWriteTimeUtc.Ticks })
& $MSBuild $project /nologo /verbosity:minimal /t:GenerateReactOSImportLibrary /p:Configuration=Debug /p:Platform=x64
if ($LASTEXITCODE -ne 0) { throw "Incremental generation failed" }
$after = @($names | ForEach-Object { (Get-Item -LiteralPath (Join-Path $directory $_)).LastWriteTimeUtc.Ticks })
if (($before -join "|") -cne ($after -join "|")) { throw "Incremental generation rewrote up-to-date files" }
Remove-Item -LiteralPath (Join-Path $directory "libntoskrnl_implib.def")
& $MSBuild $project /nologo /verbosity:minimal /t:GenerateReactOSImportLibrary /p:Configuration=Debug /p:Platform=x64
if ($LASTEXITCODE -ne 0) { throw "Missing-output regeneration failed" }
if (-not (Test-Path -LiteralPath (Join-Path $directory "libntoskrnl_implib.def"))) { throw "Deleted output was not regenerated" }
"spec2def native: $generated configuration builds passed; incremental and missing-output tests passed"
