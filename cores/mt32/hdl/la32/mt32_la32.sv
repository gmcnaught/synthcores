// SPDX-License-Identifier: GPL-2.0-or-later
// Roland LA32 sound chip, bit-exact to Nuked-MT32's la32.cpp (nukeykt, after John McMaster's
// decap), pinned commit 5b4e239. Tables mt32_la32_pow.hex and mt32_la32_logsin.hex are
// generated from that file by sim/sound/gen_la32_tables.py.
//
// One partial slot per slot_go; slot_done (one clock) with slot_idx when the slot's state,
// accumulators and interrupt are updated. The CPU writes (bus_we) only between slot_done and
// the next slot_go. A sample is slots 0-31; the accumulators of the sample before are on
// acc_prev* (clamped to 16 bits, as la32_t::clock leaves them) until the next slot 0 starts.
//
// PCM ROM: synthframe memory-port client (64-bit words, in-order responses, any latency);
// byte b of the ROM is bits [8*(b&7)+7 : 8*(b&7)] of word PCM_BASE + b/8.
module mt32_la32 #(
    parameter logic [28:0] PCM_BASE = 29'd0,
    parameter POW_HEX = "mt32_la32_pow.hex",
    parameter LOGSIN_HEX = "mt32_la32_logsin.hex"
) (
    input  logic        clk,
    input  logic        rst,

    input  logic        slot_go,
    output logic        slot_done,
    output logic [4:0]  slot_idx,

    input  logic        bus_we,
    input  logic [8:0]  bus_addr,
    input  logic [7:0]  bus_wdata,
    output logic [7:0]  bus_rdata,
    output logic        extint,

    output logic        pcm_req_valid,
    input  logic        pcm_req_ready,
    output logic [28:0] pcm_req_addr,
    output logic [1:0]  pcm_req_tag,
    input  logic        pcm_rsp_valid,
    input  logic [63:0] pcm_rsp_data,
    input  logic [1:0]  pcm_rsp_tag,

    output logic signed [15:0] acc_prev2, acc_prev3, acc_prev6, acc_prev7,

    // debug, valid at slot_done (the reference's --slot-log fields)
    output logic signed [31:0] dbg_oo, dbg_prev,
    output logic [7:0]  dbg_w46,
    output logic [25:0] dbg_ram2_0, dbg_ram2_1, dbg_ram2_2,
    output logic signed [31:0] dbg_acc2, dbg_acc3, dbg_acc6, dbg_acc7,
    output logic [14:0] dbg_w186, dbg_w187, dbg_w188
);
    // ---------------------------------------------------------------- helpers
    function automatic logic signed [31:0] mul(input logic [31:0] a, input logic [31:0] b);
        logic signed [21:0] pr;
        pr = $signed(a[13:0]) * $signed(b[7:0]);
        return {{10{pr[21]}}, pr};
    endfunction

    // calc_pow after the table read (rom = {powdrom, powrom} at v[11:3]), in two steps:
    // p = (powrom + ((powdrom * low) >> 3)) | 0x1000, then (p << hi) >> 9
    function automatic logic [12:0] pow_p(input logic [15:0] rom, input logic [15:0] v);
        logic [12:0] p;
        p = {1'b0, rom[11:0]} + 13'((7'(rom[15:12]) * 7'(v[2:0])) >> 3);
        return p | 13'h1000;
    endfunction
    function automatic logic [31:0] pow_sh(input logic [12:0] p, input logic [3:0] hi);
        logic [31:0] sh;
        sh = {19'd0, p} << hi;
        return sh >> 9;
    endfunction

    function automatic logic [6:0] wcfg(input logic [3:0] cfg);
        logic w278, w283, w284, w285, w286;
        w278 = cfg[0]; w283 = cfg[1]; w284 = cfg[2];
        w285 = !(w284 || w283);
        w286 = !(w284 && w283);
        return {!(w284 && w283 && w278), w286, !(w284 && w278) && w286, !w284,
                !(w283 && w278) && !w284, w285, !w278 && w285};
    endfunction

    function automatic logic signed [15:0] clamp16(input logic signed [31:0] v);
        if (v > 32'sd32767) return 16'sd32767;
        if (v < -32'sd32768) return -16'sd32768;
        return v[15:0];
    endfunction

    function automatic logic signed [31:0] sext15(input logic [31:0] v);
        return {{17{v[14]}}, v[14:0]};
    endfunction

    // 14-bit add with carry out, as the C's "t = a + b + 1; tc = (t >> 14) & 1; t &= 0x3fff"
    function automatic logic [14:0] add14(input logic [31:0] a, input logic [31:0] b, input logic one);
        logic [31:0] s;
        s = a + b + 32'(one);
        return s[14:0];
    endfunction

    // ---------------------------------------------------------------- tables
    logic [15:0] pow_rom [512];
    logic [12:0] logsin_rom [512];
    initial begin
        $readmemh(POW_HEX, pow_rom);
        $readmemh(LOGSIN_HEX, logsin_rom);
    end
    logic [15:0] pa_v, pb_v, pa_vq, pb_vq, pa_q, pb_q;
    logic [8:0]  la_a, lb_a;
    logic [12:0] la_q, lb_q;
    always_ff @(posedge clk) begin
        pa_q <= pow_rom[pa_v[11:3]];
        pb_q <= pow_rom[pb_v[11:3]];
        pa_vq <= pa_v;
        pb_vq <= pb_v;
        la_q <= logsin_rom[la_a];
        lb_q <= logsin_rom[lb_a];
    end
    // calc_pow results and logsin values, registered twice more: a lookup issued (address
    // presented) in stage X is used in stage X + 3
    logic [12:0] pa_p2, pb_p2;
    logic [3:0]  pa_h2, pb_h2;
    logic [31:0] pa_r, pb_r;
    logic [12:0] la_q2, lb_q2, la_qq, lb_qq;
    always_ff @(posedge clk) begin
        pa_p2 <= pow_p(pa_q, pa_vq); pb_p2 <= pow_p(pb_q, pb_vq);
        pa_h2 <= pa_vq[15:12]; pb_h2 <= pb_vq[15:12];
        pa_r <= pow_sh(pa_p2, pa_h2);
        pb_r <= pow_sh(pb_p2, pb_h2);
        la_q2 <= la_q; lb_q2 <= lb_q;
        la_qq <= la_q2; lb_qq <= lb_q2;
    end

    // ---------------------------------------------------------------- state
    logic [15:0] rf0 [32], rf1 [32], rf2 [32], rf3 [32], rf4 [32], rf5 [32];
    logic [25:0] r2_0 [32], r2_1 [32], r2_2 [32];
    logic [7:0]  reg_1c0, reg_1c1, reg_1c2, reg_1c3, reg_data_l;
    logic [7:0]  int_status;
    logic        int_state;
    logic [7:0]  w46;
    logic [14:0] w186, w187, w188;
    logic signed [31:0] prev;
    logic signed [31:0] acc [2][4];       // accum[parity][2,3,6,7]
    logic        oddeven, started;
    logic [4:0]  slot;                    // next slot to compute

    assign extint = int_state;
    assign bus_rdata = ((bus_addr & 9'h1c0) != 9'h1c0) ? int_status : 8'hff;
    assign acc_prev2 = acc[!oddeven][0][15:0];
    assign acc_prev3 = acc[!oddeven][1][15:0];
    assign acc_prev6 = acc[!oddeven][2][15:0];
    assign acc_prev7 = acc[!oddeven][3][15:0];

    typedef enum logic [4:0] {
        ST_IDLE, ST_TVW, ST_TVW2, ST_TV, ST_WGW, ST_WGW2, ST_WG1, ST_WG2, ST_PREQ1, ST_PREQ2, ST_PWAIT,
        ST_W3, ST_W3W, ST_W3W2, ST_N2W, ST_N2W2, ST_N3A, ST_N3A2, ST_N3B, ST_N3C, ST_N3D, ST_N4, ST_N5, ST_N5W,
        ST_N5W2, ST_G0, ST_G1, ST_G2, ST_ACC1, ST_ACC1B, ST_ACC2, ST_WG3
    } st_t;
    st_t st;

    // slot-stage registers
    logic [4:0]  i;
    logic [15:0] rf0i, rf1i, rf2i, rf3i, rf4i;
    logic [25:0] r20i, r21i, r22i;
    logic [25:0] tvv0, tvv1;
    logic [31:0] phase;
    logic        sign_flip;

    wire [15:0] ctrl = rf4i;
    wire        mode = ctrl[6];
    wire        ring = ctrl[5];
    wire [1:0]  outch = ctrl[4:3];
    wire [7:0]  pan1 = (ctrl[2] ? 8'h49 : 8'h00) | (ctrl[1] ? 8'h24 : 8'h00) | (ctrl[0] ? 8'h12 : 8'h00);

    // ================================================================ ST_IDLE: w46 sequence
    logic        w49, w50, w51, w52, w53, w63;
    logic [1:0]  w47, w48;
    logic [7:0]  w46n;
    always_comb begin : b_c1
        logic w64, w65, w66, w67, w68, w69, w70, w71, w72, w73, bitv;
        logic [4:0] ii;
        ii = slot;
        case (ii[4:3])
            2'd0: begin w50 = reg_1c0[0]; w49 = reg_1c0[1]; end
            2'd1: begin w50 = reg_1c0[2]; w49 = reg_1c0[3]; end
            2'd2: begin w50 = reg_1c0[4]; w49 = reg_1c0[5]; end
            default: begin w50 = reg_1c0[6]; w49 = reg_1c0[7]; end
        endcase
        case (reg_1c1[5:4])
            2'd0: begin w51 = !w50; w52 = !w49; w53 = 1'b1; end
            2'd1: begin w51 = !reg_1c1[2]; w52 = !reg_1c1[3]; w53 = (ii & 5'd24) == 0; end
            2'd2: begin w51 = !reg_1c1[2]; w52 = !reg_1c1[3]; w53 = (ii & 5'd8) == 0; end
            default: begin w51 = reg_1c1[2]; w52 = !(reg_1c1[2] ^ reg_1c1[3]); w53 = (ii & 5'd16) == 0; end
        endcase
        w47 = reg_1c1[3:2];
        if (w53) w47 = 2'd3;
        w48 = w47 ^ 2'd3;
        if (w49) w48 = 2'd0;
        w65 = ii[1] || ii[0];
        w66 = ii[2] || ii[1] || ii[0];
        w67 = w47 == 2'd2;
        w68 = w47 == 2'd3;
        w69 = w47 == 2'd1;
        w64 = !((w65 && w67) || w68 || (ii[0] && w69));
        w71 = w51 && !w52;
        w72 = !w51 && w52;
        w73 = !w51 && !w52;
        w70 = !((w72 && ii[0]) || (w71 && w65) || (w73 && w66));
        w63 = !((w64 && !w53) || (w70 && w53));
        case (w48)
            2'd0: bitv = rf3[ii][15:12] == 4'hf;
            2'd1: bitv = w46[1];
            2'd2: bitv = w46[3];
            default: bitv = w46[7];
        endcase
        w46n = w63 ? w46 : {w46[6:0], bitv};
    end

    // ================================================================ ST_TV: envelopes
    logic [25:0] tv_val [2];
    logic        tv_status [2];
    always_comb begin : b_c2
        logic [15:0] rf;
        logic [31:0] p, sum, val2;
        logic [7:0] dest, sum_cmp;
        logic s, zero, cmp, neg, stop, status;
        for (int tv = 0; tv < 2; tv++) begin
            rf = (tv != 0) ? rf2i : rf0i;
            dest = rf[15:8];
            p = (tv != 0) ? pb_r : pa_r;
            s = rf[7];
            zero = rf[6:0] == 0;
            if (zero) p = p & ~32'd8;
            else if (s) p = p ^ 32'h3ffffff;
            sum = 32'((tv != 0) ? r21i : r20i) + p;
            sum_cmp = sum[25:18];
            cmp = sum_cmp >= dest;
            neg = p[25];
            if (neg) stop = !sum[26] || !cmp;
            else stop = sum[26] || cmp;
            status = !w46[0] && !zero && stop;
            val2 = sum & 32'hffff;
            if (!status && !w46[0]) val2 = val2 | (sum & 32'h3ff0000);
            if (tv == 1 && w46[0]) val2 = val2 | (32'(reg_1c3 & 8'hf0) << 18);
            if (status) begin
                val2 = val2 | (32'(dest) << 18);
                val2 = val2 & ~32'hffff;
            end
            tv_val[tv] = val2[25:0];
            tv_status[tv] = status;
        end
    end

    // ================================================================ ST_WG: phase, branch set-up
    logic [31:0] ph_new;                  // phase after the pitch step
    logic        sf;                      // sign_flip
    // wave (PCM) branch
    logic [31:0] wv_t27, wv_t22;
    logic [19:0] wa1, wa2;
    logic        wv_end, wv_end2;
    // synthesis branch
    logic [31:0] nw_t5, nw_t12, nw_t31, nw_t32, nw_l1, nw_m1, nw_ix1, nw_e1, nw_e2;
    logic [8:0]  nw_ix1q, nw_ix2;
    logic        nw_w467;
    always_comb begin : b_c3
        logic [31:0] t10, t17, t14, t19, ph, ph2;
        logic [6:0] w271, w271b;
        logic w282, w282b;
        logic [7:0] base_l, base_h;
        logic [4:0] s2;
        logic w59, w60, w61, w62;
        logic [31:0] t1, t2, t3, t4, t6, t7, t8, t9, t11, t13, t18, t19n, t29, t30, l5, l6, e2b;
        logic [14:0] a15;

        ph_new = 32'(r22i) + pa_r;
        if (w46[0]) ph_new = 32'd0;

        t10 = 32'(tvv0) >> 12;
        t17 = 32'(tvv1) >> 12;

        s2 = i + 5'd2;
        w60 = (s2 & 5'd30) != 0;
        w61 = (s2 & 5'd16) == 0 || (s2 & 5'd14) == 0;
        w59 = !((w60 && w61) || !reg_1c1[5] || !reg_1c1[4]);
        w62 = s2[2] ^ s2[1];
        sf = !((w59 || w62) && (!w59 || s2[1]));

        // ---- PCM wave
        a15 = add14(t10, 32'h3ffe, 1'b1);
        t14 = 32'(a15[13:0]);
        wv_t27 = t14 & 32'h1fff;
        if (!a15[14]) wv_t27 = 32'd0;
        if (a15[14] && t14[13]) wv_t27 = wv_t27 | 32'h2000;
        a15 = add14(t17, 32'h3ffe, 1'b1);
        t19 = 32'(a15[13:0]);
        wv_t22 = t19 & 32'h1fff;
        if (!a15[14]) wv_t22 = 32'd0;
        if (a15[14] && t19[13]) wv_t22 = wv_t22 | 32'h2000;

        ph = (phase >> 8) & 32'h3ffff;
        w271 = wcfg(ctrl[15:12]);
        w282 = ctrl[15];
        wv_end = ((ph[17:11] & w271) != 0) && !w282;
        base_l = rf1i[7:0];
        base_h = rf1i[15:8];
        wa1 = {8'd0, ph[10:0], 1'b0} | (20'(w271 & base_h[6:0]) << 12) | (20'(7'(w271 ^ 7'h7f) & ph[17:11]) << 12);
        if (base_h[7]) wa1 = wa1 | 20'h80000;
        if (mode) begin
            ph2 = (ph + 32'd1) & 32'h3ffff;
            w271b = w271;
            w282b = w282;
        end else begin
            ph2 = ph;
            w271b = wcfg(ctrl[11:8]);
            w282b = ctrl[11];
        end
        wv_end2 = ((ph2[17:11] & w271b) != 0) && !w282b;
        wa2 = {8'd0, ph2[10:0], 1'b0} | (20'(7'(w271b ^ 7'h7f) & ph2[17:11]) << 12);
        if (mode) begin
            wa2 = wa2 | (20'(w271b & base_h[6:0]) << 12);
            if (base_h[7]) wa2 = wa2 | 20'h80000;
        end else begin
            wa2 = wa2 | (20'(w271b & base_l[6:0]) << 12);
            if (base_l[7]) wa2 = wa2 | 20'h80000;
        end

        // ---- synthesis
        t1 = (32'(rf1i) << 6) & 32'h3fc0;
        t2 = (32'(w186) >> 1) & 32'h3ffe;
        a15 = add14(t1, t2, 1'b0);
        t3 = 32'(a15[13:0]);
        t4 = t3 & 32'h1fff;
        if (a15[14] && t3[13]) t4 = t4 | 32'h1fff;
        if (!a15[14] && !t3[13]) t4 = 32'd0;
        if (a15[14]) t4 = t4 | 32'h2000;
        nw_t5 = (t4 >> 2) | 32'h2000;

        t6 = (32'(rf1i) >> 8) << 6;
        t7 = (32'(w187) >> 1) & 32'h3ffe;
        a15 = add14(t6, t7, 1'b0);
        t8 = 32'(a15[13:0]);
        t9 = t8 & 32'h1fff;
        if (a15[14] && t8[13]) t9 = t9 | 32'h1fff;
        if (!a15[14] && !t8[13]) t9 = 32'd0;
        if (a15[14]) t9 = t9 | 32'h2000;

        a15 = add14(t9, t10 & 32'h3fff, 1'b0);
        t11 = 32'(a15[13:0]);
        nw_t31 = t11 & 32'h1fff;
        if (t11[13] || a15[14]) nw_t31 = 32'h1fff;
        nw_t12 = t11 & 32'h1fff;
        if (a15[14] || (t11 & 32'h3c00) == 32'h3c00) nw_t12 = 32'h1bff;
        if (!a15[14] && !t11[13]) nw_t12 = 32'd0;
        nw_t12 = nw_t12 | 32'h2000;

        nw_e1 = ((nw_t12 & 32'h1fff) << 2) | 32'h8000;

        t13 = nw_t5 ^ 32'h3fff;
        a15 = add14(t13, nw_t12, 1'b1);
        e2b = a15[14] ? (~nw_t5 & 32'hfff) : (~nw_t12 & 32'hfff);
        e2b = e2b | 32'h1000;
        nw_e2 = (e2b << 2) | 32'h8003;

        t18 = (ring && i[0]) ? 32'h7ffe : 32'(w188);
        t18 = (t18 >> 1) & 32'h3ffe;
        a15 = add14(t17 & 32'h3fff, t18, 1'b1);
        t19n = 32'(a15[13:0]);
        t30 = t19n & 32'h1fff;
        if (!a15[14]) t30 = 32'd0;
        if (a15[14] && t19n[13]) t30 = t30 | 32'h2000;

        nw_m1 = (phase >> 6) & 32'hfff;
        if (phase[18]) nw_m1 = nw_m1 | 32'h3000;

        a15 = add14(t30, nw_t31 << 1, 1'b1);
        t29 = 32'(a15[13:0]);
        nw_t32 = t29 & 32'h1fff;
        if (!a15[14]) nw_t32 = 32'd0;
        if (a15[14] && t29[13]) nw_t32 = nw_t32 | 32'h2000;

        nw_l1 = (~phase >> 3) & 32'hffff;
        nw_ix1 = (nw_l1 >> 5) & 32'h7ff;
        if (nw_ix1[9]) nw_ix1 = nw_ix1 ^ 32'h400;
        nw_ix1 = nw_ix1 ^ 32'h200;
        if (!mode) nw_ix1 = nw_ix1 & ~32'h400;
        nw_w467 = nw_ix1[10];
        nw_ix1q = nw_ix1[8:0];
        if (nw_ix1[9]) nw_ix1q = nw_ix1q ^ 9'h1ff;

        l5 = ~nw_t12 & 32'h1fff;
        l6 = 32'h1ff;
        if ((l5 & 32'h1c00) == 32'h1c00) l6 = (~l5 >> 1) & 32'h1ff;
        nw_ix2 = l6[8:0];
    end

    // registered results of ST_WG
    logic [31:0] r_t27, r_t22, r_t5, r_t12, r_t32, r_l1, r_m1;
    logic        r_w467, r_end, r_end2;
    logic [15:0] r_e1, r_e2;
    logic [8:0]  r_ix1q, r_ix2;
    logic [19:0] r_wa1, r_wa2;
    logic [7:0]  s1l, s1h, s2l, s2h;
    logic [1:0]  pcm_need, pcm_got;       // [0] wa1, [1] wa2

    // ================================================================ PCM stages
    logic [31:0] w_v1, w_v2;               // calc_pow arguments for w1, w2
    logic        w_zero1, w_zero2, w_sign1, w_sign2;
    always_comb begin : b_c4
        logic [31:0] s1d, s2d, t28, t24, t26;
        logic [14:0] a15;
        w_sign1 = s1l[7];
        s1d = 32'd0;
        if (!r_end) begin
            if (reg_1c1[6]) s1d = 32'(s1h[5:0]) | (32'(s1l[6:0]) << 6) | (32'(s1h[6]) << 13);
            else if (s1l[5:0] != 0) s1d = 32'h2000;
        end
        a15 = add14(s1d, r_t27, 1'b1);
        t24 = a15[14] ? 32'(a15[13:0]) : 32'd0;
        w_zero1 = (a15[13:11] == 0 || !a15[14]) && mode;
        w_v1 = t24 << 2;

        w_sign2 = s2l[7];
        s2d = 32'd0;
        if (!r_end2) begin
            if (reg_1c1[6]) s2d = 32'(s2h[5:0]) | (32'(s2l[6:0]) << 6) | (32'(s2h[6]) << 13);
            else if (s2l[5:0] != 0) s2d = 32'h2000;
        end
        t28 = mode ? r_t27 : r_t22;
        a15 = add14(s2d, t28, 1'b1);
        t26 = a15[14] ? 32'(a15[13:0]) : 32'd0;
        w_zero2 = a15[13:11] == 0 || !a15[14];
        w_v2 = t26 << 2;
    end
    logic        r_zero1, r_zero2, r_sign1, r_sign2;

    // ================================================================ combine (both branches)
    // ST_G1: w1, w2 (registered pow) with their signs -> the PCM interpolation sum, or the
    // square/saw sum ot3 + (ot4 >> 1) + c (= ot7 of the ring modulator)
    logic signed [31:0] g1_v;
    always_comb begin : b_g1
        logic signed [31:0] o1, o2, ot3, ot4, c;
        logic [31:0] w1, w2;
        logic [6:0] interp;
        interp = phase[7:1];
        w1 = r_w1;
        w2 = r_w2;
        o1 = '0; o2 = '0; ot3 = '0; ot4 = '0; c = '0;
        if (ctrl[7] && mode) begin
            o1 = mul(w1 >> 6, 32'(interp ^ 7'h7f)) >>> 7;
            o2 = mul(w2 >> 6, 32'(interp)) >>> 7;
            g1_v = o1 + o2;
        end else begin
            o1 = w1 >> 5;
            o2 = w2 >> 5;
            ot4 = sext15(o1);
            ot3 = sext15(o2);
            c = 32'(ot4[0]);
            g1_v = ot3 + (ot4 >>> 1) + c;
        end
    end
    logic signed [31:0] r_g1;
    logic [31:0] r_w1, r_w2;                 // ST_G0: w1, w2 with their signs applied
    // ST_G2: ring modulation (not for PCM in interpolation mode)
    logic signed [31:0] g2_oo;
    always_comb begin : b_g2
        logic signed [31:0] ot5, ot6;
        logic [31:0] ot8;
        ot8 = 32'({r_g1[13], r_g1[13:7]});
        ot5 = mul(prev, ot8) >>> 6;
        ot6 = mul(prev, 32'(r_g1[6:0])) >>> 13;
        if (!(ctrl[7] && mode) && ring) g2_oo = ot5 + ot6;
        else g2_oo = r_g1;
    end

    // ================================================================ synthesis stages
    // ST_N3A: p1, p2 (pow), ls1, ls2 (logsin) -> m2..m5, t34, t52
    logic signed [31:0] n3a_m2, n3a_m3, n3a_m4, n3a_m5;
    logic [31:0] n3a_l2, n3a_t34, n3a_t52, n3a_p1lo, n3a_p1hi;
    always_comb begin : b_n3a
        logic [31:0] t21, p1lo, p1hi, l7, l8, t37, t38, t50;
        logic [14:0] a15;
        t21 = (pb_r >> 4) & 32'h7fff;
        n3a_l2 = (r_l1 + t21 + 32'h10001) & 32'h1ffff;
        p1lo = (pa_r >> 4) & 32'd127;
        p1hi = (pa_r >> 11) & 32'd127;
        n3a_p1lo = p1lo;
        n3a_p1hi = p1hi;
        n3a_m2 = mul(r_m1, p1lo);
        n3a_m3 = mul(r_m1, p1hi);
        n3a_m4 = mul(r_l2 >> 3, r_p1lo);      // ST_N3A2, from the registered l2
        n3a_m5 = mul(r_l2 >> 3, r_p1hi);

        l7 = 32'(la_qq) ^ 32'h3fff;
        l8 = mode ? l7 : 32'h3fff;
        t37 = 32'(ctrl[12:8]) << 8;
        if (ctrl[12:8] != 0) t37 = t37 | 32'h2000;
        t38 = 32'(lb_qq) ^ 32'h3fff;
        a15 = add14(t37, t38, 1'b1);
        t50 = a15[14] ? 32'(a15[13:0]) : 32'd0;
        a15 = add14(l8, r_t32, 1'b1);
        n3a_t34 = a15[14] ? 32'(a15[13:0]) : 32'd0;
        a15 = add14(t50, n3a_t34, 1'b1);
        n3a_t52 = a15[14] ? 32'(a15[13:0]) : 32'd0;
    end
    logic signed [31:0] r_m2, r_m3, r_m4, r_m5;
    logic [31:0] r_l2, r_t34, r_t52, r_p1lo, r_p1hi;

    // ST_N3B: l3, l4, the w4xx flags, logsin lookups ix3/ix4, t44, t45
    logic [31:0] n3b_t44, n3b_t45;
    logic        w326, w402, w407, w413, w417, w418, w419, w429, w430, w464;
    logic [8:0]  n3b_ix3, n3b_ix4;
    always_comb begin : b_n3b
        logic [31:0] ix3, ix4, l3, l4, l3_, l4_, w348, w518;
        logic l3c, l4c, l3c_, l4c_, w403, w404, w408, w409, w352, w353, w354, w355;
        logic [7:0] rom8;
        w352 = 1'b0; w353 = 1'b0; w354 = 1'b0; w355 = 1'b0; w518 = '0;
        l3 = (32'(r_m2 >>> 8) & 32'hffff) + (32'(r_m3 >>> 1) & 32'hffff);
        l3c = l3[16];
        l3 = l3 & 32'hffff;
        l3_ = (32'(r_m2 >>> 8) & 32'h3ffff) + (32'(r_m3 >>> 1) & 32'h3ffff);
        l3c_ = l3_[18];
        l3_ = l3_ & 32'h3ffff;
        l4 = (32'(r_m4 >>> 8) & 32'hffff) + (32'(r_m5 >>> 1) & 32'hffff);
        l4c = l4[16];
        l4 = l4 & 32'hffff;
        l4_ = (32'(r_m4 >>> 8) & 32'h3ffff) + (32'(r_m5 >>> 1) & 32'h3ffff);
        l4c_ = l4_[18];
        l4_ = l4_ & 32'h3ffff;

        w403 = !(l3c || (l3 & 32'hfc00) != 0 || (32'(r_m3 >>> 8) & 32'h1e00) != 0);
        w408 = !(!l3c && !(((32'(r_m3 >>> 8) & 32'h1000) != 0) ^ ((32'(r_m2 >>> 8) & 32'h8000) != 0)))
               && (32'(r_m3 >>> 8) & 32'h1e00) == 32'h1e00 && (l3 & 32'hfc00) == 32'hfc00;
        w404 = !(l4c || (l4 & 32'hfc00) != 0 || (32'(r_m5 >>> 8) & 32'h1e00) != 0);
        w409 = !(!l4c && !(((32'(r_m5 >>> 8) & 32'h1000) != 0) ^ ((32'(r_m4 >>> 8) & 32'h8000) != 0)))
               && (32'(r_m5 >>> 8) & 32'h1e00) == 32'h1e00 && (l4 & 32'hfc00) == 32'hfc00;
        w326 = r_l2[16];
        w417 = !(w326 || w403);
        w402 = !(!w326 && w403);
        w419 = !(w408 || !w326);
        w407 = !(w326 && w408);
        w418 = !(w326 || w404);
        w429 = !(!w326 && w404);
        w413 = !(!w326 || w409);
        w430 = !(w326 && w409);

        ix3 = (l3 >> 1) & 32'h7ff;
        if (ix3[9]) ix3 = ix3 ^ 32'h1ff;
        ix4 = (l4 >> 1) & 32'h7ff;
        if (ix4[9]) ix4 = ix4 ^ 32'h1ff;
        n3b_ix3 = ix3[8:0];
        n3b_ix4 = ix4[8:0];
        if (w326) w464 = r_w467 ^ !ix4[10];
        else w464 = r_w467 ^ !ix3[10];

        case (ctrl[15:13])
            3'd0: rom8 = 8'd127; 3'd1: rom8 = 8'd64; 3'd2: rom8 = 8'd48; 3'd3: rom8 = 8'd32;
            3'd4: rom8 = 8'd20;  3'd5: rom8 = 8'd12; 3'd6: rom8 = 8'd8;  default: rom8 = 8'd4;
        endcase
        n3b_t44 = 32'(rom8);
        if (w326) n3b_t44 = n3b_t44 ^ 32'd255;
        if (w326) begin
            n3b_t45 = (l4_ >> 5) & 32'h1ff;
            w348 = 32'(r_m5 >>> 8) & 32'hffff;
            w352 = !(w348[12] ^ ((32'(r_m4 >>> 8) & 32'h8000) != 0));
            w353 = !(w352 && !l4c_);
            w354 = w353 && w348[11];
            w518 = w354 ? ((l4_ >> 14) & 32'd15) : 32'd0;
            n3b_t45 = n3b_t45 | (w518 << 9) | 32'h2000;
        end else begin
            n3b_t45 = (l3_ >> 5) & 32'h1ff;
            w348 = 32'(r_m3 >>> 8) & 32'hffff;
            w355 = !(l3c_ || w348[11]);
            w518 = w355 ? ((l3_ >> 14) & 32'd15) : 32'd15;
            n3b_t45 = n3b_t45 | (w518 << 9);
        end
    end
    logic [31:0] r_t44, r_t45, r_t43;
    logic        r_w326, r_w402, r_w407, r_w413, r_w417, r_w418, r_w419, r_w429, r_w430, r_w464;

    // ST_N3C: t42 = mul(t45, t44) -> t43
    logic [31:0] n3c_t43;
    always_comb begin : b_n3c
        logic [31:0] t42;
        t42 = 32'(mul(r_t45, r_t44));
        n3c_t43 = (~t42 >> 1) & 32'h3ff;
        if (t42 < 32'h8000) n3c_t43 = n3c_t43 | ((~t42 >> 1) & 32'h3c00);
    end

    // ST_N4: ls3, ls4 -> t53, t56, t48
    logic [31:0] n4_t53, n4_t56, n4_t48;
    always_comb begin : b_n4
        logic [31:0] t40, t55, ls3, ls4;
        logic [14:0] a15;
        ls3 = 32'(la_qq);
        ls4 = 32'(lb_qq);
        t40 = 32'd0;
        if (!r_w407 || !r_w402) t40 = (ls3 ^ 32'h1fff) << 1;
        if (r_w417) t40 = ls3 ^ 32'h3fff;
        if (r_w419) t40 = 32'h3fff;
        a15 = add14(r_t52, t40, 1'b1);
        n4_t53 = a15[14] ? 32'(a15[13:0]) : 32'd0;
        t55 = 32'd0;
        if (!(r_w429 && r_w430)) t55 = t55 | (ls4 ^ 32'h3fff);
        if (!(r_w402 && r_w407)) t55 = t55 | (ls3 ^ 32'h3fff);
        if ((r_w417 && r_w418) || (r_w419 && r_w413)) t55 = t55 | 32'h3fff;
        a15 = add14(r_t34, t55, 1'b1);
        n4_t56 = a15[14] ? 32'(a15[13:0]) : 32'd0;
        n4_t48 = 32'd0;
        if (!(r_w430 && r_w429)) n4_t48 = (ls4 ^ 32'h1fff) << 1;
        if (r_w418) n4_t48 = n4_t48 | 32'h3fff;
        if (r_w413) n4_t48 = ls4 ^ 32'h3fff;
    end
    logic [31:0] r_t53, r_t56, r_t48;

    // ST_N5: t41 -> t46 -> t47 -> t49, zero
    logic [31:0] n5_t49;
    logic        n5_zero;
    always_comb begin : b_n5
        logic [31:0] t46;
        logic [14:0] a15;
        a15 = add14(r_t53, r_t43, 1'b1);
        t46 = a15[14] ? 32'(a15[13:0]) : 32'd0;
        a15 = add14(t46, r_t48, 1'b1);
        n5_t49 = a15[14] ? 32'(a15[13:0]) : 32'd0;
        n5_zero = a15[13:11] == 0 || !a15[14];
    end

    // ================================================================ ST_ACC1/2: pan, accumulate, rf5
    logic signed [31:0] r_oo;
    logic signed [31:0] acc1_lo, acc1_hi, r_add_lo, r_add_hi;
    logic        acc_en_lo, acc_en_hi;
    logic        rf5_we;
    logic [4:0]  rf5_wa;
    logic [15:0] rf5_wd;
    logic        latch_w;
    logic [4:0]  latch_a;
    logic        r_rf5_we, r_latch_w;
    logic [4:0]  r_rf5_wa, r_latch_a;
    logic [15:0] r_rf5_wd;
    // ST_ACC1: ot1 = mul(oo, pan1), the rf5 counter operands; ST_ACC1B: sums; ST_ACC2: writes
    logic signed [31:0] r_ot1;
    logic [31:0] acc0_c1, acc0_t2;
    logic [31:0] r_c1, r_t2;
    always_comb begin : b_acc
        logic signed [31:0] c;
        logic [31:0] t1, t3, t4, t5;
        logic [4:0] pv, a;
        acc1_lo = r_ot1 >>> 7;
        c = 32'((~r_ot1 >>> 6) & 32'sd1);
        acc1_hi = r_oo + (~r_ot1 >>> 7) + c;
        acc_en_lo = !(ring && !i[0]) && outch[1];
        acc_en_hi = !(ring && !i[0]) && outch[1] && (ctrl[2:0] != 3'd7);

        rf5_we = i[2:0] == 3'd1 || i[2:0] == 3'd2 || i[2:0] == 3'd3;
        t1 = ~(r_c1 >> 1) & 32'h3fff;
        t3 = t1 + r_t2 + 32'd1;
        t4 = (t3 >> 6) & 32'hff;
        if (!t3[14]) t4 = t4 | 32'hff00;
        t5 = r_c1 + t4;
        if (t3[14]) t5 = t5 + 32'd1;
        rf5_wa = 5'd4 | (i & 5'h1b);
        rf5_wd = t5[15:0] & 16'h7fff;
        pv = i - 5'd1;
        a = pv & 5'h1b;
        acc0_c1 = (i[2:0] == 3'd1) ? 32'(w186) : (i[2:0] == 3'd2) ? 32'(w187) : 32'(w188);
        acc0_t2 = (32'(rf5[a]) >> 2) & 32'h3ffe;

        latch_w = i[2:0] == 3'd7;
        latch_a = 5'd4 | ((i[4] ^ i[3]) ? 5'h10 : 5'h0) | (!i[3] ? 5'h8 : 5'h0);
    end

    // ================================================================ ROM address selection
    always_comb begin
        pa_v = 16'd0; pb_v = 16'd0; la_a = 9'd0; lb_a = 9'd0;
        case (st)
            ST_IDLE: begin
                pa_v = (rf0[slot] << 9);
                pb_v = (rf2[slot] << 9);
            end
            ST_TV: pa_v = rf3i;
            ST_WG3: begin
                pa_v = r_e1;
                pb_v = r_e2;
                la_a = r_ix1q;
                lb_a = r_ix2;
            end
            ST_W3: begin
                pa_v = w_v1[15:0];
                pb_v = w_v2[15:0];
            end
            ST_N3B: begin
                la_a = n3b_ix3;
                lb_a = n3b_ix4;
            end
            ST_N5: begin
                pa_v = {r_t56[13:0], 2'b00};
                pb_v = {n5_t49[13:0], 2'b00};
            end
            default: ;
        endcase
    end

    // ================================================================ PCM requests
    wire [19:0] cur_wa = (st == ST_PREQ1) ? r_wa1 : r_wa2;
    assign pcm_req_valid = (st == ST_PREQ1 && pcm_need[0]) || (st == ST_PREQ2 && pcm_need[1]);
    assign pcm_req_addr = PCM_BASE + 29'(cur_wa[18:3]);
    assign pcm_req_tag = (st == ST_PREQ1) ? 2'd0 : 2'd1;

    function automatic logic [15:0] pick16(input logic [63:0] w, input logic [2:0] off);
        logic [63:0] s;
        s = w >> {off, 3'b000};
        return s[15:0];                    // {byte off+1, byte off}
    endfunction

    // ================================================================ sequencer
    always_ff @(posedge clk) begin
        slot_done <= 1'b0;
        if (rst) begin
            st <= ST_IDLE;
            slot <= 5'd0;
            oddeven <= 1'b0;
            started <= 1'b0;
            reg_1c0 <= 8'd0; reg_1c1 <= 8'd0; reg_1c2 <= 8'd0; reg_1c3 <= 8'd0; reg_data_l <= 8'd0;
            int_status <= 8'd0;
            int_state <= 1'b0;
            w46 <= 8'd0;
            w186 <= 15'd0; w187 <= 15'd0; w188 <= 15'd0;
            prev <= 32'sd0;
            for (int p = 0; p < 2; p++)
                for (int k = 0; k < 4; k++) acc[p][k] <= 32'sd0;
            for (int k = 0; k < 32; k++) begin
                rf0[k] <= 16'd0; rf1[k] <= 16'd0; rf2[k] <= 16'd0; rf3[k] <= 16'd0; rf4[k] <= 16'd0; rf5[k] <= 16'd0;
                r2_0[k] <= 26'd0; r2_1[k] <= 26'd0; r2_2[k] <= 26'd0;
            end
            pcm_got <= 2'd0;
        end else begin
            // CPU writes (la32_t::write)
            if (bus_we) begin
                if ((bus_addr & 9'h1c0) == 9'h1c0) begin
                    case (bus_addr[1:0])
                        2'd0: reg_1c0 <= bus_wdata;
                        2'd1: reg_1c1 <= bus_wdata;
                        2'd2: reg_1c2 <= bus_wdata;
                        default: reg_1c3 <= bus_wdata & 8'hf3;
                    endcase
                end else if (!bus_addr[0])
                    reg_data_l <= bus_wdata;
                else begin
                    case (bus_addr[8:6])
                        3'd0: rf0[bus_addr[5:1]] <= {bus_wdata, reg_data_l};
                        3'd1: rf1[bus_addr[5:1]] <= {bus_wdata, reg_data_l};
                        3'd2: rf2[bus_addr[5:1]] <= {bus_wdata, reg_data_l};
                        3'd3: rf3[bus_addr[5:1]] <= {bus_wdata, reg_data_l};
                        3'd4: rf4[bus_addr[5:1]] <= {bus_wdata, reg_data_l};
                        3'd5: rf5[bus_addr[5:1]] <= {bus_wdata, reg_data_l};
                        default: ;
                    endcase
                    int_state <= 1'b0;
                end
            end

            // PCM responses (any state)
            if (pcm_rsp_valid) begin
                if (pcm_rsp_tag == 2'd0) begin
                    {s1h, s1l} <= pick16(pcm_rsp_data, r_wa1[2:0]);
                    pcm_got[0] <= 1'b1;
                end else begin
                    {s2h, s2l} <= pick16(pcm_rsp_data, r_wa2[2:0]);
                    pcm_got[1] <= 1'b1;
                end
            end

            case (st)
                ST_IDLE: if (slot_go) begin
                    i <= slot;
                    if (slot == 5'd0) begin
                        // memset(accum[oddeven]) at the start of a sample
                        if (started) begin
                            oddeven <= !oddeven;
                            for (int k = 0; k < 4; k++) acc[!oddeven][k] <= 32'sd0;
                        end else
                            for (int k = 0; k < 4; k++) acc[oddeven][k] <= 32'sd0;
                        started <= 1'b1;
                    end
                    w46 <= w46n;
                    rf0i <= rf0[slot]; rf1i <= rf1[slot]; rf2i <= rf2[slot]; rf3i <= rf3[slot]; rf4i <= rf4[slot];
                    r20i <= r2_0[slot]; r21i <= r2_1[slot]; r22i <= r2_2[slot];
                    st <= ST_TVW;
                end
                ST_TVW: st <= ST_TVW2;
                ST_TVW2: st <= ST_TV;
                ST_TV: begin
                    tvv0 <= tv_val[0];
                    tvv1 <= tv_val[1];
                    r2_0[i] <= tv_val[0];
                    r2_1[i] <= tv_val[1];
                    if (tv_status[0] && !int_state) begin
                        int_state <= 1'b1;
                        int_status <= 8'h80 | 8'(i);
                    end else if (tv_status[1] && !int_state) begin
                        int_state <= 1'b1;
                        int_status <= 8'h80 | 8'(i) | 8'h20;
                    end
                    st <= ST_WGW;
                end
                ST_WGW: st <= ST_WGW2;
                ST_WGW2: st <= ST_WG1;
                ST_WG1: begin
                    phase <= ph_new;
                    r2_2[i] <= ph_new[25:0];
                    st <= ST_WG2;
                end
                ST_WG2: begin
                    sign_flip <= sf;
                    r_t27 <= wv_t27; r_t22 <= wv_t22;
                    r_wa1 <= wa1; r_wa2 <= wa2;
                    r_end <= wv_end; r_end2 <= wv_end2;
                    r_t5 <= nw_t5; r_t12 <= nw_t12; r_t32 <= nw_t32; r_l1 <= nw_l1; r_m1 <= nw_m1;
                    r_w467 <= nw_w467;
                    if (ctrl[7]) begin
                        if (wv_end && !int_state) begin
                            int_state <= 1'b1;
                            int_status <= 8'(i + 5'd1);
                        end else if (wv_end2 && !int_state) begin
                            int_state <= 1'b1;
                            int_status <= 8'(i + 5'd1) | 8'h20;
                        end
                        pcm_need <= {!wa2[19], !wa1[19]};
                        pcm_got <= 2'd0;
                        s1l <= 8'd0; s1h <= 8'd0; s2l <= 8'd0; s2h <= 8'd0;
                        st <= ST_PREQ1;
                    end else
                        st <= ST_WG3;
                    r_e1 <= nw_e1[15:0]; r_e2 <= nw_e2[15:0]; r_ix1q <= nw_ix1q; r_ix2 <= nw_ix2;
                end
                ST_WG3: st <= ST_N2W;
                ST_PREQ1: if (!pcm_need[0] || pcm_req_ready) st <= ST_PREQ2;
                ST_PREQ2: if (!pcm_need[1] || pcm_req_ready) st <= ST_PWAIT;
                ST_PWAIT: if (pcm_got == pcm_need) st <= ST_W3;
                ST_W3: begin
                    r_zero1 <= w_zero1; r_zero2 <= w_zero2;
                    r_sign1 <= w_sign1; r_sign2 <= w_sign2;
                    st <= ST_W3W;
                end
                ST_W3W: st <= ST_W3W2;
                ST_W3W2: st <= ST_G0;
                ST_N2W: st <= ST_N2W2;
                ST_N2W2: st <= ST_N3A;
                ST_N3A: begin
                    r_m2 <= n3a_m2; r_m3 <= n3a_m3;
                    r_l2 <= n3a_l2; r_t34 <= n3a_t34; r_t52 <= n3a_t52;
                    r_p1lo <= n3a_p1lo; r_p1hi <= n3a_p1hi;
                    st <= ST_N3A2;
                end
                ST_N3A2: begin
                    r_m4 <= n3a_m4; r_m5 <= n3a_m5;
                    st <= ST_N3B;
                end
                ST_N3B: begin
                    r_t44 <= n3b_t44; r_t45 <= n3b_t45;
                    r_w326 <= w326; r_w402 <= w402; r_w407 <= w407; r_w413 <= w413; r_w417 <= w417;
                    r_w418 <= w418; r_w419 <= w419; r_w429 <= w429; r_w430 <= w430; r_w464 <= w464;
                    st <= ST_N3C;
                end
                ST_N3C: begin
                    r_t43 <= n3c_t43;
                    st <= ST_N3D;
                end
                ST_N3D: begin
                    st <= ST_N4;
                end
                ST_N4: begin
                    r_t53 <= n4_t53; r_t56 <= n4_t56; r_t48 <= n4_t48;
                    st <= ST_N5;
                end
                ST_N5: begin
                    // w1: sign1 = !w469 = w467 ^ w326, no zero test; w2: sign2 = !w464, zero
                    r_zero1 <= 1'b0;
                    r_sign1 <= r_w467 ^ r_w326;
                    r_sign2 <= !r_w464;
                    r_zero2 <= n5_zero;
                    st <= ST_N5W;
                end
                ST_N5W: st <= ST_N5W2;
                ST_N5W2: st <= ST_G0;
                ST_G0: begin
                    r_w1 <= (!r_zero1 && (sign_flip ^ r_sign1)) ? ~pa_r : pa_r;
                    r_w2 <= (!r_zero2 && (sign_flip ^ r_sign2)) ? ~pb_r : pb_r;
                    st <= ST_G1;
                end
                ST_G1: begin
                    r_g1 <= g1_v;
                    st <= ST_G2;
                end
                ST_G2: begin
                    r_oo <= g2_oo;
                    prev <= r_g1;
                    st <= ST_ACC1;
                end
                ST_ACC1: begin
                    r_ot1 <= mul(r_oo, 32'(pan1));
                    r_c1 <= acc0_c1;
                    r_t2 <= acc0_t2;
                    st <= ST_ACC1B;
                end
                ST_ACC1B: begin
                    r_add_lo <= acc1_lo;
                    r_add_hi <= acc1_hi;
                    r_rf5_we <= rf5_we; r_rf5_wa <= rf5_wa; r_rf5_wd <= rf5_wd;
                    r_latch_w <= latch_w; r_latch_a <= latch_a;
                    st <= ST_ACC2;
                end
                ST_ACC2: begin
                    if (acc_en_lo) acc[oddeven][{1'b0, outch[0]}] <= acc[oddeven][{1'b0, outch[0]}] + r_add_lo;
                    if (acc_en_hi) acc[oddeven][{1'b1, outch[0]}] <= acc[oddeven][{1'b1, outch[0]}] + r_add_hi;
                    if (r_rf5_we) rf5[r_rf5_wa] <= r_rf5_wd;
                    if (r_latch_w) begin
                        w186 <= rf5[r_latch_a + 5'd1][14:0];
                        w187 <= rf5[r_latch_a + 5'd2][14:0];
                        w188 <= rf5[r_latch_a + 5'd3][14:0];
                    end
                    st <= ST_IDLE;
                    slot <= i + 5'd1;
                    slot_done <= 1'b1;
                end
                default: st <= ST_IDLE;
            endcase

            // end of sample: clamp the accumulators of this sample, one clock after slot 31
            if (slot_done && slot_idx == 5'd31)
                for (int k = 0; k < 4; k++) acc[oddeven][k] <= 32'(clamp16(acc[oddeven][k]));
        end
    end
    assign slot_idx = i;

    // debug
    always_comb begin
        dbg_oo = r_oo;
        dbg_prev = prev;
        dbg_w46 = w46;
        dbg_ram2_0 = r2_0[i];
        dbg_ram2_1 = r2_1[i];
        dbg_ram2_2 = r2_2[i];
        dbg_acc2 = acc[oddeven][0];
        dbg_acc3 = acc[oddeven][1];
        dbg_acc6 = acc[oddeven][2];
        dbg_acc7 = acc[oddeven][3];
        dbg_w186 = w186;
        dbg_w187 = w187;
        dbg_w188 = w188;
    end
endmodule
