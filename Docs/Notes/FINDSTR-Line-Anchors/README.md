# FINDSTR accepted position switches but matched arbitrary substrings

**Open; the [candidate](Assets/findstr-anchors.patch) is not in the build.** On lines `red`, `tired`, `red fox` and `redred`, `/B`, `/E` and `/X` set parser flags but the matcher still used a substring search. `/V` could also return the wrong exit status. Against the [seven recorded short-input cases](../../../Tests/ReactOS/Desktop/Evidence/Results.tsv#L7-L13) the candidate improved **1/7 → 7/7**.

The [boundary check](Tests/findstr-boundaries.py) disproved that apparent fix. For `/B red` with 1023 `a` bytes followed by `red\r\n`, Windows returned 1 with zero output; the candidate returned 0 and emitted five bytes. For `/E red` with 1020 `a` bytes followed by `redZ\r\n`, Windows again returned 1 with zero output; the candidate returned 0 and emitted 1025 bytes. A `fgets` chunk boundary was mistaken for the start or end of a line.

The next candidate must retain the logical line boundary across chunks and repeat both controls as well as the short-line suite. Regex and multiword parsing have separate limitations. The short-run score is not release acceptance.
