# Session log

One entry per iteration of `SESSION-PLAN.md`, newest first.

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
- **Pending:** CI green; fit resources compared.
