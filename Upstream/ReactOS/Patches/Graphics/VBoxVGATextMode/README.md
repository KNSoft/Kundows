# EFI Setup text was clipped on VBoxVGA

The pre-rework x64 Debug image used for comparison is the one that completed
a hard-disk installation in the Hyper-V Gen 1 matrix (SHA-256
`9398249C4C4C2391BBE56FE86CC51B180CD6D57BD77FDFC4659A69499EA86432`).
The CMake/VS control is the default x64 Debug ISO from `973eafc` (SHA-256
`71FA8B3F81B9EF1651C2F84B38ACFA05FFA08C7FCC5297E48252BC6140879555`).

The original captures used VirtualBox 7.1.10, one CPU, 1 GiB of RAM,
VBoxVGA, an IDE DVD, no network, no serial ports and no hard disk. After
selecting **ReactOS Setup (Text Mode)**, the visible language page was
compared with the VGA and VBE state and the text-memory view.

| Media | Firmware | VGA state / text-memory view | Visible result |
| --- | --- | --- | --- |
| Pre-rework Debug | BIOS | 80×50 / 80 columns; VBE disabled | Readable Setup |
| Pre-rework Debug | EFI64 | 80×50 / 400 columns; 3,200-byte VBE pitch | Clipped text and stale pixels |
| CMake/VS Debug control | EFI64 | 80×50 / 400 columns; 3,200-byte VBE pitch | Identical to pre-rework EFI64 |
| CMake/VS Debug with the display fix | EFI64 | 80×50 / 80 columns; VBE disabled | Readable Setup |

The two corrupted EFI screenshots were byte-identical. The `blue.sys`
source was also identical at the two ReactOS pins (`1103f43a0c0` and
`4fbbefd614b`). On this adapter the firmware left its 800×600×32 VBE
framebuffer enabled. Setup's `blue.sys` programmed legacy VGA text
registers, but the emulated display still fetched rows using the graphics
pitch. This explains why the language list was present in guest memory
and nevertheless unreadable on screen.

The [driver patch](BlueVBoxVBE.patch) detects
the VBoxVGA PCI adapter (`80ee:beef`, VGA class, I/O decoding enabled) and
disables the extended mode while `blue.sys` owns the text console. In the
published x64 Debug test image (`209008C7335C311DB636E128418DE58A240506D5E5B61AAE452E54EB35B7C57C`),
`info vga` reported `cbLine: 0x280`, versus `0xc80` before the fix.

| Before the fix | After the fix |
| --- | --- |
| ![Setup language page with clipped text](Assets/VBoxEfiTextBefore.png) | ![Readable Setup language page in 80-column mode](Assets/VBoxEfiTextAfter.png) |
