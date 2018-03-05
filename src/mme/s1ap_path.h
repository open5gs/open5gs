#ifndef __S1AP_PATH_H__
#define __S1AP_PATH_H__

#include "core_pkbuf.h"

#include "mme_context.h"
#include "s1ap/s1ap_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

CORE_DECLARE(status_t) s1ap_init(c_uint16_t port);
CORE_DECLARE(status_t) s1ap_final();

CORE_DECLARE(status_t) s1ap_open();
CORE_DECLARE(status_t) s1ap_close();

CORE_DECLARE(status_t) s1ap_server(sock_node_t *snode, int type);
CORE_DECLARE(status_t) s1ap_delete(sock_id sock);

CORE_DECLARE(int) s1ap_recv_handler(sock_id sock, void *data);

CORE_DECLARE(status_t) s1ap_send(sock_id sock,
        pkbuf_t *pkbuf, c_sockaddr_t *addr);
CORE_DECLARE(status_t) s1ap_recv(sock_id id, pkbuf_t *pkbuf);

CORE_DECLARE(status_t) s1ap_send_to_enb(mme_enb_t *enb, pkbuf_t *pkb);
CORE_DECLARE(status_t) s1ap_delayed_send_to_enb(mme_enb_t *enb,
        pkbuf_t *pkbuf, c_uint32_t duration);
CORE_DECLARE(status_t) s1ap_send_to_nas(
        enb_ue_t *enb_ue,
        S1AP_ProcedureCode_t procedureCode, S1AP_NAS_PDU_t *nasPdu);
CORE_DECLARE(status_t) s1ap_send_to_esm(mme_ue_t *mme_ue, pkbuf_t *esmbuf);

CORE_DECLARE(status_t) s1ap_send_initial_context_setup_request(
        mme_ue_t *mme_ue);
CORE_DECLARE(status_t) s1ap_send_ue_context_release_command(
    enb_ue_t *enb_ue, S1AP_Cause_PR group, long cause,
    c_uint8_t action, c_uint32_t delay);

CORE_DECLARE(status_t) s1ap_send_path_switch_ack(mme_ue_t *mme_ue);
CORE_DECLARE(status_t) s1ap_send_path_switch_failure(mme_enb_t *enb,
    c_uint32_t enb_ue_s1ap_id, c_uint32_t mme_ue_s1ap_id,
    S1AP_Cause_PR group, long cause);

CORE_DECLARE(status_t) s1ap_send_handover_command(enb_ue_t *source_ue);
CORE_DECLARE(status_t) s1ap_send_handover_preparation_failure(
        enb_ue_t *source_ue, S1AP_Cause_t *cause);

CORE_DECLARE(status_t) s1ap_send_handover_request(
        mme_ue_t *mme_ue, S1AP_HandoverRequiredIEs_t *required);

CORE_DECLARE(status_t) s1ap_send_handover_cancel_ack(enb_ue_t *source_ue);

CORE_DECLARE(status_t) s1ap_send_mme_status_transfer(
        enb_ue_t *target_ue, S1AP_ENBStatusTransferIEs_t *ies);
CORE_DECLARE(status_t) s1ap_send_error_indication(
        mme_enb_t *enb, c_uint16_t presenceMask,
        c_uint32_t enb_ue_s1ap_id, c_uint32_t mme_ue_s1ap_id,
        S1AP_Cause_PR group, long cause);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__S1_PATH_H__ */
