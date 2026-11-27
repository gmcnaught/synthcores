#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""CI (no ROMs): cores/proteus/tools/p2k_build_simm.py refuses a library that does not fit the 2 MB
Baby image (a bytearray slice assignment past the end would grow baby.bin instead), and a small
library still builds a 2 MB Baby with its objects and the 0xFF end marker."""
import os
import sys
import unittest

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
sys.path.insert(0, os.path.join(ROOT, "cores", "proteus", "tools"))
import p2k_build_simm  # noqa: E402

TEMPLATE = bytes(980)          # a pr01 record's size; contents do not matter here


class BuildSimmTest(unittest.TestCase):
    def spec(self, presets):
        return {"id": 4, "name": "T", "instruments": [],
                "presets": [{"name": "P%d" % i, "instrument": 0} for i in range(presets)]}

    def test_fits(self):
        big, baby, info = p2k_build_simm.build(self.spec(4), TEMPLATE)
        self.assertEqual(len(baby), p2k_build_simm.BABY)
        self.assertLess(info["baby_used"], p2k_build_simm.BABY)
        self.assertEqual(baby[info["baby_used"]], 0xFF)

    def test_overflow(self):
        with self.assertRaises(AssertionError):
            p2k_build_simm.build(self.spec(2200), TEMPLATE)    # > 2 MB of presets


if __name__ == "__main__":
    unittest.main()
