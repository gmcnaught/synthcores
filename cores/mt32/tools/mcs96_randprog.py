#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Random MCS-96 test programs as a 128 KB MT-32 control ROM (plan milestone M1).

    mcs96_randprog.py SEED OUT.bin [--n N] [--sfr]

The program starts at 2080 (reset), sets up SP, pointer registers and random register
contents, then loops over N random instructions taken from the decode table
(hdl/mcs96/mcs96_decode.txt): every valid opcode and addressing form, including FE-prefixed
ones. Operands are constrained so that:
  - writes go to registers 30-FF (never SP 18, the pointer registers 20-2F or SFRs) or to RAM
    C100-F000 (pointers and indexed bases point there);
  - branches only go forward, calls go to small subroutines that return, the stack stays
    balanced over one pass of the loop.
With --sfr the program also reads and writes SFRs (timers, interrupt mask/pending, IOC1, HSO,
A/D, SBUF, PWM), enables interrupts, and every vector points at a RET handler.
"""
import argparse
import random
import re
from pathlib import Path

CORE = Path(__file__).resolve().parent.parent
SP_INIT = 0xFF00
PTRS = list(range(0x20, 0x30, 2))
HANDLER = 0x7F00
SUBS = 0x7000

BANNED = {"xch", "xchb", "rst", "br", "trap"}     # br/trap get their own templates below
WRITES_OP1_MEM = {"st", "stb", "pop"}
BYTE_MN = {"andb", "addb", "subb", "orb", "xorb", "addcb", "subcb", "cmpb", "ldb", "stb", "mulub",
           "mulb", "divub", "divb", "clrb", "notb", "negb", "decb", "incb", "extb", "shrb", "shlb",
           "shrab", "ldbze", "ldbse"}


def decode_table():
    t = []
    for line in (CORE / "hdl" / "mcs96" / "mcs96_decode.txt").read_text().splitlines():
        m = re.match(r"([0-9a-f]{3})\s+(\w+)\s+(\w+)\s+n(\d)\s+(w?)\s*cyc", line)
        if m and "INVALID" not in line:
            st, mn, am, n, w = m.groups()
            t.append((int(st, 16), mn, am, int(n), w == "w"))
    return t


class Gen:
    def __init__(self, seed, sfr):
        self.r = random.Random(seed)
        self.sfr = sfr
        self.items = []          # (bytes or callable(addr_map) -> bytes, label)
        self.table = [e for e in decode_table() if e[1] not in BANNED]

    def reg(self, lo=0x30, hi=0xff, even=False, align=1):
        while True:
            v = self.r.randrange(lo, hi + 1)
            if align > 1:
                v &= ~(align - 1)
            elif even:
                v &= ~1
            if v >= lo:
                return v

    def src_reg(self, word):
        c = self.r.random()
        if c < 0.05:
            return 0x00
        if c < 0.10:
            return self.r.choice([0x18, 0x1a, 0x1c, 0x1e]) | (0 if word else self.r.randrange(2))
        if c < 0.15:
            return self.r.choice(PTRS) | (0 if word else self.r.randrange(2))
        if self.sfr and c < 0.22:
            return self.r.choice([0x02, 0x03, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d,
                                  0x0e, 0x0f, 0x10, 0x11, 0x15, 0x16])
        return self.reg(0x1a, 0xff)

    def dst_reg(self, mn, word_out, dword=False):
        if dword:
            return self.reg(0x30, 0xfc, align=4) | self.r.choice([0, 0, 1, 2, 3])
        return self.reg(0x30, 0xfe if word_out else 0xff) | (self.r.randrange(2) if self.r.random() < 0.1 else 0)

    def imm(self, w):
        v = self.r.choice([0, 1, 0x7f, 0x80, 0xff, 0x8000, 0xffff, self.r.randrange(0x10000)])
        return [v & 0xff, (v >> 8) & 0xff] if w else [v & 0xff]

    def mem_operand(self, am, writes):
        """bytes after the opcode for an indirect / indexed memory operand (OPI, disp)."""
        if am == "indirect":
            p = self.r.choice(PTRS)
            return [p | (1 if self.r.random() < 0.4 else 0)]
        c = self.r.random()
        if c < 0.4:      # pointer + short displacement
            return [self.r.choice(PTRS), self.r.randrange(256)]
        if c < 0.6:      # base 0, short: register space
            lo = 0x30 if writes else 0x00
            return [0x00, self.r.randrange(lo, 0x80)]
        if c < 0.8:      # base 0, long: RAM (or ROM when only read)
            a = self.r.randrange(0xC100, 0xF000) if (writes or self.r.random() < 0.5) else self.r.randrange(0x2000, 0x7000)
            return [0x01, a & 0xff, a >> 8]
        d = self.r.randrange(-0x400, 0x400) & 0xffff        # pointer + long displacement
        return [self.r.choice(PTRS) | 1, d & 0xff, d >> 8]

    def instr(self):
        st, mn, am, n, immw = self.r.choice(self.table)
        pre = [0xfe] if st >= 0x100 else []
        op = st & 0xff
        word = mn not in BYTE_MN
        dword_out = mn in ("mulu", "mul", "divu", "div") and n == 2 or mn in ("shrl", "shll", "shral", "norml")
        if am == "none":
            if mn in ("ei", "di", "pushf", "popf") and not self.sfr:
                return None
            return ("stack", mn, pre + [op]) if mn in ("pushf", "popf") else (pre + [op])
        if am in ("rel8", "rel11", "rel16", "rrel8"):
            return ("branch", st, mn, am, pre)
        if am == "immed_or_reg":
            cnt = self.r.randrange(16) if self.r.random() < 0.6 else self.reg(0x30, 0xff)
            return pre + [op, cnt, self.dst_reg(mn, True, dword_out)]
        if mn in ("push", "pop"):
            return ("stack", mn, (pre, op, am))
        b = []
        writes_mem = mn in WRITES_OP1_MEM
        if am == "direct":
            if n == 1:
                if mn in ("clr", "not", "neg", "dec", "inc", "ext"):
                    b = [self.dst_reg(mn, True, mn == "ext")]
                else:
                    b = [self.dst_reg(mn, False)]
            elif mn in ("st", "stb"):
                b = [self.dst_reg(mn, mn == "st"), self.src_reg(mn == "st")]
            elif mn == "norml":
                b = [self.dst_reg(mn, False), self.dst_reg(mn, True, True)]
            else:
                b = [self.src_reg(word), self.dst_reg(mn, True, dword_out)]
                if n == 3:
                    b.append(self.dst_reg(mn, True, mn in ("mulu", "mul")))
        elif am == "immed":
            b = self.imm(immw)
            if n >= 2:
                b.append(self.dst_reg(mn, True, dword_out))
            if n == 3:
                b.append(self.dst_reg(mn, True, mn in ("mulu", "mul")))
        elif am in ("indirect", "indexed"):
            b = self.mem_operand(am, writes_mem)
            if n >= 2:
                b.append(self.src_reg(True) if mn in ("st", "stb") else self.dst_reg(mn, True, dword_out))
            if n == 3:
                b.append(self.dst_reg(mn, True, mn in ("mulu", "mul")))
        else:
            return None
        return pre + [op] + b

    def build(self, n, out):
        rom = bytearray(0x20000)
        for i in range(9):
            rom[0x2000 + 2 * i] = HANDLER & 0xff
            rom[0x2001 + 2 * i] = HANDLER >> 8
        rom[HANDLER] = 0xf0                                  # ret
        # ROM data for reads
        for a in range(0x6000, 0x6800):
            rom[a] = self.r.randrange(256)
        code = []                                            # list of instruction byte lists / fixups
        def ldw(reg, v):
            code.append([0xa1, v & 0xff, v >> 8, reg])        # ld immed_2w
        ldw(0x18, SP_INIT)
        for a in range(0x1a, 0x100, 2):
            if 0x20 <= a < 0x30:
                continue
            ldw(a, self.r.randrange(0x10000))
        if self.sfr:
            code.append([0xb1, 0x75, 0x16])                  # ioc1 = 75 (as the firmware)
        loop_top = len(code)
        for p in PTRS:
            ldw(p, self.r.randrange(0xC100, 0xF000) & ~1)
        if self.sfr:
            code.append([0xb1, self.r.choice([0x61, 0xe1, 0x23, 0xff]), 0x08])   # int mask
            code.append([0xfb])                              # ei
        depth = []
        subs = []
        for i in range(n):
            if i % 40 == 39:
                for p in PTRS:
                    ldw(p, self.r.randrange(0xC100, 0xF000) & ~1)
            if self.sfr and self.r.random() < 0.03:
                code.extend(self.sfr_block())
                continue
            if self.r.random() < 0.01:
                code.append(["trap"])
                continue
            x = self.instr()
            if x is None:
                continue
            if isinstance(x, tuple) and x[0] == "stack":
                _, mn, rest = x
                if mn in ("push", "pushf"):
                    if len(depth) >= 24:
                        continue
                    if mn == "pushf":
                        code.append(rest)
                        depth.append("f")
                    else:
                        pre, op, am = rest
                        if am == "immed":
                            code.append(pre + [op] + self.imm(True))
                        elif am == "direct":
                            code.append(pre + [op, self.src_reg(True)])
                        else:
                            mo = self.mem_operand(am, False)
                            if am == "indirect":
                                mo[0] &= 0xfe
                            code.append(pre + [op] + mo)
                        depth.append("w")
                else:
                    if not depth:
                        continue
                    k = depth.pop()
                    if k == "f":
                        code.append([0xf3])                  # popf
                    else:
                        if mn == "popf":
                            code.append([0xf3])
                        else:
                            pre, op, am = rest
                            if am == "direct":
                                code.append(pre + [op, self.dst_reg("pop", True)])
                            else:
                                code.append(pre + [op] + self.mem_operand(am, True))
                continue
            if isinstance(x, tuple) and x[0] == "branch":
                _, st, mn, am, pre = x
                if mn in ("scall", "lcall"):
                    body = []
                    for _ in range(self.r.randrange(1, 4)):
                        y = self.instr()
                        if isinstance(y, list):
                            body.append(y)
                    code.append(("call", st, am, pre, body))
                    continue
                skip = self.r.randrange(0, 4)
                code.append(("br", st, mn, am, pre, skip, None,
                             self.dst_reg("djnz", False) if am == "rrel8" else 0))
                continue
            code.append(x)
        while depth:
            code.append([0xf3] if depth.pop() == "f" else [0xcc, self.dst_reg("pop", True)])
        code.append(("jmp_top",))
        self.assemble(rom, code, loop_top)
        Path(out).write_bytes(bytes(rom))

    def sfr_block(self):
        c = self.r.random()
        b = []
        if c < 0.25:   # HSO software timer soon: hso_command = 18/19, hso_time = timer1 + k
            b.append([0xb1, self.r.choice([0x18, 0x19, 0x38, 0x08]), 0x06])
            b.append([0xa0, 0x0a, 0x30])                     # ld 30, timer1
            b.append([0x65, self.r.randrange(2, 40), 0x00, 0x30])
            b.append([0xc4, 0x04, 0x30])                     # stb hso_time lo
            b.append([0xc4, 0x05, 0x31])                     # stb hso_time hi
        elif c < 0.45:
            b.append([0xb1, self.r.choice([0x0f, 0x0c, 0x07]), 0x02])   # A/D start
        elif c < 0.55:
            b.append([0xb1, self.r.randrange(256), 0x07])    # sbuf
        elif c < 0.7:
            b.append([0xb1, self.r.choice([0x61, 0xe1, 0x23, 0xff, 0x00]), 0x08])
        elif c < 0.8:
            b.append([0xb1, self.r.randrange(256), 0x17])    # pwm
        elif c < 0.9:
            b.append([0xb1, self.r.choice([0x75, 0x7f, 0x01, 0x0d]), 0x16])  # ioc1
        else:
            b.append([0xb1, self.r.randrange(256), 0x09])    # int pending
        return b

    def assemble(self, rom, code, loop_top):
        # sizes first
        def size(x):
            if isinstance(x, list):
                return len(x) if x != ["trap"] else 1
            if x[0] == "br":
                _, st, mn, am, pre, skip, sub, reg = x
                return len(pre) + {"rel8": 2, "rel11": 2, "rel16": 3, "rrel8": 3}[am]
            if x[0] == "call":
                _, st, am, pre, body = x
                return len(pre) + (2 if am == "rel11" else 3) + 2 + sum(len(b) for b in body) + 1
            return 3                                         # jmp_top: ljmp
        addr = 0x2080
        addrs = []
        for x in code:
            addrs.append(addr)
            addr += size(x)
        end = addr
        assert end < 0x6000, hex(end)
        for i, x in enumerate(code):
            at = addrs[i]
            if isinstance(x, list):
                ins = [0xf7] if x == ["trap"] else x
            elif x[0] == "call":
                # call L1; sjmp L2; L1: body; ret; L2:
                _, st, am, pre, body = x
                cl = len(pre) + (2 if am == "rel11" else 3)
                d = 2                                        # L1 = after the call and the sjmp
                if am == "rel11":
                    ins = pre + [(st & 0xf8) | ((d >> 8) & 7), d & 0xff]
                else:
                    ins = pre + [st & 0xff, d & 0xff, 0]
                blen = sum(len(b) for b in body) + 1
                ins += [0x20 | ((blen >> 8) & 7), blen & 0xff]   # sjmp over the body
                for b in body:
                    ins += b
                ins += [0xf0]
            elif x[0] == "jmp_top":
                d = (addrs[loop_top] - (at + 3)) & 0xffff
                ins = [0xe7, d & 0xff, d >> 8]
            else:
                _, st, mn, am, pre, skip, sub, reg = x
                op = st & 0xff
                ln = size(x)
                j = min(i + 1 + skip, len(code) - 1)
                target = addrs[j]
                d = target - (at + ln)
                if am in ("rel8", "rrel8") and not -128 <= d < 128:
                    d = addrs[i + 1] - (at + ln)             # out of range: the next instruction
                if am == "rel8":
                    ins = pre + [op, d & 0xff]
                elif am == "rel11":
                    if not -1024 <= d < 1024:
                        target = addrs[min(i + 1, len(code) - 1)]
                        d = target - (at + ln)
                    ins = pre + [(op & 0xf8) | ((d >> 8) & 7), d & 0xff]
                elif am == "rel16":
                    ins = pre + [op, d & 0xff, (d >> 8) & 0xff]
                else:
                    ins = pre + [op, reg, d & 0xff]
            rom[at:at + len(ins)] = bytes(ins)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("seed", type=int)
    ap.add_argument("out")
    ap.add_argument("--n", type=int, default=3000)
    ap.add_argument("--sfr", action="store_true")
    a = ap.parse_args()
    Gen(a.seed, a.sfr).build(a.n, a.out)


if __name__ == "__main__":
    main()
