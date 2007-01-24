# Process this file with automake to produce Makefile.in

desktopdir 	 = $(datadir)/applications
schemasdir	 = $(GCONF_SCHEMA_FILE_DIR)

schemas_in_files = diff-ext.schemas.in
schemas_DATA 	 = $(schemas_in_files:.schemas.in=.schemas)
@INTLTOOL_SCHEMAS_RULE@

desktop_in_files = diff-ext.desktop.in
desktop_DATA = $(diff-ext:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@