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

## Platforms

- **MiSTer (DE10-Nano, Cyclone V):** Quartus 17.0 builds, through the per-core GitHub "Build
  RBF" workflows. These still sit under `cores/*/.github/` and don't run until they move to the
  root `.github/`. MIDI comes in through MidiLink.
- **`xcau15p` target (AMD Artix UltraScale+), first board the ModRetro M64:** a test board loaded
  over JTAG.
    - No MIDI port.
    - Audio over HDMI only.
    - Vivado needs an x86-64 Linux host. This Mac is arm64.

## Conventions shared by the cores

- **Licences:** each core keeps its own (SC-55 GPL-2.0-or-later, Proteus GPL-3.0, Synclavier
  MIT). The shared framework `vsframe` (to come) is MIT. GPL-3 code borrowed from JTFRAME stays
  outside it.
- **Tests:** every core is checked against a reference model, as bit-exact lockstep or output
  hashes. A refactor must keep those tests bit-identical.
- **Vendor code:** keep vendor-specific code out of core HDL. Today it is limited to `pll.v` and
  `mister/sys`, plus one RAM hint in `cores/synclavier/hdl/dev/pcm_reverb.sv`.
- **ROMs:** no ROMs, firmware or disk images are committed.
