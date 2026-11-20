#!/usr/bin/env python3
"""No Roland ROMs, MIDI-derived renders or built cores in the repository (plan.md R6)."""
import hashlib
import subprocess
import sys

from testlib import ROOT, run

FORBIDDEN = (".bin", ".rom", ".rbf", ".zip", ".wav", ".raw", ".sst")
MAX_BYTES = 256 * 1024


def tracked():
    out = subprocess.run(["git", "ls-files", "-z"], cwd=ROOT, capture_output=True, check=True).stdout
    return [p for p in out.decode().split("\0") if p and not p.startswith("third_party/")]


def test_no_forbidden_types():
    bad = [p for p in tracked() if p.lower().endswith(FORBIDDEN)]
    assert not bad, f"files that must not be committed: {bad}"


def test_no_large_files():
    big = [p for p in tracked() if (ROOT / p).is_file() and (ROOT / p).stat().st_size > MAX_BYTES]
    assert not big, f"files over {MAX_BYTES} bytes: {big}"


def test_no_rom_content():
    sys.path.insert(0, str(ROOT / "tools"))
    import romset
    known = {h for rs in romset.load_table().values() for h in rs.values()}
    bad = []
    for p in tracked():
        f = ROOT / p
        if f.is_file() and f.stat().st_size >= 4096:
            if hashlib.sha256(f.read_bytes()).hexdigest() in known:
                bad.append(p)
    assert not bad, f"ROM dumps committed: {bad}"


if __name__ == "__main__":
    run(globals())
