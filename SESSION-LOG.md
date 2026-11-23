# Session log

One entry per iteration of `SESSION-PLAN.md`, newest first.

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
