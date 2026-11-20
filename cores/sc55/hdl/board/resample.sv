// Output rate -> 48 kHz: polyphase FIR from tools/gen_resampler.py, 64 taps a
// phase, Q1.17, integer arithmetic as its model():
//   mk1: 64 kHz -> 48 kHz (L = 3, M = 4), coefficients at 0
//   mk2: 66,207 Hz -> 48 kHz (L = 29, M = 40), coefficients at 256
// q starts at 0; after input n, if q < L an output with phase p = q is made
// (q += M); then q -= L. y = clamp((sum_j h[L j + p] * x[n - j]) >>> 17).
// Inputs come in bursts (the PCM's two samples a frame) and wait in a small
// queue; an output takes 66 clocks, both channels at once. The coefficients
// are in a block RAM (resample_coef.hex), read one clock ahead.
`ifndef RESAMPLE_HEX
    `define RESAMPLE_HEX "../hdl/board/resample_coef.hex"     // Quartus: relative to mister/
`endif
module resample (
    input  logic        clk,
    input  logic        rst,
    input  logic        mk2,
    input  logic        in_valid,
    input  logic signed [15:0] in_l,
    input  logic signed [15:0] in_r,
    output logic        out_valid,
    output logic signed [15:0] out_l,
    output logic signed [15:0] out_r
);
    wire [5:0]  L    = mk2 ? 6'd29 : 6'd3;
    wire [5:0]  M    = mk2 ? 6'd40 : 6'd4;
    wire [11:0] BASE = mk2 ? 12'd256 : 12'd0;

    // coefficients
    logic [11:0] cidx;
    logic [17:0] cq;
    sdp_ram #(.AW(12), .DW(18), .INIT(`RESAMPLE_HEX)) u_coef (
        .clk, .we(1'b0), .wa(12'd0), .wd(18'd0), .ra(cidx), .rd(cq));

    // input queue
    logic [31:0] q [4];
    logic [2:0]  q_wp, q_rp;
    // history (newest at wp - 1) in LUT RAM; entries not yet written read as 0
    logic signed [15:0] hl [64], hr [64];
    logic [5:0]  wp;
    logic [6:0]  filled;
    logic [6:0]  ph;               // the phase accumulator q of the model
    // MAC: issue j (coefficient address, history read) one clock, accumulate the next
    logic        busy, acc_v, last;
    logic [6:0]  j;
    logic [5:0]  base;
    logic signed [15:0] xl, xr;
    logic signed [39:0] al, ar;

    function automatic logic signed [15:0] clamp(input logic signed [39:0] a);
        logic signed [39:0] s;
        s = a >>> 17;
        if (s > 40'sd32767) return 16'sh7fff;
        if (s < -40'sd32768) return -16'sh8000;
        return s[15:0];
    endfunction

    always_ff @(posedge clk) begin : b_resample_1
        logic signed [39:0] sl, sr;
        out_valid <= 1'b0;
        if (rst) begin
            q_wp <= '0; q_rp <= '0; wp <= '0; busy <= 1'b0; filled <= '0; ph <= '0;
            acc_v <= 1'b0; last <= 1'b0;
        end else begin
            if (in_valid && (q_wp - q_rp) != 3'd4) begin
                q[q_wp[1:0]] <= {in_l, in_r};
                q_wp <= q_wp + 3'd1;
            end
            // accumulate the product issued last clock
            sl = al; sr = ar;
            if (acc_v) begin
                sl = al + 40'($signed(cq) * xl);
                sr = ar + 40'($signed(cq) * xr);
            end
            al <= sl; ar <= sr;
            acc_v <= 1'b0;
            if (last) begin
                out_l <= clamp(sl);
                out_r <= clamp(sr);
                out_valid <= 1'b1;
                last <= 1'b0;
            end
            if (!busy) begin
                if (q_wp != q_rp && !last && !acc_v) begin
                    hl[wp] <= q[q_rp[1:0]][31:16];
                    hr[wp] <= q[q_rp[1:0]][15:0];
                    q_rp <= q_rp + 3'd1;
                    base <= wp;
                    wp <= wp + 6'd1;
                    if (filled != 7'd64) filled <= filled + 7'd1;
                    if (ph < {1'b0, L}) begin
                        busy <= 1'b1;
                        j <= 7'd0;
                        cidx <= BASE + {5'd0, ph};
                        al <= 40'sd0;
                        ar <= 40'sd0;
                        ph <= ph + {1'b0, M} - {1'b0, L};
                    end else begin
                        ph <= ph - {1'b0, L};
                    end
                end
            end else begin
                // issue tap j: its coefficient (BRAM, data next clock) and history sample
                xl <= ({1'b0, j[5:0]} < filled) ? hl[base - j[5:0]] : 16'sd0;
                xr <= ({1'b0, j[5:0]} < filled) ? hr[base - j[5:0]] : 16'sd0;
                acc_v <= 1'b1;
                cidx <= cidx + {6'd0, L};
                j <= j + 7'd1;
                if (j == 7'd63) begin
                    busy <= 1'b0;
                    last <= 1'b1;
                end
            end
        end
    end
endmodule
