#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-2.0-or-later
# Hardware smoke test of a JV-880 RBF on a MiSTer (cores/jv880/docs/plan.md, J5).
#
#   MISTER=root@<address> scripts/mister/smoke.sh RBF PACK OUTDIR [SECONDS]
#
# Installs RBF (synth-mister install; run 'synth-mister backup' once first) and PACK (romset.py
# pack --romset jv880-... [--nvram]) as games/JV-880/jv880_smoke.rom, removes its save file,
# loads the core and the pack through an MGL (as the OSD's "Load ROM pack": the save file
# saves/JV-880/jv880_smoke.sav is mounted), waits BOOT seconds (default 20: the download and the firmware's
# start-up screens), screenshots, plays jv880.mid (Nuked-SC55's integration test) repeated for
# SECONDS (default 60) at 9600 baud (the core's MIDI input takes 9600 as well as MidiLink's
# 31250), screenshots again. Then the NVRAM save: SYSTEM, five encoder steps to "System MIDI /
# Unit number", the cursor to the value, three steps up; the OSD opens (autosave) and closes;
# the save file must be 32 KB; the core and pack load again, and the same page must show the
# changed value. Writes OUTDIR/{boot,end,nv_before,nv_changed,nv_reloaded}.png and
# OUTDIR/result.txt: the status strip (band, underrun, lag; jv880_lcd_video.sv), the number of
# lit LCD dots at boot (the panel shows text), the save file's size and whether the value came
# back. Exit 1 if the core is not running, an underrun or lag box is set, the LCD is blank, or
# the NVRAM did not survive the reload.
set -euo pipefail
cd "$(dirname "$0")/../.."
RBF=${1:?usage: smoke.sh RBF PACK OUTDIR [SECONDS]}; PACK=${2:?}; OUT=${3:?}
SECS=${4:-60}
M=../../modules/synthframe/bin/synth-mister
mkdir -p "$OUT"
python3 ../sc55/scripts/mister/smf2events.py ../../modules/nuked-sc55/test/integration/jv880/jv880.mid "$OUT/jv880.txt" >/dev/null

$M lock jv880-smoke
trap '$M unlock' EXIT
$M install jv880 "$RBF"
# boot.rom would load too, without a save file; the smoke pack and its save file are its own
$M sh "mkdir -p /media/fat/games/JV-880; rm -f /media/fat/games/JV-880/boot.rom /media/fat/saves/JV-880/jv880_smoke.sav"
scp -q "$PACK" "$MISTER:/media/fat/games/JV-880/jv880_smoke.rom"
printf '<mistergamedescription>\n  <rbf>_Devices/JV880</rbf>\n  <file delay="1" type="f" index="1" path="jv880_smoke.rom"/>\n</mistergamedescription>\n' |
    $M sh "cat > /tmp/jv880_smoke.mgl"
scp -q ../synclavier/scripts/mister/mister_keys.py "$MISTER:/tmp/"
keys() { $M sh "python3 /tmp/mister_keys.py $*"; }
$M midi-stop
$M load /tmp/jv880_smoke.mgl; sleep "${BOOT:-20}"
$M shot jv880 "$OUT/boot.png" >/dev/null
$M sh "stty -F /dev/ttyS1 9600 raw -echo"
# a few zero bytes (framing errors at 31250) switch the input to 9600 (synthframe_midi_rx)
printf '0000000000000000' | xxd -r -p | $M sh "cat > /dev/ttyS1; sleep 1"
scp -q ../sc55/scripts/mister/play_tty.py "$OUT/jv880.txt" "$MISTER:/tmp/"
$M sh "python3 /tmp/play_tty.py /tmp/jv880.txt $SECS"
$M shot jv880 "$OUT/end.png" >/dev/null

# NVRAM: change System MIDI / Unit number, autosave when the OSD opens, reload, look again
keys text:e sleep:1 text:..... sleep:1 RIGHT sleep:1
$M shot jv880 "$OUT/nv_before.png" >/dev/null
keys text:... sleep:1
$M shot jv880 "$OUT/nv_changed.png" >/dev/null
keys text:e sleep:1 F12 sleep:3 ESC sleep:1
SAV=$($M sh "stat -c %s /media/fat/saves/JV-880/jv880_smoke.sav 2>/dev/null || echo 0")
$M load /tmp/jv880_smoke.mgl; sleep "${BOOT:-20}"
keys text:e sleep:1 text:..... sleep:1 RIGHT sleep:1
$M shot jv880 "$OUT/nv_reloaded.png" >/dev/null
keys text:e sleep:1
echo "$SAV" > "$OUT/sav_size"
python3 -I - "$OUT" <<'EOF' | tee "$OUT/result.txt"
# jv880_lcd_video.sv, 800 x 600 frame: the panel at (16, 196), 768 x 104, dot-on colour 0a1404;
# the status strip 144 lines below the panel's top: boxes at x 0-199 (state band), 220-259 (MIDI),
# 280-319 (underrun), 340-379 (lag) from the panel's left edge. Screenshots come scaled; every
# offset is scaled by width / 800. The strip is found by the state band's colour.
import sys
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
if not s:
    print("band: not found"); sys.exit(1)
x0, y0, band = s
und, lag = box(end, x0, y0, 280), box(end, x0, y0, 340)
und_s = {(0x30, 0x38, 0x48): "never", (0x90, 0x20, 0x20): "seen", (0xff, 0x40, 0x40): "now"}.get(und, "?%02x%02x%02x" % und)
lag_s = {(0x30, 0x38, 0x48): "never", (0xe0, 0xa0, 0x20): "seen"}.get(lag, "?%02x%02x%02x" % lag)
k = boot.size[0] / 800
bx, by = s[0], s[1] - int(144 * k)
lit = 0
for y in range(by, by + int(104 * k)):
    for x in range(bx, bx + int(768 * k)):
        r, g, b = boot.getpixel((x, y))
        lit += r < 0x40 and g < 0x40 and b < 0x40
print(f"band: {band}")
print(f"underrun: {und_s}")
print(f"lag: {lag_s}")
print(f"lcd_lit_pixels: {lit}")
# the NVRAM: the LCD panel's pixels before the change, after it, and after the reload
def panel(name):
    im = Image.open(f"{out}/{name}.png").convert("RGB")
    return im.crop((bx, by, bx + int(768 * k), by + int(104 * k))).tobytes()
before, changed, reloaded = panel("nv_before"), panel("nv_changed"), panel("nv_reloaded")
sav = int(open(f"{out}/sav_size").read().strip() or 0)
nv_ok = sav == 32768 and changed != before and reloaded == changed
print(f"save_file_bytes: {sav}")
print(f"nvram_after_reload: {'kept' if reloaded == changed else 'as before the change' if reloaded == before else 'other'}"
      f"{'' if changed != before else ' (the change did not show)'}")
sys.exit(0 if band == "running" and und_s == "never" and lag_s == "never" and lit > 200 and nv_ok else 1)
EOF
