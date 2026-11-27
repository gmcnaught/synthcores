#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Random H8/300H test programs for the U1 lockstep against MAME.

Usage: h8h_randprog.py MAME_DIR SEED OUT.bin [--count N] [--only NAME,...]

Writes a program image (big-endian, as the CPU sees it) for the MU50 memory map. Every enabled
H8/300H opcode of MAME's h8.lst (all but SLEEP, RTS and RTE, which run from stubs) is drawn with
random operands; each encoding is checked against MAME's own decoder (h8h_gen.decode_words).
Rules that keep a random program on the rails:

  000000  vectors: reset -> 000400; TRAPA #0-3 -> the trap stub; JMP @@aa:8 entries
  000100  RTS stub, 000104 BRA $, 000106 trap stub: ADDS #4,ER7; JMP @ER6
  000400  ER0-ER6 random, ER7 = 20F800 (stack), then the random instructions, then BRA $

- Data registers are never ER7 (R7, E7, R7H, R7L).
- A memory operand's pointer register (ER0-ER6) is loaded just before the instruction with
  MOV.L #imm, so the effective address is in work RAM (200000-20DFFF) or on-chip RAM. @-ER7 and
  @ER7+ (push and pop) use the stack as it is.
- @aa:8 is FFFF00-FFFF0F (on-chip RAM), @aa:16 is on-chip RAM or ROM, @aa:24 work RAM or ROM.
- Bcc: displacement 0, or 2 over a NOP. BSR: to an RTS placed after it. JSR: the RTS stub.
  JMP: the next instruction. TRAPA: ER6 = the next instruction; the trap stub drops the frame
  and jumps there. (MAME's TRAPA pushes its own address, m_NPC, so RTE would run it again.)
- RTS and RTE: a frame for the next instruction (and a random CCR) is pushed first.
- EEPMOV: ER5 and ER6 in work RAM, a count of 0-15.
"""
import argparse
import random
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from h8h_gen import decode_words, dispatch_tables, load_opcodes  # noqa: E402

MAIN = 0x400
RTS_STUB, HALT = 0x100, 0x104
SP_INIT = 0x20F800
EXCLUDE = {"sleep"}
TRAP_STUB = 0x106
NOP = bytes.fromhex("0000")
FREE_VECTORS = [v for v in range(1, 64) if v not in (8, 9, 10, 11)]


def mov_l_imm(r, v):
    return bytes([0x7A, r]) + struct.pack(">I", v & 0xffffffff)


class Gen:
    def __init__(self, mame, seed):
        self.ol = load_opcodes(mame)
        self.tabs = dispatch_tables(self.ol)
        self.r = random.Random(seed)
        self.img = bytearray(0x80000)
        self.pc = MAIN
        self.vec = list(FREE_VECTORS)
        self.r.shuffle(self.vec)
        self.ops = [o for o in self.ol.opcode_info if o.enabled and o.name not in EXCLUDE]

    def emit(self, b):
        self.img[self.pc:self.pc + len(b)] = b
        self.pc += len(b)

    # operand helpers -------------------------------------------------------------------------
    def data_reg(self, kind):
        if kind in ("r8l", "r8h", "r8u", "r16l", "r16h"):
            return self.r.choice([x for x in range(16) if x & 7 != 7])
        return self.r.randrange(7)

    def ram_addr(self):
        if self.r.random() < 0.15:
            return self.r.randrange(0xFFFD10, 0xFFFF00)
        return self.r.randrange(0x200000, 0x20E000)

    def setf(self, v, idx, shift, width, value):
        m = ((1 << width) - 1) << shift
        if self.op.mask[len(v) + idx] & m:
            raise AssertionError("%s: field %d<<%d not free" % (self.op.function_name(), idx, shift))
        v[idx] = (v[idx] & ~m) | ((value << shift) & m)

    # one instruction ---------------------------------------------------------------------------
    def one(self, o):
        self.op = o
        r = self.r
        v = [b | (r.randrange(256) & ~m) for b, m in zip(o.val, o.mask)]
        extra = b""
        setup = b""
        after = b""
        for am in (o.am1, o.am2):
            if am in ("r8l", "r16l"):
                x = self.data_reg(am)
                self.setf(v, -1, 0, 4, x)
            elif am in ("r8h", "r16h"):
                x = self.data_reg(am)
                self.setf(v, -1, 4, 4, x)
            elif am == "r8u":
                x = self.data_reg(am)
                self.setf(v, -2, 0, 4, x)
            elif am == "r32l":
                x = self.data_reg(am)
                self.setf(v, -1, 0, 3, x)
            elif am == "r32h" and o.name not in ("jmp", "jsr"):
                x = self.data_reg(am)
                self.setf(v, -1, 4, 3, x)
            elif am == "imm3":
                self.setf(v, -1, 4, 3, r.randrange(8))
            elif am == "imm2":
                self.setf(v, -1, 4, 2, r.randrange(4))
            elif am == "imm8":
                v[-1] = r.randrange(256)
            elif am == "imm16":
                extra += struct.pack(">H", r.randrange(0x10000))
            elif am == "imm32":
                extra += struct.pack(">I", r.getrandbits(32))
            elif am == "abs8":
                v[1] = r.randrange(0x10)
            elif am == "abs16":
                a = r.randrange(0xFD10, 0xFF10) if r.random() < 0.7 else r.randrange(0x8000)
                extra += struct.pack(">H", a)
            elif am == "abs32":
                a = self.ram_addr() if r.random() < 0.8 else r.randrange(0x80000)
                extra += struct.pack(">I", a | (r.randrange(256) << 24 if r.random() < 0.2 else 0))
            elif am in ("r32ih", "r32ihh", "pr32h", "r32ph", "r32d16h", "r32d32hh"):
                hh = am in ("r32ihh", "r32d32hh")
                if am in ("pr32h", "r32ph") and r.random() < 0.3:
                    p = 7
                else:
                    p = r.randrange(7)
                    t = self.ram_addr()
                    if am == "r32d16h":
                        d = r.randrange(-0x8000, 0x8000)
                        extra += struct.pack(">h", d)
                        t -= d
                    elif am == "r32d32hh":
                        d = r.getrandbits(32)
                        extra += struct.pack(">I", d)
                        t -= d
                    setup += mov_l_imm(p, t)
                if hh:
                    self.setf(v, -3, 4, 3, p)
                else:
                    self.setf(v, -1, 4, 3, p)
            elif am in ("one", "two", "four", "ccr", "-", "rel8", "rel16", "abs8i", "abs24e", "r32h"):
                pass
            else:
                raise SystemExit("addressing mode %s" % am)

        # control transfers
        size = len(v) + len(extra)
        if o.am1 == "rel8":
            if o.name == "bsr":
                v[1] = 2           # bsr -> rts at +4; bra over it
                after = bytes.fromhex("4002") + bytes.fromhex("5470")
            else:
                d = r.choice([0, 2])
                v[1] = d
                after = NOP if d else b""
        elif o.am1 == "rel16":
            if o.name == "bsr":
                extra = struct.pack(">h", 2)
                after = bytes.fromhex("4002") + bytes.fromhex("5470")
            else:
                d = r.choice([0, 2])
                extra = struct.pack(">h", d)
                after = NOP if d else b""
        elif o.name in ("jmp", "jsr"):
            size = len(v) + 2 * o.extra_words
            nxt = self.pc + len(setup) + size + (6 if o.am1 == "r32h" else 0)   # + its MOV.L
            target = RTS_STUB if o.name == "jsr" else nxt
            if o.am1 == "r32h":
                p = r.randrange(7)
                setup += mov_l_imm(p, target)
                self.setf(v, -1, 4, 3, p)
            elif o.am1 == "abs24e":
                v[1] = (target >> 16) & 0xff      # @aa:24: second byte and an extra word
                extra = struct.pack(">H", target & 0xffff)
            elif o.am1 == "abs8i":
                if o.name == "jsr":
                    vec = r.choice(self.vec_jsr) if self.vec_jsr else None
                    if vec is None:
                        if not self.vec:
                            return False
                        vec = self.vec.pop()
                        self.vec_jsr.append(vec)
                        struct.pack_into(">I", self.img, vec * 4, RTS_STUB)
                else:
                    if not self.vec:
                        return False
                    vec = self.vec.pop()
                    struct.pack_into(">I", self.img, vec * 4, nxt)
                v[1] = vec * 4
        elif o.name == "trapa":
            setup += mov_l_imm(6, self.pc + 6 + size)
        elif o.name in ("rts", "rte"):
            nxt = self.pc + 10 + size
            setup += mov_l_imm(6, nxt | (r.randrange(256) << 24 if o.name == "rte" else 0))
            setup += bytes.fromhex("01006df6")     # mov.l er6,@-er7
        elif o.name.startswith("eepmov"):
            setup += mov_l_imm(5, self.r.randrange(0x200000, 0x20D000))
            setup += mov_l_imm(6, self.r.randrange(0x200000, 0x20D000))
            n = r.randrange(16)
            setup += bytes([0xFC, n]) if o.name == "eepmov.b" else bytes([0x79, 0x04, 0, n])

        words = [(v[i] << 8) | v[i + 1] for i in range(0, len(v), 2)] + [0, 0, 0]
        if decode_words(self.ol, self.tabs, words) is not o:
            return False
        self.emit(setup)
        self.emit(bytes(v) + extra)
        self.emit(after)
        return True

    def program(self, count, only):
        struct.pack_into(">I", self.img, 0, MAIN)
        for t in range(8, 12):
            struct.pack_into(">I", self.img, t * 4, TRAP_STUB)
        self.img[RTS_STUB:RTS_STUB + 2] = bytes.fromhex("5470")
        self.img[TRAP_STUB:TRAP_STUB + 4] = bytes.fromhex("0b975960")
        self.img[HALT:HALT + 2] = bytes.fromhex("40fe")
        self.vec_jsr = []
        for i in range(7):
            self.emit(mov_l_imm(i, self.r.getrandbits(32)))
        self.emit(mov_l_imm(7, SP_INIT))
        ops = [o for o in self.ops if not only or o.name in only]
        n = 0
        while n < count:
            if self.one(self.r.choice(ops)):
                n += 1
            if self.pc > 0x7F000:
                break
        self.emit(bytes.fromhex("40fe"))
        return bytes(self.img[:max(self.pc, 0x1000)])


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("mame")
    ap.add_argument("seed", type=int)
    ap.add_argument("out", type=Path)
    ap.add_argument("--count", type=int, default=2000)
    ap.add_argument("--only", default="")
    a = ap.parse_args()
    g = Gen(a.mame, a.seed)
    a.out.write_bytes(g.program(a.count, set(x for x in a.only.split(",") if x)))


if __name__ == "__main__":
    main()
