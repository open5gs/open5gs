#ifndef __S1AP_PATH_H__
#define __S1AP_PATH_H__

#include "mme_context.h"
#include "asn1c/s1ap_message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S1AP_NON_UE_SIGNALLING   0

int s1ap_open();
void s1ap_close();

void s1ap_server(ogs_socknode_t *node, int type);
void s1ap_closesocket(ogs_sock_t *sock);
void s1ap_delete(ogs_socknode_t *node);
void s1ap_recv_handler(short when, ogs_socket_t fd, void *data);

int s1ap_send(ogs_sock_t *sock,
        ogs_pkbuf_t *pkbuf, ogs_sockaddr_t *addr, uint16_t stream_no);
int s1ap_recv(ogs_sock_t *sock, ogs_pkbuf_t *pkbuf);

int s1ap_send_to_enb(
        mme_enb_t *enb, ogs_pkbuf_t *pkb, uint16_t stream_no);
int s1ap_send_to_enb_ue(enb_ue_t *enb_ue, ogs_pkbuf_t *pkbuf);
int s1ap_delayed_send_to_enb_ue(enb_ue_t *enb_ue,
        ogs_pkbuf_t *pkbuf, ogs_time_t duration);
int s1ap_send_to_nas(enb_ue_t *enb_ue,
        S1AP_ProcedureCode_t procedureCode, S1AP_NAS_PDU_t *nasPdu);
int s1ap_send_to_esm(mme_ue_t *mme_ue, ogs_pkbuf_t *esmbuf);

int s1ap_send_initial_context_setup_request(
        mme_ue_t *mme_ue);
int s1ap_send_ue_context_release_command(
    enb_ue_t *enb_ue, S1AP_Cause_PR group, long cause,
    uint8_t action, uint32_t delay);

int s1ap_send_mme_configuration_transfer(
        mme_enb_t *target_enb,
        S1AP_SONConfigurationTransfer_t *SONConfigurationTransfer);

int s1ap_send_path_switch_ack(mme_ue_t *mme_ue);

int s1ap_send_handover_command(enb_ue_t *source_ue);
int s1ap_send_handover_preparation_failure(
        enb_ue_t *source_ue, S1AP_Cause_t *cause);

int s1ap_send_handover_request(
        mme_ue_t *mme_ue,
        mme_enb_t *target_enb,
        S1AP_ENB_UE_S1AP_ID_t *enb_ue_s1ap_id,
        S1AP_MME_UE_S1AP_ID_t *mme_ue_s1ap_id,
        S1AP_HandoverType_t *handovertype,
        S1AP_Cause_t *cause,
        S1AP_Source_ToTarget_TransparentContainer_t
            *source_totarget_transparentContainer);

int s1ap_send_handover_cancel_ack(enb_ue_t *source_ue);

int s1ap_send_mme_status_transfer(
        enb_ue_t *target_ue,
        S1AP_ENB_StatusTransfer_TransparentContainer_t
            *enb_statustransfer_transparentContainer);
int s1ap_send_error_indication(
        mme_enb_t *enb,
        S1AP_MME_UE_S1AP_ID_t *mme_ue_s1ap_id,
        S1AP_ENB_UE_S1AP_ID_t *enb_ue_s1ap_id,
        S1AP_Cause_PR group, long cause);
int s1ap_send_s1_reset_ack(
        mme_enb_t *enb,
        S1AP_UE_associatedLogicalS1_ConnectionListRes_t *partOfS1_Interface);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__S1_PATH_H__ */
