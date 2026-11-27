# Worst setup paths of the JV-880 build (quartus_sta -t sta_paths.tcl): the shared Roland script,
# modules/roland/mister/sta_paths.tcl, lists the reports.
set project JV880
set board {jv880_board:board}
set extra_mods {}
source [file join [file dirname [info script]] ../../../modules/roland/mister/sta_paths.tcl]
