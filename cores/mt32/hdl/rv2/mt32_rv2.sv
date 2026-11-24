// SPDX-License-Identifier: GPL-2.0-or-later
// MT-32 reverb chip, bit-exact to Nuked-MT32's rv2.cpp (nukeykt, after InfoSecDJ's decap),
// pinned commit 5b4e239.
//
// go runs rv2_t::clock(8): two program steps (one step per 4 LA32 cycles, 64 a sample);
// done pulses when both are finished. A step takes three clocks here: program word read,
// delay RAM write/read, result. The program ROM (32 KB, loaded through load_*) is four
// 8 K x 8 byte lanes; the delay RAM is 16 K x 16.
//
// CPU writes, as the MT-32 board decodes them (mt32.cpp cpu_write):
//   reg 0 (0x0800): rv2->write(0, data << 1)  -> saw0 = (data << 1) & 30
//   reg 1 (0x0400): rv2->write(1, data << 1)  -> saw1 = (data << 1) & 31
//   reg 2 (0x0200): rv2->set_hiaddr((data >> 1) & 3)
module mt32_rv2 (
    input  logic        clk,
    input  logic        rst,

    input  logic        go,
    output logic        done,

    input  logic        in_we,            // set_input
    input  logic signed [15:0] in_value,

    input  logic        reg_we,
    input  logic [1:0]  reg_sel,
    input  logic [7:0]  reg_data,

    input  logic        load_we,
    input  logic [14:0] load_addr,
    input  logic [7:0]  load_data,

    output logic signed [15:0] sample0,
    output logic signed [15:0] sample1
);
    // ---------------------------------------------------------------- memories
    logic [7:0] rom0 [8192], rom1 [8192], rom2 [8192], rom3 [8192];
    logic [12:0] rom_a;
    logic [7:0] b0, b1, b2, b3;
    always_ff @(posedge clk) begin
        if (load_we) begin
            case (load_addr[1:0])
                2'd0: rom0[load_addr[14:2]] <= load_data;
                2'd1: rom1[load_addr[14:2]] <= load_data;
                2'd2: rom2[load_addr[14:2]] <= load_data;
                default: rom3[load_addr[14:2]] <= load_data;
            endcase
        end
        b0 <= rom0[rom_a];
        b1 <= rom1[rom_a];
        b2 <= rom2[rom_a];
        b3 <= rom3[rom_a];
    end

    logic [15:0] ram [16384];
    logic [13:0] ram_a;
    logic        ram_we;
    logic [15:0] ram_d, ram_q;
    always_ff @(posedge clk) begin
        if (ram_we) ram[ram_a] <= ram_d;
        ram_q <= ram[ram_a];
    end

    // ---------------------------------------------------------------- state (rv2_t)
    logic [7:0]  state;
    logic [15:0] samples;
    logic [4:0]  saw0, saw1;
    logic [1:0]  hiaddr;
    logic [12:0] hiaddrsaw;
    logic signed [15:0] input_v, shifter, shifter_add, accum, ramin;
    logic [7:0]  ctrl0, ctrl1;
    logic [1:0]  ctrlext, shift_sel;
    logic [3:0]  saw1v;
    logic        wrote;
    logic [15:0] wd;

    // accum_op(ctrl, ext) on (accum, shifter, shifter_add); ramin and shift_sel are inputs
    typedef struct packed {
        logic signed [15:0] accum, shifter, shifter_add;
    } ac_t;

    function automatic ac_t accum_op(input ac_t a, input logic [7:0] ctrl, input logic ext,
                                     input logic [1:0] sel, input logic signed [15:0] rin);
        ac_t r;
        logic [3:0] m;
        logic signed [31:0] sum;
        r = a;
        if (!ctrl[4])
            r.accum = 16'sd0;
        else begin
            m = {ctrl[7:5], ext};
            if (m[sel]) begin
                sum = 32'(a.accum) + 32'(a.shifter) + 32'(a.shifter_add);
                if (sum < -32'sd32768) sum = -32'sd32768;
                else if (sum > 32'sd32767) sum = 32'sd32767;
                if (ctrl[3]) sum = ~sum;
                r.accum = sum[15:0];
            end
        end
        r.shifter_add = 16'(a.shifter[0]);
        r.shifter = a.shifter >>> 1;
        if (!ctrl[2]) r.shifter = rin;
        if (r.shifter >= 0) r.shifter_add = 16'sd0;
        return r;
    endfunction

    ac_t ac_in, ac_mid, ac_out;
    always_comb begin
        ac_in = '{accum: accum, shifter: shifter, shifter_add: shifter_add};
        ac_mid = accum_op(ac_in, ctrl0, ctrlext[1], shift_sel, ramin);
        ac_out = accum_op(ac_mid, ctrl1, ctrlext[0], shift_sel, ramin);
    end

    typedef enum logic [1:0] { S_IDLE, S_OPS, S_RAM, S_FIN } st_t;
    st_t st;
    logic second;                          // second step of the pair

    // ROM word address: rom = rvrom + (hiaddr << 13); bytes[k] = rom[hiaddrsaw | state + k]
    assign rom_a = {hiaddr, 11'd0} + {2'b00, hiaddrsaw[12:2] | {5'd0, state[7:2]}};

    always_comb begin
        ram_a = 14'((samples + {b2, b0}));
        ram_we = (st == S_RAM) && !b1[1];
        ram_d = b1[0] ? input_v : accum;
    end

    always_ff @(posedge clk) begin
        done <= 1'b0;
        if (rst) begin
            st <= S_IDLE;
            state <= 8'd0; samples <= 16'd0; saw0 <= 5'd0; saw1 <= 5'd0; hiaddr <= 2'd0; hiaddrsaw <= 13'd0;
            input_v <= 16'sd0; sample0 <= 16'sd0; sample1 <= 16'sd0;
            shifter <= 16'sd0; shifter_add <= 16'sd0; accum <= 16'sd0; ramin <= 16'sd0;
            ctrl0 <= 8'd0; ctrl1 <= 8'd0; ctrlext <= 2'd0; shift_sel <= 2'd0; saw1v <= 4'd0;
        end else begin
            if (in_we) input_v <= in_value >>> 1;
            if (reg_we) begin
                case (reg_sel)
                    2'd0: saw0 <= {reg_data[3:0], 1'b0} & 5'd30;
                    2'd1: saw1 <= {reg_data[3:0], 1'b0};
                    default: hiaddr <= reg_data[2:1];
                endcase
            end
            case (st)
                S_IDLE: if (go) begin
                    // saw1v at the start of rv2_t::clock
                    saw1v <= saw0[4] ? 4'd15 : saw1[4:1];
                    second <= 1'b0;
                    st <= S_OPS;
                end
                S_OPS: begin
                    // output, then the two accumulator operations (rom_a is presented this clock)
                    if (state == 8'd192) sample0 <= accum;
                    else if (state == 8'd228) sample1 <= accum;
                    accum <= ac_out.accum;
                    shifter <= ac_out.shifter;
                    shifter_add <= ac_out.shifter_add;
                    st <= S_RAM;
                end
                S_RAM: begin
                    // program bytes b0-b3 arrived; ram write (ram_we) and read at ram_a
                    wrote <= !b1[1];
                    wd <= ram_d;
                    ctrl0 <= b1;
                    ctrl1 <= b3;
                    ctrlext <= b2[7:6];
                    st <= S_FIN;
                end
                S_FIN: begin
                    ramin <= wrote ? wd : ram_q;
                    state <= state + 8'd4;
                    if (state == 8'd252) begin
                        samples <= samples + 16'd1;
                        hiaddrsaw <= ({8'd0, saw0} << 9) & 13'h1c00 | ({9'd0, saw1v} << 6) & 13'h300;
                        shift_sel <= saw1v[1:0];
                    end
                    if (second) begin
                        st <= S_IDLE;
                        done <= 1'b1;
                    end else begin
                        second <= 1'b1;
                        st <= S_OPS;
                    end
                end
                default: st <= S_IDLE;
            endcase
        end
    end
endmodule
