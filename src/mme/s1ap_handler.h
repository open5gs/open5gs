#ifndef __S1AP_HANDLER_H__
#define __S1AP_HANDLER_H__

#include "mme_ctx.h"
#include "s1ap_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(void) s1ap_handle_s1_setup_request(
        enb_ctx_t *enb, s1ap_message_t *message);
CORE_DECLARE(void) s1ap_handle_initial_ue_message(
        enb_ctx_t *enb, s1ap_message_t *message);
CORE_DECLARE(void) s1ap_handle_uplink_nas_transport(
        enb_ctx_t *enb, s1ap_message_t *message);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __S1AP_HANDLER_H__ */
