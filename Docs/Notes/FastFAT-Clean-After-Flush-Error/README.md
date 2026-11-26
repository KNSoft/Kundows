# FastFAT requests a clean volume after a filesystem flush returns an error

**Status: the ReactOS control flow is verified; Windows compatibility and the right layer for a permanent fix are open.** [`fastfat-clean-after-flush.patch`](../../../Upstream/ReactOS/Patches/Runtime/fastfat-clean-after-flush.patch) is in the default queue as a defensive guard. It matters when the filesystem **already knows** that its flush failed. In the [separate FAT32 missed-flush incident](../FAT32-Volume-Flush/README.md), the filesystem returned **success** because CC had lost a dirty range record. The guard cannot distinguish that success from a real one.

## Where the decision comes from

ReactOS [records](https://github.com/reactos/reactos/blob/4fbbefd614b/media/doc/3rd%20Party%20Files.txt#L117-L121) its FastFAT as a port of Microsoft's driver sample at `16022c5`. The two paths in question are present in the sample:

| Path | Microsoft sample | Pinned ReactOS, before the guard |
|---|---|---|
| Explicit volume flush | [`FatCommonFlushBuffers`](https://github.com/microsoft/Windows-driver-samples/blob/16022c5/filesys/fastfat/flush.c#L400-L425) saves the return status. In the dirty-volume branch it tests whether the volume was *mounted dirty*, then marks it clean and clears the in-memory dirty flag. | [Same decision](https://github.com/reactos/reactos/blob/4fbbefd614b/drivers/filesystems/fastfat/flush.c#L405-L429); the lower-device flush is [sent afterwards](https://github.com/reactos/reactos/blob/4fbbefd614b/drivers/filesystems/fastfat/flush.c#L486-L504). |
| Shutdown | [`FatCommonShutdown`](https://github.com/microsoft/Windows-driver-samples/blob/16022c5/filesys/fastfat/shutdown.c#L213-L230) discards the returned status. If not mounted dirty it requests a purge and a clean mark. An exception skips those statements. | [Same distinction between a returned error and an exception](https://github.com/reactos/reactos/blob/4fbbefd614b/drivers/filesystems/fastfat/shutdown.c#L216-L239). |

The sample is not the driver binary shipped in Windows. [IDA MCP disassembly and decompilation](Evidence/ida-callers.txt) of `fastfat.sys` **10.0.26100.9974** show that `FatFlushVolume` propagates negative child statuses. The explicit caller saves that status but tests volume flags before marking clean; shutdown overwrites the return register before its flag test. An [earlier CDB read of shutdown](Evidence/windows-shutdown.txt) shows the same instructions. This does not establish which flags a *real* failed write leaves set, or what reaches the disk.

## What the controlled comparison establishes

The [host test](../../../Tests/ReactOS/Host/fat.c) runs the production volume and shutdown callers. The original ten cases substituted the result of `FatFlushVolume` directly. The additional `fat-clean-chain` cases keep the [production `FatFlushVolume`](https://github.com/reactos/reactos/blob/4fbbefd614b/drivers/filesystems/fastfat/flush.c#L989-L1036) and return `STATUS_IO_DEVICE_ERROR` from either its directory flush or its FAT flush. Both child operations are controlled. The comparison uses the same pinned sources (`4fbbefd614b`), first untouched and then with **only** the FastFAT guard applied.

| Input on a dirty volume not mounted dirty | Stock result | With the guard |
|---|---|---|
| Directory or FAT flush returns an error; lower-device flush succeeds | Explicit caller receives `c0000185`, but `FatMarkVolume(VolumeClean)` is called and the in-memory dirty flag is cleared. | Same caller error, no clean-mark call, dirty flag retained. |
| Same returned error during shutdown | Shutdown returns success as before, but requests a purge and a clean mark. | Shutdown still notifies the device and returns success; neither purge nor clean mark is requested. |
| Filesystem flush succeeds; **only the later device flush fails** | Explicit caller receives `c0000185`; the clean-mark call has already occurred. | Identical. This guard acts before the later error is known. |

On both x86 and x64 the direct fixture went **6/10 → 10/10** and the child-error fixture **4/8 → 8/8**. [Captured output, including success, mounted-dirty and raised-error controls](Evidence/host-control.txt). `mark` and `purge` count requests; `dirty` is an in-memory flag. No disk or Windows guest was involved.

From an MSVC developer prompt, use `python Tests/ReactOS/Host/run.py Upstream/ReactOS/Source --stock --suite fat --suite fat-clean-chain` for the negative control. To isolate the patch, export the pinned `drivers/filesystems/fastfat/{flush.c,shutdown.c}` into an empty directory with the same relative layout, apply the patch there, and replace `--stock` with `--overlay <directory>`.

## What would settle the root cause?

The missing status test explains **why ReactOS requested a clean mark once given this returned error**. It does not explain the first failed write, nor prove that Windows handles that write by returning the same error to FastFAT. In particular, a lower layer that wrongly reports success needs its own [MM/CC error-propagation fix](../MM-Flush-Write-Errors/README.md).

1. Compare an initially clean, then dirtied, disposable FAT32 volume on Windows and ReactOS. Separate a failed metadata write during the filesystem flush from a failure of the later device flush. A read-only volume or a normal successful dismount is not this test.
2. Record the first failing paging I/O, CC/MM's status and completed bytes, the return **or exception** from `FatFlushVolume`, volume flags at the decision, clean-mark call and lower completion. Record the explicit caller's status. Test shutdown separately.
3. Take a read-only disk copy at the decision boundary and inspect it before a remount or delayed cleanup can change it. Compare the on-disk clean/dirty state, both FAT copies and the data after retry; an in-memory flag alone is insufficient.

If the first difference is below FastFAT, fix it there and rerun with the original Microsoft-derived scan **and clean-state callers**. Retire the guard if those callers then match Windows and the failed-write regression stays covered. If the same failure really reaches equivalent callers but ReactOS still needs a local safeguard, document the observable difference and the reason for keeping it. The original installation disk is not evidence for either decision.
