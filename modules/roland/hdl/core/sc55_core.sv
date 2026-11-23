// SC-55 (mk1) core: MCU system + PCM chip. Board-independent; the MiSTer top
// level adds the HPS interface, wave ROM memory, MIDI, video and audio.
module sc55_core #(
    parameter int RXBUF_BITS = 11,
    parameter int ROM2_AW = 18,            // 19 for the mk2's 512 KB ROM2
    parameter bit JV880_RAMS = 1'b0        // the JV-880's NVRAM and card RAM (sc55_mcu)
) (
    input  logic        clk,
    input  logic        rst,

    input  logic        run_step,
    output logic        step_done,
    input  logic        reset_start,
    output logic        reset_done,
    output logic        ready,            // power-on clears done

    input  logic        ld_we,
    input  logic [2:0]  ld_sel,           // 0 rom1, 1 rom2, 2 sram, 3 ram, 4 sub-MCU rom, 5 NVRAM, 6 card RAM
    input  logic [18:0] ld_addr,
    input  logic        mk2,              // model (see sc55_mcu)
    input  logic        sc155,
    input  logic        jv880,            // JV-880 (cores/jv880; needs JV880_RAMS)
    input  logic        enc_we,           // JV-880 encoder detent (sc55_mcu)
    input  logic        enc_dir,
    output logic        rom2x_req,        // ROM2 upper half outside (sc55_mcu)
    output logic [17:0] rom2x_addr,
    input  logic        rom2x_ack,
    input  logic [7:0]  rom2x_data,
    input  logic [7:0]  ld_data,
    input  logic [14:0] nv_dump_a,        // JV-880 NVRAM copy for the save file (sc55_mcu)
    output logic [7:0]  nv_dump_d,
    output logic        nv_wrote,         // the NVRAM was written this clock
    output logic        idle,             // no step in progress

    input  logic        midi_valid,
    input  logic [7:0]  midi_data,
    output logic        midi_full,
    input  logic [31:0] buttons,

    output logic        rom_req,
    output logic [21:0] rom_addr,
    input  logic        rom_ready,
    input  logic [7:0]  rom_data,
    input  logic        rom_rvalid,

    input  logic        enable_oversampling,
    output logic        sample_valid,
    output logic signed [31:0] sample_l,
    output logic signed [31:0] sample_r,

    output logic        lcd_we,
    output logic        lcd_rs,
    output logic [7:0]  lcd_data,
    output logic        lcd_enable,

    output logic [47:0] cycles,
    output logic [15:0] dbg_pc,
    output logic [7:0]  dbg_cp,
    output logic [15:0] dbg_sr
);
    logic        p_req, p_we, p_ack, p_tick, p_done, p_irq0, p_known;
    logic [5:0]  p_addr;
    logic [7:0]  p_wdata, p_rdata;
    logic [15:0] zr [8];
    always_comb for (int i = 0; i < 8; i++) zr[i] = 16'h0;

    logic m_ready, p_ready;
    assign ready = m_ready && p_ready;
    sc55_mcu #(.RXBUF_BITS(RXBUF_BITS), .ROM2_AW(ROM2_AW), .JV880_RAMS(JV880_RAMS)) mcu (
        .clk, .rst, .mk2, .sc155, .jv880, .enc_we, .enc_dir, .rom2x_req, .rom2x_addr, .rom2x_ack, .rom2x_data, .run_step, .step_done, .reset_start, .reset_done, .ready(m_ready),
        .ld_we, .ld_sel, .ld_addr, .ld_data, .nv_dump_a, .nv_dump_d, .nv_wrote, .idle,
        .midi_valid, .midi_data, .midi_full, .buttons,
        .pcm_req(p_req), .pcm_we(p_we), .pcm_addr(p_addr), .pcm_wdata(p_wdata), .pcm_rdata(p_rdata), .pcm_ack(p_ack),
        .pcm_tick(p_tick), .pcm_irq0(p_irq0), .pcm_irq_known(p_known),
        .lcd_we, .lcd_rs, .lcd_data, .lcd_enable,
        .cycles,
        .mon_req(), .mon_we(), .mon_addr(), .mon_wdata(), .mon_rdata(), .mon_ack(),
        .dbg_r(), .dbg_pc(dbg_pc), .dbg_sr(dbg_sr), .dbg_cp(dbg_cp), .dbg_dp(), .dbg_ep(), .dbg_tp(), .dbg_br(),
        .dbg_sleep(), .dbg_int(), .dbg_exign(), .dbg_exc(), .dbg_trapa(), .dbg_traps(),
        .cpu_ld_en(1'b0), .cpu_ld_r(zr), .cpu_ld_pc(16'h0), .cpu_ld_sr(16'h0),
        .cpu_ld_cp(8'h0), .cpu_ld_dp(8'h0), .cpu_ld_ep(8'h0), .cpu_ld_tp(8'h0), .cpu_ld_br(8'h0)
    );

    sc55_pcm pcm (
        .clk, .rst, .mk2, .jv880,
        .cpu_req(p_req), .cpu_we(p_we), .cpu_addr(p_addr), .cpu_wdata(p_wdata), .cpu_rdata(p_rdata), .cpu_ack(p_ack),
        .run(p_tick), .target(cycles), .done(p_done),
        .irq0(p_irq0), .enable_oversampling,
        .rom_req, .rom_addr, .rom_ready, .rom_data, .rom_rvalid,
        .sample_valid, .sample_l, .sample_r,
        .ready(p_ready), .irq_known(p_known)
    );
endmodule
