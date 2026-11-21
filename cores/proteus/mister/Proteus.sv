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
	"FS1,BIN,Load System;",
	"-;",
	"O[9:8],Volume,Max,75%,50%,25%;",
	"O[11:10],Output Level,0dB,+6dB,+12dB,+18dB;",
	"O[15],Tone Filter,On,Off;",
	"-;",
	"T[12],Save Presets;",
	"O[13],Autosave on OSD open,On,Off;",
	"T[14],Factory Reset Presets;",
	"-;",
	"O[122:121],Aspect ratio,Original,Full Screen,[ARC1],[ARC2];",
	"J1,Master,Edit,Cursor,Enter;",
	"jn,A,B,X,Y;",
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

wire        img_mounted, img_readonly;
wire [63:0] img_size;
wire [31:0] sd_lba;
wire  [5:0] sd_blk_cnt;
wire        sd_rd, sd_wr, sd_ack, sd_buff_wr;
wire [13:0] sd_buff_addr;
wire  [7:0] sd_buff_dout, sd_buff_din;

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
	.ioctl_wait(ioctl_wait),
	.img_mounted(img_mounted),
	.img_readonly(img_readonly),
	.img_size(img_size),
	.sd_lba('{sd_lba}),
	.sd_blk_cnt('{sd_blk_cnt}),
	.sd_rd(sd_rd),
	.sd_wr(sd_wr),
	.sd_ack(sd_ack),
	.sd_buff_addr(sd_buff_addr),
	.sd_buff_dout(sd_buff_dout),
	.sd_buff_din('{sd_buff_din}),
	.sd_buff_wr(sd_buff_wr)
);

///////////////////////   CLOCKS   ///////////////////////////////

wire clk_sys;   // 80 MHz: 68000 at 10 MHz, 2048 clocks per 39062.5 Hz output sample
pll pll
(
	.refclk(CLK_50M),
	.rst(0),
	.outclk_0(clk_sys)
);

///////////////////////   CORE   ///////////////////////////////

wire ce_pix, hs, vs, hbl, vbl, loaded, saving;
wire [7:0] r, g, b;
wire signed [15:0] aud_l, aud_r;

proteus_core core
(
	.clk(clk_sys), .reset_in(RESET | status[0] | buttons[1]),
	.ioctl_download(ioctl_download), .ioctl_index(ioctl_index), .ioctl_wr(ioctl_wr), .ioctl_addr(ioctl_addr),
	.ioctl_dout(ioctl_dout), .ioctl_wait(ioctl_wait),
	.img_mounted(img_mounted), .img_readonly(img_readonly), .img_size(img_size),
	.sd_lba(sd_lba), .sd_blk_cnt(sd_blk_cnt), .sd_rd(sd_rd), .sd_wr(sd_wr), .sd_ack(sd_ack),
	.sd_buff_addr(sd_buff_addr), .sd_buff_dout(sd_buff_dout), .sd_buff_wr(sd_buff_wr), .sd_buff_din(sd_buff_din),
	.volume_sel(status[9:8]), .gain_sel(status[11:10]),
	.save_req(status[12]), .autosave(~status[13]), .osd_open(OSD_STATUS), .factory_reset(status[14]),
	.tone_en(~status[15]),
	.ps2_key(ps2_key), .joy(joy0), .midi_in(UART_RXD), .midi_out(UART_TXD),
	.audio_l(aud_l), .audio_r(aud_r),
	.ce_pix(ce_pix), .hsync(hs), .vsync(vs), .hblank(hbl), .vblank(vbl), .r(r), .g(g), .b(b),
	.loaded_out(loaded), .saving(saving),
	.DDRAM_BUSY(DDRAM_BUSY), .DDRAM_BURSTCNT(DDRAM_BURSTCNT), .DDRAM_ADDR(DDRAM_ADDR), .DDRAM_DOUT(DDRAM_DOUT),
	.DDRAM_DOUT_READY(DDRAM_DOUT_READY), .DDRAM_RD(DDRAM_RD), .DDRAM_DIN(DDRAM_DIN), .DDRAM_BE(DDRAM_BE), .DDRAM_WE(DDRAM_WE),
	.dbg_wr_strobe(), .dbg_wr_addr(), .dbg_wr_data(), .dbg_wr_ds(), .dbg_sample(), .dbg_out_l(), .dbg_out_r()
);
assign DDRAM_CLK = clk_sys;
assign UART_RTS  = 0;
assign UART_DTR  = 0;

assign AUDIO_S   = 1;
assign AUDIO_L   = aud_l;
assign AUDIO_R   = aud_r;
assign AUDIO_MIX = 0;

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
assign LED_USER  = saving | (loaded ? act_cnt[25] : act_cnt[23]);

endmodule
