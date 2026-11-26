"""ABI adapter for the frozen metadata-writeback driver. Preserve both case bodies."""
from pathlib import Path
import sys, tempfile, subprocess, runpy

here = Path(__file__).resolve().parent
root = here.parents[4]
src = Path(sys.argv[1]).resolve()
driver = here / "metadata-writeback"
extract = runpy.run_path(str(driver / "run.py"))["functions"]
out = Path(tempfile.mkdtemp(dir=root/"OutDir",prefix="cc-handoff-contract-"))
print(out, flush=True)
pin, view = [(src/"ntoskrnl/cc"/f).read_text(encoding="utf8") for f in ("pin.c","view.c")]
names = [("CcpFreeBcb","static VOID"),("CcpFindBcb","static PINTERNAL_BCB"),
         ("CcpDereferenceBcb","static VOID"),("CcpReferenceBcb","static NTSTATUS"),
         ("CcpGetAppropriateBcb","static PVOID")]
code = extract(pin,names) + "\n" + pin[pin.index("typedef struct _CC_PAGE_OWNER"):pin.index("static\nBOOLEAN\nCcpPinData(")]
code += extract(pin,[(n,"VOID NTAPI") for n in ("CcSetDirtyPinnedData","CcUnpinDataForThread","CcUnpinData")]+
                [("CcpPinData","static BOOLEAN"),("CcPinRead","BOOLEAN NTAPI"),
                 ("CcRepinBcb","VOID NTAPI"),("CcUnpinRepinnedBcb","VOID NTAPI")])
code += "\n"+extract(view,[("CcRosFlushVacb","NTSTATUS"),("CcFlushCache","VOID NTAPI")])
(out/"production.inc").write_text(code)
t = (driver/"contract.c").read_text()
t=t.replace('"../checks.h"','"checks.h"').replace("typedef struct { Section *SectionObjectPointer;",
    "typedef struct { SRWLOCK *PagingIoResource; } *PFSRTL_COMMON_FCB_HEADER;\ntypedef struct { Section *SectionObjectPointer; PFSRTL_COMMON_FCB_HEADER FsContext;")
t=t.replace("ULONG Flags; File", "BOOLEAN BcbWriteBehindDisabled; ULONG OpenCount,ActiveBcbFlushes,Flags; File")
t=t.replace("InitializeListHead(&map.BcbList);","map.OpenCount=1; InitializeListHead(&map.BcbList);")
t=t.replace("BOOLEAN Dirty, BcbFlushActive;", "BOOLEAN Dirty; SRWLOCK MetadataIo;")
t=t.replace("Vacb *Vacb; LIST_ENTRY","ULONGLONG PendingPages; Map *SharedCacheMap; Vacb *Vacb; LIST_ENTRY")
t=t.replace("static File file = {&section};","static struct { SRWLOCK *PagingIoResource; } header;\nstatic File file = {&section,(PFSRTL_COMMON_FCB_HEADER)&header};")
t=t.replace("static __declspec(thread) int exclusive;","static __declspec(thread) SRWLOCK *owned;\nstatic __declspec(thread) int exclusive;")
t=t.replace("exclusive = ex;", "owned=r; exclusive = ex;")
t=t.replace("(exclusive ? ReleaseSRWLockExclusive", "(owned=NULL, exclusive ? ReleaseSRWLockExclusive")
t=t.replace("InterlockedExchange(&dirty,1)", "(InterlockedExchange(&dirty,1),STATUS_SUCCESS)")
t=t.replace('#include "production.inc"', '''
#define ExIsResourceAcquiredSharedLite(r) (owned==(r))
#define KeWaitForSingleObject(r,a,b,c,t) ((t) ? (TryAcquireSRWLockExclusive(r)?STATUS_SUCCESS:STATUS_TIMEOUT) : (AcquireSRWLockExclusive(r),STATUS_SUCCESS))
#define KeReleaseMutex(r,b) ReleaseSRWLockExclusive(r)
#define KeEnterCriticalRegion() ((void)0)
#define KeLeaveCriticalRegion() ((void)0)
static SRWLOCK master;
#define KeAcquireQueuedSpinLock(l) (AcquireSRWLockExclusive(&master),0)
#define KeReleaseQueuedSpinLock(l,k) ReleaseSRWLockExclusive(&master)
#define CcRosDeleteFileCache(...) (abort(),STATUS_UNSUCCESSFUL)
#define ExAllocatePoolWithTag(p,n,t) malloc(n)
#define ExFreePoolWithTag(p,t) free(p)
#define STATUS_CANT_WAIT ((NTSTATUS)0xc00000d8)
#include "production.inc"''')
(out/"contract.c").write_text(t)
subprocess.run(["cl","/nologo","/TC","/W3","/MT","/Od",f"/I{here}","contract.c"],cwd=out,check=True)
for case in ("pin","retry"):
    subprocess.run([out/"contract.exe",case],cwd=out,timeout=30,check=True)
