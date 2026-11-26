#!/usr/bin/env python3
"""The pass/fail step of scripts/mister/smoke.sh on synthetic inputs (no MiSTer needed).

Extracts the Python analysis that smoke.sh runs on OUTDIR/{soak.txt,boot.png,end.png} and feeds
it screenshots drawn here (320 x 240 frame scaled x3: running bar, LCD with N lit dots) and
soak reports. A booted core with audio must pass; a blank LCD (firmware not running), silence,
a reset during the soak, missed deadlines or an old status layout must fail
(review 2026-11-26 proteus1.md#1, #16).
Exit status: 0 passed, 1 failed, 77 skipped (Pillow missing).
"""
import os
import re
import subprocess
import sys
import tempfile

CORE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
try:
    from PIL import Image, ImageDraw
except ImportError:
    print("SKIP: Pillow not installed")
    sys.exit(77)

K = 3  # screenshot scale


def screen(path, lit_dots, bar=(0, 160, 0)):
    im = Image.new("RGB", (320 * K, 240 * K), (16, 24, 40))
    d = ImageDraw.Draw(im)
    d.rectangle([12 * K, 16 * K, 305 * K - 1, 75 * K - 1], fill=(40, 70, 40))
    for n in range(lit_dots):  # dots of 3 x 3 frame pixels in the LCD's dot area
        x, y = 16 + 3 * (n % 90), 20 + 3 * (n // 90)
        d.rectangle([x * K, y * K, (x + 3) * K - 1, (y + 3) * K - 1], fill=(210, 255, 200))
    d.rectangle([0, 232 * K, 320 * K - 1, 240 * K - 1], fill=bar)
    im.save(path)


def soak(path, produced=2343750, m0=0, m1=0, magic="5031", e0=1, e1=1, peak=5000):
    with open(path, "w") as f:
        f.write(f"elapsed 60.0 s, 480 MIDI bursts\n"
                f"samples produced {produced} (expected ~2343750, status updates every 4096)\n"
                f"missed deadlines: before {m0}, after {m1}; worst clocks per sample: 1300 of 2048\n"
                f"status layout {magic}; reset epoch: before {e0}, after {e1}\n"
                f"peak level during soak: {peak}\n")


def main():
    src = open(os.path.join(CORE, "scripts", "mister", "smoke.sh")).read()
    gate = re.search(r"<<'EOF' \| tee \"\$OUT/result.txt\"\n(.*?)\nEOF\n", src, re.S).group(1)
    cases = [
        ("booted, audio", {}, 120, 120, None, 0),
        ("LCD blank (CPU hung)", {}, 120, 0, None, 1),
        ("silent output", {"peak": 0}, 120, 120, None, 1),
        ("reset during the soak", {"produced": -1, "e1": 2}, 120, 120, None, 1),
        ("reset, counter wrapped", {"produced": 4294000000, "e1": 2}, 120, 120, None, 1),
        ("missed deadlines", {"m1": 3}, 120, 120, None, 1),
        ("old RBF status layout", {"magic": "0000"}, 120, 120, None, 1),
        ("running bar red", {}, 120, 120, (160, 0, 0), 1),
    ]
    fails = 0
    with tempfile.TemporaryDirectory() as tmp:
        for name, soak_args, boot_dots, end_dots, bar, want in cases:
            soak(os.path.join(tmp, "soak.txt"), **soak_args)
            screen(os.path.join(tmp, "boot.png"), boot_dots)
            screen(os.path.join(tmp, "end.png"), end_dots, bar or (0, 160, 0))
            r = subprocess.run([sys.executable, "-I", "-", tmp], input=gate, text=True,
                               stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            ok = (r.returncode != 0) == bool(want)
            fails += not ok
            last = r.stdout.strip().splitlines()[-1] if r.stdout.strip() else ""
            print(f"{'ok  ' if ok else 'FAIL'} {name}: exit {r.returncode} ({last})")
    print(f"test_smoke_gate: {len(cases) - fails} passed, {fails} failed")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
