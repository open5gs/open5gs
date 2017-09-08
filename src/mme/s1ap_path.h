#ifndef __S1AP_PATH_H__
#define __S1AP_PATH_H__

#include "core_pkbuf.h"
#include "core_net.h"

#include "mme_context.h"
#include "s1ap_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) s1ap_open();
CORE_DECLARE(status_t) s1ap_close();

CORE_DECLARE(status_t) s1ap_send(net_sock_t *s, pkbuf_t *pkb);
CORE_DECLARE(status_t) s1ap_send_to_enb(mme_enb_t *enb, pkbuf_t *pkb);
CORE_DECLARE(status_t) s1ap_send_to_nas(
        enb_ue_t *enb_ue, S1ap_NAS_PDU_t *nasPdu);
CORE_DECLARE(status_t) s1ap_send_to_esm(mme_ue_t *mme_ue, pkbuf_t *esmbuf);
CORE_DECLARE(status_t) s1ap_send_ue_context_release_commmand(
        enb_ue_t *enb_ue, S1ap_Cause_t *cause);

int _s1ap_recv_cb(net_sock_t *net_sock, void *data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__S1_PATH_H__ */
