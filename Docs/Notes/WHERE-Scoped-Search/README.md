# WHERE searched the current directory even when given an explicit scope

With `review.qa` in both `C:\HVQA\Work` and `C:\HVQA\Scope`, running `where C:\HVQA\Scope:review.qa` from `Work` printed both paths. Worse, `where /Q C:\HVQA\Scope:onlycwd.qa` returned 0 for a file present only in `Work`. The helper prepended the current directory even for explicit directory and `$variable:` searches.

[`where-scoped.patch`](../../../Upstream/ReactOS/Patches/Runtime/where-scoped.patch) adds the current directory only for the unscoped search. The explicit directory, named variable, explicit `$PATH:`, quiet negative and ordinary search cases went **1/5 → 5/5**. [Exact path lists and exit codes](../../../Tests/ReactOS/Desktop/Evidence/Results.tsv#L14-L18). The quiet negative now returns 1.
