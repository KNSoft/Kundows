/*
 * Disposable FAT32 fixture only: cached file data, real volume lock/dismount,
 * new-path remount, and full byte-for-byte readback. Physical disk is read-only.
 * Reuse the existing flush test's marker/helpers without changing that EXE.
 */
#define entry FatFlushEntryUnused
#include "fatflush.c"
#undef entry

#define CYCLE_BYTES (8*MB)
#define BLOCK_BYTES (64*1024UL)

static BOOL ctl(HANDLE volume, DWORD code)
{
    DWORD got;
    return DeviceIoControl(volume, code, NULL, 0, NULL, 0, &got, NULL);
}

static BYTE pattern(DWORD round, DWORD block, DWORD index)
{
    return (BYTE)(round*29 + block*7 + index + (index>>8));
}

static int cycleRun(WCHAR letter)
{
    WCHAR root[] = L"X:\\", vol[] = L"\\\\.\\X:", id[] = L"X:\\KWC2.ID";
    WCHAR file[] = L"X:\\KWCYCLE.BIN";
    WCHAR windows[MAX_PATH], label[48], fs[24], physical[50];
    STORAGE_DEVICE_NUMBER dev, again;
    PARTITION_INFORMATION part;
    GET_LENGTH_INFORMATION size;
    HANDLE volume=INVALID_HANDLE_VALUE, disk=INVALID_HANDLE_VALUE, f=INVALID_HANDLE_VALUE;
    BYTE *buffer=NULL;
    DWORD got, serial, mountedSerial, round, block, i, error;
    BOOL created=FALSE;
    int result=2;
    root[0]=vol[4]=id[0]=file[0]=letter;
    if (!GetWindowsDirectoryW(windows, MAX_PATH)) return api("GetWindowsDirectory");
    if (windows[1]==':' && (windows[0]&~0x20)==letter) return skip("system drive");
    if (!GetVolumeInformationW(root,label,48,&serial,NULL,NULL,fs,24))
        return api("volume information");
    if (lstrcmpiW(label,L"KWFATQA") || lstrcmpiW(fs,L"FAT32") || !isMarker(id))
        return skip("expected marked KWFATQA test volume");
    volume=CreateFileW(vol,GENERIC_READ|GENERIC_WRITE,SHARE,NULL,OPEN_EXISTING,0,NULL);
    if (volume==INVALID_HANDLE_VALUE) return api("open volume");
    if (!DeviceIoControl(volume,IOCTL_STORAGE_GET_DEVICE_NUMBER,NULL,0,&dev,sizeof(dev),&got,NULL))
        ERROR_OUT("device identity");
    if (got!=sizeof(dev) || dev.DeviceType!=FILE_DEVICE_DISK || dev.PartitionNumber!=1)
        SKIP_OUT("not test partition 1");
    if (!DeviceIoControl(volume,IOCTL_DISK_GET_PARTITION_INFO,NULL,0,&part,sizeof(part),&got,NULL))
        ERROR_OUT("partition identity");
    if (got!=sizeof(part) || part.StartingOffset.QuadPart!=MB ||
        part.PartitionLength.QuadPart<500*MB || part.PartitionLength.QuadPart>511*MB)
        SKIP_OUT("unexpected test partition");
    wsprintfW(physical,L"\\\\.\\PhysicalDrive%lu",dev.DeviceNumber);
    disk=CreateFileW(physical,GENERIC_READ,SHARE,NULL,OPEN_EXISTING,FILE_FLAG_NO_BUFFERING,NULL);
    if (disk==INVALID_HANDLE_VALUE) ERROR_OUT("open read-only disk");
    if (!DeviceIoControl(disk,IOCTL_DISK_GET_LENGTH_INFO,NULL,0,&size,sizeof(size),&got,NULL))
        ERROR_OUT("disk length");
    if (got!=sizeof(size) || size.Length.QuadPart!=512*MB)
        SKIP_OUT("expected 512 MiB test VHD");
    buffer=(BYTE*)VirtualAlloc(NULL,BLOCK_BYTES,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
    if (!buffer) ERROR_OUT("VirtualAlloc");
    if (!sector(disk,MB,buffer)) ERROR_OUT("read-only boot sector check");
    if (word(buffer+510)!=0xaa55 || word(buffer+11)!=512 || buffer[13]!=8 || buffer[16]!=2 ||
        !word(buffer+14) || word(buffer+17) || word(buffer+22) || (word(buffer+40)&0x80))
        SKIP_OUT("expected mirrored FAT32 geometry");
    CloseHandle(disk); disk=INVALID_HANDLE_VALUE;

    for (round=1;round<=3;++round) {
        f=CreateFileW(file,GENERIC_READ|GENERIC_WRITE,SHARE,NULL,
                      round==1?CREATE_NEW:OPEN_EXISTING,0,NULL);
        if (f==INVALID_HANDLE_VALUE) ERROR_OUT("open owned cycle file");
        created=TRUE;
        for (block=0;block<CYCLE_BYTES/BLOCK_BYTES;++block) {
            for (i=0;i<BLOCK_BYTES;++i) buffer[i]=pattern(round,block,i);
            if (!WriteFile(f,buffer,BLOCK_BYTES,&got,NULL) || got!=BLOCK_BYTES)
                ERROR_OUT("write full pattern block");
        }
        if (!SetEndOfFile(f) || !FlushFileBuffers(f) || !FlushFileBuffers(volume))
            ERROR_OUT("flush file and volume");

        /* A failed lock must be caused by the still-open file, not an
         * unsupported FSCTL. Do not proceed while a surprising lock is held. */
        if (ctl(volume,FSCTL_LOCK_VOLUME)) {
            print("FAIL volume locked while test file remained open");
            result=1; goto done;
        }
        error=GetLastError();
        if (error!=ERROR_ACCESS_DENIED && error!=ERROR_SHARING_VIOLATION && error!=ERROR_LOCK_VIOLATION)
            ERROR_OUT("lock rejection was not an open-file conflict");
        wsprintfA(line,"cycle=%lu open-file-lock=rejected win32=%lu",round,error); print(line);
        if (!CloseHandle(f)) ERROR_OUT("close before lock");
        f=INVALID_HANDLE_VALUE;
        if (!ctl(volume,FSCTL_LOCK_VOLUME)) ERROR_OUT("lock closed test volume");
        if (!ctl(volume,FSCTL_DISMOUNT_VOLUME)) ERROR_OUT("dismount locked test volume");
        if (ctl(volume,FSCTL_IS_VOLUME_MOUNTED)) {
            print("FAIL old volume handle still reports mounted");
            result=1; goto done;
        }
        error=GetLastError();
        if (error==ERROR_INVALID_FUNCTION || error==ERROR_NOT_SUPPORTED)
            ERROR_OUT("mounted-state query unsupported");
        wsprintfA(line,"cycle=%lu lock=success dismount=success mounted-after=0 win32=%lu",round,error);
        print(line);
        /* Closing the lock owner releases the lock; new path opens must
         * mount afresh, rather than continuing through the old volume handle. */
        if (!CloseHandle(volume)) ERROR_OUT("close dismounted volume");
        volume=INVALID_HANDLE_VALUE;
        if (!GetVolumeInformationW(root,label,48,&mountedSerial,NULL,NULL,fs,24))
            ERROR_OUT("remount through volume path");
        if (mountedSerial!=serial || lstrcmpiW(label,L"KWFATQA") ||
            lstrcmpiW(fs,L"FAT32") || !isMarker(id)) {
            print("FAIL remount identity differs"); result=1; goto done;
        }
        f=CreateFileW(file,GENERIC_READ,SHARE,NULL,OPEN_EXISTING,0,NULL);
        if (f==INVALID_HANDLE_VALUE) ERROR_OUT("open file after remount");
        for (block=0;block<CYCLE_BYTES/BLOCK_BYTES;++block) {
            if (!ReadFile(f,buffer,BLOCK_BYTES,&got,NULL) || got!=BLOCK_BYTES)
                ERROR_OUT("read remounted full block");
            for (i=0;i<BLOCK_BYTES;++i) {
                if (buffer[i]!=pattern(round,block,i)) {
                    wsprintfA(line,"FAIL cycle=%lu byte=%lu actual=%u expected=%u",
                              round,block*BLOCK_BYTES+i,buffer[i],pattern(round,block,i));
                    print(line); result=1; goto done;
                }
            }
        }
        if (!ReadFile(f,buffer,1,&got,NULL) || got) {
            print("FAIL remounted EOF differs"); result=1; goto done;
        }
        CloseHandle(f); f=INVALID_HANDLE_VALUE;
        volume=CreateFileW(vol,GENERIC_READ|GENERIC_WRITE,SHARE,NULL,OPEN_EXISTING,0,NULL);
        if (volume==INVALID_HANDLE_VALUE) ERROR_OUT("reopen mounted volume");
        if (!DeviceIoControl(volume,IOCTL_STORAGE_GET_DEVICE_NUMBER,NULL,0,&again,sizeof(again),&got,NULL))
            ERROR_OUT("remounted device identity");
        if (got!=sizeof(again) || again.DeviceType!=dev.DeviceType ||
            again.DeviceNumber!=dev.DeviceNumber || again.PartitionNumber!=dev.PartitionNumber) {
            print("FAIL remounted device differs"); result=1; goto done;
        }
        wsprintfA(line,"cycle=%lu remount=success verified-bytes=%lu eof=match identity=match",
                  round,CYCLE_BYTES); print(line);
    }
    result=0;
    print("PASS three real lock/dismount/remount cycles, 24 MiB byte-for-byte verified");
done:
    if (f!=INVALID_HANDLE_VALUE) CloseHandle(f);
    if (volume!=INVALID_HANDLE_VALUE) CloseHandle(volume);
    if (disk!=INVALID_HANDLE_VALUE) CloseHandle(disk);
    if (created && !DeleteFileW(file)) { api("remove owned cycle file"); result=2; }
    if (buffer) VirtualFree(buffer,0,MEM_RELEASE);
    return result;
}

void WINAPI entry(void)
{
    int n, result=2;
    LPWSTR *args=CommandLineToArgvW(GetCommandLineW(),&n);
    if (args && n==2 && lstrlenW(args[1])==2 && args[1][1]==':') {
        WCHAR c=(WCHAR)(args[1][0]&~0x20);
        if (c>='A' && c<='Z') result=cycleRun(c);
    } else print("usage: fatcycle.exe X: (marked disposable test volume only)");
    if (args) LocalFree(args);
    ExitProcess(result);
}
