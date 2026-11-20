# SC-55mk2 (M7): scope

What the reference (`third_party/nuked-sc55/src/backend`) does differently
for `mk2-v1.01` (and `sc155mk2-v1.01`, same ROMs), and what that means for
the HDL. The user's mk2 ROMs are in `firmware/` (git-ignored); the reference
build reproduces jcmoyer's mk2 hashes for avmidi 01 and 0A [OBS].

## Differences from the mk1

| Part | mk1 | mk2 | Source |
|---|---|---|---|
| Step time (renderer) | 600 ns (12 cycles at 20 MHz) | 500 ns (24 MHz) | `renderer/main.cpp` `R_NSPerStep` |
| ROM2 | 256 KB | 512 KB | `rom_io.cpp` |
| Wave ROMs | 3 × 1 MB | 2 MB + 1 MB (bank 0 is 2 MB) | `pcm.cpp` `PCM_ReadROM` |
| MIDI in | MCU SCI | through the sub-MCU (M37450, `rom_sm.bin` 4 KB, 128 B RAM, 192 B shared RAM) | `submcu.cpp`, `mcu.cpp` `MCU_UpdateDevices` |
| Memory map | gate array; PCM at E000; SRAM page 5 | PCM at E000–E3FF, LCD and I/O at E400–E7FF, sub-MCU at EC00–EFFF, SRAM page 10 | `mcu.cpp` `MCU_Read`/`MCU_Write` |
| Timers | mk1 step tables | generic tables | `mcu_timer.cpp` |
| Model pins | — | P9DR bit 1: 1 = SC-55mk2, 0 = SC-155mk2 | `mcu.cpp` |
| A/D | battery, SC-155 sliders | knobs through `io_sd` | `mcu.cpp` `MCU_AnalogReadPin` |
| PCM filter | `multi`/`addclip20` | plain 32-bit products and sums | `pcm.cpp` |
| Output | 64 kHz | 66,207 Hz (`reg_slots` 28 → 725 cycles a frame) | `pcm.cpp` |

The sub-MCU runs one instruction per 48 of its cycles at 5 × the MCU's
cycle count: about 1.25 instructions per MCU step. Its interpreter is
`submcu.cpp`, 1,443 lines (opcode table, BBC/BBS, SEB/CLB, timer, three
UARTs, interrupts).

## Already identical

The H8/532 CPU (`hdl/cpu/h8_cpu.sv`) runs the mk2 firmware unchanged
(`cpu_lockstep --romset mk2-v1.01`, devices from the model):

| Run | Result |
|---|---|
| boot, 20 M steps | identical: 3,080 distinct PCs, 46,323 interrupt entries; 6.4 HDL clocks a step |
| GM reset + random MIDI (through the model's sub-MCU), 15 M steps | identical: 9,896 distinct PCs; 10.4 clocks a step |

Where the board's clocks go (mk1, free run with loud MIDI, `board_lockstep
--free-run`, per step on average; 25.2 in total over 100 M clocks):

| Part | Clocks |
|---|---|
| instruction fetch (`S_FETCH`, 2 clocks a byte) | 7.9 |
| sequencer and CPU idle between steps, timers | 4.0 |
| waiting for the PCM (register access 1.4, IRQ0 check 2.5) | 3.9 |
| the rest of execution (operands, ALU, stack, interrupts) | 9.4 |

Fetching one byte a clock (or two bytes a read) and trimming the step
overhead gives about 20, inside the mk2's 25.

## Consequences for the HDL

1. **ROM2 cannot be block RAM.** The mk1 build uses 453 of 553 M10K blocks
   and the mk1 ROM2 alone takes 256; the mk2 ROM2 needs 512. Decision
   (2026-11-20, with the user): ROM2 in SDRAM (the MiSTer SDRAM board: fixed,
   low latency, unlike DDR3), with a small block-RAM cache in front for
   instruction fetch; ROM1, SRAM and the sub-MCU memories stay in block
   RAM; wave ROMs stay in DDR3. The mk2 needs the SDRAM board.
2. **Step budget.** 500 ns at 50 MHz is 25 clocks a step; the mk1 core
   averages 26.6 (free-run bench, loud MIDI) and must also absorb ROM2
   cache misses. Needed: fewer clocks a step (the CPU's two-clock memory
   accesses, PCM catch-up waits) and/or a faster core clock (at 50 MHz the
   worst slack is +0.84 ns, about 52 MHz).
3. **One RBF or two.** With ROM2 external, one core can carry both models,
   chosen by the ROM pack header (`romset.py pack --romset`); the sub-MCU
   is idle on mk1.

## Order of work

1. Benches take the romset as an argument (mk1 or mk2); the reference
   model already runs both.
2. Sub-MCU CPU (`hdl/submcu/`) in lockstep with `submcu.cpp`, as M1 did for
   the H8 (random programs, firmware, seeded faults).
3. MCU mk2 memory map, timers and pins; PCM mk2 filter, bank 0 at 2 MB,
   28 voices.
4. ROM2 in SDRAM with a fetch cache; the board bench (with an SDRAM model) measures clocks
   per step against 25.
5. System lockstep: avmidi files against jcmoyer's mk2 hashes (the M4
   criterion), then the M5 criteria on the MiSTer.
