# Create and close a *new* disposable VHDX. Does not take a disk number or
# format any pre-existing volume. Requires elevated Windows PowerShell + Hyper-V.
param([Parameter(Mandatory)][string]$Path)
$ErrorActionPreference='Stop'
$Path=[IO.Path]::GetFullPath($Path)
if ([IO.Path]::GetExtension($Path) -ne '.vhdx' -or (Test-Path -LiteralPath $Path)) {throw 'Expected an unused .vhdx path'}
$v=New-VHD -Path $Path -Dynamic -SizeBytes 512MB -LogicalSectorSizeBytes 512
$mounted=$false
try {
    $m=Mount-VHD -Path $v.Path -NoDriveLetter -Passthru
    $mounted=$true;$disk=$m|Get-Disk
    if($m.Path -ne $Path -or $disk.Number -ne (Get-VHD -Path $Path).DiskNumber -or
       $disk.IsSystem -or $disk.IsBoot -or $disk.Size -ne 512MB -or $disk.PartitionStyle -ne 'RAW'){
        throw 'New virtual disk identity or geometry mismatch'
    }
    $disk|Initialize-Disk -PartitionStyle MBR
    $p=New-Partition -DiskNumber $disk.Number -Offset 1MB -UseMaximumSize -MbrType FAT32
    if($p.DiskNumber -ne $disk.Number -or $p.PartitionNumber -ne 1 -or $p.Offset -ne 1MB){throw 'Unexpected new partition'}
    $vol=$p|Format-Volume -FileSystem FAT32 -AllocationUnitSize 4096 -NewFileSystemLabel KWFATQA -Confirm:$false
    if(!$vol.Path -or $vol.FileSystem -ne 'FAT32' -or $vol.FileSystemLabel -ne 'KWFATQA'){throw 'Fixture format not confirmed'}
    [IO.File]::WriteAllText([IO.Path]::Combine($vol.Path,'KWC2.ID'),"Kundows FAT32 volume-flush test v1`r`n",[Text.Encoding]::ASCII)
} finally {
    if($mounted){Dismount-VHD -Path $Path}
}
