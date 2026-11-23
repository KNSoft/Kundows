# Native build investigation (2026-11-23)

## Scope and baseline

All four images use native Windows MSBuild/MSVC. No ReactOS CMake/Ninja build
was run, so these measurements do **not** establish an upstream/native speed
ratio. Reference CMake projects were configured only for declaration parity.
`DLL_EXPORT_VERSION=0x502`, disabled rostests, and strict packaging/PE checks
remain unchanged.

The host has an i9-13900HX (24 cores / 32 logical processors). The original
sequential matrix used `/m:4 /lowPriority /p:CL_MPCount=2`. Its cache state was
not uniform: x64 had existing outputs; Win32 was effectively a first system
build. Recent generator changes also caused real rebuilding.

| Original build | Active wall seconds | System MSBuild seconds |
| --- | ---: | ---: |
| x64 Release | 3437.560 | 3168.98 |
| x64 Debug | 4051.695 | 3698.86 |
| Win32 Release | 3311.430 | 3021.69 |
| Win32 Debug | 2074.458 | 1594.93 |

Win32 Debug includes failed packaging and its packaging-only retry, but
excludes the intervening 205.011-second investigation gap. The failure was
`mkisofs` exit 28 from E: exhaustion, not a compiler failure. Only this batch's
images were relocated to D:, and failed/replaced images were retained.
Packaging/layout/hash/invocation residuals are not pure compression timings.
Subsequent correctness rebuilds, repackaging, and VM tests are separate.

## Findings established from the actual build commands

- All 1507 / 1509 / 1518 / 1520 recorded normal CL command lines (table order)
  used **`/MP2`**, not unrestricted `/MP`. `/m:4` plus this compiler setting is
  conservative on this host, but does not guarantee eight busy cores.
  Win32 used HostX86 compilers; x64 used HostX64.
- x64 Release/Debug issued 4852/4860 patch-check PowerShell command lines.
  `ReactOSPatches.targets` starts four processes before `PrepareForBuild` in
  each visited project. Even a cache hit hashes/reads state and starts Git
  twice. There is no target-level incremental skip.
- Message/resource probes have broad project/`None`-item invalidation.
  Message probing covers roughly 1200 projects and discovers `mc.exe`
  recursively before determining whether generation is needed. Resource
  probing covers roughly 680 projects, although only four upstream modules
  need the special generated resources. Unlike patch checks, these probes
  are stamp-incremental and must not be called unconditional.
- Packaging repeatedly parses native project XML and upstream CMake
  declarations. Several packaging functions independently rebuild the
  project-output map. Strict audits are intentional additional work, not
  evidence that cabinet compression itself is slow.
- The x64 Release solution traversal built Hpp/Widl/WppHost references as
  Win32 Debug, while generation used the separately built x64 Release tools.
  This is a producer/configuration mapping defect and a clean-build risk,
  not evidence that target PE machine types were wrong.
- Repeated references alone do not prove repeated tool compilation.
  Ntoskrnl retains per-source PCH metadata. XML size and a broad Prepare
  dependency barrier are scheduling/evaluation hypotheses, not measured
  dominant costs.

## Controlled experiments

The following tests ran with the owned VMs stopped and no correctness build
running. They do not alter production targets, sources, or cache state.

| Read-only cached-path experiment | Trials | Median seconds |
| --- | ---: | ---: |
| Empty PowerShell process | 5 | 0.216 |
| Four separate patch-probe processes | 5 | 1.730 |
| One process performing the same four probes | 5 | 0.629 |

Both probe variants perform the same four hashes/state reads and eight Git
invocations. Process consolidation saved about 1.101 seconds per four-probe
group in this isolated test. That is **not** a full-build speedup: project
scheduling overlaps work, and safely centralizing validation needs a real
dependency model. Host processor load was 4% before and after the experiment.

Loading the original packaging functions from their AST, without executing
the packaging entry point, measured:

| Original function | Seconds | Result count |
| --- | ---: | ---: |
| `Get-ProjectOutputMap` | 9.460 | 1268 |
| `Get-RegistryInfSources` (includes another map scan) | 8.653 | 36 |
| `Get-CdDeclarations` (includes another map scan) | 171.753 | 3267 |

Declaration interpretation is therefore a measured substantial packaging
cost, not just a suspicion about PowerShell. These nested timings must not be
added as if every scan were independent work in the real packaging invocation.
They are single cached-path samples, not cabinet/ISO-tool timings.

A separately logged full incremental experiment is still required to
distinguish stable traversal overhead from rebuilding caused by the corrected
generator executables. Such a run must not be described as a no-op unless its
compiler/generator counts establish that.

## Proposals requiring approval

1. Verify/apply patches once through an explicit shared producer with correct
   dependency and invalidation semantics; do not replace validation with an
   unchecked success stamp.
2. Import explicit message/resource recipes, use evaluated SDK tools, and track
   actual outputs rather than probing unrelated projects or reparsing CMake.
3. Generate/reuse a packaging declaration manifest and output map while
   retaining unresolved-input, layout, and exact PE checks.
4. Correct host-tool solution configuration mapping and validate from clean
   outputs before removing the separate host-tool build.
5. Benchmark scheduling/priority and compiler-host choices separately, using
   equal source/tool/cache/target/validation scope. Do not claim a projected
   speedup from logical-processor counts alone.

None of these proposals has been implemented. Correctness fixes to decorated
C++ exports and KD packet handling are distinct from build optimizations.
