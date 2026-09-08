# SPDX-License-Identifier: GPL-3.0-only
# MuseScore-CLA-applies
set(MODULE_TEST musesounds_test)
set(MODULE_TEST_SRC ${CMAKE_CURRENT_LIST_DIR}/promotionpolicy_tests.cpp)
set(MODULE_TEST_LINK musesounds appshell)
include(SetupGTest)
target_include_directories(musesounds_test PRIVATE ${PROJECT_SOURCE_DIR}/src/appshell ${PROJECT_SOURCE_DIR}/src/musesounds)
