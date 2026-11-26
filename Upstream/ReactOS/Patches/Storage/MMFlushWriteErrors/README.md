# A failed paging write was counted as a successful section flush

**Fixed on the tested ReactOS paths** by [`MmFlushStatus.patch`](MmFlushStatus.patch). The original MM page-write helper returned a boolean about page reclamation. `MmFlushSegment` counted the page and could return success even when the write failed and the page was still dirty.

The [first single-page test](Tests/SinglePage.c) made the distinction visible: on failure the caller saw success and 4096 completed bytes, but the backing value stayed zero. A retry wrote the expected value. For a range with a good page followed by a failed page, the repair reports the error **and only the already written prefix**, and retains the failed page for retry. A page modified again during a successful write must also stay dirty.

Changing MM alone exposed a second problem: the CC lazy writer could immediately pick the same failing view off the dirty list over and over. The patch bounds the work in that pass while allowing another view and a later retry. The [host cases](Tests/Writeback.c) went from **8/21 to 21/21**.

This establishes error propagation, not that the old installation disk suffered a device error. The [CC writeback investigation](../FAT32VolumeFlush/README.md#what-the-debugger-showed) shows how the caller receives the completed byte count on a partial write.

Run the [host case](Tests/Run.cmd) from a native VS developer prompt. Pass `--stock` to exercise the pinned source.
