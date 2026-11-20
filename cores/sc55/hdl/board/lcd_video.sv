// 800 x 600 at 60 Hz (40 MHz pixel clock): the SC-55's LCD drawn 1:1 as the
// reference's LCD_Render draws it for the mk1 (lcd.cpp: 741 x 268, the
// background bitmap, the eight text fields of 5 x 7 dots of 5 x 5 pixels on
// a 6-pixel pitch, the L/R marks and the level bars), placed at (29, 40);
// below it a status strip. Reads the LCD controller's state from the core
// clock without synchronisation (display only).
module lcd_video (
    input  logic       clk,              // pixel clock
    input  logic [7:0] dd [80],
    input  logic [4:0] cg [64],
    input  logic       enable,
    // status strip
    input  logic       rom_ok,
    input  logic       loading,
    input  logic       running,
    input  logic       midi_led,
    input  logic       underrun_led,
    output logic       hs,
    output logic       vs,
    output logic       hblank,
    output logic       vblank,
    output logic [7:0] r,
    output logic [7:0] g,
    output logic [7:0] b,
    // the LCD pixel at the output (bench): valid, position, colour
    output logic       lcd_pix,
    output logic [9:0] lcd_x,
    output logic [8:0] lcd_y
);
    localparam int X0 = 29, Y0 = 40, LW = 741, LH = 268;

    // background bitmap: pixel p in bit p % 8 of byte p / 8
    logic [7:0] back [24824];
`ifndef LCD_BACK_HEX
    `define LCD_BACK_HEX "../hdl/board/lcd_back.hex"     // Quartus: relative to mister/
`endif
    initial $readmemh(`LCD_BACK_HEX, back);

    function automatic logic [4:0] font_row(input logic [7:0] fc, input logic [3:0] fr);
        `include "lcd_font.svh"
    endfunction

    // LR marks (lcd.cpp LR), 12 rows of 11 dots
    function automatic logic lr_dot(input logic letter, input logic [3:0] i, input logic [3:0] j);
        logic [10:0] row;
        if (!letter) row = (i >= 4'd10) ? 11'b11111111111 : 11'b11000000000;
        else case (i)
            4'd0: row = 11'b11111111100;
            4'd1: row = 11'b11111111110;
            4'd2, 4'd3, 4'd4: row = 11'b11000000110;
            4'd5: row = 11'b11111111110;
            4'd6: row = 11'b11111111100;
            4'd7: row = 11'b11000001100;
            4'd8, 4'd9: row = 11'b11000000110;
            default: row = 11'b11000000011;
        endcase
        return row[4'd10 - j];
    endfunction

    // ------------------------------------------------------------ timing
    logic [10:0] x;
    logic [9:0]  y;
    always_ff @(posedge clk) begin
        if (x == 11'd1055) begin
            x <= 11'd0;
            y <= (y == 10'd627) ? 10'd0 : y + 10'd1;
        end else x <= x + 11'd1;
    end

    // ------------------------------------------------------------ stage 1: what is at this pixel
    // kind: 0 background, 1 text dot, 2 level dot, 3 LR mark
    logic [1:0]  k1;
    logic [6:0]  ch_idx1;          // index into dd
    logic [3:0]  frow1;            // font row
    logic [2:0]  fbit1;            // dot column (bit 4 - j)
    logic        lr_letter1, lr_on1;
    logic        in_lcd1, vis1, hs1, vs1, hb1, vb1;
    logic [17:0] p1;
    logic [9:0]  lx1;
    logic [8:0]  ly1;
    always_ff @(posedge clk) begin : stage1
        int lx, ly, d, c;
        lx = int'(x) - X0;
        ly = int'(y) - Y0;
        hb1 <= x >= 11'd800;
        vb1 <= y >= 10'd600;
        hs1 <= (x >= 11'd840 && x < 11'd968);
        vs1 <= (y >= 10'd601 && y < 10'd605);
        in_lcd1 <= lx >= 0 && lx < LW && ly >= 0 && ly < LH;
        lx1 <= 10'(lx);
        ly1 <= 9'(ly);
        p1 <= 18'(ly * LW + lx);
        k1 <= 2'd0;
        ch_idx1 <= 7'd0;
        frow1 <= 4'd0;
        fbit1 <= 3'd0;
        lr_letter1 <= 1'b0;
        lr_on1 <= 1'b0;
        // text fields: rows 11, 75, 139, 203; columns 34 (3 chars) and 153 (16 or 3 chars)
        for (int f = 0; f < 4; f++) begin
            d = ly - (11 + 64 * f);
            if (d >= 0 && d < 42 && (d % 6) < 5) begin
                for (int side = 0; side < 2; side++) begin
                    int c0, n, base;
                    c0 = (side != 0) ? 153 : 34;
                    n = (side != 0 && f == 0) ? 16 : 3;
                    case (f * 2 + side)
                        0: base = 0;  1: base = 3;  2: base = 40; 3: base = 43;
                        4: base = 49; 5: base = 46; 6: base = 52; default: base = 55;
                    endcase
                    c = lx - c0;
                    if (c >= 0 && c < 35 * n && (c % 35) < 30 && ((c % 35) % 6) < 5) begin
                        k1 <= 2'd1;
                        ch_idx1 <= 7'(base + c / 35);
                        frow1 <= 4'(d / 6);
                        fbit1 <= 3'(4 - (c % 35) / 6);
                    end
                end
            end
        end
        // level bars: rows 71 + 88 i, 8 dots of 9 rows on 11; columns 293 + 130 j, dots 24 wide on 26
        for (int i = 0; i < 2; i++) begin
            d = ly - (71 + 88 * i);
            if (d >= 0 && d < 86 && (d % 11) < 9) begin
                c = lx - 293;
                if (c >= 0 && c < 4 * 130 && ((c % 130) % 26) < 24) begin
                    if ((c % 130) / 26 < ((c / 130 == 3) ? 1 : 5)) begin
                        k1 <= 2'd2;
                        ch_idx1 <= 7'(20 + c / 130 + i * 40);
                        frow1 <= 4'(d / 11);
                        fbit1 <= 3'(4 - (c % 130) / 26);
                    end
                end
            end
        end
        // LR marks at rows 70 and 232, columns 264..274
        for (int l = 0; l < 2; l++) begin
            d = ly - ((l != 0) ? 232 : 70);
            c = lx - 264;
            if (d >= 0 && d < 12 && c >= 0 && c < 11) begin
                if (lr_dot(1'(l), 4'(d), 4'(c))) begin
                    k1 <= 2'd3;
                    ch_idx1 <= 7'd58;
                    frow1 <= 4'd0;
                    fbit1 <= 3'd0;
                end
            end
        end
    end

    // ------------------------------------------------------------ stage 2: character code, background bit
    logic [1:0]  k2;
    logic [7:0]  ch2;
    logic [3:0]  frow2;
    logic [2:0]  fbit2;
    logic        in_lcd2, hs2, vs2, hb2, vb2;
    logic [7:0]  back_byte2;
    logic [2:0]  back_bit2;
    logic [9:0]  lx2;
    logic [8:0]  ly2;
    always_ff @(posedge clk) begin
        k2 <= k1; frow2 <= frow1; fbit2 <= fbit1;
        ch2 <= dd[ch_idx1];
        back_byte2 <= back[15'(p1 >> 3)];
        back_bit2 <= p1[2:0];
        in_lcd2 <= in_lcd1; hs2 <= hs1; vs2 <= vs1; hb2 <= hb1; vb2 <= vb1;
        lx2 <= lx1; ly2 <= ly1;
    end

    // ------------------------------------------------------------ stage 3: font row
    logic [1:0]  k3;
    logic [4:0]  row3;
    logic [2:0]  fbit3;
    logic        back3, in_lcd3, hs3, vs3, hb3, vb3;
    logic [9:0]  lx3;
    logic [8:0]  ly3;
    always_ff @(posedge clk) begin
        k3 <= k2; fbit3 <= fbit2;
        row3 <= (ch2 >= 8'd16) ? font_row(ch2 - 8'd16, frow2) : cg[{ch2[2:0], frow2[2:0]}];
        back3 <= back_byte2[back_bit2];
        in_lcd3 <= in_lcd2; hs3 <= hs2; vs3 <= vs2; hb3 <= hb2; vb3 <= vb2;
        lx3 <= lx2; ly3 <= ly2;
    end

    // ------------------------------------------------------------ stage 4: colour
    // color1 (dot on) 000000, color2 (dot off) 0050c8, background 0f6fff / 000000
    logic [4:0] st;
    always_ff @(posedge clk) begin
        logic on;
        st <= {underrun_led, midi_led, running, loading, rom_ok};
        hs <= !hs3; vs <= !vs3; hblank <= hb3; vblank <= vb3;
        lcd_pix <= in_lcd3; lcd_x <= lx3; lcd_y <= ly3;
        on = (k3 == 2'd3) ? row3[0] : row3[fbit3];
        if (in_lcd3) begin
            if (!enable) {r, g, b} <= 24'h000000;
            else if (k3 == 2'd0) {r, g, b} <= back3 ? 24'h000000 : 24'h0f6fff;
            else {r, g, b} <= on ? 24'h000000 : 24'h0050c8;
        end else begin
            {r, g, b} <= 24'h101828;
            // status strip under the LCD: state band, MIDI and underrun boxes
            if (ly3 >= 9'd300 && ly3 < 9'd320 && lx3 < 10'd200)
                {r, g, b} <= st[2] ? 24'h20c040 : st[1] ? 24'he0a020 : st[0] ? 24'h2060c0 : 24'hc02020;
            if (ly3 >= 9'd300 && ly3 < 9'd320 && lx3 >= 10'd220 && lx3 < 10'd260)
                {r, g, b} <= st[3] ? 24'hffffff : 24'h303848;
            if (ly3 >= 9'd300 && ly3 < 9'd320 && lx3 >= 10'd280 && lx3 < 10'd320)
                {r, g, b} <= st[4] ? 24'hff4040 : 24'h303848;
        end
    end
endmodule
