# Autonomous session plan

As of 2026-11-22. The session works through the iterations below in order. Each one ends at a
gate that passes or fails on evidence: a test result, a hash, a fit report or a screenshot.

Assumptions:

- **GitHub:** this repo is public on GitHub. CI runs the tests and the Quartus builds
  (`Build RBF` takes about 35 minutes per core).
- **MiSTer:** reachable at `root@192.168.20.62` with SSH key authentication. Checked 2026-11-22:
  SSH and FTP are open; SC55_20261120, Proteus_20261121 and Synclavier_20261118 are installed;
  MidiLink is running.

## Rules

- **One branch and one PR per iteration.** A PR merges once its tests pass and its gate
  passes. Since 2026-11-22 (decided by the user) that means Quartus builds on the Windows build
  host (`scripts/build-hosts/winbuild.sh`) and the MiSTer smoke tests. GitHub's Build RBF runs
  only for releases. The PR body or a comment records the evidence: test output, hashes, fit
  numbers, smoke results.
- **Tests stay bit-identical.** Lockstep tests, output hashes and the LCD pixel tests must not
  change. A refactor that changes any of them is wrong until proven otherwise.
- **The MiSTer is shared hardware. Touch only:**
    - this project's cores (`SC55_*`, `Proteus_*`, `Synclavier*` in `_Devices`);
    - `games/{SC-55,Proteus,Synclavier}`;
    - `/tmp`.
  Before replacing anything, copy it to `/media/fat/_synth_backup/<date>/`, and restore the
  installed releases at the end of the session. Never edit `MiSTer.ini`. Never reboot. Stop
  MidiLink only for a test and restart it afterwards. If the MiSTer stops answering, stop the
  hardware gates and continue with simulation-only iterations.
- **Not touched without the user:** the three old GitHub repos (archive, README or release
  changes), the SC-55 downloader database, `synthframe` names that leave the repo, and anything
  that costs money.
- **Stuck rule:** if a gate still fails after two fix attempts, write the diagnosis to
  `SESSION-LOG.md` and move on to the next iteration that doesn't depend on it.
- **Status ledger:** `SESSION-LOG.md` gets one entry per iteration (start, gate result, PR,
  open items) so progress can be read without the transcript.
- **ROMs** come from the untracked folders of the old repos, through `SC55_ROM_DIR`, the
  Proteus system image and the Synclavier disk images. They are never committed. The hygiene
  tests run in CI.

## Iterations

| # | Iteration | Depends on | Hardware | Gate |
|---|---|---|---|---|
| 0 | Publish and root CI | — | — | CI green for all 3 cores; fit resources equal the last release builds |
| 1 | One MiSTer test harness | 0 | yes | all 3 CI-built cores pass their smoke test on the MiSTer |
| 2 | Shared `audio_fifo`, `midi_rx` | 1 | yes | lockstep bit-identical; smoke tests pass |
| 3 | One shared `sys/` | 1 | yes | fit resources equal ±1%; smoke tests pass |
| 4 | Memory port + DDR3 backend, SC-55 | 2, 3 | yes | `0A.mid` hash unchanged at latencies 3, 7 and random; 100 s real-time playback with no underrun |
| 5 | Synclavier poly on the memory port | 4 | yes | `tests/hdl` pass; DDR3 late-read count no worse |
| 6 | Proteus/1 `snd_ctrl` on the memory port | 4 | yes | sound-chip tests bit-exact; 60 s soak, 0 missed deadlines |
| 7 | ~~PSRAM model + controller (simulation)~~ deferred: M64 work waits (user, 2026-11-22) | — | — | — |
| 8 | Panel as data (HD44780 model, renderer) | 3 | yes | SC-55 LCD pixel test; Proteus display tests; screenshots match |
| 9 | `cfg/` files and the `synthcore` generator | 3 | — | generated `files.qip` and qsf macros byte-identical to today's |
| 10 | ~~`xcau15p` size estimate~~ deferred: M64 work waits | — | — | — |
| 11 | P2K CPU against MAME | 0 | — | bus trace matches MAME up to the main screen |
| 12 | JV-880 core on the MiSTer | 4, 8 | yes | H8/532 firmware lockstep and `jv880.mid` render hash `73feda3c…bbbc` against Nuked-SC55 (`jv880-v1.0.1`); plays MIDI on the MiSTer with no underrun |
| 13 | CZ-101 core on the MiSTer (`cores/cz101/docs/plan.md`, C0–C5) | 1, 2 | yes | uPD7811 trace and uPD933 samples identical to MAME (`cz101`) over generated test ROMs, firmware boot and a MIDI corpus; plays MIDI on the MiSTer with no underrun |
| 14 | P2K sound HDL against `p2k_render` (simulation) | 11 | — | `snd.raw` bit-identical to `p2k_render` on all 22 corpus cases: 14a with `--nofilter --nofx`, 14b with `--nofx`, 14c in full |
| 15 | CZ-101 and JV-880 follow-ups | 12, 13 | yes | listed per item below; reference tests unchanged |
| 16 | P2K sound HDL for the Cyclone V | 14 | — | `snd.raw` still bit-identical on all 22 cases × 3 modes through a DDR3 model; standalone Quartus 17 build meets timing at 100 MHz; worst sample within the 100 MHz budget (2267 clocks) |
| 17 | MT-32 core on the MiSTer (`cores/mt32/docs/plan.md`, M0–M6) | 1, 2 | yes | MCS-96 trace, LA32 and reverb samples identical to Nuked-MT32 (`5b4e239`, firmware 2.07) over generated test ROMs, firmware boot and a MIDI corpus; plays MIDI on the MiSTer with no underrun |
| 18 | P2K MiSTer core (`cores/p2k/docs/plan.md`, P0–P6) | 11, 16 | yes | OS 2.26 boots to its main screen in the system bench and on the MiSTer with the Composer stand-in SIMM; the bench's sound-write log replayed by `p2k_render` equals the bench audio; plays MIDI on the MiSTer; user data in flash survives a reload |
| 19 | P2K downloads straight to DDR3 | 18 | yes | the loader bench places packs and SIMMs identically by bytes and direct (3 seeds and a 34 MB SIMM); Quartus build meets timing; the MiSTer smoke test passes, with the main screen well under the 110 s of iteration 18 |

Order: 0 → 1 → 2 and 3 → 4 → 5 and 6 → 8 → 12 (JV-880) → 13 (CZ-101, brief §8's next Phase 1 core; the MT-32 follows it). 9 and 11 fill build wait time. Overnight 2026-11-23 (user): 14, with 15 filling build and simulation wait time. 7 and 10 (M64) are deferred until the open source community answers the M64's open questions (user, 2026-11-22).

### 0. Publish and root CI

- Create the public GitHub repo and push `main`.
- Move the per-core workflows to the root `.github/workflows/`:
    - `tests.yml` runs a matrix over cores, filtered by path so a change in one core doesn't
      rebuild the others;
    - `build-rbf.yml` does the same for Quartus.
- Release workflows stay disabled until the user decides about synthbin.
- The SC-55 tests still to finish locally (started 2026-11-22) are part of this gate.
- **Gate:** all three cores' CI jobs are green. Each RBF's fit summary (ALMs, M10K, DSP) equals
  the numbers in brief §3.

### 1. One MiSTer test harness

- Merge `cores/sc55/scripts/mister/sc55_run.sh`, `cores/synclavier/scripts/mister/mister_run.sh`
  and `cores/proteus/tools/mister_system.sh` into `modules/synthframe/bin/synth-mister`. It does:
    - back up, install the RBF and support files, load the core;
    - send MIDI to `/dev/ttyS1`;
    - take a screenshot;
    - read a DDR3 address through `/dev/mem`;
    - restore.
- Download the RBFs from the CI artifacts with `gh run download`.
- **Gate:**
    - SC-55: the LCD in the screenshot matches the reference renderer after the `0A.mid` intro.
    - Proteus: `mister_soak.py` for 60 s, 0 missed deadlines.
    - Synclavier: the 5.3 MGL reaches the Main Menu screenshot.

### 2. Shared `audio_fifo` and `midi_rx`

- Move both modules to `modules/synthframe/hdl/{audio,midi}` with MIT SPDX headers. The SC-55
  copies are marked "From synclavierii".
- **Gate:** the SC-55 and Synclavier lockstep and board tests are unchanged, and the iteration 1
  smoke tests pass on CI builds.

### 3. One shared `sys/`

- Keep one Template_MiSTer `sys/` in `modules/synthframe/target/mister/sys`. Resolve the Proteus
  `arcade_video.v` difference by taking the newer upstream file and checking its git history.
- **Gate:** fit resources within ±1% for each core, and the smoke tests pass.

### 4. Memory port and DDR3 backend, SC-55 first

- Define the contract: a request carries a tag, an address and a length; responses return in
  order after any delay.
- The DDR3 backend comes from `wave_ddr.sv` and `ddram_word.sv`. `rom2x_cache` becomes the line
  cache.
- Add a random-latency bench in `modules/synthframe/ver`.
- **Gate:**
    - SC-55 system lockstep (`0A.mid`, mk1 and mk2) has an unchanged hash at latencies 3, 7 and
      random.
    - Real-time playback for 100 s on the MiSTer, with no underrun (the existing board metric).

### 5. Synclavier poly on the memory port

- **Gate:** `tests/hdl` and `tests/sim` pass. On the MiSTer, poly playback of a sound-file demo
  MGL shows a DDR3 late-read count no higher than before.

### 6. Proteus/1 `snd_ctrl` on the memory port

- Split `snd_ctrl`'s four DDR3 clients (load, capture, status, sample reads) into four port
  clients and an arbiter.
- **Gate:** the `sim/` sound-chip tests are bit-exact, and a 60 s soak shows 0 missed deadlines.

### 7. PSRAM model and controller, simulation only

- Write a Verilator timing model of the APS256XXN and APS512XXN from the AP Memory datasheets:
  chip-select time limit, page-boundary penalty, fixed latency. Then the controller, behind the
  memory port.
- **Gate:** the SC-55 system lockstep hash is unchanged through PSRAM, and the model reports
  sustained random 16-byte reads per second (compare with the brief's estimate of 8–12 M/s).

### 8. Panel as data

- Move the Proteus `hd44780.sv` to `modules/synthframe/hdl/panel`. The SC-55 `lcd_video` becomes a
  generic MiSTer renderer that reads panel data.
- **Gate:** the SC-55 LCD pixel test is unchanged, the Proteus display tests pass, and the
  hardware screenshots match the iteration 1 ones.

### 9. `cfg/` and `synthcore`

- Write `files.yaml`, `macros.def` and `core.yaml` per core, and a Python `synthcore` that
  generates `files.qip` and the qsf macros.
- **Gate:** the generated files are byte-identical to the hand-written ones they replace.

### 10. `xcau15p` size estimate

- Run Yosys `synth_xilinx -family xcup` on the SC-55, with sv2v or yosys-slang for the
  SystemVerilog.
- This is not Vivado. It is labelled as an estimate and only replaces the ±30% conversions in
  brief §3 until a Vivado build exists.
- **Gate:** a report with LUT, BRAM and DSP counts per module.

### 11. P2K CPU against MAME

- This needs Docker (colima) for the GHDL step of `p2k/sim/build_cpu.sh`.
- Start from the last divergence: at the 2 ms tick, the RTL writes 8080 to 0x00400000
  (pc 02003B04) while MAME reads 02000074 (pc 02003A94).
- **Gate:** `tools/p2k_trace_cmp.py` finds no divergence up to the MAME main screen.

### 13. CZ-101

- Scope, hardware map and milestones C0–C5: `cores/cz101/docs/plan.md`.
- Reference: MAME `cz101` (BSD-3-Clause), built with logging patches in `../mame-cz101`.
- ROMs: dbwbp.com `Casio CZ-101 EPROM Firmware.zip`, downloaded by the user. C1 and C3 run on
  generated test ROMs and do not wait for it.
- **Gate:** C4's corpus hashes identical to MAME's, and C5 on the MiSTer.

### 14. P2K sound HDL (overnight 2026-11-23)

Definition: `cores/proteus/p2k/model/p2k_sound.h` (integer; decisions in `docs/p2k-mame-plan.md`).
The HDL replays a corpus case's `snd.log` and must give `p2k_render`'s `snd.raw` bit for bit.
No MiSTer top and no Quartus build in this iteration; the CPU (iteration 11) is not connected.

- **0. Inputs.** The corpus outputs and SIMM images are not on disk. Rebuild them:
    - `big.bin`, `baby.bin` with `tools/p2k_spec_from_dir.py` + `tools/p2k_build_simm.py`;
      check their SHA-256 against `manifest.json` `inputs.simm`.
    - `flash_clean.nv`: one `P2K_CLEAN=1` boot.
    - MAME from `mame/patches/BASE` + 0001-0004. The `proteus` binary in the old repo is from
      07:14, before today's patch 0001 updates: rebuild it if `inputs.mame_*` differs.
    - `tools/p2k_corpus.py run --jobs 10` must match the manifest, and `p2k_corpus.py model`
      must report `p2k_render` identical on all 22 cases.
    - Gate: all 22 cases match. If they don't after two attempts, stop iteration 14 and log the
      difference.
- **1. Bench.** `p2k/sim/p2k_snd_tb.cpp` (Verilator): reads `snd.log`, applies each write
  before its sample index with the `COMBINE_DATA` mask, serves sample memory from the SIMM images
  through a memory model with a configurable latency, writes `snd.raw`, and on a difference prints
  the first sample and, with `--taps`, per-voice values. `p2k_render` gains a matching
  `--taps` dump (debug only; its audio must not change: the 22 hashes are rechecked).
- **14a. G20 voices + H level interpolators + mixer** (`p2k/rtl/snd/`): 128 voices
  time-multiplexed, 4.12 phase, end select and loop jumps, linear interpolation Q8, the level
  interpolators (32 << k samples), level gain decode, per-bus mix by H output channel, output
  `rsh(sum, 9)` clamped. Gate: identical to `p2k_render --nofilter --nofx` on all 22 cases.
- **14b. H filter:** three DF-II biquads per voice, Q8 signal and state saturated to 48 bits,
  Q28 coefficients from the cutoff interpolator, 12th-order chains through the previous slot's
  block, route byte +0x7B. Gate: identical to `--nofx` on all 22 cases.
- **14c. G05 effects** (`fx_int`), if time is left: identical in full on all 22 cases.
- **Run time:** about 66 M samples for the 21 short cases and 26 M for `soak`. The cases run
  as parallel jobs; `soak` runs last and its result can come in after the PR is opened.
- **Recorded as measurements, not gates:** clocks per sample against the 44.1 kHz budget at
  the planned core clock, sample-memory reads per sample (DDR3 sizing for the MiSTer top),
  multipliers and block RAM per stage.
- **Merge:** with 14a or 14b complete, CI green and the evidence in the PR (self-merge, user
  2026-11-23). 14c can follow as its own PR.

### 15. CZ-101 and JV-880 follow-ups (overnight 2026-11-23)

Fills iteration 14's wait time. One branch and PR per item. MiSTer rules as above, through
`synth-mister`'s lock.

- **CZ-101 edited patch survives a reload:** edit a patch from the panel keys, let the save
  write, reload the core, check the LCD and the `.sav` hash. A fix needs C4 lockstep unchanged,
  the Windows build meeting timing and the smoke test passing.
- **CZ-101 pitch wheel:** `cz101_board.pitch_wheel` (AN1, 0x7F centre) is driven from a MiSTer
  analog axis. Gate: the firmware's pitch changes in simulation; smoke test passes.
- **CZ-101 MIDI out:** the uPD7811 serial TX goes to the UART TX (MidiLink). Gate: the TX bytes
  in simulation equal MAME's MIDI out for the C4 case; on the MiSTer, the bytes are seen on
  `/dev/ttyS1`.
- **JV-880 card RAM (pages 14/15):** check whether the firmware reads it without a card (J2
  trace). Remove the 32 KB only if the `jv880.mid` render hash stays 73feda3c…bbbc and the
  board bench is identical.

### 16. P2K sound HDL for the Cyclone V (2026-11-24, user)

Iteration 14's datapath does not synthesize: Quartus 17 stops on the effects delay lines (4.4 Mbit
cleared in one clock), and the H RAM is read 4–6 times a clock with 128-bit filter products. This
iteration restructures it for the DE10-Nano without changing a single output sample.

- **Sample and effects memory through the synthframe memory port** (`synthframe_mem_ddr3`
  contract): one 64-bit read per voice (two when the pair straddles a word); the effects delay
  lines live in DDR3 and read as zero until written after a reset or a resize.
- **Block RAM with registered reads** for every register file, with shadow copies where more
  than one unit reads.
- **Filter as a pipelined unit** shared by up to 8 voices in flight; the products split over
  27×27 DSP blocks.
- **Bench:** the Verilator bench drives a DDR3 model (random BUSY, in-order latency 8–24
  clocks). Sample-memory words are served from the SIMM images, the effects region starts with
  random data.
- **Gates:**
    - `tests/p2k_snd` (CI, random logs) identical in the three modes, also at other DDR3 timings.
    - Corpus: all 22 cases identical in the three modes (`tools/p2k_snd_check.py`).
    - Standalone Quartus 17 compile of the sound engine with `synthframe_mem_ddr3` on the
      5CSEBA6U23I7: timing met at 100 MHz; ALMs, M10K and DSP recorded.
    - Worst clocks per sample on the corpus and the 128-voice random log at most 2267 (100 MHz).
- **Not in this iteration:** the MiSTer top (CPU, MIDI, sample loading) and hardware tests.

### 17. MT-32 (2026-11-24, user)

- Scope, hardware map and milestones M0–M6: `cores/mt32/docs/plan.md`.
- Reference: Nuked-MT32 (GPL-2.0-or-later; MCS-96 code from MAME, BSD-3-Clause), submodule
  `modules/nuked-mt32` at `5b4e239`. MAME's `mt32` driver has no sound.
- ROMs: the user's archives (2026-11-24), checked against MAME; tests use `MT32_ROM_DIR`.
- Runs alongside iteration 16: M0, M1, M3 and M4 need no hardware. The Mac's CPUs are shared
  with the P2K corpus gate; the MiSTer only through `synth-mister`'s lock.
- **Gate:** M5's corpus hashes identical to Nuked-MT32's, and M6 on the MiSTer.

### 18. P2K MiSTer core (2026-11-24, user: "finish this out to a full usable mister core")

Plan and design: `cores/p2k/docs/plan.md`. A new core folder `cores/p2k` (LAYOUT-PLAN) holds the
board, the MiSTer top and the system bench; the CPU and sound RTL stay in `cores/proteus/p2k/rtl`
for now. All memory is in DDR3 (no SDRAM board needed); the CPU reads it through a block-RAM cache.
Steps P0-P6 and their gates are in the plan. User flash SIMMs (G20 port B writes) are P6 and may
move to a later iteration.

### 19. P2K downloads straight to DDR3 (2026-11-24, user: "speed up the load")

Iteration 18's SIMM download took about 100 s: Main_MiSTer sends a download without a load address
one byte at a time, with a handshake per byte. The pack and SIMM entries now carry the load
address 0x3C000000, so Main_MiSTer writes the file into DDR3 itself; the loader copies it to its
places (byte-swapped) after the download. File formats unchanged; the byte path stays as the
fallback. Design and results: `cores/p2k/docs/plan.md`.

## Not in this session

- **Vivado builds and loading the M64:** no x86-64 Linux host, and AMD's download needs an
  account login.
- **New cores (JV-880 and later):** they start after iterations 4 and 8 exist.
- **GitHub repo retirement, synthbin and downloader moves:** these need the user's decision.

## Needed from the user before starting

1. **Repo:** decided, `gmcnaught/synthcores` (framework synthframe, binaries synthbin). The owner is assumed to be the logged-in GitHub account.
2. **Self-merge:** confirm the session may merge its own PRs once the gates pass.
3. **MiSTer use:** confirm the core files listed in Rules may be replaced during the session,
   and that the session restores the installed releases at the end.
