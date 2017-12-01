#define TRACE_MODULE _tcp

#include "core_debug.h"
#include "core_arch_network.h"

status_t tcp_server(sock_id *new, c_sockaddr_t *addr)
{
    status_t rv;
    c_sockaddr_t *iter;
    char buf[CORE_ADDRSTRLEN];

    d_assert(new, return CORE_ERROR,);
    d_assert(addr, return CORE_ERROR,);

    iter = addr;
    while(iter)
    {
        rv = sock_socket(new, iter->c_sa_family, SOCK_STREAM, IPPROTO_TCP);
        if (rv != CORE_OK) continue;
        
        d_assert(sock_setsockopt(*new, SOCK_O_REUSEADDR, 1) == CORE_OK,
                return CORE_ERROR,
                "setsockopt [%s]:%d failed(%d:%s)",
                CORE_ADDR(iter, buf), CORE_PORT(iter), errno, strerror(errno));

        if (sock_bind(*new, iter) == CORE_OK)
        {
            d_trace(1, "tcp_server() [%s]:%d\n",
                    CORE_ADDR(iter, buf), CORE_PORT(iter));
            break;
        }

        rv = sock_delete(*new);
        d_assert(rv == CORE_OK, return CORE_ERROR,);

        iter = iter->next;
    }

    if (iter == NULL)
    {
        d_error("tcp_server() [%s]:%d failed(%d:%s)",
                CORE_ADDR(addr, buf), CORE_PORT(addr), errno, strerror(errno));
        return CORE_ERROR;
    }

    rv = sock_listen(*new);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    return CORE_OK;
}

status_t tcp_client(sock_id *new, c_sockaddr_t *addr)
{
    status_t rv;
    c_sockaddr_t *iter;
    char buf[CORE_ADDRSTRLEN];

    d_assert(new, return CORE_ERROR,);
    d_assert(addr, return CORE_ERROR,);

    iter = addr;
    while(iter)
    {
        rv = sock_socket(new, iter->c_sa_family, SOCK_STREAM, IPPROTO_TCP);
        if (rv != CORE_OK) continue;
        
        if (sock_connect(*new, iter) == CORE_OK)
        {
            d_trace(1, "tcp_client() [%s]:%d\n",
                    CORE_ADDR(iter, buf), CORE_PORT(iter));
            break;
        }

        rv = sock_delete(*new);
        d_assert(rv == CORE_OK, return CORE_ERROR,);

        iter = iter->next;
    }

    if (iter == NULL)
    {
        d_error("tcp_client() [%s]:%d failed(%d:%s)",
                CORE_ADDR(addr, buf), CORE_PORT(addr), errno, strerror(errno));
        return CORE_ERROR;
    }

    return CORE_OK;
}
