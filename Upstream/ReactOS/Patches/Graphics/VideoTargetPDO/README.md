# Video port returns the adapter FDO for a target-device relation

**Open: the returned object was observed; no replacement is selected.** On the faulty x64 installation, the VGA adapter had both a functional device object (FDO) and a distinct physical device object (PDO). After [making delivery of the PnP result reliable](../MonitorTargetRelation/README.md), its single returned object was still the FDO. The consumer uses the result to open a device registry key.

The [pinned dispatch function](https://github.com/reactos/reactos/blob/4fbbefd614b/win32ss/drivers/videoprt/dispatch.c#L1125) and [host control](Assets/Video.c) distinguish an attached adapter from a legacy adapter with no PDO. The [WDK target-relation contract](https://learn.microsoft.com/en-us/windows-hardware/drivers/kernel/irp-mn-query-device-relations) is about returning a referenced child PDO. Blindly returning the physical-object field would be invalid for the legacy case and could enable bus re-enumeration on the primary display.

Before selecting a change, compare the returned object and bus invalidation for an attached primary adapter, a secondary adapter and a legacy adapter without a PDO, through re-enumeration. This is about **which object** is returned; it did not explain the earlier attempt to read the relationship before completion.

Run the [host case](Assets/Run.cmd) from a native VS developer prompt. Pass `--stock` to exercise the pinned source.
