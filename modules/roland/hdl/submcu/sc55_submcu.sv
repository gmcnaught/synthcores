// SC-55mk2 sub-MCU (M37450, a 740-family CPU) as the reference models it:
// submcu.cpp (SM_Update, SM_Read/SM_Write, SM_SysRead/SM_SysWrite, the opcode
// table, SM_HandleInterrupt, SM_UpdateTimer, SM_UpdateUART), quirks included:
//
// * only the opcodes in SM_Opcode_Table; the others do nothing (the
//   reference prints a diagnostic) and take one byte
// * SM_Read16 reads the same address twice: pointers are {b, b}
// * the stack is at address S (page 0), not 0x100 + S
// * STA zp,X and STX zp,"Y" add X without wrapping at 0xff
// * T flag: LDA/ORA/AND use mem[X] for A (no flags)
// * one instruction is 48 cycles; SM_Update(cycles) runs while
//   sm.cycles < 5 * cycles; the timer advances every 16 cycles
//
// Ports: while `run` is high the loop runs up to `target5` (5 * the MCU's
// cycle count after its instruction: SM_Update(cycles)); `target5` may grow
// while it runs (the loop simply continues), `done` pulses when it is
// reached and `caught_up` is high while it is idle there; the sys port is the main
// MCU's EC00-ECFF window (address & 0xff); MIDI bytes come from a buffer
// (rx_avail/rx_data/rx_pop, the MCU's uart_buffer); `buttons` and the P0
// latch give MCU_ReadP1; ga5_we/ga5_val report MCU_GA_SetGAInt(5, ...).
module sc55_submcu (
    input  logic        clk,
    input  logic        rst,              // power-on: all state zero

    input  logic        ld_we,            // rom_sm.bin load
    input  logic [11:0] ld_addr,
    input  logic [7:0]  ld_data,

    input  logic        ld_en,            // bench: load registers (in Q_IDLE)
    input  logic [15:0] ld_pc,
    input  logic [7:0]  ld_a, ld_x, ld_y, ld_s, ld_sr,
    input  logic        ld_sleep,

    input  logic        reset_start,      // SM_Reset
    output logic        reset_done,
    output logic        ready,            // power-on clear of the RAMs finished
    input  logic        run,
    input  logic [63:0] target5,
    output logic        done,
    output logic        caught_up,

    input  logic        sys_req,
    input  logic        sys_we,
    input  logic [7:0]  sys_addr,
    input  logic [7:0]  sys_wdata,
    output logic [7:0]  sys_rdata,
    output logic        sys_ack,

    input  logic        rx_avail,
    input  logic [7:0]  rx_data,
    output logic        rx_pop,
    output logic [7:0]  rx_byte,          // mcu.uart_rx_byte
    input  logic        rx_delay_we,      // the MCU's SSR write sets mcu.uart_rx_delay too
    input  logic [63:0] rx_delay_wv,

    input  logic [31:0] buttons,
    output logic        ga5_we,
    output logic        ga5_val,
    // MIDI out: the UART2 data register written (patch_ref.py's tx hook, src 1)
    output logic        tx_stb,
    output logic [7:0]  tx_byte,

    // state (bench)
    output logic [15:0] st_pc,
    output logic [7:0]  st_a, st_x, st_y, st_s, st_sr,
    output logic [63:0] st_cycles,
    output logic        st_sleep,
    output logic [7:0]  st_p0_data, st_p0_dir, st_p1_dir, st_p1_data,
    output logic [7:0]  st_prescaler, st_counter,
    output logic [63:0] st_timer_cycles, st_rx_delay,
    output logic        st_gotbyte,
    output logic [31:0] st_instructions
);
    localparam logic [4:0] D_P1_DATA = 5'h00, D_P1_DIR = 5'h01, D_RAM_DIR = 5'h02,
        D_U1_MS = 5'h05, D_U1_CTRL = 5'h06, D_U2_DATA = 5'h08, D_U2_MS = 5'h09, D_U2_CTRL = 5'h0a,
        D_U3_MS = 5'h0d, D_U3_CTRL = 5'h0e, D_IPCM0 = 5'h10, D_IPCE0 = 5'h14, D_SEMA = 5'h19,
        D_COLL = 5'h1a, D_INT_EN = 5'h1b, D_INT_REQ = 5'h1c, D_PRESC = 5'h1d, D_TIMER = 5'h1e,
        D_TCTRL = 5'h1f;
    localparam int F_C = 0, F_Z = 1, F_I = 2, F_D = 3, F_T = 5, F_N = 7;   // B (4), V (6): never changed by the implemented opcodes

    // ------------------------------------------------------------ state
    logic [15:0] pc;
    logic [7:0]  a, x, y, s, sr;
    logic [63:0] cycles, timer_cycles, rx_delay;
    logic        sleep, gotbyte;
    // RAM (128 B) and shared RAM (192 B) are block RAMs: reads as the ROM's
    // (address from the state, data next clock); writes go through a
    // one-entry buffer (written the next clock), forwarded to a read of the
    // same byte in that clock.
    logic        wq_v, wq_ram;               // pending write: valid, to RAM (else shared)
    logic [7:0]  wq_a, wq_d;
    logic [7:0]  ram_q, sh_q;
    logic [7:0]  clr_i;
    logic        clr_on;
    logic [7:0]  access [24];
    logic [7:0]  dm     [32];
    logic [7:0]  p0_data, p0_dir, p1_dir, p1_data, presc, counter;
    logic [31:0] ninstr;

    logic [11:0] rom_ra;
    logic [7:0]  rom_q;
    sdp_ram #(.AW(12), .DW(8)) u_rom (
        .clk, .we(ld_we), .wa(ld_addr), .wd(ld_data), .ra(rom_ra), .rd(rom_q));
    logic [15:0] mem_ra;                       // the address read this clock (see the state mux below)
    logic [7:0]  sh_ra;
    sdp_ram #(.AW(7), .DW(8)) u_ram (
        .clk, .we(clr_on || (wq_v && wq_ram)), .wa(clr_on ? clr_i[6:0] : wq_a[6:0]), .wd(clr_on ? 8'h00 : wq_d),
        .ra(mem_ra[6:0]), .rd(ram_q));
    sdp_ram #(.AW(8), .DW(8)) u_shared (
        .clk, .we(clr_on || (wq_v && !wq_ram)), .wa(clr_on ? clr_i : wq_a), .wd(clr_on ? 8'h00 : wq_d),
        .ra(sh_ra), .rd(sh_q));

    // MCU_ReadP1
    function automatic logic [7:0] read_p1(input logic [7:0] p0, input logic [31:0] b);
        logic [7:0] d;
        d = 8'hff;
        if (!p0[0]) d &= ~b[7:0];
        if (!p0[1]) d &= ~b[15:8];
        if (!p0[2]) d &= ~b[23:16];
        if (!p0[3]) d &= ~b[31:24];
        return d;
    endfunction

    // ------------------------------------------------------------ decode
    typedef enum logic [3:0] {
        M_IMP, M_IMM, M_ZP, M_ZPX, M_ZPXU, M_ZPY, M_ABS, M_ABSX, M_ABSY,
        M_INDX, M_INDY, M_REL, M_ZP2       // M_ZP2: two operand bytes, the first a zero-page address (BBx zp)
    } mode_t;
    typedef enum logic [4:0] {
        K_NI, K_NOP, K_LDA, K_LDX, K_LDY, K_STA, K_STX, K_STY, K_ORA, K_AND, K_CMP, K_CPX, K_CPY,
        K_INC, K_DEC, K_INCA, K_DECA, K_LDM, K_BB, K_SEB, K_BR, K_JSR, K_JSRP, K_JMP, K_JMPI,
        K_RTS, K_RTI, K_PHA, K_PLA, K_IMPL, K_STP
    } kind_t;

    function automatic logic [8:0] dec(input logic [7:0] op);   // {mode, kind}
        mode_t m; kind_t k;
        m = M_IMP; k = K_NI;
        casez (op)
            8'b???0_0011, 8'b???1_0011: begin m = M_REL; k = K_BB; end        // BBx A
            8'b???0_0111, 8'b???1_0111: begin m = M_ZP2; k = K_BB; end        // BBx zp
            8'b???0_1011, 8'b???1_1011: begin m = M_IMP; k = K_SEB; end       // SEB/CLB A
            8'b???0_1111, 8'b???1_1111: begin m = M_ZP;  k = K_SEB; end       // SEB/CLB zp
            default: ;
        endcase
        case (op)
            8'h01: begin m = M_INDX; k = K_ORA; end  8'h05: begin m = M_ZP;   k = K_ORA; end
            8'h09: begin m = M_IMM;  k = K_ORA; end  8'h0d: begin m = M_ABS;  k = K_ORA; end
            8'h11: begin m = M_INDY; k = K_ORA; end  8'h15: begin m = M_ZPX;  k = K_ORA; end
            8'h19: begin m = M_ABSY; k = K_ORA; end  8'h1d: begin m = M_ABSX; k = K_ORA; end
            8'h21: begin m = M_INDX; k = K_AND; end  8'h25: begin m = M_ZP;   k = K_AND; end
            8'h29: begin m = M_IMM;  k = K_AND; end  8'h2d: begin m = M_ABS;  k = K_AND; end
            8'h31: begin m = M_INDY; k = K_AND; end  8'h35: begin m = M_ZPX;  k = K_AND; end
            8'h39: begin m = M_ABSY; k = K_AND; end  8'h3d: begin m = M_ABSX; k = K_AND; end
            8'ha1: begin m = M_INDX; k = K_LDA; end  8'ha5: begin m = M_ZP;   k = K_LDA; end
            8'ha9: begin m = M_IMM;  k = K_LDA; end  8'had: begin m = M_ABS;  k = K_LDA; end
            8'hb1: begin m = M_INDY; k = K_LDA; end  8'hb5: begin m = M_ZPX;  k = K_LDA; end
            8'hb9: begin m = M_ABSY; k = K_LDA; end  8'hbd: begin m = M_ABSX; k = K_LDA; end
            8'hc1: begin m = M_INDX; k = K_CMP; end  8'hc5: begin m = M_ZP;   k = K_CMP; end
            8'hc9: begin m = M_IMM;  k = K_CMP; end  8'hcd: begin m = M_ABS;  k = K_CMP; end
            8'hd1: begin m = M_INDY; k = K_CMP; end  8'hd5: begin m = M_ZPX;  k = K_CMP; end
            8'hd9: begin m = M_ABSY; k = K_CMP; end  8'hdd: begin m = M_ABSX; k = K_CMP; end
            8'h81: begin m = M_INDX; k = K_STA; end  8'h85: begin m = M_ZP;   k = K_STA; end
            8'h8d: begin m = M_ABS;  k = K_STA; end  8'h91: begin m = M_INDY; k = K_STA; end
            8'h95: begin m = M_ZPXU; k = K_STA; end  8'h99: begin m = M_ABSY; k = K_STA; end
            8'h9d: begin m = M_ABSX; k = K_STA; end
            8'ha2: begin m = M_IMM;  k = K_LDX; end  8'ha6: begin m = M_ZP;   k = K_LDX; end
            8'hae: begin m = M_ABS;  k = K_LDX; end  8'hb6: begin m = M_ZPY;  k = K_LDX; end
            8'hbe: begin m = M_ABSY; k = K_LDX; end
            8'ha0: begin m = M_IMM;  k = K_LDY; end  8'ha4: begin m = M_ZP;   k = K_LDY; end
            8'hac: begin m = M_ABS;  k = K_LDY; end  8'hb4: begin m = M_ZPX;  k = K_LDY; end
            8'hbc: begin m = M_ABSX; k = K_LDY; end
            8'h86: begin m = M_ZP;   k = K_STX; end  8'h96: begin m = M_ZPXU; k = K_STX; end
            8'h8e: begin m = M_ABS;  k = K_STX; end
            8'h84: begin m = M_ZP;   k = K_STY; end  8'h94: begin m = M_ZPX;  k = K_STY; end
            8'h8c: begin m = M_ABS;  k = K_STY; end
            8'he0: begin m = M_IMM;  k = K_CPX; end  8'he4: begin m = M_ZP;   k = K_CPX; end
            8'hec: begin m = M_ABS;  k = K_CPX; end
            8'hc0: begin m = M_IMM;  k = K_CPY; end  8'hc4: begin m = M_ZP;   k = K_CPY; end
            8'hcc: begin m = M_ABS;  k = K_CPY; end
            8'h3a: begin m = M_IMP;  k = K_INCA; end
            8'he6: begin m = M_ZP;   k = K_INC; end  8'hf6: begin m = M_ZPX;  k = K_INC; end
            8'hee: begin m = M_ABS;  k = K_INC; end  8'hfe: begin m = M_ABSX; k = K_INC; end
            8'h1a: begin m = M_IMP;  k = K_DECA; end
            8'hc6: begin m = M_ZP;   k = K_DEC; end  8'hd6: begin m = M_ZPX;  k = K_DEC; end
            8'hce: begin m = M_ABS;  k = K_DEC; end  8'hde: begin m = M_ABSX; k = K_DEC; end
            8'h3c: begin m = M_ZP2;  k = K_LDM; end
            8'h10, 8'h80, 8'h90, 8'hb0, 8'hd0, 8'hf0: begin m = M_REL; k = K_BR; end
            8'h20: begin m = M_ABS;  k = K_JSR; end
            8'h02: begin m = M_ZP;   k = K_JSRP; end   // JSR (zp)
            8'h22: begin m = M_IMM;  k = K_JSR; end    // JSR \$ff00|n
            8'h4c: begin m = M_ABS;  k = K_JMP; end
            8'h6c: begin m = M_ABS;  k = K_JMPI; end
            8'hb2: begin m = M_ZP;   k = K_JMPI; end
            8'h60: begin m = M_IMP;  k = K_RTS; end
            8'h40: begin m = M_IMP;  k = K_RTI; end
            8'h48: begin m = M_IMP;  k = K_PHA; end
            8'h68: begin m = M_IMP;  k = K_PLA; end
            8'h42: begin m = M_IMP;  k = K_STP; end
            8'hea: begin m = M_IMP;  k = K_NOP; end
            8'h78, 8'h58, 8'hd8, 8'h12, 8'h38, 8'h18, 8'h9a, 8'h8a, 8'haa, 8'he8, 8'hc8:
                   begin m = M_IMP;  k = K_IMPL; end
            default: ;
        endcase
        return {m, k};
    endfunction

    function automatic int nbytes(input mode_t m);
        case (m)
            M_IMP: return 0;
            M_ABS, M_ABSX, M_ABSY, M_ZP2: return 2;
            default: return 1;
        endcase
    endfunction

    // ------------------------------------------------------------ interrupts (SM_HandleInterrupt)
    logic       int_go;
    logic [3:0] int_vec;
    logic [7:0] int_clr;          // INT_REQUEST bits to clear (COLLISION handled separately)
    logic       int_coll;
    // {go, vector, request bits to clear, collision} for request register `rq`
    function automatic logic [13:0] int_eval(input logic i_flag, input logic [7:0] u1c, input logic [7:0] u2c,
                                             input logic [7:0] u3c, input logic [7:0] en, input logic [7:0] rq,
                                             input logic [7:0] tctrl, input logic [7:0] coll);
        if (i_flag) return 14'h0;
        if (u1c[3] && en[7] && rq[7]) return {1'b1, 4'd8, 8'h80, 1'b0};
        if (u2c[3] && en[6] && rq[6]) return {1'b1, 4'd7, 8'h40, 1'b0};
        if (u3c[3] && en[5] && rq[5]) return {1'b1, 4'd6, 8'h20, 1'b0};
        if (tctrl[7] && en[4] && rq[4]) return {1'b1, 4'd5, 8'h10, 1'b0};
        if (tctrl[6] && en[3] && rq[3]) return {1'b1, 4'd4, 8'h08, 1'b0};
        if (coll[7:6] == 2'b11) return {1'b1, 4'd3, 8'h00, 1'b1};
        if (!u1c[4] && en[2] && rq[2]) return {1'b1, 4'd2, 8'h04, 1'b0};
        if (!u2c[4] && en[1] && rq[1]) return {1'b1, 4'd1, 8'h02, 1'b0};
        if (!u3c[4] && en[0] && rq[0]) return {1'b1, 4'd0, 8'h01, 1'b0};
        return 14'h0;
    endfunction
    always_comb begin
        {int_go, int_vec, int_clr, int_coll} = int_eval(sr[F_I], dm[D_U1_CTRL], dm[D_U2_CTRL], dm[D_U3_CTRL],
                                                        dm[D_INT_EN], dm[D_INT_REQ], dm[D_TCTRL], dm[D_COLL]);
    end

    // ------------------------------------------------------------ sequencer
    // Every read is issued in one state from an address held in a register
    // (pc, ea, s, npc) and its data is used in the next state: ROM data from
    // the block RAM (address = rom_ra, combinational from the state), the
    // others from rd_reg. Side effects of a read happen when it is issued.
    typedef enum logic [4:0] {
        Q_IDLE, Q_LOOP, Q_PUSH, Q_VEC_LO, Q_VEC_HI, Q_VEC_END, Q_F, Q_OPC, Q_B1, Q_B2,
        Q_ADDR, Q_PTR, Q_PTRJ, Q_VX, Q_DATA, Q_JPUSH, Q_POP, Q_POPD, Q_TIMER
    } q_t;
    q_t q;

    logic [7:0]  opc;
    mode_t       md;
    kind_t       kd;
    logic        nb2;              // two operand bytes
    logic        ind_done;         // the pointer of (zp,X)/(zp),Y has been read
    logic [7:0]  b1, b2, vx, plo;
    logic [15:0] ea, npc;
    logic [1:0]  pcnt;
    logic        in_reset;         // the vector fetch is SM_Reset's
    logic        tcont;            // Q_TIMER continues (cycles already advanced)
    logic [63:0] cyc48;            // cycles + 48
    logic signed [64:0] tlag, tlag48;   // cycles - timer_cycles, and + 48
    logic [1:0]  rd_src;                     // 0 rd_reg, 1 ROM, 2 RAM, 3 shared
    logic [7:0]  rd_reg;
    wire  [7:0]  rdata = (rd_src == 2'd1) ? rom_q : (rd_src == 2'd2) ? ram_q : (rd_src == 2'd3) ? sh_q : rd_reg;
    logic        sys_rd;                     // a sys read of the shared RAM: data next clock
    assign caught_up = (q == Q_IDLE) && !(cycles < target5) && !sys_rd;
    assign ready = !clr_on;

    // the address each state reads (dispatch: the operand or pointer, or mem[X] for T-mode ORA/AND)
    function automatic logic [15:0] dispatch_ra(input logic [15:0] e, input mode_t m, input kind_t k,
                                                input logic ind, input logic t, input logic [7:0] xv);
        if ((m == M_INDX || m == M_INDY) && !ind) return e;
        if (t && (k == K_ORA || k == K_AND)) return {8'h00, xv};
        return e;
    endfunction
    always_comb begin
        case (q)
            Q_LOOP, Q_F, Q_OPC, Q_TIMER: mem_ra = pc;
            Q_B1:                    mem_ra = nb2 ? pc : dispatch_ra(calc_ea(md, rdata, 8'h00, x, y), md, kd, ind_done, sr[F_T], x);
            Q_VEC_LO, Q_VEC_HI:      mem_ra = npc;
            Q_POP:                   mem_ra = {8'h00, s};
            Q_ADDR:                  mem_ra = dispatch_ra(ea, md, kd, ind_done, sr[F_T], x);
            default:                 mem_ra = ea;
        endcase
        rom_ra = mem_ra[11:0];
        sh_ra = (q == Q_IDLE) ? sys_addr : mem_ra[7:0];
    end

    // SM_Read: side effects now, data next clock (rdata)
    task automatic issue_read(input logic [15:0] addr);
        logic [12:0] ad;
        ad = addr[12:0];
        // synthesis translate_off
        if (ad != mem_ra[12:0]) $error("sc55_submcu: read of %h issued with mem_ra %h (state %0d)", ad, mem_ra[12:0], q);
        // synthesis translate_on
        rd_src <= 2'd0;
        rd_reg <= 8'h00;
        if (ad[12]) begin
            rd_src <= 2'd1;
        end else if (ad < 13'h80) begin
            if (wq_v && wq_ram && wq_a[6:0] == ad[6:0]) rd_reg <= wq_d;
            else rd_src <= 2'd2;
        end else if (ad >= 13'hc0 && ad < 13'hd8) begin
            rd_reg <= access[5'(ad[4:0])];
        end else if (ad >= 13'he0 && ad < 13'h100) begin
            case (ad[4:0])
                D_U2_DATA: begin gotbyte <= 1'b0; rd_reg <= rx_byte; end
                D_U1_MS:   rd_reg <= 8'h05;
                D_U2_MS:   rd_reg <= {6'h0, gotbyte, 1'b0} | 8'h05;
                D_U3_MS:   rd_reg <= 8'h05;
                D_P1_DATA: rd_reg <= read_p1(p0_data, buttons);
                D_P1_DIR:  rd_reg <= p1_dir;
                D_PRESC:   rd_reg <= presc;
                D_TIMER:   rd_reg <= counter;
                default:   rd_reg <= dm[ad[4:0]];
            endcase
        end else if (ad >= 13'h200 && ad < 13'h2c0) begin
            if (dm[D_RAM_DIR][ad[7:5]]) access[5'(ad[7:3])][ad[2:0]] <= 1'b0;
            if (wq_v && !wq_ram && wq_a == ad[7:0]) rd_reg <= wq_d;
            else rd_src <= 2'd3;
        end
    endtask

    // SM_Write
    task automatic do_write(input logic [15:0] addr, input logic [7:0] d);
        logic [12:0] ad;
        logic [7:0]  m0d, m0e;
        ad = addr[12:0];
        if (ad < 13'h80) begin
            wq_v <= 1'b1; wq_ram <= 1'b1; wq_a <= {1'b0, ad[6:0]}; wq_d <= d;
        end else if (ad >= 13'he0 && ad < 13'h100) begin
            m0d = dm[D_U3_MS];
            m0e = dm[D_U3_CTRL];
            case (ad[4:0])
                D_P1_DATA: p1_data <= d;
                D_P1_DIR:  p1_dir <= d;
                D_INT_REQ: dm[D_INT_REQ] <= dm[D_INT_REQ] & d;
                D_COLL:    dm[D_COLL] <= {d[7] & dm[D_COLL][7], d[6:0]};
                default:   dm[ad[4:0]] <= d;
            endcase
            if (ad[4:0] == D_U2_DATA) begin tx_stb <= 1'b1; tx_byte <= d; end
            if (ad[4:0] == D_U3_MS) m0d = d;
            if (ad[4:0] == D_U3_CTRL) m0e = d;
            if (ad[4:0] == D_U3_MS || ad[4:0] == D_U3_CTRL) begin
                ga5_we <= 1'b1;
                ga5_val <= m0d[7] && !m0e[5];
            end
        end else if (ad >= 13'h200 && ad < 13'h2c0) begin
            access[5'(ad[7:3])][ad[2:0]] <= 1'b1;
            wq_v <= 1'b1; wq_ram <= 1'b0; wq_a <= ad[7:0]; wq_d <= d;
        end
    endtask

    function automatic logic [7:0] nz(input logic [7:0] f, input logic [7:0] v);
        logic [7:0] r;
        r = f;
        r[F_Z] = (v == 8'h00);
        r[F_N] = v[7];
        return r;
    endfunction

    // address of the memory operand (or of the pointer, for (zp,X) and (zp),Y)
    function automatic logic [15:0] calc_ea(input mode_t m, input logic [7:0] lo, input logic [7:0] hi,
                                            input logic [7:0] xv, input logic [7:0] yv);
        case (m)
            M_ZPX, M_INDX: return {8'h00, lo + xv};
            M_ZPXU: return {8'h00, lo} + {8'h00, xv};
            M_ZPY:  return {8'h00, lo + yv};
            M_ABS:  return {hi, lo};
            M_ABSX: return {hi, lo} + {8'h00, xv};
            M_ABSY: return {hi, lo} + {8'h00, yv};
            default: return {8'h00, lo};         // M_ZP, M_ZP2, M_INDY, M_IMM, M_REL
        endcase
    endfunction

    // Q_ADDR's work: the memory operand's (or pointer's) address e, the first operand byte o1
    task automatic dispatch(input logic [15:0] e, input logic [7:0] o1);
        logic cond;
        q <= Q_TIMER;
        case (kd)
            K_BR: begin
                case (opc)
                    8'h10: cond = !sr[F_N];
                    8'h90: cond = !sr[F_C];
                    8'hb0: cond = sr[F_C];
                    8'hd0: cond = !sr[F_Z];
                    8'hf0: cond = sr[F_Z];
                    default: cond = 1'o1;     // BRA
                endcase
                if (cond) pc <= pc + {{8{o1[7]}}, o1};
            end
            K_BB: begin
                if (md == M_REL) begin
                    if (a[opc[7:5]] != opc[4]) pc <= pc + {{8{o1[7]}}, o1};
                end else begin
                    issue_read(e);           // BBx zp: the byte, then the test (disp in b2)
                    q <= Q_DATA;
                end
            end
            K_SEB: begin issue_read(e); q <= Q_DATA; end
            K_LDM: do_write({8'h00, b2}, o1);
            K_JMP: pc <= {b2, o1};
            K_JSR: begin
                npc <= (opc == 8'h22) ? {8'hff, o1} : {b2, o1};
                pcnt <= 2'd0;
                q <= Q_JPUSH;
            end
            K_JSRP, K_JMPI: begin issue_read(e); q <= Q_PTRJ; end
            default: begin
                if ((md == M_INDX || md == M_INDY) && !ind_done) begin
                    issue_read(e);           // the pointer
                    q <= Q_PTR;
                end else if (kd == K_STA) do_write(e, a);
                else if (kd == K_STX) do_write(e, x);
                else if (kd == K_STY) do_write(e, y);
                else if (kd == K_NI || kd == K_NOP) begin
                end else if (t_mode && (kd == K_ORA || kd == K_AND)) begin
                    issue_read({8'h00, x});   // T: the A operand is mem[X]
                    q <= Q_VX;
                end else if (md == M_IMM) begin
                    exec(o1, a);
                end else begin
                    issue_read(e);
                    q <= Q_DATA;
                end
            end
        endcase
    endtask

    wire t_mode = sr[F_T];

    always_ff @(posedge clk) begin : b_sc55_submcu_1
        logic [8:0]  dk;
        logic [7:0]  v, r, f;
        logic [8:0]  df;
        logic        cond;
        done <= 1'b0;
        reset_done <= 1'b0;
        sys_ack <= 1'b0;
        rx_pop <= 1'b0;
        ga5_we <= 1'b0;
        tx_stb <= 1'b0;
        wq_v <= 1'b0;                              // the pending write goes to its RAM at this edge
        if (rx_delay_we) rx_delay <= rx_delay_wv;
        if (clr_on) begin
            clr_i <= clr_i + 8'd1;
            if (clr_i == 8'd191) clr_on <= 1'b0;
        end
        if (rst) begin
            q <= Q_IDLE;
            clr_on <= 1'b1; clr_i <= 8'd0; sys_rd <= 1'b0;
            pc <= '0; a <= '0; x <= '0; y <= '0; s <= '0; sr <= '0;
            cycles <= '0; timer_cycles <= '0; rx_delay <= '0; sleep <= 1'b0; gotbyte <= 1'b0;
            cyc48 <= 64'd48; tlag <= '0; tlag48 <= 65'sd48;
            for (int i = 0; i < 24; i++) access[i] <= 8'h00;
            for (int i = 0; i < 32; i++) dm[i] <= 8'h00;
            p0_data <= '0; p0_dir <= '0; p1_dir <= '0; p1_data <= '0; presc <= '0; counter <= '0;
            rx_byte <= '0; ninstr <= '0; rd_src <= 2'd0; rd_reg <= '0; in_reset <= 1'b0;
            ea <= '0; npc <= '0; sys_rdata <= '0; ga5_val <= 1'b0; tcont <= 1'b0;
        end else begin
            case (q)
            // ---------------------------------------------------- idle: sys port, reset, run
            Q_IDLE: begin
                if (sys_rd) begin
                    sys_rd <= 1'b0;                    // the shared RAM's data (read last clock)
                    sys_rdata <= sh_q;
                    sys_ack <= 1'b1;
                end else if (sys_req && !sys_ack) begin
                    sys_ack <= 1'b1;
                    if (sys_we) begin
                        // SM_SysWrite
                        if (sys_addr < 8'hc0) begin
                            access[5'(sys_addr[7:3])][sys_addr[2:0]] <= 1'b1;
                            wq_v <= 1'b1; wq_ram <= 1'b0; wq_a <= sys_addr; wq_d <= sys_wdata;
                        end else if (sys_addr >= 8'hf8 && sys_addr < 8'hfc) begin
                            dm[D_IPCM0 + 5'(sys_addr[1:0])] <= sys_wdata;
                            if (sys_addr[1:0] == 2'd0) begin
                                dm[D_INT_REQ] <= dm[D_INT_REQ] | 8'h10;
                                dm[D_SEMA] <= dm[D_SEMA] & 8'h7f;
                            end
                        end else if (sys_addr == 8'hff) begin
                            dm[D_SEMA] <= {dm[D_SEMA][7:5], sys_wdata[4:0]};
                        end else if (sys_addr == 8'hf5) begin
                            p1_data <= sys_wdata;
                        end else if (sys_addr == 8'hf6) begin
                            p0_data <= sys_wdata;
                        end else if (sys_addr == 8'hf7) begin
                            p0_dir <= sys_wdata;
                        end
                    end else begin
                        // SM_SysRead
                        sys_rdata <= 8'h00;
                        if (sys_addr < 8'hc0) begin
                            if (!dm[D_RAM_DIR][sys_addr[7:5]]) access[5'(sys_addr[7:3])][sys_addr[2:0]] <= 1'b0;
                            if (wq_v && !wq_ram && wq_a == sys_addr) sys_rdata <= wq_d;
                            else begin sys_ack <= 1'b0; sys_rd <= 1'b1; end     // block RAM: data next clock
                        end else if (sys_addr >= 8'hf8 && sys_addr < 8'hfc) begin
                            if (sys_addr[1:0] == 2'd0) dm[D_INT_REQ] <= dm[D_INT_REQ] | 8'h10;
                            sys_rdata <= dm[D_IPCE0 + 5'(sys_addr[1:0])];
                            dm[D_IPCE0 + 5'(sys_addr[1:0])] <= 8'h00;
                        end else if (sys_addr == 8'hff) begin
                            sys_rdata <= dm[D_SEMA];
                        end else if (sys_addr == 8'hf5) begin
                            sys_rdata <= read_p1(p0_data, buttons);
                        end else if (sys_addr == 8'hf6) begin
                            sys_rdata <= 8'hff;
                        end else if (sys_addr == 8'hf7) begin
                            sys_rdata <= p0_dir;
                        end
                    end
                end else if (ld_en) begin
                    pc <= ld_pc; a <= ld_a; x <= ld_x; y <= ld_y; s <= ld_s; sr <= ld_sr; sleep <= ld_sleep;
                end else if (reset_start) begin
                    // SM_Reset: PC from vector 9 (0x1ffe), registers and cycles to 0
                    npc <= 16'h1ffe;
                    in_reset <= 1'b1;
                    q <= Q_VEC_LO;
                end else if (run && cycles < target5) begin
                    q <= Q_LOOP;
                end
            end

            // ---------------------------------------------------- SM_Update loop
            Q_LOOP: begin
                if (!(cycles < target5)) begin
                    done <= 1'b1;
                    q <= Q_IDLE;
                end else if (int_go) begin
                    if (int_coll) dm[D_COLL][7] <= 1'b0;
                    else dm[D_INT_REQ] <= dm[D_INT_REQ] & ~int_clr;
                    npc <= 16'h1fec + {11'h0, int_vec, 1'b0};
                    pcnt <= 2'd0;
                    q <= Q_PUSH;
                end else if (sleep) begin
                    q <= Q_TIMER;
                end else begin
                    issue_read(pc);                   // the opcode (Q_F's work)
                    pc <= pc + 16'd1;
                    q <= Q_OPC;
                end
            end

            // SM_StartVector: push PC high, PC low, SR; I set; wake; PC from the vector
            Q_PUSH: begin
                case (pcnt)
                    2'd0: do_write({8'h00, s}, pc[15:8]);
                    2'd1: do_write({8'h00, s}, pc[7:0]);
                    default: do_write({8'h00, s}, sr);
                endcase
                s <= s - 8'd1;
                pcnt <= pcnt + 2'd1;
                if (pcnt == 2'd2) begin
                    sr[F_I] <= 1'b1;
                    sleep <= 1'b0;
                    q <= Q_VEC_LO;
                end
            end
            Q_VEC_LO: begin
                issue_read(npc);
                npc <= npc + 16'd1;
                q <= Q_VEC_HI;
            end
            Q_VEC_HI: begin
                plo <= rdata;
                issue_read(npc);
                q <= Q_VEC_END;
            end
            Q_VEC_END: begin
                pc <= {rdata, plo};
                if (in_reset) begin
                    in_reset <= 1'b0;
                    a <= '0; x <= '0; y <= '0; s <= '0; sr <= '0;
                    cycles <= '0; sleep <= 1'b0;
                    cyc48 <= 64'd48;
                    tlag <= -$signed({1'b0, timer_cycles});            // cycles - timer_cycles
                    tlag48 <= 65'sd48 - $signed({1'b0, timer_cycles});
                    reset_done <= 1'b1;
                    q <= Q_IDLE;
                end else begin
                    q <= Q_F;                     // the instruction of this iteration
                end
            end

            // ---------------------------------------------------- fetch and operands
            Q_F: begin
                issue_read(pc);
                pc <= pc + 16'd1;
                q <= Q_OPC;
            end
            Q_OPC: begin
                dk = dec(rdata);
                opc <= rdata;
                md <= mode_t'(dk[8:5]);
                kd <= kind_t'(dk[4:0]);
                ninstr <= ninstr + 32'd1;
                ind_done <= 1'b0;
                nb2 <= nbytes(mode_t'(dk[8:5])) == 2;
                if (nbytes(mode_t'(dk[8:5])) != 0) begin
                    issue_read(pc);
                    pc <= pc + 16'd1;
                    q <= Q_B1;
                end else begin
                    // implied
                    q <= Q_TIMER;
                    case (kind_t'(dk[4:0]))
                        K_INCA: begin a <= a + 8'd1; sr <= nz(sr, a + 8'd1); end
                        K_DECA: begin a <= a - 8'd1; sr <= nz(sr, a - 8'd1); end
                        K_STP:  sleep <= 1'b1;
                        K_SEB: begin
                            r = a;
                            r[rdata[7:5]] = !rdata[4];
                            a <= r;
                        end
                        K_PHA: begin do_write({8'h00, s}, a); s <= s - 8'd1; end
                        K_PLA, K_RTS, K_RTI: begin
                            s <= s + 8'd1;
                            pcnt <= 2'd0;
                            q <= Q_POP;
                        end
                        K_IMPL: begin
                            case (rdata)
                                8'h78: sr[F_I] <= 1'b1;
                                8'h58: sr[F_I] <= 1'b0;
                                8'hd8: sr[F_D] <= 1'b0;
                                8'h12: sr[F_T] <= 1'b0;
                                8'h38: sr[F_C] <= 1'b1;
                                8'h18: sr[F_C] <= 1'b0;
                                8'h9a: s <= x;
                                8'h8a: begin a <= x; sr <= nz(sr, x); end
                                8'haa: begin x <= a; sr <= nz(sr, a); end
                                8'he8: begin x <= x + 8'd1; sr <= nz(sr, x + 8'd1); end
                                8'hc8: begin y <= y + 8'd1; sr <= nz(sr, y + 8'd1); end
                                default: ;
                            endcase
                        end
                        default: ;                // NOP, not implemented
                    endcase
                end
            end
            Q_B1: begin
                b1 <= rdata;
                if (nb2) begin
                    issue_read(pc);
                    pc <= pc + 16'd1;
                    q <= Q_B2;
                end else begin
                    // one operand byte: its data or pointer address is below 0x200 (never ROM), so the
                    // dispatch can issue its read now
                    ea <= calc_ea(md, rdata, 8'h00, x, y);
                    dispatch(calc_ea(md, rdata, 8'h00, x, y), rdata);
                end
            end
            Q_B2: begin
                b2 <= rdata;
                ea <= calc_ea(md, b1, rdata, x, y);
                q <= Q_ADDR;
            end

            // ---------------------------------------------------- address known: dispatch
            Q_ADDR: dispatch(ea, b1);
            Q_PTR: begin                          // pointer byte: {p, p} (+ Y)
                ea <= {rdata, rdata} + ((md == M_INDY) ? {8'h00, y} : 16'h0000);
                ind_done <= 1'b1;
                q <= Q_ADDR;
            end
            Q_PTRJ: begin
                if (kd == K_JMPI) begin
                    pc <= {rdata, rdata};
                    q <= Q_TIMER;
                end else begin
                    npc <= {rdata, rdata};
                    pcnt <= 2'd0;
                    q <= Q_JPUSH;
                end
            end
            Q_VX: begin
                vx <= rdata;
                if (md == M_IMM) begin
                    exec(b1, rdata);
                    q <= Q_TIMER;
                end else begin
                    issue_read(ea);
                    q <= Q_DATA;
                end
            end
            Q_DATA: begin
                exec(rdata, t_mode ? vx : a);
                q <= Q_TIMER;
            end

            Q_JPUSH: begin
                do_write({8'h00, s}, (pcnt == 2'd0) ? pc[15:8] : pc[7:0]);
                s <= s - 8'd1;
                pcnt <= pcnt + 2'd1;
                if (pcnt == 2'd1) begin
                    pc <= npc;
                    q <= Q_TIMER;
                end
            end

            // pops: s already incremented; read mem[s]
            Q_POP: begin
                issue_read({8'h00, s});
                q <= Q_POPD;
            end
            Q_POPD: begin
                q <= Q_TIMER;
                case (kd)
                    K_PLA: begin a <= rdata; sr <= nz(sr, rdata); end
                    K_RTS: begin
                        if (pcnt == 2'd0) begin pc[7:0] <= rdata; s <= s + 8'd1; pcnt <= 2'd1; q <= Q_POP; end
                        else pc[15:8] <= rdata;
                    end
                    default: begin                // RTI: SR, PC low, PC high
                        if (pcnt == 2'd0) begin sr <= rdata; s <= s + 8'd1; pcnt <= 2'd1; q <= Q_POP; end
                        else if (pcnt == 2'd1) begin pc[7:0] <= rdata; s <= s + 8'd1; pcnt <= 2'd2; q <= Q_POP; end
                        else pc[15:8] <= rdata;
                    end
                endcase
            end

            // ---------------------------------------------------- after the instruction
            Q_TIMER: begin : b_timer             // cycles += 48; SM_UpdateTimer (up to three 16-cycle ticks a clock); SM_UpdateUART
                logic [7:0]  pr, ct, ir;
                logic [63:0] tc, cy;
                logic signed [64:0] d;               // cy - timer_cycles
                logic [1:0]  n;                      // ticks this clock
                cy = tcont ? cycles : cyc48;
                d  = tcont ? tlag : tlag48;
                cycles <= cy;
                pr = presc; ct = counter; ir = dm[D_INT_REQ]; tc = timer_cycles;
                // ticks while timer_cycles < cy: tick k (k = 0, 1, 2) when d > 16 k
                n = (d > 65'sd32) ? 2'd3 : (d > 65'sd16) ? 2'd2 : (d > 65'sd0) ? 2'd1 : 2'd0;
                for (int k = 0; k < 3; k++) begin
                    if (2'(k) < n) begin
                        if (!dm[D_TCTRL][5] && !sleep) begin
                            if (pr == 8'h00) begin
                                pr = dm[D_PRESC];
                                if (ct == 8'h00) begin
                                    ct = dm[D_TIMER];
                                    ir = ir | 8'h08;
                                end else ct = ct - 8'd1;
                            end else pr = pr - 8'd1;
                        end
                    end
                end
                tc = timer_cycles + {58'h0, n, 4'h0};
                tlag <= d - $signed({59'h0, n, 4'h0});
                presc <= pr; counter <= ct; timer_cycles <= tc;
                tcont <= 1'b1;
                if (!(d > 65'sd48)) begin            // caught up: timer_cycles >= cy now
                    // SM_UpdateUART
                    if (dm[D_U1_CTRL][2] && rx_avail && !gotbyte && !(cy < rx_delay)) begin
                        rx_byte <= rx_data;
                        rx_pop <= 1'b1;
                        gotbyte <= 1'b1;
                        ir = ir | 8'h40;
                        rx_delay <= cy + 64'd12000;
                    end
                    tcont <= 1'b0;
                    cyc48 <= cy + 64'd48;
                    tlag48 <= d - $signed({59'h0, n, 4'h0}) + 65'sd48;
                    // the next iteration (Q_LOOP's work) with the request register as now updated
                    if (!(cy < target5)) begin
                        done <= 1'b1;
                        q <= Q_IDLE;
                    end else begin : b_next
                        logic [13:0] iv;
                        iv = int_eval(sr[F_I], dm[D_U1_CTRL], dm[D_U2_CTRL], dm[D_U3_CTRL], dm[D_INT_EN], ir, dm[D_TCTRL], dm[D_COLL]);
                        if (iv[13]) begin
                            if (iv[0]) dm[D_COLL][7] <= 1'b0;
                            else ir = ir & ~iv[8:1];
                            npc <= 16'h1fec + {11'h0, iv[12:9], 1'b0};
                            pcnt <= 2'd0;
                            q <= Q_PUSH;
                        end else if (sleep) begin
                            q <= Q_TIMER;                 // a sleeping iteration: cycles += 48 next clock
                        end else if (pc[12]) begin
                            issue_read(pc);               // the opcode, from ROM
                            pc <= pc + 16'd1;
                            q <= Q_OPC;
                        end else begin
                            q <= Q_LOOP;                  // code outside the ROM: fetch with its side effects in order
                        end
                    end
                end
                dm[D_INT_REQ] <= ir;
            end

            default: q <= Q_IDLE;
            endcase
        end
    end

    // the operation with the operand (data, or the immediate byte), A-side value av (A, or mem[X] in T mode)
    task automatic exec(input logic [7:0] d, input logic [7:0] av);
        logic [7:0] r;
        logic [8:0] df;
        case (kd)
            K_LDA: if (t_mode) do_write({8'h00, x}, d); else begin a <= d; sr <= nz(sr, d); end
            K_LDX: begin x <= d; sr <= nz(sr, d); end
            K_LDY: begin y <= d; sr <= nz(sr, d); end
            K_ORA, K_AND: begin
                r = (kd == K_ORA) ? (av | d) : (av & d);
                if (t_mode) do_write({8'h00, x}, r); else begin a <= r; sr <= nz(sr, r); end
            end
            K_CMP, K_CPX, K_CPY: begin
                df = {1'b0, (kd == K_CMP) ? a : (kd == K_CPX) ? x : y} - {1'b0, d};
                r = nz(sr, df[7:0]);
                r[F_C] = !df[8];
                sr <= r;
            end
            K_INC: begin do_write(ea, d + 8'd1); sr <= nz(sr, d + 8'd1); end
            K_DEC: begin do_write(ea, d - 8'd1); sr <= nz(sr, d - 8'd1); end
            K_BB:  if (d[opc[7:5]] != opc[4]) pc <= pc + {{8{b2[7]}}, b2};
            K_SEB: begin
                r = d;
                r[opc[7:5]] = !opc[4];
                do_write(ea, r);
            end
            default: ;
        endcase
    endtask

    assign st_pc = pc; assign st_a = a; assign st_x = x; assign st_y = y; assign st_s = s; assign st_sr = sr;
    assign st_cycles = cycles; assign st_sleep = sleep;
    assign st_p0_data = p0_data; assign st_p0_dir = p0_dir; assign st_p1_dir = p1_dir; assign st_p1_data = p1_data;
    assign st_prescaler = presc; assign st_counter = counter;
    assign st_timer_cycles = timer_cycles; assign st_rx_delay = rx_delay; assign st_gotbyte = gotbyte;
    assign st_instructions = ninstr;
endmodule
