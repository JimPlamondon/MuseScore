# Local-development product identity; inherited engine/file-format versions stay separate.
set(MELO_SCORE_APP_ID "com.melopresto.score")
set(MELO_SCORE_APP_NAME "MeloPresto Score")
set(MELO_SCORE_APP_VERSION "0.0.0")
set(MELO_SCORE_ORGANIZATION "MeloPresto LLC")
set(MELO_SCORE_DOMAIN "melopresto.com")
set(MELO_SCORE_SETTINGS_NAME "MeloPrestoScore")
set(MELO_SCORE_IPC_NAME "com.melopresto.score.local")
set(MUSE_APP_NAME_HUMAN_READABLE "${MELO_SCORE_APP_NAME}")
set(MUSE_APP_NAME_MACHINE_READABLE "MeloPrestoScore")
set(MUSE_APP_GUI_IDENTIFIER "${MELO_SCORE_APP_ID}")
set(MUSE_APP_TITLE "${MELO_SCORE_APP_NAME}")
set(MUSE_APP_TITLE_VERSION "${MELO_SCORE_APP_NAME} (Development)")

# Turn OFF to restore retained upstream offers when integrating with MuseScore.
option(MELO_SCORE_SUPPRESS_UPSTREAM_PROMOTIONS "Suppress upstream product offers in MeloPresto Score" ON)
