include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR} )


########### main target ###############
set(settings_STAT_SRCS "")

kde4_add_kcfg_files(settings_STAT_SRCS settings.kcfgc)

add_library(settings STATIC ${settings_STAT_SRCS})
set_target_properties(settings PROPERTIES COMPILE_FLAGS -fPIC)

########### install files ###############

install(FILES kdiffext.kcfg DESTINATION ${CMAKE_INSTALL_PREFIX}/share/config.kcfg)

