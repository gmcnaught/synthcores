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



def test_mras_current():
    """mra/*.mra are what romset.py mra writes (regenerate: python3 tools/romset.py mra mra)."""
    for rs, (name, *_rest) in romset.MAME_SETS.items():
        path = ROOT / "mra" / f"{name}.mra"
        assert path.read_text() == romset.mra(rs, romset.MRA_MD5[rs]), f"{path} is stale"


def test_mra_assembles_pack_v3():
    """Each MRA, assembled from the local dumps by CRC32 as Main_MiSTer does, gives the v3 pack and
    the MD5 in the MRA (so MAME's CRC32s are those of Nuked-SC55's romsets)."""
    import hashlib
    for rs in romset.MAME_SETS:
        need_roms(rs)
        from testlib import ROM_DIR
        built = romset.assemble_mra(rs, ROM_DIR)
        assert built == romset.pack(ROM_DIR, rs, v3=True), rs
        assert hashlib.md5(built).hexdigest() == romset.MRA_MD5[rs], rs


def test_unscramble_is_its_bit_tables():
    """The v2 pack's unscramble is the bit permutation rom_loader.sv applies to v3 packs."""
    import random
    rng = random.Random(1)
    src = bytes(rng.randrange(256) for _ in range(0x100000))
    out = romset.unscramble(src)
    AB = [2, 0, 3, 4, 1, 9, 13, 10, 18, 17, 6, 15, 11, 16, 8, 5, 12, 7, 14, 19]
    DB = [2, 0, 4, 5, 7, 6, 3, 1]
    for a in rng.sample(range(0x100000), 2000):   # the loader: dump byte at a goes to i, data bits permuted
        i = sum(((a >> AB[b]) & 1) << b for b in range(20))
        d = sum(((src[a] >> DB[j]) & 1) << j for j in range(8))
        assert out[i] == d, (a, i)

if __name__ == "__main__":
    run(globals())
