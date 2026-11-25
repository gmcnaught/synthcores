// SPDX-License-Identifier: GPL-2.0-or-later
//============================================================================
//  Roland MT-32 (second generation) for MiSTer.
//
//  The instrument is hdl/mt32_core.sv (8098 CPU, LA32, reverb chip, LCD, MIDI), checked against
//  Nuked-MT32 cycle for cycle and sample for sample (cores/mt32/docs/plan.md). This file
//  connects it to the MiSTer framework (sys/, Template_MiSTer, GPL-2.0): OSD and ROM download
//  through hps_io, the PCM ROM in DDR3, video, audio (resampled from 32 to 96 kHz), MidiLink.
//
//  Use: make the ROM pack with `cores/mt32/tools/mkpack.py`, copy it to games/MT-32/ and load
//  it once from the OSD (FSC: remembered). Set UART to MidiLink for MIDI in and out (MIDI out:
//  the CPU's serial transmitter, e.g. DT1 replies to RQ1 requests).
//
//  Pack: 00000-1FFFF control ROM (mt32_cpu.bin, firmware 2.0x), 20000-27FFF reverb ROM
//  (mt32_reverb.bin), 28000-A7FFF PCM ROM (mt32_pcm.bin).
//============================================================================

module emu
(
	`include "sys/emu_ports.vh"
);

assign ADC_BUS  = 'Z;
assign USER_OUT = '1;
assign {UART_RTS, UART_DTR} = 0;
assign {SD_SCK, SD_MOSI, SD_CS} = 'Z;
assign {SDRAM_DQ, SDRAM_A, SDRAM_BA, SDRAM_CLK, SDRAM_CKE, SDRAM_DQML, SDRAM_DQMH, SDRAM_nWE, SDRAM_nCAS, SDRAM_nRAS, SDRAM_nCS} = 'Z;

assign VGA_SL = 0;
assign VGA_F1 = 0;
assign VGA_SCALER  = 0;
assign VGA_DISABLE = 0;
assign HDMI_FREEZE = 0;
assign HDMI_BLACKOUT = 0;
assign HDMI_BOB_DEINT = 0;

assign LED_POWER = 0;
assign BUTTONS = 0;

assign VIDEO_ARX = 12'd4;
assign VIDEO_ARY = 12'd3;

`include "build_id.v"
localparam CONF_STR = {
	"MT-32;UART31250,MIDI;",
	"-;",
	"FSC1,ROM,Load ROM pack;",
	"-;",
	"-,Buttons: Q W E R T, A S D F G;",
	"-,Volume knob: - and =;",
	"O[3:2],Output gain,0 dB,+6 dB,+12 dB,+18 dB;",
	"-;",
	"T[0],Reset;",
	"R[0],Reset and close OSD;",
	"V,v",`BUILD_DATE
};

wire   [1:0] buttons;
wire [127:0] status;
wire  [10:0] ps2_key;
wire         ioctl_download, ioctl_wr;
wire  [26:0] ioctl_addr;
wire   [7:0] ioctl_dout;
wire  [15:0] ioctl_index;
reg          ioctl_wait = 0;

hps_io #(.CONF_STR(CONF_STR)) hps_io
(
	.clk_sys(clk_sys),
	.HPS_BUS(HPS_BUS),
	.EXT_BUS(),
	.gamma_bus(),

	.buttons(buttons),
	.status(status),
	.status_menumask(16'd0),
	.ps2_key(ps2_key),

	.ioctl_download(ioctl_download),
	.ioctl_index(ioctl_index),
	.ioctl_wr(ioctl_wr),
	.ioctl_addr(ioctl_addr),
	.ioctl_dout(ioctl_dout),
	.ioctl_wait(ioctl_wait)
);

///////////////////////   CLOCKS   ///////////////////////////////

// rtl/pll: clk_sys 96 MHz (core; CLK_HZ below must match), clk_vid 40 MHz (800 x 600)
localparam longint CLK_HZ = 96_000_000;
wire clk_sys, clk_vid;
pll pll
(
	.refclk(CLK_50M),
	.rst(0),
	.outclk_0(clk_sys),
	.outclk_1(clk_vid)
);

reg rst_hard = 1, rst_user = 1;
always @(posedge clk_sys) begin
	rst_hard <= RESET;
	rst_user <= status[0] | buttons[1];
end

///////////////////////   ROM PACK   /////////////////////////////

localparam [28:0] PCM_BASE = 29'h0600_0000;   // DDR3 byte 0x30000000, free for cores
wire        pack_wr = ioctl_download && ioctl_wr && ioctl_index[5:0] == 6'd1;
reg         rom_ok = 0;
reg         dl_q = 0;
reg  [19:0] dl_last;
always @(posedge clk_sys) begin
	dl_q <= ioctl_download && ioctl_index[5:0] == 6'd1;
	if (pack_wr) dl_last <= ioctl_addr[19:0];
	if (ioctl_download && ioctl_index[5:0] == 6'd1 && !dl_q) begin rom_ok <= 0; dl_last <= 0; end
	if (!ioctl_download && dl_q) rom_ok <= dl_last == 20'hA7FFF;
end

// PCM bytes go to DDR3 as 64-bit words (byte b of mt32_pcm.bin in lane b & 7)
wire        pcm_byte = pack_wr && ioctl_addr[26:20] == 0 && ioctl_addr[19:0] >= 20'h28000;
wire [19:0] pcm_off = ioctl_addr[19:0] - 20'h28000;
reg  [63:0] ld_word;
reg  [15:0] ld_waddr;
reg         ld_valid = 0;
wire        ld_ready;
always @(posedge clk_sys) begin
	if (ld_valid && ld_ready) ld_valid <= 0;
	if (pcm_byte) begin
		ld_word[pcm_off[2:0]*8 +: 8] <= ioctl_dout;
		if (pcm_off[2:0] == 3'd7) begin ld_valid <= 1; ld_waddr <= pcm_off[18:3]; end
	end
	ioctl_wait <= ld_valid && !ld_ready;
end

///////////////////////   DDR3   /////////////////////////////////

wire        c_req_valid [2];
wire        c_req_ready [2];
wire        c_req_we    [2];
wire [28:0] c_req_addr  [2];
wire [63:0] c_req_wdata [2];
wire  [7:0] c_req_be    [2];
wire  [7:0] c_req_len   [2];
wire  [1:0] c_req_tag   [2];
wire        c_rsp_valid [2];
wire [63:0] c_rsp_data;
wire  [1:0] c_rsp_tag;
wire        m_req_valid, m_req_ready, m_req_we, m_rsp_valid;
wire [28:0] m_req_addr;
wire [63:0] m_req_wdata, m_rsp_data;
wire  [7:0] m_req_be, m_req_len;
wire  [2:0] m_req_tag, m_rsp_tag;

assign c_req_valid[0] = ld_valid;
assign ld_ready       = c_req_ready[0];
assign c_req_we[0]    = 1'b1;
assign c_req_addr[0]  = PCM_BASE + {13'd0, ld_waddr};
assign c_req_wdata[0] = ld_word;
assign c_req_be[0]    = 8'hFF;
assign c_req_len[0]   = 8'd1;
assign c_req_tag[0]   = 2'd0;
assign c_req_we[1]    = 1'b0;
assign c_req_wdata[1] = 64'd0;
assign c_req_be[1]    = 8'hFF;
assign c_req_len[1]   = 8'd1;

synthframe_mem_arb #(.N(2), .CTW(2)) arb
(
	.c_req_valid, .c_req_ready, .c_req_we, .c_req_addr, .c_req_wdata, .c_req_be, .c_req_len,
	.c_req_tag, .c_rsp_valid, .c_rsp_data, .c_rsp_tag,
	.req_valid(m_req_valid), .req_ready(m_req_ready), .req_we(m_req_we), .req_addr(m_req_addr),
	.req_wdata(m_req_wdata), .req_be(m_req_be), .req_len(m_req_len), .req_tag(m_req_tag),
	.rsp_valid(m_rsp_valid), .rsp_data(m_rsp_data), .rsp_tag(m_rsp_tag)
);

assign DDRAM_CLK = clk_sys;
synthframe_mem_ddr3 #(.TW(3), .QBITS(4)) ddr3
(
	.clk(clk_sys), .rst(rst_hard),
	.req_valid(m_req_valid), .req_ready(m_req_ready), .req_we(m_req_we), .req_addr(m_req_addr),
	.req_wdata(m_req_wdata), .req_be(m_req_be), .req_len(m_req_len), .req_tag(m_req_tag),
	.rsp_valid(m_rsp_valid), .rsp_data(m_rsp_data), .rsp_tag(m_rsp_tag),
	.DDRAM_BUSY, .DDRAM_BURSTCNT, .DDRAM_ADDR, .DDRAM_DOUT, .DDRAM_DOUT_READY,
	.DDRAM_RD, .DDRAM_DIN, .DDRAM_BE, .DDRAM_WE
);

///////////////////////   PANEL   ////////////////////////////////

// Nuked-MT32's keys: Q W E R T = Part 1, 2, 3, Sound group, Volume; A S D F G = Part 4, 5,
// Rhythm, Sound, Master volume (mt32.h button order). - and = turn the volume knob.
reg  [9:0] panel = 0;
reg  [9:0] knob = 10'd1020;
reg        ps2_q = 0;
always @(posedge clk_sys) begin
	ps2_q <= ps2_key[10];
	if (ps2_key[10] != ps2_q) begin
		case (ps2_key[7:0])
			8'h15: panel[0] <= ps2_key[9];
			8'h1D: panel[1] <= ps2_key[9];
			8'h24: panel[2] <= ps2_key[9];
			8'h2D: panel[3] <= ps2_key[9];
			8'h2C: panel[4] <= ps2_key[9];
			8'h1C: panel[5] <= ps2_key[9];
			8'h1B: panel[6] <= ps2_key[9];
			8'h23: panel[7] <= ps2_key[9];
			8'h2B: panel[8] <= ps2_key[9];
			8'h34: panel[9] <= ps2_key[9];
			8'h4E: if (ps2_key[9]) knob <= (knob < 10'd20) ? 10'd0 : knob - 10'd20;
			8'h55: if (ps2_key[9]) knob <= (knob > 10'd1000) ? 10'd1020 : knob + 10'd20;
			default: ;
		endcase
	end
end

///////////////////////   CORE   /////////////////////////////////

wire core_reset = rst_hard | rst_user | ioctl_download | !rom_ok;

wire       mb_valid;
wire [7:0] mb_data;
wire       midi_alt;
synthframe_midi_rx #(.CLK_HZ(CLK_HZ)) midi_rx
(
	.clk(clk_sys), .rst(rst_hard), .rxd(UART_RXD),
	.byte_valid(mb_valid), .byte_data(mb_data), .frame_err(), .alt(midi_alt)
);

// MIDI out: the bytes the CPU writes to SBUF, queued for the wire (the CPU keeps Nuked-MT32's
// 9600-cycle transmit time, 800 us, slower than the wire's 320 us a byte); at 9600 baud when the
// receiver found the HPS UART there
wire       tx_stb, midi_tx;
wire [7:0] tx_byte;
synthframe_midi_tx #(.CLK_HZ(CLK_HZ)) midi_tx_out
(
	.clk(clk_sys), .rst(rst_hard), .alt(midi_alt),
	.valid(tx_stb), .data(tx_byte), .txd(midi_tx), .busy(), .dropped()
);
assign UART_TXD = midi_tx;

// Pacing: the core may run at most 8 samples ahead of a 32 kHz real-time count
reg  [26:0] pace_acc = 0;
reg  [15:0] rt_samples = 0, made = 0;
wire [15:0] ahead = made - rt_samples;
wire        sample_valid;
wire signed [15:0] out_l, out_r;
always @(posedge clk_sys) begin
	if (core_reset) begin
		pace_acc <= 0; rt_samples <= 0; made <= 0;
	end else begin
		if (pace_acc + 27'd32000 >= 27'(CLK_HZ)) begin
			pace_acc <= pace_acc + 27'd32000 - 27'(CLK_HZ);
			// behind by more than 0.1 s (cannot happen at speed): drop the backlog
			rt_samples <= (!ahead[15] || ahead > 16'hF380) ? rt_samples + 16'd1 : made;
		end else pace_acc <= pace_acc + 27'd32000;
		if (sample_valid) made <= made + 16'd1;
	end
end
wire hold = !ahead[15] && ahead >= 16'd8;

wire [4:0] lcd_rd_addr, lcd_cg_addr, lcd_cursor;
wire [7:0] lcd_rd_text;
wire [4:0] lcd_cg_row;
wire       lcd_cursor_on, lcd_display_on, core_ready, msg_led;

mt32_core #(
	.PCM_BASE(PCM_BASE),
	.DECODE_HEX("../hdl/mcs96/mcs96_decode.hex"),
	.POW_HEX("../hdl/la32/mt32_la32_pow.hex"),
	.LOGSIN_HEX("../hdl/la32/mt32_la32_logsin.hex")
) core
(
	.clk(clk_sys), .rst(core_reset), .ready(core_ready),
	.rom_we(pack_wr && ioctl_addr[26:17] == 0),
	.rom_addr(ioctl_addr[16:0]), .rom_data(ioctl_dout),
	.rvrom_we(pack_wr && ioctl_addr[26:15] == 12'd4),
	.rvrom_addr(ioctl_addr[14:0]), .rvrom_data(ioctl_dout),
	.pcm_req_valid(c_req_valid[1]), .pcm_req_ready(c_req_ready[1]), .pcm_req_addr(c_req_addr[1]),
	.pcm_req_tag(c_req_tag[1]), .pcm_rsp_valid(c_rsp_valid[1]), .pcm_rsp_data(c_rsp_data),
	.pcm_rsp_tag(c_rsp_tag),
	.hold(hold), .at_hold(),
	.midi_valid(mb_valid), .midi_byte(mb_data), .midi_full(),
	.buttons(panel), .knob(knob), .tx_stb(tx_stb), .tx_byte(tx_byte), .msg_led(msg_led),
	.sample_valid(sample_valid), .out_l(out_l), .out_r(out_r),
	.lcd_rd_addr(lcd_rd_addr), .lcd_rd_text(lcd_rd_text), .lcd_cg_addr(lcd_cg_addr),
	.lcd_cg_row(lcd_cg_row), .lcd_cursor(lcd_cursor), .lcd_cursor_on(lcd_cursor_on),
	.lcd_display_on(lcd_display_on),
	.cpu_cycles(), .la32_cycle(), .dbg_fetch(), .dbg_irq(), .dbg_regw(), .dbg_busw(), .dbg_midi(),
	.dbg_lcd(), .bad_op(), .dbg_level(), .dbg_pc(), .dbg_fpc(), .dbg_psw(), .dbg_waddr(), .dbg_addr(),
	.dbg_data(), .dbg_wdata(), .dbg_mbyte(), .extint(), .dbg_acc2(), .dbg_acc3(), .dbg_acc6(),
	.dbg_acc7(), .dbg_rv0(), .dbg_rv1(), .dbg_vol()
);

// LEDs: MIDI activity (stretched); disk: loading
reg [22:0] midi_stretch;
always @(posedge clk_sys) midi_stretch <= mb_valid ? 23'h7fffff : (midi_stretch ? midi_stretch - 1'd1 : 23'd0);
assign LED_USER = midi_stretch != 0;
assign LED_DISK = {1'b1, ioctl_download};

///////////////////////   AUDIO   ////////////////////////////////

// 32 kHz -> 96 kHz (polyphase FIR, cutoff 15 kHz, images below -90 dB)
wire        rs_valid;
wire signed [15:0] rs_l, rs_r;
synthframe_resample #(.L(3), .M(1), .TAPS(80),
	.COEF_HEX("../../../modules/synthframe/hdl/audio/synthframe_resample_32k_96k.hex")) resample
(
	.clk(clk_sys), .rst(core_reset),
	.in_valid(sample_valid), .in_l(out_l), .in_r(out_r),
	.out_valid(rs_valid), .out_l(rs_l), .out_r(rs_r)
);

wire signed [15:0] fifo_l, fifo_r;
wire [15:0] underruns;
synthframe_audio_fifo #(.CLK_HZ(CLK_HZ), .RATE(96_000), .DEPTH_BITS(8)) fifo
(
	.clk(clk_sys), .rst(core_reset),
	.in_valid(rs_valid), .in_l(rs_l), .in_r(rs_r),
	.out_l(fifo_l), .out_r(fifo_r), .out_strobe(),
	.underruns(underruns), .overruns()
);

// output gain with saturation
reg signed [15:0] audio_l, audio_r;
always @(posedge clk_sys) begin : gain
	reg signed [18:0] vl, vr;
	vl = 19'(fifo_l) <<< status[3:2];
	vr = 19'(fifo_r) <<< status[3:2];
	audio_l <= (vl > 19'sd32767) ? 16'sd32767 : (vl < -19'sd32768) ? 16'sh8000 : 16'(vl);
	audio_r <= (vr > 19'sd32767) ? 16'sd32767 : (vr < -19'sd32768) ? 16'sh8000 : 16'(vr);
end

assign AUDIO_S   = 1;
assign AUDIO_L   = audio_l;
assign AUDIO_R   = audio_r;
assign AUDIO_MIX = 0;

///////////////////////   VIDEO   ////////////////////////////////

wire hs, vs, hblank, vblank;
wire [7:0] r, g, b;
mt32_lcd_video #(.FONT_HEX("../hdl/mt32_lcd_font.hex")) video
(
	.clk(clk_vid),
	.text_addr(lcd_rd_addr), .text(lcd_rd_text), .cg_addr(lcd_cg_addr), .cg_row(lcd_cg_row),
	.cursor(lcd_cursor), .cursor_on(lcd_cursor_on), .display_on(lcd_display_on),
	.buttons(panel), .msg_led(msg_led),
	.rom_ok(rom_ok), .loading(ioctl_download), .running(core_ready && !core_reset),
	.midi_led(LED_USER), .underrun_seen(underruns != 0),
	.hs(hs), .vs(vs), .hblank(hblank), .vblank(vblank), .r(r), .g(g), .b(b)
);

assign CLK_VIDEO = clk_vid;
assign CE_PIXEL  = 1'b1;
assign VGA_DE = ~(hblank | vblank);
assign VGA_HS = hs;
assign VGA_VS = vs;
assign VGA_R  = r;
assign VGA_G  = g;
assign VGA_B  = b;

endmodule
