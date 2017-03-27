#ifndef __GTP_PATH_H__
#define __GTP_PATH_H__

#include "core_pkbuf.h"
#include "core_net.h"
#include "core_list.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTPV2_C_UDP_PORT                2123
#define GTPV1_U_UDP_PORT                2152

#define GTP_COMPARE_NODE(__id1, __id2) \
    (((__id1)->addr) == ((__id2)->addr) && ((__id1)->port) == ((__id2)->port))

/**
 * This structure represents the commonalities of GTP node such as MME, SGW,
 * PGW gateway. Some of members may not be used by the specific type of node */
typedef struct _gtp_node_t {
    lnode_t         node;           /**< A node of list_t */

    c_uint32_t      addr;           /**< Network byte order IP Address */
    c_uint16_t      port;           /**< Host byte order Port number */

    /**< List of local initiated transactions */
    list_t          local_xlist;    
    /**< List of Remote initiated transactions */
    list_t          remote_xlist;   
} gtp_node_t;

CORE_DECLARE(status_t) gtp_listen(net_sock_t **sock, 
    net_sock_handler handler, c_uint32_t addr, c_uint16_t port, void *data);
CORE_DECLARE(status_t) gtp_close(net_sock_t *sock);

CORE_DECLARE(pkbuf_t *) gtp_read(net_sock_t *sock);

CORE_DECLARE(status_t) gtp_send(net_sock_t *sock, 
    gtp_node_t *gnode, pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_PATH_H__ */
