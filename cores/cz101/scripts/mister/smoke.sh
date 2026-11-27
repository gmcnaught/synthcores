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
# program change selects preset 15). MIDI out: keys "3" (Preset) and F3 (Tone 3) while
# tty_midi_xfer.py --baud 9600 reads /dev/ttyS1 at 9600 baud (synthframe_midi_rx found the HPS
# UART at 9600 above, and synthframe_midi_tx follows it); Tone 3 in the bank half
# that holds preset 15 selects preset 11, and the firmware sends its program change, C0 0A
# (the C4 panel case, tests/panel/basic.txt, checks the MIDI-out bytes against MAME).
# An edited tone survives a reload: with Write ("y") held, Internal ("4") and Tone 2 (F2) store
# preset 11 in internal tone 2 (as tests/panel/write.txt; tests/reload_run.sh checks it on the
# bench); screenshot; the OSD opens (autosave) and closes: save 1. The core and pack load again
# (the save file is mounted and restored), "4" and F2 select internal tone 2, screenshot, the OSD
# opens and closes: save 2. Both saves must be 8 KB; save 1's battery RAM must differ from the
# pack's init_ram, including internal tone 2 (0880-08FF); save 2 must hold the same internal
# tone 2, and the same tone buffers (0300-03FF) as save 1, where internal tone 2 was selected
# too: the firmware recalled the written tone after the reload. A screenshot after each save
# (saved1, saved2: the reload resets the audio FIFO and its underrun count, so each save is
# checked before it) must show no underrun: a save must not stop the audio.
# Power: the OSD's Power (F12, DOWN, ENTER, ESC) off: the firmware powers down, the LCD is dark
# (MAME m_power = 0) and no underrun is seen while powered down (power_off); on again: the board
# restarts, the band is green and the LCD is lit (power_on).
# Writes OUTDIR/{boot,end,nv_changed,saved1,nv_reloaded,saved2,power_off,power_on}.png,
# midi_out.txt, save1.sav, save2.sav and result.txt. Exit 1 if the core is not running (band not
# green), an underrun was seen (end, saved1, saved2, power_off), the LCD is blank at boot, MIDI did
# not change the LCD, the program change did not come out, a save file is wrong, the LCD is lit
# while powered off, or the core does not restart with a lit LCD.
set -euo pipefail
cd "$(dirname "$0")/../.."
RBF=${1:?usage: smoke.sh RBF PACK OUTDIR}; PACK=${2:?}; OUT=${3:?}
M=../../modules/synthframe/bin/synth-mister
mkdir -p "$OUT"
python3 ../../modules/synthframe/tools/midi/mkmidi.py tests/midi/basic.txt "$OUT/basic.mid" >/dev/null
python3 ../../modules/synthframe/tools/midi/smf2events.py "$OUT/basic.mid" "$OUT/basic.txt" >/dev/null
SECS=$(awk 'END { print int($1) + 2 }' "$OUT/basic.txt")

$M lock cz101-smoke
trap '$M unlock' EXIT
$M install cz101 "$RBF"
$M sh "mkdir -p /media/fat/games/CZ-101; rm -f /media/fat/games/CZ-101/boot.rom /media/fat/saves/CZ-101/cz101_smoke.sav"
scp -q "$PACK" "$MISTER:/media/fat/games/CZ-101/cz101_smoke.rom"
printf '<mistergamedescription>\n  <rbf>_Devices/CZ101</rbf>\n  <file delay="1" type="f" index="1" path="cz101_smoke.rom"/>\n</mistergamedescription>\n' |
    $M sh "cat > /tmp/cz101_smoke.mgl"
scp -q ../../modules/synthframe/tools/mister/mister_keys.py "$MISTER:/tmp/"
keys() { $M sh "python3 /tmp/mister_keys.py $*"; }
$M midi-stop
$M load /tmp/cz101_smoke.mgl; sleep "${BOOT:-8}"
$M shot cz101 "$OUT/boot.png" >/dev/null
$M sh "stty -F /dev/ttyS1 9600 raw -echo"
# a few zero bytes (framing errors at 31250) switch the input to 9600 (synthframe_midi_rx)
printf '0000000000000000' | xxd -r -p | $M sh "cat > /dev/ttyS1; sleep 1"
scp -q ../../modules/synthframe/tools/mister/play_tty.py "$OUT/basic.txt" "$MISTER:/tmp/"
$M sh "python3 /tmp/play_tty.py /tmp/basic.txt $SECS"
$M shot cz101 "$OUT/end.png" >/dev/null

# MIDI out
scp -q ../../modules/synthframe/tools/mister/tty_midi_xfer.py "$MISTER:/tmp/"
$M sh "nohup python3 /tmp/tty_midi_xfer.py --baud 9600 6 > /tmp/cz101_midi_out.txt 2>&1 < /dev/null &"
sleep 1
keys text:3 sleep:1 F3 sleep:1
sleep 2
$M sh "cat /tmp/cz101_midi_out.txt" > "$OUT/midi_out.txt"

# write a tone, save, reload, recall it, save again
rm -f "$OUT/save1.sav" "$OUT/save2.sav"
keys down:y sleep:0.4 text:4 sleep:0.3 F2 sleep:0.5 up:y sleep:1
$M shot cz101 "$OUT/nv_changed.png" >/dev/null
keys F12 sleep:3 ESC sleep:1
$M shot cz101 "$OUT/saved1.png" >/dev/null
scp -q "$MISTER:/media/fat/saves/CZ-101/cz101_smoke.sav" "$OUT/save1.sav" 2>/dev/null || true
$M load /tmp/cz101_smoke.mgl; sleep "${BOOT:-8}"
keys text:4 sleep:1 F2 sleep:1
$M shot cz101 "$OUT/nv_reloaded.png" >/dev/null
keys F12 sleep:3 ESC sleep:1
$M shot cz101 "$OUT/saved2.png" >/dev/null
scp -q "$MISTER:/media/fat/saves/CZ-101/cz101_smoke.sav" "$OUT/save2.sav" 2>/dev/null || true

# Power off (OSD), then on again
keys F12 sleep:1 DOWN sleep:0.5 ENTER sleep:0.5 ESC sleep:3
$M shot cz101 "$OUT/power_off.png" >/dev/null
keys F12 sleep:1 DOWN sleep:0.5 ENTER sleep:0.5 ESC sleep:"${BOOT:-8}"
$M shot cz101 "$OUT/power_on.png" >/dev/null
python3 -I - "$OUT" "$PACK" <<'EOF' | tee "$OUT/result.txt"
# cz101_lcd_video.sv, 800 x 600 frame: the LCD (MAME's 97 x 19 pixels at 4x) at (206, 46),
# 388 x 76, background 8a9294, dot on 3f3b3e; synthframe_status_strip at (12, 562): band at
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
    x0, y0 = s[0] + int(194 * k), s[1] - int(516 * k)
    return im.crop((x0, y0, x0 + int(388 * k), y0 + int(76 * k)))
def lit(p):
    return sum(1 for c in p.getdata() if abs(c[0] - 0x3f) < 16 and abs(c[1] - 0x3b) < 16 and abs(c[2] - 0x3e) < 16)
end = Image.open(f"{out}/end.png").convert("RGB")
s = strip(end)
if not s:
    print("band: not found"); sys.exit(1)
k = end.size[0] / 800
def underrun(name):
    im = Image.open(f"{out}/{name}.png").convert("RGB")
    u = im.getpixel((s[0] + int(300 * k), s[1] + int(10 * k)))[:3]
    return {(0x30, 0x38, 0x48): "never", (0x90, 0x20, 0x20): "seen", (0xff, 0x40, 0x40): "now"}.get(u, "?%02x%02x%02x" % u)
def band(name):
    im = Image.open(f"{out}/{name}.png").convert("RGB")
    return BAND.get(im.getpixel((s[0] + int(100 * k), s[1] + int(10 * k)))[:3], "?")
und_s = underrun("end")
boot, endp = panel("boot", s), panel("end", s)
changed, reloaded = panel("nv_changed", s), panel("nv_reloaded", s)
def load(name):
    p = f"{out}/{name}"
    return open(p, "rb").read() if os.path.exists(p) else b""
s1, s2 = load("save1.sav"), load("save2.sav")
sz = len(s1)
ram = s1[:4096]
init = open(pack, "rb").read()[0x8000:0x9000]
print(f"band: {s[2]}")
print(f"underrun: {und_s}")
print(f"lcd_lit_pixels_boot: {lit(boot)}")
print(f"midi_changed_lcd: {endp.tobytes() != boot.tobytes()}")
print(f"key_changed_lcd: {changed.tobytes() != endp.tobytes()}")
midi_out = open(f"{out}/midi_out.txt").read().split()
pc = "c0 0a" in " ".join(midi_out)
print(f"midi_out: {' '.join(midi_out)[:60]}")
print(f"midi_out_program_change: {pc}")
print(f"save_file_bytes: {sz}")
written = len(s1) == 8192 and s1[0x880:0x900] != init[0x880:0x900]
kept = len(s2) == 8192 and s2[0x880:0x900] == s1[0x880:0x900]
recalled = len(s2) == 8192 and s2[0x300:0x400] == s1[0x300:0x400]
print(f"save_ram_differs_from_init_ram: {bool(ram) and ram != init}")
print(f"internal_tone_2_written: {written}")
print(f"save2_bytes: {len(s2)}")
print(f"internal_tone_2_kept_after_reload: {kept}")
print(f"tone_recalled_after_reload: {recalled}")
print(f"lcd_after_reload_same_as_before: {reloaded.tobytes() == changed.tobytes()}")
und_saves = {n: underrun(n) for n in ("saved1", "saved2", "power_off")}
print(f"underrun_after_save1: {und_saves['saved1']}")
print(f"underrun_after_save2: {und_saves['saved2']}")
off, on = panel("power_off", s), panel("power_on", s)
off_dark = lit(off) == 0
print(f"power_off_lcd_dark: {off_dark} (lit pixels {lit(off)})")
print(f"underrun_while_powered_off: {und_saves['power_off']}")
on_ok = band("power_on") == "running" and lit(on) > 100
print(f"power_on_restarts: {on_ok} (band {band('power_on')}, lit pixels {lit(on)})")
ok = (s[2] == "running" and und_s == "never" and lit(boot) > 100 and endp.tobytes() != boot.tobytes()
      and pc and sz == 8192 and ram != init and written and kept and recalled
      and all(v == "never" for v in und_saves.values()) and off_dark and on_ok)
sys.exit(0 if ok else 1)
EOF
