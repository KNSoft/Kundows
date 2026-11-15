#requires -Version 7.0
[CmdletBinding()]
param(
    [string[]]$Target,
    [ValidateSet('', 'x86', 'Win32', 'x64', 'ARM64')]
    [string]$Platform = '',
    [ValidateSet('Debug', 'Release')]
    [string]$Configuration = 'Debug',
    [ValidateRange(1, 32)]
    [int]$ThrottleLimit = 8,
    [ValidateRange(10, 1800)]
    [int]$TimeoutSeconds = 120
)

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $PSScriptRoot
$catalogPath = Join-Path $root 'Upstream\ReactOS\Projects\Migration.json'
$catalog = Get-Content -LiteralPath $catalogPath -Raw | ConvertFrom-Json -AsHashtable
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
$installation = & $vswhere -latest -products '*' -version '[18.0,19.0)' -requires Microsoft.Component.MSBuild -property installationPath
if (-not $installation) { throw 'Visual Studio 2026 / MSBuild 18 is required.' }
$msbuild = Join-Path $installation 'MSBuild\Current\Bin\amd64\MSBuild.exe'
if (-not (Test-Path -LiteralPath $msbuild -PathType Leaf)) { throw "MSBuild not found: $msbuild" }
$version = (& $msbuild -version -nologo | Out-String).Trim()
$projects = @($catalog.Projects)
if ($Target) {
    $projects = @($projects | Where-Object { $_.Target -in $Target })
    $unknown = @($Target | Where-Object { $_ -notin $projects.Target })
    if ($unknown) { throw "Unknown targets: $($unknown -join ', ')" }
}
if ($Platform -eq 'x86') { $Platform = 'Win32' }
if (-not (Test-Path -LiteralPath $root -PathType Container)) { throw 'Repository root is missing.' }
$logRoot = Join-Path $root 'OutDir\ModuleBuildLogs'
[void][IO.Directory]::CreateDirectory($logRoot)

$results = @($projects | ForEach-Object -Parallel {
    $entry = $_
    $repo = $using:root
    $projectPath = Join-Path $repo $entry.Project
    $recordPath = Join-Path $repo $entry.Record
    $record = [IO.File]::ReadAllText($recordPath) | ConvertFrom-Json -AsHashtable
    $projectHash = (Get-FileHash -LiteralPath $projectPath -Algorithm SHA256).Hash.ToLowerInvariant()
    $requestedPlatform = $using:Platform
    $configuration = $using:Configuration
    $platforms = if ($requestedPlatform) { @($requestedPlatform) } else { @('Win32', 'x64', 'ARM64') }
    $selected = $null
    foreach ($candidate in $platforms) {
        $candidateKey = "$configuration|$candidate"
        if ($record.Configurations[$candidateKey].AvailableInBaseline) {
            $selected = $candidate
            break
        }
    }
    if (-not $selected) {
        return @{ Target = $entry.Target; Status = 'NotApplicable'; Configuration = $configuration; Platform = $requestedPlatform }
    }
    $cfgKey = "$configuration|$selected"
    $folder = Join-Path $using:logRoot "$selected\$configuration"
    [void][IO.Directory]::CreateDirectory($folder)
    $stem = [IO.Path]::GetFileNameWithoutExtension($projectPath)
    $log = Join-Path $folder "$stem.log"
    $arguments = @(
        $projectPath, '/t:Build', '/m:1', '/nologo', '/verbosity:quiet',
        "/p:Configuration=$configuration", "/p:Platform=$selected", '/p:BuildProjectReferences=false',
        '/p:CL_MPCount=1', '/p:PreferredUILang=en-US',
        "/flp:LogFile=$log;Encoding=UTF-8;Verbosity=normal"
    )
    $start = [Diagnostics.ProcessStartInfo]::new()
    $start.FileName = $using:msbuild
    $start.WorkingDirectory = $repo
    $start.UseShellExecute = $false
    $start.CreateNoWindow = $true
    $start.RedirectStandardOutput = $true
    $start.RedirectStandardError = $true
    $start.Environment['VSLANG'] = '1033'
    foreach ($argument in $arguments) { $start.ArgumentList.Add($argument) }
    $process = [Diagnostics.Process]::new()
    $process.StartInfo = $start
    $watch = [Diagnostics.Stopwatch]::StartNew()
    $status = 'InvocationFailed'
    $exitCode = $null
    $diagnostic = ''
    try {
        [void]$process.Start()
        $stdout = $process.StandardOutput.ReadToEndAsync()
        $stderr = $process.StandardError.ReadToEndAsync()
        if (-not $process.WaitForExit($using:TimeoutSeconds * 1000)) {
            $process.Kill($true)
            $process.WaitForExit()
            $status = 'TimedOut'
        } else {
            $exitCode = $process.ExitCode
            $status = if ($exitCode -eq 0) { 'Succeeded' } else { 'Failed' }
        }
        $diagnostic = $stdout.GetAwaiter().GetResult() + $stderr.GetAwaiter().GetResult()
    } catch {
        $diagnostic = $_.Exception.Message
    } finally {
        $watch.Stop()
        $process.Dispose()
    }
    $text = if ([IO.File]::Exists($log)) { [IO.File]::ReadAllText($log) } else { $diagnostic }
    $errors = @($text -split '\r?\n' | Where-Object {
        $_ -match '(?i)(?:fatal error|error|错误)\s+(?:[A-Z]+\d{4,5})\b'
    } | ForEach-Object { $_.Trim().Replace($repo + '\', '').Replace($repo + '/', '') } | Select-Object -Unique)
    $codes = @([regex]::Matches(($errors -join "`n"), '\b(?:C|D|LNK|MSB|RC|A|U)\d{4,5}\b') |
        ForEach-Object { $_.Value } | Sort-Object -Unique)
    $missing = @([regex]::Matches(($errors -join "`n"), "(?i)(?:include file|source file|input file|file):?\s*['`"]([^'`"]+)['`"]") |
        ForEach-Object { $_.Groups[1].Value } | Sort-Object -Unique)
    $missing += @([regex]::Matches(($errors -join "`n"), '(?im)cannot open file\s*:\s*(.+?)(?=\s*\[|$)') |
        ForEach-Object { $_.Groups[1].Value.Trim() } | Sort-Object -Unique)
    $missing = @($missing | Sort-Object -Unique)
    $missing = @($missing | ForEach-Object {
        if ($_ -match '[\\/]') {
            $path = [IO.Path]::GetFullPath($_, $repo)
            if ($path.StartsWith($repo + [IO.Path]::DirectorySeparatorChar, [StringComparison]::OrdinalIgnoreCase)) {
                [IO.Path]::GetRelativePath($repo, $path).Replace('\', '/')
            } else { $_ }
        } else { $_ }
    } | Sort-Object -Unique)
    $rawBoot = $record.Configurations[$cfgKey].CMakeTargetType -eq 'UTILITY'
    $output = $record.Configurations[$cfgKey].Output
    $artifact = Join-Path $repo "OutDir\$selected\$configuration\$stem\$output"
    $artifactExists = -not $rawBoot -and [IO.File]::Exists($artifact)
    if ($status -eq 'Succeeded' -and -not $artifactExists) { $status = 'NoArtifact' }
    $summary = if ($errors.Count) { @($errors[0..([Math]::Min($errors.Count, 12) - 1)]) } else { @() }
    $summary = @($summary | ForEach-Object { if ($_.Length -gt 1200) { $_.Substring(0, 1200) + ' [see full log]' } else { $_ } })
    if (-not $errors.Count -and $diagnostic.Trim() -and $status -eq 'InvocationFailed') { $summary = @($diagnostic.Trim()) }
    $result = [ordered]@{
        Target = $entry.Target
        Status = $status
        Configuration = $configuration
        Platform = $selected
        IndependentProjectBuild = $true
        ProjectSHA256 = $projectHash
        MatchesCurrentProject = $true
        ExitCode = $exitCode
        DurationSeconds = [Math]::Round($watch.Elapsed.TotalSeconds, 2)
        TimestampUtc = [DateTime]::UtcNow.ToString('o')
        MSBuildVersion = $using:version
        Arguments = @($arguments | ForEach-Object { $_.Replace($repo + '\', '').Replace($repo + '/', '') })
        Log = [IO.Path]::GetRelativePath($repo, $log).Replace('\', '/')
        ErrorCodes = $codes
        UniqueErrorCount = $errors.Count
        ErrorSummary = $summary
        ObservedMissingFiles = $missing
        RequiredHostTools = @($record.Configurations[$cfgKey].RequiredHostTools)
        RequiredInfrastructure = @($record.Configurations[$cfgKey].RequiredInfrastructure)
        PendingBuildInfrastructure = @($record.Deferred)
        ArtifactExists = $artifactExists
        Artifact = if ($artifactExists) { [IO.Path]::GetRelativePath($repo, $artifact).Replace('\', '/') } else { $null }
    }
    $record.Configurations[$cfgKey].TrialBuild = $result
    $record.TrialBuild = $result
    $json = $record | ConvertTo-Json -Depth 100
    [IO.File]::WriteAllText($recordPath, $json + "`r`n", [Text.UTF8Encoding]::new($false))
    $result
} -ThrottleLimit $ThrottleLimit)

$summary = [ordered]@{
    TimestampUtc = [DateTime]::UtcNow.ToString('o')
    Attempted = @($results | Where-Object { $_.Status -ne 'NotApplicable' }).Count
    Succeeded = @($results | Where-Object { $_.Status -eq 'Succeeded' }).Count
    Failed = @($results | Where-Object { $_.Status -eq 'Failed' }).Count
    TimedOut = @($results | Where-Object { $_.Status -eq 'TimedOut' }).Count
    InvocationFailed = @($results | Where-Object { $_.Status -eq 'InvocationFailed' }).Count
    NoArtifact = @($results | Where-Object { $_.Status -eq 'NoArtifact' }).Count
    NotApplicable = @($results | Where-Object { $_.Status -eq 'NotApplicable' }).Count
}
$catalog.LastTrialRun = $summary
$byTarget = @{}
foreach ($result in $results) { $byTarget[$result.Target] = $result }
foreach ($entry in $catalog.Projects) {
    if ($byTarget.ContainsKey($entry.Target) -and $byTarget[$entry.Target].Status -ne 'NotApplicable') {
        $result = $byTarget[$entry.Target]
        $entry.TrialBuildStatus = $result.Status
        $entry.TrialBuildConfiguration = "$($result.Configuration)|$($result.Platform)"
        $entry.TrialBuildLog = $result.Log
    } elseif (-not $entry.Contains('TrialBuildStatus')) {
        $saved = [IO.File]::ReadAllText((Join-Path $root $entry.Record)) | ConvertFrom-Json -AsHashtable
        $entry.TrialBuildStatus = $saved.TrialBuild.Status
    }
}
$catalog.TrialBuildSummary = [ordered]@{
    TimestampUtc = [DateTime]::UtcNow.ToString('o')
    Attempted = @($catalog.Projects | Where-Object { $_.TrialBuildStatus -ne 'NotAttempted' }).Count
    Succeeded = @($catalog.Projects | Where-Object { $_.TrialBuildStatus -eq 'Succeeded' }).Count
    Failed = @($catalog.Projects | Where-Object { $_.TrialBuildStatus -eq 'Failed' }).Count
    TimedOut = @($catalog.Projects | Where-Object { $_.TrialBuildStatus -eq 'TimedOut' }).Count
    InvocationFailed = @($catalog.Projects | Where-Object { $_.TrialBuildStatus -eq 'InvocationFailed' }).Count
    NoArtifact = @($catalog.Projects | Where-Object { $_.TrialBuildStatus -eq 'NoArtifact' }).Count
}
$catalog | ConvertTo-Json -Depth 100 | Set-Content -LiteralPath $catalogPath -Encoding utf8
$summary | ConvertTo-Json
