// SPDX-License-Identifier: GPL-2.0-or-later
// LCD renderer bench top: hdl/mt32_lcd_video.sv with the LCD state written by lcd_bench.cpp
// (text 0-19 at wa 0-19, CG RAM rows at wa 32-63); panel inputs idle.
module tb_lcd (
    input  logic        clk,
    input  logic        wr,
    input  logic [5:0]  wa,
    input  logic [7:0]  wd,
    input  logic [4:0]  cursor,
    input  logic        cursor_on, cursor_blink, cursor_font, lines2, display_on,
    output logic        hs, vs, hblank, vblank,
    output logic [7:0]  r, g, b
);
    logic [7:0] txt [20];
    logic [4:0] cg [32];
    logic [4:0] text_addr, cg_addr;
    always_ff @(posedge clk) if (wr) begin
        if (wa < 6'd20) txt[wa[4:0]] <= wd;
        else if (wa >= 6'd32) cg[wa[4:0]] <= wd[4:0];
    end
    mt32_lcd_video #(.FONT_HEX(`MT32_FONT_HEX), .PANEL_DIR(`MT32_PANEL_DIR)) dut (
        .clk, .text_addr, .text(txt[text_addr < 5'd20 ? text_addr : 5'd0]), .cg_addr,
        .cg_row(cg[cg_addr]), .cursor, .cursor_on, .cursor_blink, .cursor_font, .lines2,
        .display_on, .buttons(10'd0), .msg_led(1'b0),
        .rom_ok(1'b1), .loading(1'b0), .running(1'b1), .midi_led(1'b0), .underrun_seen(1'b0),
        .hs, .vs, .hblank, .vblank, .r, .g, .b);
endmodule
