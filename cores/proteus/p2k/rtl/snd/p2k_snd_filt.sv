// SPDX-License-Identifier: GPL-3.0-or-later
// Proteus 2000 H filter (sound::filter / sound::stage in p2k/model/p2k_sound.h), shared by up to
// 8 voices in flight.
//
// A voice is loaded into a free context with its interpolated sample. Each context issues one
// operation every 8 clocks (its slot): sections 0, 1, 2 and the gain of stage 0, then the same for
// stage 1 when the voice is chained (12th order). An operation reads its block's coefficient
// words, the cutoff interpolator of the previous slot, the block flags and the section state, and
// goes down a 14-clock pipeline. Only the product g * x depends on the voice's running value x; it
// is read 6 clocks after the issue and the new value is written back 13 clocks after it, before
// the context's next slot. Everything else (a1 s0 - a2 s1, n2 s1 - n1 s0) depends on the state and
// the coefficients only.
//
// Per section, with s0 = w1 and s1 = w2 (Q8, saturated to 48 bits) and Q28 coefficients:
//   w  = sat47(rsh(g x + a1 s0 - a2 s1, 28))
//   x' = sat47(rsh((w << 28) - n1 s0 + n2 s1, 28)) = sat47(w + rsh(n2 s1 - n1 s0, 28))
// Gain: x = clamp(x, 32767 << 8), then x = sat47(rsh(x gg, 28)) in gain mode.
// After the last gain the voice leaves with x (the mixer clamps it to 2^31 - 1).
module p2k_snd_filt (
    input  logic        clk,
    input  logic        rst,

    input  logic        in_valid,          // load a voice (a context must be free)
    input  logic [6:0]  in_v,
    input  logic        in_ch,             // chained: run stage 1 through the next slot's block
    input  logic signed [47:0] in_y,
    input  logic [5:0]  in_route,
    input  logic [15:0] in_xl,
    input  logic [15:0] in_xr,
    output logic [3:0]  free_n,            // free contexts
    output logic        busy,

    output logic [9:0]  fc_ra,             // {block, section}: coefficient words 8s .. 8s+3
    input  logic [15:0] fc_q [0:3],
    output logic        rd_en,             // an operation is issued (its reads are used)
    output logic [9:0]  fst_ra,            // {voice, stage, section}: {w2, w1}
    input  logic [95:0] fst_q,
    output logic        fst_we,
    output logic [9:0]  fst_wa,
    output logic [95:0] fst_wd,
    output logic [7:0]  ipx_ra,            // cutoff interpolator of the previous slot's block
    input  logic [15:0] ipx_q,
    output logic [7:0]  hfa_ra,            // flags (byte 0x7A) of the block: gain mode
    input  logic [7:0]  hfa_q,
    output logic [7:0]  hfb_ra,            // flags of the previous slot's block: unnormalised
    input  logic [7:0]  hfb_q,

    output logic        out_valid,
    output logic signed [47:0] out_y,
    output logic [5:0]  out_route,
    output logic [15:0] out_xl,
    output logic [15:0] out_xr,
    output logic [31:0] stat_chained
);
    `include "p2k_snd_fn.svh"

    localparam int K = 8;
    localparam logic signed [31:0] ONE = 32'sd1 <<< 28;

    // ------------------------------------------------------------------ contexts
    logic        c_val [0:K-1];
    logic        c_fin [0:K-1];       // last operation issued
    logic [6:0]  c_v   [0:K-1];
    logic        c_ch  [0:K-1];
    logic        c_st  [0:K-1];
    logic [1:0]  c_op  [0:K-1];       // 0-2 section, 3 gain
    logic signed [47:0] c_x [0:K-1];
    logic [5:0]  c_route [0:K-1];
    logic [15:0] c_xl [0:K-1];
    logic [15:0] c_xr [0:K-1];

    logic [2:0]  sl;                  // slot

    always_comb begin
        free_n = '0;
        for (int k = 0; k < K; k++)
            free_n = free_n + {3'd0, !c_val[k]};
    end

    // ------------------------------------------------------------------ issue (P0)
    wire        i_ok   = c_val[sl] && !c_fin[sl];
    wire        i_gain = c_op[sl] == 2'd3;
    wire [7:0]  i_blk  = c_st[sl] ? nblk(hblk(c_v[sl])) : hblk(c_v[sl]);
    wire [7:0]  i_pb   = pblk(i_blk);
    wire        i_last = i_gain && !(!c_st[sl] && c_ch[sl]);

    assign rd_en  = i_ok;
    assign fc_ra  = {i_blk, i_gain ? 2'd2 : c_op[sl]};
    assign fst_ra = {c_v[sl], c_st[sl], c_op[sl]};
    assign ipx_ra = i_pb;
    assign hfa_ra = i_blk;
    assign hfb_ra = i_pb;

    // pipeline registers: stage n holds the operation issued n clocks earlier
    // P1: RAM data
    logic        p1_v, p1_gain, p1_last; logic [2:0] p1_k; logic [9:0] p1_fa; logic [1:0] p1_sec;
    // P2
    logic        p2_v, p2_gain, p2_last; logic [2:0] p2_k; logic [9:0] p2_fa;
    logic [7:0]  p2_a [0:3];
    logic signed [8:0] p2_d [0:3];
    logic [15:0] p2_t;
    logic        p2_p, p2_z, p2_gp, p2_gn;
    logic signed [47:0] p2_s0, p2_s1;
    // P3
    logic        p3_v, p3_gain, p3_last; logic [2:0] p3_k; logic [9:0] p3_fa;
    logic [7:0]  p3_a [0:3];
    logic signed [24:0] p3_m [0:3];
    logic        p3_p, p3_z, p3_gp, p3_gn;
    logic signed [47:0] p3_s0, p3_s1;
    // P4
    logic        p4_v, p4_gain, p4_last; logic [2:0] p4_k; logic [9:0] p4_fa;
    logic [16:0] p4_c [0:3];
    logic        p4_p, p4_z, p4_gp, p4_gn;
    logic signed [47:0] p4_s0, p4_s1;
    // P5
    logic        p5_v, p5_gain, p5_last; logic [2:0] p5_k; logic [9:0] p5_fa;
    logic signed [31:0] p5_pf2, p5_pb2, p5_zf2, p5_zb2;
    logic [26:0] p5_graw;
    logic        p5_pz, p5_gp, p5_gn;
    logic signed [47:0] p5_s0, p5_s1;
    // P6
    logic        p6_v, p6_gain, p6_last; logic [2:0] p6_k; logic [9:0] p6_fa;
    logic signed [31:0] p6_g, p6_a1, p6_a2, p6_n1, p6_n2;
    logic [41:0] p6_gg;
    logic        p6_pz, p6_gp;
    logic signed [47:0] p6_s0, p6_s1;
    // P7: multiplier inputs
    logic        p7_v, p7_gain, p7_last; logic [2:0] p7_k; logic [9:0] p7_fa;
    logic signed [31:0] m_g, m_a1, m_a2, m_n1, m_n2;
    logic signed [47:0] m_x, m_s0, m_s1;
    logic        p7_pz, p7_gp;
    logic signed [47:0] p7_s0, p7_xb;
    // P8-P10: inside p2k_mul (carried values follow)
    logic        p8_v, p9_v, p10_v, p8_gain, p9_gain, p10_gain, p8_last, p9_last, p10_last;
    logic [2:0]  p8_k, p9_k, p10_k;
    logic [9:0]  p8_fa, p9_fa, p10_fa;
    logic        p8_pz, p9_pz, p10_pz, p8_gp, p9_gp, p10_gp;
    logic signed [47:0] p8_s0, p9_s0, p10_s0, p8_xb, p9_xb, p10_xb;
    logic signed [79:0] r_gx, r_a1, r_a2, r_n1, r_n2;
    // P11
    logic        p11_v, p11_gain, p11_last; logic [2:0] p11_k; logic [9:0] p11_fa;
    logic        p11_pz, p11_gp;
    logic signed [47:0] p11_s0, p11_xb;
    logic signed [83:0] p11_gx, p11_u, p11_q;
    // P12
    logic        p12_v, p12_gain, p12_last; logic [2:0] p12_k; logic [9:0] p12_fa;
    logic        p12_pz, p12_gp;
    logic signed [47:0] p12_s0, p12_xb;
    logic signed [83:0] p12_sum;
    logic signed [55:0] p12_qr;
    // P13
    logic        p13_v, p13_gain, p13_last; logic [2:0] p13_k; logic [9:0] p13_fa;
    logic        p13_pz;
    logic signed [47:0] p13_s0, p13_xb, p13_w;
    logic signed [55:0] p13_qr;

    p2k_mul #(.WA(32), .WB(48), .LA(16), .LB(24)) u_gx (.clk, .a(m_g),  .b(m_x),  .p(r_gx));
    p2k_mul #(.WA(32), .WB(48), .LA(16), .LB(24)) u_a1 (.clk, .a(m_a1), .b(m_s0), .p(r_a1));
    p2k_mul #(.WA(32), .WB(48), .LA(16), .LB(24)) u_a2 (.clk, .a(m_a2), .b(m_s1), .p(r_a2));
    p2k_mul #(.WA(32), .WB(48), .LA(16), .LB(24)) u_n1 (.clk, .a(m_n1), .b(m_s0), .p(r_n1));
    p2k_mul #(.WA(32), .WB(48), .LA(16), .LB(24)) u_n2 (.clk, .a(m_n2), .b(m_s1), .p(r_n2));

    // P13 result (combinational): the new x
    logic signed [47:0] xo;
    always_comb begin
        if (p13_gain)
            xo = p13_w;
        else if (p13_pz)
            xo = sat47(64'(p13_w) + 64'(p13_qr));
        else
            xo = p13_xb;
    end

    always_ff @(posedge clk) begin
        out_valid <= 1'b0;
        fst_we <= 1'b0;
        if (rst) begin
            sl <= '0;
            stat_chained <= '0;
            for (int k = 0; k < K; k++) begin
                c_val[k] <= 1'b0;
                c_fin[k] <= 1'b0;
            end
            p1_v <= 1'b0; p2_v <= 1'b0; p3_v <= 1'b0; p4_v <= 1'b0; p5_v <= 1'b0; p6_v <= 1'b0;
            p7_v <= 1'b0; p8_v <= 1'b0; p9_v <= 1'b0; p10_v <= 1'b0; p11_v <= 1'b0; p12_v <= 1'b0;
            p13_v <= 1'b0;
        end else begin
            sl <= sl + 3'd1;

            // ---- load a voice into the lowest free context
            if (in_valid) begin
                automatic int f = -1;
                for (int k = K - 1; k >= 0; k--)
                    if (!c_val[k]) f = k;
`ifdef VERILATOR
                if (f < 0) $fatal(1, "p2k_snd_filt: load with no free context");
`endif
                if (f >= 0) begin
                    c_val[f] <= 1'b1;
                    c_fin[f] <= 1'b0;
                    c_v[f] <= in_v;
                    c_ch[f] <= in_ch;
                    c_st[f] <= 1'b0;
                    c_op[f] <= 2'd0;
                    c_x[f] <= in_y;
                    c_route[f] <= in_route;
                    c_xl[f] <= in_xl;
                    c_xr[f] <= in_xr;
                end
            end

            // ---- P0: issue from the slot's context and step it to its next operation
            p1_v <= i_ok;
            p1_gain <= i_gain;
            p1_last <= i_last;
            p1_k <= sl;
            p1_fa <= fst_ra;
            p1_sec <= c_op[sl];
            if (i_ok) begin
                if (i_gain) begin
                    if (i_last)
                        c_fin[sl] <= 1'b1;
                    else begin
                        c_st[sl] <= 1'b1;
                        c_op[sl] <= 2'd0;
                        stat_chained <= stat_chained + 32'd1;
                    end
                end else
                    c_op[sl] <= c_op[sl] + 2'd1;
            end

            // ---- P1: coefficient words, flags, state, cutoff
            begin
                automatic logic [15:0] phi = fc_q[0], plo = fc_q[1], zhi = fc_q[2], zlo = fc_q[3];
                automatic logic gmode = hfa_q[6];
                automatic logic pp = !(phi == 16'hffcf && plo == 16'hffcf);
                automatic logic zz = !(p1_sec == 2'd2 && gmode) && !(zhi == 16'hffcf && zlo == 16'hffcf);
                p2_v <= p1_v; p2_gain <= p1_gain; p2_last <= p1_last; p2_k <= p1_k; p2_fa <= p1_fa;
                // hcode operands: 0 = f^2 of the pole (low bytes), 1 = B2' of the pole (high bytes), 2/3 the zero
                p2_a[0] <= phi[7:0];  p2_d[0] <= $signed({1'b0, plo[7:0]})  - $signed({1'b0, phi[7:0]});
                p2_a[1] <= phi[15:8]; p2_d[1] <= $signed({1'b0, plo[15:8]}) - $signed({1'b0, phi[15:8]});
                p2_a[2] <= zhi[7:0];  p2_d[2] <= $signed({1'b0, zlo[7:0]})  - $signed({1'b0, zhi[7:0]});
                p2_a[3] <= zhi[15:8]; p2_d[3] <= $signed({1'b0, zlo[15:8]}) - $signed({1'b0, zhi[15:8]});
                p2_t <= ipx_q;
                p2_p <= pp;
                p2_z <= zz;
                p2_gp <= gmode && !(zhi == 16'hffcf && zlo == 16'hffcf);
                p2_gn <= p1_sec != 2'd0 && hfb_q[7];
                p2_s0 <= $signed(fst_q[47:0]);
                p2_s1 <= $signed(fst_q[95:48]);
            end

            // ---- P2: (b - a) t
            p3_v <= p2_v; p3_gain <= p2_gain; p3_last <= p2_last; p3_k <= p2_k; p3_fa <= p2_fa;
            for (int j = 0; j < 4; j++) begin
                p3_a[j] <= p2_a[j];
                p3_m[j] <= 25'(p2_d[j] * $signed({1'b0, p2_t}));
            end
            p3_p <= p2_p; p3_z <= p2_z; p3_gp <= p2_gp; p3_gn <= p2_gn;
            p3_s0 <= p2_s0; p3_s1 <= p2_s1;

            // ---- P3: hcode = ((a << 16) + (b - a) t + 128) >> 8
            p4_v <= p3_v; p4_gain <= p3_gain; p4_last <= p3_last; p4_k <= p3_k; p4_fa <= p3_fa;
            for (int j = 0; j < 4; j++) begin
                automatic logic signed [25:0] s = $signed({2'b0, p3_a[j], 16'h0}) + 26'(p3_m[j]) + 26'sd128;
                p4_c[j] <= 17'(s >>> 8);
            end
            p4_p <= p3_p; p4_z <= p3_z; p4_gp <= p3_gp; p4_gn <= p3_gn;
            p4_s0 <= p3_s0; p4_s1 <= p3_s1;

            // ---- P4: hdec
            p5_v <= p4_v; p5_gain <= p4_gain; p5_last <= p4_last; p5_k <= p4_k; p5_fa <= p4_fa;
            p5_pf2 <= p4_p ? $signed({1'b0, hdec(p4_c[0]), 4'b0}) : ONE;
            p5_pb2 <= p4_p ? $signed({3'b0, hdec(p4_c[1]), 2'b0}) : ONE;
            p5_zf2 <= p4_z ? $signed({1'b0, hdec(p4_c[2]), 4'b0}) : ONE;
            p5_zb2 <= p4_z ? $signed({3'b0, hdec(p4_c[3]), 2'b0}) : ONE;
            p5_graw <= hdec(p4_c[2]);
            p5_pz <= p4_p || p4_z; p5_gp <= p4_gp; p5_gn <= p4_gn;
            p5_s0 <= p4_s0; p5_s1 <= p4_s1;

            // ---- P5: coefficients
            p6_v <= p5_v; p6_gain <= p5_gain; p6_last <= p5_last; p6_k <= p5_k; p6_fa <= p5_fa;
            p6_g  <= p5_gn ? ONE : p5_pf2;
            p6_a1 <= 2 * ONE - p5_pf2 - p5_pb2;
            p6_a2 <= ONE - p5_pb2;
            p6_n1 <= 2 * ONE - p5_zf2 - p5_zb2;
            p6_n2 <= ONE - p5_zb2;
            p6_gg <= 42'(p5_graw) * 42'd16644;
            p6_pz <= p5_pz; p6_gp <= p5_gp;
            p6_s0 <= p5_s0; p6_s1 <= p5_s1;

            // ---- P6: multiplier inputs; x is read here
            begin
                automatic logic signed [47:0] x = c_x[p6_k];
                automatic logic signed [47:0] xg = clamp48(x, VOICE_LIM);
                p7_v <= p6_v; p7_gain <= p6_gain; p7_last <= p6_last; p7_k <= p6_k; p7_fa <= p6_fa;
                m_g  <= p6_gain ? $signed(32'(p6_gg >> 12)) : p6_g;
                m_x  <= p6_gain ? xg : x;
                m_a1 <= p6_gain ? 32'sd0 : p6_a1;
                m_a2 <= p6_gain ? 32'sd0 : p6_a2;
                m_n1 <= p6_gain ? 32'sd0 : p6_n1;
                m_n2 <= p6_gain ? 32'sd0 : p6_n2;
                m_s0 <= p6_s0;
                m_s1 <= p6_s1;
                p7_pz <= p6_pz; p7_gp <= p6_gp;
                p7_s0 <= p6_s0;
                p7_xb <= p6_gain ? xg : x;
            end

            // ---- P7-P9: products (p2k_mul); carry the rest
            p8_v <= p7_v; p8_gain <= p7_gain; p8_last <= p7_last; p8_k <= p7_k; p8_fa <= p7_fa;
            p8_pz <= p7_pz; p8_gp <= p7_gp; p8_s0 <= p7_s0; p8_xb <= p7_xb;
            p9_v <= p8_v; p9_gain <= p8_gain; p9_last <= p8_last; p9_k <= p8_k; p9_fa <= p8_fa;
            p9_pz <= p8_pz; p9_gp <= p8_gp; p9_s0 <= p8_s0; p9_xb <= p8_xb;
            p10_v <= p9_v; p10_gain <= p9_gain; p10_last <= p9_last; p10_k <= p9_k; p10_fa <= p9_fa;
            p10_pz <= p9_pz; p10_gp <= p9_gp; p10_s0 <= p9_s0; p10_xb <= p9_xb;

            // ---- P10: products valid; partial sums
            p11_v <= p10_v; p11_gain <= p10_gain; p11_last <= p10_last; p11_k <= p10_k; p11_fa <= p10_fa;
            p11_pz <= p10_pz; p11_gp <= p10_gp; p11_s0 <= p10_s0; p11_xb <= p10_xb;
            p11_gx <= 84'(r_gx);
            p11_u  <= 84'(r_a1) - 84'(r_a2);
            p11_q  <= 84'(r_n2) - 84'(r_n1);

            // ---- P11
            p12_v <= p11_v; p12_gain <= p11_gain; p12_last <= p11_last; p12_k <= p11_k; p12_fa <= p11_fa;
            p12_pz <= p11_pz; p12_gp <= p11_gp; p12_s0 <= p11_s0; p12_xb <= p11_xb;
            p12_sum <= p11_gx + p11_u;
            p12_qr <= 56'((p11_q + (84'sd1 <<< 27)) >>> 28);

            // ---- P12: w
            p13_v <= p12_v; p13_gain <= p12_gain; p13_last <= p12_last; p13_k <= p12_k; p13_fa <= p12_fa;
            p13_pz <= p12_pz; p13_s0 <= p12_s0; p13_xb <= p12_xb; p13_qr <= p12_qr;
            begin
                automatic logic signed [83:0] r = (p12_sum + (84'sd1 <<< 27)) >>> 28;
                automatic logic signed [47:0] w = sat47(64'(r));
                p13_w <= (p12_gain && !p12_gp) ? p12_xb : w;
            end

            // ---- P13: new x, state write, output
            if (p13_v) begin
                c_x[p13_k] <= xo;
                if (!p13_gain && p13_pz) begin
                    fst_we <= 1'b1;
                    fst_wa <= p13_fa;
                    fst_wd <= {p13_s0, p13_w};
                end
                if (p13_last) begin
                    out_valid <= 1'b1;
                    out_y <= xo;
                    out_route <= c_route[p13_k];
                    out_xl <= c_xl[p13_k];
                    out_xr <= c_xr[p13_k];
                    c_val[p13_k] <= 1'b0;
                    c_fin[p13_k] <= 1'b0;
                end
            end
        end
    end

    always_comb begin
        busy = p1_v || p2_v || p3_v || p4_v || p5_v || p6_v || p7_v || p8_v || p9_v || p10_v ||
               p11_v || p12_v || p13_v || out_valid;
        for (int k = 0; k < K; k++)
            busy = busy || c_val[k];
    end
endmodule
