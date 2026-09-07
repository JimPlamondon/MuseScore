# SPDX-License-Identifier: GPL-3.0-only
# MuseScore-Studio-CLA-applies
#
# JiMStaff Milestone 1 — build and link the melo-musescore-bridge Rust
# staticlib from an explicit JiMS Kernel checkout. Fails clearly when the
# checkout, cargo, or the built ABI is missing; never vendors Rust build
# outputs into this repository.

function(setup_jims_bridge target)
    # Preserve the selected checkout across automatic CMake reconfiguration.
    # The environment supplies the initial default; -DJIMS_ROOT changes an
    # existing build explicitly.
    if (NOT DEFINED JIMS_ROOT)
        set(JIMS_ROOT "$ENV{JIMS_ROOT}")
        if (NOT JIMS_ROOT)
            set(JIMS_ROOT "/Users/jim/Developer/JiMS/GitHub/melopresto")
        endif()
    endif()
    set(JIMS_ROOT "${JIMS_ROOT}" CACHE PATH "JiMS Kernel checkout used by this build")
    set(JIMS_WORKSPACE "${JIMS_ROOT}/Libraries/melo")
    set(JIMS_BRIDGE_CRATE "${JIMS_WORKSPACE}/crates/melo-musescore-bridge")

    if (NOT EXISTS "${JIMS_BRIDGE_CRATE}/Cargo.toml")
        message(FATAL_ERROR
                "JiMS bridge crate not found at ${JIMS_BRIDGE_CRATE}. "
                "Configure with -DJIMS_ROOT=<path> to select a jims checkout whose main contains "
                "crates/melo-musescore-bridge (JiMStaff Milestone 1).")
    endif()

    find_program(CARGO_EXECUTABLE cargo HINTS "$ENV{HOME}/.cargo/bin")
    if (NOT CARGO_EXECUTABLE)
        message(FATAL_ERROR "cargo not found; install Rust to build the JiMS bridge.")
    endif()

    set(JIMS_BRIDGE_LIB "${JIMS_WORKSPACE}/target/release/libmelo_musescore_bridge.a")
    execute_process(
        COMMAND ${CARGO_EXECUTABLE} build --release -p melo-musescore-bridge
        WORKING_DIRECTORY ${JIMS_WORKSPACE}
        RESULT_VARIABLE JIMS_CARGO_RESULT
        OUTPUT_VARIABLE JIMS_CARGO_OUT
        ERROR_VARIABLE JIMS_CARGO_ERR)
    if (NOT JIMS_CARGO_RESULT EQUAL 0)
        message(FATAL_ERROR "cargo build of melo-musescore-bridge failed:\n${JIMS_CARGO_ERR}")
    endif()
    if (NOT EXISTS "${JIMS_BRIDGE_LIB}")
        message(FATAL_ERROR "expected staticlib missing after build: ${JIMS_BRIDGE_LIB}")
    endif()

    add_library(melo_musescore_bridge STATIC IMPORTED GLOBAL)
    set_target_properties(melo_musescore_bridge PROPERTIES
                          IMPORTED_LOCATION "${JIMS_BRIDGE_LIB}")
    target_include_directories(${target} PRIVATE "${JIMS_BRIDGE_CRATE}/include")
    target_link_libraries(${target} PRIVATE melo_musescore_bridge)
    message(STATUS "JiMS bridge linked from ${JIMS_BRIDGE_LIB}")
endfunction()
