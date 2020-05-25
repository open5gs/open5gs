/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "ngap-handler.h"
#include "ngap-path.h"

static bool served_tai_is_found(amf_gnb_t *gnb)
{
    int i;
    int served_tai_index;

    for (i = 0; i < gnb->num_of_supported_ta_list; i++) {
        served_tai_index = amf_find_served_tai(&gnb->supported_ta_list[i]);
        if (served_tai_index >= 0 &&
                served_tai_index < OGS_MAX_NUM_OF_SERVED_TAI) {
            ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);
            return true;
        }
    }

    return false;
}

static bool maximum_number_of_gnbs_is_reached(void)
{
    amf_gnb_t *gnb = NULL, *next_gnb = NULL;
    int number_of_gnbs_online = 0;

    ogs_list_for_each_safe(&amf_self()->gnb_list, next_gnb, gnb) {
        if (gnb->state.ng_setup_success) {
            number_of_gnbs_online++;
        }
    }

    return number_of_gnbs_online >= ogs_config()->max.gnb;
}

void ngap_handle_ng_setup_request(amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, j;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_NGSetupRequest_t *NGSetupRequest = NULL;

    NGAP_NGSetupRequestIEs_t *ie = NULL;
    NGAP_GlobalRANNodeID_t *GlobalRANNodeID = NULL;
    NGAP_GlobalGNB_ID_t *globalGNB_ID = NULL;
    NGAP_SupportedTAList_t *SupportedTAList = NULL;
    NGAP_PagingDRX_t *PagingDRX = NULL;

    uint32_t gnb_id;
    NGAP_Cause_PR group = NGAP_Cause_PR_NOTHING;
    long cause = 0;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    NGSetupRequest = &initiatingMessage->value.choice.NGSetupRequest;
    ogs_assert(NGSetupRequest);

    ogs_debug("[AMF] NG-Setup request");

    for (i = 0; i < NGSetupRequest->protocolIEs.list.count; i++) {
        ie = NGSetupRequest->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_GlobalRANNodeID:
            GlobalRANNodeID = &ie->value.choice.GlobalRANNodeID;
            break;
        case NGAP_ProtocolIE_ID_id_SupportedTAList:
            SupportedTAList = &ie->value.choice.SupportedTAList;
            break;
        case NGAP_ProtocolIE_ID_id_DefaultPagingDRX:
            PagingDRX = &ie->value.choice.PagingDRX;
            break;
        default:
            break;
        }
    }

    if (!GlobalRANNodeID) {
        ogs_warn("No GlobalRANNodeID");
        group = NGAP_Cause_PR_protocol;
        cause = NGAP_CauseProtocol_semantic_error;
        ngap_send_ng_setup_failure(gnb, group, cause);
        return;
    }

    globalGNB_ID = GlobalRANNodeID->choice.globalGNB_ID;
    if (!globalGNB_ID) {
        ogs_warn("No globalGNB_ID");
        group = NGAP_Cause_PR_protocol;
        cause = NGAP_CauseProtocol_semantic_error;
        ngap_send_ng_setup_failure(gnb, group, cause);
        return;
    }

    if (!SupportedTAList) {
        ogs_warn("No SupportedTAList");
        group = NGAP_Cause_PR_protocol;
        cause = NGAP_CauseProtocol_semantic_error;
        ngap_send_ng_setup_failure(gnb, group, cause);
        return;
    }

    ogs_ngap_GNB_ID_to_uint32(&globalGNB_ID->gNB_ID, &gnb_id);
    ogs_debug("    IP[%s] GNB_ID[%x]", OGS_ADDR(gnb->addr, buf), gnb_id);

    if (PagingDRX)
        ogs_debug("    PagingDRX[%ld]", *PagingDRX);

    amf_gnb_set_gnb_id(gnb, gnb_id);

    /* Parse Supported TA */
    gnb->num_of_supported_ta_list = 0;
    for (i = 0; i < SupportedTAList->list.count; i++) {
        NGAP_SupportedTAItem_t *SupportedTAItem = NULL;
        NGAP_TAC_t *tAC = NULL;

        SupportedTAItem = (NGAP_SupportedTAItem_t *)
                SupportedTAList->list.array[i];
        ogs_assert(SupportedTAItem);
        tAC = &SupportedTAItem->tAC;
        ogs_assert(tAC);

        for (j = 0; j < SupportedTAItem->broadcastPLMNList.list.count; j++) {
            NGAP_BroadcastPLMNItem_t *BroadcastPLMNItem = NULL;
            NGAP_PLMNIdentity_t *pLMNIdentity = NULL;

            BroadcastPLMNItem = (NGAP_BroadcastPLMNItem_t *)
                    SupportedTAItem->broadcastPLMNList.list.array[i];
            ogs_assert(BroadcastPLMNItem);
            pLMNIdentity = (NGAP_PLMNIdentity_t *)
                    &BroadcastPLMNItem->pLMNIdentity;
            ogs_assert(pLMNIdentity);

            memcpy(&gnb->supported_ta_list[gnb->num_of_supported_ta_list].tac,
                    tAC->buf, sizeof(ogs_uint24_t));
            gnb->supported_ta_list[gnb->num_of_supported_ta_list].tac =
                ogs_be24toh(
                    gnb->supported_ta_list[gnb->num_of_supported_ta_list].tac);
            memcpy(&gnb->supported_ta_list
                        [gnb->num_of_supported_ta_list].plmn_id,
                    pLMNIdentity->buf, sizeof(ogs_plmn_id_t));
            ogs_debug("    PLMN_ID[MCC:%d MNC:%d] TAC[%d]",
                ogs_plmn_id_mcc(&gnb->supported_ta_list
                    [gnb->num_of_supported_ta_list].plmn_id),
                ogs_plmn_id_mnc(&gnb->supported_ta_list
                    [gnb->num_of_supported_ta_list].plmn_id),
                gnb->supported_ta_list[gnb->num_of_supported_ta_list].tac.v);
            gnb->num_of_supported_ta_list++;
        }
    }

    if (maximum_number_of_gnbs_is_reached()) {
        ogs_warn("NG-Setup failure:");
        ogs_warn("    Maximum number of gNBs reached");
        group = NGAP_Cause_PR_misc;
        cause = NGAP_CauseMisc_control_processing_overload;

        ngap_send_ng_setup_failure(gnb, group, cause);
        return;
    }

    if (gnb->num_of_supported_ta_list == 0) {
        ogs_warn("NG-Setup failure:");
        ogs_warn("    No supported TA exist in NG-Setup request");
        group = NGAP_Cause_PR_protocol;
        cause = NGAP_CauseProtocol_message_not_compatible_with_receiver_state;

        ngap_send_ng_setup_failure(gnb, group, cause);
        return;
    }

    if (!served_tai_is_found(gnb)) {
        ogs_warn("NG-Setup failure:");
        ogs_warn("    Cannot find Served TAI. Check 'amf.tai' configuration");
        group = NGAP_Cause_PR_misc;
        cause = NGAP_CauseMisc_unknown_PLMN;

        ngap_send_ng_setup_failure(gnb, group, cause);
        return;
    }

    gnb->state.ng_setup_success = true;
    ngap_send_ng_setup_response(gnb);
}

#if 0
void ngap_handle_initial_ue_message(amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int i;
    char buf[OGS_ADDRSTRLEN];

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_InitialUEMessage_t *InitialUEMessage = NULL;

    NGAP_InitialUEMessage_IEs_t *ie = NULL;
    NGAP_ENB_UE_NGAP_ID_t *ENB_UE_NGAP_ID = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;
    NGAP_TAI_t *TAI = NULL;
    NGAP_EUTRAN_CGI_t *EUTRAN_CGI = NULL;
    NGAP_S_TMSI_t *S_TMSI = NULL;

	NGAP_PLMNidentity_t	*pLMNidentity = NULL;
	NGAP_TAC_t *tAC = NULL;
	NGAP_CellIdentity_t *cell_ID = NULL;

    gnb_ue_t *gnb_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    InitialUEMessage = &initiatingMessage->value.choice.InitialUEMessage;
    ogs_assert(InitialUEMessage);

    ogs_debug("[AMF] Initial UE Message");

    for (i = 0; i < InitialUEMessage->protocolIEs.list.count; i++) {
        ie = InitialUEMessage->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_eNB_UE_NGAP_ID:
            ENB_UE_NGAP_ID = &ie->value.choice.ENB_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_NAS_PDU:
            NAS_PDU = &ie->value.choice.NAS_PDU;
            break;
        case NGAP_ProtocolIE_ID_id_TAI:
            TAI = &ie->value.choice.TAI;
            break;
        case NGAP_ProtocolIE_ID_id_EUTRAN_CGI:
            EUTRAN_CGI = &ie->value.choice.EUTRAN_CGI;
            break;
        case NGAP_ProtocolIE_ID_id_S_TMSI:
            S_TMSI = &ie->value.choice.S_TMSI;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(ENB_UE_NGAP_ID);
    gnb_ue = gnb_ue_find_by_gnb_ue_ngap_id(gnb, *ENB_UE_NGAP_ID);
    if (!gnb_ue) {
        gnb_ue = gnb_ue_add(gnb, *ENB_UE_NGAP_ID);
        ogs_assert(gnb_ue);

        /* Find AMF_UE if S_TMSI included */
        if (S_TMSI) {
            served_guamfi_t *served_guamfi = &amf_self()->served_guamfi[0];
            ogs_nas_5gs_guti_t nas_guti;
            amf_ue_t *amf_ue = NULL;

            memset(&nas_guti, 0, sizeof(ogs_nas_5gs_guti_t));

            /* Use the first configured plmn_id and amf group id */
            ogs_nas_from_plmn_id(&nas_guti.nas_plmn_id, &served_guamfi->plmn_id[0]);
            nas_guti.amf_gid = served_guamfi->amf_gid[0];

            /* size must be 1 */
            memcpy(&nas_guti.amf_code, S_TMSI->mMEC.buf, S_TMSI->mMEC.size);
            /* size must be 4 */
            memcpy(&nas_guti.m_tmsi, S_TMSI->m_TMSI.buf, S_TMSI->m_TMSI.size);
            nas_guti.m_tmsi = ntohl(nas_guti.m_tmsi);

            amf_ue = amf_ue_find_by_guti(&nas_guti);
            if (!amf_ue) {
                ogs_warn("Unknown UE by S_TMSI[G:%d,C:%d,M_TMSI:0x%x]",
                        nas_guti.amf_gid, nas_guti.amf_code, nas_guti.m_tmsi);
            } else {
                ogs_debug("    S_TMSI[G:%d,C:%d,M_TMSI:0x%x] IMSI:[%s]",
                        amf_ue->guti.amf_gid,
                        amf_ue->guti.amf_code,
                        amf_ue->guti.m_tmsi,
                        AMF_UE_HAVE_IMSI(amf_ue) 
                            ? amf_ue->imsi_bcd : "Unknown");

                /* If NAS(amf_ue_t) has already been associated with
                 * older S1(gnb_ue_t) context */
                if (ECM_CONNECTED(amf_ue)) {
                   /* Implcit S1 release */
                    ogs_debug("Implicit S1 release");
                    ogs_debug("    ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
                          amf_ue->gnb_ue->gnb_ue_ngap_id,
                          amf_ue->gnb_ue->amf_ue_ngap_id);
                    gnb_ue_remove(amf_ue->gnb_ue);
                }
                amf_ue_associate_gnb_ue(amf_ue, gnb_ue);
            }
        }
    }

    ogs_assert(TAI);
    pLMNidentity = &TAI->pLMNidentity;
    ogs_assert(pLMNidentity && pLMNidentity->size == sizeof(ogs_plmn_id_t));
    tAC = &TAI->tAC;
    ogs_assert(tAC && tAC->size == sizeof(uint16_t));

    memcpy(&gnb_ue->saved.tai.plmn_id, pLMNidentity->buf, 
            sizeof(gnb_ue->saved.tai.plmn_id));
    memcpy(&gnb_ue->saved.tai.tac, tAC->buf, sizeof(gnb_ue->saved.tai.tac));
    gnb_ue->saved.tai.tac = ntohs(gnb_ue->saved.tai.tac);
    
    ogs_assert(EUTRAN_CGI);
    pLMNidentity = &EUTRAN_CGI->pLMNidentity;
    ogs_assert(pLMNidentity && pLMNidentity->size == sizeof(ogs_plmn_id_t));
    cell_ID = &EUTRAN_CGI->cell_ID;
    ogs_assert(cell_ID);
    memcpy(&gnb_ue->saved.e_cgi.plmn_id, pLMNidentity->buf, 
            sizeof(gnb_ue->saved.e_cgi.plmn_id));
    memcpy(&gnb_ue->saved.e_cgi.cell_id, cell_ID->buf,
            sizeof(gnb_ue->saved.e_cgi.cell_id));
    gnb_ue->saved.e_cgi.cell_id = (ntohl(gnb_ue->saved.e_cgi.cell_id) >> 4);

    ogs_debug("    ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d] TAC[%d]",
        gnb_ue->gnb_ue_ngap_id, gnb_ue->amf_ue_ngap_id, gnb_ue->saved.tai.tac);

    ngap_send_to_nas(gnb_ue,
            NGAP_ProcedureCode_id_initialUEMessage, NAS_PDU);
}

void ngap_handle_uplink_nas_transport(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UplinkNASTransport_t *UplinkNASTransport = NULL;

    NGAP_UplinkNASTransport_IEs_t *ie = NULL;
    NGAP_ENB_UE_NGAP_ID_t *ENB_UE_NGAP_ID = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;

    gnb_ue_t *gnb_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    UplinkNASTransport = &initiatingMessage->value.choice.UplinkNASTransport;
    ogs_assert(UplinkNASTransport);

    ogs_debug("[AMF] Uplink NAS transport");

    for (i = 0; i < UplinkNASTransport->protocolIEs.list.count; i++) {
        ie = UplinkNASTransport->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_eNB_UE_NGAP_ID:
            ENB_UE_NGAP_ID = &ie->value.choice.ENB_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_NAS_PDU:
            NAS_PDU = &ie->value.choice.NAS_PDU;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(ENB_UE_NGAP_ID);
    gnb_ue = gnb_ue_find_by_gnb_ue_ngap_id(gnb, *ENB_UE_NGAP_ID);
    ogs_expect_or_return(gnb_ue);

    ogs_debug("    ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            gnb_ue->gnb_ue_ngap_id, gnb_ue->amf_ue_ngap_id);

    ngap_send_to_nas(gnb_ue,
            NGAP_ProcedureCode_id_uplinkNASTransport, NAS_PDU);
}

void ngap_handle_ue_capability_info_indication(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UECapabilityInfoIndication_t *UECapabilityInfoIndication = NULL;

    NGAP_UECapabilityInfoIndicationIEs_t *ie = NULL;
    NGAP_ENB_UE_NGAP_ID_t *ENB_UE_NGAP_ID = NULL;
    NGAP_UERadioCapability_t *UERadioCapability = NULL;

    gnb_ue_t *gnb_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    UECapabilityInfoIndication =
        &initiatingMessage->value.choice.UECapabilityInfoIndication;
    ogs_assert(UECapabilityInfoIndication);

    ogs_debug("[AMF] UE capability info indication");

    for (i = 0; i < UECapabilityInfoIndication->protocolIEs.list.count; i++) {
        ie = UECapabilityInfoIndication->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_eNB_UE_NGAP_ID:
            ENB_UE_NGAP_ID = &ie->value.choice.ENB_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_UERadioCapability:
            UERadioCapability = &ie->value.choice.UERadioCapability;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(ENB_UE_NGAP_ID);
    gnb_ue = gnb_ue_find_by_gnb_ue_ngap_id(gnb, *ENB_UE_NGAP_ID);
    ogs_assert(gnb_ue);

    ogs_debug("    ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            gnb_ue->gnb_ue_ngap_id, gnb_ue->amf_ue_ngap_id);

    if (gnb_ue->amf_ue) {
        ogs_assert(UERadioCapability);
        OGS_NGAP_STORE_DATA(&gnb_ue->amf_ue->ueRadioCapability,
                UERadioCapability);
    }
}

void ngap_handle_initial_context_setup_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_InitialContextSetupResponse_t *InitialContextSetupResponse = NULL;

    NGAP_InitialContextSetupResponseIEs_t *ie = NULL;
    NGAP_ENB_UE_NGAP_ID_t *ENB_UE_NGAP_ID = NULL;
    NGAP_E_RABSetupListCtxtSURes_t *E_RABSetupListCtxtSURes = NULL;

    amf_ue_t *amf_ue = NULL;
    gnb_ue_t *gnb_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    InitialContextSetupResponse =
        &successfulOutcome->value.choice.InitialContextSetupResponse;
    ogs_assert(InitialContextSetupResponse);

    ogs_debug("[AMF] Initial context setup response");

    for (i = 0; i < InitialContextSetupResponse->protocolIEs.list.count; i++) {
        ie = InitialContextSetupResponse->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_eNB_UE_NGAP_ID:
            ENB_UE_NGAP_ID = &ie->value.choice.ENB_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_E_RABSetupListCtxtSURes:
            E_RABSetupListCtxtSURes =
                &ie->value.choice.E_RABSetupListCtxtSURes;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(ENB_UE_NGAP_ID);
    gnb_ue = gnb_ue_find_by_gnb_ue_ngap_id(gnb, *ENB_UE_NGAP_ID);
    ogs_expect_or_return(gnb_ue);
    amf_ue = gnb_ue->amf_ue;
    ogs_assert(amf_ue);

    ogs_debug("    ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            gnb_ue->gnb_ue_ngap_id, gnb_ue->amf_ue_ngap_id);

    ogs_assert(E_RABSetupListCtxtSURes);
    for (i = 0; i < E_RABSetupListCtxtSURes->list.count; i++) {
        NGAP_E_RABSetupItemCtxtSUResIEs_t *ie2 = NULL;
        NGAP_E_RABSetupItemCtxtSURes_t *e_rab = NULL;

        amf_bearer_t *bearer = NULL;

        ie2 = (NGAP_E_RABSetupItemCtxtSUResIEs_t *)
            E_RABSetupListCtxtSURes->list.array[i];
        ogs_assert(ie2);

        e_rab = &ie2->value.choice.E_RABSetupItemCtxtSURes;
        ogs_assert(e_rab);

        bearer = amf_bearer_find_by_ue_ebi(amf_ue, e_rab->e_RAB_ID);
        ogs_assert(bearer);
        memcpy(&bearer->gnb_s1u_teid, e_rab->gTP_TEID.buf, 
                sizeof(bearer->gnb_s1u_teid));
        bearer->gnb_s1u_teid = ntohl(bearer->gnb_s1u_teid);
        rv = ogs_asn_BIT_STRING_to_ip(
                &e_rab->transportLayerAddress, &bearer->gnb_s1u_ip);
        ogs_assert(rv == OGS_OK);

        ogs_debug("    EBI[%d] ENB-S1U-TEID[%d]",
                bearer->ebi, bearer->gnb_s1u_teid);

        if (OGS_FSM_CHECK(&bearer->sm, esm_state_active)) {
            ogs_debug("    NAS_EPS Type[%d]", amf_ue->nas_eps.type);
            int uli_presence = 0;
            if (amf_ue->nas_eps.type != AMF_EPS_TYPE_ATTACH_REQUEST) {
                ogs_debug("    ### ULI PRESENT ###");
                uli_presence = 1;
            }
            amf_gtp_send_modify_bearer_request(bearer, uli_presence);
        }
    }

    if (SMS_SERVICE_INDICATOR(amf_ue)) {
        sgsap_send_service_request(amf_ue, SGSAP_EMM_CONNECTED_MODE);
    }

    CLEAR_SERVICE_INDICATOR(amf_ue);
}

void ngap_handle_initial_context_setup_failure(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    NGAP_InitialContextSetupFailure_t *InitialContextSetupFailure = NULL;

    NGAP_InitialContextSetupFailureIEs_t *ie = NULL;
    NGAP_ENB_UE_NGAP_ID_t *ENB_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    amf_ue_t *amf_ue = NULL;
    gnb_ue_t *gnb_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    unsuccessfulOutcome = message->choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    InitialContextSetupFailure =
        &unsuccessfulOutcome->value.choice.InitialContextSetupFailure;
    ogs_assert(InitialContextSetupFailure);

    ogs_debug("[AMF] Initial context setup failure");

    for (i = 0; i < InitialContextSetupFailure->protocolIEs.list.count; i++) {
        ie = InitialContextSetupFailure->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_eNB_UE_NGAP_ID:
            ENB_UE_NGAP_ID = &ie->value.choice.ENB_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(ENB_UE_NGAP_ID);
    ogs_assert(Cause);

    gnb_ue = gnb_ue_find_by_gnb_ue_ngap_id(gnb, *ENB_UE_NGAP_ID);
    if (gnb_ue == NULL) {
        ogs_warn("Initial context setup failure : "
                "cannot find eNB-UE-NGAP-ID[%d]", (int)*ENB_UE_NGAP_ID);
        return;
    }

    ogs_debug("    ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            gnb_ue->gnb_ue_ngap_id, gnb_ue->amf_ue_ngap_id);
    ogs_debug("    Cause[Group:%d Cause:%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

    if (amf_ue)
        CLEAR_SERVICE_INDICATOR(amf_ue);

    /*
     * 19.2.2.3 in Spec 36.300
     *
     * In case of failure, eNB and AMF behaviours are not mandated.
     *
     * Both implicit release (local release at each node) and
     * explicit release (AMF-initiated UE Context Release procedure)
     * may in principle be adopted. The eNB should ensure
     * that no hanging resources remain at the eNB.
     */
    amf_send_delete_session_or_gnb_ue_context_release(gnb_ue);
}

void ngap_handle_ue_context_modification_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_UEContextModificationResponse_t *UEContextModificationResponse = NULL;

    NGAP_UEContextModificationResponseIEs_t *ie = NULL;
    NGAP_ENB_UE_NGAP_ID_t *ENB_UE_NGAP_ID = NULL;

    amf_ue_t *amf_ue = NULL;
    gnb_ue_t *gnb_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    UEContextModificationResponse =
        &successfulOutcome->value.choice.UEContextModificationResponse;
    ogs_assert(UEContextModificationResponse);

    ogs_debug("[AMF] UE context modification response");

    for (i = 0; i < UEContextModificationResponse->protocolIEs.list.count; i++) {
        ie = UEContextModificationResponse->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_eNB_UE_NGAP_ID:
            ENB_UE_NGAP_ID = &ie->value.choice.ENB_UE_NGAP_ID;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(ENB_UE_NGAP_ID);
    gnb_ue = gnb_ue_find_by_gnb_ue_ngap_id(gnb, *ENB_UE_NGAP_ID);
    ogs_assert(gnb_ue);
    amf_ue = gnb_ue->amf_ue;
    ogs_assert(amf_ue);

    ogs_debug("    ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            gnb_ue->gnb_ue_ngap_id, gnb_ue->amf_ue_ngap_id);

    CLEAR_SERVICE_INDICATOR(amf_ue);
}

void ngap_handle_ue_context_modification_failure(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    NGAP_UEContextModificationFailure_t *UEContextModificationFailure = NULL;

    NGAP_UEContextModificationFailureIEs_t *ie = NULL;
    NGAP_ENB_UE_NGAP_ID_t *ENB_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    gnb_ue_t *gnb_ue = NULL;
    amf_ue_t *amf_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    unsuccessfulOutcome = message->choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    UEContextModificationFailure =
        &unsuccessfulOutcome->value.choice.UEContextModificationFailure;
    ogs_assert(UEContextModificationFailure);

    ogs_warn("[AMF] UE context modification failure");

    for (i = 0; i < UEContextModificationFailure->protocolIEs.list.count; i++) {
        ie = UEContextModificationFailure->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_eNB_UE_NGAP_ID:
            ENB_UE_NGAP_ID = &ie->value.choice.ENB_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(ENB_UE_NGAP_ID);
    ogs_assert(Cause);

    gnb_ue = gnb_ue_find_by_gnb_ue_ngap_id(gnb, *ENB_UE_NGAP_ID);
    if (gnb_ue == NULL) {
        ogs_warn("Initial context setup failure : "
                "cannot find eNB-UE-NGAP-ID[%d]", (int)*ENB_UE_NGAP_ID);
        goto cleanup;
    }

    ogs_debug("    ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            gnb_ue->gnb_ue_ngap_id, gnb_ue->amf_ue_ngap_id);
    ogs_debug("    Cause[Group:%d Cause:%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

cleanup:
    amf_ue = gnb_ue->amf_ue;
    ogs_assert(amf_ue);
    CLEAR_SERVICE_INDICATOR(amf_ue);
}


void ngap_handle_e_rab_setup_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_E_RABSetupResponse_t *E_RABSetupResponse = NULL;

    NGAP_E_RABSetupResponseIEs_t *ie = NULL;
    NGAP_ENB_UE_NGAP_ID_t *ENB_UE_NGAP_ID = NULL;
    NGAP_E_RABSetupListBearerSURes_t *E_RABSetupListBearerSURes = NULL;

    gnb_ue_t *gnb_ue = NULL;
    amf_ue_t *amf_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    E_RABSetupResponse = &successfulOutcome->value.choice.E_RABSetupResponse;
    ogs_assert(E_RABSetupResponse);

    ogs_debug("[AMF] E-RAB setup response");

    for (i = 0; i < E_RABSetupResponse->protocolIEs.list.count; i++) {
        ie = E_RABSetupResponse->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_eNB_UE_NGAP_ID:
            ENB_UE_NGAP_ID = &ie->value.choice.ENB_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_E_RABSetupListBearerSURes:
            E_RABSetupListBearerSURes =
                &ie->value.choice.E_RABSetupListBearerSURes;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(ENB_UE_NGAP_ID);
    gnb_ue = gnb_ue_find_by_gnb_ue_ngap_id(gnb, *ENB_UE_NGAP_ID);
    ogs_assert(gnb_ue);
    amf_ue = gnb_ue->amf_ue;
    ogs_assert(amf_ue);

    ogs_debug("    ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            gnb_ue->gnb_ue_ngap_id, gnb_ue->amf_ue_ngap_id);

    ogs_assert(E_RABSetupListBearerSURes);
    for (i = 0; i < E_RABSetupListBearerSURes->list.count; i++) {
        NGAP_E_RABSetupItemBearerSUResIEs_t *ie2 = NULL;
        NGAP_E_RABSetupItemBearerSURes_t *e_rab = NULL;

        amf_bearer_t *bearer = NULL;

        ie2 = (NGAP_E_RABSetupItemBearerSUResIEs_t *)
            E_RABSetupListBearerSURes->list.array[i];
        ogs_assert(ie2);

        e_rab = &ie2->value.choice.E_RABSetupItemBearerSURes;
        ogs_assert(e_rab);

        bearer = amf_bearer_find_by_ue_ebi(amf_ue, e_rab->e_RAB_ID);
        ogs_assert(bearer);

        memcpy(&bearer->gnb_s1u_teid, e_rab->gTP_TEID.buf, 
                sizeof(bearer->gnb_s1u_teid));
        bearer->gnb_s1u_teid = ntohl(bearer->gnb_s1u_teid);
        rv = ogs_asn_BIT_STRING_to_ip(
                &e_rab->transportLayerAddress, &bearer->gnb_s1u_ip);
        ogs_assert(rv == OGS_OK);

        ogs_debug("    EBI[%d]", bearer->ebi);

        if (OGS_FSM_CHECK(&bearer->sm, esm_state_active)) {
            amf_bearer_t *linked_bearer = amf_linked_bearer(bearer);
            ogs_assert(linked_bearer);
            ogs_debug("    Linked-EBI[%d]", linked_bearer->ebi);

            if (bearer->ebi == linked_bearer->ebi) {
                amf_gtp_send_modify_bearer_request(bearer, 0);
            } else {
                amf_gtp_send_create_bearer_response(bearer);
            }
        }
    }
}

void ngap_handle_ue_context_release_request(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UEContextReleaseRequest_t *UEContextReleaseRequest = NULL;

    NGAP_UEContextReleaseRequest_IEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_ENB_UE_NGAP_ID_t *ENB_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    gnb_ue_t *gnb_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    UEContextReleaseRequest =
        &initiatingMessage->value.choice.UEContextReleaseRequest;
    ogs_assert(UEContextReleaseRequest);

    ogs_debug("[AMF] UE Context release request");

    for (i = 0; i < UEContextReleaseRequest->protocolIEs.list.count; i++) {
        ie = UEContextReleaseRequest->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_eNB_UE_NGAP_ID:
            ENB_UE_NGAP_ID = &ie->value.choice.ENB_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(AMF_UE_NGAP_ID);
    gnb_ue = gnb_ue_find_by_amf_ue_ngap_id(*AMF_UE_NGAP_ID);
    if (!gnb_ue) {
        ogs_warn("No ENB UE Context : AMF_UE_NGAP_ID[%d]",
                (int)*AMF_UE_NGAP_ID);
        ngap_send_error_indication(gnb, 
                AMF_UE_NGAP_ID, ENB_UE_NGAP_ID,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_amf_ue_ngap_id);
        return;
    }

    ogs_debug("    ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            gnb_ue->gnb_ue_ngap_id, gnb_ue->amf_ue_ngap_id);

    ogs_assert(Cause);
    ogs_debug("    Cause[Group:%d Cause:%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

    switch (Cause->present) {
    case NGAP_Cause_PR_radioNetwork:
    case NGAP_Cause_PR_transport:
    case NGAP_Cause_PR_protocol:
    case NGAP_Cause_PR_misc:
        break;
    case NGAP_Cause_PR_nas:
        ogs_warn("NAS-Cause[%d]", (int)Cause->choice.nas);
        break;
    default:
        ogs_warn("Invalid cause group[%d]", Cause->present);
        break;
    }

    amf_send_release_access_bearer_or_ue_context_release(gnb_ue);
}

void ngap_handle_ue_context_release_complete(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_UEContextReleaseComplete_t *UEContextReleaseComplete = NULL;

    NGAP_UEContextReleaseComplete_IEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;

    amf_ue_t *amf_ue = NULL;
    gnb_ue_t *gnb_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    UEContextReleaseComplete =
        &successfulOutcome->value.choice.UEContextReleaseComplete;
    ogs_assert(UEContextReleaseComplete);

    ogs_debug("[AMF] UE Context release complete");

    for (i = 0; i < UEContextReleaseComplete->protocolIEs.list.count; i++) {
        ie = UEContextReleaseComplete->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(AMF_UE_NGAP_ID);
    gnb_ue = gnb_ue_find_by_amf_ue_ngap_id(*AMF_UE_NGAP_ID);
    if (!gnb_ue) {
        ogs_warn("No ENB UE Context : AMF_UE_NGAP_ID[%d]",
                (int)*AMF_UE_NGAP_ID);
        ngap_send_error_indication(gnb, 
                AMF_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_amf_ue_ngap_id);
        return;
    }

    amf_ue = gnb_ue->amf_ue;

    ogs_debug("    ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            gnb_ue->gnb_ue_ngap_id, gnb_ue->amf_ue_ngap_id);

    switch (gnb_ue->ue_ctx_rel_action) {
    case NGAP_UE_CTX_REL_S1_CONTEXT_REMOVE:
        ogs_debug("    No Action");
        gnb_ue_remove(gnb_ue);
        break;
    case NGAP_UE_CTX_REL_S1_REMOVE_AND_UNLINK:
        ogs_debug("    Action: S1 normal release");
        gnb_ue_remove(gnb_ue);
        amf_ue_deassociate(amf_ue);
        break;
    case NGAP_UE_CTX_REL_UE_CONTEXT_REMOVE:
        ogs_debug("    Action: UE context remove()");
        gnb_ue_remove(gnb_ue);
        amf_ue_remove(amf_ue);
        break;
    case NGAP_UE_CTX_REL_DELETE_INDIRECT_TUNNEL:
        ogs_debug("    Action: Delete indirect tunnel");

        source_ue_deassociate_target_ue(gnb_ue);
        gnb_ue_remove(gnb_ue);

        ogs_assert(amf_ue);
        if (amf_ue_have_indirect_tunnel(amf_ue)) {
            amf_gtp_send_delete_indirect_data_forwarding_tunnel_request(
                    amf_ue);
        } else {
            ogs_warn("Check your eNodeB");
            ogs_warn("  There is no INDIRECT TUNNEL");
            ogs_warn("  Packet could be dropped during S1-Handover");
            rv = amf_ue_clear_indirect_tunnel(amf_ue);
            ogs_expect(rv == OGS_OK);
        }
        break;
    default:
        ogs_fatal("Invalid Action[%d]", gnb_ue->ue_ctx_rel_action);
        ogs_assert_if_reached();
        break;
    }
}

void ngap_handle_path_switch_request(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_PathSwitchRequest_t *PathSwitchRequest = NULL;

    NGAP_PathSwitchRequestIEs_t *ie = NULL;
    NGAP_ENB_UE_NGAP_ID_t *ENB_UE_NGAP_ID = NULL;
    NGAP_E_RABToBeSwitchedDLList_t *E_RABToBeSwitchedDLList = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_EUTRAN_CGI_t *EUTRAN_CGI = NULL;
    NGAP_TAI_t *TAI = NULL;
    NGAP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;

	NGAP_PLMNidentity_t *pLMNidentity = NULL;
	NGAP_CellIdentity_t	*cell_ID = NULL;
	NGAP_TAC_t *tAC = NULL;
	NGAP_EncryptionAlgorithms_t	*encryptionAlgorithms = NULL;
	NGAP_IntegrityProtectionAlgorithms_t *integrityProtectionAlgorithms = NULL;
    uint16_t eea = 0, eia = 0;

    gnb_ue_t *gnb_ue = NULL;
    amf_ue_t *amf_ue = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    PathSwitchRequest = &initiatingMessage->value.choice.PathSwitchRequest;
    ogs_assert(PathSwitchRequest);

    ogs_debug("[AMF] Path switch request");

    for (i = 0; i < PathSwitchRequest->protocolIEs.list.count; i++) {
        ie = PathSwitchRequest->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_eNB_UE_NGAP_ID:
            ENB_UE_NGAP_ID = &ie->value.choice.ENB_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_E_RABToBeSwitchedDLList:
            E_RABToBeSwitchedDLList =
                &ie->value.choice.E_RABToBeSwitchedDLList;
            break;
        case NGAP_ProtocolIE_ID_id_SourceAMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_EUTRAN_CGI:
            EUTRAN_CGI = &ie->value.choice.EUTRAN_CGI;
            break;
        case NGAP_ProtocolIE_ID_id_TAI:
            TAI = &ie->value.choice.TAI;
            break;
        case NGAP_ProtocolIE_ID_id_UESecurityCapabilities:
            UESecurityCapabilities = &ie->value.choice.UESecurityCapabilities;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(EUTRAN_CGI);
    pLMNidentity = &EUTRAN_CGI->pLMNidentity;
    ogs_assert(pLMNidentity && pLMNidentity->size == sizeof(ogs_plmn_id_t));
    cell_ID = &EUTRAN_CGI->cell_ID;
    ogs_assert(cell_ID);

    ogs_assert(TAI);
    pLMNidentity = &TAI->pLMNidentity;
    ogs_assert(pLMNidentity && pLMNidentity->size == sizeof(ogs_plmn_id_t));
    tAC = &TAI->tAC;
    ogs_assert(tAC && tAC->size == sizeof(uint16_t));

    ogs_assert(UESecurityCapabilities);
    encryptionAlgorithms =
        &UESecurityCapabilities->encryptionAlgorithms;
    integrityProtectionAlgorithms =
        &UESecurityCapabilities->integrityProtectionAlgorithms;

    ogs_assert(AMF_UE_NGAP_ID);
    ogs_assert(ENB_UE_NGAP_ID);
    gnb_ue = gnb_ue_find_by_amf_ue_ngap_id(*AMF_UE_NGAP_ID);
    if (!gnb_ue) {
        ogs_error("Cannot find UE from sourceAMF-UE-NGAP-ID[%d] and eNB[%s:%d]",
                (int)*AMF_UE_NGAP_ID, OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

        ngapbuf = ngap_build_path_switch_failure(
                *ENB_UE_NGAP_ID, *AMF_UE_NGAP_ID,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_amf_ue_ngap_id);
        ogs_expect_or_return(ngapbuf);

        ogs_expect(OGS_OK ==
                ngap_send_to_gnb(gnb, ngapbuf, NGAP_NON_UE_SIGNALLING));
        return;
    }

    ogs_debug("    ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            gnb_ue->gnb_ue_ngap_id, gnb_ue->amf_ue_ngap_id);

    amf_ue = gnb_ue->amf_ue;
    ogs_assert(amf_ue);

    if (SECURITY_CONTEXT_IS_VALID(amf_ue)) {
        amf_ue->nhcc++;
        amf_kdf_nh(amf_ue->kasme, amf_ue->nh, amf_ue->nh);
    } else {
        ngapbuf = ngap_build_path_switch_failure(
                *ENB_UE_NGAP_ID, *AMF_UE_NGAP_ID,
                NGAP_Cause_PR_nas, NGAP_CauseNas_authentication_failure);
        ogs_expect_or_return(ngapbuf);

        ngap_send_to_gnb_ue(gnb_ue, ngapbuf);
        return;
    }

    gnb_ue->gnb_ue_ngap_id = *ENB_UE_NGAP_ID;

    memcpy(&gnb_ue->saved.tai.plmn_id, pLMNidentity->buf, 
            sizeof(gnb_ue->saved.tai.plmn_id));
    memcpy(&gnb_ue->saved.tai.tac, tAC->buf, sizeof(gnb_ue->saved.tai.tac));
    gnb_ue->saved.tai.tac = ntohs(gnb_ue->saved.tai.tac);

    memcpy(&gnb_ue->saved.e_cgi.plmn_id, pLMNidentity->buf, 
            sizeof(gnb_ue->saved.e_cgi.plmn_id));
    memcpy(&gnb_ue->saved.e_cgi.cell_id, cell_ID->buf,
            sizeof(gnb_ue->saved.e_cgi.cell_id));
    gnb_ue->saved.e_cgi.cell_id = (ntohl(gnb_ue->saved.e_cgi.cell_id) >> 4);

    ogs_debug("    OLD TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&amf_ue->tai.plmn_id),
            amf_ue->tai.tac);
    ogs_debug("    OLD E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            ogs_plmn_id_hexdump(&amf_ue->e_cgi.plmn_id),
            amf_ue->e_cgi.cell_id);
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&gnb_ue->saved.tai.plmn_id),
            gnb_ue->saved.tai.tac);
    ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            ogs_plmn_id_hexdump(&gnb_ue->saved.e_cgi.plmn_id),
            gnb_ue->saved.e_cgi.cell_id);

    /* Copy TAI and ECGI from gnb_ue */
    memcpy(&amf_ue->tai, &gnb_ue->saved.tai, sizeof(ogs_5gs_tai_t));
    memcpy(&amf_ue->e_cgi, &gnb_ue->saved.e_cgi, sizeof(ogs_e_cgi_t));

    memcpy(&eea, encryptionAlgorithms->buf, sizeof(eea));
    eea = ntohs(eea);
    amf_ue->ue_network_capability.eea = eea >> 9;
    amf_ue->ue_network_capability.eea0 = 1;

    memcpy(&eia, integrityProtectionAlgorithms->buf, sizeof(eia));
    eia = ntohs(eia);
    amf_ue->ue_network_capability.eia = eia >> 9;
    amf_ue->ue_network_capability.eia0 = 0;

    ogs_assert(E_RABToBeSwitchedDLList);
    for (i = 0; i < E_RABToBeSwitchedDLList->list.count; i++) {
        NGAP_E_RABToBeSwitchedDLItemIEs_t *ie2 = NULL;
        NGAP_E_RABToBeSwitchedDLItem_t *e_rab = NULL;

        amf_bearer_t *bearer = NULL;

        ie2 = (NGAP_E_RABToBeSwitchedDLItemIEs_t *)
            E_RABToBeSwitchedDLList->list.array[i];
        ogs_assert(ie2);

        e_rab = &ie2->value.choice.E_RABToBeSwitchedDLItem;
        ogs_assert(e_rab);

        bearer = amf_bearer_find_by_ue_ebi(amf_ue, e_rab->e_RAB_ID);
        ogs_assert(bearer);

        memcpy(&bearer->gnb_s1u_teid, e_rab->gTP_TEID.buf, 
                sizeof(bearer->gnb_s1u_teid));
        bearer->gnb_s1u_teid = ntohl(bearer->gnb_s1u_teid);
        rv = ogs_asn_BIT_STRING_to_ip(
                &e_rab->transportLayerAddress, &bearer->gnb_s1u_ip);
        ogs_expect(rv == OGS_OK);

        GTP_COUNTER_INCREMENT(
                amf_ue, GTP_COUNTER_MODIFY_BEARER_BY_PATH_SWITCH);

        amf_gtp_send_modify_bearer_request(bearer, 1);
    }

    /* Switch to gnb */
    gnb_ue_switch_to_gnb(gnb_ue, gnb);
}

void ngap_handle_gnb_configuration_transfer(
        amf_gnb_t *gnb, ogs_ngap_message_t *message, ogs_pkbuf_t *pkbuf)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_ENBConfigurationTransfer_t *ENBConfigurationTransfer = NULL;

    NGAP_ENBConfigurationTransferIEs_t *ie = NULL;
    NGAP_SONConfigurationTransfer_t *SONConfigurationTransfer = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    ENBConfigurationTransfer =
        &initiatingMessage->value.choice.ENBConfigurationTransfer;
    ogs_assert(ENBConfigurationTransfer);

    ogs_debug("[AMF] ENB configuration transfer");
    for (i = 0; i < ENBConfigurationTransfer->protocolIEs.list.count; i++) {
        ie = ENBConfigurationTransfer->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_SONConfigurationTransferECT:
            SONConfigurationTransfer =
                &ie->value.choice.SONConfigurationTransfer;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    if (SONConfigurationTransfer) {
        NGAP_TargeteNB_ID_t *targeteNB_ID =
            &SONConfigurationTransfer->targeteNB_ID;
        NGAP_SourceeNB_ID_t *sourceeNB_ID =
            &SONConfigurationTransfer->sourceeNB_ID;

        amf_gnb_t *target_gnb = NULL;
        uint32_t source_gnb_id, target_gnb_id;
        uint16_t source_tac, target_tac;

        ogs_ngap_ENB_ID_to_uint32(
                &sourceeNB_ID->global_ENB_ID.eNB_ID, &source_gnb_id);
        ogs_ngap_ENB_ID_to_uint32(
                &targeteNB_ID->global_ENB_ID.eNB_ID, &target_gnb_id);

        memcpy(&source_tac, sourceeNB_ID->selected_TAI.tAC.buf,
                sizeof(source_tac));
        source_tac = ntohs(source_tac);
        memcpy(&target_tac, targeteNB_ID->selected_TAI.tAC.buf,
                sizeof(target_tac));
        target_tac = ntohs(target_tac);

        ogs_debug("    Source : ENB_ID[%s:%d], TAC[%d]",
                sourceeNB_ID->global_ENB_ID.eNB_ID.present == 
                    NGAP_ENB_ID_PR_homeENB_ID ? "Home" : 
                sourceeNB_ID->global_ENB_ID.eNB_ID.present == 
                    NGAP_ENB_ID_PR_macroENB_ID ? "Macro" : "Others",
                source_gnb_id, source_tac);
        ogs_debug("    Target : ENB_ID[%s:%d], TAC[%d]",
                targeteNB_ID->global_ENB_ID.eNB_ID.present == 
                    NGAP_ENB_ID_PR_homeENB_ID ? "Home" : 
                targeteNB_ID->global_ENB_ID.eNB_ID.present == 
                    NGAP_ENB_ID_PR_macroENB_ID ? "Macro" : "Others",
                target_gnb_id, target_tac);

        target_gnb = amf_gnb_find_by_gnb_id(target_gnb_id);
        if (target_gnb == NULL) {
            ogs_warn("eNB configuration transfer : cannot find target eNB-id[%d]",
                    target_gnb_id);
            return;
        }

        ngap_send_amf_configuration_transfer(
                target_gnb, SONConfigurationTransfer);
    }
}

void ngap_handle_handover_required(amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_HandoverRequired_t *HandoverRequired = NULL;

    NGAP_HandoverRequiredIEs_t *ie = NULL;
    NGAP_ENB_UE_NGAP_ID_t *ENB_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_HandoverType_t *HandoverType = NULL;
    NGAP_Cause_t *Cause = NULL;
    NGAP_TargetID_t *TargetID = NULL;
    NGAP_Source_ToTarget_TransparentContainer_t
        *Source_ToTarget_TransparentContainer = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    HandoverRequired = &initiatingMessage->value.choice.HandoverRequired;
    ogs_assert(HandoverRequired);

    gnb_ue_t *source_ue = NULL;
    amf_ue_t *amf_ue = NULL;
    amf_gnb_t *target_gnb = NULL;
    uint32_t target_gnb_id = 0;

    ogs_debug("[AMF] Handover required");
    for (i = 0; i < HandoverRequired->protocolIEs.list.count; i++) {
        ie = HandoverRequired->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_eNB_UE_NGAP_ID:
            ENB_UE_NGAP_ID = &ie->value.choice.ENB_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_HandoverType:
            HandoverType = &ie->value.choice.HandoverType;
            break;
        case NGAP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        case NGAP_ProtocolIE_ID_id_TargetID:
            TargetID = &ie->value.choice.TargetID;
            break;
        case NGAP_ProtocolIE_ID_id_Source_ToTarget_TransparentContainer:
            Source_ToTarget_TransparentContainer =
                &ie->value.choice.Source_ToTarget_TransparentContainer;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(TargetID);
    switch (TargetID->present) {
    case NGAP_TargetID_PR_targeteNB_ID:
        ogs_ngap_ENB_ID_to_uint32(
            &TargetID->choice.targeteNB_ID->global_ENB_ID.eNB_ID,
            &target_gnb_id);
        break;
    default:
        ogs_error("Not implemented(%d)", TargetID->present);
        return;
    }

    target_gnb = amf_gnb_find_by_gnb_id(target_gnb_id);
    if (target_gnb == NULL) {
        ogs_warn("Handover required : cannot find target eNB-id[%d]",
                target_gnb_id);
        return;
    }

    ogs_assert(ENB_UE_NGAP_ID);
    ogs_assert(AMF_UE_NGAP_ID);
    source_ue = gnb_ue_find_by_gnb_ue_ngap_id(gnb, *ENB_UE_NGAP_ID);
    ogs_assert(source_ue);
    ogs_assert(source_ue->amf_ue_ngap_id == *AMF_UE_NGAP_ID);

    ogs_debug("    Source : ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            source_ue->gnb_ue_ngap_id, source_ue->amf_ue_ngap_id);

    amf_ue = source_ue->amf_ue;
    ogs_assert(amf_ue);

    if (SECURITY_CONTEXT_IS_VALID(amf_ue)) {
        amf_ue->nhcc++;
        amf_kdf_nh(amf_ue->kasme, amf_ue->nh, amf_ue->nh);
    } else {
        ogs_assert(Cause);

        ngap_send_handover_preparation_failure(source_ue, Cause);

        return;
    }

    ogs_assert(HandoverType);
    source_ue->handover_type = *HandoverType;

    ngap_send_handover_request(amf_ue, target_gnb,
            ENB_UE_NGAP_ID, AMF_UE_NGAP_ID,
            HandoverType, Cause,
            Source_ToTarget_TransparentContainer);
}

void ngap_handle_handover_request_ack(amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_HandoverRequestAcknowledge_t *HandoverRequestAcknowledge = NULL;

    NGAP_HandoverRequestAcknowledgeIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_ENB_UE_NGAP_ID_t *ENB_UE_NGAP_ID = NULL;
    NGAP_E_RABAdmittedList_t *E_RABAdmittedList = NULL;
    NGAP_Target_ToSource_TransparentContainer_t
        *Target_ToSource_TransparentContainer = NULL;

    gnb_ue_t *source_ue = NULL;
    gnb_ue_t *target_ue = NULL;
    amf_ue_t *amf_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    HandoverRequestAcknowledge =
        &successfulOutcome->value.choice.HandoverRequestAcknowledge;
    ogs_assert(HandoverRequestAcknowledge);

    ogs_debug("[AMF] Handover request acknowledge");
    for (i = 0; i < HandoverRequestAcknowledge->protocolIEs.list.count; i++) {
        ie = HandoverRequestAcknowledge->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_eNB_UE_NGAP_ID:
            ENB_UE_NGAP_ID = &ie->value.choice.ENB_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_E_RABAdmittedList:
            E_RABAdmittedList = &ie->value.choice.E_RABAdmittedList;
            break;
        case NGAP_ProtocolIE_ID_id_Target_ToSource_TransparentContainer:
            Target_ToSource_TransparentContainer =
                &ie->value.choice.Target_ToSource_TransparentContainer;
            break;
        default:
            break;
        }
    }
    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(AMF_UE_NGAP_ID);
    ogs_assert(ENB_UE_NGAP_ID);
    ogs_assert(E_RABAdmittedList);
    ogs_assert(Target_ToSource_TransparentContainer);

    target_ue = gnb_ue_find_by_amf_ue_ngap_id(*AMF_UE_NGAP_ID);
    ogs_assert(target_ue);

    target_ue->gnb_ue_ngap_id = *ENB_UE_NGAP_ID;

    source_ue = target_ue->source_ue;
    ogs_assert(source_ue);
    amf_ue = source_ue->amf_ue;
    ogs_assert(amf_ue);

    ogs_debug("    Source : ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            source_ue->gnb_ue_ngap_id, source_ue->amf_ue_ngap_id);
    ogs_debug("    Target : ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            target_ue->gnb_ue_ngap_id, target_ue->amf_ue_ngap_id);

    for (i = 0; i < E_RABAdmittedList->list.count; i++) {
        NGAP_E_RABAdmittedItemIEs_t *ie2 = NULL;
        NGAP_E_RABAdmittedItem_t *e_rab = NULL;

        amf_bearer_t *bearer = NULL;

        ie2 = (NGAP_E_RABAdmittedItemIEs_t *)E_RABAdmittedList->list.array[i];
        ogs_assert(ie2);

        e_rab = &ie2->value.choice.E_RABAdmittedItem;
        ogs_assert(e_rab);

        bearer = amf_bearer_find_by_ue_ebi(amf_ue, e_rab->e_RAB_ID);
        ogs_assert(bearer);

        memcpy(&bearer->target_s1u_teid, e_rab->gTP_TEID.buf, 
                sizeof(bearer->target_s1u_teid));
        bearer->target_s1u_teid = ntohl(bearer->target_s1u_teid);
        rv = ogs_asn_BIT_STRING_to_ip(
                &e_rab->transportLayerAddress, &bearer->target_s1u_ip);
        ogs_assert(rv == OGS_OK);

        if (e_rab->dL_transportLayerAddress && e_rab->dL_gTP_TEID) {
            ogs_assert(e_rab->dL_gTP_TEID->buf);
            ogs_assert(e_rab->dL_transportLayerAddress->buf);
            memcpy(&bearer->gnb_dl_teid, e_rab->dL_gTP_TEID->buf, 
                    sizeof(bearer->gnb_dl_teid));
            bearer->gnb_dl_teid = ntohl(bearer->gnb_dl_teid);
            rv = ogs_asn_BIT_STRING_to_ip(
                    e_rab->dL_transportLayerAddress, &bearer->gnb_dl_ip);
            ogs_assert(rv == OGS_OK);
        }

        if (e_rab->uL_TransportLayerAddress && e_rab->uL_GTP_TEID) {
            ogs_assert(e_rab->uL_GTP_TEID->buf);
            ogs_assert(e_rab->uL_TransportLayerAddress->buf);
            memcpy(&bearer->gnb_ul_teid, e_rab->uL_GTP_TEID->buf, 
                    sizeof(bearer->gnb_ul_teid));
            bearer->gnb_ul_teid = ntohl(bearer->gnb_ul_teid);
            rv = ogs_asn_BIT_STRING_to_ip(
                    e_rab->uL_TransportLayerAddress, &bearer->gnb_ul_ip);
            ogs_assert(rv == OGS_OK);
        }
    }

    OGS_NGAP_STORE_DATA(&amf_ue->container,
            Target_ToSource_TransparentContainer);

    if (amf_ue_have_indirect_tunnel(amf_ue) == 1) {
        amf_gtp_send_create_indirect_data_forwarding_tunnel_request(
                amf_ue);
    } else {
        ngap_send_handover_command(source_ue);
    }
}

void ngap_handle_handover_failure(amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    NGAP_HandoverFailure_t *HandoverFailure = NULL;

    NGAP_HandoverFailureIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    gnb_ue_t *target_ue = NULL;
    gnb_ue_t *source_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    unsuccessfulOutcome = message->choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    HandoverFailure = &unsuccessfulOutcome->value.choice.HandoverFailure;
    ogs_assert(HandoverFailure);

    ogs_debug("[AMF] Handover failure");
    for (i = 0; i < HandoverFailure->protocolIEs.list.count; i++) {
        ie = HandoverFailure->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(AMF_UE_NGAP_ID);
    ogs_assert(Cause);

    target_ue = gnb_ue_find_by_amf_ue_ngap_id(*AMF_UE_NGAP_ID);
    ogs_assert(target_ue);

    source_ue = target_ue->source_ue;
    ogs_assert(source_ue);

    ogs_debug("    Source : ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            source_ue->gnb_ue_ngap_id, source_ue->amf_ue_ngap_id);
    ogs_debug("    Target : ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            target_ue->gnb_ue_ngap_id, target_ue->amf_ue_ngap_id);

    ngap_send_handover_preparation_failure(source_ue, Cause);

    ngap_send_ue_context_release_command(
        target_ue, NGAP_Cause_PR_radioNetwork,
        NGAP_CauseRadioNetwork_ho_failure_in_target_EPC_eNB_or_target_system,
        NGAP_UE_CTX_REL_DELETE_INDIRECT_TUNNEL, 0);
}

void ngap_handle_handover_cancel(amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_HandoverCancel_t *HandoverCancel = NULL;

    NGAP_HandoverCancelIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_ENB_UE_NGAP_ID_t *ENB_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    gnb_ue_t *source_ue = NULL;
    gnb_ue_t *target_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    HandoverCancel = &initiatingMessage->value.choice.HandoverCancel;
    ogs_assert(HandoverCancel);

    ogs_debug("[AMF] Handover cancel");
    for (i = 0; i < HandoverCancel->protocolIEs.list.count; i++) {
        ie = HandoverCancel->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_eNB_UE_NGAP_ID:
            ENB_UE_NGAP_ID = &ie->value.choice.ENB_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        default:
            break;
        }
    }
    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(AMF_UE_NGAP_ID);
    ogs_assert(ENB_UE_NGAP_ID);
    ogs_assert(Cause);

    source_ue = gnb_ue_find_by_gnb_ue_ngap_id(gnb, *ENB_UE_NGAP_ID);
    ogs_assert(source_ue);
    ogs_assert(source_ue->amf_ue_ngap_id == *AMF_UE_NGAP_ID);

    target_ue = source_ue->target_ue;
    ogs_assert(target_ue);

    ogs_debug("    Source : ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            source_ue->gnb_ue_ngap_id, source_ue->amf_ue_ngap_id);
    ogs_debug("    Target : ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            target_ue->gnb_ue_ngap_id, target_ue->amf_ue_ngap_id);

    ngap_send_handover_cancel_ack(source_ue);

    ngap_send_ue_context_release_command(
            target_ue, NGAP_Cause_PR_radioNetwork,
            NGAP_CauseRadioNetwork_handover_cancelled,
            NGAP_UE_CTX_REL_DELETE_INDIRECT_TUNNEL,
            ogs_time_from_msec(300));

    ogs_debug("[AMF] Handover Cancel : "
            "UE[eNB-UE-NGAP-ID(%d)] --> eNB[%s:%d]",
            source_ue->gnb_ue_ngap_id,
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);
}

void ngap_handle_gnb_status_transfer(amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_ENBStatusTransfer_t *ENBStatusTransfer = NULL;

    NGAP_ENBStatusTransferIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_ENB_UE_NGAP_ID_t *ENB_UE_NGAP_ID = NULL;
    NGAP_ENB_StatusTransfer_TransparentContainer_t
        *ENB_StatusTransfer_TransparentContainer = NULL;

    gnb_ue_t *source_ue = NULL, *target_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    ENBStatusTransfer = &initiatingMessage->value.choice.ENBStatusTransfer;
    ogs_assert(ENBStatusTransfer);

    ogs_debug("[AMF] ENB status transfer");
    for (i = 0; i < ENBStatusTransfer->protocolIEs.list.count; i++) {
        ie = ENBStatusTransfer->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_eNB_UE_NGAP_ID:
            ENB_UE_NGAP_ID = &ie->value.choice.ENB_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_eNB_StatusTransfer_TransparentContainer:
            ENB_StatusTransfer_TransparentContainer =
                &ie->value.choice.ENB_StatusTransfer_TransparentContainer;
            break;
        default:
            break;
        }
    }
    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(AMF_UE_NGAP_ID);
    ogs_assert(ENB_UE_NGAP_ID);
    ogs_assert(ENB_StatusTransfer_TransparentContainer);

    source_ue = gnb_ue_find_by_gnb_ue_ngap_id(gnb, *ENB_UE_NGAP_ID);
    ogs_assert(source_ue);
    ogs_assert(source_ue->amf_ue_ngap_id == *AMF_UE_NGAP_ID);

    target_ue = source_ue->target_ue;
    ogs_assert(target_ue);

    ogs_debug("    Source : ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            source_ue->gnb_ue_ngap_id, source_ue->amf_ue_ngap_id);
    ogs_debug("    Target : ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            target_ue->gnb_ue_ngap_id, target_ue->amf_ue_ngap_id);

    ngap_send_amf_status_transfer(target_ue,
            ENB_StatusTransfer_TransparentContainer);
}

void ngap_handle_handover_notification(amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_HandoverNotify_t *HandoverNotify = NULL;

    NGAP_HandoverNotifyIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_ENB_UE_NGAP_ID_t *ENB_UE_NGAP_ID = NULL;
    NGAP_EUTRAN_CGI_t *EUTRAN_CGI = NULL;
    NGAP_TAI_t *TAI = NULL;

	NGAP_PLMNidentity_t *pLMNidentity = NULL;
	NGAP_CellIdentity_t	*cell_ID = NULL;
	NGAP_TAC_t *tAC = NULL;

    gnb_ue_t *source_ue = NULL;
    gnb_ue_t *target_ue = NULL;
    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;
    amf_bearer_t *bearer = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    HandoverNotify = &initiatingMessage->value.choice.HandoverNotify;
    ogs_assert(HandoverNotify);

    ogs_debug("[AMF] Handover notification");
    for (i = 0; i < HandoverNotify->protocolIEs.list.count; i++) {
        ie = HandoverNotify->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_eNB_UE_NGAP_ID:
            ENB_UE_NGAP_ID = &ie->value.choice.ENB_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_EUTRAN_CGI:
            EUTRAN_CGI = &ie->value.choice.EUTRAN_CGI;
            break;
        case NGAP_ProtocolIE_ID_id_TAI:
            TAI = &ie->value.choice.TAI;
            break;
        default:
            break;
        }
    }
    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(EUTRAN_CGI);
    pLMNidentity = &EUTRAN_CGI->pLMNidentity;
    ogs_assert(pLMNidentity && pLMNidentity->size == sizeof(ogs_plmn_id_t));
    cell_ID = &EUTRAN_CGI->cell_ID;
    ogs_assert(cell_ID);

    ogs_assert(TAI);
    pLMNidentity = &TAI->pLMNidentity;
    ogs_assert(pLMNidentity && pLMNidentity->size == sizeof(ogs_plmn_id_t));
    tAC = &TAI->tAC;
    ogs_assert(tAC && tAC->size == sizeof(uint16_t));

    ogs_assert(ENB_UE_NGAP_ID);
    ogs_assert(AMF_UE_NGAP_ID);
    target_ue = gnb_ue_find_by_gnb_ue_ngap_id(gnb, *ENB_UE_NGAP_ID);
    ogs_assert(target_ue);
    ogs_assert(target_ue->amf_ue_ngap_id == *AMF_UE_NGAP_ID);

    source_ue = target_ue->source_ue;
    ogs_assert(source_ue);
    amf_ue = source_ue->amf_ue;
    ogs_assert(amf_ue);

    ogs_debug("    Source : ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            source_ue->gnb_ue_ngap_id, source_ue->amf_ue_ngap_id);
    ogs_debug("    Target : ENB_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            target_ue->gnb_ue_ngap_id, target_ue->amf_ue_ngap_id);

    amf_ue_associate_gnb_ue(amf_ue, target_ue);

    memcpy(&target_ue->saved.tai.plmn_id, pLMNidentity->buf, 
            sizeof(target_ue->saved.tai.plmn_id));
    memcpy(&target_ue->saved.tai.tac,
            tAC->buf, sizeof(target_ue->saved.tai.tac));
    target_ue->saved.tai.tac = ntohs(target_ue->saved.tai.tac);

    memcpy(&target_ue->saved.e_cgi.plmn_id, pLMNidentity->buf, 
            sizeof(target_ue->saved.e_cgi.plmn_id));
    memcpy(&target_ue->saved.e_cgi.cell_id, cell_ID->buf,
            sizeof(target_ue->saved.e_cgi.cell_id));
    target_ue->saved.e_cgi.cell_id =
        (ntohl(target_ue->saved.e_cgi.cell_id) >> 4);

    ogs_debug("    OLD TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&amf_ue->tai.plmn_id),
            amf_ue->tai.tac);
    ogs_debug("    OLD E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            ogs_plmn_id_hexdump(&amf_ue->e_cgi.plmn_id),
            amf_ue->e_cgi.cell_id);
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&target_ue->saved.tai.plmn_id),
            target_ue->saved.tai.tac);
    ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            ogs_plmn_id_hexdump(&target_ue->saved.e_cgi.plmn_id),
            target_ue->saved.e_cgi.cell_id);

    /* Copy TAI and ECGI from gnb_ue */
    memcpy(&amf_ue->tai, &target_ue->saved.tai, sizeof(ogs_5gs_tai_t));
    memcpy(&amf_ue->e_cgi, &target_ue->saved.e_cgi, sizeof(ogs_e_cgi_t));

    sess = amf_sess_first(amf_ue);
    while (sess) {
        bearer = amf_bearer_first(sess);
        while (bearer) {
            bearer->gnb_s1u_teid = bearer->target_s1u_teid;
            memcpy(&bearer->gnb_s1u_ip, &bearer->target_s1u_ip,
                    sizeof(ogs_ip_t));

            GTP_COUNTER_INCREMENT(
                    amf_ue, GTP_COUNTER_MODIFY_BEARER_BY_HANDOVER_NOTIFY);

            amf_gtp_send_modify_bearer_request(bearer, 1);

            bearer = amf_bearer_next(bearer);
        }
        sess = amf_sess_next(sess);
    }
}

void ngap_handle_s1_reset(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_Reset_t *Reset = NULL;

    NGAP_ResetIEs_t *ie = NULL;
    NGAP_Cause_t *Cause = NULL;
    NGAP_ResetType_t *ResetType = NULL;
    NGAP_UE_associatedLogicalS1_ConnectionListRes_t *partOfS1_Interface = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    Reset = &initiatingMessage->value.choice.Reset;
    ogs_assert(Reset);

    ogs_debug("[AMF] Reset");

    for (i = 0; i < Reset->protocolIEs.list.count; i++) {
        ie = Reset->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        case NGAP_ProtocolIE_ID_id_ResetType:
            ResetType = &ie->value.choice.ResetType;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(Cause);
    ogs_debug("    Cause[Group:%d Cause:%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

    switch (Cause->present) {
    case NGAP_Cause_PR_radioNetwork:
    case NGAP_Cause_PR_transport:
    case NGAP_Cause_PR_protocol:
    case NGAP_Cause_PR_misc:
        break;
    case NGAP_Cause_PR_nas:
        ogs_warn("NAS-Cause[%d]", (int)Cause->choice.nas);
        break;
    default:
        ogs_warn("Invalid cause group[%d]", Cause->present);
        break;
    }

    ogs_assert(ResetType);
    switch (ResetType->present) {
    case NGAP_ResetType_PR_s1_Interface:
        ogs_debug("    NGAP_ResetType_PR_s1_Interface");

        gnb_ue_remove_in_gnb(gnb);
        break;
    case NGAP_ResetType_PR_partOfS1_Interface:
        ogs_debug("    NGAP_ResetType_PR_partOfS1_Interface");

        partOfS1_Interface = ResetType->choice.partOfS1_Interface;
        ogs_assert(partOfS1_Interface);
        for (i = 0; i < partOfS1_Interface->list.count; i++) {
            NGAP_UE_associatedLogicalS1_ConnectionItemRes_t *ie2 = NULL;
            NGAP_UE_associatedLogicalS1_ConnectionItem_t *item = NULL;

            gnb_ue_t *gnb_ue = NULL;

            ie2 = (NGAP_UE_associatedLogicalS1_ConnectionItemRes_t *)
                partOfS1_Interface->list.array[i];
            ogs_assert(ie2);

            item = &ie2->value.choice.UE_associatedLogicalS1_ConnectionItem;
            ogs_assert(item);
            
            ogs_debug("    AMF_UE_NGAP_ID[%d] ENB_UE_NGAP_ID[%d]",
                    item->mME_UE_NGAP_ID ? (int)*item->mME_UE_NGAP_ID : -1,
                    item->eNB_UE_NGAP_ID ? (int)*item->eNB_UE_NGAP_ID : -1);

            if (item->mME_UE_NGAP_ID)
                gnb_ue = gnb_ue_find_by_amf_ue_ngap_id(
                        *item->mME_UE_NGAP_ID);
            else if (item->eNB_UE_NGAP_ID)
                gnb_ue = gnb_ue_find_by_gnb_ue_ngap_id(gnb,
                        *item->eNB_UE_NGAP_ID);

            if (gnb_ue == NULL) {
                ogs_warn("Cannot find S1 Context "
                    "(AMF_UE_NGAP_ID[%d] ENB_UE_NGAP_ID[%d])",
                    item->mME_UE_NGAP_ID ? (int)*item->mME_UE_NGAP_ID : -1,
                    item->eNB_UE_NGAP_ID ? (int)*item->eNB_UE_NGAP_ID : -1);
                continue;
            }

            gnb_ue_remove(gnb_ue);
        }
        break;
    default:
        ogs_warn("Invalid ResetType[%d]", ResetType->present);
        break;
    }

    ngap_send_s1_reset_ack(gnb, partOfS1_Interface);
}

void ngap_handle_write_replace_warning_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_WriteReplaceWarningResponse_t *WriteReplaceWarningResponse = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    WriteReplaceWarningResponse =
        &successfulOutcome->value.choice.WriteReplaceWarningResponse;
    ogs_assert(WriteReplaceWarningResponse);

    ogs_debug("[AMF] Write replace warning response");

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

}

void ngap_handle_kill_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_KillResponse_t *KillResponse = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    KillResponse =
        &successfulOutcome->value.choice.KillResponse;
    ogs_assert(KillResponse);

    ogs_debug("[AMF] Kill response");

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);
}
#endif
