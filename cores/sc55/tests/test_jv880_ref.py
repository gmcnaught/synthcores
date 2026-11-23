#!/usr/bin/env python3
"""JV-880 reference (SESSION-PLAN iteration 12): jv880-nvram makes the factory-preset NVRAM
headless, and the reference renders Nuked-SC55's jv880.mid integration test from it.

  JV880_ROM_DIR=DIR python3 tests/test_jv880_ref.py      (skips without the ROMs)

Expected hashes are Nuked-SC55's (doc/building.rst, test/integration/CMakeLists.txt).
test_mix_saturates: the 20-bit mix saturates instead of wrapping (ref/patch_ref.py FIXES).
"""
import hashlib
import os
import pathlib
import shutil
import struct
import subprocess
import sys
import tempfile

from testlib import ROOT, REF_BUILD, Skip, need_file, run

NVRAM_SHA = "d5da784546f9fd482c82beb366c527f313e8ea81bc9039dbb8c531197aa6d207"
RENDER_SHA = "73feda3c5d7153e2ae04eb3a12e3b4a08e0a0e8eeebb39a77159b45385d3bbbc"
ROMSET = "jv880-v1.0.1"


def rom_dir():
    d = os.environ.get("JV880_ROM_DIR")
    if not d or not pathlib.Path(d).is_dir():
        raise Skip("set JV880_ROM_DIR to the JV-880 ROM dumps")
    return d


def test_factory_nvram_and_render():
    roms = rom_dir()
    nvram_exe = need_file(REF_BUILD / "jv880-nvram", "jv880-nvram")
    render_exe = need_file(REF_BUILD / "sc55-render", "reference renderer")
    tmp = pathlib.Path(tempfile.mkdtemp())
    try:
        nv = tmp / "factory.nvram"
        r = subprocess.run([str(nvram_exe), roms, str(nv), ROMSET], capture_output=True, text=True)
        assert r.returncode == 0, r.stdout[-2000:] + r.stderr[-2000:]
        assert "Are you sure?" in r.stdout, r.stdout[-2000:]
        nv_sha = hashlib.sha256(nv.read_bytes()).hexdigest()
        assert nv_sha == NVRAM_SHA, f"factory NVRAM {nv_sha}"
        # the renderer appends the instance number to --nvram, and saves it back at the end
        shutil.copy(nv, tmp / "render0")
        midi = ROOT / "third_party" / "nuked-sc55" / "test" / "integration" / "jv880" / "jv880.mid"
        p = subprocess.run([str(render_exe), "--quiet", "--stdout", str(midi), "--rom-directory", roms,
                            "--romset", ROMSET, "--nvram", str(tmp / "render")], capture_output=True)
        assert p.returncode == 0, p.stderr[-2000:]
        got = hashlib.sha256(p.stdout).hexdigest()
        assert got == RENDER_SHA, f"jv880.mid render {got}"
        print(f"    factory NVRAM {nv_sha[:8]}..., jv880.mid render {got[:8]}...")
    finally:
        shutil.rmtree(tmp)


def smf(events):
    """Format-0 SMF, 500 ticks a quarter at 120 bpm (1 tick = 1 ms); events = [(ms, bytes)]."""
    trk, t = b"", 0
    for ms, data in sorted(events, key=lambda e: e[0]):
        d, t = ms - t, ms
        vlq = [d & 0x7F]
        while d > 0x7F:
            d >>= 7
            vlq.insert(0, 0x80 | (d & 0x7F))
        trk += bytes(vlq) + bytes(data)
    trk += b"\x00\xff\x2f\x00"
    return b"MThd" + struct.pack(">IHHH", 6, 0, 1, 500) + b"MTrk" + struct.pack(">I", len(trk)) + trk


def test_mix_saturates():
    """I-22 SAW Lead (program 21 in the factory preset), one note then a 4-note chord at velocity
    127: the mix reaches the 20-bit rail and stays there (68 samples); the fork's plain add
    wrapped it to the other rail (8 times)."""
    roms = rom_dir()
    nvram_exe = need_file(REF_BUILD / "jv880-nvram", "jv880-nvram")
    render_exe = need_file(REF_BUILD / "sc55-render", "reference renderer")
    tmp = pathlib.Path(tempfile.mkdtemp())
    try:
        nv = tmp / "factory.nvram"
        r = subprocess.run([str(nvram_exe), roms, str(nv), ROMSET], capture_output=True, text=True)
        assert r.returncode == 0, r.stdout[-2000:] + r.stderr[-2000:]
        shutil.copy(nv, tmp / "render0")
        chord = (0x30, 0x37, 0x3C, 0x40)
        ev = [(500, (0xC0, 21)), (1000, (0x90, 0x3C, 127)), (2500, (0x80, 0x3C, 64))]
        ev += [(3000, (0x90, n, 127)) for n in chord] + [(5000, (0x80, n, 64)) for n in chord]
        (tmp / "chord.mid").write_bytes(smf(ev))
        p = subprocess.run([str(render_exe), "--quiet", "--stdout", "-f", "f32", str(tmp / "chord.mid"),
                            "--rom-directory", roms, "--romset", ROMSET, "--nvram", str(tmp / "render")],
                           capture_output=True)
        assert p.returncode == 0, p.stderr[-2000:]
        f = struct.unpack(f"<{len(p.stdout) // 4}f", p.stdout)
        v = [round(x * (1 << 17)) for x in f]          # f32 = sample / 2^29; sample = 20-bit mix << 12
        rail = sum(1 for x in v if x in (-(1 << 19), (1 << 19) - 1))
        wraps = sum(1 for ch in (v[0::2], v[1::2]) for a, b in zip(ch, ch[1:]) if abs(b - a) > (1 << 19))
        assert max(abs(x) for x in v) <= 1 << 19
        assert rail > 0, "the case no longer reaches the 20-bit rail"
        assert wraps == 0, f"{wraps} wraps"
        print(f"    {len(v) // 2} frames, {rail} samples at the 20-bit rail, no wraps")
    finally:
        shutil.rmtree(tmp)


if __name__ == "__main__":
    sys.exit(run(globals()))
