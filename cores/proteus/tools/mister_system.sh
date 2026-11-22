#!/usr/bin/env bash
# Install and run the full Proteus core on a MiSTer, play MIDI into it, and collect evidence.
#
#   tools/mister_system.sh RBF SYSTEM.bin MIDI_SCRIPT [BOOT_SECONDS]
#
# MISTER=root@<address> (SSH key login) is required. Installs
#   /media/fat/_Other/Proteus_<date>.rbf, /media/fat/games/Proteus/proteus_system.bin,
#   /media/fat/Proteus.mgl (loads the core and the system image)
# loads it, waits BOOT_SECONDS (default 15), screenshots (build/mister_boot.png), plays
# MIDI_SCRIPT ("<seconds> <hex bytes>" lines) into /dev/ttyS1 with tools/mister_midi.py,
# screenshots again (build/mister_play.png) and copies the audio capture ring
# (0x31000000, left/right s16 LE at 39062.5 Hz) to build/mister_system_capture.raw.
# Uses the shared device lock /tmp/claude-device.lock on the MiSTer.
set -euo pipefail
cd "$(dirname "$0")/.."
RBF=${1:?usage: mister_system.sh RBF SYSTEM.bin MIDI_SCRIPT [BOOT_SECONDS]}
IMG=${2:?}; MIDI=${3:?}; BOOT=${4:-15}
DEV=${MISTER:?set MISTER to root@<address of the MiSTer>}
DATE=$(date +%Y%m%d)
OUT=build; mkdir -p "$OUT"

ssh "$DEV" "mkdir /tmp/claude-device.lock 2>/dev/null && echo proteus-system > /tmp/claude-device.lock/owner" \
    || { echo "device locked by: $(ssh "$DEV" cat /tmp/claude-device.lock/owner 2>/dev/null)" >&2; exit 3; }
trap 'ssh "$DEV" "rm -rf /tmp/claude-device.lock"' EXIT

shot() {
    ssh "$DEV" "rm -f /media/fat/screenshots/Proteus/*.png; timeout 10 sh -c 'echo screenshot > /dev/MiSTer_cmd'; sleep 3"
    local f; f=$(ssh "$DEV" "ls -t /media/fat/screenshots/Proteus/ | head -1")
    scp -q "$DEV:/media/fat/screenshots/Proteus/$f" "$OUT/$1"
    echo "$OUT/$1"
}

cat > "$OUT/Proteus.mgl" <<EOF
<mistergamedescription>
    <rbf>_Other/Proteus</rbf>
    <file delay="1" type="f" index="1" path="proteus_system.bin"/>
</mistergamedescription>
EOF
ssh "$DEV" "mkdir -p /media/fat/games/Proteus /media/fat/_Other; rm -f /media/fat/_Other/Proteus_*.rbf"
scp -q "$RBF" "$DEV:/media/fat/_Other/Proteus_$DATE.rbf"
scp -q "$IMG" "$DEV:/media/fat/games/Proteus/proteus_system.bin"
scp -q "$OUT/Proteus.mgl" "$DEV:/media/fat/Proteus.mgl"
scp -q tools/mister_midi.py "$MIDI" "$DEV:/tmp/"
ssh "$DEV" "timeout 10 sh -c 'echo load_core /media/fat/Proteus.mgl > /dev/MiSTer_cmd'"
echo "core loading; waiting $BOOT s"
sleep "$BOOT"
shot mister_boot.png
ssh "$DEV" "killall -q midilink 2>/dev/null; python3 /tmp/mister_midi.py /tmp/$(basename "$MIDI")"
shot mister_play.png
MIB=16
ssh "$DEV" "python3 -c 'import mmap,os,sys; fd=os.open(\"/dev/mem\",os.O_RDONLY|os.O_SYNC); m=mmap.mmap(fd,$MIB<<20,mmap.MAP_SHARED,mmap.PROT_READ,offset=0x31000000); sys.stdout.buffer.write(m[:])'" > "$OUT/mister_system_capture.raw"
python3 -I - "$OUT/mister_system_capture.raw" <<'EOF'
import array, math, sys
a = array.array("h", open(sys.argv[1], "rb").read())
fs = 39062.5
for s in range(0, len(a) // 2 // int(fs)):
    seg = a[int(s * fs) * 2:int((s + 1) * fs) * 2]
    rms = math.sqrt(sum(x * x for x in seg) / len(seg))
    if rms > 1: print(f"capture second {s:3d}: rms {rms:8.1f} peak {max(abs(x) for x in seg)}")
EOF
