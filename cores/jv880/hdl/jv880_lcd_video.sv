// SPDX-License-Identifier: GPL-2.0-or-later
// 800 x 600 at 60 Hz (40 MHz pixel clock): the JV-880's 2 x 24 character LCD, as the reference
// draws it (lcd.cpp LCD_Render, jv880: characters of 5 x 7 dots plus the cursor row; text from
// the controller's display RAM, codes below 16 from its character-generator RAM; the cursor as an
// underline when on), scaled to fit: dots of 4 x 4 pixels on a 5-pixel pitch, characters on a
// 32-pixel pitch, the two rows 50 pixels apart, the panel at (16, 196). Below it the status
// strip of the SC-55 core (state band, MIDI, underrun, lag), same colours, so the MiSTer smoke
// tests read both alike. Reads the LCD controller's state from the core clock without
// synchronisation (display only).
module jv880_lcd_video (
    input  logic       clk,              // pixel clock
    input  logic [7:0] dd [80],
    input  logic [4:0] cg [64],
    input  logic [6:0] dd_ram,           // cursor address (lcd_ctrl)
    input  logic       cursor_on,        // display control C
    // status strip
    input  logic       rom_ok,
    input  logic       loading,
    input  logic       running,
    input  logic       midi_led,
    input  logic       underrun_led,
    input  logic       underrun_seen,
    input  logic       lag_seen,
    output logic       hs,
    output logic       vs,
    output logic       hblank,
    output logic       vblank,
    output logic [7:0] r,
    output logic [7:0] g,
    output logic [7:0] b
);
    localparam int X0 = 16, Y0 = 196, PW = 768, PH = 104;     // the LCD panel
    localparam int SY = Y0 + 144;                             // the status strip, 20 lines

    function automatic logic [4:0] font_row(input logic [7:0] fc, input logic [3:0] fr);
        `include "lcd_font.svh"
    endfunction

    // ------------------------------------------------------------ timing (as the SC-55's lcd_video)
    logic [10:0] x;
    logic [9:0]  y;
    always_ff @(posedge clk) begin
        if (x == 11'd1055) begin
            x <= 11'd0;
            y <= (y == 10'd627) ? 10'd0 : y + 10'd1;
        end else x <= x + 11'd1;
    end

    // ------------------------------------------------------------ stage 0: cell and dot
    // x: character j = (x - X0) / 32, dot column (offset / 5, on when offset % 5 < 4 and < 25)
    // y: row i = (y - Y0 - 4) / 50, dot row (offset / 5, on when offset % 5 < 4 and < 40)
    logic        in_panel0, dot_x0, dot_y0, hs0, vs0, hb0, vb0;
    logic [4:0]  j0;
    logic        i0;
    logic [2:0]  dc0;
    logic [2:0]  dr0;
    logic [10:0] x0;
    logic [9:0]  y0;
    always_ff @(posedge clk) begin : stage0
        logic [10:0] lx;
        logic [9:0]  ly, oy;
        logic [4:0]  ox;
        lx = x - 11'(X0);
        ly = y - 10'(Y0 + 4);
        ox = lx[4:0];
        oy = (ly >= 10'd50) ? ly - 10'd50 : ly;
        x0 <= x; y0 <= y;
        in_panel0 <= x >= 11'(X0) && x < 11'(X0 + PW) && y >= 10'(Y0) && y < 10'(Y0 + PH);
        j0 <= lx[9:5];
        i0 <= ly >= 10'd50;
        dc0 <= 3'(ox / 5'd5);
        dot_x0 <= x >= 11'(X0) && ox < 5'd25 && (ox % 5'd5) < 5'd4;
        dr0 <= 3'(oy / 10'd5);
        dot_y0 <= y >= 10'(Y0 + 4) && ly < 10'd100 && oy < 10'd40 && (oy % 10'd5) < 10'd4;
        hb0 <= x >= 11'd800;
        vb0 <= y >= 10'd600;
        hs0 <= (x >= 11'd840 && x < 11'd968);
        vs0 <= (y >= 10'd601 && y < 10'd605);
    end

    // ------------------------------------------------------------ stage 1: character code
    logic [7:0]  ch1;
    logic        cur1, in_panel1, dot1, hs1, vs1, hb1, vb1;
    logic [2:0]  dc1, dr1;
    logic [10:0] x1;
    logic [9:0]  y1;
    always_ff @(posedge clk) begin
        ch1 <= dd[7'(i0) * 7'd40 + 7'(j0)];
        cur1 <= cursor_on && dd_ram[6] == i0 && dd_ram[5:0] == 6'(j0) && dr0 == 3'd7;
        in_panel1 <= in_panel0; dot1 <= in_panel0 && dot_x0 && dot_y0 && j0 < 5'd24;
        dc1 <= dc0; dr1 <= dr0;
        hs1 <= hs0; vs1 <= vs0; hb1 <= hb0; vb1 <= vb0; x1 <= x0; y1 <= y0;
    end

    // ------------------------------------------------------------ stage 2: font row
    logic [4:0]  row2;
    logic        cur2, in_panel2, dot2, hs2, vs2, hb2, vb2;
    logic [2:0]  dc2;
    logic [10:0] x2;
    logic [9:0]  y2;
    always_ff @(posedge clk) begin
        row2 <= (dr1 == 3'd7) ? 5'h00 : (ch1 >= 8'd16) ? font_row(ch1 - 8'd16, {1'b0, dr1}) : cg[{ch1[2:0], dr1}];
        cur2 <= cur1; in_panel2 <= in_panel1; dot2 <= dot1; dc2 <= dc1;
        hs2 <= hs1; vs2 <= vs1; hb2 <= hb1; vb2 <= vb1; x2 <= x1; y2 <= y1;
    end

    // ------------------------------------------------------------ stage 3: colour
    logic [6:0] st;
    always_ff @(posedge clk) begin
        st <= {lag_seen, underrun_seen, underrun_led, midi_led, running, loading, rom_ok};
        hs <= !hs2; vs <= !vs2; hblank <= hb2; vblank <= vb2;
        if (in_panel2) begin
            if (dot2 && (row2[3'd4 - dc2] || cur2)) {r, g, b} <= 24'h0a1404;   // dot on
            else if (dot2) {r, g, b} <= 24'h6aa500;                           // dot off
            else {r, g, b} <= 24'h78b500;                                      // panel (lcd.cpp color2)
        end else begin
            {r, g, b} <= 24'h101828;
            if (y2 >= 10'(SY) && y2 < 10'(SY + 20) && x2 >= 11'(X0) && x2 < 11'(X0 + 200))
                {r, g, b} <= st[2] ? 24'h20c040 : st[1] ? 24'he0a020 : st[0] ? 24'h2060c0 : 24'hc02020;
            if (y2 >= 10'(SY) && y2 < 10'(SY + 20) && x2 >= 11'(X0 + 220) && x2 < 11'(X0 + 260))
                {r, g, b} <= st[3] ? 24'hffffff : 24'h303848;
            if (y2 >= 10'(SY) && y2 < 10'(SY + 20) && x2 >= 11'(X0 + 280) && x2 < 11'(X0 + 320))
                {r, g, b} <= st[4] ? 24'hff4040 : st[5] ? 24'h902020 : 24'h303848;
            if (y2 >= 10'(SY) && y2 < 10'(SY + 20) && x2 >= 11'(X0 + 340) && x2 < 11'(X0 + 380))
                {r, g, b} <= st[6] ? 24'he0a020 : 24'h303848;
        end
    end
endmodule
