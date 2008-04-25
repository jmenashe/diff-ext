include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR} )


########### next target ###############
set(settings_STAT_SRCS "")

kde3_add_kcfg_files(settings_STAT_SRCS settings.kcfgc)

#kde3_automoc(${settings_STAT_SRCS})

add_library(settings STATIC ${settings_STAT_SRCS})


########### install files ###############
message("settings prefix: ${CMAKE_INSTALL_PREFIX}")

install(FILES kdiffext.kcfg DESTINATION ${CMAKE_INSTALL_PREFIX}/share/config.kcfg)

#original Makefile.am contents follow:

#INCLUDES = $(all_includes)
#METASOURCES = AUTO
#kde_kcfg_DATA = kdiffext.kcfg
#
#libsettings_la_LDFLAGS = -avoid-version $(all_libraries)
#noinst_LTLIBRARIES = libsettings.la
#libsettings_la_SOURCES = kdiffext.kcfg settings.cpp
