"""Compare the recorded Windows DDI, file and FAT observations to the installed guest."""
import json
from pathlib import Path

EVIDENCE = Path(__file__).resolve().parents[1] / "Evidence/CcTruncate"


def read(path):
    return json.loads(path.read_bytes())


def require(equal, label):
    if not equal:
        raise ValueError(label)


def compare():
    base = EVIDENCE / "Native-9457"
    win = read(base / "windows-ddi.json")
    dbg = read(base / "windbg-trace.json")
    ros = read(base / "reactos-guest.json")
    build = read(base / "checks.json")["cleanX64Debug"]
    media = build["isoMembersMatchLinks"]
    require((ros["installed"]["iso"], ros["installed"]["kernel"], ros["installed"]["fastfat"])
            == (build["isoSha256"], media["system32/ntkrnlmp.exe"], media["system32/drivers/fastfat.sys"]),
            "installed guest does not match the tested ISO")
    require(ros["installed"]["tools"]["DDI10.SYS"] == win["testDriverSha256"] == dbg["driverSha256"],
            "main DDI driver differs")
    require({r["name"]: r["valueHex"] for r in dbg["valuesInOrder"]} == win["valuesHex"],
            "WinDbg trace differs from the Windows driver results")
    for name in ("MappedTailStatus", "SharedTailStatus"):
        require(win["valuesHex"][name] == "c0000435", name)
    for name in ("Finished", "NextGrowthStatus", "SharedNextGrowthStatus"):
        require(win["valuesHex"][name] == "00000000", name)
    require(ros["ddi"]["main"] == win["valuesHex"], "main DDI results differ")
    control = win["lastPageControl"]
    require(control["driverSha256"] == ros["installed"]["tools"]["DDI08.SYS"], "control driver differs")
    for key, value in control["windows"].items():
        require(ros["ddi"]["lastPage"].get(key) == value, "last-page control: " + key)
    for key, exe, source in (("sizes", "SDKTEST.EXE", "file-size-contract"),
                             ("flush", "FATFLUSH.EXE", "fatflush"), ("cycle", "FATCYCLE.EXE", "fatcycle")):
        expected = read(EVIDENCE / "Default-9457" / f"guest-fat-{source}.json")
        actual = ros["programs"][key]
        require(ros["installed"]["tools"][exe] == expected["sha"].lower(), exe)
        require(actual["exitCode"] == expected["rc"] == 0 and actual["stdout"] == expected["stdout"], key)
    print("PASS: installed PEs, both native DDI cases, WinDbg values, file sizes, FAT flush and three remounts")


if __name__ == "__main__":
    compare()
