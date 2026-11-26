/* Allocation shrink in an in-memory stream. Compile the checkout's CC and MM
 * functions against synthetic views, pages and I/O boundaries. */
#include "Checks.h"
#include <setjmp.h>

#define NPAGES (2 * VACB_MAPPING_GRANULARITY / PAGE_SIZE)
#define ROUND_DOWN(n, s) ((n) - (n) % (s))
#define PAGE_ROUND_DOWN_64(n) ROUND_DOWN(n, PAGE_SIZE)
#define STATUS_PURGE_FAILED ((NTSTATUS)0xc0000435)
#define STATUS_UNEXPECTED_MM_EXTEND_ERR ((NTSTATUS)0xc00000ec)
typedef UCHAR KIRQL, *PKIRQL;
typedef struct File File, *PFILE_OBJECT;
typedef struct Event { struct Event *Next; int Event; } Event, *PCACHE_UNINITIALIZE_EVENT;
typedef struct { LIST_ENTRY PrivateLinks; File *FileObject; int NodeTypeCode; } PRIVATE_CACHE_MAP, *PPRIVATE_CACHE_MAP;
typedef struct {
    LIST_ENTRY BcbEntry;
    struct { LARGE_INTEGER MappedFileOffset; ULONG MappedLength; } PFCB;
    ULONGLONG PendingPages;
    BOOLEAN DirtyPending;
    ULONG PinCount, RefCount;
    void *Vacb;
} INTERNAL_BCB, *PINTERNAL_BCB;
typedef struct {
    LARGE_INTEGER FileOffset;
    LIST_ENTRY CacheMapVacbListEntry, VacbLruListEntry;
    ULONG refs;
    BOOLEAN Dirty;
    PUCHAR BaseAddress;
} ROS_VACB, *PROS_VACB;
typedef struct {
    LIST_ENTRY PrivateList, BcbList, CacheMapVacbListHead;
    LIST_ENTRY SharedCacheMapLinks;
    PRIVATE_CACHE_MAP PrivateCacheMap;
    ULONG OpenCount;
    File *FileObject;
    Event *UninitializeEvents;
    int BcbSpinLock, CacheMapLock;
    BOOLEAN RetainDirtyBcbs, TruncatePending, PinAccess, PurgeOnClose;
    ULONG ActiveBcbFlushes, DirtyPages;
    LARGE_INTEGER SectionSize, FileSize, ValidDataLength, TruncateOffset;
    void *Section;
} Map, *PROS_SHARED_CACHE_MAP;
typedef struct { Map *SharedCacheMap; void *DataSectionObject; } Section, *PSECTION_OBJECT_POINTERS;
struct File { Section *SectionObjectPointer; PPRIVATE_CACHE_MAP PrivateCacheMap; };
typedef struct { LARGE_INTEGER AllocationSize, FileSize, ValidDataLength; } Sizes, CC_FILE_SIZES, *PCC_FILE_SIZES;
typedef struct { LARGE_INTEGER FileOffset; ULONG_PTR PageEntries[NPAGES]; } Table, *PCACHE_SECTION_PAGE_TABLE;
typedef enum { MmPurgeRange, MmPurgeTruncatedTail, MmPurgeDataSectionTail } MM_PURGE_MODE;
typedef struct { Table PageTable; ULONG SectionCount; LARGE_INTEGER RawLength; } Segment, *PMM_SECTION_SEGMENT;
typedef void *PEPROCESS;

static Map map;
static Section sop = {&map};
static File file = {&sop};
static Segment seg;
static Table highSlice;
static int sparse;
static ROS_VACB view[2];
static INTERNAL_BCB bcb[3];
static LIST_ENTRY lru;
static UCHAR bytes[NPAGES][PAGE_SIZE];
static int master, segLock, segmentCalls, retired, extensions, closes, foreignPage;
static NTSTATUS closeStatus, extendStatus, lastSizeStatus, flushStatus;
static int flushes, signals, expectFinalIO, privateFrees;
static jmp_buf raised;
typedef struct { struct { Map *ClosingMap; } Parameters; int Function; } WorkType, *PWORK_QUEUE_ENTRY;
static WorkType workItem, *posted;
static int noWork;
#define CloseCache 5
#define RTL_CONSTANT_LARGE_INTEGER(n) { (n) }
#define ExAllocateFromNPagedLookasideList(l) (noWork ? NULL : &workItem)
#define CcPostWorkQueue(w,q) do { ASSERT(!master && !posted); posted=(w); } while (0)
static int normalized;
VOID CcpApplyCloseSize(File *, Map *, LARGE_INTEGER);
BOOLEAN NTAPI CcPurgeCacheSection(Section *, PLARGE_INTEGER, ULONG, BOOLEAN);

#define LockQueueMasterLock 0
#define KeAcquireQueuedSpinLock(l) (ASSERT_MASTER(), master=1, 0)
static void ASSERT_MASTER(void) { ASSERT(!master && !map.CacheMapLock && !map.BcbSpinLock); }
#define KeReleaseQueuedSpinLock(l, k) do { ASSERT(master && !map.CacheMapLock); master=0; } while (0)
static void take(int *lock) { ASSERT(!*lock); *lock=1; }
static void drop(int *lock) { ASSERT(*lock); *lock=0; }
#define KeAcquireSpinLock(l, k) (take(l), *(k)=0)
#define KeReleaseSpinLock(l, k) drop(l)
#define KeAcquireSpinLockAtDpcLevel(l) take(l)
#define KeReleaseSpinLockFromDpcLevel(l) drop(l)
#define MmLockSectionSegment(s) take(&segLock)
#define MmUnlockSectionSegment(s) drop(&segLock)
static PLIST_ENTRY RemoveHeadList(PLIST_ENTRY h) { PLIST_ENTRY e=h->Flink; RemoveEntryList(e); return e; }
static void InsertHeadList(PLIST_ENTRY h, PLIST_ENTRY e) { InsertTailList(h->Flink,e); }
static void CcpFreeBcb(PINTERNAL_BCB b)
{
    ASSERT(!master && !map.BcbSpinLock && !b->PendingPages && !b->RefCount && !b->PinCount && !b->Vacb);
    ++retired;
}
#define CcRosVacbGetRefCount(v) ((v)->refs)
#define CcRosVacbIncRefCount(v) (++(v)->refs)
#define CcRosVacbDecRefCount(v) (--(v)->refs)
static void CcRosUnmarkDirtyVacb(PROS_VACB v, BOOLEAN lock)
{
    ASSERT(v->Dirty && master && map.CacheMapLock && !lock && v->refs==2);
    v->Dirty=FALSE; --v->refs;
}
static NTSTATUS CcpDeleteBcbs(Map *m)
{
    while (!IsListEmpty(&m->BcbList)) {
        PINTERNAL_BCB b=CONTAINING_RECORD(RemoveHeadList(&m->BcbList),INTERNAL_BCB,BcbEntry);
        b->DirtyPending=FALSE; b->PendingPages=0; CcpFreeBcb(b);
    }
    return closeStatus;
}
static void signal_event(int *e)
{
    ASSERT(!master && !map.CacheMapLock && !map.BcbSpinLock && !*e);
    ASSERT(!expectFinalIO || (closes==1 && flushes>=1));
    ++*e; ++signals;
}
#define KeSetEvent(e, i, w) signal_event(e)
#define ExFreePoolWithTag(p,t) (++privateFrees)
#define ExFreeToNPagedLookasideList(l,m) (++closes)
#define ObDereferenceObject(o) ((void)0)
#define MmExtendSection(s, z) (++extensions, extendStatus)
#define FsRtlNormalizeNtstatus(s,generic) (normalized ? (generic) : (s))
static NTSTATUS MmFlushSegment(Section *s, PLARGE_INTEGER off, ULONG len, PIO_STATUS_BLOCK io)
{
    ASSERT(!master && !map.CacheMapLock); ++flushes;
    io->Information=0; io->Status=flushStatus; return flushStatus;
}
static void ExRaiseStatus(NTSTATUS s) { ASSERT(!master && !map.CacheMapLock && map.OpenCount==1); lastSizeStatus=s; longjmp(raised, 1); }
BOOLEAN NTAPI CcUninitializeCacheMap(File *, PLARGE_INTEGER, PCACHE_UNINITIALIZE_EVENT);

#define MiGrabDataSection(s) (++segmentCalls, &seg)
#define MmDereferenceSegment(s) ((void)0)
static NTSTATUS RtlLongLongAdd(LONGLONG a, LONGLONG b, LONGLONG *r)
{
    if (a > MAXLONGLONG-b) return STATUS_UNSUCCESSFUL;
    *r=a+b; return 0;
}
#define RtlNumberGenericTableElements(t) (sparse ? 2 : 1)
#define RtlGetElementGenericTable(t, i) (t)
static Table *RtlEnumerateGenericTableWithoutSplaying(Table *t, void **key)
{
    ASSERT(segLock);
    if (!*key) { *key=t; return t; }
    if (*key==t && sparse) { *key=&highSlice; return &highSlice; }
    return NULL;
}
static ULONG_PTR *pageAt(Segment *s, PLARGE_INTEGER o)
{
    Table *t = o->QuadPart >= highSlice.FileOffset.QuadPart ? &highSlice : &s->PageTable;
    LONGLONG n = (o->QuadPart-t->FileOffset.QuadPart)/PAGE_SIZE;
    ASSERT(n>=0 && n<NPAGES);
    return &t->PageEntries[(unsigned)n];
}
#define MmGetPageEntrySectionSegment(s,o) (*pageAt(s,o))
#define MmSetPageEntrySectionSegment(s,o,e) (MmGetPageEntrySectionSegment(s,o)=(e))
#define IS_SWAP_FROM_SSE(e) ((e)&1)
#define SWAPENTRY_FROM_SSE(e) ((e)>>1)
#define MM_WAIT_ENTRY 0
#define IS_WRITE_SSE(e) ((e)&4)
#define SHARE_COUNT_FROM_SSE(e) (((e)>>3)&511)
#define PFN_FROM_SSE(e) ((e)>>12)
#define MmReleasePageMemoryConsumer(c,p) ((void)0)
#define KeDelayExecutionThread(...) abort()
#define PsGetCurrentProcess() NULL
#define MiMapPageInHyperSpace(proc,p,k) (*(k)=0, bytes[(p)-1])
#define MiUnmapPageInHyperSpace(...) ((void)0)
#define MmIsAddressValid(p) TRUE
static LARGE_INTEGER MmGetPhysicalAddress(PVOID p)
{
    LARGE_INTEGER addr;
    ASSERT(segLock && (PUCHAR)p >= (PUCHAR)bytes && (PUCHAR)p < (PUCHAR)bytes+sizeof(bytes));
    addr.QuadPart=(ULONG_PTR)p-(ULONG_PTR)bytes+PAGE_SIZE+(foreignPage ? PAGE_SIZE : 0);
    return addr;
}
#include "truncate.inc"

static void reset(void)
{
    unsigned i;
    ASSERT(!master && !map.CacheMapLock && !map.BcbSpinLock && !segLock);
    memset(&map,0,sizeof(map)); memset(view,0,sizeof(view)); memset(bcb,0,sizeof(bcb));
    memset(bytes,0x51,sizeof(bytes));
    retired=extensions=closes=foreignPage=segmentCalls=sparse=flushes=signals=expectFinalIO=privateFrees=0;
    posted=NULL; noWork=normalized=0;
    closeStatus=extendStatus=flushStatus=lastSizeStatus=0;
    memset(&highSlice,0,sizeof(highSlice)); highSlice.FileOffset.QuadPart=0x100000000LL;
    sop.SharedCacheMap=&map; sop.DataSectionObject=&seg; map.RetainDirtyBcbs=TRUE;
    seg.SectionCount=0; seg.RawLength.QuadPart=sizeof(bytes);
    file.PrivateCacheMap=&map.PrivateCacheMap; map.PrivateCacheMap.FileObject=&file;
    map.FileObject=&file; map.OpenCount=1; map.Section=&seg;
    map.SectionSize.QuadPart=map.FileSize.QuadPart=map.ValidDataLength.QuadPart=sizeof(bytes);
    InitializeListHead(&map.PrivateList); InitializeListHead(&map.BcbList);
    InitializeListHead(&map.SharedCacheMapLinks);
    InsertTailList(&map.PrivateList,&map.PrivateCacheMap.PrivateLinks);
    InitializeListHead(&map.CacheMapVacbListHead); InitializeListHead(&lru);
    for (i=0;i<2;++i) {
        view[i].refs=1; view[i].FileOffset.QuadPart=i*VACB_MAPPING_GRANULARITY;
        view[i].BaseAddress=bytes[i*(NPAGES/2)];
        InsertTailList(&map.CacheMapVacbListHead,&view[i].CacheMapVacbListEntry);
        InsertTailList(&lru,&view[i].VacbLruListEntry);
    }
    for (i=0;i<NPAGES;++i) seg.PageTable.PageEntries[i]=(i+1)<<12;
}
static void owner(unsigned n, unsigned first, unsigned length, ULONGLONG pages)
{
    PROS_VACB v=&view[first/VACB_MAPPING_GRANULARITY];
    bcb[n].PFCB.MappedFileOffset.QuadPart=first; bcb[n].PFCB.MappedLength=length;
    bcb[n].DirtyPending=TRUE; bcb[n].PendingPages=pages;
    InsertTailList(&map.BcbList,&bcb[n].BcbEntry);
    if (!v->Dirty) { v->Dirty=TRUE; ++v->refs; }
}
static int resizeAlloc(int allocation, int size, int valid)
{
    Sizes z;
    z.AllocationSize.QuadPart=allocation; z.FileSize.QuadPart=size; z.ValidDataLength.QuadPart=valid;
    if (setjmp(raised)) return 0;
    lastSizeStatus=CcSetFileSizesEx(&file,&z);
    return NT_SUCCESS(lastSizeStatus);
}
static int resize(int size,int valid) { return resizeAlloc(size,size,valid); }
static int sizes(int size, int valid)
{
    Sizes z;
    return CcGetFileSizes(&file,&z) && z.FileSize.QuadPart==size &&
           z.AllocationSize.QuadPart==size && z.ValidDataLength.QuadPart==valid;
}
static int gone(unsigned first)
{
    for (;first<NPAGES;++first) if (seg.PageTable.PageEntries[first]) return 0;
    return 1;
}
int main(void)
{
    const int cut=PAGE_SIZE+123, old=sizeof(bytes);
    LARGE_INTEGER off={0}; Event ev={0}; int pass;
    reset(); owner(0,0,PAGE_SIZE,1);
    verdict("dirty prefix survives; separate clean view and MM suffix removed",
            resize(PAGE_SIZE,PAGE_SIZE-1) && bcb[0].PendingPages==1 &&
            map.BcbList.Flink==&bcb[0].BcbEntry && view[0].Dirty && view[0].refs==2 &&
            !view[1].refs && seg.PageTable.PageEntries[0] && gone(1) && sizes(PAGE_SIZE,PAGE_SIZE-1));

    reset(); owner(0,PAGE_SIZE-12,2*PAGE_SIZE,7);
    owner(1,cut+1,6,2); owner(2,VACB_MAPPING_GRANULARITY,1,1);
    seg.PageTable.PageEntries[1]|=10; /* Dirty; shared by our own boundary view. */
    pass=resize(cut,cut-3) && retired==2 && bcb[0].PendingPages==3 &&
         bcb[0].PFCB.MappedLength==135 && seg.PageTable.PageEntries[1]==0x200a &&
         bytes[1][122]==0x51 && bytes[1][123]==0 && bytes[1][PAGE_SIZE-1]==0 &&
         gone(2) && !view[1].refs && !view[1].Dirty && sizes(cut,cut-3);
    verdict("overlapping owner clipped; partial-page prefix and dirty state retained",pass);
    verdict("regrowth keeps true VDL and does not restore discarded cache",
            resize(old,cut-3) && extensions==1 && sizes(old,cut-3) && !bytes[1][123] && gone(2));

    reset(); owner(0,PAGE_SIZE,PAGE_SIZE,2); ++view[1].refs;
    verdict("busy CC view skips MM purge and still publishes EOF successfully",
            resize(PAGE_SIZE,PAGE_SIZE) && !retired && !segmentCalls && bcb[0].PendingPages==2 &&
            lastSizeStatus==STATUS_SUCCESS && map.TruncatePending && sizes(PAGE_SIZE,PAGE_SIZE));
    --view[1].refs;
    verdict("same-size retry purges the outstanding tail",resize(PAGE_SIZE,PAGE_SIZE) &&
            retired==1 && !map.TruncatePending && gone(1));
    reset(); ++view[1].refs;
    pass=resize(PAGE_SIZE,PAGE_SIZE) && map.TruncatePending;
    --view[1].refs;
    verdict("direct regrowth after the obstruction clears",pass && resize(old,PAGE_SIZE) &&
            !map.TruncatePending && sizes(old,PAGE_SIZE) && gone(1));
    reset(); ++view[1].refs; pass=resize(PAGE_SIZE,PAGE_SIZE) && map.TruncatePending; --view[1].refs;
    bytes[1][0]=0x7c;  /* the FSD now declares the second page initialized */
    verdict("retry cannot discard newly valid bytes",pass && resize(old,2*PAGE_SIZE) &&
            seg.PageTable.PageEntries[1] && bytes[1][0]==0x7c && gone(2));
    reset();
    verdict("EOF-only shrink invalidates at EOF without resizing the allocation",
            resizeAlloc(old,cut,cut) && map.SectionSize.QuadPart==old && map.FileSize.QuadPart==cut &&
            !extensions && bytes[1][122]==0x51 && !bytes[1][123] && gone(2));
    reset(); sop.SharedCacheMap=NULL; flushStatus=STATUS_IO_DEVICE_ERROR;
    verdict("MM-only boundary flush error propagates before purge",!resize(cut,cut) &&
            lastSizeStatus==STATUS_IO_DEVICE_ERROR && flushes==1 && !segmentCalls);
    reset(); sop.SharedCacheMap=NULL;
    verdict("MM-only aligned tail purges directly",resize(PAGE_SIZE,PAGE_SIZE) &&
             !flushes && gone(1));
    reset(); sop.SharedCacheMap=NULL; seg.SectionCount=1; seg.RawLength.QuadPart=cut;
    seg.PageTable.PageEntries[1]|=8;
    verdict("VDL update with unchanged unaligned EOF preserves a mapped last page",
            resizeAlloc(old,cut,cut-1) && flushes==1 && seg.PageTable.PageEntries[1] && gone(2));
    reset(); sop.SharedCacheMap=NULL; seg.SectionCount=1;
    pass=!resizeAlloc(old,2*PAGE_SIZE,PAGE_SIZE) && lastSizeStatus==STATUS_PURGE_FAILED;
    seg.SectionCount=0;
    verdict("held data section refuses a shorter tail; next direct growth succeeds",
            pass && resizeAlloc(2*old,2*old,old));
    reset(); sop.SharedCacheMap=NULL; sop.DataSectionObject=NULL;
    verdict("uncached file without a data section succeeds",resize(cut,cut) && !flushes && !segmentCalls);
    reset(); seg.PageTable.PageEntries[3]|=8;
    { Sizes z; z.AllocationSize.QuadPart=z.FileSize.QuadPart=z.ValidDataLength.QuadPart=PAGE_SIZE;
      pass=0; if (!setjmp(raised)) { CcSetFileSizes(&file,&z); pass=1; }
      verdict("legacy wrapper does not raise a returned purge failure",pass && map.TruncatePending);
    }
    reset(); extendStatus=STATUS_INSUFFICIENT_RESOURCES;
    { Sizes z; z.AllocationSize.QuadPart=z.FileSize.QuadPart=z.ValidDataLength.QuadPart=2*old;
      pass=0; if (!setjmp(raised)) CcSetFileSizes(&file,&z); else pass=1;
      verdict("section extension exception crosses the legacy wrapper",pass && sizes(old,old) && extensions==1);
    }

    reset(); owner(0,0,PAGE_SIZE,1); seg.PageTable.PageEntries[3]|=8;
    pass=!resize(PAGE_SIZE,PAGE_SIZE) && lastSizeStatus==STATUS_PURGE_FAILED && !seg.PageTable.PageEntries[2] &&
         seg.PageTable.PageEntries[3] && map.TruncatePending && bcb[0].PendingPages==1;
    seg.PageTable.PageEntries[3]&=~(ULONG_PTR)8;
    verdict("partial MM refusal remains retryable",pass && resize(PAGE_SIZE,PAGE_SIZE) && gone(1));
    reset(); seg.PageTable.PageEntries[3]|=8;
    pass=!resize(PAGE_SIZE,PAGE_SIZE);
    verdict("prior MM refusal is not a veto on growth while a page remains in use",
            pass && resize(old,PAGE_SIZE) && map.TruncatePending && sizes(old,PAGE_SIZE));
    reset(); map.PinAccess=TRUE; seg.PageTable.PageEntries[3]|=8;
    verdict("pin-access size notification does not return a private cleanup error",
            resize(PAGE_SIZE,PAGE_SIZE) && map.TruncatePending && sizes(PAGE_SIZE,PAGE_SIZE));
    reset(); normalized=1; extendStatus=STATUS_UNSUCCESSFUL;
    verdict("unexpected extend exception uses the MM extend fallback",
            !resize(2*old,old) && lastSizeStatus==STATUS_UNEXPECTED_MM_EXTEND_ERR);

    reset(); sparse=1; highSlice.PageEntries[2]=0x7000;
    verdict("distant sparse slice is purged without visiting the hole",
            resize(PAGE_SIZE,PAGE_SIZE) && gone(1) && !highSlice.PageEntries[2]);
    reset(); seg.PageTable.PageEntries[4]|=4;
    pass=!resize(PAGE_SIZE,PAGE_SIZE) && map.TruncatePending && seg.PageTable.PageEntries[4];
    seg.PageTable.PageEntries[4]&=~(ULONG_PTR)4;
    verdict("in-flight MM write can finish before retry",pass && resize(PAGE_SIZE,PAGE_SIZE) && gone(1));
    reset(); seg.PageTable.PageEntries[1]|=8; foreignPage=1;
    verdict("different mapping of the boundary page is not modified",!resize(cut,cut) &&
            bytes[1][123]==0x51 && map.TruncatePending && view[0].refs==1);
    reset(); owner(0,0,1,1); map.ActiveBcbFlushes=1;
    verdict("in-flight metadata flush defers cleanup without failing the notification",
            resize(0,0) && map.TruncatePending && !retired && !segmentCalls);
    map.ActiveBcbFlushes=0;
    verdict("retry to offset zero retires the suffix",resize(0,0) && retired==1 && gone(0));

    reset(); owner(0,0,PAGE_SIZE,1);
    verdict("ordinary purge preserves pending ownership",!CcPurgeCacheSection(&sop,&off,0,FALSE) &&
            bcb[0].PendingPages==1 && !segmentCalls && !retired);
    reset(); off.QuadPart=PAGE_SIZE; map.ActiveBcbFlushes=1; ++map.OpenCount;
    verdict("close defers notification while a cache user remains",
            !CcUninitializeCacheMap(&file,&off,&ev) && !ev.Event && !closes && map.TruncatePending);
    reset(); ev.Event=0; off.QuadPart=cut; owner(0,0,PAGE_SIZE,1);
    verdict("uninitialize honors a byte-sized tail",CcUninitializeCacheMap(&file,&off,&ev) &&
            ev.Event==1 && closes==1 && !map.TruncatePending && bytes[1][122]==0x51 && !bytes[1][123]);
    reset(); ev.Event=0; closeStatus=STATUS_IO_DEVICE_ERROR;
    { BOOLEAN completed=FALSE;
      verdict("final I/O status is separate from synchronous teardown",
              CcRosUninitializeFileCache(&file,NULL,&ev,&completed)==closeStatus && completed && closes==1);
    }
    reset();
    { File second={&sop}, third={&sop}; PRIVATE_CACHE_MAP p={0},p2={0};
      Event e1={0},e2={0},e3={0}; p.FileObject=&second; p2.FileObject=&third;
      second.PrivateCacheMap=&p; third.PrivateCacheMap=&p2; map.OpenCount=3;
      InsertTailList(&map.PrivateList,&p.PrivateLinks); InsertTailList(&map.PrivateList,&p2.PrivateLinks);
      owner(0,0,PAGE_SIZE,1); off.QuadPart=PAGE_SIZE;
      pass=!CcUninitializeCacheMap(&second,&off,&e1) && signals==1 && e1.Event && !closes &&
           map.FileSize.QuadPart==old && !CcUninitializeCacheMap(&third,NULL,&e2) && e2.Event &&
           signals==2 && map.OpenCount==1;
      expectFinalIO=1;
      verdict("private peers receive their own event before return; last clean close completes",
              pass && CcUninitializeCacheMap(&file,NULL,&e3) && signals==3 && e1.Event && e2.Event &&
              e3.Event && !e1.Next && !e2.Next && !e3.Next && privateFrees==2 && closes==1);
    }
    reset(); ev.Event=0; owner(0,0,PAGE_SIZE,1); map.DirtyPages=1;
    { File other={&sop}; Event e2={0}; expectFinalIO=1;
      pass=!CcUninitializeCacheMap(&file,NULL,&ev) && posted==&workItem && !ev.Event && !closes &&
           !CcUninitializeCacheMap(&other,NULL,&e2) && !e2.Event;
      if (posted) CcpDereferenceCacheMap(posted->Parameters.ClosingMap);
      verdict("dirty final close and uncached waiter notified after worker flush",
              pass && closes==1 && signals==2 && ev.Event && e2.Event);
    }
    reset(); ev.Event=0; off.QuadPart=0; ++map.OpenCount;
    pass=!CcUninitializeCacheMap(&file,&off,&ev) && map.PurgeOnClose && !ev.Event;
    CcpSignalUninitializeEvents(CcpReuseFileCache(&map));
    verdict("reusing a map drains old waiters and cancels final zero purge",
            pass && ev.Event && !map.PurgeOnClose && !map.UninitializeEvents && !closes);
    reset(); ev.Event=0; owner(0,0,PAGE_SIZE,1); map.DirtyPages=1; off.QuadPart=0;
    seg.SectionCount=1; /* The destructor still holds its own section after detaching the map. */
    pass=!CcUninitializeCacheMap(&file,&off,&ev) && posted && !ev.Event;
    if(posted) CcpDereferenceCacheMap(posted->Parameters.ClosingMap);
    verdict("final zero purge discards old data instead of writing it",pass && gone(0) && !flushes && ev.Event);
    reset(); ev.Event=0; map.DirtyPages=1; noWork=1; owner(0,0,PAGE_SIZE,1); expectFinalIO=1;
    verdict("work allocation failure falls back to real synchronous completion",
            CcUninitializeCacheMap(&file,NULL,&ev) && closes==1 && ev.Event && !posted);
    reset(); sop.SharedCacheMap=NULL; ev.Event=0;
    verdict("no cache map: event signalled, no shared map deleted",
            !CcUninitializeCacheMap(&file,NULL,&ev) && ev.Event && signals==1 && !closes);
    return summary("CC allocation shrink");
}
