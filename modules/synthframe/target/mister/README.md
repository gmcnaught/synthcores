# synthframe MiSTer target

`sys/` is the MiSTer framework, [Template_MiSTer](https://github.com/MiSTer-devel/Template_MiSTer)
at d6324f2 (2026-11-20, "Fix screen_rotate's triple buffering"), unchanged, under the GNU
General Public License version 2 (`sys/LICENSE-GPL2`). Each core's `mister/sys` is a symlink
to it, so the Quartus projects keep their `source sys/sys.tcl` lines.

To update: copy upstream `sys/` over this one in a single commit naming the upstream commit, and
run every core's Build RBF (CI does: this folder is in the shared path filter).
