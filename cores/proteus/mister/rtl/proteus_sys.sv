//============================================================================
//  E-mu Proteus/1 XR system: 68000 (fx68k) at 10 MHz + memory map from the
//  MAME proteusxr driver (see docs/hardware-notes.md):
//    000000-01FFFF  program ROM (128 KB, block RAM, loaded from the image)
//    400001         latch (write, ignored)
//    500000-50004F  per-voice pan byte at 500009 + 2 * voice
//    700000-700003  HD44780 (odd bytes; A1 = RS)
//    800000-8003FF  sound chip
//    900000-90002F  MC68901 MFP (odd bytes; A5..A1 = register)
//    A00000         volume pot strobe (one-shot measured by MFP timer B)
//    B00001         latch (write, ignored)
//    C00001         output jack sense (0 = nothing plugged)
//    D00000-D00003  debug serial 6850 (status: transmit ready, no receive)
//    FF0000-FFFFFF  RAM 64 KB (battery-backed in the XR), loaded from the image
//  Unmapped reads return 0, every access gets DTACK (as MAME).
//  IRQ: MFP on level 1, vectored through an IACK cycle.
//
//  Image (ioctl index 1), byte offsets: 0x800000 ROM (big-endian words),
//  0x820000 RAM (big-endian words), 0x830000 HD44780 font (4 KB). Offsets
//  below 0x800000 (samples) are handled by snd_ctrl.
//============================================================================

module proteus_sys
(
	input             clk,            // 80 MHz
	input             reset,          // holds the CPU (and peripherals) in reset
	input             loading,

	input             ioctl_wr,
	input      [26:0] ioctl_addr,
	input       [7:0] ioctl_dout,

	input             midi_in,
	output            midi_out,
	input       [7:0] buttons,        // MFP GPIP inputs (wiring unknown; 0 = released)
	input       [7:0] volume,         // front-panel volume 0..255

	// sound chip
	output reg        snd_wr,
	output reg        snd_pan,
	output reg  [4:0] snd_voice,
	output reg  [3:0] snd_word,
	output reg [15:0] snd_data,
	output reg [15:0] snd_mask,
	input             snd_wr_full,
	output reg        snd_rb,
	input             snd_rb_done,
	input      [15:0] snd_rb_data,
	input      [15:0] snd_raw,

	// LCD render port
	input             lcd_line,
	input       [3:0] lcd_pos,
	input       [2:0] lcd_row,
	output      [7:0] lcd_code,
	output      [4:0] lcd_cgbits,
	output            lcd_is_cg,
	output            lcd_cursor,
	output            lcd_on,
	input      [11:0] font_addr,
	output      [7:0] font_data,
	input      [11:0] font2_addr,      // second read port (display labels)
	output      [7:0] font2_data,
	output reg  [7:0] leds,           // latch 0xB00001, active low: bit0 MASTER, bit1 EDIT, bit3 MIDI

	// debug
	output     [23:0] dbg_pc_addr,
	output reg        bus_wr_strobe,  // one pulse per completed CPU write (for tracing)
	output reg [23:0] bus_wr_addr,
	output reg [15:0] bus_wr_data,
	output reg  [1:0] bus_wr_ds
);

// ---------------------------------------------------------------- clock enables
reg [2:0] ph;
reg [4:0] tdiv;
reg [7:0] sdiv;
reg       phi1, phi2, ce_timer, ce_ser;
always @(posedge clk) begin
	ph       <= ph + 1'd1;
	phi1     <= (ph == 3'd3);
	phi2     <= (ph == 3'd7);
	ce_timer <= 0;
	ce_ser   <= 0;
	tdiv <= tdiv + 1'd1;
	if (tdiv == 5'd31) ce_timer <= 1;                         // 2.5 MHz
	if (sdiv == 8'd159) begin sdiv <= 0; ce_ser <= 1; end      // 500 kHz = 16 x 31250
	else sdiv <= sdiv + 1'd1;
end

// ---------------------------------------------------------------- CPU
wire        ASn, UDSn, LDSn, eRWn, FC0, FC1, FC2;
wire [23:1] eab;
wire [15:0] oEdb;
reg  [15:0] iEdb;
reg         DTACKn;
wire        mfp_irq;

fx68k cpu
(
	.clk(clk), .HALTn(1'b1), .extReset(reset), .pwrUp(reset),
	.enPhi1(phi1), .enPhi2(phi2),
	.eRWn(eRWn), .ASn(ASn), .LDSn(LDSn), .UDSn(UDSn), .E(), .VMAn(),
	.FC0(FC0), .FC1(FC1), .FC2(FC2), .BGn(), .oRESETn(), .oHALTEDn(),
	.DTACKn(DTACKn), .VPAn(1'b1), .BERRn(1'b1), .BRn(1'b1), .BGACKn(1'b1),
	.IPL0n(~mfp_irq), .IPL1n(1'b1), .IPL2n(1'b1),
	.iEdb(iEdb), .oEdb(oEdb), .eab(eab)
);

wire [23:0] addr = {eab, 1'b0};
reg  bus_busy;          // access in progress (until AS rises)
wire ds_act = !UDSn || !LDSn;
assign dbg_pc_addr = addr;
wire iack_cycle = FC0 & FC1 & FC2;

// ---------------------------------------------------------------- bus decode
wire sel_rom  = (addr[23:17] == 7'd0);
wire sel_ram  = (addr[23:16] == 8'hFF);
wire sel_pan  = (addr[23:8] == 16'h5000) && (addr[7:0] < 8'h50);
wire sel_lcd  = (addr[23:2] == 22'h1C0000);                 // 700000-700003
wire sel_snd  = (addr[23:10] == 14'h2000);                   // 800000-8003FF
wire sel_mfp  = (addr[23:8] == 16'h9000) && (addr[7:0] < 8'h30);
wire sel_vol  = (addr[23:1] == 23'h500000);                  // A00000
wire sel_jack = (addr[23:1] == 23'h600000);                  // C00000/1
wire sel_acia = (addr[23:2] == 22'h340000);                  // D00000-D00003

// ---------------------------------------------------------------- memories
// Byte-wide block RAMs (high and low byte of each 16-bit word). The loader writes
// image bytes directly: even offsets -> high byte, odd offsets -> low byte.
wire ld_rom  = loading && ioctl_wr && (ioctl_addr[26:17] == 10'h040);    // 0x800000-0x81FFFF
wire ld_ram  = loading && ioctl_wr && (ioctl_addr[26:16] == 11'h082);    // 0x820000-0x82FFFF
wire ld_font = loading && ioctl_wr && (ioctl_addr[26:12] == 15'h0830);   // 0x830000-0x830FFF

// CPU writes to RAM happen in the first cycle of a write access (see the bus FSM)
wire cpu_wr_go = !reset && !ASn && !bus_busy && ds_act && !eRWn && !iack_cycle && sel_ram;
wire ram_we_hi = ld_ram ? !ioctl_addr[0] : (cpu_wr_go && !UDSn);
wire ram_we_lo = ld_ram ?  ioctl_addr[0] : (cpu_wr_go && !LDSn);
wire [14:0] ram_wa = ld_ram ? ioctl_addr[15:1] : addr[15:1];

wire [7:0] rom_qh, rom_ql, ram_qh, ram_ql;
wire [15:0] rom_q = {rom_qh, rom_ql};
wire [15:0] ram_q = {ram_qh, ram_ql};

bram #(16, 8) rom_hi (.clk(clk), .we(ld_rom && !ioctl_addr[0]), .waddr(ioctl_addr[16:1]), .wdata(ioctl_dout), .raddr(addr[16:1]), .rdata(rom_qh));
bram #(16, 8) rom_lo (.clk(clk), .we(ld_rom &&  ioctl_addr[0]), .waddr(ioctl_addr[16:1]), .wdata(ioctl_dout), .raddr(addr[16:1]), .rdata(rom_ql));
bram #(15, 8) ram_hi (.clk(clk), .we(ram_we_hi), .waddr(ram_wa), .wdata(ld_ram ? ioctl_dout : oEdb[15:8]), .raddr(addr[15:1]), .rdata(ram_qh));
bram #(15, 8) ram_lo (.clk(clk), .we(ram_we_lo), .waddr(ram_wa), .wdata(ld_ram ? ioctl_dout : oEdb[7:0]),  .raddr(addr[15:1]), .rdata(ram_ql));
bram #(12, 8) fontm  (.clk(clk), .we(ld_font), .waddr(ioctl_addr[11:0]), .wdata(ioctl_dout), .raddr(font_addr), .rdata(font_data));
bram #(12, 8) fontm2 (.clk(clk), .we(ld_font), .waddr(ioctl_addr[11:0]), .wdata(ioctl_dout), .raddr(font2_addr), .rdata(font2_data));


// one access per AS assertion, started when a data strobe is active (bus_busy, ds_act above)

reg  mfp_cs_wr, mfp_cs_rd, mfp_iack, lcd_wr, lcd_rd;
wire [7:0] mfp_dout, mfp_vector, lcd_dout;

// volume one-shot: TBI high for 12..201 timer-B counts (50 kHz each)
reg  [19:0] vol_cnt;
wire        vol_pulse = (vol_cnt != 0);
// counts = 12 + volume * 191 / 256 (12..202), 1600 clocks (50 kHz) per count
wire [15:0] vol_scaled = {8'd0, volume} * 16'd191;   // 255 -> 190, total 202 counts as MAME at 100 %
reg  [19:0] vol_load;
always @(posedge clk) vol_load <= {12'd0, 8'd12 + vol_scaled[15:8]} * 20'd1600;   // up to 321,600 clocks

localparam B_IDLE = 3'd0, B_RAMRD = 3'd1, B_ACK = 3'd2, B_SNDWR = 3'd3, B_SNDRB = 3'd4, B_IACK = 3'd5, B_IACK2 = 3'd6;
reg  [2:0] bstate;

always @(posedge clk) begin
	mfp_cs_wr     <= 0;
	mfp_cs_rd     <= 0;
	mfp_iack      <= 0;
	lcd_wr        <= 0;
	lcd_rd        <= 0;
	snd_wr        <= 0;
	snd_rb        <= 0;
	bus_wr_strobe <= 0;

	if (vol_cnt != 0) vol_cnt <= vol_cnt - 1'd1;


	if (reset) begin
		bus_busy <= 0;
		DTACKn   <= 1;
		bstate   <= B_IDLE;
		vol_cnt  <= 0;
		leds     <= 8'hFF;
	end
	else begin
		if (ASn) begin
			bus_busy <= 0;
			DTACKn   <= 1;
			bstate   <= B_IDLE;
		end
		else if (!bus_busy && (ds_act || iack_cycle)) begin
			bus_busy <= 1;
			if (iack_cycle) begin
				mfp_iack <= 1;
				bstate   <= B_IACK;
			end
			else if (!eRWn) begin
				// ------------------------------ write
				bus_wr_strobe <= 1; bus_wr_addr <= addr; bus_wr_data <= oEdb; bus_wr_ds <= {~UDSn, ~LDSn};
				if (addr[23:1] == 23'h580000 && !LDSn) leds <= oEdb[7:0];   // B00001
				// RAM bytes are written by the block RAMs (cpu_wr_go)
				if (sel_mfp) mfp_cs_wr <= 1;
				if (sel_lcd) lcd_wr <= 1;
				if (sel_snd || sel_pan) begin
					bstate <= B_SNDWR;
				end
				else bstate <= B_ACK;
			end
			else begin
				// ------------------------------ read
				if (sel_mfp) mfp_cs_rd <= 1;
				if (sel_lcd) lcd_rd <= 1;
				if (sel_vol) vol_cnt <= vol_load;
				if (sel_snd && addr[4:1] == 4'hC) begin
					snd_voice <= addr[9:5];
					snd_word  <= addr[4:1];
					snd_rb    <= 1;
					bstate    <= B_SNDRB;
				end
				else bstate <= B_RAMRD;
				snd_voice <= addr[9:5];
				snd_word  <= addr[4:1];
			end
		end
		else case (bstate)
			B_RAMRD: begin
				// block RAM data is valid one clock after the address
				if      (sel_rom)  iEdb <= rom_q;
				else if (sel_ram)  iEdb <= ram_q;
				else if (sel_mfp)  iEdb <= {8'hFF, mfp_dout};
				else if (sel_lcd)  iEdb <= {8'hFF, lcd_dout};
				else if (sel_snd)  iEdb <= snd_raw;
				else if (sel_vol)  iEdb <= 16'hFFFF;
				else if (sel_jack) iEdb <= 16'h0000;
				else if (sel_acia) iEdb <= addr[1] ? 16'h0000 : 16'h0002;
				else               iEdb <= 16'h0000;
				DTACKn <= 0;
				bstate <= B_IDLE;
			end
			B_ACK: begin DTACKn <= 0; bstate <= B_IDLE; end
			B_SNDWR:
				if (!snd_wr_full) begin
					snd_wr    <= 1;
					snd_pan   <= sel_pan;
					snd_voice <= sel_pan ? (addr[6:1] - 6'd4) : addr[9:5];
					snd_word  <= addr[4:1];
					snd_data  <= oEdb;
					snd_mask  <= {{8{~UDSn}}, {8{~LDSn}}};
					DTACKn    <= 0;
					bstate    <= B_IDLE;
				end
			B_SNDRB:
				if (snd_rb_done) begin iEdb <= snd_rb_data; DTACKn <= 0; bstate <= B_IDLE; end
			B_IACK:  bstate <= B_IACK2;   // the MFP latches the vector on the clock after the iack strobe
			B_IACK2: begin iEdb <= {8'h00, mfp_vector}; DTACKn <= 0; bstate <= B_IDLE; end
			default: ;
		endcase
	end
end

// ---------------------------------------------------------------- MFP
mfp68901 mfp
(
	.clk(clk), .reset(reset),
	.cs(1'b1), .wr(mfp_cs_wr), .rd(mfp_cs_rd), .rs(addr[5:1]), .din(oEdb[7:0]), .dout(mfp_dout),
	.iack(mfp_iack), .vector(mfp_vector), .irq(mfp_irq),
	.ce_timer(ce_timer), .ce_ser(ce_ser),
	.gpio_in(buttons), .tai(1'b0), .tbi(vol_pulse), .tout(),
	.si(midi_in), .so(midi_out)
);

// ---------------------------------------------------------------- LCD
hd44780 lcd
(
	.clk(clk), .reset(reset),
	.cs(1'b1), .wr(lcd_wr), .rd(lcd_rd), .rs(addr[1]), .din(oEdb[7:0]), .dout(lcd_dout),
	.r_line(lcd_line), .r_pos(lcd_pos), .r_row(lcd_row), .r_code(lcd_code), .r_cgbits(lcd_cgbits),
	.r_is_cg(lcd_is_cg), .r_cursor(lcd_cursor), .display_on(lcd_on)
);

endmodule
