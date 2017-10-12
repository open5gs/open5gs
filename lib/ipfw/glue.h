/*
 * Copyright (c) 2009 Luigi Rizzo, Marta Carbone, Universita` di Pisa
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*
 * $Id: glue.h 8327 2011-03-22 17:01:35Z marta $
 *
 * glue code to adapt the FreeBSD version to linux and windows,
 * userland and kernel.
 * This is included before any other headers, so we do not have
 * a chance to override any #define that should appear in other
 * headers.
 * First handle headers for userland and kernel. Then common code
 * (including headers that require a specific order of inclusion),
 * then the user- and kernel- specific parts.
 */
 
#ifndef _GLUE_H
#define	_GLUE_H

/*
 * common definitions to allow portability
 */
#ifndef __FBSDID
#define __FBSDID(x)	struct __hack
#endif  /* FBSDID */

#include <stdint.h>	/* linux needs it in addition to sys/types.h */
#include <sys/types.h>	/* for size_t */

#define true 1		/* stdbool */
#ifdef _KERNEL		/* prevent a warning */
#undef _KERNEL
#include <sys/ioctl.h>
#include <sys/time.h>
#include <errno.h>	/* we want errno */
#define _KERNEL
#else
#include <sys/ioctl.h>
#endif

#include <time.h>
#ifndef USERSPACE
#include <netinet/ether.h>
#endif


/*----- */

/* ipfw2.c - from timeconv.h */
static __inline time_t
_long_to_time(long tlong)
{
    if (sizeof(long) == sizeof(__int32_t))
        return((time_t)(__int32_t)(tlong));
    return((time_t)tlong);
}

#define min(a, b) ((a) < (b) ? (a) : (b) )	// radix.c
/*
 * debugging macros from ip_dn_private.h
 */
#include <sys/time.h>
#include <stdio.h>
extern char *strrchr(const char *, int);
static inline const char *xyz(const char *s) {
	static char buf[128];
	struct timeval t;
	const char *ret = strrchr(s, '/');
	if (ret) s = ret + 1;
	gettimeofday(&t, NULL);
	buf[sizeof(buf) - 1] = '\0';
	snprintf(buf, sizeof(buf), "[%4d.%06d] %s",
		(int)(t.tv_sec % 1000), (int)(t.tv_usec), s);
	return buf;
}

#define ND(fmt, ...) do {} while (0)
#define D1(fmt, ...) do {} while (0)
#define D(fmt, ...) fprintf(stderr, "%s:%-10s [%d] " fmt "\n",      	\
        	xyz(__FILE__), __FUNCTION__, __LINE__,  ## __VA_ARGS__)

/* Rate limited version of "D", lps indicates how many per second */
#define RD(lps, format, ...)                                    \
    do {                                                        \
        static int t0, __cnt;                                   \
        struct timeval __xxts;                                  \
        gettimeofday(&__xxts, NULL);                            \
        if (t0 != __xxts.tv_sec) {                              \
            t0 = __xxts.tv_sec;                                 \
            __cnt = 0;                                          \
        }                                                       \
        if (__cnt++ < lps) {                                    \
            D(format, ##__VA_ARGS__);                           \
        }                                                       \
    } while (0)

#define DX(lev, fmt, ...) do {              \
        if (dn_cfg.debug > lev) D(fmt, ## __VA_ARGS__); } while (0)
/* end debugging macros */


/*
 * sbin/ipfw on non-freebsd platform
 */
#ifdef NEED_STRTONUM
/* prototypes from libutil */
/* humanize_number(3) */
#define HN_DECIMAL              0x01
#define HN_NOSPACE              0x02
#define HN_B                    0x04
#define HN_DIVISOR_1000         0x08
#define HN_IEC_PREFIXES         0x10

#define HN_GETSCALE             0x10
#define HN_AUTOSCALE            0x20


int humanize_number(char *_buf, size_t _len, int64_t _number,
            const char *_suffix, int _scale, int _flags);
int expand_number(const char *buf, uint64_t *num);


long long
strtonum(const char *nptr, long long minval, long long maxval,
 const char **errstr);
#ifndef __APPLE__
int ishexnumber(int c);
#endif
#endif /* NEED_STRTONUM */

#ifdef NEED_SYSCTLBYNAME /* and other linux calls */
int sysctlbyname(const char *name, void *oldp, size_t *oldlenp,
        void *newp, size_t newlen);
#define setprogname(x)  /* not present in linux */
 
extern int optreset;    /* not present in linux */
 
long long int strtonum(const char *nptr, long long minval,
        long long maxval, const char **errstr);


struct ether_addr;
struct ether_addr * ether_aton(const char *a);

#define ICMP6_MAXTYPE   201
#define __u6_addr       in6_u
#define in6_u __in6_u   /* missing type for ipv6 (linux 2.6.28) */


#define __u6_addr32     u6_addr32
/* on freebsd sys/socket.h pf specific */
#define NET_RT_IFLIST   3               /* survey interface list */

#define RTM_VERSION     5       /* Up the ante and ignore older versions */

#endif // NEED_SYSCTLBYNAME

#ifdef NEED_SIN_LEN
/*
 * linux at least does not have sin_len and sin6_len, so we remap
 * to some safe fields (check use of sin6_flowinfo XXX)
 */
#define sin_len sin_zero[0]
#define sin6_len sin6_flowinfo
#endif /* NEED_SIN_LEN */

#ifdef NEED_ROUNDUP2 /* in freensd is in sys/param.h */
/* round up to the next power of 2 (y) */
#define roundup2(x, y)  (((x)+((y)-1))&(~((y)-1))) /* if y is powers of two */
#endif // NEED_ROUNDUP2

/* possibly redundant, does not harm */
size_t strlcpy(char * dst, const char * src, size_t siz);

/*
 * Part 2: common userland and kernel definitions
 */

#define ICMP6_DST_UNREACH_NOROUTE       0       /* no route to destination */
#define ICMP6_DST_UNREACH_ADMIN         1       /* administratively prohibited */
#define ICMP6_DST_UNREACH_ADDR          3       /* address unreachable */
#define ICMP6_DST_UNREACH_NOPORT        4       /* port unreachable */

/*
 * linux: sysctl are mapped into /sys/module/ipfw_mod parameters
 * windows: they are emulated via get/setsockopt
 */
#define CTLFLAG_RD	1
#define CTLFLAG_RDTUN	1
#define CTLFLAG_RW	2
#define CTLFLAG_SECURE3	0	/* unsupported */
#define CTLFLAG_VNET    0	/* unsupported */

/* if needed, queue.h must be included here after list.h */

/*
 * our own struct thread
 */
struct thread {	/* ip_fw_sockopt */
        void *sopt_td;
        void *td_ucred;
};

enum sopt_dir { SOPT_GET, SOPT_SET };

struct  sockopt {
        enum    sopt_dir sopt_dir; /* is this a get or a set? */
        int     sopt_level;     /* second arg of [gs]etsockopt */
        int     sopt_name;      /* third arg of [gs]etsockopt */
	void   *sopt_val;       /* fourth arg of [gs]etsockopt */
	size_t  sopt_valsize;   /* (almost) fifth arg of [gs]etsockopt */
	struct  thread *sopt_td; /* calling thread or null if kernel */
};


/*
 * List of values used for set/getsockopt options.
 * The base value on FreeBSD is defined as a macro,
 * if not available we will use our own enum.
 * The TABLE_BASE value is used in the kernel.
 */
#define _IPFW_SOCKOPT_BASE	100	/* 40 on freebsd */
#define	IP_FW_TABLE_ADD		(_IPFW_SOCKOPT_BASE +   0)
#define	IP_FW_TABLE_DEL		(_IPFW_SOCKOPT_BASE +   1)
#define	IP_FW_TABLE_FLUSH	(_IPFW_SOCKOPT_BASE +   2)
#define	IP_FW_TABLE_GETSIZE	(_IPFW_SOCKOPT_BASE +   3)
#define	IP_FW_TABLE_LIST	(_IPFW_SOCKOPT_BASE +   4)
#define	IP_FW_DYN_GET		(_IPFW_SOCKOPT_BASE +   5)

#define	IP_FW3			(_IPFW_SOCKOPT_BASE +   8)
#define	IP_DUMMYNET3		(_IPFW_SOCKOPT_BASE +   9)

#define	IP_FW_ADD		(_IPFW_SOCKOPT_BASE +  10)
#define	IP_FW_DEL		(_IPFW_SOCKOPT_BASE +  11)
#define	IP_FW_FLUSH		(_IPFW_SOCKOPT_BASE +  12)
#define	IP_FW_ZERO		(_IPFW_SOCKOPT_BASE +  13)
#define	IP_FW_GET		(_IPFW_SOCKOPT_BASE +  14)
#define	IP_FW_RESETLOG		(_IPFW_SOCKOPT_BASE +  15)

#define	IP_FW_NAT_CFG		(_IPFW_SOCKOPT_BASE +  16)
#define	IP_FW_NAT_DEL		(_IPFW_SOCKOPT_BASE +  17)
#define	IP_FW_NAT_GET_CONFIG	(_IPFW_SOCKOPT_BASE +  18)
#define	IP_FW_NAT_GET_LOG	(_IPFW_SOCKOPT_BASE +  19)

#define	IP_DUMMYNET_CONFIGURE	(_IPFW_SOCKOPT_BASE +  20)
#define	IP_DUMMYNET_DEL		(_IPFW_SOCKOPT_BASE +  21)
#define	IP_DUMMYNET_FLUSH	(_IPFW_SOCKOPT_BASE +  22)
        /* 63 is missing */
#define	IP_DUMMYNET_GET         (_IPFW_SOCKOPT_BASE + 24)
#define	_IPFW_SOCKOPT_END	(_IPFW_SOCKOPT_BASE + 25)

/*
 * Part 3: userland stuff for linux/windows
 */


/*
 * now remap functions for userland or linux kernel etc.
 */
#ifdef USERSPACE
/*
 * definitions used when the programs communicate through userspace.
 * We need to define the socket and addresses used to talk, and
 * the userland side must also remap socket() and [gs]etsockopt()
 * to appropriate wrappers.
 */

#define LOCALADDR       "127.0.0.1"
#define IPFW_PORT       5555

#ifndef KERNEL_SIDE
#ifdef _KERNEL
#error _KERNEL defined in user space
#endif
int do_connect(const char *addr, int port);
#include <sys/socket.h> /* for socklen_t */

#define socket(a, b, c) do_connect(LOCALADDR, IPFW_PORT)
#define setsockopt      setsockopt2
#define getsockopt      getsockopt2
int getsockopt2(int s, int lev, int optname, void *optval, socklen_t *optlen);
int setsockopt2(int s, int lev, int optname, void *optval, socklen_t optlen);
#endif /* KERNEL_SIDE */

#endif	/* USERSPACE */

/*
 * Part 5: windows specific stuff and sysctl emulation
 */

/*******************
* SYSCTL emulation *
********************/
#ifdef EMULATE_SYSCTL

/* this needs to be here, as it is part of the user-kernel messages */
/* flag is set with the last 2 bits for access, as defined in glue.h
 * and the rest for type
 */
enum {
	SYSCTLTYPE_INT		= 0,
	SYSCTLTYPE_UINT		= 1,
	SYSCTLTYPE_SHORT	= 2,
	SYSCTLTYPE_USHORT	= 3,
	SYSCTLTYPE_LONG		= 4,
	SYSCTLTYPE_ULONG	= 5,
	SYSCTLTYPE_STRING	= 6,

	/* the following are SYSCTL_PROC equivalents of the above,
	 * where the SYSCTLTYPE is shifted 2 bits,
	 * and SYSCTLTYPE_PROC is set
	 */
	SYSCTLTYPE_PROC		= 0x100,
	CTLTYPE_INT		= (0x100 | (0<<2)),
	CTLTYPE_UINT		= (0x100 | (1<<2)),
	CTLTYPE_LONG		= (0x100 | (4<<2)),
	CTLTYPE_ULONG		= (0x100 | (5<<2))
};

struct sysctlhead {
	uint32_t blocklen; //total size of the entry
	uint32_t namelen; //strlen(name) + '\0'
	uint32_t flags; //type and access
	uint32_t datalen;
};


#endif /* EMULATE_SYSCTL */

#ifndef __FreeBSD__
int sysctlbyname(const char *name, void *oldp, size_t *oldlenp, void *newp,
         size_t newlen);

#define test_bit(ix, pData)     ((*pData) & (1<<(ix)))
#define __set_bit(ix, pData)    (*pData) |= (1<<(ix))
#define __clear_bit(ix, pData)  (*pData) &= ~(1<<(ix))

static inline int fls(int _n)
{
	unsigned int n = _n;
        int i = 0;
        for (i = 0; n > 0; n >>= 1, i++)
                ;
        return i;
}

static inline unsigned long __fls(unsigned long word)
{
        return fls(word) - 1;
}


#endif /* !FreeBSD */

#ifdef KERNEL_SIDE
/* sys/counter.h , to be moved to a file */
typedef uint64_t *counter_u64_t; // XXX kernel
static inline void counter_u64_add(counter_u64_t c, int64_t v)
{
	*c += v;
}
static inline void counter_u64_zero(counter_u64_t c)
{
	*c = 0;
}
static inline uint64_t counter_u64_fetch(counter_u64_t c)
{
	return *c;
}

struct rm_priotracker {
};

#define vslock(_a, _b)	(0)
#define vsunlock(_a, _b)

typedef uint64_t	u_register_t; // XXX not on osx ?

typedef uintptr_t eventhandler_tag;
#define EVENTHANDLER_REGISTER(_a, _b, ...)	(uintptr_t)_b;
#define EVENTHANDLER_DEREGISTER(_a, _b, ...)	(void)_b;

// XXX this needs to be completed
#define if_name(_ifp)	(_ifp->if_xname)
#define ifunit_ref(_n)	NULL	// XXX
#define if_rele(_n)

#define rtalloc1_fib(_a, ...)	NULL
#define rt_key(_a)	NULL
#define rt_mask(_a)	NULL
#define RTFREE_LOCKED(_a)	((void)NULL)
struct rtentry {
};
#define rt_tables_get_rnh(_a, _b)	NULL

#endif /* KERNEL_SIDE */

#ifdef _KERNEL
/* XXX kernel support */
/* on freebsd net/if.h XXX used */
#ifdef linux
#define div64(a,b) (((int64_t)a)/((int64_t)b))
#define LINUX_VERSION_CODE	30003
#define	KERNEL_VERSION(a,b,c)	(a*10000+b*100 + c)
#define __printflike(a,b)
#endif /* linux */

#endif /* _KERNEL */

#ifndef __FreeBSD__
#ifndef	IFNAMSIZ
#define	IFNAMSIZ	16
#endif
#include "missing.h"

struct if_data {
        /* ... */
        u_long ifi_mtu; /* maximum transmission unit */
};

#endif

#ifdef __APPLE__
#include <sys/socketvar.h>	// need in kernel

/* needed both in kernel and userspace */
struct if_data64 {	// XXX Darwin version
        /* ... */
        u_long ifi_mtu; /* maximum transmission unit */
};

struct net_event_data {
};

struct in_addr;
#endif /* __APPLE__ */

#define __PAST_END(v, idx)	v[idx]

/*
 * a fast copy routine
 */
#include <strings.h>
// XXX only for multiples of 64 bytes, non overlapped.
static inline void
_pkt_copy(const void *_src, void *_dst, int l)
{
        const uint64_t *src = _src;
        uint64_t *dst = _dst;
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)       __builtin_expect(!!(x), 0)
        if (unlikely(l >= 1024)) {
                bcopy(src, dst, l);
                return;
        }
        for (; l > 0; l-=64) {
                *dst++ = *src++;
                *dst++ = *src++;
                *dst++ = *src++;
                *dst++ = *src++;
                *dst++ = *src++;
                *dst++ = *src++;
                *dst++ = *src++;
                *dst++ = *src++;
        }
}

#endif /* !_GLUE_H */
