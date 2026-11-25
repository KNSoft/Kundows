#include "checks.h"
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
typedef struct { PROS_SHARED_CACHE_MAP SharedCacheMap; void *BaseAddress; BOOLEAN Dirty; } Vacb, *PROS_VACB;
typedef struct { PROS_SHARED_CACHE_MAP SharedCacheMap; } Section;
typedef struct { Section *SectionObjectPointer; } File, *PFILE_OBJECT;
typedef struct Map { LIST_ENTRY BcbList; int BcbSpinLock, PinAccess; File *FileObject; } Map;
typedef struct { USHORT NodeTypeCode, NodeByteSize; ULONG MappedLength; LARGE_INTEGER MappedFileOffset; } Public;
typedef struct {
    Public PFCB; Vacb *Vacb; ULONG PinCount, RefCount; Resource Lock; LIST_ENTRY BcbEntry; BOOLEAN Dirty;
} INTERNAL_BCB, *PINTERNAL_BCB;
static Map map;
static Vacb view;
static Section section;
static File file;
static INTERNAL_BCB temp, winner;
static ULONG CcMapDataNoWait, CcMapDataWait, CcPinReadWait, CcPinReadNoWait;
static int iBcbLookasideList, refs, allocFail, race, toPin, busy, resident;
static int allocs, frees, deletes, lockedDelete, missingUnlock, mmDirty, writes;
static Resource *lastLock;
static int exclusive, waited;
static void KeAcquireSpinLock(int *v,KIRQL *k) { ASSERT(!*v); *v=1; *k=0; }
static void KeReleaseSpinLock(int *v,KIRQL k) { ASSERT(*v); *v=0; }
static void *ExAllocateFromNPagedLookasideList(int *p) { if (allocFail) return NULL; ++allocs; return &temp; }
static void ExFreeToNPagedLookasideList(int *p, void *b) { ASSERT(b == &temp || b == &winner); ++frees; }
static void ExInitializeResourceLite(Resource *r) { ASSERT(!r->depth); }
static BOOLEAN acquire(Resource *r, int ex, BOOLEAN wait)
{
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
    winner.RefCount=1; winner.Vacb=&view; winner.Lock.deny=busy;
    /* Track only acquisitions by the caller under test. */
    ++refs; InsertTailList(&map.BcbList,&winner.BcbEntry);
}
static NTSTATUS CcRosGetVacb(Map *m, LONGLONG pos, Vacb **v)
{
    ASSERT(m == &map && !pos); *v=&view; ++refs;
    if (race) seed(); /* Publish after initial lookup, before the duplicate check. */
    return STATUS_SUCCESS;
}
static void CcRosReleaseVacb(Map *m, Vacb *v, BOOLEAN dirty, BOOLEAN mapped) { ASSERT(m==&map && v==&view); --refs; }
static BOOLEAN CcRosEnsureVacbResident(Vacb *v, BOOLEAN wait, BOOLEAN noRead, ULONG off, ULONG len) { return resident; }
static void MmMakeSegmentDirty(Section *s, LONGLONG pos, ULONG size) { mmDirty=1; }
static void CcRosMarkDirtyVacb(Vacb *v) { v->Dirty=TRUE; }
static void CcFlushCache(Section *s, PLARGE_INTEGER off, ULONG size, PIO_STATUS_BLOCK io)
{
    ASSERT(s==&section);
    if (mmDirty) ++writes;
    view.Dirty=FALSE; mmDirty=0; io->Status=STATUS_SUCCESS; io->Information=size;
}
static jmp_buf raised;
static void ExRaiseStatus(NTSTATUS s) { ASSERT(s==STATUS_INSUFFICIENT_RESOURCES); longjmp(raised,1); }

typedef void *PBCB;
typedef struct { ULONG Flags; NTSTATUS ExceptionStatus; } IRP_CONTEXT,*PIRP_CONTEXT;
typedef struct { int VcbState,VcbCondition,Bpb; struct {int FatIndexBitSize;} AllocationSupport; File *VirtualVolumeFile; } VCB,*PVCB;
#define VCB_STATE_FLAG_WRITE_PROTECTED 1
#define VcbGood 1
#define STATUS_FILE_INVALID ((NTSTATUS)0xc0000098)
#define FatVerifyVcb(c,v) ((void)0)
#define FatResetExceptionState(c) abort()
#define FsRtlIsNtstatusExpected(x) 0
#define FatExceptionFilter(c,e) EXCEPTION_CONTINUE_SEARCH
#define FatReservedBytes(bpb) 512
#define FatBytesPerFat(bpb) (PAGE_SIZE-512)
#include "bcb.inc"

static void reset(void)
{
    memset(&temp,0,sizeof(temp)); memset(&winner,0,sizeof(winner));
    memset(&view,0,sizeof(view)); memset(&map,0,sizeof(map));
    map.PinAccess=TRUE; InitializeListHead(&map.BcbList); map.FileObject=&file;
    file.SectionObjectPointer=&section; section.SharedCacheMap=&map; view.SharedCacheMap=&map;
    view.BaseAddress=&view; lastLock=NULL;
    refs=allocFail=race=toPin=busy=allocs=frees=deletes=lockedDelete=missingUnlock=mmDirty=writes=0;
    resident=TRUE;
}
static BOOLEAN request(int pin, ULONG flags, void **b)
{
    void *buf; LARGE_INTEGER o={0};
    if (pin) return CcPinRead(&file,&o,PAGE_SIZE,flags,b,&buf);
    if (setjmp(raised)) return FALSE;
    return CcMapData(&file,&o,PAGE_SIZE,flags,b,&buf);
}
static void run(const char *name, int pin, int when, ULONG flags, int deny, int oom, int present)
{
    void *b=NULL; reset();
    toPin=pin; race=(when==2); busy=deny; allocFail=oom; resident=present;
    if (when==1) seed();
    BOOLEAN ok=request(pin,flags,&b);
    int expected=!oom && present && !(deny && !(flags & PIN_WAIT));
    int check=(!!ok == expected);
    PINTERNAL_BCB target=when ? &winner : &temp;
    if (ok) {
        check &= b == &target->PFCB && (!pin || (lastLock==&target->Lock &&
                 exclusive==!!(flags&PIN_EXCLUSIVE) && waited==!!(flags&PIN_WAIT)));
        CcUnpinData(b);
    }
    if (when && !oom)
        check &= winner.RefCount==1 && winner.PinCount==(ULONG)pin && !winner.Lock.depth;
    verdict(name,check && !lockedDelete && !missingUnlock && !map.BcbSpinLock &&
            refs==!!when && allocs==frees && frees==deletes && IsListEmpty(&map.BcbList)==!when);
}
static void fatCase(int keepPin)
{
    IRP_CONTEXT ctx={0}; VCB v={0};
    void *b=NULL; reset();
    v.VcbCondition=VcbGood; v.AllocationSupport.FatIndexBitSize=32; v.VirtualVolumeFile=&file;
    ASSERT(request(1,PIN_WAIT,&b));
    CcSetDirtyPinnedData(b,NULL);
    if (!keepPin) CcUnpinData(b);
    NTSTATUS s=FatFlushFat(&ctx,&v);
    printf("C2 %s: status=%08lx resident-dirty=%d writes=%d\n",
           keepPin ? "held pin" : "last pin released",(ULONG)s,mmDirty,writes);
    ASSERT(s==0 && (keepPin ? (writes==1 && !mmDirty) : (!writes && mmDirty)));
    if (keepPin) CcUnpinData(b);
}
int main(void)
{
    run("new pinned",1,0,PIN_WAIT,0,0,1);
    run("new mapped",0,0,MAP_WAIT,0,0,1);
    run("initial lookup hit",1,1,PIN_WAIT,0,0,1);
    run("duplicate map",0,2,MAP_WAIT,0,0,1);
    run("duplicate shared pin",1,2,PIN_WAIT,0,0,1);
    run("duplicate exclusive pin",1,2,PIN_WAIT|PIN_EXCLUSIVE,0,0,1);
    run("duplicate busy no-wait",1,2,PIN_EXCLUSIVE,1,0,1);
    run("duplicate busy shared no-wait",1,2,0,1,0,1);
    run("pin allocation failure",1,0,PIN_WAIT,0,1,1);
    run("map allocation failure",0,0,MAP_WAIT,0,1,1);
    run("pin residency failure",1,0,PIN_WAIT,0,0,0);
    run("duplicate pin residency failure",1,2,PIN_WAIT,0,0,0);
    fatCase(1); fatCase(0);
    return summary("C1");
}
