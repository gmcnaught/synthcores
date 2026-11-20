#!/usr/bin/env python3
"""hdl/pcm/sc55_pcm_interp.svh is what tools/gen_pcm_tables.py makes from the reference pcm.cpp."""
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
        assert out.read_text() == (ROOT / "hdl" / "pcm" / "sc55_pcm_interp.svh").read_text()


def test_interp_table_has_2026_fix():
    text = (ROOT / "hdl" / "pcm" / "sc55_pcm_interp.svh").read_text()
    assert "{2'd0, 7'd10}: return 12'd3535;" in text
    assert "{2'd1, 7'd65}: return 12'd2071;" in text


if __name__ == "__main__":
    run(globals())
