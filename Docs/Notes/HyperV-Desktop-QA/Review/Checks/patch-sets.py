"""Check Configure's explicit queues on disposable Git indices; no checkout edits."""
import os
from pathlib import Path
import re
import subprocess
import tempfile

root = Path(__file__).resolve().parents[5]
source = root / "Upstream/ReactOS/Source"
base = root / "Upstream/ReactOS/Patches"
entry = (root / "Build/ReactOS.cmd").read_text()
sets = [(folder, names.split()) for names, folder in
        re.findall(r"for %%P in \(([^)\r\n]+)\) do call :queuePatch (\w+) %%P", entry)]
optional, = re.findall(r"call :queuePatch Runtime ([\w-]+) \|\|", entry)
pin = subprocess.check_output(["git", "-C", root, "rev-parse", ":Upstream/ReactOS/Source"], text=True).strip()


def path(group, name):
    return base / group / ("ReactOS-" + name + ".patch")


with tempfile.TemporaryDirectory(prefix="Kundows-index-") as tmp:
    env = dict(os.environ, GIT_INDEX_FILE=str(Path(tmp) / "index"))

    def git(*args):
        return subprocess.check_output(["git", "-C", source, *args], env=env).strip()

    for arm in (False, True):
        for extra in (False, True):
            patches = [path(g, n) for g, names in sets if arm or g != "ARM64" for n in names]
            if extra:
                patches.append(path("Runtime", optional))
            git("read-tree", pin)
            git("apply", "--cached", "--whitespace=nowarn", *patches)
            for patch in patches:
                git("apply", "--cached", "-R", "--check", "--whitespace=nowarn", patch)
            for patch in reversed(patches):
                git("apply", "--cached", "-R", "--whitespace=nowarn", patch)
            assert git("write-tree") == git("rev-parse", pin + "^{tree}")
            print(f"{len(patches)} patches, ARM64={arm}, image-fixes={extra}: apply / recognize / reverse OK")

    # The cache fixes are separate repair points, not a textual patch stack.
    cache = path("Runtime", "cc-bcb-ownership")
    errors = path("Runtime", "mm-flush-status")
    cache_tree = None
    for order in ((errors, cache), (cache, errors)):
        git("read-tree", pin)
        for patch in order:
            git("apply", "--cached", "--whitespace=nowarn", patch)
            git("apply", "--cached", "-R", "--check", "--whitespace=nowarn", patch)
        tree = git("write-tree")
        if cache_tree is None:
            cache_tree = tree
        assert tree == cache_tree
        for patch in order:
            git("apply", "--cached", "-R", "--check", "--whitespace=nowarn", patch)
        for patch in reversed(order):
            git("apply", "--cached", "-R", "--whitespace=nowarn", patch)
        assert git("write-tree") == git("rev-parse", pin + "^{tree}")
    print("Cache ownership / write-error fixes: either order, standalone recognition and complete reverse OK")
