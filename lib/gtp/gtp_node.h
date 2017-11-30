#ifndef __GTP_NODE_H__
#define __GTP_NODE_H__

#include "core_list.h"
#include "core_network.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef list_t gnode_list_t;

/**
 * This structure represents the commonalities of GTP node such as MME, SGW,
 * PGW gateway. Some of members may not be used by the specific type of node */
typedef struct _gtp_node_t {
    lnode_t         node;           /**< A node of list_t */
    c_sockaddr_t    *addr;          /**< Socket Address */

    c_sockaddr_t    old_addr;       /**< Will be removed */
    sock_id         sock;           /**< Socket Descriptor */

    list_t          local_list;    
    list_t          remote_list;   
} gtp_node_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_NODE_H__ */
