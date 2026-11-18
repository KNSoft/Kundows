param(
    [Parameter(Mandatory = $true)]
    [string]$SourceRoot,
    [Parameter(Mandatory = $true)]
    [string]$GeneratedRoot,
    [Parameter(Mandatory = $true)]
    [string]$Compiler
)

$ErrorActionPreference = "Stop"

$sourceFile = Join-Path $SourceRoot "sdk\tools\gendib\gendib.c"
$outputDirectory = Join-Path $GeneratedRoot "win32ss\gdi\dib"
$toolDirectory = Join-Path $GeneratedRoot "ReactOSHostTools"
$toolPath = Join-Path $toolDirectory "gendib.exe"
$objectPath = Join-Path $toolDirectory "gendib.obj"

if (-not (Test-Path -LiteralPath $sourceFile -PathType Leaf)) {
    throw "gendib source was not found: $sourceFile"
}
if (-not (Test-Path -LiteralPath $Compiler -PathType Leaf)) {
    throw "MSVC compiler was not found: $Compiler"
}

New-Item -ItemType Directory -Force -Path $outputDirectory,$toolDirectory | Out-Null
$compilerItem = Get-Item -LiteralPath $Compiler
$msvcRoot = $compilerItem.Directory.Parent.Parent.Parent.FullName
$msvcInclude = Join-Path $msvcRoot "include"
$compilerArchitecture = $compilerItem.Directory.Name
$windowsKitRoot = $env:WindowsSdkDir
$windowsKitVersion = $env:WindowsSDKVersion
if (-not $windowsKitRoot -or -not $windowsKitVersion) {
    $windowsKits = Join-Path ${env:ProgramFiles(x86)} "Windows Kits\10\Include"
    $latestWindowsKit = Get-ChildItem -LiteralPath $windowsKits -Directory -ErrorAction SilentlyContinue |
        Where-Object { $_.Name -match "^\d+\.\d+\.\d+\.\d+$" } |
        Sort-Object Name -Descending |
        Select-Object -First 1
    if ($latestWindowsKit) {
        $windowsKitRoot = Split-Path -Parent (Split-Path -Parent $latestWindowsKit.FullName)
        $windowsKitVersion = $latestWindowsKit.Name
    }
}

$includeArguments = @("/I$msvcInclude")
$libraryArguments = @("/LIBPATH:$msvcRoot\lib\$compilerArchitecture")
if ($windowsKitRoot -and $windowsKitVersion) {
    $windowsKitInclude = Join-Path $windowsKitRoot "Include\$windowsKitVersion"
    $windowsKitLib = Join-Path $windowsKitRoot "Lib\$windowsKitVersion"
    $includeArguments += @(
        "/I$(Join-Path $windowsKitInclude 'ucrt')"
        "/I$(Join-Path $windowsKitInclude 'shared')"
        "/I$(Join-Path $windowsKitInclude 'um')"
    )
    $libraryArguments += @(
        "/LIBPATH:$(Join-Path $windowsKitLib "ucrt\$compilerArchitecture")"
        "/LIBPATH:$(Join-Path $windowsKitLib "um\$compilerArchitecture")"
    )
}

& $Compiler /nologo /O2 /MT @includeArguments "/Fo$objectPath" "/Fe$toolPath" $sourceFile /link @libraryArguments
if ($LASTEXITCODE -ne 0) {
    throw "gendib compilation failed with exit code $LASTEXITCODE."
}

& $toolPath $outputDirectory
if ($LASTEXITCODE -ne 0) {
    throw "gendib failed with exit code $LASTEXITCODE."
}

foreach ($name in @("dib8gen.c", "dib16gen.c", "dib32gen.c")) {
    $output = Join-Path $outputDirectory $name
    if (-not (Test-Path -LiteralPath $output -PathType Leaf)) {
        throw "gendib did not generate: $output"
    }
}
