// Self-checking test of hdl/dev/dev_terminal.sv's input FIFO (review
// 2026-11-26 sync-hdl #8): 300 host bytes into a 255-byte FIFO (FIFO_BITS
// 8), then D50 reads at 76800 baud. The receiver takes the first byte at
// once, so the first 256 bytes must read back in order and the other 44 be
// dropped; before, the 256th wrote over the full FIFO and it read as empty.
module dev_terminal_tb;
    logic        clk = 0, rst = 1;
    logic        sel = 0, rd = 0, wr = 0, reg_sel = 0, tick = 0;
    logic [15:0] wdata = 0, rdata;
    logic        ack, hold, irq, host_rx_valid = 0, host_tx_valid;
    logic [7:0]  host_rx_data = 0, host_tx_data;
    logic        ld_we = 0;
    logic [3:0]  ld_addr = 0;
    logic [31:0] ld_data = 0;
    int          errors = 0, reads = 0;

    dev_terminal #(.FIFO_BITS(8)) dut (
        .clk, .rst, .device_bits(16'd0), .sel, .rd, .wr, .reg_sel, .wdata, .rdata, .ack, .hold,
        .tick, .tick_ns(10'd1000), .irq, .host_rx_valid, .host_rx_data, .host_tx_valid, .host_tx_data,
        .ld_we, .ld_addr, .ld_data
    );

    always #5 clk = !clk;

    initial begin
        repeat (2) @(negedge clk);
        ld_we = 1; ld_addr = 4'd0; ld_data = 32'd7;         // 76800 baud
        @(negedge clk); ld_we = 0;
        rst = 0;
        for (int i = 0; i < 300; i++) begin
            @(negedge clk); host_rx_valid = 1; host_rx_data = 8'(i);
        end
        @(negedge clk); host_rx_valid = 0;
        for (int i = 0; i < 400; i++) begin
            int n;
            n = 0;
            while (dut.rx_wait > 0 && n < 200) begin   // the character's time
                @(negedge clk); tick = 1; @(negedge clk); tick = 0; n++;
            end
            @(negedge clk);
            if (!(dut.char_rdy || (dut.fifo_any && dut.rx_wait <= 0))) break;
            sel = 1; rd = 1; reg_sel = 0;
            @(posedge clk);
            if (ack) begin
                if (rdata[7:0] != 8'(reads)) begin
                    errors++; if (errors < 10) $display("read %0d: %0d", reads, rdata[7:0]);
                end
                reads++;
            end
            @(negedge clk); sel = 0; rd = 0;
        end
        if (errors == 0 && reads == 256) $display("dev_terminal OK (%0d of 300 bytes read, in order)", reads);
        else $display("dev_terminal FAILED: %0d errors, %0d bytes read", errors, reads);
        $finish;
    end
endmodule
