#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Hardware smoke test of an MU50 RBF on a MiSTer (cores/mu50/docs/plan.md, U5).
#
#   MISTER=root@<address> scripts/mister/smoke.sh RBF PACK OUTDIR
#
# Installs RBF (synth-mister install; run 'synth-mister backup' once first) and PACK
# (tools/mkpack.py) as games/MU50/mu50_smoke.rom, removes its save file, loads the core and the
# pack through an MGL (as the OSD's "Load ROM pack": saves/MU50/mu50_smoke.sav is mounted), waits
# BOOT seconds (default 20; the pack load takes about 10), screenshots (boot), plays tests/midi/basic.txt over the UART at
# 9600 baud (synthframe_midi_rx also takes 9600), screenshots (end); presses U (Util) and
# screenshots (key); MIDI out: tty_midi_xfer.py at 31,250 baud sends XG System On twice and
# the bulk dump requests of tests/midi/dump.txt and reads the answer, which must be MAME's
# (tests/midi/dump.out); opens and closes the OSD (autosave) and copies the save file.
# Writes OUTDIR/{boot,end,key}.png, midi_out.txt, save.sav and result.txt. Exit 1 if the core
# is not running (band not green), an underrun was seen, the LCD is blank at boot, MIDI or the
# key did not change the LCD, MIDI out differs from MAME's, or the save file is not 64 KB.
set -euo pipefail
cd "$(dirname "$0")/../.."
RBF=${1:?usage: smoke.sh RBF PACK OUTDIR}; PACK=${2:?}; OUT=${3:?}
M=../../modules/synthframe/bin/synth-mister
mkdir -p "$OUT"
python3 ../../modules/synthframe/tools/midi/mkmidi.py tests/midi/basic.txt "$OUT/basic.mid" >/dev/null
python3 ../../modules/synthframe/tools/midi/smf2events.py "$OUT/basic.mid" "$OUT/basic.txt" >/dev/null
SECS=$(awk 'END { print int($1) + 2 }' "$OUT/basic.txt")

$M lock mu50-smoke
trap '$M unlock' EXIT
$M install mu50 "$RBF"
$M sh "mkdir -p /media/fat/games/MU50; rm -f /media/fat/games/MU50/boot.rom /media/fat/saves/MU50/mu50_smoke.sav"
scp -q "$PACK" "$MISTER:/media/fat/games/MU50/mu50_smoke.rom"
printf '<mistergamedescription>\n  <rbf>_Devices/MU50</rbf>\n  <file delay="1" type="f" index="1" path="mu50_smoke.rom"/>\n</mistergamedescription>\n' |
    $M sh "cat > /tmp/mu50_smoke.mgl"
scp -q ../../modules/synthframe/tools/mister/mister_keys.py "$MISTER:/tmp/"
keys() { $M sh "python3 /tmp/mister_keys.py $*"; }
$M midi-stop
$M load /tmp/mu50_smoke.mgl; sleep "${BOOT:-20}"
$M shot mu50 "$OUT/boot.png" >/dev/null
$M sh "stty -F /dev/ttyS1 9600 raw -echo"
# a few zero bytes (framing errors at 31250) switch the input to 9600 (synthframe_midi_rx)
printf '0000000000000000' | xxd -r -p | $M sh "cat > /dev/ttyS1; sleep 1"
scp -q ../../modules/synthframe/tools/mister/play_tty.py "$OUT/basic.txt" "$MISTER:/tmp/"
$M sh "python3 /tmp/play_tty.py /tmp/basic.txt $SECS"
$M shot mu50 "$OUT/end.png" >/dev/null
keys text:u sleep:2
$M shot mu50 "$OUT/key.png" >/dev/null
scp -q ../../modules/synthframe/tools/mister/tty_midi_xfer.py "$MISTER:/tmp/"
$M sh "python3 /tmp/tty_midi_xfer.py --baud 31250 6 F043104C00007E00F7 F043104C00007E00F7 F043204C000000F7 F043204C080000F7" > "$OUT/midi_out.txt"
keys F12 sleep:3 ESC sleep:1
rm -f "$OUT/save.sav"
scp -q "$MISTER:/media/fat/saves/MU50/mu50_smoke.sav" "$OUT/save.sav" 2>/dev/null || true
grep -v '^#' tests/midi/dump.out > "$OUT/midi_out_expected.txt"
python3 -I - "$OUT" <<'EOF' | tee "$OUT/result.txt"
# mu50_lcd_video.sv, 800 x 600 frame: the LCD panel at (80, 48), 640 x 220, dot on 102018;
# synthframe_status_strip at (16, 560): band at x 0-199 (green running, amber loading, blue ROM
# pack not running, red no ROM), boxes at 220-259 (MIDI), 280-319 (underrun: dark red seen,
# bright red now). Screenshots come scaled; offsets are scaled by width / 800; the strip is
# found by the band's colour.
import os, sys
from PIL import Image
out = sys.argv[1]
BAND = {(0x20, 0xc0, 0x40): "running", (0xe0, 0xa0, 0x20): "loading", (0x20, 0x60, 0xc0): "rom ok",
        (0xc0, 0x20, 0x20): "no rom"}
def strip(im):
    px = im.load(); w, h = im.size
    for y in range(h):
        for x in range(w):
            if px[x, y][:3] in BAND:
                return x, y, BAND[px[x, y][:3]]
    return None
end = Image.open(f"{out}/end.png").convert("RGB")
s = strip(end)
if not s:
    print("band: not found"); sys.exit(1)
k = end.size[0] / 800
def panel(name):
    im = Image.open(f"{out}/{name}.png").convert("RGB")
    x0, y0 = s[0] + int((80 - 16) * k), s[1] - int((560 - 48) * k)
    return im.crop((x0, y0, x0 + int(640 * k), y0 + int(220 * k)))
def lit(p):
    return sum(1 for c in p.getdata() if abs(c[0] - 0x10) < 16 and abs(c[1] - 0x20) < 16 and abs(c[2] - 0x18) < 16)
u = end.getpixel((s[0] + int(300 * k), s[1] + int(10 * k)))[:3]
und = {(0x30, 0x38, 0x48): "never", (0x90, 0x20, 0x20): "seen", (0xff, 0x40, 0x40): "now"}.get(u, "?%02x%02x%02x" % u)
boot, endp, key = panel("boot"), panel("end"), panel("key")
sav = os.path.getsize(f"{out}/save.sav") if os.path.exists(f"{out}/save.sav") else 0
print(f"band: {s[2]}")
print(f"underrun: {und}")
print(f"lcd_lit_pixels_boot: {lit(boot)}")
print(f"midi_changed_lcd: {endp.tobytes() != boot.tobytes()}")
print(f"key_changed_lcd: {key.tobytes() != endp.tobytes()}")
print(f"save_bytes: {sav}")
got = open(f"{out}/midi_out.txt").read().split()
want = open(f"{out}/midi_out_expected.txt").read().split()
mo = [b.upper() for b in got] == [b.upper() for b in want]
print(f"midi_out: {len(got)} bytes, {'MAME' if mo else 'differs from MAME'}")
ok = (s[2] == "running" and und == "never" and lit(boot) > 0 and endp.tobytes() != boot.tobytes()
      and key.tobytes() != endp.tobytes() and sav == 65536 and mo)
print("PASS" if ok else "FAIL")
sys.exit(0 if ok else 1)
EOF
