#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Generate the uPD7810/7811 decode ROM from MAME's opcode tables.

Usage: upd7810_gen.py MAME_DIR OUT_DIR

Writes OUT_DIR/upd7810_decode.hex (2048 words, index = table << 8 | opcode, table order
XX 48 4C 4D 60 64 70 74) and OUT_DIR/upd7810_decode.txt (a readable listing).

Every word is derived from the handler name in MAME's table; the handler bodies were read to
fix each class's semantics (cores/cz101/docs/upd7810.md). Cycle, skip-cycle, length and
L0/L1-mask columns are copied from the table, so timing matches MAME by construction.

Word layout (LSB first), see upd7810_pkg.sv:
  [2:0]   len        opcode length in bytes (table oplen)
  [8:3]   cyc        cycles (table cycles)
  [12:9]  cyc_skip   cycles when skipped (table cycles_skip, max 14)
  [14:13] lmask      PSW L0/L1 bits this opcode clears
  [20:15] cls        class
  [25:21] aop        ALU / sub-operation
  [32:26] s1         first operand selector (destination)
  [39:33] s2         second operand selector (source)
  [43:40] mode       memory addressing mode
  [45:44] lskip      1 = skip args if L0 set, 2 = if L1 set
  [47:46] nargs      operand bytes read with RDOPARG
"""
import json, re, sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from upd7810_tables import extract  # noqa: E402

TABLE_ORDER = ["XX", "48", "4C", "4D", "60", "64", "70", "74"]

# --- operand selectors (7 bits): 0..9 registers, 0x20|r = 8-bit register in a 16-bit op,
#     64 + id special registers, 0x7E immediate, 0x7F memory
R8 = {"V": 0, "A": 1, "B": 2, "C": 3, "D": 4, "E": 5, "H": 6, "L": 7, "EAH": 8, "EAL": 9}
# 16-bit register pairs, used by s1/s2 in 16-bit classes
R16 = {"SP": 0, "BC": 1, "DE": 2, "HL": 3, "EA": 4, "VA": 5}
SFR = {"PA": 0, "PB": 1, "PC": 2, "PD": 3, "PF": 4, "MKH": 5, "MKL": 6, "ANM": 7, "SMH": 8,
       "SML": 9, "EOM": 10, "ETMM": 11, "TMM": 12, "MM": 13, "MCC": 14, "MA": 15, "MB": 16,
       "MC": 17, "MF": 18, "TXB": 19, "RXB": 20, "TM0": 21, "TM1": 22, "CR0": 23, "CR1": 24,
       "CR2": 25, "CR3": 26, "ZCM": 27, "MT": 28, "PT": 29,
       # 16-bit
       "ETM0": 30, "ETM1": 31, "ECNT": 32, "ECPT": 33}
SFR_BASE = 64  # selector = 64 + id (bit 6); 0x7E = immediate, 0x7F = memory

def sel(name):
    if name in R8:
        return R8[name]
    if name in SFR:
        return SFR_BASE + SFR[name]
    raise KeyError(name)

# --- addressing modes
MODE = {"B": 0, "D": 1, "H": 2, "Dp": 3, "Hp": 4, "Dm": 5, "Hm": 6, "D_xx": 7, "H_A": 8,
        "H_B": 9, "H_EA": 10, "H_xx": 11, "Dp2": 12, "Hp2": 13, "wa": 14, "w": 15}

# --- classes
CLS = [
    "NOP", "ALU8", "ALU16", "MOV16", "LXI", "LD16", "ST16", "PUSH", "POP", "INCDEC8", "INX",
    "DCX", "SHIFT8", "SHIFT16", "RLD", "RRD", "DAA", "NEGA", "MUL", "DIV", "BLOCK", "TABLE",
    "EX", "JMP", "JR", "JRE", "JB", "JEA", "CALL", "CALB", "CALF", "CALT", "RET", "RETS", "RETI",
    "SOFTI", "BIT", "SK", "SKIT", "EI", "DI", "HLT", "STOP", "CLC", "STC", "PREFIX", "ILLEGAL0",
]
C = {n: i for i, n in enumerate(CLS)}

# --- ALU operations (8-bit and 16-bit share codes)
AOP = ["MOV", "ADD", "ADC", "SUB", "SBB", "ANA", "ORA", "XRA", "ADDNC", "SUBNB", "GTA", "LTA",
       "NEA", "EQA", "ONA", "OFFA"]
A = {n: i for i, n in enumerate(AOP)}
# immediate-form names -> ALU op
IMM_AOP = {"MVI": "MOV", "ADI": "ADD", "ACI": "ADC", "SUI": "SUB", "SBI": "SBB", "ANI": "ANA",
           "ORI": "ORA", "XRI": "XRA", "ADINC": "ADDNC", "SUINB": "SUBNB", "GTI": "GTA",
           "LTI": "LTA", "NEI": "NEA", "EQI": "EQA", "ONI": "ONA", "OFFI": "OFFA"}
D16_AOP = {"DADD": "ADD", "DADC": "ADC", "DSUB": "SUB", "DSBB": "SBB", "DAN": "ANA",
           "DOR": "ORA", "DXR": "XRA", "DADDNC": "ADDNC", "DSUBNB": "SUBNB", "DGT": "GTA",
           "DLT": "LTA", "DNE": "NEA", "DEQ": "EQA", "DON": "ONA", "DOFF": "OFFA"}

# SKIT / SKNIT flag ids (peripheral side); NMI is special
SKIT_ID = {"F0": 0, "FT0": 1, "FT1": 2, "F1": 3, "F2": 4, "FE0": 5, "FE1": 6, "FEIN": 7,
           "FAD": 8, "FSR": 9, "FST": 10, "ER": 11, "OV": 12, "AN4": 13, "AN5": 14, "AN6": 15,
           "AN7": 16, "SB": 17, "NMI": 18}

SHIFT8 = {"SLL": 0, "SLR": 1, "SLLC": 2, "SLRC": 3, "RLL": 4, "RLR": 5}
SHIFT16 = {"DSLL": 0, "DSLR": 1, "DRLL": 2, "DRLR": 3}
EXOP = {"EXA": 1, "EXX": 2, "EXH": 4, "EXR": 3}  # bit0: EA/VA, bit1: BC/DE/HL, bit2: HL only
SKCOND = {"CY": 0, "HC": 1, "Z": 2, "NV": 3}


def word(len_, cyc, cyc_skip, lmask, cls, aop=0, s1=0, s2=0, mode=0, lskip=0, nargs=0):
    assert cyc < 64 and cyc_skip < 16 and len_ < 8
    w = len_ | cyc << 3 | cyc_skip << 9 | lmask << 13 | cls << 15 | aop << 21
    w |= s1 << 26 | s2 << 33 | mode << 40 | lskip << 44 | nargs << 46
    return w


def classify(name):
    """Return dict(cls, aop, s1, s2, mode, lskip, nargs) for a MAME handler name."""
    d = dict(aop=0, s1=0, s2=0, mode=0, lskip=0, nargs=0)

    def r(**kw):
        d.update(kw)
        return d

    if name in ("NOP", "SK_NV", "illegal", "illegal2"):
        return r(cls=C["NOP"])
    if name.startswith("PRE_"):
        return r(cls=C["PREFIX"])
    for simple in ("BLOCK", "TABLE", "RLD", "RRD", "DAA", "NEGA", "JB", "JEA", "CALB", "CALT",
                   "RET", "RETS", "RETI", "SOFTI", "EI", "DI", "HLT", "STOP", "CLC", "STC", "JR"):
        if name == simple:
            return r(cls=C[simple])
    if name == "JRE":
        return r(cls=C["JRE"], nargs=1)
    if name == "CALF":
        return r(cls=C["CALF"], nargs=1)
    if name == "JMP_w":
        return r(cls=C["JMP"], nargs=2)
    if name == "CALL_w":
        return r(cls=C["CALL"], nargs=2)
    if name in EXOP:
        return r(cls=C["EX"], aop=EXOP[name])

    m = re.fullmatch(r"(SKN?)_(CY|HC|Z|NV)", name)
    if m:
        return r(cls=C["SK"], aop=SKCOND[m[2]] | (4 if m[1] == "SKN" else 0))
    m = re.fullmatch(r"(SKN?IT)_(\w+)", name)
    if m:
        return r(cls=C["SKIT"], aop=SKIT_ID[m[2]] | (0x20 if False else 0),
                 s1=1 if m[1] == "SKNIT" else 0)
    m = re.fullmatch(r"BIT_(\d)_wa", name)
    if m:
        return r(cls=C["BIT"], aop=int(m[1]), mode=MODE["wa"], nargs=1)

    # 8-bit register-register ALU: OP_X_Y
    m = re.fullmatch(r"(ADD|ADC|SUB|SBB|ANA|ORA|XRA|ADDNC|SUBNB|GTA|LTA|NEA|EQA|ONA|OFFA)_(\w+)_(\w+)", name)
    if m and m[2] in R8 and m[3] in R8:
        return r(cls=C["ALU8"], aop=A[m[1]], s1=R8[m[2]], s2=R8[m[3]])
    # memory forms: OPX_rpa (A op (rpa)), OPW_wa (A op (V:wa))
    m = re.fullmatch(r"(ADD|ADC|SUB|SBB|ANA|ORA|XRA|ADDNC|SUBNB|GTA|LTA|NEA|EQA|ONA|OFFA)X_(\w+)", name)
    if m:
        # MAME's ADCX handlers pass carry 0 to ZHC_ADD (ADC/ADCW/ACI pass CY): aop 16
        return r(cls=C["ALU8"], aop=16 if m[1] == "ADC" else A[m[1]], s1=R8["A"], s2=0x7F,
                 mode=MODE[m[2]])
    m = re.fullmatch(r"(ADD|ADC|SUB|SBB|ANA|ORA|XRA|ADDNC|SUBNB|GTA|LTA|NEA|EQA|ONA|OFFA)W_wa", name)
    if m:
        return r(cls=C["ALU8"], aop=A[m[1]], s1=R8["A"], s2=0x7F, mode=MODE["wa"], nargs=1)
    # immediate forms: OPI_r_xx, OPIW_wa_xx
    m = re.fullmatch(r"(MVI|ADI|ACI|SUI|SBI|ANI|ORI|XRI|ADINC|SUINB|GTI|LTI|NEI|EQI|ONI|OFFI)_(\w+)_xx", name)
    if m:
        lskip = {"MVI_A_xx": 2, "MVI_L_xx": 1}.get(name, 0)
        return r(cls=C["ALU8"], aop=A[IMM_AOP[m[1]]], s1=sel(m[2]), s2=0x7E, lskip=lskip, nargs=1)
    m = re.fullmatch(r"(MVI|ADI|ACI|SUI|SBI|ANI|ORI|XRI|ADINC|SUINB|GTI|LTI|NEI|EQI|ONI|OFFI)W_wa_xx", name)
    if m:
        return r(cls=C["ALU8"], aop=A[IMM_AOP[m[1]]], s1=0x7F, s2=0x7E, mode=MODE["wa"], nargs=2)
    m = re.fullmatch(r"MVIX_(BC|DE|HL)_xx", name)
    if m:
        return r(cls=C["ALU8"], aop=A["MOV"], s1=0x7F, s2=0x7E, mode=MODE[m[1][0]], nargs=1)

    # 8-bit moves
    m = re.fullmatch(r"MOV_(\w+)_(\w+)", name)
    if m:
        dst, src = m[1], m[2]
        if dst == "w":
            return r(cls=C["ALU8"], aop=A["MOV"], s1=0x7F, s2=R8[src], mode=MODE["w"], nargs=2)
        if src == "w":
            return r(cls=C["ALU8"], aop=A["MOV"], s1=R8[dst], s2=0x7F, mode=MODE["w"], nargs=2)
        return r(cls=C["ALU8"], aop=A["MOV"], s1=sel(dst), s2=sel(src))
    if name == "LDAW_wa":
        return r(cls=C["ALU8"], aop=A["MOV"], s1=R8["A"], s2=0x7F, mode=MODE["wa"], nargs=1)
    if name == "STAW_wa":
        return r(cls=C["ALU8"], aop=A["MOV"], s1=0x7F, s2=R8["A"], mode=MODE["wa"], nargs=1)
    m = re.fullmatch(r"LDAX_(\w+)", name)
    if m:
        return r(cls=C["ALU8"], aop=A["MOV"], s1=R8["A"], s2=0x7F, mode=MODE[m[1]],
                 nargs=1 if m[1].endswith("xx") else 0)
    m = re.fullmatch(r"STAX_(\w+)", name)
    if m:
        return r(cls=C["ALU8"], aop=A["MOV"], s1=0x7F, s2=R8["A"], mode=MODE[m[1]],
                 nargs=1 if m[1].endswith("xx") else 0)

    # 16-bit
    m = re.fullmatch(r"(D[A-Z]+)_EA_(BC|DE|HL)", name)
    if m and m[1] in D16_AOP:
        return r(cls=C["ALU16"], aop=A[D16_AOP[m[1]]], s1=R16["EA"], s2=R16[m[2]])
    m = re.fullmatch(r"(EADD|ESUB)_EA_(A|B|C)", name)
    if m:
        return r(cls=C["ALU16"], aop=A["ADD" if m[1] == "EADD" else "SUB"], s1=R16["EA"],
                 s2=0x20 | R8[m[2]])
    m = re.fullmatch(r"DMOV_(\w+)_(\w+)", name)
    if m:
        def s16(x):
            return R16[x] if x in R16 else SFR_BASE + SFR[x]
        return r(cls=C["MOV16"], s1=s16(m[1]), s2=s16(m[2]))
    m = re.fullmatch(r"LXI_(B|D|H|EA|S)_[ws]", name)
    if m:
        rp = {"B": "BC", "D": "DE", "H": "HL", "EA": "EA", "S": "SP"}[m[1]]
        return r(cls=C["LXI"], s1=R16[rp], lskip=1 if m[1] == "H" else 0, nargs=2)
    for nm, rp, ld in (("LBCD_w", "BC", 1), ("SBCD_w", "BC", 0), ("LDED_w", "DE", 1),
                       ("SDED_w", "DE", 0), ("LHLD_w", "HL", 1), ("SHLD_w", "HL", 0),
                       ("LSPD_w", "SP", 1), ("SSPD_w", "SP", 0)):
        if name == nm:
            return r(cls=C["LD16" if ld else "ST16"], s1=R16[rp], mode=MODE["w"], nargs=2)
    m = re.fullmatch(r"(LDEAX|STEAX)_(\w+)", name)
    if m:
        mode = {"Dp": "Dp2", "Hp": "Hp2"}.get(m[2], m[2])
        return r(cls=C["LD16" if m[1] == "LDEAX" else "ST16"], s1=R16["EA"], mode=MODE[mode],
                 nargs=1 if m[2].endswith("xx") else 0)
    m = re.fullmatch(r"(PUSH|POP)_(BC|DE|HL|EA|VA)", name)
    if m:
        return r(cls=C[m[1]], s1=R16[m[2]])
    m = re.fullmatch(r"(INR|DCR)_(A|B|C)", name)
    if m:
        return r(cls=C["INCDEC8"], aop=0 if m[1] == "INR" else 1, s1=R8[m[2]])
    m = re.fullmatch(r"(INR|DCR)W_wa", name)
    if m:
        return r(cls=C["INCDEC8"], aop=0 if m[1] == "INR" else 1, s1=0x7F, mode=MODE["wa"], nargs=1)
    m = re.fullmatch(r"(INX|DCX)_(BC|DE|HL|EA|SP)", name)
    if m:
        return r(cls=C[m[1]], s1=R16[m[2]])
    m = re.fullmatch(r"(SLL|SLR|SLLC|SLRC|RLL|RLR)_(A|B|C)", name)
    if m:
        return r(cls=C["SHIFT8"], aop=SHIFT8[m[1]], s1=R8[m[2]])
    m = re.fullmatch(r"(DSLL|DSLR|DRLL|DRLR)_EA", name)
    if m:
        return r(cls=C["SHIFT16"], aop=SHIFT16[m[1]])
    m = re.fullmatch(r"(MUL|DIV)_(A|B|C)", name)
    if m:
        return r(cls=C[m[1]], s2=R8[m[2]])
    raise ValueError(f"unclassified handler {name}")


def main():
    mame, out = sys.argv[1], Path(sys.argv[2])
    tables = extract(mame)
    out.mkdir(parents=True, exist_ok=True)
    words, listing = [], []
    for ti, t in enumerate(TABLE_ORDER):
        for op, (name, oplen, cyc, cyc_skip, lmask) in enumerate(tables[t]):
            d = classify(name)
            if t == "XX" and name == "illegal" and cyc == 0:
                d["cls"] = C["ILLEGAL0"]  # 73: zero cycles in MAME
            base = 1 if t == "XX" else 2
            if d["cls"] not in (C["NOP"], C["PREFIX"], C["ILLEGAL0"]) and d["nargs"] != oplen - base:
                raise ValueError(f"{t} {op:02X} {name}: nargs {d['nargs']} != len {oplen} - {base}")
            w = word(oplen, cyc, cyc_skip, lmask, **d)
            words.append(w)
            listing.append(f"{t} {op:02X} {name:16s} len={oplen} cyc={cyc:2d}/{cyc_skip:2d} "
                           f"L={lmask} {CLS[d['cls']]:7s} aop={d['aop']:2d} s1={d['s1']:3d} "
                           f"s2={d['s2']:3d} mode={d['mode']:2d} lskip={d['lskip']} nargs={d['nargs']}")
    (out / "upd7810_decode.hex").write_text("".join(f"{w:012X}\n" for w in words))
    (out / "upd7810_decode.txt").write_text("\n".join(listing) + "\n")
    print(f"{len(words)} words -> {out}/upd7810_decode.hex")


if __name__ == "__main__":
    main()
