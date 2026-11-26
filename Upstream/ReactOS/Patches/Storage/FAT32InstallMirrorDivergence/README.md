# One FAT copy on an installation disk lost 29 allocation entries

**Open: the damaged cluster chains explain the saved Debug boot failures; the write that first damaged them was not captured.** On the sealed disk, all 29 differing entries were in sector 311. FAT1 lacked entries 39907–39935: `PALAI.TTF` occupied one cluster according to FAT1 and 68 according to FAT2. Reading the latter chain recovered all 278,228 bytes, identical to the install CAB. The disk had already failed and rebooted before these measurements.

| Controlled observation | What it tells us |
|---|---|
| Restore only the 29 entries on a copy. | The ordinary boot entry [reached Setup](Assets/FirstDiskRepairedGui.png); Debug still stopped. With independent [display-query repairs](../../Graphics/MonitorListCompletion/README.md), the ordinary entry also reached Setup with the FAT corruption unchanged. |
| Read the first exception message and its file name on copies of the stopped Debug guest. | FastFAT reported a broken `PAGEFILE.SYS` chain. Isolating that directory entry exposed `SYSTEM.ALT`, then the font. The first two chains were short in **both** FATs. Repairing or isolating all three on a copy allowed [graphical Setup](Assets/FirstDiskDebugRecoveredGui.png) and a [cold boot](Assets/FirstDiskDebugColdDesktop.png). These were recovery controls, not a filesystem fix. |
| Trace a new installation from the same ISO. | Both copies contained the correct 68-cluster font chain. The original divergence did not repeat. In the diagnostic run, all 148 mirror writes completed with their requested lengths; [the paired submissions and completions](Assets/MirrorRequests.tsv) include both copies of the watched sector. |

The [disk reader](Tests/ReadMirrors.py) compares directory length, chain length and both tables on a read-only disk copy. The [saved-state reader](Tests/KdFileError.py) identifies which file the original Debug assertion was reporting. The [mirror probe](Assets/FatMirrorProbe.patch), [trace reader](Tests/ReadTrace.py) and [VHD event summarizer](Tests/SummarizeVhd.py) distinguish what the guest submitted, what completed, and what the subsequent disk read returned.

A [host short-write control](Tests/ShortWrite.c) found that one successful short child write could be counted as a full mirror request. None of the measured children returned that result. Separately, the [reproduced volume flush defect](../FAT32VolumeFlush/README.md) left **both** copies temporarily stale; it did not reproduce this one-sided gap. Closing this issue requires evidence at the **first** divergent write: source contents, both child requests' status and byte counts, and immediate reads of both copies.

Run the [host case](Tests/Run.cmd) from a native VS developer prompt. Pass `--stock` to exercise the pinned source.
