// SPDX-License-Identifier: MIT
// Yamaha SWP00 (AWM2 voices + MEG effects) as MAME's swp00 device runs it
// (src/devices/sound/swp00.cpp, Olivier Galibert, BSD-3-Clause). Bit-exact to MAME: every sample
// pair equals MAME's put_int_clamp(dry >> 6, 0x20000) output.
//
// Commands follow MAME's stream semantics. Each command carries n, the number of samples that
// must exist before it acts (MAME runs the stream up to the CPU's time first): for a CPU access
// at CPU state s (10 MHz, counted after the access), n = floor(s * 441 / 100000) + 1. The engine
// generates samples until it has n, then applies the command:
//   CMD_GEN    nothing more (keeps the stream at n samples)
//   CMD_WRITE  one byte to register 000-7FF (a 16-bit CPU write is two, the even address first)
//   CMD_READ   one byte from register 000-7FF, on rd_data while cmd_ready
// cmd_ready is high for one clock when the command is done.
//
// Memories (request/acknowledge, one request outstanding, data with the acknowledge):
//   wave ROM: 4 bytes from byte address wv_addr (22 bits, the ROM mirrors every 4 MB), byte
//             wv_addr in wv_data[7:0]; unaligned addresses allowed.
//   delay lines: 32-bit words, reverb 00000-1FFFF, chorus 20000-27FFF, variation 28000-47FFF.
module swp00
    import swp00_pkg::*;
(
    input  logic        clk,
    input  logic        rst,

    input  logic        cmd_valid,
    input  logic [1:0]  cmd_kind,
    input  logic [31:0] cmd_n,
    input  logic [10:0] cmd_reg,
    input  logic [7:0]  cmd_data,
    output logic        cmd_ready,
    output logic [7:0]  rd_data,

    output logic        smp_valid,
    output logic [17:0] smp_l,
    output logic [17:0] smp_r,
    output logic [31:0] sample_count,

    // the seven voice sums of each sample, before the MEG (bench)
    output logic        mix_valid,
    output logic [31:0] mix [7],

    output logic        wv_req,
    output logic [21:0] wv_addr,
    input  logic        wv_ack,
    input  logic [31:0] wv_data,

    output logic        dm_req,
    output logic        dm_we,
    output logic [18:0] dm_addr,
    output logic [31:0] dm_wdata,
    input  logic        dm_ack,
    input  logic [31:0] dm_rdata
);
    localparam logic [1:0] CMD_GEN = 2'd0, CMD_WRITE = 2'd1, CMD_READ = 2'd2;

    // ---------------------------------------------------------------- storage
    voice_t vram [32];
    logic [4:0]  vaddr;
    logic        vwe;
    voice_t      vwd, vrd;
    always_ff @(posedge clk) begin
        if (vwe) vram[vaddr] <= vwd;
        vrd <= vram[vaddr];
    end

    // control registers
    logic [7:0]  state_adr, meg_control, waverom_access;
    logic [31:0] sample_counter;
    assign sample_count = sample_counter;

    // MEG constants (m_const, 0xc0) and offsets (m_offset, 0x40): RAMs of 256 words, constants at
    // 00-BF, offsets at C0-FF. Two copies, for the MEG's two read ports.
    logic [15:0] mram [256], mram2 [256];
    logic [7:0]  m_waddr, m_raddr_cmd, m_raddr_meg, m_raddr, m_raddr2;
    logic [15:0] m_wd, m_rd, m_rd2;
    logic        m_we;
    logic        meg_busy;
    assign m_raddr = meg_busy ? m_raddr_meg : m_raddr_cmd;
    always_ff @(posedge clk) begin
        if (m_we) begin
            mram[m_waddr] <= m_wd;
            mram2[m_waddr] <= m_wd;
        end
        m_rd <= mram[m_raddr];
        m_rd2 <= mram2[m_raddr2];
    end

    // ---------------------------------------------------------------- MEG
    logic        meg_start, meg_done;
    logic        meg_pend;           // the MEG has a sample (started or running)
    logic [31:0] meg_out_l, meg_out_r;
    logic [31:0] buffer_offset;
    logic [31:0] mix_r [7];
    swp00_meg meg (
        .clk, .rst,
        .start(meg_start), .done(meg_done), .busy(meg_busy),
        .variant(meg_control[7:6]), .buffer_offset,
        .mix(mix_r),
        .c_addr(m_raddr_meg), .c_data(m_rd), .c2_addr(m_raddr2), .c2_data(m_rd2),
        .out_l(meg_out_l), .out_r(meg_out_r),
        .dm_req, .dm_we, .dm_addr, .dm_wdata, .dm_ack, .dm_rdata
    );

    // ---------------------------------------------------------------- engine
    typedef enum logic [5:0] {
        S_RESET, S_IDLE,
        S_CMD_RD, S_CMD_RD2, S_CMD_APPLY, S_KEYON_RD, S_KEYON_RD2, S_KEYON_WR,
        S_MRD2, S_WAVEVAL,
        S_V_RD, S_V_RD2, S_V_LD, S_V_LFO, S_V_FETCH, S_V_FETCHA, S_V_FETCH2, S_V_DPCM, S_V_DPCMB,
        S_V_DPCMC, S_V_VALS,
        S_V_INTERP, S_V_POS,
        S_V_F1, S_V_F2, S_V_F3, S_V_F4, S_V_F5, S_V_MIX, S_V_WB, S_V_DONE
    } st_t;
    st_t st;

    voice_t      v;              // the channel being worked on
    logic [4:0]  chan;
    logic [7:0]  keyon_bits;
    logic [2:0]  keyon_i;
    logic [31:0] acc [7];        // dry_l, dry_r, rev, cho_l, cho_r, var_l, var_r

    // per voice
    logic [31:0] amod, fmod, lmod;
    logic [31:0] b0_3, b4_7;     // fetched ROM bytes (little endian)
    logic [15:0] smp1;           // streaming output (s16)
    logic [31:0] kk, hh;         // filter: k (24.0) and h
    logic [24:0] mulv [7];
    logic [2:0]  mix_i;
    logic [31:0] mix_s;          // filter output l
    logic [15:0] env_out;
    logic        wv_second;      // 12-bit format: second fetch
    logic [21:0] fa;             // sample address
    logic [31:0] dtgt;           // pos + 2: where read_8c stops
    logic [1:0]  dbyte;          // compressed format: byte of b0_3 being decoded
    logic [31:0] pdr;            // pos_dec + step + (fmod << 4)
    logic [15:0] dsamp;          // compressed format: the decoded sample
    logic [31:0] ddelta;         // and its delta before the decay
    logic [31:0] val0r, val1r;   // the two samples to interpolate

    // shared multiplier: 33 x 33 signed, product used the clock after the inputs are set
    logic signed [32:0] mul_a, mul_b;
    wire  signed [65:0] prod = mul_a * mul_b;

    // ---------------------------------------------------------------- combinational helpers
    // lfo_block::step
    function automatic logic [21:0] lfo_inc(input logic [7:0] speed);
        logic [2:0] e;
        e = speed[5:3];
        if (e != 3'd7) return 22'({4'h0, 1'b1, speed[2:0]} << e);
        return ({18'h0, speed[2:0], 1'b0} + 22'd8) << 7;
    endfunction

    // the 12-bit/16-bit/8-bit sample pair from the fetched bytes
    function automatic logic [31:0] fetch_vals(input logic [1:0] fmt, input logic [1:0] p3,
                                               input logic [31:0] a, input logic [31:0] b);
        logic [15:0] w0, w1, w2, w3, v0, v1;
        w0 = a[15:0]; w1 = a[31:16]; w2 = b[15:0]; w3 = b[31:16];
        case (fmt)
            2'd0: begin v0 = w0; v1 = w1; end
            2'd1: case (p3)
                2'd0: begin v0 = {w0[11:0], 4'h0};
                            v1 = {w1[7:0], w0[15:8]} & 16'hfff0; end
                2'd1: begin v0 = {w1[7:0], w0[15:8]} & 16'hfff0;
                            v1 = {w2[3:0], w1[15:4]} & 16'hfff0 | {w2[3:0], 12'h0}; end
                2'd2: begin v0 = ({4'h0, w1[15:4]} & 16'h0ff0) | {w2[3:0], 12'h0};
                            v1 = w2 & 16'hfff0; end
                default: begin v0 = w2 & 16'hfff0; v1 = {w3[11:0], 4'h0}; end
            endcase
            default: begin v0 = {a[7:0], 8'h0}; v1 = {a[15:8], 8'h0}; end   // 8-bit
        endcase
        return {v0, v1};
    endfunction

    // envelope_block::step: returns {result, mode, level}
    function automatic logic [33:0] env_step(input voice_t vv, input logic [31:0] sc);
        logic [31:0] level, limit, key;
        logic [1:0]  mode;
        logic [15:0] result;
        level = {{16{vv.env_level[15]}}, vv.env_level};
        mode = vv.env_mode;
        result = vv.env_level;
        case (mode)
            ENV_ATTACK: begin
                if (vv.att_speed[7]) begin
                    level = level - {27'h0, interpolation_step({2'b0, vv.att_speed[6:0]} + {2'b0, level[11:7], 2'b0}, sc)};
                    if ($signed(level) <= 0) begin
                        level = 32'h0; mode = ENV_DECAY;
                    end
                end else begin
                    level = level + {27'h0, interpolation_step({1'b0, vv.att_speed}, sc)};
                    if (vv.att_level != 8'h0) begin
                        limit = {20'h0, vv.att_level, 4'h0};
                        if ($signed(level) >= $signed(limit)) begin
                            mode = ENV_DECAY;
                            if ($signed(level) >= 32'shfff) level = 32'hfff;
                        end
                    end else begin
                        if ($signed(level) >= 32'sh800) begin
                            level = 32'h0; mode = ENV_DECAY;
                        end
                        result = 16'h0;
                    end
                end
            end
            ENV_DECAY: begin
                key = (vv.dec_speed >= 8'he0) ? {24'h0, vv.dec_speed} - 32'h90 + {25'h0, level[11:7], 2'b0}
                                             : {25'h0, vv.dec_speed[6:0]};
                limit = {20'h0, vv.dec_level, 4'h0};
                if ($signed(level) < $signed(limit)) begin
                    level = level + {27'h0, interpolation_step(key[8:0], sc)};
                    if ($signed(level) > $signed(limit)) begin
                        mode = ENV_DONE;
                        if ($signed(level) > 32'shfff) level = 32'hfff;
                    end
                end else if ($signed(level) > $signed(limit)) begin
                    level = level - {27'h0, interpolation_step(key[8:0], sc)};
                    if ($signed(level) < $signed(limit)) begin
                        mode = ENV_DONE;
                        if ($signed(level) < 0) level = 32'h0;
                    end
                end else
                    mode = ENV_DONE;
            end
            default: ;
        endcase
        return {result, mode, level[15:0]};
    endfunction

    // filter_block::step, the k interpolation part (after the filter math)
    function automatic logic [17:0] filter_k_step(input voice_t vv, input logic [31:0] sc);
        logic [31:0] k, kt;
        logic [4:0]  s;
        logic [1:0]  sweep;
        k = {16'h0, vv.f_k}; kt = {16'h0, vv.f_kt}; sweep = vv.f_sweep;
        s = interpolation_step({2'b0, vv.f_speed[6:0]}, sc);
        case (sweep)
            SWEEP_NONE:
                if (k < kt) begin
                    k = k + {27'h0, s};
                    if (k > kt) k = kt;
                end else if (k > kt) begin
                    k = k - {27'h0, s};
                    if ($signed(k) < $signed(kt)) k = kt;
                end
            SWEEP_UP: begin
                k = k + {27'h0, s};
                if (k >= 32'hfff) begin
                    k = kt; sweep = SWEEP_DONE;
                end
            end
            default: ;
        endcase
        return {sweep, k[15:0]};
    endfunction

    function automatic logic [11:0] towards(input logic [11:0] c, input logic [11:0] t);
        if (c < t) return c + 12'd1;
        if (c > t) return c - 12'd1;
        return c;
    endfunction

    // ---------------------------------------------------------------- engine FSM
    logic [7:0]  clr;
    logic [10:0] creg;
    logic [7:0]  cdata;
    logic [1:0]  ckind;

    always_comb begin
        mix = acc;
    end

    always_ff @(posedge clk) begin
        cmd_ready <= 1'b0;
        smp_valid <= 1'b0;
        mix_valid <= 1'b0;
        vwe <= 1'b0;
        m_we <= 1'b0;
        meg_start <= 1'b0;
        if (rst) begin
            st <= S_RESET;
            clr <= 8'h0;
            chan <= 5'd0;
            state_adr <= 8'h0; meg_control <= 8'h0; waverom_access <= 8'h0;
            sample_counter <= 32'h0;
            buffer_offset <= 32'h0;
            meg_pend <= 1'b0;
            wv_req <= 1'b0;
            m_waddr <= 8'h0;
        end else case (st)
            // device_reset: clear() every channel, zero the MEG constants and offsets
            S_RESET: begin
                m_waddr <= clr; m_wd <= 16'h0; m_we <= 1'b1;
                if (clr < 8'd32) begin
                    vaddr <= clr[4:0]; vwd <= voice_clear(); vwe <= 1'b1;
                end
                clr <= clr + 8'd1;
                if (clr == 8'hff) st <= S_IDLE;
            end

            S_IDLE: if (cmd_valid && !cmd_ready) begin
                if (sample_counter < cmd_n) begin
                    // generate one sample: all 32 channels, then the MEG
                    chan <= 5'd0;
                    for (int i = 0; i < 7; i++) acc[i] <= 32'h0;
                    vaddr <= 5'd0;
                    st <= S_V_RD;
                end else if (meg_pend && (cmd_kind == CMD_GEN || cmd_reg == 11'h004 ||
                                          (cmd_reg >= 11'h180 && cmd_reg < 11'h380))) begin
                    // MEG constants, offsets and control (and the end of a run) wait for the
                    // MEG to finish the samples before them
                end else begin
                    creg <= cmd_reg; cdata <= cmd_data; ckind <= cmd_kind;
                    if (cmd_kind == CMD_GEN) cmd_ready <= 1'b1;
                    else st <= S_CMD_RD;
                end
            end

            // ---------------- register access
            S_CMD_RD: begin
                if (creg >= 11'h180 && creg < 11'h380) begin
                    // MEG offsets 180-1FF -> C0-FF, constants 200-37F -> 00-BF
                    m_raddr_cmd <= creg < 11'h200 ? {2'b11, creg[6:1]} : creg[8:1];
                    st <= S_MRD2;
                end else if (creg < 11'h40) begin
                    if (ckind == CMD_WRITE) begin
                        case (creg[5:0])
                            6'h01: state_adr <= cdata;
                            6'h02: waverom_access <= cdata;
                            6'h04: meg_control <= cdata;
                            default: ;
                        endcase
                        if (creg[5:0] >= 6'h08 && creg[5:0] <= 6'h0b && cdata != 8'h0) begin
                            keyon_bits <= cdata;
                            keyon_i <= 3'd0;
                            st <= S_KEYON_RD;
                        end else begin
                            cmd_ready <= 1'b1; st <= S_IDLE;
                        end
                    end else begin
                        case (creg[5:0])
                            6'h01: begin vaddr <= state_adr[4:0]; st <= S_CMD_RD2; end
                            6'h03: begin vaddr <= 5'd31; st <= S_CMD_RD2; end
                            6'h04: begin rd_data <= meg_control; cmd_ready <= 1'b1; st <= S_IDLE; end
                            default: begin rd_data <= 8'h0; cmd_ready <= 1'b1; st <= S_IDLE; end
                        endcase
                    end
                end else if (is_chan_reg(chan_index(creg))) begin
                    vaddr <= creg[5:1];
                    st <= S_CMD_RD2;
                end else begin
                    rd_data <= 8'h0; cmd_ready <= 1'b1; st <= S_IDLE;
                end
            end
            S_CMD_RD2: st <= S_CMD_APPLY;          // vram read latency
            S_CMD_APPLY: begin
                if (creg >= 11'h180 && creg < 11'h380) begin
                    // offset_w / const_w: (offset & 1) ? low byte : high byte
                    if (ckind == CMD_WRITE) begin
                        m_waddr <= m_raddr_cmd;
                        m_wd <= creg[0] ? {m_rd[15:8], cdata} : {cdata, m_rd[7:0]};
                        m_we <= 1'b1;
                    end else
                        rd_data <= creg[0] ? m_rd[7:0] : m_rd[15:8];
                    cmd_ready <= 1'b1; st <= S_IDLE;
                end else if (creg < 11'h40) begin
                    if (creg[5:0] == 6'h01) begin
                        rd_data <= voice_state(vrd, state_adr[7:5], sample_counter);
                        cmd_ready <= 1'b1; st <= S_IDLE;
                    end else begin
                        // 03: waverom_val_r: byte at channel 1f's address, which then counts up
                        wv_req <= 1'b1; wv_addr <= vrd.address[21:0];
                        v <= vrd;
                        st <= S_WAVEVAL;
                    end
                end else if (ckind == CMD_WRITE) begin
                    vwd <= voice_write(vrd, chan_index(creg), cdata); vwe <= 1'b1;
                    cmd_ready <= 1'b1; st <= S_IDLE;
                end else begin
                    rd_data <= voice_read(vrd, chan_index(creg));
                    cmd_ready <= 1'b1; st <= S_IDLE;
                end
            end
            S_WAVEVAL: if (wv_ack) begin
                wv_req <= 1'b0;
                rd_data <= wv_data[7:0];
                vaddr <= 5'd31;
                vwd <= v; vwd.address <= v.address + 24'd1; vwe <= 1'b1;
                cmd_ready <= 1'b1; st <= S_IDLE;
            end
            S_MRD2: st <= S_CMD_APPLY;            // mram read latency
            S_KEYON_RD: begin
                if (keyon_bits[keyon_i]) begin
                    vaddr <= {(2'd3 - (creg[1:0])), keyon_i};   // keyon_w<3-(a-8)>: 8*Sel + i
                    st <= S_KEYON_RD2;
                end else if (keyon_i == 3'd7) begin
                    cmd_ready <= 1'b1; st <= S_IDLE;
                end else
                    keyon_i <= keyon_i + 3'd1;
            end
            S_KEYON_RD2: st <= S_KEYON_WR;
            S_KEYON_WR: begin
                vwd <= voice_keyon(vrd); vwe <= 1'b1;
                if (keyon_i == 3'd7) begin
                    cmd_ready <= 1'b1; st <= S_IDLE;
                end else begin
                    keyon_i <= keyon_i + 3'd1;
                    st <= S_KEYON_RD;
                end
            end

            // ---------------- one sample: the channels
            S_V_RD: begin vaddr <= chan; st <= S_V_RD2; end
            S_V_RD2: st <= S_V_LD;                 // vram read latency
            S_V_LD: begin
                v <= vrd;
                st <= S_V_LFO;
            end
            S_V_LFO: begin : lfo
                logic [21:0] cnt;
                logic [31:0] shaped;
                cnt = v.lfo_counter + lfo_inc(v.lfo_speed);
                v.lfo_counter <= cnt;
                amod <= 32'h0; fmod <= 32'h0; lmod <= 32'h0;
                if (!v.lfo_speed[7]) begin
                    shaped = v.lfo_speed[6] ? {10'h0, {cnt[20:0], 1'b0} ^ (cnt[21] ? 22'h3fffff : 22'h0)}
                                            : {10'h0, cnt};
                    amod <= 32'((64'(shaped) * 64'(v.lfo_lamod[4:0])) >> 16);
                    fmod <= 32'(($signed(64'(shaped) - 64'h200000) * $signed({56'h0, v.lfo_fmod})) >>> 21);
                    lmod <= 32'((64'(shaped) * 64'({v.lfo_lamod[7:5], 5'h0})) >> 20);
                end
                // envelope_block::active()
                if (v.env_mode == ENV_DONE && !($signed({{16{v.env_level[15]}}, v.env_level}) < 32'sh800))
                    st <= S_V_WB;
                else if (v.done) begin
                    smp1 <= v.last_;
                    st <= S_V_F1;
                end else
                    st <= S_V_FETCH;
            end
            // sample address (read_16 / read_12 / read_8 / read_8c), then the request
            S_V_FETCH: begin : fetch
                logic [31:0] q;
                q = 32'($signed(v.pos) >>> 2);
                case (v.format[7:6])
                    2'd0: fa <= 22'({8'h0, v.address} + (v.pos << 1));
                    2'd1: fa <= 22'({8'h0, v.address} + (q << 2) + (q << 1));      // (pos >> 2) * 6
                    2'd2: fa <= 22'({8'h0, v.address} + v.pos);
                    default: fa <= 22'({8'h0, v.address} + v.dpcm_pos);
                endcase
                dtgt <= v.pos + 32'd2;
                st <= S_V_FETCHA;
            end
            S_V_FETCHA: begin
                if (v.format[7:6] == 2'd3 && v.dpcm_pos == dtgt) begin
                    st <= S_V_VALS;               // nothing to decode
                end else begin
                    wv_req <= 1'b1; wv_addr <= fa; wv_second <= 1'b0;
                    st <= v.format[7:6] == 2'd3 ? S_V_DPCM : S_V_FETCH2;
                end
            end
            S_V_FETCH2: if (wv_ack) begin
                if (v.format[7:6] == 2'd1 && !wv_second) begin
                    b0_3 <= wv_data; wv_second <= 1'b1;
                    wv_addr <= wv_addr + 22'd4;
                end else begin
                    wv_req <= 1'b0;
                    if (wv_second) b4_7 <= wv_data; else b0_3 <= wv_data;
                    st <= S_V_VALS;
                end
            end
            // read_8c: dpcm_step over bytes until dpcm_pos == pos + 2, four bytes per fetch
            // read_8c: dpcm_step on each byte until dpcm_pos == pos + 2; four bytes a fetch,
            // one byte a clock
            S_V_DPCM: if (wv_ack) begin
                wv_req <= 1'b0;
                b0_3 <= wv_data;
                dbyte <= 2'd0;
                st <= S_V_DPCMB;
            end
            S_V_DPCMB: begin : dpcm
                logic [47:0] r;
                r = dpcm_sample(v.format, v.dpcm_s1, v.dpcm_delta, b0_3[8*dbyte +: 8]);
                dsamp <= r[47:32]; ddelta <= r[31:0];
                st <= S_V_DPCMC;
            end
            S_V_DPCMC: begin : dpcm2
                logic [31:0] p;
                p = v.dpcm_pos + 32'd1;
                v.dpcm_s0 <= v.dpcm_s1; v.dpcm_s1 <= dsamp; v.dpcm_delta <= dpcm_decay(v.format, ddelta);
                v.dpcm_pos <= p;
                dbyte <= dbyte + 2'd1;
                st <= S_V_DPCMB;
                if (p == dtgt)
                    st <= S_V_VALS;
                else if (dbyte == 2'd3) begin
                    wv_req <= 1'b1; wv_addr <= 22'({8'h0, v.address} + p);
                    st <= S_V_DPCM;
                end
            end
            // the two samples (read_16 / read_12 / read_8 / read_8c)
            S_V_VALS: begin : vals_
                logic [31:0] vals;
                vals = fetch_vals(v.format[7:6], v.pos[1:0], b0_3, b4_7);
                if (v.format[7:6] == 2'd3) begin
                    val0r <= {{16{v.dpcm_s0[15]}}, v.dpcm_s0}; val1r <= {{16{v.dpcm_s1[15]}}, v.dpcm_s1};
                end else begin
                    val0r <= {{16{vals[31]}}, vals[31:16]}; val1r <= {{16{vals[15]}}, vals[15:0]};
                end
                st <= S_V_INTERP;
            end
            S_V_INTERP: begin : interp
                logic [31:0] val0, val1, step, result, dif;
                logic [3:0]  e;
                val0 = val0r; val1 = val1r;
                e = v.pitch[15:12];
                step = ({20'h0, v.pitch[11:0]} << (4'd8 + e)) >> 4;   // 8 + (s16(pitch) >> 12)
                dif = val1 - val0;
                result = val0 + ((32'($signed(dif) * $signed({23'h0, v.pos_dec[14:6]})) >>> 10) << 1);
                smp1 <= result[15:0];
                pdr <= v.pos_dec + step + (fmod << 4);
                st <= S_V_POS;
            end
            // the position update of streaming_block::step
            S_V_POS: begin : posu
                logic [31:0] pd, pos;
                logic        trig;
                pd = pdr;
                pos = v.pos;
                trig = 1'b0;
                if ($signed(pd) >= 32'sh8000) begin
                    v.first_ <= 1'b0;
                    pos = pos + 32'($signed(pd) >>> 15);
                    pd = pd & 32'h7fff;
                    if ($signed(pos) >= $signed({16'h0, v.loop_})) begin
                        if (v.loop_ != 16'h0) begin
                            pos = pos - {16'h0, v.loop_};
                            if (v.format[7:6] != 2'd3) begin
                                pd = pd + ({24'h0, v.format} << 9 & 32'h7e00);
                                if ($signed(pd) >= 32'sh8000) pos = pos + 32'd1;
                                pd = pd & 32'h7fff;
                            end
                            v.dpcm_pos <= 32'd1;
                        end else begin
                            v.done <= 1'b1;
                            v.last_ <= smp1;
                            trig = 1'b1;
                        end
                    end
                end
                v.pos <= pos; v.pos_dec <= pd;
                if (trig) v.env_mode <= ENV_DECAY;    // trigger_release
                st <= S_V_F1;
            end
            // filter_block::step
            S_V_F1: begin : f1
                logic [31:0] km;
                km = {16'h0, v.f_k} - lmod;
                if ($signed(km) < 0) km = 32'h0;
                kk <= ({23'h0, 9'h101} + {24'h0, km[7:0]}) << km[11:8];
                mul_a <= {17'h0, v.f_q}; mul_b <= {v.f_b[31], v.f_b};
                st <= S_V_F2;
            end
            S_V_F2: begin : f2
                logic [31:0] h;
                h = ({{16{smp1[15]}}, smp1} << 6) - v.f_l - prod[38:7];
                mul_a <= {1'b0, kk}; mul_b <= {h[31], h};
                st <= S_V_F3;
            end
            S_V_F3: begin : f3
                logic [31:0] b;
                b = v.f_b + prod[55:24];
                v.f_b <= b;
                mul_a <= {1'b0, kk}; mul_b <= {b[31], b};
                st <= S_V_F4;
            end
            S_V_F4: begin : f4
                logic [31:0] l;
                logic [17:0] ks;
                logic [33:0] es;
                l = v.f_l + prod[55:24];
                v.f_l <= l;
                mix_s <= l;
                ks = filter_k_step(v, sample_counter);
                v.f_sweep <= ks[17:16]; v.f_k <= ks[15:0];
                es = env_step(v, sample_counter);
                v.env_mode <= es[17:16]; v.env_level <= es[15:0];
                env_out <= es[33:18];
                st <= S_V_F5;
            end
            S_V_F5: begin : f5
                logic [31:0] base;
                // mixer_block::step: base = u16(envelope + tremolo + cglo)
                base = {16'h0, 16'(env_out + amod[15:0] + {4'h0, v.cglo})};
                mulv[0] <= volume_mul(base + {20'h0, v.dry, 4'h0} + {20'h0, v.cpanl});
                mulv[1] <= volume_mul(base + {20'h0, v.dry, 4'h0} + {20'h0, v.cpanr});
                mulv[2] <= volume_mul(base + {20'h0, v.rev, 4'h0});
                mulv[3] <= volume_mul(base + {20'h0, v.cho, 4'h0} + {20'h0, v.cpanl});
                mulv[4] <= volume_mul(base + {20'h0, v.cho, 4'h0} + {20'h0, v.cpanr});
                mulv[5] <= volume_mul(base + {20'h0, v.var_, 4'h0} + {20'h0, v.cpanl});
                mulv[6] <= volume_mul(base + {20'h0, v.var_, 4'h0} + {20'h0, v.cpanr});
                mix_i <= 3'd0;
                st <= S_V_MIX;
            end
            // seven products, one a clock: issue i, add i-1
            S_V_MIX: begin
                if (mix_i != 3'd0)
                    acc[mix_i - 3'd1] <= acc[mix_i - 3'd1] + prod[55:24];
                if (mix_i != 3'd7) begin
                    mul_a <= {mix_s[31], mix_s}; mul_b <= {8'h0, mulv[mix_i]};
                    mix_i <= mix_i + 3'd1;
                end else begin
                    v.cglo <= towards(v.cglo, v.tglo);
                    v.cpanl <= towards(v.cpanl, v.tpanl);
                    v.cpanr <= towards(v.cpanr, v.tpanr);
                    st <= S_V_WB;
                end
            end
            S_V_WB: begin
                vaddr <= chan; vwd <= v; vwe <= 1'b1;
                if (chan == 5'd31)
                    st <= S_V_DONE;
                else begin
                    chan <= chan + 5'd1;
                    st <= S_V_RD;
                end
            end
            // the channels of this sample are done: hand their sums to the MEG once it has
            // finished the previous sample; the MEG then runs while the next sample's channels do
            S_V_DONE: if (!meg_pend) begin
                for (int i = 0; i < 7; i++) mix_r[i] <= acc[i];
                mix_valid <= 1'b1;
                meg_start <= 1'b1;
                meg_pend <= 1'b1;
                sample_counter <= sample_counter + 32'd1;
                st <= S_IDLE;
            end
            default: st <= S_IDLE;
        endcase

        // the MEG's end of a sample: output, the delay lines move on
        if (!rst && meg_done) begin : out
            logic signed [31:0] l, r;
            l = $signed(meg_out_l) >>> 6;
            r = $signed(meg_out_r) >>> 6;
            smp_l <= l < -32'sh20000 ? 18'h20000 : l > 32'sh1ffff ? 18'h1ffff : l[17:0];
            smp_r <= r < -32'sh20000 ? 18'h20000 : r > 32'sh1ffff ? 18'h1ffff : r[17:0];
            smp_valid <= 1'b1;
            buffer_offset <= buffer_offset - 32'd1;
            meg_pend <= 1'b0;
        end

    end
endmodule
