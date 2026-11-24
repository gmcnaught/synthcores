# synth-projects workspace

This directory is a monorepo laid out like jotego's jtcores. Each core's full history was imported
under `cores/<core>/` by `scripts/import-cores.sh`.

| Dir | Core | Imported from |
|---|---|---|
| `cores/sc55/` | Roland SC-55 / mk2 / SC-155 (H8/532, PCM chip), lockstep against Nuked-SC55 | gmcnaught/sc55-MiSTer `main` |
| `cores/proteus/` | E-mu Proteus/1 XR (fx68k); Proteus 2000 core in progress under `p2k/` (TG68K + ColdFire) | gmcnaught/emu-proteus-xr-MiSTer `p2k-research` |
| `cores/synclavier/` | NED Synclavier (ABLE CPU, FM, poly sampling) | gmcnaught/synclavierii `main` |
| `modules/nuked-sc55/` | SC-55 reference model (submodule) | jcmoyer/Nuked-SC55 |

- **SC-55 reference path:** `cores/sc55/third_party/nuked-sc55` is a symlink to
  `modules/nuked-sc55`, so the SC-55's existing paths still work.
- **Old repo folders:** the original repos (`nuked-sc55/`, `emu-proteus-2k-MiSTer/`,
  `synclavier-ii-MiSTer/`) are gitignored here. They stay as they were until the monorepo passes
  its tests (`LAYOUT-PLAN.md`, step 2). They still hold the untracked ROMs, `out/`, `build/` and
  the MAME checkout. Don't commit new work there.
- **Design brief (Vintage Synth FPGA Module):**
  https://claude.ai/artifact/C3PuPP2mcDQTQASFHiE1Um
- **Plans:** `HARDWARE-PLAN.md` (MiSTer and M64) and `LAYOUT-PLAN.md` (monorepo layout,
  licences, migration order).

## Platforms and build hosts

- **MiSTer (DE10-Nano, Cyclone V):** `root@192.168.20.62`, SSH key login. Hardware smoke tests:
  `modules/synthframe/bin/synth-mister` and each core's smoke script
  (`cores/sc55/scripts/mister/smoke.sh`, `cores/proteus/tools/mister_smoke.sh`,
  `cores/synclavier/scripts/mister/smoke.sh`). The installed releases are backed up in
  `/media/fat/_synth_backup/<date>/`. MIDI comes in through MidiLink.
- **Windows build host (`winbuild` in `~/.ssh/config`):** Quartus Lite 17.0 and Vivado 2026.1,
  PowerShell over SSH.
    - Build a core's RBF from the working tree with `scripts/build-hosts/winbuild.sh CORE`
      (`SYNTHROOT=` another checkout).
    - Pull requests are gated on these builds plus the MiSTer smoke tests.
    - `scripts/build-hosts/enable-windows-sshd.ps1` sets up the SSH side.
- **GitHub Actions:**
    - `tests.yml` runs on every pull request.
    - `build-rbf.yml` (Quartus in the raetro/quartus:17.0 container) runs only on `v*` tags and
      on demand.
    - Quartus on Windows does not place bit-identically to the Linux container: the SC-55 at
      main was 32,973 against 32,963 ALMs. Compare fit numbers only between builds from the
      same platform.
- **`xcau15p` target (AMD Artix UltraScale+), first board the ModRetro M64:** a test board loaded
  over JTAG.
    - No MIDI port.
    - Audio over HDMI only (`hw/m64/README.md`).
    - Vivado runs on the Windows build host. It has the xcau15p-ffvb676 parts.

## Conventions shared by the cores

- **Licences:** each core keeps its own (SC-55 GPL-2.0-or-later, Proteus GPL-3.0, Synclavier
  MIT). The shared framework `modules/synthframe` is MIT. GPL-3 code borrowed from JTFRAME stays
  outside it.
- **Tests:** every core is checked against a reference model, as bit-exact lockstep or output
  hashes. A refactor must keep those tests bit-identical.
- **Quartus file lists:** `mister/files.qip` and `mister/<core>.qsf` are generated. Edit
  `cores/<core>/cfg/` (`files.yaml`, `macros.def`, `core.yaml`) and run
  `modules/synthframe/bin/synthcore`; CI checks them (`modules/synthframe/target/mister/README.md`).
- **Vendor code:** keep vendor-specific code out of core HDL. Today it is limited to `pll.v` and
  `mister/sys`, plus one RAM hint in `cores/synclavier/hdl/dev/pcm_reverb.sv`.
- **ROMs:** no ROMs, firmware or disk images are committed. The tests find them through
  `SC55_ROM_DIR`, `SYNCLAVIER_RELEASE_L` and so on, in the old repo folders.
- **Python simulators:** run them and their tests with `pypy3`, not `python3`. That covers the
  Synclavier `sim/`, `tests/sim` and `tests/hdl`, and the Proteus `tools/proteus_model.py`.
  Scripts that only need Pillow can use `python3`.
- **Parallel sessions:** more than one session may work in this checkout. Do iteration work in
  its own `git worktree`, and `git add` only your own paths.
