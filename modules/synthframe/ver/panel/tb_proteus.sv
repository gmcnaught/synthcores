// SPDX-License-Identifier: MIT
// Frame bench for the Proteus/1 display (cores/proteus/mister/rtl/display.sv) with its HD44780
// controller: a seeded command sequence (function set, display control, CG RAM, both lines,
// display shifts, cursor address), a seeded font in place of the ROM pack's, seeded LEDs and
// voice levels. The C++ driver lets the sequence finish, then hashes every clock's outputs.
module tb_proteus (
    input  logic        clk,
    input  logic        load,
    input  logic [31:0] seed,
    output logic        hs, vs, hblank, vblank,
    output logic [7:0]  r, g, b,
    output logic [31:0] aux
);
    `include "tb_common.svh"
    // command sequence: {rs, data}
    logic [8:0]  seq [256];
    logic [8:0]  seq_n, seq_i;
    logic [5:0]  gap;
    logic [7:0]  font [4096];
    logic [7:0]  leds;
    logic [31:0] lvl_seed;
    always_ff @(posedge clk) if (load) begin : gen
        logic [31:0] s;
        int n;
        s = seed;
        for (int i = 0; i < 4096; i++) begin s = xs(s); font[i] <= s[7:0]; end
        n = 0;
        seq[n] <= 9'h038; n++;                                        // 8-bit, two lines
        seq[n] <= {1'b0, 5'b00001, 1'b1, seed[0], seed[1]}; n++;      // display on, cursor, blink
        seq[n] <= 9'h006; n++;                                        // increment, no shift
        seq[n] <= 9'h001; n++;                                        // clear
        seq[n] <= 9'h040; n++;                                        // CG RAM 0
        for (int i = 0; i < 64; i++) begin s = xs(s); seq[n] <= {1'b1, s[7:0]}; n++; end
        seq[n] <= 9'h080; n++;
        for (int i = 0; i < 40; i++) begin s = xs(s); seq[n] <= {1'b1, s[7:0]}; n++; end
        seq[n] <= 9'h0c0; n++;
        for (int i = 0; i < 40; i++) begin s = xs(s); seq[n] <= {1'b1, s[7:0]}; n++; end
        for (int i = 0; i < 7; i++) if (i < int'(seed[6:4])) begin
            seq[n] <= seed[7] ? 9'h01c : 9'h018; n++;                 // display shift right/left
        end
        s = xs(s);
        seq[n] <= {2'b01, s[0], 1'b0, s[11:8] < 4'd10 ? 2'(s[9:8]) : 2'd0, s[3:0]}; n++;   // cursor address
        seq_n <= 9'(n);
        s = xs(s);
        leds <= s[7:0];
        lvl_seed <= xs(s);
    end
    logic        wr;
    logic [7:0]  din;
    logic        rs;
    always_ff @(posedge clk) begin
        wr <= 1'b0;
        if (load) begin seq_i <= 9'd0; gap <= 6'd0; end
        else if (seq_i < seq_n) begin
            gap <= gap + 6'd1;
            if (gap == 6'd63) begin
                wr <= 1'b1; {rs, din} <= seq[seq_i[7:0]]; seq_i <= seq_i + 9'd1;
            end
        end
    end

    logic        lcd_line, lcd_is_cg, lcd_cursor, lcd_on;
    logic [3:0]  lcd_pos;
    logic [2:0]  lcd_row;
    logic [7:0]  lcd_code, lcd_dout;
    logic [4:0]  lcd_cgbits;
    logic [11:0] font_addr, font2_addr;
    logic [7:0]  font_data, font2_data;
    always_ff @(posedge clk) begin font_data <= font[font_addr]; font2_data <= font[font2_addr]; end

    hd44780 lcd (
        .clk, .reset(load), .cs(1'b1), .wr, .rd(1'b0), .rs, .din, .dout(lcd_dout),
        .r_line(lcd_line), .r_pos(lcd_pos), .r_row(lcd_row), .r_code(lcd_code), .r_cgbits(lcd_cgbits),
        .r_is_cg(lcd_is_cg), .r_cursor(lcd_cursor), .display_on(lcd_on));

    logic [4:0] voice;
    logic [7:0] level;
    logic       ce_pix;
    assign level = 8'(xs(lvl_seed ^ {27'd0, voice}));
    display dut (
        .clk, .ce_pix, .hsync(hs), .vsync(vs), .hblank, .vblank, .r, .g, .b,
        .lcd_line, .lcd_pos, .lcd_row, .lcd_code, .lcd_cgbits, .lcd_is_cg, .lcd_cursor, .lcd_on,
        .font_addr, .font_data, .font2_addr, .font2_data, .leds,
        .voice, .level, .running(lvl_seed[0]));
    assign aux = {31'd0, ce_pix};
endmodule
