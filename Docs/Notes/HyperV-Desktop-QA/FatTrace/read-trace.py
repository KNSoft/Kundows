"""Read the diagnostic Fastfat trace from a saved x64 Hyper-V guest (Windows SDK)."""
import argparse
import ctypes as c
import csv
import hashlib
import json
import struct
from pathlib import Path

p = argparse.ArgumentParser(description=__doc__)
p.add_argument("vmrs", type=Path)
p.add_argument("output", type=Path, help="new directory for table, watched-sector bytes and summary")
p.add_argument("--provider", type=Path, required=True, help="SDK x64 vmsavedstatedumpprovider.dll")
args = p.parse_args()
magic = b"KW-FAT-TRACE-V1\0"
header = struct.Struct("<16s6I3Q")
record = struct.Struct("<10IQ")
P, U, Q = c.c_void_p, c.c_uint32, c.c_uint64
lib = c.WinDLL(str(args.provider.resolve(strict=True)))


def api(name, *types):
    f = getattr(lib, name)
    f.restype, f.argtypes = c.c_int32, types
    return f


load = api("LoadSavedStateFile", c.c_wchar_p, c.POINTER(P))
close = api("ReleaseSavedStateFiles", P)
chunks = api("GetGuestPhysicalMemoryChunks", P, c.POINTER(Q), P, c.POINTER(Q))
read = api("ReadGuestPhysicalAddress", P, Q, P, U, c.POINTER(U))
translate = api("GuestVirtualAddressToPhysicalAddress", P, U, Q, c.POINTER(Q), P)
handle = P()


def check(hr):
    if hr < 0:
        raise OSError(f"saved-state provider: {hr & 0xffffffff:08x}")


def physical(addr, n):
    b, got = (c.c_ubyte*n)(), U()
    check(read(handle, addr, b, n, c.byref(got)))
    if got.value != n:
        raise OSError("short read")
    return bytes(b)


def pa(va):
    address = Q()
    check(translate(handle, 0, va, c.byref(address), None))
    return address.value


def virtual(va, length):
    out = bytearray()
    while length:
        n = min(length, 4096 - va % 4096)
        out += physical(pa(va), n)
        va += n
        length -= n
    return bytes(out)


def locate():
    page, count = Q(), Q(128)
    layout = (Q*2*count.value)()
    check(chunks(handle, c.byref(page), layout, c.byref(count)))
    assert count.value <= len(layout) and page.value == 4096
    for base, pages in layout[:count.value]:
        start, end = base*4096, (base+pages)*4096
        for pos in range(start, end, 1 << 20):
            data = physical(pos, min(1 << 20, end-pos))
            off = data.find(magic)
            while off >= 0:
                addr = pos + off
                try:
                    h = header.unpack(physical(addr, header.size))
                    self = h[-3]
                    if self >= 0xffff800000000000 and pa(self) == addr and virtual(self, 16) == magic:
                        return h
                except OSError:
                    pass
                off = data.find(magic, off+1)
    raise ValueError("no live diagnostic trace found in this saved guest")


check(load(str(args.vmrs.resolve(strict=True)), c.byref(handle)))
try:
    _, groups, count, seq, samples, closed, _, _, table, buf = locate()
    if not (closed == 1 and 0 < groups and 2*groups == count <= 32768 and samples <= 256):
        raise ValueError(f"incomplete capture or capacity exceeded: {groups=} {count=} {samples=} {closed=}")
    rows = list(record.iter_unpack(virtual(table, count*record.size)))
    sectors = virtual(buf, samples*512)
finally:
    check(close(handle))

by_group, events, used = {}, set(), set()
for group, mirror, start, done, vbo, lbo, size, sample, status, _, transferred in rows:
    assert 1 <= group <= groups and mirror in (0, 1) and 0 < start < done <= seq
    assert start not in events and done not in events and size > 0
    events.update((start, done))
    pair = by_group.setdefault(group, {})
    assert mirror not in pair
    pair[mirror] = (vbo, lbo, size, sample)
    if sample:
        assert 1 <= sample <= samples and sample not in used
        used.add(sample)
assert len(events) == seq == 2*count and len(used) == samples
assert len(by_group) == groups
for pair in by_group.values():
    a, b = pair[0], pair[1]
    assert (a[0], a[2]) == (b[0], b[2]) and a[0] == a[1] < b[1]

args.output.mkdir()
with (args.output/"writes.tsv").open("w", newline="", encoding="ascii") as f:
    w = csv.writer(f, delimiter="\t", lineterminator="\n")
    w.writerow("group mirror start done vbo lbo length sample status transferred".split())
    for g,m,s,d,v,l,n,sample,status,_,info in sorted(rows, key=lambda r:r[2]):
        w.writerow((g,m+1,s,d,v,l,n,sample,f"{status:08x}",info))
(args.output/"fat-sectors.bin").write_bytes(sectors)
def payload(i):
    return sectors[512*(i-1):512*i]
watched = [p for p in by_group.values() if p[0][3] or p[1][3]]
summary = dict(groups=groups, writes=count, sequenced_events=seq, watched_groups=len(watched),
               missing_samples=sum(not p[0][3] or not p[1][3] for p in watched),
               divergent_samples=sum(payload(p[0][3]) != payload(p[1][3]) for p in watched if p[0][3] and p[1][3]),
               errors=sum(r[8] != 0 for r in rows), short=sum(r[10] != r[6] for r in rows),
               payload_sha256=hashlib.sha256(sectors).hexdigest())
(args.output/"summary.json").write_text(json.dumps(summary, indent=2)+"\n", encoding="ascii")
print(json.dumps(summary))
