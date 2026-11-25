# synthcores

FPGA recreations of vintage digital synthesizers. Each one runs the original firmware on the
original CPU, with the custom sound chips reproduced exactly and checked against a reference
model. Today they run on the MiSTer (DE10-Nano). An AMD Artix UltraScale+ target (`xcau15p`) is
planned. The layout follows jotego's jtcores and JTFRAME.

| Core | Folder | Status | Release | Licence |
|---|---|---|---|---|
| Roland SC-55 / SC-55mk2 / SC-155 | [`cores/sc55`](cores/sc55) | plays MIDI on the MiSTer; sample-exact against Nuked-SC55 | [sc55-MiSTer](https://github.com/gmcnaught/sc55-MiSTer/releases) v20261120 | GPL-2.0-or-later |
| Roland JV-880 | [`cores/jv880`](cores/jv880) | plays MIDI on the MiSTer; sample-exact against Nuked-SC55's `jv880` model | [JV-880-MiSTer](https://github.com/gmcnaught/JV-880-MiSTer/releases) v20261124 | GPL-2.0-or-later |
| Roland MT-32 (second generation) | [`cores/mt32`](cores/mt32) | plays MIDI on the MiSTer; instruction- and sample-exact against Nuked-MT32 | [MT-32-MiSTer](https://github.com/gmcnaught/MT-32-MiSTer/releases) v20261124 | GPL-2.0-or-later (8098 CPU MIT) |
| Casio CZ-101 | [`cores/cz101`](cores/cz101) | plays MIDI on the MiSTer; instruction- and sample-exact against MAME | [CZ-101-MiSTer](https://github.com/gmcnaught/CZ-101-MiSTer/releases) v20261124 | MIT (MAME's BSD-3-Clause notice in its `NOTICE`) |
| E-mu Proteus/1 XR | [`cores/proteus`](cores/proteus) | plays MIDI on the MiSTer | [emu-proteus-xr-MiSTer](https://github.com/gmcnaught/emu-proteus-xr-MiSTer/releases) v0.2.0 | GPL-3.0-or-later |
| E-mu Proteus 2000 | [`cores/p2k`](cores/p2k) (CPU and sound RTL in [`cores/proteus/p2k`](cores/proteus/p2k)) | boots OS 2.26 and plays MIDI on the MiSTer with a stand-in SIMM; sound bit-exact against the integer MAME model; user flash SIMMs written as MAME writes them | — (not released) | GPL-3.0-or-later |
| New England Digital Synclavier | [`cores/synclavier`](cores/synclavier) | Release L and 5.3 on the MiSTer | [synclavierii](https://github.com/gmcnaught/synclavierii/releases) Synclavier_20261118 | MIT |

Each core's README gives its checks, the ROM dumps it needs and how to install it.

- **`modules/`:** shared code.
    - `synthframe`: the shared framework (MIT): MIDI, audio, memory ports, status strip and save
      HDL, the MiSTer target (`target/mister`) and `bin/synthcore`, which writes each core's
      Quartus and Verilator file lists from `cores/<core>/cfg/`.
    - `roland`: the H8/532 and Roland PCM chip HDL that the SC-55 and JV-880 share
      (GPL-2.0-or-later).
    - Reference models, as submodules: `nuked-sc55` (jcmoyer's Nuked-SC55) and `nuked-mt32`
      (Nuked-MT32).
- **Releases:** each released core has its own repository, which holds the RBFs, the
  instructions and any ROM-pack tool. The SC-55, Proteus/1 XR and Synclavier repositories are
  also the cores' original source repositories; their history was imported here by
  [`scripts/import-cores.sh`](scripts/import-cores.sh), and new work happens in this one.
- **Plans:** [`SESSION-PLAN.md`](SESSION-PLAN.md) and [`SESSION-LOG.md`](SESSION-LOG.md) (work by
  iteration), [`LAYOUT-PLAN.md`](LAYOUT-PLAN.md) and [`HARDWARE-PLAN.md`](HARDWARE-PLAN.md).
- **ROMs:** no ROMs, firmware or disk images are included. Each core's README says which dumps
  it needs.

Clone with `git clone --recursive`. CI (`.github/workflows/`): `tests.yml` runs on every pull
request, for the cores a change touches. `build-rbf.yml` builds RBFs with Quartus Lite 17.0: for
one core on demand (the "Build RBF" workflow with `cores` set, which is how releases are built), for
every core on a `v*` tag.

Each folder carries its own licence. A file's SPDX header, where it has one, takes precedence.
