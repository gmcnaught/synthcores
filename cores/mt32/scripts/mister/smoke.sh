#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-2.0-or-later
# MT-32 MiSTer smoke test: scripts/mister/smoke.sh RBF PACK OUTDIR
# Installs the RBF (synth-mister, with its lock and backup), loads the ROM pack, checks:
# boot LCD lit; MIDI over the UART (master volume 60 by SysEx, then notes on part 1, which make
# the firmware redraw: "vol: 60", as Nuked-MT32 shows for the same bytes) changes the LCD;
# no audio underrun; status band "running". The Part 1 key (Q) changes the LCD
# (Nuked-MT32 with mt32_ref --button shows "1|Slap Bass1|vol> 80" for the same input) and shows
# pressed on the front panel. The MIDI MESSAGE LED's state in each screenshot is printed only.
# MIDI out: two RQ1 requests (system area, patch temp) at 31,250 baud right after boot get the
# DT1 replies Nuked-MT32 sends (tests/midi/rq1.txt, T lines of its trace), byte for byte; the same
# requests at 9600 after the 9600-baud MIDI above (synthframe_midi_rx and _tx follow the HPS UART
# to 9600) get them with master volume 60.
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
scp -q ../../modules/synthframe/tools/mister/mister_keys.py "$MISTER:/tmp/"
keys() { $M sh "python3 /tmp/mister_keys.py $*"; }
$M midi-stop
$M load /tmp/mt32_smoke.mgl; sleep "${BOOT:-10}"
$M shot mt32 "$OUT/boot.png" >/dev/null
RQ1="f04110161110000000001759f7 f0411016110300000000106df7"
scp -q ../../modules/synthframe/tools/mister/tty_midi_xfer.py "$MISTER:/tmp/"
# The core load starts `/sbin/uartmode 3`, which respawns MidiLink every second after
# `synth-mister midi-stop` kills it; MidiLink holds ttyS1 and would read the replies. Stop the
# loops too ([u]: the pattern must not match this shell's own command line).
stop_midilink() {
    $M sh 'for p in /proc/[0-9]*; do grep -q "sbin/[u]artmode" $p/cmdline 2>/dev/null && kill ${p#/proc/}; done; killall -q midilink; sleep 2; true'
}
stop_midilink
$M sh "python3 /tmp/tty_midi_xfer.py --baud 31250 2 $RQ1" > "$OUT/midi_out_31250.txt"
$M sh "stty -F /dev/ttyS1 9600 raw -echo"
scp -q ../../modules/synthframe/tools/mister/play_tty.py "$MISTER:/tmp/"
scp -q "$OUT/midi.txt" "$MISTER:/tmp/mt32_midi.txt"
$M sh "python3 /tmp/play_tty.py /tmp/mt32_midi.txt $SECS"
sleep 1
$M shot mt32 "$OUT/midi.png" >/dev/null
# hold Q while the screenshot is taken: mister_keys.py's device goes away (and its keys come up)
# when it exits, so it runs in the background for the whole shot
$M sh "nohup python3 /tmp/mister_keys.py down:q sleep:20 up:q > /dev/null 2>&1 < /dev/null &"
sleep 5
$M shot mt32 "$OUT/key.png" >/dev/null
$M sh "while pgrep -f mister_keys.py > /dev/null; do sleep 1; done"
stop_midilink
# zero bytes first (framing errors at 31250): synthframe_midi_rx is at 9600 before the requests
$M sh "python3 /tmp/tty_midi_xfer.py --baud 9600 2.5 0000000000000000 $RQ1" > "$OUT/midi_out_9600.txt"
python3 -I - "$OUT" <<'PY' | tee "$OUT/result.txt"
import sys
from PIL import Image
out = sys.argv[1]
# mt32_lcd_video.sv, 800 x 600 frame: the LCD (20 x 36 by 48, background 6cb030, dot 10300c) at
# (40, 70); synthframe_status_strip at (40, 560): band at x 0-199, boxes at 220-259 (MIDI),
# 280-319 (underrun: dark red seen, bright red now), 340-379 (lag); the front panel art around
# them (cfg/panel.yaml): Part 1 (Q) button at (90, 226) 40 x 40, MIDI MESSAGE LED at (709, 147)
# 22 x 6, lit ff3020. Screenshots come scaled; offsets are scaled by width / 800; the strip is
# found by the band's colour.
BAND = {(0x20, 0xc0, 0x40): "running", (0xe0, 0xa0, 0x20): "loading", (0x20, 0x60, 0xc0): "rom ok",
        (0xc0, 0x20, 0x20): "no rom"}
def strip(im):
    px = im.load(); w, h = im.size
    for y in range(h):
        for x in range(w):
            if px[x, y][:3] in BAND:
                return x, y, BAND[px[x, y][:3]]
    return None
ims = {n: Image.open(f"{out}/{n}.png").convert("RGB") for n in ("boot", "midi", "key")}
s = strip(ims["midi"])
if not s:
    print("band: not found"); sys.exit(1)
k = ims["midi"].size[0] / 800
def area(im, x, y, w, h):   # frame rectangle, relative to the strip at (40, 560)
    x0, y0 = s[0] + int((x - 40) * k), s[1] + int((y - 560) * k)
    return im.crop((x0, y0, x0 + int(w * k), y0 + int(h * k)))
def lcd_im(im):
    return area(im, 40, 70, 720, 48)
def dots(im):   # LCD dots (0x10300c) inside the LCD
    return sum(1 for c in lcd_im(im).getdata() if c[0] < 0x30 and 0x20 <= c[1] < 0x50 and c[2] < 0x30)
def led(im):    # MIDI MESSAGE LED: red channel of its centre
    return "lit" if area(im, 709, 147, 22, 6).resize((1, 1)).getpixel((0, 0))[0] > 0xa0 else "off"
q_face = lambda im: area(im, 98, 234, 24, 24).tobytes()
und = ims["midi"].getpixel((s[0] + int(300 * k), s[1] + int(10 * k)))[:3]
und_s = {(0x30, 0x38, 0x48): "never", (0x90, 0x20, 0x20): "seen", (0xff, 0x40, 0x40): "now"}.get(und, "?%02x%02x%02x" % und)
lcd = lambda im: lcd_im(im).tobytes()
print(f"band: {s[2]}")
print(f"underrun: {und_s}")
print(f"lcd_dots_boot: {dots(ims['boot'])}")
print(f"midi_changed_lcd: {lcd(ims['midi']) != lcd(ims['boot'])}")
print(f"key_changed_lcd: {lcd(ims['key']) != lcd(ims['midi'])}")
print(f"key_pressed_on_panel: {q_face(ims['key']) != q_face(ims['midi'])}")
print("midi_message_led: " + ", ".join(f"{n} {led(im)}" for n, im in ims.items()))
# DT1 replies to the RQ1s, as Nuked-MT32 sends them (tests/midi/rq1.txt)
SYS = bytes.fromhex("4a000503030a06040300000006010203040506070809") + bytes([0x64])
def dt1(addr, data):
    return bytes.fromhex("f041101612") + addr + data + bytes([-sum(addr + data) & 0x7f, 0xf7])
TEMP = dt1(bytes.fromhex("030000"), bytes.fromhex("01041832" "0c000100" "5007007f" "00000000"))
want = {"31250": dt1(bytes.fromhex("100000"), SYS) + TEMP,
        "9600": dt1(bytes.fromhex("100000"), SYS[:-1] + bytes([60])) + TEMP}   # master volume 60
mo = {}
for b, w in want.items():
    got = bytes.fromhex(open(f"{out}/midi_out_{b}.txt").read().strip())
    mo[b] = got == w
    print(f"midi_out_{b}: {'as Nuked-MT32' if mo[b] else 'DIFFERENT'} ({len(got)} bytes: {got.hex(' ')})")
ok = s[2] == "running" and und_s == "never" and dots(ims["boot"]) > 200 and lcd(ims["midi"]) != lcd(ims["boot"]) \
     and lcd(ims["key"]) != lcd(ims["midi"]) and q_face(ims["key"]) != q_face(ims["midi"]) \
     and all(mo.values())
sys.exit(0 if ok else 1)
PY
