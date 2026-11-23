#!/usr/bin/env python3
"""Seeded-fault check for the lockstep benches.

Each mutation changes one detail of the HDL; the bench must report a mismatch
for every one. Builds each mutant with Verilator directly against the
reference libraries from out/sim.

Usage: mutate.py ROM_DIR [cpu|mcu|pcm|submcu] [OUT_DIR] [--only=1,4]
"""
import os
import pathlib
import shutil
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
ROLAND = ROOT.parent.parent / "modules" / "roland" / "hdl"   # shared Roland HDL
CPU_SV = ROLAND / "cpu" / "h8_cpu.sv"
MCU_SV = ROLAND / "mcu" / "sc55_mcu.sv"
PCM_SV = ROLAND / "pcm" / "sc55_pcm.sv"
SUBMCU_SV = ROLAND / "submcu" / "sc55_submcu.sv"
SDP_SV = ROLAND / "common" / "sdp_ram.sv"
SIM_BUILD = ROOT / "out" / "sim"

# (name, old, new): `old` must occur exactly once in the target file
MCU_MUTATIONS = [
    ("FRT clear on match B", "if (ts_n[i][0] && ma) frc_n[i] = 16'h0;", "if (ts_n[i][0] && mb) frc_n[i] = 16'h0;"),
    ("FRT stride CKS=0", "case (cks) 2'd0: return 8'd4; 2'd1: return 8'd8;", "case (cks) 2'd0: return 8'd8; 2'd1: return 8'd8;"),
    ("FRT stride CKS=1 (the mk1 firmware does not use it; the mk2 run catches it)", "case (cks) 2'd0: return 8'd4; 2'd1: return 8'd8;", "case (cks) 2'd0: return 8'd4; 2'd1: return 8'd16;"),
    ("FRT TCSR flag clear ignores status read", "if (!d[5] && sr[5]) begin ts[5] = 1'b0; sr[5] = 1'b0; int_req[4 + 4 * t]",
     "if (!d[5]) begin ts[5] = 1'b0; sr[5] = 1'b0; int_req[4 + 4 * t]"),
    ("TMR compare A clear mode", "if (tmr_tcr[4:3] == 2'b01 && ma) cnt_n = 8'h0;", "if (tmr_tcr[4:3] == 2'b10 && ma) cnt_n = 8'h0;"),
    ("timer deadline align", "return (v + m) & ~m;", "return v & ~m;"),
    ("SSR not overwritten", "                                dev[7'h5c] <= d;\n", "                                dev[7'h5c] <= d | 8'h01;\n"),
    ("UART RX delay", "uart_rx_delay <= cycles + 48'd3000;", "uart_rx_delay <= cycles + 48'd2000;"),
    ("A/D period", "analog_end <= cycles + 48'd200;\n                                    dev[7'h68] <= d | 8'h80;",
     "analog_end <= cycles + 48'd201;\n                                    dev[7'h68] <= d | 8'h80;"),
    ("battery level", "return (s155 && p9[1]) ? 10'h000 : 10'h2a0;", "return (s155 && p9[1]) ? 10'h000 : 10'h2a4;"),
    ("GA counter length", "ga_lcd_counter <= 9'd500; end\n                        else if (a16 == 16'hf104)", "ga_lcd_counter <= 9'd499; end\n                        else if (a16 == 16'hf104)"),
    ("internal RAM ignores RAMCR (the firmware keeps RAMCR bit 7 set; caught where the speculation decode disagrees)", "else if (a16 >= 16'hfb80 && dev[7'h79][7]) rs = 3'd4;", "else if (a16 >= 16'hfb80) rs = 3'd4;"),
    ("button groups swapped", "if (!a16[1]) d = d & ~buttons[15:8];", "if (!a16[1]) d = d & ~buttons[23:16];"),
    ("ROM2 page decode", "wire  [18:0] bram_a_rom2 = {mk2 & bram_a[19], bram_a[17:0]};", "wire  [18:0] bram_a_rom2 = {mk2 & bram_a[19], 1'b0, bram_a[16:0]};"),
    ("speculative read kept across a write", "if (ld_we || q == Q_CLEAR || (accept && c_we)) spec_v <= 1'b0;", "if (ld_we || q == Q_CLEAR) spec_v <= 1'b0;"),
    ("speculation crosses the page", "wire  [23:0] c_next = {c_addr[23:16], c_addr[15:0] + 16'd1};", "wire  [23:0] c_next = c_addr + 24'd1;"),
    ("mk2: GA line 5 ignores its enable", "tg = (sm_ga5_val && !ga_int5 && ga_en[5]) ? 3'd5 : ga_trigger;", "tg = (sm_ga5_val && !ga_int5) ? 3'd5 : ga_trigger;"),
    ("mk2: SRAM in page 11", "c_addr[19:16] == (mk2 ? 4'd10 : 4'd5);", "c_addr[19:16] == (mk2 ? 4'd11 : 4'd5);"),
    ("mk2: switch level", "2'd2: return 10'h3ff;", "2'd2: return 10'h3fe;"),
    ("mk2: no IRQ1 hold while the sub-MCU runs",
     "(mk2 && !sm_caught_up && !int_req[2] && dev[7'h7c][6] && (dev[7'h70][2:0] > dbg_sr[10:8]))", "1'b0"),
    ("mk2: P9DR model bit", "((mk2 && !sc155) ? (8'h02 & ~dev[7'h7e]) : 8'h00)", "((mk2 && !sc155) ? (8'h00 & ~dev[7'h7e]) : 8'h00)"),
]

PCM_MUTATIONS = [
    ("interp table: the pre-2026-09 bit flip (3535 -> 3534)", "{2'd0, 7'd10}: return 12'd3535;", "{2'd0, 7'd10}: return 12'd3534;"),
    ("sign extension from bit 18", "return {{12{v[19]}}, v[19:0]};", "return {{13{v[18]}}, v[18:0]};"),
    ("noise LFSR tap", "xr = shv[0] ^ shv[1] ^ shv[7] ^ shv[12];\n                shv = {xr, shv[15:1]};", "xr = shv[0] ^ shv[1] ^ shv[7] ^ shv[11];\n                shv = {xr, shv[15:1]};"),
    ("envelope dither bit order", "if (tvc[3]) addlow |= 1;\n            if (tvc[2]) addlow |= 2;", "if (tvc[2]) addlow |= 1;\n            if (tvc[3]) addlow |= 2;"),
    ("eram block-float threshold", "if (top >= 16) sh = 3;", "if (top >= 15) sh = 3;"),
    ("filter fine multiply shift", "vv1 = fadd(vv2, pp[1] >>> 13, (pp[1] >>> 12) & 1, mk2);", "vv1 = fadd(vv2, pp[1] >>> 12, (pp[1] >>> 11) & 1, mk2);"),
    ("pan bytes swapped", "pp[0] <= multi(v_sample3, pan[15:8]);\n                pp[1] <= multi(v_sample3, pan[7:0]);", "pp[0] <= multi(v_sample3, pan[7:0]);\n                pp[1] <= multi(v_sample3, pan[15:8]);"),
    ("DPCM shift", "shift_ = (10 - int'(sel)) & 15;\n        shifted", "shift_ = (11 - int'(sel)) & 15;\n        shifted"),
    ("IRQ ignores irq_assert", "if (v_active && r2[6][0] && !r2[8][14] && !irq_assert && v_irq_flag) begin", "if (v_active && r2[6][0] && !r2[8][14] && v_irq_flag) begin"),
    ("frame length 24 cycles per slot", "pcm_cycles <= pcm_cycles + ({42'h0, reg_slots} + 48'd1) * 48'd25;", "pcm_cycles <= pcm_cycles + ({42'h0, reg_slots} + 48'd1) * 48'd24;"),
    ("reverb step 17 reads tv + 0", "            `ER(`EA1(29, 10))\n", "            `ER(`EA(29, 10))\n"),
    ("send slot 21 to the wrong sum (ram2[31][2] low byte is 00 for every reverb x chorus macro on mk1: expected MISSED)", "6'd21: rs0 = addclip20(rs0, rcadd2[2] >>> 1, rcadd2[2] & 1);", "6'd21: rs1 = addclip20(rs1, rcadd2[2] >>> 1, rcadd2[2] & 1);"),
    ("ROM bank select ignores 3d bit 5", "bank = r3d[5] ? address[23:21] : address[21:19];", "bank = address[21:19];"),
]

SUBMCU_MUTATIONS = [
    ("pointer high byte from the next address (SM_Read16 reads one address twice)",
     "ea <= {rdata, rdata} + ((md == M_INDY)", "ea <= {8'h00, rdata} + ((md == M_INDY)"),
    ("JSR pushes PC low first", "do_write({8'h00, s}, (pcnt == 2'd0) ? pc[15:8] : pc[7:0]);",
     "do_write({8'h00, s}, (pcnt == 2'd0) ? pc[7:0] : pc[15:8]);"),
    ("STA zp,X wraps at 0xff", "M_ZPXU: return {8'h00, lo} + {8'h00, xv};", "M_ZPXU: return {8'h00, lo + xv};"),
    ("STX 96 indexes with Y", "8'h96: begin m = M_ZPXU; k = K_STX; end", "8'h96: begin m = M_ZPY; k = K_STX; end"),
    ("LDA ignores T", "K_LDA: if (t_mode) do_write({8'h00, x}, d);", "K_LDA: if (1'b0) do_write({8'h00, x}, d);"),
    ("IPCM0 interrupt to the timer vector", "return {1'b1, 4'd5, 8'h10, 1'b0};", "return {1'b1, 4'd4, 8'h10, 1'b0};"),
    ("timer reload one less", "ct = dm[D_TIMER];", "ct = dm[D_TIMER] - 8'd1;"),
    ("MIDI byte delay 11000", "rx_delay <= cy + 48'd12000;", "rx_delay <= cy + 48'd11000;"),
    ("sys read keeps IPCE", "dm[D_IPCE0 + 5'(sys_addr[1:0])] <= 8'h00;", "dm[D_IPCE0 + 5'(sys_addr[1:0])] <= dm[D_IPCE0 + 5'(sys_addr[1:0])];"),
    ("SM read clears access when RAM_DIR clear", "if (dm[D_RAM_DIR][ad[7:5]]) access", "if (!dm[D_RAM_DIR][ad[7:5]]) access"),
    ("GA line 5 from UART3 control bit 4", "ga5_val <= m0d[7] && !m0e[5];", "ga5_val <= m0d[7] && !m0e[4];"),
    ("compare carry inverted", "r[F_C] = !df[8];", "r[F_C] = df[8];"),
    ("BBx A sense inverted", "if (a[opc[7:5]] != opc[4]) pc <= pc", "if (a[opc[7:5]] == opc[4]) pc <= pc"),
    ("collision write sets bit 7", "D_COLL:    dm[D_COLL] <= {d[7] & dm[D_COLL][7], d[6:0]};", "D_COLL:    dm[D_COLL] <= {d[7], d[6:0]};"),
    ("instruction 47 cycles", "cyc48 <= cy + 48'd48;", "cyc48 <= cy + 48'd47;"),
    ("P1 button group swapped", "if (!p0[1]) d &= ~b[15:8];", "if (!p0[1]) d &= ~b[23:16];"),
]

CPU_MUTATIONS = [
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


def build(target: str, srcs: list, out: pathlib.Path) -> pathlib.Path:
    inc = [SIM_BUILD / "ref" / "traced" / "src", SIM_BUILD / "ref" / "traced" / "src" / "backend",
           SIM_BUILD / "ref" / "traced" / "src" / "common",
           SIM_BUILD / "ref" / "cfg_traced", SIM_BUILD / "ref" / "cfg_traced" / "backend"]
    libs = [SIM_BUILD / "ref" / "libsc55_common_traced.a", SIM_BUILD / "ref" / "libsc55_backend_traced.a"]
    cflags = "-std=c++20 -O2 " + " ".join(f"-I{p}" for p in inc)
    bench = ROOT / "sim" / target / f"{target}_lockstep.cpp"
    top = {"cpu": "h8_cpu", "mcu": "sc55_mcu", "pcm": "sc55_pcm", "submcu": "sc55_submcu"}[target]
    extra = [] if target == "cpu" else ["--public-flat-rw"]
    if target == "pcm":
        extra += [f"-I{srcs[-1].parent}", f"-I{ROOT / 'hdl' / 'pcm'}"]
    cmd = ["verilator", "--cc", "--exe", "--build", "-j", str(os.cpu_count() or 4), "-O3",
           "-Wno-fatal", "-Wno-BLKSEQ", "--top-module", top, "--Mdir", str(out), *extra,
           "-CFLAGS", cflags, "-LDFLAGS", " ".join(str(l) for l in libs),
           "-o", "bench", str(bench), str(ROOT / "sim" / "common" / "midi_demo.cpp"), *map(str, srcs)]
    subprocess.run(cmd, check=True, capture_output=True)
    return out / "bench"


def caught(target: str, bench: pathlib.Path, roms: str) -> bool:
    if target == "cpu":
        runs = [["--steps", "3000000"]] + [["--random", str(s), "--steps", "300000"] for s in (1, 2, 3)]
    elif target == "submcu":
        runs = ([["--steps", "3000000"], ["--midi-demo", "5", "--buttons", "9", "--steps", "6000000"]]
                + [["--random", str(s), "--steps", "1000000"] for s in (1, 2, 3)])
    elif target == "mcu":
        runs = [["--steps", "3000000"], ["--midi-demo", "5", "--buttons", "9", "--steps", "6000000"],
                ["--romset", "mk2-v1.01", "--midi-demo", "5", "--buttons", "9", "--steps", "6000000"]]
    else:
        runs = [["--midi-demo", "1", "--steps", "7000000"], ["--midi-loud", "1", "--steps", "16000000"]]
    for args in runs:
        r = subprocess.run([str(bench), "--roms", roms] + args, capture_output=True, text=True)
        if r.returncode != 0:
            return True
    return False


def main() -> int:
    args = [a for a in sys.argv[1:] if not a.startswith("--only=")]
    only = {int(x) for a in sys.argv[1:] if a.startswith("--only=") for x in a[7:].split(",")}
    roms = args[0]
    target = args[1] if len(args) > 1 else "cpu"
    work = pathlib.Path(args[2]) if len(args) > 2 else ROOT / "out" / f"mutants-{target}"
    work.mkdir(parents=True, exist_ok=True)
    sv = {"cpu": CPU_SV, "mcu": MCU_SV, "pcm": PCM_SV, "submcu": SUBMCU_SV}[target]
    muts = {"cpu": CPU_MUTATIONS, "mcu": MCU_MUTATIONS, "pcm": PCM_MUTATIONS, "submcu": SUBMCU_MUTATIONS}[target]
    src = sv.read_text()
    missed = []
    incs = {f.name: f.read_text() for f in (ROLAND / "pcm").glob("*.svh")} if target == "pcm" else {}
    for i, (name, old, new) in enumerate(muts):
        if only and i not in only:
            continue
        where = [n for n, s in [(sv.name, src), *incs.items()] if s.count(old) == 1]
        if target == "pcm" and len(where) == 1 and where[0] != sv.name:
            d = work / f"m{i:02d}"
            shutil.rmtree(d, ignore_errors=True)
            d.mkdir()
            (d / sv.name).write_text(src)
            for n, s in incs.items():
                (d / n).write_text(s.replace(old, new) if n == where[0] else s)
            bench = build(target, [SDP_SV, d / sv.name], d / "obj")
            ok = caught(target, bench, roms)
            print(f"[{i:02d}] {'caught' if ok else 'MISSED'}: {name}", flush=True)
            if not ok and "expected MISSED" not in name:
                missed.append(name)
            continue
        if src.count(old) != 1:
            print(f"[{i}] {name}: pattern found {src.count(old)} times", file=sys.stderr)
            return 2
        if src.count(old) != 1:
            print(f"[{i}] {name}: pattern found {src.count(old)} times", file=sys.stderr)
            return 2
        d = work / f"m{i:02d}"
        shutil.rmtree(d, ignore_errors=True)
        d.mkdir()
        (d / sv.name).write_text(src.replace(old, new))
        if target == "pcm":
            for n, s in incs.items():
                (d / n).write_text(s)
        srcs = {"cpu": [d / sv.name], "mcu": [SDP_SV, CPU_SV, SUBMCU_SV, d / sv.name], "pcm": [SDP_SV, d / sv.name],
                "submcu": [SDP_SV, d / sv.name]}[target]
        bench = build(target, srcs, d / "obj")
        ok = caught(target, bench, roms)
        print(f"[{i:02d}] {'caught' if ok else 'MISSED'}: {name}", flush=True)
        if not ok and "expected MISSED" not in name:
            missed.append(name)
    print(f"{len(muts) - len(missed)}/{len(muts)} mutations caught")
    return 1 if missed else 0


if __name__ == "__main__":
    sys.exit(main())
