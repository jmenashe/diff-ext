# Process this file with autoconf to produce a configure script.

AC_INIT(configure.in)
AM_INIT_AUTOMAKE(setup, 0.1)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC

pkg_modules="gtk+-2.0 >= 2.0.0 gconf-2.0 >= 2.14.0"
PKG_CHECK_MODULES(SETUP, [$pkg_modules])
AC_SUBST(SETUP_CFLAGS)
AC_SUBST(SETUP_LIBS)

GETTEXT_PACKAGE=setup
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package.])

dnl Add the languages which your application supports here.
ALL_LINGUAS=""
AM_GLIB_GNU_GETTEXT

AC_OUTPUT([
Makefile
src/Makefile
po/Makefile.in
])

