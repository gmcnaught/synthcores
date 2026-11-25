// SPDX-License-Identifier: MIT
// Frame bench for the MT-32 renderer (cores/mt32/hdl/mt32_lcd_video.sv): seeded LCD text and
// CG RAM rows (the core's combinational read ports), cursor, display on (off for seed 0x10),
// pressed panel buttons (about 1/4), the MIDI MESSAGE LED and status bits.
module tb_mt32 (
    input  logic        clk,
    input  logic        load,
    input  logic [31:0] seed,
    output logic        hs, vs, hblank, vblank,
    output logic [7:0]  r, g, b,
    output logic [31:0] aux
);
    `include "tb_common.svh"
    logic [7:0] txt [20];
    logic [4:0] cg [32];
    logic [4:0] text_addr, cg_addr, cursor;
    logic       cursor_on, display_on, msg_led;
    logic [9:0] buttons;
    logic [4:0] st;
    always_ff @(posedge clk) if (load) begin : gen
        logic [31:0] s;
        s = seed;
        for (int i = 0; i < 20; i++) begin
            s = xs(s);
            // mostly printable characters, some CG RAM codes (0-3) and upper-half codes
            txt[i] <= s[9:8] == 2'b00 ? {6'd0, s[1:0]} : s[10] ? s[7:0] : {1'b0, 2'b01 + {1'b0, s[6]}, s[4:0]};
        end
        for (int i = 0; i < 32; i++) begin s = xs(s); cg[i] <= s[4:0]; end
        s = xs(s);
        cursor <= 5'(s[4:0] % 5'd20);
        cursor_on <= s[5] | seed[0];
        display_on <= seed != 32'h10;
        buttons <= s[15:6] & s[25:16] & {s[31:26], s[3:0]};
        s = xs(s);
        msg_led <= s[0];
        st <= s[5:1];
    end
    mt32_lcd_video #(.FONT_HEX(`MT32_FONT_HEX), .PANEL_DIR(`MT32_PANEL_DIR)) dut (
        .clk, .text_addr, .text(txt[text_addr < 5'd20 ? text_addr : 5'd0]), .cg_addr,
        .cg_row(cg[cg_addr]), .cursor, .cursor_on, .display_on, .buttons, .msg_led,
        .rom_ok(st[0]), .loading(st[1]), .running(st[2]), .midi_led(st[3]),
        .underrun_seen(st[4]),
        .hs, .vs, .hblank, .vblank, .r, .g, .b);
    assign aux = 32'd0;
endmodule
