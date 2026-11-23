# SPDX-License-Identifier: MIT
# uPD933 (CZ-101 sound chip, milestone C3): C++ reference, table generator, stimulus generator,
# log replay and the Verilator bench. Included by cores/cz101/sim/CMakeLists.txt, which sets
# CMAKE_CXX_STANDARD, find_package(verilator) and VFLAGS.
# cores/cz101/tests/upd933_test.sh builds these targets and runs the gate.

set(UPD933_REF "${CMAKE_CURRENT_LIST_DIR}/../ref")
set(UPD933_HDL "${CMAKE_CURRENT_LIST_DIR}/../hdl/upd933")
set(UPD933_SIM "${CMAKE_CURRENT_LIST_DIR}/upd933")

add_library(upd933_ref STATIC "${UPD933_REF}/upd933_ref.cpp")
target_include_directories(upd933_ref PUBLIC "${UPD933_REF}")
target_compile_options(upd933_ref PRIVATE -Wall -Wextra)

add_executable(upd933_tables "${UPD933_REF}/upd933_tables.cpp")
target_link_libraries(upd933_tables PRIVATE upd933_ref)

add_executable(upd933_stimgen "${UPD933_SIM}/upd933_stimgen.cpp")
target_link_libraries(upd933_stimgen PRIVATE upd933_ref)

add_executable(upd933_replay "${UPD933_SIM}/upd933_replay.cpp")
target_link_libraries(upd933_replay PRIVATE upd933_ref)

add_executable(upd933_bench "${UPD933_SIM}/upd933_bench.cpp")
target_link_libraries(upd933_bench PRIVATE upd933_ref)
verilate(upd933_bench SOURCES "${UPD933_HDL}/upd933_mem.sv" "${UPD933_HDL}/upd933.sv"
         TOP_MODULE upd933 PREFIX Vupd933
         VERILATOR_ARGS ${VFLAGS} "-GTABLE_DIR=\"${UPD933_HDL}/\"")
