/* Characterization of the buffered monitor-list IOCTL on the original path.
 * The copy-back boundary is controlled without touching an expired stack. */
#include "Checks.h"
#define TRACE(...) ((void)0)
#define STATUS_BUFFER_OVERFLOW ((NTSTATUS)0x80000005)
#define STATUS_NOT_IMPLEMENTED ((NTSTATUS)0xc0000002)
#define STATUS_BUFFER_TOO_SMALL ((NTSTATUS)0xc0000023)
#define STATUS_DEVICE_DOES_NOT_EXIST ((NTSTATUS)0xc00000c0)
typedef struct Device { void *DeviceExtension; int refs; } Device,*PDEVICE_OBJECT;
typedef struct { LIST_ENTRY ChildDeviceList; } Port,*PVIDEO_PORT_DEVICE_EXTENSION;
typedef struct { LIST_ENTRY ListEntry; Device *PhysicalDeviceObject; } VIDEO_PORT_CHILD_EXTENSION,*PVIDEO_PORT_CHILD_EXTENSION;
typedef struct { Device *pdo; ULONG HwID; } VIDEO_MONITOR_DEVICE,*PVIDEO_MONITOR_DEVICE;
typedef int KEVENT;
typedef struct { PVIDEO_MONITOR_DEVICE systemBuffer,*output; IO_STATUS_BLOCK status,*iosb; KEVENT *event; } Packet,*PIRP;
static Packet request;
static int timing;
static Port port;
static Device fdo={&port}, child={0};
static VIDEO_PORT_CHILD_EXTENSION ext;
static VIDEO_MONITOR_DEVICE empty;
#define KeInitializeEvent(e,t,s) (*(e)=0)
#define ExAllocatePoolZero(p,n,t) calloc(1,n)
#define ObReferenceObject(p) (++(p)->refs)

static NTSTATUS VideoPortEnumMonitorPdo(Device*,PVIDEO_MONITOR_DEVICE*,ULONG_PTR*);
static PIRP IoBuildDeviceIoControlRequest(DWORD code,Device *d,void *in,DWORD ilen,
                                          void *out,DWORD olen,BOOLEAN internal,KEVENT *e,IO_STATUS_BLOCK *iosb)
{
    ASSERT(code==0x230018 && d==&fdo && !in && !ilen && olen==sizeof(void*) && !internal);
    request.event=e; request.iosb=iosb; request.output=out; request.systemBuffer=NULL;
    iosb->Information=0; iosb->Status=STATUS_UNSUCCESSFUL; /* known unfilled marker */
    return &request;
}
static void deliver(void)
{
    *request.output=request.systemBuffer; *request.iosb=request.status; *request.event=1;
}
static NTSTATUS IoCallDriver(Device *d,PIRP i)
{
    i->status.Status=VideoPortEnumMonitorPdo(d,&i->systemBuffer,&i->status.Information);
    ASSERT(i->status.Status==0 && i->status.Information==sizeof(void*));
    if(timing==0) deliver();
    return timing==2 ? STATUS_PENDING : i->status.Status;
}
#define KeWaitForSingleObject(e,reason,mode,alert,timeout) (deliver(),STATUS_SUCCESS)
#include "ioctl.inc"

int main(void)
{
    InitializeListHead(&port.ChildDeviceList); ext.PhysicalDeviceObject=&child;
    InsertTailList(&port.ChildDeviceList,&ext.ListEntry);
    for(timing=0;timing<3;++timing) {
        DWORD bytes=~0u; PVIDEO_MONITOR_DEVICE got=&empty;
        DWORD code=EngDeviceIoControl(&fdo,0x230018,NULL,0,&got,sizeof(got),&bytes);
        int received=(got==request.systemBuffer);
        printf("Buffered monitor-list IOCTL %s: result=%lu driver-bytes=%zu caller-bytes=%lu buffer-delivered=%d\n",
               timing==0?"inline/delivered":timing==1?"inline/deferred":"pending",
               code,(size_t)request.status.Information,bytes,received);
        ASSERT(code==0 && received==(timing!=1) && bytes==(received ? sizeof(got) : 0));
        PVIDEO_MONITOR_DEVICE list=request.systemBuffer;
        ASSERT(list[0].pdo==&child && !list[1].pdo && child.refs==1);
        --child.refs; free(list);
    }
    return 0;
}
