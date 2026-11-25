# ReactOS patch organization

`Build/ReactOS.cmd` lists the selected patches explicitly. A directory listing
is not the application order. `BuildSystem`, `Runtime` and target-specific
`ARM64` patches remain separate; `Retired` is historical evidence only.

## Cache and FAT repair points

| Repair point | Active patch | Scope |
| --- | --- | --- |
| BCB ownership and lifetime | [cc-bcb-ownership](Runtime/ReactOS-cc-bcb-ownership.patch) | Premark publication, duplicate winner/loser ownership, zero-pin lookup, dirty retention, writeback generations, successful retirement and quiesced teardown |
| Write errors and retry policy | [mm-flush-status](Runtime/ReactOS-mm-flush-status.patch) | Report MM write errors without losing dirty data; bound the CC dirty-view pass after an error |
| FAT volume clean state | [fastfat-clean-after-flush](Runtime/ReactOS-fastfat-clean-after-flush.patch) | F1: an unsuccessful filesystem flush must not mark the volume clean |
| Independent MM write collisions | [mm-flush-collision](Runtime/ReactOS-mm-flush-collision.patch) | Still optional, selected only by `--image-fixes`; not implied by the CC reservation |

The ownership patch replaces the old `cc-pinned-dirty` and `cc-duplicate-bcb`
patches and the FastFAT C2 `fastfat-volume-dirty-runs` workaround. Those three
are in [Retired](Retired/README.md), outside the default queue. FastFAT retains
its original FAT scan; no C2 fallback remains in the reorganized default source.

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

The user requested a local commit only, without pushing. This reorganization
is not a new released or VM-tested image. Prior guest evidence and the current integration
boundary are recorded in [CcOwnership.md](../../../Docs/Notes/HyperV-Desktop-QA/Review/CcOwnership.md).
