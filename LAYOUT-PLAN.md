# Monorepo layout modelled on jtcores, JTFRAME and jtbin

As of 2026-11-22. Companion to `HARDWARE-PLAN.md`.

This directory becomes one git repository, laid out like jotego's `jtcores`:

- a shared framework under `modules/`;
- third-party chips and CPUs as submodules;
- one folder per core under `cores/`, each with the same subfolders;
- one folder per platform under the framework's `target/`;
- release binaries in a separate repo, like `jtbin`.

## What carries over from JTFRAME, and what doesn't

- **The layout and conventions carry over:**
    - `setprj.sh` with environment variables;
    - `cores/<core>/{cfg,hdl,ver,doc}`;
    - `modules/<framework>/target/<platform>/`;
    - per-core config files that the build reads;
    - a separate binaries repo.
- **The JTFRAME tool itself does not.** It is built for arcade games. It generates SDRAM wiring,
  video and MRA files, and has targets only for Intel and Gowin parts (MiSTer, MiST, SiDi,
  Pocket, NeptUNO). These cores use DDR3 or PSRAM, show an LCD rather than game video, and need
  a Vivado target.
- **The JTFRAME PSRAM controller doesn't fit the M64.**
  `modules/jtframe/hdl/psram/jtframe_psram_rand.sv` drives the Pocket's asynchronous AS1C8M16.
  The M64's AP Memory chips have a different interface.
- **Individual JTFRAME modules can still be borrowed** (clock enables, filters, the UART). They
  are GPL-3.0-or-later; see Licences below.

## Layout

```
synth-projects/                     one repo: synthcores
  setprj.sh                         sets SYNTHROOT, CORES, SYNTHFRAME, MODULES, SYNTHBIN
  modules/
    synthframe/                        the framework
      hdl/
        mem/                        memory-port contract + DDR3 backend (from ddram_word, wave_ddr)
                                    + line cache (rom2x_cache) + PSRAM backend (new)
        panel/                      panel-as-data: HD44780 model (from Proteus), small framebuffer,
                                    key/encoder events; MiSTer renderer (generalised lcd_video)
        midi/                       midi_rx (SC-55 and Synclavier copies today), midi_tx
        audio/                      audio_fifo (copied in 2 repos today), resampler, output gain
        load/                       block-loader contract; MiSTer ioctl adapter
        clocking/                   clock enables, step pacer
      target/
        mister/                     the one Template_MiSTer sys/, emu wrapper, qsf template, build.sh
        xcau15p/                    top wrapper, Vivado tcl, UART debug; named after the FPGA, not a board
          boards/m64.xdc            M64 pinout from its schematic; the own board's XDC goes alongside
      ver/                          framework benches (random-latency memory, MIDI, panel)
      bin/                          synthcore (build a core for a target), synthsim, deploy to MiSTer
    fx68k/                          submodule (jtcores uses jtfpga/fx68k)
    tg68k/                          TG68K.C with the P2K changes (LGPL-3), now in proteus/p2k/rtl/cpu/tg68k
    nuked-sc55/                     submodule, reference model (now cores/sc55/third_party/nuked-sc55)
  cores/
    sc55/      {cfg,hdl,ver,tools,tests,doc}
    synclavier/
    proteus/                        Proteus/1 XR
    p2k/                            Proteus 2000, split out of proteus/p2k once the CPU boots
    jv880/ …                        new cores start in this layout
  release/                          gitignored; synthbin is the published copy
```

## Per-core `cfg/` files

| File | Contents | Used by |
|---|---|---|
| `core.yaml` | name, top module, clock-enable rates, memory ports (name, size, width), panel type (HD44780 rows×cols, or framebuffer w×h), MIDI ports, audio outputs, targets | `synthcore` writes the target wrapper's parameters and macros |
| `files.yaml` | HDL file list, with `synthframe:` and `modules:` references | `synthcore` writes `files.qip` (Quartus), a Vivado tcl list, Verilator `.f` |
| `macros.def` | Per-target macros in JTFRAME syntax (`[mister]`, `[xcau15p]`) | today's qsf macros, e.g. `MISTER_DISABLE_ALSA`, `MISTER_DEBUG_NOHDMI` |
| `reg.yaml` | the core's reference tests (lockstep, `0A.mid` hash) | CI |

Write `synthcore` in Python, like the existing tools in all three repos. Go, which JTFRAME uses, is
not needed.

## Binaries repo (synthbin, like jtbin)

- Laid out as a MiSTer SD card: `_Devices/`, `_Computer/`, `games/<core>/` READMEs, MGLs.
- Adds `xcau15p/` for the `.bit` files.
- Holds the downloader database. The SC-55 is installed today from its own repo's database, so
  existing users need a pointer to the new one.

## What the trial import found

| Check | Result |
|---|---|
| History import (`scripts/import-cores.sh`, trial in the scratchpad) | 376 commits (58 SC-55, 268 Synclavier, 46 Proteus); `git log` and `git blame` work on the new `cores/*/` paths |
| Template_MiSTer `sys/` copies | SC-55 and Synclavier identical; Proteus differs only in `arcade_video.v`. One shared copy is possible after checking that difference. |
| Copied code | `audio_fifo.sv` and `midi_rx.sv` in the SC-55 are marked "From synclavierii" |
| Submodules | only `cores/sc55/third_party/nuked-sc55`; the import leaves it without a root `.gitmodules` (to fix by moving it to `modules/`) |
| Paths in CI workflows | none cross the repo root; each workflow moves under the root `.github/` as one job per core |
| Untracked bulk (not imported) | `nuked-sc55/out` 2.8 GB, `emu-proteus-2k-MiSTer/build` 2.2 GB, its MAME checkout 1.1 GB, ROMs 730 MB, `synclavier-ii-MiSTer/out` 621 MB. These stay gitignored. |

## Licences

| Part | Licence |
|---|---|
| SC-55 | GPL-2.0-or-later |
| Proteus | GPL-3.0 |
| Synclavier | MIT |
| TG68K | LGPL-3 |
| JTFRAME modules | GPL-3.0-or-later |

Each core keeps its own `LICENSE`, as now. The repo root has no single licence; every file
carries an SPDX header.

**Decision: `synthframe` is MIT.**

- **It combines with every core and with any licence a future core picks**, including
  GPL-2.0-only. The modules extracted first (`audio_fifo`, `midi_rx`) are already MIT, from the
  Synclavier.
- **Apache-2.0 was not chosen.** It adds a patent grant, but it is incompatible with GPL-2.0-only.
  19 of the 32 Template_MiSTer `sys/` files carry no "or later" wording.
- **GPL-3 code stays out of `synthframe`.** Borrowed JTFRAME modules (GPL-3.0-or-later) go under
  `modules/` as separate GPL-3 folders, used only by GPL-compatible cores (SC-55, Proteus). A
  Synclavier build that uses one becomes a GPL-3 bitstream.

## Migration order

Each step has a gate. Nothing moves until the step before it passes.

Status on 2026-11-22:

- **Step 1 is done:** the P2K work is committed as `1f2cfdb`.
- **Step 2 has started:**
    - 377 commits imported into this directory.
    - The `nuked-sc55` submodule is at `modules/nuked-sc55`, with a symlink at its old path.
    - The test gate is still pending.

1. **Commit the P2K work.**
    - It is currently 5 uncommitted files on `p2k-research`.
    - Another session's simulation was running in that tree.
    - Gate: `git status` is clean in all three repos.
2. **Import.** Run `scripts/import-cores.sh` into a new directory, add the root `.gitmodules`,
   `setprj.sh` and `.gitignore`, and move the ROMs, `out/` and `build/` folders across. The old
   repos stay where they are, untouched.
    - Gate: every core's existing tests pass from `cores/<core>/`, and the CI builds all three RBFs.
3. **Shared `sys/` and target wrapper.** Move the one `sys/` copy to
   `modules/synthframe/target/mister/`.
    - Gate: the Quartus fit summaries match the pre-move builds.
4. **Extract the shared modules one at a time:** `audio_fifo`, then `midi_rx`, then `hd44780`,
   then the memory port (HARDWARE-PLAN Phase 1, steps 2–3).
    - Gate: the lockstep tests and output hashes stay bit-identical after each move.
5. **`synthcore` and `cfg/`.** Generate `files.qip`, the qsf macros and the Vivado file lists from
   `cfg/`. The `xcau15p` target, with the M64 board file, is added here.
6. **GitHub.**
    - Create the monorepo and synthbin.
    - Archive the three old repos, each with a README pointing to the new location.
    - Move the SC-55 downloader database.

## Not decided

- Whether to retire the three GitHub repos (step 6) or keep them as read-only mirrors.
