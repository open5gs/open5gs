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

/* GTP-U message type, defined in 3GPP TS 29.281 Release 11 */
#define GTPU_MSGTYPE_ECHO_REQ               1
#define GTPU_MSGTYPE_ECHO_RSP               2
#define GTPU_MSGTYPE_ERR_IND                26
#define GTPU_MSGTYPE_SUPP_EXTHDR_NOTI       31
#define GTPU_MSGTYPE_END_MARKER             254
#define GTPU_MSGTYPE_GPDU                   255

/* GTU-U flags */
#define GTPU_FLAGS_PN                       0x1
#define GTPU_FLAGS_S                        0x2

typedef struct _gtp_header
{
    c_uint8_t flags;
    c_uint8_t type;
    c_uint16_t length;
    c_uint32_t teid;
} gtp_header_t;
/**
 * This structure represents the commonalities of GTP node such as MME, SGW,
 * PGW gateway. Some of members may not be used by the specific type of node */
typedef struct _gtp_node_t {
    lnode_t         node;           /**< A node of list_t */

    c_uint32_t      addr;           /**< Network byte order IP Address */
    c_uint16_t      port;           /**< Host byte order Port number */

    list_t          local_list;    
    list_t          remote_list;   
} gtp_node_t;

CORE_DECLARE(status_t) gtp_listen(net_sock_t **sock, 
    net_sock_handler handler, c_uint32_t addr, c_uint16_t port, void *data);
CORE_DECLARE(status_t) gtp_close(net_sock_t *sock);

CORE_DECLARE(pkbuf_t *) gtp_read(net_sock_t *sock);
CORE_DECLARE(status_t) gtp_send(net_sock_t *sock, 
    gtp_node_t *gnode, pkbuf_t *pkbuf);

CORE_DECLARE(pkbuf_t*) gtp_handle_echo_req(pkbuf_t *pkt);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_PATH_H__ */
