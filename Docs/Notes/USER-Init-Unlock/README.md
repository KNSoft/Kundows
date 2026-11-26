# Failed GDI initialization left the USER lock held

`NtUserInitialize` acquired the exclusive USER lock before calling `InitializeGreCSRSS`. When GDI initialization returned false, the original function returned directly. Later users of the same lock could then wait indefinitely. The neighboring power-initialization failure path already released it.

[`user-init-unlock.patch`](../../../Upstream/ReactOS/Patches/Runtime/user-init-unlock.patch) routes the GDI failure through the common unlock path, preserves the original error, and does not proceed to `UserInitialize`. The [controlled host test](../../../Tests/ReactOS/Host/user.c) checks the lock count, the recorded error and which initialization functions ran: **4/5 → 5/5**. Recheck with `python Tests/ReactOS/Host/run.py Upstream/ReactOS/Source --suite user`.

The patch does not undo previously registered state or promise a retry of initialization. Starting a window on a working installation did not test this failure; the gray-screen disk replay did not establish that GDI returned false.
