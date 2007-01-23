# Process this file with autoconf to produce a configure script.

AC_INIT(diff-ext, 0.1, szorin@comcast.net)
AM_INIT_AUTOMAKE(diff-ext, 0.1)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_PROG_LIBTOOL
AC_PROG_CC
AC_PATH_PROG(GCONFTOOL, gconftool-2)
AM_GCONF_SOURCE_2

PKG_CHECK_MODULES(DIFF_EXT, [libnautilus-extension >= 2.14.0])
AC_SUBST(DIFF_EXT_CFLAGS)
AC_SUBST(DIFF_EXT_LIBS)

_PKG_CONFIG(EXTENSION_DIR,[variable=extensiondir], [libnautilus-extension])
AC_ARG_VAR(EXTENSION_DIR)
libdir=$pkg_cv_EXTENSION_DIR

GETTEXT_PACKAGE=diff-ext
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Package name for gettext])
AC_DEFINE_UNQUOTED(GNOME_EXPLICIT_TRANSLATION_DOMAIN,"$GETTEXT_PACKAGE", [library gettext package])

ALL_LINGUAS=""
AM_GLIB_GNU_GETTEXT

AC_CONFIG_FILES([
Makefile
po/Makefile.in
src/Makefile
])

AC_OUTPUT
