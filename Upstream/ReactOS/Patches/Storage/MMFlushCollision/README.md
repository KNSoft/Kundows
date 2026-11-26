# A second section flush could return while a write was still in flight

The [two-writer fixture](Assets/CacheFlushCollisionTests.c) holds one page write, starts a second caller and records write ordering, completion and the backing value. On `4fbbefd614b` seven controlled cases produced five failed checks before the repair and none after it. They do not identify the write that corrupted a particular installation disk.

[MmFlushCollision.patch](MmFlushCollision.patch) waits for the in-flight write before starting another or returning from that flush. It is selected only with `--image-fixes`. Use the [case entry](Assets/Run.cmd) from a VS developer prompt on a source tree with this optional patch applied; `--stock` runs the negative control. The [write-error reporting issue](../MMFlushWriteErrors/README.md) has a different test and a separate fix.
