//============================================================================
//  Yamaha MU50 for MiSTer.
//
//  The instrument is hdl/mu50_board.sv (H8/3003 CPU and peripherals, SWP00 sound chip, LCD,
//  bit-exact to MAME's mu50 driver) inside hdl/mu50_top.sv (ROMs and effect delay lines in
//  DDR3, pacing, MIDI in, audio, NVRAM, LCD video); this file connects it to the MiSTer
//  framework (sys/, Template_MiSTer, GPL-2.0): OSD and ROM download through hps_io, DDR3,
//  video, audio.
//
//  Use: make the ROM pack from your own MU50 ROMs with
//  `cores/mu50/tools/mkpack.py ROMPATH mu50.rom` (ROMPATH: a MAME rompath with mu50/ and
//  mulcd/), copy it to games/MU50/ and load it once from the OSD (FSC: remembered;
//  saves/MU50/<pack>.sav holds the 64 KB battery RAM); set UART to MidiLink for MIDI in.
//  Panel keys from the keyboard as in MAME: A Play, E Edit, S Mute/Solo, [ ] Part -/+, U Util,
//  F Effect, Enter, , . Select </>, M Mode, Backspace Exit, - = Value -/+.
//============================================================================

module emu
(
	`include "sys/emu_ports.vh"
);

assign ADC_BUS  = 'Z;
assign USER_OUT = '1;
assign {UART_RTS, UART_DTR} = 0;
wire   midi_tx;
assign UART_TXD = midi_tx;
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
	"MU50;UART31250,MIDI;",
	"-;",
	"FSC1,ROM,Load ROM pack;",
	"-;",
	"-,A play E edit S mute [ ] part;",
	"-,U util F effect Enter , . select;",
	"-,M mode Bksp exit - = value;",
	"-;",
	"O[3:2],Output gain,0 dB,+6 dB,+12 dB,+18 dB;",
	"O[1],Real-time pacing,On,Off;",
	"-;",
	"T[4],Save NVRAM;",
	"O[5],Autosave on OSD open,On,Off;",
	"-;",
	"T[0],Reset;",
	"R[0],Reset and close OSD;",
	"V,v",`BUILD_DATE
};

wire   [1:0] buttons;
wire [127:0] status;
wire  [10:0] ps2_key;
wire         ioctl_download, ioctl_wr, ioctl_wait;
wire  [26:0] ioctl_addr;
wire   [7:0] ioctl_dout;
wire  [15:0] ioctl_index;
wire         img_mounted, img_readonly;
wire  [63:0] img_size;
wire  [31:0] sd_lba;
wire   [5:0] sd_blk_cnt;
wire         sd_rd, sd_wr, sd_ack, sd_buff_wr;
wire  [13:0] sd_buff_addr;
wire   [7:0] sd_buff_dout, sd_buff_din;

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

// rtl/pll: clk_sys 50 MHz (core; mu50_top CLK_HZ must match), clk_vid 40 MHz (800 x 600)
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

///////////////////////   CORE   /////////////////////////////////

wire signed [15:0] audio_l, audio_r;
wire hs, vs, hblank, vblank;
wire [7:0] r, g, b;
wire        rom_ok, running, midi_seen, saving;
wire [15:0] underruns;

assign DDRAM_CLK = clk_sys;
mu50_top #(.CLK_HZ(50_000_000)) top
(
	.clk(clk_sys),
	.clk_vid(clk_vid),
	.rst(rst_hard),
	.user_reset(rst_user),

	.ioctl_download(ioctl_download && ioctl_index[5:0] == 6'd1),
	.ioctl_wr(ioctl_wr),
	.ioctl_addr(ioctl_addr),
	.ioctl_dout(ioctl_dout),
	.ioctl_wait(ioctl_wait),

	.img_mounted(img_mounted), .img_readonly(img_readonly), .img_size(img_size),
	.sd_lba(sd_lba), .sd_blk_cnt(sd_blk_cnt), .sd_rd(sd_rd), .sd_wr(sd_wr), .sd_ack(sd_ack),
	.sd_buff_addr(sd_buff_addr), .sd_buff_dout(sd_buff_dout), .sd_buff_wr(sd_buff_wr), .sd_buff_din(sd_buff_din),
	.save_req(status[4]), .osd_open(OSD_STATUS), .autosave(~status[5]), .saving(saving),

	.uart_rxd(UART_RXD),
	.uart_txd(midi_tx),
	.ps2_key(ps2_key),

	.pace_enable(~status[1]),
	.gain(status[3:2]),

	.audio_l(audio_l),
	.audio_r(audio_r),
	.vid_hs(hs), .vid_vs(vs), .vid_hblank(hblank), .vid_vblank(vblank),
	.vid_r(r), .vid_g(g), .vid_b(b),

	.rom_ok(rom_ok),
	.running(running),
	.midi_seen(midi_seen),
	.underruns(underruns),

	.DDRAM_BUSY(DDRAM_BUSY),
	.DDRAM_BURSTCNT(DDRAM_BURSTCNT),
	.DDRAM_ADDR(DDRAM_ADDR),
	.DDRAM_DOUT(DDRAM_DOUT),
	.DDRAM_DOUT_READY(DDRAM_DOUT_READY),
	.DDRAM_RD(DDRAM_RD),
	.DDRAM_DIN(DDRAM_DIN),
	.DDRAM_BE(DDRAM_BE),
	.DDRAM_WE(DDRAM_WE)
);

// LEDs: MIDI activity (stretched); disk: loading or saving
reg [21:0] midi_stretch;
always @(posedge clk_sys) midi_stretch <= midi_seen ? 22'h3fffff : (midi_stretch != 0 ? midi_stretch - 1'd1 : 22'd0);
assign LED_USER = midi_stretch != 0;
assign LED_DISK = {1'b1, ioctl_download | saving};

///////////////////////   VIDEO   ////////////////////////////////

assign CLK_VIDEO = clk_vid;
assign CE_PIXEL  = 1'b1;
assign VGA_DE = ~(hblank | vblank);
assign VGA_HS = hs;
assign VGA_VS = vs;
assign VGA_R  = r;
assign VGA_G  = g;
assign VGA_B  = b;

///////////////////////   AUDIO   ////////////////////////////////

assign AUDIO_S   = 1;
assign AUDIO_L   = audio_l;
assign AUDIO_R   = audio_r;
assign AUDIO_MIX = 0;

endmodule
