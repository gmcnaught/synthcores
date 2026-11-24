// SPDX-License-Identifier: GPL-2.0-or-later
// MT-32 output mix, bit-exact to the end of la32_t::clock in Nuked-MT32 (nukeykt), 5b4e239:
//   s_l = expand(acc2) + expand(acc3) + reverb1 * 17 / 25
//   s_r = expand(acc6) + expand(acc7) + reverb0 * 17 / 25
//   s = s * (255 - pwm) / 255, clamped to 16 bits
// C division truncates toward zero; both divisions are exact reciprocal products here
// (|x| <= 557,056 for /25: (|x| * 1342178) >> 25; |x| <= 22,393,590 for /255:
// (|x| * 33686019) >> 33; checked for every value in those ranges).
// go latches the inputs; valid pulses with out_l/out_r a few clocks later.
module mt32_mix (
    input  logic        clk,
    input  logic        rst,
    input  logic        go,
    input  logic signed [15:0] acc2, acc3, acc6, acc7,   // clamped accumulators of the sample
    input  logic signed [15:0] rv0, rv1,
    input  logic [7:0]  pwm,
    output logic        valid,
    output logic signed [15:0] out_l,
    output logic signed [15:0] out_r
);
    function automatic logic signed [15:0] expand(input logic signed [15:0] s);
        return {s[15], s[13:0], s[14]};
    endfunction

    function automatic logic signed [31:0] div25(input logic signed [31:0] x);
        logic [31:0] a;
        logic [63:0] p;
        a = x[31] ? 32'(-x) : 32'(x);
        p = 64'(a) * 64'd1342178;
        return x[31] ? -32'(p >> 25) : 32'(p >> 25);
    endfunction

    function automatic logic signed [31:0] div255(input logic signed [31:0] x);
        logic [31:0] a;
        logic [63:0] p;
        a = x[31] ? 32'(-x) : 32'(x);
        p = 64'(a) * 64'd33686019;
        return x[31] ? -32'(p >> 33) : 32'(p >> 33);
    endfunction

    function automatic logic signed [15:0] clamp16(input logic signed [31:0] v);
        if (v > 32'sd32767) return 16'sd32767;
        if (v < -32'sd32768) return -16'sd32768;
        return v[15:0];
    endfunction

    // one channel at a time: stage 0 sum, 1 scale, 2 divide
    logic [2:0] st;
    logic       ch;                         // 0 left, 1 right
    logic signed [31:0] s, v;
    logic signed [15:0] a_lo, a_hi, rv;
    logic [7:0] vol;
    logic signed [15:0] l_q;

    always_ff @(posedge clk) begin
        valid <= 1'b0;
        if (rst) begin
            st <= 3'd0;
            out_l <= 16'sd0;
            out_r <= 16'sd0;
        end else begin
            case (st)
                3'd0: if (go) begin
                    a_lo <= acc2; a_hi <= acc3; rv <= rv1;
                    vol <= 8'd255 - pwm;
                    ch <= 1'b0;
                    st <= 3'd1;
                end
                3'd1: begin
                    s <= 32'(expand(a_lo)) + 32'(expand(a_hi)) + div25(32'(rv) * 32'sd17);
                    st <= 3'd2;
                end
                3'd2: begin
                    v <= s * 32'(vol);
                    st <= 3'd3;
                end
                3'd3: begin
                    if (!ch) begin
                        l_q <= clamp16(div255(v));
                        a_lo <= acc6; a_hi <= acc7; rv <= rv0;
                        ch <= 1'b1;
                        st <= 3'd1;
                    end else begin
                        out_l <= l_q;
                        out_r <= clamp16(div255(v));
                        valid <= 1'b1;
                        st <= 3'd0;
                    end
                end
                default: st <= 3'd0;
            endcase
        end
    end
endmodule
