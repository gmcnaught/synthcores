# 96 MHz target
create_clock -name clk -period 10.417 [get_ports clk]
derive_clock_uncertainty
