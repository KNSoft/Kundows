# WHERE searched the current directory even when given an explicit scope

With `review.qa` in both `C:\HVQA\Work` and `C:\HVQA\Scope`, running `where C:\HVQA\Scope:review.qa` from `Work` printed both paths. Worse, `where /Q C:\HVQA\Scope:onlycwd.qa` returned 0 for a file present only in `Work`. The helper prepended the current directory even for explicit directory and `$variable:` searches.

[`WhereScoped.patch`](WhereScoped.patch) adds the current directory only for the unscoped search. The explicit directory, named variable, explicit `$PATH:`, quiet negative and ordinary search cases went **1/5 → 5/5**. The [returned paths and exit codes](Assets/Results.tsv) were also checked on Windows build 26300. The quiet negative now returns 1.

To reproduce the recorded paths, copy [Work](Assets/Inputs/Work/) and [Scope](Assets/Inputs/Scope/) to `C:\HVQA` and run WHERE from `C:\HVQA\Work`.
