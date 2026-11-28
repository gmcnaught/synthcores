# Yamaha MU50 for MiSTer: scope and plan

SESSION-PLAN iteration 22, 2026-11-27. The MU50 (1995) is a half-rack General MIDI / GS / XG tone
module with 16 parts and 32-note polyphony. Its parts are a Hitachi H8/3003 running a 512 KB program
ROM, a Yamaha SWP00 sound chip (AWM2 sample player and MEG effects DSP) reading a 4 MB wave ROM,
64 KB of battery-backed RAM, and a 4×20 character LCD with LEDs. It is brief §8's Phase 1 core after
the MT-32. [OBS] marks what was read in the reference or measured; [INF] marks inferences; [UNK]
what is still open.

## Reference

- **MAME** `src/mame/yamaha/ymmu50.cpp` (R. Belmont, Olivier Galibert),
  `src/devices/cpu/h8/` (Olivier Galibert), `src/devices/sound/swp00.cpp` (Olivier Galibert) and
  `src/mame/yamaha/mulcd.cpp`. All are BSD-3-Clause [OBS]. MAME marks `mu50` working (flags 0)
  [OBS, `CONS( 1995, mu50, ...)`].
- **MAME is the definition**, as for the CZ-101 and the P2K: the SWP00 is a reimplementation.
  MAME's comments say the MEG programs are internal, not dumpable, and were reimplemented
  [OBS, swp00.cpp header]. The HDL matches MAME's integer behaviour; MAME's error against a real
  MU50 is out of scope.
- **Integer model** [OBS]: `swp00.cpp` (2,398 lines) uses `double` only in two debug helpers
  (`sx7`, `sx9`); the sound path is integer. A bit-exact output gate is possible.
- **MAME build:** a `../mame-mu50` checkout of upstream master (2026-11-27 or later) with this
  core's logging patches (`cores/mu50/mame/patches/`), building the `mu50` subtarget. The other
  MAME checkouts are not touched.
- **No usable H8/300H HDL** (checked 2026-11-27) [OBS]: the only open core found
  (sugawara-systems.com, 2004) has no licence, implements an instruction subset, is deliberately
  not cycle-compatible and has no on-chip peripherals. The H8/532 in `modules/roland` is the
  H8/500 family, a different instruction set.

## ROMs

MAME's set `mu50` [OBS, `ROM_START( mu50 )`]:

| File | Size | Use |
|---|---|---|
| `xr174c0.ic7` (v1.05, 1995-08-21), `yamaha_mu50.bin` (v1.04) or `xq332e0.bin` (v1.02) | 512 KB | program ROM, 000000–07FFFF, 16-bit, word-swapped |
| `xq057c0.ic18`, `xq058c0.ic19` | 2 MB each | SWP00 wave ROM, 4 MB |
| MULCD `hd44780u_b04.bin` | 4 KB | LCD character ROM (part of MAME's `mulcd` device) |

- **The user's set** (2026-11-27): `synth-projects/nuked-sc55/firmware/mu50-mame/` (gitignored),
  a MAME rompath with `mu50/` and `mulcd/`. All seven files match MAME's SHA-1s: v1.05, v1.04
  (from the separate `YAMAHA MU-50 (ver.1.04).BIN`; the copy in the uploaded `mu50.zip` was
  truncated), v1.02, both wave ROMs, and the mulcd character ROM and artwork. Tests find the set
  through `MU50_ROM_DIR`.
- The wave ROM is identical to the DB50XG's and the MU10's [OBS, ROM hashes], so the same core
  can later run their program ROMs (see "Variants").

## Hardware, from the MAME driver [OBS]

| Part | Detail |
|---|---|
| CPU | H8/3003 at 10 MHz, one state per clock; mode pins give a 24-bit address space |
| Memory map | 000000–07FFFF program ROM; 200000–20FFFF 64 KB work RAM (the NVRAM); 400000–4007FF SWP00 |
| Port 6 | bit 0 LCD E (inverted), bit 1 LCD R/W, bit 2 LCD RS |
| Port 9 | bits 4–5 LCD contrast |
| Port A | LCD data bus |
| Port B | bit 1 contrast, bits 2–6 the five LEDs (active low) |
| Port C | bits 0, 1, 7 select the three key rows (active low); the rows read back on bits 2–6 |
| A/D | AN0 / AN2 analog input level, AN4 host switch (000 = MIDI), AN6 battery (200) |
| Serial | SCI1 is MIDI in and MIDI out; the host-computer port (TO HOST) is not emulated |
| Interrupts | none from the SWP00; the firmware's timing comes from the H8's own timers [INF] |
| Sound | SWP00 at 33.8688 MHz, 768 clocks a sample pair = 44,100 Hz stereo, 32 voices |
| Effects | MEG: 192-step fixed program, delay lines of 128 K (reverb), 32 K (chorus) and 128 K (variation) words |
| LCD | HD44780 with ROM B04, 4×20 characters, plus the five LEDs and the contrast |

## How MAME runs the CPU [OBS, `h8.cpp`, `h8.lst`]

- The instruction handlers are generated from `h8.lst` by `h8make.py` (2,932 lines of
  description). Each handler is a sequence of bus accesses and internal steps.
- Every bus access costs 2 states on the H8/300H, with no wait states and no 8-bit/16-bit area
  difference (`read16i`, `read8`, `write16` …: `m_icount -= 2`). Internal operations cost
  `cycles + 1` (`internal()`).
- An instruction can stop between accesses (`m_inst_substate`, `do_exec_partial`), and the
  peripherals are brought up to date at the access where their next event falls (`m_bcount`,
  `internal_update`). So peripheral events land at bus-access granularity, not at instruction
  boundaries.
- Interrupts, DMA and DTC requests are taken at `prefetch_done`.

The HDL follows the same model: it performs the instruction's bus accesses in MAME's order, each
taking 2 states of a 10 MHz clock enable, and the internal steps in MAME's state counts. The
access sequences and counts are generated from `h8.lst`, so timing matches by construction, as the
CZ-101's tables were generated from MAME's.

## Sound timing [OBS, U0 and U3]

- The SWP00 stream runs at 44,100 Hz, sample k at time k / 44,100 s. A CPU access to the SWP00
  at CPU state s (10 MHz, counted after the access) acts after
  **n = floor(s × 441 / 100000) + 1** samples: every sample whose time is at or before the
  access is made first. Checked on all 39,507 writes of the corpus, including writes exactly on
  a sample time (s a multiple of 100,000).
- Every SWP00 handler in MAME runs `m_stream->update()` first, except `waverom_val_r` (register
  003) and `meg_control_w` (004). Stock MAME applies those two at the last stream update, which
  depends on MAME's scheduling. Patch 0002 adds the update to both, so every access acts at its
  CPU time; the core does the same. The v1.05 firmware reads only 001 (`state_r`) in the corpus.

## Layout

- `cores/mu50/`: `hdl/` (board), `mister/`, `sim/`, `tests/`, `tools/`, `mame/patches/`, `ref/`.
- New CPU and sound HDL:
    - `cores/mu50/hdl/h8h/` (H8/300H CPU and the H8/3003 peripherals: interrupt controller,
      16-bit timers, SCI, A/D, ports, DMA, watchdog, refresh controller);
    - `cores/mu50/hdl/swp00/` (AWM2 and MEG).
  The DB50XG, DB60XG, MU10 and QY70 also use an H8/3002 or H8/3003, so `h8h/` moves to
  `modules/` once a second core needs it.
- From synthframe: `midi_rx`, `midi_tx`, `audio_fifo`, `nvsave` (the 64 KB RAM to the SD card),
  `mem_ddr3` / `mem_arb` (wave ROM, program ROM and the effects delay lines),
  `synthframe_hd44780` with the B04 character ROM, `synthframe_charlcd`, `synthframe_vtiming`
  and `synthframe_panel_art` for the screen. The front panel under the LCD is MAME's CC0
  `mu50.lay` (`cfg/panel.yaml`, `docs/panel.png`): the five lit buttons (PLAY, EDIT, UTIL,
  EFFECT, MODE) show the firmware's LEDs, the other nine show pressed, each with its PS/2 key.

## Size [INF, to be checked by the first build]

| Part | Estimate |
|---|---|
| External memory | 4 MB wave ROM, 512 KB program ROM, 288 K words of effects delay lines. The delay lines (about 1.1 MB at 32 bits a word in MAME) do not fit in block RAM |
| Block RAM | 64 KB work RAM (about 52 M10K), on-chip RAM 512 B, SWP00 voice state (32 voices × 5 blocks), MEG constants and offsets, a program-ROM cache, the LCD |
| Memory bandwidth | per sample: about 64 MEG delay-line accesses and 2 wave-ROM bytes per voice for 32 voices, at 44.1 kHz; about 5.6 M accesses a second, plus CPU instruction fetches that miss the cache |
| Logic | H8/300H core plus peripherals and the SWP00; to be measured. The CZ-101's uPD7811 + uPD933 core fitted in 14,577 ALMs |

## Milestones

| # | Milestone | Gate |
|---|---|---|
| U0 | Reference | Patched MAME logs, per instruction, PC, opcode, registers, CCR and state count; every bus access with its state; every SWP00 register write with its state count; the raw 44.1 kHz stereo samples. A test-ROM mode runs a generated program on the same machine. With the ROMs: a MIDI corpus rendered to hashes, the same on two runs |
| U1 | H8/300H instruction set | Generated test ROMs covering every opcode in `h8.lst` for the H8/300H (all addressing modes, byte/word/long, MULXS/DIVXS, EEPMOV, TRAPA, SLEEP) give a trace identical to MAME's: registers, CCR, memory writes, state counts |
| U2 | H8/3003 peripherals | Interrupt controller, 16-bit timers, SCI (MIDI in and out), A/D, ports, DMA, watchdog, refresh. Random programs exercising them identical to MAME; then the firmware from power-on identical through boot and the first screen |
| U3 | SWP00 | Fed MAME's register-write log (with times) and the wave ROM, the HDL gives the same samples: AWM2 (streaming, envelopes, filter, LFO, mixer) first, then the MEG. Synthetic write streams first, so U3 does not wait for the ROMs |
| U4 | System bench | CPU + SWP00 + LCD + keys + MIDI from power-on: every corpus hash identical to MAME's; LCD text and LEDs identical |
| U5 | MiSTer | Windows build meets timing; on the MiSTer: boots to the play screen, plays MIDI from MidiLink with no underrun, panel keys work, the RAM survives a reload, MIDI out matches MAME's |

U1 and U3 run in parallel: they share nothing but the bench.

## Status (2026-11-27)

| # | Result |
|---|---|
| U0 | Done: `../mame-mu50` (upstream `db140b2d`) with `mame/patches/0001-mu50-logging.patch`; `tools/mame-mu50.sh`; test-ROM mode (`tools/mu50_testrom.py`); `tests/u0_check.sh` passes. Corpus: `tests/u0_corpus.sh` renders `tests/midi/basic.txt` (18 s) identically twice and matches `tests/midi/expected.txt`. More corpus cases come with U4 |
| U1 | Done: `hdl/h8h/h8h_core.sv` with `h8h_gen.svh` / `h8h_gen_states.svh` generated by `tools/h8h_gen.py` from MAME's `h8.lst` and `h8.cpp` (264 opcodes, 15 dispatch steps, reset and the interrupt entry: 881 states, 43 ALU helpers). `tests/u1_check.sh`: 200 random programs of 2,000 instructions (`tools/h8h_randprog.py`, seeds 100-299, 1.5 M instructions) give traces identical to MAME's: registers, CCR, every write, state counts. 263 of 264 handlers run; SLEEP needs interrupts and moves to U2. CI (`tests/ci.sh`) replays four committed programs against MAME's trace hashes. Quartus 17, the CPU alone on the DE10-Nano's 5CSEBA6: 5,108 ALMs, 888 registers, 6 DSP blocks, +5.3 ns slack at 50 MHz |
| U2 | Done: `hdl/h8h/h83003_periph.sv` (MAME's interrupt controller, 16-bit timer, SCI, A/D, ports, SYSCR, watchdog; lazy event model in virtual time), `hdl/h8h/h8h_sys.sv` (core + peripherals + on-chip RAM, execute_run's sequencing). `tests/u2_check.sh`: the v1.05 firmware on `sim/mu50sys_bench.cpp` (software board) is identical to MAME for 12 s of boot and `basic.txt` MIDI: 27,789,525 instruction lines, 7,265 interrupts, 240 MIDI pin changes. CI: a peripheral program (`tools/u2_periph_prog.py`: timer interrupts, SLEEP, A/D scan) against MAME's trace hash. Quartus 17, CPU + peripherals alone: 12,456 ALMs, 4,023 registers (27,252 before the timer datapath was shared and the event minimum pipelined); timing at 50 MHz is U5 work. |
| U3 | Done: `hdl/swp00/` (`swp00.sv` voices and registers, `swp00_meg.sv` MEG executor, `swp00_pkg.sv`), the MEG program compiled by `tools/swp00_meg_gen.py` from MAME's `swp00.cpp`. Reference `ref/swp00_ref` = MAME's code as a C++ class (`tools/swp00_ref_extract.py`), built with wrapping arithmetic. Gate: corpus `basic` (794,099 sample pairs, 1,711 reads) HDL output byte-identical to MAME's; four random SWP00 stimuli, one per MEG variant (`tools/swp00_stimgen.py`, seeds 2-5, 44,101 samples each, about 21,000 writes and 550 reads each) identical to MAME with every channel record and MEG register checked after every sample (`tests/u3_run.sh`, `VCHECK=1`); one 'wild' stimulus (seed 1, extreme constants) identical to the wrapping reference. CI replays committed access lists (160 KB) against MAME's sample hashes. Quartus 17, the SWP00 alone on 5CSEBA6: 5,066 ALMs, 3,148 registers, 20 M10K, 14 DSP blocks, +3.3 ns slack at 50 MHz |
| U4 | Done: `hdl/mu50_board.sv` (h8h_sys + SWP00 + 64 KB RAM + `hdl/mu50_lcd.sv` + keys, LEDs, A/D constants, IRQ0) on `sim/mu50_bench.cpp` (program ROM, wave ROM and delay lines in C++ arrays): the corpus case `basic` from power-on gives a sample file identical to MAME's, 794,099 pairs (`f19a8d2d159a7fb8`, `tests/midi/expected.txt`); the LCD at the end reads `GrandPno01` / `000.001 01`. `tests/u4_check.sh`. 2.66 FPGA clocks a CPU state with 1-clock memories. Speed: 5.49 FPGA clocks a CPU state with 1-clock memories (9.1 before the bus fast path); real time at 50 MHz needs 5 (U5). |
| U5 | Core done, hardware test partial: `mister/MU50.sv` + `hdl/mu50_top.sv` (DDR3 memories with a 32 KB program ROM cache, pacing, MIDI from MidiLink, 44.1 kHz audio FIFO, 64 KB NVRAM save, PS/2 panel keys, LCD video). Windows build (Quartus 17, seed 1): 26,806 ALMs, 185 RAM blocks, 59 DSP; core clock +1.241 ns at 50 MHz, HDMI +0.560 ns. `sim/mu50_top_bench.cpp` (DDR3 model, 12-clock latency): bus hash over 4 s equals the board bench's; the 18 s corpus through the UART with pacing on: 0 underruns, 22.7% of clocks waiting for real time (rerun after fixing the bench's UART stop bits, which had garbled the MIDI in of the first measurement; all 92 bytes now reach RDR1). MIDI out: SCI1's transmit line is read back as bytes in virtual time and sent by `synthframe_midi_tx` at 31,250 baud; the corpus case `dump` (XG bulk dump requests) gives MAME's 70 bytes on the board bench, the wrapper bench and the MiSTer. `scripts/mister/smoke.sh` on the MiSTer: PASS (running, no underrun, LCD lit, MIDI and the Util key change the LCD, a 64 KB save file). MIDI out build (seed 1): 26,967 ALMs, core +1.093 ns, HDMI +0.473 ns; smoke PASS with MIDI out equal to MAME's. Not yet checked on hardware: the NVRAM restore after a reload, listening |

U0 findings [OBS]:
- **Program ROM file order:** MAME's `ROM_LOAD16_WORD_SWAP` wants each byte pair of the CPU
  image swapped in the file. A reset vector of 00000100 written unswapped was read as 00000001.
- **Reset:** the CPU reads the vector as two words (000000, 000002) and runs from its low 24
  bits; the first instruction starts at state 6.
- **State counts** (t0): MOV.W #imm,Rd 4; MOV.L #imm,ERd 6; MOV.W Rs,@ERd 4 (the write lands at
  the end, state 20 for an instruction starting at 16); BRA 4. Each is 2 states per bus access.
- **SWP00 write timing:** a CPU write at state s (counted after the access) is applied after
  floor(s × 44,100 / 10⁷) + 1 samples: state 18 after 1 sample, state 393,236 (39.32 ms) after
  1,735. One second makes 44,101 samples. The exact rule at sample boundaries (MAME's
  attosecond periods) is pinned in U3.
- **Determinism:** two runs give identical traces, write logs and samples.
- **What the firmware uses** (v1.05, 18 s: boot plus `basic.txt`): interrupts IRQ0 (vector 12,
  33 times; MAME holds the pin low from reset), ITU1 IMIA (28, 15,691: the main tick), ITU4
  IMIA/IMIB/OVI (40-42: 420, 696, 201), SCI1 RXI (57, 92 at level 1: MIDI in). No DMA and no
  A/D interrupt; the A/D is started by the CPU and polled. SCI0 and SCI1 are set up; the
  watchdog and refresh registers are written only at init. Every init register is written twice
  [UNK why]. So U2 needs the interrupt controller, the five 16-bit timer channels, both SCIs,
  the A/D and the ports; DMA can wait.
- **Sound:** silent until MIDI arrives; the basic case makes 39,507 SWP00 writes in 18 s.

U1 design [OBS]:
- **Generated, not hand-written.** `tools/h8h_gen.py` imports MAME's own `h8make.py` to read
  `h8.lst`, expands the macros, and compiles each handler's C statements to SystemVerilog. A small
  C front end keeps C's types: every value is a 32-bit vector equal to the C value after integer
  promotion; narrowing, signed compares, signed shifts and divisions are written out. The ALU
  helpers (`do_add8`, `do_shal32`, `set_nzv16` ...) are compiled the same way from `h8.cpp` into
  functions that take and return the CCR. The decoder mirrors `h8make.py`'s `save_exec` switch.
  An unknown statement stops the generator.
- **Timing by construction.** A handler is split into states at each bus access and
  `internal(n)`; each is an operation of MAME's cost (2 states, n+1). An operation starts when
  the previous one's states have elapsed (`ce` counts states); everything else takes FPGA clocks
  only. `vtime` is MAME's `total_cycles()`.
- **Shared units.** The first build compiled every statement in place: 18,325 ALMs and about
  10 MHz (combinational 32-bit dividers, a copy of each helper at every call site, a write mux
  into the register file from every state). Now a helper call ends a state and runs on one shared
  ALU (one instance of each of the 43 helpers), `/` and `%` run on a one-bit-per-clock divider,
  and the register file has one write port, with a state split before any register access that
  follows a write. These cost FPGA clocks only. 5,108 ALMs, +5.3 ns at 50 MHz.
- **Firmware without peripherals:** v1.05 runs 212,098 instructions (106 ms of boot) identically
  to MAME, up to a read-modify-write of port register FFFFCB that reads the pins (MAME FB, the
  bench 00). That is where U2 starts.
- **MAME's TRAPA pushes its own address** (`m_NPC` is the TRAPA's address when it runs), so RTE
  returns to the TRAPA and runs it again. The real H8/300H pushes the next instruction's
  address. The core matches MAME; the random programs leave TRAPA through a stub that drops the
  frame. The v1.05 firmware executes no TRAPA and no SLEEP in its first 3 M instructions (1.19 s
  of boot) [OBS]; MIDI playback is checked in U4.

U2 design and findings [OBS]:
- **MAME's peripherals, as MAME runs them.** Each peripheral keeps the time of its next event and
  catches up when a register is accessed; the H8 services events between bus accesses. The HDL
  keeps this model in virtual time: every bus access happens at its end time t (MAME decrements
  icount before the access), then every event at or before t is serviced, earliest first. The
  interrupt request changes only where MAME calls update_irq_state(); the CPU's
  update_irq_filter() and interrupt_taken() end their generated state so the peripherals see them
  before the next access (the RTE case: the filter must change before the prefetch).
- **MAME quirks kept:** ICR writes keep only the written byte (`icr_w`), so the firmware's
  ICRA=04, ICRB=0C leave ICR = 0C00 (SCI0 and SCI1 at priority 1, the timers at 0); the H8H
  timer phase is always 0; clearing a TSR flag does not clear the pending interrupt.
- **Timeslice edges.** MAME's H8 stops a timeslice after the first bus access ending at or after
  the next timer's time (icount <= 0, even mid-instruction), or at the end of an instruction whose
  internal operations crossed it, and leaves the events that access crossed for the next
  timeslice. Two consequences, both measured on the firmware:
  - MIDI in: a pin change at time T goes in after the events before T and before those at or
    after T (an SCI tick at the same state as a bit edge must see the edge first, or the receiver
    re-syncs one tick early). The bench replays MAME's logged positions (`MU_RX_LOG`, patch 0003).
  - LCD busy flag: cleared by MAME's timer at t_w + n / 270 kHz; a port A read sees busy while
    max(end of the access before it, start of the current step) - t_w <= 369 (n = 10) or 15184
    (n = 410). This rule explains all 3,049 clears in 12 s (`MU_LCD_LOG`); the bench and
    `hdl/mu50_lcd.sv` use it.
- **What the firmware uses** (12 s): timer 1 compare A (vector 28, the tick), timer 4 compare A,
  B and overflow (40-42), SCI1 receive (57), IRQ0 (12, held asserted by the board); A/D scan of
  AN0-2 started and stopped by the CPU; one SWP00 register (001) read 327 times. No DMA,
  watchdog or refresh. Not implemented (not used): DMA registers, SCI synchronous and external
  clocks, refresh counting.

U4 design and findings [OBS]:
- **SWP00 time base.** A counter follows the CPU's virtual time t with k = floor(t * 441 /
  100000) (and the remainder), so the SWP00 gets CMD_GEN whenever k grows and keeps making
  samples while the CPU does not touch it; an access to the SWP00 brings the counter to the
  access's end time and sends its command with n = k + 1.
- **LCD busy flag in hardware:** `synthframe_hd44780` with BUSY_EXT, BUSY_SHORT 369 and BUSY_LONG
  15184, read time = max(end of the previous access, start of the current step) (U2 rule).
- **Speed.** Real time needs the CPU at 10 MHz (5 FPGA clocks a state at 50 MHz) and the SWP00
  at 44.1 kHz (1,134 clocks a sample). First board run: 9.1 clocks a state. Changes: no
  wait in the core for an operation's states (the bus side keeps time), the pre stage answered in
  the same clock, no event pass after an access when no event is due and the peripherals are
  idle, the ROM read acknowledged in the clock it arrives: 5.49 (1-clock memories). In the
  MiSTer wrapper with a 12-clock DDR3 model, 55% of the clocks went to the CPU waiting behind a
  CMD_GEN (the SWP00 making a sample); CMD_GEN now runs beside the CPU's ROM and RAM accesses:
  4.44 clocks a state, program ROM cache misses the largest share; 2.66 with 1-clock memories.

U3 design and findings [OBS]:
- **Reference.** `ref/swp00_ref.{h,cpp}` is MAME's `swp00.cpp` (BSD-3-Clause) with the device glue
  replaced: `write()`/`read()` decode the address map as `swp00_device::map()`, `generate()` is
  the stream. `tools/swp00_ref_extract.py` regenerates it from the MAME checkout. Its replay of
  MAME's logs equals MAME's samples and read values (corpus and every stimulus).
- **MAME's C++ is undefined on s32 overflow.** With extreme MEG constants the effect sums
  overflow s32; clang then does not wrap (stimulus seed 1: 15,946 of 88,201 samples differ
  between MAME and the same code built with `-fwrapv`). The core wraps (two's complement), so
  the reference is built with `-fwrapv`. Gate stimuli use moderate constants (`swp00_stimgen.py`
  default), where MAME, the wrapping reference and the HDL agree; the corpus never overflows.
- **Voices** (`swp00.sv`): one channel at a time from a 32 x 628-bit record RAM, one shared
  33x33 multiplier (filter, mixer), the small LFO and interpolation multiplies in place.
  Compressed samples decode one byte per two clocks. Register writes, keyon and reads are
  read-modify-writes of the record between samples.
- **MEG** (`swp00_meg.sv`): MAME's effects code (about 430 lines, four variants) compiled to
  1,413 ops of a small executor: 256 x 32 registers, one multiplier, the delay line port, ops for
  MAME's helpers (m7, m9, m9v, lfo_step, filtered_lfo_step, alfo, rlfo, rlfo2 ...). Pipeline F, D,
  E1, E2, one op a clock; the generator orders the ops so that an op rarely needs the previous
  op's result (result waits 145-186 -> 10 a sample).
- **Overlap.** The MEG works on sample k while the voices make sample k+1. Accesses to MEG
  registers (180-37F, 004) wait for the MEG to finish the samples before them; voice registers
  and reads act at their sample.
- **Interface for U4.** Commands in CPU order, each with n = samples that must exist first
  (Sound timing): generate, byte write, byte read (`rd_data` with `cmd_ready`). Samples out as
  18-bit L/R with a strobe. Wave ROM port: 4 bytes from any byte address (22 bits, the ROM mirrors
  every 4 MB). Delay port: 32-bit words, reverb 00000-1FFFF, chorus 20000-27FFF, variation
  28000-47FFF. One request outstanding on each port; data with the acknowledge.
- **Speed** (bench): corpus 518 clocks a sample on average, 716 at most, with 2-clock memories;
  854 and 1,058 with 8-clock memories;
  dense random stimuli 690-1,020 on average (2-clock). A sample at 50 MHz has 1,134 clocks. Compressed
  samples at high pitch decode more bytes a sample and have no bound (a random stimulus reached
  500,000 clocks in one sample); the firmware's pitches were within the budget in the corpus.
- **Size** (Quartus 17, SWP00 alone, 5CSEBA6): 5,066 ALMs, 3,148 registers, 20 M10K (43,104 bits:
  channel records, MEG registers, constants), 14 DSP blocks, +3.3 ns slack at 50 MHz. The MEG
  program (1,413 x 64 bits) is in logic, not block RAM. The first build was -45.6 ns (a four-byte
  compressed-sample chain and a one-stage MEG execute); splitting those met timing.

## Variants (after U5)

| Variant | Difference from the MU50 [OBS, MAME] | Core change |
|---|---|---|
| DB50XG (`xr253a0`) | H8/3002 at 12 MHz, no panel, MIDI only | an H8/3002 mode and a firmware option |
| DB60XG / NEC XR385 (`xr560b0`) | as the DB50XG, plus the analog input | the same |
| MU10 | H8/3002 at 12 MHz, no screen, a gate array that sends the analog input to the effects | the gate array, if wanted |

The SWP00 and the wave ROM are the same in all four.

## Open questions

- **Timing reference (user, 2026-11-27):** the core follows MAME's timing until better data
  exists; later it should follow the real MU50's. What is MAME's rather than the chip's, so the
  places to change with hardware captures (a logic analyser on the H8 bus, the SCI and the LCD):
  every bus access 2 states with no wait states (the H8/3003's bus controller has wait states
  and ASTCR, which MAME leaves unmapped); TRAPA pushing its own address; ICR writes keeping one
  byte; timer phase 0; the lazy event model and its timeslice edges (MIDI pin changes, the LCD
  busy flag after floor(t_w + n / 270 kHz)); the SWP00's sample boundary rule
  (floor(s * 441 / 100000) + 1) and MAME's reimplemented MEG programs. The generator
  (`tools/h8h_gen.py`) keeps access order and costs in one place, so wait states are a cost
  change there.

- **Licence (default, following the CZ-101):** MIT, as the rest of the repository. The core is
  written from MAME's BSD-3-Clause sources, so `NOTICE` keeps MAME's notice and names the
  authors; `ref/` and `mame/patches/` stay BSD-3-Clause.
- **Effects delay-line width:** resolved in U3. MAME stores unsaturated `s32` sums, so the core
  keeps 32-bit words (0x48000 words, 1.125 MB) to stay exact for any constants. The corpus uses
  18 bits (reverb), 7 (chorus); a narrower store would differ from MAME only when a value
  exceeds it [OBS].
- **DMA:** not used by v1.05 in boot or MIDI playback (U0 trace) [OBS]. U2 leaves it for
  later; the bench reports any DMA register write.
- **Program ROM speed:** MAME fetches every instruction in 2 states with no wait. The core needs
  the program ROM in block RAM or behind a cache that never stalls the CPU's virtual time. The CPU
  runs ahead of real time and waits for it, as the CZ-101 does, so a cache miss costs FPGA clocks
  only, not virtual time [INF].
