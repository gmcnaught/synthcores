//============================================================================
//  Proteus display: 320x240 @ 60 Hz (pixel clock = clk / 12)
//   - the front panel (cfg/panel.yaml, drawn by synthframe_panel_art from the ROMs in
//     rtl/panel/ that modules/synthframe/tools/panel_art.py makes), its LEDs lit and its
//     buttons shown pressed
//   - over it: the LCD (2 x 16 characters, 5x8 dots, each dot 3x3 pixels), the 32 voice
//     level bars in the panel's VOICES area, and the running bar (y 232-239)
//  Font ROM (HD44780 A00, 16 bytes per character) is read through font_addr/font_data
//  with one clock of latency; the picture is output one pixel late to cover it.
//============================================================================

module display #(
	parameter PANEL_DIR = "rtl/panel/"   // Quartus: relative to mister/
)
(
	input             clk,
	output reg        ce_pix,
	output reg        hsync,
	output reg        vsync,
	output reg        hblank,
	output reg        vblank,
	output reg  [7:0] r,
	output reg  [7:0] g,
	output reg  [7:0] b,

	// LCD render port (hd44780) and font ROM
	output reg        lcd_line,
	output reg  [3:0] lcd_pos,
	output reg  [2:0] lcd_row,
	input       [7:0] lcd_code,
	input       [4:0] lcd_cgbits,
	input             lcd_is_cg,
	input             lcd_cursor,
	input             lcd_on,
	output     [11:0] font_addr,
	input       [7:0] font_data,
	input       [7:0] leds,             // latch 0xB00001, active low: bit0 MASTER, bit1 EDIT, bit3 MIDI
	input       [3:0] buttons,          // MFP GPIP 3-0, active low: MASTER, EDIT, CURSOR, ENTER

	// voice levels
	output      [4:0] voice,
	input       [7:0] level,
	input             running
);

reg [3:0] cediv;
reg [8:0] hc;
reg [8:0] vc;

always @(posedge clk) begin
	ce_pix <= 0;
	cediv <= (cediv == 4'd11) ? 4'd0 : cediv + 1'd1;
	if (cediv == 4'd11) begin
		ce_pix <= 1;
		if (hc == 9'd423) begin
			hc <= 0;
			vc <= (vc == 9'd261) ? 9'd0 : vc + 1'd1;
		end
		else hc <= hc + 1'd1;
	end
end

// ------------------------------------------------ LCD geometry
// panel x 16..303 (16 chars x 18 px: 5 dots x 3 px + 3 px gap), y 20..73 (2 lines x 27 px: 8 dots x 3 + 3)
localparam LX0 = 9'd16, LY0 = 9'd20;
reg  [4:0] cx;     // 0..17 within character cell
reg  [4:0] cy;     // 0..26 within line
reg        in_lx, in_ly;
reg  [2:0] dot_x;
reg  [1:0] sub_x, sub_y;

always @(posedge clk) if (cediv == 4'd11) begin
	// horizontal (advance with hc)
	if (hc == LX0 - 1'd1) begin in_lx <= 1; cx <= 0; lcd_pos <= 0; dot_x <= 0; sub_x <= 0; end
	else if (in_lx) begin
		if (cx == 5'd17) begin
			cx <= 0; dot_x <= 0; sub_x <= 0;
			if (lcd_pos == 4'd15) in_lx <= 0; else lcd_pos <= lcd_pos + 1'd1;
		end
		else begin
			cx <= cx + 1'd1;
			if (sub_x == 2'd2) begin sub_x <= 0; dot_x <= dot_x + 1'd1; end
			else sub_x <= sub_x + 1'd1;
		end
	end
	// vertical (advance at the end of each line)
	if (hc == 9'd423) begin
		if (vc == LY0 - 1'd1) begin in_ly <= 1; cy <= 0; lcd_line <= 0; lcd_row <= 0; sub_y <= 0; end
		else if (in_ly) begin
			if (cy == 5'd26) begin
				cy <= 0; lcd_row <= 0; sub_y <= 0;
				if (lcd_line) in_ly <= 0; else lcd_line <= 1;
			end
			else begin
				cy <= cy + 1'd1;
				if (sub_y == 2'd2) begin sub_y <= 0; lcd_row <= lcd_row + 1'd1; end
				else sub_y <= sub_y + 1'd1;
			end
		end
	end
end

assign font_addr = {lcd_code, 1'b0, lcd_row};   // code * 16 + row

// LCD background: panel.yaml's lcd rect, x 12..304, y 16..74 (4 pixels around the dots)
wire in_panel = (hc >= 9'd12) && (hc < 9'd305) && (vc >= 9'd16) && (vc < 9'd75);
wire dot_area = in_lx && in_ly && (cx < 5'd15) && (cy < 5'd24);

// MIDI LED: stretch short blinks to ~60 ms
reg [22:0] midi_hold;
always @(posedge clk) begin
	if (!leds[3]) midi_hold <= 23'd4800000;
	else if (midi_hold != 0) midi_hold <= midi_hold - 1'd1;
end
wire led_midi = (midi_hold != 0);

// ------------------------------------------------ front panel art, 3 enabled clocks
// It is addressed two pixels ahead so its colour lines up with the one-pixel-late picture.
wire        ahead_wrap = (hc >= 9'd422);
wire [10:0] art_x = ahead_wrap ? {2'b0, hc - 9'd422} : {2'b0, hc + 9'd2};
wire  [9:0] art_y = !ahead_wrap ? {1'b0, vc} : (vc == 9'd261) ? 10'd0 : {1'b0, vc + 9'd1};
`include "proteus_panel.svh"
wire [2**PANEL_EB-1:0] panel_state;
`include "proteus_panel_state.svh"
generate if (PANEL_NE + 1 < 2**PANEL_EB)
	assign panel_state[2**PANEL_EB-1:PANEL_NE+1] = '0;
endgenerate
wire [23:0] art;
synthframe_panel_art #(.W(PANEL_W), .H(PANEL_H), .BPP(PANEL_BPP), .TB(PANEL_TB), .EB(PANEL_EB),
                       .NT(PANEL_NT), .PAL(PANEL_PAL), .LIT(PANEL_LIT),
                       .MAP_HEX({PANEL_DIR, "proteus_panel_map.hex"}),
                       .TILE_HEX({PANEL_DIR, "proteus_panel_tiles.hex"})) panel_art (
	.clk(clk), .ce(ce_pix), .x(art_x), .y(art_y), .state(panel_state), .rgb(art));

// ------------------------------------------------ voice bars in the VOICES area: 32 columns of
// 7 pixels on a 9-pixel pitch from x 16, rising from y 225, up to 63 pixels (level / 4)
reg  [4:0] col;
reg  [3:0] colx;
reg        in_bx;
always @(posedge clk) if (cediv == 4'd11) begin
	if (hc == 9'd15) begin in_bx <= 1; col <= 0; colx <= 0; end
	else if (in_bx) begin
		if (colx == 4'd8) begin colx <= 0; if (col == 5'd31) in_bx <= 0; else col <= col + 1'd1; end
		else colx <= colx + 1'd1;
	end
end
assign voice = col;
wire [8:0] height = {3'b0, level[7:2]};
wire [8:0] y      = 9'd225 - vc;

// ------------------------------------------------ pixel pipeline (one pixel late)
reg        p_dot_area, p_in_panel, p_cursor, p_bar, p_bar_hot, p_bar_warm, p_status;
reg  [2:0] p_dot_x;
reg  [4:0] p_cg;
reg        p_is_cg, p_on;
reg        p_hbl, p_vbl, p_hs, p_vs;

always @(posedge clk) if (ce_pix) begin
	p_dot_area <= dot_area;
	p_in_panel <= in_panel;
	p_dot_x    <= dot_x;
	p_cg       <= lcd_cgbits;
	p_is_cg    <= lcd_is_cg;
	p_cursor   <= lcd_cursor;
	p_on       <= lcd_on;
	p_bar      <= (vc >= 9'd162) && (vc <= 9'd225) && in_bx && (colx < 4'd7) && (y < height);
	p_bar_hot  <= (y > 9'd36);
	p_bar_warm <= (y > 9'd23);
	p_status   <= (vc >= 9'd232) && (vc < 9'd240);
	p_hbl      <= (hc >= 9'd320);
	p_vbl      <= (vc >= 9'd240);
	p_hs       <= (hc >= 9'd336) && (hc < 9'd368);
	p_vs       <= (vc >= 9'd244) && (vc < 9'd247);

	hblank <= p_hbl;
	vblank <= p_vbl;
	hsync  <= p_hs;
	vsync  <= p_vs;
	{r, g, b} <= (p_hbl || p_vbl) ? {8'd16, 8'd16, 8'd32} : art;
	if (p_in_panel) {r, g, b} <= {8'd40, 8'd70, 8'd40};
	if (p_dot_area && p_on) begin
		// font_data / p_cg row bits: bit 4 is the leftmost dot
		if ((p_is_cg ? p_cg[3'd4 - p_dot_x] : font_data[3'd4 - p_dot_x]) || p_cursor)
			{r, g, b} <= {8'd210, 8'd255, 8'd200};
		else
			{r, g, b} <= {8'd56, 8'd92, 8'd56};
	end
	if (p_bar) {r, g, b} <= p_bar_hot ? {8'd255, 8'd64, 8'd32} : p_bar_warm ? {8'd255, 8'd200, 8'd0} : {8'd0, 8'd200, 8'd120};
	if (p_status && !p_hbl) {r, g, b} <= running ? {8'd0, 8'd160, 8'd0} : {8'd160, 8'd0, 8'd0};
end

endmodule
