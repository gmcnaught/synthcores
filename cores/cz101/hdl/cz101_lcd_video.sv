// SPDX-License-Identifier: MIT
// CZ-101 LCD on an 800 x 600 / 60 Hz raster (40 MHz pixel clock), drawn as MAME's cz101
// screen (97 x 19 pixels, cz101_palette, lcd_pixel_update; BSD-3-Clause) at 8 x 8 per pixel:
// character `pos` of `line` occupies MAME pixels x = 1 + 6 pos .. + 4, y = 1 + 9 line .. + 7.
// HD44780 render() as MAME's: CGRAM for codes below 0x10, else the character ROM
// (hd44780_a00.bin, 16 bytes a code, loaded from the ROM pack); cursor underline on row 7;
// blink fills the cell every other 102,400 LCD clocks; nothing while the display is off or
// the power is off. Below: the 16 panel LEDs and a status strip.
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
    input  logic [7:0] r_code,
    input  logic       r_cursor,
    output logic [2:0] r_cg_code,
    output logic [2:0] r_cg_row,
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
    always_ff @(posedge clk) begin
        if (x == 11'd1055) begin
            x <= 11'd0;
            y <= (y == 10'd627) ? 10'd0 : y + 10'd1;
        end else x <= x + 11'd1;
    end

    // ------------------------------------------------------------ stage 0: MAME pixel
    logic        in_scr0, gap0, cell0, hs0, vs0, hb0, vb0;
    logic        line0;
    logic [3:0]  pos0;
    logic [2:0]  col0, row0;
    logic [10:0] x0;
    logic [9:0]  y0;
    always_ff @(posedge clk) begin : stage0
        logic [10:0] lx;
        logic [9:0]  ly;
        logic [6:0]  px, cx;
        logic [4:0]  py, cy;
        lx = x - 11'(X0);
        ly = y - 10'(Y0);
        px = lx[9:3];
        py = ly[7:3];
        in_scr0 <= x >= 11'(X0) && x < 11'(X0 + 97 * 8) && y >= 10'(Y0) && y < 10'(Y0 + 19 * 8);
        gap0 <= lx[2:0] == 3'd7 || ly[2:0] == 3'd7;
        cx = px - 7'd1;                          // 0..95: pos = cx / 6, col = cx % 6
        cy = py - 5'd1;                          // 0..17: line = cy / 9, row = cy % 9
        pos0 <= 4'(cx / 7'd6);
        col0 <= 3'(cx % 7'd6);
        line0 <= cy >= 5'd9;
        row0 <= 3'((cy >= 5'd9) ? cy - 5'd9 : cy);
        cell0 <= px >= 7'd1 && px <= 7'd96 && cx % 7'd6 < 7'd5 && py >= 5'd1 && py <= 5'd18
                 && cy != 5'd8 && cy != 5'd17;
        x0 <= x; y0 <= y;
        hb0 <= x >= 11'd800;
        vb0 <= y >= 10'd600;
        hs0 <= (x >= 11'd840 && x < 11'd968);
        vs0 <= (y >= 10'd601 && y < 10'd605);
    end

    // ------------------------------------------------------------ stage 1: character code
    assign r_line = line0;
    assign r_pos = pos0;
    assign r_cg_code = r_code[2:0];
    assign r_cg_row = row0;
    logic        cg1, cur1, in_scr1, gap1, cell1, hs1, vs1, hb1, vb1;
    logic [4:0]  cgb1;
    logic [2:0]  col1, row1;
    logic [10:0] x1;
    logic [9:0]  y1;
    logic [7:0]  font_q;
    always_ff @(posedge clk) begin
        font_q <= font[{r_code, 1'b0, row0}];
        cg1 <= r_code < 8'h10;
        cgb1 <= r_cg_bits;
        cur1 <= r_cursor;
        in_scr1 <= in_scr0; gap1 <= gap0; cell1 <= cell0; col1 <= col0; row1 <= row0;
        hs1 <= hs0; vs1 <= vs0; hb1 <= hb0; vb1 <= vb0; x1 <= x0; y1 <= y0;
    end

    // ------------------------------------------------------------ stage 2: dot
    logic        on2, in_scr2, gap2, cell2, hs2, vs2, hb2, vb2;
    logic [10:0] x2;
    logic [9:0]  y2;
    always_ff @(posedge clk) begin : stage2
        logic [4:0] bits;
        bits = cg1 ? cgb1 : font_q[4:0];
        if (cur1 && cursor_on && row1 == 3'd7) bits = 5'h1F;
        if (cur1 && blink_on && !blink) bits = 5'h1F;
        if (!display_on) bits = 5'h00;
        on2 <= bits[3'd4 - col1];
        in_scr2 <= in_scr1; gap2 <= gap1; cell2 <= cell1 && power;
        hs2 <= hs1; vs2 <= vs1; hb2 <= hb1; vb2 <= vb1; x2 <= x1; y2 <= y1;
    end

    // ------------------------------------------------------------ stage 3: colour
    wire        in_led = y2 >= 10'(LY) && y2 < 10'(LY + 24) && x2 >= 11'(X0) && x2 < 11'(X0 + 16 * 48);
    wire [10:0] lxo = x2 - 11'(X0);
    wire [3:0]  led_i = 4'(lxo / 11'd48);
    always_ff @(posedge clk) begin
        hs <= !hs2; vs <= !vs2; hblank <= hb2; vblank <= vb2;
        if (in_scr2) begin
            if (cell2 && !gap2) {r, g, b} <= on2 ? 24'h3F3B3E : 24'h83888B;   // on / off
            else {r, g, b} <= 24'h8A9294;                                   // background
        end else if (in_led && (lxo % 11'd48) < 11'd24) begin
            {r, g, b} <= leds[led_i] ? 24'hFF3020 : 24'h402018;
        end else if (y2 >= 10'(SY) && y2 < 10'(SY + 20) && x2 >= 11'(X0) && x2 < 11'(X0 + 200)) begin
            {r, g, b} <= loading ? 24'h2060C0 : rom_ok ? 24'h20C040 : 24'hC02020;
        end else if (y2 >= 10'(SY) && y2 < 10'(SY + 20) && x2 >= 11'(X0 + 220) && x2 < 11'(X0 + 260)) begin
            {r, g, b} <= midi_led ? 24'hFFFFFF : 24'h303848;
        end else if (y2 >= 10'(SY) && y2 < 10'(SY + 20) && x2 >= 11'(X0 + 280) && x2 < 11'(X0 + 320)) begin
            {r, g, b} <= underrun_seen ? 24'hE0A020 : 24'h303848;
        end else {r, g, b} <= 24'h101828;
    end
endmodule
