# Session log

One entry per iteration of `SESSION-PLAN.md`, newest first.

## Iteration 15: JV-880 card RAM (2026-11-23, branch `jv880-card-ram`)

- **Question (`cores/jv880/docs/plan.md`):** does the firmware need the card RAM (pages 14/15)
  without a card? The reference keeps 32 KB there.
- **Measured** with a copy of the reference that counts page 14/15 accesses (not committed):
  power-on to the factory preset (`jv880-nvram`) 3,683 reads, no writes; rendering `jv880.mid`
  6,311 reads, no writes; every read returned 0. Hashes unchanged in that copy: factory NVRAM
  `d5da7845…d207`, render `73feda3c…bbbc`.
- **Change (`modules/roland/hdl/mcu/sc55_mcu.sv`):** with `JV880_RAMS` the card RAM's read data is
  0 and its block RAM is gone; writes go nowhere. The SC-55's branch of the generate is untouched.
  An earlier version that also removed the card write-port logic changed the SC-55's netlist
  (32,840 ALMs, setup slack -0.018 ns at seed 2), so it was narrowed to the JV-880 branch.
- **Checks:**
    - `mcu_lockstep --romset jv880-v1.0.1 --jv880-factory`: 21,324,800 steps identical, NVRAM
      `d5da7845…d207`.
    - `jv880_board_bench --smf jv880.mid` (factory pack): 42,181,775 steps and 1,640,121 samples
      identical, SHA-256 `73feda3c…bbbc`.
    - SC-55 `test_mcu_lockstep.py` (mk1, mk2, SC-155), `test_system_lockstep.py` (mk1, mk2),
      `test_board_units.py`, `test_repo_hygiene.py`: pass (long hash runs skipped).
    - Windows builds (seed from `cfg/core.yaml`):

      | Core | ALMs | main | Block memory bits | main | M10K | main | Worst setup slack |
      |---|---|---|---|---|---|---|---|
      | SC-55 | 32,655 | 32,655 (PR #23) | 3,696,465 | — | 472 | — | +0.372 ns |
      | JV-880 | 28,769 | 28,902 | 3,904,561 | 4,166,705 | 495 | 527 | +0.365 ns |

      JV-880 `main` is a build of `origin/main` (7320e58) on the same host: 262,144 bits (32 KB)
      and 32 M10K fewer.

    - MiSTer smoke (`cores/jv880/scripts/mister/smoke.sh`, factory pack, the JV-880 build above): pass. Running, no underrun or lag, 60 s of `jv880.mid`, save file 32 KB, NVRAM kept after the reload.
- **Difference from the reference:** Util:Data card writes are not kept (no card fitted).

## Iteration 15: CZ-101 pitch wheel (2026-11-23, branch `cz101-pitch-wheel`)

- **`mister/CZ101.sv`:** `cz101_board.pitch_wheel` (AN1) was tied to 0x7F. It now follows the
  first joystick: Y axis as 127 - Y (-127 = full up = 0xFF), or the up/down directions for 0xFF /
  0x00, as MAME's paddle on the joystick's Y switches. OSD help line added.
- **Checks:**
    - `tests/c4_run.sh tests/midi/basic.txt 6500000 tests/panel/wheel.txt` (five wheel moves
      during a held note, from 12.9 s): 7,923,263 trace lines and 716,620 samples identical to
      MAME (SHA-256 `4fd2c703…9035`), on the bench and live MIDI paths; 57 MIDI-out bytes
      identical (the firmware's pitch bends `E0 40 51 …`), within 19 cycles. MAME's uPD933 writes
      first differ from the plain run at 12.904 s: the firmware retunes the voice.
    - Windows build (seed 2): 14,611 ALMs (`main` 14,553), worst setup slack +0.774 ns, core
      clock +4.178 ns.
    - MiSTer smoke (`scripts/mister/smoke.sh` of `main`): pass; MIDI out `c0 0a`, no underrun,
      save file 8 KB. The joystick itself was not moved on the hardware (no automated gamepad).

## Iteration 15: CZ-101 written tone survives a reload (2026-11-23, branch `cz101-reload`)

- **Finding (bench, `CZ_KEYS`/`CZ_LCD_LOG`/`CZ_NV_OUT`):** the firmware writes a tone only while
  Write is held: Write down, Internal, a tone button, Write up. Pressed and released first, Write
  shows "WRITE / SELECT MEMORY!" and leaves write mode. Internal tones are 128 bytes each at
  0800-0FFF; 0300-03FF hold the selected tone.
- **`tests/reload_run.sh`** (bench only; panel keys match MAME, PR #27): `tests/panel/write.txt`
  stores preset 3 in internal tone 2; the bench restarts from that battery RAM (as the core does
  after a reload with its save file) and `tests/panel/recall.txt` selects internal tone 2. Pass:
  internal tone 2 written and kept; tone buffers 0300-03FF equal those of a run that selects
  preset 3 (`tests/panel/preset3.txt`).
- **Smoke test:** the "4" key step is replaced by write (Write held via the new
  `down:`/`up:` actions of `mister_keys.py`), save 1, reload, recall internal tone 2, save 2; save 2
  must keep internal tone 2 (0880-08FF) and the tone buffers of save 1.
- **Checks:**
    - `tests/reload_run.sh`: PASS.
    - No HDL change: C4 and the Windows build are those of `main` (PR #27: C4 unchanged; 14,553
      ALMs, +0.650 ns); the smoke test ran on that RBF.
    - MiSTer smoke: pass. internal tone 2 written, kept after the reload, recalled (tone buffers
      equal); LCD after the reload equals the one before; `c0 0a` on MIDI out; no underrun.
    - `mister_keys.py` only gains `down:`/`up:` and F5-F8; the Synclavier smoke test was not
      rerun.

## Iteration 15: CZ-101 MIDI out, panel scripts (2026-11-23, branch `cz101-midi-out`)

- **MIDI out was already wired** (`midi_tx` to `UART_TXD` in `mister/CZ101.sv`) but untested.
- **New test inputs, the same in MAME and the HDL:**
    - `tests/panel/*.txt` (`tools/panel_sched.py`): panel buttons and the pitch wheel at given
      CPU cycles. The bench reads them as `CZ_KEYS` and `CZ_WHEEL`; MAME's patch 0001 gains the
      same two variables (in `keys_r` and AN1) and `CZ_TX_LOG` (its TxD line changes).
    - `cz101_bench`: also `CZ_TX_LOG`, `CZ_LCD_LOG` (LCD text on each change) and `CZ_NV_OUT`
      (the 8 KB battery RAM and cartridge at the end).
    - `tests/c4_run.sh MIDI N [PANEL]` applies a panel script and compares the MIDI-out bytes
      (`tools/txlog_bytes.py`): same bytes, each starting within 32 CPU cycles of MAME's. MAME
      moves TxD on its 2 MHz serial clock edges, the board at the next instruction boundary.
- **Checks (`../mame-cz101` rebuilt with the new patch 0001):**
    - `tests/c4_run.sh tests/midi/basic.txt 6500000`: unchanged, 7,922,583 trace lines and
      716,728 samples identical, SHA-256 `08745dc2…0eb7`; no MIDI out.
    - With `tests/panel/basic.txt` (tone selection, five pitch-wheel moves during a held note):
      7,922,248 trace lines and 717,228 samples identical (SHA-256 `472320fd…08a9`), on both the
      bench's MIDI path and the live path; 63 MIDI-out bytes identical (program changes `C0 02`,
      `22`, `21`, `01`, `00`, then the wheel's pitch bends `E0 40 51 …`), within 19 cycles.
    - Windows build: 14,553 ALMs (= `main`), worst setup slack +0.650 ns.
    - MiSTer smoke (`scripts/mister/smoke.sh`, now also reading `/dev/ttyS1` at 31,250 baud with
      `scripts/mister/tty_midi_read.py`): pass. Keys "3" and F3 after the MIDI test (preset 15)
      select preset 11, and `c0 0a` came out of the UART. No underrun; save file 8 KB.

## Iteration 9b: qpf, qip header and Verilator file lists (2026-11-23, branch `iter9b-filelists`)

- **synthcore now also writes:**
    - `mister/<core>.qpf`;
    - `sim/files.f` for `verilator -F`: `+incdir+` per search directory, then the core's HDL.
- **`cfg/files.yaml`:** the MiSTer top level and `.sdc` moved to a `mister:` list (Quartus only).
- **Quartus files:**
    - The qsf and qip say they are generated from `cfg/`; the old "add the files manually to
      files.qip" header is gone.
    - All qsf and qpf are LF with a final newline; `qsf_eol` is dropped.
    - The non-comment content of every qsf, qpf and qip is identical to `main`.
- **Benches on `sim/files.f`:**
    - SC-55 `board_lockstep`, JV-880 `jv880_board_bench`, CZ-101 `cz101_bench`: through
      `modules/synthframe/cmake/synthframe_files.cmake`.
    - Synclavier `sim/lockstep.py` `board` top: `SYSTEM_RTL` first, then the rest of
      `files.f`. In `files.f` order Verilator emitted the same logic in a different statement
      order; with `SYSTEM_RTL` first the generated C++ is identical to `main`.
- **Checks:**
    - Verilator lint of every core's `files.f` under its core top: no errors (Proteus needs
      `-Wno-BLKANDNBLK` for fx68k).
    - SC-55 `test_system_lockstep.py`: 4 passed (board mk1 10 M steps, mk2 8 M steps,
      samples identical to the reference); the two `SC55_LONG` hash runs skipped.
    - JV-880 board bench, `jv880.mid` from power-on with the factory NVRAM (`d5da7845…`) in
      the pack: 42,181,775 steps and 1,640,121 samples identical to the reference (peak
      -14.6 dBFS, 18,510,322 DDR3 reads), LCD `I-01:Syn Lead`, output SHA-256
      `73feda3c…bbbc`, the reference's hash.
    - CZ-101 `cz101_bench`, real firmware, 4 M instructions: trace (4,838,063 lines) and
      438,294 samples byte-identical to the `main` build.
    - Synclavier `tests/hdl/test_board.py` with `SYNCLAVIER_RELEASE_L`: 30 tests OK, none
      skipped, including the MONITOR boot and ORK-LM state image on `board_top`. The Release L
      image was not on the Mac or the NAS; it was downloaded from
      `Synclavier/Synclavier_Distributions` (SHA-256 `97bb43de…1db0`, as `docs/sources.md`) to
      `synclavier-ii-MiSTer/Synclavier_Distributions/Release_L/` (untracked).
    - Windows builds, all meet timing:

      | Core | ALMs | main | DSP | Block memory bits |
      |---|---|---|---|---|
      | SC-55 | 32,655 | 32,655 | 99 | 3,696,465 |
      | JV-880 | 28,902 | 28,902 | 97 | 4,166,705 |
      | CZ-101 | 14,527 | 14,527 | 37 | 927,169 |
      | Proteus/1 | 25,071 | 25,071 | 40 | 2,065,010 |
      | Synclavier | 26,399 | 26,399 | 52 | 3,579,505 |

      `main` is 9c6f186 built on the same host. Worst slack +0.241 ns (Proteus, Synclavier,
      JV-880) to +0.250 ns (SC-55).
    - MiSTer smoke tests of these RBFs, all pass:
        - SC-55: running, no underrun or lag.
        - JV-880: running, no underrun, NVRAM kept after a reload.
        - CZ-101: MIDI and keys change the LCD, 8 KB save file.
        - Synclavier: menu up, no underrun, 0 late P in 30 s.
        - Proteus: 0 missed sample deadlines in the 60 s soak, produced 1.008 of the expected
          samples.

- **Open:** the Proteus `sys_top` simulation has no build script; `sim/files.f` is its file
  list when one is written. The Vivado list waits for the M64.

## Iteration 9: `cfg/` files and the `synthcore` generator (2026-11-23, branch `iter9-synthcore`)

- **Generator:** `modules/synthframe/bin/synthcore` (Python, MIT, PyYAML) writes each core's
  `mister/files.qip` and `mister/<core>.qsf` from `cores/<core>/cfg/`:
    - `core.yaml`: project name, seed, qsf line ending (the Proteus/1 qsf is CRLF);
    - `files.yaml`: search paths and files in Quartus order;
    - `macros.def`: JTFRAME-syntax macros per target.
- **qsf template:** `modules/synthframe/target/mister/project.qsf.in`. The five qsf files
  differed only in SEED, `MISTER_DISABLE_ALSA` (off for the Proteus/1) and line endings.
- **Gate passed:** all 10 files (5 `files.qip`, 5 qsf) deleted and regenerated are
  byte-identical to `main` (SHA-256 and `git diff`), under python3 and pypy3. Quartus input is
  unchanged, so no Windows build or MiSTer smoke test was run.
- **CI:** a `synthcore` job in `tests.yml` runs `synthcore check` and the unit tests on every
  change.
- **Proteus `.gitignore`:** `cfg/` (MAME run output) now ignores subfolders only; `/cfg/*.cfg`
  stays ignored.
- **Open:**
    - The qsf header still says "Add the files manually to files.qip". Changing it changes the
      qsf bytes, so it waits for a later PR.
    - `.qpf` files, Verilator `.f` lists and the Vivado list (M64 deferred) are not generated.
    - `core.yaml` holds only the fields synthcore reads; LAYOUT-PLAN's clock, memory-port and
      panel fields wait for a consumer.

## Iteration 8, CZ-101: the panel modules (2026-11-23, branch `iter8b-cz101-panel`)

- **`synthframe_hd44780`** now carries the CZ-101 controller's MAME details:
    - busy time from an external time base (`BUSY_EXT`; the CZ-101's virtual CPU cycles) or
      clock cycles (Proteus/1);
    - reset and clear fill DDRAM one address a clock; reset zeroes CGRAM;
    - instruction 0x00 sets no busy;
    - one-line mode wraps at 80 as MAME's render().
  `cores/cz101/hdl/cz101_lcd.sv` is now the pin side only.
- **CZ-101 video:** `synthframe_vtiming`, `synthframe_charlcd` (new `LEFT` parameter and
  `cell_row` output) and `synthframe_status_strip`. The strip's colours are now the shared
  ones (amber loading, dark red underrun seen, lag box); the smoke test reads them.
- **Frame benches:** new CZ-101 bench through the CZ-101's pins. The Proteus bench's cursor
  command was 10 bits in a 9-bit slot (it wrote a data byte); fixed, and the Proteus/1 and
  CZ-101 hashes recorded again from main (4c45522) before any change. All 16 hashes unchanged by
  the move; the CZ-101 strip change alters only the strip's pixels (x 12-391, y 412-431). A
  one-line wrap mutation changes the one-line seed's hash.
- **C4** (`tests/c4_run.sh tests/midi/basic.txt 6500000`): pass, as on main: 7,922,583 trace
  lines and 716,728 samples identical to MAME (sha256 08745dc2...0eb7), live-MIDI path too.
- **Proteus/1 system simulation**, 4 s, against the iteration 8 baseline: writes, audio and LCD
  text identical.
- **Windows builds**, timing met: CZ-101 14,527 ALMs (main 14,577), Proteus/1 25,071 (24,710:
  CGRAM clear and the sequential DDRAM clear), JV-880 28,902 (unchanged). Block memory and DSP
  unchanged.
- **MiSTer smoke:**
    - CZ-101: running, no underrun, 16,317 lit pixels at boot (as main), MIDI and key change the
      LCD, 8 KB save file.
    - Proteus/1: 0 missed, ratio 1.008, worst 928/2048; boot sha a0c4feec... as iteration 8.
    - The previous RBFs are in `/media/fat/_synth_backup/20261123-czpanel-pre/`.
- **Open:** in one-line mode MAME draws only line 0; the render port returns line 0's
  characters for line 1 as well (both cores' firmware use two lines).

## Iteration 13: CZ-101, C0-C5 (2026-11-23, branch `iter13-cz101`)

Plan: `cores/cz101/docs/plan.md`; CPU notes: `cores/cz101/docs/upd7810.md`.
- **C0, reference:** MAME `cz101` subtarget built in `../mame-cz101` (commit 4a92d534, the P2K
  checkout's) with `cores/cz101/mame/patches/0001-cz101-logging.patch`: CPU trace with IRR
  (`CZ_TRACE`), uPD933 write log and raw 40 kHz samples (`CZ_SND_LOG`, `CZ_SAMPLES`). Generated test
  ROMs run on the same machine. Finding: MAME maps the uPD7811 internal ROM over 0000-0FFF in front
  of the program ROM, so the core needs `upd7811g-120.bin`.
- **C1, instruction set:** `hdl/upd7810/upd7810_core.sv` with a decode ROM generated from MAME's
  tables. 120 random programs x 25,000 instructions identical to MAME. MAME quirk found: `ADCX_*`
  pass carry 0 to `ZHC_ADD`.
- **C2, peripherals (random programs):** `upd7810_periph.sv`, `upd7811.sv`. 160 programs with special
  registers, interrupt handlers, SKIT/SKNIT and HLT identical, 309,527 interrupt entries. Fixed on the
  way: prefixed opcodes call `handle_timers` twice (A/D reset timing); prescaler backlog drains as
  MAME's while-loops; mode-0 serial steps only on TO changes. 1,064 of 1,110 opcode forms covered.
- **C3, uPD933:** a subagent is writing the HDL and a standalone copy of MAME's model.
- **Timing model for C4:** MAME's scheduler applies an event at time T before the instruction
  starting at cycle s when T < (s + 1) x 300 ns; during an instruction `machine().time()` is its start,
  or its start + cycles for a prefixed opcode. The core now exports `vcyc`/`acc_time` and waits at
  each boundary for the board's events. `hdl/cz101_lcd.sv`: MAME's HD44780 with the busy flag in
  CPU cycles (busy while s - t_w <= 122 or 5060).
- **ROMs:** the user's MAME set (`synth-projects/nuked-sc55/firmware/cz101-mame/`). The internal ROM
  `upd7811g-120.bin` is byte-identical to the program ROM's first 4 KB.
- **C3 done:** uPD933 HDL (subagent), identical to a standalone copy of MAME's model; external-timer
  mode so the board places its IRQ at the exact instruction boundary.
- **C4:** `hdl/cz101_board.sv`. With the real firmware and `tests/midi/basic.txt`, 6.5 M instructions
  (boot and MIDI playback, 17.9 s) identical to MAME and 716,728 samples identical. Fixed on the way:
  the timer base must be the arming /CS request, not the latest one.
- **C5, MiSTer core:** `mister/CZ101.sv` with a 40 KB ROM pack (program ROM, `init_ram`, HD44780
  character ROM), pacing at 3,333,334 CPU cycles/s, live MIDI through a board serializer (10 bits
  of 128 SCK edges; the C4 run fed this way is identical too), 800 x 600 LCD and LED video, MAME's
  panel keys plus F1-F8/Enter, Power/cartridge/protect switches, and the 8 KB RAM save.
  - Quartus 17 fixes: untyped string parameters, `synthesis translate_off` around the RAM
    `initial` (5000-iteration limit), a true-dual-port RAM (two write addresses had made it
    registers: 187 % fit, now 35 %), `DDRAM_CLK` clocked (an unclocked one gave 0-byte screenshots).
  - Audio: new `modules/synthframe/hdl/audio/synthframe_resample.sv` (MIT), L/M polyphase FIR,
    40 -> 96 kHz, 960 taps; `tests/resample/run.sh`: 13,443 outputs identical to the integer
    model at both pacings.
  - After the rebase onto main: `synthframe_nvsave` now saves with the core running, so the board
    exports `nv_written` for its `mem_written`.
  - Build 11: 14,577 ALMs, 37 DSP, core clock +4.13 ns. Smoke test passes (ROM ok, no underrun,
    MIDI and a key change the LCD, 8 KB save differs from `init_ram`).
  - On the MiSTer the user heard nothing at first: `uartmode.CZ-101` routed MidiLink to
    FluidSynth; setting the UART to MidiLink fixed it.
- **Open:** an edited patch surviving a reload is not shown by the smoke test (the firmware boots
  to preset 1); music keys, pitch wheel and MIDI out are not mapped.
- **Licence (after #18):** MIT, with MAME's BSD-3-Clause notice in `cores/cz101/NOTICE`; SPDX
  headers on the core's own files; `ref/` and `mame/patches/` stay BSD-3-Clause.

## Iteration 8: panel as data (2026-11-23, branch `iter8-panel`)

- **Modules** (`modules/synthframe/hdl/panel`, MIT; contract in its `README.md`):
    - `synthframe_hd44780`: the Proteus/1's MAME-model HD44780, moved from
      `cores/proteus/mister/rtl/hd44780.sv` (the author's own file, now MIT).
    - `synthframe_charlcd`: character LCD renderer behind a read port (character, then dot row).
      The JV-880 draws its LCD and its debug line with it.
    - `synthframe_vtiming` and `synthframe_status_strip`: they replace the copies in the SC-55
      and JV-880 renderers.
- **What stays with the core:**
    - Each core's LCD controller, because it must match that core's reference: Roland
      `lcd_ctrl` (Nuked-SC55) and the HD44780 (MAME).
    - The SC-55's 1:1 `LCD_Render` drawing.
    - The Proteus/1's 320 x 240 `display.sv` with its voice bars.
    - The CZ-101 moves to these modules after its PR merges.
- **Gate passed:**
    - Frame benches (`modules/synthframe/ver/panel`): SC-55, JV-880 and Proteus/1, 4 seeds each,
      two frames hashed every pixel clock. All 12 hashes are identical to the ones recorded
      before the change.
    - The bench catches a mutation: one colour changed in the JV-880 debug line changed its
      hash.
    - SC-55 suite: all pass, including the board bench from power-on, whose LCD frame is
      compared pixel by pixel with the reference, mk1 and mk2.
    - JV-880 board bench on `jv880.mid`: 42,181,775 steps and 1,640,121 samples identical to
      the reference; LCD RAM identical.
    - Proteus/1 system simulation, 4 s with two panel presses, old build against new: CPU
      writes, audio and LCD text identical (`PRESET NAME / 000 Stereo Piano`).
    - Windows builds meet timing:

      | Core | ALMs | main | Block memory, DSP |
      |---|---|---|---|
      | SC-55 | 32,655 | 32,863 | unchanged |
      | JV-880 | 28,902 | 28,980 | unchanged |
      | Proteus/1 | 24,710 | 24,710 | unchanged |

    - MiSTer smoke tests pass for all three cores:
        - SC-55: boot LCD `1137774a…`, the same as the main build of PR #11. The iteration 1
          hash `67a34ac6…` changed on 2026-11-23, before this branch.
        - JV-880: NVRAM kept after a reload.
        - Proteus/1: 0 missed deadlines, ratio 1.008.
- **CI:** the panel frame test runs in the SC-55 and Proteus jobs. A change under `cores/jv880`
  now runs the SC-55 job.

## JV-880 clipping and OSD dropout (2026-11-23, branch `jv880-clip`)

User report: clipping on the saw lead patch; underrun when the OSD opens; two notes drop one.
- **Output level:** the board's s16 conversion was the renderer's `>> 15`, which clamps the top
  6 dB of the PCM's 20-bit output. 48 of the 64 factory patches reach it with one note or a
  4-note chord at velocity 127 (I-22 SAW Lead: +5.3 dB with one note). Now `>> (16 - gain)`:
  0 dB = 20-bit full scale; +6 dB = the old level.
- **20-bit mix (JV-880):** jcmoyer/Nuked-SC55 7204125 replaced the original saturating
  `addclip20` with a plain add; loud JV-880 chords wrapped full negative -> full positive (624
  times over the 64 patches; SAW Lead 8). The PCM saturates again when `jv880`, and the reference
  does the same for the JV-880 (`cores/sc55/ref/patch_ref.py` FIXES, applied to both builds).
  `test_jv880_ref.py test_mix_saturates`: 68 samples at the rail, no wraps (8 wraps before).
- **SC-55 left as it was:** saturating its `addclip20` too kept all 112 SC-55 integration
  hashes on disk (mk1-v1.21, mk2-v1.01, sc155-rev1) and the mk1/mk2 lockstep, but the SC-55 then
  failed timing on seeds 1-7 (-0.025 to -1.005 ns; paths in the MCU timer compare and the PCM
  address counter, ~33,300 ALMs). With `jv880` tied low its netlist is unchanged.
- **NVRAM save:** `synthframe_nvsave` held the core for the whole 32 KB SD write (the audio
  FIFO is 5.3 ms). It now writes with the core running, from the MCU's NVRAM copy, and again with
  the core paused only if the NVRAM was written meanwhile (`sc55_core` `nv_wrote`). The
  reference firmware wrote no NVRAM byte in 10 s of notes and program changes.
  `cores/jv880/sim/nvsave_tb.cpp`: no write / write behind / write ahead, file = RAM.
- **Two notes on SAW Lead:** a SOLO patch on the reference (the first note's fundamental falls
  43 dB after the second note); I-01 JV Strings keeps both. Not a core issue.
- **Checks:** board bench vs reference on `jv880.mid` (1,640,121 samples, hash 73feda3c) and a
  SAW Lead chord file (1,261,941 samples); `--save-at` with and without a save file; SC-55 mk1
  lockstep.
- **Windows build:** JV-880 seed 5, 28,944 ALMs, core setup slack +0.527 ns (seed 2 missed by
  0.385 ns after the change). SC-55 seed 2 unchanged: 33,160 ALMs, +0.786 ns.
- **MiSTer smoke:** SC-55 100 s of 0A.mid, underrun and lag never. JV-880 (seed 5): boot, 60 s of `jv880.mid`, autosave on OSD open: underrun
  never, lag never; save file 32 KB; Unit number 17 -> 20 kept after reload (the script's pixel
  compare said "other": the cursor blink). `synth-mister shot` now waits up to 60 s for the
  screenshot (Main_MiSTer took 30 s to fill it).
- **Build host:** seven parallel Quartus builds dropped winbuild off the network for ~30 min
  (the builds finished); run at most three at a time.

## P2K MAME prerequisites, steps 1-5 (2026-11-23, branch `p2k-sound-model`)

Plan: `cores/proteus/docs/p2k-mame-plan.md` (user decisions: no unit, the integer MAME model is the
definition, stereo mixdown, user flash SIMMs only).
- **Step 1, register contract:** every G20/H/G05 field the OS writes is [O] or a recorded [I] choice
  (`docs/p2k-findings.md` "Register contract"). New: G20 +0x00 bit 12 end select (read by the OS);
  H interpolators (+0x30-0x3E, live destinations +0x71/+0x73/+0x7C, approach time +0x44/+0x46);
  +0x7B bit fields and output channels; level law = H code decode; G05 send/dry bytes; filter
  modulation goes to +0x7C, not +0x3E. Corrections recorded for the selector table address, dead init
  code, port roles and the panel Enter button.
- **Step 2, integer model:** `p2k/model/p2k_sound.h` (BSD-3, used by MAME through patch 0004).
  Against its double-precision twin: SNR 65-85 dB on all 22 cases, no saturation.
- **Step 3, standalone model:** `p2k/model/p2k_render` replays `P2K_SNDLOG` and reproduces MAME's
  `P2K_SNDOUT` bit for bit on all 22 cases (`p2k_corpus.py model`).
- **Step 4, corpus:** 9 new cases (51 filter types, FX A/B algorithms, routing and sends, modulation,
  10-minute soak); all deterministic over two renders and feature-checked. Memory port: peak 256
  sample words per output sample (11.3 M words/s).
- **Step 5, user flash SIMMs:** `p2k/model/p2k_flash.h`; `tools/p2k_flash_test.py` drives the OS's
  RENAME FLASH SIMM page (scripted panel `P2K_KEYS`): ID 106 -> 108 written to Big and Baby,
  persists, plays after a restart. The OS has no sample import.
- **CI:** Proteus now runs the model/flash unit tests (`p2k/model/build.sh`, no ROMs).
- **Open:** step 6 (hardware); the arpeggiator did not start over SysEx [U]; listening check of the
  new model by the user.

## P2K reference corpus (2026-11-22, branch `p2k-ref-corpus`)

- **`cores/proteus/tools/p2k_corpus.py` and `tests/p2k_corpus/`:** 13 MIDI cases rendered by
  MAME with the stand-in Composer SIMM. Each records audio and the sound-chip register log
  (`P2K_TRACE`); the hashes and stats are in `manifest.json`.
- **Cases:** boot, 150-note polyphony, controllers, 16-channel multi, key range, and all 1,024
  stand-in presets (8 sweeps). About 980 s emulated, about 2 minutes with 6 jobs.
- **Checks:**
    - every case rendered twice with identical WAV and log hashes;
    - check mode exits 0 against the manifest;
    - the composer SIMM rebuilt from the WAVs is byte-identical.
- **Findings** (`docs/p2k-findings.md`):
    - voice allocation order (0-55, 64-119, then 56-63 and 120-127);
    - the 22 extra notes in the 150-note case took voices 0-21;
    - the stand-in presets' Key High is 91 (from the template preset).

## Iteration 11: P2K CPU against MAME (2026-11-22, branch `iter11-p2k-mame`)

- **Gate passed:** from reset to the OS 2.26 main screen, the RTL matches all 5,175,832
  non-DRAM word accesses of a 12 s MAME trace (MAME main screen at 10.5 s; demo SIMM).
  `tools/p2k_trace_cmp.py --nodram` on the saved RTL trace: `matched 5175832 word accesses;
  end of MAME trace`. RTL LCD: `C02A Vol127 Pan00  P2Ksy|     (not installed)`, as MAME.
- **The recorded divergence** (8080 to 0x00400000 vs MAME's read of 0x02000074) was a timer
  interrupt taken at a different instruction, not a CPU error. MAME's cycle counts do not model
  the MCF5206e, so the testbench now replays MAME's interrupt points (details in
  `cores/proteus/docs/p2k-core-cpu.md` §6).
- **RTL fix:** the interrupt-acknowledge cycle no longer goes to the board bus (`p2k_cpu.sv`).
- **Testbench/model changes:** replay mode with interrupt gating and timer warp, LCD and flash
  status replay, a flash command model, an MBAR write log, and P2K debug outputs on the TG68K
  kernel. MAME patch 0003 keeps vector-table reads in NODRAM traces.
- **Regression:** `p2k/sim/frame_test.py` passes.
- **Open:** a run before the warp change ended with a segfault after its STALL report (exit
  139); it did not recur in the later runs and was not reproduced.

## JV-880 silent with a MIDI controller (2026-11-22)

- **Symptom:** the user's Launchkey MK3 25 played nothing on the JV-880, though PREVIEW
  sounded. The SC-55 played the same input.
- **Cause:** the Launchkey sends MIDI clock (F8) continuously, about 48 bytes a second. The
  reference (Nuked-SC55) keeps bytes that arrive while the firmware's serial receiver is off
  (the first ~4 s of boot, until step 6,757,031) and delivers them later. After that backlog the
  JV-880 firmware ignores notes. The HDL did the same.
- **Evidence:**
    - The bench's new `--clock-hz` option sends clock from power-on. The HDL and the reference
      are identical, and both are silent.
    - With clock only while SCR.RE is on, the song plays.
    - On hardware, the new OSD diagnostics line read PCM peak 0000 with clock from boot, and
      0x100C without it.
- **Fix:** the JV-880 drops bytes while SCR.RE is off, as the H8/532 does. The SC-55 is
  unchanged.
- **Verification:**
    - Clock from power-on: lockstep identical, −14.1 dBFS; through the UART pin, −14.6 dBFS.
    - `jv880.mid` hash 73feda3c… unchanged. SC-55 MCU, system and board tests pass.
    - Windows build 28,452 ALMs, timing met. After the rebase onto #15 (2026-11-23): seed 5
      missed by 0.717 ns (the PCM address counter); seed 4 28,980 ALMs, +0.948 ns (seeds 1/3:
      +0.136/+0.466). SC-55 seed 2 32,863 ALMs, +0.344 ns. MiSTer smoke: JV-880 (autosave,
      NVRAM kept) and SC-55 pass, underrun and lag never.
    - Hardware: with clock from boot, the PCM peak reads 0x1078.
- **Also:**
    - `synth-mister shot` now waits until the PNG is written. It used to copy a 0-byte file
      under load.
    - The smoke test's NVRAM comparison ignores the cursor rows (blink).

## JV-880 NVRAM save file (2026-11-22)

- **Module:** `modules/synthframe/hdl/save/synthframe_nvsave.sv` (MIT) moves battery-backed RAM
  to and from the MiSTer save file on SD slot 0.
    - CONF_STR `FSC1` mounts `saves/JV-880/<pack>.sav`.
    - The module holds the core in reset until the restore is done.
    - On a save (OSD "Save NVRAM", or autosave when the OSD opens) it pauses the steps, waits
      until the MCU is idle, and writes 32 KB.
    - The MCU has a second copy of the NVRAM for the dump, so the CPU's fetch path is unchanged.
- **Bench (`--sav`, `--save-at`):**
    - Restore from the factory file: boots to "I-01:Syn Lead", 12 M steps identical.
    - A new file, and an empty NVRAM: saves requested in the middle of a step equal the model's
      NVRAM, including the 2 bytes the firmware wrote while booting.
- **Windows build:** JV-880 28,390 ALMs, block memory 74 %, timing met. SC-55 unchanged
  (33,160 ALMs).
- **MiSTer smoke (extended):**
    - The file is 32 KB, and exactly the changed byte differs (Unit number 17 → 20 at 0x0d).
    - After a reload the unit shows 20.
- **Load path:** a `boot.rom` autoload gets no save file from Main_MiSTer (`opensave` 0), so
  the pack is loaded from the OSD (and remembered).

## Iteration 12: JV-880 core, J1–J5 (2026-11-22, PR #8)

- **J1:** the SC-55's CPU, MCU, PCM and board blocks moved to `modules/roland`. SC-55 suite
  unchanged; Windows fit 33,215 ALMs, as in iteration 6.
- **J2:** MCU lockstep from an empty NVRAM through the factory preset: 21.3 M steps
  identical, NVRAM hash d5da7845…d207.
    - A one-clock race on GA line 5 (PCM IRQ) showed at step 24,709,513 of the render. It is
      fixed with a combinational IRQ0 term.
- **J3:** the system render of `jv880.mid` gives 73feda3c…bbbc.
- **J4:** the board bench from power-on (pack download, DDR3 wave ROM) gives the same hash in
  42.2 M steps, at 18.1 clocks a step of 30.
    - NVRAM loads are independent of the RAM clear.
    - Card RAM is cleared at reset.
- **Quartus:**
    - The first JV-880 build missed timing by 90 ns: a 48-bit divide for the 25/29 frame length
      fed the IRQ look-ahead. It is now a 64-entry table, and the render hash is unchanged.
    - Seed 4 then missed by 0.3 ns in the HDMI clock domain on both cores. Seed 2 meets timing
      for both (SC-55 33,160 ALMs, JV-880 28,180).
    - Quartus Lite has no Rapid Recompile. `winbuild.sh` now runs every build as a full compile
      in its own folder; 4 builds ran side by side in about 20 minutes.
- **J5 on the MiSTer:**
    - Boots to "Perf / I-01:Syn Lead [U001]".
    - 60 s of `jv880.mid` over MidiLink with no underrun or lag.
    - The panel keys and encoder respond as the reference's do. `jv880-nvram KEYS` showed that
      PATCH/PERFORM doesn't leave Utility or System in the reference either; pressing the mode
      key again does.
    - NVRAM saving to the SD card is not done.

||||||| parent of 1c62c0f (P2K CPU matches MAME from reset to the OS 2.26 main screen (iteration 11))
## Iteration 12 preparation: JV-880 (2026-11-22)

- **ROMs:** the user's dumps are in `nuked-sc55/firmware/Roland JV880/`. All five files match
  the Nuked-SC55 hashes for jv880-v1.0.0 and v1.0.1.
- **Reference:** the Nuked-SC55 backend's JV-880 support is about 50 lines over the SC-55 mk1:
    - the memory map (ROM2 pages 12/14, 32 KB NVRAM and card RAM);
    - IRQ0 instead of IRQ1;
    - no sub-MCU;
    - the PCM at 64 kHz with a 25/29 cycle ratio;
    - the LCD;
    - panel buttons on P7.
- **Render test:** `jv880.mid` has hash 73feda3c…bbbc, which needs a factory-preset NVRAM
  (d5da7845…d207).
- **`cores/sc55/ref/jv880_nvram.cpp`:** makes the factory-preset NVRAM headless and gets the
  documented hash d5da7845…d207. The reference renderer then gives the jv880.mid hash
  73feda3c…bbbc (`cores/sc55/tests/test_jv880_ref.py`, with `JV880_ROM_DIR`).
- **Why it first looked stuck:** the reference's `LCD_Write` keeps no LCD state without a
  display backend, so headless runs never show the screen. The firmware boots normally:
  "SYNTHESIZER MODULE / Roland JV-880", then "Internal RAM read error" for an empty NVRAM. It
  renders silence until a factory preset is loaded. The harness attaches a null LCD backend.

## Iterations 5 and 6: Synclavier and Proteus on the memory port (2026-11-22, PR #7)

- **Contract:** `req_len` for read bursts and write fills.
- **Synclavier `ddram_word`:** two clients.
- **Proteus `snd_ctrl`:** four clients.
- **Gate:** the evidence is in PR #7. Simulations are identical to main; the Windows builds
  meet timing; all three MiSTer smoke tests pass. The Proteus worst case went from 889 to 1007
  clocks of 2048.
- **PyPy:** 51 unclosed file writes fixed.
- **Process:**
    - Since 2026-11-22, PRs are gated on Windows builds. GitHub Build RBF runs only on v* tags.
    - The Windows builds found two errors CI would have reported 30 minutes later: Quartus 17's
      localparam rule, and the Proteus `files.qip`.

## Iteration 4: memory port, SC-55 (merged as #5)

- **Board bench:** identical in 7 DDR3 timing variants, with the same 6,244,003 reads.
- **MiSTer:** 100 s with no underrun; LCD hash unchanged.

## Iteration 2: shared `audio_fifo` and `midi_rx` (2026-11-22, branch `iter2-shared-audio-midi`)

- **`modules/synthframe/hdl/audio/synthframe_audio_fifo.sv`:** the Synclavier's
  `audio_fifo`. The SC-55 copy was identical apart from a comment.
- **`modules/synthframe/hdl/midi/synthframe_midi_rx.sv`:** the SC-55's `midi_rx`, with
  `synthframe_midi_uart_rx`. The Synclavier's `midi_in.sv` had the same receiver logic inline
  and now instantiates it.
- **Updated:** both cores' `files.qip`, `sta_paths.tcl`, the SC-55 `sim/CMakeLists.txt`, the
  Synclavier `sim/lockstep.py`, `tests/hdl/test_board.py` and two testbenches.
- **Local tests:**
    - Synclavier: `tests/hdl/test_board.py` 30 OK, `test_lockstep.py` 11 OK, hygiene 3 OK.
    - SC-55: `test_system_lockstep` 4 passed and 2 long tests skipped (as before);
      `test_board_units` 1 passed; hygiene 3 passed.
- **Pending:** CI (tests, fit resources against iteration 0) and the hardware smoke tests on
  the CI RBFs.

## Iteration 1: one MiSTer test harness (2026-11-22, branch `iter1-mister-harness`)

- **`modules/synthframe/bin/synth-mister`:** lock, backup, install, release, restore, load,
  shot, peek, midi-stop, sh.
- **Backup on the MiSTer:** `/media/fat/_synth_backup/20261122`, 45 files: 17 RBFs and 28
  MGLs of this project.
- **Per-core smoke tests:** `cores/sc55/scripts/mister/smoke.sh`,
  `cores/proteus/tools/mister_smoke.sh` and `cores/synclavier/scripts/mister/smoke.sh`.
- **Gate passed:** the CI-built RBFs from `main` (run 45067354440) against the installed
  releases.

  | Core | Release | CI build |
  |---|---|---|
  | SC-55 | 12,813 bytes in 100 s; no underrun or lag; boot LCD 67a34ac6… | same; boot LCD 67a34ac6… in 2 of 2 repeats |
  | Proteus | 60 s soak: 0 missed, ratio 1.008, worst 889/2048 | 0 missed, ratio 1.008, worst 899/2048 |
  | Synclavier | Main Menu, underrun 0000, menu sha 8bd1f607… | identical |

- **Screenshots are 640×480 (Proteus 320×240):** each core's frame, scaled.
- **Timing fixes:**
    - The first CI SC-55 run had a different LCD hash: the screenshot came before the level
      bars were drawn. The wait after the GM reset went from 1 s to 2 s.
    - The Proteus boot hash is information only; it follows the cursor's blink phase.

## Iteration 0: publish and root CI (started 2026-11-22)

- **Monorepo built:** import of the three repos, 377 commits. `modules/nuked-sc55` is a
  submodule, with a symlink at the SC-55's old path.
- **Local tests from `cores/`:**
    - Synclavier: all `tests/`, `tests/hdl/` and `tests/sim/` suites pass.
    - SC-55: the sim build and 10 test files pass. `test_ref_patch` skipped because
      `SC55_REF_BUILD` was not set (CI sets it). `test_system_lockstep` was still running.
- **Quartus file references:** all resolve in all three cores.
- **Root workflows:** `tests.yml` and `build-rbf.yml`, filtered by core path. The release
  workflows are parked in `.github/disabled/`.
- **Pushed** to https://github.com/gmcnaught/synthcores (public).
- **Correction:** the import used local `main` branches that were behind GitHub. SC-55 was 1
  commit behind (release RBF) and Synclavier 3 (release methodology, release RBF, PR #89). No
  HDL changed.
    - Merged on branch `iter0-upstream-sync`. Re-filtering is deterministic, so the shared
      history matched and only the 4 commits came in.
    - `import-cores.sh` now refuses a branch that is behind its remote.
- **Baselines for the fit comparison:** old-repo Build RBF runs 44834929645 (SC-55 500a4cc),
  44778489707 (Synclavier e80c4fe) and 44867771202 (Proteus 6a9df91). These match the
  imported HDL.
- **Gate passed (2026-11-22):**
    - CI tests are green for SC-55 and Synclavier.
    - Build RBF run 45067354440 on `main` built all 3 cores. Their fit summaries (ALMs,
      registers, block memory, DSP, pins, PLLs) are identical to the baselines:

      | Core | ALMs | DSP |
      |---|---|---|
      | SC-55 | 32,963 | 99 |
      | Proteus | 24,249 | 40 |
      | Synclavier | 26,568 | 52 |

    - PR #1 merged as 39cd1d7.
    - The first push's builds were cancelled by the concurrency group when I started a run by
      hand. A single hand-started run then built all three.
