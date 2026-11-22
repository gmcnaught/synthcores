#!/usr/bin/env python3
"""Generated HDL tables are what the tools make from the reference sources:
modules/roland/hdl/pcm/sc55_pcm_interp.svh (pcm.cpp), hdl/board/lcd_font.svh and lcd_back.hex (lcd_font.h, lcd_back.h)."""
import subprocess
import sys
import tempfile
import pathlib

from testlib import ROOT, run


def test_interp_table_current():
    with tempfile.TemporaryDirectory() as d:
        out = pathlib.Path(d) / "interp.svh"
        subprocess.run([sys.executable, "-I", str(ROOT / "tools" / "gen_pcm_tables.py"),
                        str(ROOT / "third_party" / "nuked-sc55" / "src" / "backend" / "pcm.cpp"), str(out)],
                       check=True, capture_output=True)
        assert out.read_text() == (ROOT.parent.parent / "modules" / "roland" / "hdl" / "pcm" / "sc55_pcm_interp.svh").read_text()


def test_interp_table_has_2026_fix():
    text = (ROOT.parent.parent / "modules" / "roland" / "hdl" / "pcm" / "sc55_pcm_interp.svh").read_text()
    assert "{2'd0, 7'd10}: return 12'd3535;" in text
    assert "{2'd1, 7'd65}: return 12'd2071;" in text


def test_lcd_tables_current():
    with tempfile.TemporaryDirectory() as d:
        subprocess.run([sys.executable, "-I", str(ROOT / "tools" / "gen_lcd_tables.py"),
                        str(ROOT / "third_party" / "nuked-sc55" / "src" / "backend"), d], check=True, capture_output=True)
        for name in ("lcd_font.svh", "lcd_back.hex"):
            assert (pathlib.Path(d) / name).read_text() == (ROOT / "hdl" / "board" / name).read_text(), name


if __name__ == "__main__":
    run(globals())
