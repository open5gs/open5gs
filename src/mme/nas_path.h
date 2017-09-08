#ifndef __NAS_PATH_H__
#define __NAS_PATH_H__

#include "core_pkbuf.h"

#include "mme_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) nas_send_to_enb(enb_ue_t *enb_ue, pkbuf_t *pkbuf);
CORE_DECLARE(status_t) nas_send_emm_to_esm(
        mme_ue_t *mme_ue, nas_esm_message_container_t *esm_message_container);
CORE_DECLARE(status_t) nas_send_to_downlink_nas_transport(
    mme_ue_t *mme_ue, pkbuf_t *pkbuf);
CORE_DECLARE(status_t) nas_send_attach_accept(mme_ue_t *mme_ue);
CORE_DECLARE(status_t) nas_send_attach_reject(mme_ue_t *mme_ue,
    e_S1ap_CauseNas s1ap_cause_nas, nas_emm_cause_t emm_cause,
    nas_esm_cause_t esm_cause);
CORE_DECLARE(status_t) nas_send_activate_dedicated_bearer_context_request(
        enb_ue_t *enb_ue, mme_bearer_t *bearer);
CORE_DECLARE(status_t) nas_send_deactivate_bearer_context_request(
        enb_ue_t *enb_ue, mme_bearer_t *bearer);
CORE_DECLARE(status_t) nas_send_tau_accept(mme_ue_t *mme_ue);
CORE_DECLARE(status_t) nas_send_tau_reject(mme_ue_t *mme_ue, 
    nas_esm_cause_t emm_cause);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NAS_PATH_H__ */
