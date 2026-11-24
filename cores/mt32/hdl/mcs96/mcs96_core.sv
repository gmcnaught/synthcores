// SPDX-License-Identifier: MIT
// MCS-96 (8098) instruction engine, written from MAME's mcs96 core as Nuked-MT32 uses it
// (modules/nuked-mt32/mame: mcs96.cpp, mcs96.hxx generated from mcs96ops.lst; Olivier Galibert,
// BSD-3-Clause; see NOTICE).
//
// One `exec` pulse runs one MAME "state": a fetch (interrupt entry, opcode, FE prefix) or the
// handler of the fetched opcode, with every register-file and bus access in MAME's order. `done`
// then reports the handler's next() count in `ncyc` (CPU cycles; the scheduler multiplies by
// cycles_scaling = 3). A fetch reports 0, as MAME's fetch_full() calls no next().
//
// MAME behaviour kept on purpose (from the compiled handler bodies in mcs96.hxx, which differ from
// mcs96ops.lst in norml, mulub indexed_3e and the div/divu dividend):
//   - sub indirect_3w (4A) uses post_indexed: no auto-increment, cycles 7/8 by OPI bit 0;
//   - mulb direct_2b (FE 7C) reads reg_r16(OP2); mulb indexed_2b (FE 7F) reads any_r16(OP1);
//   - div's remainder is the unsigned TMP % (uint32) int16(d); divb's is signed;
//   - indexed modes clear OPI bit 0 before post_indexed, so they always take the first count
//     (divb indexed: 1 cycle);
//   - push indirect auto-increments the pointer by the pushed value + 2 (OP1 was reloaded);
//   - interrupt entry takes no cycles and leaves PSW alone.
// The reference has empty handlers for xch/xchb and none for the 8x196-only opcodes; it would
// loop forever on them. Here `bad_op` goes high and the opcode acts as a 4-cycle no-op.
//
// SFRs 08 (INT_MASK = PSW low byte) and 09 (INT_PENDING) live here, as in MAME's mcs96_device;
// their accesses still go out on the port (the peripheral block acks them without effect) so
// the trace sees every register write.
module mcs96_core (
    input  logic        clk,
    input  logic        rst,

    input  logic        exec,          // run one state
    output logic        done,          // pulse: the state is finished
    output logic [6:0]  ncyc,          // its next() count (0 for a fetch)
    output logic        bad_op,        // level: an opcode the reference has no handler for

    // byte accesses in MAME's order; space 0 = register file / SFRs (reg_r8 / reg_w8),
    // 1 = external bus (mt32_t::cpu_read / cpu_write). Hold req until ack (a one-clock pulse),
    // then drop it for at least one clock.
    output logic        m_req,
    output logic        m_space,
    output logic        m_we,
    output logic [15:0] m_addr,
    output logic [7:0]  m_wdata,
    input  logic [7:0]  m_rdata,
    input  logic        m_ack,

    input  logic [7:0]  irq_set,       // interrupt-pending bits from the peripherals (pulses)

    output logic [15:0] pc,
    output logic [15:0] psw,
    output logic        dbg_fetch,     // pulse: instruction fetch at dbg_fpc (after any interrupt entry)
    output logic [15:0] dbg_fpc,
    output logic        dbg_irq,       // pulse: interrupt taken (dbg_level; pc = return address)
    output logic [2:0]  dbg_level
);
`include "mcs96_decode.svh"

    localparam int F_ST = 8, F_I = 9, F_C = 11, F_VT = 12, F_V = 13, F_N = 14, F_Z = 15;

`ifndef MCS96_DECODE_HEX
`define MCS96_DECODE_HEX "mcs96_decode.hex"
`endif
    logic [31:0] dec_rom [512];
    initial $readmemh(`MCS96_DECODE_HEX, dec_rom);

    // ---------------------------------------------------------------- state
    logic [15:0] PC, PSW, OP1;
    logic [7:0]  OP2, OP3, OPI, pending;
    logic [31:0] TMP;
    logic [8:0]  ist;          // inst_state of the fetched opcode (FE prefix = bit 8)
    logic        need_fetch;   // the next state is a fetch
    logic        fetch_noirq;  // ... and it is fetch_noirq
    assign pc = PC;
    assign psw = PSW;

    wire irq_req = (|(PSW[7:0] & pending)) && PSW[F_I];

    logic [31:0] dw;
    wire        d_valid = dw[31];
    wire        d_noirq = dw[30];
    wire [5:0]  d_ceven = dw[29:24];
    wire [5:0]  d_codd  = dw[23:18];
    wire [3:0]  d_am    = dw[17:14];
    wire [1:0]  d_n     = dw[13:12];
    wire        d_immw  = dw[11];
    wire [6:0]  d_mn    = dw[10:4];

    // ---------------------------------------------------------------- memory helper
    localparam logic [1:0] K_R8 = 2'd0, K_R16 = 2'd1, K_W8 = 2'd2, K_W16 = 2'd3;
    localparam logic [1:0] SP_REG = 2'd0, SP_EXT = 2'd1, SP_ANY = 2'd2;

    typedef enum logic [3:0] { S_IDLE, S_FETCH, S_DEC, S_EA, S_EAT, S_EX, S_POST, S_DIV, S_MEM,
                               S_FIN } st_t;
    st_t st, ret_st;
    logic [3:0]  ph, ret_ph;

    logic [1:0]  mk;
    logic [15:0] ma, mwd, md;
    logic        mbyte, msp, m_go;

    assign m_req   = m_go;
    assign m_space = msp;
    assign m_we    = mk[1];
    assign m_addr  = mbyte ? (ma | 16'h0001) : ma;
    assign m_wdata = mbyte ? mwd[15:8] : mwd[7:0];

    wire m16 = mk[0];
    wire m_sfr = !msp && (m_addr[15:1] == 15'h0004);          // register 08 or 09

    // ---------------------------------------------------------------- flag helpers
    function automatic logic [15:0] nz(input logic [15:0] p, input logic [15:0] v, input logic w);
        logic [15:0] r;
        r = p & ~16'hE800;                                     // ~(Z|N|V|C)
        if (w ? (v == 16'h0) : (v[7:0] == 8'h0)) r[F_Z] = 1'b1;
        else if (w ? v[15] : v[7]) r[F_N] = 1'b1;
        return r;
    endfunction

    function automatic logic [31:0] f_add(input logic [15:0] p, input logic [15:0] a, input logic [15:0] b,
                                          input logic cin, input logic w);
        logic [16:0] s;
        logic [15:0] r;
        if (w) s = {1'b0, a} + {1'b0, b} + {16'h0, cin};
        else   s = {9'h0, a[7:0]} + {9'h0, b[7:0]} + {16'h0, cin};
        r = p & ~16'hE800;
        if (w ? (s[15:0] == 16'h0) : (s[7:0] == 8'h0)) r[F_Z] = 1'b1;
        else if (w ? s[15] : s[7]) r[F_N] = 1'b1;
        if (w ? (~(a[15] ^ b[15]) & (a[15] ^ s[15])) : (~(a[7] ^ b[7]) & (a[7] ^ s[7]))) begin
            r[F_V] = 1'b1;
            r[F_VT] = 1'b1;
        end
        if (w ? s[16] : s[8]) r[F_C] = 1'b1;
        return {r, w ? s[15:0] : {8'h00, s[7:0]}};
    endfunction

    function automatic logic [31:0] f_sub(input logic [15:0] p, input logic [15:0] a, input logic [15:0] b,
                                          input logic bin, input logic w);
        logic [16:0] d;
        logic [15:0] r;
        if (w) d = {1'b0, a} - {1'b0, b} - {16'h0, bin};
        else   d = {9'h0, a[7:0]} - {9'h0, b[7:0]} - {16'h0, bin};
        r = p & ~16'hE800;
        if (w ? (d[15:0] == 16'h0) : (d[7:0] == 8'h0)) r[F_Z] = 1'b1;
        else if (w ? d[15] : d[7]) r[F_N] = 1'b1;
        if (w ? ((a[15] ^ b[15]) & (a[15] ^ d[15])) : ((a[7] ^ b[7]) & (a[7] ^ d[7]))) r[F_V] = 1'b1;
        if (!(w ? d[16] : d[8])) r[F_C] = 1'b1;
        return {r, w ? d[15:0] : {8'h00, d[7:0]}};
    endfunction

    function automatic logic [31:0] alu(input logic [6:0] mn, input logic [15:0] p,
                                        input logic [15:0] a, input logic [15:0] b);
        logic [15:0] r;
        case (mn)
            MN_AND:   begin r = a & b; return {nz(p, r, 1'b1), r}; end
            MN_ANDB:  begin r = a & b; return {nz(p, r, 1'b0), 8'h00, r[7:0]}; end
            MN_OR:    begin r = a | b; return {nz(p, r, 1'b1), r}; end
            MN_ORB:   begin r = a | b; return {nz(p, r, 1'b0), 8'h00, r[7:0]}; end
            MN_XOR:   begin r = a ^ b; return {nz(p, r, 1'b1), r}; end
            MN_XORB:  begin r = a ^ b; return {nz(p, r, 1'b0), 8'h00, r[7:0]}; end
            MN_ADD:   return f_add(p, a, b, 1'b0, 1'b1);
            MN_ADDB:  return f_add(p, a, b, 1'b0, 1'b0);
            MN_ADDC:  return f_add(p, a, b, p[F_C], 1'b1);
            MN_ADDCB: return f_add(p, a, b, p[F_C], 1'b0);
            MN_SUB, MN_CMP:   return f_sub(p, a, b, 1'b0, 1'b1);
            MN_SUBB, MN_CMPB: return f_sub(p, a, b, 1'b0, 1'b0);
            MN_SUBC:  return f_sub(p, a, b, !p[F_C], 1'b1);
            MN_SUBCB: return f_sub(p, a, b, !p[F_C], 1'b0);
            default:  return {p, 16'h0};
        endcase
    endfunction

    function automatic logic jcond(input logic [6:0] mn, input logic [15:0] p);
        case (mn)
            MN_JNST: return !p[F_ST];
            MN_JNH:  return {p[F_C], p[F_Z]} != 2'b10;
            MN_JGT:  return !(p[F_Z] | p[F_N]);
            MN_JNC:  return !p[F_C];
            MN_JNVT: return !p[F_VT];
            MN_JNV:  return !p[F_V];
            MN_JGE:  return !p[F_N];
            MN_JNE:  return !p[F_Z];
            MN_JST:  return p[F_ST];
            MN_JH:   return {p[F_C], p[F_Z]} == 2'b10;
            MN_JLE:  return p[F_Z] | p[F_N];
            MN_JC:   return p[F_C];
            MN_JVT:  return p[F_VT];
            MN_JV:   return p[F_V];
            MN_JLT:  return p[F_N];
            default: return p[F_Z];                            // MN_JE
        endcase
    endfunction

    // shifts: {psw, result}; t = operand (zero-extended), n = count 0..31
    function automatic logic [47:0] shift(input logic [6:0] mn, input logic [15:0] p,
                                          input logic [31:0] t, input logic [4:0] n);
        logic [15:0] r;
        logic [31:0] o, m;
        logic [5:0]  n6;
        n6 = {1'b0, n};
        r = p & ~16'hE900;                                     // ~(Z|N|C|V|ST)
        o = 32'h0;
        case (mn)
            MN_SHR, MN_SHRA: begin
                m = 32'h0000ffff >> ((n <= 5'd16) ? (6'd17 - n6) : 6'd0);
                if (n >= 5'd2 && (t & m) != 0) r[F_ST] = 1'b1;
                if (n >= 5'd1 && n <= 5'd16 && t[n - 5'd1]) r[F_C] = 1'b1;
                if (mn == MN_SHR) o = {16'h0, t[15:0]} >> n;
                else              o = $unsigned($signed({{16{t[15]}}, t[15:0]}) >>> n);
                if (o == 32'h0) r[F_Z] = 1'b1; else if (o[15]) r[F_N] = 1'b1;
            end
            MN_SHL: begin
                m = 32'h0000ffff << ((n <= 5'd16) ? (6'd17 - n6) : 6'd0);
                if (n >= 5'd2 && (t & m) != 0) r[F_ST] = 1'b1;
                if (n >= 5'd1 && n <= 5'd16 && t[6'd16 - n6]) r[F_C] = 1'b1;
                o = {16'h0, t[15:0]} << n;
                o = {16'h0, o[15:0]};
                if (o == 32'h0) r[F_Z] = 1'b1; else if (o[15]) r[F_N] = 1'b1;
            end
            MN_SHRB, MN_SHRAB: begin
                m = 32'h000000ff >> ((n <= 5'd8) ? (6'd9 - n6) : 6'd0);
                if (n >= 5'd2 && (t & m) != 0) r[F_ST] = 1'b1;
                if (n >= 5'd1 && n <= 5'd8 && t[n - 5'd1]) r[F_C] = 1'b1;
                if (mn == MN_SHRB) o = {24'h0, t[7:0]} >> n;
                else               o = $unsigned($signed({{24{t[7]}}, t[7:0]}) >>> n);
                o = {24'h0, o[7:0]};
                if (o[7:0] == 8'h0) r[F_Z] = 1'b1; else if (o[7]) r[F_N] = 1'b1;
            end
            MN_SHLB: begin
                m = 32'h000000ff << ((n <= 5'd8) ? (6'd9 - n6) : 6'd0);
                if (n >= 5'd2 && (t & m) != 0) r[F_ST] = 1'b1;
                if (n >= 5'd1 && n <= 5'd8 && t[6'd8 - n6]) r[F_C] = 1'b1;
                o = {24'h0, t[7:0]} << n;
                o = {24'h0, o[7:0]};
                if (o[7:0] == 8'h0) r[F_Z] = 1'b1; else if (o[7]) r[F_N] = 1'b1;
            end
            MN_SHRL, MN_SHRAL: begin
                m = 32'hffffffff >> (6'd33 - n6);
                if (n >= 5'd2 && (t & m) != 0) r[F_ST] = 1'b1;
                if (n >= 5'd1 && t[n - 5'd1]) r[F_C] = 1'b1;
                if (mn == MN_SHRL) o = t >> n;
                else               o = $unsigned($signed(t) >>> n);
                if (o == 32'h0) r[F_Z] = 1'b1; else if (o[31]) r[F_N] = 1'b1;
            end
            default: begin                                     // MN_SHLL
                m = 32'hffffffff << (6'd33 - n6);
                if (n >= 5'd2 && (t & m) != 0) r[F_ST] = 1'b1;
                if (n >= 5'd1 && t[6'd32 - n6]) r[F_C] = 1'b1;
                o = t << n;
                if (o == 32'h0) r[F_Z] = 1'b1; else if (o[31]) r[F_N] = 1'b1;
            end
        endcase
        return {r, o};
    endfunction

    // ---------------------------------------------------------------- groups
    wire mn_byte = (d_mn == MN_ANDB || d_mn == MN_ADDB || d_mn == MN_SUBB || d_mn == MN_ORB ||
                    d_mn == MN_XORB || d_mn == MN_ADDCB || d_mn == MN_SUBCB || d_mn == MN_CMPB ||
                    d_mn == MN_LDB || d_mn == MN_STB || d_mn == MN_LDBZE || d_mn == MN_LDBSE ||
                    d_mn == MN_MULUB || d_mn == MN_MULB || d_mn == MN_DIVUB || d_mn == MN_DIVB);
    wire g_alu = (d_mn == MN_AND || d_mn == MN_ANDB || d_mn == MN_ADD || d_mn == MN_ADDB ||
                  d_mn == MN_SUB || d_mn == MN_SUBB || d_mn == MN_OR || d_mn == MN_ORB ||
                  d_mn == MN_XOR || d_mn == MN_XORB || d_mn == MN_ADDC || d_mn == MN_ADDCB ||
                  d_mn == MN_SUBC || d_mn == MN_SUBCB || d_mn == MN_CMP || d_mn == MN_CMPB);
    wire g_cmp = (d_mn == MN_CMP || d_mn == MN_CMPB);
    wire g_mul = (d_mn == MN_MULU || d_mn == MN_MUL || d_mn == MN_MULUB || d_mn == MN_MULB);
    wire g_mulw = (d_mn == MN_MULU || d_mn == MN_MUL);
    wire g_muls = (d_mn == MN_MUL || d_mn == MN_MULB);
    wire g_divw = (d_mn == MN_DIVU || d_mn == MN_DIV);
    wire g_divb = (d_mn == MN_DIVUB || d_mn == MN_DIVB);
    wire g_ld  = (d_mn == MN_LD || d_mn == MN_LDB || d_mn == MN_LDBZE || d_mn == MN_LDBSE);
    wire g_st  = (d_mn == MN_ST || d_mn == MN_STB);
    wire g_un  = (d_mn == MN_NOT || d_mn == MN_NOTB || d_mn == MN_NEG || d_mn == MN_NEGB ||
                  d_mn == MN_DEC || d_mn == MN_DECB || d_mn == MN_INC || d_mn == MN_INCB);
    wire g_unw = (d_mn == MN_NOT || d_mn == MN_NEG || d_mn == MN_DEC || d_mn == MN_INC);
    wire g_sh  = (d_mn == MN_SHR || d_mn == MN_SHRA || d_mn == MN_SHL || d_mn == MN_SHRB ||
                  d_mn == MN_SHRAB || d_mn == MN_SHLB);
    wire g_shb = (d_mn == MN_SHRB || d_mn == MN_SHRAB || d_mn == MN_SHLB);
    wire g_shl = (d_mn == MN_SHRL || d_mn == MN_SHRAL || d_mn == MN_SHLL);
    wire g_jcc = (d_mn == MN_JNST || d_mn == MN_JNH || d_mn == MN_JGT || d_mn == MN_JNC ||
                  d_mn == MN_JNVT || d_mn == MN_JNV || d_mn == MN_JGE || d_mn == MN_JNE ||
                  d_mn == MN_JST || d_mn == MN_JH || d_mn == MN_JLE || d_mn == MN_JC ||
                  d_mn == MN_JVT || d_mn == MN_JV || d_mn == MN_JLT || d_mn == MN_JE);

    wire am_d  = (d_am == AM_DIRECT);
    wire am_i  = (d_am == AM_IMMED);
    wire am_m  = (d_am == AM_INDIRECT || d_am == AM_INDEXED);
    wire [5:0] cyc_post = OPI[0] ? d_codd : d_ceven;

    // the reading size of the memory/source operand
    wire src16 = !mn_byte || (ist == 9'h17f);                  // mulb indexed_2b reads any_r16
    // post_indirect's auto-increment
    wire autoinc = (d_am == AM_INDIRECT) && OPI[0] && (ist[7:0] != 8'h4a);   // 4A and FE 4A
    wire [15:0] inc_size = mn_byte ? 16'd1 : 16'd2;

    // ---------------------------------------------------------------- multiplier, divider
    logic signed [16:0] mul_a, mul_b;
    wire  signed [33:0] mul_p = mul_a * mul_b;

    logic [31:0] dv_n, dv_d, dv_q, dv_r;
    logic [5:0]  dv_i;
    logic [31:0] dvq_s;        // signed quotient kept across a second division

    logic [15:0] src;           // source operand value
    logic [15:0] sp_t;
    logic        in_handler;   // the state being run is a handler, not a fetch

    // a register-file or bus access; returns to (ret, rph) with the data in md
    task automatic mem(input logic [1:0] k, input logic [1:0] s, input logic [15:0] a,
                       input logic [15:0] wd, input st_t ret, input logic [3:0] rph);
        logic sp;
        logic [15:0] aa;
        sp = (s == SP_EXT) || ((s == SP_ANY) && ((k[0] ? (a & 16'hfffe) : a) >= 16'h0100));
        if (k[0]) aa = sp ? (a & 16'hfffe) : {8'h00, a[7:1], 1'b0};
        else      aa = sp ? a : {8'h00, a[7:0]};
        mk <= k; ma <= aa; mwd <= wd; msp <= sp; mbyte <= 1'b0;
        ret_st <= ret; ret_ph <= rph;
        st <= S_MEM;
    endtask

    // read the next byte at PC
    task automatic rdpc(input st_t ret, input logic [3:0] rph);
        mem(K_R8, SP_EXT, PC, 16'h0, ret, rph);
        PC <= PC + 16'd1;
    endtask

    task automatic finish(input logic [6:0] n);
        ncyc <= n;
        st <= S_FIN;
    endtask

    logic [2:0] lvl;
    always_comb begin
        lvl = 3'd0;
        for (int i = 0; i < 8; i++)
            if (PSW[i] && pending[i]) lvl = 3'(i);
    end

    wire [7:0] dst = (d_n == 2'd3) ? OP3 : OP2;

    always_ff @(posedge clk) begin
        logic [31:0] ar;
        logic [47:0] sr;
        logic [15:0] t16;
        done <= 1'b0;
        dbg_fetch <= 1'b0;
        dbg_irq <= 1'b0;
        if (rst) begin
            st <= S_IDLE;
            PC <= 16'h2080;
            PSW <= 16'h0;
            pending <= 8'h0;
            need_fetch <= 1'b1;
            fetch_noirq <= 1'b0;
            ist <= 9'h0;
            m_go <= 1'b0;
            bad_op <= 1'b0;
            OP1 <= 16'h0; OP2 <= 8'h0; OP3 <= 8'h0; OPI <= 8'h0; TMP <= 32'h0;
            ph <= 4'd0;
            dw <= 32'h0;
            ncyc <= 7'd0;
            in_handler <= 1'b0;
        end else begin
            pending <= pending | irq_set;
            case (st)
            S_IDLE: if (exec) begin
                ph <= 4'd0;
                in_handler <= !need_fetch;
                st <= need_fetch ? S_FETCH : S_DEC;
            end

            // ------------------------------------------------ fetch_full / fetch_noirq
            S_FETCH: case (ph)
                4'd0: if (!fetch_noirq && irq_req) begin
                          pending <= (pending | irq_set) & ~(8'd1 << lvl);
                          OP1 <= {13'h0, lvl};
                          dbg_level <= lvl;
                          dbg_irq <= 1'b1;
                          mem(K_R16, SP_REG, 16'h0018, 16'h0, S_FETCH, 4'd1);
                      end else ph <= 4'd4;
                4'd1: begin sp_t <= md - 16'd2; mem(K_W16, SP_REG, 16'h0018, md - 16'd2, S_FETCH, 4'd2); end
                4'd2: mem(K_W16, SP_ANY, sp_t, PC, S_FETCH, 4'd3);
                4'd3: mem(K_R16, SP_ANY, 16'h2000 + {12'h0, OP1[2:0], 1'b0}, 16'h0, S_FETCH, 4'd5);
                4'd5: begin PC <= md; ph <= 4'd4; end
                4'd4: begin dbg_fetch <= 1'b1; dbg_fpc <= PC; rdpc(S_FETCH, 4'd6); end
                4'd6: if (md[7:0] == 8'hfe) rdpc(S_FETCH, 4'd7);
                      else begin ist <= {1'b0, md[7:0]}; need_fetch <= 1'b0; finish(7'd0); end
                4'd7: begin ist <= {1'b1, md[7:0]}; need_fetch <= 1'b0; finish(7'd0); end
                default: ;
            endcase

            S_DEC: begin dw <= dec_rom[ist]; st <= S_EA; end

            // ------------------------------------------------ operand fetch (eadr)
            S_EA: if (!d_valid) begin
                bad_op <= 1'b1;
                finish(7'd4);
            end else case (d_am)
                AM_NONE: begin st <= S_EX; ph <= 4'd0; end
                AM_DIRECT, AM_IMMED: case (ph)
                    4'd0: rdpc(S_EA, 4'd1);
                    4'd1: begin
                        OP1 <= {8'h00, md[7:0]};
                        if (am_i && d_immw) rdpc(S_EA, 4'd2);
                        else begin st <= S_EAT; ph <= 4'd0; end
                    end
                    4'd2: begin OP1[15:8] <= md[7:0]; st <= S_EAT; ph <= 4'd0; end
                    default: ;
                endcase
                AM_INDIRECT, AM_INDIRECT_1N: case (ph)
                    4'd0: rdpc(S_EA, 4'd1);
                    4'd1: begin
                        OPI <= (d_am == AM_INDIRECT_1N) ? (md[7:0] & 8'hfe) : md[7:0];
                        mem(K_R16, SP_REG, {8'h00, md[7:0]}, 16'h0, S_EA, 4'd2);
                    end
                    4'd2: begin OP1 <= md; st <= S_EAT; ph <= 4'd0; end
                    default: ;
                endcase
                AM_INDEXED: case (ph)
                    4'd0: rdpc(S_EA, 4'd1);
                    4'd1: begin OPI <= md[7:0]; rdpc(S_EA, 4'd2); end
                    4'd2: if (OPI[0]) begin
                              OPI <= OPI & 8'hfe;
                              OP1 <= {8'h00, md[7:0]};
                              rdpc(S_EA, 4'd3);
                          end else begin
                              OP1 <= {{8{md[7]}}, md[7:0]};
                              ph <= 4'd4;
                          end
                    4'd3: begin OP1[15:8] <= md[7:0]; ph <= 4'd4; end
                    4'd4: if (OPI != 8'h00) mem(K_R16, SP_REG, {8'h00, OPI}, 16'h0, S_EA, 4'd5);
                          else begin st <= S_EAT; ph <= 4'd0; end
                    4'd5: begin OP1 <= OP1 + md; st <= S_EAT; ph <= 4'd0; end
                    default: ;
                endcase
                AM_IMMED_OR_REG: case (ph)
                    4'd0: rdpc(S_EA, 4'd1);
                    4'd1: begin OP1 <= {8'h00, md[7:0]}; rdpc(S_EA, 4'd2); end
                    4'd2: begin
                        OP2 <= md[7:0];
                        if (OP1 >= 16'h0010) mem(K_R8, SP_REG, OP1, 16'h0, S_EA, 4'd3);
                        else begin st <= S_EX; ph <= 4'd0; end
                    end
                    4'd3: begin OP1 <= {11'h0, md[4:0]}; st <= S_EX; ph <= 4'd0; end
                    default: ;
                endcase
                AM_REL8: case (ph)
                    4'd0: rdpc(S_EA, 4'd1);
                    4'd1: begin OP1 <= {{8{md[7]}}, md[7:0]}; st <= S_EX; ph <= 4'd0; end
                    default: ;
                endcase
                AM_REL11: case (ph)
                    4'd0: rdpc(S_EA, 4'd1);
                    4'd1: begin OP1 <= {{5{ist[2]}}, ist[2:0], md[7:0]}; st <= S_EX; ph <= 4'd0; end
                    default: ;
                endcase
                AM_REL16: case (ph)
                    4'd0: rdpc(S_EA, 4'd1);
                    4'd1: begin OP1 <= {8'h00, md[7:0]}; rdpc(S_EA, 4'd2); end
                    4'd2: begin OP1[15:8] <= md[7:0]; st <= S_EX; ph <= 4'd0; end
                    default: ;
                endcase
                default: case (ph)                             // AM_RREL8
                    4'd0: rdpc(S_EA, 4'd1);
                    4'd1: begin OP2 <= md[7:0]; rdpc(S_EA, 4'd2); end
                    4'd2: begin OP1 <= {{8{md[7]}}, md[7:0]}; st <= S_EX; ph <= 4'd0; end
                    default: ;
                endcase
            endcase

            // OP2 and OP3 bytes of the 2- and 3-operand modes
            S_EAT: case (ph)
                4'd0: if (d_n >= 2'd2) rdpc(S_EAT, 4'd1); else begin st <= S_EX; ph <= 4'd0; end
                4'd1: begin
                    OP2 <= md[7:0];
                    if (d_n == 2'd3) rdpc(S_EAT, 4'd2); else begin st <= S_EX; ph <= 4'd0; end
                end
                4'd2: begin OP3 <= md[7:0]; st <= S_EX; ph <= 4'd0; end
                default: ;
            endcase

            // ------------------------------------------------ execute
            S_EX:
            if (g_alu) case (ph)
                4'd0: if (am_i) begin src <= OP1; ph <= 4'd2; end
                      else mem(mn_byte ? K_R8 : K_R16, am_d ? SP_REG : SP_ANY, OP1, 16'h0, S_EX, 4'd1);
                4'd1: begin src <= md; ph <= 4'd2; end
                4'd2: mem(mn_byte ? K_R8 : K_R16, SP_REG, {8'h00, OP2}, 16'h0, S_EX, 4'd3);
                4'd3: begin
                    ar = alu(d_mn, PSW, md, src);
                    PSW <= ar[31:16];
                    if (g_cmp) begin st <= S_POST; ph <= 4'd0; end
                    else mem(mn_byte ? K_W8 : K_W16, SP_REG, {8'h00, dst}, ar[15:0], S_POST, 4'd0);
                end
                default: ;
            endcase
            else if (g_mul) case (ph)
                4'd0: begin
                    if (d_n == 2'd2) OP2 <= OP2 & (g_mulw ? 8'hfc : 8'hfe);
                    if (am_i) begin src <= OP1; ph <= 4'd2; end
                    else mem(src16 ? K_R16 : K_R8, am_d ? SP_REG : SP_ANY, OP1, 16'h0, S_EX, 4'd1);
                end
                4'd1: begin src <= md; ph <= 4'd2; end
                4'd2: mem((g_mulw || ist == 9'h17c) ? K_R16 : K_R8, SP_REG, {8'h00, OP2}, 16'h0, S_EX, 4'd3);
                4'd3: begin
                    if (g_mulw) begin
                        mul_a <= g_muls ? {md[15], md} : {1'b0, md};
                        mul_b <= g_muls ? {src[15], src} : {1'b0, src};
                    end else begin
                        mul_a <= g_muls ? {{9{md[7]}}, md[7:0]} : {9'h0, md[7:0]};
                        mul_b <= g_muls ? {{9{src[7]}}, src[7:0]} : {9'h0, src[7:0]};
                    end
                    ph <= 4'd4;
                end
                4'd4: begin
                    TMP <= mul_p[31:0];
                    ph <= 4'd5;
                end
                4'd5: if (g_mulw) mem(K_W16, SP_REG, {8'h00, (d_n == 2'd3) ? (OP3 & 8'hfc) : OP2}, TMP[15:0], S_EX, 4'd6);
                      else mem(K_W16, SP_REG, {8'h00, dst}, TMP[15:0], S_POST, 4'd0);
                4'd6: mem(K_W16, SP_REG, {8'h00, ((d_n == 2'd3) ? (OP3 & 8'hfc) : OP2) + 8'd2}, TMP[31:16], S_POST, 4'd0);
                default: ;
            endcase
            else if (g_divw) case (ph)
                4'd0: begin
                    OP2 <= OP2 & 8'hfc;
                    PSW[F_V] <= 1'b0;
                    if (am_i) begin src <= OP1; ph <= 4'd2; end
                    else mem(K_R16, am_d ? SP_REG : SP_ANY, OP1, 16'h0, S_EX, 4'd1);
                end
                4'd1: begin src <= md; if (am_d) OP1 <= md; ph <= 4'd2; end
                4'd2: if (src == 16'h0) begin st <= S_POST; ph <= 4'd0; end
                      else mem(K_R16, SP_REG, {8'h00, OP2}, 16'h0, S_EX, 4'd3);
                4'd3: begin TMP <= {16'h0, md}; mem(K_R16, SP_REG, {8'h00, OP2 + 8'd2}, 16'h0, S_EX, 4'd4); end
                4'd4: begin
                    // TMP = reg_r16(OP2) | reg_r16(OP2+2) << 16
                    logic [31:0] t;
                    t = {md, TMP[15:0]};
                    TMP <= t;
                    if (d_mn == MN_DIV) begin
                        dv_n <= t[31] ? -t : t;                        // |int32(T)|
                        dv_d <= src[15] ? {16'h0, -src} : {16'h0, src};
                    end else begin
                        dv_n <= t;
                        dv_d <= {16'h0, src};
                    end
                    dv_i <= 6'd0; dv_q <= 32'h0; dv_r <= 32'h0;
                    st <= S_DIV; ret_ph <= 4'd5;
                end
                4'd5: begin
                    if (d_mn == MN_DIVU) begin
                        if (dv_q > 32'd65535) begin PSW[F_V] <= 1'b1; PSW[F_VT] <= 1'b1; end
                        dvq_s <= dv_q;
                        TMP <= {dv_r[15:0], dv_q[15:0]};
                        ph <= 4'd6;
                    end else begin
                        // TMP2 = int32(T) / int16(d), truncated
                        logic [31:0] q;
                        q = (TMP[31] ^ src[15]) ? -dv_q : dv_q;
                        if ($signed(q) > 32'sd32767 || $signed(q) < -32'sd32768) begin
                            PSW[F_V] <= 1'b1; PSW[F_VT] <= 1'b1;
                        end
                        dvq_s <= q;
                        if (src[15]) begin
                            // T % (2**32 - |d|): T, or T - (2**32 - |d|) when T is at least that
                            logic [31:0] dd;
                            dd = {{16{1'b1}}, src};
                            TMP <= {(TMP >= dd) ? 16'(TMP - dd) : TMP[15:0], q[15:0]};
                            ph <= 4'd6;
                        end else if (TMP[31]) begin
                            // unsigned T % d: a second division
                            dv_n <= TMP; dv_d <= {16'h0, src};
                            dv_i <= 6'd0; dv_q <= 32'h0; dv_r <= 32'h0;
                            st <= S_DIV; ret_ph <= 4'd8;
                        end else begin
                            TMP <= {dv_r[15:0], q[15:0]};
                            ph <= 4'd6;
                        end
                    end
                end
                4'd8: begin TMP <= {dv_r[15:0], dvq_s[15:0]}; ph <= 4'd6; end
                4'd6: mem(K_W16, SP_REG, {8'h00, OP2}, TMP[15:0], S_EX, 4'd7);
                4'd7: mem(K_W16, SP_REG, {8'h00, OP2 + 8'd2}, TMP[31:16], S_POST, 4'd0);
                default: ;
            endcase
            else if (g_divb) case (ph)
                4'd0: begin
                    PSW[F_V] <= 1'b0;
                    if (am_i) begin src <= {8'h00, OP1[7:0]}; ph <= 4'd2; end
                    else mem(K_R8, am_d ? SP_REG : SP_ANY, OP1, 16'h0, S_EX, 4'd1);
                end
                4'd1: begin src <= {8'h00, md[7:0]}; if (am_d) OP1 <= {8'h00, md[7:0]}; ph <= 4'd2; end
                4'd2: if (src[7:0] == 8'h0) begin st <= S_POST; ph <= 4'd0; end
                      else mem(K_R16, SP_REG, {8'h00, OP2}, 16'h0, S_EX, 4'd3);
                4'd3: begin
                    TMP <= {16'h0, md};
                    if (d_mn == MN_DIVUB) begin
                        dv_n <= {16'h0, md}; dv_d <= {24'h0, src[7:0]};
                    end else begin
                        dv_n <= md[15] ? {16'h0, -md} : {16'h0, md};
                        dv_d <= src[7] ? {24'h0, -src[7:0]} : {24'h0, src[7:0]};
                    end
                    dv_i <= 6'd0; dv_q <= 32'h0; dv_r <= 32'h0;
                    st <= S_DIV; ret_ph <= 4'd4;
                end
                4'd4: begin
                    if (d_mn == MN_DIVUB) begin
                        if (dv_q > 32'd255) begin PSW[F_V] <= 1'b1; PSW[F_VT] <= 1'b1; end
                        TMP <= {16'h0, dv_r[7:0], dv_q[7:0]};
                    end else begin
                        logic [31:0] q, r;
                        logic [15:0] q16;
                        q = (TMP[15] ^ src[7]) ? -dv_q : dv_q;
                        r = TMP[15] ? -dv_r : dv_r;
                        q16 = q[15:0];
                        if ($signed(q16) > 16'sd127 || $signed(q16) < -16'sd128) begin
                            PSW[F_V] <= 1'b1; PSW[F_VT] <= 1'b1;
                        end
                        TMP <= {16'h0, r[7:0], q[7:0]};
                    end
                    ph <= 4'd5;
                end
                4'd5: mem(K_W16, SP_REG, {8'h00, OP2}, TMP[15:0], S_POST, 4'd0);
                default: ;
            endcase
            else if (g_ld) case (ph)
                4'd0: if (am_i) begin src <= OP1; ph <= 4'd2; end
                      else mem((d_mn == MN_LD) ? K_R16 : K_R8, am_d ? SP_REG : SP_ANY, OP1, 16'h0, S_EX, 4'd1);
                4'd1: begin src <= md; ph <= 4'd2; end
                4'd2: case (d_mn)
                    MN_LD:    mem(K_W16, SP_REG, {8'h00, OP2}, src, S_POST, 4'd0);
                    MN_LDB:   mem(K_W8,  SP_REG, {8'h00, OP2}, src, S_POST, 4'd0);
                    MN_LDBZE: mem(K_W16, SP_REG, {8'h00, OP2}, {8'h00, src[7:0]}, S_POST, 4'd0);
                    default:  mem(K_W16, SP_REG, {8'h00, OP2}, {{8{src[7]}}, src[7:0]}, S_POST, 4'd0);
                endcase
                default: ;
            endcase
            else if (g_st) case (ph)
                4'd0: mem(mn_byte ? K_R8 : K_R16, SP_REG, {8'h00, OP2}, 16'h0, S_EX, 4'd1);
                4'd1: mem(mn_byte ? K_W8 : K_W16, am_d ? SP_REG : SP_ANY, OP1, md, S_POST, 4'd0);
                default: ;
            endcase
            else if (d_mn == MN_CLR || d_mn == MN_CLRB)
                mem((d_mn == MN_CLR) ? K_W16 : K_W8, SP_REG, OP1, 16'h0, S_POST, 4'd0);
            else if (g_un) case (ph)
                4'd0: mem(g_unw ? K_R16 : K_R8, SP_REG, OP1, 16'h0, S_EX, 4'd1);
                4'd1: begin
                    case (d_mn)
                        MN_NOT, MN_NOTB: begin ar[15:0] = ~md; ar[31:16] = nz(PSW, ~md, g_unw); end
                        MN_NEG, MN_NEGB: ar = f_sub(PSW, 16'h0, md, 1'b0, g_unw);
                        MN_DEC, MN_DECB: ar = f_sub(PSW, md, 16'h1, 1'b0, g_unw);
                        default:         ar = f_add(PSW, md, 16'h1, 1'b0, g_unw);
                    endcase
                    PSW <= ar[31:16];
                    mem(g_unw ? K_W16 : K_W8, SP_REG, OP1, ar[15:0], S_POST, 4'd0);
                end
                default: ;
            endcase
            else if (d_mn == MN_EXT) case (ph)
                4'd0: begin OP1 <= OP1 & 16'h00fc; mem(K_R16, SP_REG, OP1 & 16'h00fc, 16'h0, S_EX, 4'd1); end
                4'd1: begin
                    PSW <= nz(PSW, md, 1'b1);
                    mem(K_W16, SP_REG, OP1 + 16'd2, {16{md[15]}}, S_POST, 4'd0);
                end
                default: ;
            endcase
            else if (d_mn == MN_EXTB) case (ph)
                4'd0: begin OP1 <= OP1 & 16'h00fe; mem(K_R8, SP_REG, OP1 & 16'h00fe, 16'h0, S_EX, 4'd1); end
                4'd1: begin
                    PSW <= nz(PSW, md, 1'b0);
                    mem(K_W16, SP_REG, OP1, {{8{md[7]}}, md[7:0]}, S_POST, 4'd0);
                end
                default: ;
            endcase
            else if (g_sh) case (ph)
                4'd0: mem(g_shb ? K_R8 : K_R16, SP_REG, {8'h00, OP2}, 16'h0, S_EX, 4'd1);
                4'd1: begin
                    sr = shift(d_mn, PSW, {16'h0, md}, OP1[4:0]);
                    PSW <= sr[47:32];
                    mem(g_shb ? K_W8 : K_W16, SP_REG, {8'h00, OP2}, sr[15:0], S_EX, 4'd2);
                end
                4'd2: finish((OP1 != 16'h0) ? (7'd7 + {2'b00, OP1[4:0]}) : 7'd8);
                default: ;
            endcase
            else if (g_shl) case (ph)
                4'd0: begin OP2 <= OP2 & 8'hfc; mem(K_R16, SP_REG, {8'h00, OP2 & 8'hfc}, 16'h0, S_EX, 4'd1); end
                4'd1: begin TMP[15:0] <= md; mem(K_R16, SP_REG, {8'h00, OP2 + 8'd2}, 16'h0, S_EX, 4'd2); end
                4'd2: begin
                    sr = shift(d_mn, PSW, {md, TMP[15:0]}, OP1[4:0]);
                    PSW <= sr[47:32];
                    TMP <= sr[31:0];
                    mem(K_W16, SP_REG, {8'h00, OP2}, sr[15:0], S_EX, 4'd3);
                end
                4'd3: mem(K_W16, SP_REG, {8'h00, OP2 + 8'd2}, TMP[31:16], S_EX, 4'd4);
                4'd4: finish((OP1 != 16'h0) ? (7'd7 + {2'b00, OP1[4:0]}) : 7'd8);
                default: ;
            endcase
            else if (d_mn == MN_NORML) case (ph)
                4'd0: begin OP2 <= OP2 & 8'hfc; mem(K_R16, SP_REG, {8'h00, OP2 & 8'hfc}, 16'h0, S_EX, 4'd1); end
                4'd1: begin TMP[15:0] <= md; mem(K_R16, SP_REG, {8'h00, OP2 + 8'd2}, 16'h0, S_EX, 4'd2); end
                4'd2: begin
                    // for (OP3 = 0; OP3 < 31 && int32(TMP) >= 0; TMP <<= 1, OP3++);
                    logic [31:0] t;
                    logic [4:0]  k;
                    t = {md, TMP[15:0]};
                    k = 5'd31;
                    for (int i = 0; i <= 30; i++) if (t[i]) k = 5'(31 - i);
                    if (t[31]) k = 5'd0;
                    t = t << k;
                    TMP <= t;
                    OP3 <= {3'b000, k};
                    PSW[F_Z] <= (t == 32'h0);
                    PSW[F_N] <= 1'b0;
                    PSW[F_C] <= 1'b0;
                    mem(K_W8, SP_REG, OP1, {11'h0, k}, S_EX, 4'd3);
                end
                4'd3: mem(K_W16, SP_REG, {8'h00, OP2}, TMP[15:0], S_EX, 4'd4);
                4'd4: mem(K_W16, SP_REG, {8'h00, OP2 + 8'd2}, TMP[31:16], S_EX, 4'd5);
                4'd5: finish(7'd11 + {2'b00, OP3[4:0]});
                default: ;
            endcase
            else if (g_jcc) begin
                if (jcond(d_mn, PSW)) begin
                    PC <= PC + OP1;
                    if (d_mn == MN_JVT) PSW[F_VT] <= 1'b0;
                    finish({1'b0, d_codd});
                end else begin
                    if (d_mn == MN_JNST || d_mn == MN_JST || d_mn == MN_JNVT) PSW[F_VT] <= 1'b0;
                    finish({1'b0, d_ceven});
                end
            end
            else if (d_mn == MN_JBC || d_mn == MN_JBS) case (ph)
                4'd0: mem(K_R8, SP_REG, {8'h00, OP2}, 16'h0, S_EX, 4'd1);
                4'd1: if (md[ist[2:0]] == (d_mn == MN_JBS)) begin PC <= PC + OP1; finish({1'b0, d_codd}); end
                      else finish({1'b0, d_ceven});
                default: ;
            endcase
            else if (d_mn == MN_DJNZ) case (ph)
                4'd0: mem(K_R8, SP_REG, {8'h00, OP2}, 16'h0, S_EX, 4'd1);
                4'd1: begin TMP <= {24'h0, md[7:0] - 8'd1}; mem(K_W8, SP_REG, {8'h00, OP2}, {8'h00, md[7:0] - 8'd1}, S_EX, 4'd2); end
                4'd2: if (TMP[7:0] != 8'h0) begin PC <= PC + OP1; finish({1'b0, d_codd}); end
                      else finish({1'b0, d_ceven});
                default: ;
            endcase
            else if (d_mn == MN_SJMP || d_mn == MN_LJMP) begin PC <= PC + OP1; finish({1'b0, d_ceven}); end
            else if (d_mn == MN_BR) begin PC <= OP1; finish({1'b0, d_ceven}); end
            else if (d_mn == MN_SCALL || d_mn == MN_LCALL || d_mn == MN_PUSHF || d_mn == MN_TRAP) case (ph)
                4'd0: mem(K_R16, SP_REG, 16'h0018, 16'h0, S_EX, 4'd1);
                4'd1: begin TMP <= {16'h0, md - 16'd2}; mem(K_W16, SP_REG, 16'h0018, md - 16'd2, S_EX, 4'd2); end
                4'd2: mem(K_W16, SP_ANY, TMP[15:0], (d_mn == MN_PUSHF) ? PSW : PC, S_EX, 4'd3);
                4'd3: case (d_mn)
                    MN_PUSHF: begin PSW <= 16'h0; finish({1'b0, d_ceven}); end
                    MN_TRAP:  mem(K_R16, SP_ANY, 16'h2010, 16'h0, S_EX, 4'd4);
                    default:  begin PC <= PC + OP1; finish({1'b0, d_ceven}); end
                endcase
                4'd4: begin PC <= md; finish({1'b0, d_ceven}); end
                default: ;
            endcase
            else if (d_mn == MN_RET || d_mn == MN_POPF) case (ph)
                4'd0: mem(K_R16, SP_REG, 16'h0018, 16'h0, S_EX, 4'd1);
                4'd1: begin TMP <= {16'h0, md}; mem(K_W16, SP_REG, 16'h0018, md + 16'd2, S_EX, 4'd2); end
                4'd2: mem(K_R16, SP_ANY, TMP[15:0], 16'h0, S_EX, 4'd3);
                4'd3: begin
                    if (d_mn == MN_RET) PC <= md; else PSW <= md;
                    finish({1'b0, d_ceven});
                end
                default: ;
            endcase
            else if (d_mn == MN_PUSH) case (ph)
                4'd0: mem(K_R16, SP_REG, 16'h0018, 16'h0, S_EX, 4'd1);
                4'd1: begin TMP <= {16'h0, md - 16'd2}; mem(K_W16, SP_REG, 16'h0018, md - 16'd2, S_EX, 4'd2); end
                4'd2: if (am_i) ph <= 4'd4;
                      else mem(K_R16, am_d ? SP_REG : SP_ANY, OP1, 16'h0, S_EX, 4'd3);
                4'd3: begin OP1 <= md; ph <= 4'd4; end
                4'd4: mem(K_W16, SP_ANY, TMP[15:0], OP1, S_POST, 4'd0);
                default: ;
            endcase
            else if (d_mn == MN_POP) case (ph)
                4'd0: mem(K_R16, SP_REG, 16'h0018, 16'h0, S_EX, 4'd1);
                4'd1: begin TMP <= {16'h0, md}; mem(K_W16, SP_REG, 16'h0018, md + 16'd2, S_EX, 4'd2); end
                4'd2: mem(K_R16, SP_ANY, TMP[15:0], 16'h0, S_EX, 4'd3);
                4'd3: begin
                    TMP <= {16'h0, md};
                    if (am_d) mem(K_W16, SP_REG, OP1, md, S_POST, 4'd0);
                    else begin
                        if (d_am == AM_INDIRECT && OPI[7:1] == 7'h0c) OP1 <= OP1 + 16'd2;
                        ph <= 4'd4;
                    end
                end
                4'd4: mem(K_W16, SP_ANY, OP1, TMP[15:0], S_POST, 4'd0);
                default: ;
            endcase
            else begin
                case (d_mn)
                    MN_CLRC:  PSW[F_C] <= 1'b0;
                    MN_SETC:  PSW[F_C] <= 1'b1;
                    MN_DI:    PSW[F_I] <= 1'b0;
                    MN_EI:    PSW[F_I] <= 1'b1;
                    MN_CLRVT: PSW[F_VT] <= 1'b0;
                    MN_RST:   PC <= 16'h2080;
                    default: ;                                 // nop, skip
                endcase
                finish({1'b0, d_ceven});
            end

            // ------------------------------------------------ post_indirect / post_indexed
            S_POST: case (ph)
                4'd0: if (autoinc) mem(K_W16, SP_REG, {8'h00, OPI}, OP1 + inc_size, S_POST, 4'd1);
                      else finish({1'b0, cyc_post});
                4'd1: finish({1'b0, cyc_post});
                default: ;
            endcase

            // ------------------------------------------------ restoring divider, 32 steps
            S_DIV: begin
                logic [32:0] t;
                t = {dv_r, dv_n[31]} - {1'b0, dv_d};
                dv_n <= {dv_n[30:0], 1'b0};
                if (!t[32]) begin dv_r <= t[31:0]; dv_q <= {dv_q[30:0], 1'b1}; end
                else begin dv_r <= {dv_r[30:0], dv_n[31]}; dv_q <= {dv_q[30:0], 1'b0}; end
                dv_i <= dv_i + 6'd1;
                if (dv_i == 6'd31) begin st <= S_EX; ph <= ret_ph; end
            end

            // ------------------------------------------------ one access (two for 16 bits)
            S_MEM: if (!m_go) m_go <= 1'b1;
                   else if (m_ack) begin
                       logic [7:0] b;
                       m_go <= 1'b0;
                       b = m_rdata;
                       if (m_sfr) begin
                           if (m_we) begin
                               if (m_addr[0]) pending <= m_wdata;
                               else PSW[7:0] <= m_wdata;
                           end else
                               b = m_addr[0] ? pending : PSW[7:0];
                       end
                       if (!mbyte) md <= {8'h00, b}; else md[15:8] <= b;
                       if (m16 && !mbyte) mbyte <= 1'b1;
                       else begin st <= ret_st; ph <= ret_ph; end
                   end

            S_FIN: begin
                done <= 1'b1;
                st <= S_IDLE;
                if (in_handler) begin
                    need_fetch <= 1'b1;
                    fetch_noirq <= d_valid && d_noirq;
                end
            end
            default: ;
            endcase
        end
    end
endmodule
