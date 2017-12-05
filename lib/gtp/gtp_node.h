#ifndef __GTP_NODE_H__
#define __GTP_NODE_H__

#include "core_list.h"
#include "core_network.h"

#include "gtp_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * This structure represents the commonalities of GTP node such as MME, SGW,
 * PGW gateway. Some of members may not be used by the specific type of node */
typedef struct _gtp_node_t {
    lnode_t         node;           /* A node of list_t */

    c_sockaddr_t    *sa_list;       /* Socket Address List */
    ip_t            ip;             /* IPv4/IPv6 address */

    sock_id         sock;           /* Socket instance */

    c_sockaddr_t    old_addr;       /* Will be removed */

    list_t          local_list;    
    list_t          remote_list;   
} gtp_node_t;

CORE_DECLARE(status_t) gtp_node_init(void);
CORE_DECLARE(status_t) gtp_node_final(void);

CORE_DECLARE(gtp_node_t *) gtp_add_node(list_t *list, c_sockaddr_t *sa_list);
CORE_DECLARE(status_t) gtp_remove_node(list_t *list, gtp_node_t *node);
CORE_DECLARE(status_t) gtp_remove_all_nodes(list_t *list);
CORE_DECLARE(status_t) gtp_filter_node(list_t *list, int family);
CORE_DECLARE(status_t) gtp_sort_node(list_t *list, int family);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_NODE_H__ */
