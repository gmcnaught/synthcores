# 100 MHz: 2267 clocks per 44.1 kHz sample
create_clock -name clk -period 10.000 [get_ports clk]
derive_clock_uncertainty
