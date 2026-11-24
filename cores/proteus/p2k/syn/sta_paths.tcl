# Worst setup paths of the standalone build (winbuild_snd.sh): output_files/paths.rpt
project_open p2k_snd
create_timing_netlist
read_sdc
update_timing_netlist
report_timing -setup -npaths 400 -nworst 1 -detail summary -file output_files/paths.rpt
report_timing -setup -npaths 3 -detail full_path -file output_files/paths_full.rpt
project_close
