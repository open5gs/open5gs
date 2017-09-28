dnl Process this file with autoconf to produce a configure script.
dnl
dnl This file is free software; as a special exception the author gives
dnl unlimited permission to copy and/or distribute it, with or without
dnl modifications, as long as this notice is preserved.
dnl
dnl This program is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
dnl implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AC_INIT([freeDiameter], [1.2.1], [acetcom@gmail.com])

dnl Must come before AM_INIT_AUTOMAKE.
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.10 -Wall -Werror foreign])

# Minimum Autoconf version required.
AC_PREREQ(2.63)

# Where to generate output; srcdir location.
AC_CONFIG_HEADERS([include/freeDiameter/freeDiameter-host.h:include/freeDiameter/freeDiameter-host.hin])dnl Keep filename to 8.3 for MS-DOS.
AC_CANONICAL_HOST

AH_TOP([
#ifndef FD_IS_CONFIG
#define FD_IS_CONFIG

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

dnl Checks CC and freinds
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_AWK
AC_PROG_SED
AC_PROG_YACC
AC_PROG_LEX
AM_PROG_AR

dnl libtool
LT_INIT

dnl Check Endian
AC_C_BIGENDIAN

#############################
#### From FreeDiameter CMAKE 
#############################
AC_DEFINE_UNQUOTED([FD_PROJECT_BINARY],
        ["freeDiameterd"], [Project Binary])
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

PREFIX=$ac_default_prefix
if test "x$prefix" != "xNONE"; then
    PREFIX=$prefix
fi
AC_DEFINE_UNQUOTED([DEFAULT_CONF_PATH],
        ["${PREFIX}/etc/freeDiameter"], [Default Configuration Path])
AC_DEFINE_UNQUOTED([DEFAULT_EXTENSIONS_PATH],
        ["${PREFIX}/lib/freeDiameter"], [Default Configuration Path])
AC_SUBST(PREFIX)


AC_CHECK_FUNCS([strndup])
AC_CHECK_DECL([ntohll],[AC_DEFINE([HAVE_NTOHLL], [1],
              [Define to 1 if you have ntohll.])])
AC_CHECK_LIB([rt], [clock_gettime],
             [AC_DEFINE([HAVE_CLOCK_GETTIME], [1], [Define clock_gettime])])
AC_CHECK_LIB([pthread], [pthread_barrier_wait],
             [AC_DEFINE([HAVE_PTHREAD_BAR], [1],
                        [Define to 1 if you have pthread_barrier_wait in libpthread])],
             [])
AC_CHECK_DECL([AI_ADDRCONFIG],
              [AC_DEFINE([HAVE_AI_ADDRCONFIG], [1], [Define AI_ADDRCONFIG])], [], [[#include <netdb.h>]])
AC_MSG_CHECKING(if sctp_connectx accepts 4 arguments)
AC_LINK_IFELSE([AC_LANG_SOURCE([
                [#include <unistd.h>
                 #include <netinet/sctp.h>
                 int main() { return sctp_connectx(0, NULL, 0, NULL); }]])
], [AC_DEFINE([SCTP_CONNECTX_4_ARGS], [1],
              [Define to 1 if sctp_connectx function accepts 4 arguments])])
AC_CHECK_LIB([gnutls], [gnutls_hash],
             [AC_DEFINE(GNUTLS_VERSION_210, 1,
              [Define to 1 if you have gnutls 2.10 installed])], [])
AC_CHECK_LIB([gnutls], [gnutls_transport_set_vec_push_function],
             [AC_DEFINE(GNUTLS_VERSION_212, 1,
              [Define to 1 if you have gnutls 2.10 installed])], [])
AC_CHECK_LIB([gnutls], [gnutls_x509_trust_list_verify_crt],
             [AC_DEFINE(GNUTLS_VERSION_300, 1,
              [Define to 1 if you have gnutls 3.0 installed])], [])
AC_CHECK_LIB([gnutls], [gnutls_handshake_set_timeout],
             [AC_DEFINE(GNUTLS_VERSION_310, 1,
              [Define to 1 if you have gnutls 3.1 installed])], [])

#####################
#### Conclusion. ####
#####################

AC_CONFIG_FILES([libfdcore/Makefile])
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
default conf path       : `eval echo \`echo ${prefix}/etc/freeDiameter\``
default extention path  : `eval echo \`echo ${prefix}/lib/freeDiameter\``
"
