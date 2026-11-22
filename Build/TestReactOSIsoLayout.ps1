[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$Iso,
    [string]$OutputJson = ""
)
$ErrorActionPreference = "Stop"

# Inspect the ISO records themselves: Windows extractors replace characters
# such as ':' in filenames and can therefore conceal an invalid image name.
$tokens = $null
$errors = $null
$ast = [Management.Automation.Language.Parser]::ParseFile(
    (Join-Path $PSScriptRoot "BuildReactOSIso.ps1"), [ref]$tokens, [ref]$errors)
if ($errors.Count) { throw ($errors | Out-String) }
$definition = $ast.Find({
    param($node)
    $node -is [Management.Automation.Language.FunctionDefinitionAst] -and
    $node.Name -eq "Test-IsoRelativePath"
}, $true)
if (-not $definition) { throw "ISO path validator not found" }
. ([scriptblock]::Create($definition.Extent.Text))

function Read-ImageBytes([long]$Offset, [int]$Count) {
    if ($Offset -lt 0 -or $Count -lt 0 -or $Offset + $Count -gt $stream.Length) {
        throw "ISO read outside image: offset=$Offset, length=$Count"
    }
    $bytes = [byte[]]::new($Count)
    $stream.Position = $Offset
    $stream.ReadExactly($bytes, 0, $Count)
    return ,$bytes
}
function Read-U32BE([byte[]]$Bytes, [int]$Offset) {
    return [uint32](
        ([uint64]$Bytes[$Offset] -shl 24) -bor
        ([uint64]$Bytes[$Offset + 1] -shl 16) -bor
        ([uint64]$Bytes[$Offset + 2] -shl 8) -bor
        [uint64]$Bytes[$Offset + 3])
}
function Read-BothEndianU32([byte[]]$Bytes, [int]$Offset) {
    $little = [BitConverter]::ToUInt32($Bytes, $Offset)
    $big = Read-U32BE $Bytes ($Offset + 4)
    if ($little -ne $big) { throw "Mismatched ISO endian copies at record offset $Offset" }
    return $little
}

$stream = [IO.File]::Open($Iso, [IO.FileMode]::Open, [IO.FileAccess]::Read, [IO.FileShare]::ReadWrite)
$descriptors = [Collections.Generic.List[object]]::new()
$invalid = [Collections.Generic.List[object]]::new()
$entryCount = 0
try {
    for ($sector = 16; $sector -lt 256; $sector++) {
        $descriptor = Read-ImageBytes ([long]$sector * 2048) 2048
        if ([Text.Encoding]::ASCII.GetString($descriptor, 1, 5) -ne "CD001") {
            throw "Invalid ISO volume descriptor at sector $sector"
        }
        $type = [int]$descriptor[0]
        if ($type -eq 255) { break }
        if ($type -notin @(1, 2)) { continue }
        $version = [int]$descriptor[6]
        $joliet = $type -eq 2 -and $version -eq 1 -and
            $descriptor[88] -eq 0x25 -and $descriptor[89] -eq 0x2f
        $rootLba = Read-BothEndianU32 $descriptor 158
        $rootLength = Read-BothEndianU32 $descriptor 166
        $descriptors.Add([pscustomobject]@{
            Sector = $sector; Type = $type; Version = $version
            Joliet = $joliet; RootLba = $rootLba; RootLength = $rootLength
        })
        $queue = [Collections.Generic.Queue[object]]::new()
        $queue.Enqueue([pscustomobject]@{ Lba = $rootLba; Length = $rootLength; Path = "" })
        $seen = [Collections.Generic.HashSet[string]]::new()
        while ($queue.Count) {
            $directory = $queue.Dequeue()
            if (-not $seen.Add("$($directory.Lba):$($directory.Length)")) { continue }
            if ($directory.Length -eq 0 -or $directory.Length -gt 64MB) {
                throw "Invalid ISO directory length: $($directory.Length)"
            }
            $data = Read-ImageBytes ([long]$directory.Lba * 2048) ([int]$directory.Length)
            for ($offset = 0; $offset -lt $data.Length;) {
                $recordLength = [int]$data[$offset]
                if ($recordLength -eq 0) {
                    $offset = ([int][Math]::Floor($offset / 2048) + 1) * 2048
                    continue
                }
                if ($recordLength -lt 34 -or $offset + $recordLength -gt $data.Length -or
                    ($offset % 2048) + $recordLength -gt 2048) {
                    throw "Invalid ISO directory record in '$($directory.Path)' at $offset"
                }
                $nameLength = [int]$data[$offset + 32]
                if ($nameLength -eq 0 -or 33 + $nameLength -gt $recordLength) {
                    throw "Invalid ISO file identifier length in '$($directory.Path)'"
                }
                $lba = Read-BothEndianU32 $data ($offset + 2)
                $length = Read-BothEndianU32 $data ($offset + 10)
                $flags = $data[$offset + 25]
                if ($nameLength -eq 1 -and $data[$offset + 33] -in @(0, 1)) {
                    $offset += $recordLength
                    continue
                }
                $encoding = if ($joliet) { [Text.Encoding]::BigEndianUnicode } else { [Text.Encoding]::Latin1 }
                $name = $encoding.GetString($data, $offset + 33, $nameLength)
                $path = if ($directory.Path) { "$($directory.Path)/$name" } else { $name }
                $entryCount++
                if (-not (Test-IsoRelativePath -Path $path)) {
                    $invalid.Add([pscustomobject]@{
                        DescriptorSector = $sector; DescriptorVersion = $version
                        Path = $path; DirectoryLba = $directory.Lba; RecordOffset = $offset
                    })
                }
                if ($flags -band 2) {
                    $queue.Enqueue([pscustomobject]@{ Lba = $lba; Length = $length; Path = $path })
                } elseif ([long]$lba * 2048 + $length -gt $stream.Length) {
                    throw "ISO file extent outside image: '$path'"
                }
                $offset += $recordLength
            }
        }
    }
    if (-not $descriptors.Count) { throw "No ISO data volume descriptors" }
} finally {
    $stream.Dispose()
}
$report = [pscustomobject]@{
    ISO = [IO.Path]::GetFullPath($Iso)
    Descriptors = @($descriptors)
    Entries = $entryCount
    InvalidNameCount = $invalid.Count
    InvalidNames = @($invalid)
}
if ($OutputJson) {
    $report | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $OutputJson -Encoding utf8
}
$report | Select-Object ISO, Entries, InvalidNameCount | Format-List
if ($invalid.Count) { $invalid | Format-Table -AutoSize; exit 2 }
