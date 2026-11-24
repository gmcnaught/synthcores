#!/usr/bin/env python3
"""Write p2k/rtl/snd/p2k_fx_sine.svh from the chorus sine table in p2k/model/p2k_sound.h (fx_int::sine).

  tools/p2k_fx_sine_gen.py      (run from cores/proteus)
"""
import re

SRC = "p2k/model/p2k_sound.h"
OUT = "p2k/rtl/snd/p2k_fx_sine.svh"


def main():
    s = open(SRC).read()
    key = "static const uint16_t q[1025] = {"
    body = s[s.index(key) + len(key):]
    body = body[:body.index("};")]
    vals = [int(x) for x in re.findall(r"\d+", body)]
    assert len(vals) == 1025, len(vals)
    lines = ["// SPDX-License-Identifier: GPL-3.0-or-later",
             "// Generated from p2k/model/p2k_sound.h fx_int::sine (quarter-wave table, 1025 entries, Q16)",
             "// by tools/p2k_fx_sine_gen.py; do not edit.",
             "function automatic logic [15:0] fx_qsine(input logic [10:0] k);",
             "    case (k)"]
    lines += [f"        11'd{i}: fx_qsine = 16'd{v};" for i, v in enumerate(vals)]
    lines += ["        default: fx_qsine = 16'd0;", "    endcase", "endfunction", ""]
    with open(OUT, "w") as f:
        f.write("\n".join(lines))


if __name__ == "__main__":
    main()
