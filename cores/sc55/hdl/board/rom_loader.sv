// ROM pack download (hps_io ioctl, 8-bit) into the core: ROM1, ROM2 (its
// first 256 KB) and the sub-MCU ROM to block RAM (ld_* port), the wave ROMs
// and ROM2's upper 256 KB to DDR3 (wave_ddr write port).
//
// Pack v2 (tools/romset.py pack), every model:
//   0x000000  header: "SC55", 2, model (0 mk1, 1 SC-155, 2 mk2, 3 SC-155mk2)
//   0x000100  ROM1 (32 KB)
//   0x008100  ROM2 (512 KB; zeros after a 256 KB ROM2)
//   0x088100  sub-MCU ROM (4 KB)
//   0x089100  wave ROMs as the PCM addresses them (4 MB)
// Pack v1 (mk1 only): "SC55", 1; ROM1 0x100, ROM2 0x8100 (256 KB), waves 0x48100 (3 MB).
// DDR3 (wave_ddr byte offsets): waves at 0, ROM2's upper half at 0x400000.
// `ok` says a pack with a known header was loaded completely; `model` is its model.
module rom_loader (
    input  logic        clk,
    input  logic        rst,

    input  logic        ioctl_download,
    input  logic        ioctl_wr,
    input  logic [26:0] ioctl_addr,
    input  logic [7:0]  ioctl_dout,
    output logic        ioctl_wait,

    output logic        ld_we,
    output logic [2:0]  ld_sel,           // 0 ROM1, 1 ROM2, 4 sub-MCU ROM
    output logic [18:0] ld_addr,
    output logic [7:0]  ld_data,

    output logic        wave_wr,
    output logic [22:0] wave_addr,
    output logic [7:0]  wave_data,
    input  logic        wave_busy,

    output logic        loading,
    output logic        ok,
    output logic [1:0]  model
);
    localparam logic [26:0] ROM1_AT = 27'h000100, ROM2_AT = 27'h008100;
    localparam logic [26:0] V2_SM_AT = 27'h088100, V2_WAVE_AT = 27'h089100, V2_END = 27'h489100;
    localparam logic [26:0] V1_WAVE_AT = 27'h048100, V1_END = 27'h348100;

    logic [31:0] magic;
    logic [7:0]  version, model_b;
    logic [26:0] seen;
    logic        pend;            // a DDR3 byte waiting for the port
    logic        dl_d;

    assign ioctl_wait = pend || wave_busy;
    assign loading = ioctl_download;

    wire v2 = version == 8'd2;
    wire [26:0] rom2_end = v2 ? V2_SM_AT : V1_WAVE_AT;
    wire [26:0] wave_at  = v2 ? V2_WAVE_AT : V1_WAVE_AT;
    wire [26:0] end_at   = v2 ? V2_END : V1_END;

    always_ff @(posedge clk) begin : b_rom_loader_1
        logic [26:0] o;
        ld_we <= 1'b0;
        wave_wr <= 1'b0;
        dl_d <= ioctl_download;
        if (rst) begin
            pend <= 1'b0;
            ok <= 1'b0;
            seen <= '0;
            model <= 2'd0;
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
                if (ioctl_addr == 27'd4) version <= ioctl_dout;
                if (ioctl_addr == 27'd5) model_b <= ioctl_dout;
                if (ioctl_addr >= ROM1_AT && ioctl_addr < ROM2_AT) begin
                    ld_we <= 1'b1; ld_sel <= 3'd0; ld_addr <= 19'(ioctl_addr - ROM1_AT); ld_data <= ioctl_dout;
                end else if (ioctl_addr >= ROM2_AT && ioctl_addr < rom2_end) begin
                    o = ioctl_addr - ROM2_AT;
                    if (o < 27'h40000) begin
                        ld_we <= 1'b1; ld_sel <= 3'd1; ld_addr <= 19'(o); ld_data <= ioctl_dout;
                    end else begin
                        wave_addr <= 23'h400000 + 23'(o - 27'h40000);
                        wave_data <= ioctl_dout;
                        pend <= 1'b1;
                    end
                end else if (v2 && ioctl_addr >= V2_SM_AT && ioctl_addr < V2_WAVE_AT) begin
                    ld_we <= 1'b1; ld_sel <= 3'd4; ld_addr <= 19'(ioctl_addr - V2_SM_AT); ld_data <= ioctl_dout;
                end else if (ioctl_addr >= wave_at && ioctl_addr < end_at) begin
                    wave_addr <= 23'(ioctl_addr - wave_at);
                    wave_data <= ioctl_dout;
                    pend <= 1'b1;
                end
            end
            if (!ioctl_download && dl_d) begin
                ok <= magic == "SC55" && (version == 8'd1 || (version == 8'd2 && model_b < 8'd4)) && seen >= end_at;
                model <= (version == 8'd2) ? model_b[1:0] : 2'd0;
            end
        end
    end
endmodule
