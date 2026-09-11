# Install script for directory: /Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/share/locale

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/mscore.app/Contents/Resources/locale" TYPE FILE FILES
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/share/locale/languages.json"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_af.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_ar.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_ar_DZ.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_ar_EG.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_ar_SD.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_ast.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_be.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_bg.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_br.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_ca.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_ca@valencia.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_cs.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_cy.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_da.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_de.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_el.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_en.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_en_GB.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_en_US.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_eo.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_es.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_et.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_eu.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_fa.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_fi.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_fil.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_fo.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_fr.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_ga.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_gd.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_gl.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_he.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_hi_IN.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_hr.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_hu.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_hy.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_id.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_ig.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_it.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_ja.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_ka.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_kab.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_ko.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_lt.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_lv.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_ml.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_mn_MN.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_mt.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_nb.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_nl.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_nn.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_pl.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_pt.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_pt_BR.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_ro.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_ru.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_scn.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_sk.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_sl.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_sr.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_sr_RS.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_sv.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_sv_SE.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_th.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_tr.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_uk.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_uz@Latn.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_vi.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_zh_CN.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_zh_HK.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/instruments_zh_TW.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_af.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_ar.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_ar_DZ.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_ar_EG.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_ar_SD.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_ast.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_be.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_bg.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_br.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_ca.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_ca@valencia.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_cs.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_cy.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_da.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_de.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_el.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_en.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_en_GB.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_en_US.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_eo.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_es.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_et.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_eu.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_fa.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_fi.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_fil.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_fo.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_fr.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_ga.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_gd.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_gl.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_he.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_hi_IN.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_hr.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_hu.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_hy.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_id.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_ig.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_it.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_ja.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_ka.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_kab.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_ko.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_lt.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_lv.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_ml.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_mn_MN.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_mt.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_nb.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_nl.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_nn.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_pl.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_pt.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_pt_BR.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_ro.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_ru.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_scn.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_sk.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_sl.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_sr.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_sr_RS.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_sv.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_sv_SE.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_th.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_tr.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_uk.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_uz@Latn.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_vi.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_zh_CN.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_zh_HK.qm"
    "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/musescore_zh_TW.qm"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/mscore.app/Contents/Resources/locale" TYPE DIRECTORY FILES "/Users/jim/Qt/6.10.2/macos/translations/" FILES_MATCHING REGEX "qt_.*\\.qm" REGEX "qt_help_.*\\.qm" EXCLUDE REGEX "qtbase_.*\\.qm" REGEX "qt*_en\\.qm" EXCLUDE)
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/share/locale/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
