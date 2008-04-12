include_directories(${CMAKE_SOURCE_DIR}/settings ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR} )


########### next target ###############

SET(kdiffextsetup_SRCS
   main.cpp
   diff.cpp
   diff3.cpp
)

kde3_automoc(${kdiffextsetup_SRCS})

set(kdiffextsetup_UI
    ui_base.ui
    general.ui
)

kde3_add_ui_files(kdiffextsetup_SRCS ${kdiffextsetup_UI})

kde3_add_executable(kdiffextsetup ${kdiffextsetup_SRCS})

target_link_libraries(kdiffextsetup ${QT_AND_KDECORE_LIBS} settings kio kdeui)

install(TARGETS kdiffextsetup DESTINATION bin)


########### install files ###############

install(FILES  kdiffextsetupui.rc DESTINATION /share/apps/kdiffextsetup)
install(FILES  kdiffextsetup.desktop DESTINATION /share/applnk/Utilities)

kde3_install_icons(hicolor)



#original Makefile.am contents follow:

## set the include path for X, qt and KDE
#INCLUDES = -I$(top_srcdir)/settings $(all_includes) \
#	-DDIFF_EXT_DOC_DIR=\""${datadir}/doc/diff-ext"\"
#
## these are the headers for your project
#
#
## let automoc handle all of the meta source files (moc)
#METASOURCES = AUTO
#
#KDE_ICON = AUTO
#
##########################################################################
## APPLICATION SECTION
##########################################################################
## this is the program that gets installed.  it's name is used for all
## of the other Makefile.am variables
#bin_PROGRAMS = kdiffextsetup
#
## the application source, library search path, and link libraries
#kdiffextsetup_SOURCES = main.cpp ui_base.ui diff.cpp diff3.cpp general.ui
#kdiffextsetup_LDFLAGS = $(KDE_RPATH) $(all_libraries)
#kdiffextsetup_LDADD = $(top_builddir)/settings/libsettings.la $(LIB_KIO) \
#	$(LIB_KDEUI)
#
## this is where the desktop file will go 
#shelldesktopdir = $(kde_appsdir)/Utilities
#shelldesktop_DATA = kdiffextsetup.desktop
#
## this is where the shell's XML-GUI resource file goes
#shellrcdir = $(kde_datadir)/kdiffextsetup
#shellrc_DATA = kdiffextsetupui.rc
#
#noinst_HEADERS = diff.h diff3.h main.h
