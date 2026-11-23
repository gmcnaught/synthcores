// CZ-101 panel buttons from a PS/2 keyboard (MiSTer ps2_key), with the keys MAME's cz101
// driver assigns (INPUT_PORTS cz101, kc9-kc15). The 49 music keys (kc0-kc8) are played over
// MIDI and stay released here.
//
// keys[i] is the key-matrix row i as the firmware reads it, active low. Not from the keyboard:
// kc10 bit 5 Memory Protect and kc15 bit 6 Auto Power Off (switches, from the OSD),
// kc13 bit 0 the cartridge sense (1 = no cartridge, MAME casio_ram_cart_device::sense).

module cz101_kbd (
    input  logic        clk,
    input  logic        reset,
    input  logic [10:0] ps2_key,         // [10] toggles per event, [9] pressed, [8] E0, [7:0] code
    input  logic        mem_protect,     // switch on
    input  logic        auto_power_off,  // switch on
    input  logic        cart_present,
    output logic [7:0]  keys [16]
);

    // pressed[r][b]: panel button at row 9 + r, bit b
    logic [7:0] pressed [7];
    logic       toggle_q;

    always_ff @(posedge clk) begin
        if (reset) begin
            for (int r = 0; r < 7; r++) pressed[r] <= 8'h00;
            toggle_q <= ps2_key[10];
        end else if (ps2_key[10] != toggle_q) begin
            logic dn;
            toggle_q <= ps2_key[10];
            dn = ps2_key[9];
            if (!ps2_key[8]) case (ps2_key[7:0])
                // kc9: W Q 2 1 3 4 5 6
                8'h1D: pressed[0][0] <= dn;  8'h15: pressed[0][1] <= dn;
                8'h1E: pressed[0][2] <= dn;  8'h16: pressed[0][3] <= dn;
                8'h26: pressed[0][4] <= dn;  8'h25: pressed[0][5] <= dn;
                8'h2E: pressed[0][6] <= dn;  8'h36: pressed[0][7] <= dn;
                // kc10: E R T Y U, keypad 0 (Select)
                8'h24: pressed[1][0] <= dn;  8'h2D: pressed[1][1] <= dn;
                8'h2C: pressed[1][2] <= dn;  8'h35: pressed[1][3] <= dn;
                8'h3C: pressed[1][4] <= dn;  8'h70: pressed[1][6] <= dn;
                // kc11: keypad 1-8 (Tone 1-8)
                8'h69: pressed[2][0] <= dn;  8'h72: pressed[2][1] <= dn;
                8'h7A: pressed[2][2] <= dn;  8'h6B: pressed[2][3] <= dn;
                8'h73: pressed[2][4] <= dn;  8'h74: pressed[2][5] <= dn;
                8'h6C: pressed[2][6] <= dn;  8'h75: pressed[2][7] <= dn;
                // kc13: A S D F G H J
                8'h1C: pressed[4][1] <= dn;  8'h1B: pressed[4][2] <= dn;
                8'h23: pressed[4][3] <= dn;  8'h2B: pressed[4][4] <= dn;
                8'h34: pressed[4][5] <= dn;  8'h33: pressed[4][6] <= dn;
                8'h3B: pressed[4][7] <= dn;
                // kc14: , Z X C V B N M
                8'h41: pressed[5][0] <= dn;  8'h1A: pressed[5][1] <= dn;
                8'h22: pressed[5][2] <= dn;  8'h21: pressed[5][3] <= dn;
                8'h2A: pressed[5][4] <= dn;  8'h32: pressed[5][5] <= dn;
                8'h31: pressed[5][6] <= dn;  8'h3A: pressed[5][7] <= dn;
                // kc15: K L . / - =
                8'h42: pressed[6][0] <= dn;  8'h4B: pressed[6][1] <= dn;
                8'h49: pressed[6][2] <= dn;  8'h4A: pressed[6][3] <= dn;
                8'h4E: pressed[6][4] <= dn;  8'h55: pressed[6][5] <= dn;
                default: ;
            endcase else case (ps2_key[7:0])
                // kc12: Down Up Left Right End Home Insert Delete
                8'h72: pressed[3][0] <= dn;  8'h75: pressed[3][1] <= dn;
                8'h6B: pressed[3][2] <= dn;  8'h74: pressed[3][3] <= dn;
                8'h69: pressed[3][4] <= dn;  8'h6C: pressed[3][5] <= dn;
                8'h70: pressed[3][6] <= dn;  8'h71: pressed[3][7] <= dn;
                default: ;
            endcase
        end
    end

    always_comb begin
        for (int i = 0; i < 9; i++) keys[i] = 8'hFF;
        for (int r = 0; r < 7; r++) keys[9 + r] = ~pressed[r];
        keys[10][5] = !mem_protect;
        keys[10][7] = 1'b1;                  // P (Reset RAM): not mapped
        keys[13][0] = !cart_present;
        keys[15][6] = !auto_power_off;
        keys[15][7] = 1'b1;
    end

endmodule
