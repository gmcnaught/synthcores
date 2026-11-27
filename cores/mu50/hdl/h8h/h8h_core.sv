// SPDX-License-Identifier: MIT
// H8/300H CPU core, advanced mode (24-bit addresses), as MAME's h8h_device runs it.
//
// The instruction handlers, the ALU helpers and the decoder are generated from MAME's h8.lst and
// h8.cpp by tools/h8h_gen.py into h8h_gen.svh and h8h_gen_states.svh. This file holds the registers, the bus port and
// the timing.
//
// Timing: each operation of a handler has MAME's cost in CPU states: 2 for a bus access (no wait
// states), n+1 for internal(n). An operation starts only when the previous one's states have
// elapsed (debt == 0); `ce` marks one elapsed state. Everything between operations takes FPGA
// clocks only. vtime counts the states of all started operations, which is MAME's
// total_cycles() at the same point.
//
// Bus: bus_req is held with the operation until bus_ack. Reads return the word (16-bit kinds)
// or the byte in bus_rdata[7:0] (OP_R8). Addresses of 16-bit kinds are even.

module h8h_core (
    input  logic        clk,
    input  logic        rst,
    input  logic        ce,

    output logic        bus_req,
    output logic [2:0]  bus_kind,
    output logic [23:0] bus_addr,
    output logic [15:0] bus_wdata,
    input  logic        bus_ack,
    input  logic [15:0] bus_rdata,

    // interrupt controller (h8_intc): requested vector (0 = none) and level
    input  logic [7:0]  irq_vector,
    input  logic [7:0]  irq_level,
    input  logic        syscr_ue,         // SYSCR.UE
    input  logic        standby_pending,  // SLEEP enters software standby
    output logic [1:0]  irq_filter,       // update_irq_filter(): 0 none, 1 I, 2 I+UI masked
    output logic        irq_taken,        // pulse: interrupt_taken()
    output logic [7:0]  irq_taken_vector,
    output logic        standby,
    output logic        illegal,

    output logic [63:0] vtime,

    // one pulse at every handler entry (MAME execute_run -> do_exec_full)
    output logic        tr_stb,
    output logic [23:0] tr_state,
    output logic [23:0] tr_npc,
    output logic [255:0] tr_r,             // m_R[15:0], 16 bits each
    output logic [7:0]  tr_ccr,
    output logic [7:0]  tr_tvec,
    output logic [7:0]  tr_tlvl
);
    localparam logic [3:0] OP_NONE = 4'd0, OP_F16 = 4'd1, OP_R8 = 4'd2, OP_R16 = 4'd3,
                           OP_W8 = 4'd4, OP_W16 = 4'd5, OP_INT = 4'd6, OP_EAT = 4'd7,
                           OP_DIV = 4'd8;

    // ---- C helpers (h8h_gen.py: every C value is a 32-bit vector after integer promotion)
    function automatic logic [31:0] c_u8(input logic [31:0] x);  return {24'h0, x[7:0]}; endfunction
    function automatic logic [31:0] c_s8(input logic [31:0] x);  return {{24{x[7]}}, x[7:0]}; endfunction
    function automatic logic [31:0] c_u16(input logic [31:0] x); return {16'h0, x[15:0]}; endfunction
    function automatic logic [31:0] c_s16(input logic [31:0] x); return {{16{x[15]}}, x[15:0]}; endfunction
    function automatic logic [31:0] c_b(input logic [31:0] x);   return {31'h0, x != 32'h0}; endfunction
    function automatic logic [63:0] c_zx64(input logic [31:0] x); return {32'h0, x}; endfunction
    function automatic logic [63:0] c_sx64(input logic [31:0] x); return {{32{x[31]}}, x}; endfunction
    function automatic logic [31:0] c_lo(input logic [63:0] x);  return x[31:0]; endfunction
    function automatic logic [7:0]  t8(input logic [31:0] x);    return x[7:0]; endfunction
    function automatic logic [15:0] t16(input logic [31:0] x);   return x[15:0]; endfunction
    function automatic logic [23:0] t24(input logic [31:0] x);   return x[23:0]; endfunction
    function automatic logic [31:0] c_sra(input logic [31:0] x, input logic [31:0] n);
        return 32'($signed(x) >>> n[4:0]);
    endfunction
    function automatic logic [31:0] c_sdiv(input logic [31:0] a, input logic [31:0] b);
        return (b == 32'h0) ? 32'h0 : 32'($signed(a) / $signed(b));
    endfunction
    function automatic logic [31:0] c_smod(input logic [31:0] a, input logic [31:0] b);
        return (b == 32'h0) ? 32'h0 : 32'($signed(a) % $signed(b));
    endfunction
    function automatic logic [31:0] c_udiv(input logic [31:0] a, input logic [31:0] b);
        return (b == 32'h0) ? 32'h0 : a / b;
    endfunction
    function automatic logic [31:0] c_umod(input logic [31:0] a, input logic [31:0] b);
        return (b == 32'h0) ? 32'h0 : a % b;
    endfunction

    // ---- register file, h8.h / h8h.h: m_R[0..7] = R0..R7, m_R[8..15] = E0..E7
    typedef logic [15:0][15:0] regs_t;
    function automatic logic [31:0] r8_r(input regs_t r, input logic [31:0] n);
        return n[3] ? {24'h0, r[n[2:0]][7:0]} : {24'h0, r[n[2:0]][15:8]};
    endfunction
    function automatic logic [31:0] r16_r(input regs_t r, input logic [31:0] n);
        return {16'h0, r[n[3:0]]};
    endfunction
    function automatic logic [31:0] r32_r(input regs_t r, input logic [31:0] n);
        return {r[{1'b1, n[2:0]}], r[{1'b0, n[2:0]}]};
    endfunction
    function automatic regs_t r8_w(input regs_t r, input logic [31:0] n, input logic [31:0] v);
        regs_t o;
        o = r;
        if (n[3]) o[n[2:0]][7:0] = v[7:0];
        else      o[n[2:0]][15:8] = v[7:0];
        return o;
    endfunction
    function automatic regs_t r16_w(input regs_t r, input logic [31:0] n, input logic [31:0] v);
        regs_t o;
        o = r;
        o[n[3:0]] = v[15:0];
        return o;
    endfunction
    function automatic regs_t r32_w(input regs_t r, input logic [31:0] n, input logic [31:0] v);
        regs_t o;
        o = r;
        o[{1'b0, n[2:0]}] = v[15:0];
        o[{1'b1, n[2:0]}] = v[31:16];
        return o;
    endfunction

    // h83003.cpp update_irq_filter()
    function automatic logic [1:0] filter_of(input logic [7:0] ccr, input logic ue);
        if (ue) return ccr[7] ? 2'd2 : 2'd0;
        if (ccr[7] && ccr[6]) return 2'd2;
        return ccr[7] ? 2'd1 : 2'd0;
    endfunction

    `include "h8h_gen.svh"

    // ---- state
    logic [SW-1:0] st, n_st;
    logic [31:0]   tmp1, tmp2, pc, npc, n_tmp1, n_tmp2, n_pc, n_npc;
    logic [15:0]   pir, n_pir;
    logic [4:0][15:0] ir, n_ir;
    logic [7:0]    ccr, exr, n_ccr, n_exr, tvec, tlvl, n_tvec, n_tlvl;
    regs_t         r;
    logic [23:0]   inst_state, n_inst_state;
    logic [15:0]   rd;
    logic [1:0]    filter, n_filter;
    logic          n_irq_taken, n_standby;
    logic [5:0]    debt;
    logic [31:0]   l_q, l_r, n_l_q, n_l_r;   // locals of DIVXS/DIVXU, across states

    // operation requested by the current state
    logic [3:0]    op_kind;
    logic [31:0]   op_addr;
    logic [15:0]   op_data;
    logic [5:0]    op_cost;

    // the register file's write port, applied when the state ends
    localparam logic [1:0] RF_NONE = 2'd0, RF_W8 = 2'd1, RF_W16 = 2'd2, RF_W32 = 2'd3;
    logic [1:0]    rf_we;
    logic [3:0]    rf_idx;
    logic [31:0]   rf_val;
    function automatic logic [3:0] t4(input logic [31:0] x); return x[3:0]; endfunction

    // shared ALU: inputs latched at the end of a state, result used by the next one
    logic          alu_ld;
    logic [ALU_SW-1:0] alu_sel, alu_sel_n;
    logic [31:0]   alu_c, alu_a, alu_b, alu_c_n, alu_a_n, alu_b_n;
    wire  [39:0]   alu_q = alu_eval(alu_sel, alu_c, alu_a, alu_b);

    // divider (C / and % of 32-bit values), one bit per clock
    logic [31:0]   div_a_n, div_b_n, div_q, div_r;
    logic          div_signed_n;

    // temporaries of generated code: a helper's or a division's result, inside one state
    logic [31:0]   f0, f1, f2, f3, f4, f5, f6, f7;

    always_comb begin
        n_st = st;
        n_tmp1 = tmp1; n_tmp2 = tmp2; n_pc = pc; n_npc = npc; n_pir = pir; n_ir = ir;
        n_ccr = ccr; n_exr = exr; n_tvec = tvec; n_tlvl = tlvl;
        n_inst_state = inst_state; n_filter = filter; n_l_q = l_q; n_l_r = l_r;
        n_irq_taken = 1'b0; n_standby = 1'b0;
        op_kind = OP_NONE; op_addr = 32'h0; op_data = 16'h0; op_cost = 6'd0;
        rf_we = RF_NONE; rf_idx = 4'h0; rf_val = 32'h0;
        alu_ld = 1'b0; alu_sel_n = alu_sel; alu_c_n = 32'h0; alu_a_n = 32'h0; alu_b_n = 32'h0;
        div_a_n = 32'h0; div_b_n = 32'h0; div_signed_n = 1'b0;
        f0 = 32'h0; f1 = 32'h0; f2 = 32'h0; f3 = 32'h0; f4 = 32'h0; f5 = 32'h0; f6 = 32'h0; f7 = 32'h0;
        case (st)
            S_EXEC: n_st = decode(inst_state);
            S_ILLEGAL: n_st = S_ILLEGAL;
            S_HALT: n_st = S_HALT;
            `include "h8h_gen_states.svh"
            default: n_st = S_ILLEGAL;
        endcase
    end

    // divider: magnitudes, restoring, 32 clocks; C semantics for signed (truncate to zero)
    logic        div_busy, div_done, div_neg_q, div_neg_r;
    logic [5:0]  div_n;
    logic [31:0] div_d, div_rem, div_quo;
    wire  [33:0] div_try = {1'b0, div_rem, div_quo[31]} - {2'b0, div_d};

    // an operation starts when the previous one's states have elapsed and the bus answers;
    // a division when the divider is done (FPGA clocks only, no states)
    wire op_bus = op_kind == OP_F16 || op_kind == OP_R8 || op_kind == OP_R16 ||
                  op_kind == OP_W8 || op_kind == OP_W16;
    wire go = op_kind == OP_NONE ? 1'b1 :
              op_kind == OP_DIV ? div_done :
              (debt == 6'd0 && (!op_bus || bus_ack));

    assign bus_req = op_bus && debt == 6'd0;
    assign bus_kind = op_kind[2:0];
    assign bus_addr = (op_kind == OP_R8 || op_kind == OP_W8) ? op_addr[23:0] : {op_addr[23:1], 1'b0};
    assign bus_wdata = op_data;

    assign tr_stb = st == S_EXEC;
    assign tr_state = inst_state;
    assign tr_npc = npc[23:0];
    assign tr_r = r;
    assign tr_ccr = ccr;
    assign tr_tvec = tvec;
    assign tr_tlvl = tlvl;
    assign irq_filter = filter;
    assign illegal = st == S_ILLEGAL;

    always_ff @(posedge clk) begin
        if (rst) begin
            st <= S_EXEC;
            inst_state <= 24'h010000;
            tmp1 <= 32'h0; tmp2 <= 32'h0; pc <= 32'h0; npc <= 32'h0; pir <= 16'h0; ir <= '0;
            ccr <= 8'h0; exr <= 8'h0; tvec <= 8'h0; tlvl <= 8'hff; r <= '0;
            rd <= 16'h0; filter <= 2'd0; debt <= 6'd0; vtime <= 64'd0;
            irq_taken <= 1'b0; irq_taken_vector <= 8'h0; standby <= 1'b0;
            l_q <= 32'h0; l_r <= 32'h0;
            alu_sel <= '0; alu_c <= 32'h0; alu_a <= 32'h0; alu_b <= 32'h0;
            div_busy <= 1'b0; div_done <= 1'b0;
        end else begin
            irq_taken <= 1'b0;
            if (ce && debt != 6'd0)
                debt <= debt - 6'd1;

            // divider
            if (op_kind == OP_DIV && !div_busy && !div_done) begin
                div_busy <= 1'b1;
                div_n <= 6'd32;
                div_neg_q <= div_signed_n && (div_a_n[31] ^ div_b_n[31]);
                div_neg_r <= div_signed_n && div_a_n[31];
                div_quo <= (div_signed_n && div_a_n[31]) ? 32'h0 - div_a_n : div_a_n;
                div_d <= (div_signed_n && div_b_n[31]) ? 32'h0 - div_b_n : div_b_n;
                div_rem <= 32'h0;
            end else if (div_busy) begin
                if (div_n != 6'd0) begin
                    if (!div_try[33]) begin
                        div_rem <= div_try[31:0];
                        div_quo <= {div_quo[30:0], 1'b1};
                    end else begin
                        div_rem <= {div_rem[30:0], div_quo[31]};
                        div_quo <= {div_quo[30:0], 1'b0};
                    end
                    div_n <= div_n - 6'd1;
                end else begin
                    div_busy <= 1'b0;
                    div_done <= 1'b1;
                    // C leaves x/0 undefined; MAME's handlers never divide by zero
                    div_q <= div_neg_q ? 32'h0 - div_quo : div_quo;
                    div_r <= div_neg_r ? 32'h0 - div_rem : div_rem;
                end
            end

            if (go) begin
                st <= n_st;
                tmp1 <= n_tmp1; tmp2 <= n_tmp2; pc <= n_pc; npc <= n_npc; pir <= n_pir; ir <= n_ir;
                ccr <= n_ccr; exr <= n_exr; tvec <= n_tvec; tlvl <= n_tlvl;
                inst_state <= n_inst_state; filter <= n_filter; l_q <= n_l_q; l_r <= n_l_r;
                case (rf_we)
                    RF_W8:  r <= r8_w(r, {28'h0, rf_idx}, rf_val);
                    RF_W16: r <= r16_w(r, {28'h0, rf_idx}, rf_val);
                    RF_W32: r <= r32_w(r, {28'h0, rf_idx}, rf_val);
                    default: ;
                endcase
                if (alu_ld) begin
                    alu_sel <= alu_sel_n; alu_c <= alu_c_n; alu_a <= alu_a_n; alu_b <= alu_b_n;
                end
                if (op_kind == OP_DIV) div_done <= 1'b0;
                if (n_irq_taken) begin
                    irq_taken <= 1'b1;
                    irq_taken_vector <= n_tvec;
                end
                if (n_standby) standby <= 1'b1;
                if (op_kind != OP_NONE && op_kind != OP_DIV) begin
                    debt <= op_cost;
                    vtime <= vtime + {58'h0, op_cost};
                end
                if (op_kind == OP_EAT) begin
                    debt <= 6'd1;
                    vtime <= vtime + 64'd1;
                end
                if (op_kind == OP_R8) rd <= {8'h0, bus_rdata[7:0]};
                else if (op_kind == OP_R16 || op_kind == OP_F16) rd <= bus_rdata;
            end
        end
    end
endmodule
