[CmdletBinding()]
param([string]$MSBuild = "MSBuild.exe")
$ErrorActionPreference = "Stop"
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot "..\.."))
& "$root\Build\ImportReactOSGenerators.ps1" -Generator BootSector -Check
$temporary = Join-Path $env:TEMP ("kundows-obj2bin-" + [guid]::NewGuid())
$null = New-Item -ItemType Directory -Path $temporary
$cases = 0
function Expand-Path([string]$Value, [string]$Platform, [string]$Configuration) {
    return $Value.Replace('$(KundowsRoot)', "$root\").Replace('$(Platform)', $Platform).Replace('$(Configuration)', $Configuration)
}
try {
    foreach ($project in Get-ChildItem "$root\Upstream\ReactOS\Projects" -Recurse -Filter "*.vcxproj" -File) {
        $xml = [xml](Get-Content -LiteralPath $project.FullName -Raw)
        foreach ($group in @($xml.Project.ItemGroup | Where-Object { $_.Label -eq "ReactOSBootSector" })) {
            $null = $group.Condition -match "=='(?<configuration>[^|]+)\|(?<platform>[^']+)'"
            $platform = $Matches.platform
            $configuration = $Matches.configuration
            $options = @("/nologo", "/v:minimal", "/t:GenerateReactOSBootSectorProjectFiles", "/p:BuildProjectReferences=false", "/p:Platform=$platform", "/p:Configuration=$configuration")
            & $MSBuild $project.FullName @options
            if ($LASTEXITCODE) { throw "Raw binary generation failed: $($project.Name) $platform $configuration" }
            $hostPlatform = if ($platform -eq "ARM64") { "x64" } else { $platform }
            $object = Expand-Path $group.ReactOSBootConvert.Include $platform $configuration
            $output = Expand-Path $group.ReactOSBootConvert.Outputs $platform $configuration
            $base = ($group.ReactOSBootConvert.Arguments -split " ")[-1]
            & "$root\OutDir\HostTools\$hostPlatform\$configuration\obj2bin.exe" $object "$temporary\reference.bin" $base
            if ($LASTEXITCODE -or (Get-FileHash $output).Hash -ne (Get-FileHash "$temporary\reference.bin").Hash) {
                throw "Native obj2bin differs from upstream: $output"
            }
            $outputs = @("ReactOSAsm16Copy", "ReactOSBootPreprocess", "ReactOSBootAssemble", "ReactOSBootConvert" |
                ForEach-Object { $group.$_ } | Where-Object { $_ } |
                ForEach-Object { Expand-Path $_.Outputs $platform $configuration })
            $before = @($outputs | ForEach-Object { (Get-Item -LiteralPath $_).LastWriteTimeUtc.Ticks })
            & $MSBuild $project.FullName @options
            if ($LASTEXITCODE) { throw "Incremental raw binary generation failed" }
            $after = @($outputs | ForEach-Object { (Get-Item -LiteralPath $_).LastWriteTimeUtc.Ticks })
            if (($before -join "|") -cne ($after -join "|")) { throw "Raw binary generation rewrote unchanged outputs" }
            Remove-Item -LiteralPath $output
            & $MSBuild $project.FullName @options
            if ($LASTEXITCODE -or (Get-FileHash $output).Hash -ne (Get-FileHash "$temporary\reference.bin").Hash) {
                throw "Raw binary missing-output regeneration failed"
            }
            $cases++
        }
    }
    $project = "$root\Upstream\ReactOS\Projects\Boot\Freeldr\Bootsect\Fat.vcxproj"
    & $MSBuild $project /nologo /v:quiet /t:GenerateReactOSBootSectorProjectFiles /p:BuildProjectReferences=false /p:Platform=x64 /p:Configuration=Debug "/p:ReactOSObj2BinTool=$temporary\missing.exe" > "$temporary\failure.log"
    if ($LASTEXITCODE -eq 0) { throw "Raw binary generation concealed a missing obj2bin tool" }
}
finally { Remove-Item -LiteralPath $temporary -Recurse -Force }
"raw binary: $cases native configuration chains match upstream; incremental, missing-output, and failure tests passed"
exit 0
