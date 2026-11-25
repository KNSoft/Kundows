# Documentation

- [Build guide](Building.md)
- [Windows reimplementation and Hyper-V verification](windows-reimplementation.md)
- [Commit scopes](Scopes.json)

## Notes

- [x64 BootCD: Setup display and hard-disk installation](Notes/BootCD-x64-Setup/README.md)
- [Choosing the Windows DLL export level](Notes/DLL-Export-Version/README.md)
- [Hyper-V desktop QA: installation evidence and application regressions](Notes/HyperV-Desktop-QA/README.md)
- [Writeback, locking and FAT clean-state fixes: decisions and regression checks](Notes/HyperV-Desktop-QA/Review/Verified.md)
- [FAT32 mirror writes: guest submissions, completions and Hyper-V readback](Notes/HyperV-Desktop-QA/FatTrace/README.md)

Keep a note when it explains a decision or records evidence useful for a future
regression. Use one directory per topic, with a `README.md` and any supporting
screenshots in `Images/`. Record the tested revision and the result; keep raw
logs, dumps and disk images outside Git.

## Commit messages

Use `[SCOPE] Summary` or `[SCOPE1][SCOPE2] Summary`. Write scopes in uppercase;
use `:` for a sub-scope such as `[NTDLL:LDR]`. Keep one change per commit,
use an imperative summary without a period, and explain why in the body.
Add new scopes to [Scopes.json](Scopes.json).
