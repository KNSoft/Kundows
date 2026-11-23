/* Execute actual pin.c functions with deterministic kernel/MM boundaries. */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IN
#define VOID void
#define NTAPI __stdcall
#define TRUE 1
#define FALSE 0
#define CCTRACE(...) Trace(__VA_ARGS__)
#define CC_API_DEBUG 0
typedef void *PVOID;
typedef uint8_t BOOLEAN, KIRQL, *PKIRQL;
typedef uint32_t ULONG;
typedef uintptr_t ERESOURCE_THREAD;
typedef struct { int unused; } ERESOURCE, *PERESOURCE;
typedef struct { int unused; } LIST_ENTRY, *PLIST_ENTRY;
typedef struct { int unused; } NPAGED_LOOKASIDE_LIST, *PNPAGED_LOOKASIDE_LIST;
typedef struct { int unused; } SECTION_OBJECT_POINTERS, *PSECTION_OBJECT_POINTERS;
typedef struct { int64_t QuadPart; } LARGE_INTEGER, *PLARGE_INTEGER;
typedef struct { long Status; uintptr_t Information; } IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;
typedef struct { PSECTION_OBJECT_POINTERS SectionObjectPointer; } FILE_OBJECT;
typedef struct { FILE_OBJECT *FileObject; ULONG BcbSpinLock; } ROS_SHARED_CACHE_MAP, *PROS_SHARED_CACHE_MAP;
typedef struct { BOOLEAN Dirty; PROS_SHARED_CACHE_MAP SharedCacheMap; } ROS_VACB, *PROS_VACB;
typedef struct { LARGE_INTEGER MappedFileOffset; ULONG MappedLength; } PUBLIC_BCB;
typedef struct {
    ERESOURCE Lock;
    PUBLIC_BCB PFCB;
    PROS_VACB Vacb;
    ULONG PinCount;
    int16_t RefCount;
    LIST_ENTRY BcbEntry;
    BOOLEAN Dirty;
} INTERNAL_BCB, *PINTERNAL_BCB;
#define CONTAINING_RECORD(p,t,f) ((t *)((char *)(p)-offsetof(t,f)))
#define STATUS_SUCCESS 0
#define ASSERT(e) do { if (!(e)) { fprintf(stderr,"Fixture invariant: %s\n",#e); exit(2); } } while (0)
#define CHECK(e) do { if (!(e)) { fprintf(stderr,"%s:%d: %s\n",__FILE__,__LINE__,#e); ++Failures; } } while (0)

static NPAGED_LOOKASIDE_LIST iBcbLookasideList;
static SECTION_OBJECT_POINTERS Section;
static FILE_OBJECT File;
static ROS_SHARED_CACHE_MAP Map;
static ROS_VACB FixtureVacb;
static ULONG Memory[8], Disk[8];
static BOOLEAN SegmentDirty[8];
static unsigned Failures, Tests, Marks, Flushes, Frees, Releases, Locks, Removed;
static ERESOURCE_THREAD ReleasedThread;
static int64_t LastMarkOffset;
static ULONG LastMarkLength;

static void Trace(unsigned channel, const char *format, ...)
{
    (void)channel; (void)format;
}

static void Flush(void)
{
    unsigned i;
    ++Flushes;
    FixtureVacb.Dirty = FALSE;
    for (i=0;i<8;++i) {
        if (SegmentDirty[i]) Disk[i] = Memory[i];
        SegmentDirty[i] = FALSE;
    }
}
static void KeAcquireSpinLock(ULONG *lock, PKIRQL old)
{
    (void)lock; *old=0; ++Locks;
}
static void KeReleaseSpinLock(ULONG *lock, KIRQL old)
{
    (void)lock; (void)old; ASSERT(Locks != 0); --Locks;
}
static void RemoveEntryList(PLIST_ENTRY entry) { (void)entry; ++Removed; }
static void ExReleaseResourceForThreadLite(PERESOURCE resource, ERESOURCE_THREAD thread)
{
    (void)resource; ++Releases; ReleasedThread=thread;
}
static void ExReleaseResourceLite(PERESOURCE resource) { (void)resource; ++Releases; }
static void ExDeleteResourceLite(PERESOURCE resource) { (void)resource; }
static void ExFreeToNPagedLookasideList(PNPAGED_LOOKASIDE_LIST list, PVOID entry)
{
    (void)list; (void)entry; ++Frees;
}
static long MmMakeSegmentDirty(PSECTION_OBJECT_POINTERS section, int64_t offset, ULONG length)
{
    unsigned i;
    ASSERT(section == &Section);
    ASSERT(offset >= 0 && offset+length <= 8*4096);
    ++Marks; LastMarkOffset=offset; LastMarkLength=length;
    for (i=(unsigned)(offset/4096); i<(unsigned)((offset+length+4095)/4096); ++i)
        SegmentDirty[i]=TRUE;
    return STATUS_SUCCESS;
}
static void CcRosMarkDirtyVacb(PROS_VACB vacb) { ASSERT(vacb == &FixtureVacb); vacb->Dirty=TRUE; }
static void CcRosReleaseVacb(PROS_SHARED_CACHE_MAP map, PROS_VACB vacb, BOOLEAN valid, BOOLEAN dirty)
{
    ASSERT(map == &Map && vacb == &FixtureVacb); (void)valid;
    if (dirty) vacb->Dirty=TRUE;
}
static void CcFlushCache(PSECTION_OBJECT_POINTERS section, PLARGE_INTEGER offset, ULONG length, PIO_STATUS_BLOCK iosb)
{
    ASSERT(section == &Section); (void)offset; (void)length;
    Flush(); iosb->Status=STATUS_SUCCESS; iosb->Information=4096;
}

#include "cache-pin-functions.inc"

static INTERNAL_BCB Bcb, OtherBcb;

static void InitializeBcb(PINTERNAL_BCB bcb, ULONG pins, int16_t refs, ULONG page)
{
    memset(bcb,0,sizeof(*bcb));
    bcb->Vacb=&FixtureVacb; bcb->PinCount=pins; bcb->RefCount=refs;
    bcb->PFCB.MappedFileOffset.QuadPart=page*4096;
    bcb->PFCB.MappedLength=4096;
}
static void Reset(ULONG pins, int16_t refs)
{
    memset(Memory,0,sizeof(Memory)); memset(Disk,0,sizeof(Disk));
    memset(SegmentDirty,0,sizeof(SegmentDirty));
    File.SectionObjectPointer=&Section; Map.FileObject=&File;
    FixtureVacb.SharedCacheMap=&Map; FixtureVacb.Dirty=FALSE;
    Marks=Flushes=Frees=Releases=Locks=Removed=0; ReleasedThread=0;
    LastMarkOffset=-1; LastMarkLength=0;
    InitializeBcb(&Bcb,pins,refs,1);
}
static void Unpin(PINTERNAL_BCB bcb)
{
    CcUnpinDataForThread(&bcb->PFCB,(ERESOURCE_THREAD)0x1234);
}
static void CompleteCase(void)
{
    CHECK(Locks == 0);
    ++Tests;
}

int main(void)
{
    IO_STATUS_BLOCK iosb;
    Reset(1,1);
    Unpin(&Bcb);
    CHECK(Marks == 0 && Frees == 1 && Removed == 1 && Releases == 1);
    CHECK(ReleasedThread == (ERESOURCE_THREAD)0x1234);
    CompleteCase();

    Reset(1,1);
    Memory[1]=600;
    CcSetDirtyPinnedData(&Bcb.PFCB,NULL);
    Unpin(&Bcb); Flush();
    CHECK(Disk[1] == 600);
    CompleteCase();

    Reset(1,1);
    CcSetDirtyPinnedData(&Bcb.PFCB,NULL);
    Flush(); /* Cache/MM writeback occurs before the caller's actual update. */
    Memory[1]=600;
    Unpin(&Bcb);
    CHECK(FixtureVacb.Dirty && SegmentDirty[1]);
    Flush();
    CHECK(Disk[1] == 600);
    CompleteCase();

    Reset(1,2); /* One pin plus the filesystem's retained repin reference. */
    CcSetDirtyPinnedData(&Bcb.PFCB,NULL);
    Flush(); Memory[1]=601; Unpin(&Bcb);
    CcUnpinRepinnedBcb(&Bcb.PFCB,TRUE,&iosb);
    CHECK(iosb.Status == STATUS_SUCCESS && Disk[1] == 601 && Frees == 1);
    CompleteCase();

    Reset(2,2); /* A later writer must survive another intervening flush. */
    CcSetDirtyPinnedData(&Bcb.PFCB,NULL);
    Flush(); Memory[1]=602; Unpin(&Bcb); Flush();
    Memory[1]=603; Unpin(&Bcb); Flush();
    CHECK(Disk[1] == 603 && Frees == 1 && Releases == 2);
    CompleteCase();

    Reset(1,1);
    InitializeBcb(&OtherBcb,1,1,2);
    CcSetDirtyPinnedData(&Bcb.PFCB,NULL);
    CcSetDirtyPinnedData(&OtherBcb.PFCB,NULL);
    Flush(); Memory[1]=604; Memory[2]=605;
    Unpin(&Bcb); Flush(); Unpin(&OtherBcb); Flush();
    CHECK(Disk[1] == 604 && Disk[2] == 605 && Frees == 2);
    CompleteCase();

    Reset(1,2);
    CcSetDirtyPinnedData(&Bcb.PFCB,NULL);
    Flush(); Memory[1]=606; Unpin(&Bcb);
    CcUnpinRepinnedBcb(&Bcb.PFCB,FALSE,&iosb);
    CHECK(FixtureVacb.Dirty && Frees == 1);
    Flush(); CHECK(Disk[1] == 606);
    CompleteCase();

    Reset(1,2);
    Unpin(&Bcb);
    CcUnpinRepinnedBcb(&Bcb.PFCB,TRUE,&iosb);
    CHECK(Marks == 0 && Flushes == 1 && Frees == 1 && Releases == 1);
    CompleteCase();

    Reset(1,1);
    Bcb.PFCB.MappedFileOffset.QuadPart=4096+64;
    Bcb.PFCB.MappedLength=16;
    CcSetDirtyPinnedData(&Bcb.PFCB,NULL);
    Flush(); Memory[1]=607; Unpin(&Bcb);
    CHECK(LastMarkOffset == 4096+64 && LastMarkLength == 16);
    CHECK(SegmentDirty[1] && !SegmentDirty[0] && !SegmentDirty[2]);
    Flush(); CHECK(Disk[1] == 607);
    CompleteCase();

    printf("CC pinned data: %u cases, %u failures\n",Tests,Failures);
    return Failures ? 1 : 0;
}
