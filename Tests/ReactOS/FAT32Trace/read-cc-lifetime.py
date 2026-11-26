"""Read the private x64 CC observer table in a saved ReactOS test guest. Outputs TSV."""
import argparse
import csv
import struct
import sys
from pathlib import Path

from saved_guest import SavedGuest

p = argparse.ArgumentParser(description=__doc__)
p.add_argument("vmrs", type=Path)
p.add_argument("--provider", required=True, type=Path)
args = p.parse_args()

kinds = ("", "pin-lookup", "pin-return", "mark-dirty", "repin",
         "ordinary-unpin", "deref", "unpin-repinned", "unlink-repinned", "unlink-ordinary")
row = struct.Struct("<10I4Q16s")
with SavedGuest(args.provider, args.vmrs) as mem:
    _, n, _, _, address = mem.locate(b"KW-CC-LIFE-V1".ljust(16,b"\0"), struct.Struct("<16s2I2Q"), 3)
    if not 0 < n <= 1024:
        raise ValueError(f"empty or truncated table ({n} records)")
    events = list(row.iter_unpack(mem.virtual(address, n*row.size)))

maps, bcbs = {}, {}
w = csv.writer(sys.stdout, delimiter="\t", lineterminator="\n")
w.writerow("seq qpc_delta process tid kind flags map bcb refs pins bcb_dirty view_dirty offset length overlap cover".split())
for i, (kind, flags, refs, pins, dirty, vd, start, length, overlap, cover, clock, m, b, tid, image) in enumerate(events, 1):
    if not 0 < kind < len(kinds) or not clock or not m or not start <= 3435520 < start+length:
        raise ValueError(f"unfinished or invalid record {i}")
    image = image.split(b"\0",1)[0].decode("ascii")
    map_id = maps.setdefault(m, "M"+str(len(maps)+1))
    bcb_id = bcbs.setdefault(b, "B"+str(len(bcbs)+1)) if b else "-"
    w.writerow((i, clock-events[0][10], image, tid, kinds[kind], flags, map_id, bcb_id,
                refs if b else "-", pins if b else "-", dirty if b else "-", vd if b else "-",
                start, length, overlap if kind==1 else "-", cover if kind==1 else "-"))
