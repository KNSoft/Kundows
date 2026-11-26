# Early kernel debugger discovery waited for a disconnected serial port

To isolate the early boot delay, one diagnostic copy (`64FF1643…`) of the control ISO
changed only `hal.dll` and `halmp.dll` to the RTC-fixed versions. It still
showed the COM1 banner after 220 s. A saved Hyper-V CPU state put the
instruction pointer at `kdcom.dll+0x21d2`, with the matching PDB resolving
it to `Cp16550ReadLsr+0x52`. The stack contained `Uart16550GetByte`,
`KdpReceiveByte` and `KdpReceivePacketLeader`. This identifies an actual
wait for serial debugger input; it does not establish an infinite loop.

[KdAutoProbe.patch](KdAutoProbe.patch) bounds discovery before the first debugger acknowledgement. Connected retries and exception packets keep their previous retry policy. With only KD changed, the same fixture passed the serial wait and reached the [separate RTC wait](../RtcPeriodic/README.md).
