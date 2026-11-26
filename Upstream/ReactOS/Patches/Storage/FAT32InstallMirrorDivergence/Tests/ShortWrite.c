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
#include "MirrorCompletion.inc"

static NTSTATUS run(NTSTATUS st, unsigned received, int first, unsigned *removed, PIRP Irp) {
    const unsigned ByteCount=4096,WriteLength=4096;
    IO_STATUS_BLOCK RaiseIosb; NTSTATUS Status;
    FatCtx ctx={Irp,2,{0}};
    IRP child[2]={{{0,4096},0},{{st,received},0}};
#include "MirrorStatus.inc"

    {
        for (int i=0;i<2;i++)
            assert(FatMultiSyncCompletionRoutine(0,&child[i^first],&ctx)==STATUS_MORE_PROCESSING_REQUIRED);
#include "MirrorWait.inc"

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
