#!/usr/bin/env python3
"""tests/testlib.py: skipped tests are counted apart from passes, a file
where every test skipped exits 77, a failure exits 1 and the remaining tests
still run (review 2026-11-26 sync-py #2).

Run:  pypy3 tests/test_testlib.py
"""
import os
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))


def _run(body):
    src = ("import sys, unittest\nsys.path.insert(0, %r)\nfrom testlib import run_tests\n%s\n"
           "run_tests(globals())\n" % (HERE, body))
    with tempfile.NamedTemporaryFile("w", suffix=".py", delete=False) as f:
        f.write(src)
    try:
        p = subprocess.run([sys.executable, f.name], capture_output=True, text=True)
    finally:
        os.unlink(f.name)
    return p.returncode, p.stdout


def test_all_skipped_exits_77():
    rc, out = _run("def test_a():\n    raise unittest.SkipTest('no image')\n")
    assert rc == 77, (rc, out)
    assert "SKIP: test_a: no image" in out and "0 passed, 1 skipped, 0 failed" in out, out


def test_skip_is_not_a_pass():
    rc, out = _run("def test_a():\n    pass\n"
                   "def test_b():\n    raise unittest.SkipTest('needs verilator')\n")
    assert rc == 0, (rc, out)
    assert "1 passed, 1 skipped, 0 failed" in out, out


def test_failure_exits_1_and_runs_the_rest():
    rc, out = _run("def test_a():\n    assert 0, 'boom'\n"
                   "def test_b():\n    pass\n"
                   "def test_c():\n    raise unittest.SkipTest('x')\n")
    assert rc == 1, (rc, out)
    assert "FAIL: test_a" in out and "1 passed, 1 skipped, 1 failed" in out, out


if __name__ == "__main__":
    sys.path.insert(0, HERE)
    from testlib import run_tests
    run_tests(globals())
