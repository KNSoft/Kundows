# The BIOS loader could contain only its 2 KiB prefix

A concrete x64 **BIOS** difference is visible on the same ReactOS commit
(`1103f43a0c0`), configuration (Release) and compiler: the upstream
[FreeLdr rule](../../../Source/boot/freeldr/freeldr/pcat.cmake)
concatenates `frldr16.bin` with `${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:freeldr_pe>`.
That expression supplies a filename, but the VS generator puts the PE in
`Release/freeldr_pe.exe`. In a Windows `copy /B` check with a present first
file and a missing second file, the command returned success and wrote only
the first file. In the retained upstream build, `freeldr.sys` was
byte-identical to the 2,048-byte prefix alone. In the old Kundows build it
was byte-identical to the prefix plus the 173,568-byte PE (175,616 bytes).
The unmodified rule is unchanged at `4fbbefd614b`. The
[FreeLdr patch](FreeLdrTargetFile.patch) uses
`$<TARGET_FILE:freeldr_pe>` to get the linked PE for the active configuration.
The generated x64 Debug and Win32 Release projects both reference that PE
as an input to the concatenation rule. The UEFI loader uses a separate rule.
