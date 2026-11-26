/* Transfer a harmless token, never dereference a bad pointer or deliver an
 * APC to an expired stack. The pending device completes at the wait boundary. */
#include "Checks.h"
typedef struct { int StackSize; } Device,*PDEVICE_OBJECT;
typedef struct { ULONG Count; Device *Object; } Relations,*PDEVICE_RELATIONS;
typedef struct { int set; } KEVENT,*PKEVENT;
typedef struct { int MajorFunction,MinorFunction; struct {struct {int Type;} QueryDeviceRelations;} Parameters; } Stack,*PIO_STACK_LOCATION;
typedef struct Packet *PIRP;
typedef NTSTATUS (NTAPI *Completion)(Device*,PIRP,void*);
struct Packet {
    IO_STATUS_BLOCK IoStatus; struct {struct {void *Thread;} Overlay;} Tail;
    Stack stack; Completion callback; void *context; IO_STATUS_BLOCK *user; KEVENT *event;
    int success,error,cancel;
};
static Device device={1};
static Relations produced={1,&device},unusedToken={0};
static struct Packet pkt;
static int delayApc,oom,allocated,done,freed,waits,completionTime,threadToken;
static NTSTATUS result;
#define PsGetCurrentThread() (&threadToken)
#define IO_NO_INCREMENT 0
#define IRP_MJ_PNP 27
#define IRP_MN_QUERY_DEVICE_RELATIONS 7
#define TargetDeviceRelation 4
#define STATUS_NOT_SUPPORTED ((NTSTATUS)0xc00000bb)
#define STATUS_MORE_PROCESSING_REQUIRED ((NTSTATUS)0xc0000016)
#define STATUS_CANCELLED ((NTSTATUS)0xc0000120)
#define KeInitializeEvent(e,t,s) ((e)->set=(s))
#define KeSetEvent(e,p,w) ((e)->set=1)
#define IoGetNextIrpStackLocation(i) (&(i)->stack)
static PIRP IoAllocateIrp(int n,BOOLEAN quota) { ASSERT(n==device.StackSize && !quota); if(oom)return NULL; ++allocated; return &pkt; }
static PIRP IoBuildSynchronousFsdRequest(int major, Device *d,void *b,ULONG l,PLARGE_INTEGER o,KEVENT *e,IO_STATUS_BLOCK *io)
{
    PIRP i=IoAllocateIrp(d->StackSize,FALSE); if(!i)return NULL;
    i->stack.MajorFunction=major; i->Tail.Overlay.Thread=PsGetCurrentThread();
    i->user=io; i->event=e;
    io->Information=(ULONG_PTR)&unusedToken; /* deterministic substitute for Debug stack fill */
    return i;
}
#define IoSetCompletionRoutine(i,f,c,s,e,x) ((i)->callback=f,(i)->context=c,(i)->success=s,(i)->error=e,(i)->cancel=x)
static void complete(void)
{
    ASSERT(allocated && !done++ && !freed); pkt.IoStatus.Status=result;
    pkt.IoStatus.Information=NT_SUCCESS(result)?(ULONG_PTR)&produced:0;
    if(pkt.callback) {
        ASSERT(pkt.success && pkt.error && pkt.cancel && pkt.callback(NULL,&pkt,pkt.context)==STATUS_MORE_PROCESSING_REQUIRED);
    } else if(!delayApc) {
        *pkt.user=pkt.IoStatus; pkt.event->set=1; ++freed;
    }
}
static NTSTATUS IoCallDriver(Device *d,PIRP i)
{
    ASSERT(i==&pkt && d==&device && i->Tail.Overlay.Thread==PsGetCurrentThread() &&
           i->stack.MajorFunction==IRP_MJ_PNP && i->stack.MinorFunction==IRP_MN_QUERY_DEVICE_RELATIONS &&
           i->stack.Parameters.QueryDeviceRelations.Type==TargetDeviceRelation && i->IoStatus.Status==STATUS_NOT_SUPPORTED);
    if(completionTime!=2) complete();
    return completionTime ? STATUS_PENDING : result;
}
static void waitFor(KEVENT *e) { ++waits; if(!done) complete(); ASSERT(e->set); }
#define KeWaitForSingleObject(e,a,b,c,d) waitFor(e)
static void IoFreeIrp(PIRP i) { ASSERT(i==&pkt && done && !freed); ++freed; }
#include "device.inc"
static void run(const char *name,int timing,NTSTATUS s,int defer,int noMem)
{
    memset(&pkt,0,sizeof(pkt)); allocated=done=freed=waits=0;
    completionTime=timing; result=s; delayApc=defer; oom=noMem;
    Relations *r=NULL; NTSTATUS ret=EngpPnPTargetRelationRequest(&device,&r);
    verdict(name,ret==(oom?STATUS_INSUFFICIENT_RESOURCES:s) && r==(!oom && NT_SUCCESS(s)?&produced:NULL) &&
        allocated==!oom && done==!oom && freed==!oom && waits==(!oom && !!timing));
}
int main(void)
{
    run("inline success",0,0,0,0);
    run("inline success, IOSB APC not yet delivered",0,0,1,0);
    run("pending, completed before dispatch returns",1,0,0,0);
    run("pending, completes in wait",2,0,0,0);
    run("inline driver error",0,STATUS_IO_DEVICE_ERROR,0,0);
    run("pending driver error",2,STATUS_IO_DEVICE_ERROR,0,0);
    run("cancel completion",1,STATUS_CANCELLED,0,0);
    run("no IRP",0,0,0,1);
    return summary("MonitorRelationCompletion.patch");
}
