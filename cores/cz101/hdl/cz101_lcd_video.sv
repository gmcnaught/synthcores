// SPDX-License-Identifier: MIT
// CZ-101 LCD on an 800 x 600 / 60 Hz raster (40 MHz pixel clock), drawn as MAME's cz101
// screen (97 x 19 pixels, cz101_palette, lcd_pixel_update; BSD-3-Clause) at 8 x 8 per pixel:
// character `pos` of `line` occupies MAME pixels x = 1 + 6 pos .. + 4, y = 1 + 9 line .. + 7.
// HD44780 render() as MAME's: CGRAM for codes below 0x10, else the character ROM
// (hd44780_a00.bin, 16 bytes a code, loaded from the ROM pack); cursor underline on row 7;
// blink fills the cell every other 102,400 LCD clocks; nothing while the display is off or
// the power is off. Below: the 16 panel LEDs and the status strip.
//
// Built from synthframe's panel modules: synthframe_vtiming, synthframe_charlcd (each MAME
// pixel is a 7 x 7 dot on an 8-pixel pitch) and synthframe_status_strip (running: a ROM pack
// and not loading; no underrun-now or lag source).
//
// The LCD state lives in the core clock domain and is read here asynchronously (display only).

module cz101_lcd_video (
    input  logic       clk,              // pixel clock, 40 MHz
    // character ROM load (core clock)
    input  logic       clk_sys,
    input  logic       font_we,
    input  logic [11:0] font_addr,
    input  logic [7:0] font_data,
    // LCD read port (combinational, in the core domain)
    output logic       r_line,
    output logic [3:0] r_pos,
    output logic [2:0] r_row,
    input  logic [7:0] r_code,
    input  logic       r_at_ac,
    input  logic [4:0] r_cg_bits,
    input  logic       display_on,
    input  logic       cursor_on,
    input  logic       blink_on,
    input  logic       power,
    input  logic [15:0] leds,
    // status
    input  logic       rom_ok,
    input  logic       loading,
    input  logic       midi_led,
    input  logic       underrun_seen,
    output logic       hs,
    output logic       vs,
    output logic       hblank,
    output logic       vblank,
    output logic [7:0] r,
    output logic [7:0] g,
    output logic [7:0] b
);
    localparam int X0 = 12, Y0 = 160;            // MAME pixel (0, 0); 97 x 19 pixels of 8 x 8
    localparam int LY = Y0 + 19 * 8 + 40;        // LED row
    localparam int SY = LY + 60;                 // status strip

    // ------------------------------------------------------------ character ROM
    logic [7:0] font [0:4095];
    always_ff @(posedge clk_sys) if (font_we) font[font_addr] <= font_data;

    // ------------------------------------------------------------ blink (102,400 / 270 kHz)
    logic [23:0] bcnt;
    logic        blink;
    always_ff @(posedge clk) begin
        if (bcnt == 24'd15170369) begin bcnt <= 24'd0; blink <= !blink; end
        else bcnt <= bcnt + 24'd1;
    end

    // ------------------------------------------------------------ timing
    logic [10:0] x;
    logic [9:0]  y;
    logic        hb, vb, hsy, vsy;
    synthframe_vtiming tm (.clk, .x, .y, .hblank(hb), .vblank(vb), .hsync(hsy), .vsync(vsy));

    // ------------------------------------------------------------ LCD dots
    logic [1:0]  c_line;
    logic [5:0]  c_col;
    logic [2:0]  c_row;
    logic [4:0]  gbits;
    logic        at_cursor, panel, dot, lit;
    synthframe_charlcd #(.X0(X0), .Y0(Y0), .COLS(16), .LINES(2), .PW(97 * 8), .PH(19 * 8),
                         .LEFT(8), .TOP(8), .CHAR_P(48), .LINE_P(72), .DOT_W(7), .DOT_H(7),
                         .DOT_PX(8), .DOT_PY(8), .ROW7_GLYPH(1)) lcd (
        .clk, .x, .y,
        .cell_line(c_line), .cell_col(c_col), .cell_row(c_row), .cell_code(r_code),
        .cell_cursor(at_cursor), .glyph_code(), .glyph_row(), .glyph_bits(gbits), .fetch(),
        .panel, .dot, .lit);
    assign r_line = c_line[0];
    assign r_pos = c_col[3:0];
    assign r_row = c_row;
    assign at_cursor = r_at_ac && cursor_on && display_on;      // underline (row 7)

    // the glyph row, looked up with the character code (font RAM read, CG RAM, blink fill)
    logic       cg1, fill1;
    logic [4:0] cgb1;
    logic [7:0] font_q;
    always_ff @(posedge clk) begin
        font_q <= font[{r_code, 1'b0, c_row}];
        cg1 <= r_code < 8'h10;
        cgb1 <= r_cg_bits;
        fill1 <= r_at_ac && blink_on && !blink;
    end
    assign gbits = !display_on ? 5'h00 : fill1 ? 5'h1F : cg1 ? cgb1 : font_q[4:0];

    // ------------------------------------------------------------ sync and position, 3 clocks
    logic [2:0]  hs_q, vs_q, hb_q, vb_q;
    logic [10:0] x0, x1, x2;
    logic [9:0]  y0, y1, y2;
    always_ff @(posedge clk) begin
        hs_q <= {hs_q[1:0], hsy}; vs_q <= {vs_q[1:0], vsy};
        hb_q <= {hb_q[1:0], hb};  vb_q <= {vb_q[1:0], vb};
        x0 <= x; x1 <= x0; x2 <= x1; y0 <= y; y1 <= y0; y2 <= y1;
    end

    // ------------------------------------------------------------ status strip
    logic        st_hit;
    logic [23:0] st_rgb;
    synthframe_status_strip #(.X0(X0), .Y0(SY)) strip (
        .x(x2), .y(y2), .rom_ok, .loading, .running(rom_ok && !loading), .midi_led,
        .underrun_led(1'b0), .underrun_seen, .lag_seen(1'b0), .hit(st_hit), .rgb(st_rgb));

    // ------------------------------------------------------------ colour
    wire        in_led = y2 >= 10'(LY) && y2 < 10'(LY + 24) && x2 >= 11'(X0) && x2 < 11'(X0 + 16 * 48);
    wire [10:0] lxo = x2 - 11'(X0);
    wire [3:0]  led_i = 4'(lxo / 11'd48);
    always_ff @(posedge clk) begin
        hs <= !hs_q[2]; vs <= !vs_q[2]; hblank <= hb_q[2]; vblank <= vb_q[2];
        if (panel) begin
            if (dot && power) {r, g, b} <= lit ? 24'h3F3B3E : 24'h83888B;   // on / off
            else {r, g, b} <= 24'h8A9294;                                  // background
        end else if (in_led && (lxo % 11'd48) < 11'd24) begin
            {r, g, b} <= leds[led_i] ? 24'hFF3020 : 24'h402018;
        end else if (st_hit) begin
            {r, g, b} <= st_rgb;
        end else {r, g, b} <= 24'h101828;
    end
endmodule
