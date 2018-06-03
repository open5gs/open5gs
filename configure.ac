dnl Process this file with autoconf to produce a configure script.
dnl
dnl This file is free software; as a special exception the author gives
dnl unlimited permission to copy and/or distribute it, with or without
dnl modifications, as long as this notice is preserved.
dnl
dnl This program is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
dnl implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AC_INIT([NextEPC], [0.3.9], [acetcom@gmail.com])

AC_SUBST(LIBVERSION)
LIBVERSION=1:0:0

CORE_CONFIG_NICE(config.nice)

dnl Must come before AM_INIT_AUTOMAKE.
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_TESTDIR(test)
AM_INIT_AUTOMAKE([1.10 -Wall -Werror foreign subdir-objects])

# Where to generate output; srcdir location.
AC_CONFIG_HEADERS([config.h:config.in])dnl Keep filename to 8.3 for MS-DOS.
AC_CONFIG_SRCDIR([main.c])
AC_CANONICAL_HOST

LO_DEV="lo0"
case $host in
    *linux*)
        OSDIR="unix"
        OSCPPFLAGS="-DLINUX=1"
        IPFW_CPPFLAGS="-DNEED_SYSCTLBYNAME -DNEED_SIN_LEN"
        LO_DEV="lo"
        ;;
    *-apple-darwin*)
        OSDIR="unix"
        OSCPPFLAGS="-DDARWIN -DSIGPROCMASK_SETS_THREAD_MASK"
        ;;
    *)
        OSDIR="unix"
        ;;
esac
AC_SUBST(OSCPPFLAGS)
AC_SUBST(OSDIR)
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

dnl kernel style compile messages
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

dnl Checks CC and freinds
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AM_PROG_AR
LT_INIT([pic-only disable-static])

AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
if test "x$PKG_CONFIG" = "xno"; then
	AC_MSG_ERROR([You need to install pkg-config])
fi
PKG_PROG_PKG_CONFIG([0.20])

dnl Checks for compile flag
AX_CHECK_COMPILE_FLAG([-Wno-unused-result], [CFLAGS="$CFLAGS -Wno-unused-result"])

dnl Checks for pointer size
AC_CHECK_SIZEOF(void*, 4)

dnl Checks for integer size
AC_CHECK_SIZEOF(char, 1)
AC_CHECK_SIZEOF(int, 4)
AC_CHECK_SIZEOF(long, 4)
AC_CHECK_SIZEOF(short, 2)
AC_CHECK_SIZEOF(long long, 8)

if test "$ac_cv_sizeof_short" = "2"; then
    short_value=short
fi
if test "$ac_cv_sizeof_int" = "4"; then
    int_value=int
fi
# Now we need to find what c_int64_t (sizeof == 8) will be.
# The first match is our preference.
if test "$ac_cv_sizeof_int" = "8"; then
    int64_literal='#define C_INT64_C(val) (val)'
    uint64_literal='#define C_UINT64_C(val) (val##U)'
    int64_t_fmt='#define C_INT64_T_FMT "d"'
    uint64_t_fmt='#define C_UINT64_T_FMT "u"'
    uint64_t_hex_fmt='#define C_UINT64_T_HEX_FMT "x"'
    int64_value="int"
    long_value=int
    int64_strfn="strtoi"
elif test "$ac_cv_sizeof_long" = "8"; then
    int64_literal='#define C_INT64_C(val) (val##L)'
    uint64_literal='#define C_UINT64_C(val) (val##UL)'
    int64_t_fmt='#define C_INT64_T_FMT "ld"'
    uint64_t_fmt='#define C_UINT64_T_FMT "lu"'
    uint64_t_hex_fmt='#define C_UINT64_T_HEX_FMT "lx"'
    int64_value="long"
    long_value=long
    int64_strfn="strtol"
elif test "$ac_cv_sizeof_long_long" = "8"; then
    int64_literal='#define C_INT64_C(val) (val##LL)'
    uint64_literal='#define C_UINT64_C(val) (val##ULL)'
    # Linux, Solaris, FreeBSD all support ll with printf.
    # BSD 4.4 originated 'q'.  Solaris is more popular and
    # doesn't support 'q'.  Solaris wins.  Exceptions can
    # go to the OS-dependent section.
    int64_t_fmt='#define C_INT64_T_FMT "lld"'
    uint64_t_fmt='#define C_UINT64_T_FMT "llu"'
    uint64_t_hex_fmt='#define C_UINT64_T_HEX_FMT "llx"'
    int64_value="long long"
    long_value="long long"
    int64_strfn="strtoll"
elif test "$ac_cv_sizeof_longlong" = "8"; then
    int64_literal='#define C_INT64_C(val) (val##LL)'
    uint64_literal='#define C_UINT64_C(val) (val##ULL)'
    int64_t_fmt='#define C_INT64_T_FMT "qd"'
    uint64_t_fmt='#define C_UINT64_T_FMT "qu"'
    uint64_t_hex_fmt='#define C_UINT64_T_HEX_FMT "qx"'
    int64_value="__int64"
    long_value="__int64"
    int64_strfn="strtoll"
else
    # int64_literal may be overriden if your compiler thinks you have
    # a 64-bit value but CORE does not agree.
    AC_ERROR([could not detect a 64-bit integer type])
fi

AC_SUBST(short_value)
AC_SUBST(int_value)
AC_SUBST(long_value)
AC_SUBST(int64_value)
AC_SUBST(int64_t_fmt)
AC_SUBST(uint64_t_fmt)
AC_SUBST(uint64_t_hex_fmt)
AC_SUBST(int64_literal)
AC_SUBST(uint64_literal)

AC_CHECK_SIZEOF(pid_t, 8)

if test "$ac_cv_sizeof_pid_t" = "$ac_cv_sizeof_short"; then
    pid_t_fmt='#define C_PID_T_FMT "hd"'
elif test "$ac_cv_sizeof_pid_t" = "$ac_cv_sizeof_int"; then
    pid_t_fmt='#define C_PID_T_FMT "d"'
elif test "$ac_cv_sizeof_pid_t" = "$ac_cv_sizeof_long"; then
    pid_t_fmt='#define C_PID_T_FMT "ld"'
elif test "$ac_cv_sizeof_pid_t" = "$ac_cv_sizeof_long_long"; then
    pid_t_fmt='#define C_PID_T_FMT APR_INT64_T_FMT'
else
    pid_t_fmt='#error Can not determine the proper size for pid_t'
fi

case $host in
   *-solaris*)
       if test "$ac_cv_sizeof_long" = "8"; then
         pid_t_fmt='#define C_PID_T_FMT "d"'
       else
         pid_t_fmt='#define C_PID_T_FMT "ld"'
       fi
       ;;
esac

AC_SUBST(pid_t_fmt)

AC_DEFINE_UNQUOTED([PACKAGE_VERSION_MAJOR],
        [`echo $PACKAGE_VERSION | $SED 's/^\([[^\.]]\+\)\.\([[^\.]]\+\)\.\([[^\.]]\+\).*/\1/'`],
        [Major version of this package])
AC_DEFINE_UNQUOTED([PACKAGE_VERSION_MINOR],
        [`echo $PACKAGE_VERSION | $SED 's/^\([[^\.]]\+\)\.\([[^\.]]\+\)\.\([[^\.]]\+\).*/\2/'`],
        [Minor version of this package])
AC_DEFINE_UNQUOTED([PACKAGE_VERSION_PATCHLEVEL],
        [`echo $PACKAGE_VERSION | $SED 's/^\([[^\.]]\+\)\.\([[^\.]]\+\)\.\([[^\.]]\+\).*/\3/'`],
        [Patch version of this package])

##################################
#### Checks for Directories. #####
##################################

adl_RECURSIVE_EVAL(["${bindir}"], [BIN_DIR])
adl_RECURSIVE_EVAL(["${libdir}"], [LIB_DIR])
adl_RECURSIVE_EVAL(["${sysconfdir}"], [SYSCONF_DIR])
adl_RECURSIVE_EVAL(["${localstatedir}"], [LOCALSTATE_DIR])
AC_SUBST(BIN_DIR)
AC_SUBST(LIB_DIR)
AC_SUBST(SYSCONF_DIR)
AC_SUBST(LOCALSTATE_DIR)

##################################
#### Checks for header files. ####
##################################

AC_HEADER_STDC
AC_CHECK_HEADERS( \
    arpa/inet.h \
    ctype.h \
    errno.h \
    fcntl.h \
    ifaddrs.h \
    limits.h \
    netdb.h \
    pthread.h \
    regex.h \
    semaphore.h \
    signal.h \
    stdarg.h \
    stdio.h \
    stdint.h \
    stdlib.h \
    string.h \
    strings.h \
    time.h \
    unistd.h \
    net/if_dl.h \
    net/if.h \
    netinet/ether.h \
    netinet/in.h \
    netinet/in_systm.h \
    netinet/udp.h \
    netinet/tcp.h \
    netinet/sctp.h \
    usrsctp.h \
    sys/ioctl.h \
    sys/param.h \
    sys/socket.h \
    sys/stat.h \
    sys/syslimits.h \
    sys/types.h \
    sys/time.h \
    sys/wait.h \
    sys/uio.h \
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

##########################################
#### Checks for typedefs, structures, ####
####  and compiler characteristics.   ####
##########################################

AC_C_BIGENDIAN

AC_CHECK_MEMBERS([struct tm.tm_gmtoff, struct tm.__tm_gmtoff],,,[
#include <sys/types.h>
#include <time.h>])

AC_CHECK_MEMBER(struct sockaddr.sa_len,
                AC_DEFINE(HAVE_SA_LEN, 1, [Define this if your stack has sa_len in sockaddr struct.]),,
                [#ifdef HAVE_SYS_TYPES_H
                 #include <sys/types.h>
                 #endif
                 #include <sys/socket.h>])

AC_CHECK_MEMBER(struct sockaddr_in.sin_len,
                AC_DEFINE(HAVE_SIN_LEN, 1, [Define this if your IPv4 has sin_len in sockaddr_in struct.]),,
                [#ifdef HAVE_SYS_TYPES_H
                 #include <sys/types.h>
                 #endif
                 #include <netinet/in.h>])

AC_CHECK_MEMBER(struct sockaddr_in6.sin6_len,
                AC_DEFINE(HAVE_SIN6_LEN, 1, [Define this if your IPv6 has sin6_len in sockaddr_in6 struct.]),,
                [#ifdef HAVE_SYS_TYPES_H
                 #include <sys/types.h>
                 #endif
                 #include <netinet/in.h>])

AC_CHECK_MEMBER(struct sockaddr_conn.sconn_len,
                AC_DEFINE(HAVE_SCONN_LEN, 1, [Define this if your userland stack has sconn_len in sockaddr_conn struct.]),,
                [#include "usrsctplib/usrsctp.h"])

AC_MSG_CHECKING(for socklen_t)
AC_TRY_COMPILE([#ifdef HAVE_SYS_TYPES_H
                #include <sys/types.h>
                #endif
                #include <sys/socket.h>],
               [socklen_t x; x = 1; return ((int)x);],
               [AC_MSG_RESULT(yes)],
               [AC_MSG_RESULT(int)
                AC_DEFINE(socklen_t, int, [Define a type for socklen_t.])])

AC_CHECK_FILE(/dev/random,
    AC_DEFINE([HAVE_DEV_RANDOM], [1],
        [Define to 1 if you have the /dev/random file.]))

AC_CACHE_CHECK([whether the compiler provides atomic builtins], [ap_cv_atomic_builtins],
[AC_TRY_RUN([
int main()
{
    unsigned long val = 1010, tmp, *mem = &val;

    if (__sync_fetch_and_add(&val, 1010) != 1010 || val != 2020)
        return 1;

    tmp = val;

    if (__sync_fetch_and_sub(mem, 1010) != tmp || val != 1010)
        return 1;

    if (__sync_sub_and_fetch(&val, 1010) != 0 || val != 0)
        return 1;

    tmp = 3030;

    if (__sync_val_compare_and_swap(mem, 0, tmp) != 0 || val != tmp)
        return 1;

    if (__sync_lock_test_and_set(&val, 4040) != 3030)
        return 1;

    mem = &tmp;

    if (__sync_val_compare_and_swap(&mem, &tmp, &val) != &tmp)
        return 1;

    __sync_synchronize();

    if (mem != &val)
        return 1;

    return 0;
}], [ap_cv_atomic_builtins=yes], [ap_cv_atomic_builtins=no], [ap_cv_atomic_builtins=no])])

if test "$ap_cv_atomic_builtins" = "yes"; then
    AC_DEFINE(HAVE_ATOMIC_BUILTINS, 1, [Define if compiler provides atomic builtins])
fi

#######################################
#### Checks for library functions. ####
#######################################

AC_FUNC_VPRINTF
AC_CHECK_FUNCS(\
    atexit \
    gettimeofday \
    memmove \
    strerror \
    inet_ntop inet_pton inet_aton \
    sigaction sigwait sigsuspend \
    stpcpy strcasecmp strtoul stricmp \
    writev \
    utime utimes sem_timedwait \
    pthread_yield sched_yield \
    getenv putenv setenv unsetenv \
)

AC_SEARCH_LIBS(gethostbyname, nsl)
AC_SEARCH_LIBS(gethostname, nsl)
AC_SEARCH_LIBS(socket, socket)
AC_SEARCH_LIBS(pthread_barrier_wait, pthread)
AC_SEARCH_LIBS(gnutls_global_set_log_level, gnutls)

AC_SEARCH_LIBS([sctp_sendmsg], [sctp], [have_sctp_lib=yes], [have_sctp_lib=no])
if test x$have_sctp_lib == xno; then
  AC_SEARCH_LIBS([usrsctp_init], [usrsctp], [have_usrsctp_lib=yes], [have_usrsctp_lib=no])
  if test x$have_usrsctp_lib == xno; then
    AC_MSG_ERROR([You must install the SCTP libraries and development headers to enable SCTP support.])
  else
    AC_DEFINE([USE_USRSCTP], [1], [Define to 1 if you have the usrsctp library.])
  fi 
fi
AM_CONDITIONAL([USRSCTP], [test x$have_usrsctp_lib = xyes])

PKG_CHECK_MODULES([YAML], yaml-0.1 >= 0.1.4)
PKG_CHECK_MODULES([MONGOC], libmongoc-1.0 >= 1.3.1)
FREEDIAMETER_DIR=freeDiameter-1.2.1
AC_SUBST(FREEDIAMETER_DIR)

#####################
#### Conclusion. ####
#####################

AC_CONFIG_SUBDIRS([lib/freeDiameter-1.2.1])

AC_CONFIG_FILES([lib/core/include/core.h])
AC_CONFIG_FILES([lib/core/src/Makefile])
AC_CONFIG_FILES([lib/core/test/Makefile])
AC_CONFIG_FILES([lib/core/Makefile])
AC_CONFIG_FILES([lib/s1ap/asn1c/Makefile])
AC_CONFIG_FILES([lib/s1ap/Makefile])
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
AC_CONFIG_FILES([test/sample.conf])
AC_CONFIG_FILES([test/sample-volte.conf])
AC_CONFIG_FILES([test/Makefile])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo "
NextEPC configuration
--------------------
version                 : ${PACKAGE_VERSION}
host                    : ${host}
source code location    : ${srcdir}
compiler                : ${CC}
compiler flags          : ${CFLAGS} ${YAML_CFLAGS} ${MONGOC_CFLAGS} 
linker flags            : ${LDFLAGS} ${LIBS} ${YAML_LIBS} ${MONGOC_LIBS}
bin directory           : ${BIN_DIR}
lib directory           : ${LIB_DIR}/nextepc
config directory        : ${SYSCONF_DIR}/nextepc
log directory           : ${LOCALSTATE_DIR}/log/nextepc
"
