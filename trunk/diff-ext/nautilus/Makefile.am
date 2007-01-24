# Process this file with automake to produce Makefile.in

SUBDIRS = diff-ext setup data

# no ${docdir} for autoconf 2.59, so replace it with ${datadir}/doc/diff-ext
diff_extdocdir = ${datadir}/doc/diff-ext
diff_extdoc_DATA = \
  README\
  COPYING\
  AUTHORS\
  ChangeLog\
  INSTALL\
  NEWS\
  TODO

EXTRA_DIST = $(diff_extdoc_DATA)

schemadir   = @GCONF_SCHEMA_FILE_DIR@
schema_DATA = diff-ext.schemas

install-data-local:
	GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $(srcdir)/$(schema_DATA)
