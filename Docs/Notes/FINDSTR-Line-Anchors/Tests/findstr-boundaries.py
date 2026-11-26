"""Feed one FINDSTR executable ASCII stdin; report output length and exit code."""
import argparse
from pathlib import Path
import subprocess

p = argparse.ArgumentParser(description=__doc__)
p.add_argument("exe", type=Path)
args = p.parse_args()
for name, switches, data in (
    ("chunk-start", ["/B", "red"], b"a"*1023 + b"red\r\n"),
    ("chunk-end", ["/E", "red"], b"a"*1020 + b"redZ\r\n"),
    ("regex", ["/R", "/X", "r.d"], b"red\r\n"),
    ("two-words", ["/X", "red blue"], b"red\r\nblue\r\n"),
):
    r = subprocess.run([args.exe.resolve(), *switches], input=data, capture_output=True, timeout=5, check=False)
    print(f"{name}: exit={r.returncode} stdout-bytes={len(r.stdout)}")
