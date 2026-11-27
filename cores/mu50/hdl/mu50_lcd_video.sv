// SPDX-License-Identifier: MIT
// 800 x 600 at 60 Hz (40 MHz pixel clock): the MU50's LCD (HD44780 set to 4 x 20, as MAME's
// mulcd) as 4 lines of 20 characters of 5 x 8 dots, the five panel LEDs under it, and the
// status strip of the other synthframe cores (state band, MIDI, underrun, lag).
//
// Characters come from the board's HD44780 render port (line 0/1, position 0-39: the 4 x 20
// layout is lines 0 and 1 at positions 0-19, then again at 20-39); codes 00-0F from its
// character-generator RAM, the others from the HD44780's ROM B04 (16 bytes a code, row r at
// code * 16 + r), loaded from the ROM pack. The render port and the LEDs are read from the core
// clock without synchronisation (display only), as the JV-880 core does.
module mu50_lcd_video (
    input  logic        clk,             // pixel clock
    // character ROM load (core clock)
    input  logic        cg_clk,
    input  logic        cg_we,
    input  logic [11:0] cg_addr,
    input  logic [7:0]  cg_data,
    // board render port (combinational on the board side)
    output logic        r_line,
    output logic [4:0]  r_pos,
    output logic [2:0]  r_row,
    input  logic [7:0]  r_code,
    input  logic [4:0]  r_cg_bits,
    input  logic        lcd_on,
    input  logic [4:0]  leds,
    // status strip
    input  logic        rom_ok,
    input  logic        loading,
    input  logic        running,
    input  logic        midi_led,
    input  logic        underrun_led,
    input  logic        underrun_seen,
    input  logic        lag_seen,
    output logic        hs,
    output logic        vs,
    output logic        hblank,
    output logic        vblank,
    output logic [7:0]  r,
    output logic [7:0]  g,
    output logic [7:0]  b
);
    localparam int X0 = 80, Y0 = 120, PW = 640, PH = 220;     // the LCD panel
    localparam int SY = 560;                                  // the status strip

    // ------------------------------------------------------------ character ROM (dual clock)
    logic [7:0] cgrom [4096];
    always_ff @(posedge cg_clk) if (cg_we) cgrom[cg_addr] <= cg_data;

    // ------------------------------------------------------------ timing
    logic [10:0] x;
    logic [9:0]  y;
    logic        hb, vb, hsy, vsy;
    synthframe_vtiming timing (.clk, .x, .y, .hblank(hb), .vblank(vb), .hsync(hsy), .vsync(vsy));

    // ------------------------------------------------------------ the LCD
    logic [1:0] c_line;
    logic [5:0] c_col;
    logic [7:0] gcode, code_q;
    logic [2:0] grow;
    logic [4:0] gbits, rom_bits;
    logic       fetch, l_panel, l_dot, l_lit;
    // cell: the board's port, line = c_line[0], position = c_col + 20 * c_line[1]
    assign r_line = c_line[0];
    assign r_pos = 5'(c_col) + (c_line[1] ? 5'd20 : 5'd0);
    synthframe_charlcd #(.X0(X0), .Y0(Y0), .COLS(20), .LINES(4), .PW(PW), .PH(PH), .LEFT(10),
                         .TOP(10), .CHAR_P(31), .LINE_P(52), .DOT_W(5), .DOT_H(5), .DOT_PX(6),
                         .DOT_PY(6), .ROW7_GLYPH(1)) lcd (
        .clk, .x, .y,
        .cell_line(c_line), .cell_col(c_col), .cell_row(), .cell_code(r_code), .cell_cursor(1'b0),
        .glyph_code(gcode), .glyph_row(grow), .glyph_bits(gbits), .fetch(fetch),
        .panel(l_panel), .dot(l_dot), .lit(l_lit));
    // glyph: CGRAM for codes below 10 (the board's port at r_row, same cell), else the ROM
    assign r_row = grow;
    always_ff @(posedge clk) begin
        rom_bits <= cgrom[{gcode, 1'b0, grow}][4:0];
        code_q <= gcode;
    end
    assign gbits = code_q < 8'h10 ? r_cg_bits : rom_bits;

    // ------------------------------------------------------------ strip, LEDs, syncs
    logic [10:0] x0, x1, x2;
    logic [9:0]  y0, y1, y2;
    logic [2:0]  hs_q, vs_q, hb_q, vb_q;
    always_ff @(posedge clk) begin
        x0 <= x; x1 <= x0; x2 <= x1;
        y0 <= y; y1 <= y0; y2 <= y1;
        hs_q <= {hs_q[1:0], hsy}; vs_q <= {vs_q[1:0], vsy};
        hb_q <= {hb_q[1:0], hb};  vb_q <= {vb_q[1:0], vb};
    end
    logic        st_hit;
    logic [23:0] st_rgb;
    synthframe_status_strip #(.X0(16), .Y0(SY)) strip (
        .x(x2), .y(y2), .rom_ok, .loading, .running, .midi_led, .underrun_led, .underrun_seen,
        .lag_seen, .hit(st_hit), .rgb(st_rgb));
    // LEDs: five 40 x 16 boxes under the LCD
    wire        led_row = y2 >= 10'd380 && y2 < 10'd396;
    wire [10:0] lx = x2 - 11'd120;
    wire        led_hit = led_row && x2 >= 11'd120 && lx < 11'd560 && lx % 11'd112 < 11'd40;
    wire [2:0]  led_i = 3'(lx / 11'd112);

    always_ff @(posedge clk) begin
        hs <= !hs_q[2]; vs <= !vs_q[2]; hblank <= hb_q[2]; vblank <= vb_q[2];
        if (l_panel) begin
            if (l_lit && lcd_on) {r, g, b} <= 24'h102018;       // dot on
            else if (l_dot) {r, g, b} <= 24'h8cb4a0;            // dot off
            else {r, g, b} <= 24'h9cc4b0;                       // panel
        end else if (led_hit) begin
            {r, g, b} <= leds[led_i] ? 24'hff3020 : 24'h401008;
        end else if (st_hit) begin
            {r, g, b} <= st_rgb;
        end else
            {r, g, b} <= 24'h202428;
    end
endmodule
