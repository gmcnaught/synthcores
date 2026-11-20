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
`ifndef LCD_BACK_HEX
    `define LCD_BACK_HEX "../hdl/board/lcd_back.hex"     // Quartus: relative to mister/
`endif
    logic [14:0] back_a;
    logic [7:0]  back_q;
    sdp_ram #(.AW(15), .DW(8), .INIT(`LCD_BACK_HEX)) u_back (
        .clk, .we(1'b0), .wa(15'd0), .wd(8'd0), .ra(back_a), .rd(back_q));
    assign back_a = 15'(p1 >> 3);

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
    logic        in_lcd1, vis1, hs1, vs1, hb1, vb1;
    logic [17:0] p1;
    logic [9:0]  lx1;
    logic [8:0]  ly1;
    // narrow helpers (10-bit unsigned): cell of a pitch and the offset in it
    function automatic logic [9:0] div35(input logic [9:0] v); return v / 10'd35; endfunction
    function automatic logic [9:0] mod35(input logic [9:0] v); return v % 10'd35; endfunction
    function automatic logic [9:0] div6(input logic [9:0] v);  return v / 10'd6;  endfunction
    function automatic logic [9:0] mod6(input logic [9:0] v);  return v % 10'd6;  endfunction
    function automatic logic [9:0] div11(input logic [9:0] v); return v / 10'd11; endfunction
    function automatic logic [9:0] mod11(input logic [9:0] v); return v % 10'd11; endfunction
    function automatic logic [9:0] div26(input logic [9:0] v); return v / 10'd26; endfunction
    function automatic logic [9:0] mod26(input logic [9:0] v); return v % 10'd26; endfunction
    function automatic logic [9:0] div130(input logic [9:0] v); return v / 10'd130; endfunction
    function automatic logic [9:0] mod130(input logic [9:0] v); return v % 10'd130; endfunction

    always_ff @(posedge clk) begin : stage1
        logic [10:0] lxs, lys;          // x - X0, y - Y0 (two's complement, 11 bits)
        logic [9:0]  lx, ly, d, c, cm;
        logic        in_area;
        lxs = x - 11'(X0);
        lys = {1'b0, y} - 11'(Y0);
        in_area = !lxs[10] && !lys[10] && lxs < 11'(LW) && lys < 11'(LH);
        lx = lxs[9:0];
        ly = lys[9:0];
        hb1 <= x >= 11'd800;
        vb1 <= y >= 10'd600;
        hs1 <= (x >= 11'd840 && x < 11'd968);
        vs1 <= (y >= 10'd601 && y < 10'd605);
        in_lcd1 <= in_area;
        lx1 <= lx;
        ly1 <= ly[8:0];
        p1 <= 18'(ly) * 18'(LW) + 18'(lx);
        k1 <= 2'd0;
        ch_idx1 <= 7'd0;
        frow1 <= 4'd0;
        fbit1 <= 3'd0;
        // text fields: rows 11, 75, 139, 203; columns 34 (3 chars) and 153 (16 or 3 chars)
        for (int f = 0; f < 4; f++) begin
            if (in_area && ly >= 10'(11 + 64 * f) && ly < 10'(11 + 64 * f + 42)) begin
                d = ly - 10'(11 + 64 * f);
                if (mod6(d) < 10'd5) begin
                    for (int side = 0; side < 2; side++) begin : b_lcd_video_1
                        logic [9:0] c0, n;
                        logic [6:0] base;
                        c0 = (side != 0) ? 10'd153 : 10'd34;
                        n = (side != 0 && f == 0) ? 10'd16 : 10'd3;
                        case (f * 2 + side)
                            0: base = 7'd0;  1: base = 7'd3;  2: base = 7'd40; 3: base = 7'd43;
                            4: base = 7'd49; 5: base = 7'd46; 6: base = 7'd52; default: base = 7'd55;
                        endcase
                        if (lx >= c0 && lx < c0 + 10'd35 * n) begin
                            c = lx - c0;
                            cm = mod35(c);
                            if (cm < 10'd30 && mod6(cm) < 10'd5) begin
                                k1 <= 2'd1;
                                ch_idx1 <= base + 7'(div35(c));
                                frow1 <= 4'(div6(d));
                                fbit1 <= 3'(10'd4 - div6(cm));
                            end
                        end
                    end
                end
            end
        end
        // level bars: rows 71 + 88 i, 8 dots of 9 rows on 11; columns 293 + 130 j, dots 24 wide on 26
        for (int i = 0; i < 2; i++) begin
            if (in_area && ly >= 10'(71 + 88 * i) && ly < 10'(71 + 88 * i + 86)) begin
                d = ly - 10'(71 + 88 * i);
                if (mod11(d) < 10'd9 && lx >= 10'd293 && lx < 10'd293 + 10'd520) begin
                    c = lx - 10'd293;
                    cm = mod130(c);
                    if (mod26(cm) < 10'd24 && div26(cm) < ((div130(c) == 10'd3) ? 10'd1 : 10'd5)) begin
                        k1 <= 2'd2;
                        ch_idx1 <= 7'd20 + 7'(div130(c)) + 7'(40 * i);
                        frow1 <= 4'(div11(d));
                        fbit1 <= 3'(10'd4 - div26(cm));
                    end
                end
            end
        end
        // LR marks at rows 70 and 232, columns 264..274
        for (int l = 0; l < 2; l++) begin
            if (in_area && ly >= 10'((l != 0) ? 232 : 70) && ly < 10'((l != 0) ? 244 : 82) && lx >= 10'd264 && lx < 10'd275) begin
                if (lr_dot(1'(l), 4'(ly - 10'((l != 0) ? 232 : 70)), 4'(lx - 10'd264))) begin
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
    logic [2:0]  back_bit2;
    logic [9:0]  lx2;
    logic [8:0]  ly2;
    always_ff @(posedge clk) begin
        k2 <= k1; frow2 <= frow1; fbit2 <= fbit1;
        ch2 <= dd[ch_idx1];
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
        back3 <= back_q[back_bit2];
        in_lcd3 <= in_lcd2; hs3 <= hs2; vs3 <= vs2; hb3 <= hb2; vb3 <= vb2;
        lx3 <= lx2; ly3 <= ly2;
    end

    // ------------------------------------------------------------ stage 4: colour
    // color1 (dot on) 000000, color2 (dot off) 0050c8, background 0f6fff / 000000
    logic [4:0] st;
    always_ff @(posedge clk) begin : b_lcd_video_2
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
