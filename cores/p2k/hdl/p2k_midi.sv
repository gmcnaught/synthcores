// SPDX-License-Identifier: GPL-3.0-or-later
// MIDI in for the Proteus 2000: the MiSTer UART line (MidiLink) is received by synthframe_midi_rx,
// which takes 31250 or 9600 baud (some MiSTer kernels leave ttyS1 at 9600), and every byte is sent
// again at 31250 baud, 8N1, to the CPU's UART1 receive pin, as the OS programs it (UBG 33 at 33 MHz).
// A 16-byte FIFO covers the rate difference (bytes never arrive faster than 31250 baud).
module p2k_midi #(
    parameter longint CLK_HZ = 88_200_000
) (
    input  logic clk,
    input  logic rst,
    input  logic uart_rxd,
    output logic cpu_rxd,
    output logic activity          // a byte arrived (one clock)
);
    logic       bv;
    logic [7:0] bd;
    synthframe_midi_rx #(.CLK_HZ(CLK_HZ)) rx (
        .clk, .rst, .rxd(uart_rxd), .byte_valid(bv), .byte_data(bd), .frame_err(), .alt());
    assign activity = bv;

    logic [7:0] q [0:15];
    logic [3:0] wp, rp;
    logic [4:0] n;
    logic [31:0] acc;               // bit timing: CLK_HZ / 31250 clocks a bit, fractional
    logic [3:0]  bit_n;             // 0 idle, 1 start, 2-9 data, 10 stop
    logic [7:0]  sh;
    wire         bit_end = acc + 32'd31_250 >= 32'(CLK_HZ);
    always_ff @(posedge clk) begin
        if (rst) begin
            wp <= '0; rp <= '0; n <= '0; bit_n <= '0; acc <= '0; cpu_rxd <= 1'b1;
        end else begin
            if (bv && n != 5'd16) begin
                q[wp] <= bd;
                wp <= wp + 4'd1;
            end
            n <= n + 5'(bv && n != 5'd16) - 5'(bit_n == 4'd0 && n != 0);
            if (bit_n == 4'd0) begin
                cpu_rxd <= 1'b1;
                if (n != 0) begin
                    sh <= q[rp];
                    rp <= rp + 4'd1;
                    bit_n <= 4'd1;
                    acc <= '0;
                    cpu_rxd <= 1'b0;                     // start bit
                end
            end else begin
                acc <= bit_end ? acc + 32'd31_250 - 32'(CLK_HZ) : acc + 32'd31_250;
                if (bit_end) begin
                    if (bit_n == 4'd10) begin
                        bit_n <= 4'd0;                   // stop bit done
                    end else begin
                        bit_n <= bit_n + 4'd1;
                        if (bit_n == 4'd9) cpu_rxd <= 1'b1;   // stop bit
                        else begin
                            cpu_rxd <= sh[0];
                            sh <= {1'b0, sh[7:1]};
                        end
                    end
                end
            end
        end
    end
endmodule
