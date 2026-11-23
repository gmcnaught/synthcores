// SPDX-License-Identifier: GPL-2.0-or-later
// JV-880 front panel from a PS/2 keyboard (hps_io ps2_key), with the reference frontend's key map
// (Nuked-SC55 standard/lcd_sdl.cpp button_map_jv880):
//   Q tone select   W edit      E system   R rhythm   T utility   P preview
//   A patch/perform S mute      D monitor  F compare  G enter     Tab data
//   arrow left / right: cursor < / >
//   , / .  or  arrow down / up: the encoder, one detent per key press
// buttons = mcu.button_pressed bit numbers (mcu.h MCU_BUTTON_* for the JV-880).
module jv880_kbd (
    input  logic        clk,
    input  logic        rst,
    input  logic [10:0] ps2_key,      // [10] toggles per event, [9] pressed, [8] extended, [7:0] code
    output logic [13:0] buttons,
    output logic        enc_we,       // one clock per detent
    output logic        enc_dir       // 1: up
);
    logic toggle_d;
    always_ff @(posedge clk) begin : b_jv880_kbd
        logic [3:0] bn;
        logic       hit;
        toggle_d <= ps2_key[10];
        enc_we <= 1'b0;
        if (rst) buttons <= 14'd0;
        else if (ps2_key[10] != toggle_d) begin
            hit = 1'b1;
            bn = 4'd0;
            if (!ps2_key[8]) begin
                case (ps2_key[7:0])
                    8'h4d: bn = 4'd9;    // P preview
                    8'h0d: bn = 4'd4;    // Tab data
                    8'h15: bn = 4'd2;    // Q tone select
                    8'h1c: bn = 4'd10;   // A patch/perform
                    8'h1d: bn = 4'd11;   // W edit
                    8'h24: bn = 4'd12;   // E system
                    8'h2d: bn = 4'd13;   // R rhythm
                    8'h2c: bn = 4'd8;    // T utility
                    8'h1b: bn = 4'd3;    // S mute
                    8'h23: bn = 4'd5;    // D monitor
                    8'h2b: bn = 4'd6;    // F compare
                    8'h34: bn = 4'd7;    // G enter
                    8'h41: begin hit = 1'b0; if (ps2_key[9]) begin enc_we <= 1'b1; enc_dir <= 1'b0; end end   // ,
                    8'h49: begin hit = 1'b0; if (ps2_key[9]) begin enc_we <= 1'b1; enc_dir <= 1'b1; end end   // .
                    default: hit = 1'b0;
                endcase
            end else begin
                case (ps2_key[7:0])
                    8'h6b: bn = 4'd0;    // left: cursor <
                    8'h74: bn = 4'd1;    // right: cursor >
                    8'h72: begin hit = 1'b0; if (ps2_key[9]) begin enc_we <= 1'b1; enc_dir <= 1'b0; end end   // down
                    8'h75: begin hit = 1'b0; if (ps2_key[9]) begin enc_we <= 1'b1; enc_dir <= 1'b1; end end   // up
                    default: hit = 1'b0;
                endcase
            end
            if (hit) buttons[bn] <= ps2_key[9];
        end
    end
endmodule
