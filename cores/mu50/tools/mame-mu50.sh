#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Run the patched MAME mu50 build headless (no window, no focus).
# Usage: tools/mame-mu50.sh ROMPATH WORKDIR [MAME args...]
#   ROMPATH holds mu50/ (program ROM and the two wave ROMs; tools/mu50_testrom.py writes a test set).
#   WORKDIR receives nvram/cfg/diff so runs never share state.
# Logging (see mame/patches/0001-mu50-logging.patch):
#   MU_TRACE=file MU_TRACE_LIMIT=n   H8 trace (one line per instruction, plus writes), exit after n
#   MU_BUS=1                         also every bus access (fetches, reads, writes)
#   MU_SND_LOG=file                  every CPU write to the SWP00 with CPU states and sample count
#   MU_SAMPLES=file                  raw 44.1 kHz samples, int32 L/R
set -euo pipefail
MAME="${MAME_MU50:-$(cd "$(dirname "${BASH_SOURCE[0]}")/../../../.." && pwd)/mame-mu50/mu50}"
ROMPATH=$1; WORK=$2; shift 2
mkdir -p "$WORK"
export SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy
cd "$WORK"
exec "$MAME" mu50 -rompath "$ROMPATH" \
    -nvram_directory "$WORK/nvram" -cfg_directory "$WORK/cfg" -diff_directory "$WORK/diff" \
    -video none -sound none -nothrottle -skip_gameinfo \
    -keyboardprovider none -mouseprovider none -joystickprovider none "$@"
