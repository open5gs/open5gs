#define TRACE_MODULE _udp

#include "core_debug.h"
#include "core_arch_network.h"

status_t udp_socket(sock_id *new, int family)
{
    status_t rv;

    rv = sock_socket(new, family, SOCK_DGRAM, IPPROTO_UDP);
    d_assert(rv == CORE_OK && new, return CORE_ERROR,);

    d_trace(1, "udp socket(%d)\n", family);

    return CORE_OK;
}

status_t udp_server(sock_id *new, c_sockaddr_t *addr)
{
    status_t rv;
    c_sockaddr_t *iter;
    char buf[CORE_ADDRSTRLEN];

    d_assert(new, return CORE_ERROR,);
    d_assert(addr, return CORE_ERROR,);

    iter = addr;
    while(iter)
    {
        rv = udp_socket(new, iter->c_sa_family);
        if (rv != CORE_OK) continue;
        
        d_assert(sock_setsockopt(*new, SOCK_O_REUSEADDR, 1) == CORE_OK,
                return CORE_ERROR,
                "setsockopt [%s]:%d failed(%d:%s)",
                CORE_ADDR(iter, buf), CORE_PORT(iter), errno, strerror(errno));

        if (sock_bind(*new, iter) == CORE_OK)
        {
            d_trace(1, "udp_server() [%s]:%d\n",
                    CORE_ADDR(iter, buf), CORE_PORT(iter));
            break;
        }

        rv = sock_delete(*new);
        d_assert(rv == CORE_OK, return CORE_ERROR,);

        iter = iter->next;
    }

    if (iter == NULL)
    {
        d_error("udpserver() [%s]:%d failed(%d:%s)",
                CORE_ADDR(addr, buf), CORE_PORT(addr), errno, strerror(errno));
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t udp_client(sock_id *new, c_sockaddr_t *addr)
{
    status_t rv;
    c_sockaddr_t *iter;
    char buf[CORE_ADDRSTRLEN];

    d_assert(new, return CORE_ERROR,);
    d_assert(addr, return CORE_ERROR,);

    iter = addr;
    while(iter)
    {
        rv = udp_socket(new, iter->c_sa_family);
        if (rv != CORE_OK) continue;
        
        if (sock_connect(*new, iter) == CORE_OK)
        {
            d_trace(1, "udp_client() [%s]:%d\n",
                    CORE_ADDR(iter, buf), CORE_PORT(iter));
            break;
        }

        rv = sock_delete(*new);
        d_assert(rv == CORE_OK, return CORE_ERROR,);

        iter = iter->next;
    }

    if (iter == NULL)
    {
        d_error("udp_client() [%s]:%d failed(%d:%s)",
                CORE_ADDR(addr, buf), CORE_PORT(addr), errno, strerror(errno));
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t udp_connect(sock_id id, c_sockaddr_t *addr)
{
    c_sockaddr_t *iter;
    char buf[CORE_ADDRSTRLEN];

    d_assert(id, return CORE_ERROR,);
    d_assert(addr, return CORE_ERROR,);

    iter = addr;
    while(iter)
    {
        if (sock_connect(id, iter) == CORE_OK)
        {
            d_trace(1, "udp_connect() [%s]:%d\n",
                    CORE_ADDR(iter, buf), CORE_PORT(iter));
            break;
        }

        iter = iter->next;
    }

    if (iter == NULL)
    {
        d_error("udp_connect() [%s]:%d failed(%d:%s)",
                CORE_ADDR(addr, buf), CORE_PORT(addr), errno, strerror(errno));
        return CORE_ERROR;
    }

    return CORE_OK;
}
