# synthframe MiSTer target

`sys/` is the MiSTer framework, [Template_MiSTer](https://github.com/MiSTer-devel/Template_MiSTer)
at d6324f2 (2026-11-20, "Fix screen_rotate's triple buffering"), unchanged, under the GNU
General Public License version 2 (`sys/LICENSE-GPL2`). Each core's `mister/sys` is a symlink
to it, so the Quartus projects keep their `source sys/sys.tcl` lines.

To update: copy upstream `sys/` over this one in a single commit naming the upstream commit, and
run every core's Build RBF (CI does: this folder is in the shared path filter).

## Quartus project files

`project.qsf.in` is the template for every core's `mister/<core>.qsf`. `bin/synthcore` writes
the qsf and `mister/files.qip` from the core's `cfg/` files:

| File | Contents |
|---|---|
| `cfg/core.yaml` | `name` (the Quartus project), `mister.seed`, `mister.qsf_eol` (`crlf` for the Proteus/1, whose qsf has always had CRLF) |
| `cfg/files.yaml` | `search:` and `files:` in Quartus order. Paths starting with `modules/` are from the repo root, the rest from the core's directory. The file type comes from the extension. |
| `cfg/macros.def` | Verilog macros, JTFRAME syntax: `[all]`, `[mister]`, `[xcau15p]` sections, `NAME` or `NAME=VALUE` |

A macro that the template lists as a commented-out line is switched on in place; any other
macro goes in a `# Core macros` block before `source sys/sys.tcl`.

    modules/synthframe/bin/synthcore            # write the files for every core
    modules/synthframe/bin/synthcore sc55       # one core
    modules/synthframe/bin/synthcore check      # exit 1 if a committed file differs (CI)

Don't edit `files.qip` or the qsf by hand: CI's `synthcore` job fails when they differ from
`cfg/`. The `.qpf` files stay hand-written. `synthcore` needs PyYAML (`pip install pyyaml`).
