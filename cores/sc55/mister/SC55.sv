//============================================================================
//  Roland SC-55 (mk1) for MiSTer.
//
//  The instrument is hdl/sc55_core.sv (CPU, peripherals, PCM chip) inside
//  hdl/board/sc55_board.sv (ROM pack, wave ROMs in DDR3, pacing, MIDI,
//  audio); this file connects it to the MiSTer framework (sys/,
//  Template_MiSTer, GPL-2.0): OSD and ROM download through hps_io, DDR3,
//  status video, audio.
//
//  Use: make the ROM pack from your own SC-55 v1.21 ROMs with
//  `tools/romset.py pack DIR boot.rom` and copy it to games/SC-55/boot.rom;
//  set UART to MidiLink for MIDI in.
//============================================================================

module emu
(
	`include "sys/emu_ports.vh"
);

assign ADC_BUS  = 'Z;
assign USER_OUT = '1;
assign {UART_RTS, UART_DTR} = 0;
assign UART_TXD = 1'b1;
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
	"SC-55;UART31250,MIDI;",
	"-;",
	"F1,ROM,Load ROM pack;",
	"-;",
	"-,Panel keys: Q W E R T Y U I O P [;",
	"-,A S D F G H J K, arrows: part;",
	"-;",
	"O[1],Real-time pacing,On,Off;",
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

// rtl/pll: clk_sys 50 MHz (core; sc55_board CLK_HZ must match), clk_vid 40 MHz (800 x 600)
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
wire [31:0] panel_buttons;
kbd_buttons kbd(.clk(clk_sys), .rst(rst_hard), .ps2_key(ps2_key), .buttons(panel_buttons));
wire hs, vs, hblank, vblank;
wire [7:0] r, g, b;
wire        rom_ok, running, midi_seen, step_done;
wire [15:0] underruns;

assign DDRAM_CLK = clk_sys;
sc55_board #(.CLK_HZ(50_000_000)) board
(
	.clk(clk_sys),
	.clk_vid(clk_vid),
	.rst(rst_hard),
	.user_reset(rst_user),

	.ioctl_download(ioctl_download),
	.ioctl_wr(ioctl_wr),
	.ioctl_addr(ioctl_addr),
	.ioctl_dout(ioctl_dout),
	.ioctl_wait(ioctl_wait),

	.uart_rxd(UART_RXD),
	.midi_inject(1'b0),
	.midi_inject_data(8'd0),
	.buttons(panel_buttons),

	.pace_enable(~status[1]),
	.step_gate(1'b1),

	.audio_l(audio_l),
	.audio_r(audio_r),
	.sample_valid(), .sample_l(), .sample_r(),
	.vid_hs(hs), .vid_vs(vs), .vid_hblank(hblank), .vid_vblank(vblank),
	.vid_r(r), .vid_g(g), .vid_b(b),
	.lcd_pix(), .lcd_x(), .lcd_y(),

	.rom_ok(rom_ok),
	.running(running),
	.step_done(step_done),
	.midi_seen(midi_seen),
	.midi_alt(),
	.lag_us(),
	.underruns(underruns),
	.overruns(),
	.dbg_pc(), .dbg_cp(),

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

// LEDs: MIDI activity (stretched), loading
reg [21:0] midi_stretch;
always @(posedge clk_sys) midi_stretch <= midi_seen ? 22'h3fffff : (midi_stretch ? midi_stretch - 1'd1 : 22'd0);
assign LED_USER = midi_stretch != 0;
assign LED_DISK = {1'b1, ioctl_download};

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
