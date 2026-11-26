# The FAT volume and duplicate-pin tests use this same pin.c fixture.
$pinCode = Read-Unit 'ntoskrnl/cc/pin.c'
$fatCode = Read-Unit 'drivers/filesystems/fastfat/flush.c'
$handoff = $pinCode.Contains('CcpWriteMetadataPage(')
$epoch = $pinCode.Contains('CcpBeginBcbFlush(')
$volume = $env:KW_CASE_ARG -eq '--volume'
if ($volume -and !($epoch -or $handoff)) {
    $Program = "$PSScriptRoot\FatVolumeLegacy.c"
    $mcb = Read-Unit 'ntoskrnl/fsrtl/largemcb.c'
    Write-Include 'fat-volume.inc' (@(Functions $mcb @('BOOLEAN NTAPI FsRtlGetNextBaseMcbEntry',
        'BOOLEAN NTAPI FsRtlGetNextLargeMcbEntry')) + (Functions (Read-Unit 'drivers/filesystems/fastfat/fsctrl.c') 'BOOLEAN FatGetNextMcbEntry') +
        (Functions $fatCode @('NTSTATUS FatFlushFat','NTSTATUS FatFlushVolume')))
    return
}
$Options += "/I`"$PSScriptRoot\..\..\..\CcDuplicateBcb\Assets`""
$decls = @()
if ($handoff) {
    $Options += '/DKW_CC_HANDOFF'
    $decls += 'static VOID CcpFreeBcb','static VOID CcpFreeBcbList','NTSTATUS CcpDeleteBcbs'
} elseif ($epoch) {
    $decls += 'static VOID CcpFreeBcb','static VOID CcpFreeBcbList','BOOLEAN CcpBeginBcbFlush','BOOLEAN CcpCompleteBcbFlush'
    $decls += if ($pinCode.Contains('CcpDeleteBcbs(')) { 'VOID CcpDeleteBcbs' } else { 'VOID CcpPurgeBcbs' }
}
if ($handoff -or $epoch) {
    $Options += '/DKW_CC_OWNERSHIP'
    if ($volume) { $Options += '/DKW_CC_VOLUME' }
}
$decls += 'PINTERNAL_BCB CcpFindBcb','VOID CcpDereferenceBcb'
if ($pinCode.Contains('CcpReferenceBcb(')) {
    $Options += '/DKW_CC_SPLIT_MAPPING'
    $decls += 'static NTSTATUS CcpReferenceBcb','BOOLEAN CcpCanPurgeBcbRange'
}
$decls += 'PVOID CcpGetAppropriateBcb','VOID NTAPI CcSetDirtyPinnedData','VOID NTAPI CcUnpinDataForThread',
    'VOID NTAPI CcUnpinData','BOOLEAN CcpPinData','BOOLEAN NTAPI CcPinRead','BOOLEAN NTAPI CcMapData',
    'VOID NTAPI CcRepinBcb','VOID NTAPI CcUnpinRepinnedBcb'
$body = @(Functions $pinCode $decls) + (Functions $fatCode 'NTSTATUS FatFlushFat')
if ($pinCode.Contains('CcpPurgeBcbs(')) { $body += '#define CcpDeleteBcbs(map) CcpPurgeBcbs(map,0,MAXLONGLONG)' }
if ($handoff) {
    $first = $pinCode.IndexOf('typedef struct _CC_PAGE_OWNER')
    $last = $pinCode.IndexOf("static`nBOOLEAN`nCcpPinData(")
    $body += $pinCode.Substring($first, $last - $first)
}
if ($volume) { $body += Functions $fatCode 'NTSTATUS FatFlushVolume' }
Write-Include 'bcb.inc' $body
