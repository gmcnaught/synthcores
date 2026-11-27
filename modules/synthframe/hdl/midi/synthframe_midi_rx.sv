// SPDX-License-Identifier: MIT
// synthframe: MIDI bytes from MiSTer's UART (MidiLink).
//
// Two receivers, 31250 and ALT_BAUD (9600): on some MiSTer kernels ttyS1
// falls back to 9600 after MidiLink sets 31250, so the line can run at
// either. The 31250 receiver is used until it sees framing errors while the
// other decodes cleanly; it switches back the same way, or after 7 clean
// bytes in a row at 31250. Every received byte (including real-time bytes)
// is passed on; the core parses MIDI itself.
//
// From synclavierii hdl/core/midi_in.sv (receiver and speed choice).
module synthframe_midi_rx #(
    parameter longint CLK_HZ = 50_000_000,
    parameter longint BAUD = 31_250,
    parameter longint ALT_BAUD = 9_600
) (
    input  logic       clk,
    input  logic       rst,
    input  logic       rxd,
    output logic       byte_valid,
    output logic [7:0] byte_data,
    output logic       frame_err,
    output logic       alt
);
    logic       v_std, e_std, v_alt, e_alt;
    logic [7:0] d_std, d_alt;
    synthframe_midi_uart_rx #(.CLK_HZ(CLK_HZ), .BAUD(BAUD)) rx_std (
        .clk, .rst, .rxd, .valid(v_std), .data(d_std), .ferr(e_std));
    synthframe_midi_uart_rx #(.CLK_HZ(CLK_HZ), .BAUD(ALT_BAUD)) rx_alt (
        .clk, .rst, .rxd, .valid(v_alt), .data(d_alt), .ferr(e_alt));

    logic [2:0] bad_cur, good_other;
    wire        v_cur = alt ? v_alt : v_std, e_cur = alt ? e_alt : e_std;
    wire        v_oth = alt ? v_std : v_alt, e_oth = alt ? e_std : e_alt;
    always_ff @(posedge clk) begin
        if (rst) begin
            alt <= 1'b0;
            bad_cur <= '0;
            good_other <= '0;
        end else if ((bad_cur >= 3'd3 && good_other >= 3'd4) || (alt && good_other == 3'd7)) begin
            alt <= !alt;
            bad_cur <= '0;
            good_other <= '0;
        end else begin
            if (e_cur && bad_cur != 3'd7) bad_cur <= bad_cur + 3'd1;
            else if (v_cur) bad_cur <= '0;
            if (e_oth) good_other <= '0;
            else if (v_oth && good_other != 3'd7) good_other <= good_other + 3'd1;
        end
    end
    assign byte_valid = v_cur;
    assign byte_data  = alt ? d_alt : d_std;
    assign frame_err  = e_cur;
endmodule

module synthframe_midi_uart_rx #(
    parameter longint CLK_HZ = 50_000_000,
    parameter longint BAUD = 31_250
) (
    input  logic       clk,
    input  logic       rst,
    input  logic       rxd,
    output logic       valid,
    output logic [7:0] data,
    output logic       ferr
);
    localparam int DIV = int'(CLK_HZ / BAUD);
    localparam int CW = $clog2(DIV + 1);
    logic [2:0]  sync;
    logic        busy;
    logic [CW-1:0] cnt;
    logic [3:0]  bitn;
    logic [7:0]  sh;
    always_ff @(posedge clk) begin
        sync <= {sync[1:0], rxd};
        valid <= 1'b0;
        ferr <= 1'b0;
        if (rst) begin
            busy <= 1'b0;
            sync <= 3'b111;
        end else if (!busy) begin
            if (sync[2:1] == 2'b10) begin           // start bit edge
                busy <= 1'b1;
                cnt <= CW'(DIV / 2);
                bitn <= 4'd0;
            end
        end else if (cnt != '0) begin
            cnt <= cnt - 1'b1;
        end else begin
            cnt <= CW'(DIV - 1);
            if (bitn == 4'd0) begin
                if (sync[2]) busy <= 1'b0;          // not a start bit after all
            end else if (bitn <= 4'd8) begin
                sh <= {sync[2], sh[7:1]};
            end else begin
                busy <= 1'b0;
                if (sync[2]) begin                  // stop bit: the byte is good
                    valid <= 1'b1;
                    data <= sh;
                end else ferr <= 1'b1;
            end
            bitn <= bitn + 4'd1;
        end
    end
endmodule
