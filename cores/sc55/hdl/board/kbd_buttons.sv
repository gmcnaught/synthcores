// Front-panel buttons from a PS/2 keyboard (hps_io ps2_key), with the
// reference frontend's key map (jcmoyer standard/lcd_sdl.cpp):
//   Q power   W all    E mute   R part<  T part>  Y inst<  U inst>
//   I key<    O key>   P level< [ level>
//   A ch<     S ch>    D pan<   F pan>   G rev<   H rev>   J cho<  K cho>
//   arrow left / right: part< / part>
// buttons = mcu.button_pressed bit numbers (mcu.h MCU_BUTTON_*).
module kbd_buttons (
    input  logic        clk,
    input  logic        rst,
    input  logic [10:0] ps2_key,      // [10] toggles per event, [9] pressed, [8] extended, [7:0] code
    output logic [31:0] buttons
);
    logic toggle_d;
    always_ff @(posedge clk) begin
        toggle_d <= ps2_key[10];
        if (rst) buttons <= 32'd0;
        else if (ps2_key[10] != toggle_d) begin
            logic [5:0] bn;
            bn = 6'd63;
            if (!ps2_key[8]) begin
                case (ps2_key[7:0])
                    8'h15: bn = 6'd0;   // Q power
                    8'h1d: bn = 6'd6;   // W inst all
                    8'h24: bn = 6'd5;   // E inst mute
                    8'h2d: bn = 6'd22;  // R part <
                    8'h2c: bn = 6'd14;  // T part >
                    8'h35: bn = 6'd3;   // Y inst <
                    8'h3c: bn = 6'd4;   // U inst >
                    8'h43: bn = 6'd16;  // I key shift <
                    8'h44: bn = 6'd17;  // O key shift >
                    8'h4d: bn = 6'd20;  // P level <
                    8'h54: bn = 6'd21;  // [ level >
                    8'h1c: bn = 6'd8;   // A midi ch <
                    8'h1b: bn = 6'd9;   // S midi ch >
                    8'h23: bn = 6'd12;  // D pan <
                    8'h2b: bn = 6'd13;  // F pan >
                    8'h34: bn = 6'd18;  // G reverb <
                    8'h33: bn = 6'd19;  // H reverb >
                    8'h3b: bn = 6'd10;  // J chorus <
                    8'h42: bn = 6'd11;  // K chorus >
                    default: ;
                endcase
            end else begin
                case (ps2_key[7:0])
                    8'h6b: bn = 6'd22;  // left: part <
                    8'h74: bn = 6'd14;  // right: part >
                    default: ;
                endcase
            end
            if (bn != 6'd63) buttons[bn[4:0]] <= ps2_key[9];
        end
    end
endmodule
