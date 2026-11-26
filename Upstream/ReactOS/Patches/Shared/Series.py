"""Read the ordered ReactOS build inputs; never search inside issue attachments."""
from pathlib import Path
import re

BASE = Path(__file__).resolve().parents[1]
ROOT = BASE.parents[2]
MANIFEST = BASE / "Series.tsv"


def selected(arm64=False, image_fixes=False):
    active = {"All"}
    if arm64:
        active.add("ARM64")
    if image_fixes:
        active.add("ImageFixes")
    seen = set()
    paths = []
    for i, line in enumerate(MANIFEST.read_text(encoding="utf-8").splitlines(), 1):
        if not line or line.startswith("#"):
            continue
        row = re.fullmatch(r"(All|ARM64|ImageFixes)\t([A-Z][A-Za-z0-9]*)/([A-Z][A-Za-z0-9]*)/([A-Z][A-Za-z0-9]*\.patch)", line)
        if not row:
            raise ValueError(f"{MANIFEST.name}:{i}: invalid row")
        when, category, issue, filename = row.groups()
        path = BASE / category / issue / filename
        if category == "Shared" or path.parent in seen or not path.is_file() or not (path.parent / "README.md").is_file():
            raise ValueError(f"{MANIFEST.name}:{i}: duplicate issue or missing issue patch: {path}")
        seen.add(path.parent)
        if when in active:
            paths.append(path)
    if not paths:
        raise ValueError("empty patch queue")
    return paths
