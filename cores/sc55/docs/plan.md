# Project plan: Roland SC-55 for MiSTer

Working plan, written 2026-11-19. Tags: **[OBS]** checked in source or on
hardware, **[INF]** inferred from observations, **[UNK]** still to check.
The layout and most methods come from `../synclavierii` (see §8).

---

## 1. Goal and scope

**Goal:** a MiSTer core that runs Roland's original SC-55 firmware and
produces the same samples as Nuked-SC55, checked against a reference
model in lockstep.

**Reference model:** `jcmoyer/Nuked-SC55` at a pinned commit, vendored as
a git submodule. Reasons:

- Its stated aim is 100 % behavioural compatibility with upstream, bugs
  included. Integration tests compare SHA-256 hashes of rendered audio.
- The backend is a library with its state in structs (`mcu_t`, `pcm_t`,
  `submcu_t`), so it links straight into a Verilator testbench.
- It contains upstream's 2026-09 interpolation-table and SHAR fixes [OBS].

Upstream `nukeykt/Nuked-SC55` `mcu_opcodes.cpp` is the readable source for
instruction behaviour, because jcmoyer's rewritten H8 interpreter caches
instructions.

| Target | Chips | Firmware | Order |
|---|---|---|---|
| **T1: SC-55 (mk1)** | H8/532 MCU, Roland PCM chip, LCD + gate array; no sub-MCU [OBS: `SM_Update` skipped when `is_mk1`] | v1.21 / v2.00 (user-supplied) | first |
| **T2: SC-55mk2** | T1 + M37450 sub-MCU (MELPS 740, a 6502 superset: `SM_STATUS_T`) [OBS] | v1.01 | second |
| Later options | CM-300/SCC-1, SC-155, SC-55st, JV-880 (same chips, different ROM maps and I/O) | | after T2 |

**Out of scope:** gate-level copies of the chips; modelling the analog
output stage (DAC, filters) until there are recordings; shipping ROMs.

## 2. Principles (carried over from synclavierii)

1. **Behavioural, timed like the reference.** The HDL follows the
   reference model's time base exactly (decision D2), not the real chips'
   cycle timing.
2. **Reference first, then HDL in lockstep.** Each block is compared with
   the C++ model: every instruction, bus access and audio sample.
3. **Users supply the ROMs.** A tool checks the ROMs against jcmoyer's
   SHA-256 table and packs them into one load file.
4. **Write each finding into `docs/`** with an [OBS]/[INF]/[UNK] tag and
   its source.
5. **Make problems visible on hardware.** A status line shows lag, audio
   underruns, MIDI bytes, UART speed and errors.

## 3. System architecture

```
 MidiLink UART ─► midi_in (31250, falls back to 9600) ─► byte FIFO ─┐
                                                                    ▼
   H8/532 CPU (behavioural, 12 cycles/instr) ── code_cache ── DDR3: ROM2 (≤512 KB)
     │ bus: address, RD/WR, data, cycle
   Memory map / peripherals
     ├─ ROM1 32 KB, SRAM 32 KB, on-chip RAM 1 KB          (BRAM)
     ├─ interrupt controller, FRT timers, 8-bit timer, SCI, A/D, ports
     ├─ LCD controller (+ mk1 gate array) ─► video: LCD + panel ◄─ mouse/keys
     ├─ [T2] M37450 sub-MCU (SMROM 4 KB) ◄─ MIDI
     └─ PCM chip registers
          PCM slot engine (time-multiplexed, ram1/ram2 + eram 32 KB in BRAM)
            ├─ wave reads ─► DDR3 read stream (waverom 3–4 MB)
            └─ samples (64 / 66.207 kHz) ─► audio_fifo ─► resampler ─► 48 kHz AUDIO_L/R
   throttle: holds the CPU while simulated cycles run ahead of real time
```

## 4. Decisions

| # | Decision | Status |
|---|---|---|
| D1 | Framework | **Template_MiSTer.** JTFRAME's MiSTer target ties off the HPS UART, which MidiLink needs (synclavierii research-log §25). |
| D2 | Time base | **The reference's model:** one instruction = 12 MCU cycles [OBS `mcu.cpp` `cycles += 12 // FIXME`]; one PCM slot = 25 cycles, `(reg_slots+1)*25` per sample [OBS]; the sub-MCU runs at `cycles*5`, 48 per instruction [OBS]. This is required for bit-exact comparison. Real H8/532 cycle timing is a possible later option, but it could not be checked bit for bit. |
| D3 | ROM storage | **Proposed:** mk1 ROM2 is 256 KB [OBS: v1.21 `sc55_rom2.bin`], so it fits in BRAM (256 M10K blocks, about 406 of 553 with the rest of R4) and the CPU needs no code cache for T1. The wave ROMs (mk1: 3 × 1 MB [OBS]) go in **DDR3** through synclavierii's `ddram_word.sv` read stream. For T2, if the mk2 ROM2 is 512 KB [UNK], it moves to DDR3 behind `code_cache.sv`. Wave-read load: 5 ROM reads per slot (`pcm.cpp:1139–1243`), about 4.8 M reads/s at 24 MHz, mostly sequential bytes. The test MiSTer sustained about 13 M random 64-bit DDR3 reads/s (synclavierii M26) [OBS]. The alternative is SDRAM (fixed latency, no cache, but needs the add-on board). Open: does the test MiSTer have SDRAM? |
| D4 | Lockstep harness | **jcmoyer's backend linked into the Verilator testbench in C++**, in the same process. There is no Python simulator and no trace-file format to maintain (synclavierii needed both). |
| D5 | Licence | **GPL-2.0-or-later**, as upstream. HDL translated from the Nuked code is a derivative work. Template_MiSTer `sys/` is GPL-2.0, which is compatible. |
| D6 | MIDI input | Bytes reach the SCI the way `MCU_PostUART` delivers them: buffered, with the reference's `uart_rx_delay` of 3000 cycles. This keeps lockstep exact. Serial input on the RX pin at the firmware's baud rate is a later option. |
| D7 | SCI fixes from mckuhei (shingo45endo) | Check in M0 whether they change any reference render. Adopt them only into the reference fork with new hashes; never in the HDL alone. |
| D8 | Releases (decided 2026-11-20, with the user) | MiSTer-devel convention: dated cores committed to `releases/SC55_YYYYMMDD.rbf` (installable with update_all / downloader) and attached to a GitHub Release `vYYYYMMDD`, by `.github/workflows/release.yml` after each successful Build RBF of main. The hygiene test allows RBFs only there. |

## 5. Milestones

Progress (2026-11-20; details in each doc):

| M | Status |
|---|---|
| M0 | done: reference builds without SDL; hooked copy; decoder2 on/off identical; mk1 = 20 MHz reference cycles ([`reference.md`](reference.md)). jcmoyer's own hashes are mk2-only; with the user's mk2-v1.01 ROMs (2026-11-20) the reference build reproduces them for avmidi 01 and 0A. |
| M1 | done: CPU identical on firmware (30 M steps), MIDI traffic, random code; 18/18 seeded faults ([`cpu.md`](cpu.md)) |
| M2 | done: MCU system identical 20 M steps with MIDI and buttons; 12/14 seeded faults (2 unused by the firmware) ([`mcu.md`](mcu.md)) |
| M3 | done: PCM identical sample for sample, loud and normal MIDI, any ROM latency; 12/13 seeded faults (1 unobservable on mk1) ([`pcm.md`](pcm.md)) |
| M4 | done: avmidi `0A.mid` through the HDL system = `sc55-render` SHA-256 ([`board.md`](board.md)) |
| M5 | simulation done (board bench from power-on, DDR3 model, pack download); Quartus: fits (28,060 ALMs 67 %, 453/553 RAM blocks, 96/112 DSP), all clocks meet timing with seed 3 (core 50 MHz +0.843 ns, video 40 MHz +8.772 ns, HDMI +0.514 ns; seed 1 failed by 0.037 ns inside the framework's ascal, seeds 2 and 4 also pass); on the test MiSTer (2026-11-20): boots to the same LCD as the bench, plays MIDI at 9600 baud on ttyS1 (not yet through the MidiLink daemon at 31250), avmidi `0A.mid` for 100 s with no underrun and no lag ([`board.md`](board.md)) |
| M6 | LCD and panel keys done in simulation (frame identical to `LCD_Render`) ([`lcd.md`](lcd.md)); mouse panel not done |
| M7 | done: sub-MCU, mk2 MCU/PCM/board identical to the reference; avmidi 0A.mid through the mk2 HDL = jcmoyer's mk2 hash; SC-155 (mk1 board) identical; on the MiSTer: 100 s of 0A.mid with no underrun or lag ([`mk2.md`](mk2.md)) |

Decisions taken while building: ROM1/ROM2/SRAM in block RAM (mk1 ROM2 is
256 KB), wave ROMs in DDR3 (D3); the PCM runs alongside the CPU with exact
ordering (register accesses wait; IRQ0 pre-scan) instead of strictly after
each step; 50 MHz core, 40 MHz video (800 × 600 shows the 741 × 268 LCD 1:1).

| M | Work | Acceptance |
|---|---|---|
| **M0** Reference and repo | git repo, CI (`tests.yml`), jcmoyer as a submodule; renderer built; `tools/romset.py` (hash check, packing); trace hooks in a thin fork: per instruction (pc, regs, cycle), bus R/W, PCM register writes, samples; measure the mk1 clock (`cycles/s`) | jcmoyer integration hashes reproduce with user ROMs; mk1 boot trace recorded |
| **M1** H8/532 CPU | `hdl/h8_cpu.sv`: H8/500 instruction set (about 1,900 lines of C in `mcu_opcodes.cpp`), all addressing modes, interrupt entry; device reads answered from the trace (synclavierii's `lockstep.cpp` pattern) | Random programs covering every opcode × addressing mode; mk1 boot to the idle loop with every instruction identical; seeded single-bit mutations are caught |
| **M2** MCU peripherals | Interrupt controller, FRT/8-bit timers, SCI, A/D (buttons, volume knob), ports, memory map, mk1 gate array, LCD controller; all counted in reference cycles | Lockstep with no device left to the trace except the PCM chip |
| **M3** PCM chip | `modules/roland/hdl/pcm/`: register interface plus the slot engine, serial through BRAM (one slot = 25 reference cycles; for example 50 core clocks at 48 MHz); `addclip20`, `multi`, the corrected interp table generated from `pcm.cpp` by a tool; reverb/chorus `eram`. giulioz's `pcm.v` is a sketch only: its table has the old 3534 entry | Standalone bench: recorded register writes in, every sample identical |
| **M4** System lockstep | CPU + peripherals + PCM against the model in one process | Each jcmoyer test MIDI rendered bit-exact (same SHA-256) |
| **M5** MiSTer core | `emu` top level; ioctl ROM load into DDR3/BRAM; board bench from power-on with random register and memory contents (`--x-initial unique`) and a DDR3 model with BUSY and latency; throttle; `audio_fifo`; 64/66.2 → 48 kHz resampler; status line; `build-rbf.yml` fails on negative slack | On the test MiSTer: boots, plays MIDI over MidiLink, Lag/Underrun 0 over 100 s |
| **M6** Panel and LCD | LCD drawn as on the unit (`lcd.cpp` is the reference, compared pixel for pixel); buttons from keyboard, mouse (`panel_mouse.sv`) and MIDI; volume from the A/D | Frame read back equal to the reference LCD buffer; buttons change parts and instruments |
| **M7** SC-55mk2 | M37450 sub-MCU (evaluate T65 as a base and add the 740 instructions), 28 voices, 66.207 kHz, MIDI routed through the sub-MCU as in the reference | M4 and M5 criteria repeated with the mk2 romset |
| **R** Public release | After M7 is merged and the mk2 pack has run on the MiSTer: scan the whole git history for ROM content (romset hashes, sizes); README install notes (releases/, ROM pack); make the repository public; the first release from main (D8) | `releases/SC55_YYYYMMDD.rbf` on main and a GitHub Release; installs on the MiSTer from the repository |
| **M8** Options | Oversampling on/off, other romsets (D-table), MGL per model | per option |

## 6. Verification

| Level | Method | Reference |
|---|---|---|
| Reference | jcmoyer integration hashes | `test/integration` |
| CPU | Random programs + firmware boot, per instruction | C++ model in the bench |
| Peripherals, PCM | Per-block benches, then system lockstep | C++ model |
| Audio | SHA-256 of rendered test MIDIs | renderer output |
| Board | Power-on with random contents, DDR3 BUSY/latency model | `board_top` pattern |
| Hardware | `scripts/mister/`: load_core, MIDI to ttyS1, screenshot + OCR of the status line | the test MiSTer |
| Benches | Every testbench must catch seeded faults (synclavierii: 3–8 per bench) | — |

## 7. Risks

| # | Risk | Mitigation |
|---|---|---|
| R1 | No H8/532 HDL exists; the CPU is the largest piece of work | Build per instruction class, with random-program lockstep from the first opcode |
| R2 | CPU fetch latency from DDR3 | `code_cache`; a "Lag by cause" status (Fch, PCM, other), as in synclavierii |
| R3 | Wave reads late → underruns | Wave reads go first on the DDR3 stream; prefetch the next slot's 4 sequential bytes; on hardware the PCM runs on a real-time clock (synclavierii M14 `voice_clock` split) while lockstep runs it on CPU cycles |
| R4 | BRAM budget | About 150 of 553 M10K planned (ROM1 32, SRAM 32, eram 32, sub-MCU 4, PCM RAMs ~4, video/LCD ~20, FIFO/cache ~24) |
| R5 | Reference timing ≠ hardware timing (12 cycles/instruction is a FIXME) | Accepted under D2; recordings from real units are the later check |
| R6 | ROM distribution | Users supply ROMs; nothing copyrighted in the repo. Local ROMs live in git-ignored `firmware/` (v1.21 set present, all five SHA-256 hashes match jcmoyer's `mk1-v1.21` table [OBS]); `test_repo_hygiene.py` (as in synclavierii) fails on any ROM-sized binary or ROM hash in the tree |

## 8. What synclavierii taught, and where it applies here

| synclavierii lesson | Applied here |
|---|---|
| Reference model + per-instruction lockstep found HDL bugs that hardware runs would hide (for example the 17-bit remainder overflow, research-log §24) | M1–M4 gate on lockstep; the reference is C++ in the same process (D4) |
| A single shared time base for CPU and devices made device lockstep exact | D2: everything counts reference cycles |
| Voices clocked by CPU time stopped during memory stalls → underruns (M14) | R3: the PCM engine gets its own real-time clock enable in hardware mode; lockstep keeps CPU time |
| Throttle + code cache + "Lag by cause" status made real-time problems measurable | Reuse `throttle.sv`, `code_cache.sv`, `ddram_word.sv`, `audio_fifo.sv` (MIT, same author) |
| Board bench from power-on with random contents caught init bugs | M5 board bench |
| MidiLink ttyS1 can fall back to 9600 baud (the Zaparoo service reopens it) | Reuse `midi_in.sv` with 31250/9600 detection; stop Zaparoo during tests |
| OSD settings must be taken while reset is held, not on reset's rising edge (it rises before the HPS sends status) | Model/romset select read during reset |
| Shared checkout with other agents; stage explicit paths | Same rule in this repo |
| CI Quartus build with a slack gate; dated releases | Same workflows; RBFs in `releases/` (MiSTer-devel convention, D8) |

## 9. Unknown

- Whether the test MiSTer has an SDRAM board (affects D3).
- ~~mk1 MCU clock~~ Resolved in M0: the renderer steps mk1 at 600 ns per
  instruction (`R_NSPerStep`), so 12 cycles = 600 ns → **20 MHz** reference
  cycles, 1.667 M instructions/s; mk2 500 ns → 24 MHz [OBS].
- How mk2 routes MIDI through the sub-MCU in the reference (read `submcu.cpp` in M7).
- Whether the H8/532's DTC is used by the firmware (no `DTC` hits in `mcu_interrupt.cpp`).
