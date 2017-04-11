#define TRACE_MODULE _mme_event

#include "core_debug.h"

#include "mme_event.h"
#include "s1ap_path.h"
#include "s1ap_message.h"
#include "nas_message.h"
#include "nas_security.h"

static char EVT_NAME_LO_MME_S1AP_ACCEPT[] = "LO_MME_S1AP_ACCEPT";
static char EVT_NAME_LO_MME_S1AP_CONNREFUSED[] = "LO_MME_S1AP_CONNREFUSED";
static char EVT_NAME_LO_MME_EMM_AUTH_REQ[] = "LO_MME_EMM_AUTH_REQ";
static char EVT_NAME_LO_MME_EMM_LOCATION_UPDATE[] = "LO_MME_EMM_LOCATION_UPDATE";
static char EVT_NAME_LO_MME_ESM_INFO_REQ[] = "LO_MME_ESM_INFO_REQ";

static char EVT_NAME_TM_MME_S11_T3[] = "TM_MME_S11_T3";

static char EVT_NAME_MSG_MME_S1AP[] = "MSG_MME_S1AP";
static char EVT_NAME_MSG_MME_EMM[] = "MSG_MME_EMM";
static char EVT_NAME_MSG_MME_ESM[] = "MSG_MME_ESM";
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

        case EVT_LO_MME_S1AP_ACCEPT: 
           return EVT_NAME_LO_MME_S1AP_ACCEPT;
        case EVT_LO_MME_S1AP_CONNREFUSED: 
           return EVT_NAME_LO_MME_S1AP_CONNREFUSED;
        case EVT_LO_MME_EMM_AUTH_REQ: 
           return EVT_NAME_LO_MME_EMM_AUTH_REQ;
        case EVT_LO_MME_EMM_LOCATION_UPDATE: 
           return EVT_NAME_LO_MME_EMM_LOCATION_UPDATE;
        case EVT_LO_MME_ESM_INFO_REQ: 
           return EVT_NAME_LO_MME_ESM_INFO_REQ;

        case EVT_TM_MME_S11_T3: 
           return EVT_NAME_TM_MME_S11_T3;

        case EVT_MSG_MME_S1AP: 
           return EVT_NAME_MSG_MME_S1AP;
        case EVT_MSG_MME_EMM: 
           return EVT_NAME_MSG_MME_EMM;
        case EVT_MSG_MME_ESM: 
           return EVT_NAME_MSG_MME_ESM;
        case EVT_MSG_MME_S11: 
           return EVT_NAME_MSG_MME_S11;

        default: 
           break;
    }

    return EVT_NAME_UNKNOWN;
}

void mme_event_s1ap_to_nas(mme_ue_t *ue, S1ap_NAS_PDU_t *nasPdu)
{
    nas_esm_header_t *h = NULL;
    pkbuf_t *sendbuf = NULL;
    event_t e;

    d_assert(ue, return, "Null param");
    d_assert(nasPdu, return, "Null param");

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    sendbuf = pkbuf_alloc(NAS_HEADROOM, nasPdu->size);
    d_assert(sendbuf, return, "Null param");
    memcpy(sendbuf->payload, nasPdu->buf, nasPdu->size);

    d_assert(nas_security_decode(ue, sendbuf) == CORE_OK,
            pkbuf_free(sendbuf); return, "Can't decode NAS_PDU");

    h = sendbuf->payload;
    d_assert(h, pkbuf_free(sendbuf); return, "Null param");
    if (h->protocol_discriminator == NAS_PROTOCOL_DISCRIMINATOR_EMM)
    {
        event_set(&e, EVT_MSG_MME_EMM);
        event_set_param1(&e, (c_uintptr_t)ue->index);
        event_set_param2(&e, (c_uintptr_t)sendbuf);
        mme_event_send(&e);
    }
    else if (h->protocol_discriminator == NAS_PROTOCOL_DISCRIMINATOR_ESM)
    {
        mme_esm_t *esm = mme_esm_find_by_pti(ue, 
                h->procedure_transaction_identity);
        if (esm)
        {
            event_set(&e, EVT_MSG_MME_ESM);
            event_set_param1(&e, (c_uintptr_t)esm->index);
            event_set_param2(&e, (c_uintptr_t)sendbuf);
            mme_event_send(&e);
        }
        else
            d_error("Can't find ESM context(UE:%s, PTI:%d)",
                    ue->imsi_bcd, h->procedure_transaction_identity);
    }
    else
        d_assert(0, pkbuf_free(sendbuf); return, "Unknown protocol:%d", 
                h->protocol_discriminator);
}

void mme_event_emm_to_esm(mme_esm_t *esm, 
                nas_esm_message_container_t *esm_message_container)
{
    pkbuf_t *sendbuf = NULL;
    event_t e;

    d_assert(esm, return, "Null param");
    d_assert(esm_message_container, return, "Null param");

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    sendbuf = pkbuf_alloc(NAS_HEADROOM, esm_message_container->len);
    d_assert(sendbuf, return, "Null param");
    memcpy(sendbuf->payload, 
            esm_message_container->data, esm_message_container->len);

    event_set(&e, EVT_MSG_MME_ESM);
    event_set_param1(&e, (c_uintptr_t)esm->index);
    event_set_param2(&e, (c_uintptr_t)sendbuf);
    mme_event_send(&e);
}

void mme_event_nas_to_s1ap(mme_ue_t *ue, pkbuf_t *pkbuf)
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
