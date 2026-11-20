# Verilator benches. Builds the reference model (ref/) too.
#   cmake -S sim -B out/sim -G Ninja -DCMAKE_BUILD_TYPE=Release && ninja -C out/sim
cmake_minimum_required(VERSION 3.23)
project(sc55_sim LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)   # verilated.cpp (5.048) does not build as C++23 with libc++; ref/ keeps 23
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_subdirectory(../ref ref)
find_package(verilator REQUIRED HINTS $ENV{VERILATOR_ROOT})

set(HDL "${CMAKE_CURRENT_SOURCE_DIR}/../hdl")
set(VFLAGS -Wall -Wno-DECLFILENAME -Wno-UNUSEDSIGNAL -Wno-BLKSEQ -O3 --x-assign unique --x-initial unique)

add_executable(cpu_lockstep cpu/cpu_lockstep.cpp)
target_link_libraries(cpu_lockstep PRIVATE sc55_common_traced)
verilate(cpu_lockstep SOURCES "${HDL}/cpu/h8_cpu.sv" TOP_MODULE h8_cpu VERILATOR_ARGS ${VFLAGS})
