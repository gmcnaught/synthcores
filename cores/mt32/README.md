# Roland MT-32 for MiSTer

An FPGA core of the second-generation Roland MT-32 LA synthesis module. It runs the original
control ROM (firmware 2.0x) on an Intel 8098 (MCS-96), with the LA32 sound chip, the BOSS reverb
chip and the 20-character LCD. Every part is checked against
[Nuked-MT32](https://github.com/nukeykt/Nuked-MT32) (`modules/nuked-mt32`, commit `5b4e239`) in
lockstep: instruction for instruction, register write for register write and sample for sample.
No ROMs are included.

## Status

| Part | Check | Result |
|---|---|---|
| Reference (M0) | `ref/build.sh`, `tools/corpus.py`: headless Nuked-MT32 with log hooks, six MIDI cases (65 s) | hashes in `tests/corpus.json`, the same on every run |
| 8098 instruction set (M1) | `tools/c1_run.sh`: random programs over every opcode form, trace against the reference CPU | 60+ seeds identical (plain and with SFRs and interrupts); `tests/test_cpu.py` runs in CI without ROMs |
| 8098 peripherals (M2) | co-simulation: the HDL CPU inside Nuked-MT32's board, LA32 and reverb (`sim/cpu/`) | boot trace and all six corpus cases identical |
| LA32, reverb chip, mix (M3, M4) | replay of the reference's logs (`sim/sound/`, `tests/sound_check.sh`) | all six cases identical, every output field, interrupt and slot state; design note `docs/sound.md` |
| System (M5) | `sim/system/`: the whole HDL from power-on, PCM ROM behind a random-latency memory model | all six corpus hashes identical; CPU trace identical |
| MiSTer (M6) | Windows build; `scripts/mister/smoke.sh` | 96 MHz: +0.263 ns (Windows), +0.570 ns (CI container); boots; MIDI and the Part 1 key change the LCD as in Nuked-MT32; no underrun on the dense corpus case or a game soundtrack; audio checked by ear (2026-11-24) |

The plan, the hardware map and the timing rules are in [`docs/plan.md`](docs/plan.md).

## Using the core

1. **Make the ROM pack** from the second-generation MT-32 ROMs (control ROM 2.04 or 2.07, 128 KB;
   PCM ROM R15449121, 512 KB; reverb ROM R15179857, 32 KB):
   `python3 cores/mt32/tools/mkpack.py CONTROL REVERB PCM MT-32.rom`
   (or `--dir DIR OUT` with Nuked-MT32's file names). It warns when a file is not MAME's.
2. Copy the pack to `games/MT-32/` and load it once from the OSD (`Load ROM pack`; remembered).
3. Set the UART to MidiLink. Parts 1-8 listen on MIDI channels 2-9, rhythm on 10.
4. Panel: Q W E R T = Part 1, 2, 3, Sound group, Volume; A S D F G = Part 4, 5, Rhythm, Sound,
   Master volume; - and = turn the volume knob.

## Licence

GPL-2.0-or-later, as Nuked-MT32; the 8098 HDL in `hdl/mcs96/` is MIT (from MAME's BSD-3-Clause
mcs96). See `NOTICE`.
