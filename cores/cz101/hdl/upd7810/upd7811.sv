// SPDX-License-Identifier: MIT
// NEC uPD7811: uPD7810 core + on-chip peripherals + 4 KB internal ROM + 256 B internal RAM.
//
// Memory map as MAME's upd_internal_4096_rom_256_ram_map: the internal ROM answers
// 0000-0FFF (MAME maps it regardless of the mode pins), the internal RAM FF00-FFFF while
// MM bit 3 is set; everything else goes to the external bus.
//
// External bus: one byte per request, ext_ack for one clock (read data with it).
// The internal ROM is loaded through rom_wr (e.g. from the ROM pack at start-up).

module upd7811 #(
    parameter DECODE_HEX = "upd7810_decode.hex"
) (
    input  logic        clk,
    input  logic        reset,
    input  logic        cyc_ce,

    // internal ROM load
    input  logic        rom_wr,
    input  logic [11:0] rom_addr,
    input  logic [7:0]  rom_data,

    // external bus
    output logic [15:0] ext_addr,
    output logic        ext_req,
    output logic        ext_we,
    output logic [7:0]  ext_wdata,
    input  logic [7:0]  ext_rdata,
    input  logic        ext_ack,

    // ports and pins
    input  logic [7:0]  pa_in, pb_in, pc_in, pd_in, pf_in,
    output logic [7:0]  pa_out, pb_out, pc_out, pd_out, pf_out,
    output logic        pa_we, pb_we, pc_we, pd_we, pf_we,
    input  logic [7:0]  an [8],
    input  logic        rxd,
    output logic        txd,
    input  logic        sck_edge,
    input  logic        sck_level,
    input  logic        nmi_line,
    input  logic        int1_line,
    input  logic        int1_rise,
    input  logic        int2_line,
    output logic        pa_rd,

    // virtual time and the event handshake (upd7810_core)
    output logic [31:0] vcyc,
    output logic [31:0] acc_time,
    output logic        at_boundary,
    input  logic        ev_done,

    // trace
    output logic        insn_done,
    output logic [6:0]  insn_cc,
    output logic [15:0] insn_pc,
    output logic [7:0]  insn_op,
    output logic [7:0]  insn_op2,
    output logic        halt_cycle,
    output logic        irq_taken,
    output logic [15:0] irq_taken_vec,
    output logic [7:0]  r_v, r_a, r_b, r_c, r_d, r_e, r_h, r_l,
    output logic [15:0] r_ea, r_sp, r_pc,
    output logic [7:0]  r_psw,
    output logic [15:0] r_irr,
    // every completed memory write (internal or external), for the trace
    output logic        wr_done,
    output logic [15:0] wr_addr,
    output logic [7:0]  wr_data
);

    logic [15:0] mem_addr;
    logic        mem_req, mem_we, mem_ack;
    logic [7:0]  mem_wdata, mem_rdata;

    logic [5:0]  sfr_id;
    logic        sfr_req, sfr_we, sfr_ack;
    logic [15:0] sfr_wdata, sfr_rdata;
    logic [4:0]  skit_id;
    logic        skit_req, skit_flag, skit_ack;
    logic [6:0]  adv_cycles;
    logic        adv_req, adv_ack;
    logic [15:0] irq_vec;
    logic        irq_nmi, irq_ack;
    logic        ram_enable;
    logic        iff_w;

    upd7810_core #(.DECODE_HEX(DECODE_HEX)) core (
        .clk, .reset, .cyc_ce,
        .mem_addr, .mem_req, .mem_we, .mem_wdata, .mem_rdata, .mem_ack,
        .sfr_id, .sfr_req, .sfr_we, .sfr_wdata, .sfr_rdata, .sfr_ack,
        .skit_id, .skit_req, .skit_flag, .skit_ack,
        .adv_cycles, .adv_req, .adv_ack,
        .irq_vec, .irq_nmi, .irq_ack,
        .iff_o(iff_w), .vcyc, .acc_time, .at_boundary, .ev_done,
        .insn_done, .insn_cc, .insn_pc, .insn_op, .insn_op2, .halt_cycle, .irq_taken,
        .irq_taken_vec, .r_v, .r_a, .r_b, .r_c, .r_d, .r_e, .r_h, .r_l, .r_ea, .r_sp, .r_pc,
        .r_psw
    );

    logic to, co0, co1;
    upd7810_periph periph (
        .clk, .reset,
        .sfr_id, .sfr_req, .sfr_we, .sfr_wdata, .sfr_rdata, .sfr_ack,
        .skit_id, .skit_req, .skit_flag, .skit_ack,
        .adv_cycles, .adv_req, .adv_ack,
        .irq_vec, .irq_nmi, .irq_ack,
        .pa_in, .pb_in, .pc_in, .pd_in, .pf_in,
        .pa_out, .pb_out, .pc_out, .pd_out, .pf_out,
        .pa_we, .pb_we, .pc_we, .pd_we, .pf_we,
        .an, .rxd, .txd, .sck_edge, .sck_level, .nmi_line, .int1_line, .int1_rise, .pa_rd, .int2_line,
        .ram_enable, .to, .co0, .co1, .r_irr
    );

    // ------------------------------------------------------------------ internal memory
    logic [7:0] irom [0:4095];
    logic [7:0] iram [0:255];
    logic [7:0] irom_q, iram_q;
    always_ff @(posedge clk) begin
        if (rom_wr) irom[rom_addr] <= rom_data;
        irom_q <= irom[mem_addr[11:0]];
    end

    wire in_rom = mem_addr[15:12] == 4'h0;
    wire in_ram = mem_addr[15:8] == 8'hFF && ram_enable;

    typedef enum logic [1:0] { B_IDLE, B_INT, B_EXT } bst_t;
    bst_t bst;
    logic int_rom;

    always_ff @(posedge clk) begin
        mem_ack <= 1'b0;
        wr_done <= 1'b0;
        if (in_ram && mem_req && mem_we && bst == B_IDLE) iram[mem_addr[7:0]] <= mem_wdata;
        iram_q <= iram[mem_addr[7:0]];
        if (reset) begin
            bst <= B_IDLE;
            ext_req <= 1'b0;
        end else case (bst)
            B_IDLE: if (mem_req && !mem_ack) begin
                if (in_rom || in_ram) begin
                    bst <= B_INT;
                    int_rom <= in_rom;
                end else begin
                    ext_addr <= mem_addr; ext_we <= mem_we; ext_wdata <= mem_wdata;
                    ext_req <= 1'b1;
                    bst <= B_EXT;
                end
            end
            B_INT: begin
                // ROM/RAM read data is valid one clock after the address
                mem_rdata <= int_rom ? irom_q : iram_q;
                mem_ack <= 1'b1;
                if (mem_we) begin wr_done <= 1'b1; wr_addr <= mem_addr; wr_data <= mem_wdata; end
                bst <= B_IDLE;
            end
            default: if (ext_ack) begin
                ext_req <= 1'b0;
                mem_rdata <= ext_rdata;
                mem_ack <= 1'b1;
                if (ext_we) begin wr_done <= 1'b1; wr_addr <= ext_addr; wr_data <= ext_wdata; end
                bst <= B_IDLE;
            end
        endcase
    end

endmodule
