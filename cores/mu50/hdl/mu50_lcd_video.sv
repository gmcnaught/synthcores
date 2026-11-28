// SPDX-License-Identifier: MIT
// 800 x 600 at 60 Hz (40 MHz pixel clock): the MU50's LCD (HD44780, two lines) with MAME's
// mulcd cell layout (below); under it the front panel (MAME's mu50.lay,
// from cfg/panel.yaml, drawn by synthframe_panel_art from the ROMs in hdl/panel/ that
// modules/synthframe/tools/panel_art.py makes) with its five LEDs lit and its buttons shown
// pressed; and the status strip of the other synthframe cores (state band, MIDI, underrun, lag).
//
// Characters come from the board's HD44780 render port (line 0/1, position 0-23, display shift
// applied by the controller); codes 00-0F from its
// character-generator RAM, the others from the HD44780's ROM B04 (16 bytes a code, row r at
// code * 16 + r), loaded from the ROM pack. The render port, the LEDs and the keys are read from
// the core clock without synchronisation (display only), as the JV-880 core does.
module mu50_lcd_video #(
    parameter PANEL_DIR = "../hdl/panel/"   // Quartus: relative to mister/
) (
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
    input  logic [4:0]  leds,             // MAME mulcd LED0-LED4
    input  logic [14:0] keys,             // mu50_board's keys (1 = pressed)
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
    localparam int X0 = 56, Y0 = 224, PW = 387, PH = 117;     // the panel's LCD window
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
    // MAME's mulcd cell layout (the dots of its LCD SVG, 2.39 pixels an SVG unit in the panel's
    // window, rounded to whole pixels; origin LX, LY):
    //  main   17 x 2 characters, line 0 above line 1 with no gap: positions 0-16; character pitch
    //         21, dot pitch 4 (3 x 3 dots), row pitch 4
    //  small  one row of five characters at y 82: line 0 positions 17, 18, line 1 positions
    //         17-19, at x 0, 20, 42, 61, 81; dot pitch 3 (2 x 2 dots)
    //  block  16 x 16 dots, no gaps, at x 105, y 78: positions 20-23 (23: its left column only),
    //         line 0 above line 1; dot pitch 3 x 2 (2 x 1 dots)
    // Other DDRAM positions are not shown, as in MAME. Four stages: decode (x, y -> region,
    // column, row), cell (line, position, row, dot), character (board port, glyph lookup), dot;
    // the panel art, status strip and syncs are delayed one clock to match.
    localparam int LX = X0 + 11, LY = Y0 + 2;
    wire        in_xy = x >= 11'(LX) && y >= 10'(LY);
    wire [10:0] dx = x - 11'(LX);
    wire [9:0]  dy = y - 10'(LY);
    // main
    wire [10:0] m_col = dx / 11'd21, m_px = dx % 11'd21;
    wire [9:0]  m_row = dy / 10'd4, m_py = dy % 10'd4;
    wire        m_hit = m_col < 11'd17 && m_px < 11'd20 && m_px % 11'd4 < 11'd3 && m_row < 10'd16
                        && m_py < 10'd3;
    // small
    wire [9:0]  s_y = dy - 10'd82, s_row = s_y / 10'd3, s_py = s_y % 10'd3;
    wire        s_rows = dy >= 10'd82 && s_row < 10'd8 && s_py < 10'd2;
    logic       s_hit;
    logic [2:0] s_cell, s_dot;
    always_comb begin
        s_hit = 1'b0; s_cell = '0; s_dot = '0;
        for (int i = 0; i < 5; i++) begin
            automatic logic [10:0] o = i == 0 ? 11'd0 : i == 1 ? 11'd20 : i == 2 ? 11'd42 :
                                       i == 3 ? 11'd61 : 11'd81;
            automatic logic [10:0] px = dx - o;
            if (dx >= o && px < 11'd15 && px % 11'd3 < 11'd2) begin
                s_hit = s_rows; s_cell = 3'(i); s_dot = 3'(px / 11'd3);
            end
        end
    end
    // block
    wire [9:0]  b_y = dy - 10'd78, b_row = b_y / 10'd2;
    wire [10:0] b_x = dx - 11'd105, b_c = b_x / 11'd3;
    wire        b_hit = dy >= 10'd78 && b_row < 10'd16 && !b_y[0]
                        && dx >= 11'd105 && b_c < 11'd16 && b_x % 11'd3 < 11'd2;
    // stage 1: decode
    logic       d_panel, d_m, d_s, d_b;
    logic [4:0] d_mcol;
    logic [3:0] d_mrow, d_brow, d_bc;
    logic [2:0] d_mdot, d_scell, d_sdot, d_srow;
    always_ff @(posedge clk) begin
        d_panel <= x >= 11'(X0) && x < 11'(X0 + PW) && y >= 10'(Y0) && y < 10'(Y0 + PH);
        d_m <= in_xy && m_hit; d_s <= in_xy && s_hit; d_b <= in_xy && b_hit;
        d_mcol <= 5'(m_col); d_mrow <= 4'(m_row); d_mdot <= 3'(m_px / 11'd4);
        d_scell <= s_cell; d_sdot <= s_dot; d_srow <= 3'(s_row);
        d_brow <= 4'(b_row); d_bc <= 4'(b_c);
    end
    // stage 2: the cell
    logic       a_hit, a_line, a_panel;
    logic [4:0] a_pos;
    logic [2:0] a_row, a_dot;
    always_ff @(posedge clk) begin
        a_panel <= d_panel;
        a_hit <= d_m || d_s || d_b;
        if (d_m) begin
            a_line <= d_mrow[3]; a_pos <= d_mcol; a_row <= d_mrow[2:0]; a_dot <= d_mdot;
        end else if (d_s) begin
            a_line <= d_scell >= 3'd2;
            a_pos <= d_scell == 3'd0 ? 5'd17 : d_scell == 3'd1 ? 5'd18 : 5'd15 + 5'(d_scell);
            a_row <= d_srow; a_dot <= d_sdot;
        end else begin
            a_line <= d_brow[3]; a_pos <= 5'd20 + 5'(d_bc / 4'd5); a_row <= d_brow[2:0];
            a_dot <= 3'(d_bc % 4'd5);
        end
    end
    // stage 3: the character, from the board's port; CGRAM for codes below 10, else the ROM
    assign r_line = a_line;
    assign r_pos = a_pos;
    assign r_row = a_row;
    logic       c_hit, c_panel, c_cg;
    logic [2:0] c_dot;
    logic [4:0] rom_bits, cg_bits;
    always_ff @(posedge clk) begin
        rom_bits <= cgrom[{r_code, 1'b0, a_row}][4:0];
        cg_bits <= r_cg_bits;
        c_cg <= r_code < 8'h10;
        c_hit <= a_hit; c_panel <= a_panel; c_dot <= a_dot;
    end
    // stage 4: the dot (bit 4 the leftmost)
    wire [4:0]  gbits = c_cg ? cg_bits : rom_bits;
    logic       l_panel, l_lit;
    always_ff @(posedge clk) begin
        l_panel <= c_panel;
        l_lit <= c_hit && gbits[3'd4 - c_dot];
    end

    // ------------------------------------------------------------ strip, syncs
    logic [10:0] x0, x1, x2, x3;
    logic [9:0]  y0, y1, y2, y3;
    logic [3:0]  hs_q, vs_q, hb_q, vb_q;
    always_ff @(posedge clk) begin
        x0 <= x; x1 <= x0; x2 <= x1; x3 <= x2;
        y0 <= y; y1 <= y0; y2 <= y1; y3 <= y2;
        hs_q <= {hs_q[2:0], hsy}; vs_q <= {vs_q[2:0], vsy};
        hb_q <= {hb_q[2:0], hb};  vb_q <= {vb_q[2:0], vb};
    end
    logic        st_hit;
    logic [23:0] st_rgb;
    synthframe_status_strip #(.X0(16), .Y0(SY)) strip (
        .x(x3), .y(y3), .rom_ok, .loading, .running, .midi_led, .underrun_led, .underrun_seen,
        .lag_seen, .hit(st_hit), .rgb(st_rgb));
    // ------------------------------------------------------------ front panel, 3 clocks from x0
    `include "mu50_panel.svh"
    logic [2**PANEL_EB-1:0] panel_state;
    `include "mu50_panel_state.svh"
    generate if (PANEL_NE + 1 < 2**PANEL_EB) begin : gen_state_pad
        assign panel_state[2**PANEL_EB-1:PANEL_NE+1] = '0;
    end
    endgenerate
    logic [23:0] art;
    synthframe_panel_art #(.W(PANEL_W), .H(PANEL_H), .BPP(PANEL_BPP), .TB(PANEL_TB), .EB(PANEL_EB),
                           .NT(PANEL_NT), .PAL(PANEL_PAL), .LIT(PANEL_LIT),
                           .MAP_HEX({PANEL_DIR, "mu50_panel_map.hex"}),
                           .TILE_HEX({PANEL_DIR, "mu50_panel_tiles.hex"})) panel_art (
        .clk, .ce(1'b1), .x(x0), .y(y0), .state(panel_state), .rgb(art));

    always_ff @(posedge clk) begin
        hs <= !hs_q[3]; vs <= !vs_q[3]; hblank <= hb_q[3]; vblank <= vb_q[3];
        if (l_panel) begin
            // MAME's mulcd.lay: the yellow-green backlight; unlit dots not drawn
            if (l_lit && lcd_on) {r, g, b} <= 24'h2a4010;       // dot on
            else {r, g, b} <= 24'hb1fe32;                       // panel
        end else if (st_hit) begin
            {r, g, b} <= st_rgb;
        end else
            {r, g, b} <= art;
    end
endmodule
