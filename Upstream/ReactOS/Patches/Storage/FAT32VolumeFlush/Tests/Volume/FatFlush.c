/*
 * Black-box FAT32 volume flush test. Run elevated: fatflush.exe X:
 * X: must be the marked disposable 512 MiB MBR VHD described in README.md.
 * Use normal file allocation. The physical disk handle is read-only.
 * Exit 0: pass; 1: stale FAT after successful flush; 2: API/test error;
 * 77: fixture does not meet the preconditions. No test driver required.
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winioctl.h>
#include <shellapi.h>

#define MB (1024UL * 1024)
#define ENTRY 69632UL
#define ITEMS 128
#define SHARE (FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE)
static const char marker[] = "Kundows FAT32 volume-flush test v1\r\n";
static char line[640];
static void print(const char *s)
{
    DWORD n;
    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), s, lstrlenA(s), &n, NULL);
    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), "\r\n", 2, &n, NULL);
}
static int api(const char *s)
{
    DWORD e = GetLastError();
    wsprintfA(line, "ERROR %s win32=%lu", s, e); print(line);
    return 2;
}
static int skip(const char *s)
{
    wsprintfA(line, "SKIP %s", s); print(line); return 77;
}
static DWORD word(const BYTE *p) { return p[0] | ((DWORD)p[1] << 8); }
static DWORD dword(const BYTE *p) { return word(p) | (word(p+2) << 16); }
static BOOL sector(HANDLE disk, DWORD offset, BYTE *out)
{
    LARGE_INTEGER pos; DWORD got;
    pos.QuadPart = offset;
    if (!SetFilePointerEx(disk, pos, NULL, FILE_BEGIN) || !ReadFile(disk, out, 512, &got, NULL))
        return FALSE;
    if (got == 512) return TRUE;
    SetLastError(ERROR_HANDLE_EOF); return FALSE;
}
static BOOL isMarker(LPCWSTR path)
{
    BYTE buf[sizeof(marker)]; DWORD got, i;
    HANDLE f = CreateFileW(path, GENERIC_READ, SHARE, NULL, OPEN_EXISTING, 0, NULL);
    if (f == INVALID_HANDLE_VALUE) return FALSE;
    BOOL ok = ReadFile(f, buf, sizeof(buf), &got, NULL) && got == sizeof(marker)-1;
    CloseHandle(f);
    for (i=0; ok && i<got; ++i) ok = buf[i] == (BYTE)marker[i];
    return ok;
}
static int run(WCHAR letter)
{
    WCHAR root[] = L"X:\\", vol[] = L"\\\\.\\X:", id[] = L"X:\\KWC2.ID", file[] = L"X:\\KWC2.TMP";
    WCHAR windows[MAX_PATH], label[48], fs[24], diskName[50];
    STORAGE_DEVICE_NUMBER dev; PARTITION_INFORMATION part; GET_LENGTH_INFORMATION size;
    STARTING_VCN_INPUT_BUFFER vcn = {0}; LARGE_INTEGER length;
    static union { RETRIEVAL_POINTERS_BUFFER first; BYTE bytes[16384]; } ext;
    RETRIEVAL_POINTERS_BUFFER *rp = &ext.first;
    DWORD got, i, j, fat, offsets[2], first = 0, last = 0;
    LONGLONG startVcn;
    HANDLE volume = INVALID_HANDLE_VALUE, disk = INVALID_HANDLE_VALUE, f = INVALID_HANDLE_VALUE;
    BYTE *pages = NULL;
    BOOL created = FALSE;
    int result = 2, postGood = 0;

    root[0] = vol[4] = id[0] = file[0] = letter;
    if (!GetWindowsDirectoryW(windows, MAX_PATH)) return api("GetWindowsDirectory");
    if (windows[1] == ':' && (windows[0] & ~0x20) == letter) return skip("system drive");
    if (!GetVolumeInformationW(root, label, 48, NULL, NULL, NULL, fs, 24)) return api("volume information");
    if (lstrcmpiW(label, L"KWFATQA") || lstrcmpiW(fs, L"FAT32") || !isMarker(id))
        return skip("expected KWFATQA FAT32 test volume and marker");
    volume = CreateFileW(vol, GENERIC_READ | GENERIC_WRITE, SHARE, NULL, OPEN_EXISTING, 0, NULL);
    if (volume == INVALID_HANDLE_VALUE) return api("open volume for flush (elevation required)");
#define ERROR_OUT(s) do { result = api(s); goto done; } while (0)
#define SKIP_OUT(s) do { result = skip(s); goto done; } while (0)
    if (!DeviceIoControl(volume, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &dev, sizeof(dev), &got, NULL))
        ERROR_OUT("storage device number");
    if (got != sizeof(dev) || dev.DeviceType != FILE_DEVICE_DISK || dev.PartitionNumber != 1)
        SKIP_OUT("not disk partition 1");
    if (!DeviceIoControl(volume, IOCTL_DISK_GET_PARTITION_INFO, NULL, 0, &part, sizeof(part), &got, NULL))
        ERROR_OUT("partition info");
    if (got != sizeof(part) || part.StartingOffset.QuadPart != MB ||
        part.PartitionLength.QuadPart < 500*MB || part.PartitionLength.QuadPart > 511*MB)
        SKIP_OUT("unexpected scratch partition size/offset");
    wsprintfW(diskName, L"\\\\.\\PhysicalDrive%lu", dev.DeviceNumber);
    disk = CreateFileW(diskName, GENERIC_READ, SHARE, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
    if (disk == INVALID_HANDLE_VALUE) ERROR_OUT("open disk for read");
    if (!DeviceIoControl(disk, IOCTL_DISK_GET_LENGTH_INFO, NULL, 0, &size, sizeof(size), &got, NULL))
        ERROR_OUT("disk size");
    if (got != sizeof(size) || size.Length.QuadPart != 512*MB) SKIP_OUT("expected 512 MiB fixture");
    pages = (BYTE *)VirtualAlloc(NULL, 6*4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!pages) ERROR_OUT("VirtualAlloc");
    if (!sector(disk, MB, pages)) ERROR_OUT("boot sector read");
    fat = dword(pages+36);
    if (word(pages+510)!=0xaa55 || word(pages+11)!=512 || pages[13]!=8 || pages[16]!=2 ||
        !word(pages+14) || word(pages+17) || word(pages+22) || (word(pages+40)&0x80) ||
        fat < (ENTRY+ITEMS)/128 || fat > 8192)
        SKIP_OUT("expected mirrored FAT32, 512-byte sectors, 4 KiB clusters");
    offsets[0] = MB + word(pages+14)*512 + ENTRY*4;
    offsets[1] = offsets[0] + fat*512;
    for (i=0; i<2; ++i) {
        if (!sector(disk, offsets[i], pages+i*4096)) ERROR_OUT("initial FAT read");
        for (j=0; j<ITEMS; ++j) if (dword(pages+i*4096+j*4)&0x0fffffff) SKIP_OUT("target entries not free");
    }
    /* Open once. Opening another FAT volume handle during the measurement
       could itself cause a filesystem flush. No fixture logging in the window. */
    f = CreateFileW(file, GENERIC_READ | GENERIC_WRITE, SHARE, NULL, CREATE_NEW, 0, NULL);
    if (f == INVALID_HANDLE_VALUE) ERROR_OUT("create fresh test file");
    created = TRUE; length.QuadPart = 288*MB;
    if (!SetFilePointerEx(f, length, NULL, FILE_BEGIN) || !SetEndOfFile(f)) ERROR_OUT("set EOF");
    if (!DeviceIoControl(f, FSCTL_GET_RETRIEVAL_POINTERS, &vcn, sizeof(vcn), rp, sizeof(ext), &got, NULL))
        ERROR_OUT("query allocated extents");
    if (got < (DWORD)FIELD_OFFSET(RETRIEVAL_POINTERS_BUFFER, Extents) ||
        rp->ExtentCount > (got-FIELD_OFFSET(RETRIEVAL_POINTERS_BUFFER, Extents))/sizeof(rp->Extents[0]))
        SKIP_OUT("incomplete extents");
    startVcn = rp->StartingVcn.QuadPart;
    for (i=0; i<rp->ExtentCount; ++i) {
        LONGLONG a = rp->Extents[i].Lcn.QuadPart, next = rp->Extents[i].NextVcn.QuadPart;
        if (a<0 || a>131072 || startVcn<0 || next<=startVcn || next>131072) SKIP_OUT("invalid extent");
        a += 2; /* FAT LCN zero is data cluster 2 */
        if (a<=ENTRY && ENTRY+ITEMS < a+next-startVcn) {
            first=(DWORD)a; last=(DWORD)(a+next-startVcn-1); break;
        }
        startVcn=next;
    }
    if (!first) SKIP_OUT("allocation did not cover selected full sector");
    for (i=0;i<2;++i) if (!sector(disk,offsets[i],pages+(2+i)*4096)) ERROR_OUT("pre-flush disk read");
    if (!FlushFileBuffers(volume)) ERROR_OUT("FlushFileBuffers(volume)");
    for (i=0;i<2;++i) if (!sector(disk,offsets[i],pages+(4+i)*4096)) ERROR_OUT("post-flush disk read");
    wsprintfA(line, "extent=%lu..%lu entry=%lu offsets=%lu,%lu flush=success", first,last,ENTRY,offsets[0],offsets[1]);
    print(line);
    for (i=0;i<6;++i) {
        DWORD correct=0, zeros=0, value;
        for (j=0;j<ITEMS;++j) {
            value=dword(pages+i*4096+j*4)&0x0fffffff;
            correct += value==ENTRY+j+1; zeros += value==0;
        }
        wsprintfA(line,"%s FAT%lu bytes=512 matching=%lu/128 zero=%lu",
                  i<2?"initial":i<4?"before":"after", i%2+1, correct, zeros); print(line);
        if (i>=4 && correct==ITEMS) ++postGood;
    }
    result=postGood==2?0:1;
    print(result ? "FAIL successful flush returned stale FAT entries" : "PASS both FAT sectors contain the allocated chain");
done:
    if (f!=INVALID_HANDLE_VALUE) CloseHandle(f);
    if (created && !DeleteFileW(file)) { api("remove test file"); result=2; }
    if (pages) VirtualFree(pages,0,MEM_RELEASE);
    if (disk!=INVALID_HANDLE_VALUE) CloseHandle(disk);
    CloseHandle(volume); return result;
}

void WINAPI entry(void)
{
    int n, result = 2;
    LPWSTR *args = CommandLineToArgvW(GetCommandLineW(),&n);
    if (args && n==2 && lstrlenW(args[1])==2 && args[1][1]==':') {
        WCHAR c=(WCHAR)(args[1][0]&~0x20);
        if (c>='A' && c<='Z') result=run(c);
    } else print("usage: fatflush.exe X:");
    if (args) LocalFree(args);
    ExitProcess(result);
}
