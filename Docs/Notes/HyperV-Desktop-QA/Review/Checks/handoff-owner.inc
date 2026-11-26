/* The epoch/reservation fixture remains in bcb-owner.inc for old checkouts.
 * This fixture executes the page handoff, with MM/FS at the host boundary. */
static PINTERNAL_BCB ownerBcb(void *b) { return CONTAINING_RECORD(b,INTERNAL_BCB,PFCB); }
static void ownerReset(void) { reset(); map.RetainDirtyBcbs=TRUE; }
static int ownerBalanced(void)
{
    return IsListEmpty(&map.BcbList) && !refs && !map.ActiveBcbFlushes &&
           allocs==frees && !usedBcbs && !lockedDelete && !missingUnlock &&
           !view.MetadataIo && !map.BcbSpinLock;
}
static void *ownerDirty(void)
{
    void *b=NULL; ASSERT(request(1,PIN_WAIT,&b)); CcSetDirtyPinnedData(b,NULL); return b;
}
static int ownerHit(void)
{
    void *b=NULL;
    if (!request(1,PIN_IF_BCB,&b)) return FALSE;
    CcUnpinData(b); return TRUE;
}
static NTSTATUS ownerFlush(void)
{
    IO_STATUS_BLOCK io;
    return CcpFlushMetadata(&view,view.FileOffset.QuadPart,PAGE_SIZE,FALSE,NULL,&io);
}
static void *concurrent;
static void racingPin(void) { duringGet=NULL; ASSERT(request(1,PIN_IF_BCB,&concurrent)); }
static void flushDuringGet(void) { duringGet=NULL; ASSERT(!ownerFlush() && !frees); }
static int ownershipStudy(void)
{
    void *b,*buf; IO_STATUS_BLOCK io; LARGE_INTEGER off={0};
    PINTERNAL_BCB p; int pass;
    ownerReset(); b=ownerDirty(); p=ownerBcb(b); CcUnpinData(b);
    pass=!mmDirty && !p->Vacb && !refs && view.Dirty && p->PendingPages==1;
    getFail=1;
    pass &= !ownerHit() && !refs && !p->RefCount && !frees;
    getFail=0; duringGet=racingPin;
    ASSERT(request(1,PIN_IF_BCB,&b)); pass &= b==concurrent && refs==1;
    CcUnpinData(concurrent); CcUnpinData(b);
    pass &= ownerFlush()==0 && writes==1 && !ownerHit() && ownerBalanced();
    verdict("idle remap, mapping failure and duplicate mapping adoption",pass);

    ownerReset(); b=ownerDirty(); CcUnpinData(b); duringGet=flushDuringGet; getFail=1;
    verdict("successful write during failed remap retires clean BCB",!ownerHit() && ownerBalanced());

    ownerReset(); ASSERT(CcPinRead(&file,&off,2*PAGE_SIZE,PIN_WAIT,&b,&buf));
    p=ownerBcb(b); CcSetDirtyPinnedData(b,NULL); CcUnpinData(b);
    pass=!ownerFlush() && writes==1 && p->PendingPages==2 && ownerHit();
    flushStatus=STATUS_IO_DEVICE_ERROR;
    pass &= CcpFlushMetadata(&view,PAGE_SIZE,PAGE_SIZE,FALSE,NULL,&io)==flushStatus &&
            !io.Information && p->PendingPages==2 && !refs;
    flushStatus=0;
    pass &= !CcpFlushMetadata(&view,PAGE_SIZE,PAGE_SIZE,FALSE,NULL,&io) && writes==2 && ownerBalanced();
    verdict("partial successful page and failed sibling keep independent receipts",pass);

    ownerReset(); b=ownerDirty(); CcUnpinData(b); off.QuadPart=PAGE_SIZE;
    ASSERT(CcPinRead(&file,&off,PAGE_SIZE,PIN_WAIT,&b,&buf)); CcSetDirtyPinnedData(b,NULL);
    pass=!ownerFlush() && writes==1 && ownerBcb(b)->PendingPages==2;
    CcUnpinData(b);
    pass &= !CcpFlushMetadata(&view,0,VACB_MAPPING_GRANULARITY,TRUE,NULL,&io) &&
            io.Information==PAGE_SIZE && writes==2 && ownerBalanced() && !view.Dirty;
    verdict("explicit page excludes active neighbor; background completes remaining page",pass);

    ownerReset(); off.QuadPart=0;
    ASSERT(CcPinRead(&file,&off,512,PIN_WAIT,&b,&buf)); CcSetDirtyPinnedData(b,NULL); CcUnpinData(b);
    off.QuadPart=512;
    ASSERT(CcPinRead(&file,&off,512,PIN_WAIT,&b,&buf)); CcSetDirtyPinnedData(b,NULL);
    pass=CcpFlushMetadata(&view,0,512,TRUE,NULL,&io)==STATUS_CANT_WAIT &&
         !io.Information && !writes && !mmDirty;
    CcUnpinData(b);
    verdict("overlapping physical page, second byte-range BCB busy: no partial handoff",
            pass && !ownerFlush() && writes==1 && ownerBalanced());

    ownerReset(); b=ownerDirty(); CcUnpinData(b);
    pass=!CcpCanPurgeBcbRange(&map,0,PAGE_SIZE);
    CcpDeleteBcbs(&map);
    verdict("failed purge preserves dirty ownership; teardown hands off before unmap",
            pass && mmDirty && !writes && ownerBalanced());

    ownerReset(); map.BcbWriteBehindDisabled=TRUE; b=ownerDirty(); p=ownerBcb(b);
    p->Lock.deny=1; pass=!ownerFlush() && writes==1;
    CcRepinBcb(b); CcUnpinRepinnedBcb(b,TRUE,&io);
    pass &= io.Status==STATUS_CANT_WAIT;
    CcUnpinData(b); p->Lock.deny=0;
    verdict("disabled explicit skips pin wait; repin write-through still needs resource",
            pass && !ownerFlush() && writes==2 && ownerBalanced());
    return summary("CC handoff ownership");
}
