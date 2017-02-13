#ifndef __S1_PATH_H__
#define __S1_PATH_H__

#include "core.h"
#include "core_pkbuf.h"
#include "core_msgq.h"

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) s1_open(msgq_id queue_id);
CORE_DECLARE(status_t) s1_close();

CORE_DECLARE(status_t) s1_send(net_sock_t *s, pkbuf_t *pkb);
CORE_DECLARE(status_t) s1_send_to_enb(enb_ctx_t *enb, pkbuf_t *pkb);

int _s1_recv_cb(net_sock_t *net_sock, void *data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__S1_PATH_H__ */
