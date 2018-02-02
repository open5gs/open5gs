#define TRACE_MODULE _s1ap_handler

#include "core_debug.h"

#include "mme_event.h"

#include "mme_kdf.h"
#include "s1ap_conv.h"
#include "s1ap_path.h"
#include "nas_path.h"
#include "mme_gtp_path.h"

#include "mme_s11_build.h"
#include "s1ap_build.h"
#include "s1ap_handler.h"

#include "mme_sm.h"

void s1ap_handle_s1_setup_request(mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[CORE_ADDRSTRLEN];

    S1ap_S1SetupRequestIEs_t *ies = NULL;
    pkbuf_t *s1apbuf = NULL;
    c_uint32_t enb_id;
    int i, j;

    d_assert(enb, return, "Null param");
    d_assert(enb->sock, return, "Null param");
    d_assert(message, return, "Null param");

    ies = &message->s1ap_S1SetupRequestIEs;
    d_assert(ies, return, "Null param");

    d_trace(3, "[MME] S1-Setup request\n");

    s1ap_ENB_ID_to_uint32(&ies->global_ENB_ID.eNB_ID, &enb_id);
    d_trace(5, "    IP[%s] ENB_ID[%d]\n", CORE_ADDR(enb->addr, buf), enb_id);

    /* Parse Supported TA */
    enb->num_of_supported_ta_list = 0;
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

            memcpy(&enb->supported_ta_list[enb->num_of_supported_ta_list].tac,
                    tAC->buf, sizeof(c_uint16_t));
            enb->supported_ta_list[enb->num_of_supported_ta_list].tac = 
                ntohs(enb->supported_ta_list
                        [enb->num_of_supported_ta_list].tac);
            d_trace(5, "    TAC[%d]\n",
                    enb->supported_ta_list[enb->num_of_supported_ta_list].tac);

            memcpy(&enb->supported_ta_list
                        [enb->num_of_supported_ta_list].plmn_id,
                    pLMNidentity->buf, sizeof(plmn_id_t));
            d_trace(5, "    PLMN_ID[MCC:%d MNC:%d]\n",
                plmn_id_mcc(&enb->supported_ta_list
                    [enb->num_of_supported_ta_list].plmn_id),
                plmn_id_mnc(&enb->supported_ta_list
                    [enb->num_of_supported_ta_list].plmn_id));
            enb->num_of_supported_ta_list++;
        }
    }

    if (enb->num_of_supported_ta_list == 0)
    {
        d_error("No supported TA exist in s1stup_req messages");
    }

    d_assert(enb->sock, return,);

    d_trace(3, "[MME] S1-Setup response\n");

    d_assert(mme_enb_set_enb_id(enb, enb_id) == CORE_OK,
            return, "hash add error");

    d_assert(s1ap_build_setup_rsp(&s1apbuf) == CORE_OK, 
            return, "build error");
    d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK, , "send error");

    d_assert(enb->sock, return,);
}

void s1ap_handle_initial_ue_message(mme_enb_t *enb, s1ap_message_t *message)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];

    enb_ue_t *enb_ue = NULL;
    S1ap_InitialUEMessage_IEs_t *ies = NULL;
    S1ap_TAI_t *tai = NULL;
	S1ap_PLMNidentity_t *pLMNidentity = NULL;
	S1ap_TAC_t *tAC = NULL;
    S1ap_EUTRAN_CGI_t *eutran_cgi = NULL;
	S1ap_CellIdentity_t	*cell_ID = NULL;

    d_assert(enb, return, "Null param");
    d_assert(enb->sock, return, "Null param");

    ies = &message->s1ap_InitialUEMessage_IEs;
    d_assert(ies, return, "Null param");

    d_trace(3, "[MME] Initial UE Message\n");
    d_trace(5, "    IP[%s] ENB_ID[%d]\n",
            CORE_ADDR(enb->addr, buf), enb->enb_id);

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

            /* Use the first configured plmn_id and mme group id */
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
                d_warn("Unknown UE by S_TMSI[G:%d,C:%d,M_TMSI:0x%x]",
                        guti.mme_gid, guti.mme_code, guti.m_tmsi);
            }
            else
            {
                d_trace(5, "    S_TMSI[G:%d,C:%d,M_TMSI:0x%x] IMSI:[%s]\n",
                        mme_ue->guti.mme_gid,
                        mme_ue->guti.mme_code,
                        mme_ue->guti.m_tmsi,
                        MME_UE_HAVE_IMSI(mme_ue) 
                            ? mme_ue->imsi_bcd : "Unknown");

                /* If NAS(mme_ue_t) has already been associated with
                 * older S1(enb_ue_t) context, send UE context release command
                 * to older S1 context. */
                if (mme_ue->enb_ue)
                {
#if NOT_WORKING
                    d_trace(5, "OLD ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
                        mme_ue->enb_ue->enb_ue_s1ap_id,
                        mme_ue->enb_ue->mme_ue_s1ap_id);
                    rv = s1ap_send_ue_context_release_command(mme_ue->enb_ue, 
                            S1ap_Cause_PR_nas, S1ap_CauseNas_normal_release,
                            S1AP_UE_CTX_REL_NO_ACTION, 0);
                    d_assert(rv == CORE_OK, return, "s1ap send error");
#else
                    d_warn("Implicit S1 release");
                    d_warn("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
                          mme_ue->enb_ue->enb_ue_s1ap_id,
                          mme_ue->enb_ue->mme_ue_s1ap_id);
                    rv = enb_ue_remove(mme_ue->enb_ue);
                    d_assert(rv == CORE_OK,,);
#endif
                }
                mme_ue_associate_enb_ue(mme_ue, enb_ue);
            }
        }
    }

    tai = &ies->tai;
    d_assert(tai, return,);
    pLMNidentity = &tai->pLMNidentity;
    d_assert(pLMNidentity && pLMNidentity->size == sizeof(plmn_id_t), return,);
    tAC = &tai->tAC;
    d_assert(tAC && tAC->size == sizeof(c_uint16_t), return,);

    memcpy(&enb_ue->nas.tai.plmn_id, pLMNidentity->buf, 
            sizeof(enb_ue->nas.tai.plmn_id));
    memcpy(&enb_ue->nas.tai.tac, tAC->buf, sizeof(enb_ue->nas.tai.tac));
    enb_ue->nas.tai.tac = ntohs(enb_ue->nas.tai.tac);
    
    eutran_cgi = &ies->eutran_cgi;
    d_assert(eutran_cgi, return,);
    pLMNidentity = &eutran_cgi->pLMNidentity;
    d_assert(pLMNidentity && pLMNidentity->size == sizeof(plmn_id_t), return,);
    cell_ID = &eutran_cgi->cell_ID;
    d_assert(cell_ID, return,);
    memcpy(&enb_ue->nas.e_cgi.plmn_id, pLMNidentity->buf, 
            sizeof(enb_ue->nas.e_cgi.plmn_id));
    memcpy(&enb_ue->nas.e_cgi.cell_id, cell_ID->buf,
            sizeof(enb_ue->nas.e_cgi.cell_id));
    enb_ue->nas.e_cgi.cell_id = (ntohl(enb_ue->nas.e_cgi.cell_id) >> 4);

    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d] TAC[%d]\n",
        enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id, enb_ue->nas.tai.tac);

    d_assert(s1ap_send_to_nas(enb_ue, &ies->nas_pdu) == CORE_OK,,
            "s1ap_send_to_nas failed");
}

void s1ap_handle_uplink_nas_transport(
        mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[CORE_ADDRSTRLEN];

    enb_ue_t *enb_ue = NULL;
    S1ap_UplinkNASTransport_IEs_t *ies = NULL;

    ies = &message->s1ap_UplinkNASTransport_IEs;
    d_assert(ies, return, "Null param");

    d_trace(3, "[MME] Uplink NAS transport\n");
    d_trace(5, "    IP[%s] ENB_ID[%d]\n",
            CORE_ADDR(enb->addr, buf), enb->enb_id);

    enb_ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(enb_ue, return, "No UE Context[%d]", ies->eNB_UE_S1AP_ID);

    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    d_assert(s1ap_send_to_nas(enb_ue, &ies->nas_pdu) == CORE_OK,,
            "s1ap_send_to_nas failed");
}

void s1ap_handle_ue_capability_info_indication(
        mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[CORE_ADDRSTRLEN];

    enb_ue_t *enb_ue = NULL;
    S1ap_UECapabilityInfoIndicationIEs_t *ies = NULL;

    ies = &message->s1ap_UECapabilityInfoIndicationIEs;
    d_assert(ies, return, "Null param");

    d_trace(3, "[MME] UE capability info indication\n");
    d_trace(5, "    IP[%s] ENB_ID[%d]\n",
            CORE_ADDR(enb->addr, buf), enb->enb_id);

    enb_ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(enb_ue, return, "No UE Context[%d]", ies->eNB_UE_S1AP_ID);

    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

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
                CORE_FREE(radio_capa->buf);
            CORE_FREE(mme_ue->radio_capa);
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
}

void s1ap_handle_initial_context_setup_response(
        mme_enb_t *enb, s1ap_message_t *message)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];
    int i = 0;

    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    S1ap_InitialContextSetupResponseIEs_t *ies = NULL;

    ies = &message->s1ap_InitialContextSetupResponseIEs;
    d_assert(ies, return, "Null param");

    d_trace(3, "[MME] Initial context setup response\n");
    d_trace(5, "    IP[%s] ENB_ID[%d]\n",
            CORE_ADDR(enb->addr, buf), enb->enb_id);

    enb_ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(enb_ue, return, "No UE Context[%d]", ies->eNB_UE_S1AP_ID);
    mme_ue = enb_ue->mme_ue;
    d_assert(mme_ue, return,);

    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    for (i = 0; i < ies->e_RABSetupListCtxtSURes.
            s1ap_E_RABSetupItemCtxtSURes.count; i++)
    {
        mme_sess_t *sess = NULL;
        mme_bearer_t *bearer = NULL;
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
        rv = s1ap_BIT_STRING_to_ip(
                &e_rab->transportLayerAddress, &bearer->enb_s1u_ip);
        d_assert(rv == CORE_OK, return,);

        d_trace(5, "    EBI[%d] ENB-S1U-TEID[%d]\n",
                bearer->ebi, bearer->enb_s1u_teid);

        if (FSM_CHECK(&bearer->sm, esm_state_active))
        {
            int uli_presence = 0;
            if (mme_ue->nas_eps.type != MME_EPS_TYPE_ATTACH_REQUEST)
            {
                uli_presence = 1;
            }
            rv = mme_gtp_send_modify_bearer_request(bearer, uli_presence);
            d_assert(rv == CORE_OK, return, "gtp send failed");
        }
    }
}

void s1ap_handle_initial_context_setup_failure(
        mme_enb_t *enb, s1ap_message_t *message)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];

    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    S1ap_InitialContextSetupFailureIEs_t *ies = NULL;

    ies = &message->s1ap_InitialContextSetupFailureIEs;
    d_assert(ies, return, "Null param");

    enb_ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(enb_ue, return, "No UE Context[%d]", ies->eNB_UE_S1AP_ID);
    mme_ue = enb_ue->mme_ue;

    d_trace(3, "[MME] Initial context setup failure\n");
    d_trace(5, "    IP[%s] ENB_ID[%d]\n",
            CORE_ADDR(enb->addr, buf), enb->enb_id);
    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);
    d_trace(5, "    Cause[Group:%d Cause:%d]\n",
            ies->cause.present, ies->cause.choice.radioNetwork);

    d_assert(mme_ue,,);
    if (mme_ue && FSM_CHECK(&mme_ue->sm, emm_state_registered))
    {
        d_trace(5, "    Registered State\n");

        /*
         * 19.2.2.3 in Spec 36.300
         *
         * In case of failure, eNB and MME behaviours are not mandated.
         *
         * Both implicit release (local release at each node) and
         * explicit release (MME-initiated UE Context Release procedure)
         * may in principle be adopted. The eNB should ensure
         * that no hanging resources remain at the eNB.
         */

#if 0 /* NOTHING TO DO */

#if 0 /* FIXME : Does it needed? */
        rv = nas_send_service_reject(mme_ue,
            EMM_CAUSE_PROTOCOL_ERROR_UNSPECIFIED);
        d_assert(rv == CORE_OK,,
                "nas_send_service_reject() failed");
#endif

#if 1 /* Explicit Release */

        d_trace(5, "    Explicit Release\n");
        rv = s1ap_send_ue_context_release_command(enb_ue,
                S1ap_Cause_PR_nas,
#if 1 /* NAS Cause: Normal Relase */
                S1ap_CauseNas_normal_release,
#else /* NAS Cause : Detach */
                S1ap_CauseNas_detach,
#endif
                S1AP_UE_CTX_REL_UNLINK_MME_UE_CONTEXT, 0);
        d_assert(rv == CORE_OK,, "s1ap send error");

#else  /* Implicit Release */

        d_trace(5, "    Implicit Release\n");
        rv = enb_ue_remove(enb_ue);
        d_assert(rv == CORE_OK,, "enb_ue_remove() failed");

        rv = mme_ue_deassociate(mme_ue);
        d_assert(rv == CORE_OK,, "mme_ue_deassociate() failed");

#endif

#endif
    }
    else
    {
        d_trace(5, "    NOT Registered State\n");
        d_assert(mme_ue,,);
        if (MME_HAVE_SGW_S11_PATH(mme_ue))
        {
            d_trace(5, "    WITH Delete Sesson\n");
            rv = mme_gtp_send_delete_all_sessions(mme_ue);
            d_assert(rv == CORE_OK,,
                "mme_gtp_send_delete_all_sessions failed");
        }
        else
        {
            d_trace(5, "    WITHOUT Delete Sesson\n");
            rv = s1ap_send_ue_context_release_command(enb_ue,
                    S1ap_Cause_PR_nas, S1ap_CauseNas_normal_release,
                    S1AP_UE_CTX_REL_REMOVE_MME_UE_CONTEXT, 0);
            d_assert(rv == CORE_OK,, "s1ap send error");
        }
    }
}

void s1ap_handle_e_rab_setup_response(
        mme_enb_t *enb, s1ap_message_t *message)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];
    int i;

    enb_ue_t *enb_ue = NULL;
    S1ap_E_RABSetupResponseIEs_t *ies = NULL;
    mme_ue_t *mme_ue = NULL;

    d_assert(enb, return, "Null param");
    d_assert(message, return, "Null param");

    ies = &message->s1ap_E_RABSetupResponseIEs;
    d_assert(ies, return, "Null param");

    d_trace(3, "[MME] E-RAB setup response\n");
    d_trace(5, "    IP[%s] ENB_ID[%d]\n",
            CORE_ADDR(enb->addr, buf), enb->enb_id);

    enb_ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(enb_ue, return, "No UE Context[%d]", ies->eNB_UE_S1AP_ID);
    mme_ue = enb_ue->mme_ue;
    d_assert(mme_ue, return,);

    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    for (i = 0; i < ies->e_RABSetupListBearerSURes.
            s1ap_E_RABSetupItemBearerSURes.count; i++)
    {
        mme_bearer_t *bearer = NULL;
        S1ap_E_RABSetupItemBearerSURes_t *e_rab = NULL;

        e_rab = (S1ap_E_RABSetupItemBearerSURes_t *)ies->
            e_RABSetupListBearerSURes.s1ap_E_RABSetupItemBearerSURes.array[i];
        d_assert(e_rab, return, "Null param");

        bearer = mme_bearer_find_by_ue_ebi(mme_ue, e_rab->e_RAB_ID);
        d_assert(bearer, return, "Null param");

        memcpy(&bearer->enb_s1u_teid, e_rab->gTP_TEID.buf, 
                sizeof(bearer->enb_s1u_teid));
        bearer->enb_s1u_teid = ntohl(bearer->enb_s1u_teid);
        rv = s1ap_BIT_STRING_to_ip(
                &e_rab->transportLayerAddress, &bearer->enb_s1u_ip);
        d_assert(rv == CORE_OK, return,);

        d_trace(5, "    EBI[%d]\n", bearer->ebi);

        if (FSM_CHECK(&bearer->sm, esm_state_active))
        {
            status_t rv;

            mme_bearer_t *linked_bearer = mme_linked_bearer(bearer);
            d_assert(linked_bearer, return, "Null param");
            d_trace(5, "    Linked-EBI[%d]\n", linked_bearer->ebi);

            if (bearer->ebi == linked_bearer->ebi)
            {
                rv = mme_gtp_send_modify_bearer_request(bearer, 0);
                d_assert(rv == CORE_OK, return, "gtp send failed");
            }
            else
            {
                rv = mme_gtp_send_create_bearer_response(bearer);
                d_assert(rv == CORE_OK, return, "gtp send failed");
            }
        }
    }
}

void s1ap_handle_ue_context_release_request(
        mme_enb_t *enb, s1ap_message_t *message)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];

    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;

    S1ap_UEContextReleaseRequest_IEs_t *ies = NULL;

    ies = &message->s1ap_UEContextReleaseRequest_IEs;
    d_assert(ies, return, "Null param");

    d_trace(3, "[MME] UE Context release request\n");
    d_trace(5, "    IP[%s] ENB_ID[%d]\n",
            CORE_ADDR(enb->addr, buf), enb->enb_id);

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(ies->mme_ue_s1ap_id);
    d_assert(enb_ue, return, "No ENB UE Context : MME_UE_S1AP_ID[%d]",
            ies->mme_ue_s1ap_id);
    mme_ue = enb_ue->mme_ue;

    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);
    d_trace(5, "    Cause[Group:%d Cause:%d]\n",
            ies->cause.present, ies->cause.choice.radioNetwork);

    switch(ies->cause.present)
    {
        case S1ap_Cause_PR_radioNetwork:
        case S1ap_Cause_PR_transport:
        case S1ap_Cause_PR_protocol:
        case S1ap_Cause_PR_misc:
            break;
        case S1ap_Cause_PR_nas:
            d_warn("NAS-Cause[%d]", ies->cause.choice.nas);
        default:
            d_warn("Invalid cause group[%d]", ies->cause.present);
            break;
    }

    d_assert(mme_ue,,);
    if (mme_ue && FSM_CHECK(&mme_ue->sm, emm_state_registered))
    {
        d_trace(5, "    EMM-Registered\n");
        if (ECM_CONNECTED(mme_ue))
        {
            d_trace(5, "    ECM-Connected\n");
            rv = mme_gtp_send_release_access_bearers_request(mme_ue);
            d_assert(rv == CORE_OK, return, "gtp send failed");
        }
        else
        {
            d_trace(5, "    ECM-Idle\n");
            rv = s1ap_send_ue_context_release_command(enb_ue, 
                    S1ap_Cause_PR_nas, S1ap_CauseNas_normal_release,
                    S1AP_UE_CTX_REL_UNLINK_MME_UE_CONTEXT, 0);
            d_assert(rv == CORE_OK, return, "s1ap send error");
        }
    }
    else
    {
        d_trace(5, "    NOT EMM-Registered\n");
        if (MME_HAVE_SGW_S11_PATH(mme_ue))
        {
            d_trace(5, "    WITH Delete Sesson\n");
            rv = mme_gtp_send_delete_all_sessions(mme_ue);
            d_assert(rv == CORE_OK,,
                "mme_gtp_send_delete_all_sessions failed");
        }
        else
        {
            d_trace(5, "    WITHOUT Delete Sesson\n");
            rv = s1ap_send_ue_context_release_command(enb_ue,
                    S1ap_Cause_PR_nas, S1ap_CauseNas_normal_release,
                    S1AP_UE_CTX_REL_REMOVE_MME_UE_CONTEXT, 0);
            d_assert(rv == CORE_OK,, "s1ap send error");
        }
    }
}

void s1ap_handle_ue_context_release_complete(
        mme_enb_t *enb, s1ap_message_t *message)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];

    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;
    S1ap_UEContextReleaseComplete_IEs_t *ies = NULL;

    ies = &message->s1ap_UEContextReleaseComplete_IEs;
    d_assert(ies, return, "Null param");

    d_trace(3, "[MME] UE Context release complete\n");
    d_trace(5, "    IP[%s] ENB_ID[%d]\n",
            CORE_ADDR(enb->addr, buf), enb->enb_id);

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(ies->mme_ue_s1ap_id);
    d_assert(enb_ue, return, "No UE Context[%d]", ies->mme_ue_s1ap_id);
    mme_ue = enb_ue->mme_ue;

    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    switch (enb_ue->ue_ctx_rel_action)
    {
        case S1AP_UE_CTX_REL_NO_ACTION:
        {
            d_trace(5, "    No Action\n");
            rv = enb_ue_remove(enb_ue);
            d_assert(rv == CORE_OK,, "enb_ue_remove() failed");
            break;
        }
        case S1AP_UE_CTX_REL_UNLINK_MME_UE_CONTEXT:
        {
            d_trace(5, "    Action: Unlink UE(mme) context\n");
            rv = enb_ue_remove(enb_ue);
            d_assert(rv == CORE_OK,, "enb_ue_remove() failed");

            d_assert(mme_ue,,);
            rv = mme_ue_deassociate(mme_ue);
            d_assert(rv == CORE_OK,, "mme_ue_deassociate() failed");
            break;
        }
        case S1AP_UE_CTX_REL_REMOVE_MME_UE_CONTEXT:
        {
            d_trace(5, "    Action: Remove UE(mme) context\n");
            rv = enb_ue_remove(enb_ue);
            d_assert(rv == CORE_OK,, "enb_ue_removeI() failed");

            d_assert(mme_ue,,);
            rv = mme_ue_remove(mme_ue);
            d_assert(rv == CORE_OK,, "mme_ue_remove() failed");
            break;
        }
        case S1AP_UE_CTX_REL_DELETE_INDIRECT_TUNNEL:
        {
            d_trace(5, "    Action: Delete indirect tunnel\n");

            rv = source_ue_deassociate_target_ue(enb_ue);
            d_assert(rv == CORE_OK,,
                    "source_ue_deassociate_target_ue() failed");

            rv = enb_ue_remove(enb_ue);
            d_assert(rv == CORE_OK,, "enb_ue_removeI() failed");

            d_assert(mme_ue,,);
            if (MME_HAVE_SGW_S11_PATH(mme_ue))
            {
                rv = mme_gtp_send_delete_indirect_data_forwarding_tunnel_request(mme_ue);
                d_assert(rv == CORE_OK,, "mme_gtp_send_delete_indirect_data_"
                    "forwarding_tunnel_request() failed");
            }
            else
            {
                mme_sess_t *sess = NULL;
                mme_bearer_t *bearer = NULL;

                d_warn("GTP-C(S11) has already been deleted");
                sess = mme_sess_first(mme_ue);
                while(sess)
                {
                    bearer = mme_bearer_first(sess);
                    while(bearer)
                    {
                        CLEAR_INDIRECT_TUNNEL(bearer);

                        bearer = mme_bearer_next(bearer);
                    }
                    sess = mme_sess_next(sess);
                }
            }
            break;
        }
        default:
        {
            d_assert(0,, "Invalid Action[%d]", enb_ue->ue_ctx_rel_action);
            break;
        }
    }
}

void s1ap_handle_paging(mme_ue_t *mme_ue)
{
    pkbuf_t *s1apbuf = NULL;
    hash_index_t *hi = NULL;
    mme_enb_t *enb = NULL;
    int i;
    status_t rv;

    /* Find enB with matched TAI */
    for (hi = mme_enb_first(); hi; hi = mme_enb_next(hi))
    {
        enb = mme_enb_this(hi);
        for (i = 0; i < enb->num_of_supported_ta_list; i++)
        {
            if (!memcmp(&enb->supported_ta_list[i],
                        &mme_ue->tai, sizeof(tai_t)))
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
    }
}

void s1ap_handle_path_switch_request(
        mme_enb_t *enb, s1ap_message_t *message)
{
    char buf[CORE_ADDRSTRLEN];
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

    d_trace(3, "[MME] Path switch request\n");
    d_trace(5, "    IP[%s] ENB_ID[%d]\n",
            CORE_ADDR(enb->addr, buf), enb->enb_id);

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
        d_error("Cannot find UE from sourceMME-UE-S1AP-ID[%d] and eNB[%s:%d]",
                ies->sourceMME_UE_S1AP_ID,
                CORE_ADDR(enb->addr, buf), enb->enb_id);

        s1ap_send_path_switch_failure(enb, ies->eNB_UE_S1AP_ID,
                ies->sourceMME_UE_S1AP_ID,
                S1ap_Cause_PR_radioNetwork,
                S1ap_CauseRadioNetwork_unknown_mme_ue_s1ap_id);
        return;
    }

    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    mme_ue = enb_ue->mme_ue;
    d_assert(mme_ue, return, "Null param");

    if (SECURITY_CONTEXT_IS_VALID(mme_ue))
    {
        mme_ue->nhcc++;
        mme_kdf_nh(mme_ue->kasme, mme_ue->nh, mme_ue->nh);
    }
    else
    {
        s1ap_send_path_switch_failure(enb, ies->eNB_UE_S1AP_ID,
                ies->sourceMME_UE_S1AP_ID,
                S1ap_Cause_PR_nas, S1ap_CauseNas_authentication_failure);
        return;
    }

    enb_ue->enb_ue_s1ap_id = ies->eNB_UE_S1AP_ID;

    memcpy(&mme_ue->tai.plmn_id, pLMNidentity->buf, 
            sizeof(mme_ue->tai.plmn_id));
    memcpy(&mme_ue->tai.tac, tAC->buf, sizeof(mme_ue->tai.tac));
    mme_ue->tai.tac = ntohs(mme_ue->tai.tac);
    memcpy(&mme_ue->e_cgi.plmn_id, pLMNidentity->buf, 
            sizeof(mme_ue->e_cgi.plmn_id));
    memcpy(&mme_ue->e_cgi.cell_id, cell_ID->buf, sizeof(mme_ue->e_cgi.cell_id));
    mme_ue->e_cgi.cell_id = (ntohl(mme_ue->e_cgi.cell_id) >> 4);

    memcpy(&eea, encryptionAlgorithms->buf, sizeof(eea));
    eea = ntohs(eea);
    mme_ue->ue_network_capability.eea = eea >> 9;
    mme_ue->ue_network_capability.eea0 = 1;

    memcpy(&eia, integrityProtectionAlgorithms->buf, sizeof(eia));
    eia = ntohs(eia);
    mme_ue->ue_network_capability.eia = eia >> 9;
    mme_ue->ue_network_capability.eia0 = 0;

    for (i = 0; i < ies->e_RABToBeSwitchedDLList.
            s1ap_E_RABToBeSwitchedDLItem.count; i++)
    {
        status_t rv;

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
        rv = s1ap_BIT_STRING_to_ip(
                &e_rab->transportLayerAddress, &bearer->enb_s1u_ip);
        d_assert(rv == CORE_OK, return,);

        GTP_COUNTER_INCREMENT(
                mme_ue, GTP_COUNTER_MODIFY_BEARER_BY_PATH_SWITCH);

        rv = mme_gtp_send_modify_bearer_request(bearer, 1);
        d_assert(rv == CORE_OK, return, "gtp send failed");
    }

    /* Switch to enb */
    enb_ue_switch_to_enb(enb_ue, enb);
}

void s1ap_handle_handover_required(mme_enb_t *enb, s1ap_message_t *message)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];

    enb_ue_t *source_ue = NULL;
    mme_ue_t *mme_ue = NULL;

    S1ap_HandoverRequiredIEs_t *ies = NULL;

    d_assert(enb, return,);

    ies = &message->s1ap_HandoverRequiredIEs;
    d_assert(ies, return,);

    d_trace(3, "[MME] Handover required\n");
    d_trace(5, "    IP[%s] ENB_ID[%d]\n",
            CORE_ADDR(enb->addr, buf), enb->enb_id);

    source_ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(source_ue, return,
            "Cannot find UE for eNB-UE-S1AP-ID[%d] and eNB[%s:%d]",
            ies->eNB_UE_S1AP_ID,
            CORE_ADDR(enb->addr, buf), enb->enb_id);
    d_assert(source_ue->mme_ue_s1ap_id == ies->mme_ue_s1ap_id, return,
            "Conflict MME-UE-S1AP-ID : %d != %d\n",
            source_ue->mme_ue_s1ap_id, ies->mme_ue_s1ap_id);

    d_trace(5, "    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);

    mme_ue = source_ue->mme_ue;
    d_assert(mme_ue, return,);

    if (SECURITY_CONTEXT_IS_VALID(mme_ue))
    {
        mme_ue->nhcc++;
        mme_kdf_nh(mme_ue->kasme, mme_ue->nh, mme_ue->nh);
    }
    else
    {
        rv = s1ap_send_handover_preparation_failure(source_ue, &ies->cause);
        d_assert(rv == CORE_OK, return, "s1ap send error");

        return;
    }

    source_ue->handover_type = ies->handoverType;

    rv = s1ap_send_handover_request(mme_ue, ies);
    d_assert(rv == CORE_OK,, "s1ap send error");
}

void s1ap_handle_handover_request_ack(mme_enb_t *enb, s1ap_message_t *message)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];
    int i;

    enb_ue_t *source_ue = NULL;
    enb_ue_t *target_ue = NULL;
    mme_ue_t *mme_ue = NULL;

    S1ap_HandoverRequestAcknowledgeIEs_t *ies = NULL;

    d_assert(enb, return,);

    ies = &message->s1ap_HandoverRequestAcknowledgeIEs;
    d_assert(ies, return,);

    d_trace(3, "[MME] Handover request acknowledge\n");
    d_trace(5, "    IP[%s] ENB_ID[%d]\n",
            CORE_ADDR(enb->addr, buf), enb->enb_id);

    target_ue = enb_ue_find_by_mme_ue_s1ap_id(ies->mme_ue_s1ap_id);
    d_assert(target_ue, return,
            "Cannot find UE for MME-UE-S1AP-ID[%d] and eNB[%s:%d]",
            ies->mme_ue_s1ap_id,
            CORE_ADDR(enb->addr, buf), enb->enb_id);

    target_ue->enb_ue_s1ap_id = ies->eNB_UE_S1AP_ID;

    source_ue = target_ue->source_ue;
    d_assert(source_ue, return,);
    mme_ue = source_ue->mme_ue;
    d_assert(mme_ue, return,);

    d_trace(5, "    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);
    d_trace(5, "    Target : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            target_ue->enb_ue_s1ap_id, target_ue->mme_ue_s1ap_id);

    for (i = 0; i < ies->e_RABAdmittedList.s1ap_E_RABAdmittedItem.count; i++)
    {
        mme_bearer_t *bearer = NULL;
        S1ap_E_RABAdmittedItem_t *e_rab = NULL;

        e_rab = (S1ap_E_RABAdmittedItem_t *)ies->e_RABAdmittedList.
            s1ap_E_RABAdmittedItem.array[i];
        d_assert(e_rab, return, "Null param");

        bearer = mme_bearer_find_by_ue_ebi(mme_ue, e_rab->e_RAB_ID);
        d_assert(bearer, return, "Null param");

        memcpy(&bearer->target_s1u_teid, e_rab->gTP_TEID.buf, 
                sizeof(bearer->target_s1u_teid));
        bearer->target_s1u_teid = ntohl(bearer->target_s1u_teid);
        rv = s1ap_BIT_STRING_to_ip(
                &e_rab->transportLayerAddress, &bearer->target_s1u_ip);
        d_assert(rv == CORE_OK, return,);

        if (e_rab->dL_transportLayerAddress && e_rab->dL_gTP_TEID)
        {
            d_assert(e_rab->dL_gTP_TEID->buf, return,);
            d_assert(e_rab->dL_transportLayerAddress->buf, return,);
            memcpy(&bearer->enb_dl_teid, e_rab->dL_gTP_TEID->buf, 
                    sizeof(bearer->enb_dl_teid));
            bearer->enb_dl_teid = ntohl(bearer->enb_dl_teid);
            rv = s1ap_BIT_STRING_to_ip(
                    e_rab->dL_transportLayerAddress, &bearer->enb_dl_ip);
            d_assert(rv == CORE_OK, return,);
        }

        if (e_rab->uL_S1ap_TransportLayerAddress && e_rab->uL_S1ap_GTP_TEID)
        {
            d_assert(e_rab->uL_S1ap_GTP_TEID->buf, return,);
            d_assert(e_rab->uL_S1ap_TransportLayerAddress->buf, return,);
            memcpy(&bearer->enb_ul_teid, e_rab->uL_S1ap_GTP_TEID->buf, 
                    sizeof(bearer->enb_ul_teid));
            bearer->enb_ul_teid = ntohl(bearer->enb_ul_teid);
            rv = s1ap_BIT_STRING_to_ip(
                    e_rab->uL_S1ap_TransportLayerAddress, &bearer->enb_ul_ip);
            d_assert(rv == CORE_OK, return,);
        }
    }

    S1AP_STORE_DATA(
        &mme_ue->container, &ies->target_ToSource_TransparentContainer);

    if (i > 0)
    {
        rv = mme_gtp_send_create_indirect_data_forwarding_tunnel_request(
                mme_ue);
        d_assert(rv == CORE_OK, return, "gtp send failed");
    }
    else
    {
        rv = s1ap_send_handover_command(source_ue);
        d_assert(rv == CORE_OK, return, "gtp send failed");
    }
}

void s1ap_handle_handover_failure(mme_enb_t *enb, s1ap_message_t *message)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];

    S1ap_HandoverFailureIEs_t *ies = NULL;
    enb_ue_t *target_ue = NULL;
    enb_ue_t *source_ue = NULL;

    d_assert(enb, return,);

    ies = &message->s1ap_HandoverFailureIEs;
    d_assert(ies, return,);

    d_trace(3, "[MME] Handover failure\n");
    d_trace(5, "    IP[%s] ENB_ID[%d]\n",
            CORE_ADDR(enb->addr, buf), enb->enb_id);

    target_ue = enb_ue_find_by_mme_ue_s1ap_id(ies->mme_ue_s1ap_id);
    d_assert(target_ue, return,
            "Cannot find UE for MME-UE-S1AP-ID[%d] and eNB[%s:%d]",
            ies->mme_ue_s1ap_id,
            CORE_ADDR(enb->addr, buf), enb->enb_id);

    source_ue = target_ue->source_ue;
    d_assert(source_ue, return,);

    d_trace(5, "    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);
    d_trace(5, "    Target : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            target_ue->enb_ue_s1ap_id, target_ue->mme_ue_s1ap_id);

    rv = s1ap_send_handover_preparation_failure(source_ue, &ies->cause);
    d_assert(rv == CORE_OK, return, "s1ap send error");

    rv = s1ap_send_ue_context_release_command(
        target_ue, S1ap_Cause_PR_radioNetwork,
        S1ap_CauseRadioNetwork_ho_failure_in_target_EPC_eNB_or_target_system,
        S1AP_UE_CTX_REL_DELETE_INDIRECT_TUNNEL, 0);
    d_assert(rv == CORE_OK, return, "s1ap send error");
}

void s1ap_handle_handover_cancel(mme_enb_t *enb, s1ap_message_t *message)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];

    enb_ue_t *source_ue = NULL;
    enb_ue_t *target_ue = NULL;

    S1ap_HandoverCancelIEs_t *ies = NULL;

    d_assert(enb, return,);

    ies = &message->s1ap_HandoverCancelIEs;
    d_assert(ies, return,);

    d_trace(3, "[MME] Handover cancel\n");
    d_trace(5, "    IP[%s] ENB_ID[%d]\n",
            CORE_ADDR(enb->addr, buf), enb->enb_id);

    source_ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(source_ue, return,
            "Cannot find UE for eNB-UE-S1AP-ID[%d] and eNB[%s:%d]",
            ies->eNB_UE_S1AP_ID,
            CORE_ADDR(enb->addr, buf), enb->enb_id);
    d_assert(source_ue->mme_ue_s1ap_id == ies->mme_ue_s1ap_id, return,
            "Conflict MME-UE-S1AP-ID : %d != %d\n",
            source_ue->mme_ue_s1ap_id, ies->mme_ue_s1ap_id);

    target_ue = source_ue->target_ue;
    d_assert(target_ue, return,);

    d_trace(5, "    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);
    d_trace(5, "    Target : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            target_ue->enb_ue_s1ap_id, target_ue->mme_ue_s1ap_id);

    rv = s1ap_send_handover_cancel_ack(source_ue);
    d_assert(rv == CORE_OK,, "s1ap send error");

    rv = s1ap_send_ue_context_release_command(
            target_ue, S1ap_Cause_PR_radioNetwork,
            S1ap_CauseRadioNetwork_handover_cancelled,
            S1AP_UE_CTX_REL_DELETE_INDIRECT_TUNNEL, 300);
    d_assert(rv == CORE_OK, return, "s1ap send error");

    d_trace(3, "[MME] Handover Cancel : "
            "UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]\n",
            source_ue->enb_ue_s1ap_id,
            CORE_ADDR(enb->addr, buf), enb->enb_id);
}

void s1ap_handle_enb_status_transfer(mme_enb_t *enb, s1ap_message_t *message)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];

    enb_ue_t *source_ue = NULL, *target_ue = NULL;

    S1ap_ENBStatusTransferIEs_t *ies = NULL;

    d_assert(enb, return,);

    ies = &message->s1ap_ENBStatusTransferIEs;
    d_assert(ies, return,);

    d_trace(3, "[MME] ENB status transfer\n");
    d_trace(5, "    IP[%s] ENB_ID[%d]\n",
            CORE_ADDR(enb->addr, buf), enb->enb_id);

    source_ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(source_ue, return,
            "Cannot find UE for eNB-UE-S1AP-ID[%d] and eNB[%s:%d]",
            ies->eNB_UE_S1AP_ID,
            CORE_ADDR(enb->addr, buf), enb->enb_id);
    d_assert(source_ue->mme_ue_s1ap_id == ies->mme_ue_s1ap_id, return,
            "Conflict MME-UE-S1AP-ID : %d != %d\n",
            source_ue->mme_ue_s1ap_id, ies->mme_ue_s1ap_id);

    target_ue = source_ue->target_ue;
    d_assert(target_ue, return,);

    d_trace(5, "    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);
    d_trace(5, "    Target : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            target_ue->enb_ue_s1ap_id, target_ue->mme_ue_s1ap_id);

    rv = s1ap_send_mme_status_transfer(target_ue, ies);
    d_assert(rv == CORE_OK,,);
}

void s1ap_handle_handover_notification(mme_enb_t *enb, s1ap_message_t *message)
{
    status_t rv;
    char buf[CORE_ADDRSTRLEN];

    enb_ue_t *source_ue = NULL;
    enb_ue_t *target_ue = NULL;
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    S1ap_HandoverNotifyIEs_t *ies = NULL;
    S1ap_EUTRAN_CGI_t *eutran_cgi;
	S1ap_PLMNidentity_t *pLMNidentity = NULL;
	S1ap_CellIdentity_t	*cell_ID = NULL;
    S1ap_TAI_t *tai;
	S1ap_TAC_t *tAC = NULL;

    d_assert(enb, return,);

    ies = &message->s1ap_HandoverNotifyIEs;
    d_assert(ies, return,);

    d_trace(3, "[MME] Handover notification\n");
    d_trace(5, "    IP[%s] ENB_ID[%d]\n",
            CORE_ADDR(enb->addr, buf), enb->enb_id);

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

    target_ue = enb_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(target_ue, return,
            "Cannot find UE for eNB-UE-S1AP-ID[%d] and eNB[%s:%d]",
            ies->eNB_UE_S1AP_ID,
            CORE_ADDR(enb->addr, buf), enb->enb_id);
    d_assert(target_ue->mme_ue_s1ap_id == ies->mme_ue_s1ap_id, return,
            "Conflict MME-UE-S1AP-ID : %d != %d\n",
            target_ue->mme_ue_s1ap_id, ies->mme_ue_s1ap_id);

    source_ue = target_ue->source_ue;
    d_assert(source_ue, return,);
    mme_ue = source_ue->mme_ue;
    d_assert(mme_ue, return,);

    d_trace(5, "    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);
    d_trace(5, "    Target : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            target_ue->enb_ue_s1ap_id, target_ue->mme_ue_s1ap_id);

    mme_ue_associate_enb_ue(mme_ue, target_ue);

    memcpy(&mme_ue->tai.plmn_id, pLMNidentity->buf, 
            sizeof(mme_ue->tai.plmn_id));
    memcpy(&mme_ue->tai.tac, tAC->buf, sizeof(mme_ue->tai.tac));
    mme_ue->tai.tac = ntohs(mme_ue->tai.tac);
    memcpy(&mme_ue->e_cgi.plmn_id, pLMNidentity->buf, 
            sizeof(mme_ue->e_cgi.plmn_id));
    memcpy(&mme_ue->e_cgi.cell_id, cell_ID->buf, sizeof(mme_ue->e_cgi.cell_id));
    mme_ue->e_cgi.cell_id = (ntohl(mme_ue->e_cgi.cell_id) >> 4);

    sess = mme_sess_first(mme_ue);
    while(sess)
    {
        bearer = mme_bearer_first(sess);
        while(bearer)
        {
            bearer->enb_s1u_teid = bearer->target_s1u_teid;
            memcpy(&bearer->enb_s1u_ip, &bearer->target_s1u_ip, sizeof(ip_t));

            GTP_COUNTER_INCREMENT(
                    mme_ue, GTP_COUNTER_MODIFY_BEARER_BY_HANDOVER_NOTIFY);

            rv = mme_gtp_send_modify_bearer_request(bearer, 1);
            d_assert(rv == CORE_OK, return, "gtp send failed");

            bearer = mme_bearer_next(bearer);
        }
        sess = mme_sess_next(sess);
    }
}
