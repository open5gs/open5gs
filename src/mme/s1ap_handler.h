#ifndef __S1AP_HANDLER_H__
#define __S1AP_HANDLER_H__

#include "mme_context.h"
#include "s1ap_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) s1ap_handle_s1_setup_request(
        mme_enb_t *enb, s1ap_message_t *message);
CORE_DECLARE(void) s1ap_handle_initial_ue_message(
        mme_enb_t *enb, s1ap_message_t *message);
CORE_DECLARE(void) s1ap_handle_uplink_nas_transport(
        mme_enb_t *enb, s1ap_message_t *message);
CORE_DECLARE(void) s1ap_handle_ue_capability_info_indication(
        mme_enb_t *enb, s1ap_message_t *message);
CORE_DECLARE(void) s1ap_handle_initial_context_setup_response(
        mme_enb_t *enb, s1ap_message_t *message);
CORE_DECLARE(void) s1ap_handle_ue_context_release_complete(
        mme_enb_t *enb, s1ap_message_t *message);
CORE_DECLARE(void) s1ap_handle_ue_context_release_request(
        mme_enb_t *enb, s1ap_message_t *message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __S1AP_HANDLER_H__ */
