# SC-55 for MiSTer

A MiSTer FPGA core of the Roland SC-55 Sound Canvas (mk1), checked sample
for sample against the [Nuked-SC55](https://github.com/nukeykt/Nuked-SC55)
emulator ([jcmoyer's fork](https://github.com/jcmoyer/Nuked-SC55) as the
reference model, `third_party/nuked-sc55`).

## Status (November 2026)

| Part | Status | Docs |
|---|---|---|
| H8/532 CPU | identical to the reference instruction by instruction (firmware, MIDI traffic, random code; 18/18 seeded faults caught) | [`docs/cpu.md`](docs/cpu.md) |
| MCU peripherals (timers, SCI, A/D, gate array, memory map) | identical step by step | [`docs/mcu.md`](docs/mcu.md) |
| PCM chip (voices, reverb/chorus) | identical sample for sample | [`docs/pcm.md`](docs/pcm.md) |
| Whole core | avmidi `0A.mid` rendered by the HDL gives the same SHA-256 as the reference renderer | [`docs/board.md`](docs/board.md) |
| MiSTer board (ROM pack, DDR3 wave ROMs, pacing, MIDI, audio) | identical in simulation from power-on | [`docs/board.md`](docs/board.md) |
| LCD and panel keys | LCD identical pixel for pixel to the reference's renderer | [`docs/lcd.md`](docs/lcd.md) |
| Quartus build | fits (67 % of ALMs); 50 MHz core and 40 MHz video meet timing | [`docs/plan.md`](docs/plan.md) |
| MiSTer hardware | not yet run | [`docs/plan.md`](docs/plan.md) |

Not yet: the SC-55mk2 (sub-MCU).

## Using the core

1. Make the ROM pack from your own SC-55 (mk1) v1.21 ROM dumps:
   `python3 tools/romset.py pack DIR boot.rom` (it checks the files'
   SHA-256 against the reference's table and unscrambles the wave ROMs).
2. Copy `boot.rom` to `games/SC-55/` on the MiSTer's SD card (it loads at
   core start; OSD "Load ROM pack" loads another).
3. Copy the core (`SC55_YYYYMMDD.rbf`) to `_Other/` or `_Utility/`.
4. Set the UART to MidiLink (MiSTer menu) and send MIDI.

Panel keys (PS/2 or USB keyboard): Q power, W all, E mute, R/T part,
Y/U instrument, I/O key shift, P/[ level, A/S MIDI channel, D/F pan,
G/H reverb, J/K chorus, arrows part.

## Building and testing

```bash
cmake -S sim -B out/sim -G Ninja -DCMAKE_BUILD_TYPE=Release && ninja -C out/sim
for t in tests/test_*.py; do python3 $t; done      # ROM tests need firmware/ (or SC55_ROM_DIR)
mister/build.sh                                    # Quartus Lite 17.0; or the "Build RBF" workflow
```

## ROMs

You need your own SC-55 ROM dumps; none are included or will be. Put them
in `firmware/` (ignored by git).

## Licence

GPL-2.0-or-later, as Nuked-SC55 ([`LICENSE`](LICENSE)). `mister/sys/` is
Template_MiSTer (GPL-2.0). Some board modules come from the author's
synclavierii project (MIT).
