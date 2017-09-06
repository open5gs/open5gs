/*
 * Copyright (C) 2009 Luigi Rizzo, Marta Carbone, Universita` di Pisa
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
 * $Id: missing.h 8377 2011-04-04 16:08:27Z marta $
 *
 * Header for kernel variables and functions that are not available in
 * userland.
 */

#ifndef _MISSING_H_
#define _MISSING_H_

#define KLD_MODULE	/* disable kernel dependencies */

/* defined as assert */
void panic(const char *fmt, ...);

#define KASSERT(exp,msg) do {                                           \
        if (__predict_false(!(exp)))                                    \
                panic msg;                                              \
} while (0)
/* don't bother to optimize */
#ifndef __predict_false
#define __predict_false(x)   (x)        /* __builtin_expect((exp), 0) */
#endif // XXX


#ifdef _KERNEL
#define NEED_KERNEL
#undef _KERNEL
#endif

#include <stdio.h>	// printf
#include <sys/socket.h>	// IFNAMSIZ ?
#include <string.h>	// strncmp
#include <stdlib.h>	// bsearch
#ifdef NEED_KERNEL
#define _KERNEL
#include <sys/cdefs.h>
#include <sys/param.h>

#define __user	// not defined here ?
#define __init
#define __exit

/* portability features, to be set before the rest: */
#define WITHOUT_BPF		/* do not use bpf logging */

#define MALLOC_DECLARE(x)	struct __hack /* nothing */
// XXX kernel malloc/free
extern void *kern_malloc(int);
extern void kern_free(void *);
#define malloc(_size, type, flags) kern_malloc(_size)
#define free(_var, type) kern_free(_var)

/* inet_ntoa_r() differs in userspace and kernel.
 * We load netinet/in.h so we get the kernel prototype ?
 * but we also need to put #defines in the two places where
 * it is used XXX fixme
 */
#include <netinet/in.h>

/* log() conflicts with the math function.
 * Revise, modifying the first argument.
 */
#define	LOG_ERR		0x100
#define	LOG_INFO	0x200
#ifndef LOG_SECURITY
#define LOG_SECURITY	0x400
#endif

#define log(_level, fmt, arg...)  do {			\
	int __attribute__((unused)) _querty = _level;	\
	printf("kernel: " fmt, ##arg); } while (0)

#endif /* _KERNEL */

/*
 * Kernel locking support.
 * FreeBSD uses mtx in dummynet.c and struct rwlock ip_fw2.c
 *
 * In linux we use spinlock_bh to implement both.
 * For 'struct rwlock' we need an #ifdef to change it to spinlock_t
 */

#ifndef DEFINE_SPINLOCK	/* this is for linux 2.4 */
#if defined(__APPLE__)
#define DEFINE_SPINLOCK(x)   struct mtx x;
#else /* linux ? */
#define DEFINE_SPINLOCK(x)   spinlock_t x	// = SPIN_LOCK_UNLOCKED
#endif
#endif

/* 20111031
 * redefine mutex in terms of threads.
 */

#undef _KERNEL
// #include <sys/types.h>
#include <pthread.h>
#ifdef NEED_KERNEL
#define _KERNEL
#endif
struct mtx {
	pthread_mutex_t p0;
};
struct rwlock {
	pthread_mutex_t p0;
};
struct rmlock {
	pthread_mutex_t p0;
};
extern pthread_mutex_t dummynet_mtx_p;
extern pthread_mutex_t ipfw_dyn_mtx_p;
extern pthread_mutex_t pfil_global_lock_p;

#define mtx_assert(a, b)
/*
 * the first argument to mtx_init is often a static variable,
 * so use (void)m to prevent a compiler warning
 */
#define mtx_init(m, a,b,c)      do {                    \
        (void)m; pthread_mutex_init(&((m)->p0), NULL); } while (0)
#define MTX_SYSINIT(a, m, c, d) // pthread_mutex_init(m##_p, NULL)
#define mtx_lock(m)             pthread_mutex_lock(m.p0)
#define mtx_unlock(m)           pthread_mutex_unlock(m.p0)
#define mtx_destroy(m)          pthread_mutex_destroy(m.p0)
#if 1
//------------------

#if 1 // used for IPFW_UH
#define rw_assert(a, b)
#define rw_destroy(_l)
#define rw_init(_l, msg)	// XXX mtx_init((_l), 0, 0, 0)
#define rw_rlock(_l)		mtx_lock(_l)
#define rw_runlock(_l)		mtx_unlock(_l)
#define rw_wlock(_l)		mtx_lock(_l)
#define rw_wunlock(_l)		mtx_unlock(_l)
#define rw_init_flags(_l, s, v)
#endif // XXX not used anymore

#define rm_init(_l, msg)	// mtx_init(...)
#define	rm_rlock(_l, _t)	((void)_t, mtx_lock(_l))
#define	rm_runlock(_l, _t)	mtx_unlock(_l)
#define	rm_wlock(_l)		mtx_lock(_l)
#define	rm_wunlock(_l)		mtx_unlock(_l)
#define rm_destroy(_l)		// XXX
#define rm_assert(_l, _w)	// XXX


#endif // locking on linux ?

/* end of locking support */

/*
 * Reference to an ipfw rule that can be carried outside critical sections.
 * A rule is identified by rulenum:rule_id which is ordered.
 * In version chain_id the rule can be found in slot 'slot', so
 * we don't need a lookup if chain_id == chain->id.
 *
 * On exit from the firewall this structure refers to the rule after
 * the matching one (slot points to the new rule; rulenum:rule_id-1
 * is the matching rule), and additional info (e.g. info often contains
 * the insn argument or tablearg in the low 16 bits, in host format).
 * On entry, the structure is valid if slot>0, and refers to the starting
 * rules. 'info' contains the reason for reinject, e.g. divert port,
 * divert direction, and so on.
 */
struct ipfw_rule_ref {
	uint32_t	slot;		/* slot for matching rule	*/
	uint32_t	rulenum;	/* matching rule number		*/
	uint32_t	rule_id;	/* matching rule id		*/
	uint32_t	chain_id;	/* ruleset id			*/
	uint32_t	info;		/* see below			*/
};

/* ISO C restricts enumerator values to range of 'int'
 * so we need IN to have a smaller value
 */
enum {
	IPFW_INFO_MASK	= 0x0000ffff,
	IPFW_INFO_OUT	= 0x00000000,	/* outgoing, just for convenience */
	IPFW_INFO_IN	= 0x00800000,	/* incoming, overloads dir */
	IPFW_ONEPASS	= 0x40000000,	/* One-pass, do not reinject */
	IPFW_IS_MASK	= 0x30000000,	/* which source ? */
	IPFW_IS_DIVERT	= 0x20000000,
	IPFW_IS_DUMMYNET =0x10000000,
	IPFW_IS_PIPE	= 0x08000000,	/* pipe=1, queue = 0 */
};

/* in netinet/in.h */
#define        in_nullhost(x)  ((x).s_addr == INADDR_ANY)

/* ip_dummynet.c */
#ifndef __FreeBSD_version
#define __FreeBSD_version 500035
#endif

/* define some macro for ip_dummynet */

struct malloc_type {
};

#define MALLOC_DEFINE(type, shortdesc, longdesc) 	\
	struct malloc_type type[1]; void *md_dummy_ ## type = type

#define CTASSERT(x)


/*
 * gettimeofday would be in sys/time.h but it is not
 * visible if _KERNEL is defined
 */
//int gettimeofday(struct timeval *, struct timezone *);


extern int	hz;
extern long	tick;		/* exists in 2.4 but not in 2.6 */
extern int	bootverbose;
extern struct timeval boottime;

/* time_uptime is a FreeBSD variable increased each second */
extern time_t time_uptime;

extern int	max_linkhdr;
extern int	ip_defttl;
extern u_long	in_ifaddrhmask;                         /* mask for hash table */
extern struct in_ifaddrhashhead *in_ifaddrhashtbl;    /* inet addr hash table  */

/*-------------------------------------------------*/

/* define, includes and functions missing in linux */
/* include and define */
#include <arpa/inet.h>		/* inet_ntoa */

struct mbuf;
// XXX #define M_MCAST         0x04 /* send/received as link-level multicast */


/* used by ip_dummynet.c */
void reinject_drop(struct mbuf* m);

#include <sys/socket.h> /* for ETHERTYPE_IP */

#ifdef _KERNEL
#define	IF_NAMESIZE	16
#ifndef IFNAMSIZ
#define	IFNAMSIZ	IF_NAMESIZE
#endif
//#include <net/if.h>		/* IFNAMESIZ */
#endif

/*
 * some network structure can be defined in the bsd way
 * by using the _FAVOR_BSD definition. This is not true
 * for icmp structure.
 * XXX struct icmp contains bsd names in 
 * /usr/include/netinet/ip_icmp.h
 */

/* missing definition */
#define TH_FIN  0x01
#define TH_SYN  0x02
#define TH_RST  0x04
#define TH_ACK  0x10

/* 20131101 IPTOS from ip.h */
/*
 * Definitions for DiffServ Codepoints as per RFC2474
 */
#define IPTOS_DSCP_CS0          0x00
#define IPTOS_DSCP_CS1          0x20
#define IPTOS_DSCP_AF11         0x28
#define IPTOS_DSCP_AF12         0x30
#define IPTOS_DSCP_AF13         0x38
#define IPTOS_DSCP_CS2          0x40
#define IPTOS_DSCP_AF21         0x48
#define IPTOS_DSCP_AF22         0x50
#define IPTOS_DSCP_AF23         0x58
#define IPTOS_DSCP_CS3          0x60
#define IPTOS_DSCP_AF31         0x68
#define IPTOS_DSCP_AF32         0x70
#define IPTOS_DSCP_AF33         0x78
#define IPTOS_DSCP_CS4          0x80
#define IPTOS_DSCP_AF41         0x88
#define IPTOS_DSCP_AF42         0x90
#define IPTOS_DSCP_AF43         0x98
#define IPTOS_DSCP_CS5          0xa0
#define IPTOS_DSCP_EF           0xb8
#define IPTOS_DSCP_CS6          0xc0
#define IPTOS_DSCP_CS7          0xe0

/*
 * ECN (Explicit Congestion Notification) codepoints in RFC3168 mapped to the
 * lower 2 bits of the TOS field.
 */
#define IPTOS_ECN_NOTECT        0x00    /* not-ECT */
#define IPTOS_ECN_ECT1          0x01    /* ECN-capable transport (1) */
#define IPTOS_ECN_ECT0          0x02    /* ECN-capable transport (0) */
#define IPTOS_ECN_CE            0x03    /* congestion experienced */
#define IPTOS_ECN_MASK          0x03    /* ECN field mask */

/*------------------------- */

#define RTF_CLONING	0x100		/* generate new routes on use */

#define IPPROTO_OSPFIGP         89              /* OSPFIGP */
#define IPPROTO_CARP            112             /* CARP */
#define	CARP_VERSION		2
#define	CARP_ADVERTISEMENT	0x01
#define PRIV_NETINET_IPFW       491     /* Administer IPFW firewall. */
#define IP_FORWARDING           0x1             /* most of ip header exists */
#define NETISR_IP       2               /* same as AF_INET */
#define PRIV_NETINET_DUMMYNET   494     /* Administer DUMMYNET. */

extern int securelevel;

#define if_xname        name
#define if_snd          XXX

// XXX we could use this to point to the incoming peer
struct ifnet {
        char    if_xname[IFNAMSIZ];     /* external name (name + unit) */
	uint32_t	if_index;	// IP_FW_3
};

struct ifaltq {

        void *ifq_head;
};
int ffs(int);	// XXX where
int fls(int);	// XXX where

struct ip;
/* machine/in_cksum.h */
int in_cksum(struct mbuf *m, int len);
#ifndef __FreeBSD__
u_short in_cksum_hdr(struct ip *);
#endif


#define CTR3(a, ...)
#define uma_zone_set_max(a, b)	// XXX

/*
 * ifnet->if_snd is used in ip_dummynet.c to take the transmission
 * clock.
 */
#if defined( __linux__)
#define	if_xname	name
#define	if_snd		XXX

struct route_in6 {
};

#elif defined( _WIN32 )
/* used in ip_dummynet.c */
struct ifnet {
	char    if_xname[IFNAMSIZ];     /* external name (name + unit) */
//        struct ifaltq if_snd;          /* output queue (includes altq) */
};

struct net_device {
	char    if_xname[IFNAMSIZ];     /* external name (name + unit) */
};
#elif defined(__APPLE__)
typedef	u_int32_t tcp_cc;
#ifndef s6_addr32	// XXX
#define s6_addr32 __u6_addr.__u6_addr32
#endif
#include <netinet/tcp.h>

struct route_in6 {
};

struct icmphdr {
        u_char  icmp_type;              /* type of message, see below */
        u_char  icmp_code;              /* type sub code */
        u_short icmp_cksum;             /* ones complement cksum of struct */
};

#define IPPROTO_SCTP            132             /* SCTP */

/* defined in linux/sctp.h with no bsd definition */
struct sctphdr {
        uint16_t src_port;      /* source port */
        uint16_t dest_port;     /* destination port */
        uint32_t v_tag;         /* verification tag of packet */
        uint32_t checksum;      /* Adler32 C-Sum */
        /* chunks follow... */
};

struct carp_header {
#if BYTE_ORDER == LITTLE_ENDIAN
        u_int8_t        carp_type:4,
                        carp_version:4;
#endif
#if BYTE_ORDER == BIG_ENDIAN
        u_int8_t        carp_version:4,
                        carp_type:4;
#endif
};


struct pim {
        int dummy;      /* windows compiler does not like empty definition */
};

#endif

/* involves mbufs */
//int in_cksum(struct mbuf *m, int len);
#define divert_cookie(mtag) 0
#define divert_info(mtag) 0
#define pf_find_mtag(a) NULL
#define pf_get_mtag(a) NULL
#if !defined(_WIN32) && !defined(AF_LINK)
#define AF_LINK AF_ASH	/* ? our sys/socket.h */
#endif

/* search local the ip addresses, used for the "me" keyword */
#define INADDR_TO_IFP(ip, b)	b = NULL

/* we don't pullup, either success or free and fail */
#define m_pullup(m, x)					\
	((m)->m_len >= x ? (m) : (FREE_PKT(m), NULL))

struct pf_mtag {
	void            *hdr;           /* saved hdr pos in mbuf, for ECN */
	sa_family_t      af;            /* for ECN */
        u_int32_t        qid;           /* queue id */
};

/* missing kernel functions */
char *inet_ntoa(struct in_addr ina);
long random(void);

/*
 * Return the risult of a/b
 *
 * this is used in linux kernel space,
 * since the 64bit division needs to
 * be done using a macro
 */
//int64_t div64(int64_t a, int64_t b);

/* from bsd sys/queue.h */
#define TAILQ_FOREACH_SAFE(var, head, field, tvar)                      \
        for ((var) = TAILQ_FIRST((head));                               \
            (var) && ((tvar) = TAILQ_NEXT((var), field), 1);            \
            (var) = (tvar))

#define SLIST_FOREACH_SAFE(var, head, field, tvar)                      \
        for ((var) = SLIST_FIRST((head));                               \
            (var) && ((tvar) = SLIST_NEXT((var), field), 1);            \
            (var) = (tvar))

/*-------------------------------------------------*/
#define RT_NUMFIBS 1
extern u_int rt_numfibs;

/* involves kernel locking function */
#ifdef RTFREE
#undef RTFREE
#define RTFREE(a) fprintf(stderr, "RTFREE: commented out locks\n");
#endif

void getmicrouptime(struct timeval *tv);

/* from sys/netinet/ip_output.c */
struct ip_moptions;
struct route;
struct ip;

struct inpcb;
struct mbuf *ip_reass(struct mbuf *);
int ip_output(struct mbuf *m, struct mbuf *opt, struct route *ro, int flags,
    struct ip_moptions *imo, struct inpcb *inp);

/* from net/netisr.c -- fails on FreeBSD */
int     netisr_dispatch(u_int proto, struct mbuf *m);


/* definition moved in missing.c */
int sooptcopyout(struct sockopt *sopt, const void *buf, size_t len);
int copyout(const void *kaddr, void *uaddr, size_t len);

int sooptcopyin(struct sockopt *sopt, void *buf, size_t len, size_t minlen);

/* defined in session.c */
int priv_check(struct thread *td, int priv);

/* struct ucred is in linux/socket.h and has pid, uid, gid.
 * We need a 'bsd_ucred' to store also the extra info
 */

struct bsd_ucred {
	uid_t		uid;
	gid_t		gid;
	uint32_t	xid;
	uint32_t	nid;
};

#ifdef _KERNEL

#if 0 // XXX
int
cred_check(void *insn, int proto, struct ifnet *oif,
    struct in_addr dst_ip, u_int16_t dst_port, struct in_addr src_ip,
    u_int16_t src_port, struct bsd_ucred *u, int *ugid_lookupp,
    struct sk_buff *skb);
#endif

struct ucred;
int securelevel_ge(struct ucred *cr, int level);

/*
 * stripped down version of the sysctl api
 */
struct sysctl_oid;
struct sysctl_req {
	void	*oldptr;	/* store here the original value */
	int	oldlen;
	void	*newptr;	/* NULL on reads */
	int	newlen;
};

#ifdef _WIN32
#define module_param_named(_name, _var, _ty, _perm)
#else /* !_WIN32 */

#endif /* !_WIN32 so maybe __linux__ */

#if 0 // XXX disable sysctl defined (__linux__) && !defined (EMULATE_SYSCTL)
#define SYSCTL_DECL(_1)
#define SYSCTL_OID(_1, _2, _3, _4, _5, _6, _7, _8)
#define SYSCTL_NODE(_1, _2, _3, _4, _5, _6)
#define _SYSCTL_BASE(_name, _var, _ty, _perm)		\
	module_param_named(_name, *(_var), _ty, 	\
		( (_perm) == CTLFLAG_RD) ? 0444: 0644 )
#define SYSCTL_PROC(_base, _oid, _name, _mode, _var, _val, _desc, _a, _b)

#define SYSCTL_INT(_base, _oid, _name, _mode, _var, _val, _desc)	\
	_SYSCTL_BASE(_name, _var, int, _mode)

#define SYSCTL_LONG(_base, _oid, _name, _mode, _var, _val, _desc)	\
	_SYSCTL_BASE(_name, _var, long, _mode)

#define SYSCTL_ULONG(_base, _oid, _name, _mode, _var, _val, _desc)	\
	_SYSCTL_BASE(_name, _var, ulong, _mode)

#define SYSCTL_UINT(_base, _oid, _name, _mode, _var, _val, _desc)	\
	 _SYSCTL_BASE(_name, _var, uint, _mode)

#define TUNABLE_INT(_name, _ptr)

#define SYSCTL_VNET_PROC		SYSCTL_PROC
#define SYSCTL_VNET_INT			SYSCTL_INT
#define SYSCTL_VNET_UINT		SYSCTL_UINT

#endif

#define SYSCTL_HANDLER_ARGS 		\
	struct sysctl_oid *oidp, void *arg1, int arg2, struct sysctl_req *req
typedef int (sysctl_h_fn_t)(SYSCTL_HANDLER_ARGS);
int sysctl_handle_int(SYSCTL_HANDLER_ARGS);
int sysctl_handle_long(SYSCTL_HANDLER_ARGS); 

#ifdef EMULATE_SYSCTL	/* mandatory here */

#define STRINGIFY(x) #x

#ifdef SYSCTL_NODE
#undef SYSCTL_NODE
#endif
#define SYSCTL_NODE(a,b,c,d,e,f) 	int a; (void)a
#define SYSCTL_DECL(a)

#define GST_HARD_LIMIT 100

/* In the module, GST is implemented as an array of
 * sysctlentry, but while passing data to the userland
 * pointers are useless, the buffer is actually made of:
 * - sysctlhead (fixed size, containing lengths)
 * - data (typically 32 bit)
 * - name (zero-terminated and padded to mod4)
 */

struct sysctlentry {
	struct sysctlhead head;
	char* name;
	void* data;
	sysctl_h_fn_t *fn;
};

struct sysctltable {
	int count; //number of valid tables
	int totalsize; //total size of valid entries of al the valid tables
	void* namebuffer; //a buffer for all chained names
	struct sysctlentry entry[GST_HARD_LIMIT];
};

#ifdef SYSBEGIN
#undef SYSBEGIN
#endif
#define SYSBEGIN(x) void sysctl_addgroup_##x() {
#ifdef SYSEND
#undef SYSEND
#endif
#define SYSEND }

/* XXX remove duplication */
#define SYSCTL_INT(a,b,c,d,e,f,g) 				\
	sysctl_pushback(STRINGIFY(a) "." STRINGIFY(c) + 1,	\
		(d) | (SYSCTLTYPE_INT << 2), sizeof(*e), e, NULL)

#define SYSCTL_UINT(a,b,c,d,e,f,g)				\
	sysctl_pushback(STRINGIFY(a) "." STRINGIFY(c) + 1,	\
		(d) | (SYSCTLTYPE_UINT << 2), sizeof(*e), e, NULL)

#define SYSCTL_LONG(a,b,c,d,e,f,g)				\
	sysctl_pushback(STRINGIFY(a) "." STRINGIFY(c) + 1,	\
		(d) | (SYSCTLTYPE_LONG << 2), sizeof(*e), e, NULL)

#define SYSCTL_ULONG(a,b,c,d,e,f,g)				\
	sysctl_pushback(STRINGIFY(a) "." STRINGIFY(c) + 1,	\
		(d) | (SYSCTLTYPE_ULONG << 2), sizeof(*e), e, NULL)
#define TUNABLE_INT(a,b)

#define SYSCTL_PROC(a,b,c,d,e,f,g,h,i)			\
	sysctl_pushback(STRINGIFY(a) "." STRINGIFY(c) + 1,	\
		(d), 4 /* XXX large */, (void *)(f /* arg2 */), g)

#define SYSCTL_VNET_PROC		SYSCTL_PROC
#define SYSCTL_VNET_INT			SYSCTL_INT
#define SYSCTL_VNET_UINT		SYSCTL_UINT

void keinit_GST(void);
void keexit_GST(void);
int kesysctl_emu_set(void* p, int l);
int kesysctl_emu_get(struct sockopt* sopt);
void sysctl_pushback(char* name, int flags, int datalen, void* data, sysctl_h_fn_t *fn);

#endif /* EMULATE_SYSCTL */

struct ifnet;
void ether_demux(struct ifnet *ifp, struct mbuf *m);

int ether_output_frame(struct ifnet *ifp, struct mbuf *m);

void in_rtalloc_ign(struct route *ro, u_long ignflags, u_int fibnum);

void icmp_error(struct mbuf *n, int type, int code, uint32_t dest, int mtu);

#define in_localip(_x)	(0)

#ifndef __FreeBSD__
struct rtentry;
#endif
void rtfree(struct rtentry *rt);

u_short in_cksum_skip(struct mbuf *m, int len, int skip);

#ifdef INP_LOCK_ASSERT
#undef INP_LOCK_ASSERT
#define INP_LOCK_ASSERT(a)
#endif

int jailed(struct ucred *cred);

/*
* Return 1 if an internet address is for a ``local'' host
* (one to which we have a connection).  If subnetsarelocal
* is true, this includes other subnets of the local net.
* Otherwise, it includes only the directly-connected (sub)nets.
*/
int in_localaddr(struct in_addr in);

int fnmatch(const char *pattern, const char *string, int flags);

/* vnet wrappers, in vnet.h and ip_var.h */
//int ipfw_init(void);
//void ipfw_destroy(void);

#define	MTAG_IPFW	1148380143	/* IPFW-tagged cookie */
#define	MTAG_IPFW_RULE	1262273568	/* rule reference */
#define	MTAG_IPFW_CALL	1308397630	/* call stack */

#ifdef __APPLE__
#define offsetof(type, field) __builtin_offsetof(type, field)
#endif
struct ip_fw_args;
extern int (*ip_dn_io_ptr)(struct mbuf **m, int dir, struct ip_fw_args *fwa);

#if 1 /* include vnet.h */
#define curvnet                 NULL
#define	CURVNET_SET(_v)
#define	CURVNET_RESTORE()
#define VNET_ASSERT(condition)

#define VNET_NAME(n)            n
#define VNET_DECLARE(t, n)      extern t n
#define VNET_DEFINE(t, n)       t n
#define _VNET_PTR(b, n)         &VNET_NAME(n)
/*
 * Virtualized global variable accessor macros.
 */
#define VNET_VNET_PTR(vnet, n)          (&(n))
#define VNET_VNET(vnet, n)              (n)

#define VNET_PTR(n)             (&(n))
#define VNET(n)                 (n)

#define	IS_DEFAULT_VNET(x)	(1)	// always true
#endif

VNET_DECLARE(int, ip_defttl);
#define V_ip_defttl    VNET(ip_defttl);


// int ipfw_check_hook(void *arg, struct mbuf **m0, struct ifnet *ifp, int dir, struct inpcb *inp);
// XXX used in netmap_io.c
int ipfw_check_packet(void *arg, struct mbuf **m0, struct ifnet *ifp, int dir, struct inpcb *inp);
int ipfw_check_frame(void *arg, struct mbuf **m0, struct ifnet *ifp, int dir, struct inpcb *inp);

/* hooks for divert */
extern void (*ip_divert_ptr)(struct mbuf *m, int incoming);

extern int (*ip_dn_ctl_ptr)(struct sockopt *);
typedef int ip_fw_ctl_t(struct sockopt *);
extern ip_fw_ctl_t *ip_fw_ctl_ptr;


/* netgraph prototypes */
typedef int ng_ipfw_input_t(struct mbuf **, int, struct ip_fw_args *, int);
extern  ng_ipfw_input_t *ng_ipfw_input_p;

/* For kernel ipfw_ether and ipfw_bridge. */
struct ip_fw_args;

#define V_ip_fw_ctl_ptr         VNET(ip_fw_ctl_ptr)
#define	V_tcbinfo		VNET(tcbinfo)
#define	V_udbinfo		VNET(udbinfo)
#endif /* _KERNEL */

// sys/eventhandler.h
#define EVENTHANDLER_DECLARE(a, b)

/* application specific */
struct sess;
typedef int (handler_t)(struct sess *sess, void *arg);

/*
 * flags to control the callback
 * WANT_READ	select on read
 * WANT_WRITE	select on write
 * WANT_RUN	run unconditionally
 * WANT_DELETE	session is exiting
 */
enum flags_t {
        WANT_READ=1, WANT_WRITE=2, WANT_RUN=4,
        WANT_DELETE=0x8000
};

struct sess {
        struct sess *next;
        int fd;
        handler_t *func;
        void *arg;
        enum flags_t flags;
        void *private;  /* pointer managed by the session code */
};
struct sess *
new_session(int fd, handler_t *func, void *arg, enum flags_t flags);


void netmap_add_port(const char *dev);
#endif /* !_MISSING_H_ */
