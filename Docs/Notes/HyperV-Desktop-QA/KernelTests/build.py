"""Build the isolated x64 guest tests; execute from an x64 MSVC prompt."""
from pathlib import Path
import re
import subprocess
import argparse

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
P = argparse.ArgumentParser(description=__doc__)
P.add_argument("--out", type=Path, default=ROOT / "OutDir/CcCycle-4fbb-v3")
OUT = P.parse_args().out.resolve()
WD = ROOT / "OutDir/amd64/Debug/drivers/base/null"
TLOG = WD / "null.dir/Debug/null.tlog"
OUT.mkdir(exist_ok=True)
lines = (TLOG / "CL.command.1.tlog").read_text(encoding="utf-16").splitlines()
cmds = [lines[i + 1] for i, s in enumerate(lines) if s.startswith("^") and s.endswith(r"\NULL.C")]
if len(cmds) != 1:
    raise ValueError("expected one existing null driver compile command")
cl = cmds[0]
cl, n = re.subn(r"\s+E:\\[^\r\n]+\\NULL\.C$", "", cl, flags=re.I)
assert n == 1
cl = re.sub(r"/(?:Yu|Fp|Fd|Fo)\"[^\"]*\"", "", cl)
cl = cl.replace("/Zi ", "/Z7 ").replace("/WX- ", "/WX ")
cl += f' /I"{HERE}" /Fo"{OUT}/cccycle.obj" "{HERE}/cccycle.c"'
link = (TLOG / "link.command.1.tlog").read_text(encoding="utf-16").splitlines()[1:]
for flag, path in (("OUT", OUT / "cccycle.sys"), ("PDB", OUT / "cccycle.pdb"),
                   ("IMPLIB", OUT / "cccycle.lib")):
    link[0], n = re.subn(r"/" + flag + r':"[^"]*"', lambda _: f'/{flag}:"{path}"',
                        link[0], flags=re.I)
    assert n == 1
link[0] = re.sub(r"/ENTRY:\"[^\"]+\"", '/ENTRY:"DriverEntry"', link[0], flags=re.I)
# Retain the same import/static libraries, not null's code or resource objects.
link = [link[0]] + [s for s in link[1:] if not s.upper().endswith((".OBJ", ".RES"))]
link[0] = re.sub(r"\s+[^\s]+\.RES\b", "", link[0], flags=re.I)
link.append(f'"{OUT}/cccycle.obj"')
for exe, data in (("cl", cl), ("link", "\n".join(link))):
    rsp = OUT / f"{exe}.rsp"
    rsp.write_text(data + "\n", encoding="ascii")
    with (OUT / f"{exe}.log").open("wb") as log:
        subprocess.run([exe, "@" + str(rsp)], cwd=WD, stdout=log,
                       stderr=subprocess.STDOUT, check=True)
subprocess.run(["cl", "/nologo", "/W4", "/WX", "/TC", "/MT", "/Od",
                str(HERE / "cccycle-user.c"), "/Fe:" + str(OUT / "cccycle.exe"),
                "/Fo:" + str(OUT / "cccycle-user.obj"), "/link", "advapi32.lib"],
               cwd=OUT, check=True)
print(OUT)
