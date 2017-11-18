#define TRACE_MODULE _udp

#include "core_debug.h"
#include "core_arch_network.h"

status_t udp_socket(sock_id *new, int family)
{
    status_t rv;

    rv = sock_create(new, family, SOCK_DGRAM, IPPROTO_UDP);
    d_assert(rv == CORE_OK && new, return CORE_ERROR,);

    d_trace(1, "udp socket(%d)\n", family);

    return CORE_OK;
}

status_t udp_server(sock_id *new,
        int family, const char *hostname, c_uint16_t port)
{
    status_t rv;
    c_sockaddr_t *sa;
    sock_t *sock = NULL;
    char buf[CORE_ADDRSTRLEN];

    rv = core_getaddrinfo(&sa, family, hostname, port, AI_PASSIVE);
    d_assert(rv == CORE_OK && sa, return CORE_ERROR,);

    while(sa)
    {
        rv = udp_socket(new, sa->sa.sa_family);
        if (rv != CORE_OK) continue;
        
        sock = (sock_t *)*new;

        d_assert(sock_setsockopt(*new, SOCK_O_REUSEADDR, 1) == CORE_OK,
                return CORE_ERROR,
                "setsockopt(%s:%d) failed(%d:%s)",
                CORE_NTOP(sa, buf), port, errno, strerror(errno));

        if (bind(sock->fd, &sa->sa, sa->sa_len) == 0)
        {
            d_trace(1, "udp bind %s:%d\n", CORE_NTOP(sa, buf), port);
            break;
        }

        rv = sock_delete(*new);
        d_assert(rv == CORE_OK, return CORE_ERROR,);

        sa = sa->next;
    }

    if (sa == NULL)
    {
        d_error("udp bind(%d:%s:%d) failed(%d:%s)",
                family, hostname, port, errno, strerror(errno));
        return CORE_ERROR;
    }

    rv = core_freeaddrinfo(sa);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    return CORE_OK;
}

status_t udp_client(sock_id *new,
        int family, const char *hostname, c_uint16_t port)
{
    status_t rv;
    c_sockaddr_t *sa;
    sock_t *sock = NULL;
    char buf[CORE_ADDRSTRLEN];

    rv = core_getaddrinfo(&sa, family, hostname, port, 0);
    d_assert(rv == CORE_OK && sa, return CORE_ERROR,);

    while(sa)
    {
        rv = udp_socket(new, sa->sa.sa_family);
        if (rv != CORE_OK) continue;
        
        sock = (sock_t *)*new;

        if (connect(sock->fd, &sa->sa, sa->sa_len) == 0)
        {
            d_trace(1, "udp connect %s:%d\n", CORE_NTOP(sa, buf), port);
            break;
        }

        rv = sock_delete(*new);
        d_assert(rv == CORE_OK, return CORE_ERROR,);

        sa = sa->next;
    }

    if (sa == NULL)
    {
        d_error("udp connect(%d:%s:%d) failed(%d:%s)",
                sock->family, hostname, port, errno, strerror(errno));
        return CORE_ERROR;
    }

    rv = core_freeaddrinfo(sa);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    return CORE_OK;
}
