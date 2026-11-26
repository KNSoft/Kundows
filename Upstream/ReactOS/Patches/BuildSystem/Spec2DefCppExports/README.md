# Win32 MSVC lost the names or ordinals of decorated C++ exports

Linking `msvcrt20` successfully did not mean its exports matched the spec. On `1103f43a0c0` only **11 of 437** decorated C++ entries had both the expected name and ordinal.

[Spec2DefCppExports.patch](Spec2DefCppExports.patch) changes the MSVC export generation in `spec2def`. Compare the linked DLL's export names **and ordinals** to the input spec: all **437/437** matched on the tested Win32 build. This is a link/export-table check, not a test of the exported functions.
