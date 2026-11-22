#!/usr/bin/env python3
"""Read mono/stereo 16-bit WAV files with smpl chunks (root key, loop) for the P2K SIMM builder."""
import struct


def read_wav(path):
    d = open(path, "rb").read()
    assert d[:4] == b"RIFF" and d[8:12] == b"WAVE", path
    p, info = 12, {"loop": None, "root": 60, "fine": 0}
    while p + 8 <= len(d):
        cid, sz = d[p:p + 4], struct.unpack_from("<I", d, p + 4)[0]
        body = d[p + 8:p + 8 + sz]
        if cid == b"fmt ":
            fmt, ch, rate, _, _, bits = struct.unpack_from("<HHIIHH", body)
            assert fmt == 1 and ch in (1, 2) and bits == 16, (path, fmt, ch, bits)
            info["rate"], info["channels"] = rate, ch
        elif cid == b"data":
            n = sz // 2
            pcm = list(struct.unpack_from("<%dh" % n, body))
            info["pcm"] = pcm   # interleaved when stereo; see channel()
        elif cid == b"smpl":
            root, fine = struct.unpack_from("<II", body, 12)
            info["root"], info["fine"] = root, fine
            if struct.unpack_from("<I", body, 28)[0]:
                _, _, start, end = struct.unpack_from("<IIII", body, 36)
                info["loop"] = (start, end)
        p += 8 + sz + (sz & 1)
    return info


def channel(info, c=0):
    """One channel of the sample (stereo files: 0 = left, 1 = right)."""
    ch = info.get("channels", 1)
    return info["pcm"][c::ch] if ch > 1 else info["pcm"]
