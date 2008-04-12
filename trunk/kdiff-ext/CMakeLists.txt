find_package(KDE3 REQUIRED)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_definitions(${QT_DEFINITIONS} ${KDE3_DEFINITIONS})

link_directories(${KDE3_LIB_DIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_subdirectory(settings)
add_subdirectory(diff-ext)
add_subdirectory(setup)
add_subdirectory(po)
add_subdirectory(doc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR} )


########### install files ###############

install(FILES  README    COPYING    AUTHORS    ChangeLog    INSTALL    NEWS
TODO DESTINATION share/doc/kdiff-ext )



#original Makefile.am contents follow:

#SUBDIRS = settings diff-ext setup po doc
#
#messages: 
#	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
#	LIST=`find . -name \*.h -o -name \*.cpp`; \
#	if test -n "$$LIST"; then \
#	  $(XGETTEXT) -o $(podir)/kdiffext.pot $$LIST; \
#	fi
#
#$(top_srcdir)/configure.in: configure.in.in $(top_srcdir)/subdirs
#	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common configure.in ;
#
#$(top_srcdir)/subdirs:
#	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common subdirs
#
#$(top_srcdir)/acinclude.m4: $(top_srcdir)/admin/acinclude.m4.in $(top_srcdir)/admin/libtool.m4.in
#	@cd $(top_srcdir) && cat admin/acinclude.m4.in admin/libtool.m4.in > acinclude.m4
#
#MAINTAINERCLEANFILES = subdirs configure.in acinclude.m4 configure.files 
#
#package-messages:
#	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common package-messages
#	$(MAKE) -C po merge
#
#diff_extdocdir = ${datadir}/doc/kdiff-ext
#diff_extdoc_DATA = \
#  README \
#  COPYING \
#  AUTHORS \
#  ChangeLog \
#  INSTALL \
#  NEWS \
#  TODO
#
#EXTRA_DIST = admin Makefile.cvs kdiff-ext.kdevelop configure.in.in
#
#dist-hook:
#	cd $(top_distdir) && perl admin/am_edit -padmin
#	cd $(top_distdir) && $(MAKE) -f admin/Makefile.common subdirs
