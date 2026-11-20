// 64 kHz -> 48 kHz (3 outputs per 4 inputs), polyphase FIR from
// tools/gen_resampler.py (192 taps, 64 per phase, Q1.17), integer arithmetic
// as gen_resampler.py model(): output after input n (n mod 4 = 0, 1, 2) with
// phase n mod 4, y = clamp((sum_j h[3j + p] * x[n - j]) >>> 17). Inputs come
// in bursts (the PCM's two samples per frame) and wait in a small queue; each
// output takes 64 clocks, both channels at once.
module resample (
    input  logic        clk,
    input  logic        rst,
    input  logic        in_valid,
    input  logic signed [15:0] in_l,
    input  logic signed [15:0] in_r,
    output logic        out_valid,
    output logic signed [15:0] out_l,
    output logic signed [15:0] out_r
);
    function automatic logic signed [17:0] coef(input logic [7:0] idx);
        `include "resample_coef.svh"
    endfunction

    // input queue
    logic [31:0] q [4];
    logic [2:0]  q_wp, q_rp;
    // history (newest at wp - 1) in LUT RAM; entries not yet written read as 0
    logic signed [15:0] hl [64], hr [64];
    logic [5:0]  wp;
    logic [6:0]  filled;
    logic [1:0]  n4;               // input index mod 4
    // MAC
    logic        busy;
    logic [5:0]  j, base;
    logic [1:0]  ph;
    logic signed [39:0] al, ar;

    function automatic logic signed [15:0] clamp(input logic signed [39:0] a);
        logic signed [39:0] s;
        s = a >>> 17;
        if (s > 40'sd32767) return 16'sh7fff;
        if (s < -40'sd32768) return -16'sh8000;
        return s[15:0];
    endfunction

    always_ff @(posedge clk) begin
        out_valid <= 1'b0;
        if (rst) begin
            q_wp <= '0; q_rp <= '0; wp <= '0; n4 <= '0; busy <= 1'b0; filled <= '0;
        end else begin
            if (in_valid && (q_wp - q_rp) != 3'd4) begin
                q[q_wp[1:0]] <= {in_l, in_r};
                q_wp <= q_wp + 3'd1;
            end
            if (!busy) begin
                if (q_wp != q_rp) begin
                    hl[wp] <= q[q_rp[1:0]][31:16];
                    hr[wp] <= q[q_rp[1:0]][15:0];
                    q_rp <= q_rp + 3'd1;
                    base <= wp;
                    wp <= wp + 6'd1;
                    if (filled != 7'd64) filled <= filled + 7'd1;
                    n4 <= n4 + 2'd1;
                    if (n4 != 2'd3) begin
                        busy <= 1'b1;
                        ph <= n4;
                        j <= 6'd0;
                        al <= 40'sd0;
                        ar <= 40'sd0;
                    end
                end
            end else begin : b_resample_1
                logic signed [17:0] c;
                c = coef(8'd3 * {2'b0, j} + {6'b0, ph});
                if ({1'b0, j} < filled) begin
                    al <= al + 40'(c * hl[base - j]);
                    ar <= ar + 40'(c * hr[base - j]);
                end
                j <= j + 6'd1;
                if (j == 6'd63) busy <= 1'b0;
            end
            if (busy && j == 6'd63) begin : b_resample_2
                // the last product is added this clock: finish from the sums including it
                logic signed [17:0] c;
                c = coef(8'd3 * 8'd63 + {6'b0, ph});
                out_l <= clamp(al + ((filled == 7'd64) ? 40'(c * hl[base - 6'd63]) : 40'sd0));
                out_r <= clamp(ar + ((filled == 7'd64) ? 40'(c * hr[base - 6'd63]) : 40'sd0));
                out_valid <= 1'b1;
            end
        end
    end
endmodule
