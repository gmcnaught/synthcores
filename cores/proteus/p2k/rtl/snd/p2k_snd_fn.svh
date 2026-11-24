// SPDX-License-Identifier: GPL-3.0-or-later
// Functions shared by p2k_snd.sv and p2k_snd_filt.sv (included inside each module).
// H blocks are named by their index b = byte offset [14:7] (256 blocks of 0x80 bytes):
//   b[7] = voice bank, b[6:5] = page, b[4] = not a voice block, b[3:0] = slot.

// sound_base: voice v's own H block
function automatic logic [7:0] hblk(input logic [6:0] v);
    logic [1:0] pg;
    case (v[1:0])
        2'd0: pg = 2'd2;
        2'd1: pg = 2'd0;
        2'd2: pg = 2'd3;
        default: pg = 2'd1;
    endcase
    hblk = {v[6], pg, 1'b0, 4'(v[5:2] + 4'd15)};
endfunction

// the next and previous slot's block (same page)
function automatic logic [7:0] nblk(input logic [7:0] b);
    nblk = {b[7:4], 4'(b[3:0] + 4'd1)};
endfunction
function automatic logic [7:0] pblk(input logic [7:0] b);
    pblk = {b[7:4], 4'(b[3:0] - 4'd1)};
endfunction

// the voice whose block is b (bit 4 ignored)
function automatic logic [6:0] hvoice(input logic [7:0] b);
    logic [1:0] lane;
    case (b[6:5])
        2'd0: lane = 2'd1;
        2'd1: lane = 2'd3;
        2'd2: lane = 2'd0;
        default: lane = 2'd2;
    endcase
    hvoice = {b[7], 4'(b[3:0] + 4'd1), lane};
endfunction

// sound::hdec: Q8 code (0..65535) -> decoded value, at most 2^26
function automatic logic [26:0] hdec(input logic [16:0] code_q8);
    logic [17:0] c;
    logic [3:0]  e;
    logic [11:0] m;
    c = 18'(code_q8) + 18'd256;
    e = c[15:12];
    m = c[11:0];
    if (c >= 18'd57344)
        hdec = 27'(c - 18'd49152) << 12;
    else if (e != 0)
        hdec = 27'({1'b1, m}) << (e - 4'd1);
    else
        hdec = 27'(m);
endfunction

// sound::interp_gain: level interpolator x -> Q22 gain (at most 2^22)
function automatic logic [22:0] igain(input logic [15:0] x);
    logic [26:0] h;
    h = hdec(17'(x) - 17'(x >> 8));
    igain = (x != 0) ? 23'(h >> 4) : 23'd0;
endfunction

function automatic logic [1:0] channel_bus(input logic [2:0] c);
    case (c[1:0])
        2'd0: channel_bus = 2'd2;
        2'd1: channel_bus = 2'd3;
        2'd2: channel_bus = 2'd0;
        default: channel_bus = 2'd1;
    endcase
endfunction

localparam logic signed [47:0] SIG_LIM   = 48'sh7fffffffffff;        // 2^47 - 1
localparam logic signed [47:0] OUT_LIM   = 48'sh00007fffffff;        // 2^31 - 1
localparam logic signed [47:0] VOICE_LIM = 48'sh0000007fff00;        // 32767 << 8

localparam logic signed [63:0] SIG_LIM64 = 64'sh00007fffffffffff;

function automatic logic signed [47:0] sat47(input logic signed [63:0] v);
    sat47 = (v > SIG_LIM64) ? SIG_LIM : (v < -SIG_LIM64) ? -SIG_LIM : 48'(v);
endfunction

function automatic logic signed [47:0] clamp48(input logic signed [47:0] v, input logic signed [47:0] lim);
    clamp48 = (v > lim) ? lim : (v < -lim) ? -lim : v;
endfunction
