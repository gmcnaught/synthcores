// SPDX-License-Identifier: MIT
// Frame bench for the CZ-101 LCD (cores/cz101/hdl/cz101_lcd.sv and cz101_lcd_video.sv): a
// seeded font loaded through the ROM-pack port, then a seeded command sequence on the HD44780
// pins as the CZ-101 drives them (port A data, port C E/RW/RS): function set (one or two
// lines), display control, CG RAM, both lines, display shifts, cursor address and a data read.
// Seeded LEDs, power and status. The C++ driver lets the sequence finish, then hashes every
// clock's outputs.
module tb_cz101 (
    input  logic        clk,
    input  logic        load,
    input  logic [31:0] seed,
    output logic        hs, vs, hblank, vblank,
    output logic [7:0]  r, g, b,
    output logic [31:0] aux
);
    `include "tb_common.svh"
    // pin events: {kind, value}; kind 0 = port A write, 1 = port C write, 2 = port A read
    logic [9:0]  ev [1024];
    logic [10:0] ev_n, ev_i;
    logic [5:0]  gap;
    logic [12:0] fi;
    logic [31:0] fs;
    logic [15:0] leds;
    logic        power, rom_ok, loading, midi, under;
    always_ff @(posedge clk) if (load) begin : gen
        logic [31:0] s;
        int n;
        logic [8:0] c [256];
        int nc;
        s = xs(seed ^ 32'h5a5a5a5a);
        nc = 0;
        c[nc] = {1'b0, 4'b0011, !(seed[3] && seed[4]), 3'b000}; nc++;  // 8-bit, lines from the seed
        c[nc] = {1'b0, 5'b00001, 1'b1, seed[0], seed[1]}; nc++;        // display on, cursor, blink
        c[nc] = 9'h006; nc++;                                         // increment, no shift
        c[nc] = 9'h001; nc++;                                         // clear
        c[nc] = 9'h040; nc++;                                         // CG RAM 0
        for (int i = 0; i < 64; i++) begin s = xs(s); c[nc] = {1'b1, s[7:0]}; nc++; end
        c[nc] = 9'h080; nc++;
        for (int i = 0; i < 40; i++) begin s = xs(s); c[nc] = {1'b1, s[7:0]}; nc++; end
        c[nc] = 9'h0c0; nc++;
        for (int i = 0; i < 40; i++) begin s = xs(s); c[nc] = {1'b1, s[7:0]}; nc++; end
        for (int i = 0; i < 7; i++) if (i < int'(seed[6:4])) begin
            c[nc] = seed[7] ? 9'h01c : 9'h018; nc++;                  // display shift right/left
        end
        s = xs(s);
        c[nc] = {2'b01, s[0], 2'b00, s[3:0]}; nc++;                    // cursor address, columns 0-15
        n = 0;
        for (int i = 0; i < nc; i++) begin
            ev[n] <= {2'd0, c[i][7:0]}; n++;                          // data on port A
            ev[n] <= {2'd1, 1'b0, 1'b0, c[i][8], 5'd0}; n++;          // E high, write, RS
            ev[n] <= {2'd1, 1'b1, 1'b0, c[i][8], 5'd0}; n++;          // E low: the write
        end
        if (seed[2]) begin                                            // data read: AC steps
            ev[n] <= {2'd1, 1'b0, 1'b1, 1'b1, 5'd0}; n++;
            ev[n] <= {2'd2, 8'd0}; n++;
            ev[n] <= {2'd1, 1'b1, 1'b1, 1'b1, 5'd0}; n++;
        end
        ev_n <= 11'(n);
        s = xs(s);
        leds <= s[15:0];
        power <= seed != 32'h10;
        rom_ok <= !s[16]; loading <= s[17]; midi <= s[18]; under <= s[19];
    end

    // font load, then the pin events
    logic        font_we, pc_we, db_we, db_rd;
    logic [11:0] font_addr;
    logic [7:0]  font_data, pc, db_in;
    logic [31:0] now;
    always_ff @(posedge clk) begin
        font_we <= 1'b0; pc_we <= 1'b0; db_we <= 1'b0; db_rd <= 1'b0;
        now <= now + 32'd1;
        if (load) begin fi <= 13'd0; ev_i <= 11'd0; gap <= 6'd0; now <= 32'd0; fs <= xs(seed ^ 32'h0f0f1234); end
        else if (!fi[12]) begin
            fs <= xs(fs);
            font_we <= 1'b1; font_addr <= fi[11:0]; font_data <= fs[7:0]; fi <= fi + 13'd1;
        end else if (ev_i < ev_n) begin
            gap <= gap + 6'd1;
            if (gap == 6'd63) begin
                case (ev[ev_i[9:0]][9:8])
                    2'd0: begin db_we <= 1'b1; db_in <= ev[ev_i[9:0]][7:0]; end
                    2'd1: begin pc_we <= 1'b1; pc <= ev[ev_i[9:0]][7:0]; end
                    default: db_rd <= 1'b1;
                endcase
                ev_i <= ev_i + 11'd1;
            end
        end
    end

    logic        r_line, r_cursor, disp_on, cur_on, blk_on;
    logic [3:0]  r_pos;
    logic [7:0]  r_code, db_out;
    logic [2:0]  r_cg_code, r_cg_row;
    logic [4:0]  r_cg_bits;
    cz101_lcd lcd (
        .clk, .reset(load), .now, .insn_start(now),
        .pc_we, .pc, .lcd_reset(1'b0), .db_in, .db_we, .db_rd, .db_out,
        .r_line, .r_pos, .r_code, .r_cursor, .r_cg_code, .r_cg_row, .r_cg_bits,
        .display_on(disp_on), .cursor_on(cur_on), .blink_on(blk_on), .cursor_addr());

    cz101_lcd_video dut (
        .clk, .clk_sys(clk), .font_we, .font_addr, .font_data,
        .r_line, .r_pos, .r_code, .r_cursor, .r_cg_code, .r_cg_row, .r_cg_bits,
        .display_on(disp_on), .cursor_on(cur_on), .blink_on(blk_on), .power, .leds,
        .rom_ok, .loading, .midi_led(midi), .underrun_seen(under),
        .hs, .vs, .hblank, .vblank, .r, .g, .b);
    assign aux = 32'd0;
endmodule
