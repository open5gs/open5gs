#ifndef __GTP_PATH_H__
#define __GTP_PATH_H__

#include "core_pkbuf.h"
#include "core_net.h"
#include "core_network.h"
#include "core_list.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define GTPV2_C_UDP_PORT                2123
#define GTPV1_U_UDP_PORT                2152

/**
 * This structure represents the commonalities of GTP node such as MME, SGW,
 * PGW gateway. Some of members may not be used by the specific type of node */
typedef struct _gtp_node_t {
    lnode_t         node;           /**< A node of list_t */

    c_sockaddr_t    addr;           /**< Network byte order IP Address */
    net_sock_t      *sock;          /**< Socket Descriptor */

    list_t          local_list;    
    list_t          remote_list;   
} gtp_node_t;

CORE_DECLARE(status_t) gtp_listen(net_sock_t **sock, 
    net_sock_handler handler, c_uint32_t addr, c_uint16_t port, void *data);
CORE_DECLARE(status_t) gtp_close(net_sock_t *sock);

CORE_DECLARE(pkbuf_t *) gtp_read(net_sock_t *sock);
CORE_DECLARE(status_t) gtp_send(gtp_node_t *gnode, pkbuf_t *pkbuf);

CORE_DECLARE(pkbuf_t*) gtp_handle_echo_req(pkbuf_t *pkt);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_PATH_H__ */
