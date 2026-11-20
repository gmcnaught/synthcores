# SC-55 for MiSTer

A MiSTer FPGA core of the Roland SC-55 Sound Canvas (mk1), the SC-55mk2,
and the SC-155 / SC-155mk2, checked sample for sample against the
[Nuked-SC55](https://github.com/nukeykt/Nuked-SC55) emulator
([jcmoyer's fork](https://github.com/jcmoyer/Nuked-SC55) as the reference
model, `third_party/nuked-sc55`). No ROMs are included: you make a ROM pack
from your own dumps.

## Status (November 2026)

| Part | Status | Docs |
|---|---|---|
| H8/532 CPU | identical to the reference instruction by instruction (firmware, MIDI traffic, random code; 18/18 seeded faults caught) | [`docs/cpu.md`](docs/cpu.md) |
| MCU peripherals (timers, SCI, A/D, gate array, memory map; mk1 and mk2) | identical step by step | [`docs/mcu.md`](docs/mcu.md) |
| mk2 sub-MCU (M37450) | identical instruction by instruction (firmware, random code; 16/16 seeded faults) | [`docs/mk2.md`](docs/mk2.md) |
| PCM chip (voices, reverb/chorus; mk1 and mk2) | identical sample for sample | [`docs/pcm.md`](docs/pcm.md) |
| Whole core | avmidi `0A.mid` rendered by the HDL = the reference renderer (mk1) and jcmoyer's own integration hash (mk2) | [`docs/board.md`](docs/board.md), [`docs/mk2.md`](docs/mk2.md) |
| MiSTer board (ROM pack, DDR3 wave ROMs, pacing, MIDI, audio) | identical in simulation from power-on | [`docs/board.md`](docs/board.md) |
| LCD and panel keys | LCD identical pixel for pixel to the reference's renderer | [`docs/lcd.md`](docs/lcd.md) |
| MiSTer hardware | mk1: plays MIDI in real time (100 s, no underrun or lag); mk2: boots and plays, real-time run pending | [`docs/board.md`](docs/board.md) |

## Using the core

1. Make the ROM pack from your own ROM dumps (tested: mk1 v1.21, SC-155 rev1,
   SC-55mk2 v1.01; other mk1 and SC-155mk2 sets are packed the same way):
   `python3 tools/romset.py pack DIR boot.rom --romset mk1-v1.21`
   (`python3 tools/romset.py check DIR` lists what DIR holds; the files are
   checked by SHA-256 against the reference's table, the wave ROMs are
   unscrambled). The pack's header tells the core which model it is.
2. Copy `boot.rom` to `games/SC-55/` on the MiSTer's SD card (it loads at
   core start; OSD "Load ROM pack" loads another).
3. Copy the newest core from [`releases/`](releases/) (`SC55_YYYYMMDD.rbf`)
   to `_Other/` (or install it with the MiSTer downloader from this
   repository).
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
