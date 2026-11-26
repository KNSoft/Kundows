#include "Checks.h"
#include <setjmp.h>
#define PIN_WAIT 1
#define PIN_EXCLUSIVE 2
#define PIN_NO_READ 4
#define PIN_IF_BCB 8
#define MAP_WAIT PIN_WAIT
#define MAP_NO_READ PIN_NO_READ
#define ROUND_DOWN(x,n) ((x) - (x) % (n))
#define PsGetCurrentThread() 1
typedef unsigned char KIRQL;
typedef ULONG_PTR ERESOURCE_THREAD;
typedef struct { ULONG depth; int deny; } Resource;
typedef struct Map *PROS_SHARED_CACHE_MAP;
typedef struct {
    PROS_SHARED_CACHE_MAP SharedCacheMap; void *BaseAddress; BOOLEAN Dirty;
    LARGE_INTEGER FileOffset; BOOLEAN BcbFlushActive; int MetadataIo;
} Vacb, *PROS_VACB;
typedef struct { PROS_SHARED_CACHE_MAP SharedCacheMap; } Section;
typedef struct { Resource *PagingIoResource; } Header,*PFSRTL_COMMON_FCB_HEADER;
static Header header;
typedef struct { Section *SectionObjectPointer; Header *FsContext; } File, *PFILE_OBJECT;
typedef struct Map {
    LIST_ENTRY BcbList; int BcbSpinLock, PinAccess; File *FileObject;
    BOOLEAN RetainDirtyBcbs,BcbWriteBehindDisabled,PurgeOnClose; ULONG Flags,ActiveBcbFlushes; ULONGLONG BcbDirtyGeneration; LARGE_INTEGER SectionSize;
} Map;
typedef struct { USHORT NodeTypeCode, NodeByteSize; ULONG MappedLength; LARGE_INTEGER MappedFileOffset; } Public;
typedef struct {
    Public PFCB; Map *SharedCacheMap; Vacb *Vacb; ULONG PinCount, RefCount; Resource Lock; LIST_ENTRY BcbEntry; BOOLEAN Dirty;
    BOOLEAN Pinned, DirtyPending; ULONG DirtyPublishers; ULONGLONG DirtyGeneration, PendingPages;
} INTERNAL_BCB, *PINTERNAL_BCB;
static Map map;
static Vacb view;
static Section section;
static File file;
static INTERNAL_BCB temp, winner;
#ifdef KW_CC_OWNERSHIP
static INTERNAL_BCB extraBcbs[8];
static ULONG usedBcbs;
BOOLEAN CcpBeginBcbFlush(PROS_VACB, PULONGLONG);
BOOLEAN CcpCompleteBcbFlush(PROS_VACB, ULONGLONG, NTSTATUS);
static void (*duringMark)(void), (*duringWrite)(void);
static PLIST_ENTRY RemoveHeadList(PLIST_ENTRY h) { PLIST_ENTRY e=h->Flink; RemoveEntryList(e); return e; }
#endif
static ULONG CcMapDataNoWait, CcMapDataWait, CcPinReadWait, CcPinReadNoWait;
static int iBcbLookasideList, refs, allocFail, race, toPin, busy, resident;
static int allocs, frees, deletes, lockedDelete, missingUnlock, mmDirty, writes;
static Resource *lastLock;
static int exclusive, waited;
static NTSTATUS flushStatus;
static int getFail, gets;
static void (*duringGet)(void);
static void KeAcquireSpinLock(int *v,KIRQL *k) { ASSERT(!*v); *v=1; *k=0; }
static void KeReleaseSpinLock(int *v,KIRQL k) { ASSERT(*v); *v=0; }
static void *ExAllocateFromNPagedLookasideList(int *p)
{
    if (allocFail) return NULL;
    ++allocs;
#ifdef KW_CC_OWNERSHIP
    for (ULONG i=0;i<=8;++i) if (!(usedBcbs & (1u<<i))) {
        usedBcbs|=1u<<i; return i ? &extraBcbs[i-1] : &temp;
    }
    abort();
#endif
    return &temp;
}
static void ExFreeToNPagedLookasideList(int *p, void *b)
{
#ifdef KW_CC_OWNERSHIP
    if (b != &winner) {
        ULONG i; for (i=0;i<=8;++i) if (b==(i ? &extraBcbs[i-1] : &temp)) break;
        ASSERT(i<=8 && (usedBcbs & (1u<<i))); usedBcbs &= ~(1u<<i);
    }
#else
    ASSERT(b == &temp || b == &winner);
#endif
    ++frees;
}
static void ExInitializeResourceLite(Resource *r) { ASSERT(!r->depth); }
static BOOLEAN acquire(Resource *r, int ex, BOOLEAN wait)
{
#ifdef KW_CC_OWNERSHIP
    ASSERT(!map.BcbSpinLock);
#endif
    lastLock=r; exclusive=ex; waited=wait;
    if (r->deny && !wait) return FALSE;
    ++r->depth; return TRUE;
}
#define ExAcquireResourceExclusiveLite(r,w) acquire(r,1,w)
#define ExAcquireSharedStarveExclusive(r,w) acquire(r,0,w)
static void ExReleaseResourceLite(Resource *r) { if (r->depth) --r->depth; else ++missingUnlock; }
#define ExReleaseResourceForThreadLite(r,t) ExReleaseResourceLite(r)
static void ExDeleteResourceLite(Resource *r) { ++deletes; if (r->depth) ++lockedDelete; }
static void seed(void)
{
    winner.PFCB.MappedLength=PAGE_SIZE; winner.PinCount=toPin;
    winner.Pinned=!!toPin;
    winner.RefCount=1; winner.Vacb=&view; winner.Lock.deny=busy;
    winner.SharedCacheMap=&map;
    /* Track only acquisitions by the caller under test. */
    ++refs; InsertTailList(&map.BcbList,&winner.BcbEntry);
}
static NTSTATUS CcRosGetVacb(Map *m, LONGLONG pos, Vacb **v)
{
    ASSERT(m == &map && pos==view.FileOffset.QuadPart && !map.BcbSpinLock);
    ++gets;
    if (duringGet) duringGet();
    if (getFail) return STATUS_INSUFFICIENT_RESOURCES;
    *v=&view; ++refs;
    if (race) seed(); /* Publish after initial lookup, before the duplicate check. */
    return STATUS_SUCCESS;
}
static void CcRosReleaseVacb(Map *m, Vacb *v, BOOLEAN dirty, BOOLEAN mapped) { ASSERT(m==&map && v==&view && !map.BcbSpinLock); --refs; }
static BOOLEAN CcRosEnsureVacbResident(Vacb *v, BOOLEAN wait, BOOLEAN noRead, ULONG off, ULONG len) { ASSERT(v==&view && refs>0 && !map.BcbSpinLock); return resident; }
static NTSTATUS MmMakeSegmentDirty(Section *s, LONGLONG pos, ULONG size)
{
    ASSERT(!map.BcbSpinLock); mmDirty=1;
#ifdef KW_CC_OWNERSHIP
    if (duringMark) duringMark();
#endif
    return STATUS_SUCCESS;
}
static void CcRosMarkDirtyVacb(Vacb *v) { ASSERT(!map.BcbSpinLock); v->Dirty=TRUE; }
#ifdef KW_CC_HANDOFF
#define WRITEBEHIND_DISABLED 2
#define ExIsResourceAcquiredSharedLite(r) ((r)->depth)
#define STATUS_CANT_WAIT ((NTSTATUS)0xc00000d8)
#define KeEnterCriticalRegion() ((void)0)
#define KeLeaveCriticalRegion() ((void)0)
#define ExAllocatePoolWithTag(p,n,t) malloc(n)
#define ExFreePoolWithTag(p,t) free(p)
#define IoGetTopLevelIrp() NULL
#define FSRTL_CACHE_TOP_LEVEL_IRP 4
typedef void *PIRP;
#define FsRtlAcquireFileForCcFlushEx(f) STATUS_SUCCESS
#define FsRtlReleaseFileForCcFlush(f) ((void)0)
static NTSTATUS gate(int *v) { ASSERT(!map.BcbSpinLock && !*v); *v=1; return 0; }
#define KeWaitForSingleObject(r,a,b,c,t) gate(r)
#define KeReleaseMutex(r,b) (*(r)=0)
static BOOLEAN CcRosUnmarkDirtyVacb(Vacb *v,BOOLEAN l) { BOOLEAN d=v->Dirty; v->Dirty=FALSE; return d; }
static NTSTATUS MmFlushSegment(Section *s, LARGE_INTEGER *pos, ULONG n, IO_STATUS_BLOCK *io)
{
    ASSERT(!map.BcbSpinLock); io->Information=0;
    if (!NT_SUCCESS(flushStatus)) return flushStatus;
    if (mmDirty) { ++writes; mmDirty=0; io->Information=PAGE_SIZE; }
    return STATUS_SUCCESS;
}
NTSTATUS CcpFlushMetadata(Vacb *, LONGLONG, ULONG, BOOLEAN, PINTERNAL_BCB, PIO_STATUS_BLOCK);
#endif
static void CcFlushCache(Section *s, PLARGE_INTEGER off, ULONG size, PIO_STATUS_BLOCK io)
{
    ASSERT(s==&section);
#ifdef KW_CC_HANDOFF
    if (map.RetainDirtyBcbs) { CcpFlushMetadata(&view,off->QuadPart,size,FALSE,NULL,io); return; }
    if (!NT_SUCCESS(flushStatus)) { io->Status=flushStatus; io->Information=0; return; }
#elif defined(KW_CC_OWNERSHIP)
    ULONGLONG epoch;
    if (!CcpBeginBcbFlush(&view,&epoch)) { io->Status=STATUS_DEVICE_BUSY; io->Information=0; return; }
    if (!NT_SUCCESS(flushStatus)) {
        io->Status=flushStatus; io->Information=0;
        CcpCompleteBcbFlush(&view,epoch,flushStatus); return;
    }
#else
    if (!NT_SUCCESS(flushStatus)) { io->Status=flushStatus; io->Information=0; return; }
#endif
    if (mmDirty) ++writes;
    view.Dirty=FALSE; mmDirty=0; io->Status=STATUS_SUCCESS; io->Information=size;
#if defined(KW_CC_OWNERSHIP) && !defined(KW_CC_HANDOFF)
    if (duringWrite) duringWrite();
    CcpCompleteBcbFlush(&view,epoch,STATUS_SUCCESS);
#endif
}
static jmp_buf raised;
static void ExRaiseStatus(NTSTATUS s) { ASSERT(s==STATUS_INSUFFICIENT_RESOURCES); longjmp(raised,1); }

typedef void *PBCB;
typedef struct { ULONG Flags; NTSTATUS ExceptionStatus; } IRP_CONTEXT,*PIRP_CONTEXT;
typedef struct { ULONG Reserved, Fat; } Bpb;
typedef struct { int VcbState,VcbCondition; Bpb Bpb; struct {int FatIndexBitSize;} AllocationSupport; File *VirtualVolumeFile; int RootDcb; } VCB,*PVCB;
#define VCB_STATE_FLAG_WRITE_PROTECTED 1
#define VcbGood 1
#define STATUS_FILE_INVALID ((NTSTATUS)0xc0000098)
#define FatVerifyVcb(c,v) ((void)0)
#define FatResetExceptionState(c) abort()
#define FsRtlIsNtstatusExpected(x) 0
#define FatExceptionFilter(c,e) EXCEPTION_CONTINUE_SEARCH
#define FatReservedBytes(bpb) ((bpb)->Reserved ? (bpb)->Reserved : 512)
#define FatBytesPerFat(bpb) ((bpb)->Fat ? (bpb)->Fat : PAGE_SIZE-512)
#ifdef KW_CC_VOLUME
typedef int FAT_FLUSH_TYPE;
#define VCB_STATE_FLAG_REMOVABLE_MEDIA 2
#define VCB_STATE_FLAG_BOOT_OR_PAGING_FILE 4
static NTSTATUS directoryStatus;
static int ejectUnlocks;
static NTSTATUS FatFlushDirectory(PIRP_CONTEXT c,int root,FAT_FLUSH_TYPE t) { return directoryStatus; }
#define FatToggleMediaEjectDisable(c,v,b) (++ejectUnlocks)
#endif
#include "bcb.inc"

static void reset(void)
{
    memset(&temp,0,sizeof(temp)); memset(&winner,0,sizeof(winner));
    memset(&view,0,sizeof(view)); memset(&map,0,sizeof(map));
    map.PinAccess=TRUE; InitializeListHead(&map.BcbList); map.FileObject=&file;
    file.SectionObjectPointer=&section; section.SharedCacheMap=&map; view.SharedCacheMap=&map;
    file.FsContext=&header;
    view.BaseAddress=&view; lastLock=NULL;
    refs=allocFail=race=toPin=busy=allocs=frees=deletes=lockedDelete=missingUnlock=mmDirty=writes=0;
    resident=TRUE; flushStatus=STATUS_SUCCESS;
    gets=getFail=0; duringGet=NULL;
#ifdef KW_CC_OWNERSHIP
    memset(extraBcbs,0,sizeof(extraBcbs)); usedBcbs=0; duringMark=duringWrite=NULL;
    map.SectionSize.QuadPart=VACB_MAPPING_GRANULARITY;
#endif
}
static BOOLEAN request(int pin, ULONG flags, void **b)
{
    void *buf; LARGE_INTEGER o={0};
    if (setjmp(raised)) return FALSE;
    if (pin) return CcPinRead(&file,&o,PAGE_SIZE,flags,b,&buf);
    return CcMapData(&file,&o,PAGE_SIZE,flags,b,&buf);
}
#include "DuplicatePins.inc"
static void fatCase(int keepPin)
{
    IRP_CONTEXT ctx={0}; VCB v={0};
    void *b=NULL; reset();
    v.VcbCondition=VcbGood; v.AllocationSupport.FatIndexBitSize=32; v.VirtualVolumeFile=&file;
    ASSERT(request(1,PIN_WAIT,&b));
    CcSetDirtyPinnedData(b,NULL);
    if (!keepPin) CcUnpinData(b);
    NTSTATUS s=FatFlushFat(&ctx,&v);
    printf("FatFlushFat baseline %s: status=%08lx resident-dirty=%d writes=%d\n",
           keepPin ? "held pin" : "last pin released",(ULONG)s,mmDirty,writes);
    ASSERT(s==0 && (keepPin ? (writes==1 && !mmDirty) : (!writes && mmDirty)));
    if (keepPin) CcUnpinData(b);
}
enum LifeStage { Held, OnlyRepin, LastUnpin, DropRepin, WriteThrough, FailedWriteThrough };

/* Observe the actual pin.c functions with no lazy writer. The output describes
 * this implementation; do not require a future CC to reproduce today's misses. */
static void lifetime(const char *name, enum LifeStage stage, BOOLEAN dirty,
                     ULONG start, ULONG len, ULONG qstart, ULONG qlen)
{
    void *b=NULL, *found=NULL, *base=NULL;
    IO_STATUS_BLOCK io={0}; LARGE_INTEGER a, q;
    PLIST_ENTRY e;
    unsigned linked=0, cover=0; int pinCount=-1, refCount=-1;
    reset(); a.QuadPart=start; q.QuadPart=qstart;
    ASSERT(CcPinRead(&file,&a,len,PIN_WAIT,&b,&base));
    if (stage!=Held && stage!=LastUnpin) CcRepinBcb(b);
    if (dirty) CcSetDirtyPinnedData(b,NULL);
    if (stage!=Held) CcUnpinData(b);
    if (stage==FailedWriteThrough) flushStatus=STATUS_IO_DEVICE_ERROR;
    if (stage>=DropRepin) {
        CcUnpinRepinnedBcb(b,stage!=DropRepin,&io);
        ASSERT(io.Status==flushStatus);
    }
    for (e=map.BcbList.Flink;e!=&map.BcbList;e=e->Flink) {
        INTERNAL_BCB *p=CONTAINING_RECORD(e,INTERNAL_BCB,BcbEntry);
        ++linked; pinCount=p->PinCount; refCount=p->RefCount;
        if (p->PFCB.MappedFileOffset.QuadPart<=qstart &&
            p->PFCB.MappedFileOffset.QuadPart+p->PFCB.MappedLength>=qstart+qlen) ++cover;
    }
    int stillDirty=mmDirty;
    BOOLEAN hit=CcPinRead(&file,&q,qlen,PIN_WAIT|PIN_IF_BCB,&found,&base);
    printf("%-25s %4lu+%-4lu %4lu+%-4lu %u %u %2d %2d %d %d %s\n",
           name,start,len,qstart,qlen,linked,cover,refCount,pinCount,stillDirty,writes,hit?"hit":"miss");
    if (hit) CcUnpinData(found);
    if (stage==Held) CcUnpinData(b);
    if (stage==OnlyRepin) CcUnpinRepinnedBcb(b,FALSE,&io);
    ASSERT(!map.BcbSpinLock && !lockedDelete && !missingUnlock);
}
static void lifetimeStudy(void)
{
    puts("state                     pinned    queried   linked cover refs pins mm-dirty writes result");
    lifetime("held-clean",Held,FALSE,0,PAGE_SIZE,0,PAGE_SIZE);
    lifetime("held-dirty",Held,TRUE,0,PAGE_SIZE,0,PAGE_SIZE);
    lifetime("repin-only-clean",OnlyRepin,FALSE,0,PAGE_SIZE,0,PAGE_SIZE);
    lifetime("repin-only-dirty",OnlyRepin,TRUE,0,PAGE_SIZE,0,PAGE_SIZE);
    lifetime("last-unpin-clean",LastUnpin,FALSE,0,PAGE_SIZE,0,PAGE_SIZE);
    lifetime("last-unpin-dirty",LastUnpin,TRUE,0,PAGE_SIZE,0,PAGE_SIZE);
    lifetime("drop-last-repin-clean",DropRepin,FALSE,0,PAGE_SIZE,0,PAGE_SIZE);
    lifetime("drop-last-repin-dirty",DropRepin,TRUE,0,PAGE_SIZE,0,PAGE_SIZE);
    lifetime("write-through-success",WriteThrough,TRUE,0,PAGE_SIZE,0,PAGE_SIZE);
    lifetime("write-through-failure",FailedWriteThrough,TRUE,0,PAGE_SIZE,0,PAGE_SIZE);
    lifetime("held-page-query-sector",Held,TRUE,0,PAGE_SIZE,512,512);
    lifetime("held-sector-query-sector",Held,TRUE,512,512,512,512);
    lifetime("held-sector-query-page",Held,TRUE,512,512,0,PAGE_SIZE);
}
#ifdef KW_CC_HANDOFF
#include "HandoffOwner.inc"
#elif defined(KW_CC_OWNERSHIP)
#include "BcbOwner.inc"
#endif
#ifdef KW_CC_VOLUME
#include "CcVolume.inc"
#endif
int main(int argc, char **argv)
{
#ifdef KW_CC_VOLUME
    if (argc==2 && !strcmp(argv[1],"--volume")) return volumeStudy();
#endif
#ifdef KW_CC_OWNERSHIP
    if (argc==2 && !strcmp(argv[1],"--ownership")) return ownershipStudy();
#else
    if (argc==2 && !strcmp(argv[1],"--ownership")) { puts("ownership helpers not present"); return 2; }
#endif
    if (argc==2 && !strcmp(argv[1],"--lifetime")) { lifetimeStudy(); return 0; }
    checkDuplicatePins();
    fatCase(1); fatCase(0);
    return summary("CcDuplicateBcb.patch");
}
