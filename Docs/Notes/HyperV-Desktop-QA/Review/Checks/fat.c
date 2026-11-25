#include "checks.h"
#undef NTDDI_VERSION
#define NTDDI_VERSION NTDDI_WS03
enum {VcbGood=1, UserVolumeOpen, Flush, VolumeClean};
enum {VCB_STATE_FLAG_VOLUME_DIRTY=1,VCB_STATE_FLAG_MOUNTED_DIRTY=2,
      VCB_STATE_FLAG_REMOVABLE_MEDIA=4,VCB_STATE_FLAG_BOOT_OR_PAGING_FILE=8,VCB_STATE_FLAG_SHUTDOWN=16};
enum {IRP_CONTEXT_FLAG_WAIT=1, IRP_CONTEXT_FLAG_DISABLE_POPUPS=2, IRP_CONTEXT_FLAG_WRITE_THROUGH=4};
typedef struct { ULONG Flags; } Context,*PIRP_CONTEXT;
typedef struct { LIST_ENTRY VcbLinks; ULONG VcbState; int VcbCondition,CleanVolumeTimer,CleanVolumeDpc;
                 void *SectionObjectPointers,*TargetDeviceObject; } VCB,*PVCB;
typedef struct { IO_STATUS_BLOCK IoStatus; int PendingReturned; } IrpType,*PIRP;
typedef struct { void *FileObject; } Stack,*PIO_STACK_LOCATION;
typedef void *PFILE_OBJECT,*PFCB,*PCCB,*PDEVICE_OBJECT;
typedef int TYPE_OF_OPEN,KEVENT;
static struct { LIST_ENTRY VcbQueue; int ShutdownStarted; } FatData;
static VCB v;
static Stack stack;
static void *completionContext;
static NTSTATUS (NTAPI *complete)(PDEVICE_OBJECT,PIRP,PVOID);
static NTSTATUS flushResult,driverResult;
static int mark,purge,sent,locks,cancelled,removed,raised,resetException,flushes;
static IrpType lowerIrp;
#define IoMarkIrpPending(i) ((void)0)
#define IoGetCurrentIrpStackLocation(i) (&stack)
#define FatDecodeFileObject(f,p,fcb,ccb) (*(p)=&v,UserVolumeOpen)
#define FatFsdPostRequest(c,i) (abort(),0)
#define FatAcquireExclusiveVcb(c,v) (++locks,TRUE)
#define FatReleaseVcb(c,v) (--locks)
#define FatReleaseFcb(c,f) abort()
#define FatUnpinRepinnedBcbs(c) ((void)0)
#define FatBugCheck(...) abort()
#define DebugUnwind(f) ((void)0)
#define KeCancelTimer(t) (++cancelled)
#define KeRemoveQueueDpc(d) (++removed)
#define FatToggleMediaEjectDisable(...) ((void)0)
#define IoCopyCurrentIrpStackLocationToNext(i) ((void)0)
#define IoSetCompletionRoutine(i,f,c,a,b,d) (complete=f,completionContext=c)
#define FatCompleteRequest(c,i,s) ((void)0)
#define KeInitializeEvent(...) ((void)0)
#define FatAcquireExclusiveGlobal(c) (++locks)
#define FatReleaseGlobal(c) (--locks)
#define FatAcquireExclusiveVolume(c,v) (++locks)
#define FatReleaseVolume(c,v) (--locks)
#define CcPurgeCacheSection(...) (++purge)
#define FatMarkVolume(c,v,state) (++mark)
#define FatResetExceptionState(c) (++resetException)
#define IoBuildSynchronousFsdRequest(...) (&lowerIrp)
#define KeWaitForSingleObject(...) ((void)0)
#define KeClearEvent(e) ((void)0)
#define FatCheckForDismount(...) FALSE
#define IoUnregisterFileSystem(...) ((void)0)
#define IoDeleteDevice(...) ((void)0)
static NTSTATUS FatFlushVolume(PIRP_CONTEXT c,PVCB v,int type)
{
    ASSERT(locks && type==Flush); ++flushes;
    if (raised) RaiseException((DWORD)STATUS_IO_DEVICE_ERROR,0,0,NULL);
    return flushResult;
}
static NTSTATUS IoCallDriver(PDEVICE_OBJECT d,PIRP irp)
{
    ++sent; irp->IoStatus.Status=driverResult;
    if(complete) complete(d,irp,completionContext);
    return driverResult;
}
#include "fat.inc"

static void run(const char *name,int shutdown,ULONG flags,NTSTATUS fs,NTSTATUS lower,
                int exception,int shouldMark,int dirty)
{
    Context c={IRP_CONTEXT_FLAG_WAIT}; IrpType i={0};
    memset(&v,0,sizeof(v)); InitializeListHead(&FatData.VcbQueue);
    v.VcbState=flags; v.VcbCondition=VcbGood; InsertTailList(&FatData.VcbQueue,&v.VcbLinks);
    flushResult=fs; driverResult=lower; complete=NULL; FatData.ShutdownStarted=0;
    mark=purge=sent=locks=cancelled=removed=resetException=flushes=0; raised=exception;
    NTSTATUS s=shutdown ? FatCommonShutdown(&c,&i) : FatCommonFlushBuffers(&c,&i);
    NTSTATUS expect=shutdown ? 0 : (!NT_SUCCESS(lower) && lower!=STATUS_INVALID_DEVICE_REQUEST ? lower : fs);
    verdict(name,s==expect && (shutdown || i.IoStatus.Status==expect) && flushes==1 && sent==1 &&
        !locks && cancelled==(!shutdown && !!(flags&VCB_STATE_FLAG_VOLUME_DIRTY)) && removed==cancelled &&
        !!(v.VcbState&VCB_STATE_FLAG_VOLUME_DIRTY)==dirty && mark==shouldMark && purge==(shutdown && shouldMark) &&
        resetException==exception && !!(v.VcbState&VCB_STATE_FLAG_SHUTDOWN)==shutdown);
}
int main(void)
{
    run("volume: success",0,1,0,0,0,1,0);
    run("volume: failure despite lower success",0,1,STATUS_IO_DEVICE_ERROR,0,0,0,1);
    run("volume: unsupported lower flush preserves FS error",0,1,STATUS_IO_DEVICE_ERROR,STATUS_INVALID_DEVICE_REQUEST,0,0,1);
    run("volume: mounted dirty",0,3,0,0,0,0,1);
    run("volume: already clean",0,0,0,0,0,0,0);
    run("volume: removable on failure",0,5,STATUS_IO_DEVICE_ERROR,0,0,0,1);
    run("shutdown: success",1,1,0,0,0,1,1);
    run("shutdown: returned error",1,1,STATUS_IO_DEVICE_ERROR,0,0,0,1);
    run("shutdown: raised error",1,1,0,0,1,0,1);
    run("shutdown: mounted dirty",1,3,0,0,0,0,1);
    return summary("F1");
}
