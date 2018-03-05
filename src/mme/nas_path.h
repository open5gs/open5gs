#ifndef __NAS_PATH_H__
#define __NAS_PATH_H__

#include "core_pkbuf.h"

#include "mme_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) nas_send_to_enb(mme_ue_t *mme_ue, pkbuf_t *pkbuf);
CORE_DECLARE(status_t) nas_send_emm_to_esm(
    mme_ue_t *mme_ue, nas_esm_message_container_t *esm_message_container);
CORE_DECLARE(status_t) nas_send_to_downlink_nas_transport(
    mme_ue_t *mme_ue, pkbuf_t *pkbuf);

CORE_DECLARE(status_t) nas_send_attach_accept(mme_ue_t *mme_ue);
CORE_DECLARE(status_t) nas_send_attach_reject(mme_ue_t *mme_ue,
    nas_emm_cause_t emm_cause, nas_esm_cause_t esm_cause);

CORE_DECLARE(status_t) nas_send_identity_request(mme_ue_t *mme_ue);

CORE_DECLARE(status_t) nas_send_authentication_request(
        mme_ue_t *mme_ue, e_utran_vector_t *e_utran_vector);
CORE_DECLARE(status_t) nas_send_authentication_reject(mme_ue_t *mme_ue);

CORE_DECLARE(status_t) nas_send_detach_accept(mme_ue_t *mme_ue);

CORE_DECLARE(status_t) nas_send_pdn_connectivity_reject(
    mme_sess_t *sess, nas_esm_cause_t esm_cause);
CORE_DECLARE(status_t) nas_send_esm_information_request(mme_bearer_t *bearer);
CORE_DECLARE(status_t) nas_send_activate_default_bearer_context_request(
    mme_bearer_t *bearer);
CORE_DECLARE(status_t) nas_send_activate_dedicated_bearer_context_request(
    mme_bearer_t *bearer);
CORE_DECLARE(status_t) nas_send_activate_all_dedicated_bearers(
    mme_bearer_t *default_bearer);
CORE_DECLARE(status_t) nas_send_modify_bearer_context_request(
        mme_bearer_t *bearer, int qos_presence, int tft_presence);
CORE_DECLARE(status_t) nas_send_deactivate_bearer_context_request(
    mme_bearer_t *bearer);

CORE_DECLARE(status_t) nas_send_tau_accept(
        mme_ue_t *mme_ue, S1AP_ProcedureCode_t procedureCode);
CORE_DECLARE(status_t) nas_send_tau_reject(
        mme_ue_t *mme_ue, nas_esm_cause_t emm_cause);

CORE_DECLARE(status_t) nas_send_service_reject(
        mme_ue_t *mme_ue, nas_emm_cause_t emm_cause);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NAS_PATH_H__ */
