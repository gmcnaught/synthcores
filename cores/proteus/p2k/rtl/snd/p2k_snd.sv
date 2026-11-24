// SPDX-License-Identifier: GPL-3.0-or-later
// Proteus 2000 sound engine: G20 voices (128), H level and cutoff interpolators, H filter and the
// output mixer. The definition it matches bit for bit is the integer model
// p2k/model/p2k_sound.h (docs/p2k-mame-plan.md); comments name the model function each part follows.
//
// Interface: one register write at a time while idle (wr_valid/wr_ready, COMBINE_DATA mask),
// then a start pulse computes one 44.1 kHz output sample (done pulse, out_l/out_r). Sample memory is
// read one 16-bit word at a time (mem_req held until mem_ack); addresses >= 2^26 read as 0.
// Writes made between two start pulses take effect before the second one's sample, as in the model.
//
// Iteration 14a: voices, level interpolators, mixer (filter_on = 0, fx_on = 0).
// Iteration 14b: H filter (filter_on = 1).  Iteration 14c: G05 effects (fx_on = 1, p2k_fx.sv).

module p2k_snd (
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

    output logic        mem_req,
    output logic [26:0] mem_addr,
    input  logic        mem_ack,
    input  logic [15:0] mem_data,

    output logic [31:0] stat_div_cycles,  // clocks spent in the loop-modulo divider (measurement)
    output logic [31:0] stat_chained      // voice-samples filtered as 12th-order chains (measurement)
);

    // ------------------------------------------------------------------ model helpers

    // sound_base::h_block / h_voice / next_block / prev_block
    function automatic logic [14:0] h_block(input logic [6:0] v);
        logic [14:0] page;
        case (v[1:0])
            2'd0: page = 15'h2000;
            2'd1: page = 15'h0000;
            2'd2: page = 15'h3000;
            default: page = 15'h1000;
        endcase
        h_block = {v[6], 14'h0} + page + {4'(v[5:2] + 4'd15), 7'h0};
    endfunction

    function automatic logic [6:0] h_voice(input logic [14:0] blk);
        logic [3:0] slot;
        logic [1:0] lane;
        slot = blk[10:7] + 4'd1;
        case (blk[13:12])
            2'd0: lane = 2'd1;
            2'd1: lane = 2'd3;
            2'd2: lane = 2'd0;
            default: lane = 2'd2;
        endcase
        h_voice = {blk[14], slot, lane};
    endfunction

    function automatic logic [14:0] next_block(input logic [14:0] blk);
        next_block = (blk[10:7] == 4'hf) ? blk - 15'h780 : blk + 15'h80;
    endfunction

    function automatic logic [14:0] prev_block(input logic [14:0] blk);
        prev_block = (blk[10:7] == 4'h0) ? blk + 15'h780 : blk - 15'h80;
    endfunction

    // sound_base::inc_decode: Table I code -> -2048..1984
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

    // sound_base::inc_encode: largest Table I value not above v, saturating
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

    // sound_base::set_target: increment code aiming at dest in about 32 << k samples
    function automatic logic [7:0] target_inc(input logic [7:0] dest, input logic [15:0] x, input logic [2:0] k);
        logic signed [31:0] diff;
        diff = $signed({16'd0, dest, dest}) - $signed({16'd0, x});
        target_inc = inc_encode(diff >>> (5 + k));
    endfunction

    // sound::hdec: Q8 code -> decoded value (f^2 in Q24 scale)
    function automatic logic signed [31:0] hdec(input logic signed [31:0] code_q8);
        logic signed [31:0] c, m;
        logic [3:0] e;
        c = code_q8 + 32'sd256;
        if (c >= 32'sd57344)
            hdec = (c - 32'sd49152) <<< 12;
        else begin
            e = 4'(c >>> 12);
            m = c & 32'hfff;
            hdec = (e != 0) ? ((32'sd4096 + m) <<< (e - 4'd1)) : m;
        end
    endfunction

    // sound::interp_gain: level interpolator x -> Q22 gain
    function automatic logic signed [63:0] interp_gain(input logic [15:0] x);
        logic signed [31:0] h;
        h = hdec(32'(x) - 32'(x >> 8));
        interp_gain = (x != 0) ? 64'(h >>> 4) : 64'sd0;
    endfunction

    function automatic logic [1:0] channel_bus(input logic [2:0] c);
        case (c[1:0])
            2'd0: channel_bus = 2'd2;
            2'd1: channel_bus = 2'd3;
            2'd2: channel_bus = 2'd0;
            default: channel_bus = 2'd1;
        endcase
    endfunction

    // rsh: shift right rounding half up
    function automatic logic signed [63:0] rsh64(input logic signed [63:0] v, input int s);
        rsh64 = (v + (64'sd1 <<< (s - 1))) >>> s;
    endfunction

    // ---- H filter (sound::stage)
    localparam logic signed [127:0] ONE = 128'sd1 <<< 28;
    localparam logic signed [63:0] SIG_LIM = (64'sd1 <<< 47) - 64'sd1;
    localparam logic signed [63:0] OUT_LIM = (64'sd1 <<< 31) - 64'sd1;
    localparam logic signed [63:0] VOICE_LIM = 64'sd32767 <<< 8;

    function automatic logic signed [63:0] rsh128(input logic signed [127:0] v, input int s);
        logic signed [127:0] q;
        q = (v + (128'sd1 <<< (s - 1))) >>> s;
        rsh128 = q[63:0];
    endfunction

    function automatic logic signed [63:0] sat47(input logic signed [63:0] v);
        sat47 = (v > SIG_LIM) ? SIG_LIM : (v < -SIG_LIM) ? -SIG_LIM : v;
    endfunction

    function automatic logic signed [63:0] clampv(input logic signed [63:0] v, input logic signed [63:0] lim);
        clampv = (v > lim) ? lim : (v < -lim) ? -lim : v;
    endfunction

    // Q8 code C0 + x (C255 - C0), rounded; shift selects the byte (0 = low: f^2, 8 = high: B2')
    function automatic logic signed [31:0] hcode(input logic [15:0] hi, input logic [15:0] lo, input logic hb,
                                                 input logic [15:0] t);
        logic signed [31:0] a, b;
        a = $signed({24'd0, hb ? hi[15:8] : hi[7:0]});
        b = $signed({24'd0, hb ? lo[15:8] : lo[7:0]});
        hcode = ((a <<< 16) + (b - a) * $signed({16'd0, t}) + 32'sd128) >>> 8;
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

    // ------------------------------------------------------------------ state

    // G20: raw registers (read-modify-write) and per-voice copies of the words the voice loop reads
    logic [15:0] g20 [0:4095];
    // vf index: 0 = +0x04 (inc), 1/2 = +0x08/+0x0A, 3/4 = +0x0C/+0x0E, 5/6 = +0x14/+0x16, 7 = +0x18
    logic [15:0] vf [0:7][0:127];
    logic [25:0] v_addr [0:127];
    logic [11:0] v_frac [0:127];
    logic        v_sel  [0:127];
    logic [47:0] v_start [0:127];
    logic [47:0] v_coef  [0:127];

    // H registers below byte offset 0x8000 (the voice blocks); writes above are ignored here
    logic [15:0] hram [0:16383];
    // interpolators per H block: 0 = right level, 1 = left level, 2 = cutoff of the next slot
    logic [15:0] ip_x [0:2][0:255];
    logic [7:0]  ip_d [0:2][0:255];
    logic [7:0]  ip_i [0:2][0:255];
    logic [2:0]  k_level [0:7];
    logic [2:0]  k_cut   [0:7];


    logic [47:0] smp;

    // filter state [voice][stage][section] w1, w2 (Q8)
    logic signed [63:0] fst1 [0:127][0:1][0:2];
    logic signed [63:0] fst2 [0:127][0:1][0:2];

    // ------------------------------------------------------------------ write path

    typedef enum logic [5:0] {
        S_IDLE, S_W1, S_W2,
        S_VA, S_VB, S_VM0, S_VM0W, S_VM1, S_VM1W, S_VC, S_VDIV, S_VMIX,
        S_FS, S_FSEC, S_FGAIN, S_FX0, S_FXC, S_FX1, S_FXW,
        S_TICK, S_OUT
    } state_t;
    state_t st;

    logic [1:0]  w_chip;
    logic [18:0] w_addr;
    logic [15:0] w_data, w_mask, w_old, w_w18;
    logic [15:0] w_ipx [0:2];

    assign wr_ready = (st == S_IDLE) && !start;

    wire [6:0]  wg_voice = w_addr[11:5];
    wire [4:0]  wg_word  = w_addr[4:0];
    wire [14:0] wh_blk   = {w_addr[13:6], 7'h0};
    wire [6:0]  wh_o     = {w_addr[5:0], 1'b0};
    wire [2:0]  wh_page  = w_addr[13:11];
    wire [15:0] w_new    = (w_old & ~w_mask) | (w_data & w_mask);

    function automatic int vf_index(input logic [4:0] word);
        case (word)
            5'd2:  vf_index = 0;
            5'd4:  vf_index = 1;
            5'd5:  vf_index = 2;
            5'd6:  vf_index = 3;
            5'd7:  vf_index = 4;
            5'd10: vf_index = 5;
            5'd11: vf_index = 6;
            5'd12: vf_index = 7;
            default: vf_index = -1;
        endcase
    endfunction

    // ------------------------------------------------------------------ voice loop registers

    logic [6:0]  v;
    logic [15:0] r_inc;
    logic [31:0] r_end8, r_endc, r_next;
    logic [25:0] r_addr;
    logic [11:0] r_frac;
    logic        r_sel;
    logic [14:0] r_hblk, r_hblk0;
    logic [47:0] r_st_v, r_st_nv, r_co_nv;
    logic        r_chain;
    // filter
    logic        f_stage;
    logic [14:0] f_blk;
    logic [15:0] f_t;
    logic        f_unnorm, f_gmode;
    logic [1:0]  f_sct;
    logic signed [63:0] f_x;
    logic signed [63:0] r_s0, r_s1, r_y;
    logic [5:0]  r_route;
    logic [15:0] r_xl, r_xr;
    logic signed [63:0] acc [0:3][0:1];

    // advance (sound_base::advance)
    logic [26:0] na;
    logic [12:0] nfrac;
    logic [25:0] le;
    logic [26:0] over;
    logic [27:0] div_r;      // remainder being reduced
    logic [26:0] div_l;
    logic [25:0] div_ls;
    logic [4:0]  div_k;
    logic        r_nsel;

    // tick pipeline
    logic [8:0]  t;
    logic        t_v;
    logic [7:0]  t_idx;
    logic [15:0] t_x [0:2];
    logic [7:0]  t_d [0:2];
    logic [7:0]  t_i [0:2];

    logic signed [63:0] suml, sumr, fin_l, fin_r, base_l, base_r;

    // ------------------------------------------------------------------ G05 effects
    logic fx_cfg, fx_go, fx_done;
    logic signed [31:0] fx_rev_in, fx_cho_in;
    logic signed [63:0] fx_add_l, fx_add_r;
    logic signed [31:0] fx_rev_send [0:3], fx_cho_send [0:3], fx_dry [0:3][0:1];
    wire fx_wr = (st == S_W2) && (w_chip == 2'd2);

    p2k_fx fx (
        .clk, .rst,
        .wr_en(fx_wr), .wr_addr(w_addr[10:0]), .wr_data(w_data), .wr_mask(w_mask),
        .cfg(fx_cfg), .go(fx_go), .rev_in(fx_rev_in), .cho_in(fx_cho_in),
        .done(fx_done), .add_l(fx_add_l), .add_r(fx_add_r),
        .rev_send(fx_rev_send), .cho_send(fx_cho_send), .dry(fx_dry)
    );
    wire state_t after_adv = filter_on ? S_FS : S_VMIX;

    always_comb begin
        suml = acc[0][0] + acc[1][0] + acc[2][0] + acc[3][0];
        sumr = acc[0][1] + acc[1][1] + acc[2][1] + acc[3][1];
    end

    function automatic logic signed [15:0] clamp16(input logic signed [63:0] s);
        logic signed [63:0] q;
        q = rsh64(s, 9);
        clamp16 = (q > 64'sd32767) ? 16'sh7fff : (q < -64'sd32768) ? -16'sh8000 : 16'(q);
    endfunction

    always_ff @(posedge clk) begin
        done <= 1'b0;
        if (rst) begin
            st <= S_IDLE;
            smp <= '0;
            mem_req <= 1'b0;
            stat_div_cycles <= '0;
            stat_chained <= '0;
            fx_cfg <= 1'b0;
            fx_go <= 1'b0;
            for (int i = 0; i < 8; i++) begin
                k_level[i] <= '0;
                k_cut[i] <= '0;
            end
            for (int i = 0; i < 128; i++) begin
                v_addr[i] <= '0; v_frac[i] <= '0; v_sel[i] <= 1'b0;
                v_start[i] <= '0; v_coef[i] <= '0;
            end
        end else begin
            case (st)
            S_IDLE: begin
                if (start) begin
                    v <= 7'd0;
                    f_stage <= 1'b0;
                    for (int b = 0; b < 4; b++) begin
                        acc[b][0] <= '0;
                        acc[b][1] <= '0;
                    end
                    st <= S_VA;
                end else if (wr_valid) begin
                    w_chip <= wr_chip;
                    w_addr <= wr_addr;
                    w_data <= (wr_chip == 2'd0 && wr_addr[4:0] == 5'd31) ? (wr_data & 16'h03ff) : wr_data;
                    w_mask <= wr_mask;
                    st <= S_W1;
                end
            end

            // ---------------- register write: read the old value
            S_W1: begin
                case (w_chip)
                    2'd0: begin
                        w_old <= g20[w_addr[11:0]];
                        w_w18 <= vf[7][wg_voice];
                    end
                    2'd1: begin
                        w_old <= (w_addr < 19'h4000) ? hram[w_addr[13:0]] : 16'h0;
                        for (int n = 0; n < 3; n++)
                            w_ipx[n] <= ip_x[n][w_addr[13:6]];
                    end
                    default: w_old <= 16'h0;
                endcase
                st <= S_W2;
            end

            // ---------------- register write: combine and side effects
            S_W2: begin
                st <= S_IDLE;
                case (w_chip)
                2'd0: begin   // sound_base::write_g20
                    g20[w_addr[11:0]] <= w_new;
                    if (vf_index(wg_word) >= 0)
                        vf[vf_index(wg_word)][wg_voice] <= w_new;
                    if (wg_word == 5'd1)
                        v_sel[wg_voice] <= w_new[12];
                    if (wg_word == 5'd13) begin   // restart
                        v_addr[wg_voice] <= {w_w18[9:0], w_new};
                        v_frac[wg_voice] <= '0;
                        v_sel[wg_voice] <= 1'b1;
                        v_start[wg_voice] <= smp;
                        for (int a = 0; a < 2; a++)
                            for (int b = 0; b < 3; b++) begin
                                fst1[wg_voice][a][b] <= '0;
                                fst2[wg_voice][a][b] <= '0;
                            end
                    end
                end
                2'd1: begin   // sound_base::write_h
                    if (w_addr < 19'h4000) begin
                        hram[w_addr[13:0]] <= w_new;
                        if (wh_o < 7'h28)
                            v_coef[h_voice(wh_blk)] <= smp;
                        case (wh_o)
                        7'h34: ip_x[0][w_addr[13:6]] <= (w_ipx[0] & ~w_mask) | (w_data & w_mask);
                        7'h36: ip_x[1][w_addr[13:6]] <= (w_ipx[1] & ~w_mask) | (w_data & w_mask);
                        7'h3e: ip_x[2][w_addr[13:6]] <= (w_ipx[2] & ~w_mask) | (w_data & w_mask);
                        7'h30, 7'h32, 7'h3c: if (w_mask[15:8] != 0) begin
                            automatic int n = (wh_o == 7'h30) ? 0 : (wh_o == 7'h32) ? 1 : 2;
                            automatic logic [2:0] k = (wh_o == 7'h3c) ? k_cut[wh_page] : k_level[wh_page];
                            ip_d[n][w_addr[13:6]] <= w_data[15:8];
                            ip_i[n][w_addr[13:6]] <= target_inc(w_data[15:8], w_ipx[n], k);
                        end
                        7'h70, 7'h72: if (w_mask[7:0] != 0) begin
                            automatic int n = (wh_o == 7'h70) ? 0 : 1;
                            ip_d[n][w_addr[13:6]] <= w_data[7:0];
                            ip_i[n][w_addr[13:6]] <= target_inc(w_data[7:0], w_ipx[n], k_level[wh_page]);
                        end
                        7'h7c: if (w_mask[15:8] != 0) begin
                            ip_d[2][w_addr[13:6]] <= w_data[15:8];
                            ip_i[2][w_addr[13:6]] <= target_inc(w_data[15:8], w_ipx[2], k_cut[wh_page]);
                        end
                        7'h44: k_cut[wh_page] <= w_data[2:0];
                        7'h46: k_level[wh_page] <= w_data[2:0];
                        default: ;
                        endcase
                    end
                end
                default: ;   // G05: stored by p2k_fx (fx_wr)
                endcase
            end

            // ---------------- voice loop (sound::step)
            S_VA: begin
                r_inc  <= vf[0][v];
                r_end8 <= {vf[1][v], vf[2][v]};
                r_endc <= {vf[3][v], vf[4][v]};
                r_next <= {vf[5][v], vf[6][v]};
                r_addr <= v_addr[v];
                r_frac <= v_frac[v];
                r_sel  <= v_sel[v];
                r_hblk0 <= h_block(v);
                r_st_v  <= v_start[v];
                r_st_nv <= v_start[h_voice(next_block(h_block(v)))];
                r_co_nv <= v_coef[h_voice(next_block(h_block(v)))];
                st <= S_VB;
            end
            S_VB: begin
                if (r_inc == 0) begin
                    if (v == 7'd127) begin
                        st <= S_FX0;
                    end else begin
                        v <= v + 7'd1; st <= S_VA;
                    end
                end else begin
                    // sound_base::chained: 12th-order continuation through the next slot [I]
                    automatic logic ch = filter_on && (49'(r_co_nv) + 49'd2205 >= 49'(r_st_v)) && (r_st_nv < r_co_nv);
                    automatic logic [14:0] hb = ch ? next_block(r_hblk0) : r_hblk0;
                    r_chain <= ch;
                    r_hblk <= hb;
                    r_route <= hram[{hb[14:7], 6'h3d}][5:0];
                    r_xr <= ip_x[0][hb[14:7]];
                    r_xl <= ip_x[1][hb[14:7]];
                    mem_req <= 1'b1;
                    mem_addr <= {1'b0, r_addr};
                    st <= S_VM0W;
                end
            end
            S_VM0W: if (mem_ack) begin
                r_s0 <= 64'(signed'(mem_data));
                mem_addr <= {1'b0, r_addr} + 27'd1;
                st <= S_VM1W;
            end
            S_VM1W: if (mem_ack) begin
                r_s1 <= mem_addr[26] ? 64'sd0 : 64'(signed'(mem_data));
                mem_req <= 1'b0;
                st <= S_VC;
            end
            S_VC: begin
                automatic logic [26:0] a_n;
                automatic logic [25:0] le_n;
                automatic logic [26:0] ov;
                automatic logic [25:0] ls;
                automatic logic nsel;
                automatic logic signed [28:0] lsz;
                r_y <= (r_s0 <<< 8) + (((r_s1 - r_s0) * $signed({52'd0, r_frac})) >>> 4);
                // f = frac + inc (17 bits); na = a + (f >> 12)
                begin
                    automatic logic [16:0] f17 = 17'(r_frac) + 17'(r_inc);
                    a_n = 27'(r_addr) + 27'(f17[16:12]);
                    v_frac[v] <= f17[11:0];
                end
                le_n = ~(r_sel ? r_endc[25:0] : r_end8[25:0]);
                ls = r_next[25:0];
                nsel = r_next[26];
                if (a_n > 27'(le_n)) begin
                    ov = a_n - 27'(le_n) - 27'd1;
                    v_sel[v] <= nsel;
                    if (nsel == r_sel) begin
                        lsz = 29'(le_n) + 29'sd1 - 29'(ls);
                        if (lsz < 0) begin
                            v_addr[v] <= 26'(27'(ls) + ov);
                            st <= after_adv;
                        end else if (lsz == 0) begin
                            v_addr[v] <= ls;
                            st <= after_adv;
                        end else if (ov < 27'(lsz)) begin
                            v_addr[v] <= 26'(27'(ls) + ov);
                            st <= after_adv;
                        end else begin
                            div_r <= 28'(ov);
                            div_l <= 27'(lsz);
                            div_ls <= ls;
                            div_k <= 5'd26;
                            st <= S_VDIV;
                        end
                    end else begin
                        v_addr[v] <= ls;
                        st <= after_adv;
                    end
                end else begin
                    v_addr[v] <= a_n[25:0];
                    st <= after_adv;
                end
            end
            // over % L by shift-and-subtract (L << k for k = 26..0)
            S_VDIV: begin
                automatic logic [53:0] lk = 54'(div_l) << div_k;
                stat_div_cycles <= stat_div_cycles + 32'd1;
                if (54'(div_r) >= lk)
                    div_r <= 28'(54'(div_r) - lk);
                if (div_k == 0) begin
                    automatic logic [27:0] rem = (54'(div_r) >= lk) ? 28'(54'(div_r) - lk) : div_r;
                    v_addr[v] <= 26'(28'(div_ls) + rem);
                    st <= after_adv;
                end else
                    div_k <= div_k - 5'd1;
            end
            // ---------------- H filter (sound::filter / sound::stage)
            S_FS: begin
                automatic logic [14:0] blk = f_stage ? next_block(r_hblk0) : r_hblk0;
                automatic logic [14:0] pb = prev_block(blk);
                f_blk <= blk;
                f_t <= ip_x[2][pb[14:7]];
                f_unnorm <= hram[{pb[14:7], 6'h3d}][7];
                f_gmode <= hram[{blk[14:7], 6'h3d}][6];
                f_sct <= 2'd0;
                if (!f_stage)
                    f_x <= r_y;
                st <= S_FSEC;
            end
            S_FSEC: begin
                automatic logic [3:0] pw = {f_sct, 2'b00};                 // pole words 0, 4, 8
                automatic logic [3:0] zw = {f_sct, 2'b01};                 // zero words 1, 5, 9
                automatic logic has_z = !(f_sct == 2'd2 && f_gmode);
                automatic logic [15:0] phi = hram[{f_blk[14:7], 6'(2 * pw)}];
                automatic logic [15:0] plo = hram[{f_blk[14:7], 6'(2 * pw + 1)}];
                automatic logic [15:0] zhi = hram[{f_blk[14:7], 6'(2 * zw)}];
                automatic logic [15:0] zlo = hram[{f_blk[14:7], 6'(2 * zw + 1)}];
                automatic logic p = !(phi == 16'hffcf && plo == 16'hffcf);
                automatic logic z = has_z && !(zhi == 16'hffcf && zlo == 16'hffcf);
                automatic logic signed [127:0] pf2 = p ? 128'(hdec(hcode(phi, plo, 1'b0, f_t))) <<< 4 : ONE;
                automatic logic signed [127:0] pb2 = p ? 128'(hdec(hcode(phi, plo, 1'b1, f_t))) <<< 2 : ONE;
                automatic logic signed [127:0] zf2 = z ? 128'(hdec(hcode(zhi, zlo, 1'b0, f_t))) <<< 4 : ONE;
                automatic logic signed [127:0] zb2 = z ? 128'(hdec(hcode(zhi, zlo, 1'b1, f_t))) <<< 2 : ONE;
                automatic logic signed [127:0] g  = (f_sct != 0 && f_unnorm) ? ONE : pf2;
                automatic logic signed [127:0] a1 = 2 * ONE - pf2 - pb2;
                automatic logic signed [127:0] a2 = ONE - pb2;
                automatic logic signed [127:0] n1 = 2 * ONE - zf2 - zb2;
                automatic logic signed [127:0] n2 = ONE - zb2;
                automatic logic signed [127:0] s0 = 128'(fst1[v][f_stage][f_sct]);
                automatic logic signed [127:0] s1 = 128'(fst2[v][f_stage][f_sct]);
                automatic logic signed [63:0] w, xo;
                if (p || z) begin
                    w = sat47(rsh128(g * 128'(f_x) + a1 * s0 - a2 * s1, 28));
                    xo = sat47(rsh128((128'(w) <<< 28) - n1 * s0 + n2 * s1, 28));
                    fst2[v][f_stage][f_sct] <= fst1[v][f_stage][f_sct];
                    fst1[v][f_stage][f_sct] <= w;
                    f_x <= xo;
                end
                if (f_sct == 2'd2)
                    st <= S_FGAIN;
                f_sct <= f_sct + 2'd1;
            end
            S_FGAIN: begin
                automatic logic [15:0] ghi = hram[{f_blk[14:7], 6'd18}];
                automatic logic [15:0] glo = hram[{f_blk[14:7], 6'd19}];
                automatic logic signed [63:0] x = clampv(f_x, VOICE_LIM);
                if (f_gmode && !(ghi == 16'hffcf && glo == 16'hffcf)) begin
                    automatic logic signed [63:0] gg = (64'(hdec(hcode(ghi, glo, 1'b0, f_t))) * 64'sd16644) >>> 12;
                    x = sat47(rsh128(128'(x) * 128'(gg), 28));
                end
                if (!f_stage && r_chain) begin
                    stat_chained <= stat_chained + 32'd1;
                    f_stage <= 1'b1;
                    f_x <= x;
                    st <= S_FS;
                end else begin
                    f_stage <= 1'b0;
                    r_y <= clampv(x, OUT_LIM);
                    st <= S_VMIX;
                end
            end

            S_VMIX: begin
                if (r_route != 6'h3f) begin
                    acc[channel_bus(r_route[5:3])][0] <= acc[channel_bus(r_route[5:3])][0] + rsh64(r_y * interp_gain(r_xl), 22);
                    acc[channel_bus(r_route[2:0])][1] <= acc[channel_bus(r_route[2:0])][1] + rsh64(r_y * interp_gain(r_xr), 22);
                end
                if (v == 7'd127) begin
                    st <= S_FX0;
                end else begin
                    v <= v + 7'd1; st <= S_VA;
                end
            end

            // ---------------- effects and output mix (sound::step after the voice loop)
            S_FX0: begin
                if (!fx_on) begin
                    fin_l <= suml;
                    fin_r <= sumr;
                    st <= S_TICK; t <= '0; t_v <= 1'b0;
                end else begin
                    fx_cfg <= (smp[7:0] == 8'h00);   // fx_int::configure every 256 samples
                    st <= S_FXC;
                end
            end
            S_FXC: begin                            // p2k_fx takes cfg on this clock
                fx_cfg <= 1'b0;
                st <= S_FX1;
            end
            S_FX1: begin
                automatic logic signed [63:0] rin = '0, cin = '0, bl = '0, br = '0, q;
                fx_cfg <= 1'b0;
                for (int b = 0; b < 4; b++) begin
                    automatic logic signed [63:0] mono = rsh64(acc[b][0] + acc[b][1], 1);
                    rin += mono * 64'(fx_rev_send[b]);
                    cin += mono * 64'(fx_cho_send[b]);
                    bl += rsh64(acc[b][0] * 64'(fx_dry[b][0]), 15);
                    br += rsh64(acc[b][1] * 64'(fx_dry[b][1]), 15);
                end
                q = rsh64(rin, 15);
                fx_rev_in <= 32'(clampv(q, 64'sd1073741823));
                q = rsh64(cin, 15);
                fx_cho_in <= 32'(clampv(q, 64'sd1073741823));
                base_l <= bl;
                base_r <= br;
                fx_go <= 1'b1;
                st <= S_FXW;
            end
            S_FXW: begin
                fx_go <= 1'b0;
                if (fx_done) begin
                    fin_l <= base_l + fx_add_l;
                    fin_r <= base_r + fx_add_r;
                    st <= S_TICK; t <= '0; t_v <= 1'b0;
                end
            end

            // ---------------- interpolators (sound_base::tick_interps), one H block a clock
            S_TICK: begin
                if (t_v)
                    for (int n = 0; n < 3; n++)
                        ip_x[n][t_idx] <= ip_tick(t_x[n], t_d[n], t_i[n]);
                if (t[8]) begin
                    st <= S_OUT;
                end else begin
                    for (int n = 0; n < 3; n++) begin
                        t_x[n] <= ip_x[n][t[7:0]];
                        t_d[n] <= ip_d[n][t[7:0]];
                        t_i[n] <= ip_i[n][t[7:0]];
                    end
                    t_idx <= t[7:0];
                    t_v <= 1'b1;
                    t <= t + 9'd1;
                end
            end

            S_OUT: begin
                out_l <= clamp16(fin_l);
                out_r <= clamp16(fin_r);
                smp <= smp + 48'd1;
                done <= 1'b1;
                st <= S_IDLE;
            end
            default: st <= S_IDLE;
            endcase
        end
    end

endmodule
