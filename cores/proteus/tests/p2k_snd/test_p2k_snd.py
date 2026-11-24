#!/usr/bin/env python3
"""P2K sound HDL against the integer model on random stimuli (no ROMs needed).

Builds p2k/model/p2k_render and the Verilator bench p2k/sim/p2k_snd_tb, makes a random-data SIMM and
random register logs (tools/p2k_snd_randlog.py: voices, loops, interpolators, routing, filter
coefficients, 12th-order chains, G05 effect registers) and checks that the bench output equals the
renderer's bit for bit in the three modes of iteration 14 (voices, + filter, + effects).
The corpus gate with the real SIMM and firmware logs is tools/p2k_snd_check.py.
"""
import hashlib
import os
import random
import shutil
import subprocess
import sys
import tempfile
import unittest

CORE = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
MODES = {"voices": ["--nofilter", "--nofx"], "filter": ["--nofx"], "full": []}
WORDS = 1 << 20
FRAMES = 12000


def sha(path):
    with open(path, "rb") as f:
        return hashlib.sha256(f.read()).hexdigest()


@unittest.skipUnless(shutil.which("verilator"), "verilator not installed")
class P2kSnd(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.mkdtemp(prefix="p2k_snd_")
        if "CXX" not in os.environ and shutil.which("g++-14"):   # CI builds the model with g++-14
            os.environ["CXX"] = "g++-14"
        cls.render = subprocess.run([os.path.join(CORE, "p2k", "model", "build.sh"), os.path.join(cls.tmp, "model")],
                                    check=True, stdout=subprocess.PIPE, text=True).stdout.strip().splitlines()[-1]
        cls.tb = subprocess.run([os.path.join(CORE, "p2k", "sim", "build_snd.sh"), os.path.join(cls.tmp, "snd")],
                                check=True, stdout=subprocess.PIPE, text=True).stdout.strip().splitlines()[-1]
        rnd = random.Random(2026)
        cls.simm = os.path.join(cls.tmp, "simm.bin")
        with open(cls.simm, "wb") as f:    # band-limited-ish random words so the filters see signal
            x, data = 0, bytearray()
            for _ in range(WORDS):
                x = max(-32768, min(32767, x + rnd.randrange(-3000, 3001)))
                data += (x & 0xFFFF).to_bytes(2, "big")
            f.write(data)

    @classmethod
    def tearDownClass(cls):
        shutil.rmtree(cls.tmp, ignore_errors=True)

    def check(self, seed, mode, extra):
        log = os.path.join(self.tmp, f"r{seed}.log")
        subprocess.run([sys.executable, os.path.join(CORE, "tools", "p2k_snd_randlog.py"), str(seed), log,
                        "--samples", str(FRAMES - 1000), "--voices", "48", "--words", str(WORDS)] + extra, check=True)
        opts = MODES[mode] + ["--simm", "1=" + self.simm, "--frames", str(FRAMES)]
        ref, hdl = os.path.join(self.tmp, f"r{seed}.{mode}.ref.raw"), os.path.join(self.tmp, f"r{seed}.{mode}.hdl.raw")
        subprocess.run([self.render] + opts + [log, ref], check=True, stdout=subprocess.DEVNULL)
        subprocess.run([self.tb] + opts + [log, hdl], check=True, stdout=subprocess.DEVNULL)
        cmp = subprocess.run([self.render, "--compare", ref, hdl], stdout=subprocess.PIPE, text=True).stdout.strip()
        self.assertEqual(sha(ref), sha(hdl), f"seed {seed} {mode}: {cmp}")
        with open(ref, "rb") as f:
            self.assertTrue(any(f.read()), f"seed {seed} {mode}: silent reference")

    def test_voices(self):
        for seed in (1, 2):
            self.check(seed, "voices", [])

    def test_filter(self):
        for seed in (3, 4):
            self.check(seed, "filter", [])

    def test_full(self):
        for seed in (5, 6):
            self.check(seed, "full", ["--fx"])


if __name__ == "__main__":
    unittest.main()
