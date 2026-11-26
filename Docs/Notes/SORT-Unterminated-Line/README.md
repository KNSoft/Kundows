# SORT silently dropped the final character without a trailing newline

Input bytes `zebra\r\nalpha` produced `alph\r\nzebra\r\n`, with exit code 0. A file containing only `Q` produced a blank line. The original reader deleted the last character of every input line, even when it was data rather than `\n`.

[`sort-eof.patch`](../../../Upstream/ReactOS/Patches/Runtime/sort-eof.patch) strips a newline only when one was actually read. The x64 installed and candidate executables were run on identical inputs, with [stdout bytes and exit codes](../../../Tests/ReactOS/Desktop/Evidence/Results.tsv#L2-L6): **3/5 → 5/5**. The ordinary CRLF, empty-file and reverse-sort cases stayed correct. This repair does not change sort order or the input buffer's long-line limit.
