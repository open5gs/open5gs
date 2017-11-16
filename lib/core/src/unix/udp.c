#define TRACE_MODULE _udp

#include "core_debug.h"
#include "core_sock.h"

status_t udp_open(sock_id *new,
        int family,
        const char *local_host, c_uint16_t local_port,
        const char *remote_host, c_uint16_t remote_port,
        int flags)
{
    status_t rv;
    sock_id id;

    rv = sock_create(new, family, SOCK_DGRAM, IPPROTO_UDP, flags);
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

    return CORE_OK;
}
