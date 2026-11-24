// SPDX-License-Identifier: GPL-3.0-or-later
// Proteus 2000 G05 effects: reverb and chorus as defined by p2k::fx_int in p2k/model/p2k_sound.h
// (a workalike driven by the EMU8000-style INIT arrays the OS writes; docs/p2k-findings.md).
// It owns the G05 register file. cfg reconfigures from the registers (the model does this every
// 256 samples); go processes one sample and pulses done with the wet signals to add to the mix.
// Arithmetic and order follow fx_int::configure / process / comb / tail / diffuse / tap exactly.
//
// Memory: the delay lines are about 106 k words (chorus 2 x 32 k). They are arrays here; a MiSTer
// build needs them in external memory or a smaller line set.

module p2k_fx (
    input  logic        clk,
    input  logic        rst,

    input  logic        wr_en,
    input  logic [10:0] wr_addr,      // {channel[4:0], word[5:0]}
    input  logic [15:0] wr_data,
    input  logic [15:0] wr_mask,

    input  logic        cfg,          // fx_int::configure (one clock; ready next clock)
    input  logic        go,           // fx_int::process
    input  logic signed [31:0] rev_in,
    input  logic signed [31:0] cho_in,
    output logic        done,
    output logic signed [63:0] add_l,
    output logic signed [63:0] add_r,

    output logic signed [31:0] rev_send [0:3],   // Q15
    output logic signed [31:0] cho_send [0:3],
    output logic signed [31:0] dry      [0:3][0:1]
);

    `include "p2k_fx_sine.svh"

    localparam int STEP = 242;
    localparam int CHORUS_LEN = 32768;
    localparam logic signed [63:0] LIM = 64'sd1 <<< 30;

    function automatic logic signed [63:0] rsh(input logic signed [63:0] v, input int s);
        rsh = (v + (64'sd1 <<< (s - 1))) >>> s;
    endfunction
    function automatic logic signed [31:0] sat(input logic signed [63:0] v);
        sat = (v < -LIM) ? -32'sd1073741824 : (v > LIM - 1) ? 32'sd1073741823 : 32'(v);
    endfunction
    function automatic logic signed [31:0] mul(input logic signed [31:0] x, input logic signed [31:0] c);
        mul = sat(rsh(64'(x) * 64'(c), 15));
    endfunction
    function automatic logic signed [31:0] q256(input logic [7:0] b);
        q256 = $signed({17'd0, b, 7'd0});
    endfunction
    function automatic logic signed [31:0] q255(input logic [7:0] b);
        q255 = $signed(32'((32'(b) * 32'd257) >> 1));
    endfunction
    function automatic logic signed [31:0] q15(input logic [3:0] n);
        q15 = $signed(32'((32'(n) * 32'd34953) >> 4));
    endfunction
    function automatic logic signed [31:0] sine(input logic [11:0] i);
        case (i[11:10])
            2'd0: sine = $signed({16'd0, fx_qsine({1'b0, i[9:0]})});
            2'd1: sine = $signed({16'd0, fx_qsine(11'd1024 - {1'b0, i[9:0]})});
            2'd2: sine = -$signed({16'd0, fx_qsine({1'b0, i[9:0]})});
            default: sine = -$signed({16'd0, fx_qsine(11'd1024 - {1'b0, i[9:0]})});
        endcase
    endfunction
    function automatic logic signed [63:0] lfo_sine(input logic [31:0] phase);
        logic [11:0] i;
        logic signed [31:0] a, b;
        logic signed [63:0] f;
        i = phase[31:20];
        a = sine(i);
        b = sine(i + 12'd1);
        f = 64'(phase[19:10]);
        lfo_sine = 64'(a) + rsh(64'(b - a) * f, 10);
    endfunction

    // ------------------------------------------------------------------ registers and lines

    logic [15:0] g05 [0:2047];
    function automatic logic [15:0] init(input int n, input int ch);
        // off = { 0x28, 0x2a, 0x38, 0x3a } bytes
        case (n)
            1: init = g05[ch * 64 + 8'h14];
            2: init = g05[ch * 64 + 8'h15];
            3: init = g05[ch * 64 + 8'h1c];
            default: init = g05[ch * 64 + 8'h1d];
        endcase
    endfunction
    function automatic logic [31:0] dword(input int ch, input int lo, input int hi);
        dword = {g05[ch * 64 + hi / 2], g05[ch * 64 + lo / 2]};
    endfunction

    // reflections 0-5, tails 0-1, allpasses [c][0..1]
    localparam int RMAX = 33 * STEP;           // refl[5] up to 33 x STEP
    localparam int TMAX = 34 * STEP;
    logic signed [31:0] rb [0:5][0:RMAX-1];
    logic signed [31:0] tb [0:1][0:TMAX-1];
    logic signed [31:0] ab [0:3][0:2*4*STEP+55-1];   // 0 = ap[0][0], 1 = ap[0][1], 2 = ap[1][0], 3 = ap[1][1]
    logic signed [31:0] cb [0:1][0:CHORUS_LEN-1];

    logic [15:0] r_len [0:5], r_pos [0:5];
    logic signed [31:0] r_lp [0:5], r_gain [0:5], r_fb [0:5];
    logic [15:0] t_len [0:1], t_pos [0:1];
    logic [15:0] a_len [0:3], a_pos [0:3];

    logic        active, stereo;
    logic signed [31:0] out_mix, ap_fb, damp, damp_state, refl_in, tail_ret;
    logic signed [31:0] ch_delay, ch_fb;
    logic signed [63:0] ch_depth;
    logic [31:0] ch_inc, phase;
    logic [14:0] cpos;

    // per-sample pipeline values
    logic [2:0]  ph;
    logic signed [31:0] p_in, p_dl, p_dr, p_t [0:1];

    function automatic logic [15:0] eff(input logic [15:0] pos, input logic [15:0] len);
        eff = (pos >= len) ? 16'd0 : pos;
    endfunction
    function automatic logic [15:0] nxt(input logic [15:0] p, input logic [15:0] len);
        nxt = (p + 16'd1 == len) ? 16'd0 : p + 16'd1;
    endfunction

    always_ff @(posedge clk) begin
        done <= 1'b0;
        if (rst) begin
            ph <= '0;
            active <= 1'b0; stereo <= 1'b0;
            out_mix <= '0; ap_fb <= '0; damp <= '0; damp_state <= '0; refl_in <= '0; tail_ret <= '0;
            ch_delay <= 32'sd1; ch_fb <= '0; ch_depth <= '0; ch_inc <= '0; phase <= '0; cpos <= '0;
            r_len[0] <= 16'(2 * STEP); r_len[1] <= 16'(4 * STEP); r_len[2] <= 16'(8 * STEP);
            r_len[3] <= 16'(13 * STEP); r_len[4] <= 16'(19 * STEP); r_len[5] <= 16'(26 * STEP);
            t_len[0] <= 16'(27 * STEP); t_len[1] <= 16'(27 * STEP);
            a_len[0] <= 16'(2 * 4 * STEP + 55); a_len[1] <= 16'(1 * 4 * STEP + 55);
            a_len[2] <= 16'(2 * 4 * STEP + 55); a_len[3] <= 16'(1 * 4 * STEP + 55);
            for (int i = 0; i < 6; i++) begin
                r_pos[i] <= '0; r_lp[i] <= '0; r_gain[i] <= '0; r_fb[i] <= '0;
                for (int k = 0; k < RMAX; k++) rb[i][k] <= '0;
            end
            for (int c = 0; c < 2; c++) begin
                t_pos[c] <= '0;
                for (int k = 0; k < TMAX; k++) tb[c][k] <= '0;
                for (int k = 0; k < CHORUS_LEN; k++) cb[c][k] <= '0;
            end
            for (int i = 0; i < 4; i++) begin
                a_pos[i] <= '0;
                for (int k = 0; k < 2 * 4 * STEP + 55; k++) ab[i][k] <= '0;
                rev_send[i] <= '0; cho_send[i] <= '0; dry[i][0] <= '0; dry[i][1] <= '0;
            end
            for (int i = 0; i < 2048; i++) g05[i] <= '0;
        end else begin
            if (wr_en)
                g05[wr_addr] <= (g05[wr_addr] & ~wr_mask) | (wr_data & wr_mask);

            // ---------------- fx_int::configure
            if (cfg) begin
                automatic int gain_n [0:5] = '{1, 1, 1, 2, 2, 2};
                automatic int gain_c [0:5] = '{8'h0f, 8'h17, 8'h1f, 8'h07, 8'h0f, 8'h17};
                automatic int fb_c   [0:5] = '{8'h09, 8'h11, 8'h19, 8'h01, 8'h09, 8'h11};
                automatic logic st = init(1, 7) == 16'h8474;
                automatic int tl = int'((init(2, 8'h14) >> 8) & 16'hf) + 18;
                automatic int tr = st ? int'((init(2, 8'h16) >> 8) & 16'hf) + 18 : tl;
                automatic int cd = int'(init(3, 8'h1a));
                active <= init(1, 0) != 16'h03ff;
                out_mix <= q256(8'(init(1, 3)));
                ap_fb <= q255(8'(init(1, 5)));
                stereo <= st;
                for (int i = 0; i < 6; i++) begin
                    r_gain[i] <= q15(4'(init(gain_n[i], gain_c[i]) >> 4));
                    r_fb[i] <= q15(4'(init(i < 3 ? 1 : 2, fb_c[i])));
                end
                damp <= q255(8'(init(2, 8'h1d)));
                refl_in <= q256(8'(init(3, 8'h01)));
                tail_ret <= q256(8'(init(4, 8'h1f)));
                if (r_len[5] != 16'(tl * STEP)) begin
                    r_len[5] <= 16'(tl * STEP);
                    for (int k = 0; k < RMAX; k++) rb[5][k] <= '0;
                end
                if (t_len[0] != 16'((tl + 1) * STEP)) begin
                    t_len[0] <= 16'((tl + 1) * STEP);
                    for (int k = 0; k < TMAX; k++) tb[0][k] <= '0;
                end
                if (t_len[1] != 16'((tr + 1) * STEP)) begin
                    t_len[1] <= 16'((tr + 1) * STEP);
                    for (int k = 0; k < TMAX; k++) tb[1][k] <= '0;
                end
                cd = (cd < 1) ? 1 : (cd > CHORUS_LEN - 4096) ? CHORUS_LEN - 4096 : cd;
                ch_delay <= 32'(cd);
                ch_fb <= q256(8'(init(3, 8'h17)));
                ch_depth <= (64'(init(4, 8'h0b) & 16'hff) * 64'(cd)) <<< 8;
                ch_inc <= 32'((64'(g05[10 * 64 + 13]) * 64'd99731) >> 10);
                for (int b = 0; b < 4; b++) begin
                    automatic logic [31:0] ptrx = dword(4 * b, 8'h16, 8'h14);
                    automatic logic [31:0] csl = dword(4 * b, 8'h76, 8'h74);
                    automatic logic [31:0] psst = dword(4 * b, 8'h66, 8'h64);
                    automatic logic [31:0] ptrx_r = dword(4 * b + 16, 8'h16, 8'h14);
                    rev_send[b] <= q255(ptrx[15:8]);
                    cho_send[b] <= q255(csl[31:24]);
                    dry[b][0] <= (b == 1 || b == 2) ? 32'sd32768 : q255(psst[31:24]);
                    dry[b][1] <= (b == 1 || b == 2) ? 32'sd32768 : q255(ptrx_r[7:0]);
                end
            end

            // ---------------- fx_int::process
            case (ph)
            3'd0: if (go) begin
                if (!active) begin
                    add_l <= '0;
                    add_r <= '0;
                    done <= 1'b1;
                end else begin
                    automatic logic signed [31:0] ds = sat(rsh(64'(rev_in) * 64'(32'sd32768 - damp) + 64'(damp_state) * 64'(damp), 15));
                    automatic logic signed [31:0] in2 = mul(ds, refl_in);
                    automatic logic signed [31:0] o [0:5];
                    // comb(): six reflection lines
                    for (int i = 0; i < 6; i++) begin
                        automatic logic [15:0] p = eff(r_pos[i], r_len[i]);
                        automatic logic signed [31:0] out = rb[i][p];
                        automatic logic signed [31:0] lp = sat(rsh(64'(out) * 64'(32'sd32768 - damp) + 64'(r_lp[i]) * 64'(damp), 15));
                        r_lp[i] <= lp;
                        rb[i][p] <= sat(64'(in2) - 64'(mul(lp, r_fb[i])));
                        r_pos[i] <= nxt(p, r_len[i]);
                        o[i] = mul(out, r_gain[i]);
                    end
                    damp_state <= ds;
                    p_in <= ds;
                    if (stereo) begin
                        p_dl <= sat(64'(o[2]) + 64'(o[4]));
                        p_dr <= sat(64'(o[0]) + 64'(o[1]) + 64'(o[3]) + 64'(o[5]));
                    end else begin
                        automatic logic signed [31:0] m = sat(64'(o[0]) + 64'(o[1]) + 64'(o[2]) + 64'(o[3]) + 64'(o[4]) + 64'(o[5]));
                        p_dl <= m;
                        p_dr <= m;
                    end
                    ph <= 3'd1;
                end
            end
            3'd1: begin   // tail(): delay line
                for (int c = 0; c < 2; c++) begin
                    automatic logic [15:0] p = eff(t_pos[c], t_len[c]);
                    automatic logic signed [31:0] tin = sat(64'(p_in) + 64'(c == 0 ? p_dl : p_dr));
                    p_t[c] <= tb[c][p];
                    tb[c][p] <= tin;
                    t_pos[c] <= nxt(p, t_len[c]);
                end
                ph <= 3'd2;
            end
            3'd2, 3'd3: begin   // diffuse(): ap[c][0], then ap[c][1]
                for (int c = 0; c < 2; c++) begin
                    automatic int a = 2 * c + (ph == 3'd3 ? 1 : 0);
                    automatic logic [15:0] p = eff(a_pos[a], a_len[a]);
                    automatic logic signed [31:0] held = ab[a][p];
                    automatic logic signed [31:0] w = sat(64'(mul(held, ap_fb)) - 64'(p_t[c]));
                    ab[a][p] <= w;
                    a_pos[a] <= nxt(p, a_len[a]);
                    p_t[c] <= sat(64'(held) - 64'(mul(w, ap_fb)));
                end
                ph <= ph + 3'd1;
            end
            3'd4: begin   // tail return, output mix, chorus
                automatic logic signed [31:0] dl = sat(64'(p_dl) + 64'(mul(p_t[0], tail_ret)));
                automatic logic signed [31:0] dr = sat(64'(p_dr) + 64'(mul(p_t[1], tail_ret)));
                automatic logic signed [63:0] lfo = rsh(lfo_sine(phase) * ch_depth, 16);
                automatic logic signed [63:0] lfo_r = rsh(lfo_sine(phase + 32'h40000000) * ch_depth, 16);
                automatic logic signed [31:0] cl, cr;
                for (int c = 0; c < 2; c++) begin
                    automatic logic signed [63:0] d = (64'(ch_delay) <<< 16) + (c == 0 ? lfo : lfo_r);
                    automatic logic signed [63:0] p = (64'(cpos) <<< 16) - d + (64'(CHORUS_LEN) <<< 16);
                    automatic logic [14:0] i0 = 15'(p >>> 16);
                    automatic logic [14:0] i1 = i0 + 15'd1;
                    automatic logic signed [63:0] f = p & 64'hffff;
                    automatic logic signed [31:0] tv = sat(rsh(64'(cb[c][i0]) * (64'sd65536 - f) + 64'(cb[c][i1]) * f, 16));
                    if (c == 0) cl = tv; else cr = tv;
                end
                cb[0][cpos] <= sat(64'(cho_in) + 64'(mul(cl, ch_fb)));
                cb[1][cpos] <= sat(64'(cho_in) + 64'(mul(cr, ch_fb)));
                cpos <= cpos + 15'd1;
                phase <= phase + ch_inc;
                add_l <= 64'(mul(dl, out_mix)) + 64'(cl);
                add_r <= 64'(mul(dr, out_mix)) + 64'(cr);
                done <= 1'b1;
                ph <= 3'd0;
            end
            default: ph <= 3'd0;
            endcase
        end
    end

endmodule
