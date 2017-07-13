dnl Process this file with autoconf to produce a configure script.
dnl
dnl This file is free software; as a special exception the author gives
dnl unlimited permission to copy and/or distribute it, with or without
dnl modifications, as long as this notice is preserved.
dnl
dnl This program is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
dnl implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AC_INIT([NEXTEPC], [0.1.0], [acetcom@gmail.com])

CORE_CONFIG_NICE(config.nice)

dnl Must come before AM_INIT_AUTOMAKE.
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.10 -Wall -Werror foreign])

# Minimum Autoconf version required.
AC_PREREQ(2.63)

# Where to generate output; srcdir location.
AC_CONFIG_HEADERS([config.h:config.in])dnl Keep filename to 8.3 for MS-DOS.
AC_CONFIG_SRCDIR([main.c])
AC_CANONICAL_HOST

case $host in
    *linux*)
        OSDIR="unix"
        OSCPPFLAGS="-DLINUX=1"
        ;;
    *-apple-darwin*)
        OSDIR="unix"
        OSPPCFLAGS="-DDARWIN -DSIGPROCMASK_SETS_THREAD_MASK"
        ;;
    *)
        OSDIR="unix"
        ;;
esac
AC_SUBST(OSCPPFLAGS)
AC_SUBST(OSDIR)

AH_TOP([
#ifndef __NEXTEPC_CONFIG_H__
#define __NEXTEPC_CONFIG_H__
/* need this, because some autoconf tests rely on this (e.g. stpcpy)
 * and it should be used for new programs  */
#define _GNU_SOURCE  1
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

dnl Checks for programs.
# We need a C compiler.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_AWK
AM_PROG_AR

dnl libtool
LT_INIT

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

AC_DEFINE_UNQUOTED([PACKAGE_VERSION_MAJOR],
        [`echo $PACKAGE_VERSION | sed 's/^\([[^\.]]\+\)\.\([[^\.]]\+\)\.\([[^\.]]\+\).*/\1/'`],
        [Major version of this package])
AC_DEFINE_UNQUOTED([PACKAGE_VERSION_MINOR],
        [`echo $PACKAGE_VERSION | sed 's/^\([[^\.]]\+\)\.\([[^\.]]\+\)\.\([[^\.]]\+\).*/\2/'`],
        [Minor version of this package])
AC_DEFINE_UNQUOTED([PACKAGE_VERSION_PATCHLEVEL],
        [`echo $PACKAGE_VERSION | sed 's/^\([[^\.]]\+\)\.\([[^\.]]\+\)\.\([[^\.]]\+\).*/\3/'`],
        [Patch version of this package])

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
    netinet/ip.h \
    netinet/sctp.h \
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

##########################################
#### Checks for typedefs, structures, ####
####  and compiler characteristics.   ####
##########################################

AC_C_BIGENDIAN

AC_CHECK_MEMBERS([struct tm.tm_gmtoff, struct tm.__tm_gmtoff],,,[
#include <sys/types.h>
#include <time.h>])

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
    inet_ntop \
    inet_pton \
    inet_aton \
    memmove \
    sigaction \
    sigwait \
    sigsuspend \
    stpcpy \
    strcasecmp \
    strtoul \
    stricmp \
    strerror \
    writev \
    utime \
    utimes \
    sem_timedwait \
    pthread_yield \
    sched_yield \
)

AC_CHECK_LIB([pthread], [pthread_mutex_init], [LIBS="${LIBS} -lpthread"])
AC_CHECK_LIB([sctp], [sctp_sendmsg], [LIBS="${LIBS} -lsctp"])
AC_CHECK_LIB([fdcore], [fd_core_initialize], [LIBS="${LIBS} -lfdcore"])
AC_CHECK_LIB([fdproto], [fd_libproto_init], [LIBS="${LIBS} -lfdproto"])
AC_CHECK_LIB([gnutls], [gnutls_global_init], [LIBS="${LIBS} -lgnutls"])

#####################
#### Conclusion. ####
#####################

AC_CONFIG_FILES([lib/core/include/core.h])
AC_CONFIG_FILES([lib/core/src/Makefile])
case $host in
   *)
       AC_CONFIG_FILES([lib/core/src/unix/Makefile])
       ;;
esac
AC_CONFIG_FILES([lib/core/test/Makefile])
AC_CONFIG_FILES([lib/core/Makefile])
AC_CONFIG_FILES([lib/logger/Makefile])
AC_CONFIG_FILES([lib/base/Makefile])
AC_CONFIG_FILES([lib/s1ap/asn1c/Makefile])
AC_CONFIG_FILES([lib/s1ap/Makefile])
AC_CONFIG_FILES([lib/nas/Makefile])
AC_CONFIG_FILES([lib/s6a/freeDiameter/Makefile])
AC_CONFIG_FILES([lib/s6a/Makefile])
AC_CONFIG_FILES([lib/gtp/Makefile])
AC_CONFIG_FILES([lib/Makefile])
AC_CONFIG_FILES([src/mme/Makefile])
AC_CONFIG_FILES([src/hss/Makefile])
AC_CONFIG_FILES([src/sgw/Makefile])
AC_CONFIG_FILES([src/pgw/Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([test/Makefile])
AC_CONFIG_FILES([support/nextepc.conf])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo "
NextEPC configuration
--------------------
version                 : ${PACKAGE_VERSION}
host                    : ${host}
source code location    : ${srcdir}
compiler                : ${CC}
compiler flags          : ${CFLAGS}
linker flags            : ${LDFLAGS} ${LIBS}
config file directory   : `eval echo \`echo ${sysconfdir}\``
log file directory      : `eval echo \`echo ${prefix}/var/log\``
"
