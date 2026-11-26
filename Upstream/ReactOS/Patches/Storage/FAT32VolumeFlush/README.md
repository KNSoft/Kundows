# FAT32 volume flush returned success with both allocation tables still stale

`FlushFileBuffers` returned success on a ReactOS FAT32 volume, but an immediate read from disk found that neither copy of the FAT contained the new allocation chain. FastFAT had asked the cache manager (CC) whether the range already had a buffer record. The data were still dirty, but CC had dropped the record. Keeping it fixed the disk contents but prevented the cache from reclaiming its mapping. Fixing that lifetime problem, in turn, exposed a truncation bug: keeping a dirty prefix could leave an unrelated tail in the cache.

The accepted fix, [`CcBcbOwnership.patch`](CcBcbOwnership.patch), lets FastFAT use its original scan. On a fresh x64 ReactOS installation, the FAT tests matched Windows and the same kernel driver reported all **28 size and close fields** identically. The investigation below explains why earlier candidates passed some of those tests yet still had to change.

## 1. Why a successful volume flush left both FATs stale

The test allocated a file, flushed the volume, and read a sector from each FAT **as soon as the API returned**. Both reads delivered 512 bytes; all 128 entries in each sector were still zero. A later write or a clean shutdown would have hidden the failure. The result established a missed flush on this fixture; it did not establish the cause of every discrepancy on the original installation disk.

### The retired FastFAT workaround

[`FastFATVolumeDirtyRuns.patch`](Assets/History/FastFATVolumeDirtyRuns.patch) left FastFAT's normal scan, `FatFlushFat`, intact. It changed **only the whole-volume caller, `FatFlushVolume`**. After the scan succeeded, the caller took the first remaining dirty range from `Vcb->DirtyFatMcb` and called `CcFlushCache` for that range. The paging write that updated the FAT copies removed the range; requesting the write did not clear it. The caller then checked the list again. An empty list needed no further I/O. A cache error was propagated; a zero-length range or one starting before the previous request's end returned `STATUS_DEVICE_BUSY` rather than spinning under the volume lock or reporting success.

In the [sector 544 before/after trace](Assets/FlushBoundary.tsv), the original pin lookup still missed with the workaround. What changed was the timing: **both mirror writes completed before the volume flush returned**, and both immediate reads matched the pinned allocation data. The attachment preserves the tested workaround's retired file byte for byte (SHA-256 `bfc120f76e249f8493ec03bc8a9aa5de64d635c94228153ca865793e2faef811`).

### Why the fix belongs in CC

ReactOS uses a port of **Microsoft's official [FastFAT driver sample](https://github.com/microsoft/Windows-driver-samples/blob/16022c5/filesys/fastfat/flush.c#L794)**. Its [third-party inventory](https://github.com/reactos/reactos/blob/4fbbefd614b/media/doc/3rd%20Party%20Files.txt#L117-L121) names that revision; ReactOS [imported it for NT 5.2](https://github.com/reactos/reactos/commit/0daa5547d98) and later [selected it as its FAT driver](https://github.com/reactos/reactos/commit/23b7c7b8234). The [ReactOS FAT12/16/32 pin scan](https://github.com/reactos/reactos/blob/4fbbefd614b/drivers/filesystems/fastfat/flush.c#L801-L939) matches the sample apart from whitespace and exception-handling macro spelling. This is a source comparison, not a claim that the port is the binary shipped in Windows. For this volume metadata stream, the scan assumes that a range without a buffer control block (BCB) has already been written. In general, [`PIN_IF_BCB`](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-ccpinread) asks only whether a record for the range exists.

The [ReactOS trace](Assets/CcLifetime.tsv) followed the same page from `CcUnpinRepinnedBcb(FALSE)` to that lookup. Releasing the last caller reference unlinked the BCB while the page was still dirty. Four runs used the **same `fastfat.sys` with the original scan and the same `fatflush.exe`**; only the kernel's CC behavior on the watched page changed:

| CC change | What the scan found | FAT 1 at return | FAT 2 at return |
|---|---|---:|---:|
| Neither (tracing only) | The record was gone. | 0/128 | 0/128 |
| Find only: allow lookup at zero pins | The record was already gone. | 0/128 | 0/128 |
| Keep only: retain the dirty record | The record covered the page, but lookup still excluded it. | 0/128 | 0/128 |
| Keep + Find | The original FastFAT scan found and flushed the page. | **128/128** | **128/128** |

[All four flush calls returned success](Assets/CcLifetimeCausal.tsv); only the last returned with the new entries on disk. That is why the accepted fix changes CC's record lifetime and lookup instead of keeping a FastFAT-only workaround. This result does not explain the [original disk's one-sided loss of 29 entries](../FAT32InstallMirrorDivergence/README.md).

## 2. Why keeping the dirty record was not enough

| ReactOS candidate | What the next test showed | Decision |
|---|---|---|
| Keep the dirty BCB and let `PIN_IF_BCB` find it after the last unpin. | Dirty lookups went from **0/5 to 5/5**, but successful-write reclamation went from **7/7 to 0/7**. The idle BCB still held the cache view (VACB), so even a clean view could not be purged. [Paired guest results](Assets/CcCycle.tsv) | Separate the lifetime of the dirty record from the lifetime of its active mapping. |
| Retire idle records on writeback; discard them before an ordinary purge. | The first 39 checks passed. A new case kept a second page pinned: the purge failed, but had already forgotten the first page's pending record. The candidate scored **43/44**. | Retire on successful writeback, preserve pending records on failure, and reserve final discard for a quiescent cache map. The same test then passed **44/44**. [Candidate comparison](Assets/CcOwnership.tsv) |
| Track generations and reserve a whole view during writeback. | The V3 ReactOS guest passed **54/54**, and the original FAT tests passed without the FastFAT fallback. One check expected `STATUS_DEVICE_BUSY` on a writeback collision. That demonstrated the candidate's behavior, not what Windows promises. [Per-case results](Assets/CcOwnership.tsv) | Commit `79db84f` retired the FastFAT fallback. Measure Windows before accepting the synchronization rules. |

### Windows lifecycle cases

The [observer's case file, `ProbeCases.inc`](Assets/ProbeCases.inc), contains `RunCase`. Its bytes match lines 256–411 of V4b's `Probe2/ccobserve.c` ([archived source identity](Assets/CcTruncate/WindowsReference.json#L66-L70)); the original V4 observer has the same case bodies. The numbers below belong to this Windows observer, not to ReactOS V3's 54 checks or the later [size/close driver](Tests/SizeDdi.c).

The observer used its own metadata stream. `Hit` tried to pin the watched page with `PIN_IF_BCB`; `Flush` requested a writeback of that one 4096-byte page. Write-behind was enabled for cases 1–4, with the observer's lazy-write callback initially held off to control timing. Guest output uses `VALUE <case> <field> <hex value>`; each field number is the second argument to `Value` in the linked code.

| Case (source) | Operation and observation point | Windows `10.0.26100.9457` record |
|---|---|---|
| [1 — idle-dirty](Assets/ProbeCases.inc#L34-L45) | Dirty the page with `0x51`, release the last pin, inspect the BCB (phase 101), then check lookup, flush, backing byte and lookup again. | [WinDbg stop](Assets/WinDbgCase1.txt); [V4 and V4b guest logs](Assets/GuestValues.txt#L6-L36). V4 timed out during close; V4b completed. |
| [2 — write-error-retry](Assets/ProbeCases.inc#L46-L63) | Repin the `0x52` page; fail the write-through unpin. Inspect the error, backing byte and remaining record (phase 201), then allow a retry. | Defined; no Windows run result in this archive. |
| [3 — lazy-explicit-overlap](Assets/ProbeCases.inc#L64-L95) | Unpin the `0x53` page, allow a lazy write and hold its paging request. If it arrives, start an explicit flush and inspect the worker before releasing the write (phase 301). | [Guest log](Assets/GuestValues.txt#L38-L47): the 15-second wait expired with zero writes. The explicit worker was never started. |
| [4 — pin-versus-flush](Assets/ProbeCases.inc#L96-L122) | Keep the `0x54` page pinned; start an explicit flush on another thread. After a 200 ms wait, record worker state and writes (phase 401). Change the page to `0x64`, unpin, then read the result. | [Sampled stack and final debugger reply](Assets/WinDbgCase4.txt); [recovered guest log](Assets/GuestValues.txt#L49-L69). |
| [5 — disabled-writebehind](Assets/ProbeCases.inc#L96-L122) | Same two threads and measurements as case 4, but [disable write-behind](Assets/ProbeCases.inc#L29-L33) before pinning (phase 501). | Defined; no Windows run result in this archive. |

### What the debugger showed

**Case 1: a dirty record outlived its mapping.** At phase 101, before the next lookup, WinDbg on the `.9457` guest returned these [BCB fields](Assets/WinDbgCase1.txt#L11-L33) (selected lines):

```text
   +0x002 Dirty            : 0x1 ''
   +0x038 Vacb             : (null)
   +0x040 PinCount         : 0
   +0x0b8 BaseAddress      : (null)
```

The next lookup hit. The [guest values](Assets/GuestValues.txt#L26-L36) show a successful 4096-byte flush, backing byte `0x51`, then a lookup miss. These measurements repeated when V4b fixed the observer's close timeout. Separately, [IDA's `CcUnpinFileDataEx` output](Assets/IDACc.txt#L6-L24) from kernel **`10.0.26100.9974`** shows a zero-pin dirty path that clears the two mapping pointers and returns before unlinking. ReactOS therefore kept the dirty range record, dropped its idle mapping reference and reacquired the mapping on a later hit.

**Case 4: the explicit flush waited for the writer.** A caller can mark a buffer dirty *before* storing its final bytes. On the ReactOS host fixture, a flush wrote the old `0x41` while that caller still held its pin. On Windows, the [WinDbg sample](Assets/WinDbgCase4.txt#L8-L37) showed `Writes=0` and a worker in `WAIT: (WrResource)`, inside `ExAcquireResourceExclusiveLite` called by `CcAcquireByteRangeForWrite`. The waited-on resource was at the watched BCB's `+0x48`. [IDA's static disassembly](Assets/IDACc.txt#L26-L42) shows this acquisition is conditional; the observation does not establish what happens with write-behind disabled.

The [final debugger reply](Assets/WinDbgCase4.txt#L39-L43) was `Error: Unable to get command output`. The [recovered guest output](Assets/GuestValues.txt#L53-L69) records both sides of the wait: `VALUE 4 6 00000000` is the write **count** before release. After the caller stored `0x64` and unpinned, field 8 was the worker's success status, field 9 was the backing byte `0x64`, and the `END` line counted one write in total. The host wrapper timed out; the original controller process exit code was not recovered.

ReactOS changed from whole-view ownership to tracking pending **pages** for the affected BCB ranges. The explicit flush takes the relevant resource, checks ownership again, and hands the selected page to MM. Successful pages can retire; failed pages remain retryable. The [two-thread regression](Tests/Host/HandoffContract.py) observed no write before release and a full page of `0x64` afterwards. A related fix preserved the completed byte count on error: if the first page was written and the second failed, the caller must see the error **and 4096 bytes** of progress. This work did not establish Windows' behavior for every write-behind mode or MM collision.

## 3. How a purge fix became a size and close fix

1. **Protecting a whole view was too broad.** A stream had a dirty page before the new end of file (EOF) and a separate clean view after it. A guard saw the pending BCB in the boundary view and skipped invalidation of the entire suffix, including the clean view and its MM pages. Simply removing the guard risked losing the valid dirty prefix. The first [range-aware repair](Assets/CcTruncate/CcTruncateDelta.patch), based on tree `be9f26ff`, clipped crossing idle records, retained valid bytes in a partial page, and walked allocated page-table slices to invalidate even distant cached pages. It passed **16/16** host checks on both architectures.
2. **The first repair implemented the wrong public contract.** It triggered on allocation shrink, raised an exception from the legacy size entry on purge failure, and could veto later growth. [Both Windows kernels](Assets/CcTruncate/ResizeAbiByBuild.json) showed that `CcSetFileSizes` calls `CcSetFileSizesEx` and returns without inspecting its status. The [four wrapper instructions and Ex branches in IDA](Assets/IDACc.txt#L44-L70) distinguish a returned purge error from an exception during section extension. The implementation now uses EOF as the tail boundary and allocation size for extension. A busy CC view can defer cleanup without failing the notification; an MM purge refusal on the measured shrink path returns `STATUS_PURGE_FAILED`. An unexpected extension error is raised as `STATUS_UNEXPECTED_MM_EXTEND_ERR`.
3. **Ordinary file tests did not settle close-event timing.** The next [candidate, `2eef1040`](Assets/CcTruncate/Default9457/ReactOSFatGuest.json), allowed direct regrowth and queued close events until final teardown. It passed **25/25** host checks and matched Windows on the file and FAT programs. But closing a handle did not tell us when CC would signal a *supplied kernel event*. The direct test below disproved the blanket delay: removing one private cache map must notify that closer if another private map remains. Conversely, if only internal work remains, waiters can stay queued for cleanup or cache-map reuse. A clean last close with an event can finish synchronously.
4. **The valid part of the last page still matters.** An intermediate ReactOS build (`b3db2236`) returned `STATUS_PURGE_FAILED` when only valid data length (VDL) changed and a read-only mapping covered the last valid byte. Windows succeeded. For a data section without a shared cache map, ReactOS now flushes the partial boundary page and purges only subsequent whole pages. In the opposite case, a live section can still prevent shortening even if its resident pages are all *below* the proposed EOF. [Before/after control and Windows measurements](Assets/CcTruncate/Native9457/WindowsDdi.json)

## 4. What callers actually observed

The [ordinary file program](Tests/FileSizeContract.c) used two handles to a real file. The [kernel test driver](Tests/SizeDdi.c) used its own zero-filled stream, called CC directly, and read each close event immediately after the call returned. In its shrink test the driver proposed a smaller EOF without changing the backing file. The distinction matters: the filesystem and CC reject different operations, with different statuses.

| Boundary and operation | Windows observation matched by the installed ReactOS guest |
|---|---|
| File API: shorten from 6144 to 4219 bytes | Allocation stayed at 8192. The second handle saw the new EOF immediately and read the correct 4219-byte prefix. |
| File API: request a further shrink with a read-only mapping | The filesystem returned `0xc0000243` (`STATUS_USER_MAPPED_FILE`); EOF stayed at 4219. After unmapping, direct growth to 14336 succeeded. The peer could still read and shorten the file after the first handle closed. Reopening confirmed EOF 4219 and the original prefix. |
| CC: keep a 4096-byte prefix mapped and propose EOF 8192 | `CcSetFileSizesEx` returned `0xc0000435` (`STATUS_PURGE_FAILED`) both with and without a shared cache map. With a shared map, its **visible cached EOF was already 8192** on return. |
| CC: release the mapping and section, then grow without repeating the shrink | Both calls returned `STATUS_SUCCESS`. In the shared-map case the resulting cached EOF was 73605. |
| CC: close the first of two file objects with private cache maps | `CcUninitializeCacheMap` returned **`FALSE`, event signalled**. The peer still had its private map. |
| CC: close the last clean private map with an event | **`TRUE`, event signalled**. The boolean reports synchronous final teardown in this case; neither the event nor the boolean is a disk I/O status. |

The ordinary-file reference ran on Windows [NTFS](Assets/CcTruncate/Default9457/GuestNtfs.json) and [FAT32](Assets/CcTruncate/Default9457/GuestFatFileSizeContract.json); the guest-to-guest filesystem comparison used FAT32. For the kernel test, a separate WinDbg run captured **34 observations in order** at the test driver's reporting point after the calls: [recorded breakpoint and output](Assets/WinDbgSizeDdi.txt), [values as JSON](Assets/WinDbgTrace.json). All **28 distinct fields** matched the Windows driver record and the ReactOS guest record.

## 5. Final acceptance

Commit `c369b1c` contains the validated repair on upstream ReactOS `4fbbefd614b`. The default x64 queue **at that commit** produced source tree `bd620b9b5a8c386e1bc981726bed3943425e4b74`. The [build record](Assets/CcTruncate/Native9457/Checks.json) identifies the patch **including its preamble at that commit**. The clean x64 Debug solution built **1419 projects, 0 failed**; the BootCD was built separately.

The x64 Hyper-V guest was installed from that CD onto a blank 6 GiB disk. After shutdown, the installed MP kernel and `fastfat.sys` were matched to the linked binaries by content. The guest then cold-booted from disk with the installation media removed and a fresh 512 MiB FAT32 test disk attached. [Installed identities, DDI results and program output](Assets/CcTruncate/Native9457/ReactOSGuest.json)

| Check | Result |
|---|---|
| Kernel drivers, identical binaries in both guests | All 28 main fields matched, including both purge refusals and both subsequent growths. The independent last-page/VDL control also matched. |
| Ordinary file, volume flush, lock/dismount/remount | All three programs exited 0 with line-for-line identical output. At flush return both FAT copies had **128/128** correct entries. Three dismount/remount cycles verified **24 MiB** byte for byte. |
| Host and build coverage | Size/close **34/34** on each of x86 and x64; writeback, BCB and FAT regressions passed. x86 Release UP/MP kernels linked. The installed-guest results are x64. All four patch queue configurations applied and reversed. |

The accepted image is `OutDir/bootcd-x64-Debug.iso`, SHA-256 `2bffbc71fc69e84ccd1fc0c38e0150c44f2aa4709848de3bf6fb26f839508bc8`. To compare the **saved** Windows, WinDbg and ReactOS results from the repository root:

```text
python Upstream/ReactOS/Patches/Storage/FAT32VolumeFlush/Tests/CompareSizeRuns.py
```

## Running the tests

- **Volume API:** from a native MSVC prompt, use [`Volume/Build.cmd`](Tests/Volume/Build.cmd) for `fatflush.exe` and [`Volume/BuildCycle.cmd`](Tests/Volume/BuildCycle.cmd) for `fatcycle.exe`. Pass an output directory or use the default `OutDir/FatVolume-<arch>-Debug/`. [`NewFatFixture.ps1`](Tests/Volume/NewFatFixture.ps1) creates a fresh 512 MiB FAT32 disk with a 1 MiB partition offset, 4 KiB clusters and the fixture label. Give each system a separate copy. `fatflush.exe X:` checks both FAT copies **immediately after the flush returns**; [the paired results](Assets/FatFlush.tsv) include the reads before and after. `fatcycle.exe X:` separately verifies three lock/dismount/remount cycles, 8 MiB each. Use the disposable fixture volume, not a system drive.
- **ReactOS kernel lifecycle:** [`Lifecycle/Build.cmd`](Tests/Lifecycle/Build.cmd) builds the guest-only synthetic-stream driver and controller using `cl`, `link` and the matching WDK in the developer prompt. It does not attach to a storage device. The original V1 tool ran eight conditions/39 checks; later versions added the failed-purge and overlap cases. [`Summarize.py`](Tests/Lifecycle/Summarize.py) checks that the controller log is complete before scoring it. See [V1 results](Assets/CcCycle.tsv) and [candidate results](Assets/CcOwnership.tsv). These are not the Windows observer's case numbers.
- **Windows observer:** start with the [case definitions and execution table](#windows-lifecycle-cases) before reading the debugger output.
- **Size and close:** [`BuildSizeCases.cmd`](Tests/BuildSizeCases.cmd) builds [`FileSizeContract.c`](Tests/FileSizeContract.c) (two ordinary file handles) and [`SizeDdi.c`](Tests/SizeDdi.c) (direct CC calls and supplied kernel events). Newly built binaries need their own guest run. [CompareSizeRuns.py](Tests/CompareSizeRuns.py) compares the archived driver identities, DDI values and complete program output.
- **Host C cases:** [Tests/Run.cmd](Tests/Run.cmd) checks ownership, volume flush and truncation with controlled I/O. The [two-thread handoff](Tests/Host/HandoffContract.py) measures a pin blocking writeback. The trace readers for the saved guests are [ReadBoundary.py](Tests/Trace/ReadBoundary.py) and [ReadCcLifetime.py](Tests/Trace/ReadCcLifetime.py).
- **Historical negative control:** [MetadataWriteback/Run.py](Tests/Host/MetadataWriteback/Run.py) reconstructs the earlier `79db84f` implementation. The held-pin check returns a failure there: the backing byte is `0x41`, not `0x64`. `--stage2 <dir>` applies the two hash-checked historical deltas; the current ABI uses the handoff runner above.

## Remaining differences and when to refactor

| Difference or open question | Reason and caller impact |
|---|---|
| ReactOS tracks an unfinished suffix (`TruncatePending`). | Its MM can purge some pages before finding one in use. Retrying only beyond the filesystem's new VDL protects newly initialized bytes. Pin-access streams also retire obsolete **idle** metadata records. This may require an extra cleanup pass; old work must neither discard a live pin nor veto valid growth. The field is a ReactOS implementation choice, not an observed Windows mechanism. |
| A close worker cannot be allocated. | ReactOS falls back to synchronous cleanup. The closer can wait longer and find its event already signalled on return. A host test exercised this fallback; the corresponding Windows low-memory timing has **not** been measured. |
| A lazy write overlaps an explicit flush, or storage fails during concurrent cleanup. | The Windows observer did not establish the intended lazy-write overlap. Static evidence distinguishes waiting from `STATUS_ENCOUNTERED_WRITE_IN_PROGRESS` on a nonwaiting MM path; it does not justify a blanket `STATUS_DEVICE_BUSY` or an unconditional wait while the filesystem holds a resource. Failure/retry host checks do not settle every Windows scheduling case. |

**Decision rule:** follow the reference source and verified Windows branches as closely as the port permits. Check the result at the caller: status versus exception, visible EOF and data, and event state at return and at completion. Fix a discrepancy locally when one return path or notification condition explains it. If truncation, writeback and close repeatedly disagree because they cannot tell who owns a page or when I/O has finished, change that shared CC/MM rule and retest all three paths together. For a retained implementation difference, state the constraint, its caller impact and what remains unverified.

## Evidence trail

The early Windows log used attempt IDs W01–W34. The [case definitions and transcripts](#windows-lifecycle-cases) above are the public extracts; the text attachments identify the source files and their SHA-256 hashes. The [reference index](Assets/CcTruncate/Native9457/Reference.json) identifies the later IDA and live-code inspections.

| Attempts | Observation and limitation |
|---|---|
| W01–W04 | IDA established the idle BCB and writeback paths. Disabled write-behind changes the synchronization path. Tests that deliberately hold a pin during an ordinary purge are ReactOS defensive checks; they do not meet the [Windows purge preconditions](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-ccpurgecachesection). |
| W05–W17 | A new isolated Windows VM was identified by its disk chain, running kernel and PDB. The first debugger session had no target. Some later commands ran but their output was not delivered. Only identified stops and retained output count as observations; a `Stopped` state or a client timeout alone does not. |
| W18–W23 | Case 1's first write and purge succeeded, but the observer timed out waiting for close. A cleanup-only revision let callbacks proceed and extended the wait: the data observations repeated and the controller exited 0. [Paired records](Assets/CcTruncate/Default9457/WindowsCloseObservation.json). The FAT disk prepared at this point was not the later FAT acceptance run. |
| W24–W25 | The [debugger sample](Assets/WinDbgCase4.txt) captured the blocked worker; the [recovered guest log](Assets/GuestValues.txt#L49-L69) recorded the later write and successful completion. The host wrapper timed out and did not recover the original process exit code. |
| W26–W34 | Case 3 never reached its lazy-write trigger. During subsequent cleanup the guest stopped in MM; the sampled file object belonged to the observer, which had already cleared its stream context. This was an observer lifetime problem, not a Windows cache result. The sample and the failure to deliver the final debugger reply were retained separately. |

The early audit distinguishes reconstructed setup notes from timestamped operations in `History/attempts.jsonl`. At its recorded checkpoint, **135 records / 298 distinct snapshots** verified; the initial Windows archive matched **29/29** entries. The later ReactOS stage-one and stage-two archives matched **9/9** and **27/27** entries respectively. The original manifests are retained in the private `OutDir/Notes-Migration-abaaf68/` snapshot. These are historical integrity counts, not compatibility scores. The pre-merge working log is preserved verbatim in the local `OutDir/WinAlign-9457-01/notes-archive/5f312abf56e3de6a76895094999febebdab9e31aa7cee8eda377d1c2ddb73809.txt`.

Several side investigations stayed outside the default repair. The MM pre-wait experiment could still encounter a new write after waiting; the ticket and filesystem-cooperation alternatives were not selected. Independent FastFAT error-propagation fixes linked, but that did not validate short-BCB writeback. A sparse whole-file **flush** miss was measured only on a separate experimental x64 tree; it is distinct from the **truncation** regression fixed above. None of these results was counted toward the accepted guest.

## Superseded patch files

These three files are the historical inputs for the missed-flush investigation, including their original preambles. `Configure.cmd` does not select them.

| Historical patch | Where the repair ended up |
|---|---|
| [`CcPinnedDirty.patch`](Assets/History/CcPinnedDirty.patch) | `CcBcbOwnership.patch` incorporates dirty-intent tracking through the final unpin. |
| [`CcDuplicateBcb.patch`](../CcDuplicateBcb/Assets/History/CcDuplicateBcb.patch) | `CcBcbOwnership.patch` locks the BCB actually returned to the caller and releases the discarded duplicate. |
| [`FastFATVolumeDirtyRuns.patch`](Assets/History/FastFATVolumeDirtyRuns.patch) | Superseded by retaining dirty BCBs in CC; FastFAT no longer needs the volume fallback. |

If migrating a checkout that still has all three old patches applied, reverse-check and remove `FastFATVolumeDirtyRuns.patch`, then `CcDuplicateBcb.patch`, then `CcPinnedDirty.patch` before configuring. Stop if a reverse check fails and inspect local edits. An already migrated checkout needs none of these steps.
