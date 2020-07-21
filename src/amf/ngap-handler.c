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
#include "sbi-path.h"

static bool served_tai_is_found(amf_gnb_t *gnb)
{
    int i, j;
    int served_tai_index;

    for (i = 0; i < gnb->num_of_supported_ta_list; i++) {
        for (j = 0; j < gnb->supported_ta_list[i].num_of_bplmn_list; j++) {
            ogs_5gs_tai_t tai;
            memcpy(&tai.plmn_id,
                    &gnb->supported_ta_list[i].bplmn_list[j].plmn_id,
                        OGS_PLMN_ID_LEN);
            tai.tac.v = gnb->supported_ta_list[i].tac.v;
            served_tai_index = amf_find_served_tai(&tai);
            if (served_tai_index >= 0 &&
                    served_tai_index < OGS_MAX_NUM_OF_SERVED_TAI) {
                ogs_debug("    TAC[%d]", gnb->supported_ta_list[i].tac.v);
                ogs_debug("    PLMN_ID[MCC:%d MNC:%d]",
                    ogs_plmn_id_mcc(&gnb->supported_ta_list[i].
                        bplmn_list[j].plmn_id),
                    ogs_plmn_id_mnc(&gnb->supported_ta_list[i].
                        bplmn_list[j].plmn_id));
                ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);
                return true;
            }
        }
    }

    return false;
}

static bool s_nssai_is_found(amf_gnb_t *gnb)
{
    int i, j, k;
    ogs_s_nssai_t *s_nssai = NULL;

    for (i = 0; i < gnb->num_of_supported_ta_list; i++) {
        for (j = 0; j < gnb->supported_ta_list[i].num_of_bplmn_list; j++) {
            for (k = 0; k < gnb->supported_ta_list[i].
                                bplmn_list[j].num_of_s_nssai; k++) {
                s_nssai = amf_find_s_nssai(
                    &gnb->supported_ta_list[i].bplmn_list[j].plmn_id,
                    &gnb->supported_ta_list[i].bplmn_list[j].s_nssai[k]);
                if (s_nssai) {
                    ogs_debug("    PLMN_ID[MCC:%d MNC:%d]",
                        ogs_plmn_id_mcc(&gnb->supported_ta_list[i].
                            bplmn_list[j].plmn_id),
                        ogs_plmn_id_mnc(&gnb->supported_ta_list[i].
                            bplmn_list[j].plmn_id));
                    ogs_debug("    S_NSSAI[SST:%d SD:0x%x]",
                        gnb->supported_ta_list[i].bplmn_list[j].s_nssai[k].sst,
                        gnb->supported_ta_list[i].bplmn_list[j].
                                    s_nssai[k].sd.v);
                    return true;
                }
            }
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
    int i, j, k;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_NGSetupRequest_t *NGSetupRequest = NULL;

    NGAP_NGSetupRequestIEs_t *ie = NULL;
    NGAP_GlobalRANNodeID_t *GlobalRANNodeID = NULL;
    NGAP_GlobalGNB_ID_t *globalGNB_ID = NULL;
    NGAP_SupportedTAList_t *SupportedTAList = NULL;
    NGAP_PagingDRX_t *PagingDRX = NULL;

    NGAP_Cause_PR group = NGAP_Cause_PR_NOTHING;
    long cause = 0;

    uint32_t gnb_id;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    NGSetupRequest = &initiatingMessage->value.choice.NGSetupRequest;
    ogs_assert(NGSetupRequest);

    ogs_debug("NG-Setup request");

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
        ogs_error("No GlobalRANNodeID");
        group = NGAP_Cause_PR_protocol;
        cause = NGAP_CauseProtocol_semantic_error;
        ngap_send_ng_setup_failure(gnb, group, cause);
        return;
    }

    globalGNB_ID = GlobalRANNodeID->choice.globalGNB_ID;
    if (!globalGNB_ID) {
        ogs_error("No globalGNB_ID");
        group = NGAP_Cause_PR_protocol;
        cause = NGAP_CauseProtocol_semantic_error;
        ngap_send_ng_setup_failure(gnb, group, cause);
        return;
    }

    if (!SupportedTAList) {
        ogs_error("No SupportedTAList");
        group = NGAP_Cause_PR_protocol;
        cause = NGAP_CauseProtocol_semantic_error;
        ngap_send_ng_setup_failure(gnb, group, cause);
        return;
    }

    ogs_ngap_GNB_ID_to_uint32(&globalGNB_ID->gNB_ID, &gnb_id);
    ogs_debug("    IP[%s] GNB_ID[0x%x]", OGS_ADDR(gnb->addr, buf), gnb_id);

    if (PagingDRX)
        ogs_debug("    PagingDRX[%ld]", *PagingDRX);

    /* Parse Supported TA */
    for (i = 0, gnb->num_of_supported_ta_list = 0;
            i < SupportedTAList->list.count &&
            gnb->num_of_supported_ta_list < OGS_MAX_NUM_OF_TAI;
                i++) {
        NGAP_SupportedTAItem_t *SupportedTAItem = NULL;

        SupportedTAItem = (NGAP_SupportedTAItem_t *)
                SupportedTAList->list.array[i];
        if (!SupportedTAItem) {
            ogs_error("No SupportedTAItem");
            group = NGAP_Cause_PR_protocol;
            cause = NGAP_CauseProtocol_semantic_error;
            ngap_send_ng_setup_failure(gnb, group, cause);
            return;
        }

        ogs_asn_OCTET_STRING_to_uint24(&SupportedTAItem->tAC,
            &gnb->supported_ta_list[i].tac);

        ogs_debug("    TAC[%d]", gnb->supported_ta_list[i].tac.v);

        for (j = 0, gnb->supported_ta_list[i].num_of_bplmn_list = 0;
                j < SupportedTAItem->broadcastPLMNList.list.count &&
                gnb->supported_ta_list[i].num_of_bplmn_list <
                    OGS_MAX_NUM_OF_BPLMN;
                        j++) {

            NGAP_BroadcastPLMNItem_t *BroadcastPLMNItem = NULL;
            NGAP_PLMNIdentity_t *pLMNIdentity = NULL;

            BroadcastPLMNItem = (NGAP_BroadcastPLMNItem_t *)
                    SupportedTAItem->broadcastPLMNList.list.array[j];
            if (!BroadcastPLMNItem) {
                ogs_error("No BroadcastPLMNItem");
                group = NGAP_Cause_PR_protocol;
                cause = NGAP_CauseProtocol_semantic_error;
                ngap_send_ng_setup_failure(gnb, group, cause);
                return;
            }

            pLMNIdentity = (NGAP_PLMNIdentity_t *)
                    &BroadcastPLMNItem->pLMNIdentity;
            ogs_assert(pLMNIdentity);

            memcpy(&gnb->supported_ta_list[i].bplmn_list[j].plmn_id,
                    pLMNIdentity->buf, sizeof(ogs_plmn_id_t));
            ogs_debug("    PLMN_ID[MCC:%d MNC:%d]",
                ogs_plmn_id_mcc(&gnb->supported_ta_list[i].
                    bplmn_list[j].plmn_id),
                ogs_plmn_id_mnc(&gnb->supported_ta_list[i].
                    bplmn_list[j].plmn_id));

            for (k = 0, gnb->supported_ta_list[i].
                            bplmn_list[j].num_of_s_nssai = 0;
                    k < BroadcastPLMNItem->tAISliceSupportList.list.count &&
                    gnb->supported_ta_list[i].bplmn_list[j].num_of_s_nssai <
                        OGS_MAX_NUM_OF_S_NSSAI;
                            k++) {
                NGAP_SliceSupportItem_t *SliceSupportItem = NULL;
                NGAP_S_NSSAI_t *s_NSSAI = NULL;

                SliceSupportItem = (NGAP_SliceSupportItem_t *)
                        BroadcastPLMNItem->tAISliceSupportList.list.array[k];
                if (!SliceSupportItem) {
                    ogs_error("No SliceSupportItem");
                    group = NGAP_Cause_PR_protocol;
                    cause = NGAP_CauseProtocol_semantic_error;
                    ngap_send_ng_setup_failure(gnb, group, cause);
                    return;
                }

                s_NSSAI = &SliceSupportItem->s_NSSAI;
                ogs_assert(s_NSSAI);

                ogs_asn_OCTET_STRING_to_uint8(&s_NSSAI->sST,
                    &gnb->supported_ta_list[i].bplmn_list[j].s_nssai[k].sst);
                if (!s_NSSAI->sD) {
                    gnb->supported_ta_list[i].bplmn_list[j].s_nssai[k].sd.v =
                        OGS_S_NSSAI_NO_SD_VALUE;
                } else {
                    ogs_asn_OCTET_STRING_to_uint24(s_NSSAI->sD,
                    &gnb->supported_ta_list[i].bplmn_list[j].s_nssai[k].sd);
                }

                ogs_debug("    S_NSSAI[SST:%d SD:0x%x]",
                    gnb->supported_ta_list[i].bplmn_list[j].s_nssai[k].sst,
                    gnb->supported_ta_list[i].bplmn_list[j].s_nssai[k].sd.v);

                gnb->supported_ta_list[i].bplmn_list[j].num_of_s_nssai++;
            }

            gnb->supported_ta_list[i].num_of_bplmn_list++;
        }

        gnb->num_of_supported_ta_list++;
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

    if (!s_nssai_is_found(gnb)) {
        ogs_warn("NG-Setup failure:");
        ogs_warn("    Cannot find S_NSSAI. "
                    "Check 'amf.plmn.s_nssai' configuration");
        group = NGAP_Cause_PR_misc;
        cause = NGAP_CauseMisc_unknown_PLMN;

        ngap_send_ng_setup_failure(gnb, group, cause);
        return;
    }

    amf_gnb_set_gnb_id(gnb, gnb_id);

    gnb->state.ng_setup_success = true;
    ngap_send_ng_setup_response(gnb);
}

void ngap_handle_initial_ue_message(amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int i;
    char buf[OGS_ADDRSTRLEN];

    ran_ue_t *ran_ue = NULL;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_InitialUEMessage_t *InitialUEMessage = NULL;

    NGAP_InitialUEMessage_IEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;
    NGAP_UserLocationInformation_t *UserLocationInformation = NULL;
    NGAP_FiveG_S_TMSI_t *FiveG_S_TMSI = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    InitialUEMessage = &initiatingMessage->value.choice.InitialUEMessage;
    ogs_assert(InitialUEMessage);

    ogs_debug("Initial UE Message");

    for (i = 0; i < InitialUEMessage->protocolIEs.list.count; i++) {
        ie = InitialUEMessage->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_NAS_PDU:
            NAS_PDU = &ie->value.choice.NAS_PDU;
            break;
        case NGAP_ProtocolIE_ID_id_UserLocationInformation:
            UserLocationInformation = &ie->value.choice.UserLocationInformation;
            break;
        case NGAP_ProtocolIE_ID_id_FiveG_S_TMSI:
            FiveG_S_TMSI = &ie->value.choice.FiveG_S_TMSI;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]", OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    if (!RAN_UE_NGAP_ID) {
        ogs_error("No RAN_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_ran_ue_ngap_id(gnb, *RAN_UE_NGAP_ID);
    if (!ran_ue) {
        ran_ue = ran_ue_add(gnb, *RAN_UE_NGAP_ID);
        ogs_assert(ran_ue);

        /* Find AMF_UE if 5G-S_TMSI included */
        if (FiveG_S_TMSI) {
            ogs_nas_5gs_guti_t nas_guti;
            amf_ue_t *amf_ue = NULL;
            uint8_t region;
            uint16_t set;
            uint8_t pointer;
            uint32_t m_tmsi;

            memset(&nas_guti, 0, sizeof(ogs_nas_5gs_guti_t));

            /* Use the first configured plmn_id and mme group id */
            ogs_nas_from_plmn_id(&nas_guti.nas_plmn_id,
                    &amf_self()->served_guami[0].plmn_id);
            region = amf_self()->served_guami[0].amf_id.region;

            /* Getting from 5G-S_TMSI */
            ogs_ngap_AMFSetID_to_uint16(&FiveG_S_TMSI->aMFSetID, &set);
            ogs_ngap_AMFPointer_to_uint8(&FiveG_S_TMSI->aMFPointer, &pointer);

            ogs_amf_id_build(&nas_guti.amf_id, region, set, pointer);

            /* size must be 4 */
            ogs_asn_OCTET_STRING_to_uint32(&FiveG_S_TMSI->fiveG_TMSI, &m_tmsi);
            nas_guti.m_tmsi = m_tmsi;

            amf_ue = amf_ue_find_by_guti(&nas_guti);
            if (!amf_ue) {
                ogs_debug("Unknown UE by 5G-S_TMSI[AMF_ID:0x%x,M_TMSI:0x%x]",
                    ogs_amf_id_hexdump(&nas_guti.amf_id), nas_guti.m_tmsi);
            } else {
                ogs_debug("[%s]    5G-S_TMSI[AMF_ID:0x%x,M_TMSI:0x%x]",
                        AMF_UE_HAVE_SUCI(amf_ue) ? amf_ue->suci : "Unknown ID",
                        ogs_amf_id_hexdump(&amf_ue->guti.amf_id),
                        amf_ue->guti.m_tmsi);
                /* If NAS(amf_ue_t) has already been associated with
                 * older NG(ran_ue_t) context */
                if (CM_CONNECTED(amf_ue)) {
                   /* Implcit NG release */
                    ogs_debug("Implicit NG release");
                    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
                          amf_ue->ran_ue->ran_ue_ngap_id,
                          (long long)amf_ue->ran_ue->amf_ue_ngap_id);
                    ran_ue_remove(amf_ue->ran_ue);
                }
                amf_ue_associate_ran_ue(amf_ue, ran_ue);
            }
        }
    }

    if (!UserLocationInformation) {
        ogs_error("No UserLocationInformation");
        ngap_send_error_indication(gnb, &ran_ue->ran_ue_ngap_id, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (!NAS_PDU) {
        ogs_error("No NAS_PDU");
        ngap_send_error_indication(gnb, &ran_ue->ran_ue_ngap_id, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (UserLocationInformation->present ==
            NGAP_UserLocationInformation_PR_userLocationInformationNR) {
        NGAP_UserLocationInformationNR_t *userLocationInformationNR =
            UserLocationInformation->choice.userLocationInformationNR;

        ogs_ngap_ASN_to_nr_cgi(
                &userLocationInformationNR->nR_CGI, &ran_ue->saved.nr_cgi);
        ogs_ngap_ASN_to_5gs_tai(
                &userLocationInformationNR->tAI, &ran_ue->saved.tai);

    } else
        ogs_error("Not implemented UserLocationInformation[%d]",
                UserLocationInformation->present);

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld] "
            "TAC[%d] CellID[0x%llx]",
        ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id,
        ran_ue->saved.tai.tac.v, (long long)ran_ue->saved.nr_cgi.cell_id);

    ngap_send_to_nas(ran_ue,
            NGAP_ProcedureCode_id_InitialUEMessage, NAS_PDU);
}

void ngap_handle_uplink_nas_transport(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    ran_ue_t *ran_ue = NULL;
    uint64_t amf_ue_ngap_id;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UplinkNASTransport_t *UplinkNASTransport = NULL;

    NGAP_UplinkNASTransport_IEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    UplinkNASTransport = &initiatingMessage->value.choice.UplinkNASTransport;
    ogs_assert(UplinkNASTransport);

    ogs_debug("Uplink NAS transport");

    for (i = 0; i < UplinkNASTransport->protocolIEs.list.count; i++) {
        ie = UplinkNASTransport->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_NAS_PDU:
            NAS_PDU = &ie->value.choice.NAS_PDU;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]", OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(gnb, NULL, &amf_ue_ngap_id, 
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        return;
    }

    if (!NAS_PDU) {
        ogs_error("No NAS_PDU");
        ngap_send_error_indication(
            gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
            NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ngap_send_to_nas(ran_ue,
        NGAP_ProcedureCode_id_UplinkNASTransport, NAS_PDU);
}

void ngap_handle_ue_radio_capability_info_indication(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    ran_ue_t *ran_ue = NULL;
    uint64_t amf_ue_ngap_id;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UERadioCapabilityInfoIndication_t
        *UERadioCapabilityInfoIndication = NULL;

    NGAP_UERadioCapabilityInfoIndicationIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_UERadioCapability_t *UERadioCapability = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    UERadioCapabilityInfoIndication =
        &initiatingMessage->value.choice.UERadioCapabilityInfoIndication;
    ogs_assert(UERadioCapabilityInfoIndication);

    ogs_debug("UE radio capability info indication");

    for (i = 0;
            i < UERadioCapabilityInfoIndication->protocolIEs.list.count; i++) {
        ie = UERadioCapabilityInfoIndication->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_UERadioCapability:
            UERadioCapability = &ie->value.choice.UERadioCapability;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]", OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        return;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    if (!UERadioCapability) {
        ogs_error("No UERadioCapability");
        ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (ran_ue->amf_ue)
        OGS_ASN_STORE_DATA(&ran_ue->amf_ue->ueRadioCapability,
                UERadioCapability);
}

void ngap_handle_initial_context_setup_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    uint64_t amf_ue_ngap_id;
    amf_nsmf_pdu_session_update_sm_context_param_t param;

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_InitialContextSetupResponse_t *InitialContextSetupResponse = NULL;

    NGAP_InitialContextSetupResponseIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceSetupListCxtRes_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceSetupItemCxtRes_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    InitialContextSetupResponse =
        &successfulOutcome->value.choice.InitialContextSetupResponse;
    ogs_assert(InitialContextSetupResponse);

    ogs_debug("Initial context setup response");

    for (i = 0; i < InitialContextSetupResponse->protocolIEs.list.count; i++) {
        ie = InitialContextSetupResponse->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtRes:
            PDUSessionList =
                &ie->value.choice.PDUSessionResourceSetupListCxtRes;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]", OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        return;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    if (!PDUSessionList) /* No PDUSessionList */
        return;

    amf_ue = ran_ue->amf_ue;
    ogs_assert(amf_ue);

    for (i = 0; i < PDUSessionList->list.count; i++) {
        amf_sess_t *sess = NULL;
        PDUSessionItem = (NGAP_PDUSessionResourceSetupItemCxtRes_t *)
            PDUSessionList->list.array[i];

        if (!PDUSessionItem) {
            ogs_error("No PDUSessionResourceSetupItemCxtRes");
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return;
        }

        transfer = &PDUSessionItem->pDUSessionResourceSetupResponseTransfer;
        if (!transfer) {
            ogs_error("No PDUSessionResourceSetupResponseTransfer");
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return;
        }

        if (PDUSessionItem->pDUSessionID ==
                OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
            ogs_error("PDU Session Identity is unassigned");
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return;
        }

        sess = amf_sess_find_by_psi(amf_ue, PDUSessionItem->pDUSessionID);
        if (!sess) {
            ogs_error("Cannot find PDU Session ID [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            return;
        }

        if (!SESSION_CONTEXT_IN_SMF(sess)) {
            ogs_error("Session Context is not in SMF [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            return;
        }

        /* UPDATE_UpCnxState - ACTIVATED */
        sess->ueUpCnxState = OpenAPI_up_cnx_state_ACTIVATED;

        memset(&param, 0, sizeof(param));
        param.n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
        param.n2SmInfoType = OpenAPI_n2_sm_info_type_PDU_RES_SETUP_RSP;
        ogs_pkbuf_put_data(param.n2smbuf, transfer->buf, transfer->size);

        amf_sess_sbi_discover_and_send(
                OpenAPI_nf_type_SMF, sess, &param,
                amf_nsmf_pdu_session_build_update_sm_context);

        ogs_pkbuf_free(param.n2smbuf);
    }
}

void ngap_handle_initial_context_setup_failure(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    ran_ue_t *ran_ue = NULL;
    uint64_t amf_ue_ngap_id;

    NGAP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    NGAP_InitialContextSetupFailure_t *InitialContextSetupFailure = NULL;

    NGAP_InitialContextSetupFailureIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    unsuccessfulOutcome = message->choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    InitialContextSetupFailure =
        &unsuccessfulOutcome->value.choice.InitialContextSetupFailure;
    ogs_assert(InitialContextSetupFailure);

    ogs_debug("Initial context setup failure");

    for (i = 0; i < InitialContextSetupFailure->protocolIEs.list.count; i++) {
        ie = InitialContextSetupFailure->protocolIEs.list.array[i];
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

    ogs_debug("    IP[%s] RAN_ID[%d]", OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        return;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);


    if (!Cause) {
        ogs_error("No Cause");
        ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }
    ogs_debug("    Cause[Group:%d Cause:%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

    /*
     * 19.2.2.3 in Spec 36.300
     *
     * In case of failure, RAN and AMF behaviours are not mandated.
     *
     * Both implicit release (local release at each node) and
     * explicit release (AMF-initiated UE Context Release procedure)
     * may in principle be adopted. The RAN should ensure
     * that no hanging resources remain at the RAN.
     */
    ngap_send_session_sync_or_context_release_command(ran_ue,
        NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
        NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE, 0);
}

#if 0
void ngap_handle_ue_context_modification_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_UEContextModificationResponse_t *UEContextModificationResponse = NULL;

    NGAP_UEContextModificationResponseIEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    UEContextModificationResponse =
        &successfulOutcome->value.choice.UEContextModificationResponse;
    ogs_assert(UEContextModificationResponse);

    ogs_debug("UE context modification response");

    for (i = 0; i < UEContextModificationResponse->protocolIEs.list.count; i++) {
        ie = UEContextModificationResponse->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]", OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(RAN_UE_NGAP_ID);
    ran_ue = ran_ue_find_by_ran_ue_ngap_id(gnb, *RAN_UE_NGAP_ID);
    ogs_assert(ran_ue);
    amf_ue = ran_ue->amf_ue;
    ogs_assert(amf_ue);

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            ran_ue->ran_ue_ngap_id, ran_ue->amf_ue_ngap_id);

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
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    ran_ue_t *ran_ue = NULL;
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
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]", OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(RAN_UE_NGAP_ID);
    ogs_assert(Cause);

    ran_ue = ran_ue_find_by_ran_ue_ngap_id(gnb, *RAN_UE_NGAP_ID);
    if (ran_ue == NULL) {
        ogs_warn("Initial context setup failure : "
                "cannot find RAN-UE-NGAP-ID[%d]", (int)*RAN_UE_NGAP_ID);
        goto cleanup;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            ran_ue->ran_ue_ngap_id, ran_ue->amf_ue_ngap_id);
    ogs_debug("    Cause[Group:%d Cause:%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

cleanup:
    amf_ue = ran_ue->amf_ue;
    ogs_assert(amf_ue);
    CLEAR_SERVICE_INDICATOR(amf_ue);
}
#endif

void ngap_handle_ue_context_release_request(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int i;
    char buf[OGS_ADDRSTRLEN];
    uint64_t amf_ue_ngap_id;

    ran_ue_t *ran_ue = NULL;
    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UEContextReleaseRequest_t *UEContextReleaseRequest = NULL;

    NGAP_UEContextReleaseRequest_IEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceListCxtRelReq_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceItemCxtRelReq_t *PDUSessionItem = NULL;
    NGAP_Cause_t *Cause = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    UEContextReleaseRequest =
        &initiatingMessage->value.choice.UEContextReleaseRequest;
    ogs_assert(UEContextReleaseRequest);

    ogs_debug("UE Context release request");

    for (i = 0; i < UEContextReleaseRequest->protocolIEs.list.count; i++) {
        ie = UEContextReleaseRequest->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_PDUSessionResourceListCxtRelReq:
            PDUSessionList = &ie->value.choice.PDUSessionResourceListCxtRelReq;
            break;
        case NGAP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]", OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_warn("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(gnb, NULL, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        return;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    if (!Cause) {
        ogs_error("No Cause");
        ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

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

    amf_ue = ran_ue->amf_ue;
    if (!amf_ue) {
        ogs_debug("No UE Context");
        ngap_send_ran_ue_context_release_command(ran_ue,
                NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
                NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE, 0);
    } else {
        if (!PDUSessionList) {
            amf_sbi_send_deactivate_all_sessions(
                    amf_ue, Cause->present, (int)Cause->choice.radioNetwork);
        } else {
            for (i = 0; i < PDUSessionList->list.count; i++) {
                PDUSessionItem = (NGAP_PDUSessionResourceItemCxtRelReq_t *)
                    PDUSessionList->list.array[i];

                if (!PDUSessionItem) {
                    ogs_error("No PDUSessionResourceSetupItemSURes");
                    ngap_send_error_indication2(
                            amf_ue, NGAP_Cause_PR_protocol,
                            NGAP_CauseProtocol_semantic_error);
                    return;
                }

                if (PDUSessionItem->pDUSessionID ==
                        OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
                    ogs_error("PDU Session Identity is unassigned");
                    ngap_send_error_indication2(
                            amf_ue, NGAP_Cause_PR_protocol,
                            NGAP_CauseProtocol_semantic_error);
                    return;
                }

                sess = amf_sess_find_by_psi(amf_ue,
                        PDUSessionItem->pDUSessionID);
                if (SESSION_CONTEXT_IN_SMF(sess)) {
                    amf_sbi_send_deactivate_session(
                            sess, Cause->present,
                            (int)Cause->choice.radioNetwork);
                }
            }
        }

        if (SESSION_SYNC_DONE(amf_ue))
            ngap_send_amf_ue_context_release_command(amf_ue,
                    NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
                    NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE, 0);
    }
}

void ngap_handle_ue_context_release_complete(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int i;
    char buf[OGS_ADDRSTRLEN];
    uint64_t amf_ue_ngap_id;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_UEContextReleaseComplete_t *UEContextReleaseComplete = NULL;

    NGAP_UEContextReleaseComplete_IEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    UEContextReleaseComplete =
        &successfulOutcome->value.choice.UEContextReleaseComplete;
    ogs_assert(UEContextReleaseComplete);

    ogs_debug("UE Context release complete");

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

    ogs_debug("    IP[%s] RAN_ID[%d]", OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(gnb, NULL, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        return;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    amf_ue = ran_ue->amf_ue;

    switch (ran_ue->ue_ctx_rel_action) {
    case NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE:
        ogs_debug("    No Action");
        ran_ue_remove(ran_ue);
        break;
    case NGAP_UE_CTX_REL_NG_REMOVE_AND_UNLINK:
        ogs_debug("    Action: NG normal release");
        ran_ue_remove(ran_ue);
        amf_ue_deassociate(amf_ue);
        break;
    case NGAP_UE_CTX_REL_UE_CONTEXT_REMOVE:
        ogs_debug("    Action: UE context remove()");
        ran_ue_remove(ran_ue);
        amf_ue_remove(amf_ue);
        break;
    case NGAP_UE_CTX_REL_DELETE_INDIRECT_TUNNEL:
#if 0
        ogs_debug("    Action: Delete indirect tunnel");

        source_ue_deassociate_target_ue(ran_ue);
        ran_ue_remove(ran_ue);

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
#endif
        break;
    default:
        ogs_error("Invalid Action[%d]", ran_ue->ue_ctx_rel_action);
        break;
    }
}

void ngap_handle_pdu_session_resource_setup_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    uint64_t amf_ue_ngap_id;
    amf_nsmf_pdu_session_update_sm_context_param_t param;

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_PDUSessionResourceSetupResponse_t *PDUSessionResourceSetupResponse;

    NGAP_PDUSessionResourceSetupResponseIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceSetupListSURes_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceSetupItemSURes_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    PDUSessionResourceSetupResponse =
        &successfulOutcome->value.choice.PDUSessionResourceSetupResponse;
    ogs_assert(PDUSessionResourceSetupResponse);

    ogs_debug("PDU session resource setup response");

    for (i = 0; i < PDUSessionResourceSetupResponse->protocolIEs.list.count;
            i++) {
        ie = PDUSessionResourceSetupResponse->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListSURes:
            PDUSessionList = &ie->value.choice.PDUSessionResourceSetupListSURes;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]", OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        return;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    amf_ue = ran_ue->amf_ue;
    if (!amf_ue) {
        ogs_error("Cannot find AMF-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        return;
    }

    if (!PDUSessionList) {
        ogs_error("No PDUSessionResourceSetupListSURes");
        ngap_send_error_indication2(amf_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    for (i = 0; i < PDUSessionList->list.count; i++) {
        amf_sess_t *sess = NULL;
        PDUSessionItem = (NGAP_PDUSessionResourceSetupItemSURes_t *)
            PDUSessionList->list.array[i];

        if (!PDUSessionItem) {
            ogs_error("No PDUSessionResourceSetupItemSURes");
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return;
        }

        transfer = &PDUSessionItem->pDUSessionResourceSetupResponseTransfer;
        if (!transfer) {
            ogs_error("No PDUSessionResourceSetupResponseTransfer");
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return;
        }

        if (PDUSessionItem->pDUSessionID ==
                OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
            ogs_error("PDU Session Identity is unassigned");
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return;
        }

        sess = amf_sess_find_by_psi(amf_ue, PDUSessionItem->pDUSessionID);
        if (!sess) {
            ogs_error("Cannot find PDU Session ID [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            return;
        }

        if (!SESSION_CONTEXT_IN_SMF(sess)) {
            ogs_error("Session Context is not in SMF [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            return;
        }

        /* UPDATE_UpCnxState - ACTIVATED */
        sess->ueUpCnxState = OpenAPI_up_cnx_state_ACTIVATED;

        memset(&param, 0, sizeof(param));
        param.n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
        param.n2SmInfoType = OpenAPI_n2_sm_info_type_PDU_RES_SETUP_RSP;
        ogs_pkbuf_put_data(param.n2smbuf, transfer->buf, transfer->size);

        amf_sess_sbi_discover_and_send(
                OpenAPI_nf_type_SMF, sess, &param,
                amf_nsmf_pdu_session_build_update_sm_context);

        ogs_pkbuf_free(param.n2smbuf);
    }
}

void ngap_handle_pdu_session_resource_release_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    uint64_t amf_ue_ngap_id;
    amf_nsmf_pdu_session_update_sm_context_param_t param;

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_PDUSessionResourceReleaseResponse_t *PDUSessionResourceReleaseResponse;

    NGAP_PDUSessionResourceReleaseResponseIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceReleasedListRelRes_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceReleasedItemRelRes_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    PDUSessionResourceReleaseResponse =
        &successfulOutcome->value.choice.PDUSessionResourceReleaseResponse;
    ogs_assert(PDUSessionResourceReleaseResponse);

    ogs_debug("PDU session resource release response");

    for (i = 0; i < PDUSessionResourceReleaseResponse->protocolIEs.list.count;
            i++) {
        ie = PDUSessionResourceReleaseResponse->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_PDUSessionResourceReleasedListRelRes:
            PDUSessionList =
                &ie->value.choice.PDUSessionResourceReleasedListRelRes;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]", OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        return;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    amf_ue = ran_ue->amf_ue;
    if (!amf_ue) {
        ogs_error("Cannot find AMF-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        return;
    }

    if (!PDUSessionList) {
        ogs_error("No PDUSessionResourceReleasedListRelRes");
        ngap_send_error_indication2(amf_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    for (i = 0; i < PDUSessionList->list.count; i++) {
        amf_sess_t *sess = NULL;
        PDUSessionItem = (NGAP_PDUSessionResourceReleasedItemRelRes_t *)
            PDUSessionList->list.array[i];

        if (!PDUSessionItem) {
            ogs_error("No PDUSessionResourceReleasedItemRelRes");
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return;
        }

        transfer = &PDUSessionItem->pDUSessionResourceReleaseResponseTransfer;
        if (!transfer) {
            ogs_error("No PDUSessionResourceReleaseResponseTransfer");
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return;
        }

        if (PDUSessionItem->pDUSessionID ==
                OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
            ogs_error("PDU Session Identity is unassigned");
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return;
        }

        sess = amf_sess_find_by_psi(amf_ue, PDUSessionItem->pDUSessionID);
        if (!sess) {
            ogs_error("Cannot find PDU Session ID [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            return;
        }

        if (!SESSION_CONTEXT_IN_SMF(sess)) {
            ogs_error("Session Context is not in SMF [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            return;
        }

        memset(&param, 0, sizeof(param));
        param.n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
        param.n2SmInfoType = OpenAPI_n2_sm_info_type_PDU_RES_REL_RSP;
        ogs_pkbuf_put_data(param.n2smbuf, transfer->buf, transfer->size);

        amf_sess_sbi_discover_and_send(
                OpenAPI_nf_type_SMF, sess, &param,
                amf_nsmf_pdu_session_build_update_sm_context);

        ogs_pkbuf_free(param.n2smbuf);
    }
}

#if 0
void ngap_handle_path_switch_request(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_PathSwitchRequest_t *PathSwitchRequest = NULL;

    NGAP_PathSwitchRequestIEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
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

    ran_ue_t *ran_ue = NULL;
    amf_ue_t *amf_ue = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    PathSwitchRequest = &initiatingMessage->value.choice.PathSwitchRequest;
    ogs_assert(PathSwitchRequest);

    ogs_debug("Path switch request");

    for (i = 0; i < PathSwitchRequest->protocolIEs.list.count; i++) {
        ie = PathSwitchRequest->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
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

    ogs_debug("    IP[%s] RAN_ID[%d]",
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
    ogs_assert(RAN_UE_NGAP_ID);
    ran_ue = ran_ue_find_by_amf_ue_ngap_id(*AMF_UE_NGAP_ID);
    if (!ran_ue) {
        ogs_error("Cannot find UE from sourceAMF-UE-NGAP-ID[%d] and RAN[%s:%d]",
                (int)*AMF_UE_NGAP_ID, OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

        ngapbuf = ngap_build_path_switch_failure(
                *RAN_UE_NGAP_ID, *AMF_UE_NGAP_ID,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_amf_ue_ngap_id);
        ogs_expect_or_return(ngapbuf);

        ogs_expect(OGS_OK ==
                ngap_send_to_gnb(gnb, ngapbuf, NGAP_NON_UE_SIGNALLING));
        return;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            ran_ue->ran_ue_ngap_id, ran_ue->amf_ue_ngap_id);

    amf_ue = ran_ue->amf_ue;
    ogs_assert(amf_ue);

    if (SECURITY_CONTEXT_IS_VALID(amf_ue)) {
        amf_ue->nhcc++;
        amf_kdf_nh_gnb(amf_ue->kamf, amf_ue->nh, amf_ue->nh);
    } else {
        ngapbuf = ngap_build_path_switch_failure(
                *RAN_UE_NGAP_ID, *AMF_UE_NGAP_ID,
                NGAP_Cause_PR_nas, NGAP_CauseNas_authentication_failure);
        ogs_expect_or_return(ngapbuf);

        ngap_send_to_ran_ue(ran_ue, ngapbuf);
        return;
    }

    ran_ue->ran_ue_ngap_id = *RAN_UE_NGAP_ID;

    memcpy(&ran_ue->saved.tai.plmn_id, pLMNidentity->buf, 
            sizeof(ran_ue->saved.tai.plmn_id));
    memcpy(&ran_ue->saved.tai.tac, tAC->buf, sizeof(ran_ue->saved.tai.tac));
    ran_ue->saved.tai.tac = ntohs(ran_ue->saved.tai.tac);

    memcpy(&ran_ue->saved.e_cgi.plmn_id, pLMNidentity->buf, 
            sizeof(ran_ue->saved.e_cgi.plmn_id));
    memcpy(&ran_ue->saved.e_cgi.cell_id, cell_ID->buf,
            sizeof(ran_ue->saved.e_cgi.cell_id));
    ran_ue->saved.e_cgi.cell_id = (ntohl(ran_ue->saved.e_cgi.cell_id) >> 4);

    ogs_debug("    OLD TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&amf_ue->tai.plmn_id),
            amf_ue->tai.tac);
    ogs_debug("    OLD E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            ogs_plmn_id_hexdump(&amf_ue->e_cgi.plmn_id),
            amf_ue->e_cgi.cell_id);
    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&ran_ue->saved.tai.plmn_id),
            ran_ue->saved.tai.tac);
    ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            ogs_plmn_id_hexdump(&ran_ue->saved.e_cgi.plmn_id),
            ran_ue->saved.e_cgi.cell_id);

    /* Copy TAI and ECGI from ran_ue */
    memcpy(&amf_ue->tai, &ran_ue->saved.tai, sizeof(ogs_5gs_tai_t));
    memcpy(&amf_ue->e_cgi, &ran_ue->saved.e_cgi, sizeof(ogs_e_cgi_t));

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
    ran_ue_switch_to_gnb(ran_ue, gnb);
}

void ngap_handle_gnb_configuration_transfer(
        amf_gnb_t *gnb, ogs_ngap_message_t *message, ogs_pkbuf_t *pkbuf)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_RANConfigurationTransfer_t *RANConfigurationTransfer = NULL;

    NGAP_RANConfigurationTransferIEs_t *ie = NULL;
    NGAP_SONConfigurationTransfer_t *SONConfigurationTransfer = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    RANConfigurationTransfer =
        &initiatingMessage->value.choice.RANConfigurationTransfer;
    ogs_assert(RANConfigurationTransfer);

    ogs_debug("RAN configuration transfer");
    for (i = 0; i < RANConfigurationTransfer->protocolIEs.list.count; i++) {
        ie = RANConfigurationTransfer->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_SONConfigurationTransferECT:
            SONConfigurationTransfer =
                &ie->value.choice.SONConfigurationTransfer;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    if (SONConfigurationTransfer) {
        NGAP_TargetRAN_ID_t *targetRAN_ID =
            &SONConfigurationTransfer->targetRAN_ID;
        NGAP_SourceRAN_ID_t *sourceRAN_ID =
            &SONConfigurationTransfer->sourceRAN_ID;

        amf_gnb_t *target_gnb = NULL;
        uint32_t source_gnb_id, target_gnb_id;
        uint16_t source_tac, target_tac;

        ogs_ngap_RAN_ID_to_uint32(
                &sourceRAN_ID->global_RAN_ID.RAN_ID, &source_gnb_id);
        ogs_ngap_RAN_ID_to_uint32(
                &targetRAN_ID->global_RAN_ID.RAN_ID, &target_gnb_id);

        memcpy(&source_tac, sourceRAN_ID->selected_TAI.tAC.buf,
                sizeof(source_tac));
        source_tac = ntohs(source_tac);
        memcpy(&target_tac, targetRAN_ID->selected_TAI.tAC.buf,
                sizeof(target_tac));
        target_tac = ntohs(target_tac);

        ogs_debug("    Source : RAN_ID[%s:%d], TAC[%d]",
                sourceRAN_ID->global_RAN_ID.RAN_ID.present ==
                    NGAP_RAN_ID_PR_homeRAN_ID ? "Home" : 
                sourceRAN_ID->global_RAN_ID.RAN_ID.present ==
                    NGAP_RAN_ID_PR_macroRAN_ID ? "Macro" : "Others",
                source_gnb_id, source_tac);
        ogs_debug("    Target : RAN_ID[%s:%d], TAC[%d]",
                targetRAN_ID->global_RAN_ID.RAN_ID.present ==
                    NGAP_RAN_ID_PR_homeRAN_ID ? "Home" : 
                targetRAN_ID->global_RAN_ID.RAN_ID.present ==
                    NGAP_RAN_ID_PR_macroRAN_ID ? "Macro" : "Others",
                target_gnb_id, target_tac);

        target_gnb = amf_gnb_find_by_gnb_id(target_gnb_id);
        if (target_gnb == NULL) {
            ogs_warn("RAN configuration transfer : "
                    "cannot find target RAN-id[%d]",
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
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
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

    ran_ue_t *source_ue = NULL;
    amf_ue_t *amf_ue = NULL;
    amf_gnb_t *target_gnb = NULL;
    uint32_t target_gnb_id = 0;

    ogs_debug("Handover required");
    for (i = 0; i < HandoverRequired->protocolIEs.list.count; i++) {
        ie = HandoverRequired->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
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

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(TargetID);
    switch (TargetID->present) {
    case NGAP_TargetID_PR_targetRAN_ID:
        ogs_ngap_RAN_ID_to_uint32(
            &TargetID->choice.targetRAN_ID->global_RAN_ID.RAN_ID,
            &target_gnb_id);
        break;
    default:
        ogs_error("Not implemented(%d)", TargetID->present);
        return;
    }

    target_gnb = amf_gnb_find_by_gnb_id(target_gnb_id);
    if (target_gnb == NULL) {
        ogs_warn("Handover required : cannot find target RAN-id[%d]",
                target_gnb_id);
        return;
    }

    ogs_assert(RAN_UE_NGAP_ID);
    ogs_assert(AMF_UE_NGAP_ID);
    source_ue = ran_ue_find_by_ran_ue_ngap_id(gnb, *RAN_UE_NGAP_ID);
    ogs_assert(source_ue);
    ogs_assert(source_ue->amf_ue_ngap_id == *AMF_UE_NGAP_ID);

    ogs_debug("    Source : RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            source_ue->ran_ue_ngap_id, source_ue->amf_ue_ngap_id);

    amf_ue = source_ue->amf_ue;
    ogs_assert(amf_ue);

    if (SECURITY_CONTEXT_IS_VALID(amf_ue)) {
        amf_ue->nhcc++;
        amf_kdf_nh_gnb(amf_ue->kamf, amf_ue->nh, amf_ue->nh);
    } else {
        ogs_assert(Cause);

        ngap_send_handover_preparation_failure(source_ue, Cause);

        return;
    }

    ogs_assert(HandoverType);
    source_ue->handover_type = *HandoverType;

    ngap_send_handover_request(amf_ue, target_gnb,
            RAN_UE_NGAP_ID, AMF_UE_NGAP_ID,
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
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_E_RABAdmittedList_t *E_RABAdmittedList = NULL;
    NGAP_Target_ToSource_TransparentContainer_t
        *Target_ToSource_TransparentContainer = NULL;

    ran_ue_t *source_ue = NULL;
    ran_ue_t *target_ue = NULL;
    amf_ue_t *amf_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    HandoverRequestAcknowledge =
        &successfulOutcome->value.choice.HandoverRequestAcknowledge;
    ogs_assert(HandoverRequestAcknowledge);

    ogs_debug("Handover request acknowledge");
    for (i = 0; i < HandoverRequestAcknowledge->protocolIEs.list.count; i++) {
        ie = HandoverRequestAcknowledge->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
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
    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(AMF_UE_NGAP_ID);
    ogs_assert(RAN_UE_NGAP_ID);
    ogs_assert(E_RABAdmittedList);
    ogs_assert(Target_ToSource_TransparentContainer);

    target_ue = ran_ue_find_by_amf_ue_ngap_id(*AMF_UE_NGAP_ID);
    ogs_assert(target_ue);

    target_ue->ran_ue_ngap_id = *RAN_UE_NGAP_ID;

    source_ue = target_ue->source_ue;
    ogs_assert(source_ue);
    amf_ue = source_ue->amf_ue;
    ogs_assert(amf_ue);

    ogs_debug("    Source : RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            source_ue->ran_ue_ngap_id, source_ue->amf_ue_ngap_id);
    ogs_debug("    Target : RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            target_ue->ran_ue_ngap_id, target_ue->amf_ue_ngap_id);

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

    OGS_ASN_STORE_DATA(&amf_ue->container,
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

    ran_ue_t *target_ue = NULL;
    ran_ue_t *source_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    unsuccessfulOutcome = message->choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    HandoverFailure = &unsuccessfulOutcome->value.choice.HandoverFailure;
    ogs_assert(HandoverFailure);

    ogs_debug("Handover failure");
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

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(AMF_UE_NGAP_ID);
    ogs_assert(Cause);

    target_ue = ran_ue_find_by_amf_ue_ngap_id(*AMF_UE_NGAP_ID);
    ogs_assert(target_ue);

    source_ue = target_ue->source_ue;
    ogs_assert(source_ue);

    ogs_debug("    Source : RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            source_ue->ran_ue_ngap_id, source_ue->amf_ue_ngap_id);
    ogs_debug("    Target : RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            target_ue->ran_ue_ngap_id, target_ue->amf_ue_ngap_id);

    ngap_send_handover_preparation_failure(source_ue, Cause);

    ngap_send_ue_context_release_command(
        target_ue, NGAP_Cause_PR_radioNetwork,
        NGAP_CauseRadioNetwork_ho_failure_in_target_EPC_RAN_or_target_system,
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
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    ran_ue_t *source_ue = NULL;
    ran_ue_t *target_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    HandoverCancel = &initiatingMessage->value.choice.HandoverCancel;
    ogs_assert(HandoverCancel);

    ogs_debug("Handover cancel");
    for (i = 0; i < HandoverCancel->protocolIEs.list.count; i++) {
        ie = HandoverCancel->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_Cause:
            Cause = &ie->value.choice.Cause;
            break;
        default:
            break;
        }
    }
    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(AMF_UE_NGAP_ID);
    ogs_assert(RAN_UE_NGAP_ID);
    ogs_assert(Cause);

    source_ue = ran_ue_find_by_ran_ue_ngap_id(gnb, *RAN_UE_NGAP_ID);
    ogs_assert(source_ue);
    ogs_assert(source_ue->amf_ue_ngap_id == *AMF_UE_NGAP_ID);

    target_ue = source_ue->target_ue;
    ogs_assert(target_ue);

    ogs_debug("    Source : RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            source_ue->ran_ue_ngap_id, source_ue->amf_ue_ngap_id);
    ogs_debug("    Target : RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            target_ue->ran_ue_ngap_id, target_ue->amf_ue_ngap_id);

    ngap_send_handover_cancel_ack(source_ue);

    ngap_send_ue_context_release_command(
            target_ue, NGAP_Cause_PR_radioNetwork,
            NGAP_CauseRadioNetwork_handover_cancelled,
            NGAP_UE_CTX_REL_DELETE_INDIRECT_TUNNEL,
            ogs_time_from_msec(300));

    ogs_debug("Handover Cancel : "
            "UE[RAN-UE-NGAP-ID(%d)] --> RAN[%s:%d]",
            source_ue->ran_ue_ngap_id,
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);
}

void ngap_handle_gnb_status_transfer(amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_RANStatusTransfer_t *RANStatusTransfer = NULL;

    NGAP_RANStatusTransferIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_RAN_StatusTransfer_TransparentContainer_t
        *RAN_StatusTransfer_TransparentContainer = NULL;

    ran_ue_t *source_ue = NULL, *target_ue = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    RANStatusTransfer = &initiatingMessage->value.choice.RANStatusTransfer;
    ogs_assert(RANStatusTransfer);

    ogs_debug("RAN status transfer");
    for (i = 0; i < RANStatusTransfer->protocolIEs.list.count; i++) {
        ie = RANStatusTransfer->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_RAN_StatusTransfer_TransparentContainer:
            RAN_StatusTransfer_TransparentContainer =
                &ie->value.choice.RAN_StatusTransfer_TransparentContainer;
            break;
        default:
            break;
        }
    }
    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->addr, buf), gnb->gnb_id);

    ogs_assert(AMF_UE_NGAP_ID);
    ogs_assert(RAN_UE_NGAP_ID);
    ogs_assert(RAN_StatusTransfer_TransparentContainer);

    source_ue = ran_ue_find_by_ran_ue_ngap_id(gnb, *RAN_UE_NGAP_ID);
    ogs_assert(source_ue);
    ogs_assert(source_ue->amf_ue_ngap_id == *AMF_UE_NGAP_ID);

    target_ue = source_ue->target_ue;
    ogs_assert(target_ue);

    ogs_debug("    Source : RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            source_ue->ran_ue_ngap_id, source_ue->amf_ue_ngap_id);
    ogs_debug("    Target : RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            target_ue->ran_ue_ngap_id, target_ue->amf_ue_ngap_id);

    ngap_send_amf_status_transfer(target_ue,
            RAN_StatusTransfer_TransparentContainer);
}

void ngap_handle_handover_notification(amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_HandoverNotify_t *HandoverNotify = NULL;

    NGAP_HandoverNotifyIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_EUTRAN_CGI_t *EUTRAN_CGI = NULL;
    NGAP_TAI_t *TAI = NULL;

	NGAP_PLMNidentity_t *pLMNidentity = NULL;
	NGAP_CellIdentity_t	*cell_ID = NULL;
	NGAP_TAC_t *tAC = NULL;

    ran_ue_t *source_ue = NULL;
    ran_ue_t *target_ue = NULL;
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

    ogs_debug("Handover notification");
    for (i = 0; i < HandoverNotify->protocolIEs.list.count; i++) {
        ie = HandoverNotify->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
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
    ogs_debug("    IP[%s] RAN_ID[%d]",
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

    ogs_assert(RAN_UE_NGAP_ID);
    ogs_assert(AMF_UE_NGAP_ID);
    target_ue = ran_ue_find_by_ran_ue_ngap_id(gnb, *RAN_UE_NGAP_ID);
    ogs_assert(target_ue);
    ogs_assert(target_ue->amf_ue_ngap_id == *AMF_UE_NGAP_ID);

    source_ue = target_ue->source_ue;
    ogs_assert(source_ue);
    amf_ue = source_ue->amf_ue;
    ogs_assert(amf_ue);

    ogs_debug("    Source : RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            source_ue->ran_ue_ngap_id, source_ue->amf_ue_ngap_id);
    ogs_debug("    Target : RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            target_ue->ran_ue_ngap_id, target_ue->amf_ue_ngap_id);

    amf_ue_associate_ran_ue(amf_ue, target_ue);

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

    /* Copy TAI and ECGI from ran_ue */
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

    ogs_debug("Reset");

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

    ogs_debug("    IP[%s] RAN_ID[%d]",
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

        ran_ue_remove_in_gnb(gnb);
        break;
    case NGAP_ResetType_PR_partOfS1_Interface:
        ogs_debug("    NGAP_ResetType_PR_partOfS1_Interface");

        partOfS1_Interface = ResetType->choice.partOfS1_Interface;
        ogs_assert(partOfS1_Interface);
        for (i = 0; i < partOfS1_Interface->list.count; i++) {
            NGAP_UE_associatedLogicalS1_ConnectionItemRes_t *ie2 = NULL;
            NGAP_UE_associatedLogicalS1_ConnectionItem_t *item = NULL;

            ran_ue_t *ran_ue = NULL;

            ie2 = (NGAP_UE_associatedLogicalS1_ConnectionItemRes_t *)
                partOfS1_Interface->list.array[i];
            ogs_assert(ie2);

            item = &ie2->value.choice.UE_associatedLogicalS1_ConnectionItem;
            ogs_assert(item);
            
            ogs_debug("    AMF_UE_NGAP_ID[%d] RAN_UE_NGAP_ID[%d]",
                    item->mME_UE_NGAP_ID ? (int)*item->mME_UE_NGAP_ID : -1,
                    item->RAN_UE_NGAP_ID ? (int)*item->RAN_UE_NGAP_ID : -1);

            if (item->mME_UE_NGAP_ID)
                ran_ue = ran_ue_find_by_amf_ue_ngap_id(
                        *item->mME_UE_NGAP_ID);
            else if (item->RAN_UE_NGAP_ID)
                ran_ue = ran_ue_find_by_ran_ue_ngap_id(gnb,
                        *item->RAN_UE_NGAP_ID);

            if (ran_ue == NULL) {
                ogs_warn("Cannot find S1 Context "
                    "(AMF_UE_NGAP_ID[%d] RAN_UE_NGAP_ID[%d])",
                    item->mME_UE_NGAP_ID ? (int)*item->mME_UE_NGAP_ID : -1,
                    item->RAN_UE_NGAP_ID ? (int)*item->RAN_UE_NGAP_ID : -1);
                continue;
            }

            ran_ue_remove(ran_ue);
        }
        break;
    default:
        ogs_warn("Invalid ResetType[%d]", ResetType->present);
        break;
    }

    ngap_send_s1_reset_ack(gnb, partOfS1_Interface);
}

#endif
