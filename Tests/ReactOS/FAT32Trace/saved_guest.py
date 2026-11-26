"""Read-only Windows SDK access to VP 0's saved physical and virtual memory."""
import ctypes as c
from pathlib import Path

P, U, Q = c.c_void_p, c.c_uint32, c.c_uint64


class SavedGuest:
    def __init__(self, provider, state):
        self.lib = c.WinDLL(str(Path(provider).resolve(strict=True)))

        def api(name, *types):
            fn = getattr(self.lib, name)
            fn.restype, fn.argtypes = c.c_int32, types

            def checked(*args):
                hr = fn(*args)
                if hr < 0:
                    raise OSError(f"{name}: {hr & 0xffffffff:08x}")
            return checked

        self.close = api("ReleaseSavedStateFiles", P)
        self.chunks = api("GetGuestPhysicalMemoryChunks", P, c.POINTER(Q), P, c.POINTER(Q))
        self.read = api("ReadGuestPhysicalAddress", P, Q, P, U, c.POINTER(U))
        self.translate = api("GuestVirtualAddressToPhysicalAddress", P, U, Q, c.POINTER(Q), P)
        self.h = P()
        api("LoadSavedStateFile", c.c_wchar_p, c.POINTER(P))(str(Path(state).resolve(strict=True)), c.byref(self.h))

    def __enter__(self):
        return self

    def __exit__(self, *args):
        self.close(self.h)

    def physical(self, addr, n):
        b, got = (c.c_ubyte*n)(), U()
        self.read(self.h, addr, b, n, c.byref(got))
        if got.value != n:
            raise OSError("short physical read")
        return bytes(b)

    def pa(self, va):
        address = Q()
        self.translate(self.h, 0, va, c.byref(address), None)
        return address.value

    def virtual(self, va, length):
        out = bytearray()
        while length:
            n = min(length, 4096-va % 4096)
            out += self.physical(self.pa(va), n)
            va += n
            length -= n
        return bytes(out)

    def locate(self, signature, header, self_index):
        """Ignore unmapped file images containing the tag by checking the live self pointer."""
        page, count = Q(), Q(128)
        layout = (Q*2*count.value)()
        self.chunks(self.h, c.byref(page), layout, c.byref(count))
        if count.value > len(layout) or page.value != 4096:
            raise ValueError("unexpected guest layout")
        for base, pages in layout[:count.value]:
            end = (base+pages)*4096
            for pos in range(base*4096, end, 1 << 20):
                buf = self.physical(pos, min((1 << 20)+len(signature)-1, end-pos))
                off = buf.find(signature)
                while off >= 0:
                    try:
                        addr = pos+off
                        row = header.unpack(self.physical(addr, header.size))
                        va = row[self_index]
                        if va >= 0xffff800000000000 and self.pa(va) == addr and self.virtual(va, len(signature)) == signature:
                            return row
                    except OSError:
                        pass
                    off = buf.find(signature, off+1)
        raise ValueError(f"no live trace header {signature!r}")
