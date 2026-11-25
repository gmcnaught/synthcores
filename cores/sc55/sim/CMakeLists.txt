# Verilator benches. Builds the reference model (ref/) too.
#   cmake -S sim -B out/sim -G Ninja -DCMAKE_BUILD_TYPE=Release && ninja -C out/sim
cmake_minimum_required(VERSION 3.23)
project(sc55_sim LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)   # verilated.cpp (5.048) does not build as C++23 with libc++; ref/ keeps 23
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_subdirectory(../ref ref)
find_package(verilator REQUIRED HINTS $ENV{VERILATOR_ROOT})

set(HDL "${CMAKE_CURRENT_SOURCE_DIR}/../hdl")
set(ROLAND "${CMAKE_CURRENT_SOURCE_DIR}/../../../modules/roland/hdl")   # H8/532 CPU, MCU, PCM chip (shared with the JV-880)
set(VFLAGS -Wall -Wno-DECLFILENAME -Wno-UNUSEDSIGNAL -Wno-BLKSEQ -O3 --x-assign unique --x-initial unique)
include(../../../modules/synthframe/cmake/synthframe_files.cmake)
synthframe_files("${CMAKE_CURRENT_SOURCE_DIR}/.." SC55_SV SC55_INC)   # the core's HDL, from cfg/files.yaml

add_executable(cpu_lockstep cpu/cpu_lockstep.cpp common/midi_demo.cpp)
target_link_libraries(cpu_lockstep PRIVATE sc55_common_traced)
verilate(cpu_lockstep SOURCES "${ROLAND}/cpu/h8_cpu.sv" TOP_MODULE h8_cpu PREFIX Vh8_cpu VERILATOR_ARGS ${VFLAGS})

add_executable(mcu_lockstep mcu/mcu_lockstep.cpp common/midi_demo.cpp)
target_link_libraries(mcu_lockstep PRIVATE sc55_common_traced)
verilate(mcu_lockstep SOURCES "${ROLAND}/common/sdp_ram.sv" "${ROLAND}/cpu/h8_cpu.sv" "${ROLAND}/submcu/sc55_submcu.sv" "${ROLAND}/mcu/sc55_mcu.sv"
         TOP_MODULE sc55_mcu PREFIX Vsc55_mcu VERILATOR_ARGS ${VFLAGS} -Wno-PINCONNECTEMPTY --public-flat-rw -GROM2_AW=19 -GJV880_RAMS=1)

add_executable(submcu_lockstep submcu/submcu_lockstep.cpp common/midi_demo.cpp)
target_link_libraries(submcu_lockstep PRIVATE sc55_common_traced)
verilate(submcu_lockstep SOURCES "${ROLAND}/common/sdp_ram.sv" "${ROLAND}/submcu/sc55_submcu.sv" TOP_MODULE sc55_submcu PREFIX Vsc55_submcu
         VERILATOR_ARGS ${VFLAGS} --public-flat-rw)

add_executable(pcm_lockstep pcm/pcm_lockstep.cpp common/midi_demo.cpp)
target_link_libraries(pcm_lockstep PRIVATE sc55_common_traced)
verilate(pcm_lockstep SOURCES "${ROLAND}/common/sdp_ram.sv" "${ROLAND}/pcm/sc55_pcm.sv" TOP_MODULE sc55_pcm PREFIX Vsc55_pcm
         INCLUDE_DIRS "${ROLAND}/pcm" VERILATOR_ARGS ${VFLAGS} --public-flat-rw)

add_executable(system_lockstep system/system_lockstep.cpp common/midi_demo.cpp "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/nuked-sc55/src/renderer/smf.cpp")
target_include_directories(system_lockstep PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/nuked-sc55/src/renderer")
target_link_libraries(system_lockstep PRIVATE sc55_common_traced)
verilate(system_lockstep SOURCES "${ROLAND}/common/sdp_ram.sv" "${ROLAND}/cpu/h8_cpu.sv" "${ROLAND}/submcu/sc55_submcu.sv" "${ROLAND}/mcu/sc55_mcu.sv" "${ROLAND}/pcm/sc55_pcm.sv" "${ROLAND}/core/sc55_core.sv"
         TOP_MODULE sc55_core PREFIX Vsc55_core INCLUDE_DIRS "${ROLAND}/pcm" VERILATOR_ARGS ${VFLAGS} -Wno-PINCONNECTEMPTY --public-flat-rw -GROM2_AW=19 -GJV880_RAMS=1)

add_executable(board_lockstep board/board_lockstep.cpp common/midi_demo.cpp)
target_link_libraries(board_lockstep PRIVATE sc55_common_traced)
verilate(board_lockstep SOURCES ${SC55_SV}
         TOP_MODULE sc55_board PREFIX Vsc55_board INCLUDE_DIRS ${SC55_INC}
         VERILATOR_ARGS -Wall -Wno-DECLFILENAME -Wno-UNUSEDSIGNAL -Wno-BLKSEQ -Wno-PINCONNECTEMPTY -O3 --x-assign unique --x-initial unique --public-flat-rw
         "-DLCD_BACK_HEX=\"${HDL}/board/lcd_back.hex\"" "-DSC55_PANEL_DIR=\"${HDL}/panel/\"" "-DRESAMPLE_HEX=\"${ROLAND}/board/resample_coef.hex\"")

add_executable(resample_tb resample/resample_tb.cpp)
verilate(resample_tb SOURCES "${ROLAND}/common/sdp_ram.sv" "${ROLAND}/board/resample.sv" TOP_MODULE resample PREFIX Vresample
         INCLUDE_DIRS "${ROLAND}/board" VERILATOR_ARGS ${VFLAGS} "-DRESAMPLE_HEX=\"${ROLAND}/board/resample_coef.hex\"")

# model-only analysis: ROM2 access statistics (mk2 ROM2-in-SDRAM design)
add_executable(rom2_stats tools/rom2_stats.cpp common/midi_demo.cpp)
target_link_libraries(rom2_stats PRIVATE sc55_common_traced)

add_executable(rom2x_cache_tb board/rom2x_cache_tb.cpp)
verilate(rom2x_cache_tb SOURCES "${ROLAND}/common/sdp_ram.sv" "${ROLAND}/board/rom2x_cache.sv" TOP_MODULE rom2x_cache PREFIX Vrom2x_cache
         VERILATOR_ARGS ${VFLAGS})
