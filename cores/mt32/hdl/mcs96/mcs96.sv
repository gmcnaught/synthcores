// SPDX-License-Identifier: MIT
// Intel 8098 (MCS-96) as Nuked-MT32 runs it: instruction engine, peripherals and MAME's
// execution loop (modules/nuked-mt32/mame/mcs96.h clock(), mcs96.cpp execute_run(); Olivier
// Galibert, BSD-3-Clause; see NOTICE).
//
// `run` with `target` does what mcs96_device::clock(target) does:
//   while (totalcycles < target) {
//       step = clamp(target - totalcycles, 4, 33); icount = step;
//       execute_run();                       // internal_update(totalcycles), then states
//       totalcycles += step;                 // an overshoot of the slice is dropped
//   }
// execute_run(): while (icount > 0) { while (icount > bcount) one state;
//                                     while (bcount && icount <= bcount) internal_update(event); }
// with icount -= 3 * next(n) per handler and bcount = slice end - next event (0 = none).
// `total_cycles` is MAME's total_cycles() = totalcycles + step - icount at every moment, also
// between runs (the board's LCD busy flag and MIDI pacing read it there).
`ifndef MCS96_DECODE_HEX
`define MCS96_DECODE_HEX "mcs96_decode.hex"
`endif
module mcs96 #(
    parameter DECODE_HEX = `MCS96_DECODE_HEX
) (
    input  logic        clk,
    input  logic        rst,
    output logic        ready,

    input  logic        run,
    input  logic [47:0] target,
    output logic        busy,
    output logic [47:0] total_cycles,

    // external bus (mt32_t::cpu_read / cpu_write), one byte per request
    output logic        ext_req,
    output logic        ext_we,
    output logic [15:0] ext_addr,
    output logic [7:0]  ext_wdata,
    input  logic [7:0]  ext_rdata,
    input  logic        ext_ack,

    // board inputs (change only while !busy) and outputs
    input  logic        serial_in,
    input  logic [7:0]  serial_byte,
    input  logic        extint,
    input  logic        la32_sh3,
    input  logic [9:0]  knob,
    output logic [7:0]  pwm,
    output logic        tx_stb,        // MIDI out: SBUF write (pulse, with tx_byte)
    output logic [7:0]  tx_byte,

    // trace
    output logic        dbg_fetch,
    output logic        dbg_irq,
    output logic [2:0]  dbg_level,
    output logic [15:0] dbg_pc,
    output logic [15:0] dbg_fpc,
    output logic [15:0] dbg_psw,
    output logic        dbg_regw,      // pulse: register-space write (dbg_addr, dbg_data)
    output logic [7:0]  dbg_addr,
    output logic [7:0]  dbg_data,
    output logic        bad_op,
    output logic        unsupported
);
    // ---------------------------------------------------------------- core and peripherals
    logic        c_exec, c_done;
    logic [6:0]  c_ncyc;
    logic        m_req, m_space, m_we, m_ack;
    logic [15:0] m_addr;
    logic [7:0]  m_wdata, m_rdata, irq_set;
    logic        r_ack;
    logic [7:0]  r_rdata;
    logic        upd_req, upd_ack, ev_valid;
    logic [47:0] upd_time, ev_time;
    logic        upd_fast;
    logic [44:0] tq;           // totalcycles / 24, kept as it advances
    logic [4:0]  tr;           // totalcycles % 24

    mcs96_core #(.DECODE_HEX(DECODE_HEX)) core (
        .clk, .rst,
        .exec(c_exec), .done(c_done), .ncyc(c_ncyc), .was_fetch(c_is_fetch), .bad_op,
        .m_req, .m_space, .m_we, .m_addr, .m_wdata, .m_rdata, .m_ack,
        .irq_set,
        .pc(dbg_pc), .psw(dbg_psw), .dbg_fetch, .dbg_fpc, .dbg_irq, .dbg_level
    );

    mcs96_periph periph (
        .clk, .rst, .ready,
        .r_req(m_req && !m_space), .r_we(m_we), .r_addr(m_addr[7:0]), .r_wdata(m_wdata),
        .r_rdata, .r_ack,
        .now(total_cycles),
        .upd_req, .upd_time, .upd_fast, .upd_q(tq), .upd_ack, .ev_valid, .ev_time,
        .serial_in, .serial_byte, .extint, .la32_sh3, .knob, .pwm, .irq_set, .unsupported, .tx_stb, .tx_byte
    );

    assign ext_req   = m_req && m_space;
    assign ext_we    = m_we;
    assign ext_addr  = m_addr;
    assign ext_wdata = m_wdata;
    assign m_ack     = m_space ? ext_ack : r_ack;
    assign m_rdata   = m_space ? ext_rdata : r_rdata;

    assign dbg_regw = r_ack && m_we && m_req && !m_space;
    assign dbg_addr = m_addr[7:0];
    assign dbg_data = m_wdata;

    // ---------------------------------------------------------------- scheduler
    logic [47:0] tc, tgt;
    logic [5:0]  step;
    logic signed [15:0] icount;
    logic [31:0] bcount;
    wire  [47:0] slice_end = tc + {42'h0, step};
    // registered: every reader comes at least two clocks after tc, step or icount changes
    always_ff @(posedge clk) total_cycles <= tc + {42'h0, step} - {{32{icount[15]}}, icount};

    // recompute_bcount(event): event at or past the slice end (or 0) -> 0
    function automatic logic [31:0] bc(input logic [47:0] se, input logic [47:0] ev);
        if (ev == 48'h0 || ev >= se) return 32'h0;
        return 32'(se - ev);
    endfunction

    typedef enum logic [2:0] { SC_IDLE, SC_TOP, SC_UPD, SC_RUN, SC_EXEC, SC_EVUPD } sc_t;
    sc_t sc;
    logic ev_moved;
    wire  c_was_fetch = (c_ncyc == 7'd0) && c_is_fetch;
    logic c_is_fetch;

    always_ff @(posedge clk) begin
        c_exec <= 1'b0;
        upd_req <= 1'b0;
        if (rst) begin
            sc <= SC_IDLE;
            tc <= 48'h0;
            step <= 6'd0;
            icount <= 16'sd0;
            bcount <= 32'h0;
            busy <= 1'b0;
            tq <= 45'h0;
            tr <= 5'd0;
            upd_fast <= 1'b0;
        end else begin
            // an internal_update called from an SFR write (HSO, A/D) moves the next event
            if (ev_valid) bcount <= bc(slice_end, ev_time);
            case (sc)
            SC_IDLE: if (run && ready) begin tgt <= target; busy <= 1'b1; sc <= SC_TOP; end
            SC_TOP: if (tc >= tgt) begin
                busy <= 1'b0;
                sc <= SC_IDLE;
            end else begin
                logic [47:0] d;
                logic [5:0]  s;
                d = tgt - tc;
                s = (d < 48'd4) ? 6'd4 : (d > 48'd33) ? 6'd33 : d[5:0];
                step <= s;
                icount <= {10'h0, s};
                upd_time <= tc;
                upd_fast <= 1'b1;
                upd_req <= 1'b1;
                sc <= SC_UPD;
            end
            SC_UPD: if (upd_ack) sc <= SC_RUN;
            SC_RUN: if ($signed({{16{icount[15]}}, icount}) > $signed(bcount)) begin
                c_exec <= 1'b1;
                ev_moved <= 1'b0;
                sc <= SC_EXEC;
            end else if (bcount != 32'h0) begin
                upd_time <= slice_end - {16'h0, bcount};
                upd_fast <= 1'b0;
                upd_req <= 1'b1;
                sc <= SC_EVUPD;
            end else begin
                logic [6:0] r;
                tc <= slice_end;
                r = {2'b00, tr} + {1'b0, step};
                if (r >= 7'd48)      begin tr <= 5'(r - 7'd48); tq <= tq + 45'd2; end
                else if (r >= 7'd24) begin tr <= 5'(r - 7'd24); tq <= tq + 45'd1; end
                else                       tr <= 5'(r);
                sc <= SC_TOP;
            end
            SC_EXEC: begin
                if (ev_valid) ev_moved <= 1'b1;
                if (c_done) begin
                    icount <= icount - 16'(3 * {9'h0, c_ncyc});
                    // a fetch takes no cycles: icount > bcount still holds unless bcount moved
                    if (c_was_fetch && !ev_moved && !ev_valid) c_exec <= 1'b1;
                    else sc <= SC_RUN;
                    ev_moved <= 1'b0;
                end
            end
            SC_EVUPD: if (upd_ack) sc <= SC_RUN;
            default: sc <= SC_IDLE;
            endcase
        end
    end
endmodule
