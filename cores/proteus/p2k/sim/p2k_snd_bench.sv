// SPDX-License-Identifier: GPL-3.0-or-later
// Bench top for the P2K sound engine: p2k_snd on synthframe_mem_ddr3, with MiSTer's DDRAM_* pins
// for the C++ DDR3 model (p2k_snd_tb.cpp). The Quartus check (p2k/syn) builds the same top.
module p2k_snd_bench (
    input  logic        clk,
    input  logic        rst,
    input  logic        filter_on,
    input  logic        fx_on,
    input  logic        wr_valid,
    output logic        wr_ready,
    input  logic [1:0]  wr_chip,
    input  logic [18:0] wr_addr,
    input  logic [15:0] wr_data,
    input  logic [15:0] wr_mask,
    input  logic        start,
    output logic        done,
    output logic signed [15:0] out_l,
    output logic signed [15:0] out_r,
    output logic [31:0] stat_div_cycles,
    output logic [31:0] stat_chained,

    input  logic        DDRAM_BUSY,
    output logic [7:0]  DDRAM_BURSTCNT,
    output logic [28:0] DDRAM_ADDR,
    input  logic [63:0] DDRAM_DOUT,
    input  logic        DDRAM_DOUT_READY,
    output logic        DDRAM_RD,
    output logic [63:0] DDRAM_DIN,
    output logic [7:0]  DDRAM_BE,
    output logic        DDRAM_WE
);
    logic        req_valid, req_ready, req_we, rsp_valid;
    logic [28:0] req_addr;
    logic [63:0] req_wdata, rsp_data;
    logic [7:0]  req_be, req_len;
    logic [1:0]  req_tag, rsp_tag;

    p2k_snd u_snd (
        .clk, .rst, .filter_on, .fx_on,
        .wr_valid, .wr_ready, .wr_chip, .wr_addr, .wr_data, .wr_mask,
        .idle(), .rd_valid(1'b0), .rd_ready(), .rd_chip(1'b0), .rd_addr(8'd0), .rd_done(), .rd_vaddr(), .rd_vsel(), .rd_ipx(),
        .start, .done, .out_l, .out_r,
        .mem_req_valid(req_valid), .mem_req_ready(req_ready), .mem_req_we(req_we), .mem_req_addr(req_addr),
        .mem_req_wdata(req_wdata), .mem_req_be(req_be), .mem_req_len(req_len), .mem_req_tag(req_tag),
        .mem_rsp_valid(rsp_valid), .mem_rsp_data(rsp_data), .mem_rsp_tag(rsp_tag),
        .stat_div_cycles, .stat_chained
    );

    synthframe_mem_ddr3 #(.TW(2), .QBITS(4)) u_ddr (
        .clk, .rst,
        .req_valid, .req_ready, .req_we, .req_addr, .req_wdata, .req_be, .req_len, .req_tag,
        .rsp_valid, .rsp_data, .rsp_tag,
        .DDRAM_BUSY, .DDRAM_BURSTCNT, .DDRAM_ADDR, .DDRAM_DOUT, .DDRAM_DOUT_READY, .DDRAM_RD,
        .DDRAM_DIN, .DDRAM_BE, .DDRAM_WE
    );
endmodule
