#include "checks.h"
#define TRACE(...) ((void)0)
#define USER_VERSION 7
#define KeBugCheckEx(...) abort()
static int lockDepth, gpepCSRSS, powerCalls, greCalls, userCalls;
static NTSTATUS powerStatus, userStatus;
static BOOLEAN greOK;
static void UserEnterExclusive(void) { ASSERT(!lockDepth); ++lockDepth; }
static void UserLeave(void) { ASSERT(lockDepth == 1); --lockDepth; }
static void InitCsrProcess(void) { ASSERT(lockDepth); ++gpepCSRSS; }
static NTSTATUS IntInitWin32PowerManagement(HANDLE e) { ASSERT(lockDepth); ++powerCalls; return powerStatus; }
static BOOLEAN InitializeGreCSRSS(void) { ASSERT(lockDepth); ++greCalls; return greOK; }
static NTSTATUS UserInitialize(void) { ASSERT(lockDepth); ++userCalls; return userStatus; }
#include "user.inc"

static void run(const char *name, int already, NTSTATUS power, int gre, NTSTATUS user,
                NTSTATUS expected, int p, int g, int u)
{
    gpepCSRSS = already; lockDepth = powerCalls = greCalls = userCalls = 0;
    greOK = gre; powerStatus = power; userStatus = user;
    NTSTATUS s = NtUserInitialize(USER_VERSION, NULL, NULL);
    verdict(name, s == expected && !lockDepth && gpepCSRSS == 1 &&
                  p == powerCalls && g == greCalls && u == userCalls);
}
int main(void)
{
    run("already registered",1,0,1,0,STATUS_UNSUCCESSFUL,0,0,0);
    run("power failure",0,STATUS_IO_DEVICE_ERROR,1,0,STATUS_IO_DEVICE_ERROR,1,0,0);
    run("GDI failure",0,0,0,0,STATUS_UNSUCCESSFUL,1,1,0);
    run("USER failure",0,0,1,STATUS_INSUFFICIENT_RESOURCES,STATUS_INSUFFICIENT_RESOURCES,1,1,1);
    run("initialized",0,0,1,0,0,1,1,1);
    return summary("G3");
}
