# Notices

This repository is licensed under the GNU General Public License v3.0 or later (`LICENSE`),
except where a file states otherwise.

| Component | Path | Author / origin | License |
|---|---|---|---|
| fx68k 68000 core (unmodified) | `mister/rtl/fx68k/` | Jorge Cwik | GPL-3.0 |
| MiSTer framework (`sys/`) | `mister/sys/` | MiSTer-devel (Template_MiSTer) | GPL-2.0-or-later / GPL-3.0-or-later, per file |
| MAME changes (driver, sound device, MC68901) | `mame/patches/` | this project, against MAME | BSD-3-Clause (MAME per-file license) |
| Proteus core HDL, tools, docs | everything else | this project | GPL-3.0-or-later |

The HDL models of the MC68901 and HD44780 follow the behaviour of MAME's `mc68901` and `hd44780`
devices (BSD-3-Clause); no MAME source code is included in the HDL.

No ROM, firmware, sample or preset data is distributed. The core loads a system image that users
build from their own files (`tools/build_image.py`). E-mu, Proteus and ProtoLogic are trademarks of
their respective owners; this project is not affiliated with them.
