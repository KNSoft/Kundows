# Missing ARM64 NTDLL and RTL objects blocked the experimental target

[ARM64Ntdll.patch](ARM64Ntdll.patch) supplies the ARM64 dispatch stubs, RTL assembly and build wiring absent from the original profile. The early link evidence is from `1103f43a0c0`. Inspect the patch before treating a linked entry as an implementation: two dispatcher entries are still breakpoint stubs.

The [build guide](../../../../../Docs/Building.md#patch-selection) selects this patch only for ARM64. Generating that solution or linking these objects does not demonstrate a working ARM64 guest.
