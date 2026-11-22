[CmdletBinding()]
param(
    [string]$MSBuild = "MSBuild.exe",
    [string]$ReferenceRoot = ""
)
$ErrorActionPreference = "Stop"
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot "..\.."))
$sourceRoot = "$root\Upstream\ReactOS\Source".Replace("\", "/")
if ($ReferenceRoot) { & "$root\Build\ImportReactOSWidl.ps1" -ReferenceRoot $ReferenceRoot -Check }
$temporary = Join-Path $env:TEMP ("kundows-widl-" + [guid]::NewGuid())
$null = New-Item -ItemType Directory -Path $temporary
$cases = 0
$parity = 0
function Expand-Value([string]$Value, [string]$GeneratedRoot) {
    return $Value.Replace('$(ReactOSWidlSourceRoot)', $sourceRoot).Replace('$(ReactOSWidlGeneratedRoot)', $GeneratedRoot)
}
try {
    $projects = @(Get-ChildItem "$root\Upstream\ReactOS\Projects" -Recurse -Filter "*.vcxproj" -File |
        Where-Object { [IO.File]::ReadAllText($_.FullName).Contains('Label="ReactOSWidl"') } |
        ForEach-Object { [pscustomobject]@{ Path = $_.FullName; Name = $_.Name; Xml = [xml](Get-Content -LiteralPath $_.FullName -Raw) } })
    $prepare = ($projects | Where-Object { $_.Name -eq "ReactOSPrepare.vcxproj" }).Path
    foreach ($platform in @("Win32", "x64", "ARM64")) {
        foreach ($configuration in @("Debug", "Release")) {
            $hostPlatform = if ($platform -eq "ARM64") { "x64" } else { $platform }
            $tool = "$root\OutDir\HostTools\$hostPlatform\$configuration\widl.exe"
            $generatedRoot = "$root/OutDir/Build/Generated/$platform/$configuration".Replace("\", "/")
            $rows = @($projects | ForEach-Object {
                $group = @($_.Xml.Project.ItemGroup | Where-Object { $_.Label -eq "ReactOSWidl" -and $_.Condition -eq "'`$(Configuration)|`$(Platform)'=='$configuration|$platform'" })
                $items = @($group.ReactOSWidl | Where-Object { $_ })
                if ($items.Count) { [pscustomobject]@{ Project = $_; Items = $items } }
            })
            $options = @("/nologo", "/v:quiet", "/p:BuildProjectReferences=false", "/p:Platform=$platform", "/p:Configuration=$configuration")
            $list = "$temporary/projects.txt"
            @($rows.Project | Where-Object { $_.Path -ne $prepare } | ForEach-Object { $_.Path }) | Set-Content -LiteralPath $list
            # Generate shared SDK prerequisites first, then independent owners.
            & $MSBuild $prepare @options /t:GenerateReactOSWidl
            if ($LASTEXITCODE) { throw "Shared WIDL generation failed: $platform $configuration" }
            & $MSBuild "$PSScriptRoot\Widl.Generation.Tests.proj" @options /m:4 "/p:ProjectList=$list"
            if ($LASTEXITCODE) { throw "WIDL generation failed: $platform $configuration" }
            $outputs = @($rows.Items | ForEach-Object { $_.Outputs.Split(";") } | ForEach-Object { Expand-Value $_ $generatedRoot })
            $before = @($outputs | ForEach-Object { (Get-Item -LiteralPath $_).LastWriteTimeUtc.Ticks })
            & $MSBuild $prepare @options /t:GenerateReactOSWidl
            if ($LASTEXITCODE) { throw "Incremental shared WIDL generation failed" }
            & $MSBuild "$PSScriptRoot\Widl.Generation.Tests.proj" @options /m:4 "/p:ProjectList=$list"
            if ($LASTEXITCODE) { throw "Incremental WIDL generation failed" }
            $after = @($outputs | ForEach-Object { (Get-Item -LiteralPath $_).LastWriteTimeUtc.Ticks })
            if (($before -join "|") -cne ($after -join "|")) { throw "WIDL rewrote unchanged outputs: $platform $configuration" }
            foreach ($row in $rows | Where-Object { $_.Project.Name -in @("ReactOSPrepare.vcxproj", "Uuid.vcxproj", "Advapi32RpcHeaders.vcxproj", "Actxprxy.vcxproj", "Oleaut32.vcxproj", "Netapi32.vcxproj") }) {
                foreach ($item in $row.Items | Select-Object -First 3) {
                    $nativeOutputs = @($item.Outputs.Split(";") | ForEach-Object { Expand-Value $_ $generatedRoot })
                    $referenceDirectory = "$temporary/reference".Replace("\", "/")
                    $null = New-Item -ItemType Directory -Force -Path $referenceDirectory
                    $arguments = @(Expand-Value $item.Arguments $generatedRoot)
                    if ($item.HeaderArguments) { $arguments += Expand-Value $item.HeaderArguments $generatedRoot }
                    foreach ($command in $arguments) {
                        foreach ($output in $nativeOutputs) {
                            $command = $command.Replace($output, "$referenceDirectory/$([IO.Path]::GetFileName($output))")
                        }
                        Push-Location (Expand-Value $item.WorkingDirectory $generatedRoot)
                        try {
                            & cmd.exe /d /c "`"$tool`" $command"
                            if ($LASTEXITCODE) { throw "Direct upstream WIDL command failed" }
                        }
                        finally { Pop-Location }
                    }
                    foreach ($output in $nativeOutputs) {
                        if ((Get-FileHash -LiteralPath $output).Hash -ne (Get-FileHash "$referenceDirectory/$([IO.Path]::GetFileName($output))").Hash) {
                            throw "Native WIDL output differs from upstream: $output"
                        }
                    }
                    $parity++
                }
                $last = Expand-Value ($row.Items[-1].Outputs.Split(";")[-1]) $generatedRoot
                Remove-Item -LiteralPath $last
                & $MSBuild $row.Project.Path @options /t:GenerateReactOSWidl
                if ($LASTEXITCODE -or -not (Test-Path -LiteralPath $last)) { throw "WIDL missing-output regeneration failed" }
            }
            $count = @($rows.Items).Count
            $cases += $count
            "WIDL ${platform} ${configuration}: $count recipes; incremental, parity and missing-output checks passed"
        }
    }
    foreach ($script in @("GenerateReactOSWidl.ps1", "GenerateReactOSUuidIids.ps1")) {
        if (Test-Path "$root\Build\$script") { throw "Obsolete WIDL adapter remains: $script" }
    }
    [IO.File]::WriteAllText("$temporary/input.idl", '[uuid(12345678-1234-5678-90ab-1234567890ab)] interface IAuditNative {};')
    $options = @("/nologo", "/v:quiet", "/p:FixtureRoot=$temporary", "/p:ModeFlags=-Os -u", "/p:ArchitectureFlags=-m64 --win64 -b amd64-x-y", "/p:HeaderCommand=--invalid-option")
    & $MSBuild "$PSScriptRoot\Widl.Tests.proj" @options > "$temporary/failure.log"
    if ($LASTEXITCODE -eq 0 -or (Test-Path "$temporary/header")) { throw "WIDL concealed a second-pass failure" }
    & "$PSScriptRoot\UuidIids.Tests.ps1" -MSBuild $MSBuild
}
finally { Remove-Item -LiteralPath $temporary -Recurse -Force }
"WIDL: $cases configuration recipes generated; $parity direct-output parity cases; incremental, missing-output, and two-pass failure tests passed"
exit 0
