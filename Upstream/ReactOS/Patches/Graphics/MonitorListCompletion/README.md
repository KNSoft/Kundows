# Monitor enumeration reported success before its output buffer was delivered

After the [PnP target-relation repair](../MonitorTargetRelation/README.md), the same installation disk reached `IOCTL_VIDEO_ENUM_MONITOR_PDO` and [stopped at the output-length assertion](Assets/FirstDiskAfterG1Stop.png). The port had supplied a non-null list and reported eight bytes. The caller still had the Debug fill values for both its pointer and length; the buffered result had not been copied to it.

[`MonitorListCompletion.patch`](MonitorListCompletion.patch) makes this private pointer-sized query own the IRP and system buffer until completion. It checks the final status, exact size and non-null list before transferring the references. It does not change generic `EngDeviceIoControl`, whose other clients can have different buffer contracts.

The [generic IOCTL control](Tests/Ioctl.c) distinguishes a successful inline return from delivery of the buffered output. The [private-query test](Tests/MonitorList.c) covers inline/pending completion, invalid lengths, empty output and failed allocations: **14/14** on x86 and x64. With both monitor repairs, the ordinary boot entry reached [graphical Setup](Assets/FirstDiskAfterG4Gui.png) with the same 29 FAT differences still present. The [Debug boot failure](../../Storage/FAT32InstallMirrorDivergence/README.md) had a different cause.

Run the [host case](Tests/Run.cmd) from a native VS developer prompt. Pass `--stock` to exercise the pinned source.
