# APIC clock calibration waited for four RTC samples

On the same Hyper-V Gen 1 fixture, a diagnostic ISO (`791ED815…`) changed only `kdcom.dll` and its ISO directory
lengths. It passed the serial wait but stopped in the calibration loop:
the saved PC at `hal.dll+0x1232c` matched `HalpInitializeTsc+0xbc`, comparing
the interrupt sample count with four. The instruction bytes matched the CD's
`hal.dll`, not `halmp.dll`. This and the [reciprocal KD control](../KdAutoProbe/README.md) isolate the waits without
attaching a debugger or changing the blank disk or VM configuration.

[RtcPeriodic.patch](RtcPeriodic.patch) starts the periodic divider after installing the handler and unmasking its interrupt. With both fixes, Setup advanced on the same VM. The [complete install](../../Shared/BootCD/README.md#boot-controls) included further patches; it is not an RTC-only acceptance test.
