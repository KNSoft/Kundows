# Notepad converted a mostly-LF document to CR on save

Opening `A\nB\nC\nD\rE`, adding `!` and saving produced `A\rB\rC\rD\rE!`. The original selection logic compared CR against CRLF without checking whether LF exceeded both. The editor could therefore choose a minority line ending.

[`NotepadMajorityEol.patch`](NotepadMajorityEol.patch) corrects that comparison without changing the tested tie behavior. Four GUI cases opened, edited, saved and reread the **file bytes**: **3/4 → 4/4**, including CR, CRLF and CR/LF tie controls. [Inputs and outputs](../../Shared/Desktop/Results.tsv#L19-L22); [the file reopened after cold boot](Assets/ColdNotepad.png). The reference here is the ReactOS selection rule before [the regression](https://github.com/reactos/reactos/commit/0a851eadcb7), not a claim about all versions of Windows Notepad.

The [mixed-ending input](../../Shared/Desktop/Inputs/mixed.txt) preserves all of the original separator bytes.
