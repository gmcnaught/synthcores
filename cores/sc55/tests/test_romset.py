#!/usr/bin/env python3
"""tools/romset.py reads the reference romset table and finds the local ROMs."""
import sys

from testlib import ROOT, need_roms, run

sys.path.insert(0, str(ROOT / "tools"))
import romset  # noqa: E402


def test_table_has_mk1():
    t = romset.load_table()
    assert set(t["mk1-v1.21"]) == {"ROM1", "ROM2", "WAVEROM1", "WAVEROM2", "WAVEROM3"}, t["mk1-v1.21"]
    assert t["mk1-v1.21"]["ROM1"].startswith("7e1bacd1")
    assert "mk2-v1.01" in t and "SMROM" in t["mk2-v1.01"]


def test_local_mk1_v121():
    roms = need_roms("mk1-v1.21")
    sizes = {loc: p.stat().st_size for loc, p in roms.items()}
    assert sizes == {"ROM1": 0x8000, "ROM2": 0x40000, "WAVEROM1": 0x100000,
                     "WAVEROM2": 0x100000, "WAVEROM3": 0x100000}, sizes


if __name__ == "__main__":
    run(globals())
