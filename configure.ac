#                                                       -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>

# This file is part of Open5GS.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

AC_INIT([NextEPC],
	m4_esyscmd([build/git-version-gen .tarball-version]),
	[acetcom@gmail.com])

AC_CONFIG_AUX_DIR([build])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_TESTDIR(tests)
AC_CANONICAL_HOST

AM_INIT_AUTOMAKE([1.10 -Wall -Werror foreign subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_CONFIG_HEADERS([lib/base/nextepc-config.h])
AC_CONFIG_SRCDIR([main.c])

LO_DEV="lo0"
case $host in
    *linux*)
        OSCPPFLAGS="-DLINUX=1"
        IPFW_CPPFLAGS="-DNEED_SYSCTLBYNAME -DNEED_SIN_LEN"
        LO_DEV="lo"
        ;;
    *-apple-darwin*)
        OSCPPFLAGS="-DDARWIN -DSIGPROCMASK_SETS_THREAD_MASK"
        ;;
    *)
        ;;
esac
AC_SUBST(OSCPPFLAGS)
AC_SUBST(IPFW_CPPFLAGS)
AC_SUBST(LO_DEV)

AH_TOP([
#ifndef __NEXTEPC_CONFIG_H__
#define __NEXTEPC_CONFIG_H__
/* need this, because some autoconf tests rely on this (e.g. stpcpy)
 * and it should be used for new programs  */
#define _DEFAULT_SOURCE 1
#define _BSD_SOURCE  	1
])

AH_BOTTOM([
#endif /* __NEXTEPC_CONFIG_H__ */
])

AH_VERBATIM([_REENTRANT],
[/* To allow the use of core in multithreaded programs we have to use
    special features from the library. */
#ifndef _REENTRANT
# define _REENTRANT 1
#endif
])

dnl Checks CC and freinds
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AM_PROG_AR
LT_INIT

AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
if test "x$PKG_CONFIG" = "xno"; then
	AC_MSG_ERROR([You need to install pkg-config])
fi
PKG_PROG_PKG_CONFIG([0.20])

dnl Checks for compile flag
AX_CHECK_COMPILE_FLAG([-Wno-unused-result], [CFLAGS="$CFLAGS -Wno-unused-result"])

##################################
#### Checks for Directories. #####
##################################

AX_RECURSIVE_EVAL(["${bindir}"], [BIN_DIR])
AX_RECURSIVE_EVAL(["${libdir}"], [LIB_DIR])
AX_RECURSIVE_EVAL(["${sysconfdir}"], [SYSCONF_DIR])
AX_RECURSIVE_EVAL(["${localstatedir}"], [LOCALSTATE_DIR])
AC_SUBST(BIN_DIR)
AC_SUBST(LIB_DIR)
AC_SUBST(SYSCONF_DIR)
AC_SUBST(LOCALSTATE_DIR)

#######################################
#### Checks for library functions. ####
#######################################

AC_SEARCH_LIBS(gethostbyname, nsl)
AC_SEARCH_LIBS(gethostname, nsl)
AC_SEARCH_LIBS(socket, socket)
AC_SEARCH_LIBS(pthread_barrier_wait, pthread)
AC_SEARCH_LIBS(gnutls_global_set_log_level, gnutls)

AC_CHECK_FILE([lib/ogslib/configure.ac], [have_srclib=yes], [have_srclib=no])
if test x$have_srclib == xyes; then
  AC_CONFIG_SUBDIRS([lib/ogslib])
  AC_SUBST(OGSCORE_CFLAGS, '-I$(top_srcdir)/lib/ogslib/src')
  AC_SUBST(OGSCORE_LIBS, '$(top_srcdir)/lib/ogslib/src/core/libogscore-1.0.la')
  AC_SUBST(OGSCRYPT_CFLAGS, '-I$(top_srcdir)/lib/ogslib/src')
  AC_SUBST(OGSCRYPT_LIBS, '$(top_srcdir)/lib/ogslib/src/crypt/libogscrypt-1.0.la')
else
  PKG_CHECK_MODULES(OGSCORE, ogscore-1.0 >= 1.1.0)
  PKG_CHECK_MODULES(OGSCRYPT, ogscrypt-1.0 >= 1.1.0)
fi
AM_CONDITIONAL([WITH_SRCLIB], [test x$have_srclib = xyes])

AC_SEARCH_LIBS([sctp_sendmsg], [sctp], [have_sctp=yes], [have_sctp=no])
if test x$have_sctp == xno; then
  AC_SEARCH_LIBS([usrsctp_init], [usrsctp], [have_usrsctp=yes], [have_usrsctp=no])
  if test x$have_usrsctp == xno; then
    AC_MSG_ERROR([You must install the SCTP libraries and development headers to enable SCTP support.])
  else
    AC_DEFINE(HAVE_USRSCTP, 1,
        [Define if your system supports the usrsctp system calls])
  fi 
fi
AM_CONDITIONAL([WITH_USRSCTP], [test x$have_usrsctp = xyes])

AC_CHECK_HEADERS( \
    netinet/sctp.h \
)

AC_CHECK_HEADERS(netinet/ip.h netinet/ip6.h net/route.h,,,[[
#include <sys/types.h>
#if HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#include <netinet/in.h>
#if HAVE_NETINET_IN_SYSTM_H
#include <netinet/in_systm.h>
#endif
]])

AC_CHECK_HEADERS(netinet/ip_icmp.h netinet/icmp6.h,,,[[
#include <sys/types.h>
#if HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#include <netinet/in.h>
#if HAVE_NETINET_IN_SYSTM_H
#include <netinet/in_systm.h>
#endif
#include <netinet/ip.h>
]])

PKG_CHECK_MODULES([YAML], yaml-0.1 >= 0.1.4)
PKG_CHECK_MODULES([MONGOC], libmongoc-1.0 >= 1.3.1)
FREEDIAMETER_DIR=freeDiameter-1.2.1
AC_SUBST(FREEDIAMETER_DIR)

#####################
#### Conclusion. ####
#####################

AC_CONFIG_SUBDIRS([lib/freeDiameter-1.2.1])

AC_CONFIG_FILES([lib/base/Makefile])
AC_CONFIG_FILES([lib/asn1c/Makefile])
AC_CONFIG_FILES([lib/nas/Makefile])
AC_CONFIG_FILES([lib/fd/Makefile])
AC_CONFIG_FILES([lib/gtp/Makefile])
AC_CONFIG_FILES([lib/ipfw/Makefile])
AC_CONFIG_FILES([lib/Makefile])
AC_CONFIG_FILES([src/app/Makefile])
AC_CONFIG_FILES([src/mme/Makefile])
AC_CONFIG_FILES([src/hss/Makefile])
AC_CONFIG_FILES([src/sgw/Makefile])
AC_CONFIG_FILES([src/pgw/Makefile])
AC_CONFIG_FILES([src/pcrf/Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([support/config/nextepc.conf])
AC_CONFIG_FILES([support/config/mme.conf])
AC_CONFIG_FILES([support/config/sgw.conf])
AC_CONFIG_FILES([support/config/pgw.conf])
AC_CONFIG_FILES([support/config/hss.conf])
AC_CONFIG_FILES([support/config/pcrf.conf])
AC_CONFIG_FILES([support/config/Makefile])
AC_CONFIG_FILES([support/freeDiameter/mme.conf])
AC_CONFIG_FILES([support/freeDiameter/pgw.conf])
AC_CONFIG_FILES([support/freeDiameter/hss.conf])
AC_CONFIG_FILES([support/freeDiameter/pcrf.conf])
AC_CONFIG_FILES([support/freeDiameter/Makefile])
AC_CONFIG_FILES([support/systemd/nextepc-mmed.service])
AC_CONFIG_FILES([support/systemd/nextepc-sgwd.service])
AC_CONFIG_FILES([support/systemd/nextepc-pgwd.service])
AC_CONFIG_FILES([support/systemd/nextepc-hssd.service])
AC_CONFIG_FILES([support/systemd/nextepc-pcrfd.service])
AC_CONFIG_FILES([support/systemd/Makefile])
AC_CONFIG_FILES([support/logrotate/nextepc])
AC_CONFIG_FILES([support/logrotate/Makefile])
AC_CONFIG_FILES([support/newsyslog/nextepc.conf])
AC_CONFIG_FILES([support/newsyslog/Makefile])
AC_CONFIG_FILES([support/Makefile])
AC_CONFIG_FILES([tests/sample-simple.conf])
AC_CONFIG_FILES([tests/sample-complex.conf])
AC_CONFIG_FILES([tests/sample-volte.conf])
AC_CONFIG_FILES([tests/sample-csfb.conf])
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

AC_MSG_NOTICE([NextEPC configuration:

  version:              ${PACKAGE_VERSION}
  compiler:             ${CC}
  compiler flags:       ${CFLAGS} ${OGSLIB_CFLAGS} ${MONGOC_CFLAGS}
  linker flags:         ${LDFLAGS} ${LIBS} ${OGSCORE_LIBS} ${OGSCRYPT_LIBS} ${MONGOC_LIBS}
  path prefix:          ${prefix}
  binary file:          ${BIN_DIR}/nextepc-epcd
  library directory:    ${LIB_DIR}/nextepc
  configuration prefix: ${SYSCONF_DIR}/nextepc
  configuration file:   ${SYSCONF_DIR}/nextepc/nextepc.conf
  pid file:             ${LOCALSTATE_DIR}/run/nextepc.pid
  logging file:         ${LOCALSTATE_DIR}/log/nextepc.log
])
