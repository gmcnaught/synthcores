# synthframe MiSTer target

`sys/` is the MiSTer framework, [Template_MiSTer](https://github.com/MiSTer-devel/Template_MiSTer)
at d6324f2 (2026-11-20, "Fix screen_rotate's triple buffering"), unchanged, under the GNU
General Public License version 2 (`sys/LICENSE-GPL2`). Each core's `mister/sys` is a symlink
to it, so the Quartus projects keep their `source sys/sys.tcl` lines.

To update: copy upstream `sys/` over this one in a single commit naming the upstream commit, and
run every core's Build RBF (CI does: this folder is in the shared path filter).

## Quartus project and Verilator file list

`bin/synthcore` writes each core's `mister/<core>.qpf`, `mister/<core>.qsf`, `mister/files.qip`
and `sim/files.f` from the core's `cfg/` files. `project.qsf.in` is the qsf template.

| File | Contents |
|---|---|
| `cfg/core.yaml` | `name` (the Quartus project), `mister.seed` |
| `cfg/files.yaml` | `search:` (search and include directories), `files:` (the core's HDL, for Quartus and Verilator), `mister:` (MiSTer-only: top level, `.sdc`). In Quartus order. Paths starting with `modules/` are from the repo root, the rest from the core's directory. The file type comes from the extension. |
| `cfg/macros.def` | Verilog macros, JTFRAME syntax: `[all]`, `[mister]`, `[xcau15p]` sections, `NAME` or `NAME=VALUE`. `[all]` goes to the qsf and to `sim/files.f`; `[xcau15p]` is checked but unused until that target has a generator. `NAME=0` is refused (`` `ifdef NAME`` would still be true). |

A macro that the template lists as a commented-out line is switched on in place; any other
macro goes in a `# Core macros` block before `source sys/sys.tcl`.

`sim/files.f` is for `verilator -F` (paths from the `.f` file's directory): `+define+` per
`[all]` macro, `+incdir+` per search directory, then `files:`. A bench adds its own top level and
options. CMake benches read it with `modules/synthframe/cmake/synthframe_files.cmake`:

    include(../../../modules/synthframe/cmake/synthframe_files.cmake)
    synthframe_files("${CMAKE_CURRENT_SOURCE_DIR}/.." CORE_SV CORE_INC CORE_DEFS)
    verilate(bench SOURCES ${CORE_SV} INCLUDE_DIRS ${CORE_INC} TOP_MODULE ...
             VERILATOR_ARGS ${CORE_DEFS} ...)

Without the fourth variable, a `.f` with `+define+` lines stops CMake with an error. (Scripts that
read `files.f` themselves, such as the Synclavier's `sim/lockstep.py`, must pass `+define+` on too.)

The SC-55, JV-880 and CZ-101 board benches and the Synclavier `board` lockstep top use it.

    modules/synthframe/bin/synthcore            # write the files for every core
    modules/synthframe/bin/synthcore sc55       # one core
    modules/synthframe/bin/synthcore check      # exit 1 if a committed file differs (CI)

Don't edit the generated files by hand: CI's `synthcore` job fails when they differ from `cfg/`.
`synthcore` needs PyYAML (`pip install pyyaml`).
