// Roland SC-55 PCM chip, behavioural, matched to the reference model's
// pcm.cpp (PCM_Write, PCM_Read, PCM_Update, calc_tv, eram_pack/unpack).
//
// One frame = one iteration of PCM_Update's loop: final mix (1 or 2 output
// samples), envelope counter, reverb/chorus over eram, the reverb address
// generator, then reg_slots voices. `run` with `target` processes frames
// while pcm_cycles < target, then pulses `done` (PCM_Update(cycles)).
//
// Storage: ram1 (32 slots x 8 x 32 bit) and ram2 (32 x 16 x 16 bit) as rows;
// rows 28..31 (reverb/mix state) are copied into registers for the frame.
// The voice loop assumes reg_slots <= 28 (the SC-55 uses 24; checked by the bench).
//
// mk1 only for now: wave ROM banks 0..2 = waverom1..3 (1 MB each), mk1 filter.

module sc55_pcm (
    input  logic        clk,
    input  logic        rst,

    // CPU side (e000-e03f)
    input  logic        cpu_req,
    input  logic        cpu_we,
    input  logic [5:0]  cpu_addr,
    input  logic [7:0]  cpu_wdata,
    output logic [7:0]  cpu_rdata,
    output logic        cpu_ack,

    // PCM_Update(target)
    input  logic        run,
    input  logic [47:0] target,
    output logic        done,

    output logic        irq0,               // INTERRUPT_SOURCE_IRQ0 request (= irq_assert)
    input  logic        enable_oversampling,

    // wave ROM: linear byte address (bank * 1 MB + offset), data with ack
    output logic        rom_req,
    output logic [21:0] rom_addr,
    input  logic [7:0]  rom_data,
    input  logic        rom_ack,

    // MCU_PostSample
    output logic        sample_valid,
    output logic signed [31:0] sample_l,
    output logic signed [31:0] sample_r,
    output logic        ready,              // power-on clear finished
    output logic        irq_known           // irq0 already equals its value after the frames due by `target`
);

    // ------------------------------------------------------------------
    // State (names as in pcm_t)
    // ------------------------------------------------------------------
    logic [31:0] ram1 [32][8];
    logic [15:0] ram2 [32][16];
    logic [15:0] pitch [32];            // copy of ram2[n][0] for the indirect read
    logic [15:0] eram [16384];
    logic [47:0] pcm_cycles;
    logic [31:0] voice_mask, voice_mask_pending, write_latch, read_latch, wave_read_address;
    logic [15:0] tv_counter;
    logic [7:0]  wave_byte_latch, config_reg_3c, config_reg_3d, irq_channel;
    logic [4:0]  select_channel;
    logic        irq_assert, voice_mask_updating, nfs;
    logic signed [31:0] accum_l, accum_r;
    logic signed [31:0] rcsum [2];
    // PCM_Config
    logic [31:0] cfg_orval;
    logic [7:0]  cfg_noise_mask, cfg_write_mask;
    logic        cfg_oversampling;
    logic [5:0]  reg_slots;

    // The CPU may run on while frames are processed (sc55_mcu). Register
    // accesses wait until the PCM has caught up with `target`. IRQ0 can only
    // be set by a frame, and only the first voice whose condition holds sets
    // it; P_PRESCAN finds that at the start of each frame from the rows as
    // they are then (a voice changes only its own row), so the CPU's
    // interrupt check need not wait for the whole frame.
    logic        pre_done, pre_hit;
    logic [5:0]  pre_i;
    wire  [47:0] frame_len = ({42'h0, reg_slots} + 48'd1) * 48'd25;
    wire         caught_up = (pcm_cycles >= target);
    assign irq0 = irq_assert | (pre_done & pre_hit);
    assign irq_known = (ps == P_IDLE && caught_up) || (pre_done && (pcm_cycles + frame_len >= target));
    assign ready = (ps != P_CLEAR);

    // ------------------------------------------------------------------
    // Arithmetic helpers (C int semantics)
    // ------------------------------------------------------------------
    function automatic logic signed [31:0] sx20(input logic [31:0] v);
        return {{12{v[19]}}, v[19:0]};
    endfunction
    function automatic logic signed [31:0] addclip20(input logic [31:0] a, input logic [31:0] b, input logic [31:0] cin);
        return sx20(a) + sx20(b) + $signed(cin);
    endfunction
    function automatic logic signed [31:0] multi(input logic [31:0] a, input logic [7:0] b);
        logic signed [31:0] p;
        p = sx20(a) * $signed({{24{b[7]}}, b});
        return p;
    endfunction
    function automatic logic signed [31:0] s8(input logic [7:0] b);
        return {{24{b[7]}}, b};
    endfunction

    function automatic logic [11:0] interp_lut(input logic [1:0] t, input logic [6:0] i);
        `include "sc55_pcm_interp.svh"
    endfunction

    // calc_tv: returns {levelcur_new[15:0], volmul[15:0]}
    function automatic logic [31:0] calc_tv(input int e, input logic [15:0] adjust, input logic [15:0] levelcur_in,
                                            input logic active, input logic [15:0] tvc, input logic nfs_f);
        int speed, tgt, type_, addlow, shift, sum1, preshift, shifted, sum2, sum2_l, sum3;
        logic w1, w2, w3, wr, neg, neg2, xnor_;
        logic [15:0] levelcur;
        int volmul;
        levelcur = levelcur_in & 16'h7fff;
        volmul = 0;
        speed = int'(adjust[7:0]);
        tgt = int'(adjust[15:8]);
        w1 = (speed & 'hf0) == 0;
        w2 = w1 || ((speed & 'h10) != 0);
        w3 = nfs_f && (((speed & 'h80) == 0) || (((speed & 'h40) == 0) && (!w2 || ((speed & 'h20) == 0))));
        type_ = int'(w2) | (int'(w3) << 3);
        if ((speed & 'h20) != 0) type_ |= 2;
        if (((speed & 'h80) == 0) || ((speed & 'h40) == 0)) type_ |= 4;
        wr = !active;
        addlow = 0;
        if ((type_ & 4) != 0) begin
            if (tvc[3]) addlow |= 1;
            if (tvc[2]) addlow |= 2;
            if (tvc[1]) addlow |= 4;
            if (tvc[0]) addlow |= 8;
            wr = 1'b1;
        end else begin
            case (type_ & 3)
                0: begin
                    if (tvc[5]) addlow |= 1; if (tvc[4]) addlow |= 2; if (tvc[3]) addlow |= 4; if (tvc[2]) addlow |= 8;
                    wr = wr | (tvc[1:0] == 2'b0);
                end
                1: begin
                    if (tvc[7]) addlow |= 1; if (tvc[6]) addlow |= 2; if (tvc[5]) addlow |= 4; if (tvc[4]) addlow |= 8;
                    wr = wr | (tvc[3:0] == 4'b0);
                end
                2: begin
                    if (tvc[9]) addlow |= 1; if (tvc[8]) addlow |= 2; if (tvc[7]) addlow |= 4; if (tvc[6]) addlow |= 8;
                    wr = wr | (tvc[5:0] == 6'b0);
                end
                default: begin
                    if (tvc[11]) addlow |= 1; if (tvc[10]) addlow |= 2; if (tvc[9]) addlow |= 4; if (tvc[8]) addlow |= 8;
                    wr = wr | (tvc[6:0] == 7'b0);
                end
            endcase
        end
        if ((type_ & 8) == 0) begin
            shift = speed & 15;
            shift = (10 - shift) & 15;
            sum1 = tgt << 11;
            if (e != 2 || active) sum1 -= (int'(levelcur) << 4);
            preshift = sum1;
            shifted = preshift >>> shift;
            shifted -= sum1;
            sum2 = (tgt << 11) + addlow + shifted;
            if (wr && nfs_f) levelcur = 16'((sum2 >>> 4) & 'h7fff);
            if (e == 0 || e == 1) volmul = (sum2 >>> 4) & 'h7ffe;
        end else begin
            shift = (speed >>> 4) & 14;
            shift |= int'(w2);
            shift = (10 - shift) & 15;
            sum1 = tgt << 11;
            if (e != 2 || active) sum1 -= (int'(levelcur) << 4);
            neg = (sum1 & 'h80000) != 0;
            preshift = (speed & 15) << 9;
            if (!w1) preshift |= 'h2000;
            if (neg) preshift ^= ~'h3f;
            shifted = preshift >>> shift;
            sum2 = shifted;
            if (e != 2 || active) sum2 += (int'(levelcur) << 4) | addlow;
            sum2_l = sum2 >>> 4;
            sum3 = (tgt << 11) - (sum2_l << 4);
            neg2 = (sum3 & 'h80000) != 0;
            xnor_ = !(neg2 ^ neg);
            if (wr && nfs_f) begin
                if (xnor_) levelcur = 16'(sum2_l & 'h7fff);
                else levelcur = 16'(tgt << 7);
            end
            if (e == 0) volmul = sum2_l & 'h7ffe;
            else if (e == 1) volmul = xnor_ ? (sum2_l & 'h7ffe) : (tgt << 7);
        end
        return {levelcur, 16'(volmul)};
    endfunction

    // ------------------------------------------------------------------
    // eram (16K x 16): one access per cycle
    // ------------------------------------------------------------------
    function automatic logic signed [31:0] eram_unpack_v(input logic [15:0] data, input int type_);
        int val, sh;
        val = int'({16'h0, data & 16'h3fff});
        sh = int'(data[15:14]);
        val = val << 18;
        return val >>> (18 - sh * 2 + type_);
    endfunction
    function automatic logic [15:0] eram_pack_v(input logic [31:0] val);
        int sh, top, data;
        top = int'((val >> 13) & 32'h7f);
        if ((top & 'h40) != 0) top ^= 'h7f;
        if (top >= 16) sh = 3;
        else if (top >= 4) sh = 2;
        else if (top >= 1) sh = 1;
        else sh = 0;
        data = int'((val >> (sh * 2)) & 32'h3fff);
        data |= sh << 14;
        return 16'(data);
    endfunction

    // ------------------------------------------------------------------
    // ROM bank decode (PCM_ReadROM, mk1)
    // ------------------------------------------------------------------
    function automatic logic [22:0] rom_map(input logic [31:0] address, input logic [7:0] r3d);
        // {valid, linear address}
        logic [2:0] bank;
        bank = r3d[5] ? address[23:21] : address[21:19];
        case (bank)
            3'd0: return {1'b1, 2'd0, address[19:0]};
            3'd1: return {1'b1, 2'd1, address[19:0]};
            3'd2: return {1'b1, 2'd2, address[19:0]};
            default: return {1'b0, 22'h0};
        endcase
    endfunction

    // ------------------------------------------------------------------
    // Frame engine registers
    // ------------------------------------------------------------------
    logic [31:0] g1 [4][8];     // ram1 rows 28..31
    logic [15:0] g2 [4][16];    // ram2 rows 28..31
    logic [31:0] voice_active;
    logic signed [31:0] rcadd [6], rcadd2 [6];
    logic signed [31:0] acc1, acc3;   // ram1[31][1], ram1[31][3] during the voice loop
    logic [5:0]  slot;
    logic [31:0] r1 [8];
    logic [15:0] r2 [16];
    logic signed [31:0] shf;          // mix shifter

    // voice temporaries
    logic        v_active, v_kon, v_key, v_okey, v_b6, v_b7, v_irq_flag, v_newsel, v_usenew, v_next_b15;
    logic [3:0]  v_old_nibble, v_hiaddr;
    logic [19:0] v_wave_address;
    logic [31:0] v_addr [4];
    logic        v_ncmp [2:6];
    logic [2:0]  v_spo;
    logic [6:0]  v_ratio;
    logic [31:0] v_next_address;
    logic [7:0]  v_rom [5];       // nibble byte, samp0..3
    logic [2:0]  v_rom_i;
    logic signed [31:0] v_reference, v_test, v_v3, v_sample3;
    logic [15:0] v_volmul1, v_volmul2;

    // reverb section: eram access sequencing
    logic [6:0]  rv_pc;           // reverb micro-step
    logic [15:0] er_q;            // eram read data (registered)

    typedef enum logic [4:0] {
        P_IDLE, P_FRAME, P_LOADG, P_MIX, P_REVERB, P_AGEN, P_SLOT0, P_SLOT_RD, P_SLOT_AG, P_SLOT_ROM,
        P_SLOT_DPCM, P_SLOT_FILT, P_SLOT_TV0, P_SLOT_TV1, P_SLOT_VOL, P_SLOT_MIX, P_SLOT_WB, P_FRAME_END,
        P_CPU_ROM, P_CLEAR, P_PRESCAN
    } pst_t;
    pst_t ps;
    logic [2:0] loadg_i;

    // CPU side ROM read (wave_read_address write)
    logic cpu_busy;
    logic [13:0] clr_a;           // power-on clear of eram (the model starts zeroed)

    // ------------------------------------------------------------------
    // Main process
    // ------------------------------------------------------------------
    always_ff @(posedge clk) begin
        // temporaries
        logic [5:0]  a;
        int          ix;
        logic [31:0] t32;
        logic signed [31:0] m1, m2, v1s, v2s, v3s, s1, s2, t1, t2;
        logic [15:0] v1;
        logic [22:0] rm;
        logic [31:0] ct;

        done <= 1'b0;
        cpu_ack <= 1'b0;
        sample_valid <= 1'b0;

        if (rst) begin
            ps <= P_CLEAR;
            clr_a <= 14'd0;
            for (int i = 0; i < 32; i++) begin
                for (int j = 0; j < 8; j++) ram1[i][j] <= 32'h0;
                for (int j = 0; j < 16; j++) ram2[i][j] <= 16'h0;
                pitch[i] <= 16'h0;
            end
            pcm_cycles <= 48'd0;
            voice_mask <= 32'h0; voice_mask_pending <= 32'h0; write_latch <= 32'h0; read_latch <= 32'h0;
            wave_read_address <= 32'h0; tv_counter <= 16'h0; wave_byte_latch <= 8'h0;
            select_channel <= 5'h0; config_reg_3c <= 8'h0; config_reg_3d <= 8'h0; irq_channel <= 8'h0;
            irq_assert <= 1'b0; voice_mask_updating <= 1'b0; nfs <= 1'b0;
            pre_done <= 1'b0; pre_hit <= 1'b0;
            accum_l <= 32'sh0; accum_r <= 32'sh0; rcsum[0] <= 32'sh0; rcsum[1] <= 32'sh0;
            cfg_orval <= 32'h0; cfg_noise_mask <= 8'h0; cfg_write_mask <= 8'h0; cfg_oversampling <= 1'b0;
            reg_slots <= 6'd1;
            rom_req <= 1'b0;
            cpu_busy <= 1'b0;
        end else begin
            // ---------------------------------------------------- CPU access
            if (cpu_req && !cpu_ack && !cpu_busy && ps == P_IDLE && caught_up) begin
                a = cpu_addr;
                if (cpu_we) begin
                    // PCM_Write
                    cpu_ack <= 1'b1;
                    if (a < 6'h04) begin
                        case (a[1:0])
                            2'd0: voice_mask_pending <= (voice_mask_pending & ~32'h0f000000) | ({28'h0, cpu_wdata[3:0]} << 24);
                            2'd1: voice_mask_pending <= (voice_mask_pending & ~32'h00ff0000) | ({24'h0, cpu_wdata} << 16);
                            2'd2: voice_mask_pending <= (voice_mask_pending & ~32'h0000ff00) | ({24'h0, cpu_wdata} << 8);
                            default: voice_mask_pending <= (voice_mask_pending & ~32'h000000ff) | {24'h0, cpu_wdata};
                        endcase
                        voice_mask_updating <= 1'b1;
                    end else if (a >= 6'h20 && a < 6'h24) begin
                        case (a[1:0])
                            2'd1: wave_read_address <= (wave_read_address & ~32'hff0000) | ({24'h0, cpu_wdata} << 16);
                            2'd2: wave_read_address <= (wave_read_address & ~32'h00ff00) | ({24'h0, cpu_wdata} << 8);
                            2'd3: begin
                                t32 = (wave_read_address & ~32'h0000ff) | {24'h0, cpu_wdata};
                                wave_read_address <= t32;
                                rm = rom_map(t32, config_reg_3d);
                                if (rm[22]) begin
                                    // read the byte before acknowledging the CPU
                                    cpu_ack <= 1'b0;
                                    cpu_busy <= 1'b1;
                                    rom_req <= 1'b1; rom_addr <= rm[21:0];
                                    ps <= P_CPU_ROM;
                                end else wave_byte_latch <= 8'h00;
                            end
                            default: ;
                        endcase
                    end else if (a == 6'h3c) begin
                        // PCM_GetConfig: fields only change where the model assigns them
                        config_reg_3c <= cpu_wdata;
                        ct = cfg_orval;
                        if (cpu_wdata[5:4] != 2'b00) begin
                            case (cpu_wdata[3:2]) 2'd1: cfg_noise_mask <= 8'd3; 2'd2: cfg_noise_mask <= 8'd7; 2'd3: cfg_noise_mask <= 8'd15; default: ; endcase
                            case (cpu_wdata[1:0]) 2'd1: ct |= 32'h100; 2'd2: ct |= 32'h400; default: ; endcase
                            cfg_write_mask <= cpu_wdata[7] ? 8'd15 : 8'd0;
                        end else begin
                            case (cpu_wdata[3:2]) 2'd2: cfg_noise_mask <= 8'd1; 2'd3: cfg_noise_mask <= 8'd3; default: ; endcase
                            case (cpu_wdata[1:0]) 2'd1: ct |= 32'h40; 2'd2: ct |= 32'h100; default: ; endcase
                            cfg_write_mask <= cpu_wdata[7] ? 8'd3 : 8'd0;
                        end
                        if (cpu_wdata[5:4] == 2'b11) ct |= 32'h1000;
                        cfg_orval <= ct;
                        if (cpu_wdata[6]) cfg_oversampling <= 1'b1;
                    end else if (a == 6'h3d) begin
                        config_reg_3d <= cpu_wdata;
                        reg_slots <= {1'b0, cpu_wdata[4:0]} + 6'd1;
                    end else if (a == 6'h3e) begin
                        select_channel <= cpu_wdata[4:0];
                    end else if ((a >= 6'h04 && a < 6'h10) || (a >= 6'h24 && a < 6'h30)) begin
                        t32 = write_latch;
                        case (a[1:0])
                            2'd1: t32 = (t32 & ~32'hf0000) | ({28'h0, cpu_wdata[3:0]} << 16);
                            2'd2: t32 = (t32 & ~32'h0ff00) | ({24'h0, cpu_wdata} << 8);
                            2'd3: t32 = (t32 & ~32'h000ff) | {24'h0, cpu_wdata};
                            default: ;
                        endcase
                        write_latch <= t32;
                        if (a[1:0] == 2'd3) begin
                            ix = 0;
                            if (a[5]) ix |= 1;
                            if (!a[3]) ix |= 4;
                            if (!a[2]) ix |= 2;
                            ram1[select_channel][ix] <= t32;
                        end
                    end else if ((a >= 6'h10 && a < 6'h20) || (a >= 6'h30 && a < 6'h38)) begin
                        t32 = write_latch;
                        if (!a[0]) t32 = (t32 & ~32'hff00) | ({24'h0, cpu_wdata} << 8);
                        else t32 = (t32 & ~32'h00ff) | {24'h0, cpu_wdata};
                        write_latch <= t32;
                        if (a[0]) begin
                            ix = int'(a[3:1]);
                            if (a[5]) ix |= 8;
                            ram2[select_channel][ix] <= t32[15:0];
                            if (ix == 0) pitch[select_channel] <= t32[15:0];
                        end
                    end
                end else begin
                    // PCM_Read
                    cpu_ack <= 1'b1;
                    cpu_rdata <= 8'h00;
                    if (a < 6'h04) begin
                        if (voice_mask_updating) voice_mask <= voice_mask_pending;
                        voice_mask_updating <= 1'b0;
                    end else if (a == 6'h3c || a == 6'h3e) begin
                        if (a == 6'h3e && irq_assert) irq_assert <= 1'b0;
                        cpu_rdata <= irq_channel | (voice_mask_updating ? 8'd32 : 8'd0);
                    end else if (a == 6'h3f) begin
                        cpu_rdata <= wave_byte_latch;
                    end else if ((a >= 6'h04 && a < 6'h10) || (a >= 6'h24 && a < 6'h30)) begin
                        if (a[1:0] == 2'd1) begin
                            ix = 0;
                            if (a[5]) ix |= 1;
                            if (!a[3]) ix |= 4;
                            if (!a[2]) ix |= 2;
                            read_latch <= ram1[select_channel][ix];
                        end
                    end else if ((a >= 6'h10 && a < 6'h20) || (a >= 6'h30 && a < 6'h38)) begin
                        if (!a[0]) begin
                            ix = int'(a[3:1]);
                            if (a[5]) ix |= 8;
                            read_latch <= {16'h0, ram2[select_channel][ix]};
                        end
                    end else if (a >= 6'h39 && a <= 6'h3b) begin
                        case (a[1:0])
                            2'd1: cpu_rdata <= {4'h0, read_latch[19:16]};
                            2'd2: cpu_rdata <= read_latch[15:8];
                            default: cpu_rdata <= read_latch[7:0];
                        endcase
                    end
                end
            end

            case (ps)
            P_CLEAR: begin
                eram[clr_a] <= 16'h0;
                clr_a <= clr_a + 14'd1;
                if (clr_a == 14'h3fff) ps <= P_IDLE;
            end

            P_IDLE: begin
                if (run || !caught_up) ps <= P_FRAME;
            end

            P_CPU_ROM: begin
                if (rom_ack) begin
                    rom_req <= 1'b0;
                    wave_byte_latch <= rom_data;
                    cpu_busy <= 1'b0;
                    cpu_ack <= 1'b1;
                    ps <= P_IDLE;
                end
            end

            P_FRAME: begin
                if (pcm_cycles >= target) begin
                    done <= 1'b1;
                    ps <= P_IDLE;
                end else begin
                    voice_active <= voice_mask & voice_mask_pending;
                    loadg_i <= 3'd0;
                    pre_i <= 6'd0;
                    pre_hit <= 1'b0;
                    pre_done <= 1'b0;
                    ps <= P_PRESCAN;
                end
            end

            P_PRESCAN: begin
                // the voice loop's IRQ condition for slot pre_i, from its row at frame start
                logic okey_, key_, active_, kon_, b15_, b7_, f;
                int address_, aend, aloop, cmp1_;
                if (pre_i >= reg_slots) begin
                    pre_done <= 1'b1;
                    ps <= P_LOADG;
                end else begin
                    okey_ = ram2[pre_i[4:0]][7][5];
                    key_ = voice_active[pre_i[4:0]];
                    active_ = okey_ && key_;
                    kon_ = key_ && !okey_;
                    b15_ = ram2[pre_i[4:0]][8][15];
                    b7_ = ram2[pre_i[4:0]][7][7];
                    address_ = int'(ram1[pre_i[4:0]][4]);
                    aend = int'(ram1[pre_i[4:0]][0]);
                    aloop = int'(ram1[pre_i[4:0]][2]);
                    cmp1_ = b15_ ? aloop : aend;
                    if (kon_) f = ((cmp1_ + aloop) & 'h100000) != 0;
                    else f = ((address_ + ((-aloop) & 'hfffff)) & 'h100000) != 0;
                    f = f ^ b7_;
                    if (active_ && ram2[pre_i[4:0]][6][0] && !ram2[pre_i[4:0]][8][14] && !irq_assert && f) pre_hit <= 1'b1;
                    pre_i <= pre_i + 6'd1;
                end
            end

            P_LOADG: begin
                for (int j = 0; j < 8; j++) g1[loadg_i[1:0]][j] <= ram1[28 + int'(loadg_i[1:0])][j];
                for (int j = 0; j < 16; j++) g2[loadg_i[1:0]][j] <= ram2[28 + int'(loadg_i[1:0])][j];
                if (loadg_i == 3'd3) ps <= P_MIX;
                loadg_i <= loadg_i + 3'd1;
            end

            // ------------------------------------------------ final mixing
            P_MIX: begin
                logic signed [31:0] al, ar, sh;
                logic [15:0] shv;
                logic xr;
                logic [31:0] r30_0, r30_1, r30_2, r30_3, r30_4, r30_5, nz;
                shv = g2[2][10];
                xr = shv[0] ^ shv[1] ^ shv[7] ^ shv[12];
                shv = {xr, shv[15:1]};
                nz = cfg_orval | ({16'h0, shv} & {24'h0, cfg_noise_mask});
                al = addclip20(accum_l, g1[2][0], 0);
                ar = addclip20(accum_r, g1[2][1], 0);
                r30_2 = addclip20(al, nz, 0);
                r30_4 = addclip20(ar, nz, 0);
                r30_0 = al & {24'h0, cfg_write_mask};
                r30_1 = ar & {24'h0, cfg_write_mask};
                sample_valid <= 1'b1;
                sample_l <= (r30_2 & ~{24'h0, cfg_write_mask}) << 12;
                sample_r <= (r30_4 & ~{24'h0, cfg_write_mask}) << 12;
                // second half
                xr = shv[0] ^ shv[1] ^ shv[7] ^ shv[12];
                sh = {16'h0, xr, shv[15:1]};
                nz = cfg_orval | (sh & {24'h0, cfg_noise_mask});
                al = addclip20(al, r30_0, 0);
                ar = addclip20(ar, r30_1, 0);
                r30_3 = addclip20(al, nz, 0);
                r30_5 = addclip20(ar, nz, 0);
                g2[2][10] <= shv;
                if (enable_oversampling && cfg_oversampling) begin
                    g2[2][10] <= sh[15:0];
                    r30_0 = al & {24'h0, cfg_write_mask};
                    r30_1 = ar & {24'h0, cfg_write_mask};
                    // second sample goes out next cycle (P_REVERB's first step)
                    shf <= 32'sd1;
                end else shf <= 32'sd0;
                accum_l <= al; accum_r <= ar;
                g1[2][0] <= r30_0; g1[2][1] <= r30_1; g1[2][2] <= r30_2; g1[2][3] <= r30_3;
                g1[2][4] <= r30_4; g1[2][5] <= r30_5;

                // global counter for envelopes
                begin
                    logic [15:0] tvc;
                    tvc = nfs ? tv_counter : g2[3][8];
                    tv_counter <= (tvc - 16'd1) & 16'h3fff;
                end
                // fixme block: two if/else assignments, the second one wins where they overlap
                begin
                    logic [15:0] r8, d;
                    logic [15:0] n9, n10;
                    r8 = g2[3][8];
                    d = 16'h4000 - r8;            // bit 15 of the int result = bit 15 of the 16-bit difference
                    n9 = g2[3][9]; n10 = g2[3][10];
                    if (r8[15]) n9 = r8 & 16'h7fff; else n10 = r8 & 16'h7fff;
                    if (d[15]) n10 = d & 16'h7fff; else n9 = d & 16'h7fff;
                    g2[3][9] <= n9; g2[3][10] <= n10;
                end
                rv_pc <= 7'd0;
                ps <= P_REVERB;
            end

            // ------------------------------------------------ reverb/chorus
            P_REVERB: begin
                if (shf == 32'sd1) begin
                    sample_valid <= 1'b1;
                    sample_l <= (g1[2][3] & ~{24'h0, cfg_write_mask}) << 12;
                    sample_r <= (g1[2][5] & ~{24'h0, cfg_write_mask}) << 12;
                    shf <= 32'sd0;
                end
                `include "sc55_pcm_reverb.svh"
            end

            // ------------------------------- reverb address generator (row 31)
            P_AGEN: begin
                logic okey_, active_, kon_, b15_, b6_, b7_, acmp, a_add, a_sub, nb15;
                int address_, aend, aloop, sp, spo_, acnt, acnt2, cmp1_, nxt, ad4, t2_, t3_;
                logic [15:0] r8n;
                okey_ = g2[3][7][5];
                active_ = okey_;
                kon_ = !okey_;
                b15_ = g2[3][8][15];
                b6_ = g2[3][7][6];
                b7_ = g2[3][7][7];
                address_ = int'(g1[3][4]);
                aend = int'(g1[3][0]);
                aloop = int'(g1[3][2]);
                sp = int'({16'h0, g2[3][8] & 16'h3fff});
                sp = sp + int'(pitch[g2[3][7][4:0]]);
                spo_ = (sp >>> 14) & 7;
                r8n = g2[3][8];
                if (nfs) r8n = (r8n & ~16'h3fff) | 16'(sp & 'h3fff);
                acnt = address_;
                cmp1_ = b15_ ? aloop : aend;
                acmp = (cmp1_ & 'hfffff) == (acnt & 'hfffff);
                nb15 = b15_;
                nxt = acnt;
                cmp1_ = (!b6_ && acmp) ? aloop : acnt;
                acnt2 = (kon_ || (!b6_ && acmp)) ? cmp1_ : acnt;
                a_add = (!acmp && b6_ && !b15_) || (!acmp && !b6_);
                a_sub = !acmp && b6_ && b15_;
                if (b7_) acnt2 = acnt2 - (int'(a_add) - int'(a_sub));
                else acnt2 = acnt2 + (int'(a_add) - int'(a_sub));
                acnt = acnt2 & 'hfffff;
                b15_ = b6_ && (b15_ ^ acmp);
                if (spo_ >= 1) begin nxt = acnt; nb15 = b15_; end
                ad4 = address_;
                if (active_ && nfs) begin ad4 = nxt; g1[3][4] <= 32'(nxt); end
                if (nfs) r8n = (r8n & ~16'h8000) | {nb15, 15'h0};
                g2[3][8] <= r8n;
                t2_ = ad4 - aloop;
                t3_ = aend - t2_;
                g2[1][10] <= 16'(t3_);
                g2[1][11] <= 16'(ad4);
                ps <= P_SLOT0;
            end

            // ---------------------------------------------- voice loop
            P_SLOT0: begin
                g1[3][1] <= 32'h0; g1[3][3] <= 32'h0;
                acc1 <= 32'sh0; acc3 <= 32'sh0;
                rcsum[0] <= 32'sh0; rcsum[1] <= 32'sh0;
                slot <= 6'd0;
                ps <= (reg_slots == 6'd0) ? P_FRAME_END : P_SLOT_RD;
            end

            P_SLOT_RD: begin
                for (int j = 0; j < 8; j++) r1[j] <= ram1[slot[4:0]][j];
                for (int j = 0; j < 16; j++) r2[j] <= ram2[slot[4:0]][j];
                ps <= P_SLOT_AG;
            end

            P_SLOT_AG: begin
                `include "sc55_pcm_agen.svh"
                v_rom_i <= 3'd0;
                ps <= P_SLOT_ROM;
            end

            P_SLOT_ROM: begin
                // five ROM reads: nibble byte, samp0..3
                if (!rom_req) begin
                    t32 = (v_rom_i == 3'd0) ? {8'h0, v_hiaddr, v_wave_address} : ({8'h0, v_hiaddr, 20'h0} | v_addr[2'(v_rom_i - 3'd1)]);
                    rm = rom_map(t32, config_reg_3d);
                    if (rm[22]) begin rom_req <= 1'b1; rom_addr <= rm[21:0]; end
                    else begin
                        v_rom[v_rom_i] <= 8'h00;
                        if (v_rom_i == 3'd4) ps <= P_SLOT_DPCM;
                        v_rom_i <= v_rom_i + 3'd1;
                    end
                end else if (rom_ack) begin
                    rom_req <= 1'b0;
                    v_rom[v_rom_i] <= rom_data;
                    if (v_rom_i == 3'd4) ps <= P_SLOT_DPCM;
                    v_rom_i <= v_rom_i + 3'd1;
                end
            end

            P_SLOT_DPCM: begin
                `include "sc55_pcm_dpcm.svh"
                ps <= P_SLOT_FILT;
            end

            P_SLOT_FILT: begin
                // mk1 filter
                logic signed [31:0] reg1, reg3, mult1, mult2, mult3, vv1, vv2, subvar, vv3, mult4, mult5, v4, v5;
                logic [15:0] filter;
                logic [6:0] reg2_6;
                reg1 = r1[1]; reg3 = r1[3];
                reg2_6 = r2[6][14:8];
                filter = r2[11];
                mult1 = multi(reg1, filter[15:8]);
                mult2 = multi(reg1, {1'b0, filter[7:1]});
                mult3 = multi(reg1, {1'b0, reg2_6});
                vv2 = addclip20(reg3, mult1 >>> 6, (mult1 >>> 5) & 1);
                vv1 = addclip20(vv2, mult2 >>> 13, (mult2 >>> 12) & 1);
                subvar = addclip20(vv1, mult3 >>> 6, (mult3 >>> 5) & 1);
                r1[3] <= vv1;
                vv3 = addclip20(v_test, subvar ^ 32'hfffff, 1);
                mult4 = multi(vv3, filter[15:8]);
                mult5 = multi(vv3, {1'b0, filter[7:1]});
                v4 = addclip20(reg1, mult4 >>> 6, (mult4 >>> 5) & 1);
                v5 = addclip20(v4, mult5 >>> 13, (mult5 >>> 12) & 1);
                r1[1] <= v5;
                v_v3 <= vv3;
                r1[5] <= v_reference;
                // irq
                if (v_active && r2[6][0] && !r2[8][14] && !irq_assert && v_irq_flag) begin
                    if (nfs) r2[8] <= r2[8] | 16'h4000;
                    irq_assert <= 1'b1;
                    irq_channel <= {2'b0, slot};
                end
                ps <= P_SLOT_TV0;
            end

            P_SLOT_TV0: begin
                ct = calc_tv(0, r2[3], r2[9], v_active, tv_counter, nfs);
                r2[9] <= ct[31:16];
                v_volmul1 <= ct[15:0];
                ps <= P_SLOT_TV1;
            end

            P_SLOT_TV1: begin
                ct = calc_tv(1, r2[4], r2[10], v_active, tv_counter, nfs);
                r2[10] <= ct[31:16];
                v_volmul2 <= ct[15:0];
                ct = calc_tv(2, r2[5], r2[11], v_active, tv_counter, nfs);
                r2[11] <= ct[31:16];
                ps <= P_SLOT_VOL;
            end

            P_SLOT_VOL: begin
                logic signed [31:0] sample, mv1, mv2, sample2, mv3, mv4;
                sample = !r2[6][1] ? $signed(r1[3]) : v_v3;
                mv1 = multi(sample, v_volmul1[15:8]);
                mv2 = multi(sample, {1'b0, v_volmul1[7:1]});
                sample2 = addclip20(mv1 >>> 6, mv2 >>> 13, ((mv2 >>> 12) | (mv1 >>> 5)) & 1);
                mv3 = multi(sample2, v_volmul2[15:8]);
                mv4 = multi(sample2, {1'b0, v_volmul2[7:1]});
                v_sample3 <= addclip20(mv3 >>> 6, mv4 >>> 13, ((mv4 >>> 12) | (mv3 >>> 5)) & 1);
                ps <= P_SLOT_MIX;
            end

            P_SLOT_MIX: begin
                logic [15:0] pan, rc;
                logic signed [31:0] sampl, sampr, rc0, rc1, suml, sumr, a1, a3, rs0, rs1;
                logic [5:0] slot2;
                pan = v_active ? r2[1] : 16'h0;
                rc = v_active ? r2[2] : 16'h0;
                sampl = multi(v_sample3, pan[15:8]);
                sampr = multi(v_sample3, pan[7:0]);
                rc0 = multi(v_sample3, rc[15:8]) >>> 5;
                rc1 = multi(v_sample3, rc[7:0]) >>> 5;
                slot2 = (slot == reg_slots - 6'd1) ? 6'd31 : slot + 6'd1;
                a1 = acc1; a3 = acc3;
                case (slot2)
                    6'd17: a1 = addclip20(a1, rcadd[0] >>> 1, rcadd[0] & 1);
                    6'd18: a3 = addclip20(a3, rcadd[1] >>> 1, rcadd[1] & 1);
                    6'd21: a1 = addclip20(a1, rcadd[2] >>> 1, rcadd[2] & 1);
                    6'd22: a3 = addclip20(a3, rcadd[3] >>> 1, rcadd[3] & 1);
                    6'd23: a1 = addclip20(a1, rcadd[4] >>> 1, rcadd[4] & 1);
                    6'd31: a3 = addclip20(a3, rcadd[5] >>> 1, rcadd[5] & 1);
                    default: ;
                endcase
                suml = addclip20(a1, sampl >>> 6, (sampl >>> 5) & 1);
                sumr = addclip20(a3, sampr >>> 6, (sampr >>> 5) & 1);
                rs0 = rcsum[0]; rs1 = rcsum[1];
                case (slot2)
                    6'd17: rs1 = addclip20(rs1, rcadd2[0] >>> 1, rcadd2[0] & 1);
                    6'd18: rs1 = addclip20(rs1, rcadd2[1] >>> 1, rcadd2[1] & 1);
                    6'd21: rs0 = addclip20(rs0, rcadd2[2] >>> 1, rcadd2[2] & 1);
                    6'd22: rs1 = addclip20(rs1, rcadd2[3] >>> 1, rcadd2[3] & 1);
                    6'd23: rs0 = addclip20(rs0, rcadd2[4] >>> 1, rcadd2[4] & 1);
                    6'd31: rs1 = addclip20(rs1, rcadd2[5] >>> 1, rcadd2[5] & 1);
                    default: ;
                endcase
                rs0 = addclip20(rs0, rc0 >>> 1, rc0 & 1);
                rs1 = addclip20(rs1, rc1 >>> 1, rc1 & 1);
                rcsum[0] <= rs0; rcsum[1] <= rs1;
                if (slot != reg_slots - 6'd1) begin
                    acc1 <= suml; acc3 <= sumr;
                end else begin
                    // ram1[31][1], [31][3] keep the last values written before the last slot
                    acc1 <= a1; acc3 <= a3;
                    accum_l <= suml; accum_r <= sumr;
                end
                // key / nibble update
                if (v_key && nfs) begin
                    r2[7] <= (r2[7] & ~16'hf020) | ({12'h0, (v_usenew || v_kon) ? v_rom_nibble() : v_old_nibble} << 12) | 16'h0020;
                end
                if (!v_active) begin
                    if (nfs) begin r1[1] <= 32'h0; r1[3] <= 32'h0; r1[5] <= 32'h0; end
                    r2[8] <= 16'h0; r2[9] <= 16'h0; r2[10] <= 16'h0;
                end
                ps <= P_SLOT_WB;
            end

            P_SLOT_WB: begin
                for (int j = 0; j < 8; j++) ram1[slot[4:0]][j] <= r1[j];
                for (int j = 0; j < 16; j++) ram2[slot[4:0]][j] <= r2[j];
                if (slot + 6'd1 >= reg_slots) ps <= P_FRAME_END;
                else begin slot <= slot + 6'd1; ps <= P_SLOT_RD; end
            end

            P_FRAME_END: begin
                for (int i = 0; i < 4; i++) begin
                    for (int j = 0; j < 8; j++) ram1[28 + i][j] <= g1[i][j];
                    for (int j = 0; j < 16; j++) ram2[28 + i][j] <= g2[i][j];
                end
                ram1[31][1] <= acc1;
                ram1[31][3] <= acc3;
                if (nfs) ram2[31][7] <= g2[3][7] | 16'h0020;
                nfs <= 1'b1;
                pre_done <= 1'b0;
                pcm_cycles <= pcm_cycles + ({42'h0, reg_slots} + 48'd1) * 48'd25;
                ps <= P_FRAME;
            end

            default: ps <= P_IDLE;
            endcase
        end
    end

    // selected new nibble (newnibble after the high/low select)
    function automatic logic [3:0] v_rom_nibble();
        return v_newsel ? v_rom[0][7:4] : v_rom[0][3:0];
    endfunction

endmodule
