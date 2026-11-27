#!/usr/bin/env python3
"""Build a synthetic Proteus 2000 sound SIMM (Big + Baby images) from WAV samples. Research only.

Formats: docs/p2k-findings.md ("Sound ROM SIMMs", "SIMM detection and the Baby device") and
docs/p2k-lib-formats.md (Baby "lib_" objects). Not a dump: sample order, instrument and preset data
are ours; the PCM comes from whatever WAVs are given.

  p2k_build_simm.py SPEC.json OUTDIR
SPEC:
  {"id": 4, "name": "P2Ksy",
   "instruments": [{"name": "Piano 1", "files": ["path/a.wav", ...], "stereo": false}, ...],
   "presets": [{"name": "kb1:Piano 1", "instrument": 1, "params": {"1410": -20}}, ...]}
  Instrument numbers start at 1; "params" sets layer-0 parameters by SysEx ID (tools/p2k_params.py).
  Presets start from the first user preset of the flash dump ("kb1:DynamicGrand"), layers 1-3 off.
Writes OUTDIR/big.bin (32 MB) and OUTDIR/baby.bin (2 MB); MAME: P2K_SIMM1=big.bin P2K_BABY1=baby.bin.
"""
import json
import math
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import p2k_simm  # noqa: E402
import p2k_wav   # noqa: E402

BIG = 32 << 20
BABY = 2 << 20
LIB = 0x10                 # Baby library offset [O 0x0100DDF4]
SAMPLE_BASE = 0x1000       # first sample byte in Big (after the 0x6AE header)
PAD_WORDS = 64             # zero words after each sample (interpolation look-ahead) [I]
OUT_RATE = 44100           # [I] P2K output rate; ys_1 +48 pitch offset in 1/64 semitone [I]


def obj(typ, oid, data):
    return struct.pack(">4sIHH", typ, 12 + len(data), oid, 0xFFFF) + data


def name16(s):
    return s.encode("latin-1")[:16].ljust(16, b" ")


def place(big, pos, pcm):
    """Write 16-bit samples at byte offset pos; return (start word, end word exclusive, next pos)."""
    for i, v in enumerate(pcm):
        struct.pack_into(">h", big, pos + 2 * i, v)
    start = pos // 2
    end = start + len(pcm)
    return start, end, pos + 2 * (len(pcm) + PAD_WORDS)


def sample_header(name, chans, rate, fine_cents, rom_id):
    """chans: [(start, end, loop_start, loop_end)] word offsets (1 or 2 channels)."""
    looped = chans[0][2] is not None
    h = bytearray(52)
    h[0:16] = name16(name)
    for ch in range(2):
        s, e, ls, le = chans[min(ch, len(chans) - 1)]
        struct.pack_into(">I", h, 16 + 4 * ch, s)
        struct.pack_into(">I", h, 24 + 4 * ch, e)
        struct.pack_into(">I", h, 32 + 4 * ch, ls if looped else s)
        struct.pack_into(">I", h, 40 + 4 * ch, le if looped else e)
    pitch = round(12 * math.log2(rate / OUT_RATE) * 64 + fine_cents * 64 / 100)
    struct.pack_into(">h", h, 48, max(-32768, min(32767, pitch)))
    h[50] = (3 if len(chans) == 2 else 1) | ((1 if looped else 0) << 4)
    h[51] = rom_id
    return bytes(h)


def zone(lo, hi, sample_no, root, rom_id):
    z = bytearray(36)
    z[0:4] = bytes([lo, 0, 0, hi])
    z[4:8] = bytes([0, 0, 0, 127])
    struct.pack_into(">Hh", z, 8, sample_no, 0)
    z[12], z[13], z[14], z[15] = root, 0, 0, 0
    z[34] = rom_id
    return bytes(z)


def instrument(name, zones):
    return name16(name) + bytes([len(zones), 0, 0, 0]) + b"".join(zones)


def preset(template, name, rom_id, inst, params=None):
    p = bytearray(template)
    p[8:24] = name16(name)
    for k, off in enumerate((0xA4, 0x170, 0x23C, 0x308)):
        p[off + 5] = rom_id
        struct.pack_into(">H", p, off + 6, inst if k == 0 else 0)
    if params:
        import p2k_params
        p2k_params.apply(p, params)
    return bytes(p)


def build(spec, template):
    rom_id = spec["id"]
    big = p2k_simm.header(BIG, rom_id)
    pos = SAMPLE_BASE
    ys, yi = [], [instrument("(none)", [])]
    for ins in spec["instruments"]:
        waves = []
        for f in ins["files"]:
            w = p2k_wav.read_wav(f)
            chans = []
            nch = 2 if (ins.get("stereo") and w.get("channels", 1) == 2) else 1
            for c in range(nch):
                pcm = p2k_wav.channel(w, c)
                s, e, pos = place(big, pos, pcm)
                lp = w["loop"]
                chans.append((s, e, s + lp[0] if lp else None, s + lp[1] + 1 if lp else None))
            assert pos < BIG, "Big image full"
            waves.append((w["root"], len(ys)))
            ys.append(sample_header(os.path.basename(f)[:-4], chans, w["rate"],
                                    (w["fine"] * 100) >> 32, rom_id))
        waves.sort()
        zones = []
        for k, (root, no) in enumerate(waves):
            lo = 0 if k == 0 else (waves[k - 1][0] + root) // 2 + 1
            hi = 127 if k == len(waves) - 1 else (root + waves[k + 1][0]) // 2
            zones.append(zone(lo, hi, no, root, rom_id))
        yi.append(instrument(ins["name"], zones))
    big = p2k_simm.finish(big)

    objs, oid = [], 0
    yh = bytearray(48)
    struct.pack_into(">I", yh, 0, rom_id)
    yh[36:42] = spec["name"].encode()[:5].ljust(5, b" ") + b"\0"
    struct.pack_into(">I", yh, 44, rom_id)
    objs.append(obj(b"yh_1", oid, bytes(yh))); oid += 1
    for r in ys:
        objs.append(obj(b"ys_1", oid, r)); oid += 1
    for r in yi:
        objs.append(obj(b"yi_1", oid, r)); oid += 1
    for p in spec.get("presets", []):
        objs.append(obj(b"yp_1", oid, preset(template, p["name"], rom_id, p["instrument"], p.get("params")))); oid += 1
    body = b"".join(objs)
    baby = bytearray(b"\xff" * BABY)
    size = BABY - LIB
    baby[LIB:LIB + 16] = struct.pack(">4s4sIHH", b"lib_", b"_POS", size, 0, 0)
    # at least one 0xFF after the last object (the end marker); a slice past the end would grow baby
    assert LIB + 16 + len(body) < BABY, "Baby image full"
    baby[LIB + 16:LIB + 16 + len(body)] = body    # 0xFF after the last object = end marker [O]
    return big, baby, dict(samples=len(ys), instruments=len(yi) - 1, presets=len(spec.get("presets", [])),
                           big_used=pos, baby_used=LIB + 16 + len(body))


def user_preset_template(flash_path):
    """First pr01 record (980 bytes) of the user bank in the byte-swapped flash dump at 0xA0000."""
    import p2k_lib
    d = open(flash_path, "rb").read()
    _, _, _, _, objs, _ = p2k_lib.walk(d, 0xA0000)
    o = next(o for o in objs if o[1] == "pr01")
    return d[o[0] + 12:o[0] + o[2]]


def main():
    spec = json.load(open(sys.argv[1]))
    out = sys.argv[2]
    os.makedirs(out, exist_ok=True)
    tmpl = user_preset_template(spec.get("flash", "build/p2k/flash124.bin"))
    big, baby, info = build(spec, tmpl)
    open(os.path.join(out, "big.bin"), "wb").write(big)
    open(os.path.join(out, "baby.bin"), "wb").write(baby)
    print(info)


if __name__ == "__main__":
    main()
