## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = libcoreunix.la

libcoreunix_la_SOURCES = \
    ../../include/arch/core_private_common.h \
    ../../include/arch/unix/core_arch_file.h \
    ../../include/arch/unix/core_arch_mutex.h \
    ../../include/arch/unix/core_arch_semaphore.h \
    ../../include/arch/unix/core_arch_thread.h \
    debug.c cond.c file.c net_lib.c thread.c errorcodes.c mutex.c rwlock.c \
    time.c semaphore.c signal.c pkbuf.c rand.c atomic.c

AM_CPPFLAGS = \
    -I$(top_srcdir) \
    -I$(top_srcdir)/lib/core/include/arch/@OSDIR@ \
    -I$(top_srcdir)/lib/core/include

AM_CFLAGS = \
	-Wall -Werror @OSCPPFLAGS@

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = core *.stackdump
