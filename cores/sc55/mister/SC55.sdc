derive_pll_clocks
derive_clock_uncertainty

# core specific constraints

# Core (outclk_0, 50 MHz) and video (outclk_1, 40 MHz) are asynchronous; the
# LCD renderer reads the LCD controller's registers and the status bits for
# display only.
set_clock_groups -asynchronous \
   -group [get_clocks {emu|pll|pll_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}] \
   -group [get_clocks {emu|pll|pll_inst|altera_pll_i|general[1].gpll~PLL_OUTPUT_COUNTER|divclk}]
