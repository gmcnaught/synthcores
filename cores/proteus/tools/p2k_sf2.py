#!/usr/bin/env python3
"""Read a SoundFont 2 file: samples (with their PCM), instruments and presets, generators by number.

  p2k_sf2.py FILE.sf2      summary

Generators used by the Composer builder (SF2.04 section 8.1.2): 17 pan (0.1 %), 41 instrument,
43 keyRange and 44 velRange (lo, hi), 48 initialAttenuation (cB), 51 coarseTune (semitones),
52 fineTune (cents), 53 sampleID, 54 sampleModes (1, 3 = looped), 58 overridingRootKey.
"""
import struct
import sys


def _chunks(d, off, end):
    while off + 8 <= end:
        cid = d[off:off + 4]
        ln = struct.unpack_from("<I", d, off + 4)[0]
        yield cid, off + 8, ln
        off += 8 + ln + (ln & 1)


def load(path):
    with open(path, "rb") as f:
        d = f.read()
    if d[:4] != b"RIFF" or d[8:12] != b"sfbk":
        raise ValueError(f"{path}: not a SoundFont 2 file")
    sub = {}
    for cid, o, ln in _chunks(d, 12, len(d)):
        if cid == b"LIST":
            for c2, o2, l2 in _chunks(d, o + 4, o + ln):
                sub[d[o:o + 4] + b"/" + c2] = (o2, l2)

    def recs(key, size, fmt):
        o, ln = sub[key]
        return [struct.unpack_from(fmt, d, o + k * size) for k in range(ln // size)]

    def name(b):
        return b.split(b"\0")[0].decode("latin-1")

    def gens(gl, a, b):
        g = {}
        for op, amt in gl[a:b]:
            g[op] = (amt & 0xFF, (amt >> 8) & 0xFF) if op in (43, 44) else amt
        return g

    phdr = recs(b"pdta/phdr", 38, "<20sHHHIII")
    pbag = recs(b"pdta/pbag", 4, "<HH")
    pgen = recs(b"pdta/pgen", 4, "<Hh")
    inst = recs(b"pdta/inst", 22, "<20sH")
    ibag = recs(b"pdta/ibag", 4, "<HH")
    igen = recs(b"pdta/igen", 4, "<Hh")
    shdr = recs(b"pdta/shdr", 46, "<20sIIIIIBbHH")
    so, _ = sub[b"sdta/smpl"]
    samples = [dict(name=name(s[0]), start=s[1], end=s[2], ls=s[3], le=s[4], rate=s[5], root=s[6],
                    corr=s[7], link=s[8], type=s[9]) for s in shdr[:-1]]
    instruments = [dict(name=name(inst[k][0]),
                        zones=[gens(igen, ibag[b][0], ibag[b + 1][0]) for b in range(inst[k][1], inst[k + 1][1])])
                   for k in range(len(inst) - 1)]
    presets = [dict(name=name(phdr[k][0]), program=phdr[k][1], bank=phdr[k][2],
                    zones=[gens(pgen, pbag[b][0], pbag[b + 1][0]) for b in range(phdr[k][3], phdr[k + 1][3])])
               for k in range(len(phdr) - 1)]

    def pcm(s):
        """The sample's 16-bit PCM as a list of ints."""
        n = s["end"] - s["start"]
        return list(struct.unpack_from(f"<{n}h", d, so + 2 * s["start"]))

    return dict(samples=samples, instruments=instruments, presets=presets, pcm=pcm)


if __name__ == "__main__":
    s = load(sys.argv[1])
    print(f"{len(s['samples'])} samples, {len(s['instruments'])} instruments, {len(s['presets'])} presets")
    for p in s["presets"][:5]:
        print(" ", p["bank"], p["program"], p["name"])
