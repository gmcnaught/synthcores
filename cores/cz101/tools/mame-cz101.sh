#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Run the patched MAME cz101 build headless (no window, no focus).
# Usage: tools/mame-cz101.sh ROMPATH WORKDIR [MAME args...]
#   ROMPATH holds cz101/ (hn613256ps40.bin, upd7811g-120.bin, init_ram.bin).
#   WORKDIR receives nvram/cfg/diff so runs never share state.
# Logging (see mame/patches/0001-cz101-logging.patch):
#   CZ_TRACE=file CZ_TRACE_LIMIT=n   CPU trace, exit after n instructions
#   CZ_SND_LOG=file CZ_SAMPLES=file  uPD933 register log, raw 40 kHz int32 samples
set -euo pipefail
MAME="${MAME_CZ101:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../../../.." && pwd)/mame-cz101/cz101}"
ROMPATH=$1; WORK=$2; shift 2
mkdir -p "$WORK"
export SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy
cd "$WORK"
exec "$MAME" cz101 -rompath "$ROMPATH" \
    -nvram_directory "$WORK/nvram" -cfg_directory "$WORK/cfg" -diff_directory "$WORK/diff" \
    -video none -sound none -nothrottle -skip_gameinfo \
    -keyboardprovider none -mouseprovider none -joystickprovider none "$@"
