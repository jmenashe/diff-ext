# Process this file with automake to produce Makefile.in

SUBDIRS = diff-ext setup po data

# no ${docdir} for autoconf 2.59, so replace it with ${datadir}/doc/diff-ext
diff_extdocdir = ${datadir}/doc/diff-ext
diff_extdoc_DATA = \
  README \
  COPYING \
  AUTHORS \
  ChangeLog \
  INSTALL \
  NEWS \
  TODO

EXTRA_DIST = \
  $(diff_extdoc_DATA) \
  intltool-extract.in \
  intltool-merge.in \
  intltool-update.in 

DISTCLEANFILES = \
  intltool-extract \
  intltool-merge \
  intltool-update
