# FastFAT requests a clean volume after a filesystem flush returns an error

**Decision: restore the Microsoft-derived callers; retire [`fastfat-clean-after-flush.patch`](Assets/fastfat-clean-after-flush.patch).** The patch added an `NT_SUCCESS` test that is absent from the Microsoft sample and both Windows driver images examined. Removing it restores the two files from the ReactOS upstream pin. This resolves the caller-branch difference. The origin and on-disk effects of a real failed write remain open.

This decision concerns an **already returned filesystem error**. In the [separate FAT32 missed-flush incident](../FAT32-Volume-Flush/README.md), CC lost a dirty range record and the filesystem returned **success**. Testing that status in FastFAT could not fix the missed flush.

## Where the decision comes from

ReactOS [records](https://github.com/reactos/reactos/blob/4fbbefd614b/media/doc/3rd%20Party%20Files.txt#L117-L121) its FastFAT as a port of Microsoft's driver sample at `16022c5`. The two paths in question are present in the sample:

| Path | Microsoft sample | Pinned ReactOS, before the guard |
|---|---|---|
| Explicit volume flush | [`FatCommonFlushBuffers`](https://github.com/microsoft/Windows-driver-samples/blob/16022c5/filesys/fastfat/flush.c#L400-L425) saves the return status. In the dirty-volume branch it tests whether the volume was *mounted dirty*, then marks it clean and clears the in-memory dirty flag. | [Same decision](https://github.com/reactos/reactos/blob/4fbbefd614b/drivers/filesystems/fastfat/flush.c#L405-L429); the lower-device flush is [sent afterwards](https://github.com/reactos/reactos/blob/4fbbefd614b/drivers/filesystems/fastfat/flush.c#L486-L504). |
| Shutdown | [`FatCommonShutdown`](https://github.com/microsoft/Windows-driver-samples/blob/16022c5/filesys/fastfat/shutdown.c#L213-L230) discards the returned status. If not mounted dirty it requests a purge and a clean mark. An exception skips those statements. | [Same distinction between a returned error and an exception](https://github.com/reactos/reactos/blob/4fbbefd614b/drivers/filesystems/fastfat/shutdown.c#L216-L239). |

The sample is not the driver binary shipped in Windows. Two binary inspections independently confirmed the decision points:

- [IDA MCP, `fastfat.sys` **10.0.26100.9974**](Evidence/ida-callers.txt): the decompiler and assembly agree. `FatFlushVolume` retains a negative directory- or FAT-flush status. The explicit caller saves it, tests the volume flags (`0x4` dirty; `0x10` mounted dirty), calls `FatMarkVolume` and clears the dirty flag on the selected branch. Shutdown overwrites the return register before testing `0x10`. An [earlier CDB excerpt](Evidence/windows-shutdown.txt) shows the same shutdown instructions.
- [WinDbg MCP, loaded guest driver](Evidence/windbg-loaded.txt): in the reference guest with kernel **10.0.26100.9457**, matching `fastfat.pdb/1A39B2959A3640F0FAFF75249C9ED9241` identified the module. `FatCommonFlushBuffers+0x1c4` calls `FatFlushVolume`, saves `eax`, then reads the flags. At `FatCommonShutdown+0x116` the call is followed by `mov eax,[r14]`, not a status test. The child flush uses signed conditional moves to propagate either error. These are reads of loaded code; no failure was forced or observed at those instructions.

## What the controlled comparison establishes

The [host test](../../../Tests/ReactOS/Host/fat.c) runs the production volume and shutdown callers. The original ten cases substituted the result of `FatFlushVolume` directly. The additional `fat-clean-chain` cases keep the [production `FatFlushVolume`](https://github.com/reactos/reactos/blob/4fbbefd614b/drivers/filesystems/fastfat/flush.c#L989-L1036) and return `STATUS_IO_DEVICE_ERROR` from either its directory flush or its FAT flush. Both child operations are controlled. The comparison uses the same pinned sources (`4fbbefd614b`), first untouched and then with **only** the FastFAT guard applied.

| Input on a dirty volume not mounted dirty | Stock result | With the guard |
|---|---|---|
| Directory or FAT flush returns an error; lower-device flush succeeds | Explicit caller receives `c0000185`, but `FatMarkVolume(VolumeClean)` is called and the in-memory dirty flag is cleared. | Same caller error, no clean-mark call, dirty flag retained. |
| Same returned error during shutdown | Shutdown returns success as before, but requests a purge and a clean mark. | Shutdown still notifies the device and returns success; neither purge nor clean mark is requested. |
| Filesystem flush succeeds; **only the later device flush fails** | Explicit caller receives `c0000185`; the clean-mark call has already occurred. | Identical. This guard acts before the later error is known. |

Scored against the **old guard's policy**, the direct fixture went **6/10 → 10/10** and the child-error fixture **4/8 → 8/8** on x86 and x64. [Original comparison, including success, mounted-dirty and raised-error controls](Evidence/host-control.txt). These scores measured the guard, not Windows compatibility. With the **Windows caller branches** as the expectation, the upstream callers pass **10/10 and 8/8**; adding the guard makes the returned-error controls fail. [Paired results](Evidence/reference-policy.txt). `mark` and `purge` count requests; `dirty` is the in-memory flag. These are host tests, not disk readback.

From an MSVC developer prompt, use `python Tests/ReactOS/Host/run.py Upstream/ReactOS/Source --stock --suite fat --suite fat-clean-chain` for the source reference. To isolate the counterexample, export the pinned `drivers/filesystems/fastfat/{flush.c,shutdown.c}` into an empty directory with the same relative layout, apply the archived patch there, and replace `--stock` with `--overlay <directory>`. Add `--expect-clean-guard` only to compare against the historical policy.

## Updating an existing checkout

A fresh checkout needs no extra step. If an existing ReactOS checkout still contains the old guard, first run the reverse check from the repository root:

```bat
git -C Upstream/ReactOS/Source apply -R --check ../../../Docs/Notes/FastFAT-Clean-After-Flush-Error/Assets/fastfat-clean-after-flush.patch
```

If the check succeeds and you intend to remove that guard, repeat without `--check`, then configure and rebuild. Inspect local changes if it fails. The clean default queue must leave `drivers/filesystems/fastfat/flush.c` and `shutdown.c` identical to the pinned upstream files.

The [applied-tree check](Evidence/source-trees.json) found exactly those two changes against the previous x64 default; the new tree is `7dd33a86d33f5ae74d7a8cb9ccc46e92592eaec4`. All four queue configurations preserve the two upstream files. The changed units were recompiled and the [x64 Debug and Win32 Release FastFAT drivers linked](Evidence/linked-drivers.json). The earlier installed-guest BootCD results belong to the earlier tree.

### Scope of the source match

The restored explicit caller uses the sample's `ClearFlag` after the clean-mark call; both Windows binaries show a `lock and` at that point. The comparison establishes which branch is taken for a given set of flags. It does not establish how every concurrent writer of the volume flags is synchronized across Windows versions and the NT 5.2 port. No private Windows layout, lock offset or decompiler variable was copied into the driver.

## What would settle the root cause?

The missing status test explains **why ReactOS requested a clean mark once given this returned error**. It does not explain the first failed write, nor prove that Windows handles that write by returning the same error to FastFAT. In particular, a lower layer that wrongly reports success needs its own [MM/CC error-propagation fix](../MM-Flush-Write-Errors/README.md).

1. Compare an initially clean, then dirtied, disposable FAT32 volume on Windows and ReactOS. Separate a failed metadata write during the filesystem flush from a failure of the later device flush. A read-only volume or a normal successful dismount is not this test.
2. Record the first failing paging I/O, CC/MM's status and completed bytes, the return **or exception** from `FatFlushVolume`, volume flags at the decision, clean-mark call and lower completion. Record the explicit caller's status. Test shutdown separately.
3. Take a read-only disk copy at the decision boundary and inspect it before a remount or delayed cleanup can change it. Compare the on-disk clean/dirty state, both FAT copies and the data after retry; an in-memory flag alone is insufficient.

The two clean-state callers now follow the reference and the extra status check is outside the queue. If the paired run finds an earlier divergence (for example CC returning success for failed paging I/O), fix the **first** differing status, exception or flag transition. If an architectural constraint really prevents alignment, record the exact Windows branch, user-visible effect, test and removal condition before introducing a local exception. The original installation disk does not establish any of those differences.
