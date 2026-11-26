# Documentation

- [Build guide](Building.md)
- [Windows reimplementation and Hyper-V verification](windows-reimplementation.md)
- [Regression test entry points](../Tests/ReactOS/README.md)
- [Commit scopes](Scopes.json)

## Notes

- [x64 BootCD: Setup display and hard-disk installation](Notes/BootCD-x64-Setup/README.md)
- [Choosing the Windows DLL export level](Notes/DLL-Export-Version/README.md)

### Storage and cache

| Problem | Status |
|---|---|
| [FAT32 volume flush returned success with both allocation tables still stale](Notes/FAT32-Volume-Flush/README.md) | Fixed in CC; verified at flush return and after remount. |
| [FastFAT requested a clean volume after a filesystem flush returned an error](Notes/FastFAT-Clean-After-Flush-Error/README.md) | Defensive guard active; error origin and Windows comparison open. |
| [One FAT copy on the original installation disk lost 29 entries](Notes/FAT32-Install-Mirror-Divergence/README.md) | Disk and boot symptoms confirmed; first divergent write not captured. |
| [A failed paging write was counted as successful](Notes/MM-Flush-Write-Errors/README.md) | Fixed on the tested writeback and retry paths. |
| [A duplicate pin could lock the wrong buffer record](Notes/CC-Duplicate-BCB/README.md) | Fixed; covered by the ownership regression. |

### Display and startup

- [win32k read the target relationship before it was delivered](Notes/Monitor-Target-Relation/README.md)
- [Monitor list query returned before its buffered output was delivered](Notes/Monitor-List-Completion/README.md)
- [Video port returned the adapter FDO for a target-device relation](Notes/Video-Target-PDO/README.md) — open
- [GDI initialization failure left the USER lock held](Notes/USER-Init-Unlock/README.md)

### Applications

- [SORT dropped the last character of an unterminated line](Notes/SORT-Unterminated-Line/README.md)
- [WHERE searched the current directory outside an explicit scope](Notes/WHERE-Scoped-Search/README.md)
- [Notepad chose CR when LF was the most frequent ending](Notes/Notepad-Mixed-Line-Endings/README.md)
- [FINDSTR ignored line anchors](Notes/FINDSTR-Line-Anchors/README.md) — candidate rejected by long-line controls

Keep one note per independently observable problem. Record its symptom, evidence,
fix or open question, and verification limits. Keep shared procedures in the
guides and link to exact test inputs and outputs. Keep full logs, dumps and
disk images outside Git.

## Commit messages

Use `[SCOPE] Summary` or `[SCOPE1][SCOPE2] Summary`. Write scopes in uppercase;
use `:` for a sub-scope such as `[NTDLL:LDR]`. Keep one change per commit,
use an imperative summary without a period, and explain why in the body.
Add new scopes to [Scopes.json](Scopes.json).
