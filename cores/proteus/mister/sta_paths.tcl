# Worst setup paths per clock -> output_files/worst_paths.txt (run by build.sh after the compile)
project_open Proteus
create_timing_netlist
read_sdc
update_timing_netlist
foreach_in_collection clk [get_clocks] {
    set name [get_clock_info -name $clk]
    report_timing -setup -to_clock $name -npaths 15 -detail summary -file output_files/worst_paths.txt -append -panel_name "Setup $name"
}
project_close
