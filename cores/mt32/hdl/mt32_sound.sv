// SPDX-License-Identifier: GPL-2.0-or-later
// MT-32 sound section: LA32 + reverb chip + output mix, sequenced as la32_t::clock in
// Nuked-MT32 (nukeykt), 5b4e239. For each slot i of a sample:
//   1. the LA32 computes slot i; at the same time the reverb chip runs its two program steps
//      (rv2->clock(8)), with set_input(expand(acc6 of the sample before)) first at i == 4 and
//      set_input(expand(acc2 of the sample before)) at i == 20 (neither depends on slot i);
//   2. la32_cycle += 8; cpu_turn pulses with slot_idx = i: the board runs the CPU up to
//      la32_cycle * 375 / 256 CPU cycles, hands over a MIDI byte, sets la32_sh3 (i == 15 or 31),
//      and writes the LA32 (bus_*), the reverb registers (rv_*) and pwm; then pulses cpu_done;
//   3. after slot 31's cpu_done: the mix of the sample before (accumulators clamped) with the
//      reverb's sample0/sample1 and pwm; sample_valid with out_l/out_r.
// run low holds the sequencer before the next slot starts.
module mt32_sound #(
    parameter logic [28:0] PCM_BASE = 29'd0,
    parameter POW_HEX = "mt32_la32_pow.hex",
    parameter LOGSIN_HEX = "mt32_la32_logsin.hex"
) (
    input  logic        clk,
    input  logic        rst,
    input  logic        run,

    output logic        cpu_turn,
    output logic [4:0]  slot_idx,
    input  logic        cpu_done,
    output logic [63:0] la32_cycle,

    // CPU side
    input  logic        bus_we,           // LA32, CPU addresses 0x0C00-0x0FFF (addr & 0x1ff)
    input  logic [8:0]  bus_addr,
    input  logic [7:0]  bus_wdata,
    output logic [7:0]  bus_rdata,
    output logic        extint,
    input  logic        rv_we,            // reverb: 0 = 0x0800 saw0, 1 = 0x0400 saw1, 2 = 0x0200 ROM bank
    input  logic [1:0]  rv_sel,
    input  logic [7:0]  rv_data,
    input  logic [7:0]  pwm,              // the CPU's PWM_CONTROL (master volume = 255 - pwm)

    // reverb program ROM load (32 KB)
    input  logic        rvrom_we,
    input  logic [14:0] rvrom_addr,
    input  logic [7:0]  rvrom_data,

    // PCM ROM (synthframe memory port)
    output logic        pcm_req_valid,
    input  logic        pcm_req_ready,
    output logic [28:0] pcm_req_addr,
    output logic [1:0]  pcm_req_tag,
    input  logic        pcm_rsp_valid,
    input  logic [63:0] pcm_rsp_data,
    input  logic [1:0]  pcm_rsp_tag,

    output logic        sample_valid,     // 32 kHz
    output logic signed [15:0] out_l,
    output logic signed [15:0] out_r,

    // debug: the reference's --out-log fields, valid with sample_valid
    output logic signed [15:0] dbg_acc2, dbg_acc3, dbg_acc6, dbg_acc7, dbg_rv0, dbg_rv1,
    output logic [8:0]  dbg_vol,
    // debug: LA32 slot state at slot_done (the reference's --slot-log)
    output logic        dbg_slot_done,
    output logic signed [31:0] dbg_oo, dbg_prev,
    output logic [7:0]  dbg_w46,
    output logic [25:0] dbg_ram2_0, dbg_ram2_1, dbg_ram2_2,
    output logic signed [31:0] dbg_sacc2, dbg_sacc3, dbg_sacc6, dbg_sacc7,
    output logic [14:0] dbg_w186, dbg_w187, dbg_w188
);
    function automatic logic signed [15:0] expand(input logic signed [15:0] s);
        return {s[15], s[13:0], s[14]};
    endfunction

    logic slot_go, slot_done, rv_go, rv_done, rv_in_we, mix_go, mix_valid;
    logic [4:0] la_idx;
    logic signed [15:0] ap2, ap3, ap6, ap7, rv0, rv1;

    mt32_la32 #(.PCM_BASE(PCM_BASE), .POW_HEX(POW_HEX), .LOGSIN_HEX(LOGSIN_HEX)) la32 (
        .clk, .rst, .slot_go, .slot_done, .slot_idx(la_idx),
        .bus_we, .bus_addr, .bus_wdata, .bus_rdata, .extint,
        .pcm_req_valid, .pcm_req_ready, .pcm_req_addr, .pcm_req_tag,
        .pcm_rsp_valid, .pcm_rsp_data, .pcm_rsp_tag,
        .acc_prev2(ap2), .acc_prev3(ap3), .acc_prev6(ap6), .acc_prev7(ap7),
        .dbg_oo, .dbg_prev, .dbg_w46, .dbg_ram2_0, .dbg_ram2_1, .dbg_ram2_2,
        .dbg_acc2(dbg_sacc2), .dbg_acc3(dbg_sacc3), .dbg_acc6(dbg_sacc6), .dbg_acc7(dbg_sacc7),
        .dbg_w186, .dbg_w187, .dbg_w188
    );
    assign dbg_slot_done = slot_done;

    logic [4:0] slot;
    wire signed [15:0] rv_in = (slot == 5'd4) ? expand(ap6) : expand(ap2);

    mt32_rv2 rv2 (
        .clk, .rst, .go(rv_go), .done(rv_done),
        .in_we(rv_in_we), .in_value(rv_in),
        .reg_we(rv_we), .reg_sel(rv_sel), .reg_data(rv_data),
        .load_we(rvrom_we), .load_addr(rvrom_addr), .load_data(rvrom_data),
        .sample0(rv0), .sample1(rv1)
    );

    mt32_mix mix (
        .clk, .rst, .go(mix_go),
        .acc2(ap2), .acc3(ap3), .acc6(ap6), .acc7(ap7), .rv0, .rv1, .pwm,
        .valid(mix_valid), .out_l, .out_r
    );

    typedef enum logic [2:0] { Q_IDLE, Q_SLOT, Q_TURN, Q_WAITCPU, Q_MIX } q_t;
    q_t q;
    logic la_fin, rv_fin;

    always_comb begin
        slot_go = (q == Q_IDLE) && run;
        rv_go = slot_go;
        rv_in_we = slot_go && (slot == 5'd4 || slot == 5'd20);
        mix_go = (q == Q_WAITCPU) && cpu_done && (slot == 5'd31);
    end

    always_ff @(posedge clk) begin
        cpu_turn <= 1'b0;
        sample_valid <= 1'b0;
        if (rst) begin
            q <= Q_IDLE;
            slot <= 5'd0;
`ifdef MT32_SIM_LA32_START
            la32_cycle <= 64'd `MT32_SIM_LA32_START;   // sim only: start near a wrap point
`else
            la32_cycle <= 64'd0;
`endif
            la_fin <= 1'b0;
            rv_fin <= 1'b0;
        end else begin
            case (q)
                Q_IDLE: if (run) begin
                    la_fin <= 1'b0;
                    rv_fin <= 1'b0;
                    q <= Q_SLOT;
                end
                Q_SLOT: begin
                    if (slot_done) la_fin <= 1'b1;
                    if (rv_done) rv_fin <= 1'b1;
                    if ((la_fin || slot_done) && (rv_fin || rv_done)) begin
                        la32_cycle <= la32_cycle + 64'd8;
                        q <= Q_TURN;
                    end
                end
                Q_TURN: begin
                    cpu_turn <= 1'b1;
                    q <= Q_WAITCPU;
                end
                Q_WAITCPU: if (cpu_done) begin
                    if (slot == 5'd31) q <= Q_MIX;
                    else q <= Q_IDLE;
                    slot <= slot + 5'd1;
                end
                Q_MIX: if (mix_valid) begin
                    sample_valid <= 1'b1;
                    dbg_acc2 <= expand(ap2); dbg_acc3 <= expand(ap3);
                    dbg_acc6 <= expand(ap6); dbg_acc7 <= expand(ap7);
                    dbg_rv0 <= rv0; dbg_rv1 <= rv1;
                    dbg_vol <= 9'd255 - 9'(pwm);
                    q <= Q_IDLE;
                end
                default: q <= Q_IDLE;
            endcase
        end
    end
    assign slot_idx = slot;
endmodule
