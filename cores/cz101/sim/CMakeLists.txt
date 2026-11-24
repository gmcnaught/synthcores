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
include(../../../modules/synthframe/cmake/synthframe_files.cmake)
synthframe_files("${CMAKE_CURRENT_SOURCE_DIR}/.." CZ101_SV CZ101_INC)   # the core's HDL, from cfg/files.yaml

# uPD7810 instruction set against MAME traces (milestone C1)
add_executable(upd7810_bench upd7810_bench.cpp)
verilate(upd7810_bench SOURCES "${HDL}/upd7810/upd7810_core.sv"
         TOP_MODULE upd7810_core PREFIX Vupd7810_core
         VERILATOR_ARGS ${VFLAGS} "-GDECODE_HEX=\"${HDL}/upd7810/upd7810_decode.hex\"")

# uPD7811 with peripherals and internal memory (milestones C1, C2)
add_executable(upd7811_bench upd7811_bench.cpp)
verilate(upd7811_bench SOURCES "${HDL}/upd7810/upd7810_core.sv" "${HDL}/upd7810/upd7810_periph.sv"
         "${HDL}/upd7810/upd7811.sv"
         TOP_MODULE upd7811 PREFIX Vupd7811
         VERILATOR_ARGS ${VFLAGS} --public-flat-rw "-GDECODE_HEX=\"${HDL}/upd7810/upd7810_decode.hex\"")

# CZ-101 board with the real firmware (milestone C4)
add_executable(cz101_bench cz101_bench.cpp)
verilate(cz101_bench SOURCES ${CZ101_SV}
         TOP_MODULE cz101_board PREFIX Vcz101_board
         VERILATOR_ARGS ${VFLAGS} --public-flat-rw -Wno-PINCONNECTEMPTY -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC
         "-GDECODE_HEX=\"${HDL}/upd7810/upd7810_decode.hex\"" "-GTABLE_DIR=\"${HDL}/upd933/\"")

include(upd933.cmake OPTIONAL)
