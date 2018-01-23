#define TRACE_MODULE _gtp_node

#include "core_debug.h"

#include "gtp_conv.h"
#include "gtp_node.h"
#include "gtp_xact.h"

#define MAX_GTP_NODE_POOL_SIZE          512

pool_declare(gtp_node_pool, gtp_node_t, MAX_GTP_NODE_POOL_SIZE);

status_t gtp_node_init(void)
{
    pool_init(&gtp_node_pool, MAX_GTP_NODE_POOL_SIZE);

    return CORE_OK;
}
status_t gtp_node_final(void)
{
    if (pool_used(&gtp_node_pool))
        d_error("%d not freed in gtp_node_pool[%d]",
            pool_used(&gtp_node_pool), pool_size(&gtp_node_pool));
    d_trace(9, "%d not freed in gtp_node_pool[%d]\n",
            pool_used(&gtp_node_pool), pool_size(&gtp_node_pool));
    pool_final(&gtp_node_pool);

    return CORE_OK;
}

status_t gtp_add_node(list_t *list, gtp_node_t **node,
        c_sockaddr_t *all_list, int no_ipv4, int no_ipv6, int prefer_ipv4)
{
    status_t rv;
    gtp_node_t *new_node = NULL;
    c_sockaddr_t *preferred_list = NULL;

    d_assert(list, return CORE_ERROR,);
    d_assert(all_list, return CORE_ERROR,);

    rv = core_copyaddrinfo(&preferred_list, all_list);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    if (no_ipv4 == 1)
    {
        rv = core_filteraddrinfo(&preferred_list, AF_INET6);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
    }
    if (no_ipv6 == 1)
    {
        rv = core_filteraddrinfo(&preferred_list, AF_INET);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
    }
    if (prefer_ipv4 == 1)
    {
        rv = core_sortaddrinfo(&preferred_list, AF_INET);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
    }
    else
    {
        rv = core_sortaddrinfo(&preferred_list, AF_INET6);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
    }

    if (preferred_list)
    {
        pool_alloc_node(&gtp_node_pool, &new_node);
        d_assert(new_node, return CORE_ERROR,);
        memset(new_node, 0, sizeof(gtp_node_t));

        new_node->sa_list = preferred_list;

        list_init(&new_node->local_list);
        list_init(&new_node->remote_list);

        list_append(list, new_node);
    }

    *node = new_node;

    return CORE_OK;
}

gtp_node_t *gtp_add_node_with_teid(list_t *list, gtp_f_teid_t *f_teid,
        c_uint16_t port, int no_ipv4, int no_ipv6, int prefer_ipv4)
{
    status_t rv;
    gtp_node_t *node = NULL;
    c_sockaddr_t *sa_list = NULL;

    d_assert(list, return NULL,);
    d_assert(f_teid, return NULL,);
    d_assert(port, return NULL,);

    rv = gtp_f_teid_to_sockaddr(f_teid, port, &sa_list);
    d_assert(rv == CORE_OK, return NULL,);

    rv = gtp_add_node(list, &node, sa_list, no_ipv4, no_ipv6, prefer_ipv4);
    d_assert(rv == CORE_OK, return NULL,);
    d_assert(node, return NULL,);

    rv = gtp_f_teid_to_ip(f_teid, &node->ip);
    d_assert(rv == CORE_OK, return NULL,);

    rv = sock_fill_scope_id_in_local(node->sa_list);
    d_assert(rv == CORE_OK, return NULL,);

    core_freeaddrinfo(sa_list);

    return node;
}

status_t gtp_remove_node(list_t *list, gtp_node_t *node)
{
    d_assert(node, return CORE_ERROR,);

    list_remove(list, node);

    if (node->sock)
        sock_delete(node->sock);

    gtp_xact_delete_all(node);

    core_freeaddrinfo(node->sa_list);
    pool_free_node(&gtp_node_pool, node);

    return CORE_OK;
}

status_t gtp_remove_all_nodes(list_t *list)
{
    gtp_node_t *node = NULL, *next_node = NULL;
    
    node = list_first(list);
    while(node)
    {
        next_node = list_next(node);

        gtp_remove_node(list, node);

        node = next_node;
    }

    return CORE_OK;
}

gtp_node_t* gtp_find_node(list_t *list, gtp_f_teid_t *f_teid)
{
    status_t rv;
    gtp_node_t *node = NULL;
    ip_t ip;

    d_assert(list, return NULL,);
    d_assert(f_teid, return NULL,);

    rv = gtp_f_teid_to_ip(f_teid, &ip);
    d_assert(rv == CORE_OK, return NULL,);

    node = list_first(list);
    while (node)
    {
        if (memcmp(&node->ip, &ip, ip.len) == 0)
            break;

        node = list_next(node);
    }

    return node;
}
