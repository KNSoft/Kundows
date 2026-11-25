/* Host-only boundaries. No guest drivers, live volumes or kernel entry points. */
#include <windows.h>
#include <winternl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATUS_SUCCESS ((NTSTATUS)0)
#define STATUS_UNSUCCESSFUL ((NTSTATUS)0xc0000001)
#define STATUS_IO_DEVICE_ERROR ((NTSTATUS)0xc0000185)
#define STATUS_INSUFFICIENT_RESOURCES ((NTSTATUS)0xc000009a)
#define STATUS_INVALID_DEVICE_REQUEST ((NTSTATUS)0xc0000010)
#define STATUS_MEDIA_WRITE_PROTECTED ((NTSTATUS)0xc00000a2)
#define STATUS_END_OF_FILE ((NTSTATUS)0xc0000011)
#define STATUS_DEVICE_BUSY ((NTSTATUS)0x80000011)
#define PAGE_SIZE 4096
#define PAGE_SHIFT 12
#define VACB_MAPPING_GRANULARITY (64 * PAGE_SIZE)
#define FlagOn(v,f) ((v) & (f))
#define BooleanFlagOn(v,f) (!!FlagOn(v,f))
#define SetFlag(v,f) ((v) |= (f))
#define ClearFlag(v,f) ((v) &= ~(f))
#define DPRINT(...) ((void)0)
#define DPRINT1(...) ((void)0)
#define CCTRACE(...) ((void)0)
#define DebugTrace(...) ((void)0)
#define PAGED_CODE() ((void)0)
#define _SEH2_TRY __try
#define _SEH2_FINALLY __finally
#define _SEH2_EXCEPT(x) __except(x)
#define _SEH2_END
#define _SEH2_VOLATILE volatile
#define _SEH2_GetExceptionCode GetExceptionCode
#define _SEH2_AbnormalTermination AbnormalTermination
#define ASSERT(e) do { if (!(e)) { fprintf(stderr,"fixture invariant at %d: %s\n",__LINE__,#e); exit(2); } } while (0)
#define NT_ASSERT ASSERT

static int Cases, Failed;
static void verdict(const char *name, int pass)
{
    ++Cases;
    if (!pass) ++Failed;
    printf("%s: %s\n", name, pass ? "PASS" : "FAIL");
}
static int summary(const char *suite)
{
    printf("%s: %d/%d passed\n", suite, Cases-Failed, Cases);
    return Failed != 0;
}
static void InitializeListHead(PLIST_ENTRY h) { h->Flink = h->Blink = h; }
static int IsListEmpty(PLIST_ENTRY h) { return h->Flink == h; }
static void InsertTailList(PLIST_ENTRY h, PLIST_ENTRY e)
{
    e->Blink = h->Blink; e->Flink = h; h->Blink->Flink = e; h->Blink = e;
}
static void RemoveEntryList(PLIST_ENTRY e)
{
    e->Flink->Blink = e->Blink; e->Blink->Flink = e->Flink;
}
