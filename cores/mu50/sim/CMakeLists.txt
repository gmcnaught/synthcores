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
