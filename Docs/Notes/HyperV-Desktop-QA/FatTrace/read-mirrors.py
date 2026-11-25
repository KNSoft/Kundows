"""Read both FAT32 tables and the PALAI.TTF directory entry from a mounted QA copy."""
import argparse
from hashlib import sha256
import json
from pathlib import Path
import re
import struct

p = argparse.ArgumentParser(description=__doc__)
p.add_argument("volume", help=r"the read-only clone's \\?\Volume{GUID}, without a drive letter")
p.add_argument("output", type=Path)
a = p.parse_args()
volume = a.volume.rstrip("\\")
if not re.fullmatch(r"\\\\\?\\Volume\{[0-9a-fA-F-]{36}\}", volume):
    p.error("expected a volume GUID")
u16 = lambda b, o: struct.unpack_from("<H", b, o)[0]
u32 = lambda b, o: struct.unpack_from("<I", b, o)[0]
digest = lambda b: sha256(b).hexdigest()
with open(volume, "rb", buffering=0) as f:
    def read(pos, size):
        f.seek(pos)
        b = f.read(size)
        if len(b) != size:
            raise ValueError("short volume read")
        return b
    b = read(0,512)
    assert b[82:90] == b"FAT32   " and b[16] == 2 and u16(b,11)==512 and b[13] != 0
    before, size, cluster = 512*u16(b,14), 512*u32(b,36), 512*b[13]
    assert 0 < size <= 64*1024*1024
    fats = [read(before+i*size, size) for i in (0,1)]
    data = before+2*size
    def chain(start, fat):
        blocks, seen = [], set()
        while 2 <= start < len(fat)//4 and start not in seen:
            seen.add(start); blocks.append(start)
            start = u32(fat,4*start) & 0x0fffffff
        return blocks, start
    def entry(start, name):
        blocks, end = chain(start, fats[0])
        assert end >= 0xffffff8
        for c in blocks:
            block = read(data+(c-2)*cluster, cluster)
            for i in range(0, cluster, 32):
                e = block[i:i+32]
                if not e[0]:
                    raise ValueError(name)
                if e[:11] == name:
                    return (u16(e,20)<<16)|u16(e,26), u32(e,28)
        raise ValueError(name)
    directory, _ = entry(u32(b,44), b"REACTOS    ")
    fonts, _ = entry(directory, b"FONTS      ")
    first, length = entry(fonts,b"PALAI   TTF")
    diffs = [i for i,(x,y) in enumerate(zip(struct.iter_unpack("<I",fats[0]),struct.iter_unpack("<I",fats[1]))) if x != y]
    chains = [chain(first, fat) for fat in fats]
    result = dict(fat_offset_in_volume=before,fat_length=size,sector_bytes=512,
                  fat_sha256=list(map(digest,fats)), cluster_bytes=cluster,
                  differing_entries=len(diffs), first_differences=diffs[:64],
                  palai_first=first,palai_length=length,palai_clusters=[len(s) for s,_ in chains],
                  palai_terminators=[hex(e) for _,e in chains],sector_sha256={})
    for index in {39907//128, first//128}:
        result["sector_sha256"][str(index)] = [digest(fat[index*512:(index+1)*512]) for fat in fats]
with a.output.open("x",encoding="utf-8") as dest:
    json.dump(result,dest,indent=2)
    dest.write("\n")
