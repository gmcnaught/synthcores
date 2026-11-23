// SPDX-License-Identifier: MIT
//============================================================================
//  Casio CZ-101 for MiSTer.
//
//  The instrument is hdl/cz101_board.sv (uPD7811, uPD933, HD44780, battery RAM, RA-3
//  cartridge), checked against MAME's cz101 instruction by instruction and sample by sample
//  (cores/cz101/docs/plan.md). This file connects it to the MiSTer framework (sys/,
//  Template_MiSTer, GPL-2.0): OSD and ROM download through hps_io, video, audio (resampled
//  from 40 to 96 kHz), MidiLink.
//
//  Use: make the ROM pack from MAME's cz101 set and the HD44780 character ROM with
//  `cores/cz101/tools/mkpack.py`, copy it to games/CZ-101/ and load it once from the OSD
//  (FSC: remembered). saves/CZ-101/<pack>.sav holds the battery RAM and the cartridge; it is
//  saved when the OSD opens or from "Save RAM". Set UART to MidiLink for MIDI in.
//
//  Pack: 0000-7FFF program ROM, 8000-8FFF initial battery RAM (MAME init_ram.bin),
//  9000-9FFF HD44780 character ROM (hd44780_a00.bin).
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
// DDR3 unused; its port still needs a running clock (the scaler shares the controller)
assign DDRAM_CLK = clk_sys;
assign {DDRAM_BURSTCNT, DDRAM_ADDR, DDRAM_DIN, DDRAM_BE, DDRAM_RD, DDRAM_WE} = '0;

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
	"CZ-101;UART31250,MIDI;",
	"-;",
	"FSC1,ROM,Load ROM pack;",
	"-;",
	"-,Keys as MAME's cz101 driver;",
	"-,Q W 1-6 E-U arrows;",
	"-,Tone 1-8: F1-F8 or keypad 1-8;",
	"-,Select: Enter or keypad 0;",
	"-,A-J Z-M K L . / - = , Home End;",
	"-;",
	"O[6],Power,On,Off;",
	"O[9],RA-3 cartridge,None,Inserted;",
	"O[7],Memory protect,Off,On;",
	"O[8],Auto power off,Off,On;",
	"O[3:2],Output gain,0 dB,+6 dB,+12 dB,+18 dB;",
	"-;",
	"T[4],Save RAM;",
	"O[5],Autosave on OSD open,On,Off;",
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
	.ioctl_wait(1'b0),

	// save file: battery RAM + cartridge (saves/CZ-101/<pack>.sav)
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

// rtl/pll: clk_sys 50 MHz (core; CLK_HZ below must match), clk_vid 40 MHz (800 x 600)
localparam longint CLK_HZ = 50_000_000;
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

wire        pack_wr = ioctl_download && ioctl_wr && ioctl_index[5:0] == 6'd1;
reg         rom_ok = 0;
reg         dl_q = 0;
reg  [15:0] dl_last;
always @(posedge clk_sys) begin
	dl_q <= ioctl_download && ioctl_index[5:0] == 6'd1;
	if (pack_wr) dl_last <= ioctl_addr[15:0];
	if (ioctl_download && ioctl_index[5:0] == 6'd1 && !dl_q) begin rom_ok <= 0; dl_last <= 0; end
	if (!ioctl_download && dl_q) rom_ok <= dl_last == 16'h9FFF;
end

///////////////////////   BATTERY RAM FILE   /////////////////////

wire        nv_we, nv_hold, nv_pause, saving, nv_written;
wire [12:0] nv_addr;
wire  [7:0] nv_wdata, nv_rdata;
reg   [9:0] pause_cnt;
wire        core_idle = &pause_cnt;      // paused long enough for the instruction in flight
always @(posedge clk_sys) pause_cnt <= !nv_pause ? 10'd0 : core_idle ? pause_cnt : pause_cnt + 1'd1;

synthframe_nvsave #(.AW(13)) nvsave
(
	.clk(clk_sys), .rst(rst_hard),
	.loading(ioctl_download), .image_ok(rom_ok),
	.img_mounted(img_mounted), .img_readonly(img_readonly), .img_size(img_size),
	.sd_lba(sd_lba), .sd_blk_cnt(sd_blk_cnt), .sd_rd(sd_rd), .sd_wr(sd_wr), .sd_ack(sd_ack),
	.sd_buff_addr(sd_buff_addr), .sd_buff_dout(sd_buff_dout), .sd_buff_wr(sd_buff_wr),
	.sd_buff_din(sd_buff_din),
	.save_req(status[4]), .osd_open(OSD_STATUS), .autosave(~status[5]), .mem_written(nv_written),
	.mem_we(nv_we), .mem_addr(nv_addr), .mem_wdata(nv_wdata), .mem_rdata(nv_rdata),
	.core_idle(core_idle), .boot_hold(nv_hold), .pause(nv_pause), .saving(saving), .restored()
);

///////////////////////   POWER   ////////////////////////////////

// Power switch: off asserts /NMI (the firmware saves and powers down); on after a power-down
// restarts the board as a power cycle (the battery RAM keeps its contents).
wire power_on = ~status[6];
wire powered_down;
reg  power_q = 1;
reg  [3:0] pcycle = 0;
always @(posedge clk_sys) begin
	power_q <= power_on;
	if (power_on && !power_q && powered_down) pcycle <= 4'hF;
	else if (pcycle) pcycle <= pcycle - 1'd1;
end

wire board_reset = rst_hard | rst_user | ioctl_download | !rom_ok | nv_hold | (pcycle != 0);

///////////////////////   PACING   ///////////////////////////////

// Real time in CPU cycles (MAME: 3,333,334 a second); the CPU runs while its virtual time
// (cycles) is behind, so it leads real time by at most one instruction.
localparam longint CPU_HZ = 3_333_334;
reg  [25:0] pace_acc = 0;
reg  [31:0] rt_cycles = 0;
wire [31:0] cycles;
wire [31:0] lag = rt_cycles - cycles;
always @(posedge clk_sys) begin
	if (board_reset || nv_pause) begin
		rt_cycles <= cycles;
		pace_acc <= 0;
	end else begin
		if (pace_acc + 26'(CPU_HZ) >= 26'(CLK_HZ)) begin
			pace_acc <= pace_acc + 26'(CPU_HZ) - 26'(CLK_HZ);
			// more than 0.1 s behind (cannot happen at speed): drop the backlog
			rt_cycles <= (!lag[31] && lag > 32'd333333) ? cycles + 32'd1 : rt_cycles + 32'd1;
		end else pace_acc <= pace_acc + 26'(CPU_HZ);
	end
end
wire cyc_ce = !lag[31] && lag != 0 && !nv_pause;

///////////////////////   MIDI IN   //////////////////////////////

wire       mb_valid;
wire [7:0] mb_data;
synthframe_midi_rx #(.CLK_HZ(CLK_HZ)) midi_rx
(
	.clk(clk_sys), .rst(rst_hard), .rxd(UART_RXD),
	.byte_valid(mb_valid), .byte_data(mb_data), .frame_err(), .alt()
);

// 64-byte queue: bytes arrive in real time, the board sends them in virtual time
reg  [7:0] mq [64];
reg  [5:0] mq_wp = 0, mq_rp = 0;
wire       mq_empty = mq_wp == mq_rp;
wire       midi_pop;
always @(posedge clk_sys) begin
	if (mb_valid && 6'(mq_wp + 1'd1) != mq_rp) begin mq[mq_wp] <= mb_data; mq_wp <= mq_wp + 1'd1; end
	if (midi_pop) mq_rp <= mq_rp + 1'd1;
end

///////////////////////   CORE   /////////////////////////////////

wire [7:0] keys [16];
cz101_kbd kbd
(
	.clk(clk_sys), .reset(rst_hard), .ps2_key(ps2_key),
	.mem_protect(status[7]), .auto_power_off(status[8]), .cart_present(status[9]),
	.keys(keys)
);

wire        sample_valid;
wire signed [15:0] sample;
wire [15:0] leds;
wire        lcd_power, lcd_display_on, lcd_cursor_on, lcd_blink_on;
wire        r_line, r_cursor;
wire  [3:0] r_pos;
wire  [7:0] r_code;
wire  [2:0] r_cg_code, r_cg_row;
wire  [4:0] r_cg_bits;
wire        midi_tx;

cz101_board #(.DECODE_HEX("../hdl/upd7810/upd7810_decode.hex"), .TABLE_DIR("../hdl/upd933/")) board
(
	.clk(clk_sys),
	.reset(board_reset),
	.cyc_ce(cyc_ce),

	.load_wr(pack_wr && ioctl_addr[15:12] < 4'h9 && ioctl_addr[26:16] == 0),
	.load_addr(ioctl_addr[15:0]),
	.load_data(ioctl_dout),

	.power_on(power_on),
	.cart_present(status[9]),
	.keys(keys),
	.pitch_wheel(8'h7F),
	.rx_valid(1'b0), .rx_cycle(32'd0), .rx_edge(32'd0), .rx_level(1'b1), .rx_pop(),
	.midi_valid(!mq_empty), .midi_byte(mq[mq_rp]), .midi_pop(midi_pop),
	.midi_tx(midi_tx),
	.nv_addr(nv_addr), .nv_we(nv_we), .nv_wdata(nv_wdata), .nv_rdata(nv_rdata), .nv_written(nv_written),
	.powered_down(powered_down),
	.cycles(cycles),

	.leds(leds), .led_latch(), .lcd_power(lcd_power),
	.sample_valid(sample_valid), .sample(sample), .sample_sum(),

	.r_line(r_line), .r_pos(r_pos), .r_code(r_code), .r_cursor(r_cursor),
	.r_cg_code(r_cg_code), .r_cg_row(r_cg_row), .r_cg_bits(r_cg_bits),
	.lcd_display_on(lcd_display_on), .lcd_cursor_on(lcd_cursor_on), .lcd_blink_on(lcd_blink_on),

	.insn_done(), .insn_cc(), .insn_pc(), .insn_op(), .insn_op2(), .halt_cycle(),
	.irq_taken(), .irq_taken_vec(),
	.r_v(), .r_a(), .r_b(), .r_c(), .r_d(), .r_e(), .r_h(), .r_l(),
	.r_ea(), .r_sp(), .r_pc(), .r_psw(), .r_irr(),
	.wr_done(), .wr_addr(), .wr_data(), .upd933_late()
);

assign UART_TXD = midi_tx;

// LEDs: MIDI activity (stretched); disk: loading or saving
reg [21:0] midi_stretch;
always @(posedge clk_sys) midi_stretch <= mb_valid ? 22'h3fffff : (midi_stretch ? midi_stretch - 1'd1 : 22'd0);
assign LED_USER = midi_stretch != 0;
assign LED_DISK = {1'b1, ioctl_download | saving};

///////////////////////   AUDIO   ////////////////////////////////

// 40 kHz -> 96 kHz (polyphase FIR, cutoff 20 kHz, images below -90 dB), so the framework's
// 48 or 96 kHz output gets no 40 kHz staircase images
wire        rs_valid;
wire signed [15:0] rs_out;
synthframe_resample #(.L(12), .M(5), .TAPS(80),
	.COEF_HEX("../../../modules/synthframe/hdl/audio/synthframe_resample_40k_96k.hex")) resample
(
	.clk(clk_sys), .rst(board_reset),
	.in_valid(sample_valid), .in_l(sample), .in_r(16'sd0),
	.out_valid(rs_valid), .out_l(rs_out), .out_r()
);

wire signed [15:0] fifo_out;
wire [15:0] underruns;
synthframe_audio_fifo #(.CLK_HZ(CLK_HZ), .RATE(96_000), .DEPTH_BITS(8)) fifo
(
	.clk(clk_sys), .rst(board_reset),
	.in_valid(rs_valid), .in_l(rs_out), .in_r(rs_out),
	.out_l(fifo_out), .out_r(), .out_strobe(),
	.underruns(underruns), .overruns()
);

// output gain with saturation
reg signed [15:0] audio;
always @(posedge clk_sys) begin : gain
	reg signed [18:0] v;
	v = 19'(fifo_out) <<< status[3:2];
	audio <= (v > 19'sd32767) ? 16'sd32767 : (v < -19'sd32768) ? 16'sh8000 : 16'(v);
end

assign AUDIO_S   = 1;
assign AUDIO_L   = audio;
assign AUDIO_R   = audio;
assign AUDIO_MIX = 0;

///////////////////////   VIDEO   ////////////////////////////////

wire hs, vs, hblank, vblank;
wire [7:0] r, g, b;
cz101_lcd_video video
(
	.clk(clk_vid),
	.clk_sys(clk_sys),
	.font_we(pack_wr && ioctl_addr[26:12] == 15'h9),
	.font_addr(ioctl_addr[11:0]),
	.font_data(ioctl_dout),
	.r_line(r_line), .r_pos(r_pos), .r_code(r_code), .r_cursor(r_cursor),
	.r_cg_code(r_cg_code), .r_cg_row(r_cg_row), .r_cg_bits(r_cg_bits),
	.display_on(lcd_display_on), .cursor_on(lcd_cursor_on), .blink_on(lcd_blink_on),
	.power(lcd_power), .leds(leds),
	.rom_ok(rom_ok), .loading(ioctl_download), .midi_led(LED_USER),
	.underrun_seen(underruns != 0),
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
