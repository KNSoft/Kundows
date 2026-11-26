"""Extract the pinned FastFAT excerpts used by ShortWrite.c."""
from pathlib import Path
import re
import subprocess


def prepare(out, src, pin):
    def read(name):
        return subprocess.check_output(["git", "-C", src, "show", f"{pin}:drivers/filesystems/fastfat/{name}"]).decode()

    dev = read("deviosup.c")
    callback, = re.findall(r"^FatMultiSyncCompletionRoutine\s*\([^;{}]*?\)\s*/\*.*?\*/\s*\{.*?^\}", dev, re.M|re.S)
    write = read("write.c")
    # Keep the real initial IOSB, completion routine, post-wait status check and
    # DirtyFatMcb removal decision. Only generation/dispatch of two child IRPs is simulated.
    start = write.index("        RaiseIosb.Status = STATUS_SUCCESS;", write.index("if (TypeOfOpen == VirtualVolumeFile) {"))
    end = write.index("\n", write.index("RaiseIosb.Information = ByteCount;", start))
    tail = write.index("            FatWaitSync( IrpContext );", end)
    stop = write.index("        return Status;", tail) + len("        return Status;")
    for name, body in (("MirrorCompletion", callback), ("MirrorStatus", write[start:end]), ("MirrorWait", write[tail:stop])):
        (out / (name + ".inc")).write_text(body + "\n", encoding="utf-8")
