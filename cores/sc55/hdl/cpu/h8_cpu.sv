// H8/532 CPU (H8/500 instruction set), behavioural, matched to the reference
// model (jcmoyer/Nuked-SC55 backend: mcu_opcodes.cpp, mcu_interrupt.cpp,
// MCU_Step in mcu.cpp) access for access.
//
// One `step` pulse runs what MCU_Step does before `cycles += 12`: the
// interrupt check (unless ex_ignore), then one instruction (unless asleep),
// then the trace check. `step_done` pulses at the end. The caller advances
// the devices between steps.
//
// Bus: byte accesses only, in the model's order (16-bit = two bytes at
// address & ~1, high first; vectors = four bytes). bus_req is held with
// address/data until bus_ack (one cycle; read data valid with it).
//
// The model's quirks are kept on purpose (see docs/cpu.md): stack accesses
// in page 0, MOV:F size bit, EXTS flags from the unextended value, RTE
// restoring SR unmasked, ErrorTrap paths that continue with side effects.
// `traps` counts MCU_ErrorTrap calls so the bench can compare them too.

module h8_cpu (
    input  logic        clk,
    input  logic        rst,

    input  logic        step,
    output logic        step_done,

    output logic        bus_req,
    output logic        bus_we,
    output logic [23:0] bus_addr,
    output logic [7:0]  bus_wdata,
    input  logic [7:0]  bus_rdata,
    input  logic        bus_ack,

    // MCU_Interrupt_Source bits (0 NMI .. 20 UART TX) and the registers
    // MCU_Interrupt_GetVL reads
    input  logic [20:0] int_pending,
    input  logic [7:0]  ipra, iprb, iprc, iprd, p1cr,
    input  logic        int_hold,         // wait before the interrupt check (a pending IRQ0 not yet known)

    // reset vector load: hold `reset_start` with the step machinery idle
    input  logic        reset_start,

    // state load (bench, savestates): applied in S_IDLE; clears sleep,
    // ex_ignore, the pending exception and TRAPA requests
    input  logic        ld_en,
    input  logic [15:0] ld_r [8],
    input  logic [15:0] ld_pc, ld_sr,
    input  logic [7:0]  ld_cp, ld_dp, ld_ep, ld_tp, ld_br,

    // state, for the lockstep bench and the status line
    output logic [15:0] r_out [8],
    output logic [15:0] pc_out,
    output logic [15:0] sr_out,
    output logic [7:0]  cp_out, dp_out, ep_out, tp_out, br_out,
    output logic        sleep_out,
    output logic        ex_ignore_out,
    output logic [2:0]  exc_out,          // {valid, type[1:0]}
    output logic [15:0] trapa_out,
    output logic [31:0] traps
);

    // ------------------------------------------------------------------
    // Architectural state
    // ------------------------------------------------------------------
    logic [15:0] r [8];
    logic [15:0] pc, sr;
    logic [7:0]  cp, dp, ep, tp, br;
    logic        sleep_f, ex_ignore;
    logic        exc_v;
    logic [1:0]  exc_t;               // 0 address error, 1 invalid instruction, 2 trace
    logic [15:0] trapa_p;

    localparam logic [15:0] SR_MASK = 16'h870f;
    localparam int ST_T = 15, ST_N = 3, ST_Z = 2, ST_V = 1, ST_C = 0;

    always_comb begin
        for (int i = 0; i < 8; i++) r_out[i] = r[i];
        pc_out = pc; sr_out = sr; cp_out = cp; dp_out = dp; ep_out = ep;
        tp_out = tp; br_out = br; sleep_out = sleep_f; ex_ignore_out = ex_ignore;
        exc_out = {exc_v, exc_t}; trapa_out = trapa_p;
    end

    // ------------------------------------------------------------------
    // Helpers
    // ------------------------------------------------------------------
    // {N, Z, V, C, result}
    function automatic logic [19:0] alu_add(input logic [15:0] a, input logic [15:0] b,
                                             input logic cin, input logic sub, input logic word);
        logic [16:0] s;
        logic [15:0] res;
        logic n, z, v, c, a7, b7, r7;
        if (word) begin
            s = sub ? ({1'b0, a} - {1'b0, b} - {16'b0, cin}) : ({1'b0, a} + {1'b0, b} + {16'b0, cin});
            res = s[15:0];
            c = s[16];
            a7 = a[15]; b7 = b[15]; r7 = res[15];
            z = (res == 16'h0);
        end else begin
            logic [8:0] s8;
            s8 = sub ? ({1'b0, a[7:0]} - {1'b0, b[7:0]} - {8'b0, cin}) : ({1'b0, a[7:0]} + {1'b0, b[7:0]} + {8'b0, cin});
            res = {8'h00, s8[7:0]};
            c = s8[8];
            a7 = a[7]; b7 = b[7]; r7 = s8[7];
            z = (s8[7:0] == 8'h0);
        end
        n = r7;
        v = sub ? ((a7 & ~b7 & ~r7) | (~a7 & b7 & r7)) : ((a7 & b7 & ~r7) | (~a7 & ~b7 & r7));
        return {n, z, v, c, res};
    endfunction

    // MCU_SetStatusCommon: N, Z from the sized value, V = 0, C kept
    function automatic logic [15:0] st_common(input logic [15:0] s, input logic [31:0] val, input logic word);
        logic [15:0] o;
        o = s;
        o[ST_N] = word ? val[15] : val[7];
        o[ST_Z] = word ? (val[15:0] == 16'h0) : (val[7:0] == 8'h0);
        o[ST_V] = 1'b0;
        return o;
    endfunction

    function automatic logic [15:0] st_nzvc(input logic [15:0] s, input logic [19:0] f);
        logic [15:0] o;
        o = s;
        o[ST_N] = f[19]; o[ST_Z] = f[18]; o[ST_V] = f[17]; o[ST_C] = f[16];
        return o;
    endfunction

    function automatic logic [7:0] page_for(input logic [2:0] rg, input logic [7:0] d, input logic [7:0] e, input logic [7:0] t);
        if (rg >= 3'd6) return t;
        else if (rg >= 3'd4) return e;
        return d;
    endfunction

    // ------------------------------------------------------------------
    // Control register access (MCU_ControlRegisterRead/Write)
    // ------------------------------------------------------------------
    function automatic logic [16:0] ctrl_read(input logic [2:0] cr, input logic word);
        // {trap, value}
        if (word) begin
            case (cr)
                3'd0: return {1'b0, sr & SR_MASK};
                3'd5: return {1'b0, dp, dp};
                3'd4: return {1'b0, ep, ep};
                3'd3: return {1'b0, br, br};
                default: return {1'b1, 16'h0};
            endcase
        end else begin
            case (cr)
                3'd1: return {1'b0, 8'h00, sr[7:0] & SR_MASK[7:0]};
                3'd3: return {1'b0, 8'h00, br};
                3'd4: return {1'b0, 8'h00, ep};
                3'd5: return {1'b0, 8'h00, dp};
                3'd7: return {1'b0, 8'h00, tp};
                default: return {1'b1, 16'h0};
            endcase
        end
    endfunction

    // ------------------------------------------------------------------
    // Sequencer
    // ------------------------------------------------------------------
    typedef enum logic [6:0] {
        S_IDLE, S_RESET,
        S_INT, S_IVEC, S_IPUSH1, S_IPUSH2, S_IPUSH3, S_IFIN,
        S_EXEC, S_OP1,
        S_FETCH, S_MEM,
        // non-general
        S_SCB1, S_SCB2, S_JMP11, S_JMP11_POPCP, S_JMP11_POPPC, S_JMP11_PJSR1, S_JMP11_PJSR2, S_JMP11_JSR,
        S_JMP16, S_PJ1, S_PJSR_PUSHPC, S_PJSR_PUSHCP,
        S_LDM1, S_LDM_POP, S_LDM_GOT, S_STM1, S_STM_PUSH, S_TRAPA1,
        S_RTE_SR, S_RTE_CP, S_RTE_PC, S_BSR1, S_BSR_PUSH,
        S_RTD1, S_RTD_POP, S_JSR1, S_JSR_PUSH, S_RTS_POP, S_BCC1,
        S_SHORT1, S_SHORT_MEM,
        // general
        S_G_MODE, S_G_EA, S_G_OPC, S_G_OPC2, S_G_DISPATCH,
        S_G_PREIMM, S_G_READ, S_G_READ2, S_G_POSTIMM, S_G_ALU, S_G_DIV, S_G_WRITE,
        S_END
    } state_t;

    state_t state, ret;

    // fetch engine
    logic [1:0]  f_n;           // bytes left
    logic [23:0] f_data;        // accumulated, big-endian
    // memory engine
    logic [23:0] m_addr;
    logic [2:0]  m_n;           // bytes left
    logic        m_we;
    logic [31:0] m_data;        // read: accumulated; write: bytes from [15:8] then [7:0] (len 2) or [7:0] (len 1)

    // instruction registers
    logic [7:0]  op1;
    logic [7:0]  opc_b;         // opcode byte (general) after extension
    logic        opc_ext;
    logic [7:0]  aux;           // second byte of non-general instructions
    logic [3:0]  idx;           // LDM/STM register index
    logic        int_clear_exc;
    logic [7:0]  int_vector;
    logic        int_has_mask;
    logic [2:0]  int_mask;
    logic [7:0]  new_cp;
    logic [15:0] new_pc;

    // general-format operand
    typedef enum logic [1:0] { T_DIRECT, T_INDIRECT, T_ABSOLUTE, T_IMMEDIATE } otype_t;
    otype_t      g_type;
    logic        g_word;
    logic [2:0]  g_reg;
    logic [15:0] g_ea;
    logic [7:0]  g_ep;
    logic [15:0] g_imm;         // operand_data (immediate)
    logic [15:0] g_disp;
    logic [1:0]  g_inc;         // 0 none, 1 predec, 2 postinc
    logic [15:0] g_opd;         // operand value read
    logic [15:0] g_res;         // value to write back
    logic [15:0] g_xi;          // extra immediate (MOVG_Immediate)
    logic        g_do_read, g_do_write;
    logic [1:0]  g_pre_n, g_post_n;
    // divider
    logic [5:0]  d_cnt;
    logic [31:0] d_q, d_rem;
    logic [15:0] d_div;

    wire  [4:0]  opc  = opc_b[7:3];
    wire  [2:0]  oreg = opc_b[2:0];
    wire         g_mem = (g_type == T_INDIRECT) || (g_type == T_ABSOLUTE);

    // interrupt source selection (MCU_Interrupt_Handle / GetVL)
    logic        irq_take;
    logic [7:0]  irq_vec;
    logic [2:0]  irq_lvl;
    always_comb begin
        irq_take = 1'b0; irq_vec = 8'd0; irq_lvl = 3'd0;
        for (int s = 20; s >= 1; s--) begin
            logic [7:0] v; logic [2:0] l; logic ok;
            ok = 1'b1; v = 8'd0; l = 3'd0;
            case (s)
                1:  begin ok = p1cr[5]; v = 8'd32; l = ipra[6:4]; end
                2:  begin ok = p1cr[6]; v = 8'd33; l = ipra[2:0]; end
                4:  begin v = 8'd37; l = iprb[6:4]; end
                5:  begin v = 8'd38; l = iprb[6:4]; end
                6:  begin v = 8'd39; l = iprb[6:4]; end
                8:  begin v = 8'd41; l = iprb[2:0]; end
                9:  begin v = 8'd42; l = iprb[2:0]; end
                10: begin v = 8'd43; l = iprb[2:0]; end
                12: begin v = 8'd45; l = iprc[6:4]; end
                13: begin v = 8'd46; l = iprc[6:4]; end
                14: begin v = 8'd47; l = iprc[6:4]; end
                15: begin v = 8'd48; l = iprc[2:0]; end
                16: begin v = 8'd49; l = iprc[2:0]; end
                17: begin v = 8'd50; l = iprc[2:0]; end
                18: begin v = 8'd56; l = iprd[2:0]; end
                19: begin v = 8'd53; l = iprd[6:4]; end
                20: begin v = 8'd54; l = iprd[6:4]; end
                default: ok = 1'b0;      // FRT ICI: no vector
            endcase
            // lowest pending source whose level beats the mask wins
            if (int_pending[s] && ok && (l > sr[10:8])) begin
                irq_take = 1'b1; irq_vec = v; irq_lvl = l;
            end
        end
    end

    logic [3:0] trapa_low;
    always_comb begin
        trapa_low = 4'd0;
        for (int i = 15; i >= 0; i--) if (trapa_p[i]) trapa_low = 4'(i);
    end

    // ------------------------------------------------------------------
    // Engine entry macros
    // ------------------------------------------------------------------
    `define FETCH(N, RET) begin f_n <= 2'(N); f_data <= 24'h0; ret <= RET; state <= S_FETCH; end
    `define READ(A, N, RET) begin m_addr <= (A); m_n <= 3'(N); m_we <= 1'b0; m_data <= 32'h0; ret <= RET; state <= S_MEM; end
    `define WRITE(A, N, D, RET) begin m_addr <= (A); m_n <= 3'(N); m_we <= 1'b1; m_data <= {16'h0, 16'(D)}; ret <= RET; state <= S_MEM; end
    `define PUSH(D, RET) begin if (r[7][0]) begin exc_v <= 1'b1; exc_t <= 2'd0; end r[7] <= r[7] - 16'd2; `WRITE({8'h00, (r[7] - 16'd2) & 16'hfffe}, 2, D, RET) end
    `define POP(RET) begin if (r[7][0]) begin exc_v <= 1'b1; exc_t <= 2'd0; end r[7] <= r[7] + 16'd2; `READ({8'h00, r[7] & 16'hfffe}, 2, RET) end
    `define TRAP ntr = ntr + 32'd1

    logic [15:0] t16;
    logic [19:0] f;
    logic [31:0] p32;
    logic [15:0] srn;
    logic [3:0]  bitn;
    logic        cond;
    logic [16:0] cr;
    logic [31:0] ntr;
    logic        ran;

    always_ff @(posedge clk) begin
        step_done <= 1'b0;
        ntr = 32'd0;
        if (rst) begin
            state <= S_IDLE;
            bus_req <= 1'b0;
            for (int i = 0; i < 8; i++) r[i] <= 16'h0;
            pc <= 16'h0; sr <= 16'h0700; cp <= 8'h0; dp <= 8'h0; ep <= 8'h0; tp <= 8'h0; br <= 8'h0;
            sleep_f <= 1'b0; ex_ignore <= 1'b0; exc_v <= 1'b0; exc_t <= 2'd0; trapa_p <= 16'h0;
            traps <= 32'h0;
        end else begin
            case (state)
            // ---------------------------------------------------------- idle
            S_IDLE: begin
                if (ld_en) begin
                    for (int i = 0; i < 8; i++) r[i] <= ld_r[i];
                    pc <= ld_pc; sr <= ld_sr; cp <= ld_cp; dp <= ld_dp; ep <= ld_ep; tp <= ld_tp; br <= ld_br;
                    sleep_f <= 1'b0; ex_ignore <= 1'b0; exc_v <= 1'b0; trapa_p <= 16'h0;
                end else if (reset_start) begin
                    // MCU_Reset: registers cleared, then the reset vector
                    for (int i = 0; i < 8; i++) r[i] <= 16'h0;
                    sr <= 16'h0700; cp <= 8'h0; dp <= 8'h0; ep <= 8'h0; tp <= 8'h0; br <= 8'h0;
                    exc_v <= 1'b0; sleep_f <= 1'b0; ex_ignore <= 1'b0; trapa_p <= 16'h0;
                    `READ(24'h000000, 4, S_RESET)
                end else if (step) begin
                    state <= S_INT;
                end
            end
            S_RESET: begin
                cp <= m_data[23:16]; pc <= m_data[15:0];
                step_done <= 1'b1;
                state <= S_IDLE;
            end

            // ---------------------------------------------------- interrupts
            S_INT: begin
                int_clear_exc <= 1'b0;
                if (int_hold && !ex_ignore) begin
                    // the check needs the PCM's IRQ0 for the frames due by now
                end else if (ex_ignore) begin
                    ex_ignore <= 1'b0;
                    state <= S_EXEC;
                end else if (trapa_p != 16'h0) begin
                    trapa_p[trapa_low] <= 1'b0;
                    int_vector <= 8'd16 + 8'(trapa_low); int_has_mask <= 1'b0;
                    `READ({14'h0, 8'd16 + 8'(trapa_low), 2'b00}, 4, S_IVEC)
                end else if (exc_v) begin
                    int_clear_exc <= 1'b1; int_has_mask <= 1'b0;
                    case (exc_t)
                        2'd0: begin int_vector <= 8'd8; `READ({14'h0, 8'd8, 2'b00}, 4, S_IVEC) end
                        2'd1: begin int_vector <= 8'd2; `READ({14'h0, 8'd2, 2'b00}, 4, S_IVEC) end
                        default: begin int_vector <= 8'd9; `READ({14'h0, 8'd9, 2'b00}, 4, S_IVEC) end
                    endcase
                end else if (int_pending[0]) begin
                    int_has_mask <= 1'b1; int_mask <= 3'd7;
                    `READ({14'h0, 8'd11, 2'b00}, 4, S_IVEC)
                end else if (irq_take) begin
                    int_has_mask <= 1'b1; int_mask <= irq_lvl;
                    `READ({14'h0, irq_vec, 2'b00}, 4, S_IVEC)
                end else begin
                    // S_EXEC's work, one clock earlier
                    g_do_write <= 1'b0;
                    ran <= !sleep_f;
                    if (sleep_f) state <= S_END;
                    else `FETCH(1, S_OP1)
                end
            end
            S_IVEC: begin
                new_cp <= m_data[23:16]; new_pc <= m_data[15:0];
                `PUSH(pc, S_IPUSH1)
            end
            S_IPUSH1: `PUSH({8'h00, cp}, S_IPUSH2)
            S_IPUSH2: `PUSH(sr, S_IPUSH3)
            S_IPUSH3: begin
                srn = sr;
                srn[ST_T] = 1'b0;
                if (int_has_mask) srn[10:8] = int_mask;
                sr <= srn;
                sleep_f <= 1'b0;
                cp <= new_cp; pc <= new_pc;
                if (int_clear_exc) exc_v <= 1'b0;
                state <= S_IFIN;
            end
            S_IFIN: state <= S_EXEC;

            // ------------------------------------------------------- execute
            S_EXEC: begin
                g_do_write <= 1'b0;
                ran <= !sleep_f;
                if (sleep_f) state <= S_END;
                else `FETCH(1, S_OP1)
            end

            S_OP1: begin
                op1 <= f_data[7:0];
                casez (f_data[7:0])
                    8'h00: state <= S_END;                                  // NOP
                    8'h01, 8'h06, 8'h07: `FETCH(1, S_SCB1)
                    8'h02: `FETCH(1, S_LDM1)
                    8'h03: `FETCH(3, S_PJ1)                                 // PJSR
                    8'h13: `FETCH(3, S_PJ1)                                 // PJMP
                    8'h08: `FETCH(1, S_TRAPA1)
                    8'h0a: `POP(S_RTE_SR)
                    8'h0e: `FETCH(1, S_BSR1)
                    8'h1e: `FETCH(2, S_BSR1)
                    8'h10: `FETCH(2, S_JMP16)
                    8'h11: `FETCH(1, S_JMP11)
                    8'h12: `FETCH(1, S_STM1)
                    8'h14, 8'h1c: `FETCH(1, S_RTD1)
                    8'h18: `FETCH(2, S_JSR1)
                    8'h19: `POP(S_RTS_POP)
                    8'h1a: begin sleep_f <= 1'b1; state <= S_END; end
                    8'h09, 8'h0b, 8'h0f, 8'h16, 8'h17, 8'h1b, 8'h1f: begin `TRAP; state <= S_END; end
                    8'b001?????: begin                                      // Bcc
                        if (f_data[4]) `FETCH(2, S_BCC1) else `FETCH(1, S_BCC1)
                    end
                    8'b0100????: begin                                      // CMP:E / CMP:I
                        if (f_data[3]) `FETCH(2, S_SHORT1) else `FETCH(1, S_SHORT1)
                    end
                    8'b01010???: `FETCH(1, S_SHORT1)                        // MOV:E
                    8'b01011???: `FETCH(2, S_SHORT1)                        // MOV:I
                    8'b011?????, 8'b100?????: `FETCH(1, S_SHORT1)           // MOV:L, MOV:S, MOV:F
                    default: state <= S_G_MODE;                             // general format
                endcase
            end

            // ------------------------------------------------- fetch engine
            S_FETCH: begin
                if (!bus_req) begin
                    bus_req <= 1'b1; bus_we <= 1'b0; bus_addr <= {cp, pc};
                    pc <= pc + 16'd1;
                end else if (bus_ack) begin
                    f_data <= {f_data[15:0], bus_rdata};
                    if (f_n == 2'd1) begin
                        bus_req <= 1'b0;
                        state <= ret;
                    end else begin
                        // next byte back to back: keep the request, new address
                        bus_addr <= {cp, pc};
                        pc <= pc + 16'd1;
                    end
                    f_n <= f_n - 2'd1;
                end
            end

            // ------------------------------------------------ memory engine
            S_MEM: begin
                if (!bus_req) begin
                    bus_req <= 1'b1; bus_we <= m_we; bus_addr <= m_addr;
                    bus_wdata <= (m_n == 3'd2) ? m_data[15:8] : m_data[7:0];
                end else if (bus_ack) begin
                    if (!m_we) m_data <= {m_data[23:0], bus_rdata};
                    m_addr <= m_addr + 24'd1;
                    if (m_n == 3'd1) begin
                        bus_req <= 1'b0;
                        state <= ret;
                    end else begin
                        bus_addr <= m_addr + 24'd1;
                        bus_wdata <= m_data[7:0];          // second byte of a 16-bit write
                    end
                    m_n <= m_n - 3'd1;
                end
            end

            // --------------------------------------------- SCB/F, /NE, /EQ
            S_SCB1: begin
                aux <= f_data[7:0];
                if (f_data[7:3] == 5'h17) `FETCH(1, S_SCB2)
                else begin `TRAP; state <= S_END; end
            end
            S_SCB2: begin
                cond = (op1 == 8'h01) || (op1 == 8'h06 && sr[ST_Z]) || (op1 == 8'h07 && !sr[ST_Z]);
                if (cond) begin
                    t16 = r[aux[2:0]] - 16'd1;
                    r[aux[2:0]] <= t16;
                    if (t16 != 16'hffff) pc <= pc + {{8{f_data[7]}}, f_data[7:0]};
                end
                state <= S_END;
            end

            // ------------------------------------------------- JMP group 11
            S_JMP11: begin
                aux <= f_data[7:0];
                if (f_data[7:0] == 8'h19) `POP(S_JMP11_POPCP)
                else if (f_data[7:3] == 5'h19) `PUSH(pc, S_JMP11_PJSR1)
                else if (f_data[7:3] == 5'h1a) begin pc <= r[f_data[2:0]]; state <= S_END; end
                else if (f_data[7:3] == 5'h1b) `PUSH(pc, S_JMP11_JSR)
                else begin `TRAP; state <= S_END; end
            end
            S_JMP11_POPCP: begin cp <= m_data[7:0]; `POP(S_JMP11_POPPC) end
            S_JMP11_POPPC: begin pc <= m_data[15:0]; state <= S_END; end
            S_JMP11_PJSR1: `PUSH({8'h00, cp}, S_JMP11_PJSR2)   // PJSR @Rn
            S_JMP11_PJSR2: begin
                cp <= r[{aux[2:1], 1'b0}][7:0];
                pc <= r[{aux[2:1], 1'b1}];
                state <= S_END;
            end
            S_JMP11_JSR: begin pc <= r[aux[2:0]]; state <= S_END; end  // JSR @Rn
            S_JMP16: begin pc <= f_data[15:0]; state <= S_END; end

            // ---------------------------------------------------- PJSR/PJMP
            S_PJ1: begin
                new_cp <= f_data[23:16]; new_pc <= f_data[15:0];
                if (op1 == 8'h03) `PUSH(pc, S_PJSR_PUSHPC)
                else begin cp <= f_data[23:16]; pc <= f_data[15:0]; state <= S_END; end
            end
            S_PJSR_PUSHPC: `PUSH({8'h00, cp}, S_PJSR_PUSHCP)
            S_PJSR_PUSHCP: begin cp <= new_cp; pc <= new_pc; state <= S_END; end

            // ---------------------------------------------------------- LDM
            S_LDM1: begin aux <= f_data[7:0]; idx <= 4'd0; state <= S_LDM_POP; end
            S_LDM_POP: begin
                if (idx == 4'd8) state <= S_END;
                else if (aux[idx[2:0]]) `POP(S_LDM_GOT)
                else idx <= idx + 4'd1;
            end
            S_LDM_GOT: begin
                if (idx != 4'd7) r[idx[2:0]] <= m_data[15:0];
                idx <= idx + 4'd1;
                state <= S_LDM_POP;
            end
            // ---------------------------------------------------------- STM
            S_STM1: begin aux <= f_data[7:0]; idx <= 4'd7; state <= S_STM_PUSH; end
            S_STM_PUSH: begin
                if (aux[idx[2:0]]) begin
                    t16 = r[idx[2:0]];
                    if (idx[2:0] == 3'd7) t16 = t16 - 16'd2;
                    aux[idx[2:0]] <= 1'b0;
                    `PUSH(t16, S_STM_PUSH)
                end else if (idx == 4'd0) state <= S_END;
                else idx <= idx - 4'd1;
            end

            // -------------------------------------------------------- TRAPA
            S_TRAPA1: begin
                if (f_data[7:4] == 4'h1) trapa_p[f_data[3:0]] <= 1'b1;
                else `TRAP;
                state <= S_END;
            end

            // ---------------------------------------------------------- RTE
            S_RTE_SR: begin sr <= m_data[15:0]; `POP(S_RTE_CP) end
            S_RTE_CP: begin cp <= m_data[7:0]; `POP(S_RTE_PC) end
            S_RTE_PC: begin pc <= m_data[15:0]; ex_ignore <= 1'b1; state <= S_END; end

            // ---------------------------------------------------------- BSR
            S_BSR1: begin
                new_pc <= (op1 == 8'h0e) ? {{8{f_data[7]}}, f_data[7:0]} : f_data[15:0];
                `PUSH(pc, S_BSR_PUSH)
            end
            S_BSR_PUSH: begin pc <= pc + new_pc; state <= S_END; end

            // ---------------------------------------------------------- RTD
            S_RTD1: begin aux <= f_data[7:0]; `POP(S_RTD_POP) end
            S_RTD_POP: begin
                pc <= m_data[15:0];
                if (op1 == 8'h14) begin
                    t16 = r[7] + {{8{aux[7]}}, aux};
                    r[7] <= t16;
                    if (t16[0]) `TRAP;
                end else `TRAP;
                state <= S_END;
            end

            // ---------------------------------------------------------- JSR
            S_JSR1: begin new_pc <= f_data[15:0]; `PUSH(pc, S_JSR_PUSH) end
            S_JSR_PUSH: begin pc <= new_pc; state <= S_END; end
            S_RTS_POP: begin pc <= m_data[15:0]; state <= S_END; end

            // ---------------------------------------------------------- Bcc
            S_BCC1: begin
                t16 = op1[4] ? f_data[15:0] : {{8{f_data[7]}}, f_data[7:0]};
                case (op1[3:0])
                    4'h0: cond = 1'b1;
                    4'h1: cond = 1'b0;
                    4'h2: cond = !(sr[ST_C] | sr[ST_Z]);
                    4'h3: cond = (sr[ST_C] | sr[ST_Z]);
                    4'h4: cond = !sr[ST_C];
                    4'h5: cond = sr[ST_C];
                    4'h6: cond = !sr[ST_Z];
                    4'h7: cond = sr[ST_Z];
                    4'h8: cond = !sr[ST_V];
                    4'h9: cond = sr[ST_V];
                    4'ha: cond = !sr[ST_N];
                    4'hb: cond = sr[ST_N];
                    4'hc: cond = !(sr[ST_N] ^ sr[ST_V]);
                    4'hd: cond = (sr[ST_N] ^ sr[ST_V]);
                    4'he: cond = !(sr[ST_Z] | (sr[ST_N] ^ sr[ST_V]));
                    default: cond = (sr[ST_Z] | (sr[ST_N] ^ sr[ST_V]));
                endcase
                if (cond) pc <= pc + t16;
                state <= S_END;
            end

            // ------------------------------------------- short formats 40-9F
            S_SHORT1: begin
                casez (op1)
                    8'b0100????: begin                        // CMP:E / CMP:I
                        f = alu_add(r[op1[2:0]], op1[3] ? f_data[15:0] : {8'h0, f_data[7:0]}, 1'b0, 1'b1, op1[3]);
                        sr <= st_nzvc(sr, f);
                        state <= S_END;
                    end
                    8'b01010???: begin                        // MOV:E
                        r[op1[2:0]][7:0] <= f_data[7:0];
                        sr <= st_common(sr, {24'h0, f_data[7:0]}, 1'b0);
                        state <= S_END;
                    end
                    8'b01011???: begin                        // MOV:I
                        r[op1[2:0]] <= f_data[15:0];
                        sr <= st_common(sr, {16'h0, f_data[15:0]}, 1'b1);
                        state <= S_END;
                    end
                    8'b0110????: begin                        // MOV:L @aa:8 (page 0)
                        if (op1[3]) begin
                            if (f_data[0]) begin exc_v <= 1'b1; exc_t <= 2'd0; end
                            `READ({8'h00, br, f_data[7:1], 1'b0}, 2, S_SHORT_MEM)
                        end else `READ({8'h00, br, f_data[7:0]}, 1, S_SHORT_MEM)
                    end
                    8'b0111????: begin                        // MOV:S @aa:8 (page 0)
                        if (op1[3]) begin
                            if (f_data[0]) begin exc_v <= 1'b1; exc_t <= 2'd0; end
                            sr <= st_common(sr, {16'h0, r[op1[2:0]]}, 1'b1);
                            `WRITE({8'h00, br, f_data[7:1], 1'b0}, 2, r[op1[2:0]], S_END)
                        end else begin
                            sr <= st_common(sr, {24'h0, r[op1[2:0]][7:0]}, 1'b0);
                            `WRITE({8'h00, br, f_data[7:0]}, 1, {8'h0, r[op1[2:0]][7:0]}, S_END)
                        end
                    end
                    default: begin                            // MOV:F @(d:8,R6) (page tp)
                        t16 = r[6] + {{8{f_data[7]}}, f_data[7:0]};
                        if (!op1[4]) begin
                            if (op1[3]) `READ({tp, t16 & 16'hfffe}, 2, S_SHORT_MEM)
                            else `READ({tp, t16}, 1, S_SHORT_MEM)
                        end else begin
                            if (op1[3]) begin
                                sr <= st_common(sr, {24'h0, r[op1[2:0]][7:0]}, 1'b0);
                                `WRITE({tp, t16}, 1, {8'h0, r[op1[2:0]][7:0]}, S_END)
                            end else begin
                                sr <= st_common(sr, {16'h0, r[op1[2:0]]}, 1'b1);
                                `WRITE({tp, t16 & 16'hfffe}, 2, r[op1[2:0]], S_END)
                            end
                        end
                    end
                endcase
            end
            S_SHORT_MEM: begin
                if (op1[7:4] == 4'h6) begin                   // MOV:L result
                    if (op1[3]) begin
                        r[op1[2:0]] <= m_data[15:0];
                        sr <= st_common(sr, {16'h0, m_data[15:0]}, 1'b1);
                    end else begin
                        r[op1[2:0]][7:0] <= m_data[7:0];
                        sr <= st_common(sr, {24'h0, m_data[7:0]}, 1'b0);
                    end
                end else begin                                // MOV:F load
                    if (op1[3]) begin                         // (reference: Read16, OR into the low byte's register)
                        r[op1[2:0]] <= (r[op1[2:0]] & 16'hff00) | m_data[15:0];
                        sr <= st_common(sr, {16'h0, m_data[15:0]}, 1'b0);
                    end else begin
                        r[op1[2:0]] <= {8'h00, m_data[7:0]};
                        sr <= st_common(sr, {24'h0, m_data[7:0]}, 1'b1);
                    end
                end
                state <= S_END;
            end

            // ================================================ general format
            S_G_MODE: begin
                g_word <= op1[3]; g_reg <= op1[2:0];
                g_inc <= 2'd0; g_disp <= 16'h0; g_imm <= 16'h0;
                case (op1[7:4])
                    4'ha: begin g_type <= T_DIRECT; state <= S_G_EA; end
                    4'hd: begin g_type <= T_INDIRECT; state <= S_G_EA; end
                    4'he: begin g_type <= T_INDIRECT; `FETCH(1, S_G_EA) end
                    4'hf: begin g_type <= T_INDIRECT; `FETCH(2, S_G_EA) end
                    4'hb: begin g_type <= T_INDIRECT; g_inc <= 2'd1; state <= S_G_EA; end
                    4'hc: begin g_type <= T_INDIRECT; g_inc <= 2'd2; state <= S_G_EA; end
                    4'h0: begin
                        if (op1[2:0] == 3'd5) begin g_type <= T_ABSOLUTE; `FETCH(1, S_G_EA) end
                        else if (op1[2:0] == 3'd4) begin g_type <= T_IMMEDIATE; if (op1[3]) `FETCH(2, S_G_EA) else `FETCH(1, S_G_EA) end
                        else begin g_type <= T_DIRECT; state <= S_G_EA; end
                    end
                    4'h1: begin
                        if (op1[2:0] == 3'd5) begin g_type <= T_ABSOLUTE; `FETCH(2, S_G_EA) end
                        else begin g_type <= T_DIRECT; state <= S_G_EA; end
                    end
                    default: begin g_type <= T_DIRECT; state <= S_G_EA; end
                endcase
            end
            S_G_EA: begin
                case (op1[7:4])
                    4'he: begin
                        t16 = r[g_reg] + {{8{f_data[7]}}, f_data[7:0]};
                        g_ea <= t16; g_ep <= page_for(g_reg, dp, ep, tp);
                    end
                    4'hf: begin
                        t16 = r[g_reg] + f_data[15:0];
                        g_ea <= t16; g_ep <= page_for(g_reg, dp, ep, tp);
                    end
                    4'hd: begin g_ea <= r[g_reg]; g_ep <= page_for(g_reg, dp, ep, tp); end
                    4'hb: begin
                        t16 = r[g_reg] - ((g_word || g_reg == 3'd7) ? 16'd2 : 16'd1);
                        r[g_reg] <= t16;
                        g_ea <= t16; g_ep <= page_for(g_reg, dp, ep, tp);
                    end
                    4'hc: begin
                        r[g_reg] <= r[g_reg] + ((g_word || g_reg == 3'd7) ? 16'd2 : 16'd1);
                        g_ea <= r[g_reg]; g_ep <= page_for(g_reg, dp, ep, tp);
                    end
                    4'h0: begin
                        if (g_type == T_ABSOLUTE) begin g_ea <= {br, f_data[7:0]}; g_ep <= 8'h00; end
                        else if (g_type == T_IMMEDIATE) begin
                            g_imm <= g_word ? f_data[15:0] : {8'h0, f_data[7:0]};
                            g_ea <= 16'h0; g_ep <= 8'h0;
                        end else begin g_ea <= 16'h0; g_ep <= 8'h0; end
                    end
                    4'h1: begin
                        if (g_type == T_ABSOLUTE) begin g_ea <= f_data[15:0]; g_ep <= dp; end
                        else begin g_ea <= 16'h0; g_ep <= 8'h0; end
                    end
                    default: begin g_ea <= 16'h0; g_ep <= 8'h0; end
                endcase
                `FETCH(1, S_G_OPC)
            end
            S_G_OPC: begin
                if (f_data[7:0] == 8'h00) begin opc_ext <= 1'b1; `FETCH(1, S_G_OPC2) end
                else begin opc_ext <= 1'b0; opc_b <= f_data[7:0]; state <= S_G_DISPATCH; end
            end
            S_G_OPC2: begin opc_b <= f_data[7:0]; state <= S_G_DISPATCH; end

            S_G_DISPATCH: begin
                // decide the access pattern of MCU_Opcode_Table[opc](opc, oreg)
                g_do_read <= 1'b0; g_do_write <= 1'b0; g_pre_n <= 2'd0; g_post_n <= 2'd0;
                state <= S_G_ALU;
                case (opc)
                    5'h00: begin                                  // MOVG_Immediate
                        if (oreg == 3'd6 && g_mem) begin g_pre_n <= 2'd1; state <= S_G_PREIMM; end
                        else if (oreg == 3'd7 && g_mem) begin g_pre_n <= 2'd2; state <= S_G_PREIMM; end
                        else if (oreg == 3'd4 && g_mem) begin g_do_read <= 1'b1; g_post_n <= 2'd1; state <= S_G_READ; end
                        else if (oreg == 3'd5 && g_mem) begin g_do_read <= 1'b1; g_post_n <= 2'd2; state <= S_G_READ; end
                        else begin `TRAP; state <= S_END; end
                    end
                    5'h02: begin                                  // CLR group
                        if (oreg == 3'd3 && g_type != T_IMMEDIATE) state <= S_G_ALU;
                        else if ((oreg == 3'd6 || oreg == 3'd5 || oreg == 3'd4) && g_type != T_IMMEDIATE) begin
                            g_do_read <= 1'b1; state <= S_G_READ;
                        end else if ((oreg == 3'd2 || oreg == 3'd0 || oreg == 3'd1) && g_type == T_DIRECT && !g_word)
                            state <= S_G_ALU;
                        else begin `TRAP; state <= S_END; end
                    end
                    5'h03: begin                                  // shifts
                        if (oreg != 3'd7 && g_type != T_IMMEDIATE) begin g_do_read <= 1'b1; state <= S_G_READ; end
                        else begin `TRAP; state <= S_END; end
                    end
                    5'h09, 5'h0b: begin                           // BSET/ORC, BCLR/ANDC
                        g_do_read <= 1'b1; state <= S_G_READ;
                    end
                    5'h0d: begin `TRAP; state <= S_END; end
                    5'h0f, 5'h18, 5'h19, 5'h1a, 5'h1b, 5'h1c, 5'h1d, 5'h1e, 5'h1f: begin
                        if (g_type != T_IMMEDIATE) begin g_do_read <= 1'b1; state <= S_G_READ; end
                        else begin `TRAP; state <= S_END; end
                    end
                    5'h10: begin                                  // MOVG load
                        if (opc_ext) begin `TRAP; state <= S_END; end
                        else begin g_do_read <= 1'b1; state <= S_G_READ; end
                    end
                    5'h12: begin                                  // MOVG store / XCH
                        if (opc_ext) begin `TRAP; state <= S_END; end
                        else state <= S_G_ALU;
                    end
                    5'h13: state <= S_G_ALU;                      // STC
                    default: begin g_do_read <= 1'b1; state <= S_G_READ; end
                endcase
            end

            S_G_PREIMM: `FETCH(g_pre_n, S_G_ALU)      // MOV:G #imm: immediate before the write
            S_G_POSTIMM: `FETCH(g_post_n, S_G_ALU)    // CMP:G #imm: immediate after the read

            S_G_READ: begin
                // MCU_Operand_Read
                case (g_type)
                    T_DIRECT: begin
                        g_opd <= g_word ? r[g_reg] : {8'h0, r[g_reg][7:0]};
                        state <= (g_post_n != 2'd0) ? S_G_POSTIMM : S_G_ALU;
                    end
                    T_IMMEDIATE: begin
                        g_opd <= g_imm;
                        state <= (g_post_n != 2'd0) ? S_G_POSTIMM : S_G_ALU;
                    end
                    default: begin
                        if (g_word) begin
                            if (g_ea[0]) begin exc_v <= 1'b1; exc_t <= 2'd0; end
                            `READ({g_ep, g_ea & 16'hfffe}, 2, S_G_READ2)
                        end else `READ({g_ep, g_ea}, 1, S_G_READ2)
                    end
                endcase
            end
            S_G_READ2: begin
                g_opd <= g_word ? m_data[15:0] : {8'h0, m_data[7:0]};
                state <= (g_post_n != 2'd0) ? S_G_POSTIMM : S_G_ALU;
            end

            // ------------------------------------------------------------ ALU
            S_G_ALU: begin
                srn = sr;
                g_do_write <= 1'b0;
                state <= S_END;
                case (opc)
                    5'h00: begin                                  // MOVG_Immediate
                        if (oreg == 3'd6) begin
                            t16 = {{8{f_data[7]}}, f_data[7:0]};
                            g_res <= t16; g_do_write <= 1'b1;
                            srn = st_common(srn, {{16{t16[15]}}, t16}, g_word);
                        end else if (oreg == 3'd7) begin
                            t16 = f_data[15:0];
                            g_res <= t16; g_do_write <= 1'b1;
                            srn = st_common(srn, {16'h0, t16}, g_word);
                        end else if (oreg == 3'd4) begin
                            t16 = g_word ? {{8{f_data[7]}}, f_data[7:0]} : {8'h0, f_data[7:0]};
                            srn = st_nzvc(srn, alu_add(g_opd, t16, 1'b0, 1'b1, g_word));
                        end else begin
                            srn = st_nzvc(srn, alu_add(g_opd, f_data[15:0], 1'b0, 1'b1, g_word));
                        end
                    end
                    5'h01: begin                                  // ADDQ
                        case (oreg)
                            3'd0: t16 = 16'd1;
                            3'd1: t16 = 16'd2;
                            3'd4: t16 = 16'hffff;
                            3'd5: t16 = 16'hfffe;
                            default: begin t16 = 16'd0; `TRAP; end
                        endcase
                        f = alu_add(g_opd, t16, 1'b0, 1'b0, g_word);
                        srn = st_nzvc(srn, f);
                        g_res <= f[15:0]; g_do_write <= 1'b1;
                    end
                    5'h02: begin                                  // CLR group
                        case (oreg)
                            3'd3: begin                           // CLR
                                g_res <= 16'h0; g_do_write <= 1'b1;
                                srn[ST_N] = 1'b0; srn[ST_Z] = 1'b1; srn[ST_V] = 1'b0; srn[ST_C] = 1'b0;
                            end
                            3'd6: begin                           // TST
                                srn = st_common(srn, {16'h0, g_opd}, g_word);
                                srn[ST_C] = 1'b0;
                            end
                            3'd2: begin                           // EXTU
                                r[g_reg] <= {8'h0, r[g_reg][7:0]};
                                srn[ST_N] = 1'b0; srn[ST_Z] = (r[g_reg][7:0] == 8'h0); srn[ST_V] = 1'b0; srn[ST_C] = 1'b0;
                            end
                            3'd0: begin                           // SWAP
                                t16 = {r[g_reg][7:0], r[g_reg][15:8]};
                                r[g_reg] <= t16;
                                srn = st_common(srn, {16'h0, t16}, 1'b1);
                            end
                            3'd5: begin                           // NOT
                                t16 = ~g_opd;
                                g_res <= t16; g_do_write <= 1'b1;
                                srn = st_common(srn, {16'h0, t16}, g_word);
                            end
                            3'd4: begin                           // NEG
                                f = alu_add(16'h0, g_opd, 1'b0, 1'b1, g_word);
                                srn = st_nzvc(srn, f);
                                g_res <= f[15:0]; g_do_write <= 1'b1;
                            end
                            default: begin                        // EXTS (flags from the unextended register)
                                r[g_reg] <= {{8{r[g_reg][7]}}, r[g_reg][7:0]};
                                srn = st_common(srn, {16'h0, r[g_reg]}, 1'b1);
                            end
                        endcase
                    end
                    5'h03: begin                                  // shifts
                        logic c;
                        t16 = g_opd;
                        case (oreg)
                            3'd3: begin c = t16[0]; t16 = t16 >> 1; end                                  // SHLR
                            3'd2, 3'd0: begin c = g_word ? t16[15] : t16[7]; t16 = t16 << 1; end       // SHLL, SHAL
                            3'd6: begin c = g_word ? t16[15] : t16[7]; t16 = (t16 << 1) | {15'h0, sr[ST_C]}; end // ROTXL
                            3'd4: begin c = g_word ? t16[15] : t16[7]; t16 = (t16 << 1) | {15'h0, c}; end       // ROTL
                            3'd1: begin                                                                    // SHAR
                                c = t16[0];
                                if (g_word) t16 = {t16[15], t16[15:1]};
                                else t16 = {8'h0, t16[7], t16[7:1]};
                            end
                            default: begin                                                                 // ROTR
                                c = t16[0];
                                if (g_word) t16 = {c, t16[15:1]};
                                else t16 = {8'h0, c, t16[7:1]};
                            end
                        endcase
                        srn[ST_C] = c;
                        srn = st_common(srn, {16'h0, t16}, g_word);
                        g_res <= t16; g_do_write <= 1'b1;
                    end
                    5'h04, 5'h06: begin                           // ADD, SUB
                        f = alu_add(r[oreg], g_opd, 1'b0, opc == 5'h06, g_word);
                        srn = st_nzvc(srn, f);
                        if (g_word) r[oreg] <= f[15:0]; else r[oreg][7:0] <= f[7:0];
                    end
                    5'h05: begin                                  // ADDS
                        r[oreg] <= r[oreg] + (g_word ? g_opd : {{8{g_opd[7]}}, g_opd[7:0]});
                    end
                    5'h07: begin                                  // SUBS
                        r[oreg] <= r[oreg] - (g_word ? g_opd : {{8{g_opd[7]}}, g_opd[7:0]});
                    end
                    5'h08, 5'h0c: begin                           // OR, XOR
                        t16 = (opc == 5'h08) ? (r[oreg] | g_opd) : (r[oreg] ^ g_opd);
                        r[oreg] <= t16;
                        srn = st_common(srn, {16'h0, t16}, g_word);
                    end
                    5'h0a: begin                                  // AND
                        t16 = r[oreg] & g_opd;
                        if (g_word) r[oreg] <= t16; else r[oreg][7:0] <= t16[7:0];
                        srn = st_common(srn, {16'h0, g_word ? t16 : {r[oreg][15:8], t16[7:0]}}, g_word);
                    end
                    5'h09, 5'h0b: begin
                        if (g_type == T_IMMEDIATE) begin          // ORC / ANDC
                            cr = ctrl_read(oreg, g_word);
                            if (cr[16]) `TRAP;
                            t16 = (opc == 5'h09) ? (cr[15:0] | g_opd) : (cr[15:0] & g_opd);
                            // MCU_ControlRegisterWrite
                            if (g_word) begin
                                case (oreg)
                                    3'd0: srn = t16 & SR_MASK;
                                    3'd5: dp <= t16[7:0];
                                    3'd4: ep <= t16[7:0];
                                    3'd3: br <= t16[7:0];
                                    default: `TRAP;
                                endcase
                            end else begin
                                case (oreg)
                                    3'd1: srn = {srn[15:8], t16[7:0]} & SR_MASK;
                                    3'd3: br <= t16[7:0];
                                    3'd4: ep <= t16[7:0];
                                    3'd5: dp <= t16[7:0];
                                    3'd7: tp <= t16[7:0];
                                    default: `TRAP;
                                endcase
                            end
                            if (oreg >= 3'd2) srn = st_common(srn, {16'h0, t16}, g_word);
                            ex_ignore <= 1'b1;
                        end else begin                            // BSET / BCLR (bit from register)
                            bitn = r[oreg][3:0];
                            srn[ST_Z] = !g_opd[bitn];
                            t16 = g_opd;
                            t16[bitn] = (opc == 5'h09);
                            g_res <= t16; g_do_write <= 1'b1;
                        end
                    end
                    5'h0e: begin                                  // CMP
                        srn = st_nzvc(srn, alu_add(r[oreg], g_opd, 1'b0, 1'b1, g_word));
                    end
                    5'h0f: begin                                  // BTST (bit from register)
                        srn[ST_Z] = !g_opd[r[oreg][3:0]];
                    end
                    5'h10: begin                                  // MOVG load
                        if (g_word) r[oreg] <= g_opd; else r[oreg][7:0] <= g_opd[7:0];
                        srn = st_common(srn, {16'h0, g_opd}, g_word);
                    end
                    5'h11: begin                                  // LDC
                        if (g_word) begin
                            case (oreg)
                                3'd0: srn = g_opd & SR_MASK;
                                3'd5: dp <= g_opd[7:0];
                                3'd4: ep <= g_opd[7:0];
                                3'd3: br <= g_opd[7:0];
                                default: `TRAP;
                            endcase
                        end else begin
                            case (oreg)
                                3'd1: srn = {srn[15:8], g_opd[7:0]} & SR_MASK;
                                3'd3: br <= g_opd[7:0];
                                3'd4: ep <= g_opd[7:0];
                                3'd5: dp <= g_opd[7:0];
                                3'd7: tp <= g_opd[7:0];
                                default: `TRAP;
                            endcase
                        end
                        ex_ignore <= 1'b1;
                    end
                    5'h12: begin                                  // MOVG store / XCH
                        if (g_type == T_DIRECT) begin
                            if (g_word) begin
                                r[oreg] <= r[g_reg];
                                r[g_reg] <= r[oreg];
                            end else `TRAP;
                        end else begin
                            g_res <= r[oreg]; g_do_write <= 1'b1;
                            srn = st_common(srn, {16'h0, r[oreg]}, g_word);
                        end
                    end
                    5'h13: begin                                  // STC
                        cr = ctrl_read(oreg, g_word);
                        if (cr[16]) `TRAP;
                        g_res <= cr[15:0]; g_do_write <= 1'b1;
                    end
                    5'h14, 5'h16: begin                           // ADDX, SUBX
                        f = alu_add(r[oreg], g_opd, sr[ST_C], opc == 5'h16, g_word);
                        srn = st_nzvc(srn, f);
                        if (opc == 5'h14 && !sr[ST_Z]) srn[ST_Z] = 1'b0;
                        if (g_word) r[oreg] <= f[15:0]; else r[oreg][7:0] <= f[7:0];
                    end
                    5'h15: begin                                  // MULXU
                        if (g_word) begin
                            p32 = {16'h0, g_opd} * {16'h0, r[oreg]};
                            r[{oreg[2:1], 1'b0}] <= p32[31:16];
                            r[{oreg[2:1], 1'b1}] <= p32[15:0];
                            srn[ST_N] = p32[31];
                        end else begin
                            p32 = {24'h0, g_opd[7:0]} * {24'h0, r[oreg][7:0]};
                            r[oreg] <= p32[15:0];
                            srn[ST_N] = p32[15];
                        end
                        srn[ST_Z] = (p32 == 32'h0); srn[ST_V] = 1'b0; srn[ST_C] = 1'b0;
                    end
                    5'h17: begin                                  // DIVXU
                        if (g_opd == 16'h0) begin
                            `TRAP;
                            srn[ST_N] = 1'b0; srn[ST_Z] = 1'b1; srn[ST_V] = 1'b0; srn[ST_C] = 1'b0;
                        end else begin
                            d_div <= g_word ? g_opd : {8'h0, g_opd[7:0]};
                            d_q <= g_word ? {r[{oreg[2:1], 1'b0}], r[{oreg[2:1], 1'b1}]} : {16'h0, r[oreg]};
                            d_rem <= 32'h0;
                            d_cnt <= 6'd32;
                            state <= S_G_DIV;
                        end
                    end
                    5'h18, 5'h19, 5'h1a, 5'h1b, 5'h1c, 5'h1d: begin   // BSET/BCLR/BNOT #imm
                        bitn = {opc[0], oreg};
                        srn[ST_Z] = !g_opd[bitn];
                        t16 = g_opd;
                        case (opc[2:1])
                            2'b00: t16[bitn] = 1'b1;
                            2'b01: t16[bitn] = 1'b0;
                            default: t16[bitn] = ~t16[bitn];
                        endcase
                        g_res <= t16; g_do_write <= 1'b1;
                    end
                    5'h1e, 5'h1f: begin                           // BTST #imm
                        srn[ST_Z] = !g_opd[{opc[0], oreg}];
                    end
                    default: ;
                endcase
                sr <= srn;
            end

            S_G_DIV: begin
                if (d_cnt != 6'd0) begin
                    logic [32:0] t;
                    t = {d_rem[31:0], d_q[31]} - {17'h0, d_div};
                    if (!t[32]) begin d_rem <= t[31:0]; d_q <= {d_q[30:0], 1'b1}; end
                    else begin d_rem <= {d_rem[30:0], d_q[31]}; d_q <= {d_q[30:0], 1'b0}; end
                    d_cnt <= d_cnt - 6'd1;
                end else begin
                    srn = sr;
                    if (g_word) begin
                        if (d_q > 32'h0000ffff) begin
                            srn[ST_N] = 1'b0; srn[ST_Z] = 1'b0; srn[ST_V] = 1'b1; srn[ST_C] = 1'b0;
                        end else begin
                            r[{oreg[2:1], 1'b0}] <= d_rem[15:0];
                            r[{oreg[2:1], 1'b1}] <= d_q[15:0];
                            srn = st_common(srn, d_q, 1'b1);
                            srn[ST_C] = 1'b0;
                        end
                    end else begin
                        if (d_q > 32'h000000ff) begin
                            srn[ST_N] = 1'b0; srn[ST_Z] = 1'b0; srn[ST_V] = 1'b1; srn[ST_C] = 1'b0;
                        end else begin
                            r[oreg] <= {d_rem[7:0], d_q[7:0]};
                            srn = st_common(srn, d_q, 1'b0);
                            srn[ST_C] = 1'b0;
                        end
                    end
                    sr <= srn;
                    state <= S_END;
                end
            end

            S_G_WRITE: begin
                // MCU_Operand_Write
                case (g_type)
                    T_DIRECT: begin
                        if (g_word) r[g_reg] <= g_res; else r[g_reg][7:0] <= g_res[7:0];
                        state <= S_END;
                    end
                    T_IMMEDIATE: begin
                        exc_v <= 1'b1; exc_t <= 2'd1;
                        state <= S_END;
                    end
                    default: begin
                        if (g_word) begin
                            if (g_ea[0]) begin exc_v <= 1'b1; exc_t <= 2'd0; end
                            `WRITE({g_ep, g_ea & 16'hfffe}, 2, g_res, S_END)
                        end else `WRITE({g_ep, g_ea}, 1, {8'h0, g_res[7:0]}, S_END)
                    end
                endcase
            end

            // ------------------------------------------------------------ end
            S_END: begin
                if (g_do_write) begin
                    g_do_write <= 1'b0;
                    state <= S_G_WRITE;
                end else begin
                    // MCU_ReadInstruction's trace check (only when an instruction ran)
                    if (ran && sr[ST_T]) begin exc_v <= 1'b1; exc_t <= 2'd2; end
                    step_done <= 1'b1;
                    state <= S_IDLE;
                end
            end

            default: state <= S_IDLE;
            endcase
            traps <= traps + ntr;
        end
    end

endmodule
