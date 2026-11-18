# Kundows

## Build

- Use the latest MSVC toolchain and native `.slnx` projects.
- Do not use CMake, Ninja, or RosBE as the build entry point.

## Upstream

- ReactOS is at `Upstream\ReactOS\Source`.
- Its baseline is in `Upstream\ReactOS\Projects\BuildProjects.json`.
- Do not commit changes to the ReactOS submodule; put Kundows changes in
  `Build\Patches`.
- Keep the ReactOS submodule at an explicit upstream commit and apply the
  Kundows patch set when preparing a build.
- When synchronizing upstream, review changes that affect generated sources,
  project metadata, packaging declarations, registry hives, boot images, or
  other build inputs, and update the corresponding Kundows adapter or patch.
- If a Kundows change appears suitable for ReactOS upstream, submitting it is
  optional. Before doing so, review the upstream contribution requirements and
  confirm that the change satisfies its source-provenance, clean-room, and
  other applicable requirements.

ISO packaging must use the upstream packaging declarations as its reference
without introducing a separately maintained duplicate file manifest. The
Kundows image-build workflow should generate its temporary file lists and
packaging inputs from those declarations.

## Commit message

Use:

```text
[SCOPE] Summary
[SCOPE1][SCOPE2] Summary
```

Rules:

- Use uppercase scopes; use `:` for sub-scopes, such as `[NTDLL:LDR]`.
- Use one commit for one change.
- Use an imperative summary without a period; explain why in the commit body.
- Add a new scope to `Scopes.json` when needed.
