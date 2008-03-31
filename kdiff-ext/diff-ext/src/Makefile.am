INCLUDES = -I$(top_srcdir)/settings -I$(top_srcdir)/src $(all_includes)
METASOURCES = AUTO

KDE_ICON = AUTO

kde_module_LTLIBRARIES=kdiff_ext.la

kdiff_ext_la_SOURCES = kdiffext.cpp
kdiff_ext_la_LDFLAGS = -avoid-version -module $(KDE_PLUGIN) $(all_libraries) \
	-lkonq
kdiff_ext_la_LIBADD = $(top_builddir)/settings/libsettings.la $(LIB_KPARTS) \
	$(LIB_KFILE)


kdiff_ext_entrydir = $(kde_servicesdir)

kde_services_DATA = kdiff-ext.desktop

