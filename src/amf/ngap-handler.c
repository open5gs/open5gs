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

    return number_of_gnbs_online >= ogs_app()->max.gnb;
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
    ogs_assert(gnb->sctp.sock);

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
    ogs_debug("    IP[%s] GNB_ID[0x%x]", OGS_ADDR(gnb->sctp.addr, buf), gnb_id);

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
    ogs_assert(gnb->sctp.sock);

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
            UserLocationInformation =
                &ie->value.choice.UserLocationInformation;
            break;
        case NGAP_ProtocolIE_ID_id_FiveG_S_TMSI:
            FiveG_S_TMSI = &ie->value.choice.FiveG_S_TMSI;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

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
                    /* Previous NG(ran_ue_t) context the holding timer(30secs)
                     * is started.
                     * Newly associated NG(ran_ue_t) context holding timer
                     * is stopped. */
                    ogs_debug("[%s] Start NG Holding Timer", amf_ue->suci);
                    ogs_debug("[%s]    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
                            amf_ue->suci, amf_ue->ran_ue->ran_ue_ngap_id,
                            (long long)amf_ue->ran_ue->amf_ue_ngap_id);

                    /* De-associate NG with NAS/EMM */
                    ran_ue_deassociate(amf_ue->ran_ue);

                    ngap_send_ran_ue_context_release_command(amf_ue->ran_ue,
                            NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
                            NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE, 0);
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
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    UplinkNASTransport = &initiatingMessage->value.choice.UplinkNASTransport;
    ogs_assert(UplinkNASTransport);

    ogs_debug("Uplink NAS transport");

    for (i = 0; i < UplinkNASTransport->protocolIEs.list.count; i++) {
        ie = UplinkNASTransport->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
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

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(
                gnb, (uint32_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
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
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_UERadioCapability_t *UERadioCapability = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

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
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
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

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(
                gnb, (uint32_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
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
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceSetupListCxtRes_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceSetupItemCxtRes_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

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
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
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

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(
                gnb, (uint32_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        return;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    if (!PDUSessionList) /* No PDUSessionList */
        return;

    amf_ue = ran_ue->amf_ue;
    ogs_expect_or_return(amf_ue);

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

        memset(&param, 0, sizeof(param));
        param.n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
        ogs_assert(param.n2smbuf);
        param.n2SmInfoType = OpenAPI_n2_sm_info_type_PDU_RES_SETUP_RSP;
        ogs_pkbuf_put_data(param.n2smbuf, transfer->buf, transfer->size);

        /* UPDATE_UpCnxState - ACTIVATED */
        amf_sess_sbi_discover_and_send(OpenAPI_nf_type_SMF,
                sess, AMF_UPDATE_SM_CONTEXT_ACTIVATED, &param,
                amf_nsmf_pdu_session_build_update_sm_context);

        ogs_pkbuf_free(param.n2smbuf);
    }
}

void ngap_handle_initial_context_setup_failure(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, old_xact_count = 0, new_xact_count = 0;

    ran_ue_t *ran_ue = NULL;
    amf_ue_t *amf_ue = NULL;
    uint64_t amf_ue_ngap_id;

    NGAP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    NGAP_InitialContextSetupFailure_t *InitialContextSetupFailure = NULL;

    NGAP_InitialContextSetupFailureIEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

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
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
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
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(
                gnb, (uint32_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
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
    amf_ue = ran_ue->amf_ue;
    if (amf_ue) {
        old_xact_count = amf_sess_xact_count(amf_ue);

        amf_sbi_send_deactivate_all_sessions(
                amf_ue, AMF_UPDATE_SM_CONTEXT_DEACTIVATED,
                Cause->present, (int)Cause->choice.radioNetwork);

        new_xact_count = amf_sess_xact_count(amf_ue);
    }

    if (old_xact_count == new_xact_count) {
        ngap_send_ran_ue_context_release_command(ran_ue,
            NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
            NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE, 0);
    }
}

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
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceListCxtRelReq_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceItemCxtRelReq_t *PDUSessionItem = NULL;
    NGAP_Cause_t *Cause = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

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
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
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

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_warn("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(
                gnb, (uint32_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
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
        int xact_count = amf_sess_xact_count(amf_ue);

        if (!PDUSessionList) {
            amf_sbi_send_deactivate_all_sessions(
                    amf_ue, AMF_UPDATE_SM_CONTEXT_DEACTIVATED,
                    Cause->present, (int)Cause->choice.radioNetwork);
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
                            sess, AMF_UPDATE_SM_CONTEXT_DEACTIVATED,
                            Cause->present, (int)Cause->choice.radioNetwork);
                }
            }
        }

        if (amf_sess_xact_count(amf_ue) == xact_count)
            ngap_send_amf_ue_context_release_command(amf_ue,
                    NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
                    NGAP_UE_CTX_REL_NG_REMOVE_AND_UNLINK, 0);
    }
}

void ngap_handle_ue_context_release_complete(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int i;
    char buf[OGS_ADDRSTRLEN];
    uint64_t amf_ue_ngap_id;

    ran_ue_t *ran_ue = NULL;

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_UEContextReleaseComplete_t *UEContextReleaseComplete = NULL;

    NGAP_UEContextReleaseComplete_IEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

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
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(
                gnb, (uint32_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        return;
    }

    ngap_handle_ue_context_release_action(ran_ue);
}

void ngap_handle_ue_context_release_action(ran_ue_t *ran_ue)
{
    amf_ue_t *amf_ue = NULL;

    ogs_assert(ran_ue);

    amf_ue = ran_ue->amf_ue;

    ogs_info("UE Context Release [Action:%d]", ran_ue->ue_ctx_rel_action);
    ogs_info("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);
    if (amf_ue)
        ogs_info("    SUCI[%s]", amf_ue->suci ? amf_ue->suci : "Unknown");

    switch (ran_ue->ue_ctx_rel_action) {
    case NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE:
        ogs_debug("    Action: NG context remove");
        ran_ue_remove(ran_ue);
        break;
    case NGAP_UE_CTX_REL_NG_REMOVE_AND_UNLINK:
        ogs_debug("    Action: NG normal release");
        ran_ue_remove(ran_ue);
        ogs_expect_or_return(amf_ue);
        amf_ue_deassociate(amf_ue);
        break;
    case NGAP_UE_CTX_REL_UE_CONTEXT_REMOVE:
        ogs_debug("    Action: UE context remove");
        ran_ue_remove(ran_ue);
        ogs_expect_or_return(amf_ue);
        amf_ue_remove(amf_ue);
        break;
    case NGAP_UE_CTX_REL_DELETE_INDIRECT_TUNNEL:
#if 0
        ogs_debug("    Action: Delete indirect tunnel");

        source_ue_deassociate_target_ue(ran_ue);
        ran_ue_remove(ran_ue);

        ogs_expect_or_return(amf_ue);
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
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceSetupListSURes_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceSetupItemSURes_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

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
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListSURes:
            PDUSessionList =
                &ie->value.choice.PDUSessionResourceSetupListSURes;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(
                gnb, (uint32_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
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

        memset(&param, 0, sizeof(param));
        param.n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
        ogs_assert(param.n2smbuf);
        param.n2SmInfoType = OpenAPI_n2_sm_info_type_PDU_RES_SETUP_RSP;
        ogs_pkbuf_put_data(param.n2smbuf, transfer->buf, transfer->size);

        /* UPDATE_UpCnxState - ACTIVATED */
        amf_sess_sbi_discover_and_send(OpenAPI_nf_type_SMF,
                sess, AMF_UPDATE_SM_CONTEXT_ACTIVATED, &param,
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
    NGAP_PDUSessionResourceReleaseResponse_t
        *PDUSessionResourceReleaseResponse;

    NGAP_PDUSessionResourceReleaseResponseIEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceReleasedListRelRes_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceReleasedItemRelRes_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

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
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
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

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(
                gnb, (uint32_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
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
        ogs_assert(param.n2smbuf);
        param.n2SmInfoType = OpenAPI_n2_sm_info_type_PDU_RES_REL_RSP;
        ogs_pkbuf_put_data(param.n2smbuf, transfer->buf, transfer->size);

        amf_sess_sbi_discover_and_send(OpenAPI_nf_type_SMF,
                sess, AMF_UPDATE_SM_CONTEXT_N2_RELEASED, &param,
                amf_nsmf_pdu_session_build_update_sm_context);

        ogs_pkbuf_free(param.n2smbuf);
    }
}


void ngap_handle_path_switch_request(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    uint64_t amf_ue_ngap_id;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_PathSwitchRequest_t *PathSwitchRequest = NULL;
    
    NGAP_PathSwitchRequestIEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_UserLocationInformation_t *UserLocationInformation = NULL;
    NGAP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;
    NGAP_PDUSessionResourceToBeSwitchedDLList_t
        *PDUSessionResourceToBeSwitchedDLList = NULL;

    NGAP_NRencryptionAlgorithms_t *nRencryptionAlgorithms = NULL;
    NGAP_NRintegrityProtectionAlgorithms_t
        *nRintegrityProtectionAlgorithms = NULL;
    NGAP_EUTRAencryptionAlgorithms_t *eUTRAencryptionAlgorithms = NULL;
    NGAP_EUTRAintegrityProtectionAlgorithms_t
        *eUTRAintegrityProtectionAlgorithms = NULL;

    NGAP_PDUSessionResourceToBeSwitchedDLItem_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    amf_nsmf_pdu_session_update_sm_context_param_t param;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

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
        case NGAP_ProtocolIE_ID_id_SourceAMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_UserLocationInformation:
            UserLocationInformation = &ie->value.choice.UserLocationInformation;
            break;
        case NGAP_ProtocolIE_ID_id_UESecurityCapabilities:
            UESecurityCapabilities = &ie->value.choice.UESecurityCapabilities;
            break;
        case NGAP_ProtocolIE_ID_id_PDUSessionResourceToBeSwitchedDLList:
            PDUSessionResourceToBeSwitchedDLList =
                &ie->value.choice.PDUSessionResourceToBeSwitchedDLList;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    if (asn_INTEGER2ulong(AMF_UE_NGAP_ID,
                (unsigned long *)&amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        ngap_send_error_indication(gnb, (uint32_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        ngap_send_error_indication(
                gnb, (uint32_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
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

    if (!UserLocationInformation) {
        ogs_error("No UserLocationInformation");
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

    if (!UESecurityCapabilities) {
        ogs_error("No UESecurityCapabilities");
        ngap_send_error_indication(gnb, &ran_ue->ran_ue_ngap_id, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    nRencryptionAlgorithms =
        &UESecurityCapabilities->nRencryptionAlgorithms;
    nRintegrityProtectionAlgorithms =
        &UESecurityCapabilities->nRintegrityProtectionAlgorithms;
    eUTRAencryptionAlgorithms =
        &UESecurityCapabilities->eUTRAencryptionAlgorithms;
    eUTRAintegrityProtectionAlgorithms =
        &UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms;

    amf_ue->ue_security_capability.nea = nRencryptionAlgorithms->buf[0];
    amf_ue->ue_security_capability.nia =
        nRintegrityProtectionAlgorithms->buf[0];
    amf_ue->ue_security_capability.eea = eUTRAencryptionAlgorithms->buf[0];
    amf_ue->ue_security_capability.eia =
        eUTRAintegrityProtectionAlgorithms->buf[0];
    
    if (!PDUSessionResourceToBeSwitchedDLList) {
        ogs_error("No PDUSessionResourceToBeSwitchedDLList");
        ngap_send_error_indication(gnb, &ran_ue->ran_ue_ngap_id, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        return;
    }

    for (i = 0; i < PDUSessionResourceToBeSwitchedDLList->list.count; i++) {
        amf_sess_t *sess = NULL;
        PDUSessionItem = (NGAP_PDUSessionResourceToBeSwitchedDLItem_t *)
            PDUSessionResourceToBeSwitchedDLList->list.array[i];

        if (!PDUSessionItem) {
            ogs_error("No NGAP_PDUSessionResourceToBeSwitchedDLItem");
            ngap_send_error_indication2(amf_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            return;
        }

        transfer = &PDUSessionItem->pathSwitchRequestTransfer;
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

        memset(&param, 0, sizeof(param));
        param.n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
        ogs_assert(param.n2smbuf);
        param.n2SmInfoType = OpenAPI_n2_sm_info_type_PATH_SWITCH_REQ;
        ogs_pkbuf_put_data(param.n2smbuf, transfer->buf, transfer->size);

        /* UPDATE_UpCnxState - ACTIVATED */
        amf_sess_sbi_discover_and_send(OpenAPI_nf_type_SMF,
                sess, AMF_UPDATE_SM_CONTEXT_ACTIVATED, &param,
                amf_nsmf_pdu_session_build_update_sm_context);

        ogs_pkbuf_free(param.n2smbuf);
    }
}
