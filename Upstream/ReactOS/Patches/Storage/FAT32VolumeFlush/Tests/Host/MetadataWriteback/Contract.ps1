$pinCode = Read-Unit 'ntoskrnl/cc/pin.c'
$start = $pinCode.IndexOf('typedef struct _CC_PAGE_OWNER')
if ($start -ge 0) {
    $Options += '/DPAGE_HANDOFF'
    $body = @(Functions $pinCode @('static VOID CcpFreeBcb','static PINTERNAL_BCB CcpFindBcb',
        'static VOID CcpDereferenceBcb','static NTSTATUS CcpReferenceBcb','static PVOID CcpGetAppropriateBcb'))
    $end = $pinCode.IndexOf("static`nBOOLEAN`nCcpPinData(")
    $body += $pinCode.Substring($start, $end - $start)
} else {
    $body = @(Functions $pinCode @('static VOID CcpFreeBcb','static VOID CcpFreeBcbList',
        'BOOLEAN CcpBeginBcbFlush','BOOLEAN CcpCompleteBcbFlush','static PINTERNAL_BCB CcpFindBcb',
        'static VOID CcpDereferenceBcb','static PVOID CcpGetAppropriateBcb'))
}
$body += Functions $pinCode @('VOID NTAPI CcSetDirtyPinnedData','VOID NTAPI CcUnpinDataForThread',
    'VOID NTAPI CcUnpinData','static BOOLEAN CcpPinData','BOOLEAN NTAPI CcPinRead','VOID NTAPI CcRepinBcb',
    'VOID NTAPI CcUnpinRepinnedBcb')
$body += Functions (Read-Unit 'ntoskrnl/cc/view.c') @('NTSTATUS CcRosFlushVacb','VOID NTAPI CcFlushCache')
Write-Include 'production.inc' $body
