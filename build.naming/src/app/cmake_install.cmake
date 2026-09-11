# Install script for directory: /Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/src/app

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}" TYPE DIRECTORY FILES "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/src/app/mscore.app" USE_SOURCE_PERMISSIONS)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/mscore.app/Contents/MacOS/mscore" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/mscore.app/Contents/MacOS/mscore")
    execute_process(COMMAND /usr/bin/install_name_tool
      -add_rpath "@loader_path/../Frameworks"
      -add_rpath "@executable_path/../Frameworks"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/mscore.app/Contents/MacOS/mscore")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/mscore.app/Contents/Resources/fonts" TYPE FILE FILES
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/bravura/BravuraText.otf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/campania/Campania.otf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/edwin/Edwin-BdIta.otf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/edwin/Edwin-Bold.otf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/edwin/Edwin-Italic.otf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/edwin/Edwin-Roman.otf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/gootville/GootvilleText.otf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/FreeSans.ttf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/FreeSerif.ttf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/FreeSerifBold.ttf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/FreeSerifItalic.ttf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/FreeSerifBoldItalic.ttf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/leland/Leland.otf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/leland/LelandText.otf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/mscore-BC.ttf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/mscoreTab.ttf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/mscore/MScoreText.otf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/musejazz/MuseJazzText.otf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/petaluma/PetalumaScript.otf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/petaluma/PetalumaText.otf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/finalemaestro/FinaleMaestroText-Regular.otf"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/fonts/finalebroadway/FinaleBroadwayText.otf"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/src/app/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
