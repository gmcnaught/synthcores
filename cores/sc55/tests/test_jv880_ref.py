#!/usr/bin/env python3
"""JV-880 reference (SESSION-PLAN iteration 12): jv880-nvram makes the factory-preset NVRAM
headless, and the reference renders Nuked-SC55's jv880.mid integration test from it.

  JV880_ROM_DIR=DIR python3 tests/test_jv880_ref.py      (skips without the ROMs)

Expected hashes are Nuked-SC55's (doc/building.rst, test/integration/CMakeLists.txt).
"""
import hashlib
import os
import pathlib
import shutil
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


if __name__ == "__main__":
    sys.exit(run(globals()))
