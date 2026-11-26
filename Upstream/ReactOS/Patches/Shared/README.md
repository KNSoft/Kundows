# Running the regression checks

Run commands from the checkout root. These are shared test tools; [issue notes](../../../../Docs/README.md#notes) give the relevant result for each defect.

| Entry | What it measures |
|---|---|
| Host C tests | Run the issue's `Run.cmd` in a native VS developer prompt. For example, [UserInitUnlock/Assets/Run.cmd](../Graphics/UserInitUnlock/Assets/Run.cmd) compiles and runs `User.c` against `NtUserInitialize` from the selected ReactOS source. |
| [CheckSeries.cmd](CheckSeries.cmd) | Check selection, application and rollback of each queue on a temporary Git index. Also check that the two cache repairs produce the same tree in either order. |

From the checkout root, with the ReactOS submodule initialized and the patches applied, run:

```bat
Upstream\ReactOS\Patches\Graphics\UserInitUnlock\Assets\Run.cmd
```

The issue's command names `win32ss/user/ntuser/ntuser.c` and `NtUserInitialize`. The [source reader](Host/Extract.ps1) takes that function from the checkout and the [host build command](Host/Run.cmd) uses the current `cl` to compile and run the C test with Debug defaults. For this example the patched source passes 5/5; adding `--stock` tests the upstream pin and exposes the GDI failure (4/5).

[BootCD/](BootCD/README.md) holds historical installation evidence. The [x64 image verifier and saved-state reader at `794f531`](https://github.com/KNSoft/Kundows/tree/794f531c7b80f726547845e7c1d19bd9d1c92e77/Upstream/ReactOS/Patches/Shared) remain available for reproducing those investigations.

## Desktop application fixtures

Copy [Desktop/Inputs/](Desktop/Inputs/) to a fresh `HVQA` directory. The files preserve the original bytes, including mixed line endings and missing final newlines. The installed and side-loaded candidates used those same inputs on the same clean x64 installation. The [combined TSV](Desktop/Results.tsv) records output as escaped bytes and exit codes. Its 17 command-line cases were also checked with Windows build 26300; the four Notepad cases used a real GUI edit/save and file readback. The historical Windows comparison does not cover Notepad.

The original parallel run was **8/21 → 21/21** on its short inputs ([guest screenshot](Desktop/QaResults.png)). After integrating SORT, WHERE and Notepad the installed set was **15/21**, including **14/14** for those three applications; FINDSTR remained at 1/7 ([installed results](Desktop/IntegratedApps.png)). The [FINDSTR counterexamples](../Applications/FindStrLineAnchors/README.md) explain why the fourth candidate was not selected.
