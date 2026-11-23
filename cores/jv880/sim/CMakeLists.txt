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

set(JV880_SV
    "${ROLAND}/common/sdp_ram.sv" "${ROLAND}/cpu/h8_cpu.sv" "${ROLAND}/submcu/sc55_submcu.sv" "${ROLAND}/mcu/sc55_mcu.sv"
    "${ROLAND}/pcm/sc55_pcm.sv" "${ROLAND}/core/sc55_core.sv"
    "${ROLAND}/board/rom_loader.sv" "${ROLAND}/board/wave_ddr.sv" "${ROLAND}/board/step_pacer.sv" "${ROLAND}/board/resample.sv"
    "${ROLAND}/board/lcd_ctrl.sv"
    "${SYNTHFRAME}/midi/synthframe_midi_rx.sv" "${SYNTHFRAME}/audio/synthframe_audio_fifo.sv"
    "${SYNTHFRAME}/mem/synthframe_mem_ddr3.sv" "${SYNTHFRAME}/mem/synthframe_mem_arb.sv"
    "${SYNTHFRAME}/save/synthframe_nvsave.sv"
    "${HDL}/jv880_lcd_video.sv" "${HDL}/jv880_board.sv")

add_executable(jv880_board_bench jv880_board_bench.cpp "${SC55}/sim/common/midi_demo.cpp" "${NUKED}/src/renderer/smf.cpp")
target_include_directories(jv880_board_bench PRIVATE "${NUKED}/src/renderer")
target_link_libraries(jv880_board_bench PRIVATE sc55_common)
verilate(jv880_board_bench SOURCES ${JV880_SV}
         TOP_MODULE jv880_board PREFIX Vjv880_board INCLUDE_DIRS "${ROLAND}/pcm" "${ROLAND}/board"
         VERILATOR_ARGS ${VFLAGS} -Wno-PINCONNECTEMPTY --public-flat-rw
         "-DRESAMPLE_HEX=\"${ROLAND}/board/resample_coef.hex\"")

# synthframe_nvsave unit bench: a save with the core running, and the paused second pass after a write
add_executable(nvsave_tb nvsave_tb.cpp)
verilate(nvsave_tb SOURCES "${SYNTHFRAME}/save/synthframe_nvsave.sv"
         TOP_MODULE synthframe_nvsave PREFIX Vsynthframe_nvsave VERILATOR_ARGS ${VFLAGS})
