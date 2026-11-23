#!/usr/bin/env bash
# Hardware smoke test of a Proteus/1 XR RBF on a MiSTer (SESSION-PLAN iteration 1).
#
#   MISTER=root@<address> tools/mister_smoke.sh RBF OUTDIR [SECONDS]
#
# Installs RBF (synth-mister install; run 'synth-mister backup' once first), loads it through
# /media/fat/Proteus.mgl with the system image already on the card, waits BOOT seconds (default
# 15), screenshots, runs tools/mister_soak.py for SECONDS (default 60, seed 1: dense random MIDI
# at 31250 baud, more than 32 voices) and screenshots again. Writes OUTDIR/{boot,end}.png,
# OUTDIR/soak.txt and OUTDIR/result.txt. Exit 1 if a sample deadline was missed during the soak
# or fewer than 95% of the expected samples were produced. boot_sha is for information only:
# it changes with the LCD cursor's blink phase and with the program the last soak left selected.
set -euo pipefail
cd "$(dirname "$0")/.."
RBF=${1:?usage: mister_smoke.sh RBF OUTDIR [SECONDS]}; OUT=${2:?usage: mister_smoke.sh RBF OUTDIR [SECONDS]}
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
print(f"missed: {m1 - m0}")
print(f"produced_ratio: {ratio:.3f}")
print(f"worst_clocks: {worst}")
print("boot_sha: " + hashlib.sha256(boot.tobytes()).hexdigest())
sys.exit(0 if m1 == m0 and ratio > 0.95 else 1)
EOF
