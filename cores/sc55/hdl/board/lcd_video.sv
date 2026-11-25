// 800 x 600 at 60 Hz (40 MHz pixel clock): the SC-55's LCD drawn 1:1 as the
// reference's LCD_Render draws it for the mk1 (lcd.cpp: 741 x 268, the
// background bitmap, the eight text fields of 5 x 7 dots of 5 x 5 pixels on
// a 6-pixel pitch, the L/R marks and the level bars), placed at (29, 40);
// around it the front panel (cfg/panel.yaml: POWER, ALL / MUTE and the eight
// pairs of arrow buttons, each shown pressed while its MiSTer key is held),
// the status strip at (29, 564). Timing, panel art and status strip from
// synthframe's panel modules (the panel ROMs in hdl/panel/ are made by
// modules/synthframe/tools/panel_art.py). Reads the LCD controller's state and
// the buttons from the core clock without synchronisation (display only).
`ifndef SC55_PANEL_DIR
    `define SC55_PANEL_DIR "../hdl/panel/"               // Quartus: relative to mister/
`endif
module lcd_video #(
    parameter PANEL_DIR = `SC55_PANEL_DIR
) (
    input  logic       clk,              // pixel clock
    input  logic [7:0] dd [80],
    input  logic [4:0] cg [64],
    input  logic       enable,
    input  logic [31:0] buttons,         // kbd_buttons: mcu.button_pressed bits (MCU_BUTTON_*)
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
    output logic [7:0] b,
    // the LCD pixel at the output (bench): valid, position, colour
    output logic       lcd_pix,
    output logic [9:0] lcd_x,
    output logic [8:0] lcd_y
);
    localparam int X0 = 29, Y0 = 40, LW = 741, LH = 268;
    localparam int SX = 29, SY = 564;           // status strip

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
    logic        hb, vb, hsy, vsy;
    synthframe_vtiming timing (.clk, .x, .y, .hblank(hb), .vblank(vb), .hsync(hsy), .vsync(vsy));

    // ------------------------------------------------------------ stage 0: per-axis cells
    // x and y separately: position in the LCD, the text/bar cell and the offset
    // in it (one constant division each); stage 1 combines them
    logic        hb0, vb0, hs0, vs0;
    logic [10:0] x0, x1, x2, x3;        // screen position, for the panel art and the status strip
    logic [9:0]  y0, y1, y2, y3;
    logic        in_area0;
    logic [9:0]  lx0, ly0;
    logic [1:0]  tx_in3, tx_in16;       // per side: inside 3 (side 1 also 16) cells
    logic [3:0]  tx_q [2];              // cell number (35-pixel pitch)
    logic [5:0]  tx_cm [2];             // offset in the cell
    logic        bx_in;
    logic [1:0]  bx_q;                  // bar column (130-pixel pitch)
    logic [7:0]  bx_cm;
    logic [3:0]  ty_in;                 // per text field: inside its 42 rows
    logic [5:0]  ty_d [4];
    logic [1:0]  by_in;                 // per bar row: inside its 86 rows
    logic [6:0]  by_d [2];

    function automatic logic [9:0] div35(input logic [9:0] v); return v / 10'd35; endfunction
    function automatic logic [9:0] mod35(input logic [9:0] v); return v % 10'd35; endfunction
    function automatic logic [9:0] div130(input logic [9:0] v); return v / 10'd130; endfunction
    function automatic logic [9:0] mod130(input logic [9:0] v); return v % 10'd130; endfunction

    always_ff @(posedge clk) begin : stage0
        logic [10:0] lxs, lys;          // x - X0, y - Y0 (two's complement, 11 bits)
        logic [9:0]  lx, ly, c;
        lxs = x - 11'(X0);
        lys = {1'b0, y} - 11'(Y0);
        lx = lxs[9:0];
        ly = lys[9:0];
        hb0 <= hb; vb0 <= vb; hs0 <= hsy; vs0 <= vsy;
        x0 <= x; y0 <= y;
        in_area0 <= !lxs[10] && !lys[10] && lxs < 11'(LW) && lys < 11'(LH);
        lx0 <= lx;
        ly0 <= ly;
        // text columns 34 (3 cells) and 153 (3 cells, or 16 in field 0)
        for (int sd = 0; sd < 2; sd++) begin
            c = lx - ((sd != 0) ? 10'd153 : 10'd34);
            tx_in3[sd]  <= lx >= ((sd != 0) ? 10'd153 : 10'd34) && c < 10'd105;
            tx_in16[sd] <= lx >= ((sd != 0) ? 10'd153 : 10'd34) && c < 10'd560;
            tx_q[sd]  <= 4'(div35(c));
            tx_cm[sd] <= 6'(mod35(c));
        end
        c = lx - 10'd293;
        bx_in <= lx >= 10'd293 && c < 10'd520;
        bx_q  <= 2'(div130(c));
        bx_cm <= 8'(mod130(c));
        for (int f = 0; f < 4; f++) begin
            ty_in[f] <= ly >= 10'(11 + 64 * f) && ly < 10'(11 + 64 * f + 42);
            ty_d[f]  <= 6'(ly - 10'(11 + 64 * f));
        end
        for (int i = 0; i < 2; i++) begin
            by_in[i] <= ly >= 10'(71 + 88 * i) && ly < 10'(71 + 88 * i + 86);
            by_d[i]  <= 7'(ly - 10'(71 + 88 * i));
        end
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

    always_ff @(posedge clk) begin : stage1
        logic [5:0] cm;
        logic [7:0] bm;
        logic       inn;
        hb1 <= hb0; vb1 <= vb0; hs1 <= hs0; vs1 <= vs0;
        x1 <= x0; y1 <= y0;
        in_lcd1 <= in_area0;
        lx1 <= lx0;
        ly1 <= ly0[8:0];
        p1 <= 18'(ly0) * 18'(LW) + 18'(lx0);
        k1 <= 2'd0;
        ch_idx1 <= 7'd0;
        frow1 <= 4'd0;
        fbit1 <= 3'd0;
        // text fields: rows 11, 75, 139, 203; dots 5 x 5 on a 6-pixel pitch, cells of 35
        for (int f = 0; f < 4; f++) begin
            if (in_area0 && ty_in[f] && (ty_d[f] % 6'd6) < 6'd5) begin
                for (int sd = 0; sd < 2; sd++) begin : b_lcd_video_1
                    logic [6:0] base;
                    case (f * 2 + sd)
                        0: base = 7'd0;  1: base = 7'd3;  2: base = 7'd40; 3: base = 7'd43;
                        4: base = 7'd49; 5: base = 7'd46; 6: base = 7'd52; default: base = 7'd55;
                    endcase
                    inn = (sd != 0 && f == 0) ? tx_in16[sd] : tx_in3[sd];
                    cm = tx_cm[sd];
                    if (inn && cm < 6'd30 && (cm % 6'd6) < 6'd5) begin
                        k1 <= 2'd1;
                        ch_idx1 <= base + 7'(tx_q[sd]);
                        frow1 <= 4'(ty_d[f] / 6'd6);
                        fbit1 <= 3'(6'd4 - cm / 6'd6);
                    end
                end
            end
        end
        // level bars: rows 71 + 88 i, 8 dots of 9 rows on 11; columns 293 + 130 j, dots 24 wide on 26
        for (int i = 0; i < 2; i++) begin
            if (in_area0 && by_in[i] && (by_d[i] % 7'd11) < 7'd9 && bx_in) begin
                bm = bx_cm;
                if ((bm % 8'd26) < 8'd24 && (bm / 8'd26) < ((bx_q == 2'd3) ? 8'd1 : 8'd5)) begin
                    k1 <= 2'd2;
                    ch_idx1 <= 7'd20 + 7'(bx_q) + 7'(40 * i);
                    frow1 <= 4'(by_d[i] / 7'd11);
                    fbit1 <= 3'(8'd4 - bm / 8'd26);
                end
            end
        end
        // LR marks at rows 70 and 232, columns 264..274
        for (int l = 0; l < 2; l++) begin
            if (in_area0 && ly0 >= 10'((l != 0) ? 232 : 70) && ly0 < 10'((l != 0) ? 244 : 82) && lx0 >= 10'd264 && lx0 < 10'd275) begin
                if (lr_dot(1'(l), 4'(ly0 - 10'((l != 0) ? 232 : 70)), 4'(lx0 - 10'd264))) begin
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
        x2 <= x1; y2 <= y1;
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
        x3 <= x2; y3 <= y2;
    end

    // ------------------------------------------------------------ stage 4: colour
    // color1 (dot on) 000000, color2 (dot off) 0050c8, background 0f6fff / 000000
    logic [6:0]  st;
    logic        st_hit;
    logic [23:0] st_rgb;
    synthframe_status_strip #(.X0(SX), .Y0(SY)) strip (
        .x(x3), .y(y3), .rom_ok(st[0]), .loading(st[1]), .running(st[2]),
        .midi_led(st[3]), .underrun_led(st[4]), .underrun_seen(st[5]), .lag_seen(st[6]),
        .hit(st_hit), .rgb(st_rgb));

    // front panel: (x0, y0) in, the colour with stage 3 (three clocks)
    /* verilator lint_off UNUSEDPARAM */
    `include "sc55_panel.svh"
    /* verilator lint_on UNUSEDPARAM */
    logic [2**PANEL_EB-1:0] panel_state;
    `include "sc55_panel_state.svh"
    generate if (PANEL_NE + 1 < 2**PANEL_EB) begin : g_state_pad
        assign panel_state[2**PANEL_EB-1:PANEL_NE+1] = '0;
    end endgenerate
    logic [23:0] art;
    synthframe_panel_art #(.W(PANEL_W), .H(PANEL_H), .BPP(PANEL_BPP), .TB(PANEL_TB), .EB(PANEL_EB),
                           .NT(PANEL_NT), .PAL(PANEL_PAL), .LIT(PANEL_LIT),
                           .MAP_HEX({PANEL_DIR, "sc55_panel_map.hex"}),
                           .TILE_HEX({PANEL_DIR, "sc55_panel_tiles.hex"})) panel_art (
        .clk, .ce(1'b1), .x(x0), .y(y0), .state(panel_state), .rgb(art));
    always_ff @(posedge clk) begin : b_lcd_video_2
        logic on;
        st <= {lag_seen, underrun_seen, underrun_led, midi_led, running, loading, rom_ok};
        hs <= !hs3; vs <= !vs3; hblank <= hb3; vblank <= vb3;
        lcd_pix <= in_lcd3; lcd_x <= lx3; lcd_y <= ly3;
        on = (k3 == 2'd3) ? row3[0] : row3[fbit3];
        if (in_lcd3) begin
            if (!enable) {r, g, b} <= 24'h000000;
            else if (k3 == 2'd0) {r, g, b} <= back3 ? 24'h000000 : 24'h0f6fff;
            else {r, g, b} <= on ? 24'h000000 : 24'h0050c8;
        end else if (st_hit) {r, g, b} <= st_rgb;
        else {r, g, b} <= art;
    end
endmodule
