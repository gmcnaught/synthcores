# SPDX-License-Identifier: MIT
# Worst setup paths of the compiled design (quartus_sta -t sta_paths.tcl):
#   output_files/worst_paths.txt        60 worst paths, all clocks
#   output_files/worst_paths_core.txt   30 worst into the core clock (96 MHz)
#   output_files/worst_paths_video.txt  30 worst into the video clock (40 MHz)
#   output_files/worst_paths_full.txt   the 5 worst into the core clock, in detail
#   output_files/core_near.txt          every core-clock endpoint with less than 1 ns of setup
#       slack, the worst path to each, and a count per module (fmax work list)
#   output_files/core_modules.txt       the worst path from each main module
project_open MT32
create_timing_netlist
read_sdc
update_timing_netlist
set core  [get_clocks {emu|pll|pll_inst|altera_pll_i|general[0].gpll~PLL_OUTPUT_COUNTER|divclk}]
set video [get_clocks {emu|pll|pll_inst|altera_pll_i|general[1].gpll~PLL_OUTPUT_COUNTER|divclk}]
report_timing -setup -npaths 60 -detail summary -file output_files/worst_paths.txt
report_timing -setup -npaths 30 -detail summary -to_clock $core -file output_files/worst_paths_core.txt
report_timing -setup -npaths 30 -detail summary -to_clock $video -file output_files/worst_paths_video.txt
report_timing -setup -npaths 5 -detail full_path -to_clock $core -file output_files/worst_paths_full.txt

# the work list: one path per endpoint within 1 ns of the core period, grouped by module
set period [get_clock_info -period $core]
set margin 1.0
set fh [open output_files/core_near.txt w]
set paths [get_timing_paths -setup -to_clock $core -less_than_slack $margin -npaths 20000 -nworst 1]
set n 0
array set count {}
array set worst {}
foreach_in_collection p $paths {
    incr n
    set slack [get_path_info $p -slack]
    set from [get_node_info -name [get_path_info $p -from]]
    set to   [get_node_info -name [get_path_info $p -to]]
    # module = the instance path below the core, up to 3 levels
    set parts [split $to |]
    set i [lsearch -glob $parts {mt32_core:core}]
    set mod [join [lrange $parts [expr {$i + 1}] [expr {$i + 3}]] |]
    if {![info exists count($mod)]} { set count($mod) 0; set worst($mod) 1e9 }
    incr count($mod)
    if {$slack < $worst($mod)} { set worst($mod) $slack }
    puts $fh [format "%8.3f  %8.3f ns  %s  ->  %s" $slack [expr {$period - $slack}] $from $to]
}
puts $fh ""
puts $fh "$n endpoints with less than $margin ns slack. Per module (endpoints, longest path ns):"
foreach mod [lsort [array names count]] {
    puts $fh [format "%6d  %8.3f  %s" $count($mod) [expr {$period - $worst($mod)}] $mod]
}
close $fh

# the worst path from each main module
set mods {mcs96_core:core mcs96_periph:periph mcs96:cpu mt32_la32:la32 mt32_rv2:rv2 mt32_sound:sound mt32_board:board mt32_lcd:lcd
          synthframe_audio_fifo:fifo synthframe_midi_rx:midi synthframe_mem_ddr3:ddr3 mt32_lcd_video:video}
set fh [open output_files/core_modules.txt w]
foreach m $mods {
    set regs [get_registers -nowarn "*|$m|*"]
    if {[get_collection_size $regs] == 0} { puts $fh "$m: no registers"; continue }
    set ps [get_timing_paths -setup -from $regs -to_clock $core -npaths 1]
    foreach_in_collection p $ps {
        puts $fh [format "%-28s %8.3f ns  %s -> %s" $m [expr {$period - [get_path_info $p -slack]}] \
            [get_node_info -name [get_path_info $p -from]] [get_node_info -name [get_path_info $p -to]]]
    }
}
close $fh
delete_timing_netlist
project_close
