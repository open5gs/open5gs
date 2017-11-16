#define TRACE_MODULE _sock

#include "core_debug.h"
#include "core_arch_sock.h"

#if HAVE_NETDB_H
#include <netdb.h>
#endif

#if HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#define MAX_SOCK_POOL_SIZE          512

static int max_fd;
static list_t fd_list;
static fd_set read_fds;

index_declare(sock_pool, sock_t, MAX_SOCK_POOL_SIZE);

static status_t sononblock(int sd);
static status_t soblock(int sd);
static struct addrinfo *resolver(
        int family, int type, int protocol,
        const char *host, c_uint16_t port, int flags);
static void set_fds(fd_set *fds);
static void fd_dispatch(fd_set *fds);

status_t sock_init(void)
{
    index_init(&sock_pool, MAX_SOCK_POOL_SIZE);

    max_fd = 0;
    list_init(&fd_list);
    memset(&read_fds, 0, sizeof(fd_set));

    return CORE_OK;
}
status_t sock_final(void)
{
    index_final(&sock_pool);

    return CORE_OK;
}

status_t sock_create(sock_id *new,
        int family, int type, int protocol, int flags)
{
    sock_t *sock = NULL;

    index_alloc(&sock_pool, &sock);
    d_assert(sock, return CORE_ENOMEM,);
    memset(sock, 0, sizeof(sock_t));

    sock->family = family;
    sock->type = type;
    sock->protocol = protocol;
    sock->flags = flags;

    sock->fd = -1;

    if (!flags)
    {
        sock->fd = socket(sock->family, sock->type, sock->protocol);
        if (sock->fd < 0)
        {
            d_error("socket create(%d:%d:%d) failed(%d:%s)",
                    sock->family, sock->type, sock->protocol,
                    errno, strerror(errno));
            return CORE_ERROR;
        }


        d_trace(1, "socket create(%d:%d:%d)\n",
                sock->family, sock->type, sock->protocol);
    }

    *new = (sock_id)sock;

    return CORE_OK;
}

status_t sock_delete(sock_id id)
{
    sock_t *sock = (sock_t *)id;
    d_assert(id, return CORE_ERROR,);

    if (sock_is_registered(id))
        sock_unregister(id);
    if (sock->fd >= 0)
        close(sock->fd);
    sock->fd = -1;

    index_free(&sock_pool, sock);
    return CORE_OK;
}

status_t sock_bind(sock_id id, const char *host, c_uint16_t port)
{
    sock_t *sock = (sock_t *)id;
    struct addrinfo *result, *rp;
    char buf[CORE_ADDRSTRLEN];

    d_assert(sock, return CORE_ERROR,);
    d_assert(sock->flags & SOCK_F_BIND, return CORE_ERROR,);

    result = resolver(
            sock->family, sock->type, sock->protocol, host, port, AI_PASSIVE);
    d_assert(result, return CORE_ERROR,);

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sock->fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sock->fd < 0)
            continue;

        if (sock_setsockopt(id, SOCK_O_REUSEADDR, 1) == CORE_ERROR)
        {
            d_error("setsockopt(%s:%d) failed(%d:%s)",
                    host, port, errno, strerror(errno));
            break;
        }

        if (bind(sock->fd, rp->ai_addr, rp->ai_addrlen) == 0)
        {
            d_trace(1, "socket bind %s:%d\n",
                    SOCK_NTOP(rp->ai_addr, buf), port);
            break;
        }
        close(sock->fd);
        sock->fd = -1;
    }
    freeaddrinfo(result);

    if (rp == NULL)
    {
        d_error("socket bind(%d:%d) failed(%d:%s)",
                host, port, errno, strerror(errno));
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t sock_connect(sock_id id, const char *host, c_uint16_t port)
{
    sock_t *sock = (sock_t *)id;
    int rc;
    struct addrinfo *result, *rp;
    char buf[CORE_ADDRSTRLEN];

    d_assert(id, return CORE_ERROR,);
    d_assert(sock->flags & SOCK_F_CONNECT, return CORE_ERROR,);

    result = resolver(
            sock->family, sock->type, sock->protocol, host, port, 0);
    d_assert(result, return CORE_ERROR,);

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        if (sock->fd < 0)
        {
            sock->fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if (sock->fd < 0)
                continue;
        }

        rc = connect(sock->fd, rp->ai_addr, rp->ai_addrlen);
        if (rc == 0 || (rc != 0 && errno == EINPROGRESS))
        {
            d_trace(1, "socket connect %s:%d\n",
                    SOCK_NTOP(rp->ai_addr, buf), port);
            break;
        }

        close(sock->fd);
        sock->fd = -1;
    }
    freeaddrinfo(result);

    if (rp == NULL)
    {
        d_error("connect(%d:%d) failed(%d:%s)",
                host, port, errno, strerror(errno));
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t sock_listen(sock_id id)
{
    int rc;
    sock_t *sock = (sock_t *)id;
    d_assert(sock, return CORE_ERROR,);

    rc = listen(sock->fd, 5);
    if (rc < 0)
    {
        d_error("listen failed(%d:%s)", errno, strerror(errno));
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t sock_accept(sock_id *new, sock_id id)
{
    sock_t *sock = (sock_t *)id;
    status_t rv;
    sock_t *remote_sock = NULL;
    int remote_fd = -1;

    d_assert(id, return CORE_ERROR,);
    d_assert(sock->flags & SOCK_F_BIND, return CORE_ERROR,);

    remote_fd = accept(sock->fd, NULL, NULL);
    if (remote_fd < 0)
    {
        d_error("accept failed(%d:%s)", errno, strerror(errno));
        return CORE_ERROR;
    }

    rv = sock_create(new,
        sock->family, sock->type, sock->protocol, sock->flags);
    d_assert(rv == CORE_OK && (*new), return CORE_ERROR,);
    remote_sock = (sock_t *)(*new);
    remote_sock->fd = remote_fd;

    return CORE_OK;
}

ssize_t sock_write(sock_id id, const void *buf, size_t len, int flags,
        const c_sockaddr_t *dest_addr, socklen_t addrlen)
{
    sock_t *sock = (sock_t *)id;
    ssize_t size;

    d_assert(id, return -1, );
    
    if (sock->type == SOCK_DGRAM && !(sock->flags & SOCK_F_CONNECT))
    {
        d_assert(dest_addr, return -1,);
        d_assert(addrlen, return -1,);
        size = sendto(sock->fd, buf, len, flags, &dest_addr->sa, addrlen);
    }
    else
    {
        size = send(sock->fd, buf, len, flags);
    }

    if (size < 0)
    {
        d_error("sock_write(len:%ld) failed(%d:%s)",
                len, errno, strerror(errno));
    }

    return size;
}

ssize_t sock_read(sock_id id, void *buf, size_t len, int flags,
        c_sockaddr_t *src_addr, socklen_t *addrlen)
{
    sock_t *sock = (sock_t *)id;
    ssize_t size;

    d_assert(id, return -1,);

    if (sock->type == SOCK_DGRAM && !(sock->flags & SOCK_F_CONNECT))
    {
        if (addrlen)
            *addrlen = sizeof(c_sockaddr_t);
        size = recvfrom(sock->fd, buf, len, flags, &src_addr->sa, addrlen);
    }
    else
    {
        size = recv(sock->fd, buf, len, flags);
    }

    if (size < 0)
    {
        d_error("sock_read(len:%ld) failed(%d:%s)",
                len, errno, strerror(errno));
    }

    return size;
}

status_t sock_register(sock_id id, sock_handler handler, void *data) 
{
    sock_t *sock = (sock_t *)id;

    d_assert(id, return CORE_ERROR,);

    if (sock_is_registered(id))
    {
        d_error("socket has already been registered");
        return CORE_ERROR;
    }

    if (sock_setsockopt(id, SOCK_O_NONBLOCK, 1) == CORE_ERROR)
    {
        d_error("cannot set socket to non-block");
        return CORE_ERROR;
    }

    if (sock->fd > max_fd)
    {
        max_fd = sock->fd;
    }
    sock->handler = handler;
    sock->data = data;

    list_append(&fd_list, sock);

    return CORE_OK;
}

status_t sock_unregister(sock_id id)
{
    d_assert(id, return CORE_ERROR,);

    list_remove(&fd_list, id);

    return CORE_OK;
}

int sock_is_registered(sock_id id)
{
    sock_t *sock = (sock_t *)id;
    sock_t *iter = NULL;

    d_assert(id, return CORE_ERROR,);
    for (iter = list_first(&fd_list); iter != NULL; iter = list_next(iter))
    {
        if (iter->index == sock->index)
        {
            return 1;
        }
    }

    return 0;
}

int sock_select_loop(c_time_t timeout)
{
    struct timeval tv;
    int rc;

    if (timeout > 0)
    {
        tv.tv_sec = time_sec(timeout);
        tv.tv_usec = time_usec(timeout);
    }

    set_fds(&read_fds);

    rc = select(max_fd + 1, &read_fds, NULL, NULL, timeout > 0 ? &tv : NULL);
    if (rc < 0)
    {
        if (errno != EINTR && errno != 0)
        {
            if (errno == EBADF)
            {
                d_error("[FIXME] socket should be closed here(%d:%s)", 
                        errno, strerror(errno));
            }
            else
            {
                d_error("select failed(%d:%s)", errno, strerror(errno));
            }
        }
        return rc;
    }

    /* Timeout */
    if (rc == 0)
    {
        return rc;
    }

    /* Dispatch Handler */
    fd_dispatch(&read_fds);

    return 0;
}

const char *sock_ntop(c_sockaddr_t *sockaddr, char *buf, int buflen)
{
    int family;
    d_assert(buf, return NULL,);
    d_assert(sockaddr, return NULL,);

    family = sockaddr->sa.sa_family;
    switch(family)
    {
        case AF_INET:
            d_assert(buflen >= INET_ADDRSTRLEN, return NULL,);
            return inet_ntop(family,
                    &sockaddr->sin.sin_addr, buf, INET_ADDRSTRLEN);
        case AF_INET6:
            d_assert(buflen >= CORE_ADDRSTRLEN, return NULL,);
            return inet_ntop(family,
                    &sockaddr->sin6.sin6_addr, buf, INET6_ADDRSTRLEN);
        default:
            d_assert(0, return NULL,);
    }
}

status_t sock_pton(const char *hostname, c_uint16_t port,
        c_sockaddr_t *sockaddr)
{
    struct addrinfo *result, *rp;

    d_assert(sockaddr, return CORE_ERROR,);

    result = resolver(AF_UNSPEC, SOCK_STREAM, 0,
           hostname, port, hostname ? 0 : AI_PASSIVE);
    d_assert(result, return CORE_ERROR,);

    memset(sockaddr, 0, sizeof(c_sockaddr_t));
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        if (rp->ai_family != AF_INET && rp->ai_family != AF_INET6)
            continue;

        sockaddr->sa.sa_family = rp->ai_family;
        memcpy(&sockaddr->sa, rp->ai_addr, rp->ai_addrlen);
        if (rp->ai_family == AF_INET)
            sockaddr->sin.sin_port = htons(port);
        else if (rp->ai_family == AF_INET6)
            sockaddr->sin6.sin6_port = htons(port);
        break;
    }
    freeaddrinfo(result);

    if (rp == NULL)
    {
        d_error("sock_pton(%d:%d) failed(%d:%s)",
                hostname, port, errno, strerror(errno));
        return CORE_ERROR;
    }

    return CORE_OK;
}

socklen_t sock_len(c_sockaddr_t *sockaddr)
{
    d_assert(sockaddr, return CORE_ERROR,);
    d_assert(sockaddr->sa.sa_family == AF_INET ||
            sockaddr->sa.sa_family == AF_INET6, return CORE_ERROR,);

    if (sockaddr->sa.sa_family == AF_INET)
        return sizeof(struct sockaddr_in);
    else if (sockaddr->sa.sa_family == AF_INET6)
        return sizeof(struct sockaddr_in6);
    else
        d_assert(0, return CORE_ERROR,);
}

status_t sock_setsockopt(sock_id id, c_int32_t opt, c_int32_t on)
{
    sock_t *sock = (sock_t *)id;
    int one;
    status_t rv;

    d_assert(sock, return CORE_ERROR,);
    if (on)
        one = 1;
    else
        one = 0;

    switch(opt)
    {
        case SOCK_O_REUSEADDR:
            if (on != sock_is_option_set(sock, SOCK_O_REUSEADDR))
            {
                if (setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR,
                            (void *)&one, sizeof(int)) == -1)
                {
                    return errno;
                }
                sock_set_option(sock, SOCK_O_REUSEADDR, on);
            }
            break;
        case SOCK_O_NONBLOCK:
            if (sock_is_option_set(sock, SOCK_O_NONBLOCK) != on)
            {
                if (on)
                {
                    if ((rv = sononblock(sock->fd)) != CORE_OK) 
                        return rv;
                }
                else
                {
                    if ((rv = soblock(sock->fd)) != CORE_OK)
                        return rv;
                }
                sock_set_option(sock, SOCK_O_NONBLOCK, on);
            }
            break;
        default:
            d_error("Not implemented(%d)", opt);
            return CORE_EINVAL;
    }

    return CORE_OK; 
}         
static status_t soblock(int sd)
{
/* BeOS uses setsockopt at present for non blocking... */
#ifndef BEOS
    int fd_flags;

    fd_flags = fcntl(sd, F_GETFL, 0);
#if defined(O_NONBLOCK)
    fd_flags &= ~O_NONBLOCK;
#elif defined(O_NDELAY)
    fd_flags &= ~O_NDELAY;
#elif defined(FNDELAY)
    fd_flags &= ~FNDELAY;
#else
#error Please teach CORE how to make sockets blocking on your platform.
#endif
    if (fcntl(sd, F_SETFL, fd_flags) == -1)
    {
        return errno;
    }
#else
    int on = 0;
    if (setsockopt(sd, SOL_SOCKET, SO_NONBLOCK, &on, sizeof(int)) < 0)
        return errno;
#endif /* BEOS */
    return CORE_OK;
}

static status_t sononblock(int sd)
{
#ifndef BEOS
    int fd_flags;

    fd_flags = fcntl(sd, F_GETFL, 0);
#if defined(O_NONBLOCK)
    fd_flags |= O_NONBLOCK;
#elif defined(O_NDELAY)
    fd_flags |= O_NDELAY;
#elif defined(FNDELAY)
    fd_flags |= FNDELAY;
#else
#error Please teach CORE how to make sockets non-blocking on your platform.
#endif
    if (fcntl(sd, F_SETFL, fd_flags) == -1)
    {
        return errno;
    }
#else
    int on = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_NONBLOCK, &on, sizeof(int)) < 0)
        return errno;
#endif /* BEOS */
    return CORE_OK;
}

static struct addrinfo *resolver(int family, int type, int protocol,
        const char *host, c_uint16_t port, int flags)
{
    int rc;
    char service[16];
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = family;
    if (type == SOCK_RAW)
    {
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;
    }
    else
    {
        hints.ai_socktype = type;
        hints.ai_protocol = protocol;
    }
    hints.ai_flags = flags;

    snprintf(service, sizeof(service), "%u", port);

    rc = getaddrinfo(host, service, &hints, &res);
    if (rc != 0)
    {
        d_error("getaddrinfo(%s:%d) failed(%d:%s)",
                host, port, errno, strerror(errno));
        return NULL;
    }

    return res;
}

static void set_fds(fd_set *fds)
{
    sock_t *sock = NULL;

    FD_ZERO(fds);
    for (sock = list_first(&fd_list); sock != NULL; sock = list_next(sock))
    {
        FD_SET(sock->fd, fds);
    }
}

static void fd_dispatch(fd_set *fds)
{
    sock_t *sock = NULL;

    for (sock = list_first(&fd_list); sock != NULL; sock = list_next(sock))
    {
        if (FD_ISSET(sock->fd, fds))
        {
            if (sock->handler)
            {
                sock->handler((sock_id)sock, sock->data);
            }
        }
    }
}
