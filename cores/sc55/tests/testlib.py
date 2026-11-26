"""Shared helpers for the stdlib-only test scripts (run each as `python3 tests/test_x.py`)."""
import os
import pathlib
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
REF_BUILD = pathlib.Path(os.environ.get("SC55_REF_BUILD", ROOT / "out" / "ref"))
ROM_DIR = pathlib.Path(os.environ.get("SC55_ROM_DIR", ROOT / "firmware"))
MIDI_DIR = ROOT / "third_party" / "nuked-sc55" / "test" / "integration"


class Skip(Exception):
    pass


def need_roms(romset: str = "mk1-v1.21") -> dict:
    """Return {location: path} for `romset`, or skip (CI has no ROMs)."""
    sys.path.insert(0, str(ROOT / "tools"))
    import romset as rs
    if not ROM_DIR.is_dir():
        raise Skip(f"no ROM directory {ROM_DIR}")
    try:
        return rs.find_romset(ROM_DIR, romset)
    except LookupError as e:
        raise Skip(str(e))


def need_file(path: pathlib.Path, what: str) -> pathlib.Path:
    if not path.exists():
        raise Skip(f"{what} not built: {path}")
    return path


def run(namespace: dict) -> None:
    """Run every test_* in `namespace`. Prints a summary; exits 1 if any test failed, 77 if every
    test skipped (missing ROMs or benches: not a pass), else 0."""
    import traceback
    tests = [(k, v) for k, v in sorted(namespace.items()) if k.startswith("test_") and callable(v)]
    passed = skipped = failed = 0
    for name, fn in tests:
        try:
            fn()
            passed += 1
        except Skip as e:
            skipped += 1
            print(f"  SKIP: {name}: {e}")
        except Exception:
            failed += 1
            print(f"  FAIL: {name}")
            traceback.print_exc()
    print(f"{passed} passed, {skipped} skipped, {failed} failed")
    if failed:
        sys.exit(1)
    if skipped and not passed:
        sys.exit(77)
