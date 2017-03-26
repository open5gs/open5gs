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

#define GTP_COMPARE_REMOTE_NODE(__id1, __id2) \
    (((__id1)->remote_addr) == ((__id2)->remote_addr) && \
     ((__id1)->remote_port) == ((__id2)->remote_port))

/**
 * This structure keeps active transactions and their histrory */
typedef struct _gtp_xact_info_t {
    list_t      local_xlist; /**< List of local initiated transactions */
    list_t      remote_xlist; /**< List of Remote initiated transactions */
} gtp_xact_info_t;

/**
 * This structure represents the commonalities of GTP node such as MME, SGW,
 * PGW gateway. Some of members may not be used by the specific type of node */
typedef struct _gtp_node_t {
    lnode_t         node;           /**< A node of list_t */

    c_uint32_t      local_addr;     /**< Network byte order IP Address */
    c_uint16_t      local_port;     /**< Host byte order Port number */
    c_uint32_t      remote_addr;    /**< Network byte order IP Address */
    c_uint16_t      remote_port;    /**< Host byte order Port number */

    gtp_xact_info_t xi;             /**< Transaction information */

    net_sock_t      *s;             /**< Network socket */
} gtp_node_t;

CORE_DECLARE(status_t) gtp_open(
        gtp_node_t *gnode, net_sock_handler handler);
CORE_DECLARE(status_t) gtp_close(gtp_node_t *gnode);

CORE_DECLARE(pkbuf_t *) gtp_read(gtp_node_t *gnode);

CORE_DECLARE(status_t) gtp_send(gtp_node_t *gnode, pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_PATH_H__ */
