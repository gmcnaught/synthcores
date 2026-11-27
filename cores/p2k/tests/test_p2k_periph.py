#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""CI (no ROMs): the MCF5206e peripherals (cores/proteus/p2k/rtl/cpu/mcf5206e_periph.sv), sim/periph_tb.cpp:
UART1 receive/transmit with CPU accesses on every clock phase (a receive-buffer read or a command
on the clock a byte completes must not lose it), timer reference events against TER clears, timer
reset as MAME's coldfire_timer_device, chip-select reset values and the interrupt level; three seeds."""
import os
import subprocess
import unittest

from p2k_vlt import LINT_VLT  # tests/ is sys.path[0]

CORE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ROOT = os.path.dirname(os.path.dirname(CORE))
OUT = os.path.join(CORE, "build", "periph_tb")


class PeriphTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        os.makedirs(OUT, exist_ok=True)
        subprocess.run(["verilator", "--cc", os.path.join(ROOT, "cores", "proteus", "p2k", "rtl", "cpu", "mcf5206e_periph.sv"),
                        "--top-module", "mcf5206e_periph",
                        "--exe", os.path.join(CORE, "sim", "periph_tb.cpp"), "-Mdir", os.path.join(OUT, "obj"),
                        "-O3", "-CFLAGS", "-O2 -std=c++17", "-Wno-style", *LINT_VLT,
                        "--build", "-j", os.environ.get("VERILATOR_JOBS", "4"), "-o", os.path.join(OUT, "periph_tb")],
                       check=True, stdout=subprocess.DEVNULL)

    def test_random(self):
        for seed in (1, 2, 3):
            r = subprocess.run([os.path.join(OUT, "periph_tb"), "--seed", str(seed), "--bytes", "5000"],
                               capture_output=True, text=True)
            print(r.stdout.strip()[-800:], r.stderr.strip()[:500])
            self.assertEqual(r.returncode, 0)
            self.assertIn("all passed", r.stdout)


if __name__ == "__main__":
    unittest.main()
