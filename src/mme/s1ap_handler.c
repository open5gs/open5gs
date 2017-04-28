#define TRACE_MODULE _s1ap_handler

#include "core_debug.h"

#include "mme_event.h"

#include "s1ap_conv.h"
#include "s1ap_build.h"
#include "s1ap_path.h"
#include "nas_message.h"
#include "nas_security.h"

#include "s1ap_handler.h"

static void event_s1ap_to_nas(enb_ue_t *ue, S1ap_NAS_PDU_t *nasPdu)
{
    nas_esm_header_t *h = NULL;
    pkbuf_t *nasbuf = NULL;
    event_t e;
    int mac_failed = 0;

    d_assert(ue, return, "Null param");
    d_assert(nasPdu, return, "Null param");

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    nasbuf = pkbuf_alloc(NAS_HEADROOM, nasPdu->size);
    d_assert(nasbuf, return, "Null param");
    memcpy(nasbuf->payload, nasPdu->buf, nasPdu->size);

    if (ue->mme_ue)
    {
        d_assert(nas_security_decode(ue->mme_ue, nasbuf, 
                    &mac_failed) == CORE_OK,
                 pkbuf_free(nasbuf);return,
                 "nas_security_decode failed");
    }
    else
    {
        /* FIXME */
        c_uint32_t hsize = sizeof(nas_security_header_t);
        nas_security_header_t *sh = NULL;

        sh = nasbuf->payload;
        switch(sh->security_header_type)
        {
            case NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE:
            case NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE:
            {
                break;
            }
            default:
                mac_failed = 1;
                d_warn("Security Protected (securiry header type:0x%x)", 
                        sh->security_header_type);
                pkbuf_header(nasbuf, -hsize);
        }
    }

    h = nasbuf->payload;
    d_assert(h, pkbuf_free(nasbuf); return, "Null param");
    if (h->protocol_discriminator == NAS_PROTOCOL_DISCRIMINATOR_EMM)
    {
        event_set(&e, MME_EVT_EMM_UE_MSG);
        event_set_param1(&e, (c_uintptr_t)ue->index);
        event_set_param2(&e, (c_uintptr_t)nasbuf);
        event_set_param3(&e, (c_uintptr_t)mac_failed);
        mme_event_send(&e);
    }
    else if (h->protocol_discriminator == NAS_PROTOCOL_DISCRIMINATOR_ESM)
    {
        mme_bearer_t *bearer = NULL;
        mme_ue_t *mme_ue = ue->mme_ue;

        if (!mme_ue)
        {
            d_error("No mme_ue exists");
            pkbuf_free(nasbuf);
            return;
        }

        bearer = mme_bearer_find_by_pti(mme_ue, 
                h->procedure_transaction_identity);
        if (bearer)
        {
            event_set(&e, MME_EVT_ESM_BEARER_MSG);
            event_set_param1(&e, (c_uintptr_t)bearer->index);
            event_set_param2(&e, (c_uintptr_t)nasbuf);
            event_set_param3(&e, (c_uintptr_t)mac_failed);
            mme_event_send(&e);
        }
        else
            d_error("Can't find ESM context(UE:%s, PTI:%d)",
                    mme_ue->imsi_bcd, h->procedure_transaction_identity);
    }
    else
        d_assert(0, pkbuf_free(nasbuf); return, "Unknown protocol:%d", 
                h->protocol_discriminator);
}


void s1ap_handle_s1_setup_request(mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[INET_ADDRSTRLEN];

    S1ap_S1SetupRequestIEs_t *ies = NULL;
    pkbuf_t *s1apbuf = NULL;
    c_uint32_t enb_id;

    d_assert(enb, return, "Null param");
    d_assert(enb->s1ap_sock, return, "Null param");
    d_assert(message, return, "Null param");

    ies = &message->s1ap_S1SetupRequestIEs;
    d_assert(ies, return, "Null param");

    s1ap_ENB_ID_to_uint32(&ies->global_ENB_ID.eNB_ID, &enb_id);

#if 0 /* FIXME : does it needed? */
    if (mme_ctx_enb_find_by_enb_id(enb_id))
    {
        S1ap_Cause_t cause;
        d_error("eNB-id[0x%x] duplicated from [%s]", enb_id,
                INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf));

        cause.present = S1ap_Cause_PR_protocol;
        cause.choice.protocol = 
            S1ap_CauseProtocol_message_not_compatible_with_receiver_state;
        rv = s1ap_build_setup_failure(&s1apbuf, cause);
    }
#endif
    d_assert(enb->s1ap_sock, return,);
    d_info("[S1AP] S1SetupRequest : eNB[%s:%d] --> MME",
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb_id);

    enb->enb_id = enb_id;

    d_assert(s1ap_build_setup_rsp(&s1apbuf) == CORE_OK, 
            return, "build error");
    d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK, , "send error");

    d_assert(enb->s1ap_sock, return,);
    d_info("[S1AP] S1SetupResponse: eNB[%s:%d] <-- MME",
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb_id);
}

void s1ap_handle_initial_ue_message(mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[INET_ADDRSTRLEN];

    enb_ue_t *ue = NULL;
    S1ap_InitialUEMessage_IEs_t *ies = NULL;
    S1ap_TAI_t *tai = NULL;
	S1ap_PLMNidentity_t *pLMNidentity = NULL;
	S1ap_TAC_t *tAC = NULL;
    S1ap_EUTRAN_CGI_t *eutran_cgi = NULL;
	S1ap_CellIdentity_t	*cell_ID = NULL;

    d_assert(enb, return, "Null param");

    ies = &message->s1ap_InitialUEMessage_IEs;
    d_assert(ies, return, "Null param");

    tai = &ies->tai;
    d_assert(tai, return,);
    pLMNidentity = &tai->pLMNidentity;
    d_assert(pLMNidentity && pLMNidentity->size == sizeof(plmn_id_t), return,);
    tAC = &tai->tAC;
    d_assert(tAC && tAC->size == sizeof(c_uint16_t), return,);

    eutran_cgi = &ies->eutran_cgi;
    d_assert(eutran_cgi, return,);
    pLMNidentity = &eutran_cgi->pLMNidentity;
    d_assert(pLMNidentity && pLMNidentity->size == sizeof(plmn_id_t), return,);
    cell_ID = &eutran_cgi->cell_ID;
    d_assert(cell_ID, return,);

    ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    if (!ue)
    {
        ue = enb_ue_add(enb);
        d_assert(ue, return, "Null param");

        ue->enb_ue_s1ap_id = ies->eNB_UE_S1AP_ID;
    }
    else
    {
        d_warn("Duplicated: eNB[0x%x] sends "
            "Initial-UE Message[eNB-UE-S1AP-ID(%d)]",
            enb->enb_id, ue->enb_ue_s1ap_id);
    }

    memcpy(&ue->tai.plmn_id, pLMNidentity->buf, sizeof(ue->tai.plmn_id));
    memcpy(&ue->tai.tac, tAC->buf, sizeof(ue->tai.tac));
    ue->tai.tac = ntohs(ue->tai.tac);
    memcpy(&ue->e_cgi.plmn_id, pLMNidentity->buf, sizeof(ue->e_cgi.plmn_id));
    memcpy(&ue->e_cgi.cell_id, cell_ID->buf, sizeof(ue->e_cgi.cell_id));
    ue->e_cgi.cell_id = (ntohl(ue->e_cgi.cell_id) >> 4);

    d_assert(enb->s1ap_sock, enb_ue_remove(ue); return,);
    d_info("[S1AP] InitialUEMessage : UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]",
        ue->enb_ue_s1ap_id,
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb->enb_id);

    event_s1ap_to_nas(ue, &ies->nas_pdu);
}

void s1ap_handle_uplink_nas_transport(
        mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[INET_ADDRSTRLEN];

    enb_ue_t *ue = NULL;
    S1ap_UplinkNASTransport_IEs_t *ies = NULL;

    ies = &message->s1ap_UplinkNASTransport_IEs;
    d_assert(ies, return, "Null param");

    ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(ue, return, "Null param");

    d_info("[S1AP] uplinkNASTransport : UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]",
        ue->enb_ue_s1ap_id,
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb->enb_id);

    event_s1ap_to_nas(ue, &ies->nas_pdu);
}

void s1ap_handle_ue_capability_info_indication(
        mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[INET_ADDRSTRLEN];

    enb_ue_t *ue = NULL;
    S1ap_UECapabilityInfoIndicationIEs_t *ies = NULL;

    ies = &message->s1ap_UECapabilityInfoIndicationIEs;
    d_assert(ies, return, "Null param");

    ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(ue, return, "No UE Context[%d]", ies->eNB_UE_S1AP_ID);

    d_info("[S1AP] UE Capability Info Indication : "
            "UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]",
        ue->enb_ue_s1ap_id,
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb->enb_id);
}

void s1ap_handle_initial_context_setup_response(
        mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[INET_ADDRSTRLEN];
    int i = 0;

    enb_ue_t *ue = NULL;
    S1ap_InitialContextSetupResponseIEs_t *ies = NULL;

    ies = &message->s1ap_InitialContextSetupResponseIEs;
    d_assert(ies, return, "Null param");

    ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(ue, return, "No UE Context[%d]", ies->eNB_UE_S1AP_ID);

    d_info("[S1AP] Initial Context Setup Response : "
            "UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]",
        ue->enb_ue_s1ap_id,
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb->enb_id);

    for (i = 0; 
        i < ies->e_RABSetupListCtxtSURes.s1ap_E_RABSetupItemCtxtSURes.count; 
        i++)
    {
        event_t e;
        mme_bearer_t *bearer = NULL;
        mme_ue_t *mme_ue = ue->mme_ue;
        S1ap_E_RABSetupItemCtxtSURes_t *e_rab = NULL;

        e_rab = (S1ap_E_RABSetupItemCtxtSURes_t *)
            ies->e_RABSetupListCtxtSURes.s1ap_E_RABSetupItemCtxtSURes.array[i];
        d_assert(e_rab, return, "Null param");

        bearer = mme_bearer_find_by_ebi(mme_ue, e_rab->e_RAB_ID);
        d_assert(bearer, return, "Null param");
        memcpy(&bearer->enb_s1u_teid, e_rab->gTP_TEID.buf, 
                sizeof(bearer->enb_s1u_teid));
        bearer->enb_s1u_teid = ntohl(bearer->enb_s1u_teid);
        memcpy(&bearer->enb_s1u_addr, e_rab->transportLayerAddress.buf,
                sizeof(bearer->enb_s1u_addr));

        event_set(&e, MME_EVT_ESM_BEARER_TO_S11);
        event_set_param1(&e, (c_uintptr_t)bearer->index);
        event_set_param2(&e, (c_uintptr_t)GTP_MODIFY_BEARER_REQUEST_TYPE);
        mme_event_send(&e);
    }
}

