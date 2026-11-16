# Kundows

## Build

- Use the latest MSVC toolchain and native `.slnx` projects.
- Do not use CMake, Ninja, or RosBE as the build entry point.

## Upstream

- ReactOS is at `Upstream\ReactOS\Source`.
- Its baseline is in `Upstream\ReactOS\Projects\BuildProjects.json`.
- Do not commit changes to the ReactOS submodule; put Kundows changes in
  `Build\Patches`.

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
