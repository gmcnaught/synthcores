#!/usr/bin/env bash
# Hardware smoke test of a Synclavier RBF on a MiSTer (SESSION-PLAN iteration 1).
#
#   MISTER=root@<address> scripts/mister/smoke.sh RBF OUTDIR
#
# Installs RBF as _Computer/Synclavier_<date>.rbf (synth-mister install; run 'synth-mister
# backup' once first), loads /media/fat/Synclavier-5.3.mgl (W0-53.IMG + W0-53.SST, already on
# the card), waits WAIT seconds (default 20), screenshots and reads the screen text with
# tools/screen_text.py. Writes OUTDIR/menu.png, OUTDIR/screen.txt and OUTDIR/result.txt.
# Exit 1 unless the screen shows SYN-5.3's Main Menu (EXPECT, default "MAIN MENU").
set -euo pipefail
cd "$(dirname "$0")/../.."
RBF=${1:?usage: smoke.sh RBF OUTDIR}; OUT=${2:?usage: smoke.sh RBF OUTDIR}
M=../../modules/synthframe/bin/synth-mister
mkdir -p "$OUT"

$M lock synclavier-smoke
trap '$M unlock' EXIT
$M install synclavier "$RBF"
$M midi-stop
$M load /media/fat/Synclavier-5.3.mgl; sleep "${WAIT:-20}"
$M shot synclavier "$OUT/menu.png" >/dev/null
python3 tools/screen_text.py "$OUT/menu.png" > "$OUT/screen.txt"
# The status line under the terminal (Lag, Underrun, Overrun, MIDI counters) changes from run
# to run; the hash covers the terminal text above the "----" rule only.
{
    echo "menu_sha: $(sed '/^-----/,$d' "$OUT/screen.txt" | shasum -a 256 | cut -d' ' -f1)"
    if grep -qi "${EXPECT:-MAIN MENU}" "$OUT/screen.txt"; then echo "menu: yes"; else echo "menu: no"; fi
    echo "underrun: $(grep -oE 'Underrun [0-9]+' "$OUT/screen.txt" | head -1 | cut -d' ' -f2)"
} | tee "$OUT/result.txt"
grep -q "menu: yes" "$OUT/result.txt" && grep -q "underrun: 0000" "$OUT/result.txt"
