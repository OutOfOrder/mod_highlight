dnl Required initializer
AC_INIT(mod_highlight, 0.2.0)
OOO_CONFIG_NICE(config.nice)
AC_PREREQ(2.53)
AC_CONFIG_SRCDIR([src/mod_highlight.c])
AM_MAINTAINER_MODE
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER([include/mod_highlight_config.h:config.in])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_LD
AC_PROG_INSTALL
AM_PROG_LIBTOOL

AP_VERSION=2.0.40
CHECK_APACHE(,$AP_VERSION,
    :,:,
    AC_MSG_ERROR([*** Apache version $AP_VERSION not found!])
)

AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
if test "x$PKG_CONFIG" = "xno"; then
        AC_MSG_ERROR([You need to install pkg-config])
fi

MODULE_CFLAGS="${APXS_CFLAGS} ${AP_INCLUDES} ${APR_INCLUDES} ${APU_INCLUDES}"

AC_SUBST(MODULE_CFLAGS)

prefix=${AP_PREFIX}
AC_CONFIG_SUBDIRS(colorer)

COLORER_DIR=colorer
AC_SUBST(COLORER_DIR)

COLORER_LDFLAGS="-L\$(top_builddir)/colorer/src/libcolorer -lcolorer"
AC_SUBST(COLORER_LDFLAGS)

COLORER_CFLAGS="-I\$(top_srcdir)/colorer/src/shared"
AC_SUBST(COLORER_CFLAGS)

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

echo "---"
echo "Configuration summary for mod_highlight"
echo ""
echo "   * Apache modules directory = $AP_LIBEXECDIR"
echo ""
echo "---"
