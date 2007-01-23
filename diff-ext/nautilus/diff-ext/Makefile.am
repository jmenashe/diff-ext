# Process this file with automake to produce Makefile.in

SUBDIRS = po src

libdiff_extdocdir = ${docdir}/diff-ext
libdiff_extdoc_DATA = \
  README\
  COPYING\
  AUTHORS\
  ChangeLog\
  INSTALL\
  NEWS\
  TODO

EXTRA_DIST = $(libdiff_extdoc_DATA)

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
	  if test -f $$specfile; then \
	    cp -p $$specfile $(distdir); \
	  fi \
	done

schemadir   = @GCONF_SCHEMA_FILE_DIR@
schema_DATA = diff-ext.schemas

install-data-local:
	GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $(srcdir)/$(schema_DATA)
