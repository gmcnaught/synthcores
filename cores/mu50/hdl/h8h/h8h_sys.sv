// SPDX-License-Identifier: MIT
// H8/3003: the H8/300H core, its on-chip peripherals and on-chip RAM, and the bus sequencing of
// MAME's h8_device::execute_run.
//
// Every CPU bus operation runs as:
//   1. pre: the board applies pin changes that MAME made between timeslices, up to the CPU's
//      current time (pre_time = vtime before the operation); it answers pre_ack.
//   2. the access, at its end time t (MAME decrements icount before the access, so handlers see
//      total_cycles() = t): on-chip registers FFFF10-FFFFFF, on-chip RAM FFFD10-FFFF0F, else the
//      external bus (ext_*), which carries t in ext_time.
//   3. events: every peripheral event at or before t, as execute_run does after the access that
//      crosses them. A pin change MAME made between timeslices at machine time T, with the CPU
//      stopped at position P (inj_pos, inj_time): MAME ends the timeslice at the first access
//      ending at or after T without servicing the events that access crossed, so events before T
//      are serviced first, then the pin change (pre_req with pre_time = t), then the rest.
// SLEEP's eat-all-cycles jumps to the next peripheral event (MAME: icount = bcount), or by 2
// states when none is pending.

module h8h_sys #(
    parameter int TW = 48
) (
    input  logic          clk,
    input  logic          rst,

    // external bus: program ROM, work RAM, SWP00 ... (byte accesses: data in [7:0])
    output logic          ext_req,
    output logic          ext_we,
    output logic          ext_word,
    output logic          ext_fetch,
    output logic [23:0]   ext_addr,
    output logic [15:0]   ext_wdata,
    output logic [TW-1:0] ext_time,
    input  logic          ext_ack,
    input  logic [15:0]   ext_rdata,

    output logic          pre_req,
    output logic [TW-1:0] pre_time,
    input  logic          pre_ack,
    input  logic          inj_pending,    // the board's next pin change: CPU position, machine time
    input  logic [TW-1:0] inj_pos,
    input  logic [TW-1:0] inj_time,

    // peripheral pins (see h83003_periph)
    input  logic          rx_req,
    input  logic          rx_sci,
    input  logic          rx_val,
    input  logic [TW-1:0] rx_now,
    output logic          rx_done,
    input  logic [5:0]    irq_pin,
    output logic          pr_stb,
    output logic [3:0]    pr_port,
    input  logic [7:0]    pr_data,
    output logic          pw_stb,
    output logic [3:0]    pw_port,
    output logic [7:0]    pw_data,
    output logic [7:0]    pw_ddr,
    input  logic [7:0][9:0] adc_in,
    output logic [1:0]    sci_tx,

    output logic          illegal,
    output logic [63:0]   vtime,

    // trace (h8h_core)
    output logic          tr_stb,
    output logic [23:0]   tr_state,
    output logic [23:0]   tr_npc,
    output logic [255:0]  tr_r,
    output logic [7:0]    tr_ccr,
    output logic [7:0]    tr_tvec,
    output logic [7:0]    tr_tlvl,
    // every completed bus access, for the bench's bus log
    output logic          bl_stb,
    output logic [2:0]    bl_kind,
    output logic [23:0]   bl_addr,
    output logic [15:0]   bl_data,
    output logic [TW-1:0] bl_time
);
    localparam logic [2:0] OP_F16 = 3'd1, OP_R8 = 3'd2, OP_R16 = 3'd3, OP_W8 = 3'd4,
                           OP_W16 = 3'd5, OP_EAT = 3'd7;

    logic        bus_req, bus_ack;
    logic [2:0]  bus_kind;
    logic [23:0] bus_addr;
    logic [15:0] bus_wdata, bus_rdata;
    logic [63:0] op_time, eat_to;
    logic [7:0]  irq_vector, irq_level, cpu_ccr;
    logic [1:0]  irq_filter;
    logic        irq_filter_we, irq_taken, syscr_ue;
    logic [7:0]  irq_taken_vector;
    logic        standby;   // software standby (SSBY): not used by the MU50

    h8h_core #(.DEBT(1'b0)) cpu (
        .clk, .rst, .ce(1'b1),
        .bus_req, .bus_kind, .bus_addr, .bus_wdata, .bus_ack, .bus_rdata,
        .irq_vector, .irq_level, .syscr_ue, .standby_pending(1'b0),
        .irq_filter, .irq_filter_we, .irq_taken, .irq_taken_vector,
        .standby(standby), .illegal,
        .vtime, .op_time, .eat_to,
        .tr_stb, .tr_state, .tr_npc, .tr_r, .tr_ccr, .tr_tvec, .tr_tlvl
    );
    assign cpu_ccr = tr_ccr;

    // ---- peripherals
    logic          acc_req, acc_done, proc_req, proc_done;
    logic [15:0]   acc_rdata;
    logic [TW-1:0] next_event, t_end;
    logic          per_idle;

    h83003_periph #(.TW(TW)) per (
        .clk, .rst,
        .acc_req, .acc_we(bus_kind == OP_W8 || bus_kind == OP_W16),
        .acc_word(bus_kind == OP_R16 || bus_kind == OP_W16 || bus_kind == OP_F16),
        .acc_off(bus_addr[7:0]), .acc_wdata(bus_wdata), .acc_time(t_end),
        .acc_done, .acc_rdata,
        .proc_req, .proc_time(plim), .proc_done, .next_event, .idle(per_idle),
        .rx_req, .rx_sci, .rx_val, .rx_now, .rx_done,
        .irq_vector, .irq_level, .filter_we(irq_filter_we), .filter(irq_filter),
        .irq_taken, .irq_taken_vector, .cpu_ccr, .syscr_ue,
        .irq_pin, .pr_stb, .pr_port, .pr_data, .pw_stb, .pw_port, .pw_data, .pw_ddr,
        .adc_in, .sci_tx
    );

    // ---- on-chip RAM FFFD10-FFFF0F (512 bytes), word wide
    logic [15:0] iram [256];
    wire  [8:0]  iram_off = 9'(bus_addr[15:0] - 16'hfd10);
    logic [15:0] iram_q;
    logic        iram_ph;             // the synchronous read has its data
    logic [1:0]  iram_be;
    logic [15:0] iram_wd;
    always_ff @(posedge clk) begin
        if (iram_be[1]) iram[iram_off[8:1]][15:8] <= iram_wd[15:8];
        if (iram_be[0]) iram[iram_off[8:1]][7:0] <= iram_wd[7:0];
        iram_q <= iram[iram_off[8:1]];
    end

    // ---- sequencer
    typedef enum logic [2:0] { B_IDLE, B_PRE, B_ACC, B_PROC, B_INJ, B_PROC2, B_ACK } b_t;
    b_t b;
    logic [15:0] data;
    wire is_reg = bus_addr >= 24'hffff10;
    wire is_iram = bus_addr >= 24'hfffd10 && bus_addr < 24'hffff10;
    wire is_w = bus_kind == OP_W8 || bus_kind == OP_W16;
    wire is_word = bus_kind == OP_R16 || bus_kind == OP_W16 || bus_kind == OP_F16;

    // pre: offered in B_IDLE already (a board that answers at once costs no clock)
    assign pre_req = (b == B_IDLE && bus_req) || b == B_PRE || b == B_INJ;
    assign pre_time = b == B_INJ ? t_end : vtime[TW-1:0];
    // events before a pin change that lands at the end of this access go first
    wire inj_here = inj_pending && inj_pos <= t_end && inj_time <= t_end;
    logic [TW-1:0] plim;
    // no event due, no pin change here, the peripherals idle: the access needs no event pass
    wire fast = (next_event == '0 || next_event > t_end) && !inj_here && per_idle;
    assign plim = (b == B_PROC && inj_here) ? inj_time - TW'(1) : t_end;
    assign ext_req = b == B_ACC && !is_reg && !is_iram && bus_kind != OP_EAT;
    assign ext_we = is_w;
    assign ext_word = is_word;
    assign ext_fetch = bus_kind == OP_F16;
    assign ext_addr = bus_addr;
    assign ext_wdata = bus_wdata;
    assign ext_time = t_end;
    assign bus_ack = b == B_ACK;
    assign bus_rdata = data;
    assign eat_to = {{(64-TW){1'b0}}, t_end};

    assign acc_req = b == B_ACC && is_reg;
    assign proc_req = b == B_PROC || b == B_PROC2;

    always_ff @(posedge clk) begin
        bl_stb <= 1'b0;
        iram_be <= 2'b00;
        if (rst) begin
            b <= B_IDLE;
            iram_ph <= 1'b0;
        end else case (b)
            B_IDLE, B_PRE: if (b == B_PRE || bus_req) begin
              if (!pre_ack) b <= B_PRE;
              else begin
                if (bus_kind == OP_EAT) begin
                    t_end <= (next_event != '0 && next_event > vtime[TW-1:0]) ? next_event
                                                                              : vtime[TW-1:0] + TW'(2);
                    b <= B_PROC;
                end else begin
                    t_end <= op_time[TW-1:0];
                    b <= B_ACC;
                end
              end
            end
            B_ACC: begin
                logic [15:0] d;
                if (is_reg) begin
                    if (acc_done) begin
                        data <= acc_rdata;
                        b <= B_PROC;
                        end
                end else if (is_iram && !iram_ph) begin
                    // the address is in this clock; the data (and the write) next clock
                    iram_ph <= 1'b1;
                    if (is_w) begin
                        iram_be <= bus_kind == OP_W16 ? 2'b11 : (bus_addr[0] ? 2'b01 : 2'b10);
                        iram_wd <= bus_kind == OP_W16 ? bus_wdata : {bus_wdata[7:0], bus_wdata[7:0]};
                    end
                end else if (is_iram) begin
                    iram_ph <= 1'b0;
                    d = is_word ? iram_q : {8'h0, bus_addr[0] ? iram_q[7:0] : iram_q[15:8]};
                    data <= d;
                    b <= fast ? B_ACK : B_PROC;
                    if (fast) begin
                        bl_stb <= 1'b1; bl_kind <= bus_kind; bl_addr <= bus_addr;
                        bl_data <= is_w ? bus_wdata : d; bl_time <= t_end;
                    end
                end else if (ext_ack) begin
                    data <= ext_rdata;
                    b <= fast ? B_ACK : B_PROC;
                    if (fast) begin
                        bl_stb <= 1'b1; bl_kind <= bus_kind; bl_addr <= bus_addr;
                        bl_data <= is_w ? bus_wdata : ext_rdata; bl_time <= t_end;
                    end
                end
            end
            B_PROC: if (proc_done) begin
                if (inj_here) b <= B_INJ;        // serviced up to the pin change only
                else begin
                    b <= B_ACK;
                    bl_stb <= bus_kind != OP_EAT;
                    bl_kind <= bus_kind;
                    bl_addr <= bus_addr;
                    bl_data <= is_w ? bus_wdata : data;
                    bl_time <= t_end;
                end
            end
            B_INJ: if (pre_ack) b <= B_PROC2;
            B_PROC2: if (proc_done) begin
                b <= B_ACK;
                bl_stb <= bus_kind != OP_EAT;
                bl_kind <= bus_kind;
                bl_addr <= bus_addr;
                bl_data <= is_w ? bus_wdata : data;
                bl_time <= t_end;
            end
            B_ACK: b <= B_IDLE;
            default: b <= B_IDLE;
        endcase
    end

    initial for (int i = 0; i < 256; i++) iram[i] = 16'h0;
endmodule
