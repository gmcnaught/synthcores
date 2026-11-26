// Self-checking test of the SD word port (review 2026-11-26 sync-hdl #2):
// hdl/core/sd_word_arb.sv in front of hdl/core/sd_floppy.sv, an hps_io SD
// side that answers its block reads and writes from a byte store, and two
// requesters as dev_floppy (drive 0) and dev_scsi (drive 3) make them: hold
// rd/wr until the ack; random word reads, and whole blocks written in order
// (sd_floppy writes a block back after its word 255). Each requester checks
// every word it reads against what it wrote last (or the store's pattern).
// Three in four SCSI requests (while the floppy side runs) start on the clock
// a floppy word is acked (the collision); the others start 30-200 clocks
// after the last (the CPU makes them, an instruction or more apart). The SCSI
// image is one 32-block line (no background reads). +ops=N: operations per
// requester.
module sd_word_arb_tb;
    logic        clk = 0, rst = 1;
    logic [34:0] f_addr = 0, s_addr = 0;
    logic        f_rd = 0, f_wr = 0, s_rd = 0, s_wr = 0;
    logic [15:0] f_wdata = 0, s_wdata = 0;
    logic        f_ack, s_ack, t_ack, t_own;
    logic [34:0] m_addr;
    logic        m_rd, m_wr, m_ack;
    logic [15:0] m_wdata, m_rdata;
    logic [2:0]  m_ack_drive;
    logic [5:0]  img_mounted = 0;
    logic [63:0] img_size = 0;
    logic [31:0] sd_lba [6];
    logic [5:0]  sd_rd, sd_wr, sd_ack = 0;
    logic [5:0]  blk_cnt3, blk_cnt4, blk_cnt5;
    logic [13:0] sd_buff_addr = 0;
    logic [7:0]  sd_buff_dout = 0, sd_buff_din;
    logic        sd_buff_wr = 0;
    int          errors = 0, collisions = 0, f_ops = 0, s_ops = 0, ops = 3000;

    sd_word_arb arb (
        .clk, .rst,
        .f_addr, .f_rd, .f_wr, .f_wdata, .f_ack,
        .s_addr, .s_rd, .s_wr, .s_wdata, .s_ack,
        .t_addr(35'd0), .t_rd(1'b0), .t_ack, .t_own,
        .m_addr, .m_rd, .m_wr, .m_wdata, .m_ack, .m_ack_drive
    );

    sd_floppy media (
        .clk, .rst,
        .addr(m_addr), .rd(m_rd), .wr(m_wr), .wdata(m_wdata), .rdata(m_rdata), .ack(m_ack),
        .ack_drive(m_ack_drive),
        .present(), .protect(), .protect3(), .sectors0(), .sectors1(), .blocks2(), .blocks3(),
        .blocks4(), .protect4(), .blocks5(), .active(),
        .img_mounted, .img_readonly(1'b0), .img_size,
        .sd_lba0(sd_lba[0]), .sd_lba1(sd_lba[1]), .sd_lba2(sd_lba[2]), .sd_lba3(sd_lba[3]),
        .sd_lba4(sd_lba[4]), .sd_lba5(sd_lba[5]),
        .sd_rd, .sd_wr, .sd_blk_cnt3(blk_cnt3), .sd_blk_cnt4(blk_cnt4), .sd_blk_cnt5(blk_cnt5),
        .sd_ack, .sd_buff_addr, .sd_buff_dout, .sd_buff_din, .sd_buff_wr,
        .hint(1'b0), .hint_drive(3'd0), .hint_blk(24'd0), .probe_drive(3'd0), .probe_blk(24'd0),
        .probe_hit()
    );

    always #5 clk = !clk;

    // the SD card: bytes written back, else a pattern of (drive, block, byte)
    logic [7:0] store [longint];
    function automatic logic [7:0] sd_byte(input int d, input longint b, input int i);
        longint key = (longint'(d) << 40) | (b << 9) | longint'(i);
        if (store.exists(key)) return store[key];
        return 8'((d * 131 + b * 29 + i * 7 + (i >> 3) * 3) & 255);
    endfunction
    function automatic logic [15:0] word_of(input int d, input longint b, input int w);
        return {sd_byte(d, b, 2 * w), sd_byte(d, b, 2 * w + 1)};
    endfunction

    // hps_io's side of sd_rd / sd_wr
    initial forever begin
        @(negedge clk);
        for (int d = 0; d < 6; d++) begin
            if (sd_rd[d]) begin
                int n;
                longint lba;
                n = d == 3 ? int'(blk_cnt3) + 1 : 1;
                lba = longint'(sd_lba[d]);
                repeat (5) @(negedge clk);
                sd_ack[d] = 1;
                @(negedge clk);
                for (int b = 0; b < n; b++)
                    for (int i = 0; i < 512; i++) begin
                        sd_buff_addr = {5'(b), 9'(i)}; sd_buff_dout = sd_byte(d, lba + b, i); sd_buff_wr = 1;
                        @(negedge clk);
                        sd_buff_wr = 0;
                    end
                sd_ack[d] = 0;
                break;
            end
            if (sd_wr[d]) begin
                longint lba;
                lba = longint'(sd_lba[d]);
                repeat (5) @(negedge clk);
                sd_ack[d] = 1;
                sd_buff_addr = 0;
                @(negedge clk);
                for (int i = 0; i < 512; i++) begin
                    sd_buff_addr = 14'(i);
                    @(negedge clk);
                    store[(longint'(d) << 40) | (lba << 9) | longint'(i)] = sd_buff_din;
                end
                sd_ack[d] = 0;
                break;
            end
        end
    end

    // what each requester wrote (words not here read as the store has them)
    logic [15:0] shadow [longint];
    function automatic logic [15:0] expect_word(input int d, input longint b, input int w);
        longint key = (longint'(d) << 40) | (b << 8) | longint'(w);
        if (shadow.exists(key)) return shadow[key];
        return word_of(d, b, w);
    endfunction

    // one access of a requester; returns after its ack
    task automatic access(input bit scsi, input bit is_wr, input int d, input longint b, input int w,
                          input logic [15:0] v);
        logic [15:0] want;
        int waited = 0;
        want = expect_word(d, b, w);
        if (scsi) begin s_addr = {3'(d), 24'(b), 8'(w)}; s_wdata = v; s_rd = !is_wr; s_wr = is_wr; end
        else      begin f_addr = {3'(d), 24'(b), 8'(w)}; f_wdata = v; f_rd = !is_wr; f_wr = is_wr; end
        forever begin
            @(posedge clk);
            if (scsi ? s_ack : f_ack) break;
            if (++waited > 200000) begin
                errors++; $display("%s: no ack for drive %0d block %0d word %0d", scsi ? "SCSI" : "floppy", d, b, w);
                break;
            end
        end
        if (is_wr) shadow[(longint'(d) << 40) | (b << 8) | longint'(w)] = v;
        else if (m_rdata !== want) begin
            errors++;
            if (errors < 20) $display("%s read drive %0d block %0d word %0d = %04x, expected %04x",
                                      scsi ? "SCSI" : "floppy", d, b, w, m_rdata, want);
        end
        @(negedge clk);
        if (scsi) begin s_rd = 0; s_wr = 0; end else begin f_rd = 0; f_wr = 0; end
    endtask

    logic f_done = 0, s_done = 0;
    initial begin
        if ($value$plusargs("ops=%d", ops)) ;
        repeat (4) @(posedge clk);
        @(negedge clk); rst = 0;
        @(negedge clk); img_size = 64'd64 * 512; img_mounted = 6'b000001;
        @(negedge clk); img_mounted = 0;
        @(negedge clk); img_size = 64'd32 * 512; img_mounted = 6'b001000;
        @(negedge clk); img_mounted = 0;
        fork
            begin                                      // dev_floppy: drive 0, sectors 0-3
                for (int n = 0; n < ops; n++) begin
                    if ($urandom_range(19) == 0) begin   // a sector written, as dev_floppy flushes one
                        int b = $urandom_range(3);
                        for (int w = 0; w < 256; w++) access(0, 1, 0, b, w, 16'($urandom));
                    end else access(0, 0, 0, $urandom_range(3), $urandom_range(255), 16'd0);
                    f_ops++;
                    repeat ($urandom_range(2)) @(negedge clk);
                end
                f_done = 1;
            end
            begin                                      // dev_scsi: drive 3 (one 32-block line), CPU-paced
                for (int n = 0; n < ops; n++) begin
                    if ($urandom_range(3) != 0 && !f_done) begin
                        // start on the clock a floppy word is acked
                        int k = 0;
                        do begin @(negedge clk); k++; end
                        while (!(m_ack && (f_rd || f_wr)) && k < 5000 && !f_done);
                        if (m_ack && (f_rd || f_wr)) collisions++;
                    end else repeat ($urandom_range(200, 30)) @(negedge clk);
                    if ($urandom_range(19) == 0) begin   // a block written, as a SCSI WRITE does
                        int b = $urandom_range(31);
                        for (int w = 0; w < 256; w++) access(1, 1, 3, b, w, 16'($urandom));
                    end else access(1, 0, 3, $urandom_range(31), $urandom_range(255), 16'd0);
                    s_ops++;
                end
                s_done = 1;
            end
        join
        if (errors == 0 && collisions >= ops / 20)
            $display("sd_word_arb OK (%0d floppy and %0d SCSI accesses, %0d SCSI requests on a floppy ack)",
                     f_ops, s_ops, collisions);
        else $display("sd_word_arb FAILED: %0d errors, %0d collisions", errors, collisions);
        $finish;
    end
endmodule
