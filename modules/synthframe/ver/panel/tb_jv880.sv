// SPDX-License-Identifier: MIT
// Frame bench for the JV-880 renderer (cores/jv880/hdl/jv880_lcd_video.sv): seeded display
// data, CG RAM, cursor, status bits and debug line.
module tb_jv880 (
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
    logic [7:0] dbg_ch [24];
    logic [6:0] dd_ram, st;
    logic       cursor_on, dbg_on;
    always_ff @(posedge clk) if (load) begin : gen
        logic [31:0] s;
        s = seed;
        for (int i = 0; i < 80; i++) begin s = xs(s); dd[i] <= s[7:0]; end
        for (int i = 0; i < 64; i++) begin s = xs(s); cg[i] <= s[4:0]; end
        for (int i = 0; i < 24; i++) begin s = xs(s); dbg_ch[i] <= s[7:0]; end
        s = xs(s);
        st <= s[6:0];
        dd_ram <= {s[7], 1'b0, s[12:8] % 5'd24};
        cursor_on <= s[13] | seed[0];
        dbg_on <= seed[1];
    end
    jv880_lcd_video dut (
        .clk, .dd, .cg, .dd_ram, .cursor_on,
        .rom_ok(st[0]), .loading(st[1]), .running(st[2]), .midi_led(st[3]),
        .underrun_led(st[4]), .underrun_seen(st[5]), .lag_seen(st[6]),
        .dbg_ch, .dbg_on,
        .hs, .vs, .hblank, .vblank, .r, .g, .b);
    assign aux = 32'd0;
endmodule
