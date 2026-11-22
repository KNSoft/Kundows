param(
    [Parameter(Mandatory = $true)]
    [string]$WidlTool,
    [Parameter(Mandatory = $true)]
    [string]$IdlFile,
    [Parameter(Mandatory = $true)]
    [string]$OutputFile,
    [Parameter(Mandatory = $true)]
    [ValidateSet("Win32", "x64", "ARM64")]
    [string]$Architecture,
    [ValidateSet("Iids", "Header")]
    [string]$Mode = "Iids",
    [string]$IncludeDirectoryList = ""
)
$ErrorActionPreference = "Stop"
$arguments = @("-nostdinc", "-D__REACTOS__", "-D_MIDL_USE_GUIDDEF_")
switch ($Architecture) {
    "Win32" { $arguments += @("-m32", "--win32", "-b", "i386-x-y") }
    "x64" { $arguments += @("-m64", "--win64", "-b", "amd64-x-y") }
    "ARM64" { $arguments += @("-b", "arm64-x-y") }
}
foreach ($directory in ($IncludeDirectoryList -split ";" | Where-Object { $_.Trim() })) {
    $arguments += "-I$($directory.Trim())"
}
$arguments += if ($Mode -eq "Header") { @("-Oicf", "-h") } else { @("-Os", "-u") }
$arguments += @("-o", $OutputFile, $IdlFile)
New-Item -ItemType Directory -Force -Path (Split-Path -Parent $OutputFile) | Out-Null
Push-Location -LiteralPath (Split-Path -Parent ([IO.Path]::GetFullPath($IdlFile)))
try {
    & $WidlTool @arguments
    $exitCode = $LASTEXITCODE
}
finally { Pop-Location }
exit $exitCode
