// SPDX-License-Identifier: MIT
// Yamaha MU50 board as MAME's ymmu50.cpp: H8/3003 (h8h_sys) at 10 MHz, 512 KB program ROM,
// 64 KB work RAM, SWP00 (swp00.sv) with its 4 MB wave ROM, the LCD (mu50_lcd.sv), the panel
// keys and LEDs, MIDI in and out on SCI1.
//
// Memory map: 000000-07FFFF program ROM (rom_*: external, 16-bit words), 200000-20FFFF work RAM
// (block RAM here), 400000-4007FF SWP00; other external addresses read 0.
//
// SWP00 timing (cores/mu50/docs/plan.md, U3): a CPU access at state s acts after
// n = floor(s * 441 / 100000) + 1 samples. A counter follows the CPU's time (k = floor(t * 441 /
// 100000), r the remainder) and sends CMD_GEN whenever k grows, so the SWP00 keeps producing
// samples while the CPU does not touch it; an access to the SWP00 brings the counter to the
// access's end time first.
//
// Board inputs as MAME's: A/D AN0 and AN2 3FF (no analog input), AN4 0 (host switch: MIDI),
// AN6 200 (battery), others 0; IRQ0 held asserted; ports 7 and 9 read 0.
//
// pre_req / pre_ack (h8h_sys) is the board's hook between CPU operations: the bench replays
// MAME's MIDI pin changes there; the MiSTer top holds it to keep the CPU near real time.

module mu50_board #(
    parameter int TW = 48
) (
    input  logic          clk,
    input  logic          rst,

    // program ROM (CPU byte order: rom_data[15:8] is the even byte)
    output logic          rom_req,
    output logic [18:1]   rom_addr,
    input  logic          rom_ack,
    input  logic [15:0]   rom_data,

    // SWP00 memories (swp00.sv)
    output logic          wv_req,
    output logic [21:0]   wv_addr,
    input  logic          wv_ack,
    input  logic [31:0]   wv_data,
    output logic          dm_req,
    output logic          dm_we,
    output logic [18:0]   dm_addr,
    output logic [31:0]   dm_wdata,
    input  logic          dm_ack,
    input  logic [31:0]   dm_rdata,

    // audio
    output logic          smp_valid,
    output logic [17:0]   smp_l,
    output logic [17:0]   smp_r,

    // between CPU operations (see above)
    output logic          pre_req,
    output logic [TW-1:0] pre_time,
    input  logic          pre_ack,
    input  logic          inj_pending,
    input  logic [TW-1:0] inj_pos,
    input  logic [TW-1:0] inj_time,
    // MIDI in: SCI1 receive pin changes (rx_now: MAME's machine time in states)
    input  logic          rx_req,
    input  logic          rx_val,
    input  logic [TW-1:0] rx_now,
    output logic          rx_done,
    output logic          midi_tx,

    // panel: keys (1 = pressed): [4:0] row O0 bits 2-6, [9:5] O1, [14:10] O2
    input  logic [14:0]   keys,
    output logic [4:0]    leds,
    output logic [2:0]    contrast,

    // LCD render port (mu50_lcd)
    input  logic          r_line,
    input  logic [4:0]    r_pos,
    input  logic [2:0]    r_row,
    output logic [7:0]    r_code,
    output logic [4:0]    r_cg_bits,
    output logic          r_at_ac,
    output logic          lcd_on,

    // the work RAM's second port, bytes (synthframe_nvsave): read data one clock later
    input  logic          nv_we,
    input  logic [15:0]   nv_addr,
    input  logic [7:0]    nv_wdata,
    output logic [7:0]    nv_rdata,
    output logic          ram_written,      // the CPU wrote the RAM this clock

    output logic          illegal,
    output logic [63:0]   vtime,
    // trace (bench)
    output logic          tr_stb,
    output logic [23:0]   tr_state,
    output logic [23:0]   tr_npc,
    output logic [255:0]  tr_r,
    output logic [7:0]    tr_ccr,
    output logic [7:0]    tr_tvec,
    output logic [7:0]    tr_tlvl,
    output logic          bl_stb,
    output logic [2:0]    bl_kind,
    output logic [23:0]   bl_addr,
    output logic [15:0]   bl_data,
    output logic [TW-1:0] bl_time
);
    localparam logic [1:0] CMD_GEN = 2'd0, CMD_WRITE = 2'd1, CMD_READ = 2'd2;

    // ---------------------------------------------------------------- CPU
    logic          ext_req, ext_we, ext_word, ext_fetch, ext_ack;
    logic [23:0]   ext_addr;
    logic [15:0]   ext_wdata, ext_rdata;
    logic [TW-1:0] ext_time;
    logic          pr_stb, pw_stb;
    logic [3:0]    pr_port, pw_port;
    logic [7:0]    pr_data, pw_data, pw_ddr;
    logic [1:0]    sci_tx;
    logic [7:0][9:0] adc_in;

    always_comb begin
        adc_in = '0;
        adc_in[0] = 10'h3ff;
        adc_in[2] = 10'h3ff;
        adc_in[6] = 10'h200;
    end

    h8h_sys #(.TW(TW)) cpu (
        .clk, .rst,
        .ext_req, .ext_we, .ext_word, .ext_fetch, .ext_addr, .ext_wdata, .ext_time,
        .ext_ack, .ext_rdata,
        .pre_req, .pre_time, .pre_ack, .inj_pending, .inj_pos, .inj_time,
        .rx_req, .rx_sci(1'b1), .rx_val, .rx_now, .rx_done,
        .irq_pin(6'b000001),
        .pr_stb, .pr_port, .pr_data, .pw_stb, .pw_port, .pw_data, .pw_ddr,
        .adc_in, .sci_tx,
        .illegal, .vtime,
        .tr_stb, .tr_state, .tr_npc, .tr_r, .tr_ccr, .tr_tvec, .tr_tlvl,
        .bl_stb, .bl_kind, .bl_addr, .bl_data, .bl_time
    );
    assign midi_tx = sci_tx[1];

    // ---------------------------------------------------------------- ports (mu50_state)
    logic [7:0] cur_p9, cur_pb, cur_pc, p6_val, pa_val;
    logic [TW-1:0] step_start, prev_end;
    always_ff @(posedge clk) begin
        if (rst) begin
            cur_p9 <= 8'hff; cur_pb <= 8'hff; cur_pc <= 8'hff;
            step_start <= '0; prev_end <= '0;
        end else begin
            if (pw_stb && pw_port == 4'd8) cur_p9 <= pw_data;
            if (pw_stb && pw_port == 4'd10) cur_pb <= pw_data;
            if (pw_stb && pw_port == 4'd11) cur_pc <= pw_data;
            if (tr_stb) step_start <= vtime[TW-1:0];
            if (bl_stb) prev_end <= bl_time;
        end
    end
    wire [TW-1:0] t_read = (prev_end > step_start) ? prev_end : step_start;

    mu50_lcd lcd (
        .clk, .reset(rst), .now(ext_time[31:0]), .t_read(t_read[31:0]),
        .p6_we(pw_stb && pw_port == 4'd5), .pa_we(pw_stb && pw_port == 4'd9), .pw_data,
        .pa_rd(pr_stb && pr_port == 4'd9), .p6_val, .pa_val,
        .r_line, .r_pos, .r_row, .r_code, .r_cg_bits, .r_at_ac,
        .display_on(lcd_on), .cursor_on(), .blink_on()
    );

    // pc_r: rows selected by bits 0, 1, 7 (active low); keys pull bits 2-6 low
    function automatic logic [7:0] key_row(input logic [4:0] k);
        return {1'b1, ~k, 2'b11};
    endfunction
    always_comb begin
        logic [7:0] pc;
        pc = cur_pc | 8'h7c;
        if (!cur_pc[0]) pc = pc & key_row(keys[4:0]);
        if (!cur_pc[1]) pc = pc & key_row(keys[9:5]);
        if (!cur_pc[7]) pc = pc & key_row(keys[14:10]);
        case (pr_port)
            4'd5: pr_data = p6_val;
            4'd9: pr_data = pa_val;
            4'd10: pr_data = cur_pb;
            4'd11: pr_data = pc;
            default: pr_data = 8'h00;
        endcase
    end
    assign leds = ~cur_pb[6:2];
    assign contrast = {~cur_p9[5:4], ~cur_pb[1]};

    // ---------------------------------------------------------------- work RAM
    // two byte-wide true dual-port RAMs (even and odd bytes): port A the CPU, port B the save
    logic [7:0]  ram_e [32768], ram_o [32768];
    logic [15:0] ram_q;
    logic [14:0] ram_a;
    logic        ram_we_hi, ram_we_lo;
    logic [15:0] ram_wd;
    logic [7:0]  nv_qe, nv_qo;
    logic        nv_odd;
    // Intel's true dual-port template (a port's write returns the new data on that port); the
    // CPU never reads a byte in the clock it writes it
    always_ff @(posedge clk)
        if (ram_we_hi) begin
            ram_e[ram_a] <= ram_wd[15:8];
            ram_q[15:8] <= ram_wd[15:8];
        end else
            ram_q[15:8] <= ram_e[ram_a];
    always_ff @(posedge clk)
        if (nv_we && !nv_addr[0]) begin
            ram_e[nv_addr[15:1]] <= nv_wdata;
            nv_qe <= nv_wdata;
        end else
            nv_qe <= ram_e[nv_addr[15:1]];
    always_ff @(posedge clk)
        if (ram_we_lo) begin
            ram_o[ram_a] <= ram_wd[7:0];
            ram_q[7:0] <= ram_wd[7:0];
        end else
            ram_q[7:0] <= ram_o[ram_a];
    always_ff @(posedge clk)
        if (nv_we && nv_addr[0]) begin
            ram_o[nv_addr[15:1]] <= nv_wdata;
            nv_qo <= nv_wdata;
        end else
            nv_qo <= ram_o[nv_addr[15:1]];
    always_ff @(posedge clk) nv_odd <= nv_addr[0];
    assign nv_rdata = nv_odd ? nv_qo : nv_qe;
    assign ram_written = ram_we_hi || ram_we_lo;

    // ---------------------------------------------------------------- SWP00 and its time base
    logic          cmd_valid, cmd_ready, sw_smp_valid;
    logic [1:0]    cmd_kind;
    logic [31:0]   cmd_n, sample_count;
    logic [10:0]   cmd_reg;
    logic [7:0]    cmd_data, rd_data;
    logic [31:0]   mix [7];

    swp00 snd (
        .clk, .rst,
        .cmd_valid, .cmd_kind, .cmd_n, .cmd_reg, .cmd_data, .cmd_ready, .rd_data,
        .smp_valid(sw_smp_valid), .smp_l, .smp_r, .sample_count,
        .mix_valid(), .mix,
        .wv_req, .wv_addr, .wv_ack, .wv_data,
        .dm_req, .dm_we, .dm_addr, .dm_wdata, .dm_ack, .dm_rdata
    );
    assign smp_valid = sw_smp_valid;

    // k = floor(t * 441 / 100000), r = t * 441 - k * 100000, for t = sb_t
    logic [TW-1:0] sb_t, sb_target;
    logic [31:0]   sb_k, sb_sent;
    logic [17:0]   sb_r;
    wire           sb_ready = sb_t == sb_target && sb_r < 18'd100000;
    always_ff @(posedge clk) begin
        if (rst) begin
            sb_t <= '0; sb_k <= 32'd0; sb_r <= 18'd0;
        end else if (sb_r >= 18'd100000) begin
            sb_r <= sb_r - 18'd100000;
            sb_k <= sb_k + 32'd1;
        end else if (sb_t < sb_target) begin
            logic [TW-1:0] d;
            d = sb_target - sb_t;
            if (d > TW'(200)) d = TW'(200);
            sb_r <= sb_r + 18'(d[7:0] * 9'd441);
            sb_t <= sb_t + d;
        end
    end

    // ---------------------------------------------------------------- external bus
    typedef enum logic [3:0] {
        X_IDLE, X_ROM, X_RAM, X_RAM2, X_SWP_T, X_SWP, X_SWP_W, X_ACK
    } x_t;
    x_t x;
    logic       second;           // word SWP00 access: the odd byte
    logic [15:0] xd;
    wire is_rom = ext_addr < 24'h080000;
    wire is_ram = ext_addr >= 24'h200000 && ext_addr < 24'h210000;
    wire is_swp = ext_addr >= 24'h400000 && ext_addr < 24'h400800;

    // a ROM read answers in the clock rom_ack comes
    assign ext_ack = x == X_ACK || (x == X_ROM && rom_ack);
    assign ext_rdata = x == X_ROM ? (ext_word ? rom_data
                                              : {8'h0, ext_addr[0] ? rom_data[7:0] : rom_data[15:8]})
                                  : xd;
    assign rom_req = x == X_ROM;
    assign rom_addr = ext_addr[18:1];

    always_comb begin
        // the time base follows the CPU, or the end of an SWP00 access
        sb_target = (x == X_SWP_T || x == X_SWP || x == X_SWP_W) ? ext_time : vtime[TW-1:0];
        if (sb_target < sb_t) sb_target = sb_t;
    end

    // CMD_GEN runs on its own: the CPU's ROM and RAM accesses go on while the SWP00 makes the
    // sample; an SWP00 access waits for it (X_SWP_T)
    logic gen_busy;
    wire  in_swp = x == X_SWP_T || x == X_SWP || x == X_SWP_W;

    always_ff @(posedge clk) begin
        ram_we_hi <= 1'b0;
        ram_we_lo <= 1'b0;
        if (rst) begin
            gen_busy <= 1'b0;
        end else if (gen_busy) begin
            if (cmd_ready) begin
                cmd_valid <= 1'b0;
                gen_busy <= 1'b0;
            end
        end else if (!in_swp && !(x == X_IDLE && ext_req && is_swp) && sb_ready &&
                     sb_k + 32'd1 > sb_sent && !cmd_valid) begin
            // keep the SWP00 at the CPU's time
            cmd_valid <= 1'b1;
            cmd_kind <= CMD_GEN;
            cmd_n <= sb_k + 32'd1;
            sb_sent <= sb_k + 32'd1;
            gen_busy <= 1'b1;
        end
        if (rst) begin
            x <= X_IDLE;
            cmd_valid <= 1'b0;
            sb_sent <= 32'd0;
        end else case (x)
            X_IDLE: begin
                if (ext_req) begin
                    second <= 1'b0;
                    if (is_rom) x <= X_ROM;
                    else if (is_ram) begin
                        ram_a <= ext_addr[15:1];
                        if (ext_we) begin
                            ram_wd <= ext_word ? ext_wdata : {ext_wdata[7:0], ext_wdata[7:0]};
                            ram_we_hi <= ext_word || !ext_addr[0];
                            ram_we_lo <= ext_word || ext_addr[0];
                            xd <= 16'h0;
                            x <= X_ACK;
                        end else
                            x <= X_RAM;
                    end else if (is_swp) x <= X_SWP_T;
                    else begin
                        xd <= 16'h0;
                        x <= X_ACK;
                    end
                end
            end
            X_ROM: if (rom_ack) x <= X_IDLE;
            X_RAM: x <= X_RAM2;          // the read address is in; data next clock
            X_RAM2: begin
                xd <= ext_word ? ram_q : {8'h0, ext_addr[0] ? ram_q[7:0] : ram_q[15:8]};
                x <= X_ACK;
            end
            X_SWP_T: if (sb_ready && !gen_busy && !cmd_valid) x <= X_SWP;
            X_SWP: begin
                // a word is two byte commands, the even address first
                cmd_valid <= 1'b1;
                cmd_kind <= ext_we ? CMD_WRITE : CMD_READ;
                cmd_n <= sb_k + 32'd1;
                if (sb_k + 32'd1 > sb_sent) sb_sent <= sb_k + 32'd1;
                cmd_reg <= ext_word ? {ext_addr[10:1], second} : ext_addr[10:0];
                cmd_data <= ext_word ? (second ? ext_wdata[7:0] : ext_wdata[15:8]) : ext_wdata[7:0];
                x <= X_SWP_W;
            end
            X_SWP_W: if (cmd_ready) begin
                cmd_valid <= 1'b0;
                if (ext_word && !second) begin
                    xd[15:8] <= rd_data;
                    second <= 1'b1;
                    x <= X_SWP;
                end else begin
                    if (ext_word) xd[7:0] <= rd_data;
                    else xd <= {8'h0, rd_data};
                    x <= X_ACK;
                end
            end
            X_ACK: x <= X_IDLE;
            default: x <= X_IDLE;
        endcase
    end

    // the RAM starts zeroed (MAME's NVRAM without a file): block RAM powers up at 0 on the FPGA;
    // Verilator randomises it (--x-initial unique), so the benches clear it
`ifdef VERILATOR
    initial for (int i = 0; i < 32768; i++) begin
        ram_e[i] = 8'h0;
        ram_o[i] = 8'h0;
    end
`endif
endmodule
