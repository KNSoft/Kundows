/* Host-side check. The fixture supplies mock I/O and includes the two
 * unmodified MM functions extracted from the pinned ReactOS source. */
#define main HistoricalCollisionTests
#include "../../MMFlushCollision/Assets/CacheFlushCollisionTests.c"
#undef main

static int
CheckRange(const char *name, int expectSuccess, ULONG_PTR expectBytes,
           ULONG expectedValue, LONG expectedWrites, int expectDirty)
{
    LARGE_INTEGER offset = {0};
    IO_STATUS_BLOCK iosb = {0};
    NTSTATUS status;
    int dirty, pass;

    iosb.Status = STATUS_UNSUCCESSFUL;
    iosb.Information = (ULONG_PTR)-1;
    status = MmFlushSegment(&FixtureSegment, &offset, PAGE_SIZE, &iosb);
    dirty = IS_DIRTY_SSE(FixtureSegment.Entry) != 0;
    pass = (NT_SUCCESS(status) == expectSuccess) &&
           (NT_SUCCESS(iosb.Status) == expectSuccess) &&
           iosb.Information == expectBytes && DiskValue == expectedValue &&
           Writes == expectedWrites && dirty == expectDirty;
    printf("%s: return=%08lx iosb=%08lx bytes=%llu dirty=%d disk=%lu calls=%ld %s\n",
           name, (ULONG)status, (ULONG)iosb.Status,
           (unsigned long long)iosb.Information, dirty, DiskValue, Writes,
           pass ? "PASS" : "FAIL");
    return !pass;
}

int main(void)
{
    int failed = 0;
    InitializeCriticalSection(&FixtureSegment.Lock);
    FirstInFlight = CreateEventW(NULL, TRUE, FALSE, NULL);
    SecondBoundary = CreateEventW(NULL, TRUE, FALSE, NULL);
    AllowFirstCompletion = CreateEventW(NULL, TRUE, FALSE, NULL);
    ASSERT(FirstInFlight && SecondBoundary && AllowFirstCompletion);

    Reset(FALSE, 1);
    failed += CheckRange("clean", 1, 0, 0, 0, 0);
    Reset(TRUE, 1);
    failed += CheckRange("write-ok", 1, PAGE_SIZE, 100, 1, 0);
    Reset(TRUE, 1);
    FailWrite = TRUE;
    failed += CheckRange("write-error", 0, 0, 0, 1, 1);
    FailWrite = FALSE;
    failed += CheckRange("retry", 1, PAGE_SIZE, 100, 2, 0);

    CloseHandle(AllowFirstCompletion);
    CloseHandle(SecondBoundary);
    CloseHandle(FirstInFlight);
    DeleteCriticalSection(&FixtureSegment.Lock);
    return failed + Failures;
}
