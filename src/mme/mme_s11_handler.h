#ifndef __MME_HANDLER_H__
#define __MME_HANDLER_H__

#include "gtp_tlv.h"

#include "mme_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) mme_s11_handle_create_session_request(mme_sess_t *sess);
CORE_DECLARE(void) mme_s11_handle_create_session_response(
        mme_sess_t *sess, gtp_create_session_response_t *rsp);
CORE_DECLARE(void) mme_s11_handle_modify_bearer_response(
        mme_sess_t *sess, gtp_modify_bearer_response_t *rsp);
CORE_DECLARE(void) mme_s11_handle_delete_session_response(
        mme_sess_t *sess, gtp_delete_session_response_t *rsp);
CORE_DECLARE(void) mme_s11_handle_release_access_bearers_response(
        mme_sess_t *sess, gtp_release_access_bearers_response_t *rsp);
CORE_DECLARE(void) mme_s11_handle_downlink_data_notification(
        gtp_xact_t *xact, mme_sess_t *sess, 
        gtp_downlink_data_notification_t *noti);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MME_HANDLER_H__ */
