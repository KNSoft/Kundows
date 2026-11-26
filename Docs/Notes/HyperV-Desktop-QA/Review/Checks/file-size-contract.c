/* Observable size/close contract. Run on a new regular file on the selected
 * volume. Uses no driver and never modifies an existing file. */
#include <windows.h>
#include <winternl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef NTSTATUS (NTAPI *SetInfo)(HANDLE, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS);
typedef NTSTATUS (NTAPI *QueryInfo)(HANDLE, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS);
typedef struct {
    LARGE_INTEGER AllocationSize, EndOfFile;
    ULONG NumberOfLinks;
    BOOLEAN DeletePending, Directory;
} Standard;

static QueryInfo query;
static SetInfo set;
static int snapshot(HANDLE f, const char *stage, Standard *s)
{
    IO_STATUS_BLOCK io = {0};
    NTSTATUS st = query(f, &io, s, sizeof(*s), (FILE_INFORMATION_CLASS)5);
    if (st < 0) return 0;
    printf("%s.alloc=%lld\n%s.eof=%lld\n", stage, s->AllocationSize.QuadPart, stage, s->EndOfFile.QuadPart);
    return 1;
}
static NTSTATUS eof(HANDLE f, const char *stage, LONGLONG bytes)
{
    IO_STATUS_BLOCK io = {0};
    LARGE_INTEGER n;
    NTSTATUS st;
    n.QuadPart = bytes;
    st = set(f, &io, &n, sizeof(n), (FILE_INFORMATION_CLASS)20);
    printf("%s.ntstatus=%08lx\n", stage, (ULONG)st);
    return st;
}
static int read_prefix(HANDLE f, BYTE *buf, DWORD n)
{
    LARGE_INTEGER z = {0};
    DWORD got;
    return SetFilePointerEx(f, z, NULL, FILE_BEGIN) && ReadFile(f, buf, n, &got, NULL) && got == n;
}
#define CHECK(c) do { if (!(c)) { printf("failed.line=%d\nfailed.win32=%lu\n",__LINE__,GetLastError()); goto done; } } while (0)

int wmain(int argc, wchar_t **argv)
{
    HANDLE a=INVALID_HANDLE_VALUE, b=INVALID_HANDLE_VALUE, mapping=NULL;
    void *view=NULL;
    DWORD sec, bytes, unused1, unused2, cluster, original, cut, grown, i, nw;
    wchar_t volume[MAX_PATH], fs[40];
    BYTE *pattern=NULL, *readback=NULL;
    Standard initial, shortFile, refused, finalSize;
    NTSTATUS st;
    int rc=1, created=0;
    if (argc!=2) { fputs("Usage: file-size-contract <absolute path to new file>\n",stderr); return 2; }
    query=(QueryInfo)(void *)GetProcAddress(GetModuleHandleW(L"ntdll.dll"),"NtQueryInformationFile");
    set=(SetInfo)(void *)GetProcAddress(GetModuleHandleW(L"ntdll.dll"),"NtSetInformationFile");
    CHECK(query && set && GetVolumePathNameW(argv[1],volume,MAX_PATH));
    CHECK(GetVolumeInformationW(volume,NULL,0,NULL,NULL,NULL,fs,40));
    CHECK(GetDiskFreeSpaceW(volume,&sec,&bytes,&unused1,&unused2));
    /* Two EOFs in the same allocation unit; verify actual allocation below. */
    CHECK(sec && bytes && sec <= (1U<<20)/bytes);
    cluster=sec*bytes;
    CHECK(cluster>=512);
    original=cluster+cluster/2; cut=cluster+123; grown=original+2*cluster;
    printf("volume=%ls\ncluster=%lu\ncut=%lu\n",fs,cluster,cut);
    pattern=(BYTE *)malloc(original); readback=(BYTE *)malloc(grown);
    CHECK(pattern && readback);
    for(i=0;i<original;++i) pattern[i]=(BYTE)(1+i%251);
    a=CreateFileW(argv[1],GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
                  NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
    CHECK(a!=INVALID_HANDLE_VALUE); created=1;
    b=CreateFileW(argv[1],GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
                  NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    CHECK(b!=INVALID_HANDLE_VALUE && WriteFile(a,pattern,original,&nw,NULL) && nw==original && FlushFileBuffers(a));
    CHECK(snapshot(b,"initial",&initial));
    CHECK(eof(a,"shorten",cut)>=0 && snapshot(b,"peer_after_shorten",&shortFile));
    CHECK(shortFile.EndOfFile.QuadPart==cut && read_prefix(b,readback,cut) && !memcmp(pattern,readback,cut));
    printf("allocation_unchanged=%d\nprefix_after_shorten=1\n",initial.AllocationSize.QuadPart==shortFile.AllocationSize.QuadPart);
    mapping=CreateFileMappingW(b,NULL,PAGE_READONLY,0,0,NULL); CHECK(mapping);
    view=MapViewOfFile(mapping,FILE_MAP_READ,0,0,0); CHECK(view);
    /* Request via the FSD, which decides whether to accept it. Do not touch
     * the mapping following the size request. */
    st=eof(a,"shorten_mapped",cluster/2);
    CHECK(snapshot(b,"after_mapped_request",&refused));
    CHECK(UnmapViewOfFile(view)); view=NULL;
    CHECK(CloseHandle(mapping)); mapping=NULL;
    CHECK(st<0 && refused.EndOfFile.QuadPart==cut);
    CHECK(eof(a,"direct_growth_after_unmap",grown)>=0 && snapshot(b,"after_growth",&finalSize));
    CHECK(finalSize.EndOfFile.QuadPart==grown && read_prefix(b,readback,grown) && !memcmp(pattern,readback,cut));
    for(i=cut;i<grown && !readback[i];++i) {}
    printf("growth_suffix_all_zero=%d\n",i==grown);
    CHECK(CloseHandle(a)); a=INVALID_HANDLE_VALUE;
    CHECK(FlushFileBuffers(b) && read_prefix(b,readback,cut) && !memcmp(pattern,readback,cut));
    puts("peer_usable_after_first_close=1");
    CHECK(eof(b,"repeat_shorten",cut)>=0 && FlushFileBuffers(b));
    CHECK(CloseHandle(b)); b=INVALID_HANDLE_VALUE;
    a=CreateFileW(argv[1],GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    CHECK(a!=INVALID_HANDLE_VALUE && snapshot(a,"reopened",&finalSize));
    CHECK(finalSize.EndOfFile.QuadPart==cut && read_prefix(a,readback,cut) && !memcmp(pattern,readback,cut));
    puts("reopened_prefix_exact=1"); rc=0;
done:
    if(view) UnmapViewOfFile(view);
    if(mapping) CloseHandle(mapping);
    if(b!=INVALID_HANDLE_VALUE) CloseHandle(b);
    if(a!=INVALID_HANDLE_VALUE) CloseHandle(a);
    if(created && !DeleteFileW(argv[1])) rc=2;
    free(pattern); free(readback); printf("exit=%d\n",rc); return rc;
}
