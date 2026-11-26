#include "Checks.h"
#define N (VACB_MAPPING_GRANULARITY/PAGE_SIZE)
#define MAXULONG ULONG_MAX
#define PAGE_ROUND_DOWN_64(v) ((v)&~(LONGLONG)(PAGE_SIZE-1))
typedef ULONG_PTR PFN_NUMBER, SWAPENTRY;
typedef void *PERESOURCE, *PIRP;
typedef UCHAR KIRQL;
#define MC_USER 1
#define MM_DATAFILE_SEGMENT 1
#define PFN_FROM_SSE(e) ((e)>>PAGE_SHIFT)
#define PAGE_FROM_SSE(e) ((e)&~(ULONG_PTR)(PAGE_SIZE-1))
#define DIRTY_SSE(e) ((e)|2)
#define IS_DIRTY_SSE(e) ((e)&2)
#define WRITE_SSE(e) ((e)|4)
#define IS_WRITE_SSE(e) ((e)&4)
#define SHARE_COUNT_FROM_SSE(e) (((e)&0xff8)>>3)
#define MAKE_SSE(p,c) ((p)|((c)<<3))
#define MAKE_SWAP_SSE(s) (((s)<<1)|1)
#define KernelMode 0
#define APC_LEVEL 1
#define FSRTL_MOD_WRITE_TOP_LEVEL_IRP 3
#define FSRTL_CACHE_TOP_LEVEL_IRP 4
#define KeRaiseIrql(level,irql) (*(irql)=0)
#define KeLowerIrql(irql) ((void)0)
#define KeDelayExecutionThread(...) abort()
#define IoSetTopLevelIrp(x) ((void)0)
#define IoGetTopLevelIrp() NULL
#define FsRtlAcquireFileForModWriteEx(f,o,r) STATUS_SUCCESS
#define FsRtlReleaseFileForModWrite(f,r) ((void)0)
#define MmGetSavedSwapEntryPage(p) (abort(),0)
#define MmAllocSwapPage() (abort(),0)
#define MmWriteToSwapPage(s,p) (abort(),0)
#define MmSetSavedSwapEntryPage(p,s) abort()
#define MmFreeSwapPage(s) abort()
typedef struct Map *PROS_SHARED_CACHE_MAP;
typedef struct { PROS_SHARED_CACHE_MAP SharedCacheMap; } Section, *PSECTION_OBJECT_POINTERS;
typedef struct { ULONG Flags; Section *SectionObjectPointer; } File;
typedef struct { LARGE_INTEGER FileOffset; ULONG_PTR PageEntries[2*N]; } Table, *PCACHE_SECTION_PAGE_TABLE;
typedef struct { ULONG *Flags; File *FileObject; int Locked; Table PageTable; struct { ULONG Characteristics; } Image; } Segment, *PMM_SECTION_SEGMENT;
static Table *table;
static Segment segment;
static Section section;
static File file;
static ULONG segFlags=MM_DATAFILE_SEGMENT, refs, releases;
static int noSegment, noTable, badPage, redirty, attempts[2*N], disk[2*N];
static NTSTATUS injected;

static int page(PLARGE_INTEGER off) { ASSERT(off->QuadPart>=0 && off->QuadPart<2*VACB_MAPPING_GRANULARITY && !(off->QuadPart%PAGE_SIZE)); return (int)(off->QuadPart/PAGE_SIZE); }
static void MmLockSectionSegment(Segment *s) { ASSERT(!s->Locked); s->Locked=1; }
static void MmUnlockSectionSegment(Segment *s) { ASSERT(s->Locked); s->Locked=0; }
static ULONG_PTR MmGetPageEntrySectionSegment(Segment *s,PLARGE_INTEGER o) { ASSERT(s->Locked); return table->PageEntries[page(o)]; }
static void MmSetPageEntrySectionSegment(Segment *s,PLARGE_INTEGER o,ULONG_PTR e) { ASSERT(s->Locked); table->PageEntries[page(o)]=e; }
static Segment *MiGrabDataSection(Section *sp) { ASSERT(sp==&section); if(noSegment) return NULL; ++refs; return &segment; }
static void MmDereferenceSegment(Segment *s) { ASSERT(s==&segment && refs); --refs; }
static NTSTATUS RtlLongLongAdd(LONGLONG a,LONGLONG b, LONGLONG *r)
{
    ASSERT(b>=0); if(a>LLONG_MAX-b) return STATUS_INTEGER_OVERFLOW; *r=a+b; return 0;
}
#define RtlNumberGenericTableElements(t) (!noTable)
#define RtlGetElementGenericTable(t,i) (t)
static NTSTATUS MiWritePage(Segment *s,LONGLONG off,PFN_NUMBER p)
{
    int i=(int)(off/PAGE_SIZE); ASSERT(!s->Locked && p==(PFN_NUMBER)(i+1));
    /* Bound even an MM-only candidate whose caller loops on permanent error. */
    if (++attempts[i] > 8) return (printf("FAIL: more than eight writes to unchanged page %d in one pass\n", i), exit(1), 0);
    if (i==badPage) return injected;
    if(redirty) table->PageEntries[i]=DIRTY_SSE(table->PageEntries[i]);
    disk[i]=100+i; return STATUS_SUCCESS;
}
#define MmReleasePageMemoryConsumer(c,p) (++releases)
#include "cache-flush-functions.inc"

typedef struct { BOOLEAN (*AcquireForLazyWrite)(void*,BOOLEAN); void (*ReleaseFromLazyWrite)(void*); } Callbacks;
typedef struct Map {
    File *FileObject; LARGE_INTEGER FileSize,ValidDataLength; ULONG Flags,OpenCount;
    Callbacks *Callbacks; void *LazyWriteContext; int FlushCacheLock;
    BOOLEAN RetainDirtyBcbs;
} Map;
typedef struct { Map *SharedCacheMap; LARGE_INTEGER FileOffset; LIST_ENTRY DirtyVacbListEntry; int Dirty,refs; } Vacb,ROS_VACB,*PROS_VACB;
static LIST_ENTRY DirtyVacbListHead;
static ULONG CcTotalDirtyPages;
static Map map;
static Vacb views[2];
static int master,critical, acquired, lookup, fsFailure;
#define FO_TEMPORARY_FILE 1
#define WRITEBEHIND_DISABLED 2
#define SHARED_CACHE_MAP_IN_LAZYWRITE 4
static KIRQL masterLock(void) { ASSERT(!master); master=1; return 0; }
static void masterUnlock(void) { ASSERT(master==1); master=0; }
#define KeAcquireQueuedSpinLock(q) masterLock()
#define KeReleaseQueuedSpinLock(q,k) masterUnlock()
#define KeEnterCriticalRegion() (++critical)
#define KeLeaveCriticalRegion() (--critical)
#define KeAcquireGuardedMutex(p) (++*(p))
#define KeReleaseGuardedMutex(p) (--*(p))
#define CcRosVacbIncRefCount(v) (++(v)->refs)
#define CcRosVacbDecRefCount(v) (--(v)->refs)
#define CcRosDeleteFileCache(...) (abort(),STATUS_UNSUCCESSFUL)
static BOOLEAN CcRosUnmarkDirtyVacb(Vacb *v,BOOLEAN lock)
{
    if(!v->Dirty) return FALSE;
    RemoveEntryList(&v->DirtyVacbListEntry); v->Dirty=0; --v->refs; CcTotalDirtyPages-=N; return TRUE;
}
static void CcRosMarkDirtyVacb(Vacb *v)
{
    if(!v->Dirty) { InsertTailList(&DirtyVacbListHead,&v->DirtyVacbListEntry); v->Dirty=1; ++v->refs; CcTotalDirtyPages+=N; }
}
static NTSTATUS FsRtlAcquireFileForCcFlushEx(File *f) { if(fsFailure && --fsFailure) return STATUS_IO_DEVICE_ERROR; ++acquired; return 0; }
static void FsRtlReleaseFileForCcFlush(File *f) { ASSERT(acquired); --acquired; }
static BOOLEAN lazyAcquire(void *c, BOOLEAN w) { ++acquired; return TRUE; }
static void lazyRelease(void *c) { --acquired; }
static Callbacks callbacks={lazyAcquire,lazyRelease};
static Vacb *CcRosLookupVacb(Map *m,LONGLONG pos) { if(!lookup) return NULL; Vacb *v=&views[pos/VACB_MAPPING_GRANULARITY]; ++v->refs; return v; }
#define CcRosReleaseVacb(m,v,d,mp) (--(v)->refs)
/* This suite has no BCBs. The separate ownership suite executes these helpers
 * in full; here retain the normal-stream boundary used to test MmFlushStatus.patch. */
static BOOLEAN CcpBeginBcbFlush(Vacb *v,PULONGLONG g) { ASSERT(!v->SharedCacheMap->RetainDirtyBcbs); *g=0; return TRUE; }
static BOOLEAN CcpCompleteBcbFlush(Vacb *v,ULONGLONG g,NTSTATUS s) { ASSERT(!v->SharedCacheMap->RetainDirtyBcbs); return FALSE; }
#define CcpFlushMetadata(...) (abort(), STATUS_UNSUCCESSFUL)
#include "writeback.inc"

static void reset(void)
{
    memset(&segment,0,sizeof(segment)); table=&segment.PageTable; segment.Flags=&segFlags; segment.FileObject=&file;
    memset(&map,0,sizeof(map)); section.SharedCacheMap=&map; file.SectionObjectPointer=&section;
    map.FileObject=&file; map.OpenCount=1; map.FileSize.QuadPart=2*VACB_MAPPING_GRANULARITY; map.Callbacks=&callbacks;
    memset(views,0,sizeof(views)); InitializeListHead(&DirtyVacbListHead);
    for (int i=0;i<2;++i) { views[i].FileOffset.QuadPart=i*VACB_MAPPING_GRANULARITY; views[i].SharedCacheMap=&map; views[i].refs=1; }
    memset(disk,0,sizeof(disk)); memset(attempts,0,sizeof(attempts));
    releases=refs=noTable=noSegment=fsFailure=redirty=master=critical=CcTotalDirtyPages=acquired=0;
    badPage=-1; injected=STATUS_IO_DEVICE_ERROR; lookup=1;
}
static void dirty(int i) { table->PageEntries[i]=DIRTY_SSE(MAKE_SSE((ULONG_PTR)(i+1)<<12,0)); }
static int isDirty(int i) { return !!IS_DIRTY_SSE(table->PageEntries[i]); }
static int balanced(void)
{
    return !refs && !segment.Locked && !master && !critical && !acquired && !map.FlushCacheLock &&
           map.OpenCount==1 && !map.Flags && views[0].refs==1+views[0].Dirty && views[1].refs==1+views[1].Dirty;
}
static IO_STATUS_BLOCK io;
static int flush(LONGLONG pos,ULONG len, int all,NTSTATUS expected,ULONG bytes)
{
    LARGE_INTEGER o; o.QuadPart=pos;
    io.Status=STATUS_UNSUCCESSFUL; io.Information=(ULONG_PTR)-1;
    NTSTATUS s=MmFlushSegment(&section,all?NULL:&o,len,&io);
    return s==expected && io.Status==expected && io.Information==bytes && balanced();
}
static void sweep(BOOLEAN all,NTSTATUS error)
{
    ULONG count=0,target=all?MAXULONG:8; reset(); dirty(0); dirty(N); badPage=0; injected=error;
    CcRosMarkDirtyVacb(&views[0]); CcRosMarkDirtyVacb(&views[1]);
    NTSTATUS s=CcRosFlushDirtyPages(target,&count,all,!all);
    verdict(all?"shutdown pass: permanent error and healthy view":"lazy pass: permanent error and healthy view",
            s==error && count==1 && views[0].Dirty && !views[1].Dirty && CcTotalDirtyPages==N &&
            isDirty(0) && !disk[0] && disk[N] && attempts[0]<=2 && balanced());
    badPage=-1;
    s=CcRosFlushDirtyPages(target,&count,all,!all);
    verdict("next pass after device recovers",s==0 && count==1 && !views[0].Dirty && !CcTotalDirtyPages && disk[0] && balanced());
}
int main(void)
{
    reset(); verdict("clean range",flush(0,PAGE_SIZE,0,0,0));
    noSegment=1; verdict("no data section",flush(0,0,1,0,0)); noSegment=0;
    noTable=1; verdict("empty page table",flush(0,0,1,0,0)); noTable=0;
    verdict("range overflow",flush(LLONG_MAX,1,0,STATUS_INTEGER_OVERFLOW,0));
    reset(); dirty(0); badPage=0;
    verdict("write error and exact status",flush(0,PAGE_SIZE,0,injected,0) && isDirty(0) && !disk[0]);
    badPage=-1; verdict("same-page retry",flush(0,PAGE_SIZE,0,0,PAGE_SIZE) && !isDirty(0) && disk[0]);
    reset(); dirty(0); dirty(2); dirty(3); badPage=2;
    verdict("partial range stops before trailing page",flush(0,0,1,injected,PAGE_SIZE) && disk[0] && isDirty(2) && !attempts[3]);
    badPage=-1;
    verdict("retry only remaining pages",flush(0,0,1,0,2*PAGE_SIZE) && attempts[0]==1 && disk[2] && disk[3]);
    reset(); dirty(0); redirty=1;
    verdict("modified during successful write",flush(0,PAGE_SIZE,0,0,PAGE_SIZE) && isDirty(0));
    LARGE_INTEGER o={0}; badPage=0;
    verdict("absent IOSB",MmFlushSegment(&section,&o,PAGE_SIZE,NULL)==injected && isDirty(0) && balanced());
    reset(); dirty(0); MmLockSectionSegment(&segment); badPage=0;
    verdict("page-out failure retains page",!MmCheckDirtySegment(&segment,&o,FALSE,TRUE) && !releases && isDirty(0));
    badPage=-1;
    verdict("page-out success releases page",MmCheckDirtySegment(&segment,&o,FALSE,TRUE) && releases==1 && !table->PageEntries[0]);
    MmUnlockSectionSegment(&segment);
    reset(); dirty(0); dirty(1); badPage=1; CcRosMarkDirtyVacb(&views[0]);
    CcFlushCache(&section,&o,2*PAGE_SIZE,&io);
    verdict("explicit CC error: requeued, VDL unchanged",io.Status==injected && views[0].Dirty && !map.ValidDataLength.QuadPart && disk[0] && isDirty(1) && balanced());
    badPage=-1; CcFlushCache(&section,&o,2*PAGE_SIZE,&io);
    verdict("explicit CC retry",io.Status==0 && io.Information==PAGE_SIZE && !views[0].Dirty && disk[1] && balanced());
    reset(); dirty(0); badPage=0; lookup=0; CcFlushCache(&section,&o,PAGE_SIZE,&io);
    verdict("CC fallback without cached view",io.Status==injected && !map.ValidDataLength.QuadPart && isDirty(0) && balanced());
    section.SharedCacheMap=NULL; CcFlushCache(&section,&o,PAGE_SIZE,&io);
    verdict("no shared cache map",io.Status==injected && io.Information==0 && isDirty(0) && balanced());
    sweep(FALSE,STATUS_IO_DEVICE_ERROR); sweep(TRUE,STATUS_MEDIA_WRITE_PROTECTED);
    reset(); dirty(0); CcRosMarkDirtyVacb(&views[0]); fsFailure=8; ULONG count;
    NTSTATUS s=CcRosFlushDirtyPages(MAXULONG,&count,TRUE,FALSE);
    verdict("failure before MM: bounded, no bytes counted",s==STATUS_IO_DEVICE_ERROR && !count && !attempts[0] && views[0].Dirty && balanced());
    return summary("MmFlushStatus.patch");
}
