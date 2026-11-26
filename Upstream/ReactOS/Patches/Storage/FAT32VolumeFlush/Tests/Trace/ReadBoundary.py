"""Compare the aligned diagnostic install's volume flush with pinned bytes and disk writes.

Input directory: closed.vmrs, boundary.bin (KWFLUSH.BIN), far.bin (KWFAR.BIN).
Output is JSON on stdout. Layouts are for the pinned x64 Debug fixture only.
"""
import argparse
import hashlib
import json
import struct
from pathlib import Path

import sys
sys.path.insert(0, str(Path(__file__).resolve().parents[4] / "Shared"))
from SavedGuest import SavedGuest

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("case", type=Path)
parser.add_argument("--provider", type=Path, required=True)
args = parser.parse_args()
phase = struct.Struct("<8I4Q")
packet = struct.Struct("<6Q4I")
fields = "kind sequence runs dirty arg value sample status qpc thread context target".split()


def sha(b):
    return hashlib.sha256(b).hexdigest()


def report(name, layout, magic):
    raw = (args.case / name).read_bytes()
    header = struct.Struct(layout)
    tag, *v = header.unpack_from(raw)
    if tag != magic.ljust(16, b"\0") or len(raw) != header.size + 2048:
        raise ValueError(f"invalid report: {name}")
    return v, [raw[i:i+512] for i in range(header.size, len(raw), 512)]


with SavedGuest(args.provider, args.case/"closed.vmrs") as mem:
    _, groups, n, seq, _, closed, _, _, _, _ = mem.locate(b"KW-FAT-TRACE-V1\0", struct.Struct("<16s6I3Q"), 7)
    if closed != 1 or not 0 < 2*groups == n <= 32768 or seq != 2*n:
        raise ValueError("incomplete mirror capture")
    _, nphase, ns, _, table, samples = mem.locate(b"KW-PHASE-4FBB-1\0", struct.Struct("<16s2I3Q"), 3)
    _, npkt, _, _, rows = mem.locate(b"KW-CLASS-4FBB-1\0", struct.Struct("<16s2I2Q"), 3)
    if not (0 < nphase <= 2048 and 0 < ns <= 256 and 0 < npkt <= 8192):
        raise ValueError("missing or overflowed observer table")
    phases = [dict(zip(fields, row)) for row in phase.iter_unpack(mem.virtual(table, nphase*phase.size))]
    mapped = mem.virtual(samples, ns*512)
    packets = list(packet.iter_unpack(mem.virtual(rows, npkt*packet.size)))

    def ptr(va):
        return int.from_bytes(mem.virtual(va, 8), "little")

    def driver(device):
        desc = ptr(device+8)+0x38  # _DEVICE_OBJECT.DriverObject; _DRIVER_OBJECT.DriverName
        size, cap = struct.unpack("<HH", mem.virtual(desc, 4))
        if not (0 < size <= cap <= 2048 and not size % 2):
            raise ValueError("unexpected driver name descriptor")
        return mem.virtual(ptr(desc+8), size).decode("utf-16-le")

    # Verify the identities before using offsets from the matching atapi PDB.
    pairs = {p[2:4] for p in packets}
    if len(pairs) != 1:
        raise ValueError("ambiguous class/port pair")
    fdo, lower = pairs.pop()
    if (driver(fdo), driver(lower)) != ("\\Driver\\Disk", "\\Driver\\atapi"):
        raise ValueError("unexpected class/port pair")
    target, = {p["target"] for p in phases}
    if driver(target) != "\\Driver\\partmgr":
        raise ValueError("unexpected FAT target")
    ext = ptr(lower+0x40)
    model = mem.virtual(ext+0x400, 41).split(b"\0")[0].decode("ascii")
    sector, transport = struct.unpack("<II", mem.virtual(ext+0xa8, 8))
    device_flags, = struct.unpack("<I", mem.virtual(ext+0xb4, 4))
    selected, = struct.unpack("<I", mem.virtual(ext+0x470, 4))
    if sector != 512 or model.strip() != "Virtual HD":
        raise ValueError("unexpected disk geometry/model")


def measure(entry, tid, clocks, statuses, counts, data, far):
    if (any(statuses) or counts != [512]*4 or not clocks[0] or clocks != sorted(clocks)
            or clocks[2] >= clocks[3]):
        raise ValueError("flush, reads or collection timestamps invalid")
    a, b = clocks[2:4]
    off = 1048576 + 16384 + 4*entry
    vbo = off-1048576
    observed = [x for x in phases if a <= x["qpc"] <= b and x["thread"] == tid
                and ((x["kind"] >= 20) == far)]
    if not observed or observed[0]["kind"] != (28 if far else 8):
        raise ValueError("flush entry does not match installer window")
    if far and any(x["arg"] != vbo for x in observed):
        raise ValueError("kernel and installer selected different sectors")
    if len({x["context"] for x in observed}) != 1:
        raise ValueError("ambiguous flush context")
    allocations = [x for x in phases if x["kind"] == (30 if far else 10)
                   and x["thread"] == tid and 0 < x["sample"] <= ns and x["qpc"] < a
                   and (x["arg"] == entry if far else x["arg"] <= entry+127 and x["value"] >= entry)]
    if not allocations:
        raise ValueError("matching pinned allocation sample missing")
    allocation = max(allocations, key=lambda x: x["qpc"])
    s = allocation["sample"]
    sample = mapped[(s-1)*512:s*512]
    if far and [k & 0x0fffffff for k in struct.unpack("<128I", sample)] != list(range(entry+1, entry+129)):
        raise ValueError("pinned sector is not the expected allocation chain")
    covered = []
    for t, start, _, _, _, info, length, srb_len, srb_status, io_status in packets:
        for copy, loc in enumerate([off, off+8371200], 1):
            if start <= loc and loc+512 <= start+length:
                covered.append(dict(copy=copy, ticks=t-a, offset=start, requested=length,
                                    srb_bytes=srb_len, info=info, srb_status=srb_status, status=io_status))
    return dict(sector=entry//128, disk_offsets=[off, off+8371200], statuses=statuses,
                read_bytes=counts, elapsed_ticks=b-a, pinned_ticks=allocation["qpc"]-a,
                read_start_ticks=[clocks[i]-a for i in (0,1,4,5)],
                pinned_sha256=sha(sample), read_sha256=list(map(sha, data)),
                after_matches=[d==sample for d in data[2:]], covering_completions=covered,
                phases=[dict(kind=x["kind"], ticks=x["qpc"]-a, dirty=x["dirty"], runs=x["runs"],
                             sequence=x["sequence"], status=x["status"]) for x in observed])


near, near_data = report("boundary.bin", "<16s12I7Q", b"KW-USER-4FBB-V1")
far, far_data = report("far.bin", "<16s16I6Q", b"KW-FAR-4FBB-V1")
entry, first, last = far[9:12]
if entry != 69632 or not first <= entry < entry+128 <= last:
    raise ValueError("wrong allocation extent for the aligned experiment")
result = dict(mirror_groups=groups, mirror_writes=n, class_packets=npkt,
              packet_errors=sum(p[8] != 1 or p[9] != 0 for p in packets),
              packet_length_mismatches=sum(not p[5] == p[6] == p[7] for p in packets),
              model=model, transport=hex(transport), device_flags=hex(device_flags),
              selected_modes=hex(selected), extent=[first,last],
              near=measure(39808, near[12], near[13:], near[:8], near[8:12], near_data, False),
              far=measure(entry, near[12], far[16:], far[:9], far[12:16], far_data, True))
print(json.dumps(result, indent=2))
