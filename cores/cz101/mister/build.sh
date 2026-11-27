#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Build the CZ-101 MiSTer core (CZ101.qpf) into an RBF with Quartus Lite 17.0: runs the shared
# modules/synthframe/target/mister/build.sh, which documents the reports and the exit status.
#
# Usage: mister/build.sh [output_dir]       (default: mister/output_files)
# SEED=N overrides the fitter seed for this build.
here=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
exec bash "$here/../../../modules/synthframe/target/mister/build.sh" "$here" CZ101 "$@"
