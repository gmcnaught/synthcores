// SPDX-License-Identifier: GPL-3.0-or-later
// Signed multiplier split into partial products that each fit a 27x27 DSP block.
// a = ah * 2^LA + al and b = bh * 2^LB + bl (al, bl unsigned); LB = 0 leaves b whole.
// Three register stages: partial products, pair sums, product. The inputs should come from
// registers.
module p2k_mul #(
    parameter int WA = 32,
    parameter int WB = 48,
    parameter int LA = 16,
    parameter int LB = 24
) (
    input  logic                    clk,
    input  logic signed [WA-1:0]    a,
    input  logic signed [WB-1:0]    b,
    output logic signed [WA+WB-1:0] p
);
    localparam int W = WA + WB;
    wire signed [WA-LA-1:0] ah = a[WA-1:LA];
    wire signed [LA:0]      al = $signed({1'b0, a[LA-1:0]});

    generate
        if (LB == 0) begin : g_a
            logic signed [WA-LA+WB-1:0] hb;
            logic signed [LA+WB:0]      lb;
            logic signed [W-1:0]        s1, s2;
            always_ff @(posedge clk) begin
                hb <= ah * b;
                lb <= al * b;
                s1 <= W'(hb) <<< LA;
                s2 <= W'(lb);
                p  <= s1 + s2;
            end
        end else begin : g_ab
            wire signed [WB-LB-1:0] bh = b[WB-1:LB];
            wire signed [LB:0]      bl = $signed({1'b0, b[LB-1:0]});
            logic signed [WA-LA+WB-LB-1:0] hh;
            logic signed [WA-LA+LB:0]      hl;
            logic signed [LA+WB-LB:0]      lh;
            logic signed [LA+LB+1:0]       ll;
            logic signed [W-1:0]           s1, s2;
            always_ff @(posedge clk) begin
                hh <= ah * bh;
                hl <= ah * bl;
                lh <= al * bh;
                ll <= al * bl;
                s1 <= (W'(hh) <<< (LA + LB)) + (W'(hl) <<< LA);
                s2 <= (W'(lh) <<< LB) + W'(ll);
                p  <= s1 + s2;
            end
        end
    endgenerate
endmodule
