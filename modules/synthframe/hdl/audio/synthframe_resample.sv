// SPDX-License-Identifier: MIT
// synthframe: rational sample-rate converter, L/M polyphase FIR (stereo).
//
// Coefficients from tools/gen_resample.py (Q1.17, 18-bit, index p + L j, one word a line of
// COEF_HEX). Integer arithmetic as that script's model(): after input n, while q < L an output
// of phase p = q is made, y = clamp((sum_j h[p + L j] x[n - j] + 2**16) >>> 17), q += M; then
// q -= L. History before the first input (and after reset) reads 0.
//
// An output takes TAPS + 3 clocks, both channels at once; inputs wait in an 8-entry queue
// (dropped when it is full). Outputs come in bursts after each input; a synthframe_audio_fifo
// at the output rate plays them evenly.
module synthframe_resample #(
    parameter int L = 12,
    parameter int M = 5,
    parameter int TAPS = 80,
    parameter COEF_HEX = "synthframe_resample_40k_96k.hex"
) (
    input  logic        clk,
    input  logic        rst,
    input  logic        in_valid,
    input  logic signed [15:0] in_l,
    input  logic signed [15:0] in_r,
    output logic        out_valid,
    output logic signed [15:0] out_l,
    output logic signed [15:0] out_r
);
    localparam int CW = $clog2(L * TAPS);
    localparam int HW = $clog2(TAPS);
    localparam int QW = $clog2(L + M + 1);

    // coefficients
    logic [17:0] coef [L * TAPS];
    initial $readmemh(COEF_HEX, coef);
    logic [CW-1:0] caddr;
    logic signed [17:0] c_q;
    always_ff @(posedge clk) c_q <= coef[caddr];

    // history, newest at wp - 1
    logic [15:0] hl [1 << HW], hr [1 << HW];
    logic [HW-1:0] haddr, hwa;
    logic          hwe;
    logic [15:0]   hwl, hwr;
    logic signed [15:0] xl_q, xr_q;
    always_ff @(posedge clk) begin
        if (hwe) begin hl[hwa] <= hwl; hr[hwa] <= hwr; end
        xl_q <= hl[haddr];
        xr_q <= hr[haddr];
    end

    // input queue
    logic [31:0] iq [8];
    logic [2:0]  iq_wp, iq_rp;
    logic [3:0]  iq_n;
    wire         iq_pop;

    typedef enum logic [2:0] { S_CLEAR, S_IDLE, S_CHECK, S_MAC, S_OUT } st_t;
    st_t st;
    logic [HW-1:0] wp;
    logic [QW-1:0] q;
    logic [HW:0]   j;              // taps issued
    logic [1:0]    vld;            // read issued 1 / 2 clocks ago
    logic signed [41:0] accl, accr;

    assign iq_pop = st == S_IDLE && iq_n != 0;

    function automatic logic signed [15:0] sat(input logic signed [41:0] a);
        logic signed [41:0] y;
        y = (a + 42'sd65536) >>> 17;
        if (y > 42'sd32767) return 16'sh7FFF;
        if (y < -42'sd32768) return 16'sh8000;
        return 16'(y);
    endfunction

    always_ff @(posedge clk) begin
        out_valid <= 1'b0;
        hwe <= 1'b0;
        if (rst) begin
            st <= S_CLEAR;
            wp <= '0; q <= '0;
            iq_wp <= '0; iq_rp <= '0; iq_n <= '0;
        end else begin
            if (in_valid && iq_n != 4'd8) begin
                iq[iq_wp] <= {in_l, in_r};
                iq_wp <= iq_wp + 3'd1;
            end
            iq_n <= iq_n + 4'((in_valid && iq_n != 4'd8)) - 4'(iq_pop);
            if (iq_pop) iq_rp <= iq_rp + 3'd1;

            case (st)
                S_CLEAR: begin
                    hwe <= 1'b1; hwa <= wp; hwl <= '0; hwr <= '0;
                    wp <= wp + 1'd1;
                    if (wp == HW'((1 << HW) - 1)) st <= S_IDLE;
                end
                S_IDLE: if (iq_n != 0) begin
                    hwe <= 1'b1; hwa <= wp;
                    {hwl, hwr} <= iq[iq_rp];
                    wp <= wp + 1'd1;
                    st <= S_CHECK;
                end
                S_CHECK: begin
                    if (q < QW'(L)) begin
                        caddr <= CW'(q);
                        haddr <= wp - 1'd1;
                        j <= '0;
                        vld <= 2'b00;
                        accl <= '0; accr <= '0;
                        st <= S_MAC;
                    end else begin
                        q <= q - QW'(L);
                        st <= S_IDLE;
                    end
                end
                S_MAC: begin
                    // issue tap j (address registered now, data next clock), accumulate tap j - 2
                    vld <= {vld[0], j < (HW + 1)'(TAPS)};
                    if (j < (HW + 1)'(TAPS)) begin
                        j <= j + 1'd1;
                        if (j != '0) begin
                            caddr <= caddr + CW'(L);
                            haddr <= haddr - 1'd1;
                        end
                    end
                    if (vld[1]) begin
                        accl <= accl + 42'(c_q * xl_q);
                        accr <= accr + 42'(c_q * xr_q);
                    end
                    if (j == (HW + 1)'(TAPS) && vld == 2'b10) st <= S_OUT;
                end
                default: begin   // S_OUT
                    out_l <= sat(accl);
                    out_r <= sat(accr);
                    out_valid <= 1'b1;
                    q <= q + QW'(M);
                    st <= S_CHECK;
                end
            endcase
        end
    end

endmodule
