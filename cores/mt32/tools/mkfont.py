#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""hdl/mt32_lcd_font.hex from Nuked-MT32's lcd_font.h (160 characters x 8 rows, 5 bits a row).
Character c: c < 4 user (CG RAM); 20-7F -> c - 20; A0-DF -> c - 40; others -> 0."""
import re
from pathlib import Path
core = Path(__file__).resolve().parent.parent
src = (core.parents[1] / "modules/nuked-mt32/lcd_font.h").read_text()
vals = [int(v, 16) for v in re.findall(r"0x([0-9a-fA-F]{2})", src)]
assert len(vals) == 160 * 8, len(vals)
(core / "hdl/mt32_lcd_font.hex").write_text("".join(f"{v:02x}\n" for v in vals))
