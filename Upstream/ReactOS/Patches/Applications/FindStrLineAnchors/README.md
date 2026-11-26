# FINDSTR accepted position switches but matched arbitrary substrings

**Open; the [candidate](Assets/FindstrAnchors.patch) is not in the build.** The [short-input fixture](Assets/Inputs/colors.txt) contains four CRLF-terminated lines: `red`, `tired`, `red fox` and `redred`. `/B`, `/E` and `/X` set parser flags but the matcher still used a substring search. `/V` could also return the wrong exit status. The [seven short-input cases](Assets/Results.tsv) were also checked on Windows build 26300; the candidate improved **1/7 → 7/7** on the same x64 installation.

The [boundary check](Assets/Run.cmd) disproved that apparent fix. For `/B red` with 1023 `a` bytes followed by `red\r\n`, Windows returned 1 with zero output; the candidate returned 0 and emitted five bytes. For `/E red` with 1020 `a` bytes followed by `redZ\r\n`, Windows again returned 1 with zero output; the candidate returned 0 and emitted 1025 bytes. A `fgets` chunk boundary was mistaken for the start or end of a line.

Run `Assets\Run.cmd` from this directory for the Windows reference, or `Assets\Run.cmd path\to\findstr.exe` for a candidate. It pipes the two chunk-boundary inputs and the regex and two-word controls into that executable and checks its exit codes and output lengths.

The next candidate must retain the logical line boundary across chunks and repeat both controls as well as the short-line suite. Regex and multiword parsing have separate limitations. The short-run score is not release acceptance.
