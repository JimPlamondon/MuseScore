# Install script for directory: /Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/braille/tables

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/mscore.app/Contents/Resources/tables" TYPE FILE FILES
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/braille/tables/ascii-to-unicode.dis"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/braille/tables/ascii-us-patterns.cti"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/braille/tables/en-us-symbols.mus"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/braille/tables/fr.mus"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/braille/tables/it.mus"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/braille/tables/smufl-symbols.mus"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/braille/tables/unicode-to-ascii.dis"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/src/braille/tables/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
