# ReactOS cache manager investigation: from a missed FAT32 flush to file truncation and close

`FlushFileBuffers` returned success on a ReactOS FAT32 volume, but an immediate read from disk found that neither copy of the FAT contained the new allocation chain. FastFAT had asked the cache manager (CC) whether the range already had a buffer record. The data were still dirty, but CC had dropped the record. Keeping it fixed the disk contents but prevented the cache from reclaiming its mapping. Fixing that lifetime problem, in turn, exposed a truncation bug: keeping a dirty prefix could leave an unrelated tail in the cache.

The [accepted CC patch](../../../../Upstream/ReactOS/Patches/Runtime/ReactOS-cc-bcb-ownership.patch) fixes these paths without a whole-volume FastFAT workaround. On a fresh x64 ReactOS installation, the FAT tests matched Windows and the same kernel driver reported all **28 size and close fields** identically. The investigation below explains why earlier candidates passed some of those tests yet still had to change.

## 1. Why a successful volume flush left both FATs stale

The test allocated a file, flushed the volume, and read a sector from each FAT **as soon as the API returned**. Both reads delivered 512 bytes; all 128 entries in each sector were still zero. A later write or a clean shutdown would have hidden the failure. The result established a missed flush on this fixture; it did not establish the cause of every discrepancy on the original installation disk.

A ReactOS trace followed the same page through its last `CcUnpinRepinnedBcb(FALSE)` and FastFAT's next `PIN_IF_BCB` lookup. The buffer control block (BCB), which identifies a cached byte range, was removed when its last caller reference went away, although the data was still dirty. FastFAT consequently skipped the page. With the same FastFAT binary and test executable, changing **only** the lookup failed because the record was already gone; changing **only** retention failed because lookup still excluded a zero-pin record. Changing both made both FATs read **128/128**. This isolated the fault to the CC lifetime and lookup rules. [Trace and four-way comparison](../FatTrace/CcLifetime.md)

An earlier workaround, **C2**, flushed residual dirty FAT runs after the normal whole-volume scan. It made the flush effective, but only in that filesystem path. Once the unchanged FastFAT scan worked with a CC-only change, the repair belonged in CC. The separate rule that a failed volume flush must not mark the volume clean (**F1**) still applied.

## 2. Why keeping the dirty record was not enough

| ReactOS candidate | What the next test showed | Decision |
|---|---|---|
| Keep the dirty BCB and let `PIN_IF_BCB` find it after the last unpin. | Dirty lookups went from **0/5 to 5/5**, but successful-write reclamation went from **7/7 to 0/7**. The idle BCB still held the cache view (VACB), so even a clean view could not be purged. [Paired guest results](../KernelTests/Results.md) | Separate the lifetime of the dirty record from the lifetime of its active mapping. |
| Retire idle records on writeback; discard them before an ordinary purge. | The first 39 checks passed. A new case kept a second page pinned: the purge failed, but had already forgotten the first page's pending record. The candidate scored **43/44**. | Retire on successful writeback, preserve pending records on failure, and reserve final discard for a quiescent cache map. The same test then passed **44/44**. [Candidate comparison](CcOwnership.md) |
| Track generations and reserve a whole view during writeback. | The V3 ReactOS guest passed **54/54**, and the original FAT tests passed without the FastFAT fallback. One check expected `STATUS_DEVICE_BUSY` on a writeback collision. That demonstrated the candidate's behavior, not what Windows promises. [Per-case results](../KernelTests/CcOwnership.tsv) | Commit `79db84f` retired the FastFAT fallback. Keep the independent MM write-error and FAT clean-state fixes; measure Windows before accepting the synchronization rules. |

**What the Windows reference showed.** IDA MCP inspected kernel `10.0.26100.9974`; WinDbg inspected the loaded `10.0.26100.9457` kernel in a separate Hyper-V guest with matching symbols. In the guest's idle-dirty test (Case 1), `Dirty=1` and `PinCount=0`, but both the VACB and base address were null. The next lookup found the record. The flush reported 4096 bytes, the backing byte read `0x51`, and a subsequent lookup missed. ReactOS therefore kept the range record, dropped its idle mapping reference, and reacquired the mapping on a later hit. [Reference identities and guest reports](Evidence/CcTruncate/windows-reference.json)

The next question was **when** CC could publish the data to the memory manager (MM). A caller can mark a buffer dirty *before* storing its new bytes. On the ReactOS host fixture, a flush wrote the old `0x41` while that caller still held its pin. On a normal Windows metadata stream (Case 4), the explicit flush worker waited for the BCB resource and had issued **zero writes** at the observation point. After unpin, it wrote `0x64` once. The wait stack was captured in a debugger sample; the debugger's final reply timed out. The completed write comes from the recovered guest log, not from treating the timeout as a pass.

ReactOS changed from whole-view ownership to tracking pending **pages** for the affected BCB ranges. The explicit flush takes the relevant resource, checks ownership again, and hands the selected page to MM. Successful pages can retire; failed pages remain retryable. The [two-thread regression](Checks/handoff-contract.py) observed no write before release and a full page of `0x64` afterwards. A related fix preserved the completed byte count on error: if the first page was written and the second failed, the caller must see the error **and 4096 bytes** of progress. This work did not establish Windows' behavior for every write-behind mode or MM collision.

## 3. How a purge fix became a size and close fix

1. **Protecting a whole view was too broad.** A stream had a dirty page before the new end of file (EOF) and a separate clean view after it. A guard saw the pending BCB in the boundary view and skipped invalidation of the entire suffix, including the clean view and its MM pages. Simply removing the guard risked losing the valid dirty prefix. The first [range-aware repair](CcTruncate.md), based on tree `be9f26ff`, clipped crossing idle records, retained valid bytes in a partial page, and walked allocated page-table slices to invalidate even distant cached pages. It passed **16/16** host checks on both architectures.
2. **The first repair implemented the wrong public contract.** It triggered on allocation shrink, raised an exception from the legacy size entry on purge failure, and could veto later growth. [Both Windows kernels](Evidence/CcTruncate/resize-abi-by-build.json) showed that `CcSetFileSizes` calls `CcSetFileSizesEx` and returns without inspecting its status. The Ex path distinguishes a returned purge error from an exception during section extension. The implementation now uses EOF as the tail boundary and allocation size for extension. A busy CC view can defer cleanup without failing the notification; an MM purge refusal on the measured shrink path returns `STATUS_PURGE_FAILED`. An unexpected extension error is raised as `STATUS_UNEXPECTED_MM_EXTEND_ERR`.
3. **Ordinary file tests did not settle close-event timing.** The next [candidate, `2eef1040`](CcSizeDefault.md), allowed direct regrowth and queued close events until final teardown. It passed **25/25** host checks and matched Windows on the file and FAT programs. But closing a handle did not tell us when CC would signal a *supplied kernel event*. The direct test below disproved the blanket delay: removing one private cache map must notify that closer if another private map remains. Conversely, if only internal work remains, waiters can stay queued for cleanup or cache-map reuse. A clean last close with an event can finish synchronously.
4. **The valid part of the last page still matters.** An intermediate ReactOS build (`b3db2236`) returned `STATUS_PURGE_FAILED` when only valid data length (VDL) changed and a read-only mapping covered the last valid byte. Windows succeeded. For a data section without a shared cache map, ReactOS now flushes the partial boundary page and purges only subsequent whole pages. In the opposite case, a live section can still prevent shortening even if its resident pages are all *below* the proposed EOF. [Before/after control and Windows measurements](Evidence/CcTruncate/Native-9457/windows-ddi.json)

## 4. What callers actually observed

The [ordinary file program](Checks/file-size-contract.c) used two handles to a real file. The [kernel test driver](Checks/size-ddi.c) used its own zero-filled stream, called CC directly, and read each close event immediately after the call returned. In its shrink test the driver proposed a smaller EOF without changing the backing file. The distinction matters: the filesystem and CC reject different operations, with different statuses.

| Boundary and operation | Windows observation matched by the installed ReactOS guest |
|---|---|
| File API: shorten from 6144 to 4219 bytes | Allocation stayed at 8192. The second handle saw the new EOF immediately and read the correct 4219-byte prefix. |
| File API: request a further shrink with a read-only mapping | The filesystem returned `0xc0000243` (`STATUS_USER_MAPPED_FILE`); EOF stayed at 4219. After unmapping, direct growth to 14336 succeeded. The peer could still read and shorten the file after the first handle closed. Reopening confirmed EOF 4219 and the original prefix. |
| CC: keep a 4096-byte prefix mapped and propose EOF 8192 | `CcSetFileSizesEx` returned `0xc0000435` (`STATUS_PURGE_FAILED`) both with and without a shared cache map. With a shared map, its **visible cached EOF was already 8192** on return. |
| CC: release the mapping and section, then grow without repeating the shrink | Both calls returned `STATUS_SUCCESS`. In the shared-map case the resulting cached EOF was 73605. |
| CC: close the first of two file objects with private cache maps | `CcUninitializeCacheMap` returned **`FALSE`, event signalled**. The peer still had its private map. |
| CC: close the last clean private map with an event | **`TRUE`, event signalled**. The boolean reports synchronous final teardown in this case; neither the event nor the boolean is a disk I/O status. |

The ordinary-file reference ran on Windows [NTFS](Evidence/CcTruncate/Default-9457/guest-ntfs.json) and [FAT32](Evidence/CcTruncate/Default-9457/guest-fat-file-size-contract.json); the guest-to-guest filesystem comparison used FAT32. For the kernel test, a separate [WinDbg run](Evidence/CcTruncate/Native-9457/windbg-trace.json) captured **34 observations in order** at the test driver's reporting point after the calls. All **28 distinct fields** matched the Windows driver record and the ReactOS guest record.

## 5. Final acceptance

Commit `c369b1c` contains the validated repair on upstream ReactOS `4fbbefd614b`. The default x64 queue produced source tree `bd620b9b5a8c386e1bc981726bed3943425e4b74`. The [build record](Evidence/CcTruncate/Native-9457/checks.json) identifies the patch **including its preamble at that commit**. The clean x64 Debug solution built **1419 projects, 0 failed**; the BootCD was built separately.

The x64 Hyper-V guest was installed from that CD onto a blank 6 GiB disk. After shutdown, the installed MP kernel and `fastfat.sys` were matched to the linked binaries by content. The guest then cold-booted from disk with the installation media removed and a fresh 512 MiB FAT32 test disk attached. [Installed identities, DDI results and program output](Evidence/CcTruncate/Native-9457/reactos-guest.json)

| Check | Result |
|---|---|
| Kernel drivers, identical binaries in both guests | All 28 main fields matched, including both purge refusals and both subsequent growths. The independent last-page/VDL control also matched. |
| Ordinary file, volume flush, lock/dismount/remount | All three programs exited 0 with line-for-line identical output. At flush return both FAT copies had **128/128** correct entries. Three dismount/remount cycles verified **24 MiB** byte for byte. |
| Host and build coverage | Size/close **34/34** on each of x86 and x64; writeback, BCB and FAT regressions passed. x86 Release UP/MP kernels linked. The installed-guest results are x64. All four patch queue configurations applied and reversed. |

The accepted image is `OutDir/bootcd-x64-Debug.iso`, SHA-256 `2bffbc71fc69e84ccd1fc0c38e0150c44f2aa4709848de3bf6fb26f839508bc8`. To compare the **saved** Windows, WinDbg and ReactOS results from the repository root:

```text
python Docs/Notes/HyperV-Desktop-QA/Review/Checks/compare-size-runs.py
```

## Remaining differences and when to refactor

| Difference or open question | Reason and caller impact |
|---|---|
| ReactOS tracks an unfinished suffix (`TruncatePending`). | Its MM can purge some pages before finding one in use. Retrying only beyond the filesystem's new VDL protects newly initialized bytes. Pin-access streams also retire obsolete **idle** metadata records. This may require an extra cleanup pass; old work must neither discard a live pin nor veto valid growth. The field is a ReactOS implementation choice, not an observed Windows mechanism. |
| A close worker cannot be allocated. | ReactOS falls back to synchronous cleanup. The closer can wait longer and find its event already signalled on return. A host test exercised this fallback; the corresponding Windows low-memory timing has **not** been measured. |
| A lazy write overlaps an explicit flush, or storage fails during concurrent cleanup. | The Windows observer did not establish the intended lazy-write overlap. Static evidence distinguishes waiting from `STATUS_ENCOUNTERED_WRITE_IN_PROGRESS` on a nonwaiting MM path; it does not justify a blanket `STATUS_DEVICE_BUSY` or an unconditional wait while the filesystem holds a resource. Failure/retry host checks do not settle every Windows scheduling case. |

**Decision rule:** compare status versus exception, visible EOF and data, and event state at return and at completion. These cases did not require a wholesale rewrite. Fix a discrepancy locally when one return path or notification condition explains it. If truncation, writeback and close repeatedly disagree because they cannot tell who owns a page or when I/O has finished, change that shared CC/MM rule and retest all three paths together. Record the reason and caller impact for any retained difference.

## Evidence trail

The early Windows log used attempt IDs W01–W34. These entries explain which early observations informed the repair and which could not be scored. Original filenames below are relative to the local `OutDir/WindowsCc-Reference-20261126/`. The [reference index](Evidence/CcTruncate/Native-9457/reference.json) identifies the later IDA and live-code inspections.

| Attempts | Observation and limitation |
|---|---|
| W01–W04 | IDA established the idle BCB and writeback paths. Disabled write-behind changes the synchronization path. Tests that deliberately hold a pin during an ordinary purge are ReactOS defensive checks; they do not meet the [Windows purge preconditions](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-ccpurgecachesection). |
| W05–W17 | A new isolated Windows VM was identified by its disk chain, running kernel and PDB. The first debugger session had no target. Some later commands ran but their output was not delivered. Only identified stops and retained output count as observations; a `Stopped` state or a client timeout alone does not. |
| W18–W23 | Case 1's first write and purge succeeded, but the observer timed out waiting for close. A cleanup-only revision let callbacks proceed and extended the wait: the data observations repeated and the controller exited 0. [Paired records](Evidence/CcTruncate/Default-9457/windows-close-observation.json). The FAT disk prepared at this point was not the later FAT acceptance run. |
| W24–W25 | `relay-case4-wait-sample.json` captured the blocked worker; `win-case4-v4b-recovered-guest.txt` recorded the later write and successful completion. The host wrapper timed out and did not recover the original process exit code. |
| W26–W34 | Case 3 never reached its lazy-write trigger. During subsequent cleanup the guest stopped in MM; the sampled file object belonged to the observer, which had already cleared its stream context. This was an observer lifetime problem, not a Windows cache result. The sample and the failure to deliver the final debugger reply were retained separately. |

The early audit distinguishes reconstructed setup notes from timestamped operations in `History/attempts.jsonl`. At its recorded checkpoint, **135 records / 298 distinct snapshots** verified; the initial Windows archive matched **29/29** entries. The later ReactOS stage-one and stage-two archives matched **9/9** and **27/27** entries respectively. The local manifests are under `Evidence/WindowsCc-20261126/`, `Evidence/OpenSource-Stage1/` and `Evidence/OpenSource-Stage2/`. These are historical integrity counts, not compatibility scores. The pre-merge working log is preserved verbatim in the local `OutDir/WinAlign-9457-01/notes-archive/5f312abf56e3de6a76895094999febebdab9e31aa7cee8eda377d1c2ddb73809.txt`.

Several side investigations stayed outside the default repair. The MM pre-wait experiment could still encounter a new write after waiting; the ticket and filesystem-cooperation alternatives were not selected. Independent FastFAT error-propagation fixes linked, but that did not validate short-BCB writeback. A sparse whole-file **flush** miss was measured only on a separate experimental x64 tree; it is distinct from the **truncation** regression fixed above. None of these results was counted toward the accepted guest.
