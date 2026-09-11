include("/Users/jim/Developer/MeloPresto/Temp/melopresto-score-claude-naming-residue/build.naming/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/MuseScoreStudio-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative;qtdeclarative")

qt6_deploy_qml_imports(TARGET MuseScoreStudio PLUGINS_FOUND plugins_found)
if(NOT DEFINED __QT_DEPLOY_POST_BUILD)
    qt6_deploy_runtime_dependencies(
        EXECUTABLE "mscore.app"
        ADDITIONAL_MODULES ${plugins_found}
    )
endif()