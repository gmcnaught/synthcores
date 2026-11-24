// SPDX-License-Identifier: GPL-3.0-or-later
// Proteus 2000 G05 effects: reverb and chorus as defined by p2k::fx_int in p2k/model/p2k_sound.h
// (a workalike driven by the EMU8000-style INIT arrays the OS writes; docs/p2k-findings.md).
// Arithmetic and order follow fx_int::configure / process / comb / tail / diffuse / tap.
//
// It owns the G05 register file (block RAM). Per output sample:
//   pre (at the sample start): fx_int::configure when cfg (every 256 samples), then the read
//       positions and the chorus LFO, then one memory read per delay-line tap (16), so the data
//       is in by the time the voice mix is done;
//   go: fx_int::process on rev_in / cho_in, one shared multiply-add (two 32x18 products), the
//       delay-line writes (13) through the memory port; done with the wet signals to add.
//
// Delay lines (32-bit values, one per 64-bit word; the chorus keeps both channels in one word) are
// in external memory at FX_BASE + line * 2^14 words (lines 0-5 reflections, 6-7 tails, 8-11
// diffusers, 12-13 chorus). The model clears a line at reset and when configure changes its length.
// Here a cleared line reads as zero until every position has been written once: a comb, tail or
// diffuser line is read and written at one position a sample, advancing by one, so its first `len`
// reads after a clear are zero (zc counts them down). The chorus is written at cpos (from 0 at
// reset) and read anywhere, so a tap at i reads zero while cpos has not passed i since the reset.
module p2k_fx #(
    parameter logic [28:0] FX_BASE = 29'h1000000
) (
    input  logic        clk,
    input  logic        rst,
    input  logic        clr,              // reset sweep: clears the register file
    input  logic [10:0] clr_addr,

    input  logic        wr_en,            // register write, read-modify-write over two clocks
    input  logic [10:0] wr_addr,          // {channel[4:0], word[5:0]}
    input  logic [15:0] wr_data,
    input  logic [15:0] wr_mask,

    input  logic        pre,
    input  logic        cfg,
    output logic        pre_busy,         // configure running: the sends are not valid yet
    input  logic        go,
    input  logic signed [31:0] rev_in,
    input  logic signed [31:0] cho_in,
    output logic        done,
    output logic signed [32:0] add_l,
    output logic signed [32:0] add_r,

    output logic signed [16:0] rev_send [0:3],   // Q15
    output logic signed [16:0] cho_send [0:3],
    output logic signed [16:0] dry      [0:3][0:1],

    output logic        req_valid,
    input  logic        req_ready,
    output logic        req_we,
    output logic [28:0] req_addr,
    output logic [63:0] req_wdata,
    output logic [7:0]  req_be,
    output logic [7:0]  req_len,
    input  logic        rsp_valid,
    input  logic [63:0] rsp_data
);

    localparam int STEP = 242;
    localparam int CHORUS_LEN = 32768;
    localparam int NCFG = 41;

    function automatic logic signed [31:0] sat(input logic signed [63:0] v);
        sat = (v < -64'sd1073741824) ? -32'sd1073741824 : (v > 64'sd1073741823) ? 32'sd1073741823 : 32'(v);
    endfunction
    function automatic logic signed [16:0] q256(input logic [7:0] b);
        q256 = $signed({2'b0, b, 7'd0});
    endfunction
    function automatic logic signed [16:0] q255(input logic [7:0] b);
        q255 = $signed(17'((17'(b) * 17'd257) >> 1));
    endfunction
    function automatic logic signed [16:0] q15(input logic [3:0] n);
        q15 = $signed(17'((21'(n) * 21'd34953) >> 4));
    endfunction
    function automatic logic [15:0] eff(input logic [15:0] pos, input logic [15:0] len);
        eff = (pos >= len) ? 16'd0 : pos;
    endfunction
    function automatic logic [15:0] nxt(input logic [15:0] p, input logic [15:0] len);
        nxt = (p + 16'd1 == len) ? 16'd0 : p + 16'd1;
    endfunction

    // configure reads: init(n, ch) = g05[ch * 64 + {0x14, 0x15, 0x1c, 0x1d}[n - 1]],
    // dword(ch, lo, hi) = {g05[ch * 64 + hi / 2], g05[ch * 64 + lo / 2]}
    function automatic logic [10:0] init_a(input int n, input int ch);
        case (n)
            1: init_a = 11'(ch * 64 + 8'h14);
            2: init_a = 11'(ch * 64 + 8'h15);
            3: init_a = 11'(ch * 64 + 8'h1c);
            default: init_a = 11'(ch * 64 + 8'h1d);
        endcase
    endfunction
    function automatic logic [10:0] cfg_a(input logic [5:0] i);
        case (i)
            6'd0:  cfg_a = init_a(1, 7);
            6'd1:  cfg_a = init_a(2, 8'h14);
            6'd2:  cfg_a = init_a(2, 8'h16);
            6'd3:  cfg_a = init_a(3, 8'h1a);
            6'd4:  cfg_a = init_a(1, 0);
            6'd5:  cfg_a = init_a(1, 3);
            6'd6:  cfg_a = init_a(1, 5);
            6'd7:  cfg_a = init_a(1, 8'h0f);     // reflection gains: init(gain_n[i], gain_c[i])
            6'd8:  cfg_a = init_a(1, 8'h17);
            6'd9:  cfg_a = init_a(1, 8'h1f);
            6'd10: cfg_a = init_a(2, 8'h07);
            6'd11: cfg_a = init_a(2, 8'h0f);
            6'd12: cfg_a = init_a(2, 8'h17);
            6'd13: cfg_a = init_a(1, 8'h09);     // reflection feedback: init(i < 3 ? 1 : 2, fb_c[i])
            6'd14: cfg_a = init_a(1, 8'h11);
            6'd15: cfg_a = init_a(1, 8'h19);
            6'd16: cfg_a = init_a(2, 8'h01);
            6'd17: cfg_a = init_a(2, 8'h09);
            6'd18: cfg_a = init_a(2, 8'h11);
            6'd19: cfg_a = init_a(2, 8'h1d);     // damp
            6'd20: cfg_a = init_a(3, 8'h01);     // refl_in
            6'd21: cfg_a = init_a(4, 8'h1f);     // tail_ret
            6'd22: cfg_a = init_a(3, 8'h17);     // ch_fb
            6'd23: cfg_a = init_a(4, 8'h0b);     // ch_depth
            6'd24: cfg_a = 11'(10 * 64 + 13);    // ch_inc
            default: begin                       // 25 + 4b + j: bus b's sends
                automatic int k = int'(i) - 25;
                automatic int b = k / 4;
                case (k % 4)
                    0: cfg_a = 11'(4 * b * 64 + 8'h16 / 2);         // dword(4b, 0x16, 0x14)[15:8]
                    1: cfg_a = 11'(4 * b * 64 + 8'h74 / 2);         // dword(4b, 0x76, 0x74)[31:24]
                    2: cfg_a = 11'(4 * b * 64 + 8'h64 / 2);         // dword(4b, 0x66, 0x64)[31:24]
                    default: cfg_a = 11'((4 * b + 16) * 64 + 8'h16 / 2);   // dword(4b + 16, 0x16, 0x14)[7:0]
                endcase
            end
        endcase
    endfunction

    typedef enum logic [3:0] {
        X_IDLE, X_CFG, X_CFGA, X_CFGB, X_POS, X_SIN, X_LFO, X_ISSUE, X_RUN
    } xst_t;
    xst_t xs;

    // ------------------------------------------------------------------ G05 register file
    logic [10:0] g_ra;
    logic [15:0] g_q;
    logic        w_d;
    logic [10:0] w_a;
    logic [15:0] w_dat, w_msk;
    logic [5:0]  ci;
    p2k_ram #(.W(16), .AW(11)) u_g05 (
        .clk, .re(wr_en || xs == X_CFG), .raddr(g_ra), .rdata(g_q),
        .we(clr || w_d), .waddr(clr ? clr_addr : w_a),
        .wdata(clr ? 16'h0 : (g_q & ~w_msk) | (w_dat & w_msk))
    );
    assign g_ra = wr_en ? wr_addr : cfg_a(ci);

    // ------------------------------------------------------------------ state
    logic        active, stereo;
    logic signed [16:0] out_mix, ap_fb, damp, refl_in, tail_ret, ch_fb;
    logic signed [16:0] r_gain [0:5], r_fb [0:5];
    logic signed [31:0] damp_state;
    logic signed [31:0] r_lp [0:5];
    logic [15:0] len [0:11];             // 0-5 reflections, 6-7 tails, 8-11 diffusers
    logic [15:0] pos [0:11];
    logic [15:0] zc  [0:11];             // reads still to return zero after a clear
    logic [15:0] ch_delay;
    logic signed [31:0] ch_depth;
    logic [31:0] ch_inc, phase;
    logic [14:0] cpos;
    logic        cfull;                  // cpos has wrapped since the reset

    logic [15:0] craw [0:NCFG-1];

    // ------------------------------------------------------------------ multiply-add
    logic signed [31:0] ma, mc;
    logic signed [17:0] mb, md;
    logic signed [49:0] mp1, mp2;
    logic signed [50:0] msum;
    logic [1:0]  mcnt;                   // clocks until msum is valid (operands set when it is loaded)
    logic        mgo;
    always_ff @(posedge clk) begin
        mp1 <= ma * mb;
        mp2 <= mc * md;
        msum <= 51'(mp1) + 51'(mp2);
        if (mgo) mcnt <= 2'd2;
        else if (mcnt != 2'd0) mcnt <= mcnt - 2'd1;
    end
    wire mrdy = mcnt == 2'd0 && !mgo;
    function automatic logic signed [31:0] rsat(input logic signed [50:0] v, input int s);
        rsat = sat(64'((v + (51'sd1 <<< (s - 1))) >>> s));
    endfunction

    // ------------------------------------------------------------------ memory requests (4-deep queue)
    logic        q_we  [0:3];
    logic [28:0] q_a   [0:3];
    logic [63:0] q_d   [0:3];
    logic [7:0]  q_be  [0:3];
    logic [2:0]  q_n;
    logic [1:0]  q_rp;
    logic        q_push, q_pwe;
    logic [28:0] q_pa;
    logic [63:0] q_pd;
    logic [7:0]  q_pbe;
    wire         q_full = q_n == 3'd4;
    assign req_valid = q_n != 3'd0;
    assign req_we = q_we[q_rp];
    assign req_addr = q_a[q_rp];
    assign req_wdata = q_d[q_rp];
    assign req_be = q_be[q_rp];
    assign req_len = 8'd1;
    wire q_pop = req_valid && req_ready;
    always_ff @(posedge clk) begin
        if (rst) begin
            q_n <= '0;
            q_rp <= '0;
        end else begin
            if (q_push) begin
                automatic logic [1:0] wp = 2'(q_rp + q_n[1:0]);
                q_we[wp] <= q_pwe;
                q_a[wp] <= q_pa;
                q_d[wp] <= q_pd;
                q_be[wp] <= q_pbe;
            end
            if (q_pop) q_rp <= q_rp + 2'd1;
            q_n <= q_n + {2'd0, q_push} - {2'd0, q_pop};
        end
    end
    function automatic logic [28:0] line_a(input logic [3:0] l, input logic [15:0] p);
        line_a = FX_BASE + {11'd0, l, p[13:0]};
    endfunction
    function automatic logic [28:0] chorus_a(input logic [14:0] i);
        chorus_a = FX_BASE + {11'd0, 4'd12, 14'd0} + {14'd0, i};
    endfunction

    // ------------------------------------------------------------------ prefetch
    // 0-5 reflections, 6-7 tails, 8-11 diffusers, 12/13 chorus left i0/i1, 14/15 right i0/i1
    logic [15:0] pp [0:11];              // positions read (and written) this sample
    logic [15:0] pn [0:11];              // and the next ones
    logic [14:0] ci0 [0:1];
    logic [15:0] cf [0:1];
    logic signed [31:0] pf [0:15];
    logic [15:0] pz;                     // chorus taps that read zero
    logic [4:0]  pi;                     // reads issued
    logic [4:0]  rc;                     // responses in
    logic [10:0] sq, sq_r;               // sine ROM: address, registered address, value
    logic [15:0] sq_q;
    always_ff @(posedge clk) sq_r <= sq;
    p2k_fx_sine_rom u_sine (.clk, .a(sq_r), .q(sq_q));

    // ------------------------------------------------------------------ control
    logic [3:0]  sx;                     // sine / LFO step
    logic signed [16:0] sa [0:3];        // sine(i), sine(i+1), sine(j), sine(j+1)
    logic signed [17:0] ls [0:1];
    logic signed [39:0] lfo [0:1];
    logic [5:0]  ci_d;
    logic        cfg_rd;
    // configure intermediates
    logic        n_st;
    logic [5:0]  n_tl, n_tr;
    logic [15:0] n_cd;

    // process
    logic [4:0]  pc;
    logic [2:0]  li;
    logic        mwait;
    logic signed [31:0] ds, in2, cur_out, lp, held, wv, cl, cr, cw0, dl, dr;
    logic signed [31:0] o [0:5];
    logic signed [31:0] p_in, p_dl, p_dr;
    logic signed [31:0] pt [0:1];
    logic [1:0]  dj;

    function automatic logic signed [16:0] sine_v(input logic [11:0] i, input logic [15:0] q);
        sine_v = i[11] ? -$signed({1'b0, q}) : $signed({1'b0, q});
    endfunction
    function automatic logic [10:0] sine_a(input logic [11:0] i);
        sine_a = i[10] ? 11'(11'd1024 - {1'b0, i[9:0]}) : {1'b0, i[9:0]};
    endfunction

    always_comb begin
        automatic logic [31:0] ph2 = phase + 32'h40000000;
        case (sx)
            4'd0: sq = sine_a(phase[31:20]);
            4'd1: sq = sine_a(12'(phase[31:20] + 12'd1));
            4'd2: sq = sine_a(ph2[31:20]);
            default: sq = sine_a(12'(ph2[31:20] + 12'd1));
        endcase
    end

    assign pre_busy = xs == X_CFG || xs == X_CFGA || xs == X_CFGB;

    always_ff @(posedge clk) begin
        done <= 1'b0;
        mgo <= 1'b0;
        q_push <= 1'b0;
        w_d <= wr_en;
        if (wr_en) begin
            w_a <= wr_addr;
            w_dat <= wr_data;
            w_msk <= wr_mask;
        end
        cfg_rd <= 1'b0;
        ci_d <= ci;
        if (cfg_rd) craw[ci_d] <= g_q;

        // prefetch responses
        if (rsp_valid) begin
            automatic logic signed [31:0] v = (rc >= 5'd14) ? $signed(rsp_data[63:32]) : $signed(rsp_data[31:0]);
            pf[rc[3:0]] <= (rc >= 5'd12 && pz[rc[3:0]]) ? 32'sd0 : v;
            rc <= rc + 5'd1;
        end

        if (rst) begin
            xs <= X_IDLE;
            ci <= '0;
            pc <= '0;
            rc <= 5'd16;
            pi <= '0;
            mwait <= 1'b0;
            active <= 1'b0; stereo <= 1'b0;
            out_mix <= '0; ap_fb <= '0; damp <= '0; damp_state <= '0; refl_in <= '0; tail_ret <= '0;
            ch_delay <= 16'd1; ch_fb <= '0; ch_depth <= '0; ch_inc <= '0; phase <= '0; cpos <= '0; cfull <= 1'b0;
            len[0] <= 16'(2 * STEP); len[1] <= 16'(4 * STEP); len[2] <= 16'(8 * STEP);
            len[3] <= 16'(13 * STEP); len[4] <= 16'(19 * STEP); len[5] <= 16'(26 * STEP);
            len[6] <= 16'(27 * STEP); len[7] <= 16'(27 * STEP);
            len[8] <= 16'(2 * 4 * STEP + 55); len[9] <= 16'(1 * 4 * STEP + 55);
            len[10] <= 16'(2 * 4 * STEP + 55); len[11] <= 16'(1 * 4 * STEP + 55);
            zc[0] <= 16'(2 * STEP); zc[1] <= 16'(4 * STEP); zc[2] <= 16'(8 * STEP);
            zc[3] <= 16'(13 * STEP); zc[4] <= 16'(19 * STEP); zc[5] <= 16'(26 * STEP);
            zc[6] <= 16'(27 * STEP); zc[7] <= 16'(27 * STEP);
            zc[8] <= 16'(2 * 4 * STEP + 55); zc[9] <= 16'(1 * 4 * STEP + 55);
            zc[10] <= 16'(2 * 4 * STEP + 55); zc[11] <= 16'(1 * 4 * STEP + 55);
            for (int i = 0; i < 12; i++) pos[i] <= '0;
            for (int i = 0; i < 6; i++) begin
                r_lp[i] <= '0; r_gain[i] <= '0; r_fb[i] <= '0;
            end
            for (int b = 0; b < 4; b++) begin
                rev_send[b] <= '0; cho_send[b] <= '0; dry[b][0] <= '0; dry[b][1] <= '0;
            end
        end else begin
            case (xs)
            // ---------------- sample start
            X_IDLE: if (pre) begin
                ci <= '0;
                xs <= cfg ? X_CFG : X_POS;
            end
            X_CFG: begin                 // one register read a clock; craw[ci] is written a clock later
                cfg_rd <= 1'b1;
                if (ci == 6'(NCFG - 1))
                    xs <= X_CFGA;
                else
                    ci <= ci + 6'd1;
            end
            X_CFGA: if (!cfg_rd) begin   // fx_int::configure
                automatic logic st = craw[0] == 16'h8474;
                automatic logic [5:0] tl = 6'(craw[1][11:8]) + 6'd18;
                automatic logic [15:0] cd = craw[3];
                n_st <= st;
                n_tl <= tl;
                n_tr <= st ? 6'(craw[2][11:8]) + 6'd18 : tl;
                n_cd <= (cd < 16'd1) ? 16'd1 : (cd > 16'(CHORUS_LEN - 4096)) ? 16'(CHORUS_LEN - 4096) : cd;
                active <= craw[4] != 16'h03ff;
                out_mix <= q256(craw[5][7:0]);
                ap_fb <= q255(craw[6][7:0]);
                stereo <= st;
                for (int i = 0; i < 6; i++) begin
                    r_gain[i] <= q15(craw[7 + i][7:4]);
                    r_fb[i] <= q15(craw[13 + i][3:0]);
                end
                damp <= q255(craw[19][7:0]);
                refl_in <= q256(craw[20][7:0]);
                tail_ret <= q256(craw[21][7:0]);
                ch_fb <= q256(craw[22][7:0]);
                ch_inc <= 32'((48'(craw[24]) * 48'd99731) >> 10);
                for (int b = 0; b < 4; b++) begin
                    rev_send[b] <= q255(craw[25 + 4 * b][15:8]);
                    cho_send[b] <= q255(craw[26 + 4 * b][15:8]);
                    dry[b][0] <= (b == 1 || b == 2) ? 17'sd32768 : q255(craw[27 + 4 * b][15:8]);
                    dry[b][1] <= (b == 1 || b == 2) ? 17'sd32768 : q255(craw[28 + 4 * b][7:0]);
                end
                xs <= X_CFGB;
            end
            X_CFGB: begin
                automatic logic [15:0] l5 = 16'(n_tl * STEP);
                automatic logic [15:0] l6 = 16'((n_tl + 6'd1) * STEP);
                automatic logic [15:0] l7 = 16'((n_tr + 6'd1) * STEP);
                ch_delay <= n_cd;
                ch_depth <= $signed(32'((32'(craw[23][7:0]) * 32'(n_cd)) << 8));
                if (len[5] != l5) begin len[5] <= l5; zc[5] <= l5; end
                if (len[6] != l6) begin len[6] <= l6; zc[6] <= l6; end
                if (len[7] != l7) begin len[7] <= l7; zc[7] <= l7; end
                xs <= X_POS;
            end
            X_POS: begin
                for (int i = 0; i < 12; i++) begin
                    pp[i] <= eff(pos[i], len[i]);
                    pn[i] <= nxt(eff(pos[i], len[i]), len[i]);
                end
                sx <= '0;
                if (active)
                    xs <= X_SIN;
                else
                    xs <= X_IDLE;
            end
            // ---------------- chorus LFO: sine reads (ROM, one clock), then (b - a) f and ls * depth
            X_SIN: begin
                // sq follows sx; the value of read sx - 2 is in sq_q
                if (sx >= 4'd2)
                    sa[sx - 4'd2] <= sine_v(sx == 4'd2 ? phase[31:20] : sx == 4'd3 ? 12'(phase[31:20] + 12'd1) :
                                            sx == 4'd4 ? 12'(phase[31:20] + 12'h400) : 12'(phase[31:20] + 12'h401), sq_q);
                if (sx == 4'd5) begin
                    sx <= '0;
                    xs <= X_LFO;
                end else
                    sx <= sx + 4'd1;
            end
            X_LFO: begin
                if (!mwait) begin
                    case (sx)
                        4'd0: begin ma <= 32'(phase[19:10]); mb <= 18'(sa[1] - sa[0]); mc <= '0; md <= '0; end
                        4'd1: begin ma <= ch_depth; mb <= ls[0]; mc <= '0; md <= '0; end
                        4'd2: begin
                            automatic logic [31:0] ph2 = phase + 32'h40000000;
                            ma <= 32'(ph2[19:10]); mb <= 18'(sa[3] - sa[2]); mc <= '0; md <= '0;
                        end
                        default: begin ma <= ch_depth; mb <= ls[1]; mc <= '0; md <= '0; end
                    endcase
                    mgo <= 1'b1;
                    mwait <= 1'b1;
                end else if (mrdy) begin
                    mwait <= 1'b0;
                    case (sx)
                        4'd0: ls[0] <= 18'(sa[0] + 18'(64'((msum + 51'sd512) >>> 10)));
                        4'd1: lfo[0] <= 40'((msum + 51'sd32768) >>> 16);
                        4'd2: ls[1] <= 18'(sa[2] + 18'(64'((msum + 51'sd512) >>> 10)));
                        default: lfo[1] <= 40'((msum + 51'sd32768) >>> 16);
                    endcase
                    if (sx == 4'd3) begin
                        for (int c = 0; c < 2; c++) begin
                            automatic logic signed [40:0] lf = (c == 0) ? 41'(lfo[0]) : 41'((msum + 51'sd32768) >>> 16);
                            automatic logic signed [40:0] d = $signed({9'd0, ch_delay, 16'h0}) + lf;
                            automatic logic signed [40:0] p = $signed({10'd0, cpos, 16'h0}) - d + 41'sd2147483648;
                            ci0[c] <= p[30:16];
                            cf[c] <= p[15:0];
                        end
                        pi <= '0;
                        rc <= '0;
                        xs <= X_ISSUE;
                    end
                    sx <= sx + 4'd1;
                end
            end
            // ---------------- the 16 reads, in pf order
            X_ISSUE: if (!q_full && !q_push) begin
                q_push <= 1'b1;
                q_pwe <= 1'b0;
                q_pd <= '0;
                q_pbe <= 8'hff;
                if (pi < 5'd12)
                    q_pa <= line_a(pi[3:0], pp[pi[3:0]]);
                else begin
                    automatic int c = (pi >= 5'd14) ? 1 : 0;
                    automatic logic [14:0] i = ci0[c] + {14'd0, pi[0]};
                    q_pa <= chorus_a(i);
                    pz[pi[3:0]] <= !cfull && i >= cpos;
                end
                if (pi == 5'd15)
                    xs <= X_RUN;
                pi <= pi + 5'd1;
            end
            // ---------------- fx_int::process
            X_RUN: ;                     // process below, once the 16 reads are in (rc)
            default: xs <= X_IDLE;
            endcase

            // process sequence (pc = 0 waits for go)
            if (pc == 5'd0) begin
                if (go) begin
                    if (!active) begin
                        add_l <= '0;
                        add_r <= '0;
                        done <= 1'b1;
                    end else
                        pc <= 5'd1;
                end
            end else if (xs == X_RUN && rc == 5'd16) begin   // process
                if (!mwait) begin
                    // issue the step's multiply-add (steps without one go straight on)
                    automatic logic issue = 1'b1;
                    case (pc)
                        5'd1: begin ma <= rev_in; mb <= 18'(17'sd32767 - damp + 17'sd1); mc <= damp_state; md <= 18'(damp); end
                        5'd2: begin ma <= ds; mb <= 18'(refl_in); mc <= '0; md <= '0; end
                        5'd3: begin
                            automatic logic signed [31:0] out = (zc[li] != 0) ? 32'sd0 : pf[li];
                            cur_out <= out;
                            ma <= out; mb <= 18'(17'sd32767 - damp + 17'sd1); mc <= r_lp[li]; md <= 18'(damp);
                        end
                        5'd4: begin ma <= lp; mb <= 18'(r_fb[li]); mc <= '0; md <= '0; end
                        5'd5: begin ma <= cur_out; mb <= 18'(r_gain[li]); mc <= '0; md <= '0; end
                        5'd6, 5'd7, 5'd8: issue = 1'b0;
                        5'd9: begin
                            automatic logic [3:0] a = {2'b10, dj[0], dj[1]};   // line 8 + 2c + j
                            automatic logic signed [31:0] h = (zc[a] != 0) ? 32'sd0 : pf[a];
                            held <= h;
                            ma <= h; mb <= 18'(ap_fb); mc <= '0; md <= '0;
                        end
                        5'd10: begin ma <= wv; mb <= 18'(ap_fb); mc <= '0; md <= '0; end
                        5'd11: begin ma <= pt[0]; mb <= 18'(tail_ret); mc <= '0; md <= '0; end
                        5'd12: begin ma <= pt[1]; mb <= 18'(tail_ret); mc <= '0; md <= '0; end
                        5'd13: begin ma <= pf[12]; mb <= 18'(32'd65536 - 32'(cf[0])); mc <= pf[13]; md <= 18'(cf[0]); end
                        5'd14: begin ma <= pf[14]; mb <= 18'(32'd65536 - 32'(cf[1])); mc <= pf[15]; md <= 18'(cf[1]); end
                        5'd15: begin ma <= cl; mb <= 18'(ch_fb); mc <= '0; md <= '0; end
                        5'd16: begin ma <= cr; mb <= 18'(ch_fb); mc <= '0; md <= '0; end
                        5'd17: begin ma <= dl; mb <= 18'(out_mix); mc <= '0; md <= '0; end
                        5'd18: begin ma <= dr; mb <= 18'(out_mix); mc <= '0; md <= '0; end
                        default: issue = 1'b0;
                    endcase
                    if (issue) begin
                        mgo <= 1'b1;
                        mwait <= 1'b1;
                    end else begin
                        case (pc)
                            5'd6: begin       // p_dl, p_dr
                                if (stereo) begin
                                    p_dl <= sat(64'(o[2]) + 64'(o[4]));
                                    p_dr <= sat(64'(o[0]) + 64'(o[1]) + 64'(o[3]) + 64'(o[5]));
                                end else begin
                                    automatic logic signed [31:0] m = sat(64'(o[0]) + 64'(o[1]) + 64'(o[2]) + 64'(o[3]) + 64'(o[4]) + 64'(o[5]));
                                    p_dl <= m;
                                    p_dr <= m;
                                end
                                pc <= 5'd7;
                            end
                            5'd7, 5'd8: if (!q_full && !q_push) begin   // tail(): c = pc - 7
                                automatic int c = (pc == 5'd8) ? 1 : 0;
                                automatic logic signed [31:0] tin = sat(64'(p_in) + 64'(c == 0 ? p_dl : p_dr));
                                pt[c] <= (zc[6 + c] != 0) ? 32'sd0 : pf[6 + c];
                                if (zc[6 + c] != 0) zc[6 + c] <= zc[6 + c] - 16'd1;
                                pos[6 + c] <= pn[6 + c];
                                q_push <= 1'b1; q_pwe <= 1'b1; q_pa <= line_a(4'(6 + c), pp[6 + c]);
                                q_pd <= {32'd0, tin}; q_pbe <= 8'h0f;
                                pc <= pc + 5'd1;
                                dj <= '0;
                            end
                            5'd19: if (q_n == 3'd0 && !q_push) begin   // writes all taken
                                done <= 1'b1;
                                pc <= 5'd0;
                                xs <= X_IDLE;
                            end
                            default: ;
                        endcase
                    end
                end else if (mrdy && (!q_full && !q_push)) begin
                    automatic logic signed [31:0] r15 = rsat(msum, 15);
                    mwait <= 1'b0;
                    case (pc)
                        5'd1: begin ds <= r15; damp_state <= r15; p_in <= r15; pc <= 5'd2; end
                        5'd2: begin in2 <= r15; li <= '0; pc <= 5'd3; end
                        5'd3: begin lp <= r15; r_lp[li] <= r15; pc <= 5'd4; end
                        5'd4: begin
                            automatic logic signed [31:0] w = sat(64'(in2) - 64'(r15));
                            q_push <= 1'b1; q_pwe <= 1'b1; q_pa <= line_a({1'b0, li}, pp[li]);
                            q_pd <= {32'd0, w}; q_pbe <= 8'h0f;
                            pos[li] <= pn[li];
                            if (zc[li] != 0) zc[li] <= zc[li] - 16'd1;
                            pc <= 5'd5;
                        end
                        5'd5: begin
                            o[li] <= r15;
                            li <= li + 3'd1;
                            pc <= (li == 3'd5) ? 5'd6 : 5'd3;
                        end
                        5'd9: begin
                            automatic logic [3:0] a = {2'b10, dj[0], dj[1]};
                            automatic logic signed [31:0] w = sat(64'(r15) - 64'(pt[dj[0]]));
                            wv <= w;
                            q_push <= 1'b1; q_pwe <= 1'b1; q_pa <= line_a(a, pp[a]);
                            q_pd <= {32'd0, w}; q_pbe <= 8'h0f;
                            pos[a] <= pn[a];
                            if (zc[a] != 0) zc[a] <= zc[a] - 16'd1;
                            pc <= 5'd10;
                        end
                        5'd10: begin
                            pt[dj[0]] <= sat(64'(held) - 64'(r15));
                            dj <= dj + 2'd1;
                            pc <= (dj == 2'd3) ? 5'd11 : 5'd9;
                        end
                        5'd11: begin dl <= sat(64'(p_dl) + 64'(r15)); pc <= 5'd12; end
                        5'd12: begin dr <= sat(64'(p_dr) + 64'(r15)); pc <= 5'd13; end
                        5'd13: begin cl <= rsat(msum, 16); pc <= 5'd14; end
                        5'd14: begin cr <= rsat(msum, 16); pc <= 5'd15; end
                        5'd15: begin cw0 <= sat(64'(cho_in) + 64'(r15)); pc <= 5'd16; end
                        5'd16: begin
                            automatic logic signed [31:0] cw1 = sat(64'(cho_in) + 64'(r15));
                            q_push <= 1'b1; q_pwe <= 1'b1; q_pa <= chorus_a(cpos);
                            q_pd <= {cw1, cw0}; q_pbe <= 8'hff;
                            cpos <= cpos + 15'd1;
                            if (cpos == 15'h7fff) cfull <= 1'b1;
                            phase <= phase + ch_inc;
                            pc <= 5'd17;
                        end
                        5'd17: begin add_l <= 33'(r15) + 33'(cl); pc <= 5'd18; end
                        5'd18: begin add_r <= 33'(r15) + 33'(cr); pc <= 5'd19; end
                        default: ;
                    endcase
                end
            end
        end
    end
endmodule
