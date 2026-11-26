/* Characterize the returned object on the pin, including the legacy path.
 * This does not emulate a display adapter or APC delivery. */
#include "Checks.h"
typedef struct Device { void *DeviceExtension; int references; } Device,*PDEVICE_OBJECT;
typedef struct { Device *PhysicalDeviceObject,*NextDeviceObject; } Extension,*PVIDEO_PORT_DEVICE_EXTENSION;
typedef struct { ULONG Count; Device *Objects[1]; } *PDEVICE_RELATIONS;
typedef int DEVICE_RELATION_TYPE;
enum {TargetDeviceRelation=1,BusRelations,IRP_MN_START_DEVICE,IRP_MN_FILTER_RESOURCE_REQUIREMENTS,
      IRP_MN_QUERY_DEVICE_RELATIONS,IRP_MN_REMOVE_DEVICE,IRP_MN_QUERY_REMOVE_DEVICE,IRP_MN_CANCEL_REMOVE_DEVICE,
      IRP_MN_SURPRISE_REMOVAL,IRP_MN_STOP_DEVICE,IRP_MN_QUERY_STOP_DEVICE,IRP_MN_CANCEL_STOP_DEVICE};
typedef struct { DWORD MinorFunction; struct { struct {int Type;} QueryDeviceRelations; } Parameters; } Stack,*PIO_STACK_LOCATION;
typedef struct { IO_STATUS_BLOCK IoStatus; Stack stack; } IrpType,*PIRP;
static int fail,completions;
#define IoGetCurrentIrpStackLocation(i) (&(i)->stack)
#define IntVideoPortForwardIrpAndWait(...) (abort(),0)
#define IntVideoPortPnPStartDevice(...) (abort(),0)
#define IoCompleteRequest(i,p) (++completions)
#define IntVideoPortFilterResourceRequirements(...) (abort(),0)
#define ExAllocatePoolZero(p,n,t) (fail?NULL:calloc(1,n))
#define ObReferenceObject(d) (++(d)->references)
#define IntVideoPortQueryBusRelations(...) (abort(),0)
#define IoSkipCurrentIrpStackLocation(i) ((void)0)
#define IoCallDriver(...) (abort(),0)
#include "video.inc"
static void run(int attached,int oom)
{
    Device pdo={0},fdo={0}; Extension ext={0}; IrpType irp={0};
    fdo.DeviceExtension=&ext; if(attached) ext.PhysicalDeviceObject=ext.NextDeviceObject=&pdo;
    irp.stack.MinorFunction=IRP_MN_QUERY_DEVICE_RELATIONS;
    irp.stack.Parameters.QueryDeviceRelations.Type=TargetDeviceRelation; completions=0; fail=oom;
    NTSTATUS s=IntVideoPortDispatchFdoPnp(&fdo,&irp);
    PDEVICE_RELATIONS r=(PDEVICE_RELATIONS)irp.IoStatus.Information;
    ASSERT(s==irp.IoStatus.Status && completions==1);
    if(oom) ASSERT(s==STATUS_INSUFFICIENT_RESOURCES && !r && !pdo.references && !fdo.references);
    else { ASSERT(s==0 && r && r->Count==1 && r->Objects[0]==&fdo && fdo.references==1 && !pdo.references); free(r); }
    printf("Video target relation %s%s: %s\n",attached?"PnP":"legacy",oom?" / no pool":"",oom?"allocation error":"Count=1, referenced FDO");
}
int main(void) { run(1,0); run(0,0); run(1,1); return 0; }
