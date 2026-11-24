# x64 BootCD: Setup display and hard-disk installation

This note records two findings from the CMake/MSVC rework on ReactOS
`4fbbefd614b`: the VBoxVGA text corruption also occurred on the earlier
`1103f43a0c0` image, while the [Hyper-V installation](#hyper-v-gen-1)
exposed separate boot and file-copy problems. For the current test procedure,
see [Hyper-V verification](../../windows-reimplementation.md#hyper-v-验证).

## Historical VBoxVGA comparison

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

The [driver patch](../../../Build/Patches/ReactOS-blue-vbox-vbe.patch) detects
the VBoxVGA PCI adapter (`80ee:beef`, VGA class, I/O decoding enabled) and
disables the extended mode while `blue.sys` owns the text console. In the
published x64 Debug test image (`209008C7335C311DB636E128418DE58A240506D5E5B61AAE452E54EB35B7C57C`),
`info vga` reported `cbLine: 0x280`, versus `0xc80` before the fix.

[Before: clipped language page](Images/vbox-efi-text-before.png) ·
[After: readable language page](Images/vbox-efi-text-after.png)

## Hard-disk installation

The display check ends at language selection. A separate BIOS test exercised
file copying from the CD to a fresh 2.5 GiB IDE disk, using an answer floppy,
FAT, one CPU, and no NIC or serial ports.

On the pre-I/O-fix image, the no-option Debug installer stopped at 68%;
the initial Setup process had exited with `STATUS_BREAKPOINT`. With the
four historical `--image-fixes`, another installation stopped in a kernel
runtime check. In that dump, the file-information IRP's completion status
block overlapped the stack guard reported by `KiDeliverApc`. The
[I/O fix](../../../Build/Patches/ReactOS-io-sync-status.patch) uses the I/O
manager's deferred-completion path to finish inline requests before the
status block goes out of scope. Pending requests retain their event waits.
MSVC Debug runtime checks remained enabled.

### Hyper-V Gen 1

The test used the x64 mainline settings: 1 vCPU, fixed 1 GiB RAM, no NIC
or COM connection, and a fresh 8 GiB IDE VHDX. The hard disk was first
in the boot order, so the blank disk fell back to the DVD. In **ReactOS
Setup (Text Mode)** we selected **FAT (Quick Format)** and installed the
MBR and VBR. A separate answer floppy was attached, but Setup still needed
keyboard input in both the text and graphical stages.

With the pinned-cache fix enabled and the MM change still optional, the
default `Configure.cmd`, `Build.cmd`, `MakeISO.cmd` sequence produced
`OutDir\bootcd-x64-Debug.iso` (SHA-256
`7D92E350127CC0020566DEE555809946EF809989FB765E73962F90B928B8344D`).
The install went from the blank disk through text Setup, the hard-disk
FreeLdr menu, graphical Setup, and Explorer. After a guest-initiated
shutdown, a cold start with **both DVD and floppy empty** reached the
desktop again. A guest Ctrl+Alt+Del opened the ReactOS Security dialog.

[First hard-disk boot](Images/hyperv-x64-debug-disk-boot.png) ·
[Cold-boot desktop](Images/hyperv-x64-debug-desktop.png) ·
[Response to Ctrl+Alt+Del](Images/hyperv-x64-debug-security.png)

After another clean shutdown, a read-only disk copy yielded all 1,110 files
under `\ReactOS` (198,183,396 bytes). All 785 installed `.exe`, `.dll`,
`.sys`, `.ocx` and `.cpl` files matched packaged build inputs. The two FAT32
tables were byte-identical, and walking the directory tree found no short
or invalid cluster chains. The fixture was then restored to `Clean-blank-Off`.

#### Boot controls

The x64 Debug build with the I/O, serial KD and RTC fixes, but without
the CC/MM changes, produced a BootCD
(SHA-256 `A80DC1D449B3CC0C469AAD4D21FCFF31D0F4D3BE33A59AD768EA87591E62614B`).
MSVC runtime checks and kernel debugging were enabled. All 878 checked
binary and CAB paths in the generated BootCD file list matched their
inputs byte for byte, including Setup, both kernels, both HALs and `kdcom.dll`.
Text Setup completed. On the first hard-disk boot, [FreeLdr asserted in
`FatReadFile`](Images/hyperv-x64-fat-chain-before.png) (`fat.c:1395`).
Before powering off the guest we saved it
and inspected a read-only disk copy. The two FAT32 tables differed.
`system32\hal.dll` was 403,456 bytes but its chain had one 4,096-byte
cluster instead of 99. The other 785 installed PEs matched packaged inputs.
The installation wrote a damaged file chain; the ISO's HAL was intact.

The `CachePinnedDataTests.c` fixture from `26617bf` ran the production
`pin.c` functions from the current pin with a writeback forced between
marking a pinned buffer dirty and the caller's update. It reported nine
failed checks in nine cases before the CC change and none afterwards.

For the x64 Debug control with the I/O fix, before enabling KD/RTC by default
(`02C6FCC59CC6AC0311160E0B605F51DD268D0EB6D0B46315928E86971007BD7B`),
the kernel, Setup executable and text driver inside the ISO matched their
VS outputs. With **ReactOS Setup (Text Mode)** selected, the last visible
output was FreeLdr's COM1 notice. At 4 min 10 s the differencing VHDX was
still at its original 4 MiB size. The banner alone does not identify the
blocking component.

With the I/O fix and all four historical runtime patches (KD, RTC, CC, MM; ISO SHA-256
`DEFD2CE8854628957AE943E3AC1724883221973B14089F7700E518EAF947E78B`),
the same fixture copied files, rebooted to the installed disk's FreeLdr
menu, completed graphical Setup, and rebooted to Explorer. With the DVD
and floppy ejected, a guest Ctrl+Alt+Del opened the ReactOS Security dialog.

To isolate the early boot delay, one diagnostic copy (`64FF1643…`) of the control ISO
changed only `hal.dll` and `halmp.dll` to the RTC-fixed versions. It still
showed the COM1 banner after 220 s. A saved Hyper-V CPU state put the
instruction pointer at `kdcom.dll+0x21d2`, with the matching PDB resolving
it to `Cp16550ReadLsr+0x52`. The stack contained `Uart16550GetByte`,
`KdpReceiveByte` and `KdpReceivePacketLeader`. This identifies an actual
wait for serial debugger input; it does not establish an infinite loop.

The reciprocal diagnostic (`791ED815…`) changed only `kdcom.dll` and its ISO directory
lengths. It passed the serial wait but stopped in the calibration loop:
the saved PC at `hal.dll+0x1232c` matched `HalpInitializeTsc+0xbc`, comparing
the interrupt sample count with four. The instruction bytes matched the CD's
`hal.dll`, not `halmp.dll`. These two controls isolate the waits without
attaching a debugger or changing the blank disk or VM configuration.
