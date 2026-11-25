`timescale 1ps/1ps
module tb;
  parameter int RATE96 = 0;
  parameter int CROSSED = 0;  // 1 = USB 3.0 A-to-A cable with SSTX/SSRX pairs crossed
  // TX MiSTer (synth) and RX MiSTer (game): independent ~24.576 MHz clocks, 200 ppm apart
  reg ctx=0, crx=0;
  always #20345 ctx = ~ctx;
  always #20349 crx = ~crx;
  reg rst=1;
  // ce generator copied from audio_out.sv
  localparam CLK_RATE=24576000, CE_RATE=48000*16*8;
  wire [31:0] real_ce = RATE96 ? CE_RATE*2 : CE_RATE;
  reg mclk_ce, i2s_ce, div=0; reg [31:0] cnt=0;
  always @(posedge ctx) begin
    mclk_ce = 0; cnt = cnt + real_ce;
    if (cnt >= CLK_RATE) begin cnt = cnt - CLK_RATE; mclk_ce = 1; end
  end
  always @(posedge ctx) begin i2s_ce <= 0; if (mclk_ce) begin div <= ~div; i2s_ce <= div; end end
  reg [15:0] L=16'h0000, R=16'h0000;
  wire bclk, lrclk, sdata;
  i2s tx(.reset(rst), .clk(ctx), .ce(i2s_ce), .sclk(bclk), .lrclk(lrclk), .sdata(sdata), .left_chan(L), .right_chan(R));
  // straight: synth 2=WS 4=BCLK 5=DATA land on the same game pins; game pin 6 floats high
  // crossed:  synth 4->game 6, 2->game 5, 5->game 2
  wire [6:0] uin = CROSSED ? {bclk, lrclk, 1'b1, 1'b1, sdata, 1'b1, 1'b1}
                           : {1'b1, sdata, bclk, 1'b1, lrclk, 1'b1, 1'b1};
  wire [6:0] uout; wire mrx; wire [15:0] gl, gr; wire av, le, lp, lu;
  wire [7:0] m0,m1,m2; wire nm;
  mt32pi rx(.CLK_AUDIO(crx), .CLK_VIDEO(crx), .CE_PIXEL(1'b0), .VGA_VS(1'b0), .VGA_DE(1'b0),
    .USER_IN(uin), .USER_OUT(uout), .reset(1'b0), .midi_tx(1'b1), .midi_rx(mrx),
    .mt32_i2s_r(gr), .mt32_i2s_l(gl), .mt32_available(av), .mt32_mode_req(1'b0), .mt32_rom_req(2'b0),
    .mt32_sf_req(8'd0), .mt32_mode(m0), .mt32_rom(m1), .mt32_sf(m2), .mt32_newmode(nm),
    .mt32_lcd_en(le), .mt32_lcd_pix(lp), .mt32_lcd_update(lu));
  // feed new sample pair at each left-word load; pattern L=k*0x1357, R=~L
  reg [15:0] k=1; reg old_lr=1;
  always @(posedge ctx) begin
    old_lr <= lrclk;
    if (old_lr & ~lrclk) begin k <= k+1; L <= (k*16'h1357) ^ 16'h8001; R <= ~((k*16'h1357) ^ 16'h8001); end
  end
  // check: every value the receiver presents must be a value the transmitter sent
  integer bad=0, good=0; reg [15:0] pl=0, pr=0;
  reg [15:0] j;
  function automatic bit sent_l(input [15:0] v); for (int i=1;i<20000;i++) if (v == (((i*16'h1357)&16'hffff) ^ 16'h8001)) return 1; return 0; endfunction
  always @(posedge crx) begin
    pl <= gl; pr <= gr;
    if (gl != pl && $time > 400_000_000) begin if (sent_l(gl)) good++; else begin bad++; if (bad<5) $display("bad L %h", gl); end end
    if (gr != pr && $time > 400_000_000) begin if (sent_l(~gr)) good++; else begin bad++; if (bad<5) $display("bad R %h", gr); end end
  end
  initial begin
    #1_000_000 rst=0;
    #40_000_000_000;  // 40 ms
    $display("RATE96=%0d CROSSED=%0d detected_crossed=%0d good=%0d bad=%0d", RATE96, CROSSED, rx.crossed, good, bad);
    $finish;
  end
endmodule
