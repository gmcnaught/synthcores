#!/usr/bin/env python3
"""Seeded-fault check for the CPU lockstep bench.

Each mutation changes one detail of hdl/cpu/h8_cpu.sv; the bench (boot run +
random-code run) must report a mismatch for every one. Builds each mutant with
Verilator directly against the reference libraries from out/sim.

Usage: mutate.py ROM_DIR [OUT_DIR]
"""
import os
import pathlib
import shutil
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
SV = ROOT / "hdl" / "cpu" / "h8_cpu.sv"
BENCH = ROOT / "sim" / "cpu" / "cpu_lockstep.cpp"
SIM_BUILD = ROOT / "out" / "sim"

# (name, old, new): `old` must occur exactly once in h8_cpu.sv
MUTATIONS = [
    ("add carry-out from bit 15 lost",
     "c = s[16];", "c = s[15];"),
    ("sub overflow rule", "v = sub ? ((a7 & ~b7 & ~r7) | (~a7 & b7 & r7))", "v = sub ? ((a7 & b7 & ~r7) | (~a7 & b7 & r7))"),
    ("SetStatusCommon keeps V", "o[ST_V] = 1'b0;\n        return o;\n    endfunction\n\n    function automatic logic [15:0] st_nzvc",
     "return o;\n    endfunction\n\n    function automatic logic [15:0] st_nzvc"),
    ("predecrement byte by 2", "t16 = r[g_reg] - ((g_word || g_reg == 3'd7) ? 16'd2 : 16'd1);",
     "t16 = r[g_reg] - ((g_word || g_reg == 3'd6) ? 16'd2 : 16'd1);"),
    ("page for R4/R5 uses dp", "else if (rg >= 3'd4) return e;", "else if (rg >= 3'd4) return d;"),
    ("interrupt level compare >=", "if (int_pending[s] && ok && (l > sr[10:8])) begin",
     "if (int_pending[s] && ok && (l >= sr[10:8])) begin"),
    ("trace flag not cleared on entry", "srn[ST_T] = 1'b0;\n                if (int_has_mask)", "if (int_has_mask)"),
    ("BHI uses only C", "4'h2: cond = !(sr[ST_C] | sr[ST_Z]);", "4'h2: cond = !sr[ST_C];"),
    ("MOV:F word load without OR quirk", "r[op1[2:0]] <= (r[op1[2:0]] & 16'hff00) | m_data[15:0];",
     "r[op1[2:0]] <= m_data[15:0];"),
    ("EXTS flags from extended value", "srn = st_common(srn, {16'h0, r[g_reg]}, 1'b1);\n",
     "srn = st_common(srn, {{16{r[g_reg][7]}}, {8{r[g_reg][7]}}, r[g_reg][7:0]}, 1'b1);\n"),
    ("ADDX Z rule dropped", "if (opc == 5'h14 && !sr[ST_Z]) srn[ST_Z] = 1'b0;", ""),
    ("stack in page tp", "`WRITE({8'h00, (r[7] - 16'd2) & 16'hfffe}, 2, D, RET)", "`WRITE({tp, (r[7] - 16'd2) & 16'hfffe}, 2, D, RET)"),
    ("RTE masks SR", "S_RTE_SR: begin sr <= m_data[15:0];", "S_RTE_SR: begin sr <= m_data[15:0] & SR_MASK;"),
    ("STM pushes R7 unadjusted", "if (idx[2:0] == 3'd7) t16 = t16 - 16'd2;", ""),
    ("ADDQ invalid register traps without adding", "default: begin t16 = 16'd0; `TRAP; end", "default: begin t16 = 16'd1; `TRAP; end"),
    ("ORC/ANDC no ex_ignore", "if (oreg >= 3'd2) srn = st_common(srn, {16'h0, t16}, g_word);\n                            ex_ignore <= 1'b1;",
     "if (oreg >= 3'd2) srn = st_common(srn, {16'h0, t16}, g_word);"),
    ("divide overflow limit", "if (d_q > 32'h0000ffff) begin", "if (d_q > 32'h0001ffff) begin"),
    ("word operand odd address not flagged", "if (g_ea[0]) begin exc_v <= 1'b1; exc_t <= 2'd0; end\n                            `READ",
     "`READ"),
]


def build(sv: pathlib.Path, out: pathlib.Path) -> pathlib.Path:
    inc = [SIM_BUILD / "ref" / "traced" / "src", SIM_BUILD / "ref" / "traced" / "src" / "backend",
           SIM_BUILD / "ref" / "traced" / "src" / "common",
           SIM_BUILD / "ref" / "cfg_traced", SIM_BUILD / "ref" / "cfg_traced" / "backend"]
    libs = [SIM_BUILD / "ref" / "libsc55_common_traced.a", SIM_BUILD / "ref" / "libsc55_backend_traced.a"]
    cflags = "-std=c++20 -O2 " + " ".join(f"-I{p}" for p in inc)
    cmd = ["verilator", "--cc", "--exe", "--build", "-j", str(os.cpu_count() or 4), "-O3",
           "-Wno-fatal", "-Wno-BLKSEQ", "--top-module", "h8_cpu", "--Mdir", str(out),
           "-CFLAGS", cflags, "-LDFLAGS", " ".join(str(l) for l in libs),
           "-o", "bench", str(BENCH), str(sv)]
    subprocess.run(cmd, check=True, capture_output=True)
    return out / "bench"


def caught(bench: pathlib.Path, roms: str) -> bool:
    runs = [["--steps", "3000000"]] + [["--random", str(s), "--steps", "300000"] for s in (1, 2, 3)]
    for args in runs:
        r = subprocess.run([str(bench), "--roms", roms] + args, capture_output=True, text=True)
        if r.returncode != 0:
            return True
    return False


def main() -> int:
    roms = sys.argv[1]
    work = pathlib.Path(sys.argv[2]) if len(sys.argv) > 2 else ROOT / "out" / "mutants"
    work.mkdir(parents=True, exist_ok=True)
    src = SV.read_text()
    missed = []
    for i, (name, old, new) in enumerate(MUTATIONS):
        if src.count(old) != 1:
            print(f"[{i}] {name}: pattern found {src.count(old)} times", file=sys.stderr)
            return 2
        d = work / f"m{i:02d}"
        shutil.rmtree(d, ignore_errors=True)
        d.mkdir()
        (d / "h8_cpu.sv").write_text(src.replace(old, new))
        bench = build(d / "h8_cpu.sv", d / "obj")
        ok = caught(bench, roms)
        print(f"[{i:02d}] {'caught' if ok else 'MISSED'}: {name}", flush=True)
        if not ok:
            missed.append(name)
    print(f"{len(MUTATIONS) - len(missed)}/{len(MUTATIONS)} mutations caught")
    return 1 if missed else 0


if __name__ == "__main__":
    sys.exit(main())
