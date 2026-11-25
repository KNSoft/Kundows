/* Historical C2 workaround checks. These intentionally model missing BCBs.
 * The active CC-only volume checks use real pin.c functions in cc-volume.inc.
 * Real flush and large-MCB traversal functions come from run.py. */
#include "checks.h"
typedef ULONG VBO, *PVBO;
typedef LONGLONG LBO, *PLBO;
typedef struct { LARGE_INTEGER RunStartVbn, RunEndVbn, StartingLbn; } Run, *PLARGE_MCB_MAPPING_ENTRY;
typedef struct { Run rows[12]; unsigned n, iter; } Table;
typedef struct { Table Table; } Mapping;
typedef struct { Mapping *Mapping; } BASE_MCB_INTERNAL, *PBASE_MCB_INTERNAL, *PBASE_MCB;
typedef struct { int *GuardedMutex; BASE_MCB_INTERNAL BaseMcb; } LARGE_MCB, *PLARGE_MCB;
typedef void *PBCB;
typedef struct { ULONG Reserved, Fat; } Bpb;
typedef struct { ULONG VcbState, VcbCondition; Bpb Bpb; struct {ULONG FatIndexBitSize;} AllocationSupport;
                 int *VirtualVolumeFile, SectionObjectPointers, RootDcb; LARGE_MCB DirtyFatMcb; } VCB, *PVCB;
typedef struct { NTSTATUS ExceptionStatus; } Context, *PIRP_CONTEXT;
typedef int FAT_FLUSH_TYPE;
enum { VcbGood=1, VCB_STATE_FLAG_WRITE_PROTECTED=1, VCB_STATE_FLAG_REMOVABLE_MEDIA=2,
       VCB_STATE_FLAG_BOOT_OR_PAGING_FILE=4, PIN_WAIT=1, PIN_IF_BCB=8 };
#define FatReservedBytes(b) ((b)->Reserved)
#define FatBytesPerFat(b) ((b)->Fat)
#define FatVerifyVcb(c,v) ((void)0)
#define FatResetExceptionState(c) abort()
#define FsRtlIsNtstatusExpected(x) 0
#define FatExceptionFilter(c,e) EXCEPTION_CONTINUE_SEARCH
#define STATUS_FILE_INVALID ((NTSTATUS)0xc0000098)
#define MCB_SCALE_LOG2 9

static VCB v;
static Context ctx;
static Mapping mapping;
static int lock, calls, widen, noProgress, pinOffset, hits, unlocks;
static NTSTATUS dirStatus;
static LONGLONG failAt;
static ULONG lengths[12];
typedef struct { LONGLONG start, end; int copy[2]; } CheckRun;
static CheckRun check[12];
static unsigned ncheck;

static void *RtlEnumerateGenericTable(Table *t,BOOLEAN first)
{
    ASSERT(lock);
    if (first) t->iter=0; else ++t->iter;
    return t->iter < t->n ? &t->rows[t->iter] : NULL;
}
static void KeAcquireGuardedMutex(int *p) { ASSERT(!*p); ++*p; }
static void KeReleaseGuardedMutex(int *p) { ASSERT(*p==1); --*p; }
static NTSTATUS FatFlushDirectory(Context *c,int d,FAT_FLUSH_TYPE t) { return dirStatus; }
#define FatToggleMediaEjectDisable(c,v,b) (++unlocks)
static BOOLEAN CcPinRead(int *f,PLARGE_INTEGER o,ULONG len,ULONG flags,PBCB *b,void **buf)
{
    ASSERT(flags==(PIN_WAIT|PIN_IF_BCB));
    if (o->QuadPart!=pinOffset) return FALSE;
    ++hits; *b=&pinOffset; *buf=f; return TRUE;
}
#define CcSetDirtyPinnedData(b,l) ASSERT(b==&pinOffset)
#define CcRepinBcb(b) ASSERT(b==&pinOffset)
#define CcUnpinData(b) ASSERT(b==&pinOffset)
static void CcFlushCache(int *section,PLARGE_INTEGER o,ULONG length,PIO_STATUS_BLOCK io)
{
    ASSERT(!lock && section==&v.SectionObjectPointers && o && length && calls<12);
    LONGLONG a=o->QuadPart, b=a+length;
    lengths[calls++]=length;
    if (widen) { a-=a%VACB_MAPPING_GRANULARITY; b=(b+VACB_MAPPING_GRANULARITY-1)/VACB_MAPPING_GRANULARITY*VACB_MAPPING_GRANULARITY; }
    io->Information=0; io->Status=STATUS_SUCCESS;
    if (a<=failAt && failAt<b) { io->Status=STATUS_IO_DEVICE_ERROR; return; }
    if (noProgress) return; /* e.g. an already submitted page write */
    Table *t=&mapping.Table;
    for (unsigned i=0;i<ncheck;i++) {
        if (a<=check[i].start && check[i].end<=b) {
            check[i].copy[0]=check[i].copy[1]=1;
            for(unsigned j=0;j<t->n;j++) if(t->rows[j].RunStartVbn.QuadPart*512==check[i].start) {
                memmove(&t->rows[j],&t->rows[j+1],(t->n-j-1)*sizeof(Run)); --t->n; break;
            }
        }
    }
}
static void CcUnpinRepinnedBcb(PBCB b,BOOLEAN through,PIO_STATUS_BLOCK io)
{
    LARGE_INTEGER o; o.QuadPart=pinOffset; ASSERT(through && b==&pinOffset);
    CcFlushCache(&v.SectionObjectPointers,&o,PAGE_SIZE,io);
}
#include "fat-volume.inc"
static void reset(ULONG bits,ULONG fatBytes)
{
    memset(&v,0,sizeof(v)); memset(&mapping,0,sizeof(mapping)); memset(check,0,sizeof(check));
    v.VcbCondition=VcbGood; v.Bpb.Reserved=512; v.Bpb.Fat=fatBytes;
    v.AllocationSupport.FatIndexBitSize=bits; v.VirtualVolumeFile=&v.SectionObjectPointers;
    v.DirtyFatMcb.GuardedMutex=&lock; v.DirtyFatMcb.BaseMcb.Mapping=&mapping;
    lock=calls=noProgress=widen=unlocks=hits=dirStatus=ncheck=0; failAt=pinOffset=-1;
}
static void dirty(ULONG a,ULONG bytes)
{
    ASSERT(a>=v.Bpb.Reserved && !(a%512) && bytes && !(bytes%512));
    ASSERT((LONGLONG)a+bytes<=v.Bpb.Reserved+(LONGLONG)v.Bpb.Fat && ncheck<12);
    if(ncheck) ASSERT(check[ncheck-1].end<a);
    check[ncheck].start=a; check[ncheck++].end=(LONGLONG)a+bytes;
    Run *r=&mapping.Table.rows[mapping.Table.n++];
    r->RunStartVbn.QuadPart=r->StartingLbn.QuadPart=a/512; r->RunEndVbn.QuadPart=(a+(LONGLONG)bytes)/512;
}
static int done(void)
{
    for (unsigned i=0;i<ncheck;i++) if(!check[i].copy[0]||!check[i].copy[1]) return 0;
    return !mapping.Table.n && !lock;
}
static NTSTATUS volume(void) { return FatFlushVolume(&ctx,&v,1); }
int main(void)
{
    reset(32,8<<20); verdict("clean FAT",volume()==0 && done() && !calls);
    reset(32,8<<20); dirty(294912,512);
    verdict("released pin in a different view: both copies before return",volume()==0 && done());
    reset(12,6144); dirty(1024,512);
    verdict("FAT12 allocation",volume()==0 && done());
    reset(16,128<<10); dirty(126976,512);
    verdict("FAT16 allocation",volume()==0 && done());
    reset(32,8<<20); dirty(262144-512,1024); dirty(6<<20,512);
    verdict("cross-view run, distant run",volume()==0 && done() && calls==2);
    reset(32,8<<20); widen=1; dirty(4096,512); dirty(8192,512); dirty(1<<20,512);
    verdict("one completion removes multiple nearby runs",volume()==0 && done() && calls==2);
    reset(32,1U<<30); dirty((1U<<30)-512,512);
    verdict("one dirty sector near end of 1 GiB FAT",volume()==0 && done() && calls==1 && lengths[0]==512);
    reset(32,8<<20); dirty(1<<20,512); dirty(2<<20,512); dirty(3<<20,512); failAt=2<<20;
    verdict("failure after first range",volume()==STATUS_IO_DEVICE_ERROR && calls==2 && !lock &&
            check[0].copy[0] && check[0].copy[1] && !check[1].copy[0] && !check[2].copy[1] && mapping.Table.n==2);
    failAt=-1;
    verdict("retry writes remaining ranges",volume()==0 && done() && calls==4);
    reset(32,8<<20); dirty(1<<20,512); noProgress=1;
    verdict("in-flight range stays dirty, bounded call",!NT_SUCCESS(volume()) && calls==1 && mapping.Table.n==1 && !lock);
    noProgress=0;
    verdict("retry after in-flight range",volume()==0 && done() && calls==2);
    reset(32,8<<20); dirty(4096,512); pinOffset=4096;
    verdict("live pin: normal path writes once",volume()==0 && hits==1 && done() && calls==1);
    reset(32,8<<20); dirty(4096,512); pinOffset=4096; failAt=4096;
    verdict("original pin-path error preserved",volume()==STATUS_IO_DEVICE_ERROR && hits==1 && calls==1 && mapping.Table.n==1);
    reset(32,8<<20); dirty(4096,512); dirStatus=STATUS_UNSUCCESSFUL;
    verdict("directory error survives FAT success",volume()==dirStatus && done());
    reset(32,8<<20); dirty(4096,512); v.VcbState=VCB_STATE_FLAG_WRITE_PROTECTED;
    verdict("write-protected volume",volume()==0 && !calls && mapping.Table.n==1);
    reset(32,8<<20); dirty(4096,512); v.VcbCondition=0;
    verdict("invalid volume",volume()==STATUS_FILE_INVALID && !calls && mapping.Table.n==1);
    return summary("C2 volume");
}
