// Self-checking test of the Synclavier's MIDI out line (review 2026-11-26
// sync-hdl #5): synthframe_midi_tx as synclavier_core wires it to dev_midi's
// byte queue (valid/ready, ready = !busy), 8N1 at 31250 baud, or 9600 with
// `alt` (changed at random, while a byte is on the wire too; with +alt_idle
// only between bytes). Decodes txd
// and checks every byte, in order, and every bit's width: 1600 clocks (50
// MHz) at 31250, 5208 at 9600, the speed `alt` had when the byte started.
module midi_out_tb;
    localparam int DIV = 1600, ALT_DIV = 5208;
    logic       clk = 0, rst = 1, alt = 0, valid = 0;
    logic [7:0] data = 0;
    logic       ready, txd, busy;
    int         errors = 0, sent = 0, got = 0, n_alt = 0;
    bit         alt_idle = 0;             // +alt_idle=1: alt changes only between bytes

    synthframe_midi_tx #(.CLK_HZ(50_000_000), .DEPTH_BITS(1)) dut (
        .clk, .rst, .alt, .valid(valid && ready), .data, .txd, .busy, .dropped()
    );
    assign ready = !busy;

    always #10 clk = !clk;

    logic [7:0] want [$];
    logic       want_alt [$];
    // the source: a byte when ready, then a random gap; alt flips at random
    always @(posedge clk) if (!rst) begin
        if (valid && ready) begin
            want.push_back(data); want_alt.push_back(alt); sent++;
            valid <= 1'b0;
        end else if (!valid && (!alt_idle || ready) && $urandom_range(3000) == 0 && sent < 300) begin
            valid <= 1'b1; data <= 8'($urandom);
        end
        if (!alt_idle ? $urandom_range(20000) == 0 : (ready && !valid && $urandom_range(1000) == 0))
            alt <= !alt;
    end

    longint t = 0;
    always @(posedge clk) t++;

    initial begin
        if ($test$plusargs("alt_idle")) alt_idle = 1;
        repeat (4) @(posedge clk);
        @(negedge clk); rst = 0;
        fork
            forever begin
                logic [7:0] b;
                int d;
                logic a;
                @(negedge txd);
                if (want.size() == 0) begin errors++; $display("a start bit with no byte sent"); continue; end
                a = want_alt[0];
                d = a ? ALT_DIV : DIV;
                // start bit: low for d clocks; data bits; stop bit high for d clocks
                for (int k = 0; k < 10; k++) begin
                    logic level;
                    int   len;
                    len = 0;
                    @(negedge clk);                 // the bit's d clocks, sampled between edges
                    level = txd;
                    repeat (d - 1) begin
                        @(negedge clk);
                        if (txd !== level) begin
                            errors++;
                            if (errors < 10) $display("byte %0d bit %0d: %0d clocks, expected %0d", got, k, len + 1, d);
                            level = txd;
                        end
                        len++;
                    end
                    if (k == 0 && level !== 1'b0) begin errors++; $display("byte %0d: no start bit", got); end
                    if (k >= 1 && k <= 8) b[k - 1] = level;
                    if (k == 9 && level !== 1'b1) begin errors++; $display("byte %0d: no stop bit", got); end
                end
                if (b !== want[0]) begin
                    errors++; if (errors < 10) $display("byte %0d = %02x, expected %02x", got, b, want[0]);
                end
                if (a) n_alt++;
                void'(want.pop_front()); void'(want_alt.pop_front());
                got++;
            end
            begin
                while (got < 300 && t < 64'd200_000_000) @(posedge clk);
            end
        join_any
        if (errors == 0 && got == 300 && n_alt > 20 && got - n_alt > 20)
            $display("midi_out OK (%0d bytes, %0d at 9600)", got, n_alt);
        else $display("midi_out FAILED: %0d errors, %0d of %0d bytes, %0d at 9600", errors, got, sent, n_alt);
        $finish;
    end
endmodule
