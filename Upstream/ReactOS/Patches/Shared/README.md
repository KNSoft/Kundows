# Running host C tests

Use a native VS developer prompt and run the `Run.cmd` linked in the [issue note](../../../../Docs/README.md#notes). For example, with the ReactOS submodule initialized and the patches applied, run this command from the checkout root:

```bat
Upstream\ReactOS\Patches\Graphics\UserInitUnlock\Assets\Run.cmd
```

The [issue's command](../Graphics/UserInitUnlock/Assets/Run.cmd) names `win32ss/user/ntuser/ntuser.c` and `NtUserInitialize`. The [source reader](Host/Extract.ps1) takes that function from the checkout and the [host build command](Host/Run.cmd) uses the current `cl` to compile and run the C test with Debug defaults. For this example the patched source passes 5/5; adding `--stock` tests the upstream pin and exposes the GDI failure (4/5).
