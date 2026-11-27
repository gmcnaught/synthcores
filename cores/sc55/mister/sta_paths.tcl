# Worst setup paths of the SC-55 build (quartus_sta -t sta_paths.tcl): the shared Roland script,
# modules/roland/mister/sta_paths.tcl, lists the reports.
set project SC55
set board {sc55_board:board}
set extra_mods {kbd_buttons:kbd}
source [file join [file dirname [info script]] ../../../modules/roland/mister/sta_paths.tcl]
