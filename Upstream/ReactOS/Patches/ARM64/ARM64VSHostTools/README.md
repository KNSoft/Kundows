# The ARM64 solution selected the target architecture for host executables

At `4fbbefd614b` the VS generator passed `-AARM64` to the child host-tools configure even though upstream had selected an `amd64_x86` compiler environment. Those tools must execute on the build host.

[ARM64VSHostTools.patch](ARM64VSHostTools.patch) selects `Win32` for that child project. The host-tools build then completed; `spec2def.exe` had PE machine type **`0x014c` (i386)**. Verify the child platform and the generated executable, not just the parent solution platform. No ARM64 runtime result is implied.
