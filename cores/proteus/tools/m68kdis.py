#!/usr/bin/env python3
"""Recursive-descent 68000 disassembler for E-mu Proteus firmware images.

Usage:
  m68kdis.py IMAGE [--base 0] [--entry ADDR ...] [--listing OUT] [--xref LO-HI ...]

IMAGE is the interleaved (big-endian) program ROM. Entry points are the reset
PC plus every exception vector that points into the image, plus --entry values.
--xref prints every instruction whose operand text references an absolute
address inside one of the given ranges (hex, e.g. 800000-8003ff).

Requires capstone (pip install capstone).
"""
import argparse
import re
import struct
import sys

import capstone
from capstone import m68k as cm

FLOW_END = {"rts", "rte", "rtr", "jmp", "bra", "bra.s", "bra.w", "illegal", "stop"}
BRANCH_RE = re.compile(r"^(b(?:ra|sr|hi|ls|cc|cs|ne|eq|vc|vs|pl|mi|ge|lt|gt|le)|db\w+|jsr|jmp)(\.\w)?$")
ABS_RE = re.compile(r"\$([0-9a-fA-F]+)(?:\.[wl])?")


def parse_args():
    ap = argparse.ArgumentParser()
    ap.add_argument("image")
    ap.add_argument("--base", type=lambda s: int(s, 16), default=0)
    ap.add_argument("--entry", type=lambda s: int(s, 16), action="append", default=[])
    ap.add_argument("--listing")
    ap.add_argument("--xref", action="append", default=[])
    return ap.parse_args()


def main():
    a = parse_args()
    data = open(a.image, "rb").read()
    base, end = a.base, a.base + len(data)
    md = capstone.Cs(capstone.CS_ARCH_M68K, capstone.CS_MODE_M68K_000)
    md.detail = True

    entries = set(a.entry)
    for v in range(1, 256):
        tgt = struct.unpack(">I", data[v * 4:v * 4 + 4])[0]
        if base <= tgt < end and tgt % 2 == 0:
            entries.add(tgt)

    insns = {}
    labels = set(entries)
    work = sorted(entries)
    while work:
        pc = work.pop()
        while base <= pc < end and pc not in insns:
            off = pc - base
            got = next(md.disasm(data[off:off + 12], pc, 1), None)
            if got is None:
                break
            insns[pc] = got
            mn = got.mnemonic
            tgt = None
            if BRANCH_RE.match(mn):
                for op in got.operands:
                    if op.type == cm.M68K_OP_BR_DISP:
                        tgt = pc + 2 + op.br_disp.disp
                    elif op.type == cm.M68K_OP_MEM and op.address_mode in (cm.M68K_AM_ABSOLUTE_DATA_LONG, cm.M68K_AM_ABSOLUTE_DATA_SHORT):
                        tgt = op.mem.disp if hasattr(op.mem, "disp") else None
                if tgt is None:
                    m = ABS_RE.search(got.op_str)
                    if m and mn in ("jsr", "jmp"):
                        tgt = int(m.group(1), 16)
                if tgt is not None and base <= tgt < end:
                    labels.add(tgt)
                    work.append(tgt)
            if mn in FLOW_END or mn.startswith("bra") or mn == "jmp":
                break
            pc += got.size

    ranges = []
    for r in a.xref:
        lo, hi = (int(x, 16) for x in r.split("-"))
        ranges.append((lo, hi))

    print(f"# {len(insns)} instructions decoded, {len(labels)} labels", file=sys.stderr)
    if ranges:
        for pc in sorted(insns):
            ins = insns[pc]
            for m in ABS_RE.finditer(ins.op_str):
                v = int(m.group(1), 16)
                if any(lo <= v <= hi for lo, hi in ranges):
                    print(f"{pc:06x}: {ins.mnemonic:8s} {ins.op_str}")
                    break

    if a.listing:
        with open(a.listing, "w") as f:
            prev_end = None
            for pc in sorted(insns):
                ins = insns[pc]
                if prev_end is not None and pc != prev_end:
                    f.write("\n")
                if pc in labels:
                    f.write(f"L_{pc:06x}:\n")
                f.write(f"  {pc:06x}: {ins.bytes.hex():<20s} {ins.mnemonic:8s} {ins.op_str}\n")
                prev_end = pc + ins.size


if __name__ == "__main__":
    main()
