#!/usr/bin/env python3
"""The MiSTer hardware scripts (scripts/mister/) without a MiSTer: the parts
that read results and start tools.

Run:  python3 tests/test_mister_scripts.py
"""
import pathlib
import os
import re
import subprocess
import tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def _pcount_def():
    src = pathlib.Path(os.path.join(ROOT, "scripts", "mister", "smoke.sh")).read_text()
    m = re.search(r"^pcount\(\) \{.*\}$", src, re.M)
    assert m, "no pcount() in smoke.sh"
    return m.group(0)


def test_smoke_pcount_without_counter():
    """smoke.sh's pcount() on a screen with no P counter gives an empty
    result under set -euo pipefail, so the script writes result.txt and
    fails on it (review 2026-11-26 sync-hdl #9: the script exited at once)."""
    with tempfile.TemporaryDirectory() as tmp:
        for name, text, want in (("none.txt", "MAIN MENU\nLag 0000\n", ""),
                                 ("p.txt", "Lag 0000 Underrun 0000 P01A\n", "01A")):
            path = os.path.join(tmp, name)
            with open(path, "w") as f:
                f.write(text)
            r = subprocess.run(["bash", "-c", "set -euo pipefail\n%s\np=$(pcount %s)\necho \"[$p]\""
                                % (_pcount_def(), path)], capture_output=True, text=True)
            assert r.returncode == 0 and r.stdout.strip() == "[%s]" % want, (name, r.returncode, r.stdout, r.stderr)


def test_smoke_fails_without_p_counter():
    """With no P counter read, result.txt says so and the p_late check fails
    (a missing counter is not 0 late reads)."""
    src = pathlib.Path(os.path.join(ROOT, "scripts", "mister", "smoke.sh")).read_text()
    assert 'p_late: unread' in src, "smoke.sh must report an unread P counter"


def test_mister_run_state_image_under_pypy():
    """mister_run.sh STATE=1 makes the state image (tools/state_image.py runs
    the simulator) with pypy3, as CLAUDE.md asks (review 2026-11-26 sync-hdl #10)."""
    src = pathlib.Path(os.path.join(ROOT, "scripts", "mister", "mister_run.sh")).read_text()
    lines = [ln for ln in src.splitlines() if "tools/state_image.py" in ln and not ln.lstrip().startswith("#")]
    assert lines and all("${PYTHON:-pypy3}" in ln for ln in lines), lines


if __name__ == "__main__":
    tests = [v for k, v in sorted(globals().items()) if k.startswith("test_")]
    for t in tests:
        t()
    print("%d tests OK" % len(tests))
