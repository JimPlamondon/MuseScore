# SPDX-License-Identifier: GPL-3.0-only
# MuseScore-CLA-applies
set(MODULE_TEST musesounds_test)
set(MODULE_TEST_SRC ${CMAKE_CURRENT_LIST_DIR}/promotionpolicy_tests.cpp)
set(MODULE_TEST_LINK musesounds)
if (MUE_BUILD_APPSHELL_MODULE)
    list(APPEND MODULE_TEST_LINK appshell)
else()
    # The hosted unit-test configuration omits the application shell. Compile
    # the configuration under test without requiring the full shell module.
    list(APPEND MODULE_TEST_SRC ${PROJECT_SOURCE_DIR}/src/appshell/internal/appshellconfiguration.cpp)
endif()
include(SetupGTest)
target_include_directories(musesounds_test PRIVATE ${PROJECT_SOURCE_DIR}/src/appshell ${PROJECT_SOURCE_DIR}/src/musesounds)
