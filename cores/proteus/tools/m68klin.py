#!/usr/bin/env python3
"""Linear 68000 disassembly of a byte range: m68klin.py IMAGE START END (hex)."""
import pathlib
import sys
import capstone
d = pathlib.Path(sys.argv[1]).read_bytes()
s, e = int(sys.argv[2], 16), int(sys.argv[3], 16)
md = capstone.Cs(capstone.CS_ARCH_M68K, capstone.CS_MODE_M68K_000)
pc = s
while pc < e:
    ins = next(md.disasm(d[pc:pc + 12], pc, 1), None)
    if ins is None:
        print(f"{pc:06x}: dc.w ${int.from_bytes(d[pc:pc+2], 'big'):04x}")
        pc += 2
        continue
    print(f"{pc:06x}: {ins.mnemonic:8s} {ins.op_str}")
    pc += ins.size
