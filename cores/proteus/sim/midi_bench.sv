// midi_in + mfp68901 as wired in proteus_sys (80 MHz, USART x16 clock 500 kHz), for midi_tb.cpp.
// bypass = 1 connects the line straight to the MFP (the wiring before midi_in).
module midi_bench
(
	input        clk,
	input        reset,
	input        line,
	input        bypass,
	input        rd,
	input  [4:0] rs,
	input        wr,
	input  [7:0] din,
	output [7:0] dout,
	output       rx_full
);

reg [7:0] sdiv = 0;
reg       ce_ser = 0;
always @(posedge clk) begin
	ce_ser <= 0;
	if (sdiv == 8'd159) begin sdiv <= 0; ce_ser <= 1; end
	else sdiv <= sdiv + 1'd1;
end

wire si;
midi_in #(.CLK_HZ(80_000_000)) mi
(
	.clk(clk), .reset(reset), .uart_rxd(line), .hold(rx_full), .mfp_rxd(si), .dropped()
);

mfp68901 mfp
(
	.clk(clk), .reset(reset), .cs(1'b1), .wr(wr), .rd(rd), .rs(rs), .din(din), .dout(dout),
	.iack(1'b0), .vector(), .irq(), .ce_timer(1'b0), .ce_ser(ce_ser),
	.gpio_in(8'hFF), .tai(1'b0), .tbi(1'b0), .tout(),
	.si(bypass ? line : si), .so(), .rx_full(rx_full)
);

endmodule
