/* Must only be run inside the disposable test VM. Never loads on the host. */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include "cccycle.h"

int main(void)
{
    static const char *Names[] = {"", "last-unpin", "last-repin", "write-error-retry",
        "flush-before-unpin", "redirty-in-flight", "lazy-write", "purge-active", "truncate",
        "failed-purge-preserves", "overlapping-flush"};
    SC_HANDLE Sc = NULL, Service = NULL;
    HANDLE Device = INVALID_HANDLE_VALUE;
    KW_CC_REPORT R;
    DWORD I, J, Got, Errors = 0, Failed = 0, Total = 0;
    WCHAR Driver[MAX_PATH];
    SERVICE_STATUS State;
    if (!GetFullPathNameW(L"C:\\CCCYCLE.SYS", MAX_PATH, Driver, NULL)) return 2;
    Sc = OpenSCManagerW(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (!Sc) { printf("SCM error=%lu\n", GetLastError()); return 2; }
    Service = CreateServiceW(Sc, L"KwCcCycle", L"Isolated CC lifecycle test",
        SERVICE_START | SERVICE_STOP | DELETE, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START,
        SERVICE_ERROR_NORMAL, Driver, NULL, NULL, NULL, NULL, NULL);
    if (!Service) { printf("create error=%lu\n", GetLastError()); Errors = 1; goto done; }
    if (!StartServiceW(Service, 0, NULL)) {
        printf("start error=%lu\n", GetLastError()); Errors = 1; goto done;
    }
    Device = CreateFileW(L"\\\\.\\KwCcCycle", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (Device == INVALID_HANDLE_VALUE) {
        printf("open error=%lu\n", GetLastError()); Errors = 1; goto done;
    }
    puts("case\tname\tcheck\tactual\texpected\tresult");
    fflush(stdout);
    for (I = 1; I <= KW_CC_CASES; ++I) {
        printf("BEGIN %lu %s\n", I, Names[I]); fflush(stdout);
        ZeroMemory(&R, sizeof(R));
        Got = 0;
        if (!DeviceIoControl(Device, KW_CC_IOCTL, &I, sizeof(I), &R, sizeof(R), &Got, NULL) ||
            Got != sizeof(R) || R.Version != KW_CC_VERSION || R.CaseId != I || R.Count > KW_CC_MAX_CHECKS) {
            printf("protocol error case=%lu win32=%lu bytes=%lu\n", I, GetLastError(), Got);
            ++Errors; break;
        }
        for (J = 0; J < R.Count; ++J) {
            KW_CC_CHECK *C = &R.Checks[J];
            int Pass = C->Actual == C->Expected;
            printf("%lu\t%s\t%lu\t%08lx\t%08lx\t%s\n", I, Names[I], C->Id,
                   C->Actual, C->Expected, Pass ? "PASS" : "FAIL");
            ++Total; Failed += !Pass;
        }
        printf("END %lu status=%08lx reads=%lu writes=%lu errors=%lu lazy=%lu\n",
               I, R.Status, R.Reads, R.Writes, R.FailedWrites, R.LazyWrites);
        fflush(stdout);
        if (R.Status) { ++Errors; break; }
    }
done:
    if (Device != INVALID_HANDLE_VALUE) CloseHandle(Device);
    if (Service) {
        /* A failed/abandoned kernel case intentionally disables unload. */
        if (!Errors) ControlService(Service, SERVICE_CONTROL_STOP, &State);
        DeleteService(Service);
        CloseServiceHandle(Service);
    }
    if (Sc) CloseServiceHandle(Sc);
    printf("SUMMARY checks=%lu failed=%lu execution_errors=%lu\n", Total, Failed, Errors);
    return Errors ? 2 : Failed ? 1 : 0;
}
