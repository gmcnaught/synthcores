// Uptime test of the poly voice time (review 2026-11-26 sync-hdl #1):
// dev_poly and poly_box wired as in able_system / synclavier_core, run for
// +run_ns=N ns of voice time from +voice_t0=T (both read by the two devices
// too). The output must not depend on T: the test runs T = 0 and T just
// before 2^48 ns (78.2 h) and compares the summary lines.
//   +hold=1  voices on CPU time: no voice tick while poly_box holds `stall`
//            (the CPU is held, voice time stops). A device that never
//            catches up hangs here, so a clock budget fails the run.
//   +hold=0  voices on real time: ticks never stop.
//   +stim=F  register writes made before time starts: lines "W r v"
//            (r 0 D154 .. 3 D157), as tests/hdl/test_board.py writes them.
// Voice ticks of 333 ns, one per 17 clocks (50 MHz); FM sample instants on
// dev_fm's 768 kHz grid (j x 10^9 / 768000 ns after reset), each sample a few
// clocks after its instant.
module poly_wrap_tb;
    logic        clk = 0, rst = 1;
    logic        tick = 0;
    logic [9:0]  tick_ns = 0;
    logic        sel = 0, rd = 0, wr = 0;
    logic [1:0]  reg_sel = 0;
    logic [15:0] wdata = 0, rdata;
    logic        ack;
    logic [23:0] m_addr;
    logic        m_rd, m_wr, m_wide, m_ack = 0;
    logic [15:0] m_wdata, m_rdata = 0;
    logic        p_valid;
    logic signed [31:0] p_l, p_r;
    logic [23:0] s_addr;
    logic        s_rd, s_ack = 0, s_rvalid = 0;
    logic [63:0] s_rdata = 0;
    logic        hold = 1'b1;
    logic        inst = 0, fm_valid = 0;
    logic [9:0]  inst_d = 0;
    logic signed [39:0] fm_l = 0, fm_r = 0;
    logic        out_valid, stall;
    logic signed [26:0] out_l, out_r;
    logic [15:0] late;

    dev_poly dut (
        .clk, .rst, .enable(1'b1), .bin_count(3'd1), .size_units(11'd1),
        .last_voices(6'd32), .mono(1'b0), .ftest(2'd0), .tick, .tick_ns,
        .sel, .rd, .wr, .reg_sel, .wdata, .rdata, .ack,
        .m_addr, .m_rd, .m_wr, .m_wide, .m_wdata, .m_rdata, .m_ack, .clearing(),
        .s_addr, .s_rd, .s_ack, .s_rvalid, .s_rdata,
        .p_valid, .p_l, .p_r,
        .out_pair(4'd0), .rt_we(1'b0), .rt_voice(7'd0), .rt_side(1'b0), .rt_tag(5'd0),
        .q_voice(7'd0), .q_side(1'b0), .q_live()
    );

    poly_box box (
        .clk, .rst, .en(1'b1), .fm_level(2'd0), .poly_level(2'd0), .hold_ok(hold), .tick, .tick_ns,
        .inst, .inst_d, .fm_valid, .fm_l, .fm_r,
        .p_valid, .p_l, .p_r, .out_valid, .out_l, .out_r, .stall, .late
    );

    always #5 clk = !clk;

    // poly memory (64K words) behind a port that answers 4 clocks late
    logic [15:0] mem [1 << 16];
    int dly = 0;
    always @(posedge clk) begin
        m_ack <= 1'b0;
        if ((m_rd || m_wr) && !m_ack) begin
            if (dly == 3) begin
                dly <= 0;
                if (m_wr && m_wide) for (int j = 0; j < 512; j++) mem[{m_addr[15:9], 9'(j)}] <= m_wdata;
                else if (m_wr) mem[m_addr[15:0]] <= m_wdata;
                m_rdata <= mem[m_addr[15:0]];
                m_ack <= 1'b1;
            end else dly <= dly + 1;
        end
    end

    // the line fetch stream: beats back in order 12 clocks after the command
    longint      cyc = 0;
    longint      due [$];
    logic [63:0] dat [$];
    always @(posedge clk) begin
        cyc <= cyc + 1;
        s_ack <= 1'b0;
        s_rvalid <= 1'b0;
        if (due.size() > 0 && due[0] <= cyc) begin
            s_rvalid <= 1'b1; s_rdata <= dat[0];
            void'(due.pop_front()); void'(dat.pop_front());
        end
        if (s_rd && !s_ack && cyc % 3 != 1) begin
            s_ack <= 1'b1;
            due.push_back(cyc + 12);
            dat.push_back({mem[{s_addr[15:2], 2'd3}], mem[{s_addr[15:2], 2'd2}],
                           mem[{s_addr[15:2], 2'd1}], mem[{s_addr[15:2], 2'd0}]});
        end
    end

    // what comes out: counts and a running hash of P(m) and x'
    longint      n_p = 0, n_pnz = 0, n_out = 0, stall_clk = 0;
    longint      h = 64'hcbf29ce484222325;
    function automatic longint mix(input longint a, input longint v);
        return (a ^ v) * 64'h100000001b3;
    endfunction
    always @(posedge clk) begin
        if (stall) stall_clk++;
        if (p_valid) begin
            n_p++;
            if (p_l != 0 || p_r != 0) n_pnz++;
            h = mix(mix(h, longint'(p_l)), longint'(p_r));
        end
        if (out_valid) begin
            n_out++;
            h = mix(mix(h, longint'(out_l)), longint'(out_r));
        end
    end

    // FM samples, a few clocks after their instants
    int          fm_pend = 0, fm_wait = 0;
    logic [31:0] lfsr = 32'h1234_5678;
    always @(negedge clk) begin
        fm_valid = 0;
        if (fm_pend > 0) begin
            if (fm_wait > 0) fm_wait--;
            else begin
                lfsr = {lfsr[30:0], lfsr[31] ^ lfsr[21] ^ lfsr[1] ^ lfsr[0]};
                fm_valid = 1; fm_l = 40'($signed(lfsr)) <<< 4; fm_r = -(40'($signed(lfsr)) <<< 3);
                fm_pend--; fm_wait = 3;
            end
        end
    end

    task automatic access(input logic [1:0] r, input logic [15:0] v);
        @(negedge clk);
        sel = 1; reg_sel = r; rd = 0; wr = 1; wdata = v;
        do @(posedge clk); while (!ack);
        @(negedge clk);
        sel = 0; wr = 0;
    endtask

    initial begin
        string path;
        int fd, code, a, b, hv;
        byte k;
        longint run_ns = 3_000_000, t0 = 0, e = 0, j = 0, clocks = 0, budget;
        if ($value$plusargs("hold=%d", hv)) hold = hv != 0;
        if ($value$plusargs("run_ns=%d", run_ns)) ;
        if ($value$plusargs("voice_t0=%d", t0)) ;
        budget = run_ns / 333 * 17 * 4 + 200_000;
        for (int i = 0; i < (1 << 16); i++) mem[i] = 16'h5A5A;
        repeat (4) @(posedge clk);
        rst = 0;
        repeat (1100) @(posedge clk);
        if ($value$plusargs("stim=%s", path)) begin
            fd = $fopen(path, "r");
            while (!$feof(fd)) begin
                code = $fscanf(fd, " %c %d %d", k, a, b);
                if (code < 3) break;
                if (k == "W") access(2'(a), 16'(b));
            end
        end
        // voice time from here: one 333 ns tick per 17 clocks
        while (e < run_ns) begin
            repeat (16) begin @(negedge clk); clocks++; end
            while (hold && stall) begin
                @(negedge clk); clocks++;
                if (clocks > budget) break;
            end
            if (clocks > budget) break;
            @(negedge clk); clocks++;
            tick = 1; tick_ns = 10'd333;
            inst = 0; inst_d = 0;
            if (j * 1_000_000_000 / 768_000 < e + 333) begin
                inst = 1; inst_d = 10'(j * 1_000_000_000 / 768_000 - e);
                j++; fm_pend++;
            end
            e += 333;
            @(negedge clk); clocks++;
            tick = 0; inst = 0;
        end
        repeat (2000) @(posedge clk);
        if (clocks > budget)
            $display("poly_wrap FAILED: hung at %0d of %0d ns voice time (stall %0d, dev_poly now %0d next_rate %0d next_env %0d)",
                     e, run_ns, stall, dut.now, dut.next_rate, dut.next_env);
        else if (n_out != j || n_p == 0)
            $display("poly_wrap FAILED: %0d instants, %0d samples out, %0d P values", j, n_out, n_p);
        else
            $display("poly_wrap OK (hold %0d, %0d ns, %0d P values, %0d non-zero, %0d samples, late %0d, stall clocks %0d, hash %016x)",
                     hold, e, n_p, n_pnz, n_out, late, stall_clk, h);
        $finish;
    end
endmodule
