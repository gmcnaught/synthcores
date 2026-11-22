# Session log

One entry per iteration of `SESSION-PLAN.md`, newest first.

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
