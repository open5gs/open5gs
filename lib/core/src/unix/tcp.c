#define TRACE_MODULE _tcp

#include "core_debug.h"
#include "core_udp.h"

status_t tcp_create(sock_id *new,
        int family,
        const char *local_host, c_uint16_t local_port,
        const char *remote_host, c_uint16_t remote_port,
        int flags)
{
    status_t rv;
    sock_id id;

    rv = sock_create(new, family, SOCK_STREAM, IPPROTO_TCP, flags);
    d_assert(new, return CORE_ERROR,);
    id = *new;

    if (flags & SOCK_F_BIND)
    {
        rv = sock_bind(id, local_host, local_port);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
    }

    if (flags & SOCK_F_CONNECT)
    {
        rv = sock_connect(id, remote_host, remote_port);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
    }

    if (flags & SOCK_F_BIND)
    {
        rv = sock_listen(id);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
    }

    return CORE_OK;
}
