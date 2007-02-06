INCLUDES = -I$(top_srcdir)/src $(all_includes)
METASOURCES = AUTO

kde_module_LTLIBRARIES=konq_diffext.la

konq_diffext_la_SOURCES = kdiffext.cpp
konq_diffext_la_LDFLAGS = -avoid-version -module $(KDE_PLUGIN) $(all_libraries) \
	-lkonq
konq_diffext_la_LIBADD = $(LIB_KPARTS) $(LIB_KFILE)


kdiffext_entrydir = $(kde_servicesdir)
noinst_HEADERS =
kde_services_DATA = diff-ext.desktop
