#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""CI (no ROMs): hdl/p2k_sndbus.sv, the CPU side of the sound chips, against MAME's emup2k.cpp read
and write handlers (sim/sndbus_tb.cpp): shadows, sound-memory ports, live values, the write FIFO
to the engine, and a user flash SIMM slot against MAME's p2k::intel_flash; three seeds."""
import os
import subprocess
import unittest

from p2k_vlt import LINT_VLT  # tests/ is sys.path[0]

CORE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(CORE, "build", "sndbus_tb")


class SndbusTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        os.makedirs(OUT, exist_ok=True)
        subprocess.run(["verilator", "--cc", os.path.join(CORE, "hdl", "p2k_sndbus.sv"), "--top-module", "p2k_sndbus",
                        "--exe", os.path.join(CORE, "sim", "sndbus_tb.cpp"), "-Mdir", os.path.join(OUT, "obj"),
                        "-O3", "-CFLAGS", "-O2 -std=c++17", "-Wno-style", *LINT_VLT,
                        "--build", "-j", os.environ.get("VERILATOR_JOBS", "4"), "-o", os.path.join(OUT, "sndbus_tb")],
                       check=True, stdout=subprocess.DEVNULL)

    def test_random(self):
        for seed in (1, 2, 3):
            r = subprocess.run([os.path.join(OUT, "sndbus_tb"), "--accesses", "100000", "--seed", str(seed)],
                               capture_output=True, text=True)
            print(r.stdout.strip(), r.stderr.strip()[:500])
            self.assertEqual(r.returncode, 0)
            self.assertIn("identical", r.stdout)


if __name__ == "__main__":
    unittest.main()
