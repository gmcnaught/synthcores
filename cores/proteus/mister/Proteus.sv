//============================================================================
//  E-mu Proteus/1 sound demo core for MiSTer
//
//  Plays a register-write event stream (captured from MAME running the
//  Proteus/1 XR firmware) through an HDL model of the Proteus/1 sound chip.
//  Load the demo image (tools/make_demo.py) with "Load Demo"; the image is
//  never part of the core.
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
assign {UART_RTS, UART_TXD, UART_DTR} = 0;
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
	"Proteus;;",
	"-;",
	"F1,BIN,Load Demo;",
	"-;",
	"O[122:121],Aspect ratio,Original,Full Screen,[ARC1],[ARC2];",
	"-;",
	"T[0],Restart Demo;",
	"R[0],Restart and close OSD;",
	"V,v",`BUILD_DATE
};

wire forced_scandoubler;
wire   [1:0] buttons;
wire [127:0] status;

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
	.ioctl_download(ioctl_download),
	.ioctl_wr(ioctl_wr),
	.ioctl_addr(ioctl_addr),
	.ioctl_dout(ioctl_dout),
	.ioctl_index(ioctl_index),
	.ioctl_wait(ioctl_wait)
);

///////////////////////   CLOCKS   ///////////////////////////////

wire clk_sys;   // 80 MHz: 2048 clocks per 39062.5 Hz output sample
pll pll
(
	.refclk(CLK_50M),
	.rst(0),
	.outclk_0(clk_sys)
);

///////////////////////   DEMO   ///////////////////////////////

wire demo_load = ioctl_download && (ioctl_index[5:0] == 6'd1);
reg  loaded = 0;
reg  dl_d;
always @(posedge clk_sys) begin
	dl_d <= demo_load;
	if (dl_d && !demo_load) loaded <= 1;
	if (demo_load) loaded <= 0;
end

wire reset = RESET | status[0] | buttons[1] | demo_load;

wire        snd_wr, pan_wr, snd_start, snd_busy, snd_done, mem_req, mem_ack;
wire  [4:0] snd_voice;
wire  [3:0] snd_word;
wire [15:0] snd_data;
wire  [7:0] pan_data;
wire signed [15:0] snd_l, snd_r;
wire [21:0] mem_addr;
wire [15:0] mem_s0, mem_s1;
wire [15:0] overruns;
wire [31:0] samples;

demo_ctrl demo_ctrl
(
	.clk(clk_sys), .reset(reset), .run(loaded),
	.ioctl_download(demo_load), .ioctl_wr(ioctl_wr), .ioctl_addr(ioctl_addr), .ioctl_dout(ioctl_dout), .ioctl_wait(ioctl_wait),
	.snd_wr(snd_wr), .snd_voice(snd_voice), .snd_word(snd_word), .snd_data(snd_data),
	.pan_wr(pan_wr), .pan_data(pan_data), .snd_start(snd_start), .snd_busy(snd_busy), .snd_done(snd_done),
	.snd_l(snd_l), .snd_r(snd_r), .mem_req(mem_req), .mem_addr(mem_addr), .mem_ack(mem_ack), .mem_s0(mem_s0), .mem_s1(mem_s1),
	.overruns(overruns), .samples(samples),
	.DDRAM_BUSY(DDRAM_BUSY), .DDRAM_BURSTCNT(DDRAM_BURSTCNT), .DDRAM_ADDR(DDRAM_ADDR), .DDRAM_DOUT(DDRAM_DOUT),
	.DDRAM_DOUT_READY(DDRAM_DOUT_READY), .DDRAM_RD(DDRAM_RD), .DDRAM_DIN(DDRAM_DIN), .DDRAM_BE(DDRAM_BE), .DDRAM_WE(DDRAM_WE)
);
assign DDRAM_CLK = clk_sys;

wire [4:0] disp_voice;
wire [7:0] disp_level;

proteus_snd proteus_snd
(
	.clk(clk_sys), .reset(reset),
	.wr(snd_wr), .wr_voice(snd_voice), .wr_word(snd_word), .wr_data(snd_data),
	.pan_wr(pan_wr), .pan_voice(snd_voice), .pan_data(pan_data),
	.start(snd_start), .busy(snd_busy), .done(snd_done), .out_l(snd_l), .out_r(snd_r),
	.mem_req(mem_req), .mem_addr(mem_addr), .mem_ack(mem_ack), .mem_s0(mem_s0), .mem_s1(mem_s1),
	.disp_voice(disp_voice), .disp_level(disp_level)
);

///////////////////////   AUDIO   ///////////////////////////////

reg [15:0] aud_l, aud_r;
always @(posedge clk_sys) if (snd_done) begin aud_l <= snd_l; aud_r <= snd_r; end

assign AUDIO_S   = 1;
assign AUDIO_L   = aud_l;
assign AUDIO_R   = aud_r;
assign AUDIO_MIX = 0;

///////////////////////   VIDEO   ///////////////////////////////

wire ce_pix, hs, vs, hbl, vbl;
wire [7:0] r, g, b;

level_display level_display
(
	.clk(clk_sys), .ce_pix(ce_pix), .hsync(hs), .vsync(vs), .hblank(hbl), .vblank(vbl),
	.r(r), .g(g), .b(b), .voice(disp_voice), .level(disp_level), .running(loaded)
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
