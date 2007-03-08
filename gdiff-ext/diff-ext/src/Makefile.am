# Process this file with automake to produce Makefile.in

INCLUDES = \
  -DDIFF_EXT_DATA_DIR=\""$(pkgdatadir)"\" \
  -DDIFF_EXT_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
  @DIFF_EXT_CFLAGS@

AM_CFLAGS =-Wall -g

libgdiff_ext_la_SOURCES = main.c

lib_LTLIBRARIES = libgdiff-ext.la

libgdiff_ext_la_LDFLAGS = -module -avoid-version

EXTRA_libgdiff_ext_la_SOURCES = gdiff-ext-2.14.c gdiff-ext-2.16.c 

libgdiff_ext_la_LIBADD = @MENU@
libgdiff_ext_la_DEPENDENCIES = @MENU@

EXTRA_DIST = gdiff-ext.h gdiff-ext-2.14.c 
