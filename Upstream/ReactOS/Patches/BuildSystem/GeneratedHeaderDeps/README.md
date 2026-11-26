# Parallel builds could start before generated SDK headers existed

On a clean x64 Debug VS build at `4fbbefd614b`, two lwIP targets included `wdm.h` before `xdk` had generated it. `pathcch_static` reached `winscard.h` before `psdk` supplied `wtypes.h`, leaving three dependent DLLs unable to link. An incremental tree with either header already present can hide the ordering defect.

[GeneratedHeaderDeps.patch](GeneratedHeaderDeps.patch) adds dependencies on the header-producing targets. Check from an empty build directory with a parallel solution build; a serial rebuild of the individual consumer is not a control for this race. See the [historical clean solution results](../../../../../Docs/Building.md#visual-studio-and-the-boot-image).
