/* Controlled interleavings around the complete production ownership helpers. */
static PINTERNAL_BCB ownerBcb(void *b) { return CONTAINING_RECORD(b,INTERNAL_BCB,PFCB); }
static void ownerReset(void) { reset(); map.RetainDirtyBcbs=TRUE; }
static NTSTATUS ownerFlush(void)
{
    IO_STATUS_BLOCK io={0}; LARGE_INTEGER o={0};
    CcFlushCache(&section,&o,PAGE_SIZE,&io); return io.Status;
}
static int ownerBalanced(void)
{
    return IsListEmpty(&map.BcbList) && !refs && allocs==frees && !usedBcbs &&
           !lockedDelete && !missingUnlock && !map.BcbSpinLock && !view.BcbFlushActive;
}
static void *ownerDirty(void)
{
    void *b=NULL; ASSERT(request(1,PIN_WAIT,&b)); CcSetDirtyPinnedData(b,NULL); return b;
}
static BOOLEAN ownerHit(void)
{
    void *b=NULL;
    BOOLEAN hit=request(1,PIN_WAIT|PIN_IF_BCB,&b);
    if (hit) CcUnpinData(b);
    return hit;
}
static void ownerRedirty(void)
{
    duringWrite=NULL;
    void *b=ownerDirty(); CcUnpinData(b);
}
static void ownerPublishFlush(void)
{
    duringMark=NULL;
    ASSERT(ownerFlush()==STATUS_SUCCESS);
}
static void ownerNewRange(void)
{
    void *b=NULL,*buffer; LARGE_INTEGER offset;
    duringWrite=NULL; offset.QuadPart=PAGE_SIZE;
    ASSERT(CcPinRead(&file,&offset,PAGE_SIZE,PIN_WAIT,&b,&buffer));
    CcSetDirtyPinnedData(b,NULL); CcUnpinData(b);
}
#ifdef KW_CC_SPLIT_MAPPING
static void *secondPin;
static void ownerConcurrentHit(void)
{
    duringGet=NULL;
    ASSERT(request(1,PIN_IF_BCB,&secondPin));
}
static void ownerFlushInsideGet(void)
{
    duringGet=NULL;
    ASSERT(ownerFlush()==0 && !frees);
}
static void ownerPublishIdle(void)
{
    duringGet=NULL;
    CcUnpinData(ownerDirty());
}
#endif
static int ownershipStudy(void)
{
    void *b=NULL,*mapped=NULL; ULONGLONG epoch;
    IO_STATUS_BLOCK io; PINTERNAL_BCB p;
    int pass;

    ownerReset(); b=ownerDirty(); p=ownerBcb(b); CcUnpinData(b);
    pass=p->RefCount==0 && p->DirtyPending && ownerHit();
    pass &= ownerFlush()==0 && writes==1 && ownerBalanced();
    verdict("ownership: dirty ordinary release and successful retirement",pass);

    ownerReset(); b=ownerDirty(); CcRepinBcb(b); CcUnpinData(b);
    pass=ownerHit(); CcUnpinRepinnedBcb(b,FALSE,&io);
    pass &= ownerHit() && ownerFlush()==0 && ownerBalanced();
    verdict("ownership: repin-only and last repin remain discoverable",pass);

    ownerReset(); ASSERT(request(1,PIN_WAIT,&b)); CcUnpinData(b);
    verdict("ownership: clean last release is not cached",ownerBalanced());

    ownerReset(); b=ownerDirty(); CcRepinBcb(b); CcUnpinData(b);
    flushStatus=STATUS_IO_DEVICE_ERROR; CcUnpinRepinnedBcb(b,TRUE,&io);
    pass=io.Status==STATUS_IO_DEVICE_ERROR && mmDirty && view.Dirty && ownerHit() && !frees;
    flushStatus=0; pass &= ownerFlush()==0 && writes==1 && ownerBalanced();
    verdict("ownership: failed write-through survives, retry retires",pass);

    ownerReset(); b=ownerDirty(); p=ownerBcb(b);
    pass=ownerFlush()==0 && !p->DirtyPending && p->Dirty && p->PinCount==1 && !frees;
    CcUnpinData(b);
    pass &= p->DirtyPending && view.Dirty && mmDirty && ownerFlush()==0 && writes==2 && ownerBalanced();
    verdict("ownership: premark intent survives flush until unpin",pass);

    ownerReset(); b=ownerDirty(); CcUnpinData(b); duringWrite=ownerRedirty;
    pass=ownerFlush()==0 && mmDirty && view.Dirty && !frees && allocs==1;
    pass &= ownerHit() && ownerFlush()==0 && writes==2 && ownerBalanced();
    verdict("ownership: newer dirty epoch survives old successful I/O",pass);

    ownerReset(); ASSERT(request(1,PIN_WAIT,&b)); p=ownerBcb(b);
    duringMark=ownerPublishFlush; CcSetDirtyPinnedData(b,NULL);
    pass=!p->DirtyPublishers && p->DirtyPending && view.Dirty && !frees;
    CcUnpinData(b); pass &= ownerFlush()==0 && ownerBalanced();
    verdict("ownership: flush inside dirty publication cannot retire it",pass);

    ownerReset(); b=ownerDirty(); CcUnpinData(b);
    pass=CcpBeginBcbFlush(&view,&epoch);
    pass &= ownerFlush()==STATUS_DEVICE_BUSY && !writes && view.BcbFlushActive;
    mmDirty=0; view.Dirty=FALSE;
    CcpCompleteBcbFlush(&view,epoch,STATUS_SUCCESS);
    verdict("ownership: overlapping metadata flush is rejected without waiting",pass && ownerBalanced());

    ownerReset(); b=ownerDirty(); CcUnpinData(b);
    CcpDeleteBcbs(&map);
    verdict("ownership: quiesced teardown drops ownership, not MM dirty data",
            ownerBalanced() && mmDirty && !writes);

    ownerReset(); b=ownerDirty(); CcpDeleteBcbs(&map);
    pass=!frees && ownerBcb(b)->RefCount==1;
    CcUnpinData(b); CcpDeleteBcbs(&map);
    verdict("ownership: retirement cannot destroy an active caller",pass && ownerBalanced());

    ownerReset(); b=ownerDirty(); CcUnpinData(b);
    ASSERT(CcpBeginBcbFlush(&view,&epoch)); CcpDeleteBcbs(&map);
    pass=!frees; mmDirty=0; view.Dirty=FALSE;
    CcpCompleteBcbFlush(&view,epoch,0);
    verdict("ownership: teardown cannot overtake a flush reservation",pass && ownerBalanced());

    ownerReset(); b=ownerDirty(); CcUnpinData(b);
    ASSERT(request(0,MAP_WAIT,&mapped));
    pass=mapped!=b && allocs==2;
    pass &= ownerFlush()==0 && frees==1 && !IsListEmpty(&map.BcbList);
    CcUnpinData(mapped);
    verdict("ownership: mapped BCB is not confused with a zero-pin pinned BCB",pass && ownerBalanced());

    ownerReset(); map.BcbDirtyGeneration=ULLONG_MAX-3;
    b=ownerDirty(); CcUnpinData(b); duringWrite=ownerRedirty;
    pass=ownerFlush()==0 && view.Dirty && mmDirty && !frees;
    pass &= ownerFlush()==0 && ownerBalanced();
    verdict("ownership: epoch wrap does not discard a newer modification",pass);

    ownerReset(); b=ownerDirty(); p=ownerBcb(b); CcUnpinData(b); p->Lock.deny=1;
    pass=!request(1,PIN_EXCLUSIVE,&b) && !frees && p->RefCount==0 && p->DirtyPending;
    p->Lock.deny=0; pass &= ownerFlush()==0 && ownerBalanced();
    verdict("ownership: failed lock acquisition preserves cache ownership",pass);

    ownerReset(); b=ownerDirty(); CcUnpinData(b); resident=FALSE;
    pass=!request(1,PIN_WAIT,&b) && !frees && !temp.PinCount;
    resident=TRUE; pass &= ownerFlush()==0 && ownerBalanced();
    verdict("ownership: failed residency preserves dirty ownership and releases pin",pass);

    ownerReset(); b=ownerDirty(); CcUnpinData(b); duringWrite=ownerNewRange;
    pass=ownerFlush()==0 && frees==1 && allocs==2 && view.Dirty && !IsListEmpty(&map.BcbList);
    pass &= ownerFlush()==0 && ownerBalanced();
    verdict("ownership: BCB created after the snapshot belongs to the next flush",pass);

#ifdef KW_CC_SPLIT_MAPPING
    ownerReset(); b=ownerDirty(); p=ownerBcb(b); CcUnpinData(b);
    pass=!refs && !p->Vacb && p->SharedCacheMap==&map && p->DirtyPending;
    getFail=1;
    pass &= !request(1,PIN_IF_BCB,&b) && gets==2 && !refs && !p->RefCount && !p->Vacb && !frees;
    getFail=0; duringGet=ownerConcurrentHit;
    pass &= request(1,PIN_IF_BCB,&b) && secondPin==b && gets==4 && refs==1 && p->PinCount==2;
    CcUnpinData(secondPin); pass &= refs==1;
    CcUnpinData(b); pass &= !refs && !p->Vacb && ownerFlush()==0 && ownerBalanced();
    verdict("split: failed remap and racing PIN_IF_BCB hits balance exactly one mapping",pass);

    ownerReset(); b=ownerDirty(); CcUnpinData(b);
    duringGet=ownerFlushInsideGet; getFail=1;
    pass=!request(1,PIN_IF_BCB,&b) && writes==1 && ownerBalanced();
    verdict("split: flush during failed remap retires on reservation rollback",pass);

    ownerReset(); duringGet=ownerPublishIdle;
    pass=request(1,PIN_WAIT,&b) && refs==1 && allocs==2 && frees==1;
    CcUnpinData(b); pass &= !refs && ownerHit() && ownerFlush()==0 && ownerBalanced();
    verdict("split: duplicate lookup adopts mapping into newly idle winner",pass);

    ownerReset(); b=ownerDirty(); CcUnpinData(b);
    {
        Vacb other=view;
        other.FileOffset.QuadPart=VACB_MAPPING_GRANULARITY;
        ASSERT(CcpBeginBcbFlush(&other,&epoch)); CcpCompleteBcbFlush(&other,epoch,0);
    }
    pass=!refs && !frees && !CcpCanPurgeBcbRange(&map,0,PAGE_SIZE) &&
         CcpCanPurgeBcbRange(&map,VACB_MAPPING_GRANULARITY,MAXLONGLONG);
    pass &= ownerFlush()==0 && !ownerHit() && CcpCanPurgeBcbRange(&map,0,PAGE_SIZE) && ownerBalanced();
    verdict("split: other view completion and rejected purge preserve pending ownership",pass);

    ownerReset();
    {
        LARGE_INTEGER offset; void *buffer=NULL;
        view.FileOffset.QuadPart=0x100000000LL + VACB_MAPPING_GRANULARITY;
        offset.QuadPart=view.FileOffset.QuadPart+PAGE_SIZE;
        ASSERT(CcPinRead(&file,&offset,PAGE_SIZE,PIN_WAIT,&b,&buffer));
        p=ownerBcb(b); CcSetDirtyPinnedData(b,NULL); CcUnpinData(b);
        pass=!refs && !p->Vacb && CcpCanPurgeBcbRange(&map,0,PAGE_SIZE);
        offset.QuadPart+=512;
        pass &= CcPinRead(&file,&offset,512,PIN_IF_BCB,&b,&buffer) &&
                p==ownerBcb(b) && refs==1 && (ULONG_PTR)buffer==(ULONG_PTR)view.BaseAddress+PAGE_SIZE+512;
        CcUnpinData(b);
        ASSERT(CcpBeginBcbFlush(&view,&epoch)); CcpCompleteBcbFlush(&view,epoch,0);
        verdict("split: subrange reacquires the correct 64-bit mapping",pass && ownerBalanced());
    }
#endif
    return summary("CC ownership");
}
