# Running the regression checks

Run commands from the checkout root. These are shared test tools; [issue notes](../../../../Docs/README.md#notes) give the relevant result for each defect.

| Entry | What it measures |
|---|---|
| Host C tests | Run the issue's `Run.cmd` in a native VS developer prompt. For example, [UserInitUnlock/Assets/Run.cmd](../Graphics/UserInitUnlock/Assets/Run.cmd) compiles and runs `User.c` against `NtUserInitialize` from the selected ReactOS source. |
| [CheckSeries.py](CheckSeries.py) | Apply each selectable queue on a disposable Git index, recognize the already-applied patches, reverse them, and print the applied tree. |
| [CheckMedia.py](CheckMedia.py) | Run `identity`, `links`, `media` or `baseline` from the repository root. For an optional x64 image pass `--image-fixes` to all candidate-identity checks. Re-record identities for the current inputs; a historical hash is valid only for its original bytes. |
| [SavedGuest.py](SavedGuest.py) | Shared Windows SDK saved-state reader used by the [one-sided mirror investigation](../Storage/FAT32InstallMirrorDivergence/README.md) and [volume-flush tests](../Storage/FAT32VolumeFlush/README.md#running-the-tests). |

The [host build command](Host/Run.cmd) uses the current `cl` with Debug defaults. For tests that exercise kernel code in user mode, the issue names the production functions to compile. The [source reader](Host/Extract.ps1) reads the checkout by default; `--stock` selects the upstream pin. Stock negative controls can intentionally return nonzero.

## Desktop application fixtures

Copy [Desktop/Inputs/](Desktop/Inputs/) to a fresh `HVQA` directory. The files preserve the original bytes, including mixed line endings and missing final newlines. The installed and side-loaded candidates used those same inputs on the same clean x64 installation. The [combined TSV](Desktop/Results.tsv) records output as escaped bytes and exit codes. Its 17 command-line cases were also checked with Windows build 26300; the four Notepad cases used a real GUI edit/save and file readback. The historical Windows comparison does not cover Notepad.

The original parallel run was **8/21 → 21/21** on its short inputs ([guest screenshot](Desktop/QaResults.png)). After integrating SORT, WHERE and Notepad the installed set was **15/21**, including **14/14** for those three applications; FINDSTR remained at 1/7 ([installed results](Desktop/IntegratedApps.png)). The [FINDSTR counterexamples](../Applications/FindStrLineAnchors/README.md) explain why the fourth candidate was not selected.
