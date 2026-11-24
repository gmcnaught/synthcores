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

    function automatic logic signed [15:0] clamp16(input logic signed [31:0] v);
        if (v > 32'sd32767) return 16'sd32767;
        if (v < -32'sd32768) return -16'sd32768;
        return v[15:0];
    endfunction

    // one channel at a time, one multiply a stage:
    // 1 |rv * 17|, 2 * 1342178, 3 sum s, 4 s * vol, 5 |v|, 6 * 33686019, 7 sign, clamp
    logic [3:0] st;
    logic       ch;                         // 0 left, 1 right
    logic signed [15:0] a_lo, a_hi, rv, l_q;
    logic [7:0] vol;
    logic       neg;
    logic [31:0] ax;
    logic [63:0] px;
    logic signed [31:0] s, v;

    always_ff @(posedge clk) begin
        logic signed [31:0] q, x17;        // blocking temporaries
        valid <= 1'b0;
        if (rst) begin
            st <= 4'd0;
            out_l <= 16'sd0;
            out_r <= 16'sd0;
        end else begin
            case (st)
                4'd0: if (go) begin
                    a_lo <= acc2; a_hi <= acc3; rv <= rv1;
                    vol <= 8'd255 - pwm;
                    ch <= 1'b0;
                    st <= 4'd1;
                end
                4'd1: begin
                    x17 = 32'(rv) * 32'sd17;
                    neg <= x17[31];
                    ax <= x17[31] ? 32'(-x17) : 32'(x17);
                    st <= 4'd2;
                end
                4'd2: begin
                    px <= 64'(ax) * 64'd1342178;
                    st <= 4'd3;
                end
                4'd3: begin
                    q = 32'(px >> 25);
                    if (neg) q = -q;
                    s <= 32'(expand(a_lo)) + 32'(expand(a_hi)) + q;
                    st <= 4'd4;
                end
                4'd4: begin
                    v <= s * 32'(vol);
                    st <= 4'd5;
                end
                4'd5: begin
                    neg <= v[31];
                    ax <= v[31] ? 32'(-v) : 32'(v);
                    st <= 4'd6;
                end
                4'd6: begin
                    px <= 64'(ax) * 64'd33686019;
                    st <= 4'd7;
                end
                4'd7: begin
                    q = 32'(px >> 33);
                    if (neg) q = -q;
                    if (!ch) begin
                        l_q <= clamp16(q);
                        a_lo <= acc6; a_hi <= acc7; rv <= rv0;
                        ch <= 1'b1;
                        st <= 4'd1;
                    end else begin
                        out_l <= l_q;
                        out_r <= clamp16(q);
                        valid <= 1'b1;
                        st <= 4'd0;
                    end
                end
                default: st <= 4'd0;
            endcase
        end
    end
endmodule
