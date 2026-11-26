#!/usr/bin/env python3
"""Tests for sim/rtp.py (M2.1: running the Release L real-time program).

The multi-floppy file format is tested on a synthetic image. If
SYNCLAVIER_RELEASE_L points to the user-supplied Release L .simg, ORK-LM is
started and run for 9 simulated seconds as well (skipped otherwise).

Run:  python3 tests/sim/test_rtp.py
"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", ".."))
from sim.rtp import rtp_words  # noqa: E402

S = 256


def image_with(parts):
    """Words of a tiny .simg: root catalog of type-7 floppies, each holding
    one type-8 part. parts: list of (disk number, data sectors, total)."""
    nfl = len(parts)
    W = [0] * (4 * S)
    base = 4
    for k, (disk, sectors, total) in enumerate(parts):
        flen = 2 + 1 + sectors          # catalog sector, spare, header + data
        e = k * 8
        W[e:e + 8] = [ord("F") | (ord("0") + k) << 8, 0, 0, 0, base, flen, (flen * S) & 0xFFFF, 7]
        fl = [0] * (flen * S)
        fl[0:8] = [ord("R") | ord("T") << 8, ord("P") | 0, 0, 0, 2, 1 + sectors, ((1 + sectors) * S) & 0xFFFF, 8]
        hdr = [3, ord("R") | ord("T") << 8, ord("P") | ord(" ") << 8, 0x2020, 0x2020, 1, 0, total, disk]
        fl[2 * S:2 * S + len(hdr)] = hdr
        for i in range(sectors * S):
            fl[3 * S + i] = (disk << 12) | (i & 0o7777)
        W += fl
        base += flen
    return W


def test_parts_joined_in_disk_order():
    W = image_with([(2, 2, 5), (1, 3, 5)])   # catalog order differs from disk order
    data = rtp_words(W, "RTP")
    assert len(data) == 5 * S
    assert data[0] == (1 << 12) and data[3 * S] == (2 << 12)


def test_inconsistent_headers_rejected():
    for parts in ([(1, 2, 5), (2, 2, 5)],      # 4 sectors, header says 5
                  [(1, 2, 4), (3, 2, 4)]):     # disk 2 missing
        try:
            rtp_words(image_with(parts), "RTP")
        except ValueError:
            continue
        raise AssertionError("accepted %r" % (parts,))


def test_replace_bank():
    import struct
    import tempfile
    from sim.devices.floppy import Media
    from sim.rtp import replace_bank
    m = Media([7] * 404 * S)
    with tempfile.NamedTemporaryFile(suffix=".stmb", delete=False) as f:
        f.write(struct.pack(">3H", 1, 2, 3))
    try:
        replace_bank(m, f.name, 1)
    finally:
        os.unlink(f.name)
    start = (4 + 49) * S                         # .NEWDATA at sector 4, 49-sector banks
    assert m.words[start - 1] == 7 and m.words[start:start + 4] == [1, 2, 3, 0]
    assert m.words[start + 49 * S] == 7          # next bank untouched


def test_release_l_ork_lm_runs():
    image = os.environ.get("SYNCLAVIER_RELEASE_L")
    if not image:
        raise unittest.SkipTest("SYNCLAVIER_RELEASE_L not set")
    from sim.rtp import start_rtp
    cpu, dbg, lay = start_rtp(image, "ORK-LM")
    assert lay["rtp_words"] == 673 * S
    n0 = len(cpu.terminal.out)
    t0 = cpu.time_ns
    while cpu.time_ns - t0 < 9e9:
        assert dbg.run(1_000_000) is None, dbg.report()
    text = bytes(b & 0o177 for b in cpu.terminal.out[n0:]).decode("latin-1")
    assert "WELCOME TO THE SYNCLAVIER" in text
    assert dbg.io_counts[(0o130, "R")] > 0 and dbg.io_counts[(0o160, "R")] > 0
    assert not cpu.probe.counts, cpu.probe.counts       # every device it touches is modelled
    assert cpu.keyboard.unknown_reads == 0 and cpu.fm.unknown_reads == 0
    assert "9  VOICES" in text                  # one 8-voice card detected (shown + 1)


def test_release_l_ork_lm_plays_a_note():
    image = os.environ.get("SYNCLAVIER_RELEASE_L")
    if not image:
        raise unittest.SkipTest("SYNCLAVIER_RELEASE_L not set")
    from sim.boot import insert_floppy
    from sim.devices.synth import button_number
    from sim.rtp import run_seconds, start_rtp
    cpu, dbg, lay = start_rtp(image, "ORK-LM")
    assert run_seconds(cpu, dbg, 8) is None
    hi = lay["mem_siz"]
    assert [cpu.mem[hi - x] for x in (55, 54, 53)] == [3328, 297, 0o1000]   # LOC.P3: .STAB-6 on F0
    insert_floppy(cpu, image, 0, "06-TIMBD")      # timbre library into F0
    for b in ("bank.0", "timb.0"):
        cpu.keyboard.press_button(button_number(b))
        assert run_seconds(cpu, dbg, 0.1) is None
        cpu.keyboard.press_button(button_number(b), False)
        assert run_seconds(cpu, dbg, 1.5) is None
    n0 = len(cpu.fm.log)
    cpu.keyboard.press_key(48)                     # C4
    assert run_seconds(cpu, dbg, 0.05) is None
    from sim.fm import frequency
    on = {}
    for t, w, fn, v in cpu.fm.log[n0:]:
        if fn in (0, 1, 2, 4):
            on.setdefault(w & 0o377, {}).setdefault(fn, v)
    assert len(on) == 1, on                        # timbre 0: one partial
    (ch, regs), = on.items()
    assert ch & 1 and set(regs) == {0, 1, 2, 4}    # carrier: TIM, ONU, NNU, INC
    assert all(v < 256 for v in regs.values())     # 8-bit data
    hz = frequency(regs[2], regs[1], regs[0])
    assert abs(hz / 523.25 - 1) < 0.002, hz        # key 48 = C4 (NED middle C = C3 = 36)
    # release: ELIM = 0, then ORK-LM polls D160 bit 0 (limit reached) and
    # cleans the voice up (INC = 255, VOL = 0) once the envelope is at 0
    assert cpu.fm.interpolator(ch).value == 250          # attack limit reached
    # M3.2: the rendered note is a C4 sine (timbre 0 = wave memory 4, a sine)
    import math
    from sim.voice import render
    t = cpu.fm.log[n0][0]
    x = [l + r for l, r in render(cpu.fm.log, t + 10_000_000, t + 50_000_000, oversample=1)]

    def level(f):
        w = 2 * math.pi * f / 48_000
        return 2 * math.hypot(sum(v * math.cos(w * i) for i, v in enumerate(x)),
                              sum(v * math.sin(w * i) for i, v in enumerate(x))) / len(x)
    assert level(hz) > 0.9 and level(2 * hz) < 0.01 and level(3 * hz) < 0.01, (level(hz), level(2 * hz))
    cpu.keyboard.press_key(48, False)
    n1 = len(cpu.fm.log)
    assert run_seconds(cpu, dbg, 0.1) is None
    after = [(w & 0o377, fn, v) for t, w, fn, v in cpu.fm.log[n1:]]
    assert (ch, 0o12, 0) in after and (ch, 0, 0o377) in after and (ch, 0o13, 0) in after, after
    assert cpu.fm.interpolator(ch).value == 0



def test_fm_log_keeps_every_write():
    """Review 2026-11-26 sync-py #7: --fm-log wrote at most the FM
    interface's first 100,000 logged writes, with no warning. A stand-in
    RTP (no image) makes 150,000 D162 writes while the key is held."""
    import csv
    import tempfile
    from sim import rtp
    from sim.system import make_system
    calls = []

    def fake_start(image, name="ORK-LM", **kw):
        return make_system(synth=True), None, {"rtp_words": 0}

    def fake_run(cpu, dbg, seconds):
        calls.append(seconds)
        if len(calls) == 2:                     # the key hold
            for i in range(150_000):
                cpu.fm.write(0o162, i & 0xFFFF)
        return None
    saved = rtp.start_rtp, rtp.run_seconds
    rtp.start_rtp, rtp.run_seconds = fake_start, fake_run
    try:
        with tempfile.TemporaryDirectory() as d:
            path = os.path.join(d, "fm.csv")
            assert rtp.main(["none.simg", "--keys", "48", "--hold", "0.1", "--release", "0",
                             "--fm-log", path]) == 0
            with open(path, newline="") as g:
                rows = list(csv.reader(g))
    finally:
        rtp.start_rtp, rtp.run_seconds = saved
    assert len(rows) == 1 + 150_000, len(rows)
    assert rows[-1][4] == "%06o" % (149_999 & 0xFFFF), rows[-1]

if __name__ == "__main__":
    sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
    from testlib import run_tests
    run_tests(globals())
