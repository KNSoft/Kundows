# Running the regression checks

Run commands from the checkout root. These are shared test tools; [issue notes](../../Docs/README.md#notes) give the relevant result for each defect.

| Entry | What it measures |
|---|---|
| `python Tests/ReactOS/Host/run.py Upstream/ReactOS/Source --suite <name>` | From an x64 or x86 MSVC prompt: compile selected production functions with controlled I/O, resource and completion boundaries. `--stock` reads the pinned Git blobs; `--overlay <tree>` substitutes only matching files. Run `--help` for suites. Stock negative controls can exit 1. |
| [`patch-sets.py`](patch-sets.py) | Apply, reverse-recognize and undo each selectable queue without modifying the source checkout. |
| [`Host/handoff-contract.py`](Host/handoff-contract.py) | Two-thread metadata handoff checks; pass the ReactOS source tree from an MSVC prompt. The [`metadata-writeback` runner](Host/metadata-writeback/run.py) also accepts `--stage2 <dir>` for the two hash-checked historical deltas. |
| [`FAT32Trace/`](FAT32Trace/) | Read saved guest states, mirror write records and read-only disk copies. Trace readers that use the Windows SDK saved-state provider require its x64 DLL. |
| [FAT volume and cache lifecycle fixtures](../../Docs/Notes/FAT32-Volume-Flush/Tests/README.md) | Workload and observation details for the missed-flush investigation, plus the command to compare saved Windows and ReactOS results. |
| [`Media/check.py`](Media/check.py) | From the repository root: `identity`, `links`, `media` and `baseline` verify inputs, linked PEs and BootCD members. Re-record the approved input for a new tree; a historical hash is only valid for its original bytes. |

## Desktop application fixtures

`python Tests/ReactOS/Desktop/make-inputs.py HVQA` generates the exact input bytes. In the original runs the installed and side-loaded candidates used the same inputs on the same clean x64 installation. The [combined TSV](Desktop/Evidence/Results.tsv) records output as escaped bytes and exit codes. Its 17 command-line cases were also checked with Windows build 26300; the four Notepad cases used a real GUI edit/save and file readback. The historical Windows comparison does not cover Notepad.

The original parallel run was **8/21 → 21/21** on its short inputs ([guest screenshot](Desktop/Evidence/qa-results.png)). After integrating SORT, WHERE and Notepad the installed set was **15/21**, including **14/14** for those three applications; FINDSTR remained at 1/7 ([installed results](Desktop/Evidence/integrated-apps.png)). The [FINDSTR counterexamples](../../Docs/Notes/FINDSTR-Line-Anchors/README.md) explain why the fourth candidate was not selected.
