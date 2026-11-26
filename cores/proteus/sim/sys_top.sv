// Simulation harness: Proteus system (fx68k + peripherals) + sound controller, DDR3 port to C++.
module sys_top
(
	input             clk,
	input             reset,
	input             loading,
	input             ioctl_wr,
	input      [26:0] ioctl_addr,
	input       [7:0] ioctl_dout,
	input             midi_in,
	input       [7:0] gpio,
	output            midi_out,
	input             DDRAM_BUSY,
	output      [7:0] DDRAM_BURSTCNT,
	output     [28:0] DDRAM_ADDR,
	input      [63:0] DDRAM_DOUT,
	input             DDRAM_DOUT_READY,
	output            DDRAM_RD,
	output     [63:0] DDRAM_DIN,
	output      [7:0] DDRAM_BE,
	output            DDRAM_WE,
	output            bus_wr_strobe,
	output     [23:0] bus_wr_addr,
	output     [15:0] bus_wr_data,
	output      [1:0] bus_wr_ds,
	output            sample_strobe,
	output signed [15:0] out_l,
	output signed [15:0] out_r,
	input             lcd_line,
	input       [3:0] lcd_pos,
	output      [7:0] lcd_code
);
wire snd_wr, snd_pan, snd_wr_full, snd_rb, snd_rb_done;
wire [4:0] snd_voice; wire [3:0] snd_word; wire [15:0] snd_data, snd_mask, snd_rb_data, snd_raw;
wire ioctl_wait;

proteus_sys sys
(
	.clk(clk), .reset(reset), .loading(loading),
	.ioctl_wr(ioctl_wr), .ioctl_addr(ioctl_addr), .ioctl_dout(ioctl_dout),
	.midi_in(midi_in), .midi_out(midi_out), .buttons(gpio), .volume(8'd255),
	.cpu_pause(1'b0), .nv_active(1'b0), .nv_we(1'b0), .nv_addr(16'd0), .nv_wdata(8'd0), .nv_rdata(), .leds(),
	.snd_wr(snd_wr), .snd_pan(snd_pan), .snd_voice(snd_voice), .snd_word(snd_word), .snd_data(snd_data), .snd_mask(snd_mask),
	.snd_wr_full(snd_wr_full), .snd_rb(snd_rb), .snd_rb_done(snd_rb_done), .snd_rb_data(snd_rb_data), .snd_raw(snd_raw),
	.lcd_line(lcd_line), .lcd_pos(lcd_pos), .lcd_row(3'd0), .lcd_code(lcd_code), .lcd_cgbits(), .lcd_is_cg(), .lcd_cursor(), .lcd_on(),
	.font_addr(12'd0), .font_data(),
	.dbg_pc_addr(), .bus_wr_strobe(bus_wr_strobe), .bus_wr_addr(bus_wr_addr), .bus_wr_data(bus_wr_data), .bus_wr_ds(bus_wr_ds)
);

snd_ctrl sctl
(
	.clk(clk), .reset(reset), .loading(loading), .tone_en(1'b1),
	.ioctl_wr(ioctl_wr), .ioctl_addr(ioctl_addr), .ioctl_dout(ioctl_dout), .ioctl_wait(ioctl_wait),
	.cpu_wr(snd_wr), .cpu_pan(snd_pan), .cpu_voice(snd_voice), .cpu_word(snd_word), .cpu_data(snd_data), .cpu_mask(snd_mask),
	.cpu_wr_full(snd_wr_full), .cpu_rb(snd_rb), .cpu_rb_done(snd_rb_done), .cpu_rb_data(snd_rb_data), .cpu_raw(snd_raw),
	.out_l(out_l), .out_r(out_r), .sample_strobe(sample_strobe),
	.disp_voice(5'd0), .disp_level(),
	.DDRAM_BUSY(DDRAM_BUSY), .DDRAM_BURSTCNT(DDRAM_BURSTCNT), .DDRAM_ADDR(DDRAM_ADDR), .DDRAM_DOUT(DDRAM_DOUT),
	.DDRAM_DOUT_READY(DDRAM_DOUT_READY), .DDRAM_RD(DDRAM_RD), .DDRAM_DIN(DDRAM_DIN), .DDRAM_BE(DDRAM_BE), .DDRAM_WE(DDRAM_WE)
);
endmodule
