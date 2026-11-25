"""Write the manual QA inputs into a new directory; no guest Python is needed."""
from pathlib import Path
import sys

if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise SystemExit("Usage: python make-inputs.py NEW_DIRECTORY")
    root = Path(sys.argv[1])
    root.mkdir()  # Do not overwrite an existing set of files.
    for name, data in {
        "list.txt": b"zebra\r\nalpha",
        "single.txt": b"Q",
        "colors.txt": b"red\r\ntired\r\nred fox\r\nredred\r\n",
        "mixed.txt": b"A\nB\nC\nD\rE",
        "Work/review.qa": b"current",
        "Work/onlycwd.qa": b"current",
        "Scope/review.qa": b"explicit",
    }.items():
        dest = root / name
        dest.parent.mkdir(exist_ok=True)
        dest.write_bytes(data)
