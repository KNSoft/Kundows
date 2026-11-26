/* One resident page, one VACB, no lazy writer or FS serialization. */
#include "../checks.h"
#define PIN_WAIT 1
#define PIN_EXCLUSIVE 2
#define PIN_NO_READ 4
#define PIN_IF_BCB 8
#define ROUND_DOWN(x,n) ((x)-(x)%(n))
#define WRITEBEHIND_DISABLED 2
#define RETRY_ERROR ((NTSTATUS)0x80000016L) /* STATUS_VERIFY_REQUIRED */
typedef UCHAR KIRQL;
typedef ULONG_PTR ERESOURCE_THREAD;
typedef void *PIRP;
typedef struct Map *PROS_SHARED_CACHE_MAP;
typedef struct { PROS_SHARED_CACHE_MAP SharedCacheMap; } Section, *PSECTION_OBJECT_POINTERS;
#ifdef PAGE_HANDOFF
typedef struct { SRWLOCK *PagingIoResource; } FcbHeader, *PFSRTL_COMMON_FCB_HEADER;
typedef struct { Section *SectionObjectPointer; FcbHeader *FsContext; } File, *PFILE_OBJECT;
#else
typedef struct { Section *SectionObjectPointer; } File, *PFILE_OBJECT;
#endif
typedef struct Map {
    LIST_ENTRY BcbList; SRWLOCK BcbSpinLock, FlushCacheLock;
#ifdef PAGE_HANDOFF
    BOOLEAN BcbWriteBehindDisabled;
    ULONG OpenCount, ActiveBcbFlushes;
#endif
    BOOLEAN PinAccess, RetainDirtyBcbs; ULONG Flags; File *FileObject;
    ULONGLONG BcbDirtyGeneration; LARGE_INTEGER ValidDataLength, FileSize, SectionSize;
} Map;
typedef struct {
#ifdef PAGE_HANDOFF
    SRWLOCK MetadataIo;
#endif
    Map *SharedCacheMap; PVOID BaseAddress; BOOLEAN Dirty, BcbFlushActive;
    LARGE_INTEGER FileOffset;
} Vacb, *PROS_VACB;
typedef struct {
    struct { USHORT NodeTypeCode; ULONG MappedLength; LARGE_INTEGER MappedFileOffset; } PFCB;
#ifdef PAGE_HANDOFF
    ULONGLONG PendingPages;
    Map *SharedCacheMap;
#endif
    Vacb *Vacb; LIST_ENTRY BcbEntry; SRWLOCK Lock; ULONG RefCount, PinCount, DirtyPublishers;
    BOOLEAN Pinned, DirtyPending, Dirty; ULONGLONG DirtyGeneration;
} INTERNAL_BCB, *PINTERNAL_BCB;
static Map map;
static Vacb view;
static Section section = {&map};
#ifdef PAGE_HANDOFF
static FcbHeader header;
static File file = {&section, &header};
/* Only one BCB per thread in these two cases. Fail closed outside that model. */
static __declspec(thread) SRWLOCK *heldResource;
#else
static File file = {&section};
#endif
static UCHAR cache[PAGE_SIZE], disk[PAGE_SIZE];
static LONG dirty, attempts;
static NTSTATUS fault;
static ULONG CcPinReadWait, CcPinReadNoWait;
static HANDLE marked, releaseWriter, blocked;
static SRWLOCK *pinResource;
static __declspec(thread) int exclusive;
static BOOLEAN acquire(SRWLOCK *r, BOOLEAN ex, BOOLEAN wait)
{
    if (!(ex ? TryAcquireSRWLockExclusive(r) : TryAcquireSRWLockShared(r))) {
        if (!wait) return FALSE;
        if (r == pinResource && ex) ASSERT(SetEvent(blocked));
        if (ex) AcquireSRWLockExclusive(r); else AcquireSRWLockShared(r);
    }
#ifdef PAGE_HANDOFF
    ASSERT(!heldResource);
    heldResource = r;
#endif
    exclusive = ex;
    return TRUE;
}
#define ExAcquireResourceExclusiveLite(r,w) acquire(r,TRUE,w)
#define ExAcquireSharedStarveExclusive(r,w) acquire(r,FALSE,w)
#define ExReleaseResourceLite(r) (exclusive ? ReleaseSRWLockExclusive(r) : ReleaseSRWLockShared(r))
#ifdef PAGE_HANDOFF
#undef ExReleaseResourceLite
#define ExReleaseResourceLite(r) do { ASSERT(heldResource==(r)); heldResource=NULL; \
    if (exclusive) ReleaseSRWLockExclusive(r); else ReleaseSRWLockShared(r); } while (0)
#define ExIsResourceAcquiredSharedLite(r) (heldResource==(r))
static SRWLOCK masterLock;
#define KeAcquireQueuedSpinLock(l) (AcquireSRWLockExclusive(&masterLock),0)
#define KeReleaseQueuedSpinLock(l,k) ReleaseSRWLockExclusive(&masterLock)
#define KeEnterCriticalRegion() ((void)0)
#define KeLeaveCriticalRegion() ((void)0)
#define KeWaitForSingleObject(r,a,b,c,t) ((t) ? (TryAcquireSRWLockExclusive(r)?STATUS_SUCCESS:STATUS_TIMEOUT) : (AcquireSRWLockExclusive(r),STATUS_SUCCESS))
#define KeReleaseMutex(r,b) ReleaseSRWLockExclusive(r)
#define ExAllocatePoolWithTag(p,n,t) malloc(n)
#define ExFreePoolWithTag(p,t) free(p)
#define CcRosDeleteFileCache(...) (abort(),STATUS_UNSUCCESSFUL)
#define STATUS_CANT_WAIT ((NTSTATUS)0xc00000d8)
#endif
#define ExReleaseResourceForThreadLite(r,t) ExReleaseResourceLite(r)
#define PsGetCurrentThread GetCurrentThread
#define KeAcquireSpinLock(r,k) (*(k)=0,AcquireSRWLockExclusive(r))
#define KeReleaseSpinLock(r,k) ReleaseSRWLockExclusive(r)
#define KeAcquireGuardedMutex AcquireSRWLockExclusive
#define KeReleaseGuardedMutex ReleaseSRWLockExclusive
#define ExInitializeResourceLite InitializeSRWLock
#define ExDeleteResourceLite(r) ((void)0)
#define ExAllocateFromNPagedLookasideList(p) malloc(sizeof(INTERNAL_BCB))
#define ExFreeToNPagedLookasideList(p,b) free(b)
#define ExRaiseStatus(s) abort()
static PLIST_ENTRY RemoveHeadList(PLIST_ENTRY h) {
    PLIST_ENTRY e=h->Flink; RemoveEntryList(e); return e;
}
static Vacb *CcRosLookupVacb(Map *m, LONGLONG pos) {
    ASSERT(m==&map && !pos); return &view;
}
static NTSTATUS CcRosGetVacb(Map *m, LONGLONG pos, Vacb **v) {
    *v=CcRosLookupVacb(m,pos); return STATUS_SUCCESS;
}
#define CcRosReleaseVacb(m,v,d,p) ((void)0)
#define CcRosEnsureVacbResident(v,w,n,o,l) TRUE
#define CcRosMarkDirtyVacb(v) ((v)->Dirty=TRUE)
static BOOLEAN CcRosUnmarkDirtyVacb(Vacb *v, BOOLEAN lock) {
    BOOLEAN old=v->Dirty; v->Dirty=FALSE; return old;
}
#define IoGetTopLevelIrp() NULL
#define FSRTL_CACHE_TOP_LEVEL_IRP 4
#define FsRtlAcquireFileForCcFlushEx(f) STATUS_SUCCESS
#define FsRtlReleaseFileForCcFlush(f) ((void)0)
#define RtlLongLongAdd(a,b,r) (*(r)=(a)+(b),STATUS_SUCCESS)
#define MmMakeSegmentDirty(s,p,n) InterlockedExchange(&dirty,1)
#ifdef PAGE_HANDOFF
#undef MmMakeSegmentDirty
#define MmMakeSegmentDirty(s,p,n) (InterlockedExchange(&dirty,1),STATUS_SUCCESS)
#endif
static NTSTATUS MmFlushSegment(Section *s, LARGE_INTEGER *o, ULONG n, IO_STATUS_BLOCK *io)
{
    ASSERT(s==&section && !o->QuadPart && n>=PAGE_SIZE);
    io->Status=STATUS_SUCCESS; io->Information=0;
    if (dirty) {
        InterlockedIncrement(&attempts);
        if (fault) return io->Status=fault;
        memcpy(disk,cache,PAGE_SIZE);
        InterlockedExchange(&dirty,0);
        io->Information=PAGE_SIZE;
    }
    return io->Status;
}
VOID NTAPI CcFlushCache(Section *, LARGE_INTEGER *, ULONG, IO_STATUS_BLOCK *);
#include "production.inc"

static IO_STATUS_BLOCK io;
static LARGE_INTEGER offset;
static HANDLE event(void) {
    HANDLE h=CreateEventW(NULL,TRUE,FALSE,NULL); ASSERT(h); return h;
}
static void join(HANDLE h) { ASSERT(WaitForSingleObject(h,10000)==WAIT_OBJECT_0); }
static DWORD WINAPI writer(void *unused)
{
    void *b,*buf;
    ASSERT(CcPinRead(&file,&offset,PAGE_SIZE,PIN_WAIT,&b,&buf));
    pinResource=&CONTAINING_RECORD(b,INTERNAL_BCB,PFCB)->Lock;
    memset(buf,0x41,PAGE_SIZE); CcSetDirtyPinnedData(b,NULL);
    ASSERT(SetEvent(marked)); join(releaseWriter);
    memset(buf,0x64,PAGE_SIZE); CcUnpinData(b);
    return 0;
}
static DWORD WINAPI flush(void *unused)
{
    CcFlushCache(&section,&offset,PAGE_SIZE,&io); return 0;
}
static int all(UCHAR value) {
    for (int i=0;i<PAGE_SIZE;++i) if (disk[i]!=value) return 0;
    return 1;
}
static int pinCase(void)
{
    HANDLE w, phases[2]; DWORD phase;
    marked=event(); releaseWriter=event(); phases[0]=blocked=event();
    w=CreateThread(NULL,0,writer,NULL,0,NULL); ASSERT(w);
    if (WaitForSingleObject(marked,5000)!=WAIT_OBJECT_0) {
        puts("INTERLEAVING NOT ESTABLISHED: writer did not publish dirty pin"); return 2;
    }
    phases[1]=CreateThread(NULL,0,flush,NULL,0,NULL); ASSERT(phases[1]);
    phase=WaitForMultipleObjects(2,phases,FALSE,5000);
    /* The writer cannot release before this snapshot. A contended exclusive
       acquisition cannot proceed until then either. Never sample an active IOSB. */
    if (phase!=WAIT_OBJECT_0 && phase!=WAIT_OBJECT_0+1) {
        puts("INTERLEAVING NOT ESTABLISHED: neither contention nor return");
        return 2; /* Isolated child exits; no unsynchronized buffer access. */
    }
    int returned=WaitForSingleObject(phases[1],0)==WAIT_OBJECT_0;
    printf("held: phase=%s attempts=%ld backing=%02x",returned?"returned":"resource-contended",attempts,disk[0]);
    if (returned) printf(" status=%08lx",io.Status);
    puts("");
    int held=!returned && !attempts && all(0);
    ASSERT(SetEvent(releaseWriter)); join(w); join(phases[1]);
    printf("final: status=%08lx info=%zu backing=%02x\n",io.Status,(size_t)io.Information,disk[0]);
    verdict("held pin vs explicit flush",held && io.Status==0 && io.Information==PAGE_SIZE && all(0x64));
    return summary("pin");
}
static int retryCase(void)
{
    void *b,*buf;
    ASSERT(CcPinRead(&file,&offset,PAGE_SIZE,PIN_WAIT,&b,&buf));
    CcRepinBcb(b); memset(buf,0x33,PAGE_SIZE); CcSetDirtyPinnedData(b,NULL); CcUnpinData(b);
    fault=RETRY_ERROR; CcUnpinRepinnedBcb(b,TRUE,&io);
    printf("write-through: status=%08lx info=%zu attempts=%ld backing=%02x\n",
           io.Status,(size_t)io.Information,attempts,disk[0]);
    int failedWrite=io.Status==fault && !io.Information && attempts==1 && all(0);
    fault=0; io.Status=STATUS_UNSUCCESSFUL; io.Information=(ULONG_PTR)-1;
    flush(NULL); /* Fresh legal request; do not reuse the consumed repin. */
    printf("retry: status=%08lx info=%zu attempts=%ld backing=%02x\n",
           io.Status,(size_t)io.Information,attempts,disk[0]);
    verdict("repin error then explicit retry",failedWrite && io.Status==0 &&
            io.Information==PAGE_SIZE && attempts==2 && all(0x33));
    return summary("retry");
}
int main(int argc,char **argv)
{
    InitializeListHead(&map.BcbList);
#ifdef PAGE_HANDOFF
    map.OpenCount=1;
#endif
    map.PinAccess=map.RetainDirtyBcbs=TRUE;
    map.FileObject=&file; map.FileSize.QuadPart=map.SectionSize.QuadPart=PAGE_SIZE;
    view.SharedCacheMap=&map; view.BaseAddress=cache;
    io.Status=0x103; io.Information=(ULONG_PTR)-1;
    ASSERT(argc==2 && !(map.Flags & WRITEBEHIND_DISABLED));
    printf("host=%zu-bit metadata=1 writebehind-disabled=0\n",8*sizeof(void*));
    return !strcmp(argv[1],"pin") ? pinCase() : retryCase();
}
