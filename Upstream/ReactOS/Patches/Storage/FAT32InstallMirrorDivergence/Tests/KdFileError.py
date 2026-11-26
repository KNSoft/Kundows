"""Identify the file in a saved first-chance FAT assertion on the original x64 QA build."""
import argparse
import ctypes as c
import hashlib
import json
from pathlib import Path
import struct


def check(ok, explanation):
    if not ok:
        raise ValueError(explanation)


p = argparse.ArgumentParser(description=__doc__)
p.add_argument("vmrs", type=Path, help="saved state from the one-CPU Hyper-V fixture")
p.add_argument("--provider", type=Path, required=True, help="Windows SDK x64 vmsavedstatedumpprovider.dll")
args = p.parse_args()
lib = c.WinDLL(str(args.provider.resolve(strict=True)))
P, U, Q = c.c_void_p, c.c_uint32, c.c_uint64


def api(name, *types):
    fn = getattr(lib, name)
    fn.restype, fn.argtypes = c.c_int32, types

    def checked(*args):
        hr = fn(*args)
        if hr < 0:
            raise OSError(f"{name}: 0x{hr & 0xffffffff:08x}")
    return checked


load = api("LoadSavedStateFile", c.c_wchar_p, c.POINTER(P))
close = api("ReleaseSavedStateFiles", P)
translate = api("GuestVirtualAddressToPhysicalAddress", P, U, Q, c.POINTER(Q), P)
physical = api("ReadGuestPhysicalAddress", P, Q, P, U, c.POINTER(U))
reg = api("GetRegisterValue", P, U, U, P)
handle = P()


def read(va, size):
    data = bytearray()
    while size:
        n = min(size, 4096 - va % 4096)
        pa, buf, got = Q(), (c.c_ubyte*n)(), U()
        translate(handle, 0, va, c.byref(pa), None)
        physical(handle, pa.value, buf, n, c.byref(got))
        check(got.value == n, "short saved-state read")
        data += bytes(buf)
        va += n
        size -= n
    return data


def number(va, n=8):
    return int.from_bytes(read(va, n), "little")


def register(idx):
    out = (Q*4)()
    reg(handle, 0, idx, out)
    return out[0]


def string(desc):
    if not desc:
        return b""
    n = number(desc, 2)
    check(n <= 4096, "unexpected KD message size")
    return read(number(desc+8), n)


def identify():
    # KD packet on the saved CPU stack. Verify the actual KdSendPacket
    # instructions, not just a possible return address in the serial driver.
    sp = register(4)  # X64_RegisterRsp
    frames = []
    for i, (ret,) in enumerate(struct.iter_unpack("<Q", read(sp, 0x300))):
        if ret < 0xffff800000000000 or ret & 0xfff != 0x9f4:
            continue
        base = ret - 0x19f4
        try:
            sig = hashlib.sha256(read(base+0x1880, 36)).hexdigest()
            if sig == "44c6e9a87ffd4c1d026d1012b85246903a04e7df6238d04c59440f3d4423aca0" and \
               read(ret-5, 5) == bytes.fromhex("e87cfaffff"):
                frames.append(sp+8*i+8)
        except OSError:
            continue
    check(len(frames) == 1, "no unique KdSendPacket ACK frame from this build")
    frame = frames[0]
    leader, kind, n, _, checksum = struct.unpack("<IHHII", read(frame+0x38, 16))
    header, body = (string(number(frame+off)) for off in (0x88, 0x90))
    check(leader == 0x30303030 and kind == number(frame+0x80, 4) == 7 and
          len(header)+len(body) == n and (sum(header)+sum(body)) & 0xffffffff == checksum,
          "not a complete KD_STATE_CHANGE64 packet")
    code = struct.unpack_from("<I", header, 0x20)[0]
    check(struct.unpack_from("<I", header)[0] == 0x3030 and code == 0xc0000420 and
          struct.unpack_from("<I", header, 0xb8)[0] == 1, "not the first-chance assertion")

    # KdpReport copies the exception context to VP 0's PRCB. Check PC against
    # both addresses in the message before using the saved assert-time RSP.
    pcr = register(61)  # X64_RegisterGs, segment Base
    check(number(pcr+0x18) == pcr, "not the kernel PCR")
    context = number(pcr+0x20) + 0x120
    pc, s = number(context+0xf8), number(context+0x98)
    check(pc == struct.unpack_from("<Q", header, 0x18)[0] ==
          struct.unpack_from("<Q", header, 0x30)[0], "KD and saved context disagree")
    fn = pc-0xf8
    check(hashlib.sha256(read(fn, 0x114)).hexdigest() ==
          "be28adf8fd611b58a78095abd309e77ea1ce300fbd87e0b75b83797783cd4698",
          "not the original IoRaiseInformationalHardError")
    ret = number(s+0x48)
    check(read(ret-6, 2) == b"\xff\x15" and
          number(ret + struct.unpack("<i", read(ret-4, 4))[0]) == fn, "unexpected import call")
    fat = ret-0x51440
    check(hashlib.sha256(read(fat+0x51380, 0xc7)).hexdigest() ==
          "69c58b50ecea1456151d06606c8620d5c40593707d7fe51641931f1db00233dc",
          "not the original FatPopUpFileCorrupt")
    parent = number(s+0x88)
    check(parent == fat+0x2d8a5 and read(parent-5, 5) == bytes.fromhex("e8db3a0200"),
          "not the observed FatLookupFileAllocation chain-break call")
    status, desc = number(s+0x50, 4), number(s+0x58)
    length, maximum = struct.unpack("<HH", read(desc, 4))
    check(status == 0xc0000102 and 0 < length <= maximum <= 1024 and length % 2 == 0,
          "invalid hard-error arguments")
    return dict(kd_packet_checksum_verified=True, first_chance=True, exception=hex(code),
                location="IoRaiseInformationalHardError+0xf8", io_status=hex(status),
                file=read(number(desc+8), length).decode("utf-16-le"),
                callers=["FatPopUpFileCorrupt+0xc0", "FatLookupFileAllocation+0x4a5"])


load(str(args.vmrs.resolve(strict=True)), c.byref(handle))
try:
    print(json.dumps(identify(), indent=2))
finally:
    close(handle)
