"""Pair VHDMP activities and locate I/O covering either on-disk FAT mirror."""
import argparse
import csv
import json
from collections import Counter
from pathlib import Path

p = argparse.ArgumentParser(description=__doc__)
p.add_argument("tsv", type=Path)
p.add_argument("geometry", type=Path, help="read-only FAT sample (fat_offset_in_volume, fat_length, sector_bytes)")
p.add_argument("partition_offset", type=int, help="partition offset in bytes from the start of the virtual disk")
p.add_argument("--guest", type=Path, help="same-boot guest writes.tsv, if available")
p.add_argument("--entry", type=int, default=39907, help="FAT32 entry whose sector to follow (default: 39907)")
args = p.parse_args()
geo = json.loads(args.geometry.read_text(encoding="utf-8-sig"))
length = geo["fat_length"]
sector = geo["sector_bytes"]
watch = (args.entry*4)//sector*sector
starts = [args.partition_offset + geo["fat_offset_in_volume"] + i*length for i in range(2)]
assert sector == 512 and 0 <= watch < length
pending, completed, errors = {}, [], []
unmatched = malformed = 0
with args.tsv.open(encoding="utf-8-sig", newline="") as f:
    for e in csv.DictReader(f, delimiter="\t"):
        event = int(e["id"])
        key = e["activity"]
        req = tuple(int(e[k]) for k in ("io", "srb", "offset", "length"))
        time = int(e["utc_ticks"])
        if event == 1001:
            if key in pending or not key:
                malformed += 1
            pending[key] = (time, req)
        elif event == 1002:
            previous = pending.pop(key, None)
            if not previous:
                unmatched += 1
                continue
            if previous[1] != req or previous[0] > time:
                malformed += 1
                continue
            completed.append((previous[0], time, *req))
        else:
            errors.append(e)
writes = [r for r in completed if r[2:4] == (1,1)]
seen = Counter((r[4],r[5]) for r in writes)
mirrors, watched = [], []
for start in starts:
    hits = [r for r in writes if r[4] < start+length and r[4]+r[5] > start]
    mirrors.append(dict(start=start, intersecting_requests=len(hits),
                        intersecting_bytes=sum(min(start+length,r[4]+r[5])-max(start,r[4]) for r in hits)))
    watched.append([{"start":s, "end":e, "relative_start":o-start, "length":n}
                    for s,e,io,srb,o,n in hits if o<=start+watch<o+n])
result = dict(completed_activities=len(completed), missing_start=unmatched,
              missing_stop=len(pending), mismatched_descriptors=malformed,
              recovery_error_events=len(errors), mirrors=mirrors,
              watched_entry=args.entry, watched_sector=watch//sector, watched=watched)
if args.guest:
    with args.guest.open(encoding="ascii", newline="") as f:
        guest = Counter((args.partition_offset+int(row["lbo"]),int(row["length"]))
                        for row in csv.DictReader(f,delimiter="\t"))
    # No common request ID across guest and host: report descriptor multiset
    # coverage, not an invented one-to-one timing or durability guarantee.
    result.update(guest_requests=sum(guest.values()), guest_exact_descriptor_coverage=sum((seen & guest).values()))
print(json.dumps(result,indent=2))
