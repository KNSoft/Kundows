"""Validate a cccycle controller log before reporting its check counts."""
import argparse
from pathlib import Path
import re

P = argparse.ArgumentParser(description=__doc__)
P.add_argument("logs", nargs="+", type=Path)
args = P.parse_args()
base_expected = {1: 4, 2: 5, 3: 7, 4: 5, 5: 9, 6: 4, 7: 2, 8: 3}
print("log\tcase\tname\tpassed\ttotal\tfailed_checks")
for log in args.logs:
    text = log.read_text(encoding="ascii")
    expected = dict(base_expected)
    if re.search(r"^BEGIN 9 ", text, re.M):
        expected[9] = 5
    if re.search(r"^BEGIN 10 ", text, re.M):
        expected[10] = 10
    rows, ends, names = {}, {}, {}
    for line in text.splitlines():
        m = re.fullmatch(r"(\d+)\t([a-z-]+)\t(\d+)\t([0-9a-f]{8})\t([0-9a-f]{8})\t(PASS|FAIL)", line)
        if m:
            case, name, check, actual, want, result = m.groups()
            case, check = int(case), int(check)
            names.setdefault(case, name)
            if case not in expected or names[case] != name:
                raise ValueError("unknown or inconsistent case")
            checks = rows.setdefault(case, {})
            if check in checks or check < 1 or check > expected[case]:
                raise ValueError("duplicate or out-of-range check")
            passed = actual == want
            if (result == "PASS") != passed:
                raise ValueError("incorrect printed verdict")
            checks[check] = passed
        m = re.fullmatch(r"END (\d+) status=([0-9a-f]{8}) reads=(\d+) writes=(\d+) errors=(\d+) lazy=(\d+)", line)
        if m:
            case = int(m[1])
            if case in ends or int(m[2], 16):
                raise ValueError(f"case {case}: duplicate completion or execution error")
            ends[case] = tuple(map(int, m.groups()[2:]))
    if set(rows) != set(expected) or set(ends) != set(expected):
        raise ValueError(f"{log}: incomplete execution, not a scored {len(expected)}-case run")
    total = failed = 0
    for case, count in expected.items():
        checks = rows[case]
        if len(checks) != count:
            raise ValueError("missing check")
        bad = [str(k) for k in sorted(checks) if not checks[k]]
        print(log.name, case, names[case], count-len(bad), count,
              ",".join(bad) or "-", sep="\t")
        total += count
        failed += len(bad)
    summary, = re.findall(r"^SUMMARY checks=(\d+) failed=(\d+) execution_errors=(\d+)$", text, re.M)
    if tuple(map(int, summary)) != (total, failed, 0):
        raise ValueError("incorrect summary")
    exit_code, = re.findall(r"^APP_EXIT=(\d+)\s*$", text, re.M)
    if int(exit_code) != bool(failed):
        raise ValueError("incorrect application exit code")
    if ends[3][2] == 0 or ends[6][3] == 0:
        raise ValueError("error injection or lazy-writer branch was not exercised")
    print(log.name, "ALL", "-", total-failed, total, failed, sep="\t")
