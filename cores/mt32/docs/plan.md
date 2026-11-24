# Roland MT-32 for MiSTer: scope and plan

SESSION-PLAN iteration 17, 2026-11-24. The MT-32 (1987) is Roland's LA-synthesis MIDI sound
module: an Intel MCS-96 microcontroller running the control ROM, the LA32 sound chip (32 partials
over a 512 KB PCM ROM), a BOSS reverb chip with its own program ROM, and a 20-character LCD.
It is brief §8's next Phase 1 core after the CZ-101 (user, 2026-11-24: MT-32 ahead of the
MU50 and the DMX). Like the CZ-101, the CPU has no existing HDL. [OBS] marks what was read in
the reference or measured; [INF] marks inferences; [UNK] what is still open.

## Reference

- **Nuked-MT32** (`nukeykt/Nuked-MT32`, GPL-2.0-or-later in every nukeykt file) [OBS]. It is
  low-level, built the same way as Nuked-SC55, which the SC-55 and JV-880 are checked against:
    - the control ROM runs on MAME's MCS-96 core (`mame/`, BSD-3-Clause, Olivier Galibert), with
      the framework removed and the board's hooks added (764 diff lines against MAME's
      `i8x9x.cpp`);
    - the LA32 model (`la32.cpp`, 1,232 lines) comes from John McMaster's decap; the reverb chip
      (`rv2.cpp`, 103 lines) from InfoSecDJ's decap.
- **Nuked-MT32 is the definition**, as Nuked-SC55 is for the SC-55. The HDL matches it bit for
  bit, including its scheduling (below); its error against a real MT-32 is out of scope.
- **Pinned commit:** `5b4e239` (2026-11-21, 13 commits; the repository says "wip") [OBS]. A later
  upstream commit is taken only by re-running the corpus and recording the hash changes.
- **MAME's own `mt32` driver** has no sound (`MACHINE_NOT_WORKING | MACHINE_NO_SOUND`) and no LA32
  device [OBS]. It is used for documentation only. **Munt** reimplements the firmware in C++
  and does not run the control ROM, so it is not a lockstep reference [OBS].
- **Headless build** (scratch check, 2026-11-24) [OBS]:
    - `mt32.cpp la32.cpp rv2.cpp lcd.cpp mame/mcs96.cpp mame/i8x9x.cpp` plus a 30-line
      harness build with Apple clang, after one fix: `mame/emu.h` includes `"..\mt32.h"`
      (a Windows path).
    - 5 s of audio renders in 0.7 s of CPU time. Two runs give the same SHA-1.
    - A note on MIDI channel 2 sounds (peak 3,472 in the 0.5 s after note-on); channel 1 is
      silent, as the MT-32's default parts are on channels 2–10.

## Models

- **Supported by the reference:** the second-generation MT-32 only (README) [OBS]: 128 KB control
  ROM in 16 KB banks (`reg_bank & 7`), firmware 2.0.x.
- **First target:** firmware **2.07** (MAME's newest MT-32 BIOS). 2.04 is a second check.
- **Later, not in this plan:**
    - first-generation MT-32 (1.0.x, 64 KB, two 32 KB EPROMs);
    - the CM-32L and LAPC-I, which need a 1 MB PCM ROM and the `r15179917` reverb ROM.
  The reference supports neither.

## ROMs

The user's two archives (2026-11-24), unpacked to `synth-projects/nuked-sc55/firmware/` (gitignored):
`Roland MT-32 various OS/` and `MT-32 CM-32L MAME-versioned/`. Checked against MAME's
`ROM_START( mt32 )`:

| Nuked-MT32 name | Size | Source file | SHA-1 | Against MAME |
|---|---|---|---|---|
| `mt32_cpu.bin` | 128 KB | `mt32_ctrl_2_07.rom` | `47b52ade…a7c` | = `mt32_2.0.7.ic28` [OBS] |
| (alternative) | 128 KB | `mt32_ctrl_2_04.rom` | `2c16432b…0eb` | = `mt32_2.0.4.ic28` [OBS] |
| (alternative) | 128 KB | `mt32_ctrl_2_06.rom` | `2869cf4c…4ed` | not in MAME |
| `mt32_pcm.bin` | 512 KB | `mt32_pcm.rom` / `r15449121.ic37.bin` | `f6b1eebc…252` | = `r15449121.ic37.bin` [OBS] |
| `mt32_reverb.bin` | 32 KB | `r15179857.ic13.bin` | `c2933cb7…37f` | = MAME's `boss` region [OBS] |

- **Reverb ROM:** Nuked-MT32 issue #1 says it is the `R15179857` chip [OBS].
- **Ready-named set:** `synth-projects/nuked-sc55/firmware/Roland MT-32 nuked/` holds the three
  files above (2.07). Tests find it through `MT32_ROM_DIR`.
- **Not usable as is:** `cm32l_pcm.rom` (SHA-1 `289cc298…`) differs from MAME's `cm32l_pcm.rom`
  (`f2a10225…`) [OBS]. It doesn't matter until the CM-32L.

## Hardware, from Nuked-MT32 [OBS]

| Part | Detail |
|---|---|
| CPU | Intel 8098 (MCS-96), modelled by MAME's `p8098_device`; the board counts 12,000,000 CPU cycles a second (`la32_cycles × 375 / 256`) |
| Memory map (CPU address) | 0x0200 buttons (read, scan lines from A1–A4); 0x0100 bank register (write); 0x0200 reverb ROM bank (write, `(d >> 1) & 3`); 0x0300 LCD data queue; 0x0380 LCD command (write) / busy (read bit 0); 0x0400 reverb `saw1`; 0x0800 reverb `saw0`; 0x0C00–0x0FFF LA32; 0x1000–0x7FFF control ROM (direct); 0x8000–0xBFFF bank window (ROM bank 0–7, or RAM bank 0–1 when bit 4 is set; writable only as RAM); 0xC000–0xFFFF RAM bank 0 |
| RAM | 32 KB, no battery: nothing to save [INF: Nuked-MT32 has no NVRAM] |
| LA32 | 32 partial slots of 8 LA32 cycles, 256 cycles a sample, 32 kHz stereo; register file 6 × 32 × 16 bit plus 4 control bytes; 512-entry `powrom`, `powdrom` and `logsin` tables; two PCM addresses per slot; interrupts to the CPU's EXTINT, status read back at 0x0C00 |
| Reverb chip | Program in the 32 KB ROM (bank from the CPU, plus the `saw` registers); one step per 4 LA32 cycles = 64 steps a sample; 16 K × 16-bit delay RAM; input from LA32 accumulators 6 and 2 (slots 4 and 20) |
| Output | L = LA32 accumulators 2 + 3, R = 6 + 7 (each `output_expand`ed), plus reverb × 17 / 25; times (255 − CPU PWM) / 255 (master volume); clamped to 16 bits |
| MIDI in | Bytes written straight into the 8098's serial buffer (`serial_w`), at least 3,840 CPU cycles (one byte at 31,250 baud) apart, checked after every slot |
| Panel | 10 buttons in two scan groups; volume knob on the A/D (Nuked fixes it at 1020); 20 × 1 LCD through a gate-array queue (`lcd.cpp`: 4 user characters, busy for 100 CPU cycles a write); MAME lists an M5238 LCD controller |

**Scheduling (this is what "lockstep" has to reproduce):** after every LA32 slot (8 LA32
cycles), the reverb chip runs 8 cycles and the CPU runs until its cycle count reaches
`la32_cycles × 375 / 256`; then one MIDI byte may be pushed; at slots 15 and 31 the CPU's
`la32_sh3` input bit changes. A CPU write to the LA32 therefore takes effect at the next slot
boundary in the reference. The HDL board uses the same interleave on purpose, as the SC-55 board
follows Nuked-SC55's step order.

## Layout

- `cores/mt32/`: `cfg/`, `hdl/`, `mister/`, `sim/`, `tests/`, `tools/`, `ref/`.
- `modules/nuked-mt32/`: submodule at `5b4e239`. The reference harness builds from a copy with
  the `emu.h` include fixed, so the submodule stays untouched.
- **New HDL:**
    - `cores/mt32/hdl/mcs96/`: CPU and 8098 peripherals (MIT; written from MAME's BSD-3-Clause
      source; `NOTICE` keeps MAME's notice). It moves to `modules/` when a second MCS-96 Roland
      (D-50, D-10/D-110 family) uses it.
    - `cores/mt32/hdl/la32/` and `hdl/rv2/`: GPL-2.0-or-later, after Nuked-MT32. The LA32 can
      move to `modules/roland` (GPL-2.0) for the D-10/D-110/CM-32L later.
- **Core licence:** GPL-2.0-or-later, as the SC-55.
- **From synthframe:** memory port and DDR3 backend (PCM ROM), `audio_fifo`, `resample`
  (32 → 96 kHz, L = 3, M = 1), `charlcd` and `vtiming` for the LCD, `midi_rx`, the ROM loader.

## Size [INF, to be checked by the first build]

| Part | Estimate |
|---|---|
| Block RAM | control ROM 128 KB + RAM 32 KB + reverb ROM 32 KB + reverb RAM 32 KB = 1.8 Mbit, about 180 M10K, plus LA32 tables (3 × 512 × 32 bit) and register file |
| PCM ROM | 512 KB (4 Mbit) in DDR3: with it in block RAM the total would exceed the DE10-Nano's 553 M10K. At most 64 16-bit reads a sample (32 slots × 2 addresses, `la32.cpp` lines 655–683 [OBS]) = 2.05 M reads/s, on a fixed slot schedule that can be fetched one slot ahead |
| Logic | brief §3: 23–28k ALMs; CPU much smaller than that, the LA32 most of it |
| DSP | brief §3: 36–56; the LA32's `mul` (14 × 8 bit signed, 13 call sites [OBS]) and interpolation, time-multiplexed over the 32 slots |

## Milestones

| # | Milestone | Gate |
|---|---|---|
| M0 | Reference | `modules/nuked-mt32` submodule; `cores/mt32/ref/mt32_ref` (C++, no SDL): ROM dir, MIDI input as a standard MIDI file or a byte schedule, raw 32 kHz stereo out; optional logs: CPU per instruction (PC, registers, PSW, cycle count), every LA32 and reverb-register write with its LA32 cycle, reverb input/output samples, LCD text. Log hooks live in `ref/patches/` and must not change the audio. MIDI corpus (`tests/midi/`): boot only; one note per part; rhythm part; program and SysEx patch edits; each reverb mode; a long multi-part piece. Two runs give the same hashes; hashes recorded in `tests/corpus.json` |
| M1 | MCS-96 instruction set | Decoder generated from `mcs96ops.lst` (206 base opcodes, 26 with the FE prefix [OBS]); generated test ROMs covering every opcode; trace identical to the reference CPU: registers, PSW, memory writes, cycle counts |
| M2 | 8098 peripherals and board | Nuked-MT32's `i8x9x`: timers, HSI/HSO, serial port (byte injection), A/D (knob), PWM (master volume), EXTINT, port bit `la32_sh3`; bank register, memory map, buttons, LCD queue and busy flag. Firmware from power-on: CPU trace identical through boot and the first LCD text |
| M3 | LA32 | Fed the reference's LA32 write log (with LA32 cycles), the HDL gives the same accumulator outputs and the same interrupts on every corpus case; tables compared word for word |
| M4 | Reverb chip | Fed the reference's reverb input and register log, the same reverb samples on every corpus case and in each reverb mode |
| M5 | System bench | CPU + LA32 + reverb + LCD + MIDI from power-on, PCM ROM behind a random-latency memory model: every corpus hash identical; LCD text identical |
| M6 | MiSTer | Windows build meets timing; on the MiSTer: boots to the firmware's start-up text, plays MIDI from MidiLink with no underrun, panel buttons and volume work; smoke test `scripts/mister/smoke.sh` |

M0 and M1 need no hardware and can run alongside the P2K work (iteration 16). M3 and M4 depend
only on M0's logs, so they can run in parallel with M1–M2.

## Status (2026-11-24)

| # | Result |
|---|---|
| M0 | Done. `ref/build.sh` (hooks: `ref/prepare_src.py`, audio unchanged), corpus of 6 cases / 65 s (`tests/midi/`, `tools/midi_corpus.py`), hashes in `tests/corpus.json` |
| M1 | Done. `hdl/mcs96/` (decode ROM generated from MAME's compiled `mcs96.hxx`, not the stale `mcs96ops.lst`: they differ in norml, mulub indexed_3e and the div/divu dividend). Random programs (`tools/mcs96_randprog.py`, `tools/c1_run.sh`): 60+ seeds identical, half with SFRs and interrupts; `tests/test_cpu.py` in CI without ROMs |
| M2 | Done. Co-simulation (`sim/cpu/`: the HDL CPU inside Nuked-MT32's board): boot trace and all 6 corpus cases identical |
| M3, M4 | Done (`docs/sound.md`): every out-log field, LA32 interrupt and slot state identical on all 6 cases |
| M5 | Done. `sim/system/` (whole HDL, PCM latency 1-24 random): all 6 corpus hashes identical; 2,658-2,828 mean clocks a sample (budget 3,000 at 96 MHz) |
| M6 | Windows build: 18,683 ALMs (45 %), 62 DSP, 40 % block memory; core clock 96 MHz, setup slack +0.263 ns, hold met. MiSTer smoke (`scripts/mister/smoke.sh`): boots to "1 2 3 4 5 R vol:100", MIDI over the UART changes the LCD ("vol: 60" after master volume 60, as Nuked-MT32 shows), no underrun. Not checked: audio by ear; panel buttons (the G key changed nothing on the LCD; the reference's response to it is not checked) |

Timing rules the HDL keeps (from the reference, confirmed by the lockstep):
- `mcs96_device::clock()` slices of 4-33 cycles; the overshoot of a slice is dropped;
  `total_cycles()` between runs is `totalcycles + step - icount` of the last slice.
- `internal_update()` at every slice start and at every `bcount` event; HSO/A-D writes call it too.
- Interrupt entry takes no cycles; at `fetch` every pending level is cleared (the `.hxx`).
- MIDI bytes go straight into SBUF after a slot's CPU turn, 3,840 CPU cycles apart.

On the MiSTer the core's UART must be in MIDI mode once (`config/uartmode.MT-32`).

## Shared resources during iteration 16

- **This Mac:** the P2K corpus gate is using the CPUs (load average 11–14 on 9 cores,
  2026-11-24). The MT-32 benches run with fewer jobs until that finishes.
- **winbuild:** each build gets its own folder (`C:\synthbuild\<id>`), so two builds can run at
  once, more slowly.
- **MiSTer:** through `synth-mister`'s lock only (M6).

## Open questions

- **MIDI in on the MiSTer:** the reference writes whole bytes into the serial buffer. The core
  does the same (bench and MiSTer), not bit-level RXD, so the firmware sees the same timing [INF].
- **MIDI out / thru:** whether the reference models the 8098's serial transmit for MIDI out
  [UNK]; not needed for sound.
- **Knob:** the reference fixes the A/D at 1020. On the MiSTer it becomes an OSD option or a
  joystick axis; the corpus keeps 1020.
- **Reference changes:** upstream is active (reverb added 2026-11-15, ring-modulation fix
  2026-11-11, `FIXME` flags `w49`/`w50` in `la32.cpp`). Each update is a deliberate submodule
  bump with recorded hash changes.
- **Start-up text** of firmware 2.07 [UNK until M0's LCD log].
