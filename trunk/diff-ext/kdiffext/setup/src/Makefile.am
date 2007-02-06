# set the include path for X, qt and KDE
INCLUDES = $(all_includes) -DDIFF_EXT_DOC_DIR=\""${datadir}/doc/diff-ext"\"

# these are the headers for your project


# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kdiffextsetup.pot

KDE_ICON = AUTO

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kdiffextsetup

# the application source, library search path, and link libraries
kdiffextsetup_SOURCES = main.cpp mainwidget.cpp ui_base.ui
kdiffextsetup_LDFLAGS = $(KDE_RPATH) $(all_libraries)
kdiffextsetup_LDADD = $(LIB_KIO) $(LIB_KDEUI)

# this is where the desktop file will go 
shelldesktopdir   = $(kde_appsdir)/Utilities
shelldesktop_DATA = kdiffextsetup.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir   = $(kde_datadir)/kdiffextsetup
shellrc_DATA = kdiffextsetupui.rc

