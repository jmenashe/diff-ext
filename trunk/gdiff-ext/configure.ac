# Process this file with autoconf to produce a configure script.

AC_INIT(gdiff-ext, 0.3.0, szorin@comcast.net)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AM_DISABLE_STATIC
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_INTLTOOL()
AC_PROG_LIBTOOL
AC_PATH_PROG(GCONFTOOL, gconftool-2)
AM_GCONF_SOURCE_2

AC_CHECK_FUNCS(nautilus_menu_new)

PKG_CHECK_MODULES(DIFF_EXT, [libnautilus-extension >= 2.14.0])
AC_SUBST(DIFF_EXT_CFLAGS)
AC_SUBST(DIFF_EXT_LIBS)

pkg_modules="gtk+-2.0 >= 2.0.0 gconf-2.0 >= 2.14.0"
PKG_CHECK_MODULES(SETUP, [$pkg_modules])
AC_SUBST(SETUP_CFLAGS)
AC_SUBST(SETUP_LIBS)

_PKG_CONFIG(EXTENSION_DIR,[variable=extensiondir], [libnautilus-extension])
AC_ARG_VAR(EXTENSION_DIR)
libdir=$pkg_cv_EXTENSION_DIR

GETTEXT_PACKAGE=diff-ext
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Package name for gettext])

ALL_LINGUAS="ru de ja uk"
AM_GLIB_GNU_GETTEXT

diff_extlocaledir='${prefix}/${DATADIRNAME}/locale'
AC_SUBST(diff_extlocaledir)

AC_CONFIG_FILES([
Makefile
po/Makefile.in
diff-ext/Makefile
diff-ext/src/Makefile
diff-ext/icons/Makefile
diff-ext/icons/diff-ext/Makefile
setup/Makefile
setup/src/Makefile
data/Makefile
])

AC_OUTPUT
