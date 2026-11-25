#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""CI (no ROMs): hdl/p2k_imgsave.sv, the flash save file and the flash SIMM files, against hps_io's SD
side and a DDR3 model (sim/imgsave_tb.cpp): restore and autosave of the save file, SIMM file loads
(odd size, the rest erased), write-back of the regions the OS changed, a ROM download or a pack
download stopping a transfer, read-only and empty mounts; three seeds."""
import os
import subprocess
import unittest

CORE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(CORE, "build", "imgsave_tb")


class ImgsaveTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        os.makedirs(OUT, exist_ok=True)
        subprocess.run(["verilator", "--cc", os.path.join(CORE, "hdl", "p2k_imgsave.sv"), "--top-module", "p2k_imgsave",
                        "--exe", os.path.join(CORE, "sim", "imgsave_tb.cpp"), "-Mdir", os.path.join(OUT, "obj"),
                        "-O3", "-CFLAGS", "-O2 -std=c++17", "-Wno-fatal", "-Wno-lint", "-Wno-style",
                        "--build", "-j", "8", "-o", os.path.join(OUT, "imgsave_tb")],
                       check=True, stdout=subprocess.DEVNULL)

    def test_seeds(self):
        for seed in (1, 2, 3):
            r = subprocess.run([os.path.join(OUT, "imgsave_tb"), "--seed", str(seed)], capture_output=True, text=True)
            print(r.stdout.strip(), r.stderr.strip()[:500])
            self.assertEqual(r.returncode, 0)
            self.assertIn("identical", r.stdout)


if __name__ == "__main__":
    unittest.main()
