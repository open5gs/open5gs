#define TRACE_MODULE _s1ap_handler

#include "core_debug.h"

#include "mme_event.h"

#include "s1ap_conv.h"
#include "s1ap_build.h"
#include "s1ap_path.h"
#include "nas_message.h"
#include "nas_security.h"
#include "mme_s11_build.h"
#include "mme_s11_path.h"

#include "s1ap_handler.h"

static void event_s1ap_to_nas(enb_ue_t *enb_ue, S1ap_NAS_PDU_t *nasPdu)
{
    nas_esm_header_t *h = NULL;
    pkbuf_t *nasbuf = NULL;
    event_t e;
    int mac_failed = 0;

    d_assert(enb_ue, return, "Null param");
    d_assert(nasPdu, return, "Null param");

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    nasbuf = pkbuf_alloc(NAS_HEADROOM, nasPdu->size);
    d_assert(nasbuf, return, "Null param");
    memcpy(nasbuf->payload, nasPdu->buf, nasPdu->size);

    if (enb_ue->mme_ue)
    {
        d_assert(nas_security_decode(
                    enb_ue->mme_ue, nasbuf, &mac_failed) == CORE_OK,
            pkbuf_free(nasbuf);return, "nas_security_decode failed");
    }
    else
    {
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
    enb_ue->mac_failed = mac_failed;

    h = nasbuf->payload;
    d_assert(h, pkbuf_free(nasbuf); return, "Null param");
    if (h->protocol_discriminator == NAS_PROTOCOL_DISCRIMINATOR_EMM)
    {
        event_set(&e, MME_EVT_EMM_UE_MSG);
        event_set_param1(&e, (c_uintptr_t)enb_ue->index);
        event_set_param2(&e, (c_uintptr_t)nasbuf);
        mme_event_send(&e);
    }
    else if (h->protocol_discriminator == NAS_PROTOCOL_DISCRIMINATOR_ESM)
    {
        mme_bearer_t *bearer = NULL;
        mme_ue_t *mme_ue = enb_ue->mme_ue;

        if (!mme_ue)
        {
            d_error("No mme_ue exists");
            pkbuf_free(nasbuf);
            return;
        }

        bearer = mme_bearer_find_by_ue_pti(mme_ue, 
                h->procedure_transaction_identity);
        if (bearer)
        {
            event_set(&e, MME_EVT_ESM_BEARER_MSG);
            event_set_param1(&e, (c_uintptr_t)bearer->index);
            event_set_param2(&e, (c_uintptr_t)nasbuf);
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

    enb_ue_t *enb_ue = NULL;
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

    enb_ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    if (!enb_ue)
    {
        enb_ue = enb_ue_add(enb);
        d_assert(enb_ue, return, "Null param");

        enb_ue->enb_ue_s1ap_id = ies->eNB_UE_S1AP_ID;

        /* Find MME_UE if s_tmsi included */
        if (ies->presenceMask &S1AP_INITIALUEMESSAGE_IES_S_TMSI_PRESENT)
        {
            S1ap_S_TMSI_t *s_tmsi = &ies->s_tmsi;
            served_gummei_t *served_gummei = &mme_self()->served_gummei[0];
            guti_t guti;
            mme_ue_t *mme_ue = NULL;

            memset(&guti, 0, sizeof(guti_t));

            /* FIXME : Use the first configured plmn_id and mme group id */
            memcpy(&guti.plmn_id, &served_gummei->plmn_id[0], PLMN_ID_LEN);
            guti.mme_gid = served_gummei->mme_gid[0];

            /* size must be 1 */
            memcpy(&guti.mme_code, s_tmsi->mMEC.buf, s_tmsi->mMEC.size);
            /* size must be 4 */
            memcpy(&guti.m_tmsi, s_tmsi->m_TMSI.buf, s_tmsi->m_TMSI.size);
            guti.m_tmsi = ntohl(guti.m_tmsi);

            mme_ue = mme_ue_find_by_guti(&guti);
            if (!mme_ue)
            {
                d_error("Can not find mme_ue with mme_code = %d, m_tmsi = %d",
                        guti.mme_code, guti.m_tmsi);
            }
            else
            {
                mme_ue->enb_ue = enb_ue;
                enb_ue->mme_ue = mme_ue;
            }
        }
    }

    memcpy(&enb_ue->tai.plmn_id, pLMNidentity->buf, 
            sizeof(enb_ue->tai.plmn_id));
    memcpy(&enb_ue->tai.tac, tAC->buf, sizeof(enb_ue->tai.tac));
    enb_ue->tai.tac = ntohs(enb_ue->tai.tac);
    memcpy(&enb_ue->e_cgi.plmn_id, pLMNidentity->buf, 
            sizeof(enb_ue->e_cgi.plmn_id));
    memcpy(&enb_ue->e_cgi.cell_id, cell_ID->buf, sizeof(enb_ue->e_cgi.cell_id));
    enb_ue->e_cgi.cell_id = (ntohl(enb_ue->e_cgi.cell_id) >> 4);

    d_assert(enb->s1ap_sock, enb_ue_remove(enb_ue); return,);
    d_info("[S1AP] InitialUEMessage : UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]",
        enb_ue->enb_ue_s1ap_id,
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb->enb_id);

    event_s1ap_to_nas(enb_ue, &ies->nas_pdu);
}

void s1ap_handle_uplink_nas_transport(
        mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[INET_ADDRSTRLEN];

    enb_ue_t *enb_ue = NULL;
    S1ap_UplinkNASTransport_IEs_t *ies = NULL;

    ies = &message->s1ap_UplinkNASTransport_IEs;
    d_assert(ies, return, "Null param");

    enb_ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(enb_ue, return, "Null param");

    d_info("[S1AP] uplinkNASTransport : UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]",
        enb_ue->enb_ue_s1ap_id,
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb->enb_id);

    event_s1ap_to_nas(enb_ue, &ies->nas_pdu);
}

void s1ap_handle_ue_capability_info_indication(
        mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[INET_ADDRSTRLEN];

    enb_ue_t *enb_ue = NULL;
    S1ap_UECapabilityInfoIndicationIEs_t *ies = NULL;

    ies = &message->s1ap_UECapabilityInfoIndicationIEs;
    d_assert(ies, return, "Null param");

    enb_ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(enb_ue, return, "No UE Context[%d]", ies->eNB_UE_S1AP_ID);

    d_info("[S1AP] UE Capability Info Indication : "
            "UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]",
            enb_ue->enb_ue_s1ap_id,
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb->enb_id);
}

void s1ap_handle_initial_context_setup_response(
        mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[INET_ADDRSTRLEN];
    int i = 0;

    enb_ue_t *enb_ue = NULL;
    S1ap_InitialContextSetupResponseIEs_t *ies = NULL;

    ies = &message->s1ap_InitialContextSetupResponseIEs;
    d_assert(ies, return, "Null param");

    enb_ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(enb_ue, return, "No UE Context[%d]", ies->eNB_UE_S1AP_ID);

    d_info("[S1AP] Initial Context Setup Response : "
            "UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]",
            enb_ue->enb_ue_s1ap_id,
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb->enb_id);

    for (i = 0; 
        i < ies->e_RABSetupListCtxtSURes.s1ap_E_RABSetupItemCtxtSURes.count; 
        i++)
    {
        event_t e;
        mme_bearer_t *bearer = NULL;
        mme_ue_t *mme_ue = enb_ue->mme_ue;
        S1ap_E_RABSetupItemCtxtSURes_t *e_rab = NULL;

        e_rab = (S1ap_E_RABSetupItemCtxtSURes_t *)
            ies->e_RABSetupListCtxtSURes.s1ap_E_RABSetupItemCtxtSURes.array[i];
        d_assert(e_rab, return, "Null param");

        bearer = mme_bearer_find_by_ue_ebi(mme_ue, e_rab->e_RAB_ID);
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

void s1ap_handle_ue_context_release_request(
        mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[INET_ADDRSTRLEN];

    enb_ue_t *enb_ue = NULL;
    S1ap_UEContextReleaseRequest_IEs_t *ies = NULL;
    long cause;

    ies = &message->s1ap_UEContextReleaseRequest_IEs;
    d_assert(ies, return, "Null param");

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(ies->mme_ue_s1ap_id);
    d_assert(enb_ue, return, "No UE Context[%d]", ies->mme_ue_s1ap_id);

    d_info("[S1AP] UE Context Release Request : "
            "UE[mME-UE-S1AP-ID(%d)] --> eNB[%s:%d]",
            enb_ue->mme_ue_s1ap_id,
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb->enb_id);

    switch(ies->cause.present)
    {
        case S1ap_Cause_PR_radioNetwork:
            cause = ies->cause.choice.radioNetwork;
            if (cause == S1ap_CauseRadioNetwork_user_inactivity)
            {
                int release_access_bearer_needed = 0;
                pkbuf_t *pkbuf = NULL;
                mme_ue_t *mme_ue = enb_ue->mme_ue;
                status_t rv;

                if (mme_ue)
                {
                    mme_sess_t *sess = mme_sess_first(mme_ue);
                    while (sess != NULL)
                    {
                        mme_bearer_t *bearer = mme_default_bearer_in_sess(sess);
                        if (bearer != NULL)
                        {
                            rv = mme_s11_build_release_access_bearers_request(
                                    &pkbuf);
                            d_assert(rv == CORE_OK, return, "S11 build error");

                            rv = mme_s11_send_to_sgw(bearer->sgw, 
                                    GTP_RELEASE_ACCESS_BEARERS_REQUEST_TYPE, 
                                    sess->sgw_s11_teid, pkbuf);
                            d_assert(rv == CORE_OK, return, "S11 send error");

                            release_access_bearer_needed = 1;
                        }
                        sess = mme_sess_next(sess);
                    }
                }

                if (!release_access_bearer_needed)
                {
                    s1ap_handle_release_access_bearers_response(enb, enb_ue);
                }
            }
            else
            {
                d_warn("Not implmented (radioNetwork cause : %d)", cause);
            }
            break;
        case S1ap_Cause_PR_transport:
            cause = ies->cause.choice.transport;
            d_warn("Not implmented (transport cause : %d)", cause);
            break;
        case S1ap_Cause_PR_nas:
            cause = ies->cause.choice.nas;
            d_warn("Not implmented (nas cause : %d)", cause);
            break;
        case S1ap_Cause_PR_protocol:
            cause = ies->cause.choice.protocol;
            d_warn("Not implmented (protocol cause : %d)", cause);
            break;
        case S1ap_Cause_PR_misc:
            cause = ies->cause.choice.misc;
            d_warn("Not implmented (misc cause : %d)", cause);
            break;
        default:
            d_warn("Invalid cause type : %d", ies->cause.present);
            break;
    }
}

void s1ap_handle_release_access_bearers_response(
        mme_enb_t *enb, enb_ue_t *enb_ue)
{
    status_t rv;
    pkbuf_t *s1apbuf;
    S1ap_Cause_t cause;

    d_assert(enb, return, "Null param");
    d_assert(enb_ue, return, "Null param");

    cause.present = S1ap_Cause_PR_nas;
    cause.choice.nas = S1ap_CauseNas_normal_release;

    rv = s1ap_build_ue_context_release_commmand(
            &s1apbuf, enb_ue, &cause);
    d_assert(rv == CORE_OK && s1apbuf, return, "s1ap build error");

    d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK,, "s1ap send error");
}

void s1ap_handle_ue_context_release_complete(
        mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[INET_ADDRSTRLEN];

    enb_ue_t *enb_ue = NULL;
    S1ap_UEContextReleaseComplete_IEs_t *ies = NULL;

    ies = &message->s1ap_UEContextReleaseComplete_IEs;
    d_assert(ies, return, "Null param");

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(ies->mme_ue_s1ap_id);
    d_assert(enb_ue, return, "No UE Context[%d]", ies->mme_ue_s1ap_id);

    d_info("[S1AP] UE Context Release Complete : "
            "UE[mME-UE-S1AP-ID(%d)] --> eNB[%s:%d]",
            enb_ue->mme_ue_s1ap_id,
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb->enb_id);

    d_assert(enb_ue_remove(enb_ue) == CORE_OK,, "No ENB_UE context");
}

