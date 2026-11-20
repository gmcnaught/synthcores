#!/usr/bin/env bash
# Run the project's MAME build without a window and without taking macOS focus.
# SDL dummy video/audio drivers keep SDL from starting a Cocoa app; host input
# providers are disabled for the same reason. Snapshots still work.
#
# Usage: tools/mame-headless.sh <system> [extra MAME args...]
#   e.g. tools/mame-headless.sh proteusxrpl -seconds_to_run 20 -midiin build/single_note.mid
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
export SDL_VIDEODRIVER=dummy SDL_AUDIODRIVER=dummy
exec "$ROOT/third_party/mame/proteus" "$@" \
    -rompath "$ROOT/roms" \
    -video none -sound none -nothrottle -skip_gameinfo \
    -keyboardprovider none -mouseprovider none -joystickprovider none
