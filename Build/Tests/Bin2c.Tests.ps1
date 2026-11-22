[CmdletBinding()]
param([string]$MSBuild = "MSBuild.exe")
$ErrorActionPreference = "Stop"
$root = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot "..\.."))
& "$root\Build\ImportReactOSGenerators.ps1" -Generator Bin2c -Check
$cases = 0
foreach ($relative in @("Hal\Halx86\PciData", "Boot\Freeldr\Install\Installfreeldr", "Subsystems\Mvdm\Ntvdm\Ntvdm")) {
    $project = "$root\Upstream\ReactOS\Projects\$relative.vcxproj"
    $xml = [xml](Get-Content -LiteralPath $project -Raw)
    foreach ($group in @($xml.Project.ItemGroup | Where-Object { $_.Label -eq "ReactOSBin2c" })) {
        $null = $group.Condition -match "=='(?<configuration>[^|]+)\|(?<platform>[^']+)'"
        $platform = $Matches.platform
        $configuration = $Matches.configuration
        $options = @("/nologo", "/v:quiet", "/t:GenerateReactOSBin2cProjectFiles", "/p:BuildProjectReferences=false", "/p:Platform=$platform", "/p:Configuration=$configuration")
        & $MSBuild $project @options
        if ($LASTEXITCODE) { throw "bin2c failed: $relative $platform $configuration" }
        $outputs = @($group.ReactOSBin2c | ForEach-Object {
            $item = $_
            $source = $item.Include.Replace('$(KundowsRoot)', "$root\").Replace('$(Platform)', $platform).Replace('$(Configuration)', $configuration)
            $files = @($item.Outputs.Split(";") | ForEach-Object { $_.Replace('$(KundowsRoot)', "$root\").Replace('$(Platform)', $platform).Replace('$(Configuration)', $configuration) })
            $nullTerminate = [int]($item.Arguments -match "\bBINSTR\b")
            $length = (Get-Item -LiteralPath $source).Length + $nullTerminate
            if ((Get-Content -LiteralPath $files[1] -Raw) -notmatch "_SIZE $length\b") { throw "Incorrect bin2c array length: $source" }
            if ($nullTerminate -and (Get-Content -LiteralPath $files[0] -Raw) -notmatch 'DATA_SEG\("INITDATA"\) unsigned char') {
                throw "Lost bin2c array attribute"
            }
            $cases++
            $files
        })
        $before = @($outputs | ForEach-Object { (Get-Item -LiteralPath $_).LastWriteTimeUtc.Ticks })
        & $MSBuild $project @options
        if ($LASTEXITCODE) { throw "Incremental bin2c failed" }
        $after = @($outputs | ForEach-Object { (Get-Item -LiteralPath $_).LastWriteTimeUtc.Ticks })
        if (($before -join "|") -cne ($after -join "|")) { throw "bin2c rewrote unchanged outputs" }
        Remove-Item -LiteralPath $outputs[1]
        & $MSBuild $project @options
        if ($LASTEXITCODE -or -not (Test-Path -LiteralPath $outputs[1])) { throw "bin2c did not regenerate a missing header" }
    }
}
$temporary = Join-Path $env:TEMP ("kundows-bin2c-" + [guid]::NewGuid())
$null = New-Item -ItemType Directory -Path $temporary
try {
    $options = @("/nologo", "/v:quiet", "/p:FixtureRoot=$temporary")
    [IO.File]::WriteAllBytes("$temporary\input.bin", [byte[]](1, 127, 255))
    & $MSBuild "$PSScriptRoot\Bin2c.Tests.proj" @options
    if ($LASTEXITCODE) { throw "bin2c fixture failed" }
    & "$root\OutDir\HostTools\x64\Debug\bin2c.exe" "$temporary\input.bin" "$temporary\reference.c" "$temporary\reference.h" BINSTR FixtureData 'DATA_SEG("INITDATA")' attribute.h
    if ($LASTEXITCODE) { throw "Direct upstream bin2c failed" }
    $actual = (Get-Content "$temporary\output.c" -Raw).Replace("$temporary\output.h", "$temporary\reference.h")
    if ($actual -cne (Get-Content "$temporary\reference.c" -Raw) -or
        (Get-FileHash "$temporary\output.h").Hash -ne (Get-FileHash "$temporary\reference.h").Hash) { throw "bin2c output differs from upstream" }
    [IO.File]::WriteAllBytes("$temporary\input.bin", [byte[]](2, 3, 4, 5))
    & $MSBuild "$PSScriptRoot\Bin2c.Tests.proj" @options
    if ($LASTEXITCODE -or (Get-Content "$temporary\output.h" -Raw) -notmatch "_SIZE 5\b") { throw "bin2c left stale output after an input change" }
    Remove-Item -LiteralPath "$temporary\input.bin"
    & $MSBuild "$PSScriptRoot\Bin2c.Tests.proj" @options > "$temporary\failure.log"
    if ($LASTEXITCODE -eq 0) { throw "bin2c concealed a missing input" }
}
finally { Remove-Item -LiteralPath $temporary -Recurse -Force }
"bin2c: $cases configuration recipes passed; upstream parity, attributes/NUL, incremental, deleted-output, changed-input, and failure tests passed"
exit 0
