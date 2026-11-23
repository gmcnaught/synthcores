#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Hardware smoke test of a CZ-101 RBF on a MiSTer (cores/cz101/docs/plan.md, C5).
#
#   MISTER=root@<address> scripts/mister/smoke.sh RBF PACK OUTDIR
#
# Installs RBF (synth-mister install; run 'synth-mister backup' once first) and PACK
# (tools/mkpack.py) as games/CZ-101/cz101_smoke.rom, removes its save file, loads the core and
# the pack through an MGL (as the OSD's "Load ROM pack": saves/CZ-101/cz101_smoke.sav is
# mounted), waits BOOT seconds (default 8), screenshots, plays tests/midi/basic.txt once over
# the UART at 9600 baud (synthframe_midi_rx also takes 9600), screenshots again (its last
# program change selects preset 15). Then the panel and the save file: key "4" (Internal),
# screenshot; the OSD opens (autosave) and closes; the save file must be 8 KB and its battery RAM
# must differ from the pack's init_ram; the core and pack load again and the LCD is compared
# with the one before the reload (information only: the firmware boots to preset 1).
# Writes OUTDIR/{boot,end,nv_changed,nv_reloaded}.png, cz101_smoke.sav and result.txt. Exit 1
# if the core is not running (band not green), an underrun was seen, the LCD is blank at boot, MIDI did not
# change the LCD, or the save file is wrong.
set -euo pipefail
cd "$(dirname "$0")/../.."
RBF=${1:?usage: smoke.sh RBF PACK OUTDIR}; PACK=${2:?}; OUT=${3:?}
M=../../modules/synthframe/bin/synth-mister
mkdir -p "$OUT"
python3 ../proteus/tools/mkmidi.py tests/midi/basic.txt "$OUT/basic.mid" >/dev/null
python3 ../sc55/scripts/mister/smf2events.py "$OUT/basic.mid" "$OUT/basic.txt" >/dev/null
SECS=$(awk 'END { print int($1) + 2 }' "$OUT/basic.txt")

$M lock cz101-smoke
trap '$M unlock' EXIT
$M install cz101 "$RBF"
$M sh "mkdir -p /media/fat/games/CZ-101; rm -f /media/fat/games/CZ-101/boot.rom /media/fat/saves/CZ-101/cz101_smoke.sav"
scp -q "$PACK" "$MISTER:/media/fat/games/CZ-101/cz101_smoke.rom"
printf '<mistergamedescription>\n  <rbf>_Devices/CZ101</rbf>\n  <file delay="1" type="f" index="1" path="cz101_smoke.rom"/>\n</mistergamedescription>\n' |
    $M sh "cat > /tmp/cz101_smoke.mgl"
scp -q ../synclavier/scripts/mister/mister_keys.py "$MISTER:/tmp/"
keys() { $M sh "python3 /tmp/mister_keys.py $*"; }
$M midi-stop
$M load /tmp/cz101_smoke.mgl; sleep "${BOOT:-8}"
$M shot cz101 "$OUT/boot.png" >/dev/null
$M sh "stty -F /dev/ttyS1 9600 raw -echo"
# a few zero bytes (framing errors at 31250) switch the input to 9600 (synthframe_midi_rx)
printf '0000000000000000' | xxd -r -p | $M sh "cat > /dev/ttyS1; sleep 1"
scp -q ../sc55/scripts/mister/play_tty.py "$OUT/basic.txt" "$MISTER:/tmp/"
$M sh "python3 /tmp/play_tty.py /tmp/basic.txt $SECS"
$M shot cz101 "$OUT/end.png" >/dev/null

# panel key and the save file
keys text:4 sleep:1
$M shot cz101 "$OUT/nv_changed.png" >/dev/null
keys F12 sleep:3 ESC sleep:1
scp -q "$MISTER:/media/fat/saves/CZ-101/cz101_smoke.sav" "$OUT/" 2>/dev/null || true
$M load /tmp/cz101_smoke.mgl; sleep "${BOOT:-8}"
$M shot cz101 "$OUT/nv_reloaded.png" >/dev/null
python3 -I - "$OUT" "$PACK" <<'EOF' | tee "$OUT/result.txt"
# cz101_lcd_video.sv, 800 x 600 frame: the LCD (MAME's 97 x 19 pixels at 8x) at (12, 160),
# 776 x 152, background 8a9294, dot on 3f3b3e; synthframe_status_strip at (12, 412): band at
# x 0-199 (green running, amber loading, blue ROM pack not running, red no ROM), boxes at
# 220-259 (MIDI), 280-319 (underrun: dark red seen, bright red now), 340-379 (lag). Screenshots
# come scaled; offsets are scaled by width / 800; the strip is found by the band's colour.
import os, sys
from PIL import Image
out, pack = sys.argv[1], sys.argv[2]
BAND = {(0x20, 0xc0, 0x40): "running", (0xe0, 0xa0, 0x20): "loading", (0x20, 0x60, 0xc0): "rom ok",
        (0xc0, 0x20, 0x20): "no rom"}
def strip(im):
    px = im.load(); w, h = im.size
    for y in range(h):
        for x in range(w):
            if px[x, y][:3] in BAND:
                return x, y, BAND[px[x, y][:3]]
    return None
def panel(name, s):
    im = Image.open(f"{out}/{name}.png").convert("RGB")
    k = im.size[0] / 800
    x0, y0 = s[0], s[1] - int(252 * k)
    return im.crop((x0, y0, x0 + int(776 * k), y0 + int(152 * k)))
def lit(p):
    return sum(1 for c in p.getdata() if abs(c[0] - 0x3f) < 16 and abs(c[1] - 0x3b) < 16 and abs(c[2] - 0x3e) < 16)
end = Image.open(f"{out}/end.png").convert("RGB")
s = strip(end)
if not s:
    print("band: not found"); sys.exit(1)
k = end.size[0] / 800
und = end.getpixel((s[0] + int(300 * k), s[1] + int(10 * k)))[:3]
und_s = {(0x30, 0x38, 0x48): "never", (0x90, 0x20, 0x20): "seen", (0xff, 0x40, 0x40): "now"}.get(und, "?%02x%02x%02x" % und)
boot, endp = panel("boot", s), panel("end", s)
changed, reloaded = panel("nv_changed", s), panel("nv_reloaded", s)
sav = f"{out}/cz101_smoke.sav"
sz = os.path.getsize(sav) if os.path.exists(sav) else 0
ram = open(sav, "rb").read()[:4096] if sz else b""
init = open(pack, "rb").read()[0x8000:0x9000]
print(f"band: {s[2]}")
print(f"underrun: {und_s}")
print(f"lcd_lit_pixels_boot: {lit(boot)}")
print(f"midi_changed_lcd: {endp.tobytes() != boot.tobytes()}")
print(f"key_changed_lcd: {changed.tobytes() != endp.tobytes()}")
print(f"save_file_bytes: {sz}")
print(f"save_ram_differs_from_init_ram: {bool(ram) and ram != init}")
print(f"lcd_after_reload_same_as_before: {reloaded.tobytes() == changed.tobytes()}")
ok = (s[2] == "running" and und_s == "never" and lit(boot) > 500 and endp.tobytes() != boot.tobytes()
      and sz == 8192 and ram != init)
sys.exit(0 if ok else 1)
EOF
