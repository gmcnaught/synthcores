// SPDX-License-Identifier: MIT
// Yamaha SWP00 as MAME's swp00 device runs it (src/devices/sound/swp00.cpp, Olivier Galibert,
// BSD-3-Clause): voice record, register decode and the small pure functions. Field names and
// widths follow MAME's member variables; the arithmetic follows its C++ (C integer promotion,
// two's complement wrap).
package swp00_pkg;

    // envelope modes (envelope_block)
    localparam logic [1:0] ENV_ATTACK = 2'd0, ENV_DECAY = 2'd2, ENV_DONE = 2'd3;
    // filter sweep (filter_block)
    localparam logic [1:0] SWEEP_NONE = 2'd0, SWEEP_UP = 2'd1, SWEEP_DONE = 2'd2;

    // One channel: streaming, envelope, filter, LFO and mixer blocks
    typedef struct packed {
        // streaming_block
        logic [15:0] phase, start, loop_, pitch;
        logic [23:0] address;
        logic [7:0]  format;
        logic [31:0] pos, pos_dec, dpcm_pos, dpcm_delta;   // s32, s32, u32, s32
        logic [15:0] dpcm_s0, dpcm_s1, last_;              // s16 (m_last)
        logic        first_, done;                     // m_first, m_done
        // envelope_block
        logic [7:0]  att_speed, att_level, dec_speed, dec_level;
        logic [15:0] env_level;                            // s32 in MAME, always 0..fff
        logic [1:0]  env_mode;
        // filter_block
        logic [15:0] f_q;                                  // s32 in MAME, 9..256
        logic [31:0] f_b, f_l;                             // s32
        logic [15:0] f_k, f_kt, f_info;
        logic [7:0]  f_speed;
        logic [1:0]  f_sweep;
        // lfo_block
        logic [21:0] lfo_counter;
        logic [7:0]  lfo_speed, lfo_lamod, lfo_fmod;
        // mixer_block
        logic [11:0] cglo, cpanl, cpanr, tglo, tpanl, tpanr;
        logic [7:0]  glo, pan, dry, rev, cho, var_;
    } voice_t;

    // clear() of every block
    function automatic voice_t voice_clear();
        voice_t v;
        v = '0;
        v.phase = 16'h8000;
        v.env_level = 16'hfff;
        v.env_mode = ENV_DONE;
        v.f_info = 16'h27ff;
        v.f_kt = 16'h7ff;
        v.f_k = 16'hfff;
        v.f_q = 16'h80;
        v.lfo_speed = 8'h80;
        v.cglo = 12'hfff; v.cpanl = 12'hfff; v.cpanr = 12'hfff;
        v.tglo = 12'hfff; v.tpanl = 12'hfff; v.tpanr = 12'hfff;
        v.glo = 8'hff; v.pan = 8'hff; v.dry = 8'hff; v.rev = 8'hff; v.cho = 8'hff; v.var_ = 8'hff;
        return v;
    endfunction

    // keyon() of every block
    function automatic voice_t voice_keyon(input voice_t vi);
        voice_t v;
        v = vi;
        v.pos = 32'h0 - {16'h0, v.start};
        v.pos_dec = {17'h0, v.phase[13:0], 1'b0};          // (phase << 1) & 0x7ffe
        v.dpcm_s0 = 16'h0; v.dpcm_s1 = 16'h0;
        v.dpcm_pos = v.pos + 32'd1;
        v.dpcm_delta = 32'h0;
        v.first_ = 1'b1; v.done = 1'b0;
        if (v.dec_speed[7]) begin
            v.env_level = 16'h0; v.env_mode = ENV_DECAY;
        end else begin
            v.env_level = v.att_speed[7] ? {4'h0, v.att_level, 4'h0} : 16'h0;
            v.env_mode = ENV_ATTACK;
        end
        v.f_b = 32'h0; v.f_l = 32'h0;
        if (v.f_speed[7]) begin
            v.f_sweep = SWEEP_UP; v.f_k = 16'h800;
        end else
            v.f_k = v.f_kt;
        v.cglo = v.tglo; v.cpanl = v.tpanl; v.cpanr = v.tpanr;
        return v;
    endfunction

    // channel register index of an address sss sscc cccs (swp00_device::map, rchan)
    function automatic logic [5:0] chan_index(input logic [10:0] a);
        return {a[10:6], a[0]};
    endfunction

    // byte write to a channel register (the *_w handlers of the five blocks)
    function automatic voice_t voice_write(input voice_t vi, input logic [5:0] idx, input logic [7:0] d);
        voice_t v;
        logic [15:0] info;
        logic [5:0]  q;
        v = vi;
        case (idx)
            6'h08: v.phase[15:8] = d;
            6'h09: v.phase[7:0] = d;
            6'h0a: v.start[15:8] = d;
            6'h0b: v.start[7:0] = d;
            6'h20, 6'h21: begin
                info = v.f_info;
                if (idx == 6'h20) begin
                    info[15:8] = d;
                    q = {1'b0, info[15:11]} + 6'd4;
                    v.f_q = {11'h0, 5'h10 - {2'b0, q[2:0]}} << (3'd4 - q[5:3]);
                end else
                    info[7:0] = d;
                v.f_info = info;
                v.f_kt = {5'h0, info[10:0]} | (info[10:0] != 11'h0 ? 16'h800 : 16'h0);
            end
            6'h22: v.f_speed = d;
            6'h23: v.lfo_lamod = d;
            6'h24: v.lfo_speed = d;
            6'h25: v.lfo_fmod = d;
            6'h26: v.att_speed = d;
            6'h27: v.att_level = d;
            6'h28: begin
                v.dec_speed = d;
                if (d[7]) v.env_mode = ENV_DECAY;
            end
            6'h29: v.dec_level = d;
            6'h2a: v.rev = d;
            6'h2b: v.dry = d;
            6'h2c: v.cho = d;
            6'h2d: v.var_ = d;
            6'h2e: begin v.glo = d; v.tglo = {d, 4'h0}; end
            6'h2f: begin
                v.pan = d;
                v.tpanl = (d[7:4] == 4'hf) ? 12'hfff : {2'b0, d[7:4], 6'h0};
                v.tpanr = (d[3:0] == 4'hf) ? 12'hfff : {2'b0, d[3:0], 6'h0};
            end
            6'h30: v.format = d;
            6'h31: v.address[23:16] = d;
            6'h32: v.address[15:8] = d;
            6'h33: v.address[7:0] = d;
            6'h34: v.pitch[15:8] = d;
            6'h35: v.pitch[7:0] = d;
            6'h36: v.loop_[15:8] = d;
            6'h37: v.loop_[7:0] = d;
            default: ;
        endcase
        return v;
    endfunction

    // byte read of a channel register (the *_r handlers)
    function automatic logic [7:0] voice_read(input voice_t v, input logic [5:0] idx);
        case (idx)
            6'h08: return v.phase[15:8];
            6'h09: return v.phase[7:0];
            6'h0a: return v.start[15:8];
            6'h0b: return v.start[7:0];
            6'h20: return v.f_info[15:8];
            6'h21: return v.f_info[7:0];
            6'h22: return v.f_speed;
            6'h23: return v.lfo_lamod;
            6'h24: return v.lfo_speed;
            6'h25: return v.lfo_fmod;
            6'h26: return v.att_speed;
            6'h27: return v.att_level;
            6'h28: return v.dec_speed;
            6'h29: return v.dec_level;
            6'h2a: return v.rev;
            6'h2b: return v.dry;
            6'h2c: return v.cho;
            6'h2d: return v.var_;
            6'h2e: return v.glo;
            6'h2f: return v.pan;
            6'h30: return v.format;
            6'h31: return v.address[23:16];
            6'h32: return v.address[15:8];
            6'h33: return v.address[7:0];
            6'h34: return v.pitch[15:8];
            6'h35: return v.pitch[7:0];
            6'h36: return v.loop_[15:8];
            6'h37: return v.loop_[7:0];
            default: return 8'h0;
        endcase
    endfunction

    function automatic logic is_chan_reg(input logic [5:0] idx);
        return idx == 6'h08 || idx == 6'h09 || idx == 6'h0a || idx == 6'h0b ||
               (idx >= 6'h20 && idx <= 6'h37);
    endfunction

    // state_r(): the internal state readout selected by state_adr
    function automatic logic [7:0] voice_state(input voice_t v, input logic [2:0] sel, input logic [31:0] sc);
        case (sel)
            3'd0: return ((v.f_sweep == SWEEP_DONE || (v.f_sweep == SWEEP_NONE && v.f_k == v.f_kt)) ? 8'hc0 : 8'h00)
                         | (v.f_k[13:6] ^ 8'h3f);
            3'd1: return 8'hc0 | sc[20:13];
            3'd2: return {v.env_mode, 6'h0} | v.env_level[13:6];
            3'd3: return ((v.cglo == v.tglo) ? 8'hc0 : 8'h00) | {2'b0, v.cglo[11:6]};
            3'd4: return ((v.cpanl == v.tpanl) ? 8'hc0 : 8'h00) | {2'b0, v.cpanl[11:6]};
            3'd5: return ((v.cpanr == v.tpanr) ? 8'hc0 : 8'h00) | {2'b0, v.cpanr[11:6]};
            default: return 8'h0;
        endcase
    endfunction

    // interpolation_step(): the common speed subblock, 0..1f to add this sample
    function automatic logic [4:0] interpolation_step(input logic [8:0] speed, input logic [31:0] sc);
        logic [3:0]  k0;
        logic [2:0]  k1;
        logic [7:0]  mx8;
        logic [15:0] mx16;
        logic [3:0]  ph;
        logic [31:0] lowmask;
        if (speed >= 9'h78) return 5'h1f;
        k1 = speed[2:0];
        if (speed >= 9'h58) begin
            k0 = speed[6:3] - 4'd10;                       // 1..4
            case (k1)
                3'd0: mx8 = 8'h00; 3'd1: mx8 = 8'h80; 3'd2: mx8 = 8'h88; 3'd3: mx8 = 8'ha8;
                3'd4: mx8 = 8'haa; 3'd5: mx8 = 8'hea; 3'd6: mx8 = 8'hee; default: mx8 = 8'hfe;
            endcase
            // ((mx << (sc & 7)) & 0x80) ? (2 << k0) - 1 : (1 << k0) - 1
            if (mx8[3'd7 - sc[2:0]]) return 5'((32'd2 << k0) - 32'd1);
            return 5'((32'd1 << k0) - 32'd1);
        end
        k0 = 4'd10 - speed[6:3];                           // 0..10
        lowmask = (32'd1 << k0) - 32'd1;
        if ((sc & lowmask) != 32'h0) return 5'h0;
        case (k1)
            3'd0: mx16 = 16'haaaa; 3'd1: mx16 = 16'heaaa; 3'd2: mx16 = 16'heaea; 3'd3: mx16 = 16'heeea;
            3'd4: mx16 = 16'heeee; 3'd5: mx16 = 16'hfeee; 3'd6: mx16 = 16'hfefe; default: mx16 = 16'hfffe;
        endcase
        ph = 4'(sc >> k0);
        return {4'h0, mx16[4'd15 - ph]};
    endfunction

    // streaming_block::dpcm_expand
    function automatic logic [31:0] dpcm_expand(input logic [7:0] i);
        logic [1:0]  e;
        logic [31:0] base, offset;
        e = i[6:5];
        case (e)
            2'd0: offset = 32'h0; 2'd1: offset = 32'h20; 2'd2: offset = 32'h60; default: offset = 32'he0;
        endcase
        base = ({27'h0, i[4:0]} << e) + offset;
        if (i == 8'h80) return 32'h88;
        return i[7] ? 32'h0 - base : base;
    endfunction

    // C integer division by 2**n, truncating towards zero
    function automatic logic [31:0] cdiv_pow2(input logic [31:0] x, input int n);
        logic [31:0] bias;
        bias = x[31] ? (32'd1 << n) - 32'd1 : 32'h0;
        return 32'($signed(x + bias) >>> n);
    endfunction

    // streaming_block::dpcm_step on (s0, s1, delta): returns {s0, s1, delta}
    // streaming_block::dpcm_step, in two parts (one a clock): the new sample with its clamp,
    // returning {sample, delta}; then the delta decay of the mode
    function automatic logic [47:0] dpcm_sample(input logic [7:0] format, input logic [15:0] s1,
                                                 input logic [31:0] dpcm_delta, input logic [7:0] in);
        logic [2:0]  scale;
        logic [31:0] limit, delta, sample;
        scale = format[4:2];
        limit = 32'h7fff - ((32'd1 << scale) - 32'd1);    // max_value[]: 7fff 7ffe 7ffc ... 7f80
        delta = dpcm_delta + dpcm_expand(in);
        sample = {{16{s1[15]}}, s1} + (delta << scale);
        if ($signed(sample) < $signed(-32'sh8000)) begin
            sample = 32'hffff8000; delta = 32'h0;
        end else if ($signed(sample) > $signed(limit)) begin
            sample = limit; delta = 32'h0;
        end
        return {sample[15:0], delta};
    endfunction
    function automatic logic [31:0] dpcm_decay(input logic [7:0] format, input logic [31:0] delta);
        case (format[1:0])
            2'd0: return cdiv_pow2(delta * 32'd7, 3);
            2'd1: return cdiv_pow2(delta * 32'd3, 2);
            2'd2: return cdiv_pow2(delta, 1);
            default: return 32'h0;
        endcase
    endfunction

    // mixer_block::volume_apply multiplier: (0x1000000 - (m << 15)) >> e, 0 when level >= 0xfff
    function automatic logic [24:0] volume_mul(input logic [31:0] level);
        if ($signed(level) >= 32'shfff) return 25'h0;
        return 25'((32'h1000000 - {9'h0, level[7:0], 15'h0}) >> level[11:8]);
    endfunction

endpackage
