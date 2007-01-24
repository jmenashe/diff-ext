## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DSETUP_DATA_DIR=\""$(datadir)"\" \
	-DSETUP_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@SETUP_CFLAGS@

bin_PROGRAMS = diff-ext-setup

diff_ext_setup_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h

diff_ext_setup_LDADD = @SETUP_LIBS@ $(INTLLIBS)

