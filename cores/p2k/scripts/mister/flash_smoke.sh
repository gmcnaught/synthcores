#!/usr/bin/env bash
# SPDX-License-Identifier: GPL-3.0-or-later
# Hardware test of user flash SIMMs (cores/p2k/docs/plan.md, P6) on a MiSTer.
#
#   MISTER=root@<address> scripts/mister/flash_smoke.sh RBF PACK FLASHSIM OUTDIR [EXPECT.sim]
#
# PACK: a system pack whose flash has the user data (tools/p2k_pack.py pack --flash ...);
# FLASHSIM: a flash SIMM file with ROM ID 106 (tools/p2k_pack.py simm --flash, from a stand-in built
# with "id": 106); EXPECT.sim: the file the OS's rename should leave (MAME's images after
# cores/proteus/tools/p2k_flash_test.py with --seconds long enough for "Done. Please reboot now!",
# Baby then Big; or the system bench's, sim/p2k_board_tb --flash-slot 1 --save-baby/--save-big).
# Installs RBF, copies PACK and FLASHSIM to games/P2K/ as p2k_fl.p2k / p2k_fl1.sim, removes the
# pack's save file, and loads an MGL that loads the pack and mounts the SIMM file as "Flash SIMM
# slot 1". Then:
#   boot.png     the main screen (load.secs: seconds from the MGL load, the 34 MB mount included);
#   rename.png   Save/Copy -> RENAME FLASH SIMM, ID 106 -> 108, Enter, Enter (as
#                cores/proteus/tests/p2k_flash/rename_106_108.keys), RENAME_WAIT s later (default 60);
#   the OSD opened and closed: autosave writes the changed regions back to the mounted file;
#   reload.png   the MGL again: the main screen from the written file.
# OUTDIR/result.txt: the ROM ID in the file before and after, its SHA-256 (and whether it equals
# EXPECT.sim), whether a sample started late (the status strip's underrun box on rename.png).
# Exit 1 if the ID is not 108, the file differs from EXPECT.sim, a sample was late, or a main
# screen is missing.
set -euo pipefail
cd "$(dirname "$0")/../.."
RBF=${1:?usage: flash_smoke.sh RBF PACK FLASHSIM OUTDIR [EXPECT.sim]}; PACK=${2:?}; SIM=${3:?}; OUT=${4:?}
EXPECT=${5:-}
M=../../modules/synthframe/bin/synth-mister
mkdir -p "$OUT"

$M lock p2k-flash-smoke
trap '$M unlock' EXIT
$M install p2k "$RBF"
$M sh "mkdir -p /media/fat/games/P2K; rm -f /media/fat/saves/P2K/p2k_fl.sav"
scp -q "$PACK" "$MISTER:/media/fat/games/P2K/p2k_fl.p2k"
scp -q "$SIM" "$MISTER:/media/fat/games/P2K/p2k_fl1.sim"
printf '<mistergamedescription>\n  <rbf>_Devices/P2K</rbf>\n  <file delay="1" type="f" index="1" path="p2k_fl.p2k"/>\n  <file delay="1" type="s" index="1" path="p2k_fl1.sim"/>\n</mistergamedescription>\n' |
    $M sh "cat > /tmp/p2k_fl.mgl"
scp -q ../synclavier/scripts/mister/mister_keys.py "$MISTER:/tmp/"
keys() { $M sh "python3 /tmp/mister_keys.py $*"; }
romid() { $M sh "dd if=/media/fat/games/P2K/p2k_fl1.sim bs=1 skip=$((0x20003E)) count=4 2>/dev/null | od -An -tu1" | awk '{print $1*16777216+$2*65536+$3*256+$4}'; }
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
# the status strip's underrun box (x 310-349 at y 556-575: strip x 280-319): dark red once a sample was late
late() {
    python3 -I - "$1" <<'PY'
import sys
from PIL import Image
im = Image.open(sys.argv[1]).convert("RGB"); k = im.size[0] / 800
r, g, b = im.getpixel((int(330 * k), int(566 * k)))
print(1 if r > 0x60 and g < 0x40 else 0)
PY
}
# wait (up to LIMIT seconds) for the LCD text with the band green; screenshot to $1; $1.secs
wait_main() {
    local t0=$(date +%s)
    while [ $(( $(date +%s) - t0 )) -lt "${2:-300}" ]; do
        sleep 2
        $M shot p2k "$1" >/dev/null 2>&1 || continue
        [ "$(lit "$1")" -gt 3000 ] && { echo "$(( $(date +%s) - t0 ))" > "$1.secs"; sleep "${SETTLE:-15}"; $M shot p2k "$1" >/dev/null; return 0; }
    done
    return 1
}
ID0=$(romid)
OK=1
$M load /tmp/p2k_fl.mgl
wait_main "$OUT/boot.png" || OK=0
# the rename: Save/Copy, 8 knob steps, cursor right x6, ID +2, Enter, Enter
keys text:s sleep:1 text:= sleep:0.4 text:= sleep:0.4 text:= sleep:0.4 text:= sleep:0.4 text:= sleep:0.4 \
     text:= sleep:0.4 text:= sleep:0.4 text:= sleep:1.2 RIGHT sleep:0.4 RIGHT sleep:0.4 RIGHT sleep:0.4 \
     RIGHT sleep:0.4 RIGHT sleep:0.4 RIGHT sleep:1 text:= sleep:0.4 text:= sleep:1 ENTER sleep:2 ENTER
sleep "${RENAME_WAIT:-60}"
$M shot p2k "$OUT/rename.png" >/dev/null || true
LATE=$(late "$OUT/rename.png" 2>/dev/null || echo 1)
[ "$LATE" = 0 ] || OK=0
T0=$(date +%s)
keys F12 sleep:"${SAVE_WAIT:-20}" ESC sleep:1
SAVE_SECS=$(( $(date +%s) - T0 ))
ID1=$(romid)
SHA=$($M sh "sha256sum /media/fat/games/P2K/p2k_fl1.sim" | cut -c1-64)
SAME=n/a
if [ -n "$EXPECT" ]; then
    E=$(shasum -a 256 "$EXPECT" | cut -c1-64)
    if [ "$E" = "$SHA" ]; then SAME=yes; else SAME=no; OK=0; fi
fi
[ "$ID1" = 108 ] || OK=0
$M load /tmp/p2k_fl.mgl
wait_main "$OUT/reload.png" || OK=0
{
    echo "load_secs=$(cat "$OUT/boot.png.secs" 2>/dev/null || echo none)"
    echo "rom_id_before=$ID0 rom_id_after=$ID1"
    echo "file_sha256=$SHA equals_expected=$SAME"
    echo "late_sample=$LATE"
    echo "osd_open_secs=$SAVE_SECS"
    echo "reload_secs=$(cat "$OUT/reload.png.secs" 2>/dev/null || echo none)"
    echo "result=$([ $OK = 1 ] && echo PASS || echo FAIL)"
} | tee "$OUT/result.txt"
[ $OK = 1 ]
