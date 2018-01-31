#ifndef __SGW_S11_HANDLER_H__
#define __SGW_S11_HANDLER_H__

#include "gtp/gtp_message.h"

#include "sgw_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) sgw_s11_handle_create_session_request(gtp_xact_t *s11_xact,
        sgw_ue_t *sgw_ue, gtp_message_t *gtp_message);
CORE_DECLARE(void) sgw_s11_handle_modify_bearer_request(gtp_xact_t *s11_xact, 
        sgw_ue_t *sgw_ue, gtp_modify_bearer_request_t *req);
CORE_DECLARE(void) sgw_s11_handle_delete_session_request(gtp_xact_t *s11_xact, 
        sgw_ue_t *sgw_ue, gtp_message_t *gtp_message);
CORE_DECLARE(void) sgw_s11_handle_create_bearer_response(gtp_xact_t *s11_xact, 
        sgw_ue_t *sgw_ue, gtp_message_t *gtp_message);
CORE_DECLARE(void) sgw_s11_handle_update_bearer_response(gtp_xact_t *s11_xact, 
        sgw_ue_t *sgw_ue, gtp_message_t *gtp_message);
CORE_DECLARE(void) sgw_s11_handle_delete_bearer_response(gtp_xact_t *s11_xact, 
        sgw_ue_t *sgw_ue, gtp_message_t *gtp_message);

CORE_DECLARE(void) sgw_s11_handle_release_access_bearers_request(
        gtp_xact_t *s11_xact, sgw_ue_t *sgw_ue,
        gtp_release_access_bearers_request_t *req);
CORE_DECLARE(void) sgw_s11_handle_lo_dldata_notification(sgw_bearer_t *bearer);
CORE_DECLARE(void) sgw_s11_handle_downlink_data_notification_ack(
        gtp_xact_t *s11_xact, sgw_ue_t *sgw_ue,
        gtp_downlink_data_notification_acknowledge_t *ack);

CORE_DECLARE(void) sgw_s11_handle_create_indirect_data_forwarding_tunnel_request(
        gtp_xact_t *s11_xact, sgw_ue_t *sgw_ue,
        gtp_create_indirect_data_forwarding_tunnel_request_t *req);
CORE_DECLARE(void) sgw_s11_handle_delete_indirect_data_forwarding_tunnel_request(
        gtp_xact_t *s11_xact, sgw_ue_t *sgw_ue);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SGW_S11_HANDLER_H__ */
