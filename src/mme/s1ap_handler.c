#define TRACE_MODULE _s1ap_handler

#include "core_debug.h"

#include "mme_event.h"

#include "s1ap_conv.h"
#include "s1ap_path.h"
#include "nas_path.h"
#include "mme_gtp_path.h"

#include "s1ap_build.h"
#include "s1ap_handler.h"

void s1ap_handle_s1_setup_request(mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[INET_ADDRSTRLEN];

    S1ap_S1SetupRequestIEs_t *ies = NULL;
    pkbuf_t *s1apbuf = NULL;
    c_uint32_t enb_id;
    int i,j;
    int num_of_tai = 0;

    d_assert(enb, return, "Null param");
    d_assert(enb->s1ap_sock, return, "Null param");
    d_assert(message, return, "Null param");

    ies = &message->s1ap_S1SetupRequestIEs;
    d_assert(ies, return, "Null param");

    s1ap_ENB_ID_to_uint32(&ies->global_ENB_ID.eNB_ID, &enb_id);

    /* Parse Supported TA */
    for (i = 0; i < ies->supportedTAs.list.count; i++)
    {
        S1ap_SupportedTAs_Item_t *tai = NULL;
        S1ap_TAC_t *tAC;

        tai = (S1ap_SupportedTAs_Item_t *)ies->supportedTAs.list.array[i];
        tAC = &tai->tAC;

        for (j = 0; j < tai->broadcastPLMNs.list.count; j++)
        {
            S1ap_PLMNidentity_t *pLMNidentity = NULL;
            pLMNidentity = 
                (S1ap_PLMNidentity_t *)tai->broadcastPLMNs.list.array[j];

            memcpy(&enb->tai[num_of_tai].tac, tAC->buf, sizeof(c_uint16_t));
            enb->tai[num_of_tai].tac = ntohs(enb->tai[num_of_tai].tac);

            memcpy(&enb->tai[num_of_tai].plmn_id, pLMNidentity->buf, 
                    sizeof(plmn_id_t));
            num_of_tai++;
        }
    }

    enb->num_of_tai = num_of_tai;

    if (enb->num_of_tai == 0)
    {
        d_error("No supported TA exist in s1stup_req messages");
    }

    d_assert(enb->s1ap_sock, return,);
    d_trace(3, "[S1AP] S1SetupRequest : eNB[%s:%d] --> MME\n",
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb_id);

    enb->enb_id = enb_id;

    d_assert(s1ap_build_setup_rsp(&s1apbuf) == CORE_OK, 
            return, "build error");
    d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK, , "send error");

    d_assert(enb->s1ap_sock, return,);
    d_trace(3, "[S1AP] S1SetupResponse: eNB[%s:%d] <-- MME\n",
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
        if (ies->presenceMask & S1AP_INITIALUEMESSAGE_IES_S_TMSI_PRESENT)
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
                mme_associate_ue_context(mme_ue, enb_ue);
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
    d_trace(3, "[S1AP] InitialUEMessage : "
            "UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]\n",
            enb_ue->enb_ue_s1ap_id,
            INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            enb->enb_id);

    d_assert(s1ap_send_to_nas(enb_ue, &ies->nas_pdu) == CORE_OK,,
            "s1ap_send_to_nas failed");
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
    d_assert(enb_ue, return, "No UE Context[%d]", ies->eNB_UE_S1AP_ID);

    d_trace(3, "[S1AP] uplinkNASTransport : "
            "UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]\n",
            enb_ue->enb_ue_s1ap_id,
            INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            enb->enb_id);

    d_assert(s1ap_send_to_nas(enb_ue, &ies->nas_pdu) == CORE_OK,,
            "s1ap_send_to_nas failed");
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

    if (enb_ue->mme_ue)
    {
        S1ap_UERadioCapability_t *ue_radio_capa = NULL;
        S1ap_UERadioCapability_t *radio_capa = NULL;
        mme_ue_t *mme_ue = enb_ue->mme_ue;

        ue_radio_capa = &ies->ueRadioCapability;

        /* Release the previous one */
        if (mme_ue->radio_capa)
        {
            radio_capa = (S1ap_UERadioCapability_t *)mme_ue->radio_capa;

            if (radio_capa->buf)
                core_free(radio_capa->buf);
            core_free(mme_ue->radio_capa);
        }
        /* Save UE radio capability */ 
        mme_ue->radio_capa = core_calloc(1, sizeof(S1ap_UERadioCapability_t));
        radio_capa = (S1ap_UERadioCapability_t *)mme_ue->radio_capa;
        d_assert(radio_capa,return,"core_calloc Error");

        radio_capa->size = ue_radio_capa->size;
        radio_capa->buf = 
            core_calloc(radio_capa->size, sizeof(c_uint8_t)); 
        d_assert(radio_capa->buf,return,"core_calloc Error(size=%d)",
                radio_capa->size);
        memcpy(radio_capa->buf, ue_radio_capa->buf, radio_capa->size);
    }

    d_trace(3, "[S1AP] UE Capability Info Indication : "
            "UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]\n",
            enb_ue->enb_ue_s1ap_id,
            INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            enb->enb_id);
}

void s1ap_handle_activate_default_bearer_accept(mme_bearer_t *bearer)
{
    status_t rv;
    mme_bearer_t *dedicated_bearer = NULL;

    d_assert(bearer, return, "Null param");

    rv = mme_gtp_send_modify_bearer_request(bearer);
    d_assert(rv == CORE_OK, return,
            "mme_gtp_send_modify_bearer_request failed");

    dedicated_bearer = mme_bearer_next(bearer);
    while(dedicated_bearer)
    {
        rv = nas_send_activate_dedicated_bearer_context_request(
                dedicated_bearer);
        d_assert(rv == CORE_OK, return,
                "nas_send_activate_dedicated_bearer_context failed");

        dedicated_bearer = mme_bearer_next(dedicated_bearer);
    }
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

    d_trace(3, "[S1AP] Initial Context Setup Response : "
            "UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]\n",
            enb_ue->enb_ue_s1ap_id,
            INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            enb->enb_id);

    for (i = 0; i < ies->e_RABSetupListCtxtSURes.
            s1ap_E_RABSetupItemCtxtSURes.count; i++)
    {
        mme_sess_t *sess = NULL;
        mme_bearer_t *bearer = NULL;
        mme_ue_t *mme_ue = enb_ue->mme_ue;
        S1ap_E_RABSetupItemCtxtSURes_t *e_rab = NULL;

        e_rab = (S1ap_E_RABSetupItemCtxtSURes_t *)
            ies->e_RABSetupListCtxtSURes.s1ap_E_RABSetupItemCtxtSURes.array[i];
        d_assert(e_rab, return, "Null param");

        sess = mme_sess_find_by_ebi(mme_ue, e_rab->e_RAB_ID);
        d_assert(sess, return, "Null param");
        bearer = mme_default_bearer_in_sess(sess);
        d_assert(bearer, return, "Null param");
        memcpy(&bearer->enb_s1u_teid, e_rab->gTP_TEID.buf, 
                sizeof(bearer->enb_s1u_teid));
        bearer->enb_s1u_teid = ntohl(bearer->enb_s1u_teid);
        memcpy(&bearer->enb_s1u_addr, e_rab->transportLayerAddress.buf,
                sizeof(bearer->enb_s1u_addr));

        if (FSM_CHECK(&bearer->sm, esm_state_active))
        {
            status_t rv;
            rv = mme_gtp_send_modify_bearer_request(bearer);
            d_assert(rv == CORE_OK, return,
                    "mme_gtp_send_modify_bearer_request failed");
        }
    }
}

void s1ap_handle_e_rab_setup_response(
        mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[INET_ADDRSTRLEN];
    int i;

    enb_ue_t *enb_ue = NULL;
    S1ap_E_RABSetupResponseIEs_t *ies = NULL;

    d_assert(enb, return, "Null param");
    d_assert(message, return, "Null param");

    ies = &message->s1ap_E_RABSetupResponseIEs;
    d_assert(ies, return, "Null param");

    enb_ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(enb_ue, return, "No UE Context[%d]", ies->eNB_UE_S1AP_ID);

    d_trace(3, "[S1AP] E-RAB Setup Response : "
            "UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]\n",
            enb_ue->enb_ue_s1ap_id,
            INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            enb->enb_id);

    for (i = 0; i < ies->e_RABSetupListBearerSURes.
            s1ap_E_RABSetupItemBearerSURes.count; i++)
    {
        mme_bearer_t *bearer = NULL;
        mme_ue_t *mme_ue = enb_ue->mme_ue;
        S1ap_E_RABSetupItemBearerSURes_t *e_rab = NULL;

        e_rab = (S1ap_E_RABSetupItemBearerSURes_t *)ies->
            e_RABSetupListBearerSURes.s1ap_E_RABSetupItemBearerSURes.array[i];
        d_assert(e_rab, return, "Null param");

        bearer = mme_bearer_find_by_ue_ebi(mme_ue, e_rab->e_RAB_ID);
        d_assert(bearer, return, "Null param");

        memcpy(&bearer->enb_s1u_teid, e_rab->gTP_TEID.buf, 
                sizeof(bearer->enb_s1u_teid));
        bearer->enb_s1u_teid = ntohl(bearer->enb_s1u_teid);
        memcpy(&bearer->enb_s1u_addr, e_rab->transportLayerAddress.buf,
                sizeof(bearer->enb_s1u_addr));

        if (FSM_CHECK(&bearer->sm, esm_state_active))
        {
            status_t rv;

            mme_bearer_t *linked_bearer = mme_linked_bearer(bearer);
            d_assert(bearer, return, "Null param");

            if (bearer->ebi == linked_bearer->ebi)
            {
                rv = mme_gtp_send_modify_bearer_request(bearer);
                d_assert(rv == CORE_OK, return,
                        "mme_gtp_send_modify_bearer_request failed");
            }
            else
            {
                rv = mme_gtp_send_create_bearer_response(bearer);
                d_assert(rv == CORE_OK, return,
                        "mme_gtp_send_create_bearer_response failed");
            }
        }
    }
}

void s1ap_handle_ue_context_release_request(
        mme_enb_t *enb, s1ap_message_t *message)
{
    status_t rv;
    char buf[INET_ADDRSTRLEN];

    enb_ue_t *enb_ue = NULL;
    S1ap_UEContextReleaseRequest_IEs_t *ies = NULL;
    long cause;

    ies = &message->s1ap_UEContextReleaseRequest_IEs;
    d_assert(ies, return, "Null param");

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(ies->mme_ue_s1ap_id);
    d_assert(enb_ue, return, "No UE Context[%d]", ies->mme_ue_s1ap_id);

    d_trace(3, "[S1AP] UE Context Release Request : "
            "UE[mME-UE-S1AP-ID(%d)] --> eNB[%s:%d]\n",
            enb_ue->mme_ue_s1ap_id,
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb->enb_id);

    switch(ies->cause.present)
    {
        case S1ap_Cause_PR_radioNetwork:
            cause = ies->cause.choice.radioNetwork;
            if (cause == S1ap_CauseRadioNetwork_user_inactivity)
            {
                mme_ue_t *mme_ue = enb_ue->mme_ue;

                if (MME_HAVE_SGW_S11_PATH(mme_ue))
                {
                    mme_sess_t *sess = mme_sess_first(mme_ue);
                    while (sess != NULL)
                    {
                        rv = mme_gtp_send_release_access_bearers_response(sess);
                        d_assert(rv == CORE_OK, return, 
                        "mme_gtp_send_release_access_bearers_response failed");

                        sess = mme_sess_next(sess);
                    }
                }
                else
                {
                    S1ap_Cause_t cause;

                    cause.present = S1ap_Cause_PR_nas;
                    cause.choice.nas = S1ap_CauseNas_normal_release;
                    rv = s1ap_send_ue_context_release_commmand(enb_ue, &cause);
                    d_assert(rv == CORE_OK, return, "s1ap send error");
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

    d_trace(3, "[S1AP] UE Context Release Complete : "
            "UE[mME-UE-S1AP-ID(%d)] --> eNB[%s:%d]\n",
            enb_ue->mme_ue_s1ap_id,
            INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            enb->enb_id);

    enb_ue_remove(enb_ue);
}

void s1ap_handle_paging(mme_ue_t *mme_ue)
{
    pkbuf_t *s1apbuf = NULL;
    mme_enb_t *enb = NULL;
    int i;
    status_t rv;

    /* Find enB with matched TAI */
    enb =  mme_enb_first();
    while (enb)
    {
        for (i = 0; i < enb->num_of_tai; i++)
        {
            if (!memcmp(&enb->tai[i], &mme_ue->tai, sizeof(tai_t)))
            {
                if (mme_ue->last_paging_msg)
                    s1apbuf = mme_ue->last_paging_msg;
                else
                {
                    /* Buidl S1Ap Paging message */
                    rv = s1ap_build_paging(&s1apbuf, mme_ue);
                    d_assert(rv == CORE_OK && s1apbuf, return, 
                            "s1ap build error");

                    /* Save it for later use */
                    mme_ue->last_paging_msg = pkbuf_copy(s1apbuf);
                }

                /* Send to enb */
                d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK, return,
                        "s1ap send error");
            }
        }
        enb = mme_enb_next(enb);
    }
}

void s1ap_handle_path_switch_request(
        mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[INET_ADDRSTRLEN];
    int i;

    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;

    S1ap_PathSwitchRequestIEs_t *ies = NULL;
    S1ap_EUTRAN_CGI_t *eutran_cgi;
	S1ap_PLMNidentity_t *pLMNidentity = NULL;
	S1ap_CellIdentity_t	*cell_ID = NULL;
    S1ap_TAI_t *tai;
	S1ap_TAC_t *tAC = NULL;
    S1ap_UESecurityCapabilities_t *ueSecurityCapabilities = NULL;
	S1ap_EncryptionAlgorithms_t	*encryptionAlgorithms = NULL;
	S1ap_IntegrityProtectionAlgorithms_t *integrityProtectionAlgorithms = NULL;
    c_uint16_t eea = 0, eia = 0;

    d_assert(enb, return, "Null param");

    ies = &message->s1ap_PathSwitchRequestIEs;
    d_assert(ies, return, "Null param");

    eutran_cgi = &ies->eutran_cgi;
    d_assert(eutran_cgi, return,);
    pLMNidentity = &eutran_cgi->pLMNidentity;
    d_assert(pLMNidentity && pLMNidentity->size == sizeof(plmn_id_t), return,);
    cell_ID = &eutran_cgi->cell_ID;
    d_assert(cell_ID, return,);

    tai = &ies->tai;
    d_assert(tai, return,);
    pLMNidentity = &tai->pLMNidentity;
    d_assert(pLMNidentity && pLMNidentity->size == sizeof(plmn_id_t), return,);
    tAC = &tai->tAC;
    d_assert(tAC && tAC->size == sizeof(c_uint16_t), return,);

    ueSecurityCapabilities = &ies->ueSecurityCapabilities;
    d_assert(ueSecurityCapabilities, return,);
    encryptionAlgorithms =
        &ueSecurityCapabilities->encryptionAlgorithms;
    integrityProtectionAlgorithms =
        &ueSecurityCapabilities->integrityProtectionAlgorithms;

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(ies->sourceMME_UE_S1AP_ID);
    if (!enb_ue)
    {
        S1ap_Cause_t cause;
        d_error("Cannot find UE from sourceMME-UE-S1AP-ID[%d] and eNB[%s:%d]",
                ies->sourceMME_UE_S1AP_ID,
                INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
                enb->enb_id);

        cause.present = S1ap_Cause_PR_radioNetwork;
        cause.choice.radioNetwork = 
            S1ap_CauseRadioNetwork_unknown_mme_ue_s1ap_id;
        s1ap_send_path_switch_failure(enb, ies->eNB_UE_S1AP_ID,
                ies->sourceMME_UE_S1AP_ID, &cause);
        return;
    }

    mme_ue = enb_ue->mme_ue;
    d_assert(mme_ue, return, "Null param");

    enb_ue->enb_ue_s1ap_id = ies->eNB_UE_S1AP_ID;

    memcpy(&enb_ue->tai.plmn_id, pLMNidentity->buf, 
            sizeof(enb_ue->tai.plmn_id));
    memcpy(&enb_ue->tai.tac, tAC->buf, sizeof(enb_ue->tai.tac));
    enb_ue->tai.tac = ntohs(enb_ue->tai.tac);
    memcpy(&enb_ue->e_cgi.plmn_id, pLMNidentity->buf, 
            sizeof(enb_ue->e_cgi.plmn_id));
    memcpy(&enb_ue->e_cgi.cell_id, cell_ID->buf, sizeof(enb_ue->e_cgi.cell_id));
    enb_ue->e_cgi.cell_id = (ntohl(enb_ue->e_cgi.cell_id) >> 4);

    memcpy(&eea, encryptionAlgorithms->buf, sizeof(eea));
    eea = ntohs(eea);
    if (eea == 0)
        mme_ue->ue_network_capability.eea0 = 1;
    else
        mme_ue->ue_network_capability.eea0 = eea >> 9;

    memcpy(&eia, integrityProtectionAlgorithms->buf, sizeof(eia));
    eia = ntohs(eia);
    if (eea == 0)
        mme_ue->ue_network_capability.eia0 = 1;
    else
        mme_ue->ue_network_capability.eia0 = eia >> 9;

    for (i = 0; i < ies->e_RABToBeSwitchedDLList.
            s1ap_E_RABToBeSwitchedDLItem.count; i++)
    {
        mme_bearer_t *bearer = NULL;
        S1ap_E_RABToBeSwitchedDLItem_t *e_rab = NULL;

        e_rab = (S1ap_E_RABToBeSwitchedDLItem_t *)ies->e_RABToBeSwitchedDLList.
            s1ap_E_RABToBeSwitchedDLItem.array[i];
        d_assert(e_rab, return, "Null param");

        bearer = mme_bearer_find_by_ue_ebi(mme_ue, e_rab->e_RAB_ID);
        d_assert(bearer, return, "Null param");

        memcpy(&bearer->enb_s1u_teid, e_rab->gTP_TEID.buf, 
                sizeof(bearer->enb_s1u_teid));
        bearer->enb_s1u_teid = ntohl(bearer->enb_s1u_teid);
        memcpy(&bearer->enb_s1u_addr, e_rab->transportLayerAddress.buf,
                sizeof(bearer->enb_s1u_addr));
    }

    d_trace(3, "[S1AP] PathSwitchRequest : "
            "UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]\n",
            enb_ue->enb_ue_s1ap_id,
            INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            enb->enb_id);

#if 0
    s1ap_send_path_switch_ack(mme_ue);
#endif
}
