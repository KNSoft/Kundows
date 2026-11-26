# ReactOS patch organization

[`Build/ReactOS.cmd`](../../../Build/ReactOS.cmd) lists the patches in application order.
`BuildSystem`, `Runtime` and `ARM64` contain selectable patches. Name each file
for the change it makes, in lowercase with hyphens, and use that filename in
the documentation. Retired patches belong beside the investigation that used them.

## Cache and FAT repair points

| Repair point | Active patch | Scope |
| --- | --- | --- |
| BCB ownership and lifetime | [`cc-bcb-ownership.patch`](Runtime/cc-bcb-ownership.patch) | Premark publication, metadata writeback, [file-size notifications, tail invalidation and cache-close events](../../../Docs/Notes/FAT32-Volume-Flush/README.md) |
| Write errors and retry policy | [`mm-flush-status.patch`](Runtime/mm-flush-status.patch) | Report MM write errors without losing dirty data; bound the CC dirty-view pass after an error |
| FAT volume clean state | [`fastfat-clean-after-flush.patch`](Runtime/fastfat-clean-after-flush.patch) | An unsuccessful filesystem flush must not mark the volume clean |
| Independent MM write collisions | [`mm-flush-collision.patch`](Runtime/mm-flush-collision.patch) | Optional; selected only by `--image-fixes` |

`cc-bcb-ownership.patch` incorporates the two earlier CC repairs and replaces the
FastFAT workaround. The [three historical files and migration instructions](../../../Docs/Notes/FAT32-Volume-Flush/README.md#superseded-patch-files)
are in the FAT32 note. FastFAT retains its original scan.

The two active cache patches do not overwrite each other's hunks. They can
apply in either order and remain individually reverse-recognizable. There is
still a **behavioral** dependency: safe dirty retirement after failed I/O
requires MM to report the real failure, so the default set contains both.
Do not split the tightly coupled BCB fields and their lifecycle exits into
another sequence of overlapping patches.

To check application, per-patch recognition, both cache orders and complete
rollback without editing the source checkout:

```text
python Docs/Notes/HyperV-Desktop-QA/Review/Checks/patch-sets.py
```

For the original FAT32 failure, the repair history, and the final Windows and
ReactOS guest results, see the [cache manager investigation](../../../Docs/Notes/FAT32-Volume-Flush/README.md).
