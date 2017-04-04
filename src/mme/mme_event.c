#define TRACE_MODULE _mme_event

#include "core_debug.h"

#include "mme_event.h"
#include "s1ap_path.h"
#include "s1ap_message.h"
#include "nas_message.h"

static char EVT_NAME_LO_ENB_S1AP_ACCEPT[] = "LO_ENB_S1AP_ACCEPT";
static char EVT_NAME_LO_ENB_S1AP_CONNREFUSED[] = "LO_ENB_S1AP_CONNREFUSED";

static char EVT_NAME_TM_MME_S11_T3[] = "TM_MME_S11_T3";

static char EVT_NAME_MSG_ENB_S1AP[] = "MSG_ENB_S1AP";
static char EVT_NAME_MSG_UE_EMM[] = "MSG_UE_EMM";
static char EVT_NAME_MSG_UE_ESM[] = "MSG_UE_ESM";
static char EVT_NAME_MSG_MME_S11[] = "MSG_MME_S11";

char* mme_event_get_name(event_t *e)
{
    if (e == NULL)
        return FSM_NAME_INIT_SIG;

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG: 
            return FSM_NAME_ENTRY_SIG;
        case FSM_EXIT_SIG: 
            return FSM_NAME_EXIT_SIG;

        case EVT_LO_ENB_S1AP_ACCEPT: 
           return EVT_NAME_LO_ENB_S1AP_ACCEPT;
        case EVT_LO_ENB_S1AP_CONNREFUSED: 
           return EVT_NAME_LO_ENB_S1AP_CONNREFUSED;

        case EVT_TM_MME_S11_T3: 
           return EVT_NAME_TM_MME_S11_T3;

        case EVT_MSG_ENB_S1AP: 
           return EVT_NAME_MSG_ENB_S1AP;
        case EVT_MSG_UE_EMM: 
           return EVT_NAME_MSG_UE_EMM;
        case EVT_MSG_UE_ESM: 
           return EVT_NAME_MSG_UE_ESM;
        case EVT_MSG_MME_S11: 
           return EVT_NAME_MSG_MME_S11;

        default: 
           break;
    }

    return EVT_NAME_UNKNOWN;
}

void mme_event_s1ap_to_nas(ue_ctx_t *ue, S1ap_NAS_PDU_t *nasPdu)
{
    pkbuf_t *sendbuf = NULL;
    event_t e;

    d_assert(nasPdu, return, "Null param");

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    sendbuf = pkbuf_alloc(NAS_HEADROOM, nasPdu->size);
    d_assert(sendbuf, return, "Null param");
    memcpy(sendbuf->payload, nasPdu->buf, nasPdu->size);

    event_set(&e, EVT_MSG_UE_EMM);
    event_set_param1(&e, (c_uintptr_t)ue);
    event_set_param2(&e, (c_uintptr_t)sendbuf);
    mme_event_send(&e);
}

void mme_event_nas_to_s1ap(ue_ctx_t *ue, pkbuf_t *pkbuf)
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
