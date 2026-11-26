# The historical short-write control always used the pinned FastFAT code.
$dev = Read-Unit 'drivers/filesystems/fastfat/deviosup.c' -Pinned
Write-Include 'MirrorCompletion.inc' (Functions $dev 'static NTSTATUS FatMultiSyncCompletionRoutine')
$write = Read-Unit 'drivers/filesystems/fastfat/write.c' -Pinned
function At([string]$marker, [int]$from) {
    $i = $write.IndexOf($marker, $from, [StringComparison]::Ordinal)
    if ($i -lt 0) { throw "Missing short-write boundary: $marker" }
    $i
}
$start = At '        RaiseIosb.Status = STATUS_SUCCESS;' (At 'if (TypeOfOpen == VirtualVolumeFile) {' 0)
$end = At "`n" (At 'RaiseIosb.Information = ByteCount;' $start)
$tail = At '            FatWaitSync( IrpContext );' $end
$last = '        return Status;'
$stop = (At $last $tail) + $last.Length
Write-Include 'MirrorStatus.inc' $write.Substring($start, $end - $start)
Write-Include 'MirrorWait.inc' $write.Substring($tail, $stop - $tail)
