// Self-checking test of hdl/core/boot_loader.sv's state mode (review
// 2026-11-26 sync-hdl #3): a state image (+image=FILE, tools/state_image.py's
// format, made by tests/hdl/test_board.py) on SD drive 2 behind a word port,
// external memory behind a write port. The loader must start the CPU with
// internal memory and every external memory word as the image has them
// (stored sectors, then zeros), and the header's register values.
module boot_loader_tb;
    logic        clk = 0, rst = 1;
    logic [16:0] ext_sectors;
    logic [1:0]  cpu_model;
    logic [15:0] cpu_d57, term_bits;
    logic        pvoice, cpu_rst, error;
    logic        ld_dev_we, ld_voice_we, dbg_we, ld_mem_we;
    logic [2:0]  ld_dev_sel;
    logic [12:0] ld_dev_addr;
    logic [31:0] ld_dev_data;
    logic [5:0]  ld_voice_addr;
    logic [63:0] ld_voice_data;
    logic [4:0]  dbg_addr;
    logic [15:0] dbg_wdata, ld_mem_addr, ld_mem_data;
    logic [25:0] fd_addr;
    logic        fd_rd, fd_ack = 0;
    logic [15:0] fd_rdata = 0;
    logic [23:0] xm_addr;
    logic        xm_wr, xm_ack = 0;
    logic [15:0] xm_wdata;
    logic        msg_valid;
    logic [7:0]  msg_data;

    boot_loader dut (
        .clk, .rst, .f0_present(1'b0), .state_present(1'b1), .floppy_ext_sectors(17'd0),
        .ext_sectors, .cpu_model, .cpu_d57, .term_bits, .mono(1'b0), .pvoice, .printer(1'b0),
        .cpu_rst, .error,
        .ld_dev_we, .ld_dev_sel, .ld_dev_addr, .ld_dev_data, .ld_voice_we, .ld_voice_addr, .ld_voice_data,
        .dbg_we, .dbg_addr, .dbg_wdata, .ld_mem_we, .ld_mem_addr, .ld_mem_data,
        .fd_addr, .fd_rd, .fd_rdata, .fd_ack, .xm_addr, .xm_wr, .xm_wdata, .xm_ack,
        .msg_valid, .msg_data
    );

    always #5 clk = !clk;

    logic [15:0] img [1 << 22];                   // the image, 16-bit words big-endian
    logic [15:0] xm [1 << 21];
    logic [15:0] mem [65536];
    logic [15:0] regs [32];
    int          xm_writes = 0, fd_reads = 0, img_words = 0;

    always @(posedge clk) begin
        fd_ack <= 1'b0;
        xm_ack <= 1'b0;
        if (fd_rd && !fd_ack) begin                // one clock late, as sd_floppy on a hit
            fd_ack <= 1'b1;
            fd_reads++;
            fd_rdata <= fd_addr[25:24] == 2'd2 && int'(fd_addr[23:0]) < img_words ? img[fd_addr[23:0]] : 16'hDEAD;
        end
        if (xm_wr && !xm_ack) begin
            xm_ack <= 1'b1;
            xm[xm_addr[20:0]] <= xm_wdata;
            xm_writes++;
        end
        if (ld_mem_we) mem[ld_mem_addr] <= ld_mem_data;
        if (dbg_we) regs[dbg_addr] <= dbg_wdata;
    end

    initial begin
        string path;
        int fd, n, errors = 0, ent_blocks, base_mem, base_xm;
        longint clocks = 0;
        if (!$value$plusargs("image=%s", path)) begin $display("no +image=FILE"); $finish; end
        fd = $fopen(path, "rb");
        n = $fread(img, fd);
        img_words = n / 2;
        repeat (4) @(posedge clk);
        rst = 0;
        while (cpu_rst && !error && clocks < 64'd200_000_000) begin @(posedge clk); clocks++; end
        if (cpu_rst || error) begin
            $display("boot_loader FAILED: CPU not started (error %0d, %0d clocks, state %0d, wp %0d)",
                     error, clocks, dut.state, dut.wp);
            $finish;
        end
        ent_blocks = (int'(img[6]) * 8 + 255) / 256;
        base_mem = (1 + ent_blocks + 16) * 256;
        base_xm = base_mem + 65536;
        if (ext_sectors != 17'(img[5])) begin
            errors++; $display("ext_sectors %0d, the image has %0d", ext_sectors, img[5]);
        end
        for (int i = 0; i < 65536; i++)
            if (mem[i] !== img[base_mem + i]) begin
                errors++;
                if (errors < 10) $display("internal memory %0d = %04x, expected %04x", i, mem[i], img[base_mem + i]);
            end
        for (int i = 0; i < int'(img[5]) * 256; i++) begin
            logic [15:0] want;
            want = i < int'(img[7]) * 256 ? img[base_xm + i] : 16'd0;
            if (xm[i] !== want) begin
                errors++;
                if (errors < 20) $display("external memory %0d (sector %0d) = %04x, expected %04x",
                                          i, i / 256, xm[i], want);
            end
        end
        for (int r = 0; r < 16; r++)
            if (regs[r] !== img[8 + r]) begin
                errors++; $display("R%0d = %0d, expected %0d", r, regs[r], img[8 + r]);
            end
        if (errors == 0)
            $display("boot_loader OK (%0d image blocks, %0d external memory sectors, %0d words read, %0d written)",
                     img_words / 256, ext_sectors, fd_reads, xm_writes);
        else $display("boot_loader FAILED: %0d errors", errors);
        $finish;
    end
endmodule
