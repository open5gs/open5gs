#ifndef __EMM_HANDLER_H__
#define __EMM_HANDLER_H__

#include "nas_message.h"

#include "mme_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) emm_handle_attach_request(
    mme_ue_t *mme_ue, nas_attach_request_t *attach_request);
CORE_DECLARE(void) emm_handle_attach_accept(mme_sess_t *sess);
CORE_DECLARE(void) emm_handle_attach_complete(
    mme_ue_t *mme_ue, nas_attach_complete_t *attach_complete);
CORE_DECLARE(void) emm_handle_attach_reject(mme_ue_t *mme_ue);

CORE_DECLARE(void) emm_handle_identity_request(mme_ue_t *mme_ue);
CORE_DECLARE(void) emm_handle_identity_response(
        mme_ue_t *mme_ue, nas_identity_response_t *identity_response);

CORE_DECLARE(void) emm_handle_authentication_request(mme_ue_t *mme_ue);
CORE_DECLARE(void) emm_handle_authentication_response(
    mme_ue_t *mme_ue, nas_authentication_response_t *authentication_response);

CORE_DECLARE(void) emm_handle_detach_request(
    mme_ue_t *mme_ue, nas_detach_request_from_ue_t *detach_request);
CORE_DECLARE(void) emm_handle_detach_accept(
    mme_ue_t *mme_ue, nas_detach_request_from_ue_t *detach_request);

CORE_DECLARE(void) emm_handle_service_request(
        mme_ue_t *mme_ue, nas_service_request_t *service_request);

CORE_DECLARE(void) emm_handle_emm_status(
        mme_ue_t *mme_ue, nas_emm_status_t *emm_status);
CORE_DECLARE(void) emm_handle_tau_request(
        mme_ue_t *mme_ue, nas_tracking_area_update_request_t *tau_request);
CORE_DECLARE(void) emm_handle_tau_accept(mme_ue_t *mme_ue);
CORE_DECLARE(void) emm_handle_tau_reject(mme_ue_t *mme_ue, 
        nas_esm_cause_t emm_cause);

/** S11 Layer in EMM handler */
CORE_DECLARE(void) emm_handle_s11_delete_session_request(mme_ue_t *mme_ue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __EMM_HANDLER_H__ */
