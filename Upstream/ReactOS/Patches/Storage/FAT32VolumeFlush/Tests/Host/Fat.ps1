$flush = Read-Unit 'drivers/filesystems/fastfat/flush.c'
$caller = Functions $flush 'NTSTATUS FatCommonFlushBuffers'
# Keep the volume switch arm, finally block, completion routine and lower I/O call.
$switch = $caller.IndexOf('switch (TypeOfOpen)')
$first = $caller.IndexOf('case ', $switch)
$volume = $caller.IndexOf('case UserVolumeOpen:', $first)
$caller = $caller.Remove($first, $volume - $first)
Write-Include 'fat.inc' @(
    '#ifdef KW_FAT_FLUSH_CHAIN'
    Functions $flush 'NTSTATUS FatFlushVolume'
    '#endif'
    Functions $flush 'NTSTATUS NTAPI FatFlushCompletionRoutine'
    $caller
    Functions (Read-Unit 'drivers/filesystems/fastfat/shutdown.c') 'NTSTATUS FatCommonShutdown'
)
