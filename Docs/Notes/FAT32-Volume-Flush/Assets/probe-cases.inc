/* Observations, not a PASS/FAIL contract imported from ReactOS.
 * No purge with active pins; all blocking operations run on a worker that
 * the control thread can unblock. Background gating keeps normal CC flags. */
static VOID Value(KW_CC_REPORT *R, ULONG Id, ULONG Actual)
{
    Check(R, Id, Actual, 0); /* Expected is reserved in this observation protocol. */
}

static VOID ObserveStop(KW_STREAM *S, ULONG Phase)
{
    DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
               "KWCC-OBS phase=%lu stream=%p file=%p bcb=%p worker=%p held=%p writes=%lu\n",
               Phase, S, S->File, S->WatchedBcb, S->FlushThreadObject, S->HeldIrp, S->Writes);
    if (KwBreak) DbgBreakPoint();
}

static NTSTATUS BeginFlush(KW_STREAM *S)
{
    return PsCreateSystemThread(&S->Thread, SYNCHRONIZE, NULL, NULL, NULL, FlushThread, S);
}

static VOID RunCase(ULONG Id, KW_CC_REPORT *R)
{
    KW_STREAM *S = NULL;
    PVOID Bcb = NULL, Buffer = NULL;
    IO_STATUS_BLOCK Io = {0};
    NTSTATUS Status = STATUS_SUCCESS, Early;
    ULONG I;
    __try {
        S = NewStream();
        Value(R, 1, Id == 5); /* configured DisableWriteBehind */
        if (Id == 5) CcSetAdditionalCacheAttributes(S->File, TRUE, TRUE);
        switch (Id) {
        case 1:
            Dirty(S, &Bcb, 0x51, FALSE);
            S->WatchedBcb = Bcb;
            CcUnpinData(Bcb); Bcb = NULL;
            ObserveStop(S, 101);
            Value(R, 2, Hit(S));
            Value(R, 3, Flush(S));
            S->WatchedBcb = NULL;
            Value(R, 4, S->Disk[0]);
            Value(R, 5, Hit(S));
            Value(R, 6, S->LastIoInformation);
            break;
        case 2:
            Dirty(S, &Bcb, 0x52, TRUE);
            S->WatchedBcb = Bcb;
            CcUnpinData(Bcb);
            Value(R, 2, Hit(S));
            S->FailWrite = 1;
            CcUnpinRepinnedBcb(Bcb, TRUE, &Io); Bcb = NULL;
            Value(R, 3, Io.Status);
            Value(R, 4, S->Disk[0]);
            Value(R, 5, S->FailedWrites);
            ObserveStop(S, 201);
            Value(R, 6, Hit(S));
            S->FailWrite = 0;
            Value(R, 7, Flush(S));
            S->WatchedBcb = NULL;
            Value(R, 8, S->Disk[0]);
            Value(R, 9, Hit(S));
            break;
        case 3:
            Dirty(S, &Bcb, 0x53, FALSE);
            S->WatchedBcb = Bcb;
            CcUnpinData(Bcb); Bcb = NULL;
            S->HoldWrite = 1;
            InterlockedExchange(&S->AllowLazy, 1);
            Status = WaitEvent(&S->WriteEntered, -150000000LL);
            Value(R, 2, Status);
            if (Status != STATUS_SUCCESS) break;
            Status = BeginFlush(S);
            Value(R, 3, Status);
            if (!NT_SUCCESS(Status)) break;
            Status = WaitEvent(&S->ThreadStarted, KW_TIMEOUT);
            Value(R, 4, Status);
            if (Status != STATUS_SUCCESS) break;
            Early = WaitEvent(&S->ThreadDone, -2000000);
            Value(R, 5, Early);
            Value(R, 6, Early == STATUS_SUCCESS ? S->ThreadStatus : STATUS_PENDING);
            ObserveStop(S, 301);
            CompleteHeld(S);
            Status = JoinThread(S);
            Value(R, 7, Status);
            if (Status != STATUS_SUCCESS) break;
            Value(R, 8, S->ThreadStatus);
            Status = WaitEvent(&S->LazyDone, KW_TIMEOUT);
            Value(R, 9, Status);
            if (Status != STATUS_SUCCESS) break;
            InterlockedExchange(&S->AllowLazy, 0);
            Value(R, 10, Flush(S));
            S->WatchedBcb = NULL;
            Value(R, 11, S->Disk[0]);
            break;
        case 4:
        case 5:
            Buffer = Dirty(S, &Bcb, 0x54, FALSE);
            S->WatchedBcb = Bcb;
            Status = BeginFlush(S);
            Value(R, 2, Status);
            if (!NT_SUCCESS(Status)) break;
            Status = WaitEvent(&S->ThreadStarted, KW_TIMEOUT);
            Value(R, 3, Status);
            if (Status != STATUS_SUCCESS) break;
            Early = WaitEvent(&S->ThreadDone, -2000000);
            Value(R, 4, Early);
            Value(R, 5, Early == STATUS_SUCCESS ? S->ThreadStatus : STATUS_PENDING);
            Value(R, 6, S->Writes);
            ObserveStop(S, Id == 4 ? 401 : 501);
            RtlFillMemory(Buffer, PAGE_SIZE, 0x64);
            CcUnpinData(Bcb); Bcb = NULL;
            Status = JoinThread(S);
            Value(R, 7, Status);
            if (Status != STATUS_SUCCESS) break;
            Value(R, 8, S->ThreadStatus);
            S->WatchedBcb = NULL;
            Value(R, 9, S->Disk[0]);
            Value(R, 10, Hit(S));
            Value(R, 11, Flush(S));
            Value(R, 12, S->Disk[0]);
            break;
        default:
            Status = STATUS_INVALID_PARAMETER;
        }
        if (Bcb) { CcUnpinData(Bcb); Bcb = NULL; }
        InterlockedExchange(&S->AllowLazy, 0);
        InterlockedExchange(&S->HoldWrite, 0);
        CompleteHeld(S);
        if (Status != STATUS_SUCCESS) {
            R->Status = Status;
        } else {
            /* No caller pins/worker threads remain. Do not test invalid purge. */
            CcSetAdditionalCacheAttributes(S->File, TRUE, TRUE);
            ExAcquireResourceExclusiveLite(&S->FileResource, TRUE);
            for (I = 0; I < 100 && !Purge(S); ++I) {
                LARGE_INTEGER T; T.QuadPart = -100000;
                KeDelayExecutionThread(KernelMode, FALSE, &T);
            }
            ExReleaseResourceLite(&S->FileResource);
            Value(R, 20, I < 100);
        }
        R->Reads = S->Reads; R->Writes = S->Writes;
        R->FailedWrites = S->FailedWrites; R->LazyWrites = S->LazyWrites;
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        R->Status = GetExceptionCode();
        KwAbandoned = TRUE;
    }
    if (Bcb || KwAbandoned) {
        KwAbandoned = TRUE;
        if (!R->Status) R->Status = STATUS_UNSUCCESSFUL;
        return;
    }
    if (S) FreeStream(S);
    if (KwAbandoned) R->Status = STATUS_IO_TIMEOUT;
}
