#!/usr/bin/env bash
# Hardware smoke test of a Proteus/1 XR RBF on a MiSTer (SESSION-PLAN iteration 1).
#
#   MISTER=root@<address> scripts/mister/smoke.sh RBF OUTDIR [SECONDS]
#
# Installs RBF (synth-mister install; run 'synth-mister backup' once first), loads it through
# /media/fat/Proteus.mgl with the system image already on the card, waits BOOT seconds (default
# 15), screenshots, runs tools/mister_soak.py for SECONDS (default 60, seed 1: dense random MIDI
# at 31250 baud, more than 32 voices) and screenshots again. Writes OUTDIR/{boot,end}.png,
# OUTDIR/soak.txt and OUTDIR/result.txt. Exit 1 if a sample deadline was missed during the soak
# or fewer than 95% of the expected samples were produced, or the running bar is not green.
# Screen (mister/rtl/display.sv, 320 x 240, cfg/panel.yaml): LCD at x 12-304, y 16-74, dots
# on d2ffc8; voice bars y 162-225; running bar y 232-239, 00a000 running / a00000 not.
# Screenshots come scaled; offsets are scaled by height / 240. boot_sha is for information only:
# it changes with the LCD cursor's blink phase and with the program the last soak left selected.
set -euo pipefail
cd "$(dirname "$0")/../.."
RBF=${1:?usage: smoke.sh RBF OUTDIR [SECONDS]}; OUT=${2:?usage: smoke.sh RBF OUTDIR [SECONDS]}
SECS=${3:-60}
M=../../modules/synthframe/bin/synth-mister
mkdir -p "$OUT"

$M lock proteus-smoke
trap '$M unlock' EXIT
$M install proteus "$RBF"
$M midi-stop
$M load /media/fat/Proteus.mgl; sleep "${BOOT:-15}"
$M shot proteus "$OUT/boot.png" >/dev/null
scp -q tools/mister_soak.py tools/mister_midi.py "$MISTER:/tmp/"
$M midi-stop
$M sh "python3 /tmp/mister_soak.py $SECS 1" | tee "$OUT/soak.txt"
$M shot proteus "$OUT/end.png" >/dev/null
python3 -I - "$OUT" <<'EOF' | tee "$OUT/result.txt"
import hashlib, re, sys
from PIL import Image
out = sys.argv[1]
t = open(f"{out}/soak.txt").read()
el = float(re.search(r"elapsed ([\d.]+) s", t).group(1))
prod = int(re.search(r"samples produced (\d+)", t).group(1))
m0, m1 = map(int, re.search(r"missed deadlines: before (\d+), after (\d+)", t).groups())
worst = int(re.search(r"worst clocks per sample: (\d+)", t).group(1))
ratio = prod / (el * 39062.5)
boot = Image.open(f"{out}/boot.png").convert("RGB")
def near(c, ref, tol=40):
    return all(abs(a - b) < tol for a, b in zip(c, ref))
def screen(im):
    # the running bar spans the frame's full width: it gives the frame's position and scale
    w, h = im.size
    def kind(y):
        row = [im.getpixel((x, y)) for x in range(w)]
        for name, ref in (("running", (0, 160, 0)), ("stopped", (160, 0, 0))):
            xs = [x for x, c in enumerate(row) if near(c, ref)]
            if len(xs) > w // 3:
                return name, xs[0], xs[-1]
        return None
    ys = [y for y in range(h - 1, h // 2, -1) if kind(y)]
    if not ys:
        return "missing", 0
    bot = ys[0]
    top = bot
    while top - 1 in ys:
        top -= 1
    name, x0, x1 = kind(bot)
    kx, ky = (x1 - x0 + 1) / 320, (bot - top + 1) / 8
    y0 = bot + 1 - 240 * ky
    lcd = im.crop((int(x0 + 12 * kx), int(y0 + 16 * ky), int(x0 + 305 * kx), int(y0 + 75 * ky)))
    return name, sum(1 for c in lcd.getdata() if near(c, (210, 255, 200)))
bar, dots = screen(boot)
end_bar, end_dots = screen(Image.open(f"{out}/end.png").convert("RGB"))
print(f"missed: {m1 - m0}")
print(f"produced_ratio: {ratio:.3f}")
print(f"worst_clocks: {worst}")
print(f"running_bar: boot {bar}, end {end_bar}")
print(f"lcd_dot_pixels: boot {dots}, end {end_dots}")
print("boot_sha: " + hashlib.sha256(boot.tobytes()).hexdigest())
sys.exit(0 if m1 == m0 and ratio > 0.95 and bar == end_bar == "running" else 1)
EOF
