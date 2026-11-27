// Simulation top for CPU-only lockstep (M4.3): the CPU and 64K words of
// synchronous internal memory with a load port. Every device access goes
// out on dev_* and the C++ bench (hdl/sim/lockstep.cpp) answers it from a
// trace recorded by the Python simulator. Same port names as
// able_sys_top so one bench drives both.
module able_sim_top (
    input  logic        clk,
    input  logic        rst,
    input  logic [1:0]  cpu_model,
    input  logic [15:0] cpu_d57,
    input  logic [21:0] ext_words,         // installed external memory (words); beyond: 177777
    input  logic        ld_xm_we,          // Model D code pages (external memory words)
    input  logic [20:0] ld_xm_addr,
    input  logic [15:0] ld_xm_data,
    input  logic        ld_mem_we,
    input  logic [15:0] ld_mem_addr,
    input  logic [15:0] ld_mem_data,
    // end-of-trace state for the bench (hdl/sim/lockstep.cpp 'M'): memory
    // read port (combinational) and the CPU's Model C/D state
    input  logic [20:0] dbg_rd_addr,
    output logic [15:0] dbg_mem_rdata,
    output logic [15:0] st_page,
    output logic [15:0] st_ppc,
    output logic [15:0] st_rpt,            // {rpt_next, rpt_left}
    output logic [15:0] st_rpt_word,
    output logic [15:0] st_rpt_pc,
    output logic [15:0] st_d2,
    output logic        st_shadow,
    input  logic        dbg_we,
    input  logic [4:0]  dbg_addr,
    input  logic [15:0] dbg_wdata,

    output logic        dev_rd,
    output logic        dev_wr,
    output logic [6:0]  dev_num,
    output logic [15:0] dev_wdata,
    input  logic [15:0] dev_rdata,
    input  logic        dev_ack,
    input  logic        dev_nosyn,
    input  logic        dev_hold,
    input  logic [15:4] dev_irq,
    output logic        irq_sample,
    output logic        irq_psw,
    output logic [15:4] irq_all,
    output logic        io_held,
    output logic        tick,
    output logic [9:0]  tick_ns,
    input  logic        halt_req,
    output logic        halted,
    output logic [1:0]  halt_reason,
    output logic        instr_done,
    output logic [15:0] instr_pc,
    output logic [15:0] instr_word,
    output logic [255:0] regs_flat,
    output logic [4:0]  flags,
    output logic        mon_valid,
    output logic        mon_wr,
    output logic [6:0]  mon_dev,
    output logic [15:0] mon_data
);
    logic [15:0] mem [65536];
    logic [15:0] mem_addr, mem_wdata, mem_rdata;
    logic        mem_we;

    always_ff @(posedge clk) begin
        if (rst && ld_mem_we) mem[ld_mem_addr] <= ld_mem_data;
        else if (mem_we) mem[mem_addr] <= mem_wdata;
        mem_rdata <= mem[mem_addr];
    end

    // Model D code fetches: the trace's external memory (writes through
    // D60-D63 are answered from the trace and do not reach this copy)
    logic [15:0] xram [1 << 21];
    logic [23:0] cm_addr;
    logic        cm_rd, cm_ack;
    logic [15:0] cm_rdata;
    always_ff @(posedge clk) begin
        if (rst) begin
            cm_ack <= 1'b0;
            if (ld_xm_we) xram[ld_xm_addr] <= ld_xm_data;
        end else begin
            cm_ack <= cm_rd && !cm_ack;
            // as sim/cpu.py _code_read: beyond the installed memory reads 177777
            cm_rdata <= {2'b0, cm_addr} >= {4'b0, ext_words} ? 16'hFFFF : xram[cm_addr[20:0]];
        end
    end

    able_cpu cpu (
        .clk, .rst, .model(cpu_model), .d57(cpu_d57),
        .cm_addr, .cm_rd, .cm_rdata, .cm_ack,
        .mem_addr, .mem_we, .mem_wdata, .mem_rdata,
        .io_rd(dev_rd), .io_wr(dev_wr), .io_dev(dev_num), .io_wdata(dev_wdata),
        .io_rdata(dev_rdata), .io_ack(dev_ack), .io_nosyn(dev_nosyn), .io_hold(dev_hold),
        .io_held, .tick, .tick_ns,
        .irq(dev_irq), .irq_sample, .irq_psw,
        .stall(1'b0), .halt_req, .halted, .halt_reason,
        .nat_en(1'b0), .nat_req(), .nat_num(), .nat_ack(1'b0), .nat_wr(1'b0), .nat_result(16'd0),
        .instr_done, .instr_pc, .instr_word,
        .dbg_we, .dbg_addr, .dbg_wdata, .regs_flat, .flags
    );
    assign dbg_mem_rdata = mem[dbg_rd_addr[15:0]];
    assign st_page     = cpu.page;
    assign st_ppc      = cpu.ppc;
    assign st_rpt      = {cpu.rpt_next, cpu.rpt_left};
    assign st_rpt_word = cpu.rpt_word;
    assign st_rpt_pc   = cpu.rpt_pc;
    assign st_d2       = cpu.d2;
    assign st_shadow   = cpu.shadow;
    assign irq_all   = dev_irq;
    assign mon_valid = (dev_rd || dev_wr) && dev_ack && !dev_nosyn;
    assign mon_wr    = dev_wr;
    assign mon_dev   = dev_num;
    assign mon_data  = dev_wr ? dev_wdata : dev_rdata;
endmodule
