#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""Render the MT-32 MIDI corpus with a renderer and record or check the output hashes.

    corpus.py record [--renderer CMD]     render every tests/midi/*.txt, write tests/corpus.json
    corpus.py check  [--renderer CMD]     render again, compare with tests/corpus.json
    corpus.py check --case NAME ...       only these cases

The renderer is build/ref/mt32_ref by default; the HDL benches take the same options
(--roms --midi --samples --out). ROMs come from MT32_ROM_DIR; without it the script prints SKIP
and exits 77. `check` fails when no case was compared.
"""
import argparse
import hashlib
import json
import os
import shlex
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

CORE = Path(__file__).resolve().parent.parent
MIDI = CORE / "tests" / "midi"
DB = CORE / "tests" / "corpus.json"


def samples_of(path):
    first = path.read_text().splitlines()[0]
    assert first.startswith("# samples "), path
    return int(first.split()[2])


def extra_args(path):
    """Renderer options from "# args ..." lines (panel buttons, knob: --button/--knob)."""
    out = []
    for line in path.read_text().splitlines():
        if line.startswith("# args "):
            out += shlex.split(line[len("# args "):])
    return out


def render(cmd, roms, case, tmp):
    out = Path(tmp) / f"{case.stem}.raw"
    args = shlex.split(cmd) + ["--roms", roms, "--midi", str(case), "--samples",
                               str(samples_of(case)), "--out", str(out)] + extra_args(case)
    subprocess.run(args, check=True)
    data = out.read_bytes()
    out.unlink()
    return {"samples": samples_of(case), "sha256": hashlib.sha256(data).hexdigest()}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("mode", choices=["record", "check"])
    ap.add_argument("--renderer", default=str(CORE / "build" / "ref" / "mt32_ref"))
    ap.add_argument("--case", nargs="*")
    ap.add_argument("--jobs", type=int, default=4)
    a = ap.parse_args()
    roms = os.environ.get("MT32_ROM_DIR")
    if not roms:
        print("SKIP: corpus.py: MT32_ROM_DIR not set")
        sys.exit(77)
    cases = sorted(MIDI.glob("*.txt"))
    if a.case:
        cases = [c for c in cases if c.stem in a.case]
    if not cases:
        sys.exit("corpus.py: no cases to render")
    with tempfile.TemporaryDirectory() as tmp, ThreadPoolExecutor(a.jobs) as ex:
        results = dict(zip([c.stem for c in cases], ex.map(lambda c: render(a.renderer, roms, c, tmp), cases)))
    if a.mode == "record":
        db = json.loads(DB.read_text()) if DB.exists() else {}
        db.update(results)
        DB.write_text(json.dumps(dict(sorted(db.items())), indent=2) + "\n")
        for k, v in results.items():
            print(f"{k:12} {v['samples']:8} {v['sha256'][:16]}")
        return
    db = json.loads(DB.read_text())
    bad = 0
    for k, v in results.items():
        ok = db.get(k) == v
        bad += not ok
        print(f"{k:12} {'identical' if ok else 'DIFFERENT'} {v['sha256'][:16]}")
    print(f"corpus: {len(results) - bad} identical, {bad} different")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
