# Session log

One entry per iteration of `SESSION-PLAN.md`, newest first.

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

    - PR #1 merged as 3d24918.
    - The first push's builds were cancelled by the concurrency group when I started a run by
      hand. A single hand-started run then built all three.
