#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Quartus 17.0 refuses a generate construct (if / for / case at module level) outside
generate ... endgenerate ("Error (10170) ... near text "if"; expecting "endmodule""), although
IEEE 1364-2005 made the keywords optional and Verilator and Icarus accept it. This finds such
constructs in the repo's HDL so they fail in CI instead of on the build host.

    python3 modules/synthframe/tests/hdl_style/test_generate_regions.py [FILE...]

Default: every tracked .sv / .v (not .svh: those are statement fragments included in
always blocks) under modules/ and cores/ except MiSTer's sys/ and the
reference-model submodules. Token-level: comments, strings and `define bodies stripped; always / initial /
final bodies and function / task bodies are skipped.
"""
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
TOKEN = re.compile(r"[A-Za-z_][A-Za-z0-9_$]*|`?[A-Za-z_]\w*|\d+'[sS]?[bBoOdDhH][0-9a-fA-FxXzZ_?]+|\S")
PROC = {"always", "always_ff", "always_comb", "always_latch", "initial", "final"}
SKIP_TO = {"function": "endfunction", "task": "endtask", "class": "endclass", "covergroup": "endgroup",
           "property": "endproperty", "sequence": "endsequence"}


def strip(text):
    text = re.sub(r"/\*.*?\*/", lambda m: "\n" * m.group().count("\n"), text, flags=re.S)
    text = re.sub(r"//[^\n]*", "", text)
    text = re.sub(r'"(\\.|[^"\\])*"', '""', text)
    # `define bodies (with \ continuation lines) are not module items
    return re.sub(r"^[ \t]*`define(?:[^\n]*\\\n)*[^\n]*", lambda m: "\n" * m.group().count("\n"), text, flags=re.M)


def tokens(text):
    out = []
    for n, line in enumerate(strip(text).split("\n"), 1):
        out += [(t, n) for t in TOKEN.findall(line)]
    return out


def skip_statement(ts, i):
    """i at the first token of a procedural statement; returns the index after it."""
    t = ts[i][0]
    if t in ("begin", "fork"):
        depth = 0
        while i < len(ts):
            w = ts[i][0]
            if w in ("begin", "fork"):
                depth += 1
            elif w in ("end", "join", "join_any", "join_none"):
                depth -= 1
                if depth == 0:
                    return i + 1
            i += 1
        return i
    if t in ("case", "casez", "casex", "unique", "priority") and t != "case" or t in ("case", "casez", "casex"):
        depth = 0
        while i < len(ts):
            w = ts[i][0]
            if w in ("case", "casez", "casex"):
                depth += 1
            elif w == "endcase":
                depth -= 1
                if depth == 0:
                    return i + 1
            i += 1
        return i
    if t in ("if", "for", "while", "repeat", "forever", "foreach"):
        i += 1
        if t != "forever" and i < len(ts) and ts[i][0] == "(":      # the condition
            p = 0
            while i < len(ts):
                p += {"(": 1, ")": -1}.get(ts[i][0], 0)
                i += 1
                if p == 0:
                    break
        i = skip_statement(ts, i)
        if t == "if" and i < len(ts) and ts[i][0] == "else":
            i = skip_statement(ts, i + 1)
        return i
    if t == "@" or t == "#":                                        # timing control, then a statement
        i += 1
        if i < len(ts) and ts[i][0] == "(":
            p = 0
            while i < len(ts):
                p += {"(": 1, ")": -1}.get(ts[i][0], 0)
                i += 1
                if p == 0:
                    break
        else:
            i += 1
        return skip_statement(ts, i)
    p = 0                                                           # a simple statement: to ';'
    while i < len(ts):
        w = ts[i][0]
        p += {"(": 1, ")": -1, "{": 1, "}": -1, "[": 1, "]": -1}.get(w, 0)
        i += 1
        if w == ";" and p <= 0:
            return i
    return i


def check(path):
    ts = tokens(Path(path).read_text(errors="replace"))
    bad, gen, i = [], 0, 0
    while i < len(ts):
        t, n = ts[i]
        if t == "generate":
            gen += 1
        elif t == "endgenerate":
            gen -= 1
        elif t in PROC:
            i = skip_statement(ts, i + 1)
            continue
        elif t in SKIP_TO:
            end = SKIP_TO[t]
            while i < len(ts) and ts[i][0] != end:
                i += 1
        elif t in ("if", "for", "case") and gen == 0:
            bad.append((n, t))
            i = skip_statement(ts, i)
            continue
        i += 1
    return bad


def files():
    out = subprocess.run(["git", "ls-files", "-z", "--", "modules", "cores"], cwd=ROOT, capture_output=True,
                         check=True).stdout.decode().split("\0")
    return [ROOT / p for p in out if p.endswith((".sv", ".v")) and "/sys/" not in p
            and not p.startswith(("modules/nuked-", "cores/sc55/third_party/")) and (ROOT / p).is_file()]


def main(argv):
    paths = [Path(a) for a in argv] or files()
    found = 0
    for p in paths:
        for n, t in check(p):
            found += 1
            rel = p.relative_to(ROOT) if p.is_absolute() and ROOT in p.parents else p
            print(f"FAIL: {rel}:{n}: '{t}' generate construct outside generate ... endgenerate (Quartus 17.0)")
    print(f"generate regions: {len(paths)} files, {found} bare generate constructs: {'FAIL' if found else 'ok'}")
    return 1 if found else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
