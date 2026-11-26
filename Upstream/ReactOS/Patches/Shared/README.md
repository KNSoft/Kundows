# Running the regression checks

Run commands from the checkout root. These are shared test tools; [issue notes](../../../../Docs/README.md#notes) give the relevant result for each defect.

| Entry | What it measures |
|---|---|
| [Host/Run.cmd](Host/Run.cmd) | From a native VS developer prompt, run one C case (e.g. `Run.cmd writeback`). It takes the current `cl`, defaults to Debug, and accepts `--source`, `--stock`, `--mm-only` and `--overlay`. Python extracts the production functions when the case needs them. Each issue links its own case entry. |
| [CheckSeries.py](CheckSeries.py) | Apply each selectable queue on a disposable Git index, recognize the already-applied patches, reverse them, and print the applied tree. |
| [CheckMedia.py](CheckMedia.py) | Run `identity`, `links`, `media` or `baseline` from the repository root. For an optional x64 image pass `--image-fixes` to all candidate-identity checks. Re-record identities for the current inputs; a historical hash is valid only for its original bytes. |
| [SavedGuest.py](SavedGuest.py) | Shared Windows SDK saved-state reader used by the [one-sided mirror investigation](../Storage/FAT32InstallMirrorDivergence/README.md) and [volume-flush tests](../Storage/FAT32VolumeFlush/README.md#running-the-tests). |

For host C tests, use the native tools for the host (e.g. x64 on x64). To check another target, supply that developer prompt explicitly. `Compile.cmd` invokes `cl` directly (`/Od /Z7 /MTd /D_DEBUG`, link `/DEBUG`); individual cases can pass `CASE_CFLAGS` or supply their own compile and link options. A checked driver or a no-CRT guest program has its own build entry. Stock controls can intentionally return nonzero.

## Desktop application fixtures

`python Upstream/ReactOS/Patches/Shared/Desktop/MakeInputs.py HVQA` generates the exact input bytes. In the original runs the installed and side-loaded candidates used the same inputs on the same clean x64 installation. The [combined TSV](Desktop/Results.tsv) records output as escaped bytes and exit codes. Its 17 command-line cases were also checked with Windows build 26300; the four Notepad cases used a real GUI edit/save and file readback. The historical Windows comparison does not cover Notepad.

The original parallel run was **8/21 → 21/21** on its short inputs ([guest screenshot](Desktop/QaResults.png)). After integrating SORT, WHERE and Notepad the installed set was **15/21**, including **14/14** for those three applications; FINDSTR remained at 1/7 ([installed results](Desktop/IntegratedApps.png)). The [FINDSTR counterexamples](../Applications/FindStrLineAnchors/README.md) explain why the fourth candidate was not selected.
