#ifndef __CORE_NETWORK_H__
#define __CORE_NETWORK_H__

#include "core_errno.h"
#include "core_time.h"
#include "core_list.h"

#if HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup sock_option Socket option definitions
 * @{
 */
#define SOCK_O_LINGER           (1 << 0)  /**< Linger */
#define SOCK_O_KEEPALIVE        (1 << 1)  /**< Keepalive */
#define SOCK_O_DEBUG            (1 << 2)  /**< Debug */
#define SOCK_O_NONBLOCK         (1 << 3)  /**< Non-blocking IO */
#define SOCK_O_REUSEADDR        (1 << 4)  /**< Reuse addresses */
#define SOCK_O_SNDBUF           (1 << 5)  /**< Send buffer */
#define SOCK_O_RCVBUF           (1 << 6)  /**< Receive buffer */
#define SOCK_O_DISCONNECTED     (1 << 7)  /**< Disconnected */
#define SOCK_O_TCP_NODELAY      (1 << 8)  /**< For SCTP sockets, this is mapped
                                   * to STCP_NODELAY internally.
                                   */
#define SOCK_O_TCP_NOPUSH       (1 << 9)  /**< No push */
#define SOCK_O_RESET_NODELAY    (1 << 10) /**< This flag is ONLY set internally
                                   * when we set SOCK_O_TCP_NOPUSH with
                                   * SOCK_O_TCP_NODELAY set to tell us that
                                   * SOCK_O_TCP_NODELAY should be turned on
                                   * again when NOPUSH is turned off
                                   */
#define SOCK_O_INCOMPLETE_READ  (1 << 11) /**< Set on non-blocking sockets
                                   * (timeout != 0) on which the
                                   * previous read() did not fill a buffer
                                   * completely.  the next sock_read() 
                                   * will first call select()/poll() rather than
                                   * going straight into read().  (Can also
                                   * be set by an application to force a
                                   * select()/poll() call before the next
                                   * read, in cases where the app expects
                                   * that an immediate read would fail.)
                                   */
#define SOCK_O_INCOMPLETE_WRITE (1 << 12) /**< like SOCK_O_INCOMPLETE_READ, but
                                   * for write
                                   * @see SOCK_O_INCOMPLETE_READ
                                   */
#define SOCK_O_IPV6_V6ONLY      (1 << 13) /**< Don't accept IPv4 connections
                                   * on a IPv6 listening socket.
                                   */
#define SOCK_O_TCP_DEFER_ACCEPT (1 << 14) /**< Delay accepting of new
                                    * connections until data is available.
                                    * @see apr_socket_accept_filter
                                    */
#define SOCK_O_BROADCAST        (1 << 15) /**< Allow broadcast */
#define SOCK_O_FREEBIND         (1 << 16) /**< Allow binding to addresses not
                                    * owned by any interface
                                    */

typedef c_uintptr_t sock_id;
typedef int (*sock_handler)(sock_id sock, void *data);

typedef struct c_sockaddr_t c_sockaddr_t;
struct c_sockaddr_t {
    /* Reserved Area
     *   - Should not add any atrribute in this area.
     *
     *   e.g) 
     *   struct sockaddr addr;
     *   ...
     *   sockaddr_len((c_sockaddr_t *)&addr);
     */
#define c_sa_family sa.sa_family
#define c_sa_port sin.sin_port
    union {
        struct sockaddr_storage ss;
        struct sockaddr_in sin;
        struct sockaddr_in6 sin6;
        struct sockaddr sa;
    };

    /* User Area
     *   - Could add your attribute.
     */
    c_sockaddr_t *next;
};

typedef struct _sock_node_t {
    lnode_t node;

    sock_id sock;
    c_sockaddr_t *list;
} sock_node_t;

typedef struct ipsubnet_t {
    int family;

    c_uint32_t sub[4]; /* big enough for IPv4 and IPv6 addresses */
    c_uint32_t mask[4];
} ipsubnet_t;

typedef struct _sctp_info_t {
    c_uint32_t ppid;
    c_uint16_t stream_no;
    c_uint16_t inbound_streams;
    c_uint16_t outbound_streams;
} sctp_info_t;

/*
 * Init/Final
 */
CORE_DECLARE(status_t) network_init(void);
CORE_DECLARE(status_t) network_final(void);

/*
 * Socket
 */
CORE_DECLARE(status_t) sock_create(sock_id *new);
CORE_DECLARE(status_t) sock_delete(sock_id id);
CORE_DECLARE(status_t) sock_delete_list(list_t *list);

CORE_DECLARE(status_t) sock_socket(
        sock_id *id, int family, int type, int protocol);
CORE_DECLARE(status_t) sock_setsockopt(sock_id id, c_int32_t opt, c_int32_t on);
CORE_DECLARE(status_t) sock_bind(sock_id id, c_sockaddr_t *addr);
CORE_DECLARE(status_t) sock_connect(sock_id id, c_sockaddr_t *addr);

CORE_DECLARE(status_t) sock_listen(sock_id id);
CORE_DECLARE(status_t) sock_accept(sock_id *new, sock_id id);

CORE_DECLARE(int) sock_family(sock_id id);
CORE_DECLARE(c_sockaddr_t *) sock_local_addr(sock_id id);
CORE_DECLARE(c_sockaddr_t *) sock_remote_addr(sock_id id);

/*
 * Socket Node
 */
CORE_DECLARE(status_t) sock_add_node(
        list_t *list, sock_node_t **node, c_sockaddr_t *sa_list, int family);
CORE_DECLARE(status_t) sock_remove_node(list_t *list, sock_node_t *node);
CORE_DECLARE(status_t) sock_remove_all_nodes(list_t *list);

CORE_DECLARE(status_t) sock_probe_node(
        list_t *list, list_t *list6, const char *dev, c_uint16_t port);
CORE_DECLARE(status_t) sock_fill_scope_id_in_local(c_sockaddr_t *sa_list);

/*
 * Socket Address
 */
CORE_DECLARE(status_t) core_getaddrinfo(c_sockaddr_t **sa_list, 
        int family, const char *hostname, c_uint16_t port, int flags);
CORE_DECLARE(status_t) core_freeaddrinfo(c_sockaddr_t *sa_list);

CORE_DECLARE(status_t) core_addaddrinfo(c_sockaddr_t **sa_list, 
        int family, const char *hostname, c_uint16_t port, int flags);
CORE_DECLARE(status_t) core_copyaddrinfo(
        c_sockaddr_t **dst, const c_sockaddr_t *src);
CORE_DECLARE(status_t) core_filteraddrinfo(c_sockaddr_t **sa_list, int family);
CORE_DECLARE(status_t) core_sortaddrinfo(c_sockaddr_t **sa_list, int family);
CORE_DECLARE(c_sockaddr_t *) core_link_local_addr_by_dev(const char *dev);

#define CORE_ADDRSTRLEN INET6_ADDRSTRLEN
#define CORE_ADDR(__aDDR, __bUF) \
    core_inet_ntop(__aDDR, buf, CORE_ADDRSTRLEN)
#define CORE_PORT(__aDDR) \
    ntohs((__aDDR)->c_sa_port)

#define INET_NTOP(src, dst) \
    inet_ntop(AF_INET,(void *)(c_uintptr_t)(src),(dst),INET_ADDRSTRLEN)
#define INET6_NTOP(src, dst) \
    inet_ntop(AF_INET6,(void *)(src),(dst),INET6_ADDRSTRLEN)
CORE_DECLARE(const char *)core_inet_ntop(void *addr, char *buf, int buflen);
CORE_DECLARE(status_t) core_inet_pton(int family, const char *src, void *addr);

CORE_DECLARE(socklen_t) sockaddr_len(const void *addr);
CORE_DECLARE(int) sockaddr_is_equal(void *p, void *q);

CORE_DECLARE(status_t) core_ipsubnet(
        ipsubnet_t *ipsub, const char *ipstr, const char *mask_or_numbits);
/*
 * UDP Socket
 */
CORE_DECLARE(status_t) udp_socket(sock_id *new, int family);
CORE_DECLARE(status_t) udp_server(sock_id *new, c_sockaddr_t *sa_list);
CORE_DECLARE(status_t) udp_client(sock_id *new, c_sockaddr_t *sa_list);
CORE_DECLARE(status_t) udp_connect(sock_id id, c_sockaddr_t *sa_list);

/*
 * TCP Socket
 */
CORE_DECLARE(status_t) tcp_server(sock_id *new, c_sockaddr_t *sa_list);
CORE_DECLARE(status_t) tcp_client(sock_id *new, c_sockaddr_t *sa_list);

/*
 * SCTP Socket
 */
CORE_DECLARE(void) sctp_set_num_ostreams(int sctp_streams);

CORE_DECLARE(status_t) sctp_socket(sock_id *new, int family, int type);
CORE_DECLARE(status_t) sctp_server(sock_id *new,
        int type, c_sockaddr_t *sa_list);
CORE_DECLARE(status_t) sctp_client(sock_id *new, 
        int type, c_sockaddr_t *sa_list);
CORE_DECLARE(status_t) sctp_connect(sock_id id, c_sockaddr_t *sa_list);
CORE_DECLARE(int) core_sctp_sendmsg(sock_id id, const void *msg, size_t len,
        c_sockaddr_t *to, c_uint32_t ppid, c_uint16_t stream_no);
CORE_DECLARE(int) core_sctp_recvmsg(sock_id id, void *msg, size_t len,
        c_sockaddr_t *from, sctp_info_t *sinfo, int *msg_flags);
CORE_DECLARE(int) core_sctp_recvdata(sock_id id, void *msg, size_t len,
        c_sockaddr_t *from, sctp_info_t *sinfo);
/*
 * TUN Driver
 */
CORE_DECLARE(status_t) tun_open(sock_id *new,
        char *ifname, int is_tap);
CORE_DECLARE(status_t) tun_set_ip(sock_id id, ipsubnet_t *gw, ipsubnet_t *sub);

/*
 * Send/Recv
 */
CORE_DECLARE(ssize_t) sock_write(sock_id id, const void *buf, size_t len);
CORE_DECLARE(ssize_t) sock_read(sock_id id, void *buf, size_t len);

CORE_DECLARE(ssize_t) core_send(sock_id id,
        const void *buf, size_t len, int flags);
CORE_DECLARE(ssize_t) core_sendto(sock_id id,
        const void *buf, size_t len, int flags, const c_sockaddr_t *to);
CORE_DECLARE(ssize_t) core_recv(sock_id id, void *buf, size_t len, int flags);
CORE_DECLARE(ssize_t) core_recvfrom(sock_id id,
        void *buf, size_t len, int flags, c_sockaddr_t *from);

/*
 * Select Loop
 */
CORE_DECLARE(status_t) sock_register(sock_id id,
        sock_handler handler, void *data); 
CORE_DECLARE(status_t) sock_unregister(sock_id id);
CORE_DECLARE(int) sock_is_registered(sock_id id);
CORE_DECLARE(int) sock_select_loop(c_time_t timeout);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
