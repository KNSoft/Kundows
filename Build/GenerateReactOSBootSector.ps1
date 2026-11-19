param(
    [Parameter(Mandatory = $true)]
    [string]$SourceRoot,
    [Parameter(Mandatory = $true)]
    [string]$GeneratedRoot,
    [Parameter(Mandatory = $true)]
    [string]$CMakeListFile,
    [Parameter(Mandatory = $true)]
    [string]$SourceDirectory,
    [Parameter(Mandatory = $true)]
    [string]$Compiler,
    [Parameter(Mandatory = $true)]
    [string]$Assembler
)

$ErrorActionPreference = "Stop"

function Expand-CMakeValue([string]$Value, [hashtable]$Variables) {
    for ($pass = 0; $pass -lt 8; $pass++) {
        $expanded = [regex]::Replace($Value, "\$\{(?<name>[A-Za-z_][A-Za-z0-9_]*)\}", {
            param($match)
            $name = $match.Groups["name"].Value
            if ($Variables.ContainsKey($name)) { return $Variables[$name] }
            return $match.Value
        })
        if ($expanded -eq $Value) { break }
        $Value = $expanded
    }
    return $Value
}

function Resolve-CMakePath([string]$Value, [string]$BaseDirectory) {
    $value = $Value.Trim().Trim('"', "'")
    if (-not $value -or $value -match "\$\{") {
        return $null
    }
    if ([IO.Path]::IsPathRooted($value)) {
        return [IO.Path]::GetFullPath($value)
    }
    return [IO.Path]::GetFullPath((Join-Path $BaseDirectory $value))
}

function Read-U16([byte[]]$Bytes, [int]$Offset) {
    return [BitConverter]::ToUInt16($Bytes, $Offset)
}

function Read-U32([byte[]]$Bytes, [int]$Offset) {
    return [BitConverter]::ToUInt32($Bytes, $Offset)
}

function Add-U16([uint16]$Left, [uint32]$Right) {
    return [uint16]((([uint32]$Left + $Right) -band 0xffff))
}

function Add-U32([uint32]$Left, [uint32]$Right) {
    return [uint32]((([uint64]$Left + $Right) -band 0xffffffff))
}

function Convert-ObjectToBinary([string]$ObjectFile, [string]$BinaryFile, [uint32]$BaseAddress) {
    $bytes = [IO.File]::ReadAllBytes($ObjectFile)
    if ($bytes.Length -lt 20) {
        throw "The boot sector object is too small: $ObjectFile"
    }

    $sectionCount = Read-U16 $bytes 2
    $symbolTableOffset = Read-U32 $bytes 8
    $symbolCount = Read-U32 $bytes 12
    $optionalHeaderSize = Read-U16 $bytes 16
    $sectionOffset = 20 + $optionalHeaderSize
    $sections = @()
    for ($index = 0; $index -lt $sectionCount; $index++) {
        $offset = $sectionOffset + $index * 40
        $sections += [ordered]@{
            VirtualAddress = Read-U32 $bytes ($offset + 12)
            RawSize = Read-U32 $bytes ($offset + 16)
            RawOffset = Read-U32 $bytes ($offset + 20)
            RelocOffset = Read-U32 $bytes ($offset + 24)
            RelocCount = Read-U16 $bytes ($offset + 32)
            Characteristics = Read-U32 $bytes ($offset + 36)
        }
    }

    $symbols = @()
    for ($index = 0; $index -lt $symbolCount; $index++) {
        $offset = [int]$symbolTableOffset + $index * 18
        $symbols += [ordered]@{ Value = Read-U32 $bytes ($offset + 8) }
    }

    $output = New-Object System.Collections.Generic.List[byte]
    $currentBaseAddress = [uint32]$BaseAddress
    foreach ($section in $sections) {
        $isCode = (($section.Characteristics -band 0x20) -ne 0) -and
            (($section.Characteristics -band 0x20000000) -ne 0)
        if (-not $isCode -or $section.RawSize -eq 0) {
            continue
        }

        $data = New-Object byte[] ([int]$section.RawSize)
        [Array]::Copy($bytes, [int]$section.RawOffset, $data, 0, [int]$section.RawSize)
        for ($relocIndex = 0; $relocIndex -lt $section.RelocCount; $relocIndex++) {
            $relocOffset = [int]$section.RelocOffset + $relocIndex * 10
            $virtualAddress = Read-U32 $bytes $relocOffset
            $symbolIndex = Read-U32 $bytes ($relocOffset + 4)
            $relocType = Read-U16 $bytes ($relocOffset + 8)
            $relativeOffset = [int]$virtualAddress - [int]$section.VirtualAddress
            if ($relativeOffset -lt 0 -or $relativeOffset -ge $data.Length -or $symbolIndex -ge $symbols.Count) {
                continue
            }
            $value = [uint32]($symbols[$symbolIndex].Value + $currentBaseAddress)
            switch ($relocType) {
                0 {
                    if ($relativeOffset + 2 -le $data.Length) {
                        $existing = [BitConverter]::ToUInt16($data, $relativeOffset)
                        [Array]::Copy([BitConverter]::GetBytes((Add-U16 $existing $value)), 0, $data, $relativeOffset, 2)
                    }
                }
                1 {
                    if ($relativeOffset + 2 -le $data.Length) {
                        $existing = [BitConverter]::ToUInt16($data, $relativeOffset)
                        [Array]::Copy([BitConverter]::GetBytes((Add-U16 $existing $value)), 0, $data, $relativeOffset, 2)
                    }
                }
                2 {
                    if ($relativeOffset + 2 -le $data.Length) {
                        $existing = [BitConverter]::ToUInt16($data, $relativeOffset)
                        [Array]::Copy([BitConverter]::GetBytes((Add-U16 $existing $value)), 0, $data, $relativeOffset, 2)
                    }
                }
                6 {
                    if ($relativeOffset + 4 -le $data.Length) {
                        $existing = [BitConverter]::ToUInt32($data, $relativeOffset)
                        [Array]::Copy([BitConverter]::GetBytes((Add-U32 $existing $value)), 0, $data, $relativeOffset, 4)
                    }
                }
                16 {
                    if ($relativeOffset + 2 -le $data.Length) {
                        $existing = [BitConverter]::ToUInt16($data, $relativeOffset)
                        [Array]::Copy([BitConverter]::GetBytes((Add-U16 $existing $value)), 0, $data, $relativeOffset, 2)
                    }
                }
                default {
                    throw "Unsupported boot sector relocation type $relocType in $ObjectFile"
                }
            }
        }
        foreach ($byte in $data) { $output.Add($byte) }
        $currentBaseAddress = [uint32]($currentBaseAddress + $section.RawSize)
    }

    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $BinaryFile) | Out-Null
    [IO.File]::WriteAllBytes($BinaryFile, $output.ToArray())
}

$sourceDirectory = [IO.Path]::GetFullPath($SourceDirectory)
$sourceRoot = [IO.Path]::GetFullPath($SourceRoot)
$generatedRoot = [IO.Path]::GetFullPath($GeneratedRoot)
$relativeDirectory = [IO.Path]::GetRelativePath($sourceRoot, $sourceDirectory)
$binaryDirectory = Join-Path $generatedRoot $relativeDirectory
$variables = @{
    "REACTOS_SOURCE_DIR" = $sourceRoot
    "CMAKE_CURRENT_SOURCE_DIR" = $sourceDirectory
    "CMAKE_CURRENT_BINARY_DIR" = $binaryDirectory
}
$contents = Get-Content -LiteralPath $CMakeListFile -Raw
foreach ($match in [regex]::Matches($contents, "(?is)set\s*\(\s*(?<name>[A-Za-z_][A-Za-z0-9_]*)\s+(?<value>.*?)\)")) {
    $variables[$match.Groups["name"].Value] = $match.Groups["value"].Value.Trim()
}

foreach ($match in [regex]::Matches($contents, "(?is)CreateBootSectorTarget\s*\((?<body>.*?)\)")) {
    $tokens = @(
        (Expand-CMakeValue $match.Groups["body"].Value $variables) -split "\s+" |
            ForEach-Object { $_.Trim() } |
            Where-Object { $_ }
    )
    if ($tokens.Count -lt 4) {
        continue
    }
    $asmFile = Resolve-CMakePath $tokens[1] $sourceDirectory
    $binaryFile = Resolve-CMakePath $tokens[2] $binaryDirectory
    $baseAddress = [Convert]::ToUInt32($tokens[3], 16)
    if (-not $asmFile -or -not $binaryFile) {
        continue
    }

    $stem = [IO.Path]::GetFileNameWithoutExtension($binaryFile)
    $tempFile = Join-Path $binaryDirectory "$stem.bin.tmp"
    $objectFile = Join-Path $binaryDirectory "$stem.bin.obj"
    New-Item -ItemType Directory -Force -Path $binaryDirectory | Out-Null
    $includeArguments = @(
        "/I$sourceRoot\sdk\include\asm"
        "/I$generatedRoot\sdk\include\asm"
        "/I$sourceRoot\boot\freeldr"
        "/I$sourceDirectory"
    )
    $defineArguments = @("/D__ASM__", "/D_USE_ML")
    & $Compiler /nologo /X @includeArguments @defineArguments /EP /c $asmFile 2>$null > $tempFile
    if ($LASTEXITCODE -ne 0) {
        throw "The C preprocessor failed for boot sector '$asmFile'."
    }
    & $Assembler /nologo /Cp "/Fo$objectFile" /c /Ta $tempFile
    if ($LASTEXITCODE -ne 0) {
        throw "The x86 assembler failed for boot sector '$asmFile'."
    }
    Convert-ObjectToBinary $objectFile $binaryFile $baseAddress
}
