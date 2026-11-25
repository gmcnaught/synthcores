// SPDX-License-Identifier: GPL-2.0-or-later
// 800 x 600 at 60 Hz (40 MHz pixel clock): the JV-880's 2 x 24 character LCD, as the reference
// draws it (lcd.cpp LCD_Render, jv880: characters of 5 x 7 dots plus the cursor row; text from
// the controller's display RAM, codes below 16 from its character-generator RAM; the cursor as an
// underline when on), scaled to fit: dots of 4 x 4 pixels on a 5-pixel pitch, characters on a
// 32-pixel pitch, the two rows 50 pixels apart, the panel at (16, 50). Around it the front panel
// (cfg/panel.yaml, owner's manual pp. 9-10), its buttons shown pressed and its LEDs lit as the
// firmware drives them; at the bottom the status strip of the SC-55 core (state band, MIDI, underrun, lag),
// same colours, so the MiSTer smoke tests read both alike. Reads the LCD controller's state,
// the buttons and the LEDs from the core clock without synchronisation (display only).
//
// Built from synthframe's panel modules: synthframe_vtiming, synthframe_charlcd (the LCD and
// the debug line, sharing one glyph lookup), synthframe_panel_art (the panel, from the ROMs in
// hdl/panel/ that modules/synthframe/tools/panel_art.py makes) and synthframe_status_strip.
module jv880_lcd_video #(
    parameter PANEL_DIR = "../hdl/panel/"   // Quartus: relative to mister/
) (
    input  logic       clk,              // pixel clock
    input  logic [7:0] dd [80],
    input  logic [4:0] cg [64],
    input  logic [6:0] dd_ram,           // cursor address (lcd_ctrl)
    input  logic       cursor_on,        // display control C
    input  logic [13:0] buttons,         // jv880_kbd's buttons (mcu.h MCU_BUTTON_*), 1: pressed
    input  logic [9:0] leds,             // sc55_mcu jv_leds: the panel LEDs as the firmware drives them
    // status strip
    input  logic       rom_ok,
    input  logic       loading,
    input  logic       running,
    input  logic       midi_led,
    input  logic       underrun_led,
    input  logic       underrun_seen,
    input  logic       lag_seen,
    // debug line above the strip: 24 characters (font codes, as the LCD's)
    input  logic [7:0] dbg_ch [24],
    input  logic       dbg_on,
    output logic       hs,
    output logic       vs,
    output logic       hblank,
    output logic       vblank,
    output logic [7:0] r,
    output logic [7:0] g,
    output logic [7:0] b
);
    localparam int X0 = 16, Y0 = 50, PW = 768, PH = 104;      // the LCD panel (panel.yaml lcd)
    localparam int SY = 560;                                  // the status strip, 20 lines
    localparam int DY = 500;                                  // the debug line, 40 lines

    function automatic logic [4:0] font_row(input logic [7:0] fc, input logic [3:0] fr);
        `include "lcd_font.svh"
    endfunction

    // ------------------------------------------------------------ timing
    logic [10:0] x;
    logic [9:0]  y;
    logic        hb, vb, hsy, vsy;
    synthframe_vtiming timing (.clk, .x, .y, .hblank(hb), .vblank(vb), .hsync(hsy), .vsync(vsy));

    // ------------------------------------------------------------ the LCD and the debug line
    logic [1:0] l_line, d_line;
    logic [5:0] l_col, d_col;
    logic [7:0] l_code, d_code, l_gcode, d_gcode, gcode;
    logic [2:0] l_grow, d_grow, grow;
    logic [4:0] gbits;
    logic       l_fetch, d_fetch, l_panel, l_dot, l_lit, d_panel, d_dot, d_lit;

    assign l_code = dd[7'(l_line[0]) * 7'd40 + 7'(l_col)];
    synthframe_charlcd #(.X0(X0), .Y0(Y0), .COLS(24), .LINES(2), .PW(PW), .PH(PH), .TOP(4),
                         .CHAR_P(32), .LINE_P(50), .DOT_W(4), .DOT_H(4), .DOT_PX(5), .DOT_PY(5)) lcd (
        .clk, .x, .y,
        .cell_line(l_line), .cell_col(l_col), .cell_row(), .cell_code(l_code),
        .cell_cursor(cursor_on && dd_ram[6] == l_line[0] && dd_ram[5:0] == l_col),
        .glyph_code(l_gcode), .glyph_row(l_grow), .glyph_bits(gbits), .fetch(l_fetch),
        .panel(l_panel), .dot(l_dot), .lit(l_lit));

    logic [2:0] dbg_q;                   // dbg_on as the pipeline saw it at stage 0
    always_ff @(posedge clk) dbg_q <= {dbg_q[1:0], dbg_on};
    assign d_code = dbg_ch[d_col < 6'd24 ? 5'(d_col) : 5'd0];
    synthframe_charlcd #(.X0(X0), .Y0(DY), .COLS(24), .LINES(1), .PW(PW), .PH(40), .TOP(0),
                         .CHAR_P(32), .LINE_P(40), .DOT_W(4), .DOT_H(4), .DOT_PX(5), .DOT_PY(5)) dbg (
        .clk, .x, .y,
        .cell_line(d_line), .cell_col(d_col), .cell_row(), .cell_code(d_code), .cell_cursor(1'b0),
        .glyph_code(d_gcode), .glyph_row(d_grow), .glyph_bits(gbits), .fetch(d_fetch),
        .panel(d_panel), .dot(d_dot), .lit(d_lit));

    assign gcode = d_fetch ? d_gcode : l_gcode;
    assign grow  = d_fetch ? d_grow : l_grow;
    assign gbits = (gcode >= 8'd16) ? font_row(gcode - 8'd16, {1'b0, grow}) : cg[{gcode[2:0], grow}];

    // ------------------------------------------------------------ the strip, syncs: stages 0-2
    logic [10:0] x0, x1, x2;
    logic [9:0]  y0, y1, y2;
    logic [2:0]  hs_q, vs_q, hb_q, vb_q;
    always_ff @(posedge clk) begin
        x0 <= x; x1 <= x0; x2 <= x1;
        y0 <= y; y1 <= y0; y2 <= y1;
        hs_q <= {hs_q[1:0], hsy}; vs_q <= {vs_q[1:0], vsy};
        hb_q <= {hb_q[1:0], hb};  vb_q <= {vb_q[1:0], vb};
    end
    logic [6:0]  st;
    logic        st_hit;
    logic [23:0] st_rgb;
    synthframe_status_strip #(.X0(X0), .Y0(SY)) strip (
        .x(x2), .y(y2), .rom_ok(st[0]), .loading(st[1]), .running(st[2]), .midi_led(st[3]),
        .underrun_led(st[4]), .underrun_seen(st[5]), .lag_seen(st[6]), .hit(st_hit), .rgb(st_rgb));

    // ------------------------------------------------------------ front panel, 3 clocks
    /* verilator lint_off UNUSEDPARAM */
    `include "jv880_panel.svh"
    /* verilator lint_on UNUSEDPARAM */
    logic [2**PANEL_EB-1:0] panel_state;
    `include "jv880_panel_state.svh"
    generate if (PANEL_NE + 1 < 2**PANEL_EB) begin : g_state_pad
        assign panel_state[2**PANEL_EB-1:PANEL_NE+1] = '0;
    end endgenerate
    logic [23:0] art;
    synthframe_panel_art #(.W(PANEL_W), .H(PANEL_H), .BPP(PANEL_BPP), .TB(PANEL_TB), .EB(PANEL_EB),
                           .NT(PANEL_NT), .PAL(PANEL_PAL), .LIT(PANEL_LIT),
                           .MAP_HEX({PANEL_DIR, "jv880_panel_map.hex"}),
                           .TILE_HEX({PANEL_DIR, "jv880_panel_tiles.hex"})) panel_art (
        .clk, .ce(1'b1), .x, .y, .state(panel_state), .rgb(art));

    // ------------------------------------------------------------ stage 3: colour
    always_ff @(posedge clk) begin
        st <= {lag_seen, underrun_seen, underrun_led, midi_led, running, loading, rom_ok};
        hs <= !hs_q[2]; vs <= !vs_q[2]; hblank <= hb_q[2]; vblank <= vb_q[2];
        if (l_panel) begin
            if (l_lit) {r, g, b} <= 24'h0a1404;                    // dot on
            else if (l_dot) {r, g, b} <= 24'h6aa500;               // dot off
            else {r, g, b} <= 24'h78b500;                          // panel (lcd.cpp color2)
        end else begin
            {r, g, b} <= art;
            if (st_hit) {r, g, b} <= st_rgb;
            if (dbg_q[2] && d_lit) {r, g, b} <= 24'he0e0e0;
        end
    end
endmodule
