# Pinned cache data regression

Filesystems can call `CcSetDirtyPinnedData` before updating a pinned buffer.
If writeback cleans the VACB and segment before that update finishes, the
caller must not lose its later changes when it releases the pin.

`ReactOS-cc-pinned-dirty.patch` retains dirty intent in the internal BCB and
publishes the completed mapped range to both MM and Cc again at unpin, before
releasing the caller's resource/reference. The flag is retained for other
outstanding pins and repin references. Clean pins do not dirty data.
The public BCB layout and debugger-detection behavior are unchanged.

The native Ntoskrnl/Ntkrnlmp producers apply the source patch through
`ReactOSCachePinnedData.targets`; unrelated consumers do not run this target.
The scoped patch application is serialized because UP and MP kernel producers
can prepare concurrently on a clean source tree.

```powershell
Build/Tests/CachePinnedData.Tests.ps1 -MSBuild <path-to-MSBuild.exe>
```

The test extractor copies the actual production function definitions from
`ntoskrnl/cc/pin.c` without rewriting their bodies. MSBuild compiles them in
ordinary Win32/x64 host-test projects. Only the kernel/MM boundary is mocked,
including an intervening writeback that clears both segment and VACB state.

Nine cases cover clean pins, normal dirtying, pre-dirty/update/writeback
interleavings, nested pins, separate ranges sharing a VACB, repinned
write-through/non-write-through release, resource ownership and exact ranges.
The original functions reproduce nine failed assertions; the corrected
functions pass all nine cases on each host platform.

These deterministic tests do not replace real kernel/filesystem durability
or installation-to-desktop tests. The four-ISO validation report must identify
the actual image revision and distinguish diagnostic boots from final,
serial-disconnected installations.

## Installation investigation

The original Win32 Debug installation had a 579,072-byte `usbhub3.sys`
whose first FAT32 cluster, 599, was marked free in both identical FAT mirrors.
The Release installation had incomplete chains for `SYSTEM` and `SYSTEM.ALT`.
Host-side reads of preserved, read-only disk clones confirmed real on-disk
corruption, rather than only a FreeLdr FAT cache failure.

KD tracing of the actual `fastfat` driver (not the legacy `vfatfs` driver)
observed the legitimate 142-cluster allocation for `usbhub3.sys` and confirmed
that its BCB/VACB was dirtied before the FAT stores. The observed subsequent
dirty-clear trap occurred **after** those stores; it did not directly capture
the losing interleaving in the failed installation. A later zero-write trap
was reuse of the unmapped address by a directory buffer, not proof of FAT
deallocation. Debugger timing also allowed that diagnostic installation to
boot its disk kernel; that is not a final installation pass.

The production-function regression independently establishes the lost-update
window and its correction. A passing fresh installation is supporting runtime
evidence, not proof that this was the only possible source of disk corruption.

Fresh, serial-disconnected tests of the first corrected revision reached
responsive installed desktops in both Debug configurations, but both Release installations
failed their first disk boot. Read-only inspection still found a truncated
Win32 SYSTEM chain and mismatching x64 FAT mirrors. The pinning correction is
therefore **not** accepted as a complete fix for the installation failures.
