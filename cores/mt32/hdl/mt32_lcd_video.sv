// SPDX-License-Identifier: GPL-2.0-or-later
// MT-32 LCD on an 800 x 600 screen: 20 characters of 5 x 8 dots at 6 x 6 pixels, the font of
// Nuked-MT32 (lcd_font.h -> mt32_lcd_font.hex, tools/mkfont.py), drawn as lcd.cpp lcd_render()
// does: rows 0-6 from the font; row 7 only in the cursor cell, which is shown when cursor_on and
// cursor_blink are both set and is blank when cursor_font is set; no characters in 1-line mode.
// tests/test_lcd.py compares the dots with lcd_render(). Around it the front panel (cfg/panel.yaml,
// owner's manual p. 2: synthframe_panel_art from the ROMs in hdl/panel/ that
// modules/synthframe/tools/panel_art.py makes), its buttons shown pressed and the MIDI MESSAGE
// LED lit; the synthframe status strip at the bottom.
module mt32_lcd_video #(
    parameter FONT_HEX = "mt32_lcd_font.hex",
    parameter PANEL_DIR = "../hdl/panel/"   // Quartus: relative to mister/
) (
    input  logic        clk,           // pixel clock (40 MHz)
    // LCD state (clk_sys domain, read asynchronously: it changes rarely)
    output logic [4:0]  text_addr,
    input  logic [7:0]  text,
    output logic [4:0]  cg_addr,
    input  logic [4:0]  cg_row,
    input  logic [4:0]  cursor,
    input  logic        cursor_on,
    input  logic        cursor_blink,
    input  logic        cursor_font,
    input  logic        lines2,
    input  logic        display_on,
    // front panel (clk_sys domain, display only)
    input  logic [9:0]  buttons,       // MT32_BUTTTON_* order of Nuked-MT32's mt32.h
    input  logic        msg_led,       // MIDI MESSAGE LED
    // status strip
    input  logic        rom_ok,
    input  logic        loading,
    input  logic        running,
    input  logic        midi_led,
    input  logic        underrun_seen,

    output logic        hs, vs, hblank, vblank,
    output logic [7:0]  r, g, b
);
    localparam int X0 = 40, Y0 = 70, CW = 36;    // 20 x 36 = 720 pixels wide, 8 x 6 = 48 high
    localparam int SX = 40, SY = 560;            // status strip

    logic [7:0] font [1280];
    initial $readmemh(FONT_HEX, font);

    logic [10:0] x;
    logic [9:0]  y;
    logic hb, vb, hsy, vsy;
    synthframe_vtiming vt (.clk, .x, .y, .hblank(hb), .vblank(vb), .hsync(hsy), .vsync(vsy));

    // stage 1: which character, row, column
    logic        in_lcd;
    logic [4:0]  ch;
    logic [5:0]  cx;               // 0..35 within the cell
    logic [2:0]  row;
    logic [10:0] lx;
    logic [9:0]  ly;
    always_comb begin
        lx = x - 11'(X0);
        ly = y - 10'(Y0);
        in_lcd = (x >= 11'(X0)) && (lx < 11'(20 * CW)) && (y >= 10'(Y0)) && (ly < 10'd48);
        ch = 5'(lx / 11'(CW));
        cx = 6'(lx - 11'(ch) * 11'(CW));
        row = 3'(ly / 10'd6);
    end
    assign text_addr = ch;
    assign cg_addr = {text[1:0], row};

    logic        s_in, s_cur, s_dot, s_cell;
    logic [7:0]  s_code;
    logic [2:0]  s_row;
    logic [2:0]  s_col;
    logic [10:0] fa;
    always_ff @(posedge clk) begin
        s_in <= in_lcd && display_on;
        s_code <= text;
        s_row <= row;
        s_col <= 3'(cx / 6'd6);
        s_cell <= cx < 6'd30;      // 5 dots of 6 pixels, then a 6-pixel gap
        s_cur <= (ch == cursor) && cursor_on && cursor_blink;
        if (text < 8'h04)                         fa <= 11'h7ff;
        else if (text >= 8'h20 && text < 8'h80)   fa <= {text - 8'h20, row};
        else if (text >= 8'ha0 && text < 8'he0)   fa <= {text - 8'h40, row};
        else                                      fa <= {8'h00, row};
    end
    logic [4:0] bits;
    logic [4:0] s_cg;
    always_ff @(posedge clk) s_cg <= cg_row;
    always_comb bits = (fa == 11'h7ff) ? s_cg : font[fa][4:0];
    always_ff @(posedge clk) begin
        logic on;
        on = s_cell && lines2 && bits[3'd4 - s_col] && (s_row != 3'd7 || s_cur) && !(s_cur && cursor_font);
        s_dot <= s_in && on;
    end
    logic s_dot2;
    always_ff @(posedge clk) s_dot2 <= s_dot;

    // status strip at the bottom
    logic        st_hit;
    logic [23:0] st_rgb;
    synthframe_status_strip #(.X0(SX), .Y0(SY)) strip (
        .x, .y, .rom_ok, .loading, .running, .midi_led, .underrun_led(1'b0),
        .underrun_seen, .lag_seen(1'b0), .hit(st_hit), .rgb(st_rgb)
    );

    // front panel, 3 clocks
    `include "mt32_panel.svh"
    logic [2**PANEL_EB-1:0] panel_state;
    `include "mt32_panel_state.svh"
    generate if (PANEL_NE + 1 < 2**PANEL_EB)
        assign panel_state[2**PANEL_EB-1:PANEL_NE+1] = '0;
    endgenerate
    logic [23:0] art;
    synthframe_panel_art #(.W(PANEL_W), .H(PANEL_H), .BPP(PANEL_BPP), .TB(PANEL_TB), .EB(PANEL_EB),
                           .NT(PANEL_NT), .PAL(PANEL_PAL), .LIT(PANEL_LIT),
                           .MAP_HEX({PANEL_DIR, "mt32_panel_map.hex"}),
                           .TILE_HEX({PANEL_DIR, "mt32_panel_tiles.hex"})) panel_art (
        .clk, .ce(1'b1), .x, .y, .state(panel_state), .rgb(art));

    // three-clock delay of the timing to match the panel art (the LCD dot is ready after two)
    logic [2:0] d_hb, d_vb, d_hs, d_vs, d_lcd, d_sth;
    logic [23:0] d_st0, d_st1, d_st2;
    always_ff @(posedge clk) begin
        d_hb <= {d_hb[1:0], hb}; d_vb <= {d_vb[1:0], vb};
        d_hs <= {d_hs[1:0], hsy}; d_vs <= {d_vs[1:0], vsy};
        d_lcd <= {d_lcd[1:0], in_lcd};
        d_sth <= {d_sth[1:0], st_hit}; d_st0 <= st_rgb; d_st1 <= d_st0; d_st2 <= d_st1;
    end
    assign hblank = d_hb[2];
    assign vblank = d_vb[2];
    assign hs = d_hs[2];
    assign vs = d_vs[2];
    always_comb begin
        {r, g, b} = art;
        if (d_sth[2]) {r, g, b} = d_st2;
        else if (d_lcd[2]) {r, g, b} = !display_on ? 24'h102008 : s_dot2 ? 24'h10300c : 24'h6cb030;
    end
endmodule
