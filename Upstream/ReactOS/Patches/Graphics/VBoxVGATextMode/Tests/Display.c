/* Drive blue's actual glyph presenter through BOOTVID into padded mock VRAM. */
#define NOGDI
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "grid.h"
#include "palette.h"

#define W 800
#define H 600
#define STRIDE 832
#define GUARD 0xdeadbeefUL
#define RGBQUAD DWORD
#undef GetRValue
#undef GetGValue
#undef GetBValue
#define GetRValue(c) ((BYTE)((c) >> 16))
#define GetGValue(c) ((BYTE)((c) >> 8))
#define GetBValue(c) ((BYTE)(c))
#define PrepareForSetPixel() ((void)0)
static ULONG Vram[STRIDE * H], CachedPalette[16], RedMask, GreenMask, BlueMask, ReservedMask;
static UCHAR BackBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
static ULONG_PTR FrameBufferStart;
static ULONG PanH = 80, PanV = 60, VidpXScale = 1, VidpYScale = 1;
static ULONG BytesPerScanLine = STRIDE * 4, BytesPerPixel = 4;
static VOID ApplyPalette(void) { } /* Only SetPixel writes the mock VRAM. */
#include "raster.inc"

static SETUP_FB Fb;
static BOOLEAN Enabled;

#include "device.inc"
typedef LARGE_INTEGER PHYSICAL_ADDRESS;
typedef struct { struct { struct { ULONG Length; } Write; } Parameters; } IO_STACK_LOCATION, *PIO_STACK_LOCATION;
typedef struct { IO_STACK_LOCATION Stack; PCHAR UserBuffer; struct { NTSTATUS Status; } IoStatus; } IRP, *PIRP;
typedef struct { PDEVICE_EXTENSION DeviceExtension; } DEVICE_OBJECT, *PDEVICE_OBJECT;
static PDEVICE_EXTENSION ResetDisplayParametersDeviceExtension;
static BOOLEAN BootReady;
static unsigned Acquires, Cleanups, Resets;
#define STATUS_SUCCESS 0L
#define TAB_WIDTH 8
#define VIDMEM_BASE 0xb8000
#define IoGetCurrentIrpStackLocation(irp) (&(irp)->Stack)
#define IoCompleteRequest(irp, boost) ((void)0)
#define ASSERT assert
#define DPRINT1(...) ((void)0)
#define ScrInbvInitialize() (++Acquires)
#define ScrInbvCleanup() (++Cleanups)
#define InbvIsBootDriverInstalled() (BootReady)
#define VidResetDisplay(mode) (assert(!(mode)), ++Resets)
#define ScrSetCursor(d) assert((d)->BootText)
#define ScrSetCursorShape(d) assert((d)->BootText)
#define ScrFbDefaultFont(fb) memset((fb)->Font, 0, sizeof((fb)->Font))
/* A framebuffer ownership transition must not enter the legacy map/allocate path. */
#define ScrAcquireOwnership(d) abort()
#define ExFreePoolWithTag(p,t) abort()
#define ExAllocatePoolWithTag(p,s,t) (abort(), (PUCHAR)NULL)
#define MmUnmapIoSpace(p,s) abort()
#define MmMapIoSpace(p,s,c) (abort(), (PUCHAR)NULL)
static void Fill32(PVOID p, SIZE_T n, ULONG v)
{
    SIZE_T i; for (i = 0; i < n / sizeof(v); ++i) ((ULONG *)p)[i] = v;
}
#undef RtlFillMemoryUlong
#define RtlFillMemoryUlong Fill32
#include "console.inc"

static void CheckMode(BOOL Bgr)
{
    ULONG table[16] = {0}, i, x, y;
    ULONG first = (60 + (480 - 400) / 2) * STRIDE + 80;
    ULONG last = first + 49 * 8 * STRIDE + 79 * 8;
    ULONG red = Bgr ? 0xffff0000UL : 0xff0000ffUL;
    ULONG blue = Bgr ? 0xff0000ffUL : 0xffff0000UL;
    RedMask = red & 0xffffff; GreenMask = 0xff00; BlueMask = blue & 0xffffff;
    ReservedMask = 0xff000000;
    table[BV_COLOR_RED] = 0xff0000; table[BV_COLOR_BLUE] = 0xff;
    InitPaletteWithTable(table, 16);
    for (i = 0; i < STRIDE * H; i++) Vram[i] = GUARD;
    memset(&Fb, 0, sizeof(Fb));
    Fb.Font[0xe1 * 8] = 0x81;  /* Two red end pixels on a blue background. */
    Fb.Cells[0] = Fb.Cells[SETUP_CELLS - 1] = 0x14e1;
    Enabled = TRUE;
    ScrFbPresent(&Fb, &Enabled, SETUP_CELLS - 1, 1, TRUE);
    assert(Vram[first] == red && Vram[first + 7] == red && Vram[first + 1] == blue);
    assert(Vram[first + STRIDE] == blue && Vram[last] == red);
    assert(Vram[last + 7 * STRIDE + 1] == red); /* cursor on bottom scan line */
    for (y = 0; y < H; ++y)
        for (x = 0; x < STRIDE; ++x)
            if (y < 100 || y >= 500 || x < 80 || x >= 720)
                assert(Vram[y * STRIDE + x] == GUARD);
    ScrFbPresent(&Fb, &Enabled, 0, 1, FALSE); /* cursor moves, no cell edit */
    assert(Vram[first + 7 * STRIDE + 1] == red && Vram[last + 7 * STRIDE + 1] == blue);
    Enabled = FALSE;
    Fb.Cells[0] = 0x4100;
    ScrFbPresent(&Fb, &Enabled, 0, 0, TRUE);
    assert(Vram[first + 1] == blue);
    Enabled = TRUE;
    ScrFbPresent(&Fb, &Enabled, 0, 0, TRUE);
    assert(Vram[first + 1] == red);
    Fb.Font[0xe1 * 8] = 0x40; /* Code-page reload must repaint unchanged cells. */
    ScrFbPresent(&Fb, &Enabled, 0, 0, TRUE);
    assert(Vram[last] == blue && Vram[last + 1] == red);
}

static void CheckConsole(void)
{
    DEVICE_EXTENSION d = {0};
    DEVICE_OBJECT device = {&d};
    IRP irp = {0};
    d.BootText = &Fb;
    ResetDisplayParametersDeviceExtension = &d;
    assert(!ScrResetScreen(&d, TRUE, TRUE)); /* no initialized BOOTVID */
    assert(!Acquires && !Resets);
    BootReady = TRUE;
    assert(ScrResetScreen(&d, TRUE, TRUE) && d.Enabled);
    assert(Acquires == 1 && Resets == 1 && d.Columns == 80 && d.Rows == 50);
    Fb.Cells[81] = 0x1ed0;
    d.CursorX = 79; d.CursorY = 49;
    irp.UserBuffer = "A"; irp.Stack.Parameters.Write.Length = 1;
    assert(ScrWrite(&device, &irp) == STATUS_SUCCESS);
    assert(d.CursorX == 0 && d.CursorY == 49 && Fb.Cells[1] == 0x1ed0);
    assert(Fb.Cells[49 * 80 - 1] == 0x1741 && Fb.Cells[49 * 80] == 0x1720);
    assert(ScrResetScreen(&d, FALSE, FALSE) && !d.Enabled);
    assert(Acquires == 1 && !Cleanups && Resets == 1);
    assert(ScrWrite(&device, &irp) == STATUS_SUCCESS && Fb.Cells[49 * 80] == 0x1720);
    assert(ScrResetScreen(&d, FALSE, TRUE) && d.Enabled && Resets == 2);
    assert(Fb.Cells[1] == 0x1ed0 && d.CursorY == 49);
    assert(ScrResetScreen(&d, TRUE, FALSE) && !d.Enabled && Cleanups == 1);
    assert(!ScrResetScreen(&d, FALSE, TRUE) && !d.Enabled && Resets == 2);
}

int main(void)
{
    FrameBufferStart = (ULONG_PTR)Vram;
    CheckMode(TRUE);
    CheckMode(FALSE);
    CheckConsole();
    puts("PASS: BGRX/RGBX, padded pitch, last glyph, attributes, cursor, font, scrolling and ownership");
    return 0;
}
