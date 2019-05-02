#ifndef __S1AP_HANDLER_H__
#define __S1AP_HANDLER_H__

#include "asn1c/s1ap_message.h"

#include "mme_context.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void s1ap_handle_s1_setup_request(
        mme_enb_t *enb, s1ap_message_t *message);
void s1ap_handle_initial_ue_message(
        mme_enb_t *enb, s1ap_message_t *message);
void s1ap_handle_uplink_nas_transport(
        mme_enb_t *enb, s1ap_message_t *message);
void s1ap_handle_ue_capability_info_indication(
        mme_enb_t *enb, s1ap_message_t *message);
void s1ap_handle_initial_context_setup_response(
        mme_enb_t *enb, s1ap_message_t *message);
void s1ap_handle_initial_context_setup_failure(
        mme_enb_t *enb, s1ap_message_t *message);
void s1ap_handle_e_rab_setup_response(
        mme_enb_t *enb, s1ap_message_t *message);

void s1ap_handle_ue_context_release_request(
        mme_enb_t *enb, s1ap_message_t *message);
void s1ap_handle_ue_context_release_complete(
        mme_enb_t *enb, s1ap_message_t *message);

void s1ap_handle_paging(mme_ue_t *mme_ue);
void s1ap_t3413_timeout(void *data);

void s1ap_handle_path_switch_request(
        mme_enb_t *enb, s1ap_message_t *message);

void s1ap_handle_handover_required(
        mme_enb_t *enb, s1ap_message_t *message);
void s1ap_handle_handover_request_ack(
        mme_enb_t *enb, s1ap_message_t *message);
void s1ap_handle_handover_failure(
        mme_enb_t *enb, s1ap_message_t *message);
void s1ap_handle_handover_cancel(
        mme_enb_t *enb, s1ap_message_t *message);

void s1ap_handle_enb_status_transfer(
        mme_enb_t *enb, s1ap_message_t *message);
void s1ap_handle_enb_configuration_transfer(
        mme_enb_t *enb, s1ap_message_t *message, ogs_pkbuf_t *pkbuf);
void s1ap_handle_handover_notification(
        mme_enb_t *enb, s1ap_message_t *message);

void s1ap_handle_s1_reset(
        mme_enb_t *enb, s1ap_message_t *message);

void s1ap_handle_write_replace_warning_response(
        mme_enb_t *enb, s1ap_message_t *message);
void s1ap_handle_kill_response(
        mme_enb_t *enb, s1ap_message_t *message);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __S1AP_HANDLER_H__ */
