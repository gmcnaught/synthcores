// SPDX-License-Identifier: MIT
// Frame bench for the SC-55 renderer (cores/sc55/hdl/board/lcd_video.sv): seeded display data,
// CG RAM, enable and status bits; the C++ driver hashes every pixel clock's outputs.
module tb_sc55 (
    input  logic        clk,
    input  logic        load,
    input  logic [31:0] seed,
    output logic        hs, vs, hblank, vblank,
    output logic [7:0]  r, g, b,
    output logic [31:0] aux
);
    `include "tb_common.svh"
    logic [7:0] dd [80];
    logic [4:0] cg [64];
    logic       enable;
    logic [6:0] st;
    always_ff @(posedge clk) if (load) begin : gen
        logic [31:0] s;
        s = seed;
        for (int i = 0; i < 80; i++) begin s = xs(s); dd[i] <= s[7:0]; end
        for (int i = 0; i < 64; i++) begin s = xs(s); cg[i] <= s[4:0]; end
        s = xs(s);
        st <= s[6:0];
        enable <= seed[3:0] != 4'd0;
    end
    logic       lcd_pix;
    logic [9:0] lcd_x;
    logic [8:0] lcd_y;
    lcd_video dut (
        .clk, .dd, .cg, .enable,
        .rom_ok(st[0]), .loading(st[1]), .running(st[2]), .midi_led(st[3]),
        .underrun_led(st[4]), .underrun_seen(st[5]), .lag_seen(st[6]),
        .hs, .vs, .hblank, .vblank, .r, .g, .b, .lcd_pix, .lcd_x, .lcd_y);
    assign aux = {12'd0, lcd_pix, lcd_x, lcd_y};
endmodule
