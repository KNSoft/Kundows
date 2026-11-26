# An inline file-information completion outlived its stack status block

The display check ends at language selection. A separate BIOS test exercised
file copying from the CD to a fresh 2.5 GiB IDE disk, using an answer floppy,
FAT, one CPU, and no NIC or serial ports.

On the pre-I/O-fix image, the no-option Debug installer stopped at 68%;
the initial Setup process had exited with `STATUS_BREAKPOINT`. With the
four historical `--image-fixes`, another installation stopped in a kernel
runtime check. In that dump, the file-information IRP's completion status
block overlapped the stack guard reported by `KiDeliverApc`. The
[I/O fix](IoSyncStatus.patch) uses the I/O
manager's deferred-completion path to finish inline requests before the
status block goes out of scope. Pending requests retain their event waits.
MSVC Debug runtime checks remained enabled.

The [installed-disk control](../../Shared/BootCD/README.md#hyper-v-gen-1) exercised file copying with the change. A successful installation alone does not identify which IRP caused the earlier failure.
