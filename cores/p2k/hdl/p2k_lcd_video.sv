// SPDX-License-Identifier: GPL-3.0-or-later
// Proteus 2000 LCD on an 800 x 600 / 60 Hz raster (40.09 MHz pixel clock), drawn as MAME's
// proteus2k screen (emup2k.cpp: 2 lines of 24 characters, 6 x 8 pixels a character, dots x < 5,
// palette 131,136,139 / 92,83,88) at 5 x 5 per MAME pixel. HD44780 render() as MAME's: CGRAM for
// codes below 0x10, else the character ROM (hd44780_a00.bin from the system pack); cursor
// underline on row 7; blink fills the cell every other 102,400 LCD clocks. Around it: the front
// panel (cfg/panel.yaml, through synthframe_panel_art and the ROMs in hdl/panel/ that
// modules/synthframe/tools/panel_art.py makes) with its LEDs lit (latches 0x900000 / 0xA00000)
// and its buttons shown pressed. Below the panel: the output level (peak of |L|, |R|, 1 pixel per
// 64 / 32768 of full scale, decaying; the smoke test reads it), the diagnostics line and the
// status strip.
// Adapted from cores/cz101/hdl/cz101_lcd_video.sv (MIT), built from synthframe's panel modules.
// The LCD state lives in the CPU clock domain and is read here asynchronously (display only).
module p2k_lcd_video #(
    parameter PANEL_DIR = "../hdl/panel/"   // Quartus: relative to mister/
) (
    input  logic       clk,              // pixel clock
    input  logic       clk_sys,
    input  logic       font_we,
    input  logic [11:0] font_addr,
    input  logic [7:0] font_data,
    output logic       r_line,
    output logic [4:0] r_pos,
    output logic [2:0] r_row,
    input  logic [7:0] r_code,
    input  logic       r_at_ac,
    input  logic [4:0] r_cg_bits,
    input  logic       display_on,
    input  logic       cursor_on,
    input  logic       blink_on,
    input  logic [15:0] leds,            // low byte 0x900000, high byte 0xA00000
    input  logic [15:7] buttons,         // p2k_kbd's panel buttons, 1 = pressed
    input  logic [14:0] level,           // output peak (any clock; display only)
    input  logic       rom_ok,
    input  logic       loading,
    input  logic       running,
    input  logic       midi_led,
    input  logic       late_seen,
    input  logic       diag,             // show the diagnostics line
    input  logic [95:0] diag_hex,        // 24 hex digits, digit 0 leftmost in bits 95:92
    output logic       hs,
    output logic       vs,
    output logic       hblank,
    output logic       vblank,
    output logic [7:0] r,
    output logic [7:0] g,
    output logic [7:0] b
);
    localparam int PW = 24 * 30 + 20, PH = 2 * 40 + 20;
    localparam int X0 = 30, Y0 = 56;             // the lcd rect of cfg/panel.yaml
    localparam int MY = 402;                     // level meter
    localparam int DY = 440;                     // diagnostics line
    localparam int SY = 556;                     // status strip

    logic [7:0] font [0:4095];
    always_ff @(posedge clk_sys) if (font_we) font[font_addr] <= font_data;

    logic [23:0] bcnt;
    logic        blink;
    always_ff @(posedge clk) begin
        if (bcnt == 24'd15204800) begin bcnt <= 24'd0; blink <= !blink; end
        else bcnt <= bcnt + 24'd1;
    end

    logic [10:0] x;
    logic [9:0]  y;
    logic        hb, vb, hsy, vsy;
    synthframe_vtiming tm (.clk, .x, .y, .hblank(hb), .vblank(vb), .hsync(hsy), .vsync(vsy));

    logic [1:0]  c_line;
    logic [5:0]  c_col;
    logic [2:0]  c_row;
    logic [4:0]  gbits;
    logic        at_cursor, panel, dot, lit;
    synthframe_charlcd #(.X0(X0), .Y0(Y0), .COLS(24), .LINES(2), .PW(PW), .PH(PH),
                         .LEFT(10), .TOP(10), .CHAR_P(30), .LINE_P(40), .DOT_W(4), .DOT_H(4),
                         .DOT_PX(5), .DOT_PY(5), .ROW7_GLYPH(1)) lcd (
        .clk, .x, .y,
        .cell_line(c_line), .cell_col(c_col), .cell_row(c_row), .cell_code(r_code),
        .cell_cursor(at_cursor), .glyph_code(), .glyph_row(), .glyph_bits(gbits), .fetch(),
        .panel, .dot, .lit);
    assign r_line = c_line[0];
    assign r_pos = c_col[4:0];
    assign r_row = c_row;
    assign at_cursor = r_at_ac && cursor_on && display_on;

    // diagnostics line: 24 hex digits through a second renderer and font copy
    logic [7:0] dfont [0:4095];
    always_ff @(posedge clk_sys) if (font_we) dfont[font_addr] <= font_data;
    logic [1:0]  d_line;
    logic [5:0]  d_col;
    logic [2:0]  d_row;
    logic        d_panel, d_dot, d_lit;
    logic [7:0]  d_code, dfont_q;
    logic [95:0] diag_q;
    always_ff @(posedge clk) diag_q <= diag_hex;
    always_comb begin
        automatic logic [3:0] nib = diag_q[95 - 4 * d_col[4:0] -: 4];
        d_code = nib < 4'd10 ? 8'h30 + 8'(nib) : 8'h37 + 8'(nib);
    end
    always_ff @(posedge clk) dfont_q <= dfont[{d_code, 1'b0, d_row}];
    synthframe_charlcd #(.X0(X0), .Y0(DY), .COLS(24), .LINES(1), .PW(24 * 18 + 8), .PH(24),
                         .LEFT(4), .TOP(4), .CHAR_P(18), .LINE_P(24), .DOT_W(2), .DOT_H(2),
                         .DOT_PX(3), .DOT_PY(2), .ROW7_GLYPH(1)) dlcd (
        .clk, .x, .y,
        .cell_line(d_line), .cell_col(d_col), .cell_row(d_row), .cell_code(d_code),
        .cell_cursor(1'b0), .glyph_code(), .glyph_row(), .glyph_bits(dfont_q[4:0]), .fetch(),
        .panel(d_panel), .dot(d_dot), .lit(d_lit));

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

    logic [2:0]  hs_q, vs_q, hb_q, vb_q;
    logic [10:0] x0, x1, x2;
    logic [9:0]  y0, y1, y2;
    always_ff @(posedge clk) begin
        hs_q <= {hs_q[1:0], hsy}; vs_q <= {vs_q[1:0], vsy};
        hb_q <= {hb_q[1:0], hb};  vb_q <= {vb_q[1:0], vb};
        x0 <= x; x1 <= x0; x2 <= x1; y0 <= y; y1 <= y0; y2 <= y1;
    end

    logic        st_hit;
    logic [23:0] st_rgb;
    synthframe_status_strip #(.X0(X0), .Y0(SY)) strip (
        .x(x2), .y(y2), .rom_ok, .loading, .running, .midi_led,
        .underrun_led(1'b0), .underrun_seen(late_seen), .lag_seen(1'b0), .hit(st_hit), .rgb(st_rgb));

    // front panel, 3 clocks (as x2 / y2)
    `include "p2k_panel.svh"
    logic [2**PANEL_EB-1:0] panel_state;
    `include "p2k_panel_state.svh"
    generate if (PANEL_NE + 1 < 2**PANEL_EB)
        assign panel_state[2**PANEL_EB-1:PANEL_NE+1] = '0;
    endgenerate
    logic [23:0] art;
    synthframe_panel_art #(.W(PANEL_W), .H(PANEL_H), .BPP(PANEL_BPP), .TB(PANEL_TB), .EB(PANEL_EB),
                           .NT(PANEL_NT), .PAL(PANEL_PAL), .LIT(PANEL_LIT),
                           .MAP_HEX({PANEL_DIR, "p2k_panel_map.hex"}),
                           .TILE_HEX({PANEL_DIR, "p2k_panel_tiles.hex"})) panel_art (
        .clk, .ce(1'b1), .x, .y, .state(panel_state), .rgb(art));

    wire [10:0] lxo = x2 - 11'(X0);
    wire        in_meter = y2 >= 10'(MY) && y2 < 10'(MY + 12) && x2 >= 11'(X0) && x2 < 11'(X0 + 512);
    logic [14:0] level_q;
    always_ff @(posedge clk) level_q <= level;
    always_ff @(posedge clk) begin
        hs <= !hs_q[2]; vs <= !vs_q[2]; hblank <= hb_q[2]; vblank <= vb_q[2];
        if (panel) begin
            if (dot) {r, g, b} <= lit ? 24'h5C5358 : 24'h83888B;   // MAME's pens 1 / 0
            else {r, g, b} <= 24'h8A9294;
        end else if (in_meter) begin
            {r, g, b} <= (lxo < 11'(level_q[14:6])) ? 24'h40E060 : 24'h203028;
        end else if (diag && d_panel) begin
            {r, g, b} <= (d_dot && d_lit) ? 24'hE0E0E0 : 24'h202838;
        end else if (st_hit) begin
            {r, g, b} <= st_rgb;
        end else {r, g, b} <= art;
    end
endmodule
