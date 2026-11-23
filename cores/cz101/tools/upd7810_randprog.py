#!/usr/bin/env python3
"""Random uPD7810 test programs for the C1 lockstep against MAME.

Usage: upd7810_randprog.py DECODE_TXT SEED OUT.bin [--sfr]

Writes a 32 KB image (the program ROM; also used as the uPD7811 internal ROM, which MAME maps
over 0000-0FFF). Every non-special-register instruction of the uPD7810 tables appears with
random operands. Pointers are set before each memory access so all data accesses stay in RAM
(8000-8FFF), and every control transfer lands on generated code:

  0000  JMP 1000
  0060  RETI                 (SOFTI)
  0080  CALT table -> 0800   (40 entries)
  0700  RETS stub
  0800  RET x 2048           (CALL, CALF, CALB targets)
  1000  main: random instructions, each followed by a filler that never sets SK, so a skip
        always lands on the filler; setups never follow MVI A, MVI L or LXI H directly
        (their L0/L1 string flags would skip the setup).
"""
import random, re, sys

SFR_NAMES = {"PA", "PB", "PC", "PD", "PF", "MKH", "MKL", "ANM", "SMH", "SML", "EOM", "ETMM",
             "TMM", "MM", "MCC", "MA", "MB", "MC", "MF", "TXB", "RXB", "TM0", "TM1", "CR0",
             "CR1", "CR2", "CR3", "ZCM", "MT", "PT", "ETM0", "ETM1", "ECNT", "ECPT"}
PREFIX = {"48": 0x48, "4C": 0x4C, "4D": 0x4D, "60": 0x60, "64": 0x64, "70": 0x70, "74": 0x74}
EXCLUDE = re.compile(r"^(illegal|PRE_|HLT|STOP|RET|RETS|RETI|LXI_S_w|LSPD_w|SKN?IT_|NOP)")
# --sfr: special registers the CZ-101 board makes unsafe or time-dependent in a random test:
# PA (LCD bus), PC/MC/MCC (power-down and LCD strobes, SCK level), MA (LCD bus direction).
SFR_UNSAFE = {"PA", "PC", "MA", "MC", "MCC"}
COMPARE = ("GTI", "LTI", "NEI", "EQI", "ONI", "OFFI")


def load(path):
    ops = {}
    for line in open(path):
        t, op, name, ln = line.split()[:4]
        ln = int(ln.split("=")[1])
        enc = [int(op, 16)] if t == "XX" else [PREFIX[t], int(op, 16)]
        ops.setdefault(name, []).append((enc, ln - len(enc)))
    return ops


class Gen:
    def __init__(self, ops, seed, sfr):
        self.r = random.Random(seed)
        self.ops = ops
        self.img = bytearray(0x8000)
        self.pc = 0x1000
        self.sfr = sfr
        names = [n for n in ops if not EXCLUDE.match(n)]
        if not sfr:
            names = [n for n in names if not (set(n.split("_")) & SFR_NAMES)]
        else:
            def ok(n):
                parts = set(n.split("_"))
                if "PC" in parts and (n == "MOV_A_PC" or n.split("_")[0] in COMPARE):
                    return True
                if parts & SFR_UNSAFE:
                    return False
                # SMH writes only through the guarded sequence (mode 3 = external SCK)
                if "SMH" in parts and not (n == "MOV_A_SMH" or n.split("_")[0] in COMPARE):
                    return False
                return True
            names = [n for n in names if ok(n)]
            names += [n for n in ops if re.match(r"SKN?IT_", n)] * 1
        self.names = names
        self.fillers = [n for n in names if re.fullmatch(
            r"(MOV_[A-L]_[A-L]|MOV_(A|V)_(EAH|EAL|[A-L])|MOV_(EAH|EAL)_A|MVI_[BCDEHV]_xx|"
            r"(ADD|ADC|SUB|SBB|ANA|ORA|XRA)_(A_[A-LV]|[A-LV]_A)|INX_(BC|DE|EA)|DCX_(BC|DE|EA)|"
            r"LXI_(B|D|EA)_[ws]|EXA|EXX|EXH|EXR|CLC|STC|NEGA|DAA|"
            r"(SLL|SLR|RLL|RLR)_[ABC]|D(SLL|SLR|RLL|RLR)_EA|"
            r"(DADD|DADC|DSUB|DSBB|DAN|DOR|DXR)_EA_(BC|DE|HL)|MUL_[ABC])", n)]

    # --- emit helpers
    def emit(self, bs):
        for b in bs:
            self.img[self.pc] = b & 0xFF
            self.pc += 1

    def op(self, name, *args):
        enc, nargs = self.r.choice(self.ops[name])
        assert len(args) == nargs, (name, args, nargs)
        self.emit(enc + list(args))

    def ram(self, lo=0x8100, hi=0x8E00):
        return self.r.randrange(lo, hi)

    def setup_rp(self, rp, val):
        self.op("NOP")
        name = {"BC": "LXI_B_w", "DE": "LXI_D_w", "HL": "LXI_H_w", "EA": "LXI_EA_s",
                "SP": "LXI_S_w"}[rp]
        self.op(name, val & 0xFF, val >> 8)

    def filler(self):
        name = self.r.choice(self.fillers) if self.r.random() < 0.7 else "NOP"
        self.emit_random(name, setups=False)

    # --- one random instruction with its setup
    def emit_random(self, name, setups=True):
        r = self.r
        enc, nargs = r.choice(self.ops[name])
        args = [r.randrange(256) for _ in range(nargs)]
        parts = name.split("_")
        if setups:
            if "wa" in parts or name.endswith("W_wa"):
                self.op("NOP")
                self.op("MVI_V_xx", r.randrange(0x81, 0x8E))
            if "w" in parts and name not in ("JMP_w", "CALL_w") and nargs == 2:
                a = self.ram()
                args = [a & 0xFF, a >> 8]
            m = re.search(r"X_(BC|DE|HL|B|D|H|Dp|Hp|Dm|Hm|D_xx|H_xx|H_A|H_B|H_EA)$", name)
            if m or name.startswith(("LDAX", "STAX", "LDEAX", "STEAX", "MVIX")):
                mode = name.split("_", 1)[1]
                if mode[0] == "B":
                    rp = "BC"
                elif mode[0] == "D":
                    rp = "DE"
                else:
                    rp = "HL"
                self.setup_rp(rp, self.ram(0x8100, 0x8C00))
                if mode == "H_EA":
                    self.setup_rp("EA", r.randrange(0, 0x300))
            if name in ("RLD", "RRD"):
                self.setup_rp("HL", self.ram())
            if name == "BLOCK":
                self.setup_rp("HL", self.ram(0x8100, 0x8600))
                self.setup_rp("DE", self.ram(0x8700, 0x8C00))
                self.op("NOP")
                self.op("MVI_C_xx", r.randrange(0, 12))
            if name == "CALB":
                self.setup_rp("BC", r.randrange(0x0800, 0x1000))
            if name in ("PUSH_BC", "PUSH_DE", "PUSH_HL", "PUSH_EA", "PUSH_VA", "POP_BC",
                        "POP_DE", "POP_HL", "POP_EA", "POP_VA", "INX_SP", "DCX_SP"):
                if r.random() < 0.1:
                    self.setup_rp("SP", r.randrange(0x8D00, 0x8E80))
        # control transfers
        if name == "JMP_w":
            gap = r.randrange(0, 6)
            t = self.pc + len(enc) + 2 + gap
            self.emit(enc + [t & 0xFF, t >> 8]); self.pc = t
            return
        if name == "JR":
            off = r.randrange(0, 32)
            self.emit([0xC0 | off]); self.pc += off
            return
        if name == "JRE":
            off = r.randrange(0, 40)
            self.emit([0x4E, off]); self.pc += off
            return
        if name in ("JB", "JEA"):
            t = self.pc + 4 + len(enc)  # NOP, LXI(3), insn
            self.setup_rp("BC" if name == "JB" else "EA", t)
            self.emit(enc)
            assert self.pc == t
            return
        if name == "CALL_w":
            if r.random() < 0.3:
                t = 0x0700   # RETS stub: skips the next instruction
            else:
                t = r.randrange(0x0800, 0x1000)
            self.emit(enc + [t & 0xFF, t >> 8])
            return
        if name == "CALF":
            self.emit([r.randrange(0x78, 0x80), r.randrange(256)])
            return
        if name == "CALT":
            self.emit([r.randrange(0x80, 0xA0)])
            return
        if name == "TABLE":
            self.emit(enc)
            return
        self.emit(enc + args)

    def vectors(self):
        """JMPs at the interrupt vectors to handlers that clear their flags and return."""
        here = self.pc
        handlers = {0x0004: [], 0x0008: ["FT0", "FT1"], 0x0010: ["F1", "F2"],
                    0x0018: ["FE0", "FE1"], 0x0020: ["FEIN", "FAD"], 0x0028: ["FSR", "FST", "ER"]}
        self.pc = 0x0400
        for vec, flags in handlers.items():
            h = self.pc
            for fl in flags:
                self.op("SKIT_" + fl)
                self.op("NOP")
            if vec != 0x0004:
                self.op("EI")
            self.op("RETI")
            self.img[vec:vec + 3] = bytes([0x54, h & 0xFF, h >> 8])
        self.pc = here

    def build(self):
        r = self.r
        # vectors and stubs
        self.img[0:3] = bytes([0x54, 0x00, 0x10])
        ret = self.ops["RET"][0][0][0]
        reti = self.ops["RETI"][0][0][0]
        rets = self.ops["RETS"][0][0][0]
        self.img[0x60] = reti
        for i in range(32):
            t = 0x0800 + i * 16
            self.img[0x80 + 2 * i] = t & 0xFF
            self.img[0x81 + 2 * i] = t >> 8
        self.img[0x0700] = rets
        for a in range(0x0800, 0x1000):
            self.img[a] = ret
        self.setup_rp("SP", 0x8E80)
        for rp in ("BC", "DE", "HL", "EA"):
            self.setup_rp(rp, r.randrange(0x10000))
        if self.sfr:
            self.vectors()
        while self.pc < 0x7E00:
            if self.sfr and r.random() < 0.02:
                # serial mode without the external SCK (mode 3)
                self.op("NOP")
                self.op("MVI_SMH_xx", r.choice([0, 1, 2]) | (r.randrange(64) << 2))
                self.op("NOP")
                continue
            if self.sfr and r.random() < 0.01:
                # HLT, woken by timer 0
                self.op("NOP")
                self.op("MVI_A_xx", r.randrange(1, 40))
                self.op("MOV_TM0_A")
                self.op("MVI_TMM_xx", 0xE3)
                self.op("ANI_MKL_xx", 0xFD)
                self.op("HLT")
                self.op("NOP")
                continue
            if r.random() < 0.05:
                self.setup_rp("SP", 0x8E80)
            if r.random() < 0.05:
                self.setup_rp(r.choice(["BC", "DE", "HL", "EA"]), r.randrange(0x10000))
            if r.random() < 0.03:
                # backward JR / JRE: P: JRE +n -> T;  S: JMP U;  pad;  T: JR/JRE back to S;  U:
                self.op("NOP")
                p = self.pc
                pad = r.randrange(0, 29)
                s_ = p + 2
                t = s_ + 3 + pad
                self.emit([0x4E, t - (p + 2)])
                u = t + (1 if r.random() < 0.5 else 2)
                self.emit([0x54, u & 0xFF, u >> 8])
                self.pc = t
                if u == t + 1:
                    off = s_ - (t + 1)                  # JR: 6-bit signed
                    self.emit([0xC0 | (off & 0x3F)])
                else:
                    off = s_ - (t + 2)                  # JRE 4F: PC -= 256 - offs
                    self.emit([0x4F, (256 + off) & 0xFF])
                assert self.pc == u
                continue
            if r.random() < 0.01:
                # LSPD from a word embedded in the code
                self.op("NOP")
                self.emit([0xC2]); w = self.pc; self.emit([0x80, 0x8E])   # JR +2 over the word
                enc, _ = self.ops["LSPD_w"][0]
                self.emit(enc + [w & 0xFF, w >> 8])
                self.op("NOP")
                continue
            if r.random() < 0.03:
                # L0/L1 string skips
                self.op("NOP")
                seq = r.choice([["MVI_A_xx", "MVI_A_xx"], ["MVI_L_xx", "MVI_L_xx"],
                                ["LXI_H_w", "MVI_L_xx"], ["MVI_L_xx", "LXI_H_w"]])
                for n in seq:
                    enc, nargs = self.ops[n][0]
                    self.emit(enc + [r.randrange(256) for _ in range(nargs)])
                self.op("NOP")
                continue
            name = r.choice(self.names)
            if name in ("MVI_A_xx", "MVI_L_xx", "LXI_H_w"):
                self.op("NOP")
            self.emit_random(name)
            self.filler()
        while self.pc < 0x7FF0:
            self.emit([0xFF])   # JR -1
        return bytes(self.img)


if __name__ == "__main__":
    ops = load(sys.argv[1])
    g = Gen(ops, int(sys.argv[2]), "--sfr" in sys.argv)
    open(sys.argv[3], "wb").write(g.build())
