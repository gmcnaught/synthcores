//============================================================================
//  Proteus display: 320x240 @ 60 Hz (pixel clock = clk / 12)
//   - LCD panel (2 x 16 characters, 5x8 dots, each dot 3x3 pixels) at the top
//   - 32 voice level bars below
//  Font ROM (HD44780 A00, 16 bytes per character) is read through font_addr/font_data
//  with one clock of latency; the picture is output one pixel late to cover it.
//============================================================================

module display
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

wire in_panel = (hc >= 9'd8) && (hc < 9'd312) && (vc >= 9'd12) && (vc < 9'd82);
wire dot_area = in_lx && in_ly && (cx < 5'd15) && (cy < 5'd24);

// ------------------------------------------------ voice bars: 10-pixel columns from y 232 upward, max 140 px
reg [5:0] col;
reg [3:0] colx;
always @(posedge clk) if (cediv == 4'd11) begin
	if (hc == 9'd423) begin col <= 0; colx <= 0; end
	else if (colx == 4'd9) begin col <= col + 1'd1; colx <= 0; end
	else colx <= colx + 1'd1;
end
assign voice = col[4:0];
wire [8:0] height = {2'b0, level[7:1]} + {3'b0, level[7:2]};   // 0..190 -> scaled to ~0..141
wire [8:0] y      = 9'd231 - vc;

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
	p_bar      <= (vc >= 9'd88) && (vc <= 9'd231) && (colx < 4'd8) && (hc < 9'd320) && (y < height);
	p_bar_hot  <= (y > 9'd110);
	p_bar_warm <= (y > 9'd70);
	p_status   <= (vc >= 9'd232) && (vc < 9'd240);
	p_hbl      <= (hc >= 9'd320);
	p_vbl      <= (vc >= 9'd240);
	p_hs       <= (hc >= 9'd336) && (hc < 9'd368);
	p_vs       <= (vc >= 9'd244) && (vc < 9'd247);

	hblank <= p_hbl;
	vblank <= p_vbl;
	hsync  <= p_hs;
	vsync  <= p_vs;
	{r, g, b} <= {8'd16, 8'd16, 8'd32};
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
