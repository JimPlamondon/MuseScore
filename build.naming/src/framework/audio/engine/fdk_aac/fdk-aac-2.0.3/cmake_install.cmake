# Install script for directory: /Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/framework/audio/thirdparty/fdk-aac/fdk-aac-2.0.3

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

if(CMAKE_INSTALL_COMPONENT STREQUAL "DEV" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/src/framework/audio/engine/fdk_aac/fdk-aac-2.0.3/libfdk-aac.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libfdk-aac.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libfdk-aac.a")
    execute_process(COMMAND "/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libfdk-aac.a")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "DEV" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/fdk-aac" TYPE FILE FILES
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/framework/audio/thirdparty/fdk-aac/fdk-aac-2.0.3/libSYS/include/machine_type.h"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/framework/audio/thirdparty/fdk-aac/fdk-aac-2.0.3/libSYS/include/genericStds.h"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/framework/audio/thirdparty/fdk-aac/fdk-aac-2.0.3/libSYS/include/FDK_audio.h"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/framework/audio/thirdparty/fdk-aac/fdk-aac-2.0.3/libSYS/include/syslib_channelMapDescr.h"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/framework/audio/thirdparty/fdk-aac/fdk-aac-2.0.3/libAACenc/include/aacenc_lib.h"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/framework/audio/thirdparty/fdk-aac/fdk-aac-2.0.3/libAACdec/include/aacdecoder_lib.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/src/framework/audio/engine/fdk_aac/fdk-aac-2.0.3/fdk-aac.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/fdk-aac/fdk-aac-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/fdk-aac/fdk-aac-targets.cmake"
         "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/src/framework/audio/engine/fdk_aac/fdk-aac-2.0.3/CMakeFiles/Export/acc8a7d61daeba99aa97ccf1c9e754eb/fdk-aac-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/fdk-aac/fdk-aac-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/fdk-aac/fdk-aac-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/fdk-aac" TYPE FILE FILES "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/src/framework/audio/engine/fdk_aac/fdk-aac-2.0.3/CMakeFiles/Export/acc8a7d61daeba99aa97ccf1c9e754eb/fdk-aac-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/fdk-aac" TYPE FILE FILES "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/src/framework/audio/engine/fdk_aac/fdk-aac-2.0.3/CMakeFiles/Export/acc8a7d61daeba99aa97ccf1c9e754eb/fdk-aac-targets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/fdk-aac" TYPE FILE FILES
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/src/framework/audio/engine/fdk_aac/fdk-aac-2.0.3/fdk-aac-config.cmake"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/src/framework/audio/engine/fdk_aac/fdk-aac-2.0.3/fdk-aac-config-version.cmake"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/src/framework/audio/engine/fdk_aac/fdk-aac-2.0.3/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
