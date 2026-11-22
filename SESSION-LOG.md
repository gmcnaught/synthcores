# Session log

One entry per iteration of `SESSION-PLAN.md`, newest first.

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
