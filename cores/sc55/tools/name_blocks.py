#!/usr/bin/env python3
"""Give a label to every `begin` whose block starts with declarations
(Quartus 17 follows SystemVerilog-2005: declarations only in named blocks).
Usage: name_blocks.py FILE..."""
import re
import sys

DECL = re.compile(r"^\s*(logic|int|bit|reg|integer)\b")
BEGIN = re.compile(r"\bbegin(\s*)$")


def fix(path):
    lines = open(path).read().split("\n")
    n = 0
    stem = re.sub(r"\W", "_", path.split("/")[-1].split(".")[0])
    for i, line in enumerate(lines):
        m = BEGIN.search(line)
        if not m or re.search(r"\bbegin\s*:", line):
            continue
        j = i + 1
        while j < len(lines) and (lines[j].strip() == "" or lines[j].strip().startswith("//")):
            j += 1
        if j < len(lines) and DECL.match(lines[j]):
            n += 1
            lines[i] = line[:m.start()] + f"begin : b_{stem}_{n}" + m.group(1)
    open(path, "w").write("\n".join(lines))
    return n


if __name__ == "__main__":
    for p in sys.argv[1:]:
        print(p, fix(p))
