# Superseded patches

These patches are **not selected by `Configure.cmd`**. Their original hunks
remain here for historical test records and migration, not as optional fixes.

| Retired patch | Active replacement |
| --- | --- |
| `cc-pinned-dirty` | `Runtime/ReactOS-cc-bcb-ownership.patch`: premark intent and final unpin publication |
| `cc-duplicate-bcb` | The same ownership patch: duplicate winner locking, references and loser cleanup |
| `fastfat-volume-dirty-runs` (C2) | The same ownership patch: retain dirty BCBs so the original FastFAT scan can find them |

`Runtime/ReactOS-mm-flush-status.patch` remains the separate MM/CC write-error
and bounded-retry fix. `Runtime/ReactOS-fastfat-clean-after-flush.patch` (F1)
still prevents an unsuccessful filesystem flush from marking a volume clean.
The optional `mm-flush-collision` investigation is unchanged.

For a source tree carrying the old patches, first verify and reverse these
three retired patches (C2, duplicate BCB, pinned dirty), then run `Configure.cmd`.
Do not reset the source tree or discard unrelated local changes. Stop if a
reverse check fails; a partial or locally modified old patch needs inspection.
Already migrated trees must **not** have the retired CC patches applied again.

See [the evidence and queue reorganization](../../../../Docs/Notes/HyperV-Desktop-QA/Review/CcOwnership.md).
