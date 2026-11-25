# JV-880 Verilator benches. Builds the reference model (Nuked-SC55, through the SC-55 core's ref/).
#   cmake -S sim -B out/sim -G Ninja -DCMAKE_BUILD_TYPE=Release && ninja -C out/sim
cmake_minimum_required(VERSION 3.23)
project(jv880_sim LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)   # verilated.cpp (5.048) does not build as C++23 with libc++
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_subdirectory(../../sc55/ref ref)
find_package(verilator REQUIRED HINTS $ENV{VERILATOR_ROOT})

set(HDL "${CMAKE_CURRENT_SOURCE_DIR}/../hdl")
set(MODULES "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules")
set(ROLAND "${MODULES}/roland/hdl")
set(SYNTHFRAME "${MODULES}/synthframe/hdl")
set(SC55 "${CMAKE_CURRENT_SOURCE_DIR}/../../sc55")
set(NUKED "${MODULES}/nuked-sc55")
set(VFLAGS -Wall -Wno-DECLFILENAME -Wno-UNUSEDSIGNAL -Wno-BLKSEQ -O3 --x-assign unique --x-initial unique)

include(${MODULES}/synthframe/cmake/synthframe_files.cmake)
synthframe_files("${CMAKE_CURRENT_SOURCE_DIR}/.." JV880_SV JV880_INC)   # the core's HDL, from cfg/files.yaml

add_executable(jv880_board_bench jv880_board_bench.cpp "${SC55}/sim/common/midi_demo.cpp" "${NUKED}/src/renderer/smf.cpp")
target_include_directories(jv880_board_bench PRIVATE "${NUKED}/src/renderer")
target_link_libraries(jv880_board_bench PRIVATE sc55_common_traced)   # ref_hooks (MIDI out)
verilate(jv880_board_bench SOURCES ${JV880_SV}
         TOP_MODULE jv880_board PREFIX Vjv880_board INCLUDE_DIRS ${JV880_INC}
         VERILATOR_ARGS ${VFLAGS} -Wno-PINCONNECTEMPTY --public-flat-rw
         "-DRESAMPLE_HEX=\"${ROLAND}/board/resample_coef.hex\""
         "-GPANEL_DIR=\"${HDL}/panel/\"")

# synthframe_nvsave unit bench: a save with the core running, and the paused second pass after a write
add_executable(nvsave_tb nvsave_tb.cpp)
verilate(nvsave_tb SOURCES "${SYNTHFRAME}/save/synthframe_nvsave.sv"
         TOP_MODULE synthframe_nvsave PREFIX Vsynthframe_nvsave VERILATOR_ARGS ${VFLAGS})
