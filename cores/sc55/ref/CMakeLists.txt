# Reference model: jcmoyer/Nuked-SC55 backend + renderer, built without the
# SDL/rtmidi frontend. Used by tools and by the Verilator benches.
#
#   sc55_backend / sc55-render          : unmodified backend (decoder2 on, as released)
#   sc55_backend_traced / sc55-render-traced
#                                        : patched copy with lockstep hooks
#                                          (ref/patch_ref.py), decoder2 off so every
#                                          code byte goes through MCU_Read
cmake_minimum_required(VERSION 3.23)
project(sc55_ref VERSION 0.7.0 LANGUAGES C CXX)

set(NUKED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/nuked-sc55")
set(NUKED_ENABLE_ASIO 0)
set(NUKED_SOURCE "https://github.com/jcmoyer/Nuked-SC55")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# --- unmodified backend -------------------------------------------------------
set(NUKED_ENABLE_DECODER2 1)
configure_file("${NUKED_DIR}/src/backend/config.h.in"
               "${CMAKE_CURRENT_BINARY_DIR}/cfg_plain/backend/config.h" @ONLY)

file(GLOB BACKEND_SRC "${NUKED_DIR}/src/backend/*.cpp")
file(GLOB DECODER2_SRC "${NUKED_DIR}/src/backend/decoder2/*.cpp")
add_library(sc55_backend STATIC ${BACKEND_SRC} ${DECODER2_SRC}
    "${NUKED_DIR}/src/backend/sha/sha224-256.c")
target_include_directories(sc55_backend PUBLIC
    "${NUKED_DIR}/src" "${NUKED_DIR}/src/backend"
    "${CMAKE_CURRENT_BINARY_DIR}/cfg_plain/backend" "${CMAKE_CURRENT_BINARY_DIR}/cfg_plain")

# --- patched backend with hooks ------------------------------------------------
set(NUKED_ENABLE_DECODER2 0)
configure_file("${NUKED_DIR}/src/backend/config.h.in"
               "${CMAKE_CURRENT_BINARY_DIR}/cfg_traced/backend/config.h" @ONLY)

set(TRACED_SRC_ROOT "${CMAKE_CURRENT_BINARY_DIR}/traced/src")
set(TRACED_DIR "${TRACED_SRC_ROOT}/backend")
file(GLOB_RECURSE NUKED_ALL_SRC "${NUKED_DIR}/src/*")
set(TRACED_SRC "")
foreach(f ${BACKEND_SRC})
    get_filename_component(n "${f}" NAME)
    list(APPEND TRACED_SRC "${TRACED_DIR}/${n}")
endforeach()
set(TRACED_COMMON_SRC "")
foreach(n gain rom_loader path_util term_io)
    list(APPEND TRACED_COMMON_SRC "${TRACED_SRC_ROOT}/common/${n}.cpp")
endforeach()
set(TRACED_RENDER_SRC "")
foreach(n main smf wav)
    list(APPEND TRACED_RENDER_SRC "${TRACED_SRC_ROOT}/renderer/${n}.cpp")
endforeach()
add_custom_command(
    OUTPUT ${TRACED_SRC} ${TRACED_COMMON_SRC} ${TRACED_RENDER_SRC}
           "${TRACED_DIR}/ref_hooks.h" "${TRACED_DIR}/sha/sha224-256.c"
    COMMAND "${Python3_EXECUTABLE}" -I "${CMAKE_CURRENT_SOURCE_DIR}/patch_ref.py"
            "${NUKED_DIR}/src" "${TRACED_SRC_ROOT}"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/patch_ref.py" ${NUKED_ALL_SRC}
    COMMENT "Patching reference backend with lockstep hooks")
add_library(sc55_backend_traced STATIC ${TRACED_SRC} "${TRACED_DIR}/sha/sha224-256.c")
target_include_directories(sc55_backend_traced PUBLIC
    "${TRACED_SRC_ROOT}" "${TRACED_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}/cfg_traced/backend" "${CMAKE_CURRENT_BINARY_DIR}/cfg_traced")

# --- common + renderers ----------------------------------------------------------
set(COMMON_SRC
    "${NUKED_DIR}/src/common/gain.cpp"
    "${NUKED_DIR}/src/common/rom_loader.cpp"
    "${NUKED_DIR}/src/common/path_util.cpp"
    "${NUKED_DIR}/src/common/term_io.cpp")
set(RENDER_SRC
    "${NUKED_DIR}/src/renderer/main.cpp"
    "${NUKED_DIR}/src/renderer/smf.cpp"
    "${NUKED_DIR}/src/renderer/wav.cpp")

add_library(sc55_common STATIC ${COMMON_SRC})
target_include_directories(sc55_common PUBLIC "${NUKED_DIR}/src/common" "${NUKED_DIR}/src")
target_link_libraries(sc55_common PUBLIC sc55_backend)
add_library(sc55_common_traced STATIC ${TRACED_COMMON_SRC})
target_include_directories(sc55_common_traced PUBLIC "${TRACED_SRC_ROOT}/common" "${TRACED_SRC_ROOT}")
target_link_libraries(sc55_common_traced PUBLIC sc55_backend_traced)

add_executable(sc55-render ${RENDER_SRC})
target_link_libraries(sc55-render PRIVATE sc55_common)
add_executable(sc55-render-traced ${TRACED_RENDER_SRC})
target_link_libraries(sc55-render-traced PRIVATE sc55_common_traced)
