#!/usr/bin/env bash
# Hardware smoke test of an SC-55 RBF on a MiSTer (SESSION-PLAN iteration 1).
#
#   MISTER=root@<address> scripts/mister/smoke.sh RBF OUTDIR [SECONDS]
#
# Installs RBF (synth-mister install; run 'synth-mister backup' once first), loads it with the
# ROM pack already on the card (games/SC-55/boot.rom), switches the MIDI input to 9600 baud
# (zero bytes, then a GM reset), screenshots the boot screen, plays avmidi 0A.mid repeated for
# SECONDS (default 100) and screenshots again. Writes OUTDIR/{boot,end}.png and
# OUTDIR/result.txt (lcd_sha: SHA-256 of the LCD area at boot; band, underrun, lag: the status
# strip at the end). Exit 1 if the core is not running or an underrun or lag box is set.
set -euo pipefail
cd "$(dirname "$0")/../.."
RBF=${1:?usage: smoke.sh RBF OUTDIR [SECONDS]}; OUT=${2:?usage: smoke.sh RBF OUTDIR [SECONDS]}
SECS=${3:-100}
M=../../modules/synthframe/bin/synth-mister
mkdir -p "$OUT"
python3 scripts/mister/smf2events.py ../../modules/nuked-sc55/test/integration/avmidi/0A.mid "$OUT/0A.txt" >/dev/null

$M lock sc55-smoke
trap '$M unlock' EXIT
$M install sc55 "$RBF"
$M midi-stop
F=$($M sh "ls /media/fat/_Devices/SC55_2*.rbf | tail -1")
$M load "$F"; sleep "${WAIT:-8}"
$M sh "stty -F /dev/ttyS1 9600 raw -echo"
# 2 s after the GM reset: with 1 s, one boot screenshot in 6 came before the level bars were drawn
printf '0000000000000000f07e7f0901f7' | xxd -r -p | $M sh "cat > /dev/ttyS1; sleep 2"
$M shot sc55 "$OUT/boot.png" >/dev/null
scp -q scripts/mister/play_tty.py "$OUT/0A.txt" "$MISTER:/tmp/"
$M sh "python3 /tmp/play_tty.py /tmp/0A.txt $SECS"
$M shot sc55 "$OUT/end.png" >/dev/null
python3 -I - "$OUT" <<'EOF' | tee "$OUT/result.txt"
# Status strip (hdl/board/lcd_video.sv, 800 x 600 frame): 20-pixel-high boxes at x 0-199 (state
# band), 220-259 (MIDI), 280-319 (underrun), 340-379 (lag) from the strip's left edge, 300 lines
# below the top of the 741 x 268 LCD area. Screenshots come scaled (640 x 480 seen), so every
# offset is scaled by width / 800. The strip is found by the state band's colour.
import hashlib, sys
from PIL import Image
out = sys.argv[1]
COL = {(0x20, 0xc0, 0x40): "running", (0xe0, 0xa0, 0x20): "loading", (0x20, 0x60, 0xc0): "rom ok",
       (0xc0, 0x20, 0x20): "no rom"}
def strip(im):
    px = im.load(); w, h = im.size
    for y in range(h):
        for x in range(w):
            if px[x, y][:3] in COL:
                return x, y, COL[px[x, y][:3]]
    return None
def box(im, x0, y0, dx):
    k = im.size[0] / 800
    return im.getpixel((x0 + int((dx + 20) * k), y0 + int(10 * k)))[:3]
end = Image.open(f"{out}/end.png").convert("RGB")
boot = Image.open(f"{out}/boot.png").convert("RGB")
s = strip(end)
ok = True
if not s:
    print("band: not found"); sys.exit(1)
x0, y0, band = s
und, lag = box(end, x0, y0, 280), box(end, x0, y0, 340)
und_s = {(0x30, 0x38, 0x48): "never", (0x90, 0x20, 0x20): "seen", (0xff, 0x40, 0x40): "now"}.get(und, "?%02x%02x%02x" % und)
lag_s = {(0x30, 0x38, 0x48): "never", (0xe0, 0xa0, 0x20): "seen"}.get(lag, "?%02x%02x%02x" % lag)
sb = strip(boot)
k = boot.size[0] / 800
lcd = boot.crop((sb[0], sb[1] - int(300 * k), sb[0] + int(741 * k), sb[1] - int(32 * k))) if sb else None
print(f"band: {band}")
print(f"underrun: {und_s}")
print(f"lag: {lag_s}")
print(f"strip_at: {x0},{y0}")
print("lcd_sha: " + (hashlib.sha256(lcd.tobytes()).hexdigest() if lcd else "none"))
sys.exit(0 if band == "running" and und_s == "never" and lag_s == "never" else 1)
EOF
