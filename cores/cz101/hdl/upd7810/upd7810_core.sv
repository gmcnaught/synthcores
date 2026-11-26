// SPDX-License-Identifier: MIT
// uPD7810/7811 CPU core: instruction execution.
//
// Follows MAME's upd7810 (BSD-3-Clause, Juergen Buchmueller) instruction for instruction:
// the same register, flag and skip results, the same memory writes in the same order and the
// same cycle counts (taken from MAME's tables by tools/upd7810_gen.py). See
// cores/cz101/docs/upd7810.md for the model.
//
// Timing model (as MAME's execute_run): an instruction is fetched and decoded, the
// peripherals advance by its cycle count (adv_req), then it executes. Interrupts are taken
// between instructions. The core runs an instruction in as many clocks as it needs; the
// owner paces real time with cyc_ce (one pulse per machine cycle = 3 crystal clocks), which
// pays off the cycles each instruction owes before the next one starts.
//
// Special registers (ports, timers, serial, masks) live outside, on the sfr_* bus.
// Interrupt priority and IRR live outside too (irq_vec / irq_ack, skit_*).

module upd7810_core #(
    parameter DECODE_HEX = "upd7810_decode.hex"
) (
    input  logic        clk,
    input  logic        reset,
    input  logic        cyc_ce,       // one machine cycle of real time

    // memory (program space), one byte per request; ack for one clock with rdata
    output logic [15:0] mem_addr,
    output logic        mem_req,
    output logic        mem_we,
    output logic [7:0]  mem_wdata,
    input  logic [7:0]  mem_rdata,
    input  logic        mem_ack,

    // special registers: id = selector - 32 (see upd7810_gen.py SFR), 16-bit for ETM/ECNT
    output logic [5:0]  sfr_id,
    output logic        sfr_req,
    output logic        sfr_we,
    output logic [15:0] sfr_wdata,
    input  logic [15:0] sfr_rdata,
    input  logic        sfr_ack,

    // SKIT/SKNIT: test an interrupt flag and clear it (NMI: line state, not cleared)
    output logic [4:0]  skit_id,
    output logic        skit_req,
    input  logic        skit_flag,
    input  logic        skit_ack,

    // advance the peripherals by adv_cycles before the instruction executes
    output logic [6:0]  adv_cycles,
    output logic        adv_req,
    input  logic        adv_ack,

    // interrupt controller (outside): highest pending unmasked vector, 0 = none
    input  logic [15:0] irq_vec,
    input  logic        irq_nmi,      // the pending one is NMI (taken even with IFF = 0)
    output logic        irq_ack,      // vector taken: clear its IRR bit per MAME's rules

    output logic        iff_o,

    // virtual time, in machine cycles since reset (MAME's CPU local time):
    //   vcyc      cycles completed (instruction starts at vcyc)
    //   acc_time  local time of the current memory access: the instruction's start, or its
    //             end for a prefixed opcode (PRE_xx charges its cycles before executing)
    // Before every instruction (and every halted cycle) the core raises at_boundary and waits
    // for ev_done: the board applies the events MAME's scheduler would have run first
    // (time < (vcyc + 1) cycles).
    output logic [31:0] vcyc,
    output logic [31:0] acc_time,
    output logic        at_boundary,
    input  logic        ev_done,

    // trace: one pulse per instruction, after it executes, before an interrupt is taken
    output logic        insn_done,
    output logic [6:0]  insn_cc,
    output logic [15:0] insn_pc,
    output logic [7:0]  insn_op,
    output logic [7:0]  insn_op2,
    output logic        halt_cycle,   // one machine cycle spent halted
    output logic        irq_taken,    // pulse with the vector in irq_taken_vec
    output logic [15:0] irq_taken_vec,
    output logic [7:0]  r_v, r_a, r_b, r_c, r_d, r_e, r_h, r_l,
    output logic [15:0] r_ea, r_sp, r_pc,
    output logic [7:0]  r_psw
);

    // ------------------------------------------------------------------ decode ROM
    typedef enum logic [5:0] {
        C_NOP, C_ALU8, C_ALU16, C_MOV16, C_LXI, C_LD16, C_ST16, C_PUSH, C_POP, C_INCDEC8, C_INX,
        C_DCX, C_SHIFT8, C_SHIFT16, C_RLD, C_RRD, C_DAA, C_NEGA, C_MUL, C_DIV, C_BLOCK, C_TABLE,
        C_EX, C_JMP, C_JR, C_JRE, C_JB, C_JEA, C_CALL, C_CALB, C_CALF, C_CALT, C_RET, C_RETS,
        C_RETI, C_SOFTI, C_BIT, C_SK, C_SKIT, C_EI, C_DI, C_HLT, C_STOP, C_CLC, C_STC, C_PREFIX,
        C_ILLEGAL0
    } cls_t;

    localparam logic [4:0] A_MOV = 0, A_ADD = 1, A_ADC = 2, A_SUB = 3, A_SBB = 4, A_ANA = 5,
                           A_ORA = 6, A_XRA = 7, A_ADDNC = 8, A_SUBNB = 9, A_GTA = 10,
                           A_LTA = 11, A_NEA = 12, A_EQA = 13, A_ONA = 14, A_OFFA = 15;

    localparam logic [3:0] M_B = 0, M_D = 1, M_H = 2, M_DP = 3, M_HP = 4, M_DM = 5, M_HM = 6,
                           M_DXX = 7, M_HA = 8, M_HB = 9, M_HEA = 10, M_HXX = 11, M_DP2 = 12,
                           M_HP2 = 13, M_WA = 14, M_W = 15;

    localparam logic [6:0] S_MEM = 7'h7F, S_IMM = 7'h7E;
    function automatic logic is_sfr(input logic [6:0] s);
        is_sfr = s[6] && s != S_MEM && s != S_IMM;
    endfunction

    localparam logic [7:0] F_CY = 8'h01, F_L0 = 8'h04, F_L1 = 8'h08, F_HC = 8'h10, F_SK = 8'h20,
                           F_Z = 8'h40;

    (* ramstyle = "M10K" *) logic [47:0] dec_rom [0:2047];
    initial $readmemh(DECODE_HEX, dec_rom);
    logic [10:0] dec_addr;
    logic [47:0] dec_q;
    always_ff @(posedge clk) dec_q <= dec_rom[dec_addr];

    // decoded fields of the current instruction
    logic [2:0]  d_len;
    logic [5:0]  d_cyc;
    logic [3:0]  d_cyc_skip;
    logic [1:0]  d_lmask;
    cls_t        d_cls;
    logic [4:0]  d_aop;
    logic [6:0]  d_s1, d_s2;
    logic [3:0]  d_mode;
    logic [1:0]  d_lskip;
    logic [1:0]  d_nargs;

    // XX-table fields kept for the skip path and the cycle total
    logic [5:0]  x_cyc;
    logic [3:0]  x_cyc_skip;
    logic [2:0]  x_len;
    logic [1:0]  x_lmask;

    // ------------------------------------------------------------------ registers
    logic [7:0]  V, A, B, C, D, E, H, L, EAH, EAL, PSW;
    logic [15:0] SP, PC;
    logic [15:0] VA2, BC2, DE2, HL2, EA2;
    logic        IFF, iff_pending, halt;

    wire [15:0] BC = {B, C};
    wire [15:0] DE = {D, E};
    wire [15:0] HL = {H, L};
    wire [15:0] EA = {EAH, EAL};
    wire [15:0] VA = {V, A};

    assign r_v = V; assign r_a = A; assign r_b = B; assign r_c = C;
    assign r_d = D; assign r_e = E; assign r_h = H; assign r_l = L;
    assign r_ea = EA; assign r_sp = SP; assign r_pc = PC; assign r_psw = PSW;
    assign iff_o = IFF;

    function automatic logic [7:0] rd8(input logic [3:0] s);
        case (s)
            4'd0: rd8 = V;   4'd1: rd8 = A;   4'd2: rd8 = B;   4'd3: rd8 = C;
            4'd4: rd8 = D;   4'd5: rd8 = E;   4'd6: rd8 = H;   4'd7: rd8 = L;
            4'd8: rd8 = EAH; default: rd8 = EAL;
        endcase
    endfunction

    function automatic logic [15:0] rd16(input logic [2:0] s);
        case (s)
            3'd0: rd16 = SP; 3'd1: rd16 = BC; 3'd2: rd16 = DE; 3'd3: rd16 = HL;
            3'd4: rd16 = EA; default: rd16 = VA;
        endcase
    endfunction

    function automatic logic [7:0] rd16_lo(input logic [2:0] s);
        logic [15:0] t;
        t = rd16(s);
        rd16_lo = t[7:0];
    endfunction
    function automatic logic [7:0] rd16_hi(input logic [2:0] s);
        logic [15:0] t;
        t = rd16(s);
        rd16_hi = t[15:8];
    endfunction

    // ------------------------------------------------------------------ flag helpers
    // MAME's ZHC_ADD / ZHC_SUB, evaluated at the width of MAME's temporary (8, 16 or 32 bits).
    function automatic logic [7:0] zhc_add(input logic [7:0] psw, input logic [31:0] after,
                                           input logic [31:0] bef, input logic carry);
        logic [7:0] p;
        p = psw;
        p[6] = (after == 0);
        if (after == bef)      p[0] = carry;
        else if (after < bef)  p[0] = 1'b1;
        else                      p[0] = 1'b0;
        p[4] = (after[3:0] < bef[3:0]);
        return p;
    endfunction

    function automatic logic [7:0] zhc_sub(input logic [7:0] psw, input logic [31:0] after,
                                           input logic [31:0] bef, input logic carry);
        logic [7:0] p;
        p = psw;
        p[6] = (after == 0);
        if (after == bef)      p[0] = carry;
        else if (after > bef)  p[0] = 1'b1;
        else                      p[0] = 1'b0;
        p[4] = (after[3:0] > bef[3:0]);
        return p;
    endfunction

    // ALU for 8- and 16-bit operations. w16 selects 16-bit width.
    // Returns {write, psw_out, result}.
    logic [15:0] alu_res;
    logic [7:0]  alu_psw;
    logic        alu_wr;
    logic [15:0] alu_l, alu_r;
    logic [4:0]  alu_op;
    logic        alu_w16;

    always_comb begin
        logic        cy;
        logic [31:0] t32, lz, mask;
        logic [7:0]  p;
        cy = PSW[0];
        lz = alu_w16 ? {16'h0, alu_l} : {24'h0, alu_l[7:0]};
        mask = alu_w16 ? 32'h0000FFFF : 32'h000000FF;
        p = PSW;
        alu_res = alu_l;
        alu_wr = 1'b0;
        t32 = 32'h0;
        case (alu_op)
            A_MOV: begin alu_res = alu_r; alu_wr = 1'b1; end
            A_ADD, A_ADDNC: begin
                t32 = (lz + {16'h0, alu_r}) & mask;
                p = zhc_add(p, t32, lz, 1'b0);
                alu_res = t32[15:0]; alu_wr = 1'b1;
                if (alu_op == A_ADDNC && !p[0]) p[5] = 1'b1;
            end
            A_ADC: begin
                t32 = (lz + {16'h0, alu_r} + {31'h0, cy}) & mask;
                p = zhc_add(p, t32, lz, cy);
                alu_res = t32[15:0]; alu_wr = 1'b1;
            end
            A_SUB, A_SUBNB, A_LTA, A_NEA, A_EQA: begin
                t32 = (lz - {16'h0, alu_r}) & mask;
                p = zhc_sub(p, t32, lz, 1'b0);
                alu_res = t32[15:0];
                alu_wr = (alu_op == A_SUB || alu_op == A_SUBNB);
                case (alu_op)
                    A_SUBNB: if (!p[0]) p[5] = 1'b1;
                    A_LTA:   if (p[0])  p[5] = 1'b1;
                    A_NEA:   if (!p[6]) p[5] = 1'b1;
                    A_EQA:   if (p[6])  p[5] = 1'b1;
                    default: ;
                endcase
            end
            A_SBB: begin
                t32 = (lz - {16'h0, alu_r} - {31'h0, cy}) & mask;
                p = zhc_sub(p, t32, lz, cy);
                alu_res = t32[15:0]; alu_wr = 1'b1;
            end
            A_GTA: begin
                // MAME: uint16_t (8-bit) or uint32_t (16-bit) temporary
                t32 = lz - {16'h0, alu_r} - 32'd1;
                if (!alu_w16) t32 = t32 & 32'h0000FFFF;
                p = zhc_sub(p, t32, lz, 1'b0);
                if (!p[0]) p[5] = 1'b1;
            end
            A_ANA, A_ORA, A_XRA: begin
                case (alu_op)
                    A_ANA:   t32 = lz & {16'h0, alu_r};
                    A_ORA:   t32 = lz | {16'h0, alu_r};
                    default: t32 = lz ^ {16'h0, alu_r};
                endcase
                t32 = t32 & mask;
                p[6] = (t32 == 0);
                alu_res = t32[15:0]; alu_wr = 1'b1;
            end
            A_ONA: begin
                if (((lz & {16'h0, alu_r}) & mask) != 0) begin p[6] = 1'b0; p[5] = 1'b1; end
                else p[6] = 1'b1;
            end
            default: begin // A_OFFA
                if (((lz & {16'h0, alu_r}) & mask) != 0) p[6] = 1'b0;
                else begin p[6] = 1'b1; p[5] = 1'b1; end
            end
        endcase
        alu_psw = p;
    end

    // ------------------------------------------------------------------ state machine
    typedef enum logic [3:0] {
        ST_RESET, ST_PACE, ST_FETCH, ST_DEC0W, ST_DEC0, ST_FETCH2, ST_DEC1W, ST_ISSUE, ST_ADV,
        ST_EXEC, ST_END, ST_IRQ, ST_HALT_ADV, ST_ADV2
    } st_t;
    st_t st;

    logic [7:0]  op, op2;
    logic        prefixed;
    logic [6:0]  cc;            // cycles of this instruction
    logic [6:0]  adv2_cycles;   // prefix-table cycles (second handle_timers call)
    logic [7:0]  owed;          // machine cycles not yet paid by cyc_ce
    logic        skipping;
    logic [15:0] ppc;
    logic [7:0]  arg0, arg1;
    logic [4:0]  step;
    logic        wait_bus;      // waiting for mem/sfr/skit/adv ack
    logic [15:0] addr;          // effective address
    logic [7:0]  lv;            // left operand (8-bit)
    logic [15:0] lv16, rv16;
    logic [7:0]  rv;            // right operand
    logic [7:0]  tmp8;
    logic [15:0] tmp16;
    // divider
    logic [15:0] div_q;
    logic [7:0]  div_r;
    logic [7:0]  div_d;
    logic [4:0]  div_n;

    // bus request helpers drive these registers
    always_comb begin
        alu_op = d_aop; alu_w16 = (d_cls == C_ALU16);
        alu_l = alu_w16 ? lv16 : {8'h0, lv};
        alu_r = alu_w16 ? rv16 : {8'h0, rv};
    end

    // effective address for the current mode (args already fetched)
    function automatic logic [15:0] eff_addr(input logic [3:0] m);
        case (m)
            M_B:   eff_addr = BC;
            M_D, M_DP, M_DM, M_DP2: eff_addr = DE;
            M_H, M_HP, M_HM, M_HP2: eff_addr = HL;
            M_DXX: eff_addr = DE + {8'h0, arg0};
            M_HA:  eff_addr = HL + {8'h0, A};
            M_HB:  eff_addr = HL + {8'h0, B};
            M_HEA: eff_addr = HL + EA;
            M_HXX: eff_addr = HL + {8'h0, arg0};
            M_WA:  eff_addr = {V, arg0};
            default: eff_addr = {arg1, arg0};
        endcase
    endfunction

    // memory request
    task automatic mem_read(input logic [15:0] a);
        mem_addr <= a; mem_we <= 1'b0; mem_req <= 1'b1; wait_bus <= 1'b1;
    endtask
    task automatic mem_write(input logic [15:0] a, input logic [7:0] d);
        mem_addr <= a; mem_we <= 1'b1; mem_wdata <= d; mem_req <= 1'b1; wait_bus <= 1'b1;
    endtask
    task automatic sfr_read(input logic [5:0] id);
        sfr_id <= id; sfr_we <= 1'b0; sfr_req <= 1'b1; wait_bus <= 1'b1;
    endtask
    task automatic sfr_write(input logic [5:0] id, input logic [15:0] d);
        sfr_id <= id; sfr_we <= 1'b1; sfr_wdata <= d; sfr_req <= 1'b1; wait_bus <= 1'b1;
    endtask

    task automatic wr8(input logic [3:0] s, input logic [7:0] d);
        case (s)
            4'd0: V <= d;   4'd1: A <= d;   4'd2: B <= d;   4'd3: C <= d;
            4'd4: D <= d;   4'd5: E <= d;   4'd6: H <= d;   4'd7: L <= d;
            4'd8: EAH <= d; default: EAL <= d;
        endcase
    endtask
    task automatic wr16(input logic [2:0] s, input logic [15:0] d);
        case (s)
            3'd0: SP <= d;
            3'd1: begin B <= d[15:8]; C <= d[7:0]; end
            3'd2: begin D <= d[15:8]; E <= d[7:0]; end
            3'd3: begin H <= d[15:8]; L <= d[7:0]; end
            3'd4: begin EAH <= d[15:8]; EAL <= d[7:0]; end
            default: begin V <= d[15:8]; A <= d[7:0]; end
        endcase
    endtask

    // post-increment/decrement of the mode register
    task automatic mode_update(input logic [3:0] m);
        case (m)
            M_DP:  {D, E} <= DE + 16'd1;
            M_HP:  {H, L} <= HL + 16'd1;
            M_DM:  {D, E} <= DE - 16'd1;
            M_HM:  {H, L} <= HL - 16'd1;
            M_DP2: {D, E} <= DE + 16'd2;
            M_HP2: {H, L} <= HL + 16'd2;
            default: ;
        endcase
    endtask

    logic [7:0]  bus_q;     // last memory read
    logic [15:0] sfr_q;     // last special-register read
    logic        skit_q;
    logic        halt_irq;  // ST_IRQ entered from the halt loop: MAME skips m_iff = m_iff_pending
    logic [6:0]  owed_add;
    assign acc_time = vcyc + ((prefixed && st == ST_EXEC) ? {25'h0, cc} : 32'h0);
    assign at_boundary = (st == ST_PACE) && owed == 0 && !wait_bus;

    always_ff @(posedge clk) begin
        insn_done <= 1'b0;
        halt_cycle <= 1'b0;
        irq_taken <= 1'b0;
        irq_ack <= 1'b0;
        owed_add = 7'd0;

        if (mem_ack)  begin mem_req <= 1'b0;  bus_q <= mem_rdata;   wait_bus <= 1'b0; end
        if (sfr_ack)  begin sfr_req <= 1'b0;  sfr_q <= sfr_rdata;   wait_bus <= 1'b0; end
        if (skit_ack) begin skit_req <= 1'b0; skit_q <= skit_flag;  wait_bus <= 1'b0; end
        if (adv_ack)  begin adv_req <= 1'b0;                        wait_bus <= 1'b0; end

        if (reset) begin
            st <= ST_RESET;
            {V, A, B, C, D, E, H, L, EAH, EAL, PSW} <= '0;
            SP <= 16'h0; PC <= 16'h0;
            {VA2, BC2, DE2, HL2, EA2} <= '0;
            IFF <= 1'b0; iff_pending <= 1'b0; halt <= 1'b0;
            mem_req <= 1'b0; sfr_req <= 1'b0; skit_req <= 1'b0; adv_req <= 1'b0;
            wait_bus <= 1'b0;
            step <= 0;
            op2 <= 8'h0;
            halt_irq <= 1'b0;
            vcyc <= 32'h0;
            prefixed <= 1'b0;
        end else if (!wait_bus) begin
            case (st)
                ST_RESET: st <= ST_PACE;

                ST_PACE: begin
                    // the next instruction starts once the previous ones' cycles are paid
                    // and the board has applied the events due before it
                    if (owed == 0 && ev_done) begin
                        if (halt) begin
                            adv_cycles <= 7'd1; adv_req <= 1'b1; wait_bus <= 1'b1;
                            st <= ST_HALT_ADV;
                        end else begin
                            ppc <= PC;
                            mem_read(PC);
                            st <= ST_FETCH;
                        end
                    end
                end

                ST_HALT_ADV: begin
                    owed_add = 7'd1;
                    vcyc <= vcyc + 32'd1;
                    halt_cycle <= 1'b1;
                    halt_irq <= 1'b1;
                    step <= 0;
                    st <= ST_IRQ;
                end

                ST_FETCH: begin
                    op <= bus_q;
                    PC <= PC + 16'd1;
                    dec_addr <= {3'd0, bus_q};
                    st <= ST_DEC0W;
                end

                ST_DEC0W: st <= ST_DEC0;   // decode ROM read

                ST_DEC0: begin
                    x_len <= dec_q[2:0]; x_cyc <= dec_q[8:3]; x_cyc_skip <= dec_q[12:9];
                    x_lmask <= dec_q[14:13];
                    PSW <= PSW & ~{4'b0, dec_q[14:13], 2'b0};
                    if (cls_t'(dec_q[20:15]) == C_PREFIX) begin
                        prefixed <= 1'b1;
                        mem_read(PC);
                        st <= ST_FETCH2;
                    end else begin
                        prefixed <= 1'b0;
                        st <= ST_ISSUE;
                    end
                end

                ST_FETCH2: begin
                    op2 <= bus_q;
                    PC <= PC + 16'd1;
                    case (op)
                        8'h48: dec_addr <= {3'd1, bus_q};
                        8'h4C: dec_addr <= {3'd2, bus_q};
                        8'h4D: dec_addr <= {3'd3, bus_q};
                        8'h60: dec_addr <= {3'd4, bus_q};
                        8'h64: dec_addr <= {3'd5, bus_q};
                        8'h70: dec_addr <= {3'd6, bus_q};
                        default: dec_addr <= {3'd7, bus_q};
                    endcase
                    st <= ST_DEC1W;
                end

                ST_DEC1W: st <= ST_ISSUE;  // decode ROM read

                ST_ISSUE: begin
                    // dec_q holds the entry of the executing table (XX or the prefix table)
                    {d_nargs, d_lskip, d_mode, d_s2, d_s1, d_aop} <= dec_q[47:21];
                    d_cls <= cls_t'(dec_q[20:15]);
                    d_lmask <= dec_q[14:13]; d_cyc_skip <= dec_q[12:9];
                    d_cyc <= dec_q[8:3]; d_len <= dec_q[2:0];
                    if (PSW[5] && op != 8'h72) begin
                        // skipped (SOFTI is never skipped)
                        skipping <= 1'b1;
                        PSW[5] <= 1'b0;
                        if (prefixed) begin
                            PC <= PC + {13'h0, dec_q[2:0]} - 16'd2;
                            cc <= {3'b0, dec_q[12:9]};
                            adv_cycles <= {3'b0, dec_q[12:9]};
                        end else begin
                            PC <= PC + {13'h0, x_len} - 16'd1;
                            cc <= {3'b0, x_cyc_skip};
                            adv_cycles <= {3'b0, x_cyc_skip};
                        end
                    end else begin
                        // MAME: handle_timers(main-table cycles), then for a prefixed opcode
                        // PRE_xx calls handle_timers(prefix-table cycles) separately (the A/D
                        // converter acts once per call)
                        skipping <= 1'b0;
                        cc <= {1'b0, x_cyc} + (prefixed ? {1'b0, dec_q[8:3]} : 7'd0);
                        adv_cycles <= {1'b0, x_cyc};
                        adv2_cycles <= {1'b0, dec_q[8:3]};
                    end
                    adv_req <= 1'b1; wait_bus <= 1'b1;
                    step <= 0;
                    st <= ST_ADV;
                end

                ST_ADV: begin
                    if (!skipping && prefixed) begin
                        adv_cycles <= adv2_cycles;
                        adv_req <= 1'b1; wait_bus <= 1'b1;
                        st <= ST_ADV2;
                    end else st <= skipping ? ST_END : ST_EXEC;
                end

                ST_ADV2: st <= ST_EXEC;

                ST_EXEC: exec_step();

                ST_END: begin
                    vcyc <= vcyc + {25'h0, cc};
                    insn_done <= 1'b1;
                    insn_cc <= cc;
                    insn_pc <= ppc;
                    insn_op <= op;
                    insn_op2 <= op2;
                    owed_add = cc;
                    halt_irq <= 1'b0;
                    st <= ST_IRQ;
                    step <= 0;
                end

                ST_IRQ: begin
                    // MAME upd7810_take_irq, then (outside the halt loop) m_iff = m_iff_pending
                    case (step)
                        5'd0: begin
                            if (irq_vec != 16'h0 && (IFF || irq_nmi)) begin
                                halt <= 1'b0;
                                SP <= SP - 16'd1;
                                mem_write(SP - 16'd1, PSW);
                                step <= 5'd1;
                            end else begin
                                if (irq_vec != 16'h0) halt <= 1'b0;
                                if (!halt_irq) IFF <= iff_pending;
                                st <= ST_PACE;
                            end
                        end
                        5'd1: begin SP <= SP - 16'd1; mem_write(SP - 16'd1, PC[15:8]); step <= 5'd2; end
                        5'd2: begin SP <= SP - 16'd1; mem_write(SP - 16'd1, PC[7:0]); step <= 5'd3; end
                        default: begin
                            irq_taken <= 1'b1;
                            irq_taken_vec <= irq_vec;
                            irq_ack <= 1'b1;
                            IFF <= 1'b0; iff_pending <= 1'b0;
                            PSW <= PSW & ~(F_SK | F_L0 | F_L1);
                            PC <= irq_vec;
                            st <= ST_PACE;
                        end
                    endcase
                end

                default: st <= ST_PACE;
            endcase
        end

        // cycle debt: paid one machine cycle per cyc_ce
        if (reset) owed <= 8'h0;
        else owed <= owed + {1'b0, owed_add} - ((cyc_ce && owed != 0) ? 8'd1 : 8'd0);
    end

    // ------------------------------------------------------------------ execution
    // Called once per clock in ST_EXEC while no bus request is outstanding. Step 0 fetches
    // the operand bytes (RDOPARG); later steps are per class. Each step either issues one
    // bus request and advances, or finishes with st <= ST_END.
    task automatic finish();
        st <= ST_END;
    endtask

    task automatic exec_step();
        logic [15:0] ea_;
        logic [7:0]  b;
        ea_ = eff_addr(d_mode);

        if (step == 5'd0) begin
            // RDOPARG: operand bytes, unless an L0/L1 string skip applies
            if (d_lskip != 0 && ((d_lskip == 2'd1 && PSW[2]) || (d_lskip == 2'd2 && PSW[3]))) begin
                PC <= PC + {14'h0, d_nargs};
                finish();
            end else if (d_nargs != 0) begin
                mem_read(PC); PC <= PC + 16'd1; step <= 5'd28;
            end else step <= 5'd1;
        end else if (step == 5'd28) begin
            arg0 <= bus_q;
            if (d_nargs == 2'd2) begin mem_read(PC); PC <= PC + 16'd1; step <= 5'd29; end
            else step <= 5'd1;
        end else if (step == 5'd29) begin
            arg1 <= bus_q;
            step <= 5'd1;
        end else
        case (d_cls)
            C_NOP, C_ILLEGAL0, C_PREFIX: finish();

            C_ALU8: begin
                // step 1: read left operand when the operation needs it
                // step 2: read right operand
                // step 3: compute, write back
                case (step)
                    5'd1: begin
                        if (d_aop == A_MOV) step <= 5'd3;
                        else if (d_s1 == S_MEM) begin mem_read(ea_); step <= 5'd2; end
                        else if (is_sfr(d_s1)) begin sfr_read(d_s1[5:0]); step <= 5'd2; end
                        else begin lv <= rd8(d_s1[3:0]); step <= 5'd3; end
                    end
                    5'd2: begin
                        lv <= is_sfr(d_s1) ? sfr_q[7:0] : bus_q;
                        step <= 5'd3;
                    end
                    5'd3: begin
                        if (d_s2 == S_IMM) begin rv <= (d_nargs == 2'd2) ? arg1 : arg0; step <= 5'd5; end
                        else if (d_s2 == S_MEM) begin mem_read(ea_); step <= 5'd4; end
                        else if (is_sfr(d_s2)) begin sfr_read(d_s2[5:0]); step <= 5'd4; end
                        else begin rv <= rd8(d_s2[3:0]); step <= 5'd5; end
                    end
                    5'd4: begin
                        rv <= (d_s2 == S_MEM) ? bus_q : sfr_q[7:0];
                        step <= 5'd5;
                    end
                    5'd5: begin
                        // memory-source forms update DE/HL after the read
                        if (d_s2 == S_MEM) mode_update(d_mode);
                        PSW <= (d_aop == A_MOV) ? PSW : alu_psw;
                        if (alu_wr) begin
                            if (d_s1 == S_MEM) begin
                                mem_write(ea_, alu_res[7:0]);
                                step <= 5'd6;
                            end else if (is_sfr(d_s1)) begin
                                sfr_write(d_s1[5:0], {8'h0, alu_res[7:0]});
                                step <= 5'd6;
                            end else begin
                                wr8(d_s1[3:0], alu_res[7:0]);
                                step <= 5'd6;
                            end
                        end else step <= 5'd6;
                        // MVI A / MVI L: set the string flag
                        if (d_lskip == 2'd2) PSW[3] <= 1'b1;
                        if (d_lskip == 2'd1) PSW[2] <= 1'b1;
                    end
                    default: begin
                        // memory-destination forms (STAX D+/H- ...) update after the write
                        if (d_s1 == S_MEM) mode_update(d_mode);
                        finish();
                    end
                endcase
            end

            C_ALU16: begin
                case (step)
                    5'd1: begin
                        lv16 <= EA;
                        rv16 <= d_s2[5] ? {8'h0, rd8(d_s2[3:0])} : rd16(d_s2[2:0]);
                        step <= 5'd2;
                    end
                    default: begin
                        PSW <= alu_psw;
                        if (alu_wr) {EAH, EAL} <= alu_res;
                        finish();
                    end
                endcase
            end

            C_MOV16: begin
                case (step)
                    5'd1: begin
                        if (is_sfr(d_s2)) begin sfr_read(d_s2[5:0]); step <= 5'd2; end
                        else if (is_sfr(d_s1)) begin sfr_write(d_s1[5:0], rd16(d_s2[2:0])); step <= 5'd3; end
                        else begin wr16(d_s1[2:0], rd16(d_s2[2:0])); finish(); end
                    end
                    5'd2: begin wr16(d_s1[2:0], sfr_q); finish(); end
                    default: finish();
                endcase
            end

            C_LXI: begin
                wr16(d_s1[2:0], {arg1, arg0});
                if (d_lskip == 2'd1) PSW[2] <= 1'b1;
                finish();
            end

            C_LD16: begin
                case (step)
                    5'd1: begin addr <= ea_; mem_read(ea_); step <= 5'd2; end
                    5'd2: begin tmp8 <= bus_q; mem_read(addr + 16'd1); step <= 5'd3; end
                    default: begin
                        wr16(d_s1[2:0], {bus_q, tmp8});
                        mode_update(d_mode);
                        finish();
                    end
                endcase
            end

            C_ST16: begin
                case (step)
                    5'd1: begin tmp16 <= rd16(d_s1[2:0]); addr <= ea_;
                                mem_write(ea_, rd16_lo(d_s1[2:0])); step <= 5'd2; end
                    5'd2: begin mem_write(addr + 16'd1, tmp16[15:8]); step <= 5'd3; end
                    default: begin mode_update(d_mode); finish(); end
                endcase
            end

            C_PUSH: begin
                case (step)
                    5'd1: begin tmp16 <= rd16(d_s1[2:0]); SP <= SP - 16'd1;
                                mem_write(SP - 16'd1, rd16_hi(d_s1[2:0])); step <= 5'd2; end
                    5'd2: begin SP <= SP - 16'd1; mem_write(SP - 16'd1, tmp16[7:0]); step <= 5'd3; end
                    default: finish();
                endcase
            end

            C_POP: begin
                case (step)
                    5'd1: begin mem_read(SP); SP <= SP + 16'd1; step <= 5'd2; end
                    5'd2: begin tmp8 <= bus_q; mem_read(SP); SP <= SP + 16'd1; step <= 5'd3; end
                    default: begin wr16(d_s1[2:0], {bus_q, tmp8}); finish(); end
                endcase
            end

            C_INCDEC8: begin
                case (step)
                    5'd1: begin
                        if (d_s1 == S_MEM) begin addr <= ea_; mem_read(ea_); step <= 5'd2; end
                        else begin lv <= rd8(d_s1[3:0]); step <= 5'd3; end
                    end
                    5'd2: begin lv <= bus_q; step <= 5'd3; end
                    5'd3: begin
                        logic [7:0] t, p;
                        t = d_aop[0] ? lv - 8'd1 : lv + 8'd1;
                        p = d_aop[0] ? zhc_sub(PSW, {24'h0, t}, {24'h0, lv}, 1'b0)
                                     : zhc_add(PSW, {24'h0, t}, {24'h0, lv}, 1'b0);
                        if (p[0]) p[5] = 1'b1;   // SKIP_CY
                        p[0] = PSW[0];           // carry restored
                        PSW <= p;
                        if (d_s1 == S_MEM) begin mem_write(addr, t); step <= 5'd4; end
                        else begin wr8(d_s1[3:0], t); finish(); end
                    end
                    default: finish();
                endcase
            end

            C_INX: begin wr16(d_s1[2:0], rd16(d_s1[2:0]) + 16'd1); finish(); end
            C_DCX: begin wr16(d_s1[2:0], rd16(d_s1[2:0]) - 16'd1); finish(); end

            C_SHIFT8: begin
                logic [7:0] v, n;
                logic c;
                v = rd8(d_s1[3:0]);
                case (d_aop[2:0])
                    3'd0, 3'd2: begin c = v[7]; n = {v[6:0], 1'b0}; end         // SLL, SLLC
                    3'd1, 3'd3: begin c = v[0]; n = {1'b0, v[7:1]}; end         // SLR, SLRC
                    3'd4:       begin c = v[7]; n = {v[6:0], PSW[0]}; end       // RLL
                    default:    begin c = v[0]; n = {PSW[0], v[7:1]}; end       // RLR
                endcase
                wr8(d_s1[3:0], n);
                PSW[0] <= c;
                if ((d_aop[2:0] == 3'd2 || d_aop[2:0] == 3'd3) && c) PSW[5] <= 1'b1;
                finish();
            end

            C_SHIFT16: begin
                logic [15:0] n;
                logic c;
                case (d_aop[1:0])
                    2'd0: begin c = EA[15]; n = {EA[14:0], 1'b0}; end       // DSLL
                    2'd1: begin c = EA[0];  n = {1'b0, EA[15:1]}; end       // DSLR
                    2'd2: begin c = EA[15]; n = {EA[14:0], PSW[0]}; end     // DRLL
                    default: begin c = EA[0]; n = {PSW[0], EA[15:1]}; end   // DRLR
                endcase
                {EAH, EAL} <= n;
                PSW[0] <= c;
                finish();
            end

            C_RLD, C_RRD: begin
                case (step)
                    5'd1: begin mem_read(HL); step <= 5'd2; end
                    5'd2: begin
                        if (d_cls == C_RLD) begin
                            A <= {A[7:4], bus_q[7:4]};
                            mem_write(HL, {bus_q[3:0], A[3:0]});
                        end else begin
                            A <= {A[7:4], bus_q[3:0]};
                            mem_write(HL, {A[3:0], bus_q[7:4]});
                        end
                        step <= 5'd3;
                    end
                    default: finish();
                endcase
            end

            C_DAA: begin
                logic [3:0] l, h;
                logic [7:0] adj, t, p;
                l = A[3:0]; h = A[7:4]; adj = 8'h00;
                if (!PSW[4]) begin
                    if (l < 10) begin
                        if (!(h < 10 && !PSW[0])) adj = 8'h60;
                    end else begin
                        if (h < 9 && !PSW[0]) adj = 8'h06;
                        else adj = 8'h66;
                    end
                end else if (l < 3) begin
                    if (h < 10 && !PSW[0]) adj = 8'h06;
                    else adj = 8'h66;
                end
                t = A + adj;
                p = zhc_add(PSW, {24'h0, t}, {24'h0, A}, PSW[0]);
                p[0] = p[0] | PSW[0];
                PSW <= p;
                A <= t;
                finish();
            end

            C_NEGA: begin A <= ~A + 8'd1; finish(); end

            C_MUL: begin {EAH, EAL} <= A * rd8(d_s2[3:0]); finish(); end

            C_DIV: begin
                // MAME: remainder = EA % r; EA /= r; r = remainder; r == 0: r = EAL, EA = FFFF
                case (step)
                    5'd1: begin
                        div_d <= rd8(d_s2[3:0]);
                        if (rd8(d_s2[3:0]) == 8'h0) begin
                            wr8(d_s2[3:0], EAL);
                            {EAH, EAL} <= 16'hFFFF;
                            finish();
                        end else begin
                            div_q <= EA; div_r <= 8'h0; div_n <= 5'd16;
                            step <= 5'd2;
                        end
                    end
                    5'd2: begin
                        // restoring division, one quotient bit per clock
                        logic [8:0] t;
                        t = {div_r, div_q[15]};
                        if (t >= {1'b0, div_d}) begin
                            div_r <= t[7:0] - div_d;
                            div_q <= {div_q[14:0], 1'b1};
                        end else begin
                            div_r <= t[7:0];
                            div_q <= {div_q[14:0], 1'b0};
                        end
                        div_n <= div_n - 5'd1;
                        if (div_n == 5'd1) step <= 5'd3;
                    end
                    default: begin
                        {EAH, EAL} <= div_q;
                        wr8(d_s2[3:0], div_r);
                        finish();
                    end
                endcase
            end

            C_BLOCK: begin
                case (step)
                    5'd1: begin mem_read(HL); step <= 5'd2; end
                    5'd2: begin mem_write(DE, bus_q); step <= 5'd3; end
                    default: begin
                        {D, E} <= DE + 16'd1;
                        {H, L} <= HL + 16'd1;
                        C <= C - 8'd1;
                        if (C == 8'h00) PSW[0] <= 1'b1;
                        else begin PSW[0] <= 1'b0; PC <= PC - 16'd1; end
                        finish();
                    end
                endcase
            end

            C_TABLE: begin
                case (step)
                    5'd1: begin addr <= PC + {8'h0, A} + 16'd1;
                                mem_read(PC + {8'h0, A} + 16'd1); step <= 5'd2; end
                    5'd2: begin C <= bus_q; mem_read(addr + 16'd1); step <= 5'd3; end
                    default: begin B <= bus_q; finish(); end
                endcase
            end

            C_EX: begin
                if (d_aop[0]) begin
                    {EAH, EAL} <= EA2; EA2 <= EA;
                    {V, A} <= VA2; VA2 <= VA;
                end
                if (d_aop[1]) begin
                    {B, C} <= BC2; BC2 <= BC;
                    {D, E} <= DE2; DE2 <= DE;
                end
                if (d_aop[1] || d_aop[2]) begin
                    {H, L} <= HL2; HL2 <= HL;
                end
                finish();
            end

            C_JMP: begin PC <= {arg1, arg0}; finish(); end
            C_JR:  begin PC <= PC + {{10{op[5]}}, op[5:0]}; finish(); end
            C_JRE: begin
                if (op[0]) PC <= PC - (16'd256 - {8'h0, arg0});
                else       PC <= PC + {8'h0, arg0};
                finish();
            end
            C_JB:  begin PC <= BC; finish(); end
            C_JEA: begin PC <= EA; finish(); end

            C_CALL, C_CALB, C_CALF, C_CALT, C_SOFTI: begin
                // SOFTI pushes PSW first; all push PCH then PCL
                case (step)
                    5'd1: begin
                        if (d_cls == C_SOFTI) begin
                            SP <= SP - 16'd1; mem_write(SP - 16'd1, PSW); step <= 5'd2;
                        end else step <= 5'd2;
                    end
                    5'd2: begin SP <= SP - 16'd1; mem_write(SP - 16'd1, PC[15:8]); step <= 5'd3; end
                    5'd3: begin SP <= SP - 16'd1; mem_write(SP - 16'd1, PC[7:0]); step <= 5'd4; end
                    5'd4: begin
                        case (d_cls)
                            C_CALL:  begin PC <= {arg1, arg0}; finish(); end
                            C_CALB:  begin PC <= BC; finish(); end
                            C_CALF:  begin PC <= {5'b00001, op[2:0], arg0}; finish(); end
                            C_SOFTI: begin PC <= 16'h0060; finish(); end
                            default: begin   // CALT
                                addr <= {8'h0, 2'b10, op[4:0], 1'b0};
                                mem_read({8'h0, 2'b10, op[4:0], 1'b0});
                                step <= 5'd5;
                            end
                        endcase
                    end
                    5'd5: begin tmp8 <= bus_q; mem_read(addr + 16'd1); step <= 5'd6; end
                    default: begin PC <= {bus_q, tmp8}; finish(); end
                endcase
            end

            C_RET, C_RETS, C_RETI: begin
                case (step)
                    5'd1: begin mem_read(SP); SP <= SP + 16'd1; step <= 5'd2; end
                    5'd2: begin tmp8 <= bus_q; mem_read(SP); SP <= SP + 16'd1; step <= 5'd3; end
                    5'd3: begin
                        PC <= {bus_q, tmp8};
                        if (d_cls == C_RETI) begin mem_read(SP); SP <= SP + 16'd1; step <= 5'd4; end
                        else begin
                            if (d_cls == C_RETS) PSW[5] <= 1'b1;
                            finish();
                        end
                    end
                    default: begin PSW <= bus_q; finish(); end
                endcase
            end

            C_BIT: begin
                case (step)
                    5'd1: begin mem_read(ea_); step <= 5'd2; end
                    default: begin if (bus_q[d_aop[2:0]]) PSW[5] <= 1'b1; finish(); end
                endcase
            end

            C_SK: begin
                logic f;
                case (d_aop[1:0])
                    2'd0: f = PSW[0];
                    2'd1: f = PSW[4];
                    2'd2: f = PSW[6];
                    default: f = 1'b0;   // NV
                endcase
                if (f ^ d_aop[2]) PSW[5] <= 1'b1;
                finish();
            end

            C_SKIT: begin
                case (step)
                    5'd1: begin skit_id <= d_aop; skit_req <= 1'b1; wait_bus <= 1'b1; step <= 5'd2; end
                    default: begin if (skit_q ^ d_s1[0]) PSW[5] <= 1'b1; finish(); end
                endcase
            end

            C_EI:  begin iff_pending <= 1'b1; finish(); end
            C_DI:  begin IFF <= 1'b0; iff_pending <= 1'b0; finish(); end
            C_HLT: begin halt <= 1'b1; finish(); end
            C_STOP: begin PC <= PC - 16'd2; finish(); end   // MAME also burns its timeslice
            C_CLC: begin PSW[0] <= 1'b0; finish(); end
            C_STC: begin PSW[0] <= 1'b1; finish(); end

            default: finish();
        endcase
    endtask

    // synthesis translate_off
    // MAME's upd7810_take_irq picks the vector once, before its three pushes; the core reads
    // irq_vec again after them. Count the interrupts whose vector changed meanwhile (cz101_bench
    // reports it; the board applies every interrupt source at instruction boundaries, so 0).
    logic [15:0] sim_vec0;
    int unsigned sim_vec_changes;
    always_ff @(posedge clk)
        if (reset) sim_vec_changes <= 0;
        else if (!wait_bus && st == ST_IRQ) begin
            if (step == 5'd0) sim_vec0 <= irq_vec;
            else if (step > 5'd2 && irq_vec != sim_vec0) sim_vec_changes <= sim_vec_changes + 1;
        end
    // synthesis translate_on

endmodule
