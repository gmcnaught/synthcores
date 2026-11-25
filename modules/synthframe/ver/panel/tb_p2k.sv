// SPDX-License-Identifier: MIT
// Frame bench for the Proteus 2000 screen (cores/p2k/hdl/p2k_lcd_video.sv) with its HD44780
// controller (2 x 24): a seeded font loaded through the system-pack port, then a seeded command
// sequence (function set, display control, CG RAM, both lines, display shifts, cursor address).
// Seeded LEDs, pressed panel buttons, output level, status and diagnostics line. The C++ driver
// lets the sequence finish, then hashes every clock's outputs.
module tb_p2k (
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
    logic [12:0] fi;
    logic [31:0] fs;
    logic [15:0] leds;
    logic [15:7] buttons;
    logic [14:0] level;
    logic [95:0] diag_hex;
    logic        rom_ok, loading, running, midi, late, diag;
    always_ff @(posedge clk) if (load) begin : gen
        logic [31:0] s;
        int n;
        s = xs(seed ^ 32'h2b2b2b2b);
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
        seq[n] <= {2'b01, s[0], 1'b0, 5'(s[5:1] % 5'd24)}; n++;       // cursor address, columns 0-23
        seq_n <= 9'(n);
        s = xs(s);
        leds <= s[15:0];
        s = xs(s);
        buttons <= s[8:0] & s[17:9];                                  // ~1/4 pressed
        s = xs(s);
        level <= s[14:0];
        rom_ok <= !s[16]; loading <= s[17]; running <= s[18]; midi <= s[19]; late <= s[20];
        diag <= seed[4];
        for (int i = 0; i < 3; i++) begin s = xs(s); diag_hex[32 * i +: 32] <= s; end
    end

    // font load, then the command sequence
    logic        font_we, wr, rs;
    logic [11:0] font_addr;
    logic [7:0]  font_data, din;
    always_ff @(posedge clk) begin
        font_we <= 1'b0; wr <= 1'b0;
        if (load) begin fi <= 13'd0; seq_i <= 9'd0; gap <= 6'd0; fs <= xs(seed ^ 32'h0f0f1234); end
        else if (!fi[12]) begin
            fs <= xs(fs);
            font_we <= 1'b1; font_addr <= fi[11:0]; font_data <= fs[7:0]; fi <= fi + 13'd1;
        end else if (seq_i < seq_n) begin
            gap <= gap + 6'd1;
            if (gap == 6'd63) begin
                wr <= 1'b1; {rs, din} <= seq[seq_i[7:0]]; seq_i <= seq_i + 9'd1;
            end
        end
    end

    logic        r_line, r_at_ac, disp_on, cur_on, blk_on;
    logic [4:0]  r_pos, r_cg_bits;
    logic [7:0]  r_code, lcd_dout;
    logic [2:0]  r_row;
    synthframe_hd44780 #(.POS_W(5)) lcd (
        .clk, .reset(load), .cs(1'b1), .wr, .rd(1'b0), .rs, .din, .dout(lcd_dout),
        .busy_t_set(32'd0), .busy_t_read(32'd0),
        .r_line, .r_pos, .r_row, .r_code, .r_cgbits(r_cg_bits), .r_is_cg(), .r_at_ac, .r_cursor(),
        .display_on(disp_on), .cursor_on(cur_on), .blink_on(blk_on));

    p2k_lcd_video #(.PANEL_DIR(`P2K_PANEL_DIR)) dut (
        .clk, .clk_sys(clk), .font_we, .font_addr, .font_data,
        .r_line, .r_pos, .r_row, .r_code, .r_at_ac, .r_cg_bits,
        .display_on(disp_on), .cursor_on(cur_on), .blink_on(blk_on), .leds, .buttons, .level,
        .rom_ok, .loading, .running, .midi_led(midi), .late_seen(late), .diag, .diag_hex,
        .hs, .vs, .hblank, .vblank, .r, .g, .b);
    assign aux = 32'd1;   // tb_frame.cpp counts a pixel per aux[0] for benches named p*
endmodule
