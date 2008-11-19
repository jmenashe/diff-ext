include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR} )


########### next target ###############
set(settings_STAT_SRCS "")

kde4_add_kcfg_files(settings_STAT_SRCS settings.kcfgc)

add_library(settings STATIC ${settings_STAT_SRCS})


########### install files ###############
message("settings prefix: ${CMAKE_INSTALL_PREFIX}")

install(FILES kdiffext.kcfg DESTINATION ${CMAKE_INSTALL_PREFIX}/share/config.kcfg)

