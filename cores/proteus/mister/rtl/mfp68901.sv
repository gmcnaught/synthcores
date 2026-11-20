//============================================================================
//  MC68901 MFP subset, modelled on MAME's mc68901_device (the reference the
//  Proteus firmware was verified against):
//   - interrupt controller: IER/IPR/ISR/IMR, VR, IRQ = |(IPR & IMR) (as MAME,
//     no in-service priority masking), vector = VR[7:4] | highest channel
//   - timers A/B: stopped / delay / pulse-width (counts while TxI == AER bit)
//     timers C/D: stopped / delay
//     on a prescaled tick: counter == 1 -> toggle TxO, interrupt (if IER),
//     reload from data register; otherwise decrement (0 counts as 256)
//   - GPIP inputs with AER edge interrupts
//   - USART: asynchronous 8 data bits, no parity, x16 clock (UCR 0x88)
//  Registers are addressed by index 0..23 (A5..A1 of the 68000 bus).
//  `wr` and `rd` are single-cycle strobes, one per bus access.
//============================================================================

module mfp68901
(
	input            clk,
	input            reset,

	input            cs,
	input            wr,
	input            rd,
	input      [4:0] rs,
	input      [7:0] din,
	output reg [7:0] dout,

	input            iack,          // single-cycle strobe: interrupt acknowledge
	output reg [7:0] vector,        // valid from the iack strobe on
	output           irq,

	input            ce_timer,      // timer clock enable (2.5 MHz in the Proteus)
	input            ce_ser,        // USART clock enable (16x baud)

	input      [7:0] gpio_in,
	input            tai,
	input            tbi,
	output reg [3:0] tout,          // TAO, TBO, TCO, TDO

	input            si,
	output reg       so
);

localparam R_GPIP = 5'd0,  R_AER = 5'd1,  R_DDR = 5'd2,  R_IERA = 5'd3, R_IERB = 5'd4,
           R_IPRA = 5'd5,  R_IPRB = 5'd6, R_ISRA = 5'd7, R_ISRB = 5'd8, R_IMRA = 5'd9,
           R_IMRB = 5'd10, R_VR = 5'd11,  R_TACR = 5'd12, R_TBCR = 5'd13, R_TCDCR = 5'd14,
           R_TADR = 5'd15, R_TBDR = 5'd16, R_TCDR = 5'd17, R_TDDR = 5'd18, R_SCR = 5'd19,
           R_UCR = 5'd20,  R_RSR = 5'd21, R_TSR = 5'd22, R_UDR = 5'd23;

// interrupt channel bits (MAME IR_*)
localparam CH_GPIP0 = 0, CH_GPIP1 = 1, CH_GPIP2 = 2, CH_GPIP3 = 3, CH_TD = 4, CH_TC = 5,
           CH_GPIP4 = 6, CH_GPIP5 = 7, CH_TB = 8, CH_TXERR = 9, CH_TXEMPTY = 10,
           CH_RXERR = 11, CH_RXFULL = 12, CH_TA = 13, CH_GPIP6 = 14, CH_GPIP7 = 15;

reg  [7:0] gpip, aer, ddr, vr, scr, ucr, rsr, tsr;
reg [15:0] ier, ipr, isr, imr;
reg  [7:0] tacr, tbcr, tcdcr;
reg  [7:0] tdr [0:3];
reg  [7:0] tmc [0:3];
reg  [7:0] gpio_last;
reg  [1:0] ti_last;

assign irq = |(ipr & imr);

// prescalers
function [7:0] prescale(input [2:0] m);
	case (m)
		3'd1: prescale = 8'd4;   3'd2: prescale = 8'd10;  3'd3: prescale = 8'd16;
		3'd4: prescale = 8'd50;  3'd5: prescale = 8'd64;  3'd6: prescale = 8'd100;
		3'd7: prescale = 8'd200; default: prescale = 8'd0;
	endcase
endfunction

wire [3:0] tmode [0:3];
assign tmode[0] = tacr[3:0];
assign tmode[1] = tbcr[3:0];
assign tmode[2] = {1'b0, tcdcr[6:4]};
assign tmode[3] = {1'b0, tcdcr[2:0]};

reg  [7:0] pcount [0:3];

// timer runs: delay mode, or pulse mode while the input is at the AER level
wire ta_act = (tacr[3:0] >= 4'd1 && tacr[3:0] <= 4'd7) || (tacr[3] && tacr[2:0] != 3'd0 && tai == aer[4]);
wire tb_act = (tbcr[3:0] >= 4'd1 && tbcr[3:0] <= 4'd7) || (tbcr[3] && tbcr[2:0] != 3'd0 && tbi == aer[3]);
wire [3:0] t_active = {tcdcr[2:0] != 3'd0, tcdcr[6:4] != 3'd0, tb_act, ta_act};
wire [3:0] t_enabled = {tcdcr[2:0] != 3'd0, tcdcr[6:4] != 3'd0,
                        tbcr[3:0] != 4'd0 && tbcr[3:0] != 4'd8, tacr[3:0] != 4'd0 && tacr[3:0] != 4'd8};
localparam [15:0] TMASK0 = 16'h2000, TMASK1 = 16'h0100, TMASK2 = 16'h0020, TMASK3 = 16'h0010;

// USART
reg  [3:0] rx_ph;
reg  [3:0] rx_bit;
reg  [8:0] rx_shift;
reg        rx_busy;
reg  [7:0] rx_data;
reg        si_s, si_ss;
reg  [3:0] tx_ph;
reg  [3:0] tx_bit;
reg  [9:0] tx_shift;
reg        tx_busy;
reg  [7:0] tx_buf;

// highest pending & unmasked channel
reg  [3:0] hi_ch;
reg        hi_any;
integer k;
always @* begin
	hi_any = 0; hi_ch = 0;
	for (k = 0; k < 16; k = k + 1)
		if (ipr[k] & imr[k]) begin hi_ch = k[3:0]; hi_any = 1; end
end

integer n;
always @(posedge clk) begin
	si_s  <= si;
	si_ss <= si_s;

	if (reset) begin
		gpip <= 0; aer <= 0; ddr <= 0; vr <= 0; scr <= 0; ucr <= 0; rsr <= 0; tsr <= 8'h80;
		ier <= 0; ipr <= 0; isr <= 0; imr <= 0;
		tacr <= 0; tbcr <= 0; tcdcr <= 0;
		for (n = 0; n < 4; n = n + 1) begin tdr[n] <= 0; tmc[n] <= 0; pcount[n] <= 0; end
		tout <= 0;
		gpio_last <= 0;
		ti_last <= 0;
		rx_busy <= 0; tx_busy <= 0; so <= 1;
		rx_ph <= 0; tx_ph <= 0;
	end
	else begin
		// ---------------- timers
		if (ce_timer) begin
			for (n = 0; n < 4; n = n + 1) begin
				if (t_active[n]) begin
					if (pcount[n] >= prescale(tmode[n][2:0]) - 8'd1) begin
						pcount[n] <= 0;
						if (tmc[n] == 8'd1) begin
							tout[n] <= ~tout[n];
							tmc[n]  <= tdr[n];
							case (n)
								0: if (ier[CH_TA]) ipr[CH_TA] <= 1;
								1: if (ier[CH_TB]) ipr[CH_TB] <= 1;
								2: if (ier[CH_TC]) ipr[CH_TC] <= 1;
								3: if (ier[CH_TD]) ipr[CH_TD] <= 1;
							endcase
						end
						else tmc[n] <= tmc[n] - 8'd1;
					end
					else pcount[n] <= pcount[n] + 8'd1;
				end
			end
		end

		// ---------------- GPIP edge interrupts (edge in the AER direction)
		gpio_last <= gpio_in;
		for (n = 0; n < 8; n = n + 1)
			if (gpio_in[n] != gpio_last[n] && gpio_in[n] == aer[n]) begin
				case (n)
					0: if (ier[CH_GPIP0]) ipr[CH_GPIP0] <= 1;
					1: if (ier[CH_GPIP1]) ipr[CH_GPIP1] <= 1;
					2: if (ier[CH_GPIP2]) ipr[CH_GPIP2] <= 1;
					3: if (ier[CH_GPIP3] && !tbcr[3]) ipr[CH_GPIP3] <= 1;
					4: if (ier[CH_GPIP4] && !tacr[3]) ipr[CH_GPIP4] <= 1;
					5: if (ier[CH_GPIP5]) ipr[CH_GPIP5] <= 1;
					6: if (ier[CH_GPIP6]) ipr[CH_GPIP6] <= 1;
					7: if (ier[CH_GPIP7]) ipr[CH_GPIP7] <= 1;
				endcase
			end
		// pulse mode: interrupt at the end of the pulse (MAME timer_input)
		ti_last <= {tbi, tai};
		if (tacr[3] && tacr[2:0] != 0 && ti_last[0] == aer[4] && tai != aer[4] && ier[CH_GPIP4]) ipr[CH_GPIP4] <= 1;
		if (tbcr[3] && tbcr[2:0] != 0 && ti_last[1] == aer[3] && tbi != aer[3] && ier[CH_GPIP3]) ipr[CH_GPIP3] <= 1;

		// ---------------- USART receiver (8N1, x16)
		if (ce_ser && rsr[0]) begin
			if (!rx_busy) begin
				if (!si_ss) begin rx_busy <= 1; rx_ph <= 0; rx_bit <= 0; end
			end
			else begin
				rx_ph <= rx_ph + 1'd1;
				if (rx_ph == 4'd7) begin
					if (rx_bit == 4'd0) begin
						if (si_ss) rx_busy <= 0;            // false start bit
						rx_bit <= 1;
					end
					else if (rx_bit <= 4'd8) begin
						rx_shift <= {si_ss, rx_shift[8:1]};
						rx_bit   <= rx_bit + 1'd1;
					end
					else begin
						// stop bit
						rx_busy <= 0;
						if (rsr[7]) rsr[6] <= 1;             // overrun
						else begin
							rx_data <= rx_shift[8:1];
							rsr[7]  <= 1;                     // buffer full
							if (ier[CH_RXFULL]) ipr[CH_RXFULL] <= 1;
						end
					end
				end
			end
		end

		// ---------------- USART transmitter
		if (ce_ser) begin
			if (tx_busy) begin
				tx_ph <= tx_ph + 1'd1;
				if (tx_ph == 4'd15) begin
					so       <= tx_shift[0];
					tx_shift <= {1'b1, tx_shift[9:1]};
					tx_bit   <= tx_bit + 1'd1;
					if (tx_bit == 4'd9) tx_busy <= 0;
				end
			end
			else if (tsr[0] && !tsr[7]) begin
				// buffer -> shift register: buffer empty interrupt (MAME tx_clock)
				tsr[7]   <= 1;
				if (ier[CH_TXEMPTY]) ipr[CH_TXEMPTY] <= 1;
				tx_shift <= {1'b1, tx_buf, 1'b0};
				tx_busy  <= 1;
				tx_ph    <= 0;
				tx_bit   <= 0;
			end
		end

		// ---------------- interrupt acknowledge
		if (iack) begin
			vector <= hi_any ? {vr[7:4], hi_ch} : 8'h0F;  // 0x0F: uninitialized vector
			if (hi_any) begin
				if (vr[3]) isr[hi_ch] <= 1;
				ipr[hi_ch] <= 0;
			end
		end

		// ---------------- register access
		if (cs && rd) begin
			case (rs)
				R_RSR: rsr[6] <= 0;
				R_UDR: rsr[7] <= 0;
				default: ;
			endcase
		end
		if (cs && wr) begin
			case (rs)
				R_GPIP:  gpip <= din;
				R_AER:   aer <= din;
				R_DDR:   ddr <= din;
				R_IERA:  begin ier[15:8] <= din; ipr[15:8] <= ipr[15:8] & din; end
				R_IERB:  begin ier[7:0]  <= din; ipr[7:0]  <= ipr[7:0]  & din; end
				R_IPRA:  ipr[15:8] <= ipr[15:8] & din;
				R_IPRB:  ipr[7:0]  <= ipr[7:0]  & din;
				R_ISRA:  isr[15:8] <= isr[15:8] & din;
				R_ISRB:  isr[7:0]  <= isr[7:0]  & din;
				R_IMRA:  begin imr[15:8] <= din; isr[15:8] <= isr[15:8] & din; end
				R_IMRB:  begin imr[7:0]  <= din; isr[7:0]  <= isr[7:0]  & din; end
				R_VR:    vr <= din;
				R_TACR:  begin tacr  <= {3'b000, din[4:0]}; pcount[0] <= 0; if (din[4]) tout[0] <= 0; end
				R_TBCR:  begin tbcr  <= {3'b000, din[4:0]}; pcount[1] <= 0; if (din[4]) tout[1] <= 0; end
				R_TCDCR: begin
					tcdcr <= din & 8'h77;
					if (din[6:4] != tcdcr[6:4]) pcount[2] <= 0;
					if (din[2:0] != tcdcr[2:0]) pcount[3] <= 0;
				end
				R_TADR:  begin tdr[0] <= din; if (!t_enabled[0]) tmc[0] <= din; end
				R_TBDR:  begin tdr[1] <= din; if (!t_enabled[1]) tmc[1] <= din; end
				R_TCDR:  begin tdr[2] <= din; if (!t_enabled[2]) tmc[2] <= din; end
				R_TDDR:  begin tdr[3] <= din; if (!t_enabled[3]) tmc[3] <= din; end
				R_SCR:   scr <= din;
				R_UCR:   ucr <= din;
				R_RSR:   rsr <= din[0] ? {rsr[7:2], din[1], 1'b1} : 8'h00;
				R_TSR:   tsr <= {tsr[7], din[0] & tsr[6], din[5], din[0] ? 1'b0 : tsr[4], din[3:0]};  // keep BE; UE cleared when TE=0; END cleared when TE=1
				R_UDR:   begin tx_buf <= din; tsr[7] <= 0; end
				default: ;
			endcase
		end
	end
end

// register read data
always @* begin
	case (rs)
		R_GPIP:  dout = (gpio_in & ~ddr) | (gpip & ddr);
		R_AER:   dout = aer;
		R_DDR:   dout = ddr;
		R_IERA:  dout = ier[15:8];
		R_IERB:  dout = ier[7:0];
		R_IPRA:  dout = ipr[15:8];
		R_IPRB:  dout = ipr[7:0];
		R_ISRA:  dout = isr[15:8];
		R_ISRB:  dout = isr[7:0];
		R_IMRA:  dout = imr[15:8];
		R_IMRB:  dout = imr[7:0];
		R_VR:    dout = vr;
		R_TACR:  dout = tacr;
		R_TBCR:  dout = tbcr;
		R_TCDCR: dout = tcdcr;
		R_TADR:  dout = tmc[0];
		R_TBDR:  dout = tmc[1];
		R_TCDR:  dout = tmc[2];
		R_TDDR:  dout = tmc[3];
		R_SCR:   dout = scr;
		R_UCR:   dout = ucr;
		R_RSR:   dout = rsr;
		R_TSR:   dout = tsr;
		R_UDR:   dout = rx_data;
		default: dout = 8'hFF;
	endcase
end

endmodule
