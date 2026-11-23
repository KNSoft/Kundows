# Colliding MM cache writeback

`MmCheckDirtySegment` previously cleared a segment entry's dirty bit, set its
write-in-progress bit, and dropped the segment lock while issuing the write.
Another caller could redirty the entry and enter the same function during that
I/O. It did not check the existing write bit before issuing a second write.
If the older I/O completed last, it could overwrite the newer data while both
callers ultimately considered the page clean.

`ReactOS-mm-flush-collision.patch` waits for an existing page write, dropping
the segment lock while waiting and rereading the entry after reacquisition.
This follows the wait/recheck pattern already used by `MmPurgeSegment`.
The range flush path also waits for a write-in-progress entry even when its
dirty bit is currently clear; otherwise a synchronous flush could return
before that earlier I/O had completed.

Both native UP and MP kernel producers apply this scoped patch. The existing
source-patch mutex serializes clean UP/MP preparation; it does not replace
source validation with an unchecked success stamp.

```powershell
Build/Tests/CacheFlushCollision.Tests.ps1 -MSBuild <path-to-MSBuild.exe>
```

The native fixture compiles the actual `MmCheckDirtySegment` and
`MmFlushSegment` function bodies, without rewriting them. Two host threads and
events control the I/O boundary so that the older write is deliberately held
while a newer writeback is attempted. The original six-case test reports four
failed assertions, reproducing lost updates for flush and page-out callers.
A seventh case independently detects the premature return for a clean entry
whose write is still in flight. All seven cases pass on native Win32 and x64
with both guards present.

Other cases cover clean pages, ordinary writes, retained dirty state after
write failure, unused-page release, share counts, and segment-lock ownership.
Swap/image-page backing is outside this fixture's scope. These tests do not
claim to fix the existing general I/O-error propagation FIXME in
`MmFlushSegment`, and do not substitute for real installation/durability tests.

## Relationship to the installation failures

The first pinned-dirty-only ISO revision still failed both Release first-disk
boots. The retained Win32 Release disk's SYSTEM file required 126 clusters but
its chain ended after 55; SYSTEM.ALT's first FAT entry was free. The retained
x64 Release disk had different FAT mirror hashes and stopped with
`0xC000021A`, status `STATUS_FILE_CORRUPT_ERROR`. Both installed kernels matched
the rebuilt binaries, so this was not testing stale kernel media.

The MM regressions establish a separate real lost-write/writeback-barrier
defect. They do not prove that every observed damaged file had this one cause.
The corrected four-image revision must still pass serial-disconnected fresh
installation through a responsive installed desktop, followed by read-only
disk checks.
