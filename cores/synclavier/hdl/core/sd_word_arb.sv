// The SD word port (sd_floppy.sv's addr/rd/wr/ack) after reset, shared by
// the floppy controller (dev_floppy's background sector copies, drives 0-1),
// the SCSI host (dev_scsi's disk words, drives 3-4) and the Digital STM
// source reads (dig_stm, drive 5). The boot loader owns the port in reset
// (synclavier_core).
//
// The request presented is chosen each clock (SCSI first, then the floppy;
// dig_stm only while it holds `t_own`, taken while both are idle). sd_floppy
// acks a word on the clock after it serves it, and says whose it is
// (`m_ack_drive`, the served word's drive): the ack goes to that requester
// only. Routing it by the requester shown on the ack clock gave a SCSI
// request that began on the clock a floppy word was acked the floppy's ack
// and data (review 2026-11-26 sync-hdl #2).
module sd_word_arb (
    input  logic        clk,
    input  logic        rst,
    // dev_floppy: {drive[2:0], block[23:0], word[7:0]}, drive 0 or 1
    input  logic [34:0] f_addr,
    input  logic        f_rd,
    input  logic        f_wr,
    input  logic [15:0] f_wdata,
    output logic        f_ack,
    // dev_scsi, drive 3 or 4
    input  logic [34:0] s_addr,
    input  logic        s_rd,
    input  logic        s_wr,
    input  logic [15:0] s_wdata,
    output logic        s_ack,
    // dig_stm, drive 5 (reads only)
    input  logic [34:0] t_addr,
    input  logic        t_rd,
    output logic        t_ack,
    output logic        t_own,
    // to sd_floppy
    output logic [34:0] m_addr,
    output logic        m_rd,
    output logic        m_wr,
    output logic [15:0] m_wdata,
    input  logic        m_ack,
    input  logic [2:0]  m_ack_drive
);
    wire s_req = s_rd || s_wr;
    wire f_req = f_rd || f_wr;
    always_ff @(posedge clk)
        if (rst) t_own <= 1'b0;
        else if (t_own) begin if (t_ack) t_own <= 1'b0; end
        else if (t_rd && !s_req && !f_req) t_own <= 1'b1;

    assign m_addr  = t_own ? t_addr : s_req ? s_addr : f_addr;
    assign m_rd    = t_own ? t_rd : s_req ? s_rd : f_rd;
    assign m_wr    = !t_own && (s_req ? s_wr : f_wr);
    assign m_wdata = s_req ? s_wdata : f_wdata;
    assign t_ack   = m_ack && m_ack_drive == 3'd5;
    assign s_ack   = m_ack && (m_ack_drive == 3'd3 || m_ack_drive == 3'd4);
    assign f_ack   = m_ack && m_ack_drive <= 3'd2;
endmodule
