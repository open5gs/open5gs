#ifndef __GTP_PATH_H__
#define __GTP_PATH_H__

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S11_UDP_PORT                2123
#define S5_UDP_PORT                 2152

CORE_DECLARE(status_t) gtp_open(net_sock_t **sock, net_sock_handler handler, 
        void *data, const c_uint32_t addr, const c_uint16_t port);
CORE_DECLARE(status_t) gtp_close(net_sock_t *sock);

CORE_DECLARE(pkbuf_t *) gtp_read(net_sock_t *net_sock);

CORE_DECLARE(status_t) gtp_send(net_sock_t *s, 
        pkbuf_t *pkb, c_uint32_t ip_addr, c_uint16_t port);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_PATH_H__ */
