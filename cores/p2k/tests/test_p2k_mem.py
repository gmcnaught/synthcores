#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""CI (no ROMs): hdl/p2k_mem.sv, the CPU's cache over DDR3 and the flash model, against a flat model
(sim/mem_tb.cpp), random accesses with random memory-port timing, three seeds."""
import os
import subprocess
import unittest

CORE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(CORE, "build", "mem_tb")


class MemTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        os.makedirs(OUT, exist_ok=True)
        subprocess.run(["verilator", "--cc", os.path.join(CORE, "hdl", "p2k_mem.sv"), "--top-module", "p2k_mem",
                        "--exe", os.path.join(CORE, "sim", "mem_tb.cpp"), "-Mdir", os.path.join(OUT, "obj"),
                        "-O3", "-CFLAGS", "-O2 -std=c++17", "-Wno-fatal", "-Wno-lint", "-Wno-style",
                        "--build", "-j", "8", "-o", os.path.join(OUT, "mem_tb")],
                       check=True, stdout=subprocess.DEVNULL)

    def test_random(self):
        for seed in (1, 2, 3):
            r = subprocess.run([os.path.join(OUT, "mem_tb"), "--accesses", "150000", "--seed", str(seed)],
                               capture_output=True, text=True)
            print(r.stdout.strip(), r.stderr.strip()[:500])
            self.assertEqual(r.returncode, 0)
            self.assertIn("identical", r.stdout)


if __name__ == "__main__":
    unittest.main()
