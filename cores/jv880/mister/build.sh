#!/usr/bin/env bash
# Build the JV-880 MiSTer core (JV880.qpf) into an RBF with Quartus Lite 17.0: runs the shared
# modules/synthframe/target/mister/build.sh, which documents the reports and the exit status.
#
# Usage: mister/build.sh [output_dir]       (default: mister/output_files)
# SEED=N overrides the fitter seed for this build.
here=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
exec bash "$here/../../../modules/synthframe/target/mister/build.sh" "$here" JV880 "$@"
