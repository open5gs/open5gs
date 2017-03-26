#define TRACE_MODULE _nas_build

#include "nas_message.h"
#include "s1ap_message.h"

void nas_send_s1ap_to_enb(ue_ctx_t *ue, pkbuf_t *pkbuf)
{
    char buf[INET_ADDRSTRLEN];

    int encoded;
    s1ap_message_t message;
    S1ap_DownlinkNASTransport_IEs_t *ies = 
        &message.s1ap_DownlinkNASTransport_IEs;
    S1ap_NAS_PDU_t *nasPdu = &ies->nas_pdu;
    pkbuf_t *sendbuf = NULL;

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = ue->mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = ue->enb_ue_s1ap_id;

    nasPdu->size = pkbuf->len;
    nasPdu->buf = core_calloc(nasPdu->size, sizeof(c_uint8_t));
    memcpy(nasPdu->buf, pkbuf->payload, nasPdu->size);

    message.procedureCode = S1ap_ProcedureCode_id_downlinkNASTransport;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(&sendbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(sendbuf && encoded >= 0,,);
    d_assert(s1ap_send_to_enb(ue->enb, sendbuf) == CORE_OK,,);
    pkbuf_free(pkbuf);

    d_assert(ue->enb,,);
    d_info("[S1AP] downlinkNASTransport : "
            "UE[eNB-UE-S1AP-ID(%d)] <-- eNB[%s:%d]",
        ue->enb_ue_s1ap_id,
        INET_NTOP(&ue->enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        ue->enb->enb_id);
}
