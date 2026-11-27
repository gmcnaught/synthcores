# SPDX-License-Identifier: GPL-3.0-or-later
"""hdl-lint's waiver files (modules/synthframe/bin/hdl-lint): every core's and module's
cfg/lint.vlt, for the Verilator unit benches. A warning no waiver covers fails the build."""
import glob
import os

_REPO = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "..")
LINT_VLT = [os.path.join(_REPO, "modules", "synthframe", "lint", "common.vlt")] + sorted(
    glob.glob(os.path.join(_REPO, "cores", "*", "cfg", "lint.vlt"))
    + glob.glob(os.path.join(_REPO, "modules", "*", "cfg", "lint.vlt")))
