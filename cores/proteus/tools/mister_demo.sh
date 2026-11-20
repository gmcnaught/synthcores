#!/usr/bin/env bash
# Install and run the Proteus demo core on a MiSTer, then read the core's audio
# capture ring back from DDR3 and compare it with tools/proteus_model.py.
#
#   tools/mister_demo.sh RBF DEMO.bin [SECONDS]
#
# MISTER=root@<address> (SSH key login) is required. Installs:
#   /media/fat/_Other/Proteus_<date>.rbf
#   /media/fat/games/Proteus/proteus_demo.bin
#   /media/fat/Proteus-demo.mgl (loads the core and the demo image)
# loads the MGL, waits SECONDS (default 20), copies SECONDS of capture from
# physical 0x31000000 to build/mister_capture.raw and compares it with the model.
# Uses the shared device lock /tmp/claude-device.lock on the MiSTer.
set -euo pipefail
cd "$(dirname "$0")/.."
RBF=${1:?usage: mister_demo.sh RBF DEMO.bin [SECONDS]}
DEMO=${2:?usage: mister_demo.sh RBF DEMO.bin [SECONDS]}
SECS=${3:-20}
DEV=${MISTER:?set MISTER to root@<address of the MiSTer>}
DATE=$(date +%Y%m%d)
OUT=build; mkdir -p "$OUT"

ssh "$DEV" "mkdir /tmp/claude-device.lock 2>/dev/null && echo proteus-demo > /tmp/claude-device.lock/owner" \
    || { echo "device locked by: $(ssh "$DEV" cat /tmp/claude-device.lock/owner 2>/dev/null)" >&2; exit 3; }
trap 'ssh "$DEV" "rm -rf /tmp/claude-device.lock"' EXIT

cat > "$OUT/Proteus-demo.mgl" <<EOF
<mistergamedescription>
    <rbf>_Other/Proteus</rbf>
    <file delay="1" type="f" index="1" path="proteus_demo.bin"/>
</mistergamedescription>
EOF
ssh "$DEV" "mkdir -p /media/fat/games/Proteus /media/fat/_Other; rm -f /media/fat/_Other/Proteus_*.rbf"
scp -q "$RBF" "$DEV:/media/fat/_Other/Proteus_$DATE.rbf"
scp -q "$DEMO" "$DEV:/media/fat/games/Proteus/proteus_demo.bin"
scp -q "$OUT/Proteus-demo.mgl" "$DEV:/media/fat/Proteus-demo.mgl"
# MiSTer_cmd is a FIFO read by Main_MiSTer: never block on it
ssh "$DEV" "timeout 10 sh -c 'echo load_core /media/fat/Proteus-demo.mgl > /dev/MiSTer_cmd'"
echo "core loading; waiting $SECS s"
sleep "$SECS"
# 4 bytes per output sample at 39062.5 Hz; read whole MiB blocks from 0x31000000 (784 MiB)
MIB=$(( (SECS * 39063 * 4 + 1048575) / 1048576 ))
ssh "$DEV" "dd if=/dev/mem bs=1M skip=784 count=$MIB 2>/dev/null" > "$OUT/mister_capture.raw"
python3 -I tools/compare_capture.py "$OUT/mister_capture.raw" "$DEMO" "$SECS"
