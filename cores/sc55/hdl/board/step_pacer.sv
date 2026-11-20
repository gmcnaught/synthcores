// Real-time pacing: the reference model's time base is 12 cycles per step
// (20 MHz on the mk1: 600 ns per step). `allow` lets the core start a step
// while the steps done so far are not ahead of real time. Credit for real
// time the core could not use (a stall) is kept up to CAP clocks, so short
// stalls are made up by running faster; longer ones are lost and counted in
// `lag_us`.
module step_pacer #(
    parameter longint CLK_HZ = 50_000_000,
    parameter int STEP_NS = 600,
    parameter int STEP_NS_FAST = 500,
    parameter int CAP_US = 200
) (
    input  logic        clk,
    input  logic        rst,
    input  logic        enable,       // 0: run as fast as possible
    input  logic        fast,         // 1: STEP_NS_FAST a step (the mk2: 500 ns)
    input  logic        step_done,
    output logic        allow,
    output logic [31:0] lag_us
);
    // credit in 1/1000 clock units: + 1000 per clock, - STEP_NS * CLK_HZ / 1e6 per step
    localparam longint STEP_MILLICLK = longint'(STEP_NS) * CLK_HZ / 1_000_000;
    localparam longint STEP_MILLICLK_FAST = longint'(STEP_NS_FAST) * CLK_HZ / 1_000_000;
    localparam longint CAP = longint'(CAP_US) * CLK_HZ / 1000;
    localparam longint US_MILLICLK = CLK_HZ / 1000;
    logic signed [47:0] credit;
    logic [31:0] lost;
    always_ff @(posedge clk) begin
        if (rst) begin
            credit <= '0;
            lost <= '0;
            lag_us <= '0;
        end else begin : b_step_pacer_1
            logic signed [47:0] c;
            c = credit + 48'sd1000;
            if (step_done) c = c - (fast ? 48'(STEP_MILLICLK_FAST) : 48'(STEP_MILLICLK));
            if (c > 48'(CAP)) begin
                lost <= lost + 32'(c - 48'(CAP));
                c = 48'(CAP);
            end
            if (lost >= 32'(US_MILLICLK)) begin
                lost <= lost - 32'(US_MILLICLK);
                lag_us <= lag_us + 32'd1;
            end
            credit <= c;
        end
    end
    assign allow = !enable || credit >= 0;
endmodule
