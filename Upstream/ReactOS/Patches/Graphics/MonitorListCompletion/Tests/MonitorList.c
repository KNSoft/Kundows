/* The producer owns the referenced list until success. The request and its
 * pointer-sized buffer belong to win32k. Exercise both ownership boundaries. */
#include "Checks.h"
#define IO_NO_INCREMENT 0
#define IRP_BUFFERED_IO 0x10
#define IRP_MJ_DEVICE_CONTROL 14
#define IOCTL_VIDEO_ENUM_MONITOR_PDO 0x230018
#define STATUS_NOT_SUPPORTED ((NTSTATUS)0xc00000bb)
#define STATUS_MORE_PROCESSING_REQUIRED ((NTSTATUS)0xc0000016)
#define STATUS_DEVICE_DATA_ERROR ((NTSTATUS)0xc000009c)
#define STATUS_CANCELLED ((NTSTATUS)0xc0000120)
enum {NonPagedPool, PagedPool, GDITAG_GDEVICE, TAG_VIDEO_PORT};
typedef struct { int StackSize, refs; void *DeviceExtension; } Device,*PDEVICE_OBJECT;
typedef struct { LIST_ENTRY ChildDeviceList; } Port,*PVIDEO_PORT_DEVICE_EXTENSION;
typedef struct { LIST_ENTRY ListEntry; Device *PhysicalDeviceObject; } VIDEO_PORT_CHILD_EXTENSION,*PVIDEO_PORT_CHILD_EXTENSION;
typedef struct { Device *pdo; ULONG HwID; } VIDEO_MONITOR_DEVICE,*PVIDEO_MONITOR_DEVICE;
typedef int KEVENT,*PKEVENT;
typedef struct { ULONG MajorFunction; struct { struct { ULONG IoControlCode,InputBufferLength,OutputBufferLength; } DeviceIoControl; } Parameters; } Stack,*PIO_STACK_LOCATION;
typedef struct Packet *PIRP;
struct Packet {
    ULONG Flags; IO_STATUS_BLOCK IoStatus; struct {void *SystemBuffer;} AssociatedIrp;
    struct {struct {void *Thread;} Overlay;} Tail; Stack stack;
    NTSTATUS (NTAPI *callback)(Device*,PIRP,void*); void *context;
};
static Port port;
static Device fdo={1,0,&port}, child;
static VIDEO_PORT_CHILD_EXTENSION ext;
static VIDEO_MONITOR_DEVICE malformed;
static struct Packet packet;
static int liveIrp, buffers, lists, signaling, waits, completed, timing, fail, thread;
static NTSTATUS driver;
static void *liveBuffer;
#define PsGetCurrentThread() (&thread)
#define ObReferenceObject(d) (++(d)->refs)
#define IoGetNextIrpStackLocation(i) (&(i)->stack)
#define KeInitializeEvent(e,t,s) (*(e)=(s))
#define KeSetEvent(e,p,w) (++signaling, *(e)=1)
#define IoSetCompletionRoutine(i,f,c,s,e,x) do { ASSERT(s && e && x); (i)->callback=f; (i)->context=c; } while (0)
static PIRP IoAllocateIrp(int size, BOOLEAN quota) { ASSERT(size==1 && !quota && !liveIrp); if(fail==1)return NULL; ++liveIrp; return &packet; }
static void IoFreeIrp(PIRP i) { ASSERT(i==&packet && liveIrp && (fail==2 || completed)); --liveIrp; }
static void *ExAllocatePoolZero(int pool, size_t n, int tag)
{
    if(tag==GDITAG_GDEVICE) {
        ASSERT(pool==NonPagedPool && n==sizeof(void*) && !buffers);
        if(fail==2)return NULL;
        ++buffers; return liveBuffer=calloc(1,n);
    }
    ASSERT(pool==PagedPool && tag==TAG_VIDEO_PORT);
    if(fail==3)return NULL;
    ++lists; return calloc(1,n);
}
static void ExFreePoolWithTag(void *b,int tag) { ASSERT(tag==GDITAG_GDEVICE && b==liveBuffer && buffers==1 && completed); free(b); --buffers; }
static NTSTATUS VideoPortEnumMonitorPdo(Device*,PVIDEO_MONITOR_DEVICE*,ULONG_PTR*);
static void finish(void)
{
    PIRP i=&packet; ASSERT(!completed++ && liveIrp && buffers);
    if(fail==6 || fail==7) {
        /* A non-null slot is not a valid transfer if the count is wrong.
         * Use a token, not an allocation with ambiguous ownership. */
        *(PVIDEO_MONITOR_DEVICE*)i->AssociatedIrp.SystemBuffer=&malformed;
        i->IoStatus.Information=fail==6 ? sizeof(void*)-1 : sizeof(void*)+1;
        i->IoStatus.Status=STATUS_SUCCESS;
    } else if(driver==0 && fail!=4 && fail!=5)
        i->IoStatus.Status=VideoPortEnumMonitorPdo(&fdo,i->AssociatedIrp.SystemBuffer,&i->IoStatus.Information);
    else { i->IoStatus.Status=driver; i->IoStatus.Information=(fail==5)?sizeof(void*):0; }
    ASSERT(i->callback(NULL,i,i->context)==STATUS_MORE_PROCESSING_REQUIRED);
}
static NTSTATUS IoCallDriver(Device *d,PIRP i)
{
    ASSERT(d==&fdo && i==&packet && i->Tail.Overlay.Thread==&thread && i->Flags==IRP_BUFFERED_IO &&
           i->AssociatedIrp.SystemBuffer==liveBuffer && i->stack.MajorFunction==IRP_MJ_DEVICE_CONTROL &&
           i->stack.Parameters.DeviceIoControl.IoControlCode==IOCTL_VIDEO_ENUM_MONITOR_PDO &&
           !i->stack.Parameters.DeviceIoControl.InputBufferLength &&
           i->stack.Parameters.DeviceIoControl.OutputBufferLength==sizeof(void*));
    if(timing!=2)finish();
    return timing?STATUS_PENDING:i->IoStatus.Status;
}
static void wait(KEVENT *e,int alertable) { ASSERT(!alertable); ++waits; if(!completed)finish(); ASSERT(*e); }
#define KeWaitForSingleObject(e,r,m,a,t) wait(e,a)
#include "dispatch.inc"
#include "device.inc"
static void run(const char *name,int when,int failure,NTSTATUS result,int children,NTSTATUS expect)
{
    memset(&packet,0,sizeof(packet)); timing=when;fail=failure;driver=result;
    waits=completed=signaling=0;
    InitializeListHead(&port.ChildDeviceList); if(children) { ext.PhysicalDeviceObject=&child; InsertTailList(&port.ChildDeviceList,&ext.ListEntry); }
    PVIDEO_MONITOR_DEVICE list=(PVIDEO_MONITOR_DEVICE)(ULONG_PTR)1;
    NTSTATUS s=EngpQueryMonitorList(&fdo,&list);
    int delivered=(expect==STATUS_SUCCESS ? (list && list[0].pdo==(children?&child:NULL) && !list[children].pdo && lists==1 && child.refs==children) : !list);
    if(list && s==0 && list!=&malformed) { if(children)--child.refs; --lists; free(list); }
    verdict(name,s==expect && delivered && !liveIrp && !buffers && !lists && !child.refs &&
                 waits==(!!when && failure!=1 && failure!=2) && signaling==completed);
}
int main(void)
{
    run("inline no children",0,0,0,0,0); run("inline referenced list",0,0,0,1,0);
    run("pending completed before return",1,0,0,1,0); run("pending completed in wait",2,0,0,1,0);
    run("inline failure",0,0,STATUS_IO_DEVICE_ERROR,1,STATUS_IO_DEVICE_ERROR);
    run("pending failure",2,0,STATUS_IO_DEVICE_ERROR,1,STATUS_IO_DEVICE_ERROR);
    run("pending cancel",2,0,STATUS_CANCELLED,1,STATUS_CANCELLED);
    run("no IRP",0,1,0,1,STATUS_INSUFFICIENT_RESOURCES); run("no system buffer",0,2,0,1,STATUS_INSUFFICIENT_RESOURCES);
    run("no producer list",2,3,0,1,STATUS_INSUFFICIENT_RESOURCES);
    run("short empty reply",0,4,0,1,STATUS_DEVICE_DATA_ERROR); run("missing result",0,5,0,1,STATUS_DEVICE_DATA_ERROR);
    run("short non-null slot",0,6,0,0,STATUS_DEVICE_DATA_ERROR);
    run("oversized non-null slot",2,7,0,0,STATUS_DEVICE_DATA_ERROR);
    return summary("MonitorListCompletion.patch");
}
