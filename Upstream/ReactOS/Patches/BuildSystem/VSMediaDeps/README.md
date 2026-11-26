# Multiple VS projects raced to generate the registry inputs for BootCD

On the clean x64 Debug solution at `4fbbefd614b`, the IDE scheduled image projects outside `ALL_BUILD`. Several projects regenerated the shared registry INFs; `bootcd_hives` failed with a sharing violation.

[VSMediaDeps.patch](VSMediaDeps.patch) leaves media assembly to explicit targets and makes hive projects depend on the common INF producer. It preserves file-level dependencies for incremental builds. Verify both operations: **Build Solution** from a clean tree, then build `bootcd` explicitly in that same tree. The [build and packaged-loader measurements](../../../../../Docs/Building.md#visual-studio-and-the-boot-image) record that sequence.
