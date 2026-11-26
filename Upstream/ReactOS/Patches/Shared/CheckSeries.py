"""Check Configure's explicit queues on disposable Git indices; no checkout edits."""
import os
from pathlib import Path
import subprocess
import tempfile

from Series import BASE, ROOT, selected

source = BASE.parent / "Source"
pin = subprocess.check_output(["git", "-C", ROOT, "rev-parse", ":Upstream/ReactOS/Source"], text=True).strip()


with tempfile.TemporaryDirectory(prefix="Kundows-index-") as tmp:
    env = dict(os.environ, GIT_INDEX_FILE=str(Path(tmp) / "index"))

    def git(*args):
        return subprocess.check_output(["git", "-C", source, *args], env=env).strip()

    for arm in (False, True):
        for extra in (False, True):
            patches = selected(arm, extra)
            if os.name == "nt":
                for arch in (("arm64",) if arm else ("amd64", "i386")):
                    cmd = [ROOT / "Build/ReactOSSeries.cmd", "List", arch]
                    listed = subprocess.check_output(cmd + (["--image-fixes"] if extra else []), text=True)
                    assert listed.splitlines() == [p.relative_to(BASE).as_posix() for p in patches]
            git("read-tree", pin)
            git("apply", "--cached", "--whitespace=nowarn", *patches)
            applied = git("write-tree").decode()
            for patch in patches:
                git("apply", "--cached", "-R", "--check", "--whitespace=nowarn", patch)
            for patch in reversed(patches):
                git("apply", "--cached", "-R", "--whitespace=nowarn", patch)
            assert git("write-tree") == git("rev-parse", pin + "^{tree}")
            print(f"{len(patches)} patches, ARM64={arm}, image-fixes={extra}: {applied} apply / recognize / reverse OK")

    # The cache fixes are separate repair points, not a textual patch stack.
    cache = BASE / "Storage/FAT32VolumeFlush/CcBcbOwnership.patch"
    errors = BASE / "Storage/MMFlushWriteErrors/MmFlushStatus.patch"
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
