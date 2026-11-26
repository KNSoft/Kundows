$blue = 'drivers/setup/blue'
$bv = 'drivers/base/bootvid'
$fb = Read-Unit "$bv/framebuf/bootvid.c"
$render = Read-Unit "$blue/setupfb.c"
Write-Include 'grid.h' (Read-Unit "$blue/setupfb.h")
Write-Include 'palette.h' (Read-Unit 'sdk/include/reactos/drivers/bootvid/display.h')
$colors = [regex]::Match($render, '(?ms)^static const UCHAR TextColor.*?^};')
$pixels = [regex]::Match($fb, '(?ms)^#define BB_PIXEL.*?(?=^static ULONG_PTR)')
if (!$colors.Success -or !$pixels.Success) { throw 'Missing console color table or framebuffer addressing macros' }
Write-Include 'raster.inc' @($pixels.Value; (Functions $fb @('ULONG PackChannel','VOID InitPaletteWithTable','VOID SetPixel')); (Functions (Read-Unit "$bv/common.c") @('VOID BitBlt','VOID VidBufferToScreenBlt')); $colors.Value; (Functions $render @('VOID ScrFbPresent')))
$driver = Read-Unit "$blue/blue.c"
Write-Include 'device.inc' ([regex]::Match($driver, '(?ms)^typedef struct _DEVICE_EXTENSION.*?^\} DEVICE_EXTENSION, \*PDEVICE_EXTENSION;').Value)
Write-Include 'console.inc' @(Functions $driver @('VOID ScrPresent','BOOLEAN ScrResetScreen','NTSTATUS ScrWrite'))
