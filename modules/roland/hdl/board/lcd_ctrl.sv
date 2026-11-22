// The SC-55's LCD controller state, as the reference's LCD_Write keeps it
// (lcd.cpp): display data (80 bytes), character generator RAM (64 x 5 bits),
// address counters and modes. The MCU writes commands (rs 0, F105) and data
// (rs 1, F104).
module lcd_ctrl (
    input  logic       clk,
    input  logic       rst,
    input  logic       we,
    input  logic       rs,
    input  logic [7:0] data,
    input  logic       enable_in,     // LCD_Enable from the MCU
    // state, read by the renderer (another clock: display only)
    output logic [7:0] dd [80],
    output logic [4:0] cg [64],
    output logic       enable,
    output logic [6:0] dd_ram,
    output logic       c_flag
);
    logic       n_, id, ram_mode;
    logic [5:0] cg_ram;

    always_ff @(posedge clk) begin
        enable <= enable_in;
        if (rst) begin
            for (int i = 0; i < 80; i++) dd[i] <= 8'h00;
            for (int i = 0; i < 64; i++) cg[i] <= 5'h00;
            n_ <= 1'b0; id <= 1'b0; ram_mode <= 1'b0; cg_ram <= 6'd0; dd_ram <= 7'd0; c_flag <= 1'b0;
        end else begin
            if (we) begin
                if (!rs) begin
                    if (data[7:5] == 3'b001) n_ <= data[3];                         // function set (DL, N, F)
                    else if (data[7:3] == 5'b00001) c_flag <= data[1];              // display control (D, C, B)
                    else if (data == 8'h01) begin
                        dd_ram <= 7'd0; id <= 1'b1;
                        for (int i = 0; i < 80; i++) dd[i] <= 8'h20;
                    end
                    else if (data == 8'h02) dd_ram <= 7'd0;
                    else if (data[7:2] == 6'b000001) id <= data[1];                 // entry mode (ID, S)
                    else if (data[7:6] == 2'b01) begin cg_ram <= data[5:0]; ram_mode <= 1'b0; end
                    else if (data[7]) begin dd_ram <= data[6:0]; ram_mode <= 1'b1; end
                end else if (!ram_mode) begin
                    cg[cg_ram] <= data[4:0];
                    cg_ram <= id ? cg_ram + 6'd1 : cg_ram - 6'd1;
                end else begin
                    if (n_) begin
                        if (dd_ram[6]) begin
                            if (dd_ram[5:0] < 6'd40) dd[7'(dd_ram[5:0]) + 7'd40] <= data;
                        end else if (dd_ram[5:0] < 6'd40) dd[7'(dd_ram[5:0])] <= data;
                    end else if (dd_ram < 7'd80) dd[dd_ram] <= data;
                    dd_ram <= id ? dd_ram + 7'd1 : dd_ram - 7'd1;
                end
            end
        end
    end
endmodule
