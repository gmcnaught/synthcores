# synthcores

FPGA recreations of vintage digital synthesizers. Each one runs the original firmware on the
original CPU, with the custom sound chips reproduced exactly and checked against a reference
model. Today they run on the MiSTer (DE10-Nano). An AMD Artix UltraScale+ target (`xcau15p`) is
planned. The layout follows jotego's jtcores and JTFRAME.

| Core | Folder | Status | Licence |
|---|---|---|---|
| Roland SC-55 / SC-55mk2 / SC-155 | [`cores/sc55`](cores/sc55) | released; sample-exact against Nuked-SC55 | GPL-2.0-or-later |
| E-mu Proteus/1 XR | [`cores/proteus`](cores/proteus) | runs on MiSTer; Proteus 2000 CPU in progress under `p2k/` | GPL-3.0-or-later |
| New England Digital Synclavier | [`cores/synclavier`](cores/synclavier) | Release L and 5.3 on MiSTer | MIT |
| Casio CZ-101 | [`cores/cz101`](cores/cz101) | runs on MiSTer; instruction- and sample-exact against MAME | BSD-3-Clause (default, not yet confirmed) |

- **`modules/`:** shared code. `synthframe`, the shared MIT framework, is to come. The
  reference models are submodules (`modules/nuked-sc55`).
- **History:** each core's history was imported from its original repository by
  [`scripts/import-cores.sh`](scripts/import-cores.sh):
  [sc55-MiSTer](https://github.com/gmcnaught/sc55-MiSTer),
  [emu-proteus-xr-MiSTer](https://github.com/gmcnaught/emu-proteus-xr-MiSTer) and
  [synclavierii](https://github.com/gmcnaught/synclavierii).
- **Plans:** [`LAYOUT-PLAN.md`](LAYOUT-PLAN.md) and [`HARDWARE-PLAN.md`](HARDWARE-PLAN.md).
- **ROMs:** no ROMs, firmware or disk images are included. Each core's README says which dumps
  it needs.

Clone with `git clone --recursive`. CI (`.github/workflows/`) runs the tests and the Quartus
build only for the cores a change touches.

Each folder carries its own licence. A file's SPDX header, where it has one, takes precedence.
