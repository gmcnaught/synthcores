#!/usr/bin/env python3
"""The hooked reference build (decoder2 off) renders the same audio as the released one."""
import hashlib
import subprocess

from testlib import MIDI_DIR, REF_BUILD, ROM_DIR, need_file, need_roms, run


def render(exe, midi):
    out = subprocess.run([str(exe), "--quiet", "--stdout", "-d", str(ROM_DIR), "--romset", "mk1-v1.21",
                          "--reset", "gm", str(midi)], capture_output=True, check=True).stdout
    return hashlib.sha256(out).hexdigest(), len(out)


def test_traced_equals_plain():
    need_roms("mk1-v1.21")
    plain = need_file(REF_BUILD / "sc55-render", "reference renderer")
    traced = need_file(REF_BUILD / "sc55-render-traced", "traced renderer")
    midi = MIDI_DIR / "avmidi" / "0A.mid"
    a, b = render(plain, midi), render(traced, midi)
    assert a == b, (a, b)
    assert a[1] > 1_000_000, a


if __name__ == "__main__":
    run(globals())
