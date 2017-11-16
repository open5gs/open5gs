#ifndef __CORE_SOCK_H__
#define __CORE_SOCK_H__

#include "core_errno.h"
#include "core_time.h"

#if HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @defgroup sock_flags Socket flags definitions
 * @{
 */
#define SOCK_F_BIND             (1 << 0)
#define SOCK_F_CONNECT          (1 << 1)

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
typedef union _c_sockaddr_t {
    struct sockaddr_storage ss;
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;
    struct sockaddr sa;
} c_sockaddr_t;

CORE_DECLARE(status_t) sock_init(void);
CORE_DECLARE(status_t) sock_final(void);

CORE_DECLARE(status_t) sock_create(
        sock_id *id, int family, int type, int protocol, int flags);
CORE_DECLARE(status_t) sock_delete(sock_id id);

CORE_DECLARE(status_t) sock_bind(sock_id id,
        const char *host, c_uint16_t port);
CORE_DECLARE(status_t) sock_connect(sock_id id,
        const char *host, c_uint16_t port);

CORE_DECLARE(status_t) sock_listen(sock_id id);
CORE_DECLARE(status_t) sock_accept(sock_id *new, sock_id id);

CORE_DECLARE(ssize_t) sock_write(sock_id id,
        const void *buf, size_t len, int flags,
        const c_sockaddr_t *dest_addr, socklen_t addrlen);
CORE_DECLARE(ssize_t) sock_read(sock_id id,
        void *buf, size_t len, int flags,
        c_sockaddr_t *src_addr, socklen_t *addrlen);

#define CORE_ADDRSTRLEN INET6_ADDRSTRLEN
#define SOCK_NTOP(__aDDR, __bUF) \
    sock_ntop((c_sockaddr_t *)__aDDR, buf, CORE_ADDRSTRLEN)
CORE_DECLARE(const char *)sock_ntop(c_sockaddr_t *sockaddr,
        char *buf, int buflen);
CORE_DECLARE(status_t) sock_pton(const char *hostname, c_uint16_t port,
        c_sockaddr_t *sockaddr);
CORE_DECLARE(socklen_t) sock_len(c_sockaddr_t *sockaddr);

CORE_DECLARE(status_t) sock_setsockopt(sock_id id, c_int32_t opt, c_int32_t on);

CORE_DECLARE(status_t) sock_register(sock_id id,
        sock_handler handler, void *data); 
CORE_DECLARE(status_t) sock_unregister(sock_id id);
CORE_DECLARE(int) sock_is_registered(sock_id id);

CORE_DECLARE(int) sock_select_loop(c_time_t timeout);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
