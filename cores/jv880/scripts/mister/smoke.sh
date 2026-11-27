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
# changed value. Last, MIDI out (MIDI-OUT-PLAN.md): a fresh load without the save file, then the
# data request (RQ1) for the system area (00 00 00 00, 16 bytes); synthframe's
# tools/mister/tty_midi_xfer.py reads /dev/ttyS1 for 4 s (at 9600: synthframe_midi_tx follows the input's
# speed). The reply must be one DT1 from 00 00 00 00, 75 bytes, with a valid checksum (with the
# factory-preset NVRAM the reference sends REF_DT1 below). 2 s after the reply the panel must show
# the play screen again (as at boot), and SYSTEM must change it (ref/patch_ref.py FIXES, the SCI
# transmitter). Writes OUTDIR/{boot,end,nv_before,nv_changed,nv_reloaded,midi_out,rq1_key}.png and
# OUTDIR/result.txt: the status strip (band, underrun, lag; jv880_lcd_video.sv), the number of
# lit LCD dots at boot (the panel shows text), the save file's size and whether the value came
# back, and the MIDI out reply. Exit 1 if the core is not running, an underrun or lag box is set,
# the LCD is blank, the NVRAM did not survive the reload, the DT1 reply did not come out, or the
# panel did not come back after it.
set -euo pipefail
cd "$(dirname "$0")/../.."
RBF=${1:?usage: smoke.sh RBF PACK OUTDIR [SECONDS]}; PACK=${2:?}; OUT=${3:?}
SECS=${4:-60}
M=../../modules/synthframe/bin/synth-mister
mkdir -p "$OUT"
python3 ../../modules/synthframe/tools/midi/smf2events.py ../../modules/nuked-sc55/test/integration/jv880/jv880.mid "$OUT/jv880.txt" >/dev/null

$M lock jv880-smoke
trap '$M unlock' EXIT
$M install jv880 "$RBF"
# boot.rom would load too, without a save file; the smoke pack and its save file are its own
$M sh "mkdir -p /media/fat/games/JV-880; rm -f /media/fat/games/JV-880/boot.rom /media/fat/saves/JV-880/jv880_smoke.sav"
scp -q "$PACK" "$MISTER:/media/fat/games/JV-880/jv880_smoke.rom"
printf '<mistergamedescription>\n  <rbf>_Devices/JV880</rbf>\n  <file delay="1" type="f" index="1" path="jv880_smoke.rom"/>\n</mistergamedescription>\n' |
    $M sh "cat > /tmp/jv880_smoke.mgl"
scp -q ../../modules/synthframe/tools/mister/mister_keys.py "$MISTER:/tmp/"
keys() { $M sh "python3 /tmp/mister_keys.py $*"; }
$M midi-stop
$M load /tmp/jv880_smoke.mgl; sleep "${BOOT:-20}"
$M midi-off
$M shot jv880 "$OUT/boot.png" >/dev/null
$M sh "stty -F /dev/ttyS1 9600 raw -echo"
# a few zero bytes (framing errors at 31250) switch the input to 9600 (synthframe_midi_rx)
printf '0000000000000000' | xxd -r -p | $M sh "cat > /dev/ttyS1; sleep 1"
scp -q ../../modules/synthframe/tools/mister/play_tty.py "$OUT/jv880.txt" "$MISTER:/tmp/"
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
$M midi-off
keys text:e sleep:1 text:..... sleep:1 RIGHT sleep:1
$M shot jv880 "$OUT/nv_reloaded.png" >/dev/null
keys text:e sleep:1
echo "$SAV" > "$OUT/sav_size"

# MIDI out, last. Load again without the save file (the NVRAM step changed the Unit number, the
# SysEx device ID), switch the input to 9600 again, send the RQ1. The LCD shows "Now bulk sending"
# for about 1 s, then the play screen; then SYSTEM.
$M sh "rm -f /media/fat/saves/JV-880/jv880_smoke.sav"
$M load /tmp/jv880_smoke.mgl; sleep "${BOOT:-20}"
$M midi-off
printf '0000000000000000' | xxd -r -p | $M sh "cat > /dev/ttyS1; sleep 1"
scp -q ../../modules/synthframe/tools/mister/tty_midi_xfer.py "$MISTER:/tmp/"
$M sh "python3 /tmp/tty_midi_xfer.py 4 f041104611000000000000001070f7" > "$OUT/midi_out.txt"
sleep 2
$M shot jv880 "$OUT/midi_out.png" >/dev/null
keys text:e sleep:1
$M shot jv880 "$OUT/rq1_key.png" >/dev/null
keys text:e sleep:1
python3 -I - "$OUT" <<'EOF' | tee "$OUT/result.txt"
# jv880_lcd_video.sv, 800 x 600 frame: the LCD panel at (16, 50), 768 x 104, dot-on colour 0a1404;
# the status strip at (16, 560), 510 lines below the LCD's top: boxes at x 0-199 (state band),
# 220-259 (MIDI), 280-319 (underrun), 340-379 (lag) from the LCD's left edge. Screenshots come
# scaled; every offset is scaled by width / 800. The strip is found by the state band's colour
# (the front-panel art's palette keeps off the band colours).
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
bx, by = s[0], s[1] - int(510 * k)
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
    # the panel without the cursor rows (dot row 7 of each text row: the underline may be in
    # either phase of its blink)
    im = Image.open(f"{out}/{name}.png").convert("RGB").crop((bx, by, bx + int(768 * k), by + int(104 * k)))
    px = im.load()
    for row in (0, 1):
        for y in range(int((4 + 50 * row + 34) * k), min(im.size[1], int((4 + 50 * row + 41) * k))):
            for x in range(im.size[0]):
                px[x, y] = (0, 0, 0)
    return im.tobytes()
before, changed, reloaded = panel("nv_before"), panel("nv_changed"), panel("nv_reloaded")
sav = int(open(f"{out}/sav_size").read().strip() or 0)
nv_ok = sav == 32768 and changed != before and reloaded == changed
print(f"save_file_bytes: {sav}")
print(f"nvram_after_reload: {'kept' if reloaded == changed else 'as before the change' if reloaded == before else 'other'}"
      f"{'' if changed != before else ' (the change did not show)'}")
# MIDI out: the DT1 reply to the RQ1 (cores/sc55/tests/test_mcu_lockstep.py has the reference's)
REF_DT1 = bytes.fromhex(
    "f041104612000000000040340001010000030700030b030100010101010101010101010101010100100f000000"
    "40404040404040404040404040404040404040404040404040404040" "41f7")
rx = bytes.fromhex(open(f"{out}/midi_out.txt").read())
i = rx.find(bytes.fromhex("f04110461200000000"))
msg = rx[i:rx.index(0xF7, i) + 1] if i >= 0 and 0xF7 in rx[i:] else b""
midi_ok = len(msg) == 75 and sum(msg[5:-1]) % 128 == 0
print(f"midi_out: {len(rx)} bytes; DT1 {len(msg)} bytes, checksum {'ok' if midi_ok else 'bad or missing'}, "
      f"{'= reference' if msg == REF_DT1 else 'differs from the reference (factory NVRAM)'}: {rx.hex(' ')}")
# the panel after the reply: back to the boot screen, then SYSTEM changes it
after, key = panel("midi_out"), panel("rq1_key")
panel_ok = after == panel("boot") and key != after
print(f"panel_after_rq1: {'play screen' if after == panel('boot') else 'not the play screen'}, "
      f"SYSTEM {'taken' if key != after else 'ignored'}")
sys.exit(0 if band == "running" and und_s == "never" and lag_s == "never" and lit > 200 and nv_ok and midi_ok
         and panel_ok else 1)
EOF
