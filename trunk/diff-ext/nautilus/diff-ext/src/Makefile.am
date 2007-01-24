# Process this file with automake to produce Makefile.in

INCLUDES = \
  -DDIFF_EXT_DATA_DIR=\""$(pkgdatadir)"\" \
  -DDIFF_EXT_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
  @DIFF_EXT_CFLAGS@

AM_CFLAGS =-Wall -g

libdiff_ext_la_SOURCES = main.c

lib_LTLIBRARIES = libdiff-ext.la

libdiff_ext_la_LDFLAGS = -module -avoid-version

libdiff_ext_la_LIBADD = @DIFF_EXT_LIBS@
