"""Run in an MSVC prompt. Characterize partial FAT mirror writes on the pinned code; no disk I/O in the test."""
from pathlib import Path
import re
import subprocess
import tempfile

here = Path(__file__).resolve().parent
src = here.parents[2]/"Upstream/ReactOS/Source"
pin = "4fbbefd614b218afd3f0cbf09aa32998ea3ad7a6"


def read(name):
    return subprocess.check_output(["git", "-C", src, "show", f"{pin}:drivers/filesystems/fastfat/{name}"]).decode()


dev = read("deviosup.c")
callback, = re.findall(r"^FatMultiSyncCompletionRoutine\s*\([^;{}]*?\)\s*/\*.*?\*/\s*\{.*?^\}", dev, re.M|re.S)
write = read("write.c")
# Keep the real initial IOSB, completion routine, post-wait status check and
# DirtyFatMcb removal decision. Only generation/dispatch of two child IRPs is simulated.
start = write.index("        RaiseIosb.Status = STATUS_SUCCESS;", write.index("if (TypeOfOpen == VirtualVolumeFile) {"))
end = write.index("\n", write.index("RaiseIosb.Information = ByteCount;", start))
tail = write.index("            FatWaitSync( IrpContext );", end)
stop = write.index("        return Status;", tail) + len("        return Status;")
stub = r"""
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
typedef int32_t NTSTATUS;
typedef struct {NTSTATUS Status; uintptr_t Information;} IO_STATUS_BLOCK;
typedef struct {IO_STATUS_BLOCK IoStatus; void *MdlAddress;} IRP,*PIRP;
typedef struct {PIRP MasterIrp; int IrpCount; struct {int SyncEvent;} Wait;} FatCtx,*PFAT_IO_CONTEXT;
typedef void *PDEVICE_OBJECT,*PVOID;
#undef _In_
#undef _In_reads_opt_
#define _In_
#define _In_reads_opt_(x)
#define NT_ASSERT assert
#define NT_SUCCESS(x) ((x)>=0)
#define STATUS_SUCCESS 0
#define STATUS_MORE_PROCESSING_REQUIRED ((NTSTATUS)0xc0000016)
#define FAIL_IO ((NTSTATUS)0xc0000185)
#define DebugTrace(...)
#define IoFreeIrp(x) ((void)(x))
#define IoFreeMdl(x) ((void)(x))
#define InterlockedDecrement(p) (--*(p))
#define KeSetEvent(p,...) (++*(p))
#define UNREFERENCED_PARAMETER(x) ((void)(x))
/* The zero-on-read MDL is absent for this write. */
#define FatDoCompletionZero(i,c) ((void)0)
#define FatWaitSync(x) assert(ctx.IrpCount==0 && ctx.Wait.SyncEvent==1)
#define FatNormalizeAndRaiseStatus(c,s) return (s)
#define FatRemoveMcbEntry(v,m,o,n) (*removed=(n))
#define FatCompleteRequest(c,i,s) ((void)0)
static NTSTATUS
"""
setup = r"""
static NTSTATUS run(NTSTATUS st, unsigned received, int first, unsigned *removed, PIRP Irp) {
    const unsigned ByteCount=4096,WriteLength=4096;
    IO_STATUS_BLOCK RaiseIosb; NTSTATUS Status;
    FatCtx ctx={Irp,2,{0}};
    IRP child[2]={{{0,4096},0},{{st,received},0}};
"""
middle = r"""
    {
        for (int i=0;i<2;i++)
            assert(FatMultiSyncCompletionRoutine(0,&child[i^first],&ctx)==STATUS_MORE_PROCESSING_REQUIRED);
"""
checks = r"""
}
int main(void) {
    for (int variant=0;variant<3;variant++) for (int first=0;first<=1;first++) {
        NTSTATUS status=variant==2?FAIL_IO:0;
        unsigned n=variant==0?4096:512,removed=0;
        IRP master={{0,0},0};
        NTSTATUS s=run(status,n,first,&removed,&master);
        printf("mirror2 status=%08x bytes=%u/4096 first-mirror=%d -> parent=%08x info=%zu dirty-bytes-removed=%u\n",
          (unsigned)status,n,1+first,(unsigned)s,(size_t)master.IoStatus.Information,removed);
        if(!variant)assert(s==0 && removed==4096 && master.IoStatus.Information==4096);
        if(variant==2)assert(s==FAIL_IO && removed==0);
    }
}
"""
with tempfile.TemporaryDirectory(prefix="Kundows-mirror-") as tmp:
    d = Path(tmp)
    (d/"check.c").write_text(stub+callback+"\n"+setup+write[start:end]+middle+write[tail:stop]+"\n"+checks,
                             encoding="utf-8")
    subprocess.run(["cl", "/nologo", "/Od", "/MT", "/TC", "check.c"], cwd=d, check=True)
    subprocess.run([d/"check.exe"], cwd=d, check=True, timeout=15)
