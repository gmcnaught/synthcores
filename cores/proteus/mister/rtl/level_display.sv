//============================================================================
//  Voice level display: 320x240, 32 vertical bars (one per voice)
//  Pixel clock = clk / 12 (6.667 MHz at 80 MHz): 424 x 262 total -> 60.0 Hz.
//============================================================================

module level_display
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
	output      [4:0] voice,      // voice whose level is shown at the current column
	input       [7:0] level,      // level[11:4] of that voice
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

// 10-pixel columns: 8-pixel bar + 2-pixel gap (counters, no divider)
reg [5:0] col;
reg [3:0] colx;
always @(posedge clk) if (cediv == 4'd11) begin
	if (hc == 9'd423) begin col <= 0; colx <= 0; end
	else if (colx == 4'd9) begin col <= col + 1'd1; colx <= 0; end
	else colx <= colx + 1'd1;
end
assign voice = col[4:0];

// bar height: level 0..255 -> 0..223 lines, from the bottom (line 231)
wire [8:0] height = {1'b0, level} - {4'b0, level[7:3]};
wire [8:0] y      = 9'd231 - vc;

always @(posedge clk) if (ce_pix) begin
	hblank <= (hc >= 9'd320);
	vblank <= (vc >= 9'd240);
	hsync  <= (hc >= 9'd336) && (hc < 9'd368);
	vsync  <= (vc >= 9'd244) && (vc < 9'd247);
	{r, g, b} <= {8'd16, 8'd16, 8'd32};
	if (hc < 9'd320 && vc < 9'd240) begin
		if (vc >= 9'd232) {r, g, b} <= running ? {8'd0, 8'd160, 8'd0} : {8'd160, 8'd0, 8'd0};
		else if (colx < 4'd8 && vc <= 9'd231 && y < height)
			{r, g, b} <= (y > 9'd180) ? {8'd255, 8'd64, 8'd32} : (y > 9'd120) ? {8'd255, 8'd200, 8'd0} : {8'd0, 8'd200, 8'd120};
	end
end

endmodule
