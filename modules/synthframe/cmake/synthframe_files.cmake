# SPDX-License-Identifier: MIT
# synthframe_files(<core dir> <sources var> <include dirs var> [<defines var>])
# The core's HDL and include directories from <core dir>/sim/files.f, which synthcore writes from
# cfg/files.yaml. Pass them to verilate() as SOURCES and INCLUDE_DIRS. CMake re-runs when the
# .f changes. The .f's +define+ lines (cfg/macros.def [all]) come back in <defines var> as
# -DNAME=VALUE, for VERILATOR_ARGS; a .f with defines and no <defines var> is an error, so a
# bench can't build the other configuration by mistake.
function(synthframe_files core_dir src_var inc_var)
    set(f "${core_dir}/sim/files.f")
    get_filename_component(dir "${f}" DIRECTORY)
    file(STRINGS "${f}" lines)
    set(src "")
    set(inc "")
    set(defs "")
    foreach(line IN LISTS lines)
        if(line MATCHES "^//" OR line STREQUAL "")
            continue()
        elseif(line MATCHES "^\\+incdir\\+(.+)$")
            get_filename_component(p "${dir}/${CMAKE_MATCH_1}" ABSOLUTE)
            list(APPEND inc "${p}")
        elseif(line MATCHES "^\\+define\\+(.+)$")
            list(APPEND defs "-D${CMAKE_MATCH_1}")
        else()
            get_filename_component(p "${dir}/${line}" ABSOLUTE)
            list(APPEND src "${p}")
        endif()
    endforeach()
    if(defs AND ARGC LESS 4)
        message(FATAL_ERROR "${f} has macros (${defs}): pass a 4th variable to synthframe_files and add it to VERILATOR_ARGS")
    endif()
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${f}")
    set(${src_var} "${src}" PARENT_SCOPE)
    set(${inc_var} "${inc}" PARENT_SCOPE)
    if(ARGC GREATER 3)
        set(${ARGV3} "${defs}" PARENT_SCOPE)
    endif()
endfunction()

# synthframe_lint_vlt(<var>): the hdl-lint waiver files (modules/synthframe/bin/hdl-lint --vlt),
# for VERILATOR_ARGS, so a bench builds with the same waivers as the lint gate instead of global
# -Wno-* options.
function(synthframe_lint_vlt var)
    get_filename_component(root "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../../.." ABSOLUTE)
    file(GLOB cores "${root}/cores/*/cfg/lint.vlt")
    file(GLOB mods "${root}/modules/*/cfg/lint.vlt")
    set(${var} "${root}/modules/synthframe/lint/common.vlt" ${cores} ${mods} PARENT_SCOPE)
endfunction()
