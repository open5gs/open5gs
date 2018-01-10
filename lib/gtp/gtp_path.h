#ifndef __GTP_PATH_H__
#define __GTP_PATH_H__

#include "core_pkbuf.h"
#include "core_network.h"

typedef struct _gtp_node_t gtp_node_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) gtp_server(sock_node_t *snode, sock_handler handler);
CORE_DECLARE(status_t) gtp_client(gtp_node_t *gnode);
CORE_DECLARE(status_t) gtp_connect(
        sock_id ipv4, sock_id ipv6, gtp_node_t *gnode);

CORE_DECLARE(status_t) gtp_server_list(list_t *list, sock_handler handler);

CORE_DECLARE(sock_id) gtp_local_sock_first(list_t *list);
CORE_DECLARE(c_sockaddr_t *) gtp_local_addr_first(list_t *list);

CORE_DECLARE(status_t) gtp_recv(sock_id sock, pkbuf_t **pkbuf);
CORE_DECLARE(status_t) gtp_recvfrom(sock_id sock,
        pkbuf_t **pkbuf, c_sockaddr_t *from);
CORE_DECLARE(status_t) gtp_send(gtp_node_t *gnode, pkbuf_t *pkbuf);

CORE_DECLARE(pkbuf_t*) gtp_handle_echo_req(pkbuf_t *pkt);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_PATH_H__ */
