derive_pll_clocks
derive_clock_uncertainty

# core specific constraints

# clk_sys (outclk_0, 88.2 MHz) and clk_cpu (outclk_1, 44.1 MHz) come from one VCO with their rising
# edges aligned: hdl/p2k_x21.sv crosses between them as synchronous paths, which stay analysed.
# Video (outclk_2, 40.09 MHz) is asynchronous to both: the LCD renderer reads the LCD controller,
# the LEDs and the status bits for display only, and the font RAM is written in clk_sys.
set_clock_groups -asynchronous \
   -group [get_clocks {emu|pll|pll_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk \
                       emu|pll|pll_inst|altera_pll_i|general[1].gpll~PLL_OUTPUT_COUNTER|divclk}] \
   -group [get_clocks {emu|pll|pll_inst|altera_pll_i|general[2].gpll~PLL_OUTPUT_COUNTER|divclk}]
