/* Production pin.c + unchanged FatFlushFat/FatFlushVolume. Only MM/I/O and
 * filesystem surroundings are stubs; real guest tests check both FAT copies. */
static VCB volumeVcb;
static IRP_CONTEXT volumeContext;
static void volumeReset(ULONG bits)
{
    ownerReset();
    memset(&volumeVcb,0,sizeof(volumeVcb));
    memset(&volumeContext,0,sizeof(volumeContext));
    volumeVcb.VcbCondition=VcbGood;
    volumeVcb.AllocationSupport.FatIndexBitSize=bits;
    volumeVcb.VirtualVolumeFile=&file;
    directoryStatus=STATUS_SUCCESS; ejectUnlocks=0;
}
static NTSTATUS volumeFlush(void) { return FatFlushVolume(&volumeContext,&volumeVcb,1); }
static void volumeNewPage(ULONG offset)
{
    void *b=NULL,*buffer; LARGE_INTEGER o;
    o.QuadPart=offset;
    ASSERT(CcPinRead(&file,&o,PAGE_SIZE,PIN_WAIT,&b,&buffer));
    CcSetDirtyPinnedData(b,NULL); CcUnpinData(b);
}
static int volumeStudy(void)
{
    void *b;
    IO_STATUS_BLOCK io;
    ULONGLONG epoch;
    int pass;

    volumeReset(32);
    verdict("CC volume: clean scan allocates no BCB",volumeFlush()==0 && !writes && !allocs && ownerBalanced());

    volumeReset(32); b=ownerDirty(); CcUnpinData(b);
    verdict("CC volume: last ordinary pin remains discoverable",volumeFlush()==0 && writes==1 && ownerBalanced());

    volumeReset(32); b=ownerDirty(); CcRepinBcb(b); CcUnpinData(b); CcUnpinRepinnedBcb(b,FALSE,&io);
    verdict("CC volume: last repin remains discoverable",volumeFlush()==0 && writes==1 && ownerBalanced());

    volumeReset(32); b=ownerDirty(); CcRepinBcb(b); CcUnpinData(b);
    pass=volumeFlush()==0 && writes==1 && ownerBcb(b)->RefCount==1 && !ownerBcb(b)->DirtyPending;
    CcUnpinRepinnedBcb(b,FALSE,&io);
    verdict("CC volume: active repin remains owned by caller",pass && ownerBalanced());

    volumeReset(32); b=ownerDirty();
    pass=volumeFlush()==0 && writes==1 && ownerBcb(b)->Dirty && ownerBcb(b)->PinCount==1;
    CcUnpinData(b);
    pass &= view.Dirty && ownerFlush()==0 && writes==2 && ownerBalanced();
    verdict("CC volume: flush cannot consume active caller premark",pass);

    volumeReset(12); b=ownerDirty(); CcUnpinData(b);
    verdict("CC volume: original FAT12 range scan",volumeFlush()==0 && writes==1 && ownerBalanced());

    volumeReset(16); b=ownerDirty(); CcUnpinData(b);
    verdict("CC volume: original FAT16 page scan",volumeFlush()==0 && writes==1 && ownerBalanced());

    volumeReset(32); b=ownerDirty(); CcUnpinData(b); flushStatus=STATUS_IO_DEVICE_ERROR;
    pass=volumeFlush()==STATUS_IO_DEVICE_ERROR && !writes && view.Dirty && ownerHit();
    flushStatus=0; pass &= volumeFlush()==0 && writes==1 && ownerBalanced();
    verdict("CC volume: failed write survives and retry retires",pass);

    volumeReset(32); b=ownerDirty(); CcUnpinData(b);
    ASSERT(CcpBeginBcbFlush(&view,&epoch));
    pass=volumeFlush()==STATUS_DEVICE_BUSY && !writes && view.Dirty && !frees;
    CcpCompleteBcbFlush(&view,epoch,STATUS_SUCCESS);
    pass &= volumeFlush()==0 && writes==1 && ownerBalanced();
    verdict("CC volume: colliding flush refuses early success",pass);

    volumeReset(32); b=ownerDirty(); CcUnpinData(b); directoryStatus=STATUS_UNSUCCESSFUL;
    verdict("CC volume: directory error survives successful FAT write",
            volumeFlush()==directoryStatus && writes==1 && ownerBalanced());

    volumeReset(32); b=ownerDirty(); CcUnpinData(b); directoryStatus=STATUS_UNSUCCESSFUL; flushStatus=STATUS_IO_DEVICE_ERROR;
    pass=volumeFlush()==STATUS_IO_DEVICE_ERROR && !writes && view.Dirty;
    flushStatus=0; pass &= ownerFlush()==0 && ownerBalanced();
    verdict("CC volume: original FAT error precedence",pass);

    volumeReset(32); b=ownerDirty(); CcUnpinData(b); volumeVcb.VcbState=VCB_STATE_FLAG_WRITE_PROTECTED;
    pass=volumeFlush()==0 && !writes && !frees && view.Dirty;
    volumeVcb.VcbState=0; pass &= volumeFlush()==0 && ownerBalanced();
    verdict("CC volume: write protection does not discard pending data",pass);

    volumeReset(32); b=ownerDirty(); CcUnpinData(b); volumeVcb.VcbCondition=0;
    pass=volumeFlush()==STATUS_FILE_INVALID && !writes && !frees;
    volumeVcb.VcbCondition=VcbGood; pass &= volumeFlush()==0 && ownerBalanced();
    verdict("CC volume: invalid volume does not discard pending data",pass);

    volumeReset(32); b=ownerDirty(); CcUnpinData(b); volumeVcb.VcbState=VCB_STATE_FLAG_REMOVABLE_MEDIA;
    verdict("CC volume: removable media unlocked after success",volumeFlush()==0 && ejectUnlocks==1 && ownerBalanced());

    volumeReset(32); b=ownerDirty(); CcUnpinData(b); volumeVcb.VcbState=VCB_STATE_FLAG_REMOVABLE_MEDIA; flushStatus=STATUS_IO_DEVICE_ERROR;
    pass=volumeFlush()==STATUS_IO_DEVICE_ERROR && ejectUnlocks==1 && view.Dirty;
    flushStatus=0; pass &= ownerFlush()==0 && ownerBalanced();
    verdict("CC volume: removable media unlocked after failure",pass);

    volumeReset(32); b=ownerDirty(); CcUnpinData(b);
    volumeVcb.VcbState=VCB_STATE_FLAG_REMOVABLE_MEDIA|VCB_STATE_FLAG_BOOT_OR_PAGING_FILE;
    verdict("CC volume: boot or paging media stays locked",volumeFlush()==0 && !ejectUnlocks && ownerBalanced());

    volumeReset(32); volumeVcb.Bpb.Fat=3*PAGE_SIZE-512;
    volumeNewPage(0); volumeNewPage(PAGE_SIZE);
    verdict("CC volume: one VACB completion retires multiple page records",
            volumeFlush()==0 && writes==1 && allocs==2 && ownerBalanced());

    volumeReset(32); volumeVcb.Bpb.Fat=3*PAGE_SIZE-512;
    volumeNewPage(PAGE_SIZE);
    verdict("CC volume: absent surrounding pages stay absent",
            volumeFlush()==0 && writes==1 && allocs==1 && ownerBalanced());
    return summary("CC/FastFAT volume");
}
