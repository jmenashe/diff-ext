include_directories(${CMAKE_CURRENT_BINARY_DIR}/settings ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR})


########### next target ###############

set(kdiff_ext_PART_SRCS
   kdiffext.cpp
)

#kde4_automoc(${kdiff_ext_PART_SRCS})

kde4_add_plugin(kdiff_ext ${kdiff_ext_PART_SRCS})

target_link_libraries(kdiff_ext ${QT_AND_KDECORE_LIBS} konq settings kparts)

message("diff-ext prefix: ${CMAKE_INSTALL_PREFIX}")

if(user_install)
  install(TARGETS kdiff_ext DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
else(user_install)
  install(TARGETS kdiff_ext DESTINATION ${PLUGIN_INSTALL_DIR})
endif(user_install)

########### install files ###############

install(FILES DESTINATION ${CMAKE_INSTALL_PREFIX}/share/services)
install(FILES kdiff-ext.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/services)

kde4_install_icons(hicolor)



#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/settings -I$(top_srcdir)/src $(all_includes)
#METASOURCES = AUTO
#
#KDE_ICON = AUTO
#
#kde_module_LTLIBRARIES=kdiff_ext.la
#
#kdiff_ext_la_SOURCES = kdiffext.cpp
#kdiff_ext_la_LDFLAGS = -avoid-version -module $(KDE_PLUGIN) $(all_libraries) \
#	-lkonq
#kdiff_ext_la_LIBADD = $(top_builddir)/settings/libsettings.la $(LIB_KPARTS) \
#	$(LIB_KFILE)
#
#
#kdiff_ext_entrydir = $(kde_servicesdir)
#
#kde_services_DATA = kdiff-ext.desktop
#
