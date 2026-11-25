// SPDX-License-Identifier: GPL-3.0-or-later
// Proteus 2000 front panel from a PS/2 keyboard (hps_io ps2_key), with the key names of MAME's
// proteus2k input ports (emup2k.cpp); Control and Audition confirmed by the LEDs they light
// (cores/proteus/docs/p2k-findings.md):
//   A Control (bit 7)       Enter Home/Enter (8)    S Save/Copy (9)      M Multi (10)
//   Space Audition (11)     right: cursor right (12) left: cursor left (13)
//   E Edit (14)             Q Master (15)
//   - / =  or  down / up: the data-entry encoder, one step per key press
// buttons[n] = panel bit n pressed (0x800000 reads them active low).
module p2k_kbd (
    input  logic        clk,
    input  logic        rst,
    input  logic [10:0] ps2_key,      // [10] toggles per event, [9] pressed, [8] extended, [7:0] code
    output logic [15:7] buttons,
    output logic        enc_step,     // one clock per step
    output logic        enc_dir       // 1: up
);
    logic toggle_d;
    always_ff @(posedge clk) begin : b_kbd
        logic [3:0] bn;
        logic       hit;
        toggle_d <= ps2_key[10];
        enc_step <= 1'b0;
        if (rst) buttons <= '0;
        else if (ps2_key[10] != toggle_d) begin
            hit = 1'b1;
            bn = 4'd0;
            if (!ps2_key[8]) begin
                case (ps2_key[7:0])
                    8'h1c: bn = 4'd7;    // A
                    8'h5a: bn = 4'd8;    // Enter
                    8'h1b: bn = 4'd9;    // S
                    8'h3a: bn = 4'd10;   // M
                    8'h29: bn = 4'd11;   // Space
                    8'h24: bn = 4'd14;   // E
                    8'h15: bn = 4'd15;   // Q
                    8'h4e, 8'h7b: begin hit = 1'b0; if (ps2_key[9]) begin enc_step <= 1'b1; enc_dir <= 1'b0; end end   // - (keypad -)
                    8'h55, 8'h79: begin hit = 1'b0; if (ps2_key[9]) begin enc_step <= 1'b1; enc_dir <= 1'b1; end end   // = (keypad +)
                    default: hit = 1'b0;
                endcase
            end else begin
                case (ps2_key[7:0])
                    8'h5a: bn = 4'd8;    // keypad Enter
                    8'h74: bn = 4'd12;   // right
                    8'h6b: bn = 4'd13;   // left
                    8'h72: begin hit = 1'b0; if (ps2_key[9]) begin enc_step <= 1'b1; enc_dir <= 1'b0; end end   // down
                    8'h75: begin hit = 1'b0; if (ps2_key[9]) begin enc_step <= 1'b1; enc_dir <= 1'b1; end end   // up
                    default: hit = 1'b0;
                endcase
            end
            if (hit) buttons[bn] <= ps2_key[9];
        end
    end
endmodule
