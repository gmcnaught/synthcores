# Roland JV-880 for MiSTer

An FPGA core of the Roland JV-880 synthesizer module (1992). It runs the original firmware on an
H8/532 with the Roland PCM chip and the 2 × 24 character LCD. The CPU, peripherals and PCM chip
are the SC-55 core's (`modules/roland`) with the `jv880` model, checked sample for sample
against [Nuked-SC55](https://github.com/jcmoyer/Nuked-SC55) (`modules/nuked-sc55`, romsets
`jv880-v1.0.0` and `jv880-v1.0.1`). No ROMs are included.

## Status

| Part | Check | Result |
|---|---|---|
| MCU (memory map, gate array, panel, encoder, NVRAM, timers) | `mcu_lockstep --romset jv880-v1.0.1 --jv880-factory`: from an empty NVRAM through the factory preset | 21,324,800 steps identical; the NVRAM's SHA-256 is d5da7845…d207, the reference's documented factory preset |
| PCM (wave ROM banks, 25/29 clock, filter) | `system_lockstep --smf jv880.mid` from the factory NVRAM | SHA-256 73feda3c…bbbc, the reference's integration-test hash |
| Board (pack, DDR3, pacing, MIDI, LCD) | `sim/jv880_board_bench` from power-on | in progress |
| MiSTer | `scripts/mister/smoke.sh` | to do |

The plan and the reference's differences from the SC-55 are in [`docs/plan.md`](docs/plan.md).

## Using the core

1. **Make the ROM pack from your own JV-880 ROM dumps.** Use `jv880_rom1.bin`, a
   `jv880_rom2` (v1.0.0 or v1.0.1) and `jv880_waverom1/2.bin`; the files are recognised by
   SHA-256, whatever their names:
   `python3 cores/sc55/tools/romset.py pack DIR boot.rom --romset jv880-v1.0.1`.
    - Optionally add `--nvram factory.nvram` to start from the factory preset.
    - `cores/sc55/ref/jv880_nvram.cpp` (built with the SC-55 simulators as `jv880-nvram`)
      makes that file: `jv880-nvram DIR factory.nvram`.
    - Without it, the unit starts as one whose battery ran down ("Internal RAM read error"). Then
      press UTILITY, turn to "Util:Factory preset" and press ENTER twice.
2. **Install:** copy `boot.rom` to `games/JV-880/` and the core (`JV880_YYYYMMDD.rbf`) to
   `_Devices/`. Load the pack from the OSD.
3. **MIDI:** set the UART to MidiLink and send MIDI.

Panel keys (PS/2 or USB keyboard, the reference frontend's map):

| Key | Button |
|---|---|
| Q | TONE SELECT |
| W | EDIT |
| E | SYSTEM |
| R | RHYTHM |
| T | UTILITY |
| P | PREVIEW |
| A | PATCH/PERFORM |
| S | MUTE |
| D | MONITOR |
| F | COMPARE |
| G | ENTER |
| Tab | DATA |
| ← → | CURSOR |
| , . or ↓ ↑ | the VALUE encoder |

NVRAM edits last until the pack is loaded again; saving them to the SD card is not done yet.

## Building and testing

```bash
cmake -S sim -B out/sim -G Ninja -DCMAKE_BUILD_TYPE=Release && ninja -C out/sim
out/sim/jv880_board_bench --roms DIR --pack boot.rom [--smf jv880.mid --out out.raw]
SYNTHROOT=$PWD/../.. ../../scripts/build-hosts/winbuild.sh jv880   # Quartus 17.0 on the Windows build host
```

The MCU and system benches are the SC-55's (`cores/sc55/sim`, `--romset jv880-v1.0.1`).

## Licence

GPL-2.0-or-later, as the SC-55 core and Nuked-SC55. `mister/sys/` is Template_MiSTer (GPL-2.0).
