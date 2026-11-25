//============================================================================
//  E-mu Proteus 2000 for MiSTer (cores/p2k/docs/plan.md).
//
//  The instrument is hdl/p2k_board.sv: the ColdFire CPU (TG68K with the MCF5206e changes and
//  peripherals) running the unit's own OS from the flash image, the sound engine (G20 voices,
//  H filters, G05 effects; bit-exact against the integer MAME model), all memory in DDR3. This
//  file connects it to the MiSTer framework (sys/, Template_MiSTer, GPL-2.0): OSD and image
//  downloads through hps_io, the flash save file, keyboard, MIDI, video and audio.
//
//  Use: make the system pack and a SIMM image from your own files with
//  cores/p2k/tools/p2k_pack.py, copy them to games/P2K/, load the pack (the save file
//  saves/P2K/<pack>.sav holds the flash: user presets and setups) and the SIMM from the OSD or
//  an MGL; set UART to MidiLink for MIDI in. The CPU restarts after each download.
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
	"P2K;UART31250,MIDI;",
	"-;",
	"FSC1,P2K,Load system pack,3C000000;",
	"FC2,SIM,SIMM slot 1,3C000000;",
	"FC3,SIM,SIMM slot 2,3C000000;",
	"-;",
	"-,Q master E edit M multi S save;",
	"-,A arp Space audition Enter home;",
	"-,<- -> cursor; - = or Dn Up data;",
	"-;",
	"O[3:1],Master volume,100%,85%,70%,55%,40%,25%,10%,0%;",
	"O[4],Filter,On,Off;",
	"O[5],Effects,On,Off;",
	"-;",
	"T[6],Save flash;",
	"O[7],Autosave on OSD open,On,Off;",
	"O[8],Diagnostics,Off,On;",
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

	// save file: the flash (saves/P2K/<pack>.sav)
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

// rtl/pll (fractional, VCO 882 MHz): clk_sys 88.2 MHz (sound engine, DDR3: 2000 clocks a 44.1 kHz
// sample), clk_cpu 44.1 MHz (CPU, aligned with clk_sys), clk_vid 40.09 MHz (800 x 600)
wire clk_sys, clk_cpu, clk_vid;
pll pll
(
	.refclk(CLK_50M),
	.rst(0),
	.outclk_0(clk_sys),
	.outclk_1(clk_cpu),
	.outclk_2(clk_vid)
);

reg rst_hard = 1, rst_user = 1;
always @(posedge clk_sys) begin
	rst_hard <= RESET;
	rst_user <= status[0] | buttons[1];
end

///////////////////////   CORE   /////////////////////////////////

localparam [28:0] SMP_BASE = 29'h6000000, FX_BASE = 29'h7000000, BABY_BASE = 29'h7100000, CPU_BASE = 29'h7200000;
localparam [28:0] STAGE_BASE = 29'h7800000;   // downloads land here (CONF_STR address 0x3C000000)

// keyboard
wire [15:7] panel_buttons;
wire        enc_step, enc_dir;
p2k_kbd kbd(.clk(clk_sys), .rst(rst_hard), .ps2_key(ps2_key), .buttons(panel_buttons), .enc_step(enc_step), .enc_dir(enc_dir));

// master volume: the volume pot's ADC conversion, MAME's adjuster 0-100 x 255 / 100
reg [7:0] vol;
always @(posedge clk_sys)
	case (status[3:1])
		3'd0: vol <= 8'd255; 3'd1: vol <= 8'd216; 3'd2: vol <= 8'd178; 3'd3: vol <= 8'd140;
		3'd4: vol <= 8'd102; 3'd5: vol <= 8'd63;  3'd6: vol <= 8'd25;  default: vol <= 8'd0;
	endcase

// MIDI in
wire midi_rxd_s, midi_byte;
p2k_midi #(.CLK_HZ(88_200_000)) midi(.clk(clk_sys), .rst(rst_hard), .uart_rxd(UART_RXD), .cpu_rxd(midi_rxd_s), .activity(midi_byte));
reg midi_rxd_c = 1;
always @(posedge clk_cpu) midi_rxd_c <= midi_rxd_s;
wire midi_txd;
assign UART_TXD = midi_txd;

// downloads
wire        font_we;
wire [11:0] font_addr;
wire  [7:0] font_data;
wire        pack_ok, loading, pack_loading, simm_loaded, ld_hold, fs_saving;
wire [24:0] big_words [4];
wire [20:0] baby_words [4];
wire        ld_req_valid, ld_req_ready, ld_req_we, ld_rsp_valid;
wire [28:0] ld_req_addr;
wire [63:0] ld_req_wdata;
wire  [7:0] ld_req_be, ld_req_len;
p2k_loader #(.SMP_BASE(SMP_BASE), .BABY_BASE(BABY_BASE), .CPU_BASE(CPU_BASE), .STAGE_BASE(STAGE_BASE)) loader
(
	.clk(clk_sys), .rst(rst_hard),
	.ioctl_download(ioctl_download), .ioctl_index(ioctl_index), .ioctl_wr(ioctl_wr),
	.ioctl_addr(ioctl_addr), .ioctl_dout(ioctl_dout), .ioctl_wait(ioctl_wait),
	.font_we(font_we), .font_addr(font_addr), .font_data(font_data),
	.pack_ok(pack_ok), .loading(loading), .pack_loading(pack_loading), .simm_loaded(simm_loaded),
	.big_words(big_words), .baby_words(baby_words),
	.ext_busy(fs_saving), .hold(ld_hold),
	.req_valid(ld_req_valid), .req_ready(ld_req_ready), .req_we(ld_req_we), .req_addr(ld_req_addr),
	.req_wdata(ld_req_wdata), .req_be(ld_req_be), .req_len(ld_req_len),
	.rsp_valid(ld_rsp_valid), .rsp_data(x_rsp_data)
);

// flash save file
wire        boot_hold, flash_lock, flash_changed;
wire        fs_req_valid, fs_req_ready, fs_req_we, fs_rsp_valid;
wire [28:0] fs_req_addr;
wire [63:0] fs_req_wdata, x_rsp_data;
wire  [7:0] fs_req_len;
p2k_flashsave #(.BASE(CPU_BASE)) fsave
(
	.clk(clk_sys), .rst(rst_hard),
	.loading(pack_loading), .image_ok(pack_ok),
	.img_mounted(img_mounted), .img_readonly(img_readonly), .img_size(img_size),
	.sd_lba(sd_lba), .sd_blk_cnt(sd_blk_cnt), .sd_rd(sd_rd), .sd_wr(sd_wr), .sd_ack(sd_ack),
	.sd_buff_addr(sd_buff_addr), .sd_buff_dout(sd_buff_dout), .sd_buff_wr(sd_buff_wr), .sd_buff_din(sd_buff_din),
	.save_req(status[6]), .osd_open(OSD_STATUS), .autosave(~status[7]), .flash_changed(flash_changed),
	.boot_hold(boot_hold), .flash_lock(flash_lock), .saving(fs_saving), .restored(), .dirty(),
	.req_valid(fs_req_valid), .req_ready(fs_req_ready), .req_we(fs_req_we), .req_addr(fs_req_addr),
	.req_wdata(fs_req_wdata), .req_len(fs_req_len), .rsp_valid(fs_rsp_valid), .rsp_data(x_rsp_data)
);

// the loader's byte writes go first; while its copier runs (ld_hold, started only with no save
// in flight) it alone uses the client and takes the responses
wire        x_valid = ld_req_valid | (fs_req_valid & ~ld_hold);
wire        x_ready;
wire        x_rsp_valid;
assign ld_req_ready = x_ready & ld_req_valid;
assign fs_req_ready = x_ready & ~ld_req_valid & ~ld_hold;
assign ld_rsp_valid = x_rsp_valid & ld_hold;
assign fs_rsp_valid = x_rsp_valid & ~ld_hold;

// the CPU starts once a system pack is in and restored, and again after every download
reg [7:0] restart;
always @(posedge clk_sys) restart <= (loading || simm_loaded || rst_user) ? 8'hff : (restart ? restart - 1'd1 : 8'd0);
wire cpu_hold = !pack_ok || boot_hold || loading || restart != 0;

wire signed [15:0] audio_l, audio_r;
wire        audio_strobe;
wire        r_line;
wire  [4:0] r_pos;
wire  [2:0] r_row;
wire  [7:0] r_code;
wire  [4:0] r_cgbits;
wire        r_at_ac, lcd_on, lcd_cursor, lcd_blink;
wire [15:0] leds;
wire        late, cpu_running;
wire [31:0] cpu_pc, cpu_addr;
wire        rsp_extra;

assign DDRAM_CLK = clk_sys;
p2k_board #(.SYS_HZ(88_200_000), .SMP_BASE(SMP_BASE), .FX_BASE(FX_BASE), .BABY_BASE(BABY_BASE), .CPU_BASE(CPU_BASE)) board
(
	.clk_sys(clk_sys), .clk_cpu(clk_cpu), .rst(rst_hard), .cpu_hold(cpu_hold),
	.buttons(panel_buttons), .enc_step(enc_step), .enc_dir(enc_dir), .master_adc(vol),
	.filter_on(~status[4]), .fx_on(~status[5]),
	.midi_rxd(midi_rxd_c), .midi_txd(midi_txd),
	.big_words(big_words), .baby_words(baby_words),
	.audio_l(audio_l), .audio_r(audio_r), .audio_strobe(audio_strobe),
	.r_line(r_line), .r_pos(r_pos), .r_row(r_row), .r_code(r_code), .r_cgbits(r_cgbits), .r_at_ac(r_at_ac),
	.lcd_display_on(lcd_on), .lcd_cursor_on(lcd_cursor), .lcd_blink_on(lcd_blink), .leds(leds),
	.flash_lock(flash_lock), .flash_changed(flash_changed),
	.x_req_valid(x_valid), .x_req_ready(x_ready), .x_req_we(ld_req_valid ? ld_req_we : fs_req_we),
	.x_req_addr(ld_req_valid ? ld_req_addr : fs_req_addr), .x_req_wdata(ld_req_valid ? ld_req_wdata : fs_req_wdata),
	.x_req_be(ld_req_valid ? ld_req_be : 8'hff), .x_req_len(ld_req_valid ? ld_req_len : fs_req_len),
	.x_rsp_valid(x_rsp_valid), .x_rsp_data(x_rsp_data),
	.late(late), .cpu_pc(cpu_pc), .cpu_addr(cpu_addr), .cpu_running(cpu_running), .mem_rsp_extra(rsp_extra),
	.dbg_snd_wr(), .dbg_snd_chip(), .dbg_snd_addr(), .dbg_snd_data(), .dbg_snd_mask(), .dbg_snd_start(),
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

// output level for the screen: peak of |L|, |R|, falling by 1/64 a 1/60 s
reg  [14:0] level;
reg  [20:0] decay;
wire [14:0] abs_l = audio_l[15] ? 15'(-audio_l) : audio_l[14:0];
wire [14:0] abs_r = audio_r[15] ? 15'(-audio_r) : audio_r[14:0];
always @(posedge clk_sys) begin
	decay <= decay == 21'd1469999 ? 21'd0 : decay + 1'd1;
	if (audio_strobe && (abs_l > level || abs_r > level)) level <= abs_l > abs_r ? abs_l : abs_r;
	else if (decay == 0) level <= level - (level >> 6) - (level != 0);
end

// LEDs: MIDI activity (stretched); disk: loading or saving
reg [21:0] midi_stretch;
always @(posedge clk_sys) midi_stretch <= midi_byte ? 22'h3fffff : (midi_stretch ? midi_stretch - 1'd1 : 22'd0);
assign LED_USER = midi_stretch != 0;
assign LED_DISK = {1'b1, loading | fs_saving};

///////////////////////   VIDEO   ////////////////////////////////

wire hs, vs, hblank, vblank;
wire [7:0] r, g, b;
p2k_lcd_video video
(
	.clk(clk_vid), .clk_sys(clk_sys),
	.font_we(font_we), .font_addr(font_addr), .font_data(font_data),
	.r_line(r_line), .r_pos(r_pos), .r_row(r_row), .r_code(r_code), .r_at_ac(r_at_ac), .r_cg_bits(r_cgbits),
	.display_on(lcd_on), .cursor_on(lcd_cursor), .blink_on(lcd_blink), .leds(leds), .level(level),
	.rom_ok(pack_ok), .loading(loading), .running(cpu_running), .midi_led(midi_stretch != 0), .late_seen(late),
	.diag(status[8]),
	.diag_hex({cpu_pc, cpu_addr, 3'd0, pack_ok, 3'd0, boot_hold, 3'd0, cpu_hold, 3'd0, lcd_on, 3'd0, late, 3'd0, flash_lock, 3'd0, rsp_extra, 4'd0}),
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

///////////////////////   AUDIO   ////////////////////////////////

// 44.1 kHz samples, held; sys/ filters and resamples to the output rate
assign AUDIO_S   = 1;
assign AUDIO_L   = audio_l;
assign AUDIO_R   = audio_r;
assign AUDIO_MIX = 0;

endmodule
