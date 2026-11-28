// SPDX-License-Identifier: GPL-3.0-or-later
//============================================================================
//  MIDI in for the Proteus/1: the MiSTer UART line (MidiLink) is received by
//  synthframe_midi_rx (31250 or 9600 baud), queued, and sent again at 31250 baud,
//  8N1, to the MFP's serial input. A byte is only started while the MFP's receive
//  buffer is empty (`hold` = RSR buffer full), so no byte is ever lost to an MFP
//  overrun: the MC68901 holds one character, and a second one that completes
//  before the firmware reads UDR is dropped. That happened while nv_ctrl froze the
//  CPU to save the RAM (OSD open with autosave) and lost note-offs (stuck notes).
//  The queue holds 512 bytes (one M10K), 164 ms of a full-rate MIDI stream; a byte
//  that arrives with the queue full is dropped and counted in `dropped`.
//  Latency: one byte time (320 us) more than the line, from re-sending.
//============================================================================

module midi_in #(
	parameter longint CLK_HZ = 80_000_000
)
(
	input             clk,
	input             reset,
	input             uart_rxd,
	input             hold,           // MFP receive buffer full: do not start a byte
	output reg        mfp_rxd = 1'b1,
	output reg [15:0] dropped         // bytes lost to a full queue (saturates)
);

localparam int DIV = int'(CLK_HZ / 31_250);

wire       bv;
wire [7:0] bd;
synthframe_midi_rx #(.CLK_HZ(CLK_HZ)) rx
(
	.clk(clk), .rst(reset), .rxd(uart_rxd), .byte_valid(bv), .byte_data(bd), .frame_err(), .alt()
);

reg  [7:0] q [0:511];
reg  [9:0] wp, rp;
wire       empty = (wp == rp);
wire       full  = (wp[8:0] == rp[8:0]) && (wp[9] != rp[9]);

reg  [7:0]  q_out;
reg  [11:0] cnt;                      // clocks left in the current bit
reg  [3:0]  bit_n;                    // 0 idle, 1 start, 2-9 data, 10 stop
reg  [7:0]  sh;
reg         rd_pend;                  // q_out valid next clock

always @(posedge clk) begin
	q_out <= q[rp[8:0]];
	if (bv && !full) q[wp[8:0]] <= bd;
end

always @(posedge clk) begin
	if (reset) begin
		wp <= 0; rp <= 0; bit_n <= 0; cnt <= 0; mfp_rxd <= 1'b1; rd_pend <= 0; dropped <= 0;
	end
	else begin
		if (bv) begin
			if (!full) wp <= wp + 1'd1;
			else if (dropped != 16'hFFFF) dropped <= dropped + 1'd1;
		end

		if (bit_n == 0) begin
			mfp_rxd <= 1'b1;
			if (rd_pend) begin
				// start bit
				rd_pend <= 0;
				sh      <= q_out;
				rp      <= rp + 1'd1;
				mfp_rxd <= 1'b0;
				bit_n   <= 4'd1;
				cnt     <= 12'(DIV - 1);
			end
			else if (!empty && !hold) rd_pend <= 1;
		end
		else if (cnt != 0) cnt <= cnt - 1'd1;
		else begin
			cnt <= 12'(DIV - 1);
			if (bit_n == 4'd10) begin
				bit_n   <= 0;                 // stop bit done
				mfp_rxd <= 1'b1;
			end
			else begin
				bit_n   <= bit_n + 1'd1;
				mfp_rxd <= (bit_n <= 4'd8) ? sh[0] : 1'b1;   // data bits LSB first, then stop
				sh      <= {1'b1, sh[7:1]};
			end
		end
	end
end

endmodule
