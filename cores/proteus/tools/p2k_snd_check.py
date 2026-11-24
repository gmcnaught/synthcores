#!/usr/bin/env python3
"""Corpus gate (iterations 14 and 16): the P2K sound HDL (p2k/rtl/snd, Verilator bench p2k/sim/p2k_snd_tb
on a DDR3 model) against the integer model on the reference corpus (tests/p2k_corpus).

For each case and mode the bench replays build/p2k/corpus/<case>.0/snd.log and must produce exactly
what p2k/model/p2k_render produces with the same options:
  voices  --nofilter --nofx   (14a: G20 voices, level interpolators, mixer)
  filter  --nofx              (14b: + H filter)
  full                        (14c: + G05 effects; also equal to MAME's snd.raw, the manifest hash)

  P2K_SIMMDIR=... tools/p2k_snd_check.py [--modes voices,filter,full] [--cases a,b] [--jobs N] [--tb-args "..."]
Results go to build/p2k/snd_check/<case>.<mode>/ (hdl.raw, ref.raw, bench output) and a summary line
per case and mode. Exit 1 on any difference.
"""
import argparse
import hashlib
import json
import os
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor, as_completed

CORE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CORPUS = os.path.join(CORE, "build", "p2k", "corpus")
OUT = os.path.join(CORE, "build", "p2k", "snd_check")
MANIFEST = os.path.join(CORE, "tests", "p2k_corpus", "manifest.json")
MODES = {"voices": ["--nofilter", "--nofx"], "filter": ["--nofx"], "full": []}


def sha(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for b in iter(lambda: f.read(1 << 20), b""):
            h.update(b)
    return h.hexdigest()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--modes", default="full,filter,voices")
    ap.add_argument("--cases")
    ap.add_argument("--jobs", type=int, default=7)
    ap.add_argument("--simm", default=os.environ.get("P2K_SIMMDIR"))
    ap.add_argument("--tb", default=os.path.join(CORE, "build", "p2k", "snd", "p2k_snd_tb"))
    ap.add_argument("--render", default=os.path.join(CORE, "build", "p2k", "model", "p2k_render"))
    ap.add_argument("--tb-args", default="", help="extra bench options (DDR3 timing: --ddr-busy N --ddr-lat MIN MAX)")
    a = ap.parse_args()
    if not a.simm:
        raise SystemExit("set P2K_SIMMDIR (or --simm)")
    man = json.load(open(MANIFEST))["cases"]
    cases = a.cases.split(",") if a.cases else list(man)
    modes = a.modes.split(",")
    big = os.path.join(a.simm, "big.bin")

    # longest cases first so the pool finishes together
    def frames(c):
        return os.path.getsize(os.path.join(CORPUS, f"{c}.0", "snd.raw")) // 4
    jobs = sorted(((c, m) for c in cases for m in modes), key=lambda cm: -frames(cm[0]))

    def one(c, m):
        d = os.path.join(OUT, f"{c}.{m}")
        os.makedirs(d, exist_ok=True)
        log = os.path.join(CORPUS, f"{c}.0", "snd.log")
        n = frames(c)
        opts = MODES[m] + ["--simm", "1=" + big, "--frames", str(n)]
        t0 = time.time()
        tb = subprocess.run([a.tb] + opts + a.tb_args.split() + [log, os.path.join(d, "hdl.raw")], check=True,
                            stdout=subprocess.PIPE, text=True).stdout.strip()
        t1 = time.time()
        if m == "full":
            ref_sha = sha(os.path.join(CORPUS, f"{c}.0", "snd.raw"))
            ok_man = ref_sha == man[c]["sndout_sha256"]
        else:
            subprocess.run([a.render] + opts + [log, os.path.join(d, "ref.raw")], check=True,
                           stdout=subprocess.PIPE, text=True)
            ref_sha = sha(os.path.join(d, "ref.raw"))
            ok_man = True
        hdl_sha = sha(os.path.join(d, "hdl.raw"))
        with open(os.path.join(d, "bench.txt"), "w") as f:
            f.write(tb + "\n")
        same = hdl_sha == ref_sha and ok_man
        return c, m, same, hdl_sha, n, t1 - t0, tb

    bad = 0
    with ThreadPoolExecutor(a.jobs) as ex:
        futs = [ex.submit(one, c, m) for c, m in jobs]
        for fu in as_completed(futs):
            c, m, same, h, n, dt, tb = fu.result()
            bad += not same
            print(f"{'same' if same else 'DIFFERENT':9} {c:8} {m:6} frames {n:9} hdl {h[:12]} {dt:7.0f}s | {tb}", flush=True)
    print(f"{len(jobs) - bad}/{len(jobs)} identical")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
