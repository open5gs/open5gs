#ifndef __S1AP_PATH_H__
#define __S1AP_PATH_H__

#include "core_pkbuf.h"
#include "core_net.h"

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) s1ap_listen();
CORE_DECLARE(status_t) s1ap_close();

CORE_DECLARE(status_t) s1ap_send(net_sock_t *s, pkbuf_t *pkb);
CORE_DECLARE(status_t) s1ap_send_to_enb(enb_ctx_t *enb, pkbuf_t *pkb);

int _s1ap_recv_cb(net_sock_t *net_sock, void *data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__S1_PATH_H__ */
