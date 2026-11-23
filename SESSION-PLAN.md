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
    - this project's cores (`SC55_*`, `Proteus_*`, `Synclavier*` in `_Devices`, `_Other`,
      `_Computer`);
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

Order: 0 → 1 → 2 and 3 → 4 → 5 and 6 → 8 → 12 (JV-880) → 13 (CZ-101, brief §8's next Phase 1 core; the MT-32 follows it). 9 and 11 fill build wait time. 7 and 10 (M64) are deferred until the open source community answers the M64's open questions (user, 2026-11-22).

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
