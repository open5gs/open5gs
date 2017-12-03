#define TRACE_MODULE _gtp_node

#include "core_debug.h"

#include "gtp_node.h"
#include "gtp_xact.h"

pool_declare(gtp_node_pool, gtp_node_t, MAX_NUM_OF_GTP_CLIENT);

status_t gtp_node_init(void)
{
    pool_init(&gtp_node_pool, MAX_NUM_OF_GTP_CLIENT);

    return CORE_OK;
}
status_t gtp_node_final(void)
{
    if (pool_size(&gtp_node_pool) != pool_avail(&gtp_node_pool))
        d_error("%d not freed in gtp_node_pool[%d]",
            pool_size(&gtp_node_pool) - pool_avail(&gtp_node_pool),
            pool_size(&gtp_node_pool));
    d_trace(3, "%d not freed in gtp_node_pool[%d]\n",
            pool_size(&gtp_node_pool) - pool_avail(&gtp_node_pool),
            pool_size(&gtp_node_pool));
    pool_final(&gtp_node_pool);

    return CORE_OK;
}

gtp_node_t *gtp_add_node(list_t *list,
    int family, const char *hostname, c_uint16_t port)
{
    status_t rv;
    gtp_node_t *node = NULL;
    c_sockaddr_t *sa_list = NULL;

    d_assert(list, return NULL,);

    rv = core_getaddrinfo(&sa_list, family, hostname, port, 0);
    if (rv != CORE_OK)
    {
        d_error("core_getaddrinfo(%d:%s:%d)", family, hostname, port);
        return NULL;
    }

    d_assert(sa_list, return NULL,);

    pool_alloc_node(&gtp_node_pool, &node);
    d_assert(node, return NULL,);
    memset(node, 0, sizeof(gtp_node_t));

    node->sa_list = sa_list;

    list_append(list, node);
    
    return node;
}

status_t gtp_remove_node(list_t *list, gtp_node_t *node)
{
    d_assert(node, return CORE_ERROR,);

    list_remove(list, node);

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

status_t gtp_filter_node(list_t *list, int family)
{
    status_t rv;
    gtp_node_t *node = NULL, *next_node = NULL;

    d_assert(list, return CORE_ERROR,);

    node = list_first(list);
    while(node)
    {
        next_node = list_next(node);

        rv = core_filteraddrinfo(&node->sa_list, family);
        d_assert(rv == CORE_OK, return CORE_ERROR,);

        if (node->sa_list == NULL)
            gtp_remove_node(list, node);

        node = next_node;
    }

    return CORE_OK;
}

status_t gtp_sort_node(list_t *list, int family)
{
    status_t rv;
    gtp_node_t *node = NULL, *next_node = NULL;

    d_assert(list, return CORE_ERROR,);

    node = list_first(list);
    while(node)
    {
        next_node = list_next(node);

        rv = core_sortaddrinfo(&node->sa_list, family);
        d_assert(rv == CORE_OK, return CORE_ERROR,);

        node = next_node;
    }

    return CORE_OK;
}

