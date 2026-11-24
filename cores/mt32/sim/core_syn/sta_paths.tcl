project_open mt32_core
create_timing_netlist
read_sdc
update_timing_netlist
report_timing -setup -npaths 200 -nworst 1 -detail summary -file output_files/paths.rpt
report_timing -setup -npaths 3 -detail full_path -file output_files/paths_full.rpt
project_close
