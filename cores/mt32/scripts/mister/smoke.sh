#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-2.0-or-later
# MT-32 MiSTer smoke test: scripts/mister/smoke.sh RBF PACK OUTDIR
# Installs the RBF (synth-mister, with its lock and backup), loads the ROM pack, checks:
# boot LCD lit; MIDI over the UART (master volume 60 by SysEx, then notes on part 1, which make
# the firmware redraw: "vol: 60", as Nuked-MT32 shows for the same bytes) changes the LCD;
# no audio underrun; status band "running". The Part 1 key (Q) changes the LCD
# (Nuked-MT32 with mt32_ref --button shows "1|Slap Bass1|vol> 80" for the same input).
# The core's UART must be in MIDI mode (config/uartmode.MT-32, set once from the OSD).
set -euo pipefail
cd "$(dirname "$0")/../.."
RBF=${1:?usage: smoke.sh RBF PACK OUTDIR}; PACK=${2:?}; OUT=${3:?}
M=../../modules/synthframe/bin/synth-mister
mkdir -p "$OUT"
# master volume 60 (DT1 10 00 16), then 20 notes on channel 2 (part 1)
python3 -c '
print("0.0 f041101612100016" + "3c1ef7")
for i in range(20):
    print(f"{0.5 + i * 0.1:.2f} 913c64")
    print(f"{0.55 + i * 0.1:.2f} 813c00")' > "$OUT/midi.txt"
SECS=4

$M lock mt32-smoke
trap '$M unlock' EXIT
$M install mt32 "$RBF"
$M sh "mkdir -p /media/fat/games/MT-32"
scp -q "$PACK" "$MISTER:/media/fat/games/MT-32/mt32_smoke.rom"
printf '<mistergamedescription>\n  <rbf>_Devices/MT32</rbf>\n  <file delay="1" type="f" index="1" path="mt32_smoke.rom"/>\n</mistergamedescription>\n' |
    $M sh "cat > /tmp/mt32_smoke.mgl"
scp -q ../synclavier/scripts/mister/mister_keys.py "$MISTER:/tmp/"
keys() { $M sh "python3 /tmp/mister_keys.py $*"; }
$M midi-stop
$M load /tmp/mt32_smoke.mgl; sleep "${BOOT:-10}"
$M shot mt32 "$OUT/boot.png" >/dev/null
$M sh "stty -F /dev/ttyS1 9600 raw -echo"
scp -q ../sc55/scripts/mister/play_tty.py "$MISTER:/tmp/"
scp -q "$OUT/midi.txt" "$MISTER:/tmp/mt32_midi.txt"
$M sh "python3 /tmp/play_tty.py /tmp/mt32_midi.txt $SECS"
sleep 1
$M shot mt32 "$OUT/midi.png" >/dev/null
keys down:q sleep:1
$M shot mt32 "$OUT/key.png" >/dev/null
keys up:q sleep:1
python3 -I - "$OUT" <<'PY' | tee "$OUT/result.txt"
import sys
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
def dots(im):   # LCD dots (0x10300c) inside the LCD background area
    return sum(1 for c in im.getdata() if c[0] < 0x30 and 0x20 <= c[1] < 0x50 and c[2] < 0x30)
ims = {n: Image.open(f"{out}/{n}.png").convert("RGB") for n in ("boot", "midi", "key")}
s = strip(ims["midi"])
if not s:
    print("band: not found"); sys.exit(1)
k = ims["midi"].size[0] / 800
und = ims["midi"].getpixel((s[0] + int(300 * k), s[1] + int(10 * k)))[:3]
und_s = {(0x30, 0x38, 0x48): "never", (0x90, 0x20, 0x20): "seen", (0xff, 0x40, 0x40): "now"}.get(und, "?%02x%02x%02x" % und)
lcd = lambda im: im.crop((0, int(240 * k), im.size[0], int(330 * k))).tobytes()
print(f"band: {s[2]}")
print(f"underrun: {und_s}")
print(f"lcd_dots_boot: {dots(ims['boot'])}")
print(f"midi_changed_lcd: {lcd(ims['midi']) != lcd(ims['boot'])}")
print(f"key_changed_lcd: {lcd(ims['key']) != lcd(ims['midi'])}")
ok = s[2] == "running" and und_s == "never" and dots(ims["boot"]) > 200 and lcd(ims["midi"]) != lcd(ims["boot"]) \
     and lcd(ims["key"]) != lcd(ims["midi"])
sys.exit(0 if ok else 1)
PY
