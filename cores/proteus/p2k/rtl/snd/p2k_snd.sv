// SPDX-License-Identifier: GPL-3.0-or-later
// Proteus 2000 sound engine: G20 voices (128), H level and cutoff interpolators, H filter, the
// output mixer and the G05 effects. The definition it matches bit for bit is the integer model
// p2k/model/p2k_sound.h (docs/p2k-mame-plan.md); comments name the model function each part follows.
// The structure is for the Cyclone V (docs/p2k-sound-hdl.md): register files in block RAM with
// registered reads, sample and delay-line memory through the synthframe memory port, the filter
// as a pipelined unit shared by the voices in flight (p2k_snd_filt.sv).
//
// Interface: one register write at a time while idle (wr_valid/wr_ready, COMBINE_DATA mask),
// then a start pulse computes one 44.1 kHz output sample (done pulse, out_l/out_r). Writes made
// between two start pulses take effect before the second one's sample, as in the model. After
// rst the engine clears its register files (2048 clocks) before it takes a write or a start.
//
// Memory (synthframe_mem_ddr3 contract; tag bit 1 = client): sample word a (a < 2^26) is bits
// [16 (a mod 4) +: 16] of 64-bit word SMP_BASE + a / 4; words from 2^26 up read as 0. The
// effects delay lines are at FX_BASE (p2k_fx.sv, 14 x 2^14 words).
//
// Per sample: the front end walks the 128 voices (3 clocks for a voice with a zero increment
// less than that: 1), reads each voice's registers, advances its address (sound_base::advance)
// and asks for its sample pair; the pairs come back in order, are interpolated and go to the
// filter (filter_on) or straight to the mixer. After the last voice: the effects sends and
// fx_int::process (fx_on) while the interpolators tick (sound_base::tick_interps), then the output.
module p2k_snd #(
    parameter logic [28:0] SMP_BASE = 29'h0000000,
    parameter logic [28:0] FX_BASE  = 29'h1000000
) (
    input  logic        clk,
    input  logic        rst,
    input  logic        filter_on,
    input  logic        fx_on,

    input  logic        wr_valid,
    output logic        wr_ready,
    input  logic [1:0]  wr_chip,      // 0 = G20, 1 = H, 2 = G05
    input  logic [18:0] wr_addr,      // word offset; G05: {index[4:0], offset[5:0]}
    input  logic [15:0] wr_data,
    input  logic [15:0] wr_mask,

    input  logic        start,
    output logic        done,
    output logic signed [15:0] out_l,
    output logic signed [15:0] out_r,

    output logic        mem_req_valid,
    input  logic        mem_req_ready,
    output logic        mem_req_we,
    output logic [28:0] mem_req_addr,
    output logic [63:0] mem_req_wdata,
    output logic [7:0]  mem_req_be,
    output logic [7:0]  mem_req_len,
    output logic [1:0]  mem_req_tag,
    input  logic        mem_rsp_valid,
    input  logic [63:0] mem_rsp_data,
    input  logic [1:0]  mem_rsp_tag,

    output logic [31:0] stat_div_cycles,  // clocks spent in the loop-modulo divider (measurement)
    output logic [31:0] stat_chained      // voice-samples filtered as 12th-order chains (measurement)
);
    `include "p2k_snd_fn.svh"

    function automatic logic signed [12:0] inc_decode(input logic [7:0] c);
        logic [2:0] f;
        logic signed [12:0] d;
        f = c[6:4];
        d = 13'(c[3:0]);
        if (!c[7])
            inc_decode = (f != 0) ? ((d + 13'sd16) <<< (f - 3'd1)) : d;
        else
            inc_decode = (f != 0) ? ((d - 13'sd32) <<< (f - 3'd1)) : (d - 13'sd16);
    endfunction

    function automatic logic [7:0] inc_encode(input logic signed [31:0] vin);
        logic signed [31:0] v, sh;
        logic [7:0] r;
        v = (vin < -32'sd2048) ? -32'sd2048 : (vin > 32'sd1984) ? 32'sd1984 : vin;
        r = 8'h00;
        if (v >= 0) begin
            if (v < 16)
                r = 8'(v);
            else begin
                for (int f = 7; f >= 1; f--) begin
                    sh = v >>> (f - 1);
                    if (sh < 32)
                        r = 8'((f << 4) | (sh - 16));
                end
            end
        end else begin
            if (v >= -16)
                r = 8'(32'h80 | (v + 16));
            else begin
                for (int f = 7; f >= 1; f--) begin
                    sh = v >>> (f - 1);
                    if (sh >= -32)
                        r = 8'(32'h80 | (f << 4) | (sh + 32));
                end
            end
        end
        inc_encode = r;
    endfunction

    // target_inc = inc_encode(target_diff >>> (5 + k)): the approach increment toward dest, 32 << k samples
    function automatic logic signed [31:0] target_diff(input logic [7:0] dest, input logic [15:0] x);
        target_diff = $signed({16'd0, dest, dest}) - $signed({16'd0, x});
    endfunction

    // sound_base::tick (FIG. 10)
    function automatic logic [15:0] ip_tick(input logic [15:0] x, input logic [7:0] dest, input logic [7:0] inc);
        logic pos;
        logic signed [18:0] sum, clamp;
        pos = !inc[7];
        sum = 19'(x) + 19'(inc_decode(inc)) + (pos ? 19'sd1 : 19'sd0);
        clamp = 19'({dest, dest});
        ip_tick = (sum < 0 || sum > 19'sd65535 || (pos ? sum > clamp : sum < clamp)) ? 16'(clamp) : 16'(sum);
    endfunction

    // G20 voice words kept (word -> field): the voice loop reads 2, 4-7, 10-12; writes need 1 and 13
    function automatic int g20_field(input logic [4:0] word);
        case (word)
            5'd2:  g20_field = 0;
            5'd4:  g20_field = 1;
            5'd5:  g20_field = 2;
            5'd6:  g20_field = 3;
            5'd7:  g20_field = 4;
            5'd10: g20_field = 5;
            5'd11: g20_field = 6;
            5'd12: g20_field = 7;
            5'd1:  g20_field = 8;
            5'd13: g20_field = 9;
            default: g20_field = -1;
        endcase
    endfunction

    // ------------------------------------------------------------------ top state
    typedef enum logic [3:0] {
        S_CLR, S_IDLE, S_W1, S_W2, S_WT, S_WE, S_W3, S_WC, S_RUN, S_MIX, S_FXW, S_FIN, S_OUT
    } state_t;
    state_t st;

    logic [10:0] clr_a;
    wire         clr = st == S_CLR;
    logic [47:0] smp;
    logic [2:0]  k_level [0:7];
    logic [2:0]  k_cut   [0:7];

    // write path registers
    logic [1:0]  w_chip;
    logic [18:0] w_addr;
    logic [15:0] w_data, w_mask;
    logic [2:0]  w_n;                // fst entries cleared so far (restart)
    logic [1:0]  w_tn;               // an interpolator target: n, dest, (dest dest - x), k
    logic [7:0]  w_tdest;
    logic signed [31:0] w_tdiff;
    logic [2:0]  w_tk;
    wire [6:0]   wg_voice = w_addr[11:5];
    wire [4:0]   wg_word  = w_addr[4:0];
    wire [7:0]   wh_b     = w_addr[13:6];
    wire [5:0]   wh_o     = w_addr[5:0];          // word in the block (byte offset / 2)
    wire [2:0]   wh_page  = w_addr[13:11];
    wire         wh_in    = w_addr < 19'h4000;
    wire         wh_coef  = !wh_o[5] && wh_o[4:3] != 2'd3 && !wh_o[2];   // words 0-3, 8-11, 16-19
    wire         wp_act   = st == S_W1 || st == S_W2 || st == S_WT || st == S_WE || st == S_W3 || st == S_WC;

    assign wr_ready = st == S_IDLE && !start;

    // ------------------------------------------------------------------ register files
    // G20 fields [voice]
    logic [6:0]  gf_ra;
    logic [15:0] gf_q [0:9];
    logic        gf_we [0:9];
    logic [6:0]  gf_wa;
    logic [15:0] gf_wd;
    // voice state [voice] {addr[25:0], frac[11:0], sel}
    logic [38:0] vs_q, vs_wd;
    logic        vs_we;
    logic [6:0]  vs_wa;
    // start / coefficient-write time stamps [voice]
    logic [6:0]  ts_ra_v, ts_ra_nv;
    logic [47:0] tsa_q, tsb_q, tco_q;
    logic        tsv_we, tco_we;
    logic [6:0]  tsv_wa, tco_wa;
    // filter coefficient words [block, section] (k = word mod 4)
    logic [9:0]  fc_ra;
    logic [15:0] fc_q [0:3];
    logic        fc_we [0:3];
    logic [9:0]  fc_wa;
    logic [15:0] fc_wd;
    // block flags (word 0x3D, low byte) [block]: 1 = write path / front end, 2/3 = filter
    logic [7:0]  hf1_ra, hf2_ra, hf3_ra;
    logic [7:0]  hf1_q, hf2_q, hf3_q;
    logic        hf_we;
    logic [7:0]  hf_wa, hf_wd;
    // interpolators [n][block]: x, {dest, inc}
    logic [7:0]  ipx_ra [0:2];
    logic [15:0] ipx_q [0:2];
    logic        ipx_we [0:2];
    logic [7:0]  ipx_wa [0:2];
    logic [15:0] ipx_wd [0:2];
    logic [7:0]  ipdi_ra;
    logic [15:0] ipdi_q [0:2];
    logic        ipdi_we [0:2];
    logic [7:0]  ipdi_wa;
    logic [15:0] ipdi_wd [0:2];
    // filter state [voice, stage, section] {w2, w1}
    logic [9:0]  fst_ra, fst_wa;
    logic [95:0] fst_q, fst_wd;
    logic        fst_we;

    // read enables: only while a read is used, so no read meets a write of the same word
    logic        tk_run;
    wire re_fe   = st == S_RUN || (st == S_IDLE && start);
    wire re_wp   = st == S_W1;
    wire re_run  = st == S_RUN;

    genvar gi;
    generate
        for (gi = 0; gi < 10; gi++) begin : g_gf
            p2k_ram #(.W(16), .AW(7)) u (.clk, .re(re_wp || re_fe), .raddr(gf_ra), .rdata(gf_q[gi]),
                .we(clr || gf_we[gi]), .waddr(clr ? clr_a[6:0] : gf_wa), .wdata(clr ? 16'h0 : gf_wd));
        end
        for (gi = 0; gi < 4; gi++) begin : g_fc
            p2k_ram #(.W(16), .AW(10)) u (.clk, .re(re_wp || re_run), .raddr(fc_ra), .rdata(fc_q[gi]),
                .we(clr || fc_we[gi]), .waddr(clr ? clr_a[9:0] : fc_wa), .wdata(clr ? 16'h0 : fc_wd));
        end
        for (gi = 0; gi < 3; gi++) begin : g_ip
            p2k_ram #(.W(16), .AW(8)) ux (.clk, .re(re_wp || re_run || tk_run), .raddr(ipx_ra[gi]), .rdata(ipx_q[gi]),
                .we(clr || ipx_we[gi]), .waddr(clr ? clr_a[7:0] : ipx_wa[gi]), .wdata(clr ? 16'h0 : ipx_wd[gi]));
            p2k_ram #(.W(16), .AW(8)) udi (.clk, .re(tk_run), .raddr(ipdi_ra), .rdata(ipdi_q[gi]),
                .we(clr || ipdi_we[gi]), .waddr(clr ? clr_a[7:0] : ipdi_wa), .wdata(clr ? 16'h0 : ipdi_wd[gi]));
        end
    endgenerate
    p2k_ram #(.W(39), .AW(7)) u_vs (.clk, .re(re_wp || re_fe), .raddr(gf_ra), .rdata(vs_q),
        .we(clr || vs_we), .waddr(clr ? clr_a[6:0] : vs_wa), .wdata(clr ? 39'h0 : vs_wd));
    p2k_ram #(.W(48), .AW(7)) u_tsa (.clk, .re(re_fe), .raddr(ts_ra_v), .rdata(tsa_q),
        .we(clr || tsv_we), .waddr(clr ? clr_a[6:0] : tsv_wa), .wdata(clr ? 48'h0 : smp));
    p2k_ram #(.W(48), .AW(7)) u_tsb (.clk, .re(re_fe), .raddr(ts_ra_nv), .rdata(tsb_q),
        .we(clr || tsv_we), .waddr(clr ? clr_a[6:0] : tsv_wa), .wdata(clr ? 48'h0 : smp));
    p2k_ram #(.W(48), .AW(7)) u_tco (.clk, .re(re_fe), .raddr(ts_ra_nv), .rdata(tco_q),
        .we(clr || tco_we), .waddr(clr ? clr_a[6:0] : tco_wa), .wdata(clr ? 48'h0 : smp));
    p2k_ram #(.W(8), .AW(8)) u_hf1 (.clk, .re(re_wp || re_run), .raddr(hf1_ra), .rdata(hf1_q),
        .we(clr || hf_we), .waddr(clr ? clr_a[7:0] : hf_wa), .wdata(clr ? 8'h0 : hf_wd));
    p2k_ram #(.W(8), .AW(8)) u_hf2 (.clk, .re(re_run), .raddr(hf2_ra), .rdata(hf2_q),
        .we(clr || hf_we), .waddr(clr ? clr_a[7:0] : hf_wa), .wdata(clr ? 8'h0 : hf_wd));
    p2k_ram #(.W(8), .AW(8)) u_hf3 (.clk, .re(re_run), .raddr(hf3_ra), .rdata(hf3_q),
        .we(clr || hf_we), .waddr(clr ? clr_a[7:0] : hf_wa), .wdata(clr ? 8'h0 : hf_wd));

    logic        fl_rd;
    logic [9:0]  fl_fst_wa;
    logic [95:0] fl_fst_wd;
    logic        fl_fst_we;
    logic [9:0]  wp_fst_wa;
    logic        wp_fst_we;
    p2k_ram #(.W(96), .AW(10)) u_fst (.clk, .re(fl_rd), .raddr(fst_ra), .rdata(fst_q),
        .we(clr || fl_fst_we || wp_fst_we), .waddr(clr ? clr_a[9:0] : wp_fst_we ? wp_fst_wa : fl_fst_wa),
        .wdata((clr || wp_fst_we) ? 96'h0 : fl_fst_wd));

    // ------------------------------------------------------------------ memory clients
    logic        vq_valid;            // voice sample-pair request
    logic [28:0] vq_addr;
    logic [7:0]  vq_len;
    logic        c_req_valid [2], c_req_ready [2], c_req_we [2], c_rsp_valid [2];
    logic [28:0] c_req_addr [2];
    logic [63:0] c_req_wdata [2], c_rsp_data;
    logic [7:0]  c_req_be [2], c_req_len [2];
    logic [0:0]  c_req_tag [2], c_rsp_tag;
    logic        fxq_valid, fxq_we;
    logic [28:0] fxq_addr;
    logic [63:0] fxq_wdata;
    logic [7:0]  fxq_be, fxq_len;

    assign c_req_valid[0] = vq_valid;
    assign c_req_we[0] = 1'b0;
    assign c_req_addr[0] = vq_addr;
    assign c_req_wdata[0] = '0;
    assign c_req_be[0] = 8'hff;
    assign c_req_len[0] = vq_len;
    assign c_req_tag[0] = 1'b0;
    assign c_req_valid[1] = fxq_valid;
    assign c_req_we[1] = fxq_we;
    assign c_req_addr[1] = fxq_addr;
    assign c_req_wdata[1] = fxq_wdata;
    assign c_req_be[1] = fxq_be;
    assign c_req_len[1] = fxq_len;
    assign c_req_tag[1] = 1'b0;

    synthframe_mem_arb #(.N(2), .CTW(1)) u_arb (
        .c_req_valid, .c_req_ready, .c_req_we, .c_req_addr, .c_req_wdata, .c_req_be, .c_req_len,
        .c_req_tag, .c_rsp_valid, .c_rsp_data, .c_rsp_tag,
        .req_valid(mem_req_valid), .req_ready(mem_req_ready), .req_we(mem_req_we), .req_addr(mem_req_addr),
        .req_wdata(mem_req_wdata), .req_be(mem_req_be), .req_len(mem_req_len), .req_tag(mem_req_tag),
        .rsp_valid(mem_rsp_valid), .rsp_data(mem_rsp_data), .rsp_tag(mem_rsp_tag)
    );

    // ------------------------------------------------------------------ G05 effects
    logic        fx_pre, fx_pre_busy, fx_go, fx_done;
    logic signed [31:0] fx_rev_in, fx_cho_in;
    logic signed [32:0] fx_add_l, fx_add_r;
    logic signed [16:0] fx_rev_send [0:3], fx_cho_send [0:3], fx_dry [0:3][0:1];

    p2k_fx #(.FX_BASE(FX_BASE)) u_fx (
        .clk, .rst,
        .clr, .clr_addr(clr_a),
        .wr_en(st == S_W1 && w_chip == 2'd2), .wr_addr(w_addr[10:0]), .wr_data(w_data), .wr_mask(w_mask),
        .pre(fx_pre), .cfg(smp[7:0] == 8'h00), .pre_busy(fx_pre_busy),
        .go(fx_go), .rev_in(fx_rev_in), .cho_in(fx_cho_in),
        .done(fx_done), .add_l(fx_add_l), .add_r(fx_add_r),
        .rev_send(fx_rev_send), .cho_send(fx_cho_send), .dry(fx_dry),
        .req_valid(fxq_valid), .req_ready(c_req_ready[1]), .req_we(fxq_we), .req_addr(fxq_addr),
        .req_wdata(fxq_wdata), .req_be(fxq_be), .req_len(fxq_len),
        .rsp_valid(c_rsp_valid[1]), .rsp_data(c_rsp_data)
    );

    // ------------------------------------------------------------------ front end (sound::step voice loop)
    typedef enum logic [2:0] { FE_IDLE, FE_B, FE_C, FE_D, FE_E, FE_DIV, FE_P, FE_END } fe_t;
    fe_t fe;
    logic [6:0]  fv;
    wire  [6:0]  fe_ra = (fe == FE_IDLE) ? 7'd0 : fv + 7'd1;
    // values of voice fv
    logic [15:0] e_inc;
    logic [31:0] e_end8, e_endc, e_next;
    logic [25:0] e_addr;
    logic [11:0] e_frac;
    logic        e_sel;
    logic [47:0] e_stv, e_stnv, e_conv;
    logic        e_ch;
    logic [7:0]  e_hb;
    logic [26:0] e_an;
    logic [25:0] e_le, e_ls;
    logic        e_nsel;
    logic signed [28:0] e_lsz;
    logic [11:0] e_nfrac;
    logic        e_over;
    logic [26:0] e_ov;
    logic [5:0]  e_route;
    logic [15:0] e_xl, e_xr;
    logic [25:0] n_addr;
    logic        n_sel;
    logic [27:0] div_r;
    logic [26:0] div_l;
    logic [4:0]  div_k;
    logic [7:0]  n_push;              // voices sent this sample

    // sample-pair ring: contexts in request order, filled by the responses
    localparam int RN = 16;
    logic [3:0]  rg_wp, rg_fp, rg_rp;
    logic [4:0]  rg_n;                // entries pushed and not popped
    logic [6:0]  rg_v [0:RN-1];
    logic [11:0] rg_frac [0:RN-1];
    logic [1:0]  rg_ws [0:RN-1];
    logic        rg_two [0:RN-1], rg_s1z [0:RN-1], rg_ch [0:RN-1], rg_dv [0:RN-1], rg_half [0:RN-1];
    logic [5:0]  rg_route [0:RN-1];
    logic [15:0] rg_xl [0:RN-1], rg_xr [0:RN-1];
    logic signed [15:0] rg_s0 [0:RN-1], rg_s1 [0:RN-1];

    // ------------------------------------------------------------------ join + interpolation
    logic        j1_v, j2_v, j3_v;
    logic [6:0]  j1_vo, j2_vo, j3_vo;
    logic        j1_ch, j2_ch, j3_ch;
    logic [5:0]  j1_route, j2_route, j3_route;
    logic [15:0] j1_xl, j2_xl, j3_xl, j1_xr, j2_xr, j3_xr;
    logic signed [15:0] j1_s0, j1_s1, j2_s0, j3_s0;
    logic [11:0] j1_frac, j2_frac;
    logic signed [16:0] j2_d;
    logic signed [29:0] j3_m;
    logic        j4_v;
    logic [6:0]  j4_vo;
    logic        j4_ch;
    logic [5:0]  j4_route;
    logic [15:0] j4_xl, j4_xr;
    logic signed [47:0] j4_y;

    // ------------------------------------------------------------------ filter
    logic [3:0]  fl_free;
    logic        fl_busy, fl_out_v;
    logic signed [47:0] fl_out_y;
    logic [5:0]  fl_out_route;
    logic [15:0] fl_out_xl, fl_out_xr;
    logic [9:0]  fl_fc_ra, fl_fst_ra;
    logic [7:0]  fl_ipx_ra, fl_hfa_ra, fl_hfb_ra;
    wire         fl_load = filter_on && j4_v;

    p2k_snd_filt u_filt (
        .clk, .rst,
        .in_valid(fl_load), .in_v(j4_vo), .in_ch(j4_ch), .in_y(j4_y), .in_route(j4_route),
        .in_xl(j4_xl), .in_xr(j4_xr), .free_n(fl_free), .busy(fl_busy),
        .rd_en(fl_rd), .fc_ra(fl_fc_ra), .fc_q, .fst_ra(fl_fst_ra), .fst_q,
        .fst_we(fl_fst_we), .fst_wa(fl_fst_wa), .fst_wd(fl_fst_wd),
        .ipx_ra(fl_ipx_ra), .ipx_q(ipx_q[2]), .hfa_ra(fl_hfa_ra), .hfa_q(hf2_q), .hfb_ra(fl_hfb_ra), .hfb_q(hf3_q),
        .out_valid(fl_out_v), .out_y(fl_out_y), .out_route(fl_out_route), .out_xl(fl_out_xl), .out_xr(fl_out_xr),
        .stat_chained
    );

    // ------------------------------------------------------------------ mixer
    logic        mx_iv;
    logic signed [47:0] mx_iy;
    logic [5:0]  mx_iroute;
    logic [15:0] mx_ixl, mx_ixr;
    always_comb begin
        if (filter_on) begin
            mx_iv = fl_out_v; mx_iy = fl_out_y; mx_iroute = fl_out_route; mx_ixl = fl_out_xl; mx_ixr = fl_out_xr;
        end else begin
            mx_iv = j4_v; mx_iy = j4_y; mx_iroute = j4_route; mx_ixl = j4_xl; mx_ixr = j4_xr;
        end
    end
    logic        m1_v, m2_v, m3_v, m4_v, m5_v;
    logic        m1_skip, m2_skip, m3_skip, m4_skip, m5_skip;
    logic [1:0]  m1_bl, m1_br, m2_bl, m2_br, m3_bl, m3_br, m4_bl, m4_br, m5_bl, m5_br;
    logic signed [31:0] m1_y;
    logic signed [23:0] m1_gl, m1_gr;
    logic signed [55:0] m4_pl, m4_pr;
    logic signed [33:0] m5_tl, m5_tr;
    logic signed [47:0] acc [0:3][0:1];
    logic [7:0]  n_mix;
    p2k_mul #(.WA(32), .WB(24), .LA(16), .LB(0)) u_ml (.clk, .a(m1_y), .b(m1_gl), .p(m4_pl));
    p2k_mul #(.WA(32), .WB(24), .LA(16), .LB(0)) u_mr (.clk, .a(m1_y), .b(m1_gr), .p(m4_pr));

    // ------------------------------------------------------------------ effects sends (S_MIX)
    logic [4:0]  xk;                  // product issued (4 b + j)
    logic signed [47:0] xa;
    logic signed [16:0] xb;
    logic signed [64:0] xp;
    logic [3:0]  xv;                  // products in flight (input register + 3 in p2k_mul)
    logic [1:0]  xvj [0:3];
    logic signed [63:0] rin, cin, bsl, bsr;
    p2k_mul #(.WA(48), .WB(17), .LA(24), .LB(0)) u_xm (.clk, .a(xa), .b(xb), .p(xp));

    // ------------------------------------------------------------------ tick (sound_base::tick_interps)
    logic        tk_done;
    logic [8:0]  tk_t;                // block being read
    logic        tk_v1, tk_v2, tk_v3;
    logic [7:0]  tk_b1, tk_b2, tk_b3;
    logic [15:0] tk_x2 [0:2], tk_x3 [0:2];
    logic [15:0] tk_di2 [0:2];

    logic signed [63:0] fin_l, fin_r;

    // ------------------------------------------------------------------ RAM read addresses
    logic [7:0]  wp_hb;               // write path: block
    always_comb begin
        gf_ra = wp_act ? wg_voice : fe_ra;
        ts_ra_v = fe_ra;
        ts_ra_nv = hvoice(nblk(hblk(fe_ra)));
        fc_ra = wp_act ? {wh_b, wh_o[4:3]} : fl_fc_ra;
        fst_ra = fl_fst_ra;
        hf1_ra = wp_act ? wh_b : e_hb;
        hf2_ra = fl_hfa_ra;
        hf3_ra = fl_hfb_ra;
        ipdi_ra = tk_t[7:0];
        for (int n = 0; n < 3; n++)
            ipx_ra[n] = wp_act ? wh_b : tk_run ? tk_t[7:0] : (n == 2) ? fl_ipx_ra : e_hb;
    end

    // ------------------------------------------------------------------ main
    always_ff @(posedge clk) begin
        done <= 1'b0;
        fx_pre <= 1'b0;
        fx_go <= 1'b0;
        for (int n = 0; n < 10; n++) gf_we[n] <= 1'b0;
        for (int n = 0; n < 4; n++) fc_we[n] <= 1'b0;
        for (int n = 0; n < 3; n++) begin
            ipx_we[n] <= 1'b0;
            ipdi_we[n] <= 1'b0;
        end
        vs_we <= 1'b0;
        tsv_we <= 1'b0;
        tco_we <= 1'b0;
        hf_we <= 1'b0;
        wp_fst_we <= 1'b0;

        if (rst) begin
            st <= S_CLR;
            clr_a <= '0;
            smp <= '0;
            fe <= FE_IDLE;
            vq_valid <= 1'b0;
            stat_div_cycles <= '0;
            rg_wp <= '0; rg_fp <= '0; rg_rp <= '0; rg_n <= '0;
            j1_v <= 1'b0; j2_v <= 1'b0; j3_v <= 1'b0; j4_v <= 1'b0;
            m1_v <= 1'b0; m2_v <= 1'b0; m3_v <= 1'b0; m4_v <= 1'b0; m5_v <= 1'b0;
            tk_run <= 1'b0; tk_done <= 1'b0; tk_v1 <= 1'b0; tk_v2 <= 1'b0; tk_v3 <= 1'b0;
            xv <= '0;
            for (int i = 0; i < 8; i++) begin
                k_level[i] <= '0;
                k_cut[i] <= '0;
            end
        end else begin
            // ======================================================== top sequence and write path
            case (st)
            S_CLR: begin
                clr_a <= clr_a + 11'd1;
                if (clr_a == 11'h7ff)
                    st <= S_IDLE;
            end
            S_IDLE: begin
                if (start) begin
                    fe <= FE_B;
                    fv <= '0;
                    n_push <= '0;
                    n_mix <= '0;
                    for (int b = 0; b < 4; b++) begin
                        acc[b][0] <= '0;
                        acc[b][1] <= '0;
                    end
                    fx_pre <= fx_on;
                    st <= S_RUN;
                end else if (wr_valid) begin
                    w_chip <= wr_chip;
                    w_addr <= wr_addr;
                    w_data <= (wr_chip == 2'd0 && wr_addr[4:0] == 5'd31) ? (wr_data & 16'h03ff) : wr_data;
                    w_mask <= wr_mask;
                    st <= S_W1;
                end
            end
            S_W1: st <= S_W2;                // the old values are read
            S_W2: begin                      // combine; side effects
                st <= S_W3;
                case (w_chip)
                2'd0: begin                  // sound_base::write_g20
                    automatic int f = g20_field(wg_word);
                    automatic logic [15:0] old = (f >= 0) ? gf_q[f] : 16'h0;
                    automatic logic [15:0] nw = (old & ~w_mask) | (w_data & w_mask);
                    gf_wa <= wg_voice;
                    gf_wd <= nw;
                    if (f >= 0) gf_we[f] <= 1'b1;
                    vs_wa <= wg_voice;
                    tsv_wa <= wg_voice;
                    if (wg_word == 5'd1) begin
                        vs_we <= 1'b1;
                        vs_wd <= {vs_q[38:1], nw[12]};
                    end
                    if (wg_word == 5'd13) begin   // restart
                        vs_we <= 1'b1;
                        vs_wd <= {gf_q[7][9:0], nw, 12'h0, 1'b1};
                        tsv_we <= 1'b1;
                        w_n <= '0;
                        st <= S_WC;
                    end
                end
                2'd1: if (wh_in) begin       // sound_base::write_h
                    if (wh_coef) begin
                        fc_wa <= {wh_b, wh_o[4:3]};
                        fc_wd <= (fc_q[wh_o[1:0]] & ~w_mask) | (w_data & w_mask);
                        fc_we[wh_o[1:0]] <= 1'b1;
                    end
                    if (wh_o == 6'h3d) begin
                        hf_we <= 1'b1;
                        hf_wa <= wh_b;
                        hf_wd <= (hf1_q & ~w_mask[7:0]) | (w_data[7:0] & w_mask[7:0]);
                    end
                    if (wh_o < 6'h14) begin
                        tco_we <= 1'b1;
                        tco_wa <= hvoice(wh_b);
                    end
                    for (int n = 0; n < 3; n++) begin
                        ipx_wa[n] <= wh_b;
                        ipx_wd[n] <= (ipx_q[n] & ~w_mask) | (w_data & w_mask);
                    end
                    ipdi_wa <= wh_b;
                    case (wh_o)
                    6'h1a: ipx_we[0] <= 1'b1;
                    6'h1b: ipx_we[1] <= 1'b1;
                    6'h1f: ipx_we[2] <= 1'b1;
                    6'h18, 6'h19, 6'h1e: if (w_mask[15:8] != 0) begin
                        automatic int n = (wh_o == 6'h18) ? 0 : (wh_o == 6'h19) ? 1 : 2;
                        automatic logic [2:0] k = (wh_o == 6'h1e) ? k_cut[wh_page] : k_level[wh_page];
                        w_tn <= 2'(n); w_tdest <= w_data[15:8]; w_tk <= k;
                        w_tdiff <= target_diff(w_data[15:8], ipx_q[n]);
                    end
                    6'h38, 6'h39: if (w_mask[7:0] != 0) begin
                        automatic int n = (wh_o == 6'h38) ? 0 : 1;
                        w_tn <= 2'(n); w_tdest <= w_data[7:0]; w_tk <= k_level[wh_page];
                        w_tdiff <= target_diff(w_data[7:0], ipx_q[n]);
                    end
                    6'h3e: if (w_mask[15:8] != 0) begin
                        w_tn <= 2'd2; w_tdest <= w_data[15:8]; w_tk <= k_cut[wh_page];
                        w_tdiff <= target_diff(w_data[15:8], ipx_q[2]);
                    end
                    6'h22: k_cut[wh_page] <= w_data[2:0];
                    6'h23: k_level[wh_page] <= w_data[2:0];
                    default: ;
                    endcase
                end
                default: ;                   // G05: p2k_fx
                endcase
                if (w_chip == 2'd1 && wh_in && ((wh_o == 6'h18 || wh_o == 6'h19 || wh_o == 6'h1e || wh_o == 6'h3e) &&
                    w_mask[15:8] != 0 || (wh_o == 6'h38 || wh_o == 6'h39) && w_mask[7:0] != 0))
                    st <= S_WT;
            end
            S_WT: begin                      // the interpolator's increment code (Table I encode): shift, clamp
                automatic logic signed [31:0] v = w_tdiff >>> (5 + w_tk);
                w_tdiff <= (v < -32'sd2048) ? -32'sd2048 : (v > 32'sd1984) ? 32'sd1984 : v;
                st <= S_WE;
            end
            S_WE: begin                      // encode
                ipdi_we[w_tn] <= 1'b1;
                ipdi_wd[w_tn] <= {w_tdest, inc_encode(w_tdiff)};
                st <= S_W3;
            end
            S_W3: st <= S_IDLE;              // the writes land
            S_WC: begin                      // restart: clear the voice's six filter sections
                wp_fst_we <= 1'b1;
                wp_fst_wa <= {wg_voice, w_n[2], w_n[1:0] == 2'd3 ? 2'd0 : w_n[1:0]};
                if (w_n == 3'd6) st <= S_IDLE;
                w_n <= (w_n == 3'd2) ? 3'd4 : w_n + 3'd1;
            end

            S_RUN: if (fe == FE_END && n_mix == n_push && !(filter_on && fl_busy) && !j1_v && !j2_v && !j3_v
                       && !j4_v && rg_n == 0 && !m1_v && !m2_v && !m3_v && !m4_v && !m5_v) begin
                fe <= FE_IDLE;
                tk_run <= 1'b1;
                tk_t <= '0;
                tk_done <= 1'b0;
                if (fx_on) begin
                    st <= S_MIX;
                    xk <= '0;
                    rin <= '0; cin <= '0; bsl <= '0; bsr <= '0;
                end else begin
                    fin_l <= 64'(acc[0][0]) + 64'(acc[1][0]) + 64'(acc[2][0]) + 64'(acc[3][0]);
                    fin_r <= 64'(acc[0][1]) + 64'(acc[1][1]) + 64'(acc[2][1]) + 64'(acc[3][1]);
                    st <= S_FIN;
                end
            end
            // effects sends (sound::step): rin, cin, the dry mix
            S_MIX: if (!fx_pre_busy) begin
                if (xk < 5'd16) begin
                    automatic logic [1:0] b = xk[3:2];
                    automatic logic signed [47:0] mono = 48'((64'(acc[b][0]) + 64'(acc[b][1]) + 64'sd1) >>> 1);
                    case (xk[1:0])
                        2'd0: begin xa <= mono; xb <= fx_rev_send[b]; end
                        2'd1: begin xa <= mono; xb <= fx_cho_send[b]; end
                        2'd2: begin xa <= acc[b][0]; xb <= fx_dry[b][0]; end
                        default: begin xa <= acc[b][1]; xb <= fx_dry[b][1]; end
                    endcase
                    xk <= xk + 5'd1;
                end
                if (xk == 5'd20) begin
                    automatic logic signed [63:0] q1 = (rin + 64'sd16384) >>> 15;
                    automatic logic signed [63:0] q2 = (cin + 64'sd16384) >>> 15;
                    fx_rev_in <= (q1 > 64'sd1073741823) ? 32'sd1073741823 : (q1 < -64'sd1073741823) ? -32'sd1073741823 : 32'(q1);
                    fx_cho_in <= (q2 > 64'sd1073741823) ? 32'sd1073741823 : (q2 < -64'sd1073741823) ? -32'sd1073741823 : 32'(q2);
                    fx_go <= 1'b1;
                    st <= S_FXW;
                end else if (xk >= 5'd16)
                    xk <= xk + 5'd1;
            end
            S_FXW: if (fx_done) begin
                fin_l <= bsl + 64'(fx_add_l);
                fin_r <= bsr + 64'(fx_add_r);
                st <= S_FIN;
            end
            S_FIN: if (tk_done) st <= S_OUT;
            S_OUT: begin
                automatic logic signed [63:0] ql = (fin_l + 64'sd256) >>> 9;
                automatic logic signed [63:0] qr = (fin_r + 64'sd256) >>> 9;
                out_l <= (ql > 64'sd32767) ? 16'sh7fff : (ql < -64'sd32768) ? -16'sh8000 : 16'(ql);
                out_r <= (qr > 64'sd32767) ? 16'sh7fff : (qr < -64'sd32768) ? -16'sh8000 : 16'(qr);
                smp <= smp + 48'd1;
                done <= 1'b1;
                st <= S_IDLE;
            end
            default: st <= S_IDLE;
            endcase

            // effects send products: 3 clocks in p2k_mul, plus the input register
            xv <= {xv[2:0], st == S_MIX && !fx_pre_busy && xk < 5'd16};
            xvj[0] <= xk[1:0];
            xvj[1] <= xvj[0];
            xvj[2] <= xvj[1];
            xvj[3] <= xvj[2];
            if (xv[3]) begin
                case (xvj[3])
                    2'd0: rin <= rin + $signed(xp[63:0]);
                    2'd1: cin <= cin + $signed(xp[63:0]);
                    2'd2: bsl <= bsl + (($signed(xp[63:0]) + 64'sd16384) >>> 15);
                    default: bsr <= bsr + (($signed(xp[63:0]) + 64'sd16384) >>> 15);
                endcase
            end

            // ======================================================== front end
            case (fe)
            FE_B: begin                      // registers of voice fv
                e_inc <= gf_q[0];
                e_end8 <= {gf_q[1], gf_q[2]};
                e_endc <= {gf_q[3], gf_q[4]};
                e_next <= {gf_q[5], gf_q[6]};
                e_addr <= vs_q[38:13];
                e_frac <= vs_q[12:1];
                e_sel <= vs_q[0];
                e_stv <= tsa_q;
                e_stnv <= tsb_q;
                e_conv <= tco_q;
                if (gf_q[0] == 16'h0) begin
                    if (fv == 7'd127) fe <= FE_END;
                    fv <= fv + 7'd1;
                end else
                    fe <= FE_C;
            end
            FE_C: begin
                // sound_base::chained: 12th-order continuation through the next slot [I]
                automatic logic ch = filter_on && (49'(e_conv) + 49'd2205 >= 49'(e_stv)) && (e_stnv < e_conv);
                automatic logic [16:0] f17 = 17'(e_frac) + 17'(e_inc);
                automatic logic [25:0] le = ~(e_sel ? e_endc[25:0] : e_end8[25:0]);
                e_ch <= ch;
                e_hb <= ch ? nblk(hblk(fv)) : hblk(fv);
                e_an <= 27'(e_addr) + 27'(f17[16:12]);
                e_nfrac <= f17[11:0];
                e_le <= le;
                e_ls <= e_next[25:0];
                e_nsel <= e_next[26];
                e_lsz <= 29'(le) + 29'sd1 - 29'(e_next[25:0]);
                fe <= FE_D;
            end
            FE_D: begin                      // the block's flags and level interpolators are read (e_hb)
                e_over <= e_an > 27'(e_le);
                e_ov <= e_an - 27'(e_le) - 27'd1;
                fe <= FE_E;
            end
            FE_E: begin                      // sound_base::advance
                e_route <= hf1_q[5:0];
                e_xr <= ipx_q[0];
                e_xl <= ipx_q[1];
                fe <= FE_P;
                if (!e_over) begin
                    n_addr <= e_an[25:0];
                    n_sel <= e_sel;
                end else begin
                    n_sel <= e_nsel;
                    if (e_nsel != e_sel)
                        n_addr <= e_ls;
                    else if (e_lsz < 0)
                        n_addr <= 26'(27'(e_ls) + e_ov);
                    else if (e_lsz == 0)
                        n_addr <= e_ls;
                    else if (e_ov < 27'(e_lsz))
                        n_addr <= 26'(27'(e_ls) + e_ov);
                    else begin               // overshoot of at least one loop: e_ov mod L
                        div_r <= 28'(e_ov);
                        div_l <= 27'(e_lsz);
                        div_k <= 5'd26;
                        fe <= FE_DIV;
                    end
                end
            end
            FE_DIV: begin                    // shift and subtract, L << k for k = 26..0
                automatic logic [53:0] lk = 54'(div_l) << div_k;
                stat_div_cycles <= stat_div_cycles + 32'd1;
                if (54'(div_r) >= lk)
                    div_r <= 28'(54'(div_r) - lk);
                if (div_k == 0) begin
                    automatic logic [27:0] rem = (54'(div_r) >= lk) ? 28'(54'(div_r) - lk) : div_r;
                    n_addr <= 26'(28'(e_ls) + rem);
                    fe <= FE_P;
                end else
                    div_k <= div_k - 5'd1;
            end
            FE_P: if (!vq_valid && rg_n < 5'(RN)) begin   // ask for the pair, save the voice
                vq_valid <= 1'b1;
                vq_addr <= SMP_BASE + {5'd0, e_addr[25:2]};
                vq_len <= (e_addr[1:0] == 2'd3 && e_addr != 26'h3ffffff) ? 8'd2 : 8'd1;
                rg_v[rg_wp] <= fv;
                rg_frac[rg_wp] <= e_frac;
                rg_ws[rg_wp] <= e_addr[1:0];
                rg_two[rg_wp] <= e_addr[1:0] == 2'd3 && e_addr != 26'h3ffffff;
                rg_s1z[rg_wp] <= e_addr == 26'h3ffffff;
                rg_ch[rg_wp] <= e_ch;
                rg_route[rg_wp] <= e_route;
                rg_xl[rg_wp] <= e_xl;
                rg_xr[rg_wp] <= e_xr;
                rg_dv[rg_wp] <= 1'b0;
                rg_half[rg_wp] <= 1'b0;
                rg_wp <= rg_wp + 4'd1;
                n_push <= n_push + 8'd1;
                vs_we <= 1'b1;
                vs_wa <= fv;
                vs_wd <= {n_addr, e_nfrac, n_sel};
                if (fv == 7'd127) fe <= FE_END;
                else fe <= FE_B;
                fv <= fv + 7'd1;
            end
            default: ;
            endcase
            if (vq_valid && c_req_ready[0])
                vq_valid <= 1'b0;

            // ======================================================== responses: fill the ring in order
            if (c_rsp_valid[0]) begin
                automatic logic [63:0] d = c_rsp_data;
                if (rg_two[rg_fp]) begin
                    if (!rg_half[rg_fp]) begin
                        rg_s0[rg_fp] <= $signed(d[63:48]);
                        rg_half[rg_fp] <= 1'b1;
                    end else begin
                        rg_s1[rg_fp] <= $signed(d[15:0]);
                        rg_dv[rg_fp] <= 1'b1;
                        rg_fp <= rg_fp + 4'd1;
                    end
                end else begin
                    case (rg_ws[rg_fp])
                        2'd0: begin rg_s0[rg_fp] <= $signed(d[15:0]);  rg_s1[rg_fp] <= $signed(d[31:16]); end
                        2'd1: begin rg_s0[rg_fp] <= $signed(d[31:16]); rg_s1[rg_fp] <= $signed(d[47:32]); end
                        2'd2: begin rg_s0[rg_fp] <= $signed(d[47:32]); rg_s1[rg_fp] <= $signed(d[63:48]); end
                        default: begin rg_s0[rg_fp] <= $signed(d[63:48]); rg_s1[rg_fp] <= 16'sh0; end   // a = 2^26 - 1
                    endcase
                    rg_dv[rg_fp] <= 1'b1;
                    rg_fp <= rg_fp + 4'd1;
                end
            end

            // ======================================================== join: take a voice when it can go on
            begin
                automatic logic [3:0] inflight = 4'(j1_v) + 4'(j2_v) + 4'(j3_v) + 4'(j4_v);
                automatic logic take = rg_n != 0 && rg_dv[rg_rp] && (!filter_on || fl_free > inflight);
                j1_v <= take;
                if (take) begin
                    j1_vo <= rg_v[rg_rp];
                    j1_ch <= rg_ch[rg_rp];
                    j1_route <= rg_route[rg_rp];
                    j1_xl <= rg_xl[rg_rp];
                    j1_xr <= rg_xr[rg_rp];
                    j1_s0 <= rg_s0[rg_rp];
                    j1_s1 <= rg_s1[rg_rp];
                    j1_frac <= rg_frac[rg_rp];
                    rg_rp <= rg_rp + 4'd1;
                end
                rg_n <= rg_n + 5'((fe == FE_P && !vq_valid && rg_n < 5'(RN)) ? 1 : 0) - 5'(take ? 1 : 0);
            end
            // (s1 - s0), (s1 - s0) frac, y = (s0 << 8) + (that >> 4)  [sound::step, Q8]
            j2_v <= j1_v; j2_vo <= j1_vo; j2_ch <= j1_ch; j2_route <= j1_route; j2_xl <= j1_xl; j2_xr <= j1_xr;
            j2_s0 <= j1_s0; j2_frac <= j1_frac;
            j2_d <= 17'(j1_s1) - 17'(j1_s0);
            j3_v <= j2_v; j3_vo <= j2_vo; j3_ch <= j2_ch; j3_route <= j2_route; j3_xl <= j2_xl; j3_xr <= j2_xr;
            j3_s0 <= j2_s0;
            j3_m <= 30'(j2_d * $signed({1'b0, j2_frac}));
            j4_v <= j3_v; j4_vo <= j3_vo; j4_ch <= j3_ch; j4_route <= j3_route; j4_xl <= j3_xl; j4_xr <= j3_xr;
            j4_y <= (48'(j3_s0) <<< 8) + 48'(j3_m >>> 4);

            // ======================================================== mixer (sound::step, per voice)
            m1_v <= mx_iv;
            if (mx_iv) begin
                m1_y <= 32'(clamp48(mx_iy, OUT_LIM));
                m1_gl <= $signed({1'b0, igain(mx_ixl)});
                m1_gr <= $signed({1'b0, igain(mx_ixr)});
                m1_skip <= mx_iroute == 6'h3f;
                m1_bl <= channel_bus(mx_iroute[5:3]);
                m1_br <= channel_bus(mx_iroute[2:0]);
            end
            m2_v <= m1_v; m2_skip <= m1_skip; m2_bl <= m1_bl; m2_br <= m1_br;
            m3_v <= m2_v; m3_skip <= m2_skip; m3_bl <= m2_bl; m3_br <= m2_br;
            m4_v <= m3_v; m4_skip <= m3_skip; m4_bl <= m3_bl; m4_br <= m3_br;
            m5_v <= m4_v; m5_skip <= m4_skip; m5_bl <= m4_bl; m5_br <= m4_br;
            m5_tl <= 34'((m4_pl + 56'sd2097152) >>> 22);
            m5_tr <= 34'((m4_pr + 56'sd2097152) >>> 22);
            if (m5_v) begin
                if (!m5_skip) begin
                    acc[m5_bl][0] <= acc[m5_bl][0] + 48'(m5_tl);
                    acc[m5_br][1] <= acc[m5_br][1] + 48'(m5_tr);
                end
                n_mix <= n_mix + 8'd1;
            end

            // ======================================================== interpolator tick, one block a clock
            if (tk_run) begin
                tk_v1 <= !tk_t[8];
                tk_b1 <= tk_t[7:0];
                if (!tk_t[8]) tk_t <= tk_t + 9'd1;
                tk_v2 <= tk_v1;
                tk_b2 <= tk_b1;
                tk_v3 <= tk_v2;
                tk_b3 <= tk_b2;
                for (int n = 0; n < 3; n++) begin
                    tk_x2[n] <= ipx_q[n];
                    tk_di2[n] <= ipdi_q[n];
                    tk_x3[n] <= ip_tick(tk_x2[n], tk_di2[n][15:8], tk_di2[n][7:0]);
                end
                if (tk_v3)
                    for (int n = 0; n < 3; n++) begin
                        ipx_we[n] <= 1'b1;
                        ipx_wa[n] <= tk_b3;
                        ipx_wd[n] <= tk_x3[n];
                    end
                if (tk_t[8] && !tk_v1 && !tk_v2 && !tk_v3) begin
                    tk_run <= 1'b0;
                    tk_done <= 1'b1;
                end
            end
        end
    end

`ifdef VERILATOR
    // ranges the narrowed datapath relies on (p2k_sound.h uses 64-bit integers)
    always_ff @(posedge clk) begin
        if (mx_iv && (mx_iy > OUT_LIM * 2 || mx_iy < -OUT_LIM * 2) && !filter_on)
            $fatal(1, "p2k_snd: unfiltered voice value out of range");
        for (int b = 0; b < 4; b++)
            for (int c = 0; c < 2; c++)
                if (acc[b][c] > 48'sh7fffffffff || acc[b][c] < -48'sh7fffffffff)
                    $fatal(1, "p2k_snd: bus accumulator beyond 2^39");
    end
`endif
endmodule
