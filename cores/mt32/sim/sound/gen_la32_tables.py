#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""Generate the LA32 tables for hdl/la32/ from Nuked-MT32's la32.cpp (powrom, powdrom, logsin).

    gen_la32_tables.py [LA32_CPP] [OUTDIR]

mt32_la32_pow.hex: 512 words of 16 bits, {powdrom[3:0], powrom[11:0]}.
mt32_la32_logsin.hex: 512 words of 13 bits (the '#if 1' table).
"""
import re
import sys
from pathlib import Path

CORE = Path(__file__).resolve().parents[2]


def table(src, name):
    m = re.search(r"static uint32_t %s\[512\] = \{(.*?)\n\};" % name, src, re.S)
    body = m.group(1)
    if "#if 1" in body:
        body = body.split("#if 1", 1)[1].split("#else", 1)[0]
    vals = [int(v) for v in re.findall(r"\d+", body)]
    assert len(vals) == 512, (name, len(vals))
    return vals


def main():
    cpp = Path(sys.argv[1]) if len(sys.argv) > 1 else CORE.parents[1] / "modules" / "nuked-mt32" / "la32.cpp"
    out = Path(sys.argv[2]) if len(sys.argv) > 2 else CORE / "hdl" / "la32"
    src = cpp.read_text()
    pw, pd, ls = table(src, "powrom"), table(src, "powdrom"), table(src, "logsin")
    assert max(pw) < 4096 and max(pd) < 16 and max(ls) < 8192
    hdr = ""  # $readmemh files carry no comments (Quartus); provenance is in mt32_la32.sv
    (out / "mt32_la32_pow.hex").write_text(hdr + "".join(f"{(d << 12) | p:04x}\n" for p, d in zip(pw, pd)))
    (out / "mt32_la32_logsin.hex").write_text(hdr + "".join(f"{v:04x}\n" for v in ls))


if __name__ == "__main__":
    main()
