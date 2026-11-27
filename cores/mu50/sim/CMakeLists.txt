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

