# SORT silently dropped the final character without a trailing newline

The [two-line input](Assets/Inputs/list.txt) contains `zebra\r\nalpha` with no final newline. SORT printed `alph\r\nzebra\r\n` and exited 0. The [one-byte input](Assets/Inputs/single.txt) contains only `Q` and produced a blank line. The original reader deleted the last character of every input line, even when it was data rather than `\n`.

[`SortEof.patch`](SortEof.patch) strips a newline only when one was actually read. The [stdout bytes and exit codes](Assets/Results.tsv) went from **3/5 to 5/5** on the same x64 installation; the five cases were also checked on Windows build 26300. The ordinary CRLF, empty-file and reverse-sort cases stayed correct. This repair does not change sort order or the input buffer's long-line limit.
