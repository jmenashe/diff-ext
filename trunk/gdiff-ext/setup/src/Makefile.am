## Process this file with automake to produce Makefile.in

INCLUDES = \
  -DSETUP_DATA_DIR=\""$(datadir)"\" \
  -DSETUP_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
  @SETUP_CFLAGS@

bin_PROGRAMS = gdiffextsetup

gdiffextsetup_SOURCES = \
  main.c \
  model.h \
  support.c \
  support.h \
  interface.c \
  interface.h \
  callbacks.c \
  callbacks.h

gdiffextsetup_LDADD = @SETUP_LIBS@ $(INTLLIBS)

