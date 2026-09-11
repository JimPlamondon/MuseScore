# Install script for directory: /Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/macos_integration

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/mscore.app/Contents/PlugIns" TYPE DIRECTORY FILES "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/src/macos_integration/MuseScoreQuickLookPreviewExtension.appex" USE_SOURCE_PERMISSIONS)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/mscore.app/Contents/PlugIns/MuseScoreQuickLookPreviewExtension.appex/Contents/MacOS/MuseScoreQuickLookPreviewExtension" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/mscore.app/Contents/PlugIns/MuseScoreQuickLookPreviewExtension.appex/Contents/MacOS/MuseScoreQuickLookPreviewExtension")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Library/Developer/CommandLineTools/usr/lib/swift/macosx"
      -delete_rpath "/usr/lib/swift"
      -add_rpath "@executable_path/../../../../Frameworks"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/mscore.app/Contents/PlugIns/MuseScoreQuickLookPreviewExtension.appex/Contents/MacOS/MuseScoreQuickLookPreviewExtension")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/mscore.app/Contents/PlugIns/MuseScoreQuickLookPreviewExtension.appex/Contents/MacOS/platforms/libqcocoa.dylib")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/usr/local/mscore.app/Contents/PlugIns/MuseScoreQuickLookPreviewExtension.appex/Contents/MacOS/platforms" TYPE FILE FILES "/Users/jim/Qt/6.10.2/macos/plugins/platforms/libqcocoa.dylib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  
    execute_process(COMMAND rm -f "/usr/local/mscore.app/Contents/PlugIns/MuseScoreQuickLookPreviewExtension.appex/Contents/MacOS/MuseScoreQuickLookPreviewExtension.emit-module.d")
    execute_process(COMMAND rm -f "/usr/local/mscore.app/Contents/PlugIns/MuseScoreQuickLookPreviewExtension.appex/Contents/MacOS/MuseScoreQuickLookPreviewExtension.d")
    # codesign refuses to sign a bundle whose nested code is unsigned, and the
    # copied Qt platform plugin is unsigned in some local Qt installs.
    execute_process(COMMAND codesign --force --sign - --timestamp=none "/usr/local/mscore.app/Contents/PlugIns/MuseScoreQuickLookPreviewExtension.appex/Contents/MacOS/platforms/libqcocoa.dylib")
    execute_process(COMMAND codesign --force --sign - --timestamp=none --options runtime --entitlements "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/macos_integration/entitlements.plist" "/usr/local/mscore.app/Contents/PlugIns/MuseScoreQuickLookPreviewExtension.appex")

endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/src/macos_integration/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
