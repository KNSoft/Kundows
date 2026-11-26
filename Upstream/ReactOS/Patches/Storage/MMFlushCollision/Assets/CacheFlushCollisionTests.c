/* Actual MM flush functions, with event-controlled overlapping writeback. */
#include <windows.h>
#include <winternl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NTAPI __stdcall
#define DPRINT(...) ((void)0)
#define DPRINT1(...) ((void)0)
#define ASSERT(e) do { if (!(e)) { fprintf(stderr,"Fixture invariant: %s\n",#e); abort(); } } while (0)
#define CHECK(e) do { if (!(e)) { fprintf(stderr,"%s:%d: %s\n",__FILE__,__LINE__,#e); ++Failures; } } while (0)
#ifndef NT_SUCCESS
#define NT_SUCCESS(s) ((NTSTATUS)(s) >= 0)
#endif
#define STATUS_SUCCESS ((NTSTATUS)0)
#define STATUS_UNSUCCESSFUL ((NTSTATUS)0xc0000001)
#define STATUS_INSUFFICIENT_RESOURCES ((NTSTATUS)0xc000009a)
#define FlagOn(v,f) ((v)&(f))
#define PAGE_SHIFT 12
#define PAGE_SIZE 4096
#define PAGE_ROUND_DOWN_64(v) ((v)&~(LONGLONG)(PAGE_SIZE-1))
#define MC_USER 1
#define MM_DATAFILE_SEGMENT 1
#define KernelMode 0
#define APC_LEVEL 1
#define FSRTL_MOD_WRITE_TOP_LEVEL_IRP ((PIRP)(uintptr_t)3)
typedef uintptr_t PFN_NUMBER, SWAPENTRY;
typedef void *PERESOURCE, *PIRP;
typedef void *PSECTION_OBJECT_POINTERS;
typedef UCHAR KIRQL;

/* SSE encoding from ntoskrnl/include/internal/mm.h. */
#define PFN_FROM_SSE(E) ((PFN_NUMBER)((E)>>PAGE_SHIFT))
#define DIRTY_SSE(E) ((E)|2)
#define IS_DIRTY_SSE(E) ((E)&2)
#define WRITE_SSE(E) ((E)|4)
#define IS_WRITE_SSE(E) ((E)&4)
#ifdef _WIN64
#define PAGE_FROM_SSE(E) ((E)&0xFFFFFFF000ULL)
#else
#define PAGE_FROM_SSE(E) ((E)&0xFFFFF000)
#endif
#define SHARE_COUNT_FROM_SSE(E) (((E)&0x00000FFC)>>3)
#define MAKE_SSE(P,C) ((ULONG_PTR)((P)|((C)<<3)))
#define MAKE_SWAP_SSE(S) (((ULONG_PTR)(S)<<1)|1)

typedef struct {
    ULONG *Flags;
    struct { ULONG Characteristics; } Image;
    void *FileObject;
    BOOLEAN Locked;
    CRITICAL_SECTION Lock;
    ULONG_PTR Entry;
    int PageTable;
} MM_SECTION_SEGMENT, *PMM_SECTION_SEGMENT;
typedef struct {
    LARGE_INTEGER FileOffset;
    ULONG_PTR PageEntries[1];
} CACHE_SECTION_PAGE_TABLE, *PCACHE_SECTION_PAGE_TABLE;

static MM_SECTION_SEGMENT FixtureSegment;
static ULONG SegmentFlags = MM_DATAFILE_SEGMENT;
static ULONG MemoryValue, DiskValue;
static volatile LONG Writes, ActiveWrites, MaximumActiveWrites, Waits, ReleasedPages;
static BOOLEAN Collide, FailWrite;
static HANDLE FirstInFlight, SecondBoundary, AllowFirstCompletion;
static unsigned Tests, Failures;
static LARGE_INTEGER TinyTime;
static volatile LONG RangeReturned;
static CACHE_SECTION_PAGE_TABLE FixturePageTable;

static void MmLockSectionSegment(PMM_SECTION_SEGMENT segment)
{
    EnterCriticalSection(&segment->Lock); segment->Locked=TRUE;
}
static void MmUnlockSectionSegment(PMM_SECTION_SEGMENT segment)
{
    ASSERT(segment->Locked); segment->Locked=FALSE; LeaveCriticalSection(&segment->Lock);
}
static ULONG_PTR MmGetPageEntrySectionSegment(PMM_SECTION_SEGMENT segment, PLARGE_INTEGER offset)
{
    ASSERT(segment->Locked && offset->QuadPart == 0); return segment->Entry;
}
static void MmSetPageEntrySectionSegment(PMM_SECTION_SEGMENT segment, PLARGE_INTEGER offset, ULONG_PTR entry)
{
    ASSERT(segment->Locked && offset->QuadPart == 0); segment->Entry=entry;
}
static NTSTATUS KeDelayExecutionThread(int mode, BOOLEAN alertable, PLARGE_INTEGER delay)
{
    (void)mode; (void)alertable; (void)delay;
    InterlockedIncrement(&Waits);
    SetEvent(SecondBoundary);
    Sleep(1);
    return STATUS_SUCCESS;
}
static NTSTATUS MiWritePage(PMM_SECTION_SEGMENT segment, LONGLONG offset, PFN_NUMBER page)
{
    LONG active, maximum, sequence;
    ULONG snapshot;
    ASSERT(segment == &FixtureSegment && offset == 0 && page == 7);
    sequence=InterlockedIncrement(&Writes);
    active=InterlockedIncrement(&ActiveWrites);
    do { maximum=MaximumActiveWrites; }
    while (active>maximum && InterlockedCompareExchange(&MaximumActiveWrites,active,maximum)!=maximum);
    snapshot=MemoryValue; /* An issued disk write owns its older data snapshot. */
    if (Collide && sequence == 1) {
        SetEvent(FirstInFlight);
        ASSERT(WaitForSingleObject(AllowFirstCompletion,10000) == WAIT_OBJECT_0);
    }
    if (!FailWrite) DiskValue=snapshot;
    InterlockedDecrement(&ActiveWrites);
    if (Collide && sequence == 2) SetEvent(SecondBoundary);
    return FailWrite ? STATUS_UNSUCCESSFUL : STATUS_SUCCESS;
}
static PIRP IoGetTopLevelIrp(void) { return NULL; }
static void IoSetTopLevelIrp(PIRP irp) { (void)irp; }
static void KeRaiseIrql(KIRQL level, KIRQL *old) { (void)level; *old=0; }
static void KeLowerIrql(KIRQL old) { (void)old; }
static NTSTATUS FsRtlAcquireFileForModWriteEx(void *file, PLARGE_INTEGER end, PERESOURCE *resource)
{
    (void)file; (void)end; *resource=NULL; return STATUS_SUCCESS;
}
static void FsRtlReleaseFileForModWrite(void *file, PERESOURCE resource) { (void)file; (void)resource; }
static SWAPENTRY MmGetSavedSwapEntryPage(PFN_NUMBER page) { (void)page; CHECK(FALSE); return 0; }
static SWAPENTRY MmAllocSwapPage(void) { CHECK(FALSE); return 0; }
static NTSTATUS MmWriteToSwapPage(SWAPENTRY swap, PFN_NUMBER page)
{
    (void)swap; (void)page; CHECK(FALSE); return STATUS_UNSUCCESSFUL;
}
static void MmSetSavedSwapEntryPage(PFN_NUMBER page, SWAPENTRY swap) { (void)page; (void)swap; CHECK(FALSE); }
static void MmFreeSwapPage(SWAPENTRY swap) { (void)swap; CHECK(FALSE); }
static void MmReleasePageMemoryConsumer(int consumer, PFN_NUMBER page)
{
    ASSERT(consumer == MC_USER && page == 7); InterlockedIncrement(&ReleasedPages);
}
static PMM_SECTION_SEGMENT MiGrabDataSection(void *section)
{
    ASSERT(section == &FixtureSegment); return &FixtureSegment;
}
static void MmDereferenceSegment(PMM_SECTION_SEGMENT segment) { ASSERT(segment == &FixtureSegment); }
static LONGLONG FixtureLongLongAdd(LONGLONG left, ULONG right)
{
    ASSERT(left == 0 && right == PAGE_SIZE); return left+right;
}
#define RtlLongLongAdd(left,right,result) (*(result)=FixtureLongLongAdd(left,right),STATUS_SUCCESS)
#define RtlNumberGenericTableElements FixtureTableElements
static ULONG FixtureTableElements(int *table) { (void)table; return 1; }
#define RtlGetElementGenericTable FixtureTableElement
static void *FixtureTableElement(int *table, ULONG index)
{
    (void)table; ASSERT(index == 0); return &FixturePageTable;
}

#include "section.inc"

static void Reset(BOOLEAN dirty, ULONG shares)
{
    FixtureSegment.Flags=&SegmentFlags;
    FixtureSegment.Locked=FALSE;
    FixtureSegment.Entry=MAKE_SSE((ULONG_PTR)7<<PAGE_SHIFT,shares);
    if (dirty) FixtureSegment.Entry=DIRTY_SSE(FixtureSegment.Entry);
    MemoryValue=100; DiskValue=0; Writes=ActiveWrites=MaximumActiveWrites=Waits=ReleasedPages=RangeReturned=0;
    Collide=FailWrite=FALSE;
    ResetEvent(FirstInFlight); ResetEvent(SecondBoundary); ResetEvent(AllowFirstCompletion);
}
static BOOLEAN Flush(BOOLEAN pageOut)
{
    LARGE_INTEGER offset;
    BOOLEAN released;
    offset.QuadPart=0;
    MmLockSectionSegment(&FixtureSegment);
    released=MmCheckDirtySegment(&FixtureSegment,&offset,FALSE,pageOut);
    CHECK(FixtureSegment.Locked);
    MmUnlockSectionSegment(&FixtureSegment);
    return released;
}
static DWORD WINAPI FlushThread(void *argument)
{
    Flush((BOOLEAN)(uintptr_t)argument);
    return 0;
}
static DWORD WINAPI FlushRangeThread(void *argument)
{
    LARGE_INTEGER offset;
    IO_STATUS_BLOCK iosb;
    NTSTATUS status;
    (void)argument; offset.QuadPart=0;
    status=MmFlushSegment(&FixtureSegment,&offset,PAGE_SIZE,&iosb);
    CHECK(NT_SUCCESS(status) && NT_SUCCESS(iosb.Status));
    InterlockedExchange(&RangeReturned,1);
    SetEvent(SecondBoundary);
    return 0;
}
static void WaitForCleanInFlightWrite(void)
{
    HANDLE first, second;
    Reset(TRUE,1); Collide=TRUE;
    first=CreateThread(NULL,0,FlushThread,NULL,0,NULL);
    ASSERT(first && WaitForSingleObject(FirstInFlight,10000) == WAIT_OBJECT_0);
    second=CreateThread(NULL,0,FlushRangeThread,NULL,0,NULL);
    ASSERT(second && WaitForSingleObject(SecondBoundary,10000) == WAIT_OBJECT_0);
    CHECK(InterlockedCompareExchange(&RangeReturned,0,0) == 0);
    SetEvent(AllowFirstCompletion);
    ASSERT(WaitForSingleObject(first,10000) == WAIT_OBJECT_0);
    ASSERT(WaitForSingleObject(second,10000) == WAIT_OBJECT_0);
    CloseHandle(first); CloseHandle(second);
    CHECK(Writes == 1 && DiskValue == 100 && ActiveWrites == 0);
    CHECK(!IS_WRITE_SSE(FixtureSegment.Entry) && !IS_DIRTY_SSE(FixtureSegment.Entry));
    ++Tests;
}
static void CollisionCase(BOOLEAN pageOut)
{
    HANDLE first, second;
    Reset(TRUE,1);
    Collide=TRUE;
    first=CreateThread(NULL,0,FlushThread,NULL,0,NULL);
    ASSERT(first && WaitForSingleObject(FirstInFlight,10000) == WAIT_OBJECT_0);
    MmLockSectionSegment(&FixtureSegment);
    MemoryValue=200;
    FixtureSegment.Entry=DIRTY_SSE(FixtureSegment.Entry);
    MmUnlockSectionSegment(&FixtureSegment);
    second=CreateThread(NULL,0,FlushThread,(void *)(uintptr_t)pageOut,0,NULL);
    ASSERT(second && WaitForSingleObject(SecondBoundary,10000) == WAIT_OBJECT_0);
    SetEvent(AllowFirstCompletion);
    ASSERT(WaitForSingleObject(first,10000) == WAIT_OBJECT_0);
    ASSERT(WaitForSingleObject(second,10000) == WAIT_OBJECT_0);
    CloseHandle(first); CloseHandle(second);
    CHECK(DiskValue == 200);
    CHECK(MaximumActiveWrites == 1);
    CHECK(Writes == 2 && ActiveWrites == 0);
    CHECK(!IS_WRITE_SSE(FixtureSegment.Entry) && !IS_DIRTY_SSE(FixtureSegment.Entry));
    CHECK(SHARE_COUNT_FROM_SSE(FixtureSegment.Entry) == 1);
    ++Tests;
}
int main(void)
{
    InitializeCriticalSection(&FixtureSegment.Lock);
    FirstInFlight=CreateEventW(NULL,TRUE,FALSE,NULL);
    SecondBoundary=CreateEventW(NULL,TRUE,FALSE,NULL);
    AllowFirstCompletion=CreateEventW(NULL,TRUE,FALSE,NULL);
    ASSERT(FirstInFlight && SecondBoundary && AllowFirstCompletion);
    Reset(FALSE,1); CHECK(!Flush(FALSE)); CHECK(Writes == 0); ++Tests;
    Reset(TRUE,1); CHECK(!Flush(FALSE)); CHECK(DiskValue == 100 && Writes == 1); ++Tests;
    Reset(TRUE,1); FailWrite=TRUE; CHECK(!Flush(FALSE)); CHECK(IS_DIRTY_SSE(FixtureSegment.Entry)); ++Tests;
    Reset(FALSE,0); CHECK(Flush(TRUE)); CHECK(FixtureSegment.Entry == 0 && ReleasedPages == 1); ++Tests;
    CollisionCase(FALSE);
    CollisionCase(TRUE);
    WaitForCleanInFlightWrite();
    printf("MM flush collision: %u cases, %u failures\n",Tests,Failures);
    CloseHandle(FirstInFlight); CloseHandle(SecondBoundary); CloseHandle(AllowFirstCompletion);
    DeleteCriticalSection(&FixtureSegment.Lock);
    return Failures ? 1 : 0;
}
