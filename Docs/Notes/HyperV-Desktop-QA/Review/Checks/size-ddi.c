/* Demand-start guest conformance test. A kernel-only test file on our own
 * device represents one zero-filled stream. No storage or IOCTLs. Results
 * are DWORDs on the test service key. Run once per cold boot. */
#include <ntifs.h>

typedef NTSTATUS (NTAPI *SET_SIZES)(PFILE_OBJECT, PCC_FILE_SIZES);
typedef struct {
    FSRTL_ADVANCED_FCB_HEADER Header;
    ERESOURCE Resource, Paging;
    FAST_MUTEX Mutex;
    SECTION_OBJECT_POINTERS Sections;
    CACHE_UNINITIALIZE_EVENT Closed[2];
} STREAM;

static HANDLE Results;
static STREAM *Stream;
static CACHE_MANAGER_CALLBACKS Callbacks;
static WCHAR FilePath[144];

static VOID Record(PCWSTR Name, ULONG Value)
{
    UNICODE_STRING Key;
    RtlInitUnicodeString(&Key, Name);
    ZwSetValueKey(Results, &Key, 0, REG_DWORD, &Value, sizeof(Value));
}

static BOOLEAN NTAPI Acquire(PVOID Context, BOOLEAN Wait)
{
    UNREFERENCED_PARAMETER(Context);
    return ExAcquireResourceSharedLite(&Stream->Paging, Wait);
}

static VOID NTAPI Release(PVOID Context)
{
    UNREFERENCED_PARAMETER(Context);
    ExReleaseResourceLite(&Stream->Paging);
}

static NTSTATUS NTAPI Dispatch(PDEVICE_OBJECT Device, PIRP Irp)
{
    PIO_STACK_LOCATION Stack = IoGetCurrentIrpStackLocation(Irp);
    NTSTATUS Status = STATUS_INVALID_DEVICE_REQUEST;
    ULONG Bytes = 0;
    UNREFERENCED_PARAMETER(Device);
    switch (Stack->MajorFunction) {
    case IRP_MJ_CREATE: {
        UNICODE_STRING Contents = RTL_CONSTANT_STRING(L"\\contents");
        PFILE_OBJECT File = Stack->FileObject;
        Status = STATUS_ACCESS_DENIED;
        if (Irp->RequestorMode == KernelMode &&
            RtlEqualUnicodeString(&File->FileName, &Contents, TRUE) &&
            !(Stack->Parameters.Create.SecurityContext->DesiredAccess & (FILE_WRITE_DATA | FILE_APPEND_DATA))) {
            File->FsContext = Stream;
            File->SectionObjectPointer = &Stream->Sections;
            Status = STATUS_SUCCESS;
            Bytes = FILE_OPENED;
        }
        break;
    }
    case IRP_MJ_CLEANUP:
    case IRP_MJ_CLOSE:
    case IRP_MJ_FLUSH_BUFFERS:
        Status = STATUS_SUCCESS;
        break;
    case IRP_MJ_QUERY_INFORMATION:
        if (Stack->Parameters.QueryFile.FileInformationClass == FileStandardInformation &&
            Stack->Parameters.QueryFile.Length >= sizeof(FILE_STANDARD_INFORMATION)) {
            PFILE_STANDARD_INFORMATION Info = Irp->AssociatedIrp.SystemBuffer;
            RtlZeroMemory(Info, sizeof(*Info));
            Info->NumberOfLinks = 1;
            Info->AllocationSize = Stream->Header.AllocationSize;
            Info->EndOfFile = Stream->Header.FileSize;
            Bytes = sizeof(*Info);
            Status = STATUS_SUCCESS;
        }
        break;
    case IRP_MJ_READ:
        if (Irp->MdlAddress && Stack->Parameters.Read.ByteOffset.QuadPart >= 0 &&
            Stack->Parameters.Read.ByteOffset.QuadPart < Stream->Header.FileSize.QuadPart) {
            PVOID Buffer = MmGetSystemAddressForMdlSafe(Irp->MdlAddress, NormalPagePriority);
            if (!Buffer) { Status = STATUS_INSUFFICIENT_RESOURCES; break; }
            Bytes = (ULONG)min(Stream->Header.FileSize.QuadPart - Stack->Parameters.Read.ByteOffset.QuadPart,
                               Stack->Parameters.Read.Length);
            RtlZeroMemory(Buffer, Bytes);
            Status = STATUS_SUCCESS;
        } else {
            Status = STATUS_END_OF_FILE;
        }
        break;
    }
    Irp->IoStatus.Status = Status;
    Irp->IoStatus.Information = Bytes;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return Status;
}

static PFILE_OBJECT OpenStream(PDEVICE_OBJECT Device)
{
    PFILE_OBJECT File = IoCreateStreamFileObject(NULL, Device);
    File->FsContext = Stream;
    File->SectionObjectPointer = &Stream->Sections;
    return File;
}

static CC_FILE_SIZES Sizes(VOID)
{
    CC_FILE_SIZES S;
    S.AllocationSize = Stream->Header.AllocationSize;
    S.FileSize = Stream->Header.FileSize;
    S.ValidDataLength = Stream->Header.ValidDataLength;
    return S;
}

static NTSTATUS ClosePair(PDEVICE_OBJECT Device)
{
    PFILE_OBJECT A = OpenStream(Device), B = OpenStream(Device);
    CC_FILE_SIZES S = Sizes();
    LARGE_INTEGER Timeout;
    NTSTATUS Status;
    /* Each object has its own private cache map on the same stream. */
    CcInitializeCacheMap(A, &S, FALSE, &Callbacks, Stream);
    CcInitializeCacheMap(B, &S, FALSE, &Callbacks, Stream);
    Record(L"PeerPresent", A->PrivateCacheMap != NULL && B->PrivateCacheMap != NULL);
    KeInitializeEvent(&Stream->Closed[0].Event, NotificationEvent, FALSE);
    KeInitializeEvent(&Stream->Closed[1].Event, NotificationEvent, FALSE);
    Record(L"FirstReturn", CcUninitializeCacheMap(A, NULL, &Stream->Closed[0]));
    Record(L"FirstEventAtReturn", KeReadStateEvent(&Stream->Closed[0].Event));
    Record(L"PeerStillCached", B->PrivateCacheMap != NULL);
    Record(L"LastReturn", CcUninitializeCacheMap(B, NULL, &Stream->Closed[1]));
    Record(L"LastEventAtReturn", KeReadStateEvent(&Stream->Closed[1].Event));
    Timeout.QuadPart = -30LL * 1000 * 1000 * 10;
    Status = KeWaitForSingleObject(&Stream->Closed[1].Event, Executive, KernelMode, FALSE, &Timeout);
    if (Status != STATUS_SUCCESS) return STATUS_IO_TIMEOUT;
    if (!KeReadStateEvent(&Stream->Closed[0].Event)) return STATUS_IO_TIMEOUT;
    ObDereferenceObject(B);
    ObDereferenceObject(A);
    return STATUS_SUCCESS;
}

/* Measure the data-section-only DDI on our own zero stream. First change
 * only VDL. Then ask CC about removing an unmapped suffix while a read-only
 * prefix is live; do not change the backing EOF or access the view again.
 * Compare that result to the MM admission check and to later direct growth. */
static NTSTATUS DataSection(SET_SIZES SetSizes, BOOLEAN Cached)
{
    PFILE_OBJECT F = NULL;
    HANDLE File = NULL, Section = NULL;
    OBJECT_ATTRIBUTES Attr;
    IO_STATUS_BLOCK Io;
    UNICODE_STRING Path;
    SIZE_T Length = PAGE_SIZE;
    PVOID View = NULL;
    CC_FILE_SIZES S = Sizes();
    NTSTATUS Status;
    RtlInitUnicodeString(&Path, FilePath);
    InitializeObjectAttributes(&Attr, &Path, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);
    Status = ZwOpenFile(&File, FILE_GENERIC_READ, &Attr, &Io, FILE_SHARE_READ, FILE_SYNCHRONOUS_IO_NONALERT);
    Record(L"OpenFileStatus", Status);
    if (!NT_SUCCESS(Status)) goto done;
    Status = ObReferenceObjectByHandle(File, FILE_READ_DATA, *IoFileObjectType, KernelMode, (PVOID *)&F, NULL);
    if (!NT_SUCCESS(Status)) goto done;
    if (Cached)
        CcInitializeCacheMap(F, &S, FALSE, &Callbacks, Stream);
    InitializeObjectAttributes(&Attr, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
    Status = ZwCreateSection(&Section, SECTION_MAP_READ, &Attr, &S.FileSize, PAGE_READONLY, SEC_COMMIT, File);
    Record(L"CreateSectionStatus", Status);
    if (!NT_SUCCESS(Status)) goto done;
    Status = ZwMapViewOfSection(Section, NtCurrentProcess(), &View, 0, 0, NULL, &Length,
                                ViewUnmap, 0, PAGE_READONLY);
    Record(L"MapStatus", Status);
    if (!NT_SUCCESS(Status)) goto done;
    Record(Cached ? L"SharedDataSectionPresent" : L"DataSectionPresent", Stream->Sections.DataSectionObject != NULL);
    Record(Cached ? L"SharedCaseCachePresent" : L"SharedCachePresent", Stream->Sections.SharedCacheMap != NULL);
    Record(L"MappedPrefixByte", *(volatile UCHAR *)View);
    S.ValidDataLength.QuadPart--;
    Stream->Header.ValidDataLength = S.ValidDataLength;
    Record(Cached ? L"SharedUnchangedStatus" : L"UnchangedStatus", SetSizes(F, &S));
    {
        CC_FILE_SIZES Proposed = S;
        Proposed.FileSize.QuadPart = Proposed.ValidDataLength.QuadPart = 2 * PAGE_SIZE;
        Record(L"PrefixViewBytes", (ULONG)Length);
        Record(L"PrefixMayTruncate", MmCanFileBeTruncated(&Stream->Sections, &Proposed.FileSize));
        Record(Cached ? L"SharedTailStatus" : L"MappedTailStatus", SetSizes(F, &Proposed));
        if (Cached) Record(L"SharedEofAfterRequest", (ULONG)CcGetFileSizePointer(F)->QuadPart);
    }
    Status = ZwUnmapViewOfSection(NtCurrentProcess(), View);
    if (!NT_SUCCESS(Status)) goto done;
    ZwClose(Section); Section = NULL;
    S.FileSize.QuadPart += PAGE_SIZE;
    S.AllocationSize.QuadPart += PAGE_SIZE;
    Stream->Header.FileSize = S.FileSize;
    Stream->Header.AllocationSize = S.AllocationSize;
    Record(Cached ? L"SharedNextGrowthStatus" : L"NextGrowthStatus", SetSizes(F, &S));
    if (Cached) Record(L"SharedEofAfterGrowth", (ULONG)CcGetFileSizePointer(F)->QuadPart);
done:
    if (Section) ZwClose(Section);
    if (Cached && F && F->PrivateCacheMap) {
        LARGE_INTEGER Timeout;
        KeInitializeEvent(&Stream->Closed[0].Event, NotificationEvent, FALSE);
        CcUninitializeCacheMap(F, NULL, &Stream->Closed[0]);
        Timeout.QuadPart = -30LL * 1000 * 1000 * 10;
        if (KeWaitForSingleObject(&Stream->Closed[0].Event, Executive, KernelMode, FALSE, &Timeout) != STATUS_SUCCESS)
            return STATUS_IO_TIMEOUT;
    }
    if (F) ObDereferenceObject(F);
    if (File) ZwClose(File);
    return Status;
}

DRIVER_INITIALIZE DriverEntry;
NTSTATUS NTAPI DriverEntry(PDRIVER_OBJECT Driver, PUNICODE_STRING RegistryPath)
{
    OBJECT_ATTRIBUTES Attr;
    UNICODE_STRING Name = RTL_CONSTANT_STRING(L"CcSetFileSizesEx");
    UNICODE_STRING Path, DriverName;
    SET_SIZES SetSizes = (SET_SIZES)MmGetSystemRoutineAddress(&Name);
    PDEVICE_OBJECT Device;
    NTSTATUS Status;
    ULONG I;
    if (!SetSizes) return STATUS_PROCEDURE_NOT_FOUND;
    InitializeObjectAttributes(&Attr, RegistryPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
    Status = ZwOpenKey(&Results, KEY_SET_VALUE, &Attr);
    if (!NT_SUCCESS(Status)) return Status;
    /* Derive a unique local device name for each isolated guest run. */
    DriverName = *RegistryPath;
    while (DriverName.Length && DriverName.Buffer[DriverName.Length / sizeof(WCHAR) - 1] != L'\\')
        DriverName.Length -= sizeof(WCHAR);
    DriverName.Buffer += DriverName.Length / sizeof(WCHAR);
    DriverName.Length = DriverName.MaximumLength = RegistryPath->Length - DriverName.Length;
    RtlInitEmptyUnicodeString(&Path, FilePath, sizeof(FilePath));
    Status = RtlAppendUnicodeToString(&Path, L"\\Device\\");
    if (NT_SUCCESS(Status)) Status = RtlAppendUnicodeStringToString(&Path, &DriverName);
    if (!NT_SUCCESS(Status) || Path.Length + sizeof(L"\\contents") > sizeof(FilePath)) {
        ZwClose(Results); return STATUS_NAME_TOO_LONG;
    }
    Status = IoCreateDevice(Driver, sizeof(STREAM), &Path, FILE_DEVICE_DISK_FILE_SYSTEM,
                            FILE_DEVICE_SECURE_OPEN, FALSE, &Device);
    if (!NT_SUCCESS(Status)) { ZwClose(Results); return Status; }
    RtlAppendUnicodeToString(&Path, L"\\contents");
    Stream = Device->DeviceExtension;
    ExInitializeFastMutex(&Stream->Mutex);
    FsRtlSetupAdvancedHeader(&Stream->Header, &Stream->Mutex);
    ExInitializeResourceLite(&Stream->Resource);
    ExInitializeResourceLite(&Stream->Paging);
    Stream->Header.NodeTypeCode = 0xACE;
    Stream->Header.NodeByteSize = sizeof(*Stream);
    Stream->Header.Resource = &Stream->Resource;
    Stream->Header.PagingIoResource = &Stream->Paging;
    Stream->Header.AllocationSize.QuadPart = 16 * PAGE_SIZE;
    Stream->Header.FileSize.QuadPart = Stream->Header.ValidDataLength.QuadPart = 16 * PAGE_SIZE - 123;
    Callbacks.AcquireForLazyWrite = Callbacks.AcquireForReadAhead = Acquire;
    Callbacks.ReleaseFromLazyWrite = Callbacks.ReleaseFromReadAhead = Release;
    for (I = 0; I <= IRP_MJ_MAXIMUM_FUNCTION; ++I) Driver->MajorFunction[I] = Dispatch;
    Device->Flags |= DO_DIRECT_IO;
    Device->Flags &= ~DO_DEVICE_INITIALIZING;
    FsRtlEnterFileSystem();
    __try {
        Status = ClosePair(Device);
        Record(L"CloseStatus", Status);
        if (NT_SUCCESS(Status)) {
            Status = DataSection(SetSizes, FALSE);
            Record(L"SectionStatus", Status);
            if (NT_SUCCESS(Status)) {
                Status = DataSection(SetSizes, TRUE);
                Record(L"SharedSectionStatus", Status);
            }
        }
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        Status = GetExceptionCode();
    }
    FsRtlExitFileSystem();
    Record(L"Finished", Status);
    ZwClose(Results);
    /* No user entry point and no unload: keep the stream storage available
     * until shutdown, including in a timeout or delayed MM close. */
    return STATUS_SUCCESS;
}
