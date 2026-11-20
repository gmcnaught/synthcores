#!/usr/bin/env python3
"""Rebuild a Proteus/1 (4 MB) sound ROM image from E-mu's Emulator X "Proteus 1" pack.

This is a FUNCTIONAL reconstruction, not a dump:
  * Sample placement comes from the firmware's sample table (XR v2.00 at 0x112EE:
    16-byte records {start, end, loop_start, loop_end}, ROM word addresses).
  * Each record is matched to an .ebl sample by its loop points:
      tier A  loop start/end offsets equal                  -> .ebl[0] at record start
      tier B  both loop offsets differ by the same shift k  -> .ebl[0] at start - k
    Unmatched records stay zero (silent) and are listed in the report.
  * Header (words 0x000-0x1BF) follows the layout verified on the ProtoLogic ROM:
      0x000-0x003  0x0100 0x0302 0x0504 0x0706   (byte-lane test pattern)
      0x004-0x01F  ID text, each char repeated on all 4 chip pairs (text here is ours)
      0x020-0x027  per-chip 16-bit checksums: lo bytes in 0x20+p, hi bytes in 0x24+p,
                   lane = pair p, byte (lo/hi); value = 16-bit sum of all bytes of that chip
      0x150-0x1BF  test sine, copied from the ProtoLogic ROM when available
    One compensation byte per chip (words 0x14C-0x14F, unused header area) makes each
    chip's byte sum equal its stored checksum, so the firmware's Sound ROM test passes.
  * Output: 8 chip images in the ProtoLogic chip layout (word n -> pair n&3, address n>>2,
    hi byte -> "hi" chip, lo byte -> "lo" chip) plus a combined big-endian 4 MB image.

Usage: rebuild_proteus1_rom.py FIRMWARE EBL_DIR OUT_DIR [--sine PROTOLOGIC_DIR]
FIRMWARE: interleaved XR v2.00 or ProtoLogic XR v2.30 image (the table is detected).
"""
import argparse
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(__file__))
import ebl  # noqa: E402

WORDS = 0x200000  # 4 MB
TABLE_BASE = 0x112EE
TABLE_RECORDS = 194


# Sample tables (16-byte records). The first 194 records of v2.30 equal the v2.00 table;
# v2.30 records 194.. are the ProtoLogic expansion (bank B).
TABLES = {"v2.00": (0x112EE, 194), "v2.30": (0x18A12, 194)}


def load_records(fw, base=TABLE_BASE, count=TABLE_RECORDS):
    d = open(fw, "rb").read() if isinstance(fw, str) else fw
    return [struct.unpack(">4I", d[base + 16 * i:base + 16 * i + 16]) for i in range(count)]


def detect_table(fw_bytes):
    """Find which known firmware table is present (by its first two records)."""
    for name, (base, count) in TABLES.items():
        recs = load_records(fw_bytes, base, 2)
        if recs[0] == (0x1C0, 0x1C0, 0x1C0, 0x1C0) and recs[1][0] == 0x1E8:
            return name, base, count
    raise SystemExit("no known Proteus/1 sample table in this firmware image")


def load_ebl(dirpath):
    out = []
    for f in sorted(os.listdir(dirpath)):
        if not f.lower().endswith(".ebl"):
            continue
        p = os.path.join(dirpath, f)
        raw = open(p, "rb").read()
        for s in ebl.parse(p):
            pcm = raw[s["body"] + s["start"]:s["body"] + s["end"]]
            words = list(struct.unpack("<%dh" % (len(pcm) // 2), pcm))
            out.append(dict(file=f, name=s["name"], pcm=words,
                            ls=(s["loop_start"] - s["start"]) // 2,
                            le=(s["loop_end"] - s["start"]) // 2))
    return out


def match(records, samples):
    """Assign .ebl samples to table records.

    shift k = (.ebl loop end) - (record loop end offset); ROM[start + j] = ebl[j + k].
      A: loop start and loop end offsets both equal (k = 0)
      B: both offsets differ by the same k
      C: loop end aligned (k), loop start within 64 words, tails consistent (k may be < 0:
         the .ebl begins after the record start; the missing head stays zero)
      D: k = 0, .ebl length = record length + 5 (the usual guard tail), loops ignored
      E: k = 0, .ebl length = record length - 2 .. + 39 (one-shots are 1 word short), loops ignored
      F: k = 0, the .ebl numbered next to an already-matched ROM neighbour, length within 10 %
      G: k = 0, nearest unused length within 3 % with a clear margin over the runner-up
      H: k = 0, remaining single-cycle records (<= 600 words) take unused short .ebl files in
         order (lowest confidence; these are 133-word waveforms)
    Tiers A-C accept any tail >= -2; placement is clipped at the next record's start.
    Each pass assigns only records with exactly one unused candidate; passes repeat
    until nothing changes, so earlier assignments disambiguate later ones.
    """
    placed = []
    used = set()
    pending = list(range(1, len(records)))  # record 0 is a zero-length placeholder

    def cands_for(i, tier):
        s, e, ls, le = records[i]
        LS, LE, L = ls - s, le - s, e - s
        out = []
        for k, x in enumerate(samples):
            if k in used:
                continue
            sh = x["le"] - LE
            tail = len(x["pcm"]) - sh - L
            if tail < -2:
                continue
            dls = x["ls"] - LS - sh
            if tier == "A" and sh == 0 and dls == 0:
                out.append((k, sh))
            elif tier == "B" and sh != 0 and dls == 0:
                out.append((k, sh))
            elif tier == "C" and abs(dls) <= 64 and abs(sh) <= 64:
                out.append((k, sh))
        if tier in ("D", "E"):
            # loops re-done by the conversion: align on start, match by length only
            for k, x in enumerate(samples):
                if k in used:
                    continue
                tail = len(x["pcm"]) - L
                if (tier == "D" and tail == 5) or (tier == "E" and -2 <= tail < 40):
                    out.append((k, 0))
        return out

    for tier in ("A", "B", "C", "D", "E", "F", "G", "H"):
        if tier == "G":
            # nearest unused length within 3 %, and clearly nearer than the runner-up
            still = []
            for i in pending:
                L = records[i][1] - records[i][0]
                c = sorted((abs(len(x["pcm"]) - L), k) for k, x in enumerate(samples) if k not in used)
                if c and c[0][0] <= max(8, L * 3 // 100) and (len(c) == 1 or c[1][0] > 2 * c[0][0] + 8):
                    used.add(c[0][1])
                    placed.append((i, samples[c[0][1]], 0, "G"))
                else:
                    still.append(i)
            pending = still
            continue
        if tier == "H":
            # remaining single-cycle waveforms (records <= 600 words): assign unused short
            # .ebl files in file order to records in ROM order
            short_recs = sorted((i for i in pending if records[i][1] - records[i][0] <= 600), key=lambda i: records[i][0])
            short_ebl = [k for k, x in enumerate(samples) if k not in used and 100 <= len(x["pcm"]) <= 600]
            for i, k in zip(short_recs, short_ebl):
                used.add(k)
                placed.append((i, samples[k], 0, "H"))
            pending = [i for i in pending if i not in short_recs[:len(short_ebl)]]
            continue
        if tier == "F":
            # neighbours in ROM order matched to .ebl k-1 and k+1 -> this record is .ebl k
            changed = True
            while changed:
                changed = False
                bymap = {i: samples.index(x) for i, x, sh, tr in placed}
                order = sorted(range(1, len(records)), key=lambda i: records[i][0])
                still = []
                for i in pending:
                    pos = order.index(i)
                    prv = next((bymap[j] for j in reversed(order[:pos]) if j in bymap), None)
                    nx = next((bymap[j] for j in order[pos + 1:] if j in bymap), None)
                    L = records[i][1] - records[i][0]
                    k = None
                    if prv is not None and prv + 1 < len(samples) and prv + 1 not in used:
                        k = prv + 1
                    elif nx is not None and nx - 1 >= 0 and nx - 1 not in used:
                        k = nx - 1
                    if k is not None and abs(len(samples[k]["pcm"]) - L) <= max(64, L // 10):
                        used.add(k)
                        placed.append((i, samples[k], 0, "F"))
                        changed = True
                    else:
                        still.append(i)
                pending = still
            continue
        changed = True
        while changed:
            changed = False
            still = []
            for i in pending:
                c = cands_for(i, tier)
                if len(c) == 1:
                    k, sh = c[0]
                    used.add(k)
                    placed.append((i, samples[k], sh, tier))
                    changed = True
                else:
                    still.append(i)
            pending = still
    return placed, pending


def build(records, samples, sine_words=None):
    """Return (rom words, placed, unmatched, words written). sine_words: 0x70 words for 0x150-0x1BF."""
    placed, unmatched = match(records, samples)

    rom = [0] * WORDS
    written = [False] * WORDS
    starts = sorted(r[0] for r in records)
    import bisect
    for i, x, sh, tier in placed:
        s = records[i][0]
        nxt = starts[bisect.bisect_right(starts, s)] if bisect.bisect_right(starts, s) < len(starts) else WORDS
        base = s - sh
        for j, v in enumerate(x["pcm"]):  # negative shift: .ebl starts after the record start
            n = base + j
            if n >= nxt:
                break
            if 0x1C0 <= n < WORDS and not written[n]:
                rom[n] = v & 0xFFFF
                written[n] = True

    # header
    for n, v in enumerate((0x0100, 0x0302, 0x0504, 0x0706)):
        rom[n] = v
    ident = b"REBUILT"
    for c, ch in enumerate(ident):
        for p in range(4):
            rom[4 + 4 * c + p] = ch << 8 | ch
    if sine_words:
        for n in range(0x150, 0x1C0):
            rom[n] = sine_words[n - 0x150]

    def chip_bytes(p, lane):
        return [(rom[n] >> (8 * lane)) & 0xFF for n in range(p, WORDS, 4)]

    # Checksums. The stored value (lo byte at word 0x20+p, hi byte at 0x24+p, in this lane) is
    # part of the summed bytes, so with S0 = sum with both stored bytes zero we need
    # S0 + lo + hi == hi*256 + lo  (mod 2^16), i.e. 255*hi == S0 + c, where c is added to
    # one compensation byte (word 0x14C+p, inside the header's unused area).
    for p in range(4):
        for lane in (0, 1):
            mask = ~(0xFF << (8 * lane))
            for n in (0x20 + p, 0x24 + p, 0x14C + p):
                rom[n] &= mask
            s0 = sum(chip_bytes(p, lane)) & 0xFFFF
            for hi_b in range(256):
                c = (255 * hi_b - s0) & 0xFFFF
                if c <= 0xFF:
                    break
            lo_b = 0
            rom[0x14C + p] |= c << (8 * lane)
            rom[0x20 + p] |= lo_b << (8 * lane)
            rom[0x24 + p] |= hi_b << (8 * lane)
            assert sum(chip_bytes(p, lane)) & 0xFFFF == (hi_b << 8 | lo_b)
    return rom, placed, unmatched, sum(written)


def to_bytes(rom):
    return b"".join(struct.pack(">H", v) for v in rom)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("firmware")
    ap.add_argument("ebl_dir")
    ap.add_argument("out_dir")
    ap.add_argument("--sine", help="ProtoLogic ROM dir (ip505..ip708) to copy the test sine from")
    a = ap.parse_args()

    fw = open(a.firmware, "rb").read()
    name, base, count = detect_table(fw)
    records = load_records(fw, base, count)
    samples = load_ebl(a.ebl_dir)
    sine = None
    if a.sine:
        lo = [open(os.path.join(a.sine, f"ip50{c}.bin"), "rb").read() for c in (5, 6, 7, 8)]
        hi = [open(os.path.join(a.sine, f"ip70{c}.bin"), "rb").read() for c in (5, 6, 7, 8)]
        sine = [hi[n & 3][n >> 2] << 8 | lo[n & 3][n >> 2] for n in range(0x150, 0x1C0)]
    rom, placed, unmatched, nwritten = build(records, samples, sine)

    def chip_bytes(p, lane):
        return [(rom[n] >> (8 * lane)) & 0xFF for n in range(p, WORDS, 4)]

    os.makedirs(a.out_dir, exist_ok=True)
    open(os.path.join(a.out_dir, "proteus1_waves.bin"), "wb").write(to_bytes(rom))
    for p in range(4):
        open(os.path.join(a.out_dir, f"rebuilt_hi{p}.bin"), "wb").write(bytes(chip_bytes(p, 1)))
        open(os.path.join(a.out_dir, f"rebuilt_lo{p}.bin"), "wb").write(bytes(chip_bytes(p, 0)))

    with open(os.path.join(a.out_dir, "rebuild_report.txt"), "w") as f:
        tiers = {"A": 0, "B": 0, "C": 0, "D": 0, "E": 0, "F": 0, "G": 0, "H": 0}
        for i, x, sh, tier in placed:
            tiers[tier] += 1
        f.write(f"records: {len(records) - 1} (excluding placeholder 0)\n")
        f.write(f"tier A (exact loops): {tiers['A']}\ntier B (shifted): {tiers['B']}\ntier C (loop end aligned, loop start differs): {tiers['C']}\ntier D (length + 5, loops differ): {tiers['D']}\ntier E (length + 0..39, loops differ): {tiers['E']}\ntier F (ROM-order neighbour): {tiers['F']}\ntier G (nearest length): {tiers['G']}\ntier H (single-cycle, order only): {tiers['H']}\nunmatched (silent): {len(unmatched)}\n")
        f.write(f"words written from .ebl: {nwritten} of {WORDS - 0x1C0}\n\n")
        for i, x, sh, tier in sorted(placed, key=lambda t: t[0]):
            s, e, ls, le = records[i]
            f.write(f"rec {i:3d} start={s:06x} len={e - s:6d} tier={tier} shift={sh:6d} <- {x['file']} '{x['name']}'\n")
        for i in unmatched:
            s, e, ls, le = records[i]
            f.write(f"rec {i:3d} start={s:06x} len={e - s:6d} UNMATCHED loop=({ls - s},{le - s})\n")
    print(open(os.path.join(a.out_dir, "rebuild_report.txt")).read().split("\n\n")[0])


if __name__ == "__main__":
    main()
