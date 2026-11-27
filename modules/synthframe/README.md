# synthframe

The framework the cores share (MIT).

| Folder | Contents |
|---|---|
| `hdl/` | MIDI in/out, audio FIFO and resampler, memory ports (`mem/`: arbiter, DDR3), NVRAM save (`save/`), panel modules (`panel/README.md`) |
| `target/mister/` | The MiSTer framework (`sys/`), the qsf template, the shared `build.sh` each core's `mister/build.sh` calls (`SEED=N` sets the fitter seed) and the `.srf` the cores link to (`README.md`) |
| `bin/synthcore` | Writes each core's Quartus project and `sim/files.f` from `cores/<core>/cfg/`; `synthcore deps CORE` lists the directories a core takes from other cores |
| `bin/synth-mister` | Drives the MiSTer for the smoke tests (lock, backup, install, load, screenshot, restore); the SD-card side is `bin/synth-mister-fat.sh` |
| `bin/hdl-lint` | Lint gate: `verilator --lint-only -Wall` on each core's MiSTer top (`emu`); fails on any warning that no waiver in `cores/<core>/cfg/lint.vlt` or `modules/<module>/cfg/lint.vlt` covers (see "Lint waivers" below) |
| `bin/counter-audit` | Uptime-counter audit: every register of 32 bits or more named `*cycle*`, `*tick*`, `*time*`, `*deadline*`, `now` or `next_*` in a core's MiSTer build needs a row in `cores/<core>/cfg/counters.yaml` (or `modules/<module>/cfg/counters.yaml`): width, clock, time to wrap, how the wrap is handled, the test that crosses it |
| `lint/` | `common.vlt` (waivers for the MiSTer framework `sys/`) and lint stand-ins for the Quartus PLL and `build_id.v` |
| `cmake/` | `synthframe_files.cmake`: a core's `sim/files.f` for CMake Verilator benches; `synthframe_lint_vlt` gives the waiver files |
| `tools/mister/` | Scripts the smoke tests copy to the MiSTer: `tty_midi_xfer.py` (MIDI out check), `play_tty.py` (timed MIDI into the core), `mister_keys.py` (virtual keyboard) |
| `tools/midi/` | `mkmidi.py` (event script to MIDI file), `smf2events.py` (MIDI file to timed bytes) |
| `tools/` | Panel art and resampler table generators |
| `tests/`, `ver/` | ROM-free tests; `tests/ci.sh` runs them all (CI runs it on every pull request) |

## Lint waivers

`bin/hdl-lint` lints each core's `mister/<Name>.sv` with the core's `sim/files.f`, `sys/hps_io.sv`
and the stand-ins in `lint/` (P2K adds `cfg/lint_stubs.sv`, the TG68K kernel's ports). A waiver
is a `lint_off` line in the `lint.vlt` of the folder that owns the file. It names the rule and the
file, and also the message, except for the style rules (`DECLFILENAME`, `BLKSEQ`,
`PINCONNECTEMPTY`, `GENUNNAMED`, `TIMESCALEMOD`, `PROCASSINIT`, `UNUSEDPARAM`). No waiver names a
line. The Verilator benches pass the same files (`hdl-lint --vlt`) instead of global `-Wno-*`
options, so a new warning in a bench build fails that build too.

    modules/synthframe/bin/hdl-lint                 # every core; exit 1 on an unwaived warning
    modules/synthframe/bin/hdl-lint --update mt32   # append waivers for the current warnings
    modules/synthframe/bin/hdl-lint --waive-log LOG --log-dir DIR   # waivers for a bench's log

Review what `--update` appends: a width or multi-driver warning can be a real bug.

## ccache

Set `OBJCACHE` to `bin/objcache` (an absolute path), not to plain `ccache`, for Verilator
`--build` benches and the `sim/build.sh` scripts. The `ci.sh` scripts that turn ccache on
already do. A Verilator build compiles a precompiled header (`V<top>__pch.h`), and the `.gch`
holds the header's absolute path. Without `-g`, ccache leaves the working directory out of the
hash. With plain `ccache`, a second worktree, checkout or build folder gets the first one's
`.gch`. Once the first build folder is gone (cleaned, a temporary folder, a removed worktree),
clang stops with "malformed or corrupted precompiled file: could not find file ...". `bin/objcache`
caches each precompiled header under its build directory (`CCACHE_NAMESPACE`) and every other
compile as usual. A build folder that already has a foreign `.gch` needs that file deleted once.

## MiSTer backups

`synth-mister install` backs up first: every core's installed RBFs and root MGLs go to
`/media/fat/_synth_backup/<YYYYMMDD-HHMMSS>/` with a `MANIFEST`, unless the newest backup already
holds the same files. Test builds that `install` put there are recorded in
`_synth_backup/INSTALLED` and never backed up. `synth-mister restore [--from NAME] [CORE...]` puts
back each named core (default: all) from the newest backup; a core the backup has no RBF of is
left alone, and an installed RBF that is neither a test build nor in a backup is never removed.
`synth-mister backups` lists them. `tests/synth-mister/run.sh` checks this on a fake SD card.
