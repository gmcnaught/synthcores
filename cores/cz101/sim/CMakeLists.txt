# CZ-101 Verilator benches.
#   cmake -S cores/cz101/sim -B cores/cz101/out/sim -G Ninja -DCMAKE_BUILD_TYPE=Release
#   ninja -C cores/cz101/out/sim
cmake_minimum_required(VERSION 3.23)
project(cz101_sim LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)   # verilated.cpp (5.048) does not build as C++23 with libc++
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(verilator REQUIRED HINTS $ENV{VERILATOR_ROOT})

set(HDL "${CMAKE_CURRENT_SOURCE_DIR}/../hdl")
set(VFLAGS -Wall -Wno-DECLFILENAME -Wno-UNUSEDSIGNAL -Wno-UNUSEDPARAM -Wno-BLKSEQ -O3 --x-assign unique --x-initial unique)

# uPD7810 instruction set against MAME traces (milestone C1)
add_executable(upd7810_bench upd7810_bench.cpp)
verilate(upd7810_bench SOURCES "${HDL}/upd7810/upd7810_core.sv"
         TOP_MODULE upd7810_core PREFIX Vupd7810_core
         VERILATOR_ARGS ${VFLAGS} "-GDECODE_HEX=\"${HDL}/upd7810/upd7810_decode.hex\"")

include(upd933.cmake OPTIONAL)
