# MU50 Verilator benches.
#   cmake -S cores/mu50/sim -B cores/mu50/out/sim -G Ninja -DCMAKE_BUILD_TYPE=Release
#   ninja -C cores/mu50/out/sim
cmake_minimum_required(VERSION 3.23)
project(mu50_sim LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)   # verilated.cpp (5.048) does not build as C++23 with libc++
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(verilator REQUIRED HINTS $ENV{VERILATOR_ROOT})

set(HDL "${CMAKE_CURRENT_SOURCE_DIR}/../hdl")
set(VFLAGS -Wall -Wno-DECLFILENAME -Wno-UNUSEDSIGNAL -Wno-UNUSEDPARAM -O3 --x-assign unique --x-initial unique)

# H8/300H instruction set against MAME traces (milestone U1)
add_executable(h8h_bench h8h_bench.cpp)
verilate(h8h_bench SOURCES "${HDL}/h8h/h8h_core.sv" INCLUDE_DIRS "${HDL}/h8h"
         TOP_MODULE h8h_core PREFIX Vh8h_core VERILATOR_ARGS ${VFLAGS})

# SWP00 against the C++ copy of MAME's model (milestone U3)
set(REF "${CMAKE_CURRENT_SOURCE_DIR}/../ref")
add_executable(swp00_bench swp00_bench.cpp "${REF}/swp00_ref.cpp")
target_include_directories(swp00_bench PRIVATE "${REF}" "${HDL}/swp00")
verilate(swp00_bench SOURCES "${HDL}/swp00/swp00_pkg.sv" "${HDL}/swp00/swp00_meg.sv" "${HDL}/swp00/swp00.sv"
         INCLUDE_DIRS "${HDL}/swp00" TOP_MODULE swp00 PREFIX Vswp00 VERILATOR_ARGS ${VFLAGS} --public-flat-rw)

# the reference alone, replaying MAME's logs (U3 reference check)
add_executable(swp00_replay swp00_replay.cpp "${REF}/swp00_ref.cpp")
target_include_directories(swp00_replay PRIVATE "${REF}")
# MAME's swp00 code overflows s32 sums under extreme settings; C leaves that undefined and clang
# then differs from two's complement wrap. The reference wraps, as the HDL (docs/plan.md, U3).
set_source_files_properties("${REF}/swp00_ref.cpp" PROPERTIES COMPILE_OPTIONS "-fwrapv")

# H8/3003 core + peripherals on a software MU50 board, against MAME's firmware trace (U2)
add_executable(mu50sys_bench mu50sys_bench.cpp)
verilate(mu50sys_bench SOURCES "${HDL}/h8h/h8h_core.sv" "${HDL}/h8h/h83003_periph.sv" "${HDL}/h8h/h8h_sys.sv"
         INCLUDE_DIRS "${HDL}/h8h" TOP_MODULE h8h_sys PREFIX Vh8h_sys VERILATOR_ARGS ${VFLAGS} -Wno-BLKSEQ)

# the same with peripheral debug messages (H8H_DEBUG)
add_executable(mu50sys_bench_dbg mu50sys_bench.cpp)
verilate(mu50sys_bench_dbg SOURCES "${HDL}/h8h/h8h_core.sv" "${HDL}/h8h/h83003_periph.sv" "${HDL}/h8h/h8h_sys.sv"
         INCLUDE_DIRS "${HDL}/h8h" TOP_MODULE h8h_sys PREFIX Vh8h_sys VERILATOR_ARGS ${VFLAGS} -Wno-BLKSEQ +define+H8H_DEBUG)

# the whole MU50 board on the real ROMs: corpus samples against MAME's (U4)
set(SF "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/synthframe/hdl")
add_executable(mu50_bench mu50_bench.cpp)
verilate(mu50_bench SOURCES "${HDL}/h8h/h8h_core.sv" "${HDL}/h8h/h83003_periph.sv" "${HDL}/h8h/h8h_sys.sv"
         "${HDL}/swp00/swp00_pkg.sv" "${HDL}/swp00/swp00_meg.sv" "${HDL}/swp00/swp00.sv"
         "${SF}/panel/synthframe_hd44780.sv" "${HDL}/mu50_lcd.sv" "${HDL}/mu50_board.sv"
         INCLUDE_DIRS "${HDL}/h8h" "${HDL}/swp00" TOP_MODULE mu50_board PREFIX Vmu50_board
         VERILATOR_ARGS ${VFLAGS} -Wno-BLKSEQ -Wno-PINCONNECTEMPTY)

# the MiSTer wrapper with a DDR3 model: pack download, ROM cache, wave and delay lines in DDR3 (U5)
add_executable(mu50_top_bench mu50_top_bench.cpp)
verilate(mu50_top_bench SOURCES "${HDL}/h8h/h8h_core.sv" "${HDL}/h8h/h83003_periph.sv" "${HDL}/h8h/h8h_sys.sv"
         "${HDL}/swp00/swp00_pkg.sv" "${HDL}/swp00/swp00_meg.sv" "${HDL}/swp00/swp00.sv"
         "${SF}/panel/synthframe_hd44780.sv" "${HDL}/mu50_lcd.sv" "${HDL}/mu50_board.sv"
         "${SF}/mem/synthframe_mem_ddr3.sv" "${SF}/mem/synthframe_mem_arb.sv" "${HDL}/mu50_mem.sv"
         "${SF}/panel/synthframe_vtiming.sv" "${SF}/panel/synthframe_charlcd.sv"
         "${SF}/panel/synthframe_status_strip.sv" "${SF}/panel/synthframe_panel_art.sv"
         "${HDL}/mu50_lcd_video.sv"
         "${SF}/midi/synthframe_midi_rx.sv" "${SF}/midi/synthframe_midi_tx.sv" "${SF}/audio/synthframe_audio_fifo.sv"
         "${SF}/save/synthframe_nvsave.sv" "${HDL}/mu50_top.sv"
         INCLUDE_DIRS "${HDL}/h8h" "${HDL}/swp00" "${HDL}/panel" TOP_MODULE mu50_top PREFIX Vmu50_top
         VERILATOR_ARGS ${VFLAGS} -Wno-BLKSEQ -Wno-PINCONNECTEMPTY --public-flat-rw
         "-GPANEL_DIR=\"${HDL}/panel/\"")
