//============================================================================
//  E-mu Proteus/1 XR core for MiSTer
//
//  68000 (fx68k) running the original firmware, MC68901 MFP (timers, MIDI
//  USART), HD44780 LCD and an HDL model of the Proteus/1 sound chip.
//  Load the system image (tools/make_system.py: sample ROMs, firmware,
//  preset RAM, LCD font) with "Load System"; no ROM data is part of the core.
//  MIDI in/out on the MiSTer UART (MidiLink, UART mode MIDI).
//
//  This program is free software; you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by the Free
//  Software Foundation; either version 2 of the License, or (at your option)
//  any later version.
//============================================================================

module emu
(
	`include "sys/emu_ports.vh"
);

assign ADC_BUS  = 'Z;
assign USER_OUT = '1;
assign {SD_SCK, SD_MOSI, SD_CS} = 'Z;
assign {SDRAM_DQ, SDRAM_A, SDRAM_BA, SDRAM_CLK, SDRAM_CKE, SDRAM_DQML, SDRAM_DQMH, SDRAM_nWE, SDRAM_nCAS, SDRAM_nRAS, SDRAM_nCS} = 'Z;

assign VGA_SL = 0;
assign VGA_F1 = 0;
assign VGA_SCALER  = 0;
assign VGA_DISABLE = 0;
assign HDMI_FREEZE = 0;
assign HDMI_BLACKOUT = 0;
assign HDMI_BOB_DEINT = 0;

assign LED_DISK  = 0;
assign LED_POWER = 0;
assign BUTTONS   = 0;

wire [1:0] ar = status[122:121];
assign VIDEO_ARX = (!ar) ? 12'd4 : (ar - 1'd1);
assign VIDEO_ARY = (!ar) ? 12'd3 : 12'd0;

`include "build_id.v"
localparam CONF_STR = {
	"Proteus;UART31250,MIDI;",
	"-;",
	"F1,BIN,Load System;",
	"-;",
	"O[9:8],Volume,Max,75%,50%,25%;",
	"O[11:10],Output Level,0dB,+6dB,+12dB,+18dB;",
	"J1,Master,Edit,Cursor,Enter;",
	"jn,A,B,X,Y;",
	"O[122:121],Aspect ratio,Original,Full Screen,[ARC1],[ARC2];",
	"-;",
	"T[0],Reset;",
	"R[0],Reset and close OSD;",
	"V,v",`BUILD_DATE
};

wire forced_scandoubler;
wire   [1:0] buttons;
wire [127:0] status;
wire  [10:0] ps2_key;
wire  [31:0] joy0;

wire        ioctl_download;
wire        ioctl_wr;
wire [26:0] ioctl_addr;
wire  [7:0] ioctl_dout;
wire [15:0] ioctl_index;
wire        ioctl_wait;

hps_io #(.CONF_STR(CONF_STR)) hps_io
(
	.clk_sys(clk_sys),
	.HPS_BUS(HPS_BUS),
	.EXT_BUS(),
	.gamma_bus(),
	.forced_scandoubler(forced_scandoubler),
	.buttons(buttons),
	.status(status),
	.status_menumask(16'd0),
	.ps2_key(ps2_key),
	.joystick_0(joy0),
	.ioctl_download(ioctl_download),
	.ioctl_wr(ioctl_wr),
	.ioctl_addr(ioctl_addr),
	.ioctl_dout(ioctl_dout),
	.ioctl_index(ioctl_index),
	.ioctl_wait(ioctl_wait)
);

///////////////////////   CLOCKS   ///////////////////////////////

wire clk_sys;   // 80 MHz: 68000 at 10 MHz, 2048 clocks per 39062.5 Hz output sample
pll pll
(
	.refclk(CLK_50M),
	.rst(0),
	.outclk_0(clk_sys)
);

///////////////////////   SYSTEM   ///////////////////////////////

wire loading = ioctl_download && (ioctl_index[5:0] == 6'd1);
reg  loaded = 0;
reg  ld_d;
always @(posedge clk_sys) begin
	ld_d <= loading;
	if (ld_d && !loading) loaded <= 1;
	if (loading) loaded <= 0;
end

wire reset = RESET | status[0] | buttons[1] | loading | ~loaded;

wire        snd_wr, snd_pan, snd_wr_full, snd_rb, snd_rb_done;
wire  [4:0] snd_voice;
wire  [3:0] snd_word;
wire [15:0] snd_data, snd_mask, snd_rb_data, snd_raw;
wire        lcd_line, lcd_is_cg, lcd_cursor, lcd_on;
wire  [3:0] lcd_pos;
wire  [2:0] lcd_row;
wire  [7:0] lcd_code, font_data;
wire  [4:0] lcd_cgbits;
wire [11:0] font_addr;
wire        midi_out;
wire  [7:0] panel_gpio, leds, font2_data;
wire [11:0] font2_addr;

panel panel
(
	.clk(clk_sys), .reset(reset), .ps2_key(ps2_key), .joy(joy0),
	.gpio(panel_gpio), .buttons_dbg(), .step_dbg()
);

wire [7:0] volume = 8'd255 - {status[9:8], 6'd0};

proteus_sys sys
(
	.clk(clk_sys), .reset(reset), .loading(loading),
	.ioctl_wr(ioctl_wr), .ioctl_addr(ioctl_addr), .ioctl_dout(ioctl_dout),
	.midi_in(UART_RXD), .midi_out(midi_out), .buttons(panel_gpio), .volume(volume),
	.snd_wr(snd_wr), .snd_pan(snd_pan), .snd_voice(snd_voice), .snd_word(snd_word), .snd_data(snd_data), .snd_mask(snd_mask),
	.snd_wr_full(snd_wr_full), .snd_rb(snd_rb), .snd_rb_done(snd_rb_done), .snd_rb_data(snd_rb_data), .snd_raw(snd_raw),
	.lcd_line(lcd_line), .lcd_pos(lcd_pos), .lcd_row(lcd_row), .lcd_code(lcd_code), .lcd_cgbits(lcd_cgbits),
	.lcd_is_cg(lcd_is_cg), .lcd_cursor(lcd_cursor), .lcd_on(lcd_on), .font_addr(font_addr), .font_data(font_data),
	.font2_addr(font2_addr), .font2_data(font2_data), .leds(leds),
	.dbg_pc_addr(), .bus_wr_strobe(), .bus_wr_addr(), .bus_wr_data(), .bus_wr_ds()
);

assign UART_TXD = midi_out;
assign UART_RTS = 0;
assign UART_DTR = 0;

wire signed [15:0] out_l, out_r;
wire [4:0] disp_voice;
wire [7:0] disp_level;

snd_ctrl snd_ctrl
(
	.clk(clk_sys), .reset(reset), .loading(loading),
	.ioctl_wr(ioctl_wr), .ioctl_addr(ioctl_addr), .ioctl_dout(ioctl_dout), .ioctl_wait(ioctl_wait),
	.cpu_wr(snd_wr), .cpu_pan(snd_pan), .cpu_voice(snd_voice), .cpu_word(snd_word), .cpu_data(snd_data), .cpu_mask(snd_mask),
	.cpu_wr_full(snd_wr_full), .cpu_rb(snd_rb), .cpu_rb_done(snd_rb_done), .cpu_rb_data(snd_rb_data), .cpu_raw(snd_raw),
	.out_l(out_l), .out_r(out_r), .sample_strobe(),
	.disp_voice(disp_voice), .disp_level(disp_level),
	.DDRAM_BUSY(DDRAM_BUSY), .DDRAM_BURSTCNT(DDRAM_BURSTCNT), .DDRAM_ADDR(DDRAM_ADDR), .DDRAM_DOUT(DDRAM_DOUT),
	.DDRAM_DOUT_READY(DDRAM_DOUT_READY), .DDRAM_RD(DDRAM_RD), .DDRAM_DIN(DDRAM_DIN), .DDRAM_BE(DDRAM_BE), .DDRAM_WE(DDRAM_WE)
);
assign DDRAM_CLK = clk_sys;

///////////////////////   AUDIO   ///////////////////////////////

// Output level: the chip model mixes with 4x headroom for 32 full-level voices
// (a single centred voice peaks near -18 dBFS). Optional extra gain with hard
// saturation; the MiSTer core-volume boost (soft knee, sys/audio_out.sv) is the
// usual way to raise the level. Applied after the DDR3 capture, so the capture
// still matches the reference model.
wire [1:0] gain_sh = status[11:10];                         // 0, +6, +12, +18 dB
function signed [15:0] sat_shift(input signed [15:0] x, input [1:0] sh);
	reg signed [18:0] y;
	begin
		y = $signed({{3{x[15]}}, x}) <<< sh;
		if (y > 19'sd32767)       sat_shift = 16'sd32767;
		else if (y < -19'sd32768) sat_shift = -16'sd32768;
		else                      sat_shift = y[15:0];
	end
endfunction
reg signed [15:0] aud_l, aud_r;
always @(posedge clk_sys) begin
	aud_l <= sat_shift(out_l, gain_sh);
	aud_r <= sat_shift(out_r, gain_sh);
end

assign AUDIO_S   = 1;
assign AUDIO_L   = aud_l;
assign AUDIO_R   = aud_r;
assign AUDIO_MIX = 0;

///////////////////////   VIDEO   ///////////////////////////////

wire ce_pix, hs, vs, hbl, vbl;
wire [7:0] r, g, b;

display display
(
	.clk(clk_sys), .ce_pix(ce_pix), .hsync(hs), .vsync(vs), .hblank(hbl), .vblank(vbl), .r(r), .g(g), .b(b),
	.lcd_line(lcd_line), .lcd_pos(lcd_pos), .lcd_row(lcd_row), .lcd_code(lcd_code), .lcd_cgbits(lcd_cgbits),
	.lcd_is_cg(lcd_is_cg), .lcd_cursor(lcd_cursor), .lcd_on(lcd_on), .font_addr(font_addr), .font_data(font_data),
	.font2_addr(font2_addr), .font2_data(font2_data), .leds(leds),
	.voice(disp_voice), .level(disp_level), .running(loaded)
);

assign CLK_VIDEO = clk_sys;
assign CE_PIXEL  = ce_pix;
assign VGA_DE    = ~(hbl | vbl);
assign VGA_HS    = hs;
assign VGA_VS    = vs;
assign VGA_R     = r;
assign VGA_G     = g;
assign VGA_B     = b;

reg [26:0] act_cnt;
always @(posedge clk_sys) act_cnt <= act_cnt + 1'd1;
assign LED_USER = loaded ? act_cnt[25] : act_cnt[23];

endmodule
