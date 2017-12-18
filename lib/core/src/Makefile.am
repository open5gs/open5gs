## Process this file with automake to produce Makefile.in

pkglib_LTLIBRARIES = libcore.la

libcore_la_SOURCES = \
	../include/core_aes_cmac.h ../include/core_aes.h ../include/core_cond.h \
	../include/core_debug.h ../include/core_errno.h ../include/core_file.h \
	../include/core_fsm.h ../include/core_general.h ../include/core.h \
	../include/core.h.in ../include/core_index.h ../include/core_lib.h \
	../include/core_list.h ../include/core_msgq.h ../include/core_mutex.h \
	../include/core_network.h ../include/core_param.h ../include/core_pkbuf.h \
	../include/core_pool.h ../include/core_queue.h ../include/core_ringbuf.h \
	../include/core_rwlock.h ../include/core_semaphore.h ../include/core_sha1.h \
	../include/core_sha1_hmac.h ../include/core_sha2.h ../include/core_sha2_hmac.h \
	../include/core_signal.h ../include/core_thread.h ../include/core_time.h \
	../include/core_timer.h ../include/core_tlv.h ../include/core_tlv_msg.h \
	../include/core_atomic.h ../include/core_portable.h \
	../include/core_version.h ../include/core_event.h ../include/core_hash.h \
	../include/3gpp_types.h \
	\
	debug.c version.c fsm.c msgq.c ringbuf.c timer.c tlv.c tlv_msg.c hash.c \
	aes.c aes_cmac.c sha1.c sha1_hmac.c sha2.c sha2_hmac.c misc.c event.c \
	3gpp_types.c \
	\
    ../include/arch/core_private_common.h \
    ../include/arch/unix/core_arch_file.h \
    ../include/arch/unix/core_arch_mutex.h \
    ../include/arch/unix/core_arch_semaphore.h \
    ../include/arch/unix/core_arch_thread.h \
	unix/start.c unix/errorcodes.c unix/pkbuf.c \
	unix/rand.c unix/time.c unix/file.c \
	unix/thread.c unix/signal.c \
	unix/atomic.c unix/cond.c unix/mutex.c unix/rwlock.c unix/semaphore.c \
	unix/socket.c unix/sockaddr.c unix/udp.c unix/tcp.c unix/tun.c \
	$(NULL)

if !USRSCTP
libcore_la_SOURCES += unix/sctp.c
endif

AM_LDFLAGS = \
	-version-info @LIBVERSION@ \
	$(NULL)

AM_CPPFLAGS = \
    -I$(top_srcdir)/lib/core/include/arch/@OSDIR@ \
    -I$(top_srcdir)/lib/core/include \
	$(NULL)

AM_CFLAGS = \
	-Wall -Werror \
	@OSCPPFLAGS@ \
	$(NULL)

MAINTAINERCLEANFILES = Makefile.in 
MOSTLYCLEANFILES = core *.stackdump
