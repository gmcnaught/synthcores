// SPDX-License-Identifier: MIT
// SWP00 MEG (effects) executor. Runs the program that tools/swp00_meg_gen.py compiles from MAME's
// swp00.cpp (the code after the voice loop, one program per variant, swp00_meg_prog.svh) in
// program order. The op semantics are MAME's helpers (m7, m9, m9v, lfo_step, filtered_lfo_step,
// alfo, the delay_block reads and writes) in 32-bit two's complement with 64-bit products.
//
// Pipeline, one op a clock:
//   F  program ROM
//   D  register reads (two RAM copies) and the first constant (c_addr)
//   E1 operands, with the register written at the same clock edge as the read taken from the
//      write itself (bypass); multiplier inputs, delay line addresses and the second constant
//      (c2_addr) prepared
//   E2 execute (multiplier from registers), write back
// An op in E1 that reads the register the op in E2 writes waits one clock. Ops with two
// products or delay line accesses (FMIX, ALFO, RD, WR, RLFO, RLFO2) take extra clocks in E2,
// and the ops behind them wait.
//
// Op word: op[63:59] d[58:51] a[50:43] b[42:35] c[34:27] c2[26:19] c3[18:11] x[10:0].
// Registers: 256 x 32; register 0 is zero.
// Constants and offsets: the swp00 mram (constants 00-BF, offsets C0-FF), two read ports
// (c_addr, c2_addr; combinational, the RAMs in swp00.sv register them).
module swp00_meg (
    input  logic        clk,
    input  logic        rst,
    input  logic        start,
    output logic        done,
    output logic        busy,
    input  logic [1:0]  variant,
    input  logic [31:0] buffer_offset,
    input  logic [31:0] mix [7],
    output logic [7:0]  c_addr,
    input  logic [15:0] c_data,
    output logic [7:0]  c2_addr,
    input  logic [15:0] c2_data,
    output logic [31:0] out_l,
    output logic [31:0] out_r,
    output logic        dm_req,
    output logic        dm_we,
    output logic [18:0] dm_addr,
    output logic [31:0] dm_wdata,
    input  logic        dm_ack,
    input  logic [31:0] dm_rdata
);
    `include "swp00_meg_prog.svh"

    // ---------------------------------------------------------------- helpers (MAME swp00.cpp)
    // rext: 10-bit constant, values above 0x200 negative
    function automatic logic [31:0] rext(input logic [15:0] c);
        logic [31:0] v;
        v = {22'h0, c[9:0]};
        if (v > 32'h200) v = v | 32'hfffffc00;
        return v;
    endfunction
    // lfo_get_step
    function automatic logic [31:0] lfo_get_step(input logic [15:0] c);
        logic [2:0] e;
        e = c[9:7];
        return {25'h0, c[6:0]} << (e == 3'd7 ? 5'd15 : {2'b0, e});
    endfunction
    // lfo_wrap(phase, delta)
    function automatic logic [31:0] lfo_wrap(input logic [31:0] phase, input logic [31:0] delta);
        logic [31:0] p;
        p = (phase - (delta << 13)) & 32'h7fffff;
        if (p[22]) p = p ^ 32'h7fffff;
        return p;
    endfunction
    // lfo_saturate
    function automatic logic [31:0] lfo_saturate(input logic [31:0] p);
        if ($signed(p) < -32'sh400000) return 32'hffc00000;
        if ($signed(p) >= 32'sh400000) return 32'h3fffff;
        return p;
    endfunction
    function automatic logic [31:0] saturate(input logic [31:0] v);
        if ($signed(v) <= -32'sh20000) return 32'hfffe0000;
        if ($signed(v) > 32'sh1ffff) return 32'h1ffff;
        return v;
    endfunction
    // delay line address: base + ((buffer_offset + offset) & (size - 1))
    function automatic logic [18:0] dl_addr(input logic [1:0] buf_, input logic [31:0] pos);
        case (buf_)
            2'd0: return {2'b00, pos[16:0]};                    // reverb 00000-1FFFF
            2'd1: return 19'h20000 | {4'h0, pos[14:0]};         // chorus 20000-27FFF
            default: return 19'h28000 + {2'b00, pos[16:0]};     // variation 28000-47FFF
        endcase
    endfunction
    function automatic logic is_dl(input logic [4:0] op);
        return op == OP_RD || op == OP_WR || op == OP_RLFO || op == OP_RLFO2;
    endfunction

    // ---------------------------------------------------------------- pipeline registers
    logic        run;
    logic [PAW-1:0] pc;
    logic [63:0] ins_q;               // D
    logic        d_valid;
    logic [63:0] i1;                  // E1
    logic        v1;
    logic [63:0] i2;                  // E2
    logic        v2;
    logic [2:0]  es;                  // E2 sub-step of a multi-clock op
    logic        hold;                // E1 (and D, F) keep their op this clock

    wire  [4:0]  op1 = i1[63:59];
    wire  [7:0]  a1 = i1[50:43], b1 = i1[42:35];
    wire  [4:0]  op2 = i2[63:59];
    wire  [7:0]  d2 = i2[58:51], c2f = i2[26:19];
    wire  [10:0] x2 = i2[10:0];
    wire  [4:0]  d_op = ins_q[63:59];

    // program ROM (holds with the pipeline)
    wire stall = hold;
    `include "swp00_meg_rom.svh"

    // registers: read the operands of the op that is in E1 next clock
    logic [31:0] rfa [256], rfb [256];
    logic [31:0] ra_q, rb_q;
    logic        w_en;
    logic [7:0]  w_addr;
    logic [31:0] w_data;
    wire  [7:0]  rda = hold ? a1 : ins_q[50:43];
    wire  [7:0]  rdb = hold ? b1 : ins_q[42:35];
    always_ff @(posedge clk) begin
        if (w_en) begin
            rfa[w_addr] <= w_data;
            rfb[w_addr] <= w_data;
        end
        ra_q <= rfa[rda];
        rb_q <= rfb[rdb];
    end
    // the write at the clock edge of the read, for the bypass
    logic        pw_en;
    logic [7:0]  pw_addr;
    logic [31:0] pw_data;
    always_ff @(posedge clk) begin
        pw_en <= w_en; pw_addr <= w_addr; pw_data <= w_data;
    end

    // constants, both ports for the op that is in E1 next clock: port 1 its c (the offset index
    // for delay line ops), port 2 its second constant (c2, or c3 for rlfo)
    logic [7:0] d_caddr, i1_caddr, d_c2addr, i1_c2addr;
    always_comb begin
        d_caddr = is_dl(d_op) ? (8'hc0 | {2'b0, ins_q[32:27]}) : ins_q[34:27];
        i1_caddr = is_dl(op1) ? (8'hc0 | {2'b0, i1[32:27]}) : i1[34:27];
        d_c2addr = d_op == OP_RLFO ? ins_q[18:11] : ins_q[26:19];
        i1_c2addr = op1 == OP_RLFO ? i1[18:11] : i1[26:19];
        c_addr = hold ? i1_caddr : d_caddr;
        c2_addr = hold ? i1_c2addr : d_c2addr;
    end

    // ---------------------------------------------------------------- E1
    wire  [31:0] A1 = (pw_en && pw_addr == a1) ? pw_data : ra_q;
    wire  [31:0] B1 = (pw_en && pw_addr == b1) ? pw_data : rb_q;
    wire  [15:0] C1 = c_data;
    wire  [31:0] rC1 = rext(C1);
    wire  [31:0] rC2 = rext(c2_data);
    logic [32:0] ma1, mb1;                // first product of the op
    always_comb begin
        ma1 = 33'h0; mb1 = 33'h0;
        case (op1)
            OP_MUL7, OP_MUL9, OP_LFOMOD: begin ma1 = {rC1[31], rC1}; mb1 = {A1[31], A1}; end
            OP_MUL9V: begin ma1 = {B1[31], B1}; mb1 = {A1[31], A1}; end
            OP_FMIX: begin ma1 = {1'b0, lfo_get_step(C1)}; mb1 = {A1[31], A1}; end
            OP_ALFO: begin ma1 = {1'b0, lfo_wrap(A1, B1)}; mb1 = {rC1[31], rC1}; end
            default: ;
        endcase
    end
    // E2 writes a register the E1 op reads: wait for it
    logic e2_writes, hazard;
    always_comb begin
        case (op2)
            OP_WR, OP_OUT, OP_END: e2_writes = 1'b0;
            default: e2_writes = 1'b1;
        endcase
        hazard = v1 && v2 && e2_writes && d2 != 8'h0 && (a1 == d2 || b1 == d2);
    end

    // ---------------------------------------------------------------- E2
    logic [31:0] A, B, Cr, C2r, t0, t1, v0, t_pos, fm2;
    logic [15:0] C, C2;
    logic signed [32:0] ma, mb;
    logic [32:0] ma2, mb2;            // first product, from E1
    logic [63:0] acc;
    /* verilator lint_off UNOPTFLAT */
    wire  signed [65:0] mp = ma * mb;
    /* verilator lint_on UNOPTFLAT */

    logic        e_done;
    logic [31:0] res;
    logic        res_we;
    logic [31:0] al_off, al_base, al_bamp;
    always_comb begin
        e_done = 1'b1;
        res = 32'h0;
        res_we = 1'b0;
        al_off = 32'h0; al_base = 32'h0; al_bamp = 32'h0;
        ma = $signed(ma2); mb = $signed(mb2);
        if (v2) case (op2)
            OP_MUL7: begin res = mp[38:7]; res_we = 1'b1; end
            OP_MUL9, OP_MUL9V: begin res = mp[40:9]; res_we = 1'b1; end
            OP_LFOMOD: begin res = 32'($signed(mp[40:9]) >>> 13) + 32'h200; res_we = 1'b1; end
            OP_ADD: begin res = A + B; res_we = 1'b1; end
            OP_SUB: begin res = A - B; res_we = 1'b1; end
            OP_AND23: begin res = A & 32'h7fffff; res_we = 1'b1; end
            OP_CONST: begin res = {16'h0, C}; res_we = 1'b1; end
            OP_SAT: begin res = saturate(A); res_we = 1'b1; end
            OP_LDI: begin res = {21'h0, x2}; res_we = 1'b1; end
            OP_LDMIX: begin res = mix[i2[29:27]]; res_we = 1'b1; end
            OP_LFOSTEP: begin res = (A + lfo_get_step(C)) & 32'h7fffff; res_we = 1'b1; end
            OP_LFOWRAPR: begin res = lfo_wrap(A, Cr); res_we = 1'b1; end
            OP_LFOSCALE: begin res = lfo_saturate((A - (Cr << 13)) << 2); res_we = 1'b1; end
            OP_WRAPI: begin res = lfo_wrap(A, {21'h0, x2}); res_we = 1'b1; end
            OP_SUBSAT: begin res = lfo_saturate(A - (Cr << 13)); res_we = 1'b1; end
            // filtered_lfo_step: (step(c) * A + (0x400000 - step(c2)) * B) >> 22
            OP_FMIX: if (es == 3'd0) e_done = 1'b0;
                     else begin
                         ma = {1'b0, fm2}; mb = {B[31], B};
                         res = 32'((acc + mp[63:0]) >> 22); res_we = 1'b1;
                     end
            // alfo(A, B, c, c2, x)
            OP_ALFO: if (es == 3'd0) e_done = 1'b0;
                     else begin
                         al_off = C2r;
                         if (x2[0]) al_off = 32'h0 - al_off;
                         al_base = 32'($signed(acc) >>> 19) + (al_off << 3);
                         al_bamp = ({23'h0, al_base[8:0]} | 32'h200) << al_base[12:9];
                         al_bamp = 32'($signed(al_bamp) >>> 8);
                         if ($signed(al_bamp) <= -32'sh200) al_bamp = 32'hfffffe01;
                         else if ($signed(al_bamp) >= 32'sh200) al_bamp = 32'h200;
                         res = al_bamp; res_we = 1'b1;
                     end
            // delay line read / write: es 0 issues, done when the memory answers
            OP_RD: begin e_done = es != 3'd0 && dm_ack; res = dm_rdata; res_we = 1'b1; end
            OP_WR: e_done = es != 3'd0 && dm_ack;
            // rlfo/rlfo2: es 0: offset, first read     es 2: second read
            //             es 3: val1 * frac            es 4: + val0 * (one - frac)
            OP_RLFO, OP_RLFO2: begin
                e_done = 1'b0;
                if (es == 3'd4) begin
                    ma = {v0[31], v0}; mb = {1'b0, (op2 == OP_RLFO ? 32'h400000 : 32'h800) - t1};
                    res = op2 == OP_RLFO ? 32'((acc + mp[63:0]) >> 22) : 32'((acc + mp[63:0]) >> 11);
                    res_we = 1'b1; e_done = 1'b1;
                end else if (es == 3'd3) begin
                    ma = {dm_rdata[31], dm_rdata}; mb = {1'b0, t1};     // val1 * frac
                end
            end
            default: ;
        endcase
    end

    assign hold = (v2 && !e_done) || hazard;
    assign busy = run;

    always_ff @(posedge clk) begin
        done <= 1'b0;
        if (rst) begin
            run <= 1'b0; d_valid <= 1'b0; v1 <= 1'b0; v2 <= 1'b0; es <= 3'd0;
            dm_req <= 1'b0; dm_we <= 1'b0;
            out_l <= 32'h0; out_r <= 32'h0;
            pc <= '0;
        end else begin
            // ---- E2: multi-clock ops
            if (v2 && !e_done) begin
                case (op2)
                    OP_FMIX, OP_ALFO: begin
                        acc <= mp[63:0];
                        es <= 3'd1;
                    end
                    OP_RD, OP_WR: if (es == 3'd0) begin
                        dm_req <= 1'b1; dm_we <= op2 == OP_WR; dm_wdata <= A;
                        dm_addr <= dl_addr(c2f[1:0], t_pos);
                        es <= 3'd1;
                    end
                    OP_RLFO, OP_RLFO2: case (es)
                        3'd0: begin : rl0
                            logic [31:0] lo, pos;
                            lo = t0 * C2r;                          // u32 product (rlfo)
                            pos = op2 == OP_RLFO ? t_pos + {22'h0, lo[31:22]} : t_pos;
                            if (op2 == OP_RLFO) t1 <= {10'h0, lo[21:0]};
                            t_pos <= pos;
                            dm_req <= 1'b1; dm_we <= 1'b0;
                            dm_addr <= dl_addr(c2f[1:0], pos);
                            es <= 3'd2;
                        end
                        3'd2: if (dm_ack) begin
                            v0 <= dm_rdata;
                            dm_addr <= dl_addr(c2f[1:0], t_pos + 32'd1);
                            es <= 3'd3;
                        end
                        3'd3: if (dm_ack) begin
                            dm_req <= 1'b0;
                            acc <= mp[63:0];
                            es <= 3'd4;
                        end
                        default: ;
                    endcase
                    default: ;
                endcase
            end
            if (v2 && e_done) begin
                es <= 3'd0;
                if (op2 == OP_RD || op2 == OP_WR) dm_req <= 1'b0;
                if (op2 == OP_OUT) begin
                    if (i2[27]) out_r <= A; else out_l <= A;
                end
                if (op2 == OP_END) begin
                    run <= 1'b0; done <= 1'b1;
                end
            end

            // ---- advance
            if (!hold) begin
                // E1 -> E2, with its operands
                v2 <= v1 && !(v2 && op2 == OP_END);
                i2 <= i1;
                A <= A1; B <= B1; C <= C1; Cr <= rC1;
                C2 <= c2_data; C2r <= rC2;
                fm2 <= 32'h400000 - lfo_get_step(c2_data);
                ma2 <= ma1; mb2 <= mb1;
                case (op1)
                    OP_RD, OP_WR: t_pos <= buffer_offset + {16'h0, C1};
                    OP_RLFO: begin
                        t0 <= lfo_wrap(A1, B1);
                        t_pos <= buffer_offset + {16'h0, C1};
                    end
                    OP_RLFO2: begin
                        t1 <= {21'h0, A1[10:0]};
                        t_pos <= buffer_offset + {16'h0, C1} + 32'($signed(A1) >>> 11);
                    end
                    default: ;
                endcase
                // D -> E1, F -> D
                v1 <= d_valid && !(v1 && op1 == OP_END) && !(v2 && op2 == OP_END);
                i1 <= ins_q;
                d_valid <= run && !(d_valid && d_op == OP_END) && !(v1 && op1 == OP_END) &&
                           !(v2 && op2 == OP_END);
                pc <= pc + 1'b1;
            end else if (v2 && e_done)
                v2 <= 1'b0;                 // hazard: a bubble into E2
            if (start && !run) begin
                run <= 1'b1;
                pc <= VAR_START[variant];
                d_valid <= 1'b0; v1 <= 1'b0; v2 <= 1'b0;
            end
        end
    end

    // registers start at zero, as MAME's MEG state after device_reset
    logic [8:0] clr;
    always_ff @(posedge clk)
        if (rst) clr <= 9'h0;
        else if (!clr[8]) clr <= clr + 9'd1;

    // write back (register 0 stays zero)
    always_comb begin
        w_en = v2 && e_done && res_we && d2 != 8'h0;
        w_addr = d2;
        w_data = res;
        if (!clr[8]) begin
            w_en = 1'b1; w_addr = clr[7:0]; w_data = 32'h0;
        end
    end
endmodule
