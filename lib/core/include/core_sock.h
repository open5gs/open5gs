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

#define SOCK_F_BIND                 (1 << 0)
#define SOCK_F_CONNECT              (1 << 1)

#define SOCK_NTOP(__aDDR, __bUF) \
    __aDDR ? \
        ((struct sockaddr_in *)__aDDR)->sin_family == AF_INET ? \
            inet_ntop(AF_INET, &(((struct sockaddr_in *)__aDDR)->sin_addr), \
                __bUF, INET_ADDRSTRLEN) : \
        ((struct sockaddr_in6 *)__aDDR)->sin6_family == AF_INET6 ? \
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)__aDDR)->sin6_addr), \
                __bUF, INET6_ADDRSTRLEN) : "Unknown Family" : \
    "Null Address"

/**
 * @defgroup core_sockopt Socket option definitions
 * @{
 */
#define CORE_SO_LINGER        1    /**< Linger */
#define CORE_SO_KEEPALIVE     2    /**< Keepalive */
#define CORE_SO_DEBUG         4    /**< Debug */
#define CORE_SO_NONBLOCK      8    /**< Non-blocking IO */
#define CORE_SO_REUSEADDR     16   /**< Reuse addresses */
#define CORE_SO_SNDBUF        64   /**< Send buffer */
#define CORE_SO_RCVBUF        128  /**< Receive buffer */
#define CORE_SO_DISCONNECTED  256  /**< Disconnected */
#define CORE_TCP_NODELAY      512  /**< For SCTP sockets, this is mapped
                                   * to STCP_NODELAY internally.
                                   */
#define CORE_TCP_NOPUSH       1024 /**< No push */
#define CORE_RESET_NODELAY    2048 /**< This flag is ONLY set internally
                                   * when we set CORE_TCP_NOPUSH with
                                   * CORE_TCP_NODELAY set to tell us that
                                   * CORE_TCP_NODELAY should be turned on
                                   * again when NOPUSH is turned off
                                   */
#define CORE_INCOMPLETE_READ 4096  /**< Set on non-blocking sockets
				   * (timeout != 0) on which the
				   * previous read() did not fill a buffer
				   * completely.  the next apr_socket_recv() 
                                   * will first call select()/poll() rather than
				   * going straight into read().  (Can also
				   * be set by an application to force a
				   * select()/poll() call before the next
				   * read, in cases where the app expects
				   * that an immediate read would fail.)
				   */
#define CORE_INCOMPLETE_WRITE 8192 /**< like CORE_INCOMPLETE_READ, but for write
                                   * @see CORE_INCOMPLETE_READ
                                   */
#define CORE_IPV6_V6ONLY     16384 /**< Don't accept IPv4 connections on an
                                   * IPv6 listening socket.
                                   */
#define CORE_TCP_DEFER_ACCEPT 32768 /**< Delay accepting of new connections 
                                    * until data is available.
                                    * @see apr_socket_accept_filter
                                    */
#define CORE_SO_BROADCAST     65536 /**< Allow broadcast
                                    */
#define CORE_SO_FREEBIND     131072 /**< Allow binding to addresses not owned
                                    * by any interface
                                    */

typedef c_uintptr_t sock_id;
typedef int (*sock_handler)(sock_id sock, void *data);

CORE_DECLARE(status_t) sock_init(void);
CORE_DECLARE(status_t) sock_final(void);

CORE_DECLARE(status_t) sock_create(
        sock_id *id, int family, int type, int protocol, int flags);
CORE_DECLARE(status_t) sock_delete(sock_id id);

CORE_DECLARE(status_t) sock_opt_set(sock_id id, c_int32_t opt, c_int32_t on);

CORE_DECLARE(status_t) sock_bind(sock_id id,
        const char *host, c_uint16_t port);
CORE_DECLARE(status_t) sock_connect(sock_id id,
        const char *host, c_uint16_t port);

CORE_DECLARE(status_t) sock_listen(sock_id id);
CORE_DECLARE(status_t) sock_accept(sock_id *new, sock_id id);

CORE_DECLARE(ssize_t) sock_send(sock_id id,
        const void *buf, size_t len, int flags,
        const struct sockaddr *dest_addr, socklen_t addrlen);
CORE_DECLARE(ssize_t) sock_recv(sock_id id,
        void *buf, size_t len, int flags,
        struct sockaddr *src_addr, socklen_t *addrlen);

CORE_DECLARE(status_t) sock_register(sock_id id,
        sock_handler handler, void *data); 
CORE_DECLARE(status_t) sock_unregister(sock_id id);
CORE_DECLARE(int) sock_is_registered(sock_id id);

CORE_DECLARE(int) sock_select_loop(c_time_t timeout);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
