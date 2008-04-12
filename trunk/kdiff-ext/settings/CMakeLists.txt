include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR} )


########### next target ###############

SET(settings_STAT_SRCS
   kdiffext.kcfg
   settings.cpp
)

kde3_automoc(${settings_STAT_SRCS})

add_library(settings STATIC ${settings_STAT_SRCS})


########### install files ###############

install(FILES  kdiffext.kcfg DESTINATION /share/config.kcfg)



#original Makefile.am contents follow:

#INCLUDES = $(all_includes)
#METASOURCES = AUTO
#kde_kcfg_DATA = kdiffext.kcfg
#
#libsettings_la_LDFLAGS = -avoid-version $(all_libraries)
#noinst_LTLIBRARIES = libsettings.la
#libsettings_la_SOURCES = kdiffext.kcfg settings.cpp
