// SPDX-License-Identifier: GPL-2.0-or-later
// MT-32 LCD on an 800 x 600 screen: 20 characters of 5 x 8 dots at 6 x 6 pixels, the font of
// Nuked-MT32 (lcd_font.h -> mt32_lcd_font.hex, tools/mkfont.py), drawn as lcd.cpp does: rows
// 0-6 from the font, row 7 only as the cursor line. A synthframe status strip sits above it.
module mt32_lcd_video #(
    parameter FONT_HEX = "mt32_lcd_font.hex"
) (
    input  logic        clk,           // pixel clock (40 MHz)
    // LCD state (clk_sys domain, read asynchronously: it changes rarely)
    output logic [4:0]  text_addr,
    input  logic [7:0]  text,
    output logic [4:0]  cg_addr,
    input  logic [4:0]  cg_row,
    input  logic [4:0]  cursor,
    input  logic        cursor_on,
    input  logic        display_on,
    // status strip
    input  logic        rom_ok,
    input  logic        loading,
    input  logic        running,
    input  logic        midi_led,
    input  logic        underrun_seen,

    output logic        hs, vs, hblank, vblank,
    output logic [7:0]  r, g, b
);
    localparam int X0 = 40, Y0 = 260, CW = 36;   // 20 x 36 = 720 pixels wide, 8 x 6 = 48 high

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
        s_cur <= (ch == cursor) && cursor_on;
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
        on = s_cell && bits[3'd4 - s_col] && (s_row != 3'd7 || s_cur);
        s_dot <= s_in && on;
    end

    // status strip at the top
    logic        st_hit;
    logic [23:0] st_rgb;
    synthframe_status_strip #(.X0(40), .Y0(40)) strip (
        .x, .y, .rom_ok, .loading, .running, .midi_led, .underrun_led(1'b0),
        .underrun_seen, .lag_seen(1'b0), .hit(st_hit), .rgb(st_rgb)
    );

    // two-clock delay of the timing to match the pixel pipeline
    logic [1:0] d_hb, d_vb, d_hs, d_vs, d_lcd;
    logic [23:0] d_st0, d_st1;
    logic [1:0]  d_sth;
    always_ff @(posedge clk) begin
        d_hb <= {d_hb[0], hb}; d_vb <= {d_vb[0], vb};
        d_hs <= {d_hs[0], hsy}; d_vs <= {d_vs[0], vsy};
        d_lcd <= {d_lcd[0], in_lcd};
        d_sth <= {d_sth[0], st_hit}; d_st0 <= st_rgb; d_st1 <= d_st0;
    end
    assign hblank = d_hb[1];
    assign vblank = d_vb[1];
    assign hs = d_hs[1];
    assign vs = d_vs[1];
    always_comb begin
        {r, g, b} = 24'h000000;
        if (d_sth[1]) {r, g, b} = d_st1;
        else if (d_lcd[1]) {r, g, b} = !display_on ? 24'h102008 : s_dot ? 24'h10300c : 24'h6cb030;
    end
endmodule
