#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# Hardware smoke test of a P2K RBF on a MiSTer (cores/p2k/docs/plan.md, P5).
#
#   MISTER=root@<address> scripts/mister/smoke.sh RBF PACK SIMM EVENTS OUTDIR [PACK_ERASED]
#
# PACK, SIMM: tools/p2k_pack.py outputs (a system pack whose flash has the user data, and a SIMM
# image); EVENTS: "seconds hexbytes" MIDI lines (modules/synthframe/tools/midi/smf2events.py).
# Installs RBF (synth-mister install; run 'synth-mister backup' once first), copies PACK and SIMM
# to games/P2K/ as p2k_smoke.p2k / p2k_smoke.sim, removes the pack's save file, loads both
# through an MGL, waits for the main screen (up to 240 s; about 19 s with direct downloads) and:
#   boot.png    the main screen;
#   play.png    during EVENTS (repeated for SECONDS, default 30) sent at 9600 baud (the core's
#               MIDI input takes 9600 as well as MidiLink's 31250): the level meter must move;
#   end.png     after it;
#   master.png  after the Master key (Q): the LCD must change.
# With PACK_ERASED (a pack whose user area is erased: the OS writes factory defaults on its
# first start), the flash save: load it, wait for its main screen and ERASED_SETTLE seconds (default 60), open and close
# the OSD (autosave), the save file must be 2 MB with user data at 0x0A0000; load again
# (restored from the file): reload.png must show the main screen.
# OUTDIR/result.txt: the status strip (band, late), LCD dots lit, the peak level seen, the
# Master screen change and the save file. Exit 1 if any check fails.
set -euo pipefail
cd "$(dirname "$0")/../.."
RBF=${1:?usage: smoke.sh RBF PACK SIMM EVENTS OUTDIR [PACK_ERASED]}; PACK=${2:?}; SIMM=${3:?}; EV=${4:?}; OUT=${5:?}
ERASED=${6:-}
SECS=${SECS:-30}
M=../../modules/synthframe/bin/synth-mister
mkdir -p "$OUT"

$M lock p2k-smoke
trap '$M unlock' EXIT
$M install p2k "$RBF"
$M sh "mkdir -p /media/fat/games/P2K; rm -f /media/fat/saves/P2K/p2k_smoke.sav /media/fat/saves/P2K/p2k_erased.sav"
scp -q "$PACK" "$MISTER:/media/fat/games/P2K/p2k_smoke.p2k"
scp -q "$SIMM" "$MISTER:/media/fat/games/P2K/p2k_smoke.sim"
mgl() {
    printf '<mistergamedescription>\n  <rbf>_Devices/P2K</rbf>\n  <file delay="1" type="f" index="1" path="%s"/>\n  <file delay="2" type="f" index="2" path="p2k_smoke.sim"/>\n</mistergamedescription>\n' "$1" |
        $M sh "cat > /tmp/$2.mgl"
}
mgl p2k_smoke.p2k p2k_smoke
scp -q ../../modules/synthframe/tools/mister/mister_keys.py ../../modules/synthframe/tools/mister/play_tty.py "$EV" "$MISTER:/tmp/"
keys() { $M sh "python3 /tmp/mister_keys.py $*"; }
# wait (up to LIMIT seconds) until the LCD shows text and the band is green, then screenshot to $1;
# $1.secs: the seconds from the MGL load to the main screen (2 s steps)
lit() {
    python3 -I - "$1" <<'PY'
import sys
from PIL import Image
im = Image.open(sys.argv[1]).convert("RGB"); k = im.size[0] / 800
band = im.getpixel((int(40 * k), int(566 * k)))
box = im.crop((int(30 * k), int(56 * k), int(770 * k), int(156 * k)))
n = sum(1 for x in range(box.size[0]) for y in range(box.size[1]) if box.getpixel((x, y))[0] < 0x70 and box.getpixel((x, y))[1] < 0x70)
print(n if band[1] > 0xa0 and band[0] < 0x60 else 0)
PY
}
wait_main() {
    local t0=$(date +%s)
    while [ $(( $(date +%s) - t0 )) -lt "${2:-240}" ]; do
        sleep 2
        $M shot p2k "$1" >/dev/null 2>&1 || continue
        [ "$(lit "$1")" -gt 3000 ] && { echo "$(( $(date +%s) - t0 ))" > "$1.secs"; sleep "${SETTLE:-15}"; $M shot p2k "$1" >/dev/null; return 0; }
    done
    return 1
}
$M midi-stop
$M load /tmp/p2k_smoke.mgl
wait_main "$OUT/boot.png" || true
$M sh "stty -F /dev/ttyS1 9600 raw -echo"
# a few zero bytes (framing errors at 31250) switch the input to 9600 (synthframe_midi_rx)
printf '0000000000000000' | xxd -r -p | $M sh "cat > /dev/ttyS1; sleep 1"
$M sh "python3 /tmp/play_tty.py /tmp/$(basename "$EV") $SECS" > "$OUT/play.txt" &
PLAY=$!
sleep 6
$M shot p2k "$OUT/play.png" >/dev/null
wait $PLAY
$M shot p2k "$OUT/end.png" >/dev/null
keys text:q sleep:2
$M shot p2k "$OUT/master.png" >/dev/null
keys text:q sleep:1

SAV=0; USER=no
if [ -n "$ERASED" ]; then
    scp -q "$ERASED" "$MISTER:/media/fat/games/P2K/p2k_erased.p2k"
    mgl p2k_erased.p2k p2k_erased
    $M load /tmp/p2k_erased.mgl
    wait_main "$OUT/erased_boot.png" 400 || true
    sleep "${ERASED_SETTLE:-60}"             # the OS writing its factory defaults
    keys F12 sleep:5 ESC sleep:1
    SAV=$($M sh "stat -c %s /media/fat/saves/P2K/p2k_erased.sav 2>/dev/null || echo 0")
    USER=$($M sh "dd if=/media/fat/saves/P2K/p2k_erased.sav bs=4096 skip=160 count=16 2>/dev/null | tr -d '\377' | wc -c")
    $M load /tmp/p2k_erased.mgl
    wait_main "$OUT/reload.png" || true
fi
echo "$SAV $USER" > "$OUT/sav"

python3 -I - "$OUT" "${ERASED:+1}" <<'EOF' | tee "$OUT/result.txt"
# p2k_lcd_video.sv, 800 x 600: LCD panel at (30, 56), 740 x 100, dots 5C5358 on / 83888B off;
# level meter at (30, 402), 512 x 12, lit 40E060; status strip at (30, 556): state band x 0-199,
# underrun box (here: a late sample) at x 280-319. Lit dots and meter: by darkness / green
# (screenshots are scaled and filtered). Screenshots come scaled: offsets scale by
# width / 800; the strip is found by the state band's colour.
import os, sys
from PIL import Image
out, erased = sys.argv[1], len(sys.argv) > 2 and sys.argv[2] == "1"
COL = {(0x20, 0xc0, 0x40): "running", (0xe0, 0xa0, 0x20): "loading", (0x20, 0x60, 0xc0): "rom ok",
       (0xc0, 0x20, 0x20): "no rom"}
def load(n): return Image.open(f"{out}/{n}.png").convert("RGB")
def strip(im):
    px = im.load(); w, h = im.size
    for y in range(h):
        for x in range(w):
            if px[x, y][:3] in COL:
                return x, y, COL[px[x, y][:3]]
    return None
def near(c, ref, tol=24): return all(abs(a - b) <= tol for a, b in zip(c, ref))
def lcd(im, s):
    k = im.size[0] / 800; x0, y0 = s[0], s[1] - int(500 * k)
    box = im.crop((x0, y0, x0 + int(740 * k), y0 + int(100 * k)))
    return sum(1 for x in range(box.size[0]) for y in range(box.size[1])
               if box.getpixel((x, y))[0] < 0x70 and box.getpixel((x, y))[1] < 0x70), box.tobytes()
def meter(im, s):
    k = im.size[0] / 800; x0, y = s[0], s[1] - int(148 * k)
    return sum(1 for x in range(x0, x0 + int(512 * k)) if im.getpixel((x, y))[1] > 0xa0 and im.getpixel((x, y))[0] < 0x80)
ok = True
end = load("end"); s = strip(end)
if not s:
    print("band: not found"); sys.exit(1)
k = end.size[0] / 800
late = end.getpixel((s[0] + int(300 * k), s[1] + int(10 * k)))
late_s = "never" if near(late, (0x30, 0x38, 0x48), 8) else "seen"
lit, boot_px = lcd(load("boot"), s)
lvl = meter(load("play"), s)
mlit, master_px = lcd(load("master"), s)
print(f"band: {s[2]}"); ok &= s[2] == "running"
secs = open(f"{out}/boot.png.secs").read().strip() if os.path.exists(f"{out}/boot.png.secs") else "none"
print(f"seconds_to_main_screen: {secs}")
print(f"late_sample: {late_s}"); ok &= late_s == "never"
print(f"lcd_lit_pixels_boot: {lit}"); ok &= lit > 200
print(f"level_meter_pixels_playing: {lvl}"); ok &= lvl > 0
print(f"master_key_changes_lcd: {master_px != boot_px}"); ok &= master_px != boot_px
if erased:
    sav, user = open(f"{out}/sav").read().split()
    rl, _ = lcd(load("reload"), s)
    print(f"save_file_bytes: {sav}"); ok &= sav == "2097152"
    print(f"save_user_area_bytes_not_ff (64 KB at 0x0A0000): {user}"); ok &= int(user) > 0
    print(f"lcd_lit_pixels_reload: {rl}"); ok &= rl > 200
sys.exit(0 if ok else 1)
EOF
