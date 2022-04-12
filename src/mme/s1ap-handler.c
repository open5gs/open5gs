/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "mme-event.h"
#include "mme-timer.h"

#include "s1ap-path.h"
#include "nas-path.h"
#include "mme-gtp-path.h"
#include "sgsap-types.h"
#include "sgsap-path.h"

#include "mme-s11-build.h"
#include "s1ap-build.h"
#include "s1ap-handler.h"

#include "mme-path.h"
#include "mme-sm.h"

static bool served_tai_is_found(mme_enb_t *enb)
{
    int i;
    int served_tai_index;

    for (i = 0; i < enb->num_of_supported_ta_list; i++) {
        served_tai_index = mme_find_served_tai(&enb->supported_ta_list[i]);
        if (served_tai_index >= 0 &&
                served_tai_index < OGS_MAX_NUM_OF_SERVED_TAI) {
            ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);
            return true;
        }
    }

    return false;
}

static bool maximum_number_of_enbs_is_reached(void)
{
    mme_enb_t *enb = NULL, *next_enb = NULL;
    int number_of_enbs_online = 0;

    ogs_list_for_each_safe(&mme_self()->enb_list, next_enb, enb) {
        if (enb->state.s1_setup_success) {
            number_of_enbs_online++;
        }
    }

    return number_of_enbs_online >= ogs_app()->max.gnb;
}

void s1ap_handle_s1_setup_request(mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, j;

    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_S1SetupRequest_t *S1SetupRequest = NULL;

    S1AP_S1SetupRequestIEs_t *ie = NULL;
    S1AP_Global_ENB_ID_t *Global_ENB_ID = NULL;
    S1AP_SupportedTAs_t *SupportedTAs = NULL;
    S1AP_PagingDRX_t *PagingDRX = NULL;

    uint32_t enb_id;
    S1AP_Cause_PR group = S1AP_Cause_PR_NOTHING;
    long cause = 0;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    S1SetupRequest = &initiatingMessage->value.choice.S1SetupRequest;
    ogs_assert(S1SetupRequest);

    ogs_debug("S1SetupRequest");

    for (i = 0; i < S1SetupRequest->protocolIEs.list.count; i++) {
        ie = S1SetupRequest->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_Global_ENB_ID:
            Global_ENB_ID = &ie->value.choice.Global_ENB_ID;
            break;
        case S1AP_ProtocolIE_ID_id_SupportedTAs:
            SupportedTAs = &ie->value.choice.SupportedTAs;
            break;
        case S1AP_ProtocolIE_ID_id_DefaultPagingDRX:
            PagingDRX = &ie->value.choice.PagingDRX;
            break;
        default:
            break;
        }
    }

    ogs_assert(Global_ENB_ID);

    ogs_s1ap_ENB_ID_to_uint32(&Global_ENB_ID->eNB_ID, &enb_id);
    ogs_debug("    IP[%s] ENB_ID[%d]", OGS_ADDR(enb->sctp.addr, buf), enb_id);

    if (PagingDRX)
        ogs_debug("    PagingDRX[%ld]", *PagingDRX);

    mme_enb_set_enb_id(enb, enb_id);

    ogs_assert(SupportedTAs);
    /* Parse Supported TA */
    enb->num_of_supported_ta_list = 0;
    for (i = 0; i < SupportedTAs->list.count; i++) {
        S1AP_SupportedTAs_Item_t *SupportedTAs_Item = NULL;
        S1AP_TAC_t *tAC = NULL;

        SupportedTAs_Item = 
            (S1AP_SupportedTAs_Item_t *)SupportedTAs->list.array[i];
        ogs_assert(SupportedTAs_Item);
        tAC = &SupportedTAs_Item->tAC;
        ogs_assert(tAC);

        for (j = 0; j < SupportedTAs_Item->broadcastPLMNs.list.count; j++) {
            S1AP_PLMNidentity_t *pLMNidentity = NULL;
            pLMNidentity = (S1AP_PLMNidentity_t *)
                SupportedTAs_Item->broadcastPLMNs.list.array[j];
            ogs_assert(pLMNidentity);

            memcpy(&enb->supported_ta_list[enb->num_of_supported_ta_list].tac,
                    tAC->buf, sizeof(uint16_t));
            enb->supported_ta_list[enb->num_of_supported_ta_list].tac = 
                be16toh(enb->supported_ta_list
                        [enb->num_of_supported_ta_list].tac);
            memcpy(&enb->supported_ta_list
                        [enb->num_of_supported_ta_list].plmn_id,
                    pLMNidentity->buf, sizeof(ogs_plmn_id_t));
            ogs_debug("    PLMN_ID[MCC:%d MNC:%d] TAC[%d]",
                ogs_plmn_id_mcc(&enb->supported_ta_list
                    [enb->num_of_supported_ta_list].plmn_id),
                ogs_plmn_id_mnc(&enb->supported_ta_list
                    [enb->num_of_supported_ta_list].plmn_id),
                enb->supported_ta_list[enb->num_of_supported_ta_list].tac);
            enb->num_of_supported_ta_list++;
        }
    }

    if (maximum_number_of_enbs_is_reached()) {
        ogs_warn("S1-Setup failure:");
        ogs_warn("    Maximum number of eNBs reached");
        group = S1AP_Cause_PR_misc;
        cause = S1AP_CauseMisc_unspecified;

        ogs_assert(OGS_OK == s1ap_send_s1_setup_failure(enb, group, cause));
        return;
    }

    if (enb->num_of_supported_ta_list == 0) {
        ogs_warn("S1-Setup failure:");
        ogs_warn("    No supported TA exist in S1-Setup request");
        group = S1AP_Cause_PR_misc;
        cause = S1AP_CauseMisc_unspecified;

        ogs_assert(OGS_OK == s1ap_send_s1_setup_failure(enb, group, cause));
        return;
    }

    if (!served_tai_is_found(enb)) {
        ogs_warn("S1-Setup failure:");
        ogs_warn("    Cannot find Served TAI. Check 'mme.tai' configuration");
        group = S1AP_Cause_PR_misc;
        cause = S1AP_CauseMisc_unknown_PLMN;

        ogs_assert(OGS_OK == s1ap_send_s1_setup_failure(enb, group, cause));
        return;
    }

    enb->state.s1_setup_success = true;
    ogs_assert(OGS_OK == s1ap_send_s1_setup_response(enb));
}

void s1ap_handle_initial_ue_message(mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    int i;
    char buf[OGS_ADDRSTRLEN];

    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_InitialUEMessage_t *InitialUEMessage = NULL;

    S1AP_InitialUEMessage_IEs_t *ie = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_NAS_PDU_t *NAS_PDU = NULL;
    S1AP_TAI_t *TAI = NULL;
    S1AP_EUTRAN_CGI_t *EUTRAN_CGI = NULL;
    S1AP_S_TMSI_t *S_TMSI = NULL;

    S1AP_PLMNidentity_t *pLMNidentity = NULL;
    S1AP_TAC_t *tAC = NULL;
    S1AP_CellIdentity_t *cell_ID = NULL;

    enb_ue_t *enb_ue = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    InitialUEMessage = &initiatingMessage->value.choice.InitialUEMessage;
    ogs_assert(InitialUEMessage);

    ogs_info("InitialUEMessage");

    for (i = 0; i < InitialUEMessage->protocolIEs.list.count; i++) {
        ie = InitialUEMessage->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_NAS_PDU:
            NAS_PDU = &ie->value.choice.NAS_PDU;
            break;
        case S1AP_ProtocolIE_ID_id_TAI:
            TAI = &ie->value.choice.TAI;
            break;
        case S1AP_ProtocolIE_ID_id_EUTRAN_CGI:
            EUTRAN_CGI = &ie->value.choice.EUTRAN_CGI;
            break;
        case S1AP_ProtocolIE_ID_id_S_TMSI:
            S_TMSI = &ie->value.choice.S_TMSI;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!ENB_UE_S1AP_ID) {
        ogs_error("No ENB_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }
    enb_ue = enb_ue_find_by_enb_ue_s1ap_id(enb, *ENB_UE_S1AP_ID);
    if (!enb_ue) {
        enb_ue = enb_ue_add(enb, *ENB_UE_S1AP_ID);
        ogs_assert(enb_ue);

        /* Find MME_UE if S_TMSI included */
        if (S_TMSI) {
            served_gummei_t *served_gummei = &mme_self()->served_gummei[0];
            ogs_nas_eps_guti_t nas_guti;
            mme_ue_t *mme_ue = NULL;

            memset(&nas_guti, 0, sizeof(ogs_nas_eps_guti_t));

            /* Use the first configured plmn_id and mme group id */
            ogs_nas_from_plmn_id(&nas_guti.nas_plmn_id,
                    &served_gummei->plmn_id[0]);
            nas_guti.mme_gid = served_gummei->mme_gid[0];

            /* size must be 1 */
            memcpy(&nas_guti.mme_code, S_TMSI->mMEC.buf, S_TMSI->mMEC.size);
            /* size must be 4 */
            memcpy(&nas_guti.m_tmsi, S_TMSI->m_TMSI.buf, S_TMSI->m_TMSI.size);
            nas_guti.m_tmsi = be32toh(nas_guti.m_tmsi);

            mme_ue = mme_ue_find_by_guti(&nas_guti);
            if (!mme_ue) {
                ogs_info("Unknown UE by S_TMSI[G:%d,C:%d,M_TMSI:0x%x]",
                        nas_guti.mme_gid, nas_guti.mme_code, nas_guti.m_tmsi);
            } else {
                ogs_info("    S_TMSI[G:%d,C:%d,M_TMSI:0x%x] IMSI:[%s]",
                        mme_ue->current.guti.mme_gid,
                        mme_ue->current.guti.mme_code,
                        mme_ue->current.guti.m_tmsi,
                        MME_UE_HAVE_IMSI(mme_ue) 
                            ? mme_ue->imsi_bcd : "Unknown");

                /* If NAS(mme_ue_t) has already been associated with
                 * older S1(enb_ue_t) context */
                if (ECM_CONNECTED(mme_ue)) {
                    /* Previous S1(enb_ue_t) context the holding timer(30secs)
                     * is started.
                     * Newly associated S1(enb_ue_t) context holding timer
                     * is stopped. */
                    ogs_debug("Start S1 Holding Timer");
                    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
                            mme_ue->enb_ue->enb_ue_s1ap_id,
                            mme_ue->enb_ue->mme_ue_s1ap_id);

                    /* De-associate S1 with NAS/EMM */
                    enb_ue_deassociate(mme_ue->enb_ue);

                    ogs_assert(OGS_OK ==
                        s1ap_send_ue_context_release_command(mme_ue->enb_ue,
                            S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release,
                            S1AP_UE_CTX_REL_S1_CONTEXT_REMOVE, 0));
                }
                mme_ue_associate_enb_ue(mme_ue, enb_ue);
            }
        }
    }

    if (!NAS_PDU) {
        ogs_error("No NAS_PDU");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    if (!TAI) {
        ogs_error("No TAI");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    if (!EUTRAN_CGI) {
        ogs_error("No EUTRAN_CGI");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    pLMNidentity = &TAI->pLMNidentity;
    ogs_assert(pLMNidentity && pLMNidentity->size == sizeof(ogs_plmn_id_t));
    tAC = &TAI->tAC;
    ogs_assert(tAC && tAC->size == sizeof(uint16_t));

    memcpy(&enb_ue->saved.tai.plmn_id, pLMNidentity->buf,
            sizeof(enb_ue->saved.tai.plmn_id));
    memcpy(&enb_ue->saved.tai.tac, tAC->buf, sizeof(enb_ue->saved.tai.tac));
    enb_ue->saved.tai.tac = be16toh(enb_ue->saved.tai.tac);
    
    pLMNidentity = &EUTRAN_CGI->pLMNidentity;
    ogs_assert(pLMNidentity && pLMNidentity->size == sizeof(ogs_plmn_id_t));
    cell_ID = &EUTRAN_CGI->cell_ID;
    ogs_assert(cell_ID);
    memcpy(&enb_ue->saved.e_cgi.plmn_id, pLMNidentity->buf,
            sizeof(enb_ue->saved.e_cgi.plmn_id));
    memcpy(&enb_ue->saved.e_cgi.cell_id, cell_ID->buf,
            sizeof(enb_ue->saved.e_cgi.cell_id));
    enb_ue->saved.e_cgi.cell_id = (be32toh(enb_ue->saved.e_cgi.cell_id) >> 4);

    ogs_info("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d] TAC[%d] CellID[0x%x]",
        enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id,
        enb_ue->saved.tai.tac, enb_ue->saved.e_cgi.cell_id);

    s1ap_send_to_nas(enb_ue,
            S1AP_ProcedureCode_id_initialUEMessage, NAS_PDU);
}

void s1ap_handle_uplink_nas_transport(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_UplinkNASTransport_t *UplinkNASTransport = NULL;

    S1AP_UplinkNASTransport_IEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_NAS_PDU_t *NAS_PDU = NULL;
    S1AP_EUTRAN_CGI_t *EUTRAN_CGI = NULL;
    S1AP_TAI_t *TAI = NULL;

    S1AP_PLMNidentity_t *pLMNidentity = NULL;
    S1AP_TAC_t *tAC = NULL;
    S1AP_CellIdentity_t *cell_ID = NULL;

    enb_ue_t *enb_ue = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    UplinkNASTransport = &initiatingMessage->value.choice.UplinkNASTransport;
    ogs_assert(UplinkNASTransport);

    ogs_debug("UplinkNASTransport");

    for (i = 0; i < UplinkNASTransport->protocolIEs.list.count; i++) {
        ie = UplinkNASTransport->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_NAS_PDU:
            NAS_PDU = &ie->value.choice.NAS_PDU;
            break;
        case S1AP_ProtocolIE_ID_id_EUTRAN_CGI:
            EUTRAN_CGI = &ie->value.choice.EUTRAN_CGI;
            break;
        case S1AP_ProtocolIE_ID_id_TAI:
            TAI = &ie->value.choice.TAI;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!enb_ue) {
        ogs_error("No eNB UE Context : MME_UE_S1AP_ID[%lld]",
                (long long)*MME_UE_S1AP_ID);
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id));
        return;
    }

    if (!ENB_UE_S1AP_ID) {
        ogs_error("No ENB_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    if (!NAS_PDU) {
        ogs_error("No NAS_PDU");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    if (!EUTRAN_CGI) {
        ogs_error("No EUTRAN_CGI");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    if (!TAI) {
        ogs_error("No TAI");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    pLMNidentity = &EUTRAN_CGI->pLMNidentity;
    ogs_assert(pLMNidentity && pLMNidentity->size == sizeof(ogs_plmn_id_t));
    cell_ID = &EUTRAN_CGI->cell_ID;
    ogs_assert(cell_ID);
    memcpy(&enb_ue->saved.e_cgi.plmn_id, pLMNidentity->buf,
            sizeof(enb_ue->saved.e_cgi.plmn_id));
    memcpy(&enb_ue->saved.e_cgi.cell_id, cell_ID->buf,
            sizeof(enb_ue->saved.e_cgi.cell_id));
    enb_ue->saved.e_cgi.cell_id = (be32toh(enb_ue->saved.e_cgi.cell_id) >> 4);

    pLMNidentity = &TAI->pLMNidentity;
    ogs_assert(pLMNidentity && pLMNidentity->size == sizeof(ogs_plmn_id_t));
    tAC = &TAI->tAC;
    ogs_assert(tAC && tAC->size == sizeof(uint16_t));

    memcpy(&enb_ue->saved.tai.plmn_id, pLMNidentity->buf,
            sizeof(enb_ue->saved.tai.plmn_id));
    memcpy(&enb_ue->saved.tai.tac, tAC->buf, sizeof(enb_ue->saved.tai.tac));
    enb_ue->saved.tai.tac = be16toh(enb_ue->saved.tai.tac);

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d] TAC[%d] CellID[0x%x]",
        enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id,
        enb_ue->saved.tai.tac, enb_ue->saved.e_cgi.cell_id);

    /* Copy Stream-No/TAI/ECGI from enb_ue */
    if (enb_ue->mme_ue) {
        mme_ue_t *mme_ue = enb_ue->mme_ue;

        memcpy(&mme_ue->tai, &enb_ue->saved.tai, sizeof(ogs_eps_tai_t));
        memcpy(&mme_ue->e_cgi, &enb_ue->saved.e_cgi, sizeof(ogs_e_cgi_t));
    } else {
        ogs_fatal("No UE Context in UplinkNASTransport");
        ogs_assert_if_reached();
    }

    s1ap_send_to_nas(enb_ue,
            S1AP_ProcedureCode_id_uplinkNASTransport, NAS_PDU);
}

void s1ap_handle_ue_capability_info_indication(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_UECapabilityInfoIndication_t *UECapabilityInfoIndication = NULL;

    S1AP_UECapabilityInfoIndicationIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_UERadioCapability_t *UERadioCapability = NULL;

    enb_ue_t *enb_ue = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    UECapabilityInfoIndication =
        &initiatingMessage->value.choice.UECapabilityInfoIndication;
    ogs_assert(UECapabilityInfoIndication);

    ogs_debug("UECapabilityInfoIndication");

    for (i = 0; i < UECapabilityInfoIndication->protocolIEs.list.count; i++) {
        ie = UECapabilityInfoIndication->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_UERadioCapability:
            UERadioCapability = &ie->value.choice.UERadioCapability;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!enb_ue) {
        ogs_error("No eNB UE Context : MME_UE_S1AP_ID[%lld]",
                (long long)*MME_UE_S1AP_ID);
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id));
        return;
    }

    if (!ENB_UE_S1AP_ID) {
        ogs_error("No ENB_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    if (enb_ue->mme_ue) {
        ogs_assert(UERadioCapability);
        OGS_ASN_STORE_DATA(&enb_ue->mme_ue->ueRadioCapability,
                UERadioCapability);
    }
}

void s1ap_handle_initial_context_setup_response(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_InitialContextSetupResponse_t *InitialContextSetupResponse = NULL;

    S1AP_InitialContextSetupResponseIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABSetupListCtxtSURes_t *E_RABSetupListCtxtSURes = NULL;

    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    InitialContextSetupResponse =
        &successfulOutcome->value.choice.InitialContextSetupResponse;
    ogs_assert(InitialContextSetupResponse);

    ogs_debug("InitialContextSetupResponse");

    for (i = 0; i < InitialContextSetupResponse->protocolIEs.list.count; i++) {
        ie = InitialContextSetupResponse->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_E_RABSetupListCtxtSURes:
            E_RABSetupListCtxtSURes =
                &ie->value.choice.E_RABSetupListCtxtSURes;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!enb_ue) {
        ogs_error("No eNB UE Context : MME_UE_S1AP_ID[%lld]",
                (long long)*MME_UE_S1AP_ID);
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id));
        return;
    }

    if (!ENB_UE_S1AP_ID) {
        ogs_error("No ENB_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    mme_ue = enb_ue->mme_ue;
    ogs_expect_or_return(mme_ue);

    if (E_RABSetupListCtxtSURes) {
        for (i = 0; i < E_RABSetupListCtxtSURes->list.count; i++) {
            S1AP_E_RABSetupItemCtxtSUResIEs_t *item = NULL;
            S1AP_E_RABSetupItemCtxtSURes_t *e_rab = NULL;

            mme_bearer_t *bearer = NULL;

            item = (S1AP_E_RABSetupItemCtxtSUResIEs_t *)
                E_RABSetupListCtxtSURes->list.array[i];
            if (!item) {
                ogs_error("No S1AP_E_RABSetupItemCtxtSUResIEs_t");
                ogs_assert(OGS_OK ==
                    s1ap_send_error_indication2(mme_ue,
                    S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
                return;
            }

            e_rab = &item->value.choice.E_RABSetupItemCtxtSURes;
            if (!e_rab) {
                ogs_error("No E_RABSetupItemCtxtSURes");
                ogs_assert(OGS_OK ==
                    s1ap_send_error_indication2(mme_ue,
                    S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
                return;
            }

            bearer = mme_bearer_find_by_ue_ebi(mme_ue, e_rab->e_RAB_ID);
            if (!bearer) {
                ogs_error("No Bearer [%d]", (int)e_rab->e_RAB_ID);
                ogs_assert(OGS_OK ==
                    s1ap_send_error_indication2(mme_ue,
                        S1AP_Cause_PR_radioNetwork,
                        S1AP_CauseRadioNetwork_unknown_E_RAB_ID));
                return;
            }

            memcpy(&bearer->enb_s1u_teid, e_rab->gTP_TEID.buf,
                    sizeof(bearer->enb_s1u_teid));
            bearer->enb_s1u_teid = be32toh(bearer->enb_s1u_teid);
            rv = ogs_asn_BIT_STRING_to_ip(
                    &e_rab->transportLayerAddress, &bearer->enb_s1u_ip);
            if (rv != OGS_OK) {
                ogs_error("No transportLayerAddress [%d]",
                        (int)e_rab->e_RAB_ID);
                ogs_assert(OGS_OK ==
                    s1ap_send_error_indication2(mme_ue,
                        S1AP_Cause_PR_protocol,
                        S1AP_CauseProtocol_abstract_syntax_error_falsely_constructed_message));
                return;
            }

            ogs_debug("    EBI[%d] ENB-S1U-TEID[%d]",
                    bearer->ebi, bearer->enb_s1u_teid);

            if (OGS_FSM_CHECK(&bearer->sm, esm_state_active)) {
                ogs_debug("    NAS_EPS Type[%d]", mme_ue->nas_eps.type);
                int uli_presence = 0;
                if (mme_ue->nas_eps.type != MME_EPS_TYPE_ATTACH_REQUEST) {
                    ogs_debug("    ### ULI PRESENT ###");
                    uli_presence = 1;
                }
                ogs_assert(OGS_OK ==
                    mme_gtp_send_modify_bearer_request(bearer, uli_presence));
            }
        }
    }

    if (mme_ue->nas_eps.type != MME_EPS_TYPE_ATTACH_REQUEST)
        mme_send_after_paging(mme_ue, OGS_GTP2_CAUSE_REQUEST_ACCEPTED);

    if (SMS_SERVICE_INDICATOR(mme_ue)) {
        ogs_assert(OGS_OK ==
            sgsap_send_service_request(mme_ue, SGSAP_EMM_CONNECTED_MODE));
    }

    CLEAR_SERVICE_INDICATOR(mme_ue);
}

void s1ap_handle_initial_context_setup_failure(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    S1AP_InitialContextSetupFailure_t *InitialContextSetupFailure = NULL;

    S1AP_InitialContextSetupFailureIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_Cause_t *Cause = NULL;

    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    unsuccessfulOutcome = message->choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    InitialContextSetupFailure =
        &unsuccessfulOutcome->value.choice.InitialContextSetupFailure;
    ogs_assert(InitialContextSetupFailure);

    ogs_debug("InitialContextSetupFailure");

    for (i = 0; i < InitialContextSetupFailure->protocolIEs.list.count; i++) {
        ie = InitialContextSetupFailure->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!enb_ue) {
        ogs_error("No eNB UE Context : MME_UE_S1AP_ID[%lld]",
                (long long)*MME_UE_S1AP_ID);
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id));
        return;
    }

    if (!ENB_UE_S1AP_ID) {
        ogs_error("No ENB_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    if (!Cause) {
        ogs_error("No Cause");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    ogs_debug("    Cause[Group:%d Cause:%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

    mme_ue = enb_ue->mme_ue;

    if (mme_ue) {
        /*
         * if T3450 is running, Attach complete will be sent.
         * So, we need to clear all the timer at this point.
         */
        CLEAR_SERVICE_INDICATOR(mme_ue);
        CLEAR_MME_UE_ALL_TIMERS(mme_ue);
    }

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
    mme_send_release_access_bearer_or_ue_context_release(enb_ue);
}

void s1ap_handle_ue_context_modification_response(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_UEContextModificationResponse_t *UEContextModificationResponse = NULL;

    S1AP_UEContextModificationResponseIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;

    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    UEContextModificationResponse =
        &successfulOutcome->value.choice.UEContextModificationResponse;
    ogs_assert(UEContextModificationResponse);

    ogs_debug("UEContextModificationResponse");

    for (i = 0;
            i < UEContextModificationResponse->protocolIEs.list.count; i++) {
        ie = UEContextModificationResponse->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!enb_ue) {
        ogs_error("No eNB UE Context : MME_UE_S1AP_ID[%lld]",
                (long long)*MME_UE_S1AP_ID);
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id));
        return;
    }

    if (!ENB_UE_S1AP_ID) {
        ogs_error("No ENB_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    mme_ue = enb_ue->mme_ue;
    ogs_expect_or_return(mme_ue);

    CLEAR_SERVICE_INDICATOR(mme_ue);
}

void s1ap_handle_ue_context_modification_failure(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    S1AP_UEContextModificationFailure_t *UEContextModificationFailure = NULL;

    S1AP_UEContextModificationFailureIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_Cause_t *Cause = NULL;

    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    unsuccessfulOutcome = message->choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    UEContextModificationFailure =
        &unsuccessfulOutcome->value.choice.UEContextModificationFailure;
    ogs_assert(UEContextModificationFailure);

    ogs_warn("UEContextModificationFailure");

    for (i = 0; i < UEContextModificationFailure->protocolIEs.list.count; i++) {
        ie = UEContextModificationFailure->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!enb_ue) {
        ogs_error("No eNB UE Context : MME_UE_S1AP_ID[%lld]",
                (long long)*MME_UE_S1AP_ID);
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id));
        return;
    }

    if (!ENB_UE_S1AP_ID) {
        ogs_error("No ENB_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    if (!Cause) {
        ogs_error("No Cause");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    ogs_debug("    Cause[Group:%d Cause:%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

    mme_ue = enb_ue->mme_ue;
    ogs_expect_or_return(mme_ue);
    CLEAR_SERVICE_INDICATOR(mme_ue);
}


void s1ap_handle_e_rab_setup_response(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_E_RABSetupResponse_t *E_RABSetupResponse = NULL;

    S1AP_E_RABSetupResponseIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABSetupListBearerSURes_t *E_RABSetupListBearerSURes = NULL;
    S1AP_E_RABList_t *E_RABFailedToSetupListBearerSURes = NULL;
    S1AP_CriticalityDiagnostics_t *CriticalityDiagnostics = NULL;

    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    E_RABSetupResponse = &successfulOutcome->value.choice.E_RABSetupResponse;
    ogs_assert(E_RABSetupResponse);

    ogs_debug("E-RABSetupResponse");

    for (i = 0; i < E_RABSetupResponse->protocolIEs.list.count; i++) {
        ie = E_RABSetupResponse->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_E_RABSetupListBearerSURes:
            E_RABSetupListBearerSURes =
                &ie->value.choice.E_RABSetupListBearerSURes;
            break;
        case S1AP_ProtocolIE_ID_id_E_RABFailedToSetupListBearerSURes:
            E_RABFailedToSetupListBearerSURes =
                &ie->value.choice.E_RABList;
            break;
        case S1AP_ProtocolIE_ID_id_CriticalityDiagnostics:
            CriticalityDiagnostics =
                &ie->value.choice.CriticalityDiagnostics;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!enb_ue) {
        ogs_error("No eNB UE Context : MME_UE_S1AP_ID[%lld]",
                (long long)*MME_UE_S1AP_ID);
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id));
        return;
    }

    if (!ENB_UE_S1AP_ID) {
        ogs_error("No ENB_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
        enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    mme_ue = enb_ue->mme_ue;
    ogs_expect_or_return(mme_ue);

    if (E_RABSetupListBearerSURes) {
        for (i = 0; i < E_RABSetupListBearerSURes->list.count; i++) {
            S1AP_E_RABSetupItemBearerSUResIEs_t *item = NULL;
            S1AP_E_RABSetupItemBearerSURes_t *e_rab = NULL;

            mme_bearer_t *bearer = NULL;

            item = (S1AP_E_RABSetupItemBearerSUResIEs_t *)
                E_RABSetupListBearerSURes->list.array[i];
            if (!item) {
                ogs_error("No S1AP_E_RABSetupItemBearerSUResIEs_t");
                ogs_assert(OGS_OK ==
                    s1ap_send_error_indication2(mme_ue,
                        S1AP_Cause_PR_protocol,
                        S1AP_CauseProtocol_semantic_error));
                return;
            }

            e_rab = &item->value.choice.E_RABSetupItemBearerSURes;
            if (!e_rab) {
                ogs_error("No E_RABSetupItemBearerSURes");
                ogs_assert(OGS_OK ==
                    s1ap_send_error_indication2(mme_ue,
                        S1AP_Cause_PR_protocol,
                        S1AP_CauseProtocol_semantic_error));
                return;
            }

            bearer = mme_bearer_find_by_ue_ebi(mme_ue, e_rab->e_RAB_ID);
            if (!bearer) {
                ogs_error("No Bearer [%d]", (int)e_rab->e_RAB_ID);
                ogs_assert(OGS_OK ==
                    s1ap_send_error_indication2(mme_ue,
                        S1AP_Cause_PR_radioNetwork,
                        S1AP_CauseRadioNetwork_unknown_E_RAB_ID));
                return;
            }

            memcpy(&bearer->enb_s1u_teid, e_rab->gTP_TEID.buf,
                sizeof(bearer->enb_s1u_teid));
            bearer->enb_s1u_teid = be32toh(bearer->enb_s1u_teid);
            rv = ogs_asn_BIT_STRING_to_ip(
                    &e_rab->transportLayerAddress, &bearer->enb_s1u_ip);
            if (rv != OGS_OK) {
                ogs_error("No transportLayerAddress [%d]",
                        (int)e_rab->e_RAB_ID);
                ogs_assert(OGS_OK ==
                    s1ap_send_error_indication2(mme_ue,
                        S1AP_Cause_PR_protocol,
                        S1AP_CauseProtocol_abstract_syntax_error_falsely_constructed_message));
                return;
            }

            ogs_debug("    EBI[%d]", bearer->ebi);

            if (OGS_FSM_CHECK(&bearer->sm, esm_state_active)) {
                mme_bearer_t *linked_bearer = mme_linked_bearer(bearer);
                ogs_assert(linked_bearer);
                ogs_debug("    Linked-EBI[%d]", linked_bearer->ebi);

                if (bearer->ebi == linked_bearer->ebi) {
                ogs_assert(OGS_OK ==
                    mme_gtp_send_modify_bearer_request(bearer, 0));
                } else {
                ogs_assert(OGS_OK ==
                    mme_gtp_send_create_bearer_response(
                        bearer, OGS_GTP2_CAUSE_REQUEST_ACCEPTED));
                }
            }
        }
    }

    if (E_RABFailedToSetupListBearerSURes) {
        ogs_debug("E_RABFailedToSetupListBearerSURes");
        for (i = 0; i < E_RABFailedToSetupListBearerSURes->list.count; i++) {
            S1AP_E_RABItem_t *item = (S1AP_E_RABItem_t *)
                E_RABFailedToSetupListBearerSURes->list.array[i];

            if (!item) {
                ogs_error("No S1AP_E_RABItem_t");
                ogs_assert(OGS_OK ==
                    s1ap_send_error_indication2(mme_ue,
                        S1AP_Cause_PR_protocol,
                        S1AP_CauseProtocol_semantic_error));
                return;
            }

            ogs_debug("RAB_ID: %d", (int)item->e_RAB_ID);
            ogs_debug("    Cause[Group:%d Cause:%d]", 
                (int)item->cause.present, (int)item->cause.choice.radioNetwork);
        }
    }

    if (CriticalityDiagnostics) {
        ogs_debug("CriticalityDiagnostics");
        S1AP_ProcedureCode_t *procedureCode =
            CriticalityDiagnostics->procedureCode;
        S1AP_TriggeringMessage_t *triggeringMessage =
            CriticalityDiagnostics->triggeringMessage;
        S1AP_Criticality_t *procedureCriticality =
            CriticalityDiagnostics->procedureCriticality;
        if (procedureCode) {
            ogs_debug("procedureCode: %lld",
                    (long long)procedureCode);
        }
        if (triggeringMessage) {
            ogs_debug("triggeringMessage: %lld",
                    (long long)triggeringMessage);
        }
        if (procedureCriticality) {
            ogs_debug("procedureCriticality: %lld",
                    (long long)procedureCriticality);
        }
    }
}

void s1ap_handle_ue_context_release_request(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_UEContextReleaseRequest_t *UEContextReleaseRequest = NULL;

    S1AP_UEContextReleaseRequest_IEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_Cause_t *Cause = NULL;

    enb_ue_t *enb_ue = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    UEContextReleaseRequest =
        &initiatingMessage->value.choice.UEContextReleaseRequest;
    ogs_assert(UEContextReleaseRequest);

    ogs_debug("UEContextReleaseRequest");

    for (i = 0; i < UEContextReleaseRequest->protocolIEs.list.count; i++) {
        ie = UEContextReleaseRequest->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }
    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!enb_ue) {
        ogs_warn("No ENB UE Context : MME_UE_S1AP_ID[%d]",
                (int)*MME_UE_S1AP_ID);
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb,
                MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id));
        return;
    }

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    if (!Cause) {
        ogs_error("No Cause");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    ogs_debug("    Cause[Group:%d Cause:%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

    switch (Cause->present) {
    case S1AP_Cause_PR_radioNetwork:
    case S1AP_Cause_PR_transport:
    case S1AP_Cause_PR_protocol:
    case S1AP_Cause_PR_misc:
        break;
    case S1AP_Cause_PR_nas:
        ogs_warn("NAS-Cause[%d]", (int)Cause->choice.nas);
        break;
    default:
        ogs_warn("Invalid cause group[%d]", Cause->present);
        break;
    }

    mme_send_release_access_bearer_or_ue_context_release(enb_ue);
}

void s1ap_handle_ue_context_release_complete(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_UEContextReleaseComplete_t *UEContextReleaseComplete = NULL;

    S1AP_UEContextReleaseComplete_IEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;

    enb_ue_t *enb_ue = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    UEContextReleaseComplete =
        &successfulOutcome->value.choice.UEContextReleaseComplete;
    ogs_assert(UEContextReleaseComplete);

    ogs_debug("UEContextReleaseComplete");

    for (i = 0; i < UEContextReleaseComplete->protocolIEs.list.count; i++) {
        ie = UEContextReleaseComplete->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }
    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!enb_ue) {
        ogs_warn("No ENB UE Context : MME_UE_S1AP_ID[%d]",
                (int)*MME_UE_S1AP_ID);
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb,
                MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id));
        return;
    }

    s1ap_handle_ue_context_release_action(enb_ue);
}

void s1ap_handle_ue_context_release_action(enb_ue_t *enb_ue)
{
    mme_ue_t *mme_ue = NULL;

    ogs_assert(enb_ue);

    if (enb_ue_cycle(enb_ue) == NULL) {
        ogs_error("S1 context has already been removed");
        return;
    }

    mme_ue = enb_ue->mme_ue;

    ogs_info("UE Context Release [Action:%d]", enb_ue->ue_ctx_rel_action);
    ogs_info("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);
    if (mme_ue) {
        ogs_info("    IMSI[%s]", mme_ue->imsi_bcd);

        /*
         * An assert occurs when a NAS message retransmission occurs.
         *
         * Because there is no `enb_ue` context.
         * 
         * Therefore, before removing enb_ue, all Timers must be stopped
         * to prevent retransmission of NAS messages.
         */
        CLEAR_MME_UE_ALL_TIMERS(mme_ue);
    }

    switch (enb_ue->ue_ctx_rel_action) {
    case S1AP_UE_CTX_REL_S1_CONTEXT_REMOVE:
        ogs_debug("    Action: S1 context remove");
        enb_ue_remove(enb_ue);
        break;
    case S1AP_UE_CTX_REL_S1_REMOVE_AND_UNLINK:
        ogs_debug("    Action: S1 normal release");
        enb_ue_remove(enb_ue);
        ogs_expect_or_return(mme_ue);
        mme_ue_deassociate(mme_ue);
        break;
    case S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE:
        ogs_debug("    Action: UE context remove");
        enb_ue_remove(enb_ue);
        ogs_expect_or_return(mme_ue);

        mme_ue_hash_remove(mme_ue);
        mme_ue_remove(mme_ue);
        break;
    case S1AP_UE_CTX_REL_S1_HANDOVER_COMPLETE:
        ogs_debug("    Action: S1 handover complete");

        source_ue_deassociate_target_ue(enb_ue);
        enb_ue_remove(enb_ue);

        ogs_expect_or_return(mme_ue);
        if (mme_ue_have_indirect_tunnel(mme_ue) == true) {
            ogs_assert(OGS_OK ==
                mme_gtp_send_delete_indirect_data_forwarding_tunnel_request(
                    mme_ue, OGS_GTP_DELETE_INDIRECT_HANDOVER_COMPLETE));
        } else {
            ogs_warn("Check your eNodeB");
            ogs_warn("  No INDIRECT TUNNEL");
            ogs_warn("  Packet could be dropped during S1-Handover");
            mme_ue_clear_indirect_tunnel(mme_ue);
        }
        break;
    case S1AP_UE_CTX_REL_S1_HANDOVER_CANCEL:
        ogs_warn("    Action: S1 handover cancel");

        source_ue_deassociate_target_ue(enb_ue);
        enb_ue_remove(enb_ue);

        ogs_expect_or_return(mme_ue);
        if (mme_ue_have_indirect_tunnel(mme_ue) == true) {
            ogs_assert(OGS_OK ==
                mme_gtp_send_delete_indirect_data_forwarding_tunnel_request(
                    mme_ue, OGS_GTP_DELETE_INDIRECT_HANDOVER_CANCEL));
        } else {
            ogs_warn("Check your eNodeB");
            ogs_warn("  No INDIRECT TUNNEL");
            ogs_warn("  Packet could be dropped during S1-Handover");
            mme_ue_clear_indirect_tunnel(mme_ue);

            ogs_expect_or_return(mme_ue->enb_ue);
            ogs_assert(OGS_OK ==
                s1ap_send_handover_cancel_ack(mme_ue->enb_ue));
        }
        break;
    case S1AP_UE_CTX_REL_S1_HANDOVER_FAILURE:
        ogs_warn("    Action: S1 handover failure");

        source_ue_deassociate_target_ue(enb_ue);
        enb_ue_remove(enb_ue);

        ogs_expect_or_return(mme_ue);
        if (mme_ue_have_indirect_tunnel(mme_ue) == true) {
            ogs_error("Check your eNodeB");
            ogs_error("  We found INDIRECT TUNNEL in HandoverFailure");
            mme_ue_clear_indirect_tunnel(mme_ue);
        }
        break;
    case S1AP_UE_CTX_REL_S1_PAGING:
        ogs_debug("    Action: S1 paging");
        enb_ue_remove(enb_ue);
        ogs_expect_or_return(mme_ue);
        mme_ue_deassociate(mme_ue);

        ogs_assert(OGS_OK == s1ap_send_paging(mme_ue, S1AP_CNDomain_ps));
        break;
    default:
        ogs_error("Invalid Action[%d]", enb_ue->ue_ctx_rel_action);
        break;
    }
}

void s1ap_handle_e_rab_modification_indication(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int rv, i;

    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_E_RABModificationIndication_t *E_RABModificationIndication = NULL;

    S1AP_E_RABModificationIndicationIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABToBeModifiedListBearerModInd_t
        *E_RABToBeModifiedListBearerModInd = NULL;

    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    E_RABModificationIndication = &initiatingMessage->value.choice.E_RABModificationIndication;
    ogs_assert(E_RABModificationIndication);

    ogs_info("E_RABModificationIndication");

    for (i = 0; i < E_RABModificationIndication->protocolIEs.list.count; i++) {
        ie = E_RABModificationIndication->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_E_RABToBeModifiedListBearerModInd:
            E_RABToBeModifiedListBearerModInd =
                &ie->value.choice.E_RABToBeModifiedListBearerModInd;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }
    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!enb_ue) {
        ogs_warn("No ENB UE Context : MME_UE_S1AP_ID[%d]",
                (int)*MME_UE_S1AP_ID);
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb,
                MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id));
        return;
    }

    if (!ENB_UE_S1AP_ID) {
        ogs_error("No ENB_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    if (!E_RABToBeModifiedListBearerModInd) {
        ogs_error("No E_RABToBeModifiedListBearerModInd");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    mme_ue = enb_ue->mme_ue;
    ogs_expect_or_return(mme_ue);

    for (i = 0; i < E_RABToBeModifiedListBearerModInd->list.count; i++) {
        S1AP_E_RABToBeModifiedItemBearerModIndIEs_t *item = NULL;
        S1AP_E_RABToBeModifiedItemBearerModInd_t *e_rab = NULL;

        mme_bearer_t *bearer = NULL;

        item = (S1AP_E_RABToBeModifiedItemBearerModIndIEs_t *)
                E_RABToBeModifiedListBearerModInd->list.array[i];
        if (!item) {
            ogs_error("No S1AP_E_RABToBeModifiedItemBearerModIndIEs_t");
            ogs_assert(OGS_OK ==
                s1ap_send_error_indication2(mme_ue,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
            return;
        }

        e_rab = &item->value.choice.E_RABToBeModifiedItemBearerModInd;
        if (!e_rab) {
            ogs_error("No E_RABToBeModifiedItemBearerModInd");
            ogs_assert(OGS_OK ==
                s1ap_send_error_indication2(mme_ue,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
            return;
        }

        bearer = mme_bearer_find_by_ue_ebi(mme_ue, e_rab->e_RAB_ID);
        if (!bearer) {
            ogs_error("No Bearer [%d]", (int)e_rab->e_RAB_ID);
            ogs_assert(OGS_OK ==
                s1ap_send_error_indication2(mme_ue,
                    S1AP_Cause_PR_radioNetwork,
                    S1AP_CauseRadioNetwork_unknown_E_RAB_ID));
            return;
        }

        memcpy(&bearer->enb_s1u_teid, e_rab->dL_GTP_TEID.buf,
                sizeof(bearer->enb_s1u_teid));
        bearer->enb_s1u_teid = be32toh(bearer->enb_s1u_teid);
        rv = ogs_asn_BIT_STRING_to_ip(
                &e_rab->transportLayerAddress, &bearer->enb_s1u_ip);
        if (rv != OGS_OK) {
            ogs_error("No transportLayerAddress [%d]",
                    (int)e_rab->e_RAB_ID);
            ogs_assert(OGS_OK ==
                s1ap_send_error_indication2(mme_ue,
                    S1AP_Cause_PR_protocol,
                    S1AP_CauseProtocol_abstract_syntax_error_falsely_constructed_message));
            return;
        }

        GTP_COUNTER_INCREMENT(
                mme_ue, GTP_COUNTER_MODIFY_BEARER_BY_E_RAB_MODIFICATION);

        ogs_assert(OGS_OK ==
            mme_gtp_send_modify_bearer_request(bearer, 0));
    }
}

void s1ap_handle_path_switch_request(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_PathSwitchRequest_t *PathSwitchRequest = NULL;

    S1AP_PathSwitchRequestIEs_t *ie = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABToBeSwitchedDLList_t *E_RABToBeSwitchedDLList = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_EUTRAN_CGI_t *EUTRAN_CGI = NULL;
    S1AP_TAI_t *TAI = NULL;
    S1AP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;

    S1AP_PLMNidentity_t *pLMNidentity = NULL;
    S1AP_CellIdentity_t *cell_ID = NULL;
    S1AP_TAC_t *tAC = NULL;
    S1AP_EncryptionAlgorithms_t    *encryptionAlgorithms = NULL;
    S1AP_IntegrityProtectionAlgorithms_t *integrityProtectionAlgorithms = NULL;
    uint16_t eea = 0, eia = 0;
    uint8_t eea0 = 0, eia0 = 0;

    enb_ue_t *enb_ue = NULL;
    mme_ue_t *mme_ue = NULL;
    ogs_pkbuf_t *s1apbuf = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    PathSwitchRequest = &initiatingMessage->value.choice.PathSwitchRequest;
    ogs_assert(PathSwitchRequest);

    ogs_info("PathSwitchRequest");

    for (i = 0; i < PathSwitchRequest->protocolIEs.list.count; i++) {
        ie = PathSwitchRequest->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_E_RABToBeSwitchedDLList:
            E_RABToBeSwitchedDLList =
                &ie->value.choice.E_RABToBeSwitchedDLList;
            break;
        case S1AP_ProtocolIE_ID_id_SourceMME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_EUTRAN_CGI:
            EUTRAN_CGI = &ie->value.choice.EUTRAN_CGI;
            break;
        case S1AP_ProtocolIE_ID_id_TAI:
            TAI = &ie->value.choice.TAI;
            break;
        case S1AP_ProtocolIE_ID_id_UESecurityCapabilities:
            UESecurityCapabilities = &ie->value.choice.UESecurityCapabilities;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!ENB_UE_S1AP_ID) {
        ogs_error("No ENB_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }
    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }
    enb_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!enb_ue) {
        ogs_error("Cannot find UE from sourceMME-UE-S1AP-ID[%d] and eNB[%s:%d]",
                (int)*MME_UE_S1AP_ID,
                OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

        s1apbuf = s1ap_build_path_switch_failure(
                *ENB_UE_S1AP_ID, *MME_UE_S1AP_ID,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id);
        ogs_expect_or_return(s1apbuf);

        ogs_expect(OGS_OK ==
                s1ap_send_to_enb(enb, s1apbuf, S1AP_NON_UE_SIGNALLING));
        return;
    }

    ogs_info("    OLD ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    if (!EUTRAN_CGI) {
        ogs_error("No EUTRAN_CGI");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    pLMNidentity = &EUTRAN_CGI->pLMNidentity;
    ogs_assert(pLMNidentity && pLMNidentity->size == sizeof(ogs_plmn_id_t));
    cell_ID = &EUTRAN_CGI->cell_ID;
    ogs_assert(cell_ID);

    if (!TAI) {
        ogs_error("No TAI");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    pLMNidentity = &TAI->pLMNidentity;
    ogs_assert(pLMNidentity && pLMNidentity->size == sizeof(ogs_plmn_id_t));
    tAC = &TAI->tAC;
    ogs_assert(tAC && tAC->size == sizeof(uint16_t));

    if (!E_RABToBeSwitchedDLList) {
        ogs_error("No E_RABToBeSwitchedDLList");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    mme_ue = enb_ue->mme_ue;
    ogs_expect_or_return(mme_ue);

    ogs_info("    OLD TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&mme_ue->tai.plmn_id),
            mme_ue->tai.tac);
    ogs_info("    OLD E_CGI[PLMN_ID:%06x,CELL_ID:0x%x]",
            ogs_plmn_id_hexdump(&mme_ue->e_cgi.plmn_id),
            mme_ue->e_cgi.cell_id);

    /* Update ENB-UE-S1AP-ID */
    enb_ue->enb_ue_s1ap_id = *ENB_UE_S1AP_ID;

    /* Change enb_ue to the NEW eNB */
    enb_ue_switch_to_enb(enb_ue, enb);

    ogs_info("    NEW ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    memcpy(&enb_ue->saved.tai.plmn_id, pLMNidentity->buf, 
            sizeof(enb_ue->saved.tai.plmn_id));
    memcpy(&enb_ue->saved.tai.tac, tAC->buf, sizeof(enb_ue->saved.tai.tac));
    enb_ue->saved.tai.tac = be16toh(enb_ue->saved.tai.tac);

    memcpy(&enb_ue->saved.e_cgi.plmn_id, pLMNidentity->buf, 
            sizeof(enb_ue->saved.e_cgi.plmn_id));
    memcpy(&enb_ue->saved.e_cgi.cell_id, cell_ID->buf,
            sizeof(enb_ue->saved.e_cgi.cell_id));
    enb_ue->saved.e_cgi.cell_id = (be32toh(enb_ue->saved.e_cgi.cell_id) >> 4);

    ogs_info("    TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&enb_ue->saved.tai.plmn_id),
            enb_ue->saved.tai.tac);
    ogs_info("    E_CGI[PLMN_ID:%06x,CELL_ID:0x%x]",
            ogs_plmn_id_hexdump(&enb_ue->saved.e_cgi.plmn_id),
            enb_ue->saved.e_cgi.cell_id);

    /* Copy Stream-No/TAI/ECGI from enb_ue */
    mme_ue->enb_ostream_id = enb_ue->enb_ostream_id;
    memcpy(&mme_ue->tai, &enb_ue->saved.tai, sizeof(ogs_eps_tai_t));
    memcpy(&mme_ue->e_cgi, &enb_ue->saved.e_cgi, sizeof(ogs_e_cgi_t));

    ogs_assert(UESecurityCapabilities);
    encryptionAlgorithms =
        &UESecurityCapabilities->encryptionAlgorithms;
    integrityProtectionAlgorithms =
        &UESecurityCapabilities->integrityProtectionAlgorithms;

    memcpy(&eea, encryptionAlgorithms->buf, sizeof(eea));
    eea = be16toh(eea);
    eea0 = mme_ue->ue_network_capability.eea0;
    mme_ue->ue_network_capability.eea = eea >> 9;
    mme_ue->ue_network_capability.eea0 = eea0;

    memcpy(&eia, integrityProtectionAlgorithms->buf, sizeof(eia));
    eia = be16toh(eia);
    eia0 = mme_ue->ue_network_capability.eia0;
    mme_ue->ue_network_capability.eia = eia >> 9;
    mme_ue->ue_network_capability.eia0 = eia0;

    if (!SECURITY_CONTEXT_IS_VALID(mme_ue)) {
        ogs_error("No Security Context");
        s1apbuf = s1ap_build_path_switch_failure(
                *ENB_UE_S1AP_ID, *MME_UE_S1AP_ID,
                S1AP_Cause_PR_nas, S1AP_CauseNas_authentication_failure);
        ogs_expect_or_return(s1apbuf);

        s1ap_send_to_enb_ue(enb_ue, s1apbuf);
        return;
    }

    /* Update Security Context (NextHop) */
    mme_ue->nhcc++;
    ogs_kdf_nh_enb(mme_ue->kasme, mme_ue->nh, mme_ue->nh);

    for (i = 0; i < E_RABToBeSwitchedDLList->list.count; i++) {
        S1AP_E_RABToBeSwitchedDLItemIEs_t *item = NULL;
        S1AP_E_RABToBeSwitchedDLItem_t *e_rab = NULL;

        mme_bearer_t *bearer = NULL;

        item = (S1AP_E_RABToBeSwitchedDLItemIEs_t *)
            E_RABToBeSwitchedDLList->list.array[i];
        if (!item) {
            ogs_error("No S1AP_E_RABToBeSwitchedDLItemIEs_t");
            ogs_assert(OGS_OK ==
                s1ap_send_error_indication2(mme_ue,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
            return;
        }

        e_rab = &item->value.choice.E_RABToBeSwitchedDLItem;
        if (!e_rab) {
            ogs_error("No E_RABToBeSwitchedDLItem");
            ogs_assert(OGS_OK ==
                s1ap_send_error_indication2(mme_ue,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
            return;
        }

        bearer = mme_bearer_find_by_ue_ebi(mme_ue, e_rab->e_RAB_ID);
        if (!bearer) {
            ogs_error("No Bearer [%d]", (int)e_rab->e_RAB_ID);
            ogs_assert(OGS_OK ==
                s1ap_send_error_indication2(mme_ue,
                    S1AP_Cause_PR_radioNetwork,
                    S1AP_CauseRadioNetwork_unknown_E_RAB_ID));
            return;
        }

        memcpy(&bearer->enb_s1u_teid, e_rab->gTP_TEID.buf, 
                sizeof(bearer->enb_s1u_teid));
        bearer->enb_s1u_teid = be32toh(bearer->enb_s1u_teid);
        rv = ogs_asn_BIT_STRING_to_ip(
                &e_rab->transportLayerAddress, &bearer->enb_s1u_ip);
        if (rv != OGS_OK) {
            ogs_error("No transportLayerAddress [%d]",
                    (int)e_rab->e_RAB_ID);
            ogs_assert(OGS_OK ==
                s1ap_send_error_indication2(mme_ue,
                    S1AP_Cause_PR_protocol,
                    S1AP_CauseProtocol_abstract_syntax_error_falsely_constructed_message));
            return;
        }

        GTP_COUNTER_INCREMENT(
                mme_ue, GTP_COUNTER_MODIFY_BEARER_BY_PATH_SWITCH);

        ogs_assert(OGS_OK ==
            mme_gtp_send_modify_bearer_request(bearer, 1));
    }
}

void s1ap_handle_enb_configuration_transfer(
        mme_enb_t *enb, ogs_s1ap_message_t *message, ogs_pkbuf_t *pkbuf)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_ENBConfigurationTransfer_t *ENBConfigurationTransfer = NULL;

    S1AP_ENBConfigurationTransferIEs_t *ie = NULL;
    S1AP_SONConfigurationTransfer_t *SONConfigurationTransfer = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    ENBConfigurationTransfer =
        &initiatingMessage->value.choice.ENBConfigurationTransfer;
    ogs_assert(ENBConfigurationTransfer);

    ogs_debug("ENBConfigurationTransfer");
    for (i = 0; i < ENBConfigurationTransfer->protocolIEs.list.count; i++) {
        ie = ENBConfigurationTransfer->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_SONConfigurationTransferECT:
            SONConfigurationTransfer =
                &ie->value.choice.SONConfigurationTransfer;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (SONConfigurationTransfer) {
        S1AP_TargeteNB_ID_t *targeteNB_ID =
            &SONConfigurationTransfer->targeteNB_ID;
        S1AP_SourceeNB_ID_t *sourceeNB_ID =
            &SONConfigurationTransfer->sourceeNB_ID;

        mme_enb_t *target_enb = NULL;
        uint32_t source_enb_id, target_enb_id;
        uint16_t source_tac, target_tac;

        ogs_s1ap_ENB_ID_to_uint32(
                &sourceeNB_ID->global_ENB_ID.eNB_ID, &source_enb_id);
        ogs_s1ap_ENB_ID_to_uint32(
                &targeteNB_ID->global_ENB_ID.eNB_ID, &target_enb_id);

        memcpy(&source_tac, sourceeNB_ID->selected_TAI.tAC.buf,
                sizeof(source_tac));
        source_tac = be16toh(source_tac);
        memcpy(&target_tac, targeteNB_ID->selected_TAI.tAC.buf,
                sizeof(target_tac));
        target_tac = be16toh(target_tac);

        ogs_debug("    Source : ENB_ID[%s:%d], TAC[%d]",
                sourceeNB_ID->global_ENB_ID.eNB_ID.present ==
                    S1AP_ENB_ID_PR_homeENB_ID ? "Home" :
                sourceeNB_ID->global_ENB_ID.eNB_ID.present ==
                    S1AP_ENB_ID_PR_macroENB_ID ? "Macro" : "Others",
                source_enb_id, source_tac);
        ogs_debug("    Target : ENB_ID[%s:%d], TAC[%d]",
                targeteNB_ID->global_ENB_ID.eNB_ID.present ==
                    S1AP_ENB_ID_PR_homeENB_ID ? "Home" :
                targeteNB_ID->global_ENB_ID.eNB_ID.present ==
                    S1AP_ENB_ID_PR_macroENB_ID ? "Macro" : "Others",
                target_enb_id, target_tac);

        target_enb = mme_enb_find_by_enb_id(target_enb_id);
        if (target_enb == NULL) {
            ogs_error("eNB configuration transfer : "
                        "cannot find target eNB-id[0x%x]", target_enb_id);
            ogs_assert(OGS_OK ==
                s1ap_send_error_indication(enb, NULL, NULL,
                    S1AP_Cause_PR_radioNetwork,
                    S1AP_CauseRadioNetwork_unknown_targetID));
            return;
        }

        ogs_assert(OGS_OK ==
            s1ap_send_mme_configuration_transfer(
                target_enb, SONConfigurationTransfer));
    }
}

void s1ap_handle_handover_required(mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_HandoverRequired_t *HandoverRequired = NULL;

    S1AP_HandoverRequiredIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_HandoverType_t *HandoverType = NULL;
    S1AP_Cause_t *Cause = NULL;
    S1AP_TargetID_t *TargetID = NULL;
    S1AP_Source_ToTarget_TransparentContainer_t
        *Source_ToTarget_TransparentContainer = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    HandoverRequired = &initiatingMessage->value.choice.HandoverRequired;
    ogs_assert(HandoverRequired);

    enb_ue_t *source_ue = NULL;
    mme_ue_t *mme_ue = NULL;
    mme_enb_t *target_enb = NULL;
    uint32_t target_enb_id = 0;

    ogs_debug("HandoverRequired");
    for (i = 0; i < HandoverRequired->protocolIEs.list.count; i++) {
        ie = HandoverRequired->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_HandoverType:
            HandoverType = &ie->value.choice.HandoverType;
            break;
        case S1AP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        case S1AP_ProtocolIE_ID_id_TargetID:
            TargetID = &ie->value.choice.TargetID;
            break;
        case S1AP_ProtocolIE_ID_id_Source_ToTarget_TransparentContainer:
            Source_ToTarget_TransparentContainer =
                &ie->value.choice.Source_ToTarget_TransparentContainer;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    source_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!source_ue) {
        ogs_error("No eNB UE Context : MME_UE_S1AP_ID[%lld]",
                (long long)*MME_UE_S1AP_ID);
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id));
        return;
    }

    if (!ENB_UE_S1AP_ID) {
        ogs_error("No ENB_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    ogs_debug("    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);

    if (!HandoverType) {
        ogs_error("No HandoverType");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    if (!Cause) {
        ogs_error("No Cause");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    if (!TargetID) {
        ogs_error("No TargetID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    if (!Source_ToTarget_TransparentContainer) {
        ogs_error("No Source_ToTarget_TransparentContainer");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    switch (TargetID->present) {
    case S1AP_TargetID_PR_targeteNB_ID:
        ogs_s1ap_ENB_ID_to_uint32(
            &TargetID->choice.targeteNB_ID->global_ENB_ID.eNB_ID,
            &target_enb_id);
        break;
    default:
        ogs_error("Not implemented(%d)", TargetID->present);
        ogs_assert(OGS_OK ==
            s1ap_send_handover_preparation_failure(source_ue,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    target_enb = mme_enb_find_by_enb_id(target_enb_id);
    if (target_enb == NULL) {
        ogs_error("Handover required : cannot find target eNB-id[0x%x]",
                    target_enb_id);
        ogs_assert(OGS_OK ==
            s1ap_send_handover_preparation_failure(source_ue,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_targetID));
        return;
    }

    mme_ue = source_ue->mme_ue;
    ogs_expect_or_return(mme_ue);

    source_ue->handover_type = *HandoverType;

    if (SECURITY_CONTEXT_IS_VALID(mme_ue)) {
        mme_ue->nhcc++;
        ogs_kdf_nh_enb(mme_ue->kasme, mme_ue->nh, mme_ue->nh);
    } else {
        ogs_assert(OGS_OK ==
            s1ap_send_handover_preparation_failure(source_ue,
                S1AP_Cause_PR_nas, S1AP_CauseNas_authentication_failure));
        return;
    }

    ogs_assert(OGS_OK ==
        s1ap_send_handover_request(
            source_ue, target_enb, HandoverType, Cause,
            Source_ToTarget_TransparentContainer));
}

void s1ap_handle_handover_request_ack(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_HandoverRequestAcknowledge_t *HandoverRequestAcknowledge = NULL;

    S1AP_HandoverRequestAcknowledgeIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABAdmittedList_t *E_RABAdmittedList = NULL;
    S1AP_Target_ToSource_TransparentContainer_t
        *Target_ToSource_TransparentContainer = NULL;

    enb_ue_t *source_ue = NULL;
    enb_ue_t *target_ue = NULL;
    mme_ue_t *mme_ue = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    HandoverRequestAcknowledge =
        &successfulOutcome->value.choice.HandoverRequestAcknowledge;
    ogs_assert(HandoverRequestAcknowledge);

    ogs_debug("HandoverRequestAcknowledge");
    for (i = 0; i < HandoverRequestAcknowledge->protocolIEs.list.count; i++) {
        ie = HandoverRequestAcknowledge->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_E_RABAdmittedList:
            E_RABAdmittedList = &ie->value.choice.E_RABAdmittedList;
            break;
        case S1AP_ProtocolIE_ID_id_Target_ToSource_TransparentContainer:
            Target_ToSource_TransparentContainer =
                &ie->value.choice.Target_ToSource_TransparentContainer;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    target_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!target_ue) {
        ogs_error("No eNB UE Context : MME_UE_S1AP_ID[%lld]",
                (long long)*MME_UE_S1AP_ID);
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id));
        return;
    }

    if (!ENB_UE_S1AP_ID) {
        ogs_error("No ENB_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    if (!E_RABAdmittedList) {
        ogs_error("No E_RABAdmittedList");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    if (!Target_ToSource_TransparentContainer) {
        ogs_error("No Target_ToSource_TransparentContainer");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    source_ue = target_ue->source_ue;
    if (!source_ue) {
        ogs_error("No Source UE");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    mme_ue = source_ue->mme_ue;
    ogs_expect_or_return(mme_ue);

    ogs_debug("    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);
    ogs_debug("    Target : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            target_ue->enb_ue_s1ap_id, target_ue->mme_ue_s1ap_id);

    target_ue->enb_ue_s1ap_id = *ENB_UE_S1AP_ID;

    for (i = 0; i < E_RABAdmittedList->list.count; i++) {
        S1AP_E_RABAdmittedItemIEs_t *item = NULL;
        S1AP_E_RABAdmittedItem_t *e_rab = NULL;

        mme_bearer_t *bearer = NULL;

        item = (S1AP_E_RABAdmittedItemIEs_t *)E_RABAdmittedList->list.array[i];
        if (!item) {
            ogs_error("No S1AP_E_RABAdmittedItemIEs_t");
            ogs_assert(OGS_OK ==
                s1ap_send_error_indication2(mme_ue,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
            return;
        }

        e_rab = &item->value.choice.E_RABAdmittedItem;
        if (!e_rab) {
            ogs_error("No E_RABAdmittedItem");
            ogs_assert(OGS_OK ==
                s1ap_send_error_indication2(mme_ue,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
            return;
        }

        bearer = mme_bearer_find_by_ue_ebi(mme_ue, e_rab->e_RAB_ID);
        if (!bearer) {
            ogs_error("No Bearer [%d]", (int)e_rab->e_RAB_ID);
            ogs_assert(OGS_OK ==
                s1ap_send_error_indication2(mme_ue,
                    S1AP_Cause_PR_radioNetwork,
                    S1AP_CauseRadioNetwork_unknown_E_RAB_ID));
            return;
        }

        memcpy(&bearer->target_s1u_teid, e_rab->gTP_TEID.buf, 
                sizeof(bearer->target_s1u_teid));
        bearer->target_s1u_teid = be32toh(bearer->target_s1u_teid);
        rv = ogs_asn_BIT_STRING_to_ip(
                &e_rab->transportLayerAddress, &bearer->target_s1u_ip);
        if (rv != OGS_OK) {
            ogs_error("No transportLayerAddress [%d]",
                    (int)e_rab->e_RAB_ID);
            ogs_assert(OGS_OK ==
                s1ap_send_error_indication2(mme_ue,
                    S1AP_Cause_PR_protocol,
                    S1AP_CauseProtocol_abstract_syntax_error_falsely_constructed_message));
            return;
        }

        if (e_rab->dL_transportLayerAddress && e_rab->dL_gTP_TEID) {
            ogs_assert(e_rab->dL_gTP_TEID->buf);
            ogs_assert(e_rab->dL_transportLayerAddress->buf);
            memcpy(&bearer->enb_dl_teid, e_rab->dL_gTP_TEID->buf, 
                    sizeof(bearer->enb_dl_teid));
            bearer->enb_dl_teid = be32toh(bearer->enb_dl_teid);
            rv = ogs_asn_BIT_STRING_to_ip(
                    e_rab->dL_transportLayerAddress, &bearer->enb_dl_ip);
            if (rv != OGS_OK) {
                ogs_error("No dL_transportLayerAddress [%d]",
                        (int)e_rab->e_RAB_ID);
                ogs_assert(OGS_OK ==
                    s1ap_send_error_indication2(mme_ue,
                        S1AP_Cause_PR_protocol,
                        S1AP_CauseProtocol_abstract_syntax_error_falsely_constructed_message));
                return;
            }
        }

        if (e_rab->uL_TransportLayerAddress && e_rab->uL_GTP_TEID) {
            ogs_assert(e_rab->uL_GTP_TEID->buf);
            ogs_assert(e_rab->uL_TransportLayerAddress->buf);
            memcpy(&bearer->enb_ul_teid, e_rab->uL_GTP_TEID->buf, 
                    sizeof(bearer->enb_ul_teid));
            bearer->enb_ul_teid = be32toh(bearer->enb_ul_teid);
            rv = ogs_asn_BIT_STRING_to_ip(
                    e_rab->uL_TransportLayerAddress, &bearer->enb_ul_ip);
            if (rv != OGS_OK) {
                ogs_error("No uL_transportLayerAddress [%d]",
                        (int)e_rab->e_RAB_ID);
                ogs_assert(OGS_OK ==
                    s1ap_send_error_indication2(mme_ue,
                        S1AP_Cause_PR_protocol,
                        S1AP_CauseProtocol_abstract_syntax_error_falsely_constructed_message));
                return;
            }
        }
    }

    OGS_ASN_STORE_DATA(&mme_ue->container,
            Target_ToSource_TransparentContainer);

    if (mme_ue_have_indirect_tunnel(mme_ue) == true) {
        ogs_assert(OGS_OK ==
            mme_gtp_send_create_indirect_data_forwarding_tunnel_request(mme_ue)\
        );
    } else {
        ogs_assert(OGS_OK ==
            s1ap_send_handover_command(source_ue));
    }
}

void s1ap_handle_handover_failure(mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    S1AP_HandoverFailure_t *HandoverFailure = NULL;

    S1AP_HandoverFailureIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_Cause_t *Cause = NULL;

    enb_ue_t *target_ue = NULL;
    enb_ue_t *source_ue = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    unsuccessfulOutcome = message->choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    HandoverFailure = &unsuccessfulOutcome->value.choice.HandoverFailure;
    ogs_assert(HandoverFailure);

    ogs_debug("HandoverFailure");
    for (i = 0; i < HandoverFailure->protocolIEs.list.count; i++) {
        ie = HandoverFailure->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, NULL,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    target_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!target_ue) {
        ogs_error("No eNB UE Context : MME_UE_S1AP_ID[%lld]",
                (long long)*MME_UE_S1AP_ID);
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id));
        return;
    }

    if (!Cause) {
        ogs_error("No Cause");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    source_ue = target_ue->source_ue;
    if (!source_ue) {
        ogs_error("No Source UE");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    ogs_debug("    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);
    ogs_debug("    Target : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            target_ue->enb_ue_s1ap_id, target_ue->mme_ue_s1ap_id);

    ogs_assert(OGS_OK ==
        s1ap_send_handover_preparation_failure(
            source_ue, Cause->present, Cause->choice.radioNetwork));

    ogs_assert(OGS_OK ==
        s1ap_send_ue_context_release_command(
        target_ue, S1AP_Cause_PR_radioNetwork,
        S1AP_CauseRadioNetwork_ho_failure_in_target_EPC_eNB_or_target_system,
        S1AP_UE_CTX_REL_S1_HANDOVER_FAILURE, 0));
}

void s1ap_handle_handover_cancel(mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_HandoverCancel_t *HandoverCancel = NULL;

    S1AP_HandoverCancelIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_Cause_t *Cause = NULL;

    enb_ue_t *source_ue = NULL;
    enb_ue_t *target_ue = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    HandoverCancel = &initiatingMessage->value.choice.HandoverCancel;
    ogs_assert(HandoverCancel);

    ogs_debug("HandoverCancel");
    for (i = 0; i < HandoverCancel->protocolIEs.list.count; i++) {
        ie = HandoverCancel->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    source_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!source_ue) {
        ogs_error("No eNB UE Context : MME_UE_S1AP_ID[%lld]",
                (long long)*MME_UE_S1AP_ID);
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id));
        return;
    }

    if (!ENB_UE_S1AP_ID) {
        ogs_error("No ENB_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    if (!Cause) {
        ogs_error("No Cause");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    target_ue = source_ue->target_ue;
    if (!target_ue) {
        ogs_error("No Target UE");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    ogs_debug("    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);
    ogs_debug("    Target : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            target_ue->enb_ue_s1ap_id, target_ue->mme_ue_s1ap_id);

    ogs_assert(OGS_OK ==
        s1ap_send_ue_context_release_command(
            target_ue, S1AP_Cause_PR_radioNetwork,
            S1AP_CauseRadioNetwork_handover_cancelled,
            S1AP_UE_CTX_REL_S1_HANDOVER_CANCEL, 0));

    ogs_debug("Handover Cancel : UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]",
            source_ue->enb_ue_s1ap_id,
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);
}

void s1ap_handle_enb_status_transfer(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_ENBStatusTransfer_t *ENBStatusTransfer = NULL;

    S1AP_ENBStatusTransferIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_ENB_StatusTransfer_TransparentContainer_t
        *ENB_StatusTransfer_TransparentContainer = NULL;

    enb_ue_t *source_ue = NULL, *target_ue = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    ENBStatusTransfer = &initiatingMessage->value.choice.ENBStatusTransfer;
    ogs_assert(ENBStatusTransfer);

    ogs_debug("ENBStatusTransfer");
    for (i = 0; i < ENBStatusTransfer->protocolIEs.list.count; i++) {
        ie = ENBStatusTransfer->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_StatusTransfer_TransparentContainer:
            ENB_StatusTransfer_TransparentContainer =
                &ie->value.choice.ENB_StatusTransfer_TransparentContainer;
            break;
        default:
            break;
        }
    }
    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    source_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!source_ue) {
        ogs_error("No eNB UE Context : MME_UE_S1AP_ID[%lld]",
                (long long)*MME_UE_S1AP_ID);
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id));
        return;
    }

    if (!ENB_UE_S1AP_ID) {
        ogs_error("No ENB_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    if (!ENB_StatusTransfer_TransparentContainer) {
        ogs_error("No ENB_StatusTransfer_TransparentContainer");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    target_ue = source_ue->target_ue;
    if (!target_ue) {
        ogs_error("No Target UE");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    ogs_debug("    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);
    ogs_debug("    Target : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            target_ue->enb_ue_s1ap_id, target_ue->mme_ue_s1ap_id);

    ogs_assert(OGS_OK ==
        s1ap_send_mme_status_transfer(target_ue,
            ENB_StatusTransfer_TransparentContainer));
}

void s1ap_handle_handover_notification(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_HandoverNotify_t *HandoverNotify = NULL;

    S1AP_HandoverNotifyIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_EUTRAN_CGI_t *EUTRAN_CGI = NULL;
    S1AP_TAI_t *TAI = NULL;

    S1AP_PLMNidentity_t *pLMNidentity = NULL;
    S1AP_CellIdentity_t *cell_ID = NULL;
    S1AP_TAC_t *tAC = NULL;

    enb_ue_t *source_ue = NULL;
    enb_ue_t *target_ue = NULL;
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    HandoverNotify = &initiatingMessage->value.choice.HandoverNotify;
    ogs_assert(HandoverNotify);

    ogs_debug("HandoverNotify");
    for (i = 0; i < HandoverNotify->protocolIEs.list.count; i++) {
        ie = HandoverNotify->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID:
            MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID:
            ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
            break;
        case S1AP_ProtocolIE_ID_id_EUTRAN_CGI:
            EUTRAN_CGI = &ie->value.choice.EUTRAN_CGI;
            break;
        case S1AP_ProtocolIE_ID_id_TAI:
            TAI = &ie->value.choice.TAI;
            break;
        default:
            break;
        }
    }
    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!MME_UE_S1AP_ID) {
        ogs_error("No MME_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    target_ue = enb_ue_find_by_mme_ue_s1ap_id(*MME_UE_S1AP_ID);
    if (!target_ue) {
        ogs_error("No eNB UE Context : MME_UE_S1AP_ID[%lld]",
                (long long)*MME_UE_S1AP_ID);
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, NULL,
                S1AP_Cause_PR_radioNetwork,
                S1AP_CauseRadioNetwork_unknown_mme_ue_s1ap_id));
        return;
    }

    if (!ENB_UE_S1AP_ID) {
        ogs_error("No ENB_UE_S1AP_ID");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    if (!EUTRAN_CGI) {
        ogs_error("No EUTRAN_CGI");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    pLMNidentity = &EUTRAN_CGI->pLMNidentity;
    ogs_assert(pLMNidentity && pLMNidentity->size == sizeof(ogs_plmn_id_t));
    cell_ID = &EUTRAN_CGI->cell_ID;
    ogs_assert(cell_ID);

    if (!TAI) {
        ogs_error("No TAI");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    pLMNidentity = &TAI->pLMNidentity;
    ogs_assert(pLMNidentity && pLMNidentity->size == sizeof(ogs_plmn_id_t));
    tAC = &TAI->tAC;
    ogs_assert(tAC && tAC->size == sizeof(uint16_t));

    source_ue = target_ue->source_ue;
    if (!source_ue) {
        ogs_error("No Source UE");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, MME_UE_S1AP_ID, ENB_UE_S1AP_ID,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    mme_ue = source_ue->mme_ue;
    ogs_expect_or_return(mme_ue);

    ogs_debug("    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);
    ogs_debug("    Target : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            target_ue->enb_ue_s1ap_id, target_ue->mme_ue_s1ap_id);

    mme_ue_associate_enb_ue(mme_ue, target_ue);

    memcpy(&target_ue->saved.tai.plmn_id, pLMNidentity->buf, 
            sizeof(target_ue->saved.tai.plmn_id));
    memcpy(&target_ue->saved.tai.tac,
            tAC->buf, sizeof(target_ue->saved.tai.tac));
    target_ue->saved.tai.tac = be16toh(target_ue->saved.tai.tac);

    memcpy(&target_ue->saved.e_cgi.plmn_id, pLMNidentity->buf, 
            sizeof(target_ue->saved.e_cgi.plmn_id));
    memcpy(&target_ue->saved.e_cgi.cell_id, cell_ID->buf,
            sizeof(target_ue->saved.e_cgi.cell_id));
    target_ue->saved.e_cgi.cell_id =
        (be32toh(target_ue->saved.e_cgi.cell_id) >> 4);

    ogs_debug("    OLD TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&mme_ue->tai.plmn_id),
            mme_ue->tai.tac);
    ogs_debug("    OLD E_CGI[PLMN_ID:%06x,CELL_ID:0x%x]",
            ogs_plmn_id_hexdump(&mme_ue->e_cgi.plmn_id),
            mme_ue->e_cgi.cell_id);
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&target_ue->saved.tai.plmn_id),
            target_ue->saved.tai.tac);
    ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:0x%x]",
            ogs_plmn_id_hexdump(&target_ue->saved.e_cgi.plmn_id),
            target_ue->saved.e_cgi.cell_id);

    /* Copy Stream-No/TAI/ECGI from enb_ue */
    mme_ue->enb_ostream_id = target_ue->enb_ostream_id;
    memcpy(&mme_ue->tai, &target_ue->saved.tai, sizeof(ogs_eps_tai_t));
    memcpy(&mme_ue->e_cgi, &target_ue->saved.e_cgi, sizeof(ogs_e_cgi_t));

    ogs_assert(OGS_OK ==
        s1ap_send_ue_context_release_command(source_ue,
            S1AP_Cause_PR_radioNetwork,
            S1AP_CauseRadioNetwork_successful_handover,
            S1AP_UE_CTX_REL_S1_HANDOVER_COMPLETE,
            ogs_app()->time.handover.duration));

    ogs_list_for_each(&mme_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            bearer->enb_s1u_teid = bearer->target_s1u_teid;
            memcpy(&bearer->enb_s1u_ip, &bearer->target_s1u_ip,
                    sizeof(ogs_ip_t));

            ogs_assert(OGS_OK ==
                mme_gtp_send_modify_bearer_request(bearer, 1));
        }
    }
}

void s1ap_handle_s1_reset(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_Reset_t *Reset = NULL;

    S1AP_ResetIEs_t *ie = NULL;
    S1AP_Cause_t *Cause = NULL;
    S1AP_ResetType_t *ResetType = NULL;
    S1AP_UE_associatedLogicalS1_ConnectionListRes_t *partOfS1_Interface = NULL;

    enb_ue_t *iter = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    Reset = &initiatingMessage->value.choice.Reset;
    ogs_assert(Reset);

    ogs_warn("Reset");

    for (i = 0; i < Reset->protocolIEs.list.count; i++) {
        ie = Reset->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        case S1AP_ProtocolIE_ID_id_ResetType:
            ResetType = &ie->value.choice.ResetType;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

    if (!Cause) {
        ogs_error("No Cause");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, NULL,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    ogs_warn("    Cause[Group:%d Cause:%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

    if (!ResetType) {
        ogs_error("No ResetType");
        ogs_assert(OGS_OK ==
            s1ap_send_error_indication(enb, NULL, NULL,
                S1AP_Cause_PR_protocol, S1AP_CauseProtocol_semantic_error));
        return;
    }

    switch (ResetType->present) {
    case S1AP_ResetType_PR_s1_Interface:
        ogs_warn("    S1AP_ResetType_PR_s1_Interface");

        mme_gtp_send_release_all_ue_in_enb(
                enb, OGS_GTP_RELEASE_S1_CONTEXT_REMOVE_BY_RESET_ALL);

        /*
         * TS36.413
         * 8.7.1.2.1 Reset Procedure Initiated from the MME
         *
         * The eNB does not need to wait for the release of radio resources
         * to be completed before returning the RESET ACKNOWLEDGE message.
         *
         * 8.7.1.2.2 Reset Procedure Initiated from the E-UTRAN
         * After the MME has released all assigned S1 resources and
         * the UE S1AP IDs for all indicated UE associations which can be used
         * for new UE-associated logical S1-connections over the S1 interface,
         * the MME shall respond with the RESET ACKNOWLEDGE message.
         */
        if (ogs_list_count(&enb->enb_ue_list) == 0)
            ogs_assert(OGS_OK ==
                s1ap_send_s1_reset_ack(enb, NULL));

        break;

    case S1AP_ResetType_PR_partOfS1_Interface:
        ogs_warn("    S1AP_ResetType_PR_partOfS1_Interface");

        partOfS1_Interface = ResetType->choice.partOfS1_Interface;
        ogs_assert(partOfS1_Interface);

        if (enb->s1_reset_ack)
            ogs_pkbuf_free(enb->s1_reset_ack);

        enb->s1_reset_ack = ogs_s1ap_build_s1_reset_ack(partOfS1_Interface);
        ogs_expect_or_return(enb->s1_reset_ack);

        for (i = 0; i < partOfS1_Interface->list.count; i++) {
            S1AP_UE_associatedLogicalS1_ConnectionItemRes_t *ie2 = NULL;
            S1AP_UE_associatedLogicalS1_ConnectionItem_t *item = NULL;

            enb_ue_t *enb_ue = NULL;
            mme_ue_t *mme_ue = NULL;

            ie2 = (S1AP_UE_associatedLogicalS1_ConnectionItemRes_t *)
                partOfS1_Interface->list.array[i];
            if (!ie2) {
                ogs_error("No S1AP_UE_associatedLogicalS1_ConnectionItemRes_t");
                continue;
            }

            item = &ie2->value.choice.UE_associatedLogicalS1_ConnectionItem;
            if (!item) {
                ogs_error("No UE_associatedLogicalS1_ConnectionItem");
                continue;
            }

            ogs_warn("    MME_UE_S1AP_ID[%d] ENB_UE_S1AP_ID[%d]",
                    item->mME_UE_S1AP_ID ? (int)*item->mME_UE_S1AP_ID : -1,
                    item->eNB_UE_S1AP_ID ? (int)*item->eNB_UE_S1AP_ID : -1);

            if (item->mME_UE_S1AP_ID)
                enb_ue = enb_ue_find_by_mme_ue_s1ap_id( *item->mME_UE_S1AP_ID);
            else if (item->eNB_UE_S1AP_ID)
                enb_ue = enb_ue_find_by_enb_ue_s1ap_id(enb,
                        *item->eNB_UE_S1AP_ID);

            if (enb_ue == NULL) {
                ogs_error("Cannot find S1 Context "
                    "(MME_UE_S1AP_ID[%d] ENB_UE_S1AP_ID[%d])",
                    item->mME_UE_S1AP_ID ? (int)*item->mME_UE_S1AP_ID : -1,
                    item->eNB_UE_S1AP_ID ? (int)*item->eNB_UE_S1AP_ID : -1);
                continue;
            }

            /* ENB_UE Context where PartOfS1_interface was requested */
            enb_ue->part_of_s1_reset_requested = true;

            mme_ue = enb_ue->mme_ue;
            if (mme_ue) {
                ogs_assert(OGS_OK ==
                    mme_gtp_send_release_access_bearers_request(mme_ue,
                        OGS_GTP_RELEASE_S1_CONTEXT_REMOVE_BY_RESET_PARTIAL));
            } else {
                enb_ue_remove(enb_ue);
            }
        }

        /*
         * TS36.413
         * 8.7.1.2.1 Reset Procedure Initiated from the MME
         *
         * The eNB does not need to wait for the release of radio resources
         * to be completed before returning the RESET ACKNOWLEDGE message.
         *
         * 8.7.1.2.2 Reset Procedure Initiated from the E-UTRAN
         * After the MME has released all assigned S1 resources and
         * the UE S1AP IDs for all indicated UE associations which can be used
         * for new UE-associated logical S1-connections over the S1 interface,
         * the MME shall respond with the RESET ACKNOWLEDGE message.
         */
        ogs_list_for_each(&enb->enb_ue_list, iter) {
            if (iter->part_of_s1_reset_requested == true) {
                /* The ENB_UE context
                 * where PartOfS1_interface was requested
                 * still remains */
                return;
            }
        }

        /* All ENB_UE context
         * where PartOfS1_interface was requested
         * REMOVED */
        ogs_assert(enb->s1_reset_ack);
        s1ap_send_to_enb(enb, enb->s1_reset_ack, S1AP_NON_UE_SIGNALLING);

        /* Clear S1-Reset Ack Buffer */
        enb->s1_reset_ack = NULL;
        break;
    default:
        ogs_warn("Invalid ResetType[%d]", ResetType->present);
        break;
    }
}

void s1ap_handle_write_replace_warning_response(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];

    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_WriteReplaceWarningResponse_t *WriteReplaceWarningResponse = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    WriteReplaceWarningResponse =
        &successfulOutcome->value.choice.WriteReplaceWarningResponse;
    ogs_assert(WriteReplaceWarningResponse);

    ogs_debug("WriteReplaceWarningResponse");

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);

}

void s1ap_handle_kill_response(
        mme_enb_t *enb, ogs_s1ap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];

    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_KillResponse_t *KillResponse = NULL;

    ogs_assert(enb);
    ogs_assert(enb->sctp.sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    KillResponse =
        &successfulOutcome->value.choice.KillResponse;
    ogs_assert(KillResponse);

    ogs_debug("KillResponse");

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(enb->sctp.addr, buf), enb->enb_id);
}
