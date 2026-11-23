#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
# Panel renderers: every bench frame hash equals expected.txt (recorded from the renderers as
# they were before iteration 8 moved them onto modules/synthframe/hdl/panel).
import pathlib, subprocess, sys, tempfile

here = pathlib.Path(__file__).resolve().parent
with tempfile.TemporaryDirectory() as out:
    got = subprocess.run([str(here / "run.sh"), out], check=True, capture_output=True, text=True).stdout
want = (here / "expected.txt").read_text()
if got != want:
    print("panel frame hashes differ\n--- expected\n" + want + "--- got\n" + got)
    sys.exit(1)
print(f"panel frames identical: {len(want.splitlines())} bench/seed hashes")
