dnl Process this file with autoconf to produce a configure script.
dnl
dnl This file is free software; as a special exception the author gives
dnl unlimited permission to copy and/or distribute it, with or without
dnl modifications, as long as this notice is preserved.
dnl
dnl This program is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
dnl implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AC_INIT([nextepc], [1.2.1], [acetcom@gmail.com])

AC_SUBST(LIBVERSION)
LIBVERSION=1:0:0

dnl Must come before AM_INIT_AUTOMAKE.
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.10 -Wall -Werror foreign])

# Where to generate output; srcdir location.
AC_CONFIG_HEADERS([include/freeDiameter/config.h])
AC_CANONICAL_HOST
case $host in
    *-freebsd*)
        AC_DEFINE_UNQUOTED([SCTP_USE_MAPPED_ADDRESSES],
                [1], [Disable SCTP])
        ;;
    *)
        ;;
esac

AH_TOP([
#ifndef FD_IS_CONFIG
#define FD_IS_CONFIG

#define _GNU_SOURCE  1

#ifdef __cplusplus
extern "C" {
#endif

])

AH_BOTTOM([
#define FD_PROJECT_COPYRIGHT "Copyright (c) 2008-2015, WIDE Project (www.wide.ad.jp) and NICT (www.nict.go.jp)"

#ifndef FD_DEFAULT_CONF_FILENAME
#define FD_DEFAULT_CONF_FILENAME "freeDiameter.conf"
#endif /* FD_DEFAULT_CONF_FILENAME */

/* Maximum number of hooks handlers that can be registered. Make this compilation option if needed */
#define FD_HOOK_HANDLE_LIMIT	5

#ifdef __cplusplus
}
#endif

#endif /* FD_IS_CONFIG */
])

dnl kernel style compile messages
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

dnl Checks CC and freinds
AC_PROG_MAKE_SET
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_YACC
AC_PROG_LEX
AM_PROG_AR
LT_INIT([dlopen pic-only disable-static])

dnl Checks for compile flag
dnl AX_CHECK_COMPILE_FLAG([-Wno-format-truncation],[CFLAGS="$CFLAGS -Wno-format-truncation"])

dnl Check Endian
AC_C_BIGENDIAN

#############################
#### From FreeDiameter CMAKE 
#############################
touch include/freeDiameter/version.h
rm -f libfdcore/fdd.tab.y
ln -s fdd.y libfdcore/fdd.tab.y
rm -f libfdcore/lex.fdd.l
ln -s fdd.l libfdcore/lex.fdd.l

AC_DEFINE_UNQUOTED([FD_PROJECT_BINARY],
        ["freeDiameterd"], [Project Binary])
AC_DEFINE_UNQUOTED([FD_PROJECT_NAME],
        ["freeDiameter"], [Project Name])
AC_DEFINE_UNQUOTED([FD_PROJECT_VERSION_MAJOR],
        [`echo $PACKAGE_VERSION | $SED 's/^\([[^\.]]\+\)\.\([[^\.]]\+\)\.\([[^\.]]\+\).*/\1/'`],
        [Major version of this package])
AC_DEFINE_UNQUOTED([FD_PROJECT_VERSION_MINOR],
        [`echo $PACKAGE_VERSION | $SED 's/^\([[^\.]]\+\)\.\([[^\.]]\+\)\.\([[^\.]]\+\).*/\2/'`],
        [Minor version of this package])
AC_DEFINE_UNQUOTED([FD_PROJECT_VERSION_REV],
        [`echo $PACKAGE_VERSION | $SED 's/^\([[^\.]]\+\)\.\([[^\.]]\+\)\.\([[^\.]]\+\).*/\3/'`],
        [Patch version of this package])
AC_DEFINE_UNQUOTED([FD_PROJECT_VERSION_API],
        [6],
        [API version of this package])

# adl_RECURSIVE_EVAL(VALUE, RESULT)
# =================================
# Interpolate the VALUE in loop until it doesn't change,
# and set the result to $RESULT.
# WARNING: It's easy to get an infinite loop with some unsane input.
# For example ${datadir} becomes ${datarootdir}, and then ${prefix}/share, and
# finally ${prefix} is replaced by the prefix.
AC_DEFUN([adl_RECURSIVE_EVAL],
[_lcl_receval="$1"
$2=`(test "x$prefix" = xNONE && prefix="$ac_default_prefix"
     test "x$exec_prefix" = xNONE && exec_prefix="${prefix}"
     _lcl_receval_old=''
     while test "[$]_lcl_receval_old" != "[$]_lcl_receval"; do
       _lcl_receval_old="[$]_lcl_receval"
       eval _lcl_receval="\"[$]_lcl_receval\""
     done
     echo "[$]_lcl_receval")`])
adl_RECURSIVE_EVAL(["${libdir}"], [LIB_DIR])
adl_RECURSIVE_EVAL(["${sysconfdir}"], [SYSCONF_DIR])

AC_SUBST(extlibdir)
extlibdir=${libdir}/nextepc/freeDiameter
AC_DEFINE_UNQUOTED([DEFAULT_CONF_PATH],
        ["${SYSCONF_DIR}/nextepc/freeDiameter"], [Default Configuration Path])
AC_DEFINE_UNQUOTED([DEFAULT_EXTENSIONS_PATH],
        ["${LIB_DIR}/nextepc/freeDiameter"], [Default Extensions Path])
AC_SUBST(PREFIX)

AC_CHECK_FUNCS([strndup])

AC_CHECK_DECL([ntohll],[AC_DEFINE([HAVE_NTOHLL], [1],
              [Define to 1 if you have ntohll.])])
AC_CHECK_DECL([AI_ADDRCONFIG],
              [AC_DEFINE([HAVE_AI_ADDRCONFIG], [1], [Define AI_ADDRCONFIG])], [], [[#include <netdb.h>]])

AC_SEARCH_LIBS([clock_gettime], [rt],
             [AC_DEFINE([HAVE_CLOCK_GETTIME], [1], [Define clock_gettime])])
AC_SEARCH_LIBS([dlopen], [dl dld])
AC_SEARCH_LIBS([pthread_barrier_wait], [pthread],
             [AC_DEFINE([HAVE_PTHREAD_BAR], [1],
                        [Define to 1 if you have pthread_barrier_wait in libpthread])],
             [])
AC_SEARCH_LIBS([sctp_sendmsg], [sctp], [have_sctp_lib=yes])
if test x$have_sctp_lib != xyes; then
    AC_DEFINE([DISABLE_SCTP], [1], [Disable SCTP])
fi
AM_CONDITIONAL([SCTP], [test x$have_sctp_lib = xyes])

AC_SEARCH_LIBS([gnutls_global_init], [gnutls],, [have_gnutls_lib=no])
if test x$have_gnutls_lib == xno; then
  AC_MSG_ERROR([You must install the GnuTLS libraries and development headers to enable GnuTLS support.])
fi
AC_SEARCH_LIBS([gnutls_hash], [gnutls],
             [AC_DEFINE(GNUTLS_VERSION_210, 1,
              [Define to 1 if you have gnutls 2.10 installed])], [])
AC_SEARCH_LIBS([gnutls_transport_set_vec_push_function], [gnutls], 
             [AC_DEFINE(GNUTLS_VERSION_212, 1,
              [Define to 1 if you have gnutls 2.10 installed])], [])
AC_SEARCH_LIBS([gnutls_x509_trust_list_verify_crt], [gnutls],
             [AC_DEFINE(GNUTLS_VERSION_300, 1,
              [Define to 1 if you have gnutls 3.0 installed])], [])
AC_SEARCH_LIBS([gnutls_handshake_set_timeout], [gnutls],
             [AC_DEFINE(GNUTLS_VERSION_310, 1,
              [Define to 1 if you have gnutls 3.1 installed])], [])

AC_SEARCH_LIBS([gcry_control], [gcrypt],, [have_gcrypt_lib=no])
if test x$have_gcrypt_lib == xno; then
  AC_MSG_ERROR([You must install the Libgcrypt libraries and development headers to enable Libgcrypt support.])
fi
AC_SEARCH_LIBS([idna_strerror], [idn],, [have_idn_lib=no])
if test x$have_idn_lib == xno; then
  AC_MSG_ERROR([You must install the GNU Libidn libraries and development headers to enable GNU Libidn support.])
fi

AC_CACHE_CHECK([whether sctp_connectx function accepts 4 arguments],
               [ap_cv_sctp_connectx],
[AC_TRY_COMPILE(
 [#include <unistd.h>
#include <netinet/sctp.h>],
 [sctp_connectx(0, NULL, 0, NULL);],
 [ap_cv_sctp_connectx=yes], [ap_cv_sctp_connectx=no])
])

if test "$ap_cv_sctp_connectx" = "yes"; then
    AC_DEFINE([SCTP_CONNECTX_4_ARGS], [1],
              [Define 1 if sctp_connectx function accepts 4 arguments])
fi

#####################
#### Conclusion. ####
#####################

AC_CONFIG_FILES([include/freeDiameter/freeDiameter-host.h:include/freeDiameter/freeDiameter-host.hin])
AC_CONFIG_FILES([libfdproto/Makefile])
AC_CONFIG_FILES([libfdcore/Makefile])
AC_CONFIG_FILES([extensions/dbg_msg_dumps/Makefile])
AC_CONFIG_FILES([extensions/dict_rfc5777/Makefile])
AC_CONFIG_FILES([extensions/dict_mip6i/Makefile])
AC_CONFIG_FILES([extensions/dict_nasreq/Makefile])
AC_CONFIG_FILES([extensions/dict_nas_mipv6/Makefile])
AC_CONFIG_FILES([extensions/dict_dcca/Makefile])
AC_CONFIG_FILES([extensions/dict_dcca_3gpp/Makefile])
AC_CONFIG_FILES([extensions/dict_s6a/Makefile])
AC_CONFIG_FILES([extensions/Makefile])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo "
FreeDiameter configuration
--------------------
version                 : ${PACKAGE_VERSION}
host                    : ${host}
source code location    : ${srcdir}
compiler                : ${CC}
compiler flags          : ${CFLAGS}
linker flags            : ${LDFLAGS} ${LIBS}
config directory        : ${SYSCONF_DIR}/nextepc/freeDiameter
extensions directory    : ${LIB_DIR}/nextepc
"
