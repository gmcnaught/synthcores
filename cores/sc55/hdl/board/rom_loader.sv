// ROM pack download (hps_io ioctl, 8-bit) into the core: ROM1 and ROM2 to the
// MCU's block RAM (ld_* port), the wave ROMs to DDR3 (wave_ddr write port).
//
// Pack layout (tools/romset.py pack), mk1:
//   0x000000  header: "SC55", model (1 = mk1), 251 bytes reserved
//   0x000100  ROM1 (32 KB)
//   0x008100  ROM2 (256 KB)
//   0x048100  wave ROMs 1..3 (1 MB each)
// `ok` says a pack with the right header was loaded completely.
module rom_loader (
    input  logic        clk,
    input  logic        rst,

    input  logic        ioctl_download,
    input  logic        ioctl_wr,
    input  logic [26:0] ioctl_addr,
    input  logic [7:0]  ioctl_dout,
    output logic        ioctl_wait,

    output logic        ld_we,
    output logic [1:0]  ld_sel,
    output logic [17:0] ld_addr,
    output logic [7:0]  ld_data,

    output logic        wave_wr,
    output logic [21:0] wave_addr,
    output logic [7:0]  wave_data,
    input  logic        wave_busy,

    output logic        loading,
    output logic        ok
);
    localparam logic [26:0] ROM1_AT = 27'h000100, ROM2_AT = 27'h008100, WAVE_AT = 27'h048100;
    localparam logic [26:0] END_AT  = WAVE_AT + 27'h300000;

    logic [31:0] magic;
    logic [26:0] seen;
    logic        pend;            // a wave byte waiting for the DDR3 port
    logic        dl_d;

    assign ioctl_wait = pend || wave_busy;
    assign loading = ioctl_download;

    always_ff @(posedge clk) begin
        ld_we <= 1'b0;
        wave_wr <= 1'b0;
        dl_d <= ioctl_download;
        if (rst) begin
            pend <= 1'b0;
            ok <= 1'b0;
            seen <= '0;
        end else begin
            if (ioctl_download && !dl_d) begin
                ok <= 1'b0;
                seen <= '0;
            end
            if (pend && !wave_busy) begin
                wave_wr <= 1'b1;
                pend <= 1'b0;
            end
            if (ioctl_download && ioctl_wr) begin
                seen <= ioctl_addr + 27'd1;
                if (ioctl_addr < 27'd4) magic <= {magic[23:0], ioctl_dout};
                if (ioctl_addr >= ROM1_AT && ioctl_addr < ROM2_AT) begin
                    ld_we <= 1'b1; ld_sel <= 2'd0; ld_addr <= 18'(ioctl_addr - ROM1_AT); ld_data <= ioctl_dout;
                end else if (ioctl_addr >= ROM2_AT && ioctl_addr < WAVE_AT) begin
                    ld_we <= 1'b1; ld_sel <= 2'd1; ld_addr <= 18'(ioctl_addr - ROM2_AT); ld_data <= ioctl_dout;
                end else if (ioctl_addr >= WAVE_AT && ioctl_addr < END_AT) begin
                    wave_addr <= 22'(ioctl_addr - WAVE_AT);
                    wave_data <= ioctl_dout;
                    pend <= 1'b1;
                end
            end
            if (!ioctl_download && dl_d)
                ok <= magic == "SC55" && seen >= END_AT;
        end
    end
endmodule
