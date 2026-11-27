// SPDX-License-Identifier: MIT
// SWP00 MEG (effects) executor. Runs the program that tools/swp00_meg_gen.py compiles from MAME's
// swp00.cpp (the code after the voice loop, one program per variant, swp00_meg_prog.svh), one
// op at a time, in program order. The op semantics are MAME's helpers (m7, m9, m9v, lfo_step,
// filtered_lfo_step, alfo, the delay_block reads and writes) in 32-bit two's complement with
// 64-bit products, as the C++.
//
// Op word: op[63:59] d[58:51] a[50:43] b[42:35] c[34:27] c2[26:19] c3[18:11] x[10:0].
// Registers: 256 x 32 (two RAM copies, for two reads an op); register 0 is zero.
// Constants and offsets: the swp00 mram (constants 00-BF, offsets C0-FF), read through c_addr.
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

    // ---------------------------------------------------------------- registers
    logic [31:0] rfa [256], rfb [256];
    logic [7:0]  ra_addr, rb_addr, w_addr;
    logic [31:0] ra, rb, w_data;
    logic        w_en;
    always_ff @(posedge clk) begin
        if (w_en) begin
            rfa[w_addr] <= w_data;
            rfb[w_addr] <= w_data;
        end
        ra <= rfa[ra_addr];
        rb <= rfb[rb_addr];
    end

    // program ROM
    logic [PAW-1:0] pc;
    logic [63:0]    ins_q;
    always_ff @(posedge clk) ins_q <= PROG[pc];

    // ---------------------------------------------------------------- executor
    typedef enum logic [3:0] {
        E_IDLE, E_CLR, E_FETCH, E_DEC, E_X1, E_X2, E_X3, E_X4, E_MEM, E_MEM2, E_I1, E_I2, E_WB
    } est_t;
    est_t st;
    logic [63:0] ins;
    wire  [4:0]  op = ins[63:59];
    wire  [7:0]  f_d = ins[58:51], f_a = ins[50:43], f_b = ins[42:35], f_c = ins[34:27],
                 f_c2 = ins[26:19], f_c3 = ins[18:11];
    wire  [10:0] f_x = ins[10:0];

    logic [31:0] A, B, M1, t0, t1, v0, t_pos;
    logic [63:0] acc;
    logic        mem_second;

    // shared multiplier
    logic signed [32:0] ma, mb;
    wire  signed [65:0] mp = ma * mb;

    assign busy = st != E_IDLE;

    always_ff @(posedge clk) begin
        w_en <= 1'b0;
        done <= 1'b0;
        if (rst) begin
            st <= E_CLR;
            w_addr <= 8'h0;
            dm_req <= 1'b0;
            out_l <= 32'h0; out_r <= 32'h0;
        end else case (st)
            // registers start at zero (MAME's MEG state is zero at reset); register 0 stays 0
            E_CLR: begin
                w_en <= 1'b1; w_data <= 32'h0;
                w_addr <= w_addr + 8'd1;
                if (w_addr == 8'hff) st <= E_IDLE;
            end
            E_IDLE: if (start) begin
                pc <= VAR_START[variant];
                st <= E_FETCH;
            end
            E_FETCH: st <= E_DEC;                 // ins_q = PROG[pc] after this clock
            E_DEC: begin
                ins <= ins_q;
                ra_addr <= ins_q[50:43];
                rb_addr <= ins_q[42:35];
                // the constant (or offset, for delay line ops) the op reads first
                case (ins_q[63:59])
                    OP_RD, OP_WR, OP_RLFO, OP_RLFO2: c_addr <= 8'hc0 | {2'b0, ins_q[32:27]};
                    default: c_addr <= ins_q[34:27];
                endcase
                st <= E_X1;
            end
            // wait for the register and constant reads
            E_X1: st <= E_X2;
            E_X2: begin
                A <= ra; B <= rb; M1 <= {16'h0, c_data};
                // second constant, when the op has one
                case (op)
                    OP_FMIX, OP_ALFO: c_addr <= f_c2;
                    OP_RLFO: c_addr <= f_c3;
                    default: ;
                endcase
                // first product
                case (op)
                    OP_MUL7, OP_MUL9, OP_LFOMOD: begin ma <= {ra[31], ra}; mb <= {rext(c_data)[31], rext(c_data)}; end
                    OP_MUL9V: begin ma <= {ra[31], ra}; mb <= {rb[31], rb}; end
                    default: ;
                endcase
                st <= E_X3;
            end
            E_X3: begin
                st <= E_WB;
                w_addr <= f_d;
                case (op)
                    OP_END: begin done <= 1'b1; st <= E_IDLE; end
                    OP_MUL7: w_data <= mp[38:7];
                    OP_MUL9, OP_MUL9V: w_data <= mp[40:9];
                    OP_LFOMOD: w_data <= 32'($signed(mp[40:9]) >>> 13) + 32'h200;
                    OP_ADD: w_data <= A + B;
                    OP_SUB: w_data <= A - B;
                    OP_AND23: w_data <= A & 32'h7fffff;
                    OP_CONST: w_data <= {16'h0, M1[15:0]};
                    OP_SAT: w_data <= saturate(A);
                    OP_LDI: w_data <= {21'h0, f_x};
                    OP_LDMIX: w_data <= mix[f_c[2:0]];
                    OP_OUT: begin
                        if (f_c[0]) out_r <= A; else out_l <= A;
                        st <= E_FETCH; pc <= pc + 1'b1;
                    end
                    OP_LFOSTEP: w_data <= (A + lfo_get_step(M1[15:0])) & 32'h7fffff;
                    OP_LFOWRAPR: w_data <= lfo_wrap(A, rext(M1[15:0]));
                    OP_LFOSCALE: w_data <= lfo_saturate((A - (rext(M1[15:0]) << 13)) << 2);
                    OP_WRAPI: w_data <= lfo_wrap(A, {21'h0, f_x});
                    OP_SUBSAT: w_data <= lfo_saturate(A - (rext(M1[15:0]) << 13));
                    // filtered_lfo_step: (step(c) * A + (0x400000 - step(c2)) * B) >> 22
                    OP_FMIX: begin
                        ma <= {1'b0, lfo_get_step(M1[15:0])}; mb <= {A[31], A};
                        st <= E_X4;
                    end
                    // alfo: lfo_phase = lfo_wrap(A, B); (lfo_phase * rext(c)) >> 19
                    OP_ALFO: begin
                        t0 <= lfo_wrap(A, B);
                        ma <= {1'b0, lfo_wrap(A, B)}; mb <= {rext(M1[15:0])[31], rext(M1[15:0])};
                        st <= E_X4;
                    end
                    OP_RD, OP_WR: begin
                        dm_req <= 1'b1;
                        dm_we <= op == OP_WR;
                        dm_addr <= dl_addr(f_c2[1:0], buffer_offset + {16'h0, M1[15:0]});
                        dm_wdata <= A;
                        st <= E_MEM;
                    end
                    // rlfo: lfo_offset = u32(lfo_wrap(A, B) * rext(c3)), reads at +i and +i+1
                    OP_RLFO: begin
                        t0 <= lfo_wrap(A, B);
                        st <= E_X4;
                    end
                    // rlfo2: i = A >> 11, frac = A & 7ff
                    OP_RLFO2: begin : rlfo2
                        logic [31:0] pos;
                        pos = buffer_offset + {16'h0, M1[15:0]} + 32'($signed(A) >>> 11);
                        t1 <= {21'h0, A[10:0]};
                        t_pos <= pos;
                        dm_req <= 1'b1; dm_we <= 1'b0;
                        dm_addr <= dl_addr(f_c2[1:0], pos);
                        mem_second <= 1'b0;
                        st <= E_MEM2;
                    end
                    default: ;
                endcase
            end
            E_X4: begin
                case (op)
                    OP_FMIX: begin
                        if (!mem_second) begin
                            acc <= mp[63:0];
                            ma <= {1'b0, 32'h400000 - lfo_get_step(c_data)}; mb <= {B[31], B};
                            mem_second <= 1'b1;
                        end else begin
                            w_data <= 32'((acc + mp[63:0]) >> 22);
                            mem_second <= 1'b0;
                            st <= E_WB;
                        end
                    end
                    OP_ALFO: begin : alfo
                        logic [31:0] off, base, bamp;
                        off = rext(c_data);
                        if (f_x[0]) off = 32'h0 - off;
                        base = 32'($signed(mp[63:0]) >>> 19) + (off << 3);
                        bamp = ({23'h0, base[8:0]} | 32'h200) << base[12:9];
                        bamp = 32'($signed(bamp) >>> 8);
                        if ($signed(bamp) <= -32'sh200) bamp = 32'hfffffe01;
                        else if ($signed(bamp) >= 32'sh200) bamp = 32'h200;
                        w_data <= bamp;
                        st <= E_WB;
                    end
                    OP_RLFO: begin : rlfo
                        logic [31:0] lo;
                        logic [31:0] pos;
                        lo = t0 * rext(c_data);           // u32 product
                        pos = buffer_offset + {16'h0, M1[15:0]} + {22'h0, lo[31:22]};
                        t1 <= {10'h0, lo[21:0]};
                        t_pos <= pos;
                        dm_req <= 1'b1; dm_we <= 1'b0;
                        dm_addr <= dl_addr(f_c2[1:0], pos);
                        mem_second <= 1'b0;
                        st <= E_MEM2;
                    end
                    default: st <= E_WB;
                endcase
            end
            // plain read or write
            E_MEM: if (dm_ack) begin
                dm_req <= 1'b0;
                if (!dm_we) w_data <= dm_rdata;
                st <= dm_we ? E_FETCH : E_WB;
                if (dm_we) pc <= pc + 1'b1;
            end
            // two reads at pos and pos + 1, then (val1 * frac + val0 * (one - frac)) >> shift
            E_MEM2: if (dm_ack) begin
                if (!mem_second) begin
                    v0 <= dm_rdata;
                    mem_second <= 1'b1;
                    dm_addr <= dl_addr(f_c2[1:0], t_pos + 32'd1);
                end else begin
                    dm_req <= 1'b0;
                    mem_second <= 1'b0;
                    ma <= {dm_rdata[31], dm_rdata}; mb <= {1'b0, t1};
                    st <= E_I1;
                end
            end
            E_I1: begin
                acc <= mp[63:0];
                ma <= {v0[31], v0};
                mb <= {1'b0, (op == OP_RLFO ? 32'h400000 : 32'h800) - t1};
                st <= E_I2;
            end
            E_I2: begin
                w_data <= op == OP_RLFO ? 32'((acc + mp[63:0]) >> 22) : 32'((acc + mp[63:0]) >> 11);
                st <= E_WB;
            end
            E_WB: begin
                w_en <= 1'b1;
                pc <= pc + 1'b1;
                st <= E_FETCH;
            end
            default: st <= E_IDLE;
        endcase
    end
endmodule
