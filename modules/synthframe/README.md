# synthframe

The framework the cores share (MIT).

| Folder | Contents |
|---|---|
| `hdl/` | MIDI in/out, audio FIFO and resampler, memory ports (`mem/`: arbiter, DDR3), NVRAM save (`save/`), panel modules (`panel/README.md`) |
| `target/mister/` | The MiSTer framework (`sys/`), the qsf template, the shared `build.sh` each core's `mister/build.sh` calls (`SEED=N` sets the fitter seed) and the `.srf` the cores link to (`README.md`) |
| `bin/synthcore` | Writes each core's Quartus project and `sim/files.f` from `cores/<core>/cfg/`; `synthcore deps CORE` lists the directories a core takes from other cores |
| `bin/synth-mister` | Drives the MiSTer for the smoke tests (lock, backup, install, load, screenshot, restore); the SD-card side is `bin/synth-mister-fat.sh` |
| `cmake/` | `synthframe_files.cmake`: a core's `sim/files.f` for CMake Verilator benches |
| `tools/mister/` | Scripts the smoke tests copy to the MiSTer: `tty_midi_xfer.py` (MIDI out check), `play_tty.py` (timed MIDI into the core), `mister_keys.py` (virtual keyboard) |
| `tools/midi/` | `mkmidi.py` (event script to MIDI file), `smf2events.py` (MIDI file to timed bytes) |
| `tools/` | Panel art and resampler table generators |
| `tests/`, `ver/` | ROM-free tests; `tests/ci.sh` runs them all (CI runs it on every pull request) |

## MiSTer backups

`synth-mister install` backs up first: every core's installed RBFs and root MGLs go to
`/media/fat/_synth_backup/<YYYYMMDD-HHMMSS>/` with a `MANIFEST`, unless the newest backup already
holds the same files. Test builds that `install` put there are recorded in
`_synth_backup/INSTALLED` and never backed up. `synth-mister restore [--from NAME] [CORE...]` puts
back each named core (default: all) from the newest backup; a core the backup has no RBF of is
left alone, and an installed RBF that is neither a test build nor in a backup is never removed.
`synth-mister backups` lists them. `tests/synth-mister/run.sh` checks this on a fake SD card.
