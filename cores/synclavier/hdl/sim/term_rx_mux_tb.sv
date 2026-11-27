// Self-checking test of hdl/core/term_rx_mux.sv (review 2026-11-26 sync-hdl
// #8): board-simulation bytes (a, single), read-back bursts (b, up to 40
// bytes on consecutive clocks) and keyboard sequences (c, 1 or 3 bytes on
// consecutive clocks, as ps2_ascii makes them) at random, often on the same
// clocks. Every byte must come out once, each source's in order, and a byte
// with nothing before it on the clock it comes.
module term_rx_mux_tb;
    logic       clk = 0, rst = 1;
    logic       a_valid = 0, b_valid = 0, c_valid = 0;
    logic [7:0] a_data = 0, b_data = 0, c_data = 0;
    logic       out_valid;
    logic [7:0] out_data, dropped;
    int         errors = 0, sent [3] = '{0, 0, 0}, got [3] = '{0, 0, 0}, meets = 0;

    term_rx_mux dut (.clk, .rst, .a_valid, .a_data, .b_valid, .b_data, .c_valid, .c_data,
                     .out_valid, .out_data, .dropped);

    always #5 clk = !clk;

    // byte = {source (2 bits), sequence number (6 bits)}: in order per source
    always @(posedge clk) if (!rst) begin
        if (a_valid + b_valid + c_valid > 1) meets++;
        if (a_valid && !(out_valid && out_data == a_data)) begin
            errors++; if (errors < 10) $display("a byte %02x not out at once", a_data);
        end
        if (out_valid) begin
            int s = out_data[7:6] - 1;
            if (s < 0 || s > 2 || out_data[5:0] != 6'(got[s])) begin
                errors++;
                if (errors < 10) $display("out %02x, expected source %0d's byte %0d", out_data, s, got[s] & 63);
            end
            if (s >= 0 && s <= 2) got[s]++;
        end
    end

    int b_left = 0, c_left = 0;
    initial begin
        repeat (4) @(posedge clk);
        @(negedge clk); rst = 0;
        for (int t = 0; t < 200000; t++) begin
            @(negedge clk);
            a_valid = $urandom_range(150) == 0;
            a_data = {2'd1, 6'(sent[0])};
            if (a_valid) sent[0]++;
            if (b_left == 0 && $urandom_range(100) == 0) b_left = $urandom_range(40, 1);
            b_valid = b_left > 0;
            b_data = {2'd2, 6'(sent[1])};
            if (b_valid) begin sent[1]++; b_left--; end
            if (c_left == 0 && $urandom_range(80) == 0) c_left = $urandom_range(1) ? 3 : 1;
            c_valid = c_left > 0;
            c_data = {2'd3, 6'(sent[2])};
            if (c_valid) begin sent[2]++; c_left--; end
        end
        @(negedge clk); a_valid = 0; b_valid = 0; c_valid = 0;
        repeat (100) @(posedge clk);
        for (int s = 0; s < 3; s++)
            if (got[s] != sent[s]) begin
                errors++; $display("source %0d: %0d bytes in, %0d out", s, sent[s], got[s]);
            end
        if (errors == 0 && dropped == 0 && meets > 500)
            $display("term_rx_mux OK (%0d, %0d, %0d bytes; %0d clocks with two at once)", sent[0], sent[1], sent[2], meets);
        else $display("term_rx_mux FAILED: %0d errors, %0d dropped, %0d meetings", errors, dropped, meets);
        $finish;
    end
endmodule
