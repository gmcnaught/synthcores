"""Shared runner for the Synclavier test files (each file's __main__).

A test that cannot run (no Verilator, no Release L or W0 image, no timbres/)
raises unittest.SkipTest("reason"). run_tests() runs every test_* function
in sorted order, keeps going after a failure, prints

    SKIP: test_name: reason            (one line per skipped test)
    FAIL: test_name                    (with the traceback)
    <file>: N passed, M skipped, K failed

and exits 1 if any test failed, 77 if every test was skipped (nothing ran),
else 0. A skip is never counted as a pass (tests/ci.sh counts files that
exit 77 separately).
"""
import os
import sys
import traceback
import unittest

SKIP_EXIT = 77


def run_tests(namespace, argv=None):
    """Run the test_* callables in `namespace` (a module's globals()); with
    names in argv (sys.argv[1:] by default) only those. Exits the process."""
    argv = sys.argv[1:] if argv is None else argv
    tests = sorted((k, v) for k, v in namespace.items() if k.startswith("test_") and callable(v))
    if argv:
        unknown = set(argv) - {k for k, _ in tests}
        if unknown:
            print("unknown tests: %s" % ", ".join(sorted(unknown)))
            sys.exit(2)
        tests = [(k, v) for k, v in tests if k in argv]
    passed, skipped, failed = [], [], []
    for name, fn in tests:
        try:
            fn()
        except unittest.SkipTest as e:
            print("SKIP: %s: %s" % (name, e))
            skipped.append(name)
        except Exception:           # noqa: BLE001 - report every failing test, then exit 1
            print("FAIL: %s" % name)
            traceback.print_exc(file=sys.stdout)
            failed.append(name)
        else:
            passed.append(name)
        sys.stdout.flush()
    path = namespace.get("__file__", "tests")
    print("%s: %d passed, %d skipped, %d failed" % (
        os.path.basename(path), len(passed), len(skipped), len(failed)))
    if failed:
        sys.exit(1)
    if skipped and not passed:
        sys.exit(SKIP_EXIT)
    sys.exit(0)
