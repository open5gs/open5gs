#ifndef __GTP_NODE_H__
#define __GTP_NODE_H__

#include "core_list.h"
#include "core_network.h"

#include "gtp_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SETUP_GTP_NODE(__cTX, __gNODE) \
    do { \
        d_assert((__cTX), break, "Null param"); \
        d_assert((__gNODE), break, "Null param"); \
        (__cTX)->gnode = __gNODE; \
    } while(0)

/**
 * This structure represents the commonalities of GTP node such as MME, SGW,
 * PGW gateway. Some of members may not be used by the specific type of node */
typedef struct _gtp_node_t {
    lnode_t         node;           /* A node of list_t */

    c_sockaddr_t    *sa_list;       /* Socket Address List */

    sock_id         sock;           /* Socket instance */
    ip_t            ip;             /* Socket Address */

    list_t          local_list;    
    list_t          remote_list;   
} gtp_node_t;

CORE_DECLARE(status_t) gtp_node_init(void);
CORE_DECLARE(status_t) gtp_node_final(void);

CORE_DECLARE(status_t) gtp_add_node(list_t *list, gtp_node_t **node,
        c_sockaddr_t *sa_list, int no_ipv4, int no_ipv6, int prefer_ipv4);
CORE_DECLARE(gtp_node_t *) gtp_add_node_with_teid(
        list_t *list, gtp_f_teid_t *f_teid,
        c_uint16_t port, int no_ipv4, int no_ipv6, int prefer_ipv4);
CORE_DECLARE(status_t) gtp_remove_node(list_t *list, gtp_node_t *node);
CORE_DECLARE(status_t) gtp_remove_all_nodes(list_t *list);

CORE_DECLARE(gtp_node_t *) gtp_find_node(list_t *list, gtp_f_teid_t *f_teid);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_NODE_H__ */
