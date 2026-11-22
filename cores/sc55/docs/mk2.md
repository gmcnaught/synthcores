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

The H8/532 CPU (`modules/roland/hdl/cpu/h8_cpu.sv`) runs the mk2 firmware unchanged
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

## How the HDL does it

1. **ROM2.** The mk1 build uses 453 of 553 M10K blocks and the mk1 ROM2
   alone takes 256; the mk2 ROM2 needs 512. The first plan was ROM2 in
   SDRAM; measured first (`sim/tools/rom2_stats.cpp`, model only): the mk2
   firmware reads ROM2 0.45 times a step (at most 8 in a step) and never
   its upper 256 KB in 30 M steps of loud or random MIDI (the upper half
   holds data, 0x40000-0x5ffff). So the first 256 KB stay in block RAM as
   on the mk1 and the upper 256 KB are in DDR3 (+4 MB after the wave ROMs)
   behind `hdl/board/rom2x_cache.sv` (2 KB direct-mapped, 8-byte lines,
   `wave_ddr` line port). No SDRAM board is needed.
2. **Step budget.** 500 ns at 50 MHz is 25 clocks a step. The sub-MCU
   runs each step's SM_Update alongside the next step (a live
   `5 * cycles` target, published at the device phase with the
   receive-buffer bytes it may take); the MCU waits for it only at the
   EC00 window, E402, RDR reads, SSR writes, and at the interrupt check
   while GA line 5 could still raise IRQ1. Its timer ticks, UART update
   and cycle add take one clock. mk2 system bench, loud MIDI: 31.2 ->
   20.4 HDL clocks a step.
3. **One RBF.** The pack's model byte (`romset.py pack --romset`, pack v2)
   selects mk1, SC-155, mk2 or SC-155mk2 at load; the sub-MCU is idle on
   the mk1.
4. **Output rate.** With oversampling the mk2 makes 48 MHz / 725 =
   66,207 Hz; 48 kHz is exactly 29/40 of it: `resample.sv` has both
   designs (L 3, M 4 and L 29, M 40), chosen by the model.

## Verification

| Bench (mk2-v1.01) | Result |
|---|---|
| `submcu_lockstep`: boot; MIDI + buttons 15 M steps | identical (18.6 M sub-MCU instructions, 14,848 MIDI bytes) |
| `submcu_lockstep --random`: random code, reseeded registers | identical; 253-256 of 256 opcode values executed |
| `mutate.py ROMS submcu` | 16/16 seeded faults caught |
| `mcu_lockstep`: CPU + devices + sub-MCU, MIDI + buttons 15 M steps | identical (also sc155-rev1 and mk1-v1.21) |
| `pcm_lockstep`: loud MIDI 12 M steps | identical, 397,260 samples |
| `system_lockstep --smf avmidi/0A.mid` | SHA-256 222e0892...0f59 = jcmoyer's mk2 integration hash |
| `board_lockstep`: power-on, pack v2 over ioctl, DDR3 model | identical; LCD frame identical |

Found on the way: a single A/D conversion (the mk1 firmware only scans)
wrote its result to dev[0x36] instead of 0x60 + 2 * channel.

## On the MiSTer

Quartus: 79 % of the ALMs, all clocks meet timing (seed 4). The first mk2
run booted and played but fell behind real time under heavy polyphony
(26-30 HDL clocks a step against 25; the paced board bench showed it). After
the CPU/MCU speed work (bus requests issued at once, read bursts streamed,
one-clock step handshake, three timer ticks a pass) and 1 ms of pacer
credit: boot, GM reset and 100 s of avmidi 0A.mid with the underrun and lag
indicators both off.
