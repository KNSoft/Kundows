/*
 * Guest-only CC lifecycle checks. Own synthetic stream; no storage attachment,
 * hooks, raw-disk access, or I/O to a real filesystem from the test driver.
 * The sparse backing store contains just the watched page. All other bytes read
 * as zero. A write can fail or remain pending behind an explicit event barrier.
 */
#include <ntifs.h>
#include "cccycle.h"

#define KW_TAG 'cCwK'
#define KW_SIZE 3675648
#define KW_PAGE 3432448
#define KW_TIMEOUT ((LONGLONG)-50000000)

typedef struct {
    FSRTL_ADVANCED_FCB_HEADER Header;
    SECTION_OBJECT_POINTERS Sections;
    FAST_MUTEX HeaderMutex;
    PFILE_OBJECT File;
    ULONG Reads, Writes, FailedWrites, LazyWrites;
    volatile LONG FailWrite, HoldWrite;
    PIRP HeldIrp;
    ULONG HeldLength;
    KEVENT WriteEntered, ThreadStarted, ThreadDone, LazyDone;
    CACHE_UNINITIALIZE_EVENT Uninit;
    NTSTATUS ThreadStatus;
    HANDLE Thread;
    UCHAR Disk[PAGE_SIZE], Snapshot[PAGE_SIZE];
} KW_STREAM;

static PDEVICE_OBJECT KwDevice;
static PDRIVER_OBJECT KwDriver;
static volatile LONG KwRunning;
static BOOLEAN KwAbandoned;
DRIVER_UNLOAD KwUnload;

static NTSTATUS Finish(PIRP Irp, NTSTATUS Status, ULONG_PTR Bytes)
{
    Irp->IoStatus.Status = Status;
    Irp->IoStatus.Information = Bytes;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return Status;
}

static VOID Check(KW_CC_REPORT *R, ULONG Id, ULONG Actual, ULONG Expected)
{
    if (R->Count >= KW_CC_MAX_CHECKS) {
        R->Status = STATUS_BUFFER_OVERFLOW;
        return;
    }
    R->Checks[R->Count].Id = Id;
    R->Checks[R->Count].Actual = Actual;
    R->Checks[R->Count++].Expected = Expected;
}

static NTSTATUS WaitEvent(PKEVENT Event, LONGLONG Timeout)
{
    LARGE_INTEGER T;
    T.QuadPart = Timeout;
    return KeWaitForSingleObject(Event, Executive, KernelMode, FALSE, &T);
}

static BOOLEAN NTAPI LazyAcquire(PVOID Context, BOOLEAN Wait)
{
    KW_STREAM *S = Context;
    UNREFERENCED_PARAMETER(Wait);
    InterlockedIncrement((PLONG)&S->LazyWrites);
    return TRUE;
}

static VOID NTAPI LazyRelease(PVOID Context)
{
    KW_STREAM *S = Context;
    KeSetEvent(&S->LazyDone, IO_NO_INCREMENT, FALSE);
}

static BOOLEAN NTAPI ReadAheadAcquire(PVOID Context, BOOLEAN Wait)
{
    UNREFERENCED_PARAMETER(Context);
    UNREFERENCED_PARAMETER(Wait);
    return TRUE;
}

static VOID NTAPI ReadAheadRelease(PVOID Context)
{
    UNREFERENCED_PARAMETER(Context);
}

static CACHE_MANAGER_CALLBACKS KwCallbacks = {
    LazyAcquire, LazyRelease, ReadAheadAcquire, ReadAheadRelease
};

static NTSTATUS NTAPI AcquireFlush(PFILE_OBJECT File, PDEVICE_OBJECT Device)
{
    UNREFERENCED_PARAMETER(File);
    UNREFERENCED_PARAMETER(Device);
    /* Intentionally no filesystem serialization: test CC's own interleaving. */
    return STATUS_SUCCESS;
}

static NTSTATUS NTAPI ReleaseFlush(PFILE_OBJECT File, PDEVICE_OBJECT Device)
{
    UNREFERENCED_PARAMETER(File);
    UNREFERENCED_PARAMETER(Device);
    return STATUS_SUCCESS;
}

static FAST_IO_DISPATCH KwFastIo;

static KW_STREAM *NewStream(VOID)
{
    KW_STREAM *S;
    CC_FILE_SIZES Sizes;
    S = ExAllocatePoolWithTag(NonPagedPool, sizeof(*S), KW_TAG);
    if (!S) ExRaiseStatus(STATUS_INSUFFICIENT_RESOURCES);
    RtlZeroMemory(S, sizeof(*S));
    ExInitializeFastMutex(&S->HeaderMutex);
    FsRtlSetupAdvancedHeader(&S->Header, &S->HeaderMutex);
    S->Header.NodeTypeCode = 0x0ACE;
    S->Header.NodeByteSize = sizeof(*S);
    Sizes.AllocationSize.QuadPart = KW_SIZE;
    Sizes.FileSize = Sizes.ValidDataLength = Sizes.AllocationSize;
    S->Header.AllocationSize = Sizes.AllocationSize;
    S->Header.FileSize = Sizes.FileSize;
    S->Header.ValidDataLength = Sizes.ValidDataLength;
    KeInitializeEvent(&S->WriteEntered, NotificationEvent, FALSE);
    KeInitializeEvent(&S->ThreadStarted, NotificationEvent, FALSE);
    KeInitializeEvent(&S->ThreadDone, NotificationEvent, FALSE);
    KeInitializeEvent(&S->LazyDone, NotificationEvent, FALSE);
    S->File = IoCreateStreamFileObject(NULL, KwDevice);
    S->File->FsContext = S;
    S->File->SectionObjectPointer = &S->Sections;
    CcInitializeCacheMap(S->File, &Sizes, TRUE, &KwCallbacks, S);
    CcSetAdditionalCacheAttributes(S->File, TRUE, TRUE);
    return S;
}

static PVOID PinOffset(KW_STREAM *S, ULONG Flags, PVOID *Bcb, ULONG Start)
{
    LARGE_INTEGER Offset;
    PVOID Buffer = NULL;
    Offset.QuadPart = Start;
    *Bcb = NULL;
    if (!CcPinRead(S->File, &Offset, PAGE_SIZE, Flags, Bcb, &Buffer))
        return NULL;
    return Buffer;
}

static PVOID Pin(KW_STREAM *S, ULONG Flags, PVOID *Bcb)
{
    return PinOffset(S, Flags, Bcb, KW_PAGE);
}

static PVOID Dirty(KW_STREAM *S, PVOID *Bcb, UCHAR Value, BOOLEAN Repin)
{
    PVOID Buffer = Pin(S, PIN_WAIT, Bcb);
    if (!Buffer) ExRaiseStatus(STATUS_INSUFFICIENT_RESOURCES);
    if (Repin) CcRepinBcb(*Bcb);
    CcSetDirtyPinnedData(*Bcb, NULL);
    RtlFillMemory(Buffer, PAGE_SIZE, Value);
    return Buffer;
}

static BOOLEAN Hit(KW_STREAM *S)
{
    PVOID Bcb;
    if (!Pin(S, PIN_WAIT | PIN_IF_BCB, &Bcb)) return FALSE;
    CcUnpinData(Bcb);
    return TRUE;
}

static NTSTATUS Flush(KW_STREAM *S)
{
    LARGE_INTEGER Offset;
    IO_STATUS_BLOCK Io = {0};
    Offset.QuadPart = KW_PAGE;
    CcFlushCache(&S->Sections, &Offset, PAGE_SIZE, &Io);
    return Io.Status;
}

static VOID NTAPI FlushThread(PVOID Context)
{
    KW_STREAM *S = Context;
    FsRtlEnterFileSystem();
    KeSetEvent(&S->ThreadStarted, IO_NO_INCREMENT, FALSE);
    S->ThreadStatus = Flush(S);
    FsRtlExitFileSystem();
    KeSetEvent(&S->ThreadDone, IO_NO_INCREMENT, FALSE);
    PsTerminateSystemThread(STATUS_SUCCESS);
}

static VOID CompleteHeld(KW_STREAM *S)
{
    PIRP Irp = S->HeldIrp;
    if (!Irp) return;
    S->HeldIrp = NULL;
    RtlCopyMemory(S->Disk, S->Snapshot, PAGE_SIZE);
    Finish(Irp, STATUS_SUCCESS, S->HeldLength);
}

static NTSTATUS JoinThread(KW_STREAM *S)
{
    NTSTATUS Status;
    LARGE_INTEGER T;
    if (!S->Thread) return STATUS_SUCCESS;
    T.QuadPart = KW_TIMEOUT;
    Status = ZwWaitForSingleObject(S->Thread, FALSE, &T);
    if (Status == STATUS_SUCCESS) {
        ZwClose(S->Thread);
        S->Thread = NULL;
    } else {
        /* Keep driver/context alive. The host must discard this VM run. */
        KwAbandoned = TRUE;
    }
    return Status;
}

static BOOLEAN Purge(KW_STREAM *S)
{
    return CcPurgeCacheSection(&S->Sections, NULL, 0, FALSE);
}

static VOID FreeStream(KW_STREAM *S)
{
    CompleteHeld(S);
    if (JoinThread(S) != STATUS_SUCCESS) return;
    KeInitializeEvent(&S->Uninit.Event, NotificationEvent, FALSE);
    CcUninitializeCacheMap(S->File, NULL, &S->Uninit);
    if (WaitEvent(&S->Uninit.Event, KW_TIMEOUT) != STATUS_SUCCESS) {
        KwAbandoned = TRUE;
        return;
    }
    S->File->FsContext = NULL;
    S->File->SectionObjectPointer = NULL;
    ObDereferenceObject(S->File);
    ExFreePoolWithTag(S, KW_TAG);
}

static VOID RunCase(ULONG Id, KW_CC_REPORT *R)
{
    KW_STREAM *S = NULL;
    PVOID Bcb = NULL, Buffer;
    IO_STATUS_BLOCK Io;
    NTSTATUS Status;
    ULONG I;
    __try {
        S = NewStream();
        switch (Id) {
        case 1: /* Last ordinary pin. */
            Dirty(S, &Bcb, 0x31, FALSE);
            CcUnpinData(Bcb); Bcb = NULL;
            Check(R, 1, Hit(S), TRUE);
            Check(R, 2, Flush(S), STATUS_SUCCESS);
            Check(R, 3, S->Disk[0], 0x31);
            Check(R, 4, Purge(S), TRUE);
            break;
        case 2: /* Repin survives ordinary unpin; then last repin is released. */
            Dirty(S, &Bcb, 0x32, TRUE);
            CcUnpinData(Bcb);
            Check(R, 1, Hit(S), TRUE);
            CcUnpinRepinnedBcb(Bcb, FALSE, &Io); Bcb = NULL;
            Check(R, 2, Hit(S), TRUE);
            Check(R, 3, Flush(S), STATUS_SUCCESS);
            Check(R, 4, S->Disk[0], 0x32);
            Check(R, 5, Purge(S), TRUE);
            break;
        case 3: /* Genuine error completion from this driver's paging WRITE. */
            Dirty(S, &Bcb, 0x33, TRUE);
            CcUnpinData(Bcb);
            S->FailWrite = 1;
            CcUnpinRepinnedBcb(Bcb, TRUE, &Io); Bcb = NULL;
            Check(R, 1, Io.Status, STATUS_IO_DEVICE_ERROR);
            Check(R, 2, S->Disk[0], 0);
            Check(R, 3, Hit(S), TRUE);
            Check(R, 4, S->FailedWrites != 0, TRUE);
            S->FailWrite = 0;
            Check(R, 5, Flush(S), STATUS_SUCCESS);
            Check(R, 6, S->Disk[0], 0x33);
            Check(R, 7, Purge(S), TRUE);
            break;
        case 4: /* Premark, flush, modify without another mark, ordinary unpin. */
            Buffer = Dirty(S, &Bcb, 0x34, FALSE);
            Check(R, 1, Flush(S), STATUS_SUCCESS);
            Check(R, 2, S->Disk[0], 0x34);
            RtlFillMemory(Buffer, PAGE_SIZE, 0x44);
            CcUnpinData(Bcb); Bcb = NULL;
            Check(R, 3, Flush(S), STATUS_SUCCESS);
            Check(R, 4, S->Disk[0], 0x44);
            Check(R, 5, Purge(S), TRUE);
            break;
        case 5: /* Pending first WRITE holds an A snapshot; a second pin writes B. */
            Dirty(S, &Bcb, 0x35, FALSE);
            CcUnpinData(Bcb); Bcb = NULL;
            S->HoldWrite = 1;
            Status = PsCreateSystemThread(&S->Thread, SYNCHRONIZE, NULL, NULL, NULL, FlushThread, S);
            Check(R, 1, Status, STATUS_SUCCESS);
            if (!NT_SUCCESS(Status)) break;
            Status = WaitEvent(&S->WriteEntered, KW_TIMEOUT);
            Check(R, 2, Status, STATUS_SUCCESS);
            if (Status == STATUS_SUCCESS) {
                Dirty(S, &Bcb, 0x45, FALSE);
                CcUnpinData(Bcb); Bcb = NULL;
            }
            CompleteHeld(S);
            Status = JoinThread(S);
            Check(R, 3, Status, STATUS_SUCCESS);
            if (Status != STATUS_SUCCESS) break;
            Check(R, 4, S->ThreadStatus, STATUS_SUCCESS);
            Check(R, 5, S->Disk[0], 0x35);
            Check(R, 6, Hit(S), TRUE);
            Check(R, 7, Flush(S), STATUS_SUCCESS);
            Check(R, 8, S->Disk[0], 0x45);
            Check(R, 9, Purge(S), TRUE);
            break;
        case 6: /* Actual lazy-writer callback, not a manual flush mislabeled lazy. */
            Dirty(S, &Bcb, 0x36, FALSE);
            CcUnpinData(Bcb); Bcb = NULL;
            CcSetAdditionalCacheAttributes(S->File, TRUE, FALSE);
            Status = WaitEvent(&S->LazyDone, -150000000LL);
            Check(R, 1, Status, STATUS_SUCCESS);
            CcSetAdditionalCacheAttributes(S->File, TRUE, TRUE);
            Check(R, 2, S->LazyWrites != 0, TRUE);
            Check(R, 3, S->Disk[0], 0x36);
            /* ReleaseFromLazyWrite precedes CC dropping its temporary refs.
             * Allow that epilogue to finish; do not mistake it for a leak. */
            for (I = 0; I < 100 && !Purge(S); ++I) {
                LARGE_INTEGER T; T.QuadPart = -100000;
                KeDelayExecutionThread(KernelMode, FALSE, &T);
            }
            Check(R, 4, I < 100, TRUE);
            break;
        case 7: /* Clean active pin cannot be purged; after unpin it can. */
            Buffer = Pin(S, PIN_WAIT, &Bcb);
            if (!Buffer) ExRaiseStatus(STATUS_INSUFFICIENT_RESOURCES);
            Check(R, 1, Purge(S), FALSE);
            CcUnpinData(Bcb); Bcb = NULL;
            Check(R, 2, Purge(S), TRUE);
            break;
        case 8: /* Successful writeback followed by truncation of the whole stream. */
        {
            CC_FILE_SIZES Sizes = {0};
            Dirty(S, &Bcb, 0x38, FALSE);
            CcUnpinData(Bcb); Bcb = NULL;
            Check(R, 1, Flush(S), STATUS_SUCCESS);
            Check(R, 2, S->Disk[0], 0x38);
            S->Header.AllocationSize = Sizes.AllocationSize;
            S->Header.FileSize = Sizes.FileSize;
            S->Header.ValidDataLength = Sizes.ValidDataLength;
            CcSetFileSizes(S->File, &Sizes);
            Check(R, 3, Purge(S), TRUE);
            break;
        }
        case 9: /* Failed whole-view purge must not forget another dirty page. */
        {
            PVOID Other = NULL;
            Dirty(S, &Bcb, 0x39, FALSE);
            CcUnpinData(Bcb); Bcb = NULL;
            if (!PinOffset(S, PIN_WAIT, &Other, KW_PAGE+PAGE_SIZE))
                ExRaiseStatus(STATUS_INSUFFICIENT_RESOURCES);
            Check(R, 1, Purge(S), FALSE);
            Check(R, 2, Hit(S), TRUE);
            CcUnpinData(Other);
            Check(R, 3, Flush(S), STATUS_SUCCESS);
            Check(R, 4, S->Disk[0], 0x39);
            Check(R, 5, Purge(S), TRUE);
            break;
        }
        case 10: /* A lazy WRITE is pending while an explicit flush enters. */
        {
            NTSTATUS Early;
            Dirty(S, &Bcb, 0x3A, FALSE);
            CcUnpinData(Bcb); Bcb = NULL;
            S->HoldWrite = 1;
            CcSetAdditionalCacheAttributes(S->File, TRUE, FALSE);
            Status = WaitEvent(&S->WriteEntered, -150000000LL);
            Check(R, 1, Status, STATUS_SUCCESS);
            if (Status != STATUS_SUCCESS) break;
            Status = PsCreateSystemThread(&S->Thread, SYNCHRONIZE, NULL, NULL, NULL, FlushThread, S);
            Check(R, 2, Status, STATUS_SUCCESS);
            if (!NT_SUCCESS(Status)) { CompleteHeld(S); break; }
            Check(R, 3, WaitEvent(&S->ThreadStarted, KW_TIMEOUT), STATUS_SUCCESS);
            Early = WaitEvent(&S->ThreadDone, -2000000);
            /* Either refuse the collision, or wait until the old write ends.
             * Success before completing that write is not acceptable. */
            Check(R, 4, Early == STATUS_SUCCESS ? S->ThreadStatus : Early,
                  Early == STATUS_SUCCESS ? STATUS_DEVICE_BUSY : STATUS_TIMEOUT);
            CompleteHeld(S);
            Status = JoinThread(S);
            Check(R, 5, Status, STATUS_SUCCESS);
            if (Status != STATUS_SUCCESS) break;
            Check(R, 6, WaitEvent(&S->LazyDone, KW_TIMEOUT), STATUS_SUCCESS);
            CcSetAdditionalCacheAttributes(S->File, TRUE, TRUE);
            Check(R, 7, S->ThreadStatus,
                  Early == STATUS_SUCCESS ? STATUS_DEVICE_BUSY : STATUS_SUCCESS);
            Check(R, 8, Flush(S), STATUS_SUCCESS);
            Check(R, 9, S->Disk[0], 0x3A);
            for (I = 0; I < 100 && !Purge(S); ++I) {
                LARGE_INTEGER T; T.QuadPart = -100000;
                KeDelayExecutionThread(KernelMode, FALSE, &T);
            }
            Check(R, 10, I < 100, TRUE);
            break;
        }
        default:
            R->Status = STATUS_INVALID_PARAMETER;
        }
        R->Reads = S->Reads; R->Writes = S->Writes;
        R->FailedWrites = S->FailedWrites; R->LazyWrites = S->LazyWrites;
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        R->Status = GetExceptionCode();
        KwAbandoned = TRUE;
    }
    /* An exceptional case may have unbalanced caller pins. Do not manufacture
     * success by deleting its backing context while CC can still reference it. */
    if (Bcb || KwAbandoned) {
        KwAbandoned = TRUE;
        R->Status = STATUS_UNSUCCESSFUL;
        return;
    }
    if (S) FreeStream(S);
    if (KwAbandoned) R->Status = STATUS_IO_TIMEOUT;
}

static NTSTATUS NTAPI Dispatch(PDEVICE_OBJECT Device, PIRP Irp)
{
    PIO_STACK_LOCATION Sp = IoGetCurrentIrpStackLocation(Irp);
    KW_STREAM *S = Sp->FileObject ? Sp->FileObject->FsContext : NULL;
    UNREFERENCED_PARAMETER(Device);
    if (Sp->MajorFunction == IRP_MJ_DEVICE_CONTROL) {
        KW_CC_REPORT *R = Irp->AssociatedIrp.SystemBuffer;
        ULONG Id;
        if (Sp->Parameters.DeviceIoControl.IoControlCode != KW_CC_IOCTL ||
            Sp->Parameters.DeviceIoControl.InputBufferLength != sizeof(Id) ||
            Sp->Parameters.DeviceIoControl.OutputBufferLength < sizeof(*R))
            return Finish(Irp, STATUS_INVALID_PARAMETER, 0);
        Id = *(PULONG)R;
        if (Id < 1 || Id > KW_CC_CASES) return Finish(Irp, STATUS_INVALID_PARAMETER, 0);
        if (KeGetCurrentIrql() != PASSIVE_LEVEL ||
            InterlockedCompareExchange(&KwRunning, 1, 0))
            return Finish(Irp, STATUS_DEVICE_BUSY, 0);
        if (KwAbandoned) {
            InterlockedExchange(&KwRunning, 0);
            return Finish(Irp, STATUS_DEVICE_BUSY, 0);
        }
        KwDriver->DriverUnload = NULL;
        RtlZeroMemory(R, sizeof(*R));
        R->Version = KW_CC_VERSION; R->CaseId = Id;
        FsRtlEnterFileSystem();
        RunCase(Id, R);
        FsRtlExitFileSystem();
        if (!KwAbandoned) KwDriver->DriverUnload = KwUnload;
        InterlockedExchange(&KwRunning, 0);
        return Finish(Irp, STATUS_SUCCESS, sizeof(*R));
    }
    if (Sp->MajorFunction == IRP_MJ_READ || Sp->MajorFunction == IRP_MJ_WRITE) {
        LARGE_INTEGER Offset = Sp->Parameters.Read.ByteOffset;
        ULONG Length = Sp->Parameters.Read.Length;
        PUCHAR Buffer;
        if (!S || !Irp->MdlAddress || Offset.QuadPart < 0 || Length > KW_SIZE ||
            Offset.QuadPart + Length > KW_SIZE)
            return Finish(Irp, STATUS_INVALID_PARAMETER, 0);
        Buffer = MmGetSystemAddressForMdlSafe(Irp->MdlAddress, NormalPagePriority);
        if (!Buffer) return Finish(Irp, STATUS_INSUFFICIENT_RESOURCES, 0);
        if (Sp->MajorFunction == IRP_MJ_READ) {
            InterlockedIncrement((PLONG)&S->Reads);
            RtlZeroMemory(Buffer, Length);
            if (Offset.QuadPart <= KW_PAGE && Offset.QuadPart + Length >= KW_PAGE + PAGE_SIZE)
                RtlCopyMemory(Buffer + (KW_PAGE - Offset.LowPart), S->Disk, PAGE_SIZE);
        } else {
            InterlockedIncrement((PLONG)&S->Writes);
            if (InterlockedCompareExchange(&S->FailWrite, 0, 0)) {
                InterlockedIncrement((PLONG)&S->FailedWrites);
                return Finish(Irp, STATUS_IO_DEVICE_ERROR, 0);
            }
            if (Offset.QuadPart != KW_PAGE || Length != PAGE_SIZE)
                return Finish(Irp, STATUS_INVALID_PARAMETER, 0);
            if (InterlockedCompareExchange(&S->HoldWrite, 0, 1) == 1) {
                RtlCopyMemory(S->Snapshot, Buffer, PAGE_SIZE);
                S->HeldLength = Length;
                IoMarkIrpPending(Irp);
                S->HeldIrp = Irp;
                KeSetEvent(&S->WriteEntered, IO_NO_INCREMENT, FALSE);
                return STATUS_PENDING;
            }
            RtlCopyMemory(S->Disk, Buffer, PAGE_SIZE);
        }
        return Finish(Irp, STATUS_SUCCESS, Length);
    }
    if (Sp->MajorFunction == IRP_MJ_QUERY_INFORMATION && S &&
        Sp->Parameters.QueryFile.FileInformationClass == FileStandardInformation &&
        Sp->Parameters.QueryFile.Length >= sizeof(FILE_STANDARD_INFORMATION)) {
        PFILE_STANDARD_INFORMATION Info = Irp->AssociatedIrp.SystemBuffer;
        RtlZeroMemory(Info, sizeof(*Info));
        Info->AllocationSize = S->Header.AllocationSize;
        Info->EndOfFile = S->Header.FileSize;
        Info->NumberOfLinks = 1;
        return Finish(Irp, STATUS_SUCCESS, sizeof(*Info));
    }
    if (Sp->MajorFunction == IRP_MJ_CREATE || Sp->MajorFunction == IRP_MJ_CLEANUP ||
        Sp->MajorFunction == IRP_MJ_CLOSE) return Finish(Irp, STATUS_SUCCESS, 0);
    return Finish(Irp, STATUS_INVALID_DEVICE_REQUEST, 0);
}

VOID NTAPI KwUnload(PDRIVER_OBJECT Driver)
{
    UNICODE_STRING Link = RTL_CONSTANT_STRING(L"\\DosDevices\\KwCcCycle");
    UNREFERENCED_PARAMETER(Driver);
    IoDeleteSymbolicLink(&Link);
    IoDeleteDevice(KwDevice);
}

NTSTATUS NTAPI DriverEntry(PDRIVER_OBJECT Driver, PUNICODE_STRING Registry)
{
    UNICODE_STRING Name = RTL_CONSTANT_STRING(L"\\Device\\KwCcCycle");
    UNICODE_STRING Link = RTL_CONSTANT_STRING(L"\\DosDevices\\KwCcCycle");
    NTSTATUS Status;
    ULONG I;
    UNREFERENCED_PARAMETER(Registry);
    KwDriver = Driver;
    for (I = 0; I <= IRP_MJ_MAXIMUM_FUNCTION; ++I) Driver->MajorFunction[I] = Dispatch;
    KwFastIo.SizeOfFastIoDispatch = sizeof(KwFastIo);
    KwFastIo.AcquireForCcFlush = AcquireFlush;
    KwFastIo.ReleaseForCcFlush = ReleaseFlush;
    Driver->FastIoDispatch = &KwFastIo;
    Driver->DriverUnload = KwUnload;
    Status = IoCreateDevice(Driver, 0, &Name, FILE_DEVICE_DISK_FILE_SYSTEM, 0, FALSE, &KwDevice);
    if (!NT_SUCCESS(Status)) return Status;
    Status = IoCreateSymbolicLink(&Link, &Name);
    if (!NT_SUCCESS(Status)) {
        IoDeleteDevice(KwDevice);
        return Status;
    }
    KwDevice->Flags |= DO_DIRECT_IO;
    KwDevice->Flags &= ~DO_DEVICE_INITIALIZING;
    return STATUS_SUCCESS;
}
