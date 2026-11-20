#!/bin/bash
# Run the core on a MiSTer and read its screen back (the hardware test).
#
#   MISTER=root@<address> scripts/mister/sc55_run.sh RBF [MIDI_HEX...]
#
# Installs RBF as /media/fat/_Other/SC55_<date>.rbf and the ROM pack (made
# from $SC55_ROM_DIR, default firmware/, with tools/romset.py pack) as
# /media/fat/games/SC-55/boot.rom; loads the core; waits WAIT seconds
# (default 8) for the firmware to start; sends MIDI_HEX byte strings (e.g.
# 90403f for a note) to the core's UART at 9600 baud (the core detects 9600
# as well as MidiLink's 31250, so no MidiLink is needed); takes a screenshot
# and copies it to out/mister/<TAG>.png (TAG default "run").
# KEEP=0 returns to the menu core afterwards.
set -euo pipefail
cd "$(dirname "$0")/../.."
RBF=${1:?usage: sc55_run.sh RBF [MIDI_HEX...]}; shift
DEV=${MISTER:?set MISTER to root@<address of the MiSTer>}
ROMS=${SC55_ROM_DIR:-firmware}; TAG=${TAG:-run}
OUT=out/mister; mkdir -p "$OUT"
DATE=$(date +%Y%m%d)

python3 tools/romset.py pack "$ROMS" "$OUT/boot.rom" >/dev/null
ssh "$DEV" "mkdir -p /media/fat/_Other /media/fat/games/SC-55; rm -f /media/fat/_Other/SC55_*.rbf /media/fat/screenshots/SC-55/*.png"
scp -q "$RBF" "$DEV:/media/fat/_Other/SC55_$DATE.rbf"
scp -q "$OUT/boot.rom" "$DEV:/media/fat/games/SC-55/boot.rom"
# Zaparoo reopens ttyS1 every second and resets its speed (synclavierii notes)
ssh "$DEV" "[ -x /media/fat/Scripts/zaparoo.sh ] && /media/fat/Scripts/zaparoo.sh -service stop >/dev/null 2>&1 || true"
# MiSTer_cmd is a FIFO read by Main_MiSTer: never block on it
ssh "$DEV" "timeout 10 sh -c 'echo load_core /media/fat/_Other/SC55_$DATE.rbf > /dev/MiSTer_cmd'; sleep ${WAIT:-8}"
if [ $# -gt 0 ]; then
    ssh "$DEV" "stty -F /dev/ttyS1 9600 raw -echo"
    for m in "$@"; do
        ssh "$DEV" "printf '$(echo "$m" | sed 's/\(..\)/\\\\x\1/g')' > /dev/ttyS1; sleep 0.3"
    done
    sleep 1
fi
ssh "$DEV" "sleep 1; timeout 10 sh -c 'echo screenshot > /dev/MiSTer_cmd'; sleep 2"
SHOT=$(ssh "$DEV" "ls -t /media/fat/screenshots/SC-55/ 2>/dev/null | head -1")
[ -n "$SHOT" ] && scp -q "$DEV:/media/fat/screenshots/SC-55/$SHOT" "$OUT/$TAG.png" && echo "$OUT/$TAG.png"
if [ "${KEEP:-1}" = 0 ]; then
    ssh "$DEV" "timeout 10 sh -c 'echo load_core /media/fat/menu.rbf > /dev/MiSTer_cmd'"
fi
