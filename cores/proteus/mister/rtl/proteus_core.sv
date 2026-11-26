//============================================================================
//  Proteus/1 XR core: everything between hps_io and the MiSTer pins, so the
//  testbench in sim/core_* (Verilator) exercises exactly the FPGA's wiring.
//
//  System image (ioctl index 1, tools/build_image.py):
//    0x000000 samples (8 MB, DDR3), 0x800000 program ROM (128 KB),
//    0x820000 RAM (64 KB), 0x830000 LCD font (4 KB),
//    0x831000 flags: bit0 = RAM is blank, initialise presets on first boot
//  Battery RAM persists in the save file mounted on SD slot 0 (nv_ctrl).
//============================================================================

module proteus_core
(
	input             clk,             // 80 MHz
	input             reset_in,        // RESET | OSD reset | user button

	// HPS download
	input             ioctl_download,
	input      [15:0] ioctl_index,
	input             ioctl_wr,
	input      [26:0] ioctl_addr,
	input       [7:0] ioctl_dout,
	output            ioctl_wait,

	// save file (SD slot 0)
	input             img_mounted,
	input             img_readonly,
	input      [63:0] img_size,
	output     [31:0] sd_lba,
	output      [5:0] sd_blk_cnt,
	output            sd_rd,
	output            sd_wr,
	input             sd_ack,
	input      [13:0] sd_buff_addr,
	input       [7:0] sd_buff_dout,
	input             sd_buff_wr,
	output      [7:0] sd_buff_din,

	// OSD controls
	input       [1:0] volume_sel,      // 0 = max .. 3 = 25 %
	input       [1:0] gain_sel,        // output gain 0/+6/+12/+18 dB
	input             save_req,        // "Save Presets" (level)
	input             autosave,        // save when the OSD opens
	input             osd_open,
	input             factory_reset,   // "Factory Reset Presets" (level)
	input             tone_en,         // tone filter on

	// inputs
	input      [10:0] ps2_key,
	input      [31:0] joy,
	input             midi_in,
	output            midi_out,

	// audio
	output reg signed [15:0] audio_l,
	output reg signed [15:0] audio_r,

	// video
	output            ce_pix,
	output            hsync,
	output            vsync,
	output            hblank,
	output            vblank,
	output      [7:0] r,
	output      [7:0] g,
	output      [7:0] b,

	output            loaded_out,
	output            saving,

	// DDR3
	input             DDRAM_BUSY,
	output      [7:0] DDRAM_BURSTCNT,
	output     [28:0] DDRAM_ADDR,
	input      [63:0] DDRAM_DOUT,
	input             DDRAM_DOUT_READY,
	output            DDRAM_RD,
	output     [63:0] DDRAM_DIN,
	output      [7:0] DDRAM_BE,
	output            DDRAM_WE,

	// debug (testbench)
	output            dbg_wr_strobe,
	output     [23:0] dbg_wr_addr,
	output     [15:0] dbg_wr_data,
	output      [1:0] dbg_wr_ds,
	output            dbg_sample,
	output signed [15:0] dbg_out_l,
	output signed [15:0] dbg_out_r
);

// ------------------------------------------------ image load
wire loading = ioctl_download && (ioctl_index[5:0] == 6'd1);
reg  loaded = 0;
reg  ld_d;
reg  init_flag;
always @(posedge clk) begin
	ld_d <= loading;
	if (ld_d && !loading) loaded <= 1;
	if (loading && !ld_d) begin loaded <= 0; init_flag <= 0; end
	if (loading && ioctl_wr && ioctl_addr == 27'h831000) init_flag <= ioctl_dout[0];
end
assign loaded_out = loaded;

// ------------------------------------------------ save file / preset initialisation
wire       boot_hold, cpu_pause, restored;
wire       nv_active, nv_we;
wire [15:0] nv_addr;
wire [7:0]  nv_wdata, nv_rdata;
wire       macro_active, macro_rst, macro_done;
wire [7:0] macro_gpio;
reg        macro_start;
reg        macro_pend = 0;  // a macro run is due; it starts when no save is in progress
reg        post_init_save;
reg [28:0] post_init_cnt;
reg        boot_hold_d, factory_d;

nv_ctrl nv
(
	.clk(clk), .reset(reset_in), .loading(loading), .loaded(loaded),
	.img_mounted(img_mounted), .img_readonly(img_readonly), .img_size(img_size),
	.sd_lba(sd_lba), .sd_blk_cnt(sd_blk_cnt), .sd_rd(sd_rd), .sd_wr(sd_wr), .sd_ack(sd_ack),
	.sd_buff_addr(sd_buff_addr), .sd_buff_dout(sd_buff_dout), .sd_buff_wr(sd_buff_wr), .sd_buff_din(sd_buff_din),
	.save_req(save_req | post_init_save), .osd_open(osd_open), .autosave(autosave),
	.inhibit(macro_pend | macro_start | macro_active),
	.nv_active(nv_active), .nv_we(nv_we), .nv_addr(nv_addr), .nv_wdata(nv_wdata), .nv_rdata(nv_rdata),
	.boot_hold(boot_hold), .cpu_pause(cpu_pause), .saving(saving), .restored(restored)
);

init_macro macro
(
	.clk(clk), .start(macro_start), .active(macro_active), .gpio(macro_gpio), .cpu_reset(macro_rst), .done(macro_done)
);

always @(posedge clk) begin
	boot_hold_d <= boot_hold;
	factory_d   <= factory_reset;
	macro_start <= 0;
	// first boot from a blank-RAM image without a save file, or OSD "Factory Reset Presets".
	// The macro and a save never overlap: nv_ctrl starts no save while a run is due or
	// active, and a run that falls due during a save waits for it (the save pauses the CPU,
	// which would make the macro's timed knob pulses and ENTER presses miss).
	if ((boot_hold_d && !boot_hold && loaded && init_flag && !restored) ||
	    (factory_reset && !factory_d && loaded && !boot_hold && !macro_active))
		macro_pend <= 1;
	if (macro_pend && !saving && !macro_active) begin
		macro_pend  <= 0;
		macro_start <= 1;
	end
	if (loading) macro_pend <= 0;
	// save the initialised presets once the firmware has rebooted (4 s)
	post_init_save <= 0;
	if (macro_done) post_init_cnt <= 29'd320_000_000;
	else if (post_init_cnt != 0) begin
		post_init_cnt <= post_init_cnt - 1'd1;
		if (post_init_cnt == 1) post_init_save <= 1;
	end
	if (loading) post_init_cnt <= 0;
end

wire cpu_reset = reset_in | loading | ~loaded | boot_hold | macro_rst;

// ------------------------------------------------ front panel
wire [7:0] panel_gpio;
panel panel
(
	.clk(clk), .reset(cpu_reset), .ps2_key(ps2_key), .joy(joy),
	.gpio(panel_gpio), .buttons_dbg(), .step_dbg()
);
wire [7:0] gpio = macro_active ? macro_gpio : panel_gpio;
wire [7:0] volume = 8'd255 - {volume_sel, 6'd0};

// ------------------------------------------------ system
wire        snd_wr, snd_pan, snd_wr_full, snd_rb, snd_rb_done;
wire  [4:0] snd_voice;
wire  [3:0] snd_word;
wire [15:0] snd_data, snd_mask, snd_rb_data, snd_raw;
wire        snd_raw_ok;
wire        lcd_line, lcd_is_cg, lcd_cursor, lcd_on;
wire  [3:0] lcd_pos;
wire  [2:0] lcd_row;
wire  [7:0] lcd_code, font_data, leds;
wire  [4:0] lcd_cgbits;
wire [11:0] font_addr;

proteus_sys sys
(
	.clk(clk), .reset(cpu_reset), .loading(loading),
	.ioctl_wr(ioctl_wr), .ioctl_addr(ioctl_addr), .ioctl_dout(ioctl_dout),
	.midi_in(midi_in), .midi_out(midi_out), .buttons(gpio), .volume(volume),
	.cpu_pause(cpu_pause),
	.nv_active(nv_active), .nv_we(nv_we), .nv_addr(nv_addr), .nv_wdata(nv_wdata), .nv_rdata(nv_rdata),
	.snd_wr(snd_wr), .snd_pan(snd_pan), .snd_voice(snd_voice), .snd_word(snd_word), .snd_data(snd_data), .snd_mask(snd_mask),
	.snd_wr_full(snd_wr_full), .snd_rb(snd_rb), .snd_rb_done(snd_rb_done), .snd_rb_data(snd_rb_data), .snd_raw(snd_raw), .snd_raw_ok(snd_raw_ok),
	.lcd_line(lcd_line), .lcd_pos(lcd_pos), .lcd_row(lcd_row), .lcd_code(lcd_code), .lcd_cgbits(lcd_cgbits),
	.lcd_is_cg(lcd_is_cg), .lcd_cursor(lcd_cursor), .lcd_on(lcd_on), .font_addr(font_addr), .font_data(font_data),
	.leds(leds),
	.dbg_pc_addr(), .bus_wr_strobe(dbg_wr_strobe), .bus_wr_addr(dbg_wr_addr), .bus_wr_data(dbg_wr_data), .bus_wr_ds(dbg_wr_ds)
);

// ------------------------------------------------ sound
wire signed [15:0] out_l, out_r;
wire [4:0] disp_voice;
wire [7:0] disp_level;

snd_ctrl snd_ctrl
(
	.clk(clk), .reset(cpu_reset), .loading(loading), .tone_en(tone_en),
	.ioctl_wr(ioctl_wr), .ioctl_addr(ioctl_addr), .ioctl_dout(ioctl_dout), .ioctl_wait(ioctl_wait),
	.cpu_wr(snd_wr), .cpu_pan(snd_pan), .cpu_voice(snd_voice), .cpu_word(snd_word), .cpu_data(snd_data), .cpu_mask(snd_mask),
	.cpu_wr_full(snd_wr_full), .cpu_rb(snd_rb), .cpu_rb_done(snd_rb_done), .cpu_rb_data(snd_rb_data), .cpu_raw(snd_raw), .cpu_raw_ok(snd_raw_ok),
	.out_l(out_l), .out_r(out_r), .sample_strobe(dbg_sample),
	.disp_voice(disp_voice), .disp_level(disp_level),
	.DDRAM_BUSY(DDRAM_BUSY), .DDRAM_BURSTCNT(DDRAM_BURSTCNT), .DDRAM_ADDR(DDRAM_ADDR), .DDRAM_DOUT(DDRAM_DOUT),
	.DDRAM_DOUT_READY(DDRAM_DOUT_READY), .DDRAM_RD(DDRAM_RD), .DDRAM_DIN(DDRAM_DIN), .DDRAM_BE(DDRAM_BE), .DDRAM_WE(DDRAM_WE)
);
assign dbg_out_l = out_l;
assign dbg_out_r = out_r;

// Output level: the chip model mixes with 4x headroom for 32 full-level voices (a single
// centred voice peaks near -18 dBFS). Optional extra gain with hard saturation; the MiSTer
// core-volume boost (soft knee, sys/audio_out.sv) is the usual way to raise the level.
// Applied after the DDR3 capture, so the capture still matches the reference model.
function signed [15:0] sat_shift(input signed [15:0] x, input [1:0] sh);
	reg signed [18:0] y;
	begin
		y = $signed({{3{x[15]}}, x}) <<< sh;
		if (y > 19'sd32767)       sat_shift = 16'sd32767;
		else if (y < -19'sd32768) sat_shift = -16'sd32768;
		else                      sat_shift = y[15:0];
	end
endfunction
always @(posedge clk) begin
	audio_l <= sat_shift(out_l, gain_sel);
	audio_r <= sat_shift(out_r, gain_sel);
end

// ------------------------------------------------ video
display display
(
	.clk(clk), .ce_pix(ce_pix), .hsync(hsync), .vsync(vsync), .hblank(hblank), .vblank(vblank), .r(r), .g(g), .b(b),
	.lcd_line(lcd_line), .lcd_pos(lcd_pos), .lcd_row(lcd_row), .lcd_code(lcd_code), .lcd_cgbits(lcd_cgbits),
	.lcd_is_cg(lcd_is_cg), .lcd_cursor(lcd_cursor), .lcd_on(lcd_on), .font_addr(font_addr), .font_data(font_data),
	.leds(leds), .buttons(gpio[3:0]),
	.voice(disp_voice), .level(disp_level), .running(loaded)
);

endmodule
