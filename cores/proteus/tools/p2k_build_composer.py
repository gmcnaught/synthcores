#!/usr/bin/env python3
"""Build a Composer (ROM ID 4) stand-in SIMM for the Proteus 2000 from public material. Research only:
not a dump of the E-mu ROM.

Sources (none shipped; docs/p2k-composer.md says where they come from):
  --cmpsr   CMPSR.zip: the 1024 Composer factory presets as SysEx preset dumps (04-NNNN-name_PRES.syx)
  --sf2     the CDXtract SoundFont conversion of the Composer set (9 banks; samples, flattened presets)
  --names   composer_instruments.tsv: "number<TAB>name" from the Proteus 2000 Operations Manual
            (Rev E, Instrument Listing), 1173 instruments
  --records two flash save files (.sav, 2 MB) from the P2K core after OS 2.26 received CMPSR presets
            0-511, then 512-1023, as user presets 0-511 (synth-mister syx-send): the OS's own
            980-byte preset records for every Composer preset

What it does:
  * Presets: the OS-written records, layer instrument ROM ID set to 4, as yp_1 0-1023.
  * Instruments 1-1173 in the manual's numbering and names. Keymaps come from the SF2 presets:
    CDXtract wrote each preset as one SF2 instrument whose zones are the layers' instrument zones
    with the layer's coarse (SysEx 1425) and fine tune (1426) added; transpose, volume and pan of
    the layer are not in them, and a layer's key range clips its zones. Per instrument the zones
    of the best layer that uses it are kept (whole key range first): from presets with one
    instrument, then by sample names already assigned, then by layer key ranges. Instruments
    still without zones get a full-keyboard zone on the SF2 sample of the same name, or none.
  * Samples: every SF2 sample, once (by name and length), at its own rate and root key;
    "-L"/"-R" pairs that an instrument zone uses together become one stereo sample header.

  p2k_build_composer.py --cmpsr CMPSR.zip --sf2 DIR --names T.tsv --records A.sav B.sav OUTDIR
Writes OUTDIR/big.bin, OUTDIR/baby.bin and OUTDIR/report.json; then
  cores/p2k/tools/p2k_pack.py simm composer.sim --baby OUTDIR/baby.bin --big OUTDIR/big.bin
"""
import argparse
import collections
import json
import math
import os
import re
import struct
import sys
import zipfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import p2k_lib            # noqa: E402
import p2k_sf2            # noqa: E402
import p2k_simm           # noqa: E402
from p2k_build_simm import BIG, BABY, LIB, SAMPLE_BASE, obj, name16, place, sample_header  # noqa: E402

ROM_ID = 4
LAYERS = (0xA4, 0x170, 0x23C, 0x308)     # yp_1 layer offsets (docs/p2k-lib-formats.md)
# layer general words (SysEx 1409 + index)
INSTR, KEY_LO, KEY_HI, VEL_LO, VEL_HI, COARSE, FINE = 0, 4, 6, 8, 10, 16, 17


def norm(s):
    return re.sub(r"[^a-z0-9]", "", s.lower())

# ------------------------------------------------------------------ inputs


def split_sysex(b):
    out, i = [], 0
    while i < len(b):
        j = b.index(0xF7, i)
        out.append(b[i:j + 1])
        i = j + 1
    return out


def load_cmpsr(path):
    """CMPSR.zip -> [{num, name, layers: [31 layer-general words] x 4}] in preset order."""
    z = zipfile.ZipFile(path)
    names = sorted(n for n in z.namelist() if n.endswith(".syx"))
    out = []
    for n in names:
        ms = split_sysex(z.read(n))
        h = ms[0]
        hw = [h[k] | h[k + 1] << 7 for k in range(7, len(h) - 1, 2)]
        data = b"".join(m[9:-2] for m in ms[1:])
        w = [data[k] | data[k + 1] << 7 for k in range(16, len(data), 2)]
        w = [x - 16384 if x >= 8192 else x for x in w]
        per, base = sum(hw[8:13]), sum(hw[3:7])
        env0 = hw[8] + hw[9] + hw[10]          # layer: general, filter, LFO, then the envelopes
        out.append(dict(num=int(os.path.basename(n)[3:7]), name=data[:16].decode("latin-1"),
                        layers=[w[base + k * per: base + k * per + hw[8]] for k in range(hw[7])],
                        env_mode=[w[base + k * per + env0] for k in range(hw[7])]))
    if [p["num"] for p in out] != list(range(1024)):
        raise SystemExit(f"{path}: expected presets 04-0000 .. 04-1023")
    return out


def load_records(paths):
    """OS-written user preset records (pr01, 980 bytes) 0-511 from each save file, in order."""
    out = []
    for p in paths:
        with open(p, "rb") as f:
            d = f.read()
        _, _, _, _, objs, _ = p2k_lib.walk(d, 0xA0000)
        recs = sorted((o[3], d[o[0] + 12:o[0] + o[2]]) for o in objs if o[1] == "pr01")   # ids 4-515
        if len(recs) != 512:
            raise SystemExit(f"{p}: {len(recs)} user presets, expected 512")
        out += [r[1] for r in recs]
    return out

# ------------------------------------------------------------------ SF2 -> CMPSR


def align(presets, banks):
    """(sf2 file, preset index) -> CMPSR preset number, by name; duplicates resolved by the file's
    majority bank."""
    by = collections.defaultdict(list)
    for p in presets:
        by[norm(p["name"].split(":", 1)[-1])].append(p["num"])
    cand = {}
    for f, s in banks.items():
        for i, p in enumerate(s["presets"]):
            n = norm(p["name"])
            cand[(f, i)] = [num for k, nums in by.items() if k.startswith(n) or (len(k) >= 8 and n.startswith(k))
                            for num in nums]
    major = {}
    for f in banks:
        c = collections.Counter(v[0] // 128 for (ff, _), v in cand.items() if ff == f and len(v) == 1)
        major[f] = c.most_common(1)[0][0] if c else None
    out, used = {}, set()
    for key, c in sorted(cand.items(), key=lambda x: len(x[1])):
        c = [x for x in c if x not in used]
        if len(c) > 1:
            c = [x for x in c if x // 128 == major[key[0]]] or c
        if c:
            out[key] = c[0]
            used.add(c[0])
    return out


def zone_of(banks, f, z):
    s = banks[f]["samples"][z[53]]
    return dict(lo=z.get(43, (0, 127))[0], hi=z.get(43, (0, 127))[1], vlo=z.get(44, (0, 127))[0],
                vhi=z.get(44, (0, 127))[1], sample=(f, z[53]), sname=s["name"], coarse=z.get(51, 0),
                fine=z.get(52, 0), atten=z.get(48, 0), pan=z.get(17, 0), root=z.get(58, -1),
                loop=z.get(54, 0) in (1, 3), env={g: z[g] for g in (34, 35, 36, 37, 38) if g in z})


def dedupe(zs):
    seen, out = set(), []
    for z in zs:
        k = (z["lo"], z["hi"], z["vlo"], z["vhi"], z["sname"])
        if k not in seen:
            seen.add(k)
            out.append(z)
    return out


def untune(L, zs):
    return [dict(z, coarse=z["coarse"] - L[COARSE], fine=z["fine"] - round(L[FINE] * 100 / 64)) for z in zs]


def score(L, n):
    return ((L[KEY_LO], L[KEY_HI]) == (0, 127), (L[VEL_LO], L[VEL_HI]) == (0, 127), n)


def extract(presets, banks, aligned):
    """Instrument number -> (score, zones, preset name, how); and instrument number ->
    {sample name: SF2 envelope} from layers in Factory envelope mode (SysEx 1793 = 0), where the
    SF2 envelope is the instrument's own."""
    obs = collections.defaultdict(list)
    factory = collections.defaultdict(dict)
    rest = []
    for (f, i), num in aligned.items():         # factory envelopes: presets with one instrument
        sp = banks[f]["presets"][i]
        inst = banks[f]["instruments"][sp["zones"][-1][41]]
        p = presets[num]
        act = [k for k, L in enumerate(p["layers"]) if L[INSTR]]
        if len({p["layers"][k][INSTR] for k in act}) == 1 and all(p["env_mode"][k] == 0 for k in act):
            for z in inst["zones"]:
                if 53 in z:
                    zz = zone_of(banks, f, z)
                    factory[p["layers"][act[0]][INSTR]].setdefault(zz["sname"], zz["env"])
    for (f, i), num in aligned.items():
        sp = banks[f]["presets"][i]
        inst = banks[f]["instruments"][sp["zones"][-1][41]]
        zones = [zone_of(banks, f, z) for z in inst["zones"] if 53 in z]
        act = [L for L in presets[num]["layers"] if L[INSTR]]
        if not act:
            continue
        if len({L[INSTR] for L in act}) == 1:
            L = next((L for L in act if any(z["coarse"] == L[COARSE] for z in zones)), act[0])
            g = [z for z in zones if z["coarse"] == L[COARSE]] or zones
            obs[L[INSTR]].append((score(L, len(g)), untune(L, dedupe(g)), presets[num]["name"], "one instrument"))
        else:
            rest.append((num, act, zones))
    for _ in range(4):                     # claim zones by the sample names of known instruments
        known = {k: {z["sname"] for o in v for z in o[1]} for k, v in obs.items()}
        nxt = []
        for num, act, zones in rest:
            left = [z for z in zones if not any(L[INSTR] in known and z["sname"] in known[L[INSTR]] for L in act)]
            unk = [L for L in act if L[INSTR] not in known]
            if len({L[INSTR] for L in unk}) == 1 and left:
                L = unk[0]
                g = [z for z in left if z["coarse"] == L[COARSE]] or left
                obs[L[INSTR]].append((score(L, len(g)), untune(L, dedupe(g)), presets[num]["name"], "sample names"))
            elif unk:
                nxt.append((num, act, zones))
        rest = nxt
    known = {k: {z["sname"] for o in v for z in o[1]} for k, v in obs.items()}
    for num, act, zones in rest:           # by layer key ranges
        left = [z for z in zones if not any(L[INSTR] in known and z["sname"] in known[L[INSTR]] for L in act)]
        unk = [L for L in act if L[INSTR] not in known]
        for L in unk:
            others = [M for M in unk if M[INSTR] != L[INSTR]]

            def inside(z, M):
                return M[KEY_LO] <= z["lo"] and z["hi"] <= M[KEY_HI] and z["coarse"] == M[COARSE]
            g = [z for z in left if inside(z, L) and not any(inside(z, M) for M in others)]
            if g:
                obs[L[INSTR]].append(((False, False, len(g)), untune(L, dedupe(g)), presets[num]["name"], "key ranges"))
    return {k: max(v, key=lambda o: o[0]) for k, v in obs.items()}, factory

# ------------------------------------------------------------------ build


def rate_table(os_image):
    """OS 2.26 envelope rate -> level increment per tick (u16 x 128 at 0x0101CBFC [O 0x0101B70C]);
    a stage moves level x 512 units, rate 0 is the fastest."""
    with open(os_image, "rb") as f:
        d = f.read()
    return struct.unpack_from(">128H", d, 0x0101CBFC - 0x01000000)


TICK = 0.0125     # s per envelope tick [I]: full scale at rate 127 = 12800 ticks = the manual's ~160 s


def envelope(env, table):
    """SF2 volume envelope (timecents attack 34, decay 36, release 38; sustain 37 in cB) -> the 12
    bytes of a P2K time-based envelope in record order (A1, A2, D1, D2, R1, R2; rate, level) [I]."""
    def secs(g):
        return 2 ** (env.get(g, -12000) / 1200)

    def rate(t):
        inc = 100 * 512 / max(t / TICK, 1e-9)
        return min(range(128), key=lambda r: abs(math.log(table[r]) - math.log(max(inc, 1e-9))))
    sus = max(0, min(100, round(100 * 10 ** (-env.get(37, 0) / 200))))
    # an attack that ends on the first tick leaves the level bytes unwritten until the release
    # (MAME: no level write while the note is held), so the attack takes at least rate 2 [O]
    # attacks over 1 s are conversion artifacts (acoustic guitar: 3803 timecents = 9 s) [I]
    return bytes([max(2, rate(min(1.0, secs(34)))), 100, 0, 100, rate(secs(36)) if sus < 100 else 0, sus, 0, sus,
                  rate(secs(38)), 0, 0, 0])


DEFAULT_ENV = {34: -12000, 36: -12000, 37: 0, 38: -2000}    # instant attack, full sustain, ~0.3 s release


def build(presets, records, banks, names, os_image, log=print):
    aligned = align(presets, banks)
    found, factory = extract(presets, banks, aligned)
    table = rate_table(os_image)
    # every SF2 sample once, by (name, length)
    uniq, order = {}, []
    for f, s in banks.items():
        for k, x in enumerate(s["samples"]):
            key = (x["name"], x["end"] - x["start"])
            if key not in uniq:
                uniq[key] = (f, k)
                order.append(key)
    by_name = {}
    for key in order:
        by_name.setdefault(norm(re.sub(r"[- ]?[LR]$", "", key[0])), key)

    big = p2k_simm.header(BIG, ROM_ID)
    pos, placed = SAMPLE_BASE, {}
    for key in order:
        f, k = uniq[key]
        s = banks[f]["samples"][k]
        start, end, pos = place(big, pos, banks[f]["pcm"](s))
        if pos >= BIG:
            raise SystemExit("samples do not fit the 32 MB SIMM")
        placed[key] = (start, end, start + s["ls"] - s["start"], start + s["le"] - s["start"], s)
    big = p2k_simm.finish(big)

    ys, ys_index = [], {}

    def header_for(keys, loop):
        """ys_1 index for 1 (mono) or 2 (left, right) samples."""
        hk = (tuple(keys), loop)
        if hk not in ys_index:
            chans, s0 = [], placed[keys[0]][4]
            for key in keys:
                st, en, ls, le, _ = placed[key]
                chans.append((st, en, ls if loop else None, le if loop else None))
            ys_index[hk] = len(ys)
            ys.append(sample_header(keys[0][0], chans, s0["rate"], s0["corr"], ROM_ID))
        return ys_index[hk]

    def skey(f, k):
        x = banks[f]["samples"][k]
        return (x["name"], x["end"] - x["start"])

    def make_zone(z, keys, pan, env, vol_db):
        s = placed[keys[0]][4]
        root = z["root"] if z["root"] >= 0 else s["root"]
        tune = round((z["coarse"] * 100 + z["fine"]) * 64 / 100)
        b = bytearray(36)
        b[0:4] = bytes([z["lo"], 0, 0, z["hi"]])
        b[4:8] = bytes([z["vlo"], 0, 0, z["vhi"]])
        struct.pack_into(">Hh", b, 8, header_for(keys, z["loop"]), max(-32768, min(32767, tune)))
        b[12] = min(127, max(0, root))
        b[13] = max(-96, min(10, vol_db)) & 0xFF
        b[14] = max(-64, min(63, pan)) & 0xFF
        b[20:32] = envelope(env, table)       # factory envelope [O 0x0101A83E: zone +0x14]
        b[32] = 1                               # time-based [O 0x0101A3D0]
        b[34] = ROM_ID
        return bytes(b)

    def zones_for(zs, fenv):
        out, used = [], set()
        floor = min((z["atten"] for z in zs), default=0)
        if zs:      # the SF2 zones stop short of the keyboard ends; E-mu instruments span it [I]
            lo, hi = min(z["lo"] for z in zs), max(z["hi"] for z in zs)
            zs = [dict(z, lo=0 if z["lo"] == lo else z["lo"], hi=127 if z["hi"] == hi else z["hi"]) for z in zs]

        def env_vol(z):
            e = fenv.get(z["sname"]) or (next(iter(fenv.values())) if fenv else DEFAULT_ENV)
            return e, round(-(z["atten"] - floor) * 0.04)     # SF2 cB, E-mu's 0.4 scaling [I]
        for a, z in enumerate(zs):
            if a in used:
                continue
            m = re.match(r"(.*?)[- ]?L$", z["sname"])
            pair = None
            if m:
                for b2, y in enumerate(zs):
                    if b2 not in used and b2 != a and re.match(re.escape(m[1]) + r"[- ]?R$", y["sname"]) and \
                            (y["lo"], y["hi"], y["vlo"], y["vhi"]) == (z["lo"], z["hi"], z["vlo"], z["vhi"]):
                        pair = b2
                        break
            if pair is not None:
                used.update((a, pair))
                out.append((z["lo"], make_zone(z, [skey(*z["sample"]), skey(*zs[pair]["sample"])], 0, *env_vol(z))))
            else:
                used.add(a)
                out.append((z["lo"], make_zone(z, [skey(*z["sample"])], round(z["pan"] * 64 / 500), *env_vol(z))))
        return [b for _, b in sorted(out, key=lambda x: x[0])]

    yi, how = [name16("(none)") + bytes(4)], collections.Counter()
    for num in range(1, max(names) + 1):
        nm = names.get(num, f"(instrument {num})")
        if num in found:
            zb = zones_for(found[num][1], factory.get(num, {}))
            how[found[num][3]] += 1
        else:
            key = by_name.get(norm(nm.split(":", 1)[-1]))
            if key:
                f, k = uniq[key]
                x = banks[f]["samples"][k]
                z = dict(lo=0, hi=127, vlo=0, vhi=127, sample=(f, k), sname=x["name"], coarse=0, fine=0,
                         atten=0, pan=0, root=-1, loop=x["ls"] < x["le"] and x["le"] - x["ls"] > 8, env={})
                zb = zones_for([z], {})
                how["sample of the same name"] += 1
            else:
                zb = []
                how["no zones"] += 1
        if len(zb) > 255:
            raise SystemExit(f"instrument {num} has {len(zb)} zones")
        yi.append(name16(nm) + bytes([len(zb), 0, 0, 0]) + b"".join(zb))

    yp = []
    for rec in records:
        p = bytearray(rec)
        for off in LAYERS:
            if struct.unpack_from(">H", p, off + 6)[0]:
                p[off + 5] = ROM_ID
        yp.append(bytes(p))

    objs, oid = [], 0
    yh = bytearray(48)
    struct.pack_into(">I", yh, 0, ROM_ID)
    yh[36:42] = b"CMPSR\0"
    struct.pack_into(">I", yh, 44, ROM_ID)
    for typ, data in [(b"yh_1", bytes(yh))] + [(b"ys_1", r) for r in ys] + [(b"yi_1", r) for r in yi] + \
            [(b"yp_1", r) for r in yp]:
        objs.append(obj(typ, oid, data))
        oid += 1
    body = b"".join(objs)
    if LIB + 16 + len(body) > BABY:
        raise SystemExit(f"library ({len(body)} bytes) does not fit the 2 MB Baby flash")
    baby = bytearray(b"\xff" * BABY)
    baby[LIB:LIB + 16] = struct.pack(">4s4sIHH", b"lib_", b"_POS", BABY - LIB, 0, 0)
    baby[LIB + 16:LIB + 16 + len(body)] = body

    used = sorted({L[INSTR] for p in presets for L in p["layers"] if L[INSTR]})
    report = dict(
        presets=len(yp), sf2_presets_aligned=len(aligned), samples=len(placed), sample_headers=len(ys),
        big_bytes_used=pos, baby_bytes_used=LIB + 16 + len(body), instruments=len(yi) - 1,
        instruments_by_source=dict(how), instruments_with_factory_envelopes=len(factory),
        preset_instruments=len(used), preset_instruments_with_zones=sum(1 for u in used if len(yi[u]) > 20),
        preset_instruments_without_zones=[names.get(u, str(u)) for u in used if len(yi[u]) <= 20],
        presets_with_a_silent_layer=sum(1 for p in presets if any(L[INSTR] and len(yi[L[INSTR]]) <= 20
                                                                  for L in p["layers"])))
    return big, baby, report


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--cmpsr", required=True)
    ap.add_argument("--sf2", required=True, help="directory with the Composer SF2 banks")
    ap.add_argument("--names", required=True)
    ap.add_argument("--records", nargs=2, required=True, metavar=("PRESETS_0_511.sav", "PRESETS_512_1023.sav"))
    ap.add_argument("--os", required=True, help="OS 2.26 image (p2k226.bin), for the envelope rate table")
    ap.add_argument("out")
    a = ap.parse_args()
    presets = load_cmpsr(a.cmpsr)
    records = load_records(a.records)
    bad = [k for k in range(1024) if records[k][8:24] != presets[k]["name"].encode("latin-1")]
    if bad:
        raise SystemExit(f"{len(bad)} records do not carry their CMPSR preset's name, first {bad[0]}: "
                         f"{records[bad[0]][8:24]!r} vs {presets[bad[0]]['name']!r}")
    banks = {f: p2k_sf2.load(os.path.join(a.sf2, f)) for f in sorted(os.listdir(a.sf2)) if f.lower().endswith(".sf2")}
    with open(a.names) as f:
        names = {int(n): s for n, s in (line.rstrip("\n").split("\t") for line in f if line.strip())}
    big, baby, report = build(presets, records, banks, names, a.os)
    os.makedirs(a.out, exist_ok=True)
    for fn, data in (("big.bin", big), ("baby.bin", baby)):
        with open(os.path.join(a.out, fn), "wb") as f:
            f.write(data)
    with open(os.path.join(a.out, "report.json"), "w") as f:
        json.dump(report, f, indent=1)
    print(json.dumps(report, indent=1))


if __name__ == "__main__":
    main()
