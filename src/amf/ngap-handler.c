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
#include "nas-path.h"

static bool maximum_number_of_gnbs_is_reached(void)
{
    amf_gnb_t *gnb = NULL, *next_gnb = NULL;
    int number_of_gnbs_online = 0;

    ogs_list_for_each_safe(&amf_self()->gnb_list, next_gnb, gnb) {
        if (gnb->state.ng_setup_success) {
            number_of_gnbs_online++;
        }
    }

    return number_of_gnbs_online >= ogs_global_conf()->max.peer;
}

static bool gnb_plmn_id_is_foreign(amf_gnb_t *gnb)
{
    int i, j;

    for (i = 0; i < gnb->num_of_supported_ta_list; i++) {
        for (j = 0; j < gnb->supported_ta_list[i].num_of_bplmn_list; j++) {
            if (memcmp(&gnb->plmn_id,
                        &gnb->supported_ta_list[i].bplmn_list[j].plmn_id,
                        OGS_PLMN_ID_LEN) == 0)
                return false;
        }
    }

    return true;
}

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
                    served_tai_index < OGS_MAX_NUM_OF_SUPPORTED_TA) {
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

void ngap_handle_ng_setup_request(amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, j, k, r;

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

    ogs_debug("NGSetupRequest");

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
        r = ngap_send_ng_setup_failure(gnb, group, cause);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    globalGNB_ID = GlobalRANNodeID->choice.globalGNB_ID;
    if (!globalGNB_ID) {
        ogs_error("No globalGNB_ID");
        group = NGAP_Cause_PR_protocol;
        cause = NGAP_CauseProtocol_semantic_error;
        r = ngap_send_ng_setup_failure(gnb, group, cause);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!SupportedTAList) {
        ogs_error("No SupportedTAList");
        group = NGAP_Cause_PR_protocol;
        cause = NGAP_CauseProtocol_semantic_error;
        r = ngap_send_ng_setup_failure(gnb, group, cause);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_ngap_GNB_ID_to_uint32(&globalGNB_ID->gNB_ID, &gnb_id);
    ogs_debug("    IP[%s] GNB_ID[0x%x]", OGS_ADDR(gnb->sctp.addr, buf), gnb_id);

    memcpy(&gnb->plmn_id,
            globalGNB_ID->pLMNIdentity.buf, sizeof(gnb->plmn_id));
    ogs_debug("    PLMN_ID[MCC:%d MNC:%d]",
            ogs_plmn_id_mcc(&gnb->plmn_id), ogs_plmn_id_mnc(&gnb->plmn_id));

    if (PagingDRX)
        ogs_debug("    PagingDRX[%ld]", *PagingDRX);

    /* Parse Supported TA */
    for (i = 0, gnb->num_of_supported_ta_list = 0;
            i < SupportedTAList->list.count &&
            gnb->num_of_supported_ta_list < OGS_MAX_NUM_OF_SUPPORTED_TA;
            i++) {
        NGAP_SupportedTAItem_t *SupportedTAItem = NULL;

        if (gnb->num_of_supported_ta_list >=
                OGS_ARRAY_SIZE(gnb->supported_ta_list)) {
            ogs_error("OVERFLOW GNB->num_of_supported_ta_list "
                    "[%d:%d:%d]",
                    gnb->num_of_supported_ta_list,
                    OGS_MAX_NUM_OF_SUPPORTED_TA,
                    (int)OGS_ARRAY_SIZE(gnb->supported_ta_list));
            break;
        }

        SupportedTAItem = (NGAP_SupportedTAItem_t *)
                SupportedTAList->list.array[i];
        if (!SupportedTAItem) {
            ogs_error("No SupportedTAItem");
            group = NGAP_Cause_PR_protocol;
            cause = NGAP_CauseProtocol_semantic_error;
            r = ngap_send_ng_setup_failure(gnb, group, cause);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
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

            if (gnb->supported_ta_list[i].num_of_bplmn_list >=
                    OGS_ARRAY_SIZE(gnb->supported_ta_list[i].bplmn_list)) {
                ogs_error("OVERFLOW GNB->supported_ta_list.num_of_bplmn_list "
                        "[%d:%d:%d]",
                        gnb->supported_ta_list[i].num_of_bplmn_list,
                        OGS_MAX_NUM_OF_BPLMN,
                        (int)OGS_ARRAY_SIZE(
                            gnb->supported_ta_list[i].bplmn_list));
                break;
            }

            BroadcastPLMNItem = (NGAP_BroadcastPLMNItem_t *)
                    SupportedTAItem->broadcastPLMNList.list.array[j];
            if (!BroadcastPLMNItem) {
                ogs_error("No BroadcastPLMNItem");
                group = NGAP_Cause_PR_protocol;
                cause = NGAP_CauseProtocol_semantic_error;
                r = ngap_send_ng_setup_failure(gnb, group, cause);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
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
                        OGS_MAX_NUM_OF_SLICE_SUPPORT;
                            k++) {
                NGAP_SliceSupportItem_t *SliceSupportItem = NULL;
                NGAP_S_NSSAI_t *s_NSSAI = NULL;

                if (gnb->supported_ta_list[i].bplmn_list[j].num_of_s_nssai >=
                        OGS_ARRAY_SIZE(
                            gnb->supported_ta_list[i].bplmn_list[j].s_nssai)) {
                    ogs_error("OVERFLOW GNB->supported_ta_list."
                            "bplmn_list.num_of_s_nssai [%d:%d:%d]",
                            gnb->supported_ta_list[i].bplmn_list[j].
                                num_of_s_nssai,
                            OGS_MAX_NUM_OF_SLICE_SUPPORT,
                            (int)OGS_ARRAY_SIZE(gnb->
                                supported_ta_list[i].bplmn_list[j].s_nssai));
                    break;
                }

                SliceSupportItem = (NGAP_SliceSupportItem_t *)
                        BroadcastPLMNItem->tAISliceSupportList.list.array[k];
                if (!SliceSupportItem) {
                    ogs_error("No SliceSupportItem");
                    group = NGAP_Cause_PR_protocol;
                    cause = NGAP_CauseProtocol_semantic_error;
                    r = ngap_send_ng_setup_failure(gnb, group, cause);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
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

        r = ngap_send_ng_setup_failure(gnb, group, cause);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    /*
     * TS38.413
     * Section 8.7.1.4 Abnormal Conditions
     *
     * If the AMF does not identify any of the PLMNs/SNPNs indicated
     * in the NG SETUP REQUEST message, it shall reject the NG Setup
     * procedure with an appropriate cause value.
     */
    if (gnb_plmn_id_is_foreign(gnb)) {
        ogs_warn("NG-Setup failure:");
        ogs_warn("    globalGNB_ID PLMN-ID is foreign");
        group = NGAP_Cause_PR_misc;
        cause = NGAP_CauseMisc_unknown_PLMN_or_SNPN;

        r = ngap_send_ng_setup_failure(gnb, group, cause);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!served_tai_is_found(gnb)) {
        ogs_warn("NG-Setup failure:");
        ogs_warn("    Cannot find Served TAI. Check 'amf.tai' configuration");
        group = NGAP_Cause_PR_misc;
        cause = NGAP_CauseMisc_unknown_PLMN_or_SNPN;

        r = ngap_send_ng_setup_failure(gnb, group, cause);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!s_nssai_is_found(gnb)) {
        ogs_warn("NG-Setup failure:");
        ogs_warn("    Cannot find S_NSSAI. "
                    "Check 'amf.plmn_support.s_nssai' configuration");
        group = NGAP_Cause_PR_radioNetwork;
        cause = NGAP_CauseRadioNetwork_slice_not_supported;

        r = ngap_send_ng_setup_failure(gnb, group, cause);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    amf_gnb_set_gnb_id(gnb, gnb_id);

    gnb->state.ng_setup_success = true;
    r = ngap_send_ng_setup_response(gnb);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);
}

void ngap_handle_initial_ue_message(amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int i, r;
    char buf[OGS_ADDRSTRLEN];

    ran_ue_t *ran_ue = NULL;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_InitialUEMessage_t *InitialUEMessage = NULL;

    NGAP_InitialUEMessage_IEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;
    NGAP_UserLocationInformation_t *UserLocationInformation = NULL;
    NGAP_UserLocationInformationNR_t *UserLocationInformationNR = NULL;
    NGAP_FiveG_S_TMSI_t *FiveG_S_TMSI = NULL;
    NGAP_UEContextRequest_t *UEContextRequest = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    InitialUEMessage = &initiatingMessage->value.choice.InitialUEMessage;
    ogs_assert(InitialUEMessage);

    ogs_info("InitialUEMessage");

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
        case NGAP_ProtocolIE_ID_id_UEContextRequest:
            UEContextRequest = &ie->value.choice.UEContextRequest;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!RAN_UE_NGAP_ID) {
        ogs_error("No RAN_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ran_ue = ran_ue_find_by_ran_ue_ngap_id(gnb, *RAN_UE_NGAP_ID);
    if (!ran_ue) {
        ran_ue = ran_ue_add(gnb, *RAN_UE_NGAP_ID);
        if (ran_ue == NULL) {
            r = ngap_send_error_indication(gnb, NULL, NULL,
                    NGAP_Cause_PR_misc,
                    NGAP_CauseMisc_control_processing_overload);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

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
                ogs_info("Unknown UE by 5G-S_TMSI[AMF_ID:0x%x,M_TMSI:0x%x]",
                    ogs_amf_id_hexdump(&nas_guti.amf_id), nas_guti.m_tmsi);
            } else {
                ogs_info("[%s]    5G-S_TMSI[AMF_ID:0x%x,M_TMSI:0x%x]",
                        AMF_UE_HAVE_SUCI(amf_ue) ? amf_ue->suci : "Unknown ID",
                        ogs_amf_id_hexdump(&amf_ue->current.guti.amf_id),
                        amf_ue->current.guti.m_tmsi);
                /* If NAS(amf_ue_t) has already been associated with
                 * older NG(ran_ue_t) context */
                if (CM_CONNECTED(amf_ue)) {
    /*
     * Issue #2786
     *
     * In cases where the UE sends an Integrity Un-Protected Registration
     * Request or Service Request, there is an issue of sending
     * a UEContextReleaseCommand for the OLD RAN Context.
     *
     * For example, if the UE switchs off and power-on after
     * the first connection, the 5G Core sends a UEContextReleaseCommand.
     *
     * However, since there is no RAN context for this on the gNB,
     * the gNB does not send a UEContextReleaseComplete,
     * so the deletion of the RAN Context does not function properly.
     *
     * To solve this problem, the 5G Core has been modified to implicitly
     * delete the RAN Context instead of sending a UEContextReleaseCommand.
     */
                    HOLDING_NG_CONTEXT(amf_ue);
                }
                amf_ue_associate_ran_ue(amf_ue, ran_ue);

                /*
                 * TS 24.501
                 * 5.3.7 Handling of the periodic registration update timer
                 *
                 * The mobile reachable timer shall be stopped
                 * when a NAS signalling connection is established for the UE.
                 * The implicit de-registration timer shall be stopped
                 * when a NAS signalling connection is established for the UE.
                 */
                CLEAR_AMF_UE_TIMER(amf_ue->mobile_reachable);
                CLEAR_AMF_UE_TIMER(amf_ue->implicit_deregistration);
            }
        }
    }

    if (!UserLocationInformation) {
        ogs_error("No UserLocationInformation");
        r = ngap_send_error_indication(gnb, &ran_ue->ran_ue_ngap_id, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (UserLocationInformation->present !=
            NGAP_UserLocationInformation_PR_userLocationInformationNR) {
        ogs_error("Not implemented UserLocationInformation[%d]",
                UserLocationInformation->present);
        r = ngap_send_error_indication(gnb, &ran_ue->ran_ue_ngap_id, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_unspecified);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!NAS_PDU) {
        ogs_error("No NAS_PDU");
        r = ngap_send_error_indication(gnb, &ran_ue->ran_ue_ngap_id, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    UserLocationInformationNR =
        UserLocationInformation->choice.userLocationInformationNR;
    ogs_assert(UserLocationInformationNR);
    ogs_ngap_ASN_to_nr_cgi(
            &UserLocationInformationNR->nR_CGI, &ran_ue->saved.nr_cgi);
    ogs_ngap_ASN_to_5gs_tai(
            &UserLocationInformationNR->tAI, &ran_ue->saved.nr_tai);

    ogs_info("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] "
            "TAC[%d] CellID[0x%llx]",
        (long long)ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id,
        ran_ue->saved.nr_tai.tac.v, (long long)ran_ue->saved.nr_cgi.cell_id);

    if (UEContextRequest) {
        if (*UEContextRequest == NGAP_UEContextRequest_requested) {
            ran_ue->ue_context_requested = true;
        }
    }

    ogs_expect(OGS_OK == ngap_send_to_nas(
                ran_ue, NGAP_ProcedureCode_id_InitialUEMessage, NAS_PDU));
}

void ngap_handle_uplink_nas_transport(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, r;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    uint64_t amf_ue_ngap_id;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UplinkNASTransport_t *UplinkNASTransport = NULL;

    NGAP_UplinkNASTransport_IEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;
    NGAP_UserLocationInformation_t *UserLocationInformation = NULL;
    NGAP_UserLocationInformationNR_t *UserLocationInformationNR = NULL;

    ogs_5gs_tai_t nr_tai;
    int served_tai_index = 0;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    UplinkNASTransport = &initiatingMessage->value.choice.UplinkNASTransport;
    ogs_assert(UplinkNASTransport);

    ogs_debug("UplinkNASTransport");

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
        case NGAP_ProtocolIE_ID_id_UserLocationInformation:
            UserLocationInformation = &ie->value.choice.UserLocationInformation;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, (uint64_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    amf_ue = ran_ue->amf_ue;
    if (!amf_ue) {
        ogs_error("Cannot find AMF-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!UserLocationInformation) {
        ogs_error("No UserLocationInformation");
        r = ngap_send_error_indication(gnb, &ran_ue->ran_ue_ngap_id, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (UserLocationInformation->present !=
            NGAP_UserLocationInformation_PR_userLocationInformationNR) {
        ogs_error("Not implemented UserLocationInformation[%d]",
                UserLocationInformation->present);
        r = ngap_send_error_indication(gnb, &ran_ue->ran_ue_ngap_id, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_unspecified);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!NAS_PDU) {
        ogs_error("No NAS_PDU");
        r = ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    UserLocationInformationNR =
        UserLocationInformation->choice.userLocationInformationNR;
    ogs_assert(UserLocationInformationNR);
    ogs_ngap_ASN_to_5gs_tai(&UserLocationInformationNR->tAI, &nr_tai);

    served_tai_index = amf_find_served_tai(&nr_tai);
    if (served_tai_index < 0) {
        ogs_error("Cannot find Served TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&nr_tai.plmn_id), nr_tai.tac.v);
        r = ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_protocol,
                NGAP_CauseProtocol_message_not_compatible_with_receiver_state);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);

    ogs_ngap_ASN_to_nr_cgi(
            &UserLocationInformationNR->nR_CGI, &ran_ue->saved.nr_cgi);
    ogs_ngap_ASN_to_5gs_tai(
            &UserLocationInformationNR->tAI, &ran_ue->saved.nr_tai);

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] "
            "TAC[%d] CellID[0x%llx]",
        (long long)ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id,
        ran_ue->saved.nr_tai.tac.v, (long long)ran_ue->saved.nr_cgi.cell_id);

    /* Copy NR-TAI/NR-CGI from ran_ue */
    memcpy(&amf_ue->nr_tai, &ran_ue->saved.nr_tai, sizeof(ogs_5gs_tai_t));
    memcpy(&amf_ue->nr_cgi, &ran_ue->saved.nr_cgi, sizeof(ogs_nr_cgi_t));

    ogs_expect(OGS_OK == ngap_send_to_nas(
                ran_ue, NGAP_ProcedureCode_id_UplinkNASTransport, NAS_PDU));
}

void ngap_handle_ue_radio_capability_info_indication(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, r;

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

    ogs_debug("UERadioCapabilityInfoIndication");

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
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, (uint64_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);

    if (!UERadioCapability) {
        ogs_error("No UERadioCapability");
        r = ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
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
    int i, r;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    amf_sess_t *sess = NULL;
    uint64_t amf_ue_ngap_id;
    amf_nsmf_pdusession_sm_context_param_t param;

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

    ogs_debug("InitialContextSetupResponse");

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
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, (uint64_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);

    ran_ue->initial_context_setup_response_received = true;

    amf_ue = ran_ue->amf_ue;
    if (!amf_ue) {
        ogs_error("Cannot find AMF-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    for (i = 0; PDUSessionList && i < PDUSessionList->list.count; i++) {
        PDUSessionItem = (NGAP_PDUSessionResourceSetupItemCxtRes_t *)
            PDUSessionList->list.array[i];

        if (!PDUSessionItem) {
            ogs_error("No PDUSessionResourceSetupItemCxtRes");
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        transfer = &PDUSessionItem->pDUSessionResourceSetupResponseTransfer;
        if (!transfer) {
            ogs_error("No PDUSessionResourceSetupResponseTransfer");
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        if (PDUSessionItem->pDUSessionID ==
                OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
            ogs_error("PDU Session Identity is unassigned");
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        sess = amf_sess_find_by_psi(amf_ue, PDUSessionItem->pDUSessionID);
        if (!sess) {
            ogs_error("Cannot find PDU Session ID [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        if (!SESSION_CONTEXT_IN_SMF(sess)) {
            ogs_error("Session Context is not in SMF [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        ogs_debug("    SUPI[%s] PSI[%d] OLD ACTIVATED[0x%x]",
                amf_ue->supi, sess->psi, ran_ue->psimask.activated);
        ran_ue->psimask.activated |= ((1 << sess->psi));
        ogs_debug("    NEW ACTIVATED[0x%x]", ran_ue->psimask.activated);

        memset(&param, 0, sizeof(param));
        param.n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
        ogs_assert(param.n2smbuf);
        param.n2SmInfoType = OpenAPI_n2_sm_info_type_PDU_RES_SETUP_RSP;
        ogs_pkbuf_put_data(param.n2smbuf, transfer->buf, transfer->size);

        r = amf_sess_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                amf_nsmf_pdusession_build_update_sm_context,
                ran_ue, sess, AMF_UPDATE_SM_CONTEXT_ACTIVATED, &param);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

        ogs_pkbuf_free(param.n2smbuf);
    }

    /*
     * TS24.501
     * 5.4.4 Generic UE configuration update procedure
     * 5.4.4.1 General
     *
     * This procedure shall be initiated by the network to assign
     * a new 5G-GUTI to the UE after a successful service request
     * procedure invoked as a response to a paging request
     * from the network and before the release of the N1 NAS signalling
     * connection.
     */
    if (DOWNLINK_SIGNALLING_PENDING(amf_ue) == true) {
        /*
         * TS24.501
         * 5.4.4 Generic UE configuration update procedure
         * 5.4.4.1 General
         *
         * If the service request procedure was triggered
         * due to 5GSM downlink signalling pending, the procedure
         * for assigning a new 5G-GUTI can be initiated by the network
         * after the transport of the 5GSM downlink signalling.
         */
        ogs_list_for_each(&amf_ue->sess_list, sess) {
            /* There is no Downlink Signalling Pending in this sesssion */
            if (sess->gsm_message.type == 0) continue;

            switch (sess->gsm_message.type) {
            case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND:
                r = nas_send_pdu_session_modification_command(sess,
                            sess->gsm_message.n1buf, sess->gsm_message.n2buf);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

                /* n1buf is de-allocated
                 * in gmm_build_dl_nas_transport() */
                sess->gsm_message.n1buf = NULL;
                /* n2buf is de-allocated
                 * in ngap_build_pdu_session_resource_modify_request() */
                sess->gsm_message.n2buf = NULL;

                AMF_SESS_CLEAR_5GSM_MESSAGE(sess);

                break;
            case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND:
                r = nas_send_pdu_session_release_command(sess,
                            sess->gsm_message.n1buf, sess->gsm_message.n2buf);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);

                /* n1buf is de-allocated
                 * in gmm_build_dl_nas_transport() */
                sess->gsm_message.n1buf = NULL;
                /* n2buf is de-allocated
                 * in ngap_build_pdu_session_resource_modify_request() */
                sess->gsm_message.n2buf = NULL;

                AMF_SESS_CLEAR_5GSM_MESSAGE(sess);

                break;
            default:
                ogs_fatal("Unknown GSM Message Type[%d]",
                        sess->gsm_message.type);
                ogs_assert_if_reached();
            }
        }
    } else if (PAGING_ONGOING(amf_ue) == true) {
        gmm_configuration_update_command_param_t param;

        /*
         * TS24.501
         * 5.3.3 Temporary identities
         *
         * The AMF shall assign a new 5G-GUTI for a particular UE:
         *
         * a) during a successful initial registration procedure;
         * b) during a successful registration procedure
         *    for mobility registration update; and
         * c) after a successful service request procedure invoked
         *    as a response to a paging request from the network and
         *    before the release of the N1 NAS signalling connection
         *    as specified in subclause 5.4.4.1.
         *
         * The AMF should assign a new 5G-GUTI for a particular UE
         * during a successful registration procedure
         * for periodic registration update.
         *
         * The AMF may assign a new 5G-GUTI at any time for a particular UE
         * by performing the generic UE configuration update procedure.
         */
        amf_ue_new_guti(amf_ue);

        memset(&param, 0, sizeof(param));
        param.acknowledgement_requested = 1;
        param.guti = 1;
        r = nas_5gs_send_configuration_update_command(amf_ue, &param);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

        AMF_UE_CLEAR_PAGING_INFO(amf_ue);
    }
}

void ngap_handle_initial_context_setup_failure(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, r, old_xact_count = 0, new_xact_count = 0;

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

    ogs_debug("InitialContextSetupFailure");

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
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, (uint64_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);

    if (!Cause) {
        ogs_error("No Cause");
        r = ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
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
        /*
         * if T3550 is running, Registration complete will be sent.
         * So, we need to clear all the timer at this point.
         */
        CLEAR_AMF_UE_ALL_TIMERS(amf_ue);

        old_xact_count = amf_sess_xact_count(amf_ue);

        amf_ue->deactivation.group = NGAP_Cause_PR_nas;
        amf_ue->deactivation.cause = NGAP_CauseNas_normal_release;

        amf_sbi_send_deactivate_all_sessions(
                ran_ue, amf_ue, AMF_UPDATE_SM_CONTEXT_DEACTIVATED,
                Cause->present, (int)Cause->choice.radioNetwork);

        new_xact_count = amf_sess_xact_count(amf_ue);
    }

    if (old_xact_count == new_xact_count) {
        r = ngap_send_ran_ue_context_release_command(ran_ue,
                NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release,
                NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE, 0);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    }
}

void ngap_handle_ue_context_modification_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int i;
    char buf[OGS_ADDRSTRLEN];
    uint64_t amf_ue_ngap_id;

    ran_ue_t *ran_ue = NULL;

    NGAP_SuccessfulOutcome_t *SuccessfulOutcome = NULL;
    NGAP_UEContextModificationResponse_t *UEContextModificationResponse = NULL;

    NGAP_UEContextModificationResponseIEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    /* NGAP_RRCState_t *RRCState = NULL; */
    /* NGAP_UserLocationInformation_t *UserLocationInformation = NULL; */
    /* NGAP_CriticalityDiagnostics_t *CriticalityDiagnostics = NULL; */

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_assert(message);
    SuccessfulOutcome = message->choice.successfulOutcome;
    ogs_assert(SuccessfulOutcome);
    UEContextModificationResponse = &SuccessfulOutcome->value.choice.UEContextModificationResponse;
    ogs_assert(UEContextModificationResponse);

    ogs_warn("UEContextModificationResponse");

    for (i = 0; i < UEContextModificationResponse->protocolIEs.list.count; i++) {
        ie = UEContextModificationResponse->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        /* case NGAP_ProtocolIE_ID_id_RRCState: */
        /*     RRCState = &ie->value.choice.RRCState; */
        /*     break; */
        /* case NGAP_ProtocolIE_ID_id_UserLocationInformation: */
        /*     UserLocationInformation = &ie->value.choice.UserLocationInformation; */
        /* case NGAP_ProtocolIE_ID_id_CriticalityDiagnostics: */
        /*     CriticalityDiagnostics = &ie->value.choice.CriticalityDiagnostics; */
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (AMF_UE_NGAP_ID) {

        if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
            ogs_warn("Invalid AMF_UE_NGAP_ID");
        }

        ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
        if (!ran_ue)
            ogs_warn("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                    (long long)amf_ue_ngap_id);

    } else if (RAN_UE_NGAP_ID) {
        ran_ue = ran_ue_find_by_ran_ue_ngap_id(gnb, *RAN_UE_NGAP_ID);
        if (!ran_ue)
            ogs_warn("No RAN UE Context : RAN_UE_NGAP_ID[%d]",
                    (int)*RAN_UE_NGAP_ID);
    }
}

void ngap_handle_ue_context_modification_failure(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int i;
    char buf[OGS_ADDRSTRLEN];
    uint64_t amf_ue_ngap_id;

    ran_ue_t *ran_ue = NULL;

    NGAP_UnsuccessfulOutcome_t *UnsuccessfulOutcome = NULL;
    NGAP_UEContextModificationFailure_t *UEContextModificationFailiure = NULL;

    NGAP_UEContextModificationFailureIEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_assert(message);
    UnsuccessfulOutcome = message->choice.unsuccessfulOutcome;
    ogs_assert(UnsuccessfulOutcome);
    UEContextModificationFailiure = &UnsuccessfulOutcome->value.choice.UEContextModificationFailure;
    ogs_assert(UEContextModificationFailiure);

    ogs_warn("UEContextModificationFailiure");

    for (i = 0; i < UEContextModificationFailiure->protocolIEs.list.count; i++) {
        ie = UEContextModificationFailiure->protocolIEs.list.array[i];
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

    ogs_warn("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (AMF_UE_NGAP_ID) {

        if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
            ogs_warn("Invalid AMF_UE_NGAP_ID");
        }

        ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
        if (!ran_ue)
            ogs_warn("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                    (long long)amf_ue_ngap_id);
        else
            ogs_warn("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
                    (long long)ran_ue->ran_ue_ngap_id,
                    (long long)ran_ue->amf_ue_ngap_id);

    } else if (RAN_UE_NGAP_ID) {
        ran_ue = ran_ue_find_by_ran_ue_ngap_id(gnb, *RAN_UE_NGAP_ID);
        if (!ran_ue)
            ogs_warn("No RAN UE Context : RAN_UE_NGAP_ID[%d]",
                    (int)*RAN_UE_NGAP_ID);
        else
            ogs_warn("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
                    (long long)ran_ue->ran_ue_ngap_id,
                    (long long)ran_ue->amf_ue_ngap_id);
    }

    if (Cause) {
        ogs_warn("    Cause[Group:%d Cause:%d]",
                Cause->present, (int)Cause->choice.radioNetwork);
    }
}


void ngap_handle_ue_context_release_request(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int i, r;
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

    ogs_debug("UEContextReleaseRequest");

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
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_warn("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, (uint64_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);

    if (!Cause) {
        ogs_error("No Cause");
        r = ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
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
        ogs_error("Cannot find AMF-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_ran_ue_context_release_command(ran_ue,
                Cause->present, (int)Cause->choice.radioNetwork,
                NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE, 0);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    } else {
        int xact_count = amf_sess_xact_count(amf_ue);

        amf_ue->deactivation.group = Cause->present;
        amf_ue->deactivation.cause = (int)Cause->choice.radioNetwork;

        if (!PDUSessionList) {
            amf_sbi_send_deactivate_all_sessions(
                    ran_ue, amf_ue, AMF_UPDATE_SM_CONTEXT_DEACTIVATED,
                    Cause->present, (int)Cause->choice.radioNetwork);
        } else {
            for (i = 0; i < PDUSessionList->list.count; i++) {
                PDUSessionItem = (NGAP_PDUSessionResourceItemCxtRelReq_t *)
                    PDUSessionList->list.array[i];

                if (!PDUSessionItem) {
                    ogs_error("No PDUSessionResourceSetupItemSURes");
                    r = ngap_send_error_indication2(
                            ran_ue, NGAP_Cause_PR_protocol,
                            NGAP_CauseProtocol_semantic_error);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                    return;
                }

                if (PDUSessionItem->pDUSessionID ==
                        OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
                    ogs_error("PDU Session Identity is unassigned");
                    r = ngap_send_error_indication2(
                            ran_ue, NGAP_Cause_PR_protocol,
                            NGAP_CauseProtocol_semantic_error);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                    return;
                }

                sess = amf_sess_find_by_psi(amf_ue,
                        PDUSessionItem->pDUSessionID);
                if (SESSION_CONTEXT_IN_SMF(sess)) {
                    amf_sbi_send_deactivate_session(
                            ran_ue, sess, AMF_UPDATE_SM_CONTEXT_DEACTIVATED,
                            Cause->present, (int)Cause->choice.radioNetwork);
                }
            }
        }
 
        if (amf_sess_xact_count(amf_ue) == xact_count) {
            r = ngap_send_amf_ue_context_release_command(amf_ue,
                    Cause->present, (int)Cause->choice.radioNetwork,
                    NGAP_UE_CTX_REL_NG_REMOVE_AND_UNLINK, 0);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
    }
}

void ngap_handle_ue_context_release_complete(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int i, r;
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

    ogs_debug("UEContextReleaseComplete");

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
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, (uint64_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ngap_handle_ue_context_release_action(ran_ue);
}

void ngap_handle_ue_context_release_action(ran_ue_t *ran_ue)
{
    int r;
    amf_ue_t *amf_ue = NULL;

    ogs_assert(ran_ue);

    if (ran_ue_cycle(ran_ue) == NULL) {
        ogs_error("NG context has already been removed");
        return;
    }

    amf_ue = ran_ue->amf_ue;

    ogs_info("UE Context Release [Action:%d]", ran_ue->ue_ctx_rel_action);
    ogs_info("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);
    if (amf_ue) {
        ogs_info("    SUCI[%s]", amf_ue->suci ? amf_ue->suci : "Unknown");

        /*
         * An assert occurs when a NAS message retransmission occurs.
         *
         * Because there is no `ran_ue` context.
         *
         * Therefore, before removing `ran_ue`, all Timers must be stopped
         * to prevent retransmission of NAS messages.
         */
        CLEAR_AMF_UE_ALL_TIMERS(amf_ue);
    }

    switch (ran_ue->ue_ctx_rel_action) {
    case NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE:
        ogs_debug("    Action: NG context remove");
        ran_ue_remove(ran_ue);
        break;
    case NGAP_UE_CTX_REL_NG_REMOVE_AND_UNLINK:
        ogs_debug("    Action: NG normal release");
        ran_ue_remove(ran_ue);
        if (!amf_ue) {
            ogs_error("No UE(amf-ue) Context");
            return;
        }
        amf_ue_deassociate(amf_ue);

        /*
         * When AMF release the NAS signalling connection,
         * ran_ue context is removed by ran_ue_remove() and
         * amf_ue/ran_ue is de-associated by amf_ue_deassociate().
         *
         * In this case, implicit deregistration is attempted
         * by the mobile reachable timer according to the standard document,
         * and amf_ue will be removed by amf_ue_remove().
         *
         * TS 24.501
         * 5.3.7 Handling of the periodic registration update timer and
         *
         * Start AMF_TIMER_MOBILE_REACHABLE
         * mobile reachable timer
         * The network supervises the periodic registration update procedure
         * of the UE by means of the mobile reachable timer.
         * If the UE is not registered for emergency services,
         * the mobile reachable timer shall be longer than the value of timer
         * T3512. In this case, by default, the mobile reachable timer is
         * 4 minutes greater than the value of timer T3512.
         * The mobile reachable timer shall be reset and started with the
         * value as indicated above, when the AMF releases the NAS signalling
         * connection for the UE.
         *
         * TODO: If the UE is registered for emergency services, the AMF shall
         * set the mobile reachable timer with a value equal to timer T3512.
         */
        ogs_timer_start(amf_ue->mobile_reachable.timer,
                ogs_time_from_sec(amf_self()->time.t3512.value + 240));

        break;

    case NGAP_UE_CTX_REL_UE_CONTEXT_REMOVE:
        ogs_debug("    Action: UE context remove");
        ran_ue_remove(ran_ue);
        if (!amf_ue) {
            ogs_error("No UE(amf-ue) context");
            return;
        }
        amf_ue_remove(amf_ue);
        break;
    case NGAP_UE_CTX_REL_NG_HANDOVER_COMPLETE:
        ogs_debug("    Action: NG handover complete");

        source_ue_deassociate_target_ue(ran_ue);
        ran_ue_remove(ran_ue);

        if (!amf_ue) {
            ogs_error("No UE(amf-ue) context");
            return;
        }
        break;
    case NGAP_UE_CTX_REL_NG_HANDOVER_CANCEL:
        ogs_warn("    Action: NG handover cancel");

        source_ue_deassociate_target_ue(ran_ue);
        ran_ue_remove(ran_ue);

        if (!amf_ue) {
            ogs_error("No UE(amf-ue) context");
            return;
        }
        if (!amf_ue->ran_ue) {
            ogs_error("No NG context");
            return;
        }

        r = ngap_send_handover_cancel_ack(amf_ue->ran_ue);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        break;
    case NGAP_UE_CTX_REL_NG_HANDOVER_FAILURE:
        ogs_warn("    Action: NG handover failure");

        source_ue_deassociate_target_ue(ran_ue);
        ran_ue_remove(ran_ue);

        if (!amf_ue) {
            ogs_error("No UE(amf-ue) context");
            return;
        }
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
    int i, r;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    uint64_t amf_ue_ngap_id;
    amf_nsmf_pdusession_sm_context_param_t param;

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_PDUSessionResourceSetupResponse_t *PDUSessionResourceSetupResponse;

    NGAP_PDUSessionResourceSetupResponseIEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceSetupListSURes_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceSetupItemSURes_t *PDUSessionItem = NULL;
    NGAP_PDUSessionResourceFailedToSetupListSURes_t
        *PDUSessionFailedList = NULL;
    NGAP_PDUSessionResourceFailedToSetupItemSURes_t
        *PDUSessionFailedItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    PDUSessionResourceSetupResponse =
        &successfulOutcome->value.choice.PDUSessionResourceSetupResponse;
    ogs_assert(PDUSessionResourceSetupResponse);

    ogs_debug("PDUSessionResourceSetupResponse");

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
        case NGAP_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListSURes:
            PDUSessionFailedList =
                &ie->value.choice.PDUSessionResourceFailedToSetupListSURes;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, (uint64_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);

    amf_ue = ran_ue->amf_ue;
    if (!amf_ue) {
        ogs_error("Cannot find AMF-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (PDUSessionList) {
        for (i = 0; i < PDUSessionList->list.count; i++) {
            amf_sess_t *sess = NULL;
            PDUSessionItem = (NGAP_PDUSessionResourceSetupItemSURes_t *)
                PDUSessionList->list.array[i];

            if (!PDUSessionItem) {
                ogs_error("No PDUSessionResourceSetupItemSURes");
                r = ngap_send_error_indication2(
                        ran_ue,
                        NGAP_Cause_PR_protocol,
                        NGAP_CauseProtocol_semantic_error);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                return;
            }

            transfer = &PDUSessionItem->pDUSessionResourceSetupResponseTransfer;
            if (!transfer) {
                ogs_error("No PDUSessionResourceSetupResponseTransfer");
                r = ngap_send_error_indication2(
                        ran_ue,
                        NGAP_Cause_PR_protocol,
                        NGAP_CauseProtocol_semantic_error);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                return;
            }

            if (PDUSessionItem->pDUSessionID ==
                    OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
                ogs_error("PDU Session Identity is unassigned");
                r = ngap_send_error_indication2(
                        ran_ue,
                        NGAP_Cause_PR_protocol,
                        NGAP_CauseProtocol_semantic_error);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                return;
            }

            sess = amf_sess_find_by_psi(amf_ue, PDUSessionItem->pDUSessionID);
            if (!sess) {
                ogs_error("Cannot find PDU Session ID [%d]",
                        (int)PDUSessionItem->pDUSessionID);
                r = ngap_send_error_indication2(ran_ue,
                        NGAP_Cause_PR_radioNetwork,
                        NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                return;
            }

            if (!SESSION_CONTEXT_IN_SMF(sess)) {
                ogs_error("Session Context is not in SMF [%d]",
                        (int)PDUSessionItem->pDUSessionID);
                r = ngap_send_error_indication2(ran_ue,
                        NGAP_Cause_PR_radioNetwork,
                        NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                return;
            }

            ogs_debug("    SUPI[%s] PSI[%d] OLD ACTIVATED[0x%x]",
                    amf_ue->supi, sess->psi, ran_ue->psimask.activated);
            ran_ue->psimask.activated |= ((1 << sess->psi));
            ogs_debug("    NEW ACTIVATED[0x%x]", ran_ue->psimask.activated);

            memset(&param, 0, sizeof(param));
            param.n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
            ogs_assert(param.n2smbuf);
            param.n2SmInfoType = OpenAPI_n2_sm_info_type_PDU_RES_SETUP_RSP;
            ogs_pkbuf_put_data(param.n2smbuf, transfer->buf, transfer->size);

            r = amf_sess_sbi_discover_and_send(
                    OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                    amf_nsmf_pdusession_build_update_sm_context,
                    ran_ue, sess, AMF_UPDATE_SM_CONTEXT_ACTIVATED, &param);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            ogs_pkbuf_free(param.n2smbuf);
        }
    } else if (PDUSessionFailedList) {
        for (i = 0; i < PDUSessionFailedList->list.count; i++) {
            amf_sess_t *sess = NULL;
            PDUSessionFailedItem =
                (NGAP_PDUSessionResourceFailedToSetupItemSURes_t *)
                PDUSessionFailedList->list.array[i];

            if (!PDUSessionFailedItem) {
                ogs_error("No PDUSessionResourceFailedToSetupItemSURes");
                r = ngap_send_error_indication2(
                        ran_ue,
                        NGAP_Cause_PR_protocol,
                        NGAP_CauseProtocol_semantic_error);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                return;
            }

            transfer =
                &PDUSessionFailedItem->
                    pDUSessionResourceSetupUnsuccessfulTransfer;
            if (!transfer) {
                ogs_error("No PDUSessionResourceSetupUnsuccessfulTransfer");
                r = ngap_send_error_indication2(
                        ran_ue,
                        NGAP_Cause_PR_protocol,
                        NGAP_CauseProtocol_semantic_error);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                return;
            }

            if (PDUSessionFailedItem->pDUSessionID ==
                    OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
                ogs_error("PDU Session Identity is unassigned");
                r = ngap_send_error_indication2(
                        ran_ue,
                        NGAP_Cause_PR_protocol,
                        NGAP_CauseProtocol_semantic_error);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                return;
            }

            sess = amf_sess_find_by_psi(
                    amf_ue, PDUSessionFailedItem->pDUSessionID);
            if (!sess) {
                ogs_error("Cannot find PDU Session ID [%d]",
                        (int)PDUSessionFailedItem->pDUSessionID);
                r = ngap_send_error_indication2(
                        ran_ue,
                        NGAP_Cause_PR_radioNetwork,
                        NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                return;
            }

            if (!SESSION_CONTEXT_IN_SMF(sess)) {
                ogs_error("Session Context is not in SMF [%d]",
                        (int)PDUSessionFailedItem->pDUSessionID);
                r = ngap_send_error_indication2(ran_ue,
                        NGAP_Cause_PR_radioNetwork,
                        NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
                return;
            }

            /*
             * TS23.502
             * 4.2.3 Service Request procedures
             * 4.2.3.2 UE Triggered Service Request
             *
             * 15. ...
             * If a PDU Session is rejected by the serving NG-RAN
             * with an indication that the PDU Session was rejected
             * because User Plane Security Enforcement is not supported
             * in the serving NG-RAN and the User Plane Enforcement Policy
             * indicates "Required" as described in clause 5.10.3
             * of TS 23.501 [2], the SMF shall trigger the release
             * of this PDU Session.
             *
             * In all other cases of PDU Session rejection,
             * the SMF can decide whether to release the PDU Session
             * or to deactivate the UP connection of this PDU Session.
             *
             *
             * TS29.502
             *
             * 5.2.2.3.2
             * Activation and Deactivation of the User Plane connection
             * of a PDU session
             * 5.2.2.3.2.2
             * Activation of User Plane connectivity of a PDU session
             *
             * 3. ...
             * N2 SM information received from the 5G-AN
             * (see PDU Session Resource Setup Unsuccessful Transfer IE
             * in clause 9.3.4.16 of 3GPP TS 38.413 [9]),
             * including the Cause of the failure, if resources failed
             * to be established for the PDU session.
             *
             * Upon receipt of this request, the SMF shall:
             * - consider that the activation of the User Plane connection
             *   has failed and set the upCnxState attribute to DEACTIVATED"
             *   otherwise.
             */
            memset(&param, 0, sizeof(param));
            param.n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
            ogs_assert(param.n2smbuf);
            param.n2SmInfoType = OpenAPI_n2_sm_info_type_PDU_RES_SETUP_FAIL;
            ogs_pkbuf_put_data(param.n2smbuf, transfer->buf, transfer->size);

            amf_ue->deactivation.group = NGAP_Cause_PR_nas;
            amf_ue->deactivation.cause = NGAP_CauseNas_normal_release;

            r = amf_sess_sbi_discover_and_send(
                    OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                    amf_nsmf_pdusession_build_update_sm_context,
                    ran_ue, sess, AMF_UPDATE_SM_CONTEXT_SETUP_FAIL, &param);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            ogs_pkbuf_free(param.n2smbuf);
        }
    } else {
        ogs_error("No PDUSessionResourceList");
        r = ngap_send_error_indication2(ran_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    }
}

void ngap_handle_pdu_session_resource_modify_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, r;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    uint64_t amf_ue_ngap_id;
    amf_nsmf_pdusession_sm_context_param_t param;

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_PDUSessionResourceModifyResponse_t *PDUSessionResourceModifyResponse;

    NGAP_PDUSessionResourceModifyResponseIEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceModifyListModRes_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceModifyItemModRes_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_assert(message);
    successfulOutcome = message->choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    PDUSessionResourceModifyResponse =
        &successfulOutcome->value.choice.PDUSessionResourceModifyResponse;
    ogs_assert(PDUSessionResourceModifyResponse);

    ogs_debug("PDUSessionResourceModifyResponse");

    for (i = 0; i < PDUSessionResourceModifyResponse->protocolIEs.list.count;
            i++) {
        ie = PDUSessionResourceModifyResponse->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_PDUSessionResourceModifyListModRes:
            PDUSessionList =
                &ie->value.choice.PDUSessionResourceModifyListModRes;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, (uint64_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);

    amf_ue = ran_ue->amf_ue;
    if (!amf_ue) {
        ogs_error("Cannot find AMF-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!PDUSessionList) {
        ogs_error("No PDUSessionResourceModifyListModRes");
        r = ngap_send_error_indication2(ran_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    for (i = 0; i < PDUSessionList->list.count; i++) {
        amf_sess_t *sess = NULL;
        PDUSessionItem = (NGAP_PDUSessionResourceModifyItemModRes_t *)
            PDUSessionList->list.array[i];

        if (!PDUSessionItem) {
            ogs_error("No PDUSessionResourceModifyItemModRes");
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        transfer = &PDUSessionItem->pDUSessionResourceModifyResponseTransfer;
        if (!transfer) {
            ogs_error("No PDUSessionResourceModifyResponseTransfer");
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        if (PDUSessionItem->pDUSessionID ==
                OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
            ogs_error("PDU Session Identity is unassigned");
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        sess = amf_sess_find_by_psi(amf_ue, PDUSessionItem->pDUSessionID);
        if (!sess) {
            ogs_error("Cannot find PDU Session ID [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        if (!SESSION_CONTEXT_IN_SMF(sess)) {
            ogs_error("Session Context is not in SMF [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        memset(&param, 0, sizeof(param));
        param.n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
        ogs_assert(param.n2smbuf);
        param.n2SmInfoType = OpenAPI_n2_sm_info_type_PDU_RES_MOD_RSP;
        ogs_pkbuf_put_data(param.n2smbuf, transfer->buf, transfer->size);

        r = amf_sess_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                amf_nsmf_pdusession_build_update_sm_context,
                ran_ue, sess, AMF_UPDATE_SM_CONTEXT_MODIFIED, &param);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

        ogs_pkbuf_free(param.n2smbuf);
    }
}

void ngap_handle_pdu_session_resource_release_response(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, r;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    uint64_t amf_ue_ngap_id;
    amf_nsmf_pdusession_sm_context_param_t param;

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

    ogs_debug("PDUSessionResourceReleaseResponse");

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
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, (uint64_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);

    amf_ue = ran_ue->amf_ue;
    if (!amf_ue) {
        ogs_error("Cannot find AMF-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!PDUSessionList) {
        ogs_error("No PDUSessionResourceReleasedListRelRes");
        r = ngap_send_error_indication2(ran_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    for (i = 0; i < PDUSessionList->list.count; i++) {
        amf_sess_t *sess = NULL;
        PDUSessionItem = (NGAP_PDUSessionResourceReleasedItemRelRes_t *)
            PDUSessionList->list.array[i];

        if (!PDUSessionItem) {
            ogs_error("No PDUSessionResourceReleasedItemRelRes");
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        transfer = &PDUSessionItem->pDUSessionResourceReleaseResponseTransfer;
        if (!transfer) {
            ogs_error("No PDUSessionResourceReleaseResponseTransfer");
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        if (PDUSessionItem->pDUSessionID ==
                OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
            ogs_error("PDU Session Identity is unassigned");
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        sess = amf_sess_find_by_psi(amf_ue, PDUSessionItem->pDUSessionID);
        if (!sess) {
            ogs_error("Cannot find PDU Session ID [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        if (!SESSION_CONTEXT_IN_SMF(sess)) {
            ogs_error("Session Context is not in SMF [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        memset(&param, 0, sizeof(param));
        param.n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
        ogs_assert(param.n2smbuf);
        param.n2SmInfoType = OpenAPI_n2_sm_info_type_PDU_RES_REL_RSP;
        ogs_pkbuf_put_data(param.n2smbuf, transfer->buf, transfer->size);

        r = amf_sess_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                amf_nsmf_pdusession_build_update_sm_context,
                ran_ue, sess, AMF_UPDATE_SM_CONTEXT_N2_RELEASED, &param);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

        ogs_pkbuf_free(param.n2smbuf);

        sess->pdu_session_resource_release_response_received = true;
        if (sess->pdu_session_release_complete_received == true)
            CLEAR_SESSION_CONTEXT(sess);
    }

}

void ngap_handle_uplink_ran_configuration_transfer(
        amf_gnb_t *gnb, ogs_ngap_message_t *message, ogs_pkbuf_t *pkbuf)
{
    char buf[OGS_ADDRSTRLEN];
    int i, r;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UplinkRANConfigurationTransfer_t
        *UplinkRANConfigurationTransfer = NULL;

    NGAP_UplinkRANConfigurationTransferIEs_t *ie = NULL;
    NGAP_SONConfigurationTransfer_t *SONConfigurationTransfer = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    UplinkRANConfigurationTransfer =
        &initiatingMessage->value.choice.UplinkRANConfigurationTransfer;
    ogs_assert(UplinkRANConfigurationTransfer);

    ogs_debug("UplinkRANConfigurationTransfer");
    for (i = 0;
            i < UplinkRANConfigurationTransfer->protocolIEs.list.count; i++) {
        ie = UplinkRANConfigurationTransfer->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_SONConfigurationTransferUL:
            SONConfigurationTransfer =
                &ie->value.choice.SONConfigurationTransfer;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] ENB_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (SONConfigurationTransfer) {
        NGAP_TargetRANNodeID_t *targetRANNodeID = NULL;
        NGAP_GlobalRANNodeID_t *targetGlobalRANNodeID;
        NGAP_GlobalGNB_ID_t *targetGlobalGNB_ID;
        NGAP_SourceRANNodeID_t *sourceRANNodeID = NULL;
        NGAP_GlobalRANNodeID_t *sourceGlobalRANNodeID;
        NGAP_GlobalGNB_ID_t *sourceGlobalGNB_ID;

        amf_gnb_t *target_gnb = NULL;
        uint32_t target_gnb_id, source_gnb_id;
        ogs_5gs_tai_t target_tai, source_tai;

        targetRANNodeID = &SONConfigurationTransfer->targetRANNodeID;
        targetGlobalRANNodeID = &targetRANNodeID->globalRANNodeID;

        if (targetGlobalRANNodeID->present !=
                NGAP_GlobalRANNodeID_PR_globalGNB_ID) {
            ogs_error("Not implemented targetGlobalRANNodeID->present[%d]",
                    targetGlobalRANNodeID->present);
            r = ngap_send_error_indication(gnb, NULL, NULL,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        targetGlobalGNB_ID = targetGlobalRANNodeID->choice.globalGNB_ID;
        if (!targetGlobalGNB_ID) {
            ogs_error("No targetGlobalGNB_ID");
            r = ngap_send_error_indication(gnb, NULL, NULL,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        ogs_ngap_GNB_ID_to_uint32(&targetGlobalGNB_ID->gNB_ID, &target_gnb_id);
        ogs_ngap_ASN_to_5gs_tai(&targetRANNodeID->selectedTAI, &target_tai);

        sourceRANNodeID = &SONConfigurationTransfer->sourceRANNodeID;
        sourceGlobalRANNodeID = &sourceRANNodeID->globalRANNodeID;

        if (sourceGlobalRANNodeID->present !=
                NGAP_GlobalRANNodeID_PR_globalGNB_ID) {
            ogs_error("Not implemented sourceGlobalRANNodeID->present[%d]",
                    sourceGlobalRANNodeID->present);
            r = ngap_send_error_indication(gnb, NULL, NULL,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        sourceGlobalGNB_ID = sourceGlobalRANNodeID->choice.globalGNB_ID;
        if (!sourceGlobalGNB_ID) {
            ogs_error("No sourceGlobalGNB_ID");
            r = ngap_send_error_indication(gnb, NULL, NULL,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        ogs_ngap_GNB_ID_to_uint32(&sourceGlobalGNB_ID->gNB_ID, &source_gnb_id);
        ogs_ngap_ASN_to_5gs_tai(&sourceRANNodeID->selectedTAI, &source_tai);

        ogs_debug("    Target : GNB_ID[0x%x], TAC[%d]",
                target_gnb_id, target_tai.tac.v);
        ogs_debug("    Source : GNB_ID[0x%x], TAC[%d]",
                source_gnb_id, source_tai.tac.v);

        target_gnb = amf_gnb_find_by_gnb_id(target_gnb_id);
        if (!target_gnb) {
            ogs_error("Uplink RAN configuration transfer : "
                    "cannot find target gNB-id[0x%x]", target_gnb_id);
            r = ngap_send_error_indication(gnb, NULL, NULL,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        r = ngap_send_downlink_ran_configuration_transfer(
                target_gnb, SONConfigurationTransfer);
        ogs_expect(r == OGS_OK);
        /* ogs_asn_copy_ie() could be failed from received packet.
         * So we should not use ogs_assert(r != OGS_ERROR).*/
    }
}

void ngap_handle_path_switch_request(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, r;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;
    uint64_t amf_ue_ngap_id;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_PathSwitchRequest_t *PathSwitchRequest = NULL;

    NGAP_PathSwitchRequestIEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_UserLocationInformation_t *UserLocationInformation = NULL;
    NGAP_UserLocationInformationNR_t *UserLocationInformationNR = NULL;
    NGAP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;
    NGAP_PDUSessionResourceToBeSwitchedDLList_t
        *PDUSessionResourceToBeSwitchedDLList = NULL;

    NGAP_NRencryptionAlgorithms_t *nRencryptionAlgorithms = NULL;
    NGAP_NRintegrityProtectionAlgorithms_t
        *nRintegrityProtectionAlgorithms = NULL;
    NGAP_EUTRAencryptionAlgorithms_t *eUTRAencryptionAlgorithms = NULL;
    NGAP_EUTRAintegrityProtectionAlgorithms_t
        *eUTRAintegrityProtectionAlgorithms = NULL;
    uint16_t nr_ea = 0, nr_ia = 0, eutra_ea = 0, eutra_ia = 0;
    uint8_t nr_ea0 = 0, nr_ia0 = 0, eutra_ea0 = 0, eutra_ia0 = 0;

    NGAP_PDUSessionResourceToBeSwitchedDLItem_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    amf_nsmf_pdusession_sm_context_param_t param;

    ogs_5gs_tai_t nr_tai;
    int served_tai_index = 0;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    PathSwitchRequest = &initiatingMessage->value.choice.PathSwitchRequest;
    ogs_assert(PathSwitchRequest);

    ogs_info("PathSwitchRequest");

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

    if (!RAN_UE_NGAP_ID) {
        ogs_error("No RAN_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!ran_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, (uint64_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    amf_ue = ran_ue->amf_ue;
    if (!amf_ue) {
        ogs_error("Cannot find AMF-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_info("    [OLD] RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] ",
        (long long)ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);
    ogs_info("    [OLD] TAC[%d] CellID[0x%llx]",
        amf_ue->nr_tai.tac.v, (long long)amf_ue->nr_cgi.cell_id);

    /* Update RAN-UE-NGAP-ID */
    ran_ue->ran_ue_ngap_id = *RAN_UE_NGAP_ID;

    /* Change ran_ue to the NEW gNB */
    ran_ue_switch_to_gnb(ran_ue, gnb);

    if (!UserLocationInformation) {
        ogs_error("No UserLocationInformation");
        r = ngap_send_error_indication2(ran_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (UserLocationInformation->present !=
            NGAP_UserLocationInformation_PR_userLocationInformationNR) {
        ogs_error("Not implemented UserLocationInformation[%d]",
                UserLocationInformation->present);
        r = ngap_send_error_indication2(ran_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_unspecified);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!UESecurityCapabilities) {
        ogs_error("No UESecurityCapabilities");
        r = ngap_send_error_indication2(ran_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!PDUSessionResourceToBeSwitchedDLList) {
        ogs_error("No PDUSessionResourceToBeSwitchedDLList");
        r = ngap_send_error_indication2(ran_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!SECURITY_CONTEXT_IS_VALID(amf_ue)) {
        ogs_error("No Security Context");
        r = ngap_send_error_indication2(ran_ue,
                NGAP_Cause_PR_nas, NGAP_CauseNas_authentication_failure);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_info("    [NEW] RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] ",
        (long long)ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    UserLocationInformationNR =
            UserLocationInformation->choice.userLocationInformationNR;
    ogs_assert(UserLocationInformationNR);
    ogs_ngap_ASN_to_5gs_tai(&UserLocationInformationNR->tAI, &nr_tai);

    served_tai_index = amf_find_served_tai(&nr_tai);
    if (served_tai_index < 0) {
        ogs_error("Cannot find Served TAI[PLMN_ID:%06x,TAC:%d]",
            ogs_plmn_id_hexdump(&nr_tai.plmn_id), nr_tai.tac.v);
        r = ngap_send_error_indication(
                gnb, &ran_ue->ran_ue_ngap_id, &ran_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_protocol,
                NGAP_CauseProtocol_message_not_compatible_with_receiver_state);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);

    ogs_ngap_ASN_to_nr_cgi(
            &UserLocationInformationNR->nR_CGI, &ran_ue->saved.nr_cgi);
    ogs_ngap_ASN_to_5gs_tai(
            &UserLocationInformationNR->tAI, &ran_ue->saved.nr_tai);

    /* Copy Stream-No/TAI/ECGI from ran_ue */
    amf_ue->gnb_ostream_id = ran_ue->gnb_ostream_id;
    memcpy(&amf_ue->nr_tai, &ran_ue->saved.nr_tai, sizeof(ogs_5gs_tai_t));
    memcpy(&amf_ue->nr_cgi, &ran_ue->saved.nr_cgi, sizeof(ogs_nr_cgi_t));

    ogs_info("    [NEW] TAC[%d] CellID[0x%llx]",
        amf_ue->nr_tai.tac.v, (long long)amf_ue->nr_cgi.cell_id);

    nRencryptionAlgorithms = &UESecurityCapabilities->nRencryptionAlgorithms;
    nRintegrityProtectionAlgorithms =
        &UESecurityCapabilities->nRintegrityProtectionAlgorithms;
    eUTRAencryptionAlgorithms =
        &UESecurityCapabilities->eUTRAencryptionAlgorithms;
    eUTRAintegrityProtectionAlgorithms =
        &UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms;

    memcpy(&nr_ea, nRencryptionAlgorithms->buf, sizeof(nr_ea));
    nr_ea = be16toh(nr_ea);
    nr_ea0 = amf_ue->ue_security_capability.nr_ea0;
    amf_ue->ue_security_capability.nr_ea = nr_ea >> 9;
    amf_ue->ue_security_capability.nr_ea0 = nr_ea0;

    memcpy(&nr_ia, nRintegrityProtectionAlgorithms->buf, sizeof(nr_ia));
    nr_ia = be16toh(nr_ia);
    nr_ia0 = amf_ue->ue_security_capability.nr_ia0;
    amf_ue->ue_security_capability.nr_ia = nr_ia >> 9;
    amf_ue->ue_security_capability.nr_ia0 = nr_ia0;

    memcpy(&eutra_ea, eUTRAencryptionAlgorithms->buf, sizeof(eutra_ea));
    eutra_ea = be16toh(eutra_ea);
    eutra_ea0 = amf_ue->ue_security_capability.eutra_ea0;
    amf_ue->ue_security_capability.eutra_ea = eutra_ea >> 9;
    amf_ue->ue_security_capability.eutra_ea0 = eutra_ea0;

    memcpy(&eutra_ia,
            eUTRAintegrityProtectionAlgorithms->buf, sizeof(eutra_ia));
    eutra_ia = be16toh(eutra_ia);
    eutra_ia0 = amf_ue->ue_security_capability.eutra_ia0;
    amf_ue->ue_security_capability.eutra_ia = eutra_ia >> 9;
    amf_ue->ue_security_capability.eutra_ia0 = eutra_ia0;

    /* Update Security Context (NextHop) */
    amf_ue->nhcc++;
    ogs_kdf_nh_gnb(amf_ue->kamf, amf_ue->nh, amf_ue->nh);

    for (i = 0; i < PDUSessionResourceToBeSwitchedDLList->list.count; i++) {
        amf_sess_t *sess = NULL;
        PDUSessionItem = (NGAP_PDUSessionResourceToBeSwitchedDLItem_t *)
            PDUSessionResourceToBeSwitchedDLList->list.array[i];

        if (!PDUSessionItem) {
            ogs_error("No NGAP_PDUSessionResourceToBeSwitchedDLItem");
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        transfer = &PDUSessionItem->pathSwitchRequestTransfer;
        if (!transfer) {
            ogs_error("No PDUSessionResourceSetupResponseTransfer");
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        if (PDUSessionItem->pDUSessionID ==
                OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
            ogs_error("PDU Session Identity is unassigned");
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        sess = amf_sess_find_by_psi(amf_ue, PDUSessionItem->pDUSessionID);
        if (!sess) {
            ogs_error("Cannot find PDU Session ID [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        if (!SESSION_CONTEXT_IN_SMF(sess)) {
            ogs_error("Session Context is not in SMF [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            r = ngap_send_error_indication2(ran_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        memset(&param, 0, sizeof(param));
        param.n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
        ogs_assert(param.n2smbuf);
        param.n2SmInfoType = OpenAPI_n2_sm_info_type_PATH_SWITCH_REQ;
        ogs_pkbuf_put_data(param.n2smbuf, transfer->buf, transfer->size);

        r = amf_sess_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                amf_nsmf_pdusession_build_update_sm_context,
                ran_ue, sess,
                AMF_UPDATE_SM_CONTEXT_PATH_SWITCH_REQUEST, &param);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

        ogs_pkbuf_free(param.n2smbuf);
    }
}

void ngap_handle_handover_required(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, r;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *source_ue = NULL, *target_ue = NULL;
    uint64_t amf_ue_ngap_id;

    amf_gnb_t *target_gnb = NULL;
    uint32_t target_gnb_id;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_HandoverRequired_t *HandoverRequired = NULL;

    NGAP_HandoverRequiredIEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_HandoverType_t *HandoverType = NULL;
    NGAP_Cause_t *Cause = NULL;
    NGAP_TargetID_t *TargetID = NULL;
    NGAP_TargetRANNodeID_t *targetRANNodeID = NULL;
    NGAP_GlobalRANNodeID_t *globalRANNodeID = NULL;
    NGAP_GlobalGNB_ID_t *globalGNB_ID = NULL;
    NGAP_PDUSessionResourceListHORqd_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceItemHORqd_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    NGAP_SourceToTarget_TransparentContainer_t
        *SourceToTarget_TransparentContainer = NULL;

    amf_nsmf_pdusession_sm_context_param_t param;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    HandoverRequired = &initiatingMessage->value.choice.HandoverRequired;
    ogs_assert(HandoverRequired);

    ogs_info("HandoverRequired");

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
        case NGAP_ProtocolIE_ID_id_PDUSessionResourceListHORqd:
            PDUSessionList = &ie->value.choice.PDUSessionResourceListHORqd;
            break;
        case NGAP_ProtocolIE_ID_id_SourceToTarget_TransparentContainer:
            SourceToTarget_TransparentContainer =
                &ie->value.choice.SourceToTarget_TransparentContainer;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    source_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!source_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, (uint64_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_debug("    Source : RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] ",
        (long long)source_ue->ran_ue_ngap_id,
        (long long)source_ue->amf_ue_ngap_id);

    amf_ue = source_ue->amf_ue;
    if (!amf_ue) {
        ogs_error("Cannot find AMF-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, &source_ue->ran_ue_ngap_id, &source_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!HandoverType) {
        ogs_error("No HandoverType");
        r = ngap_send_error_indication2(source_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!Cause) {
        ogs_error("No Cause");
        r = ngap_send_error_indication2(source_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!TargetID) {
        ogs_error("No TargetID");
        r = ngap_send_error_indication2(source_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (TargetID->present != NGAP_TargetID_PR_targetRANNodeID) {
        ogs_error("Not implemented TargetID[%d]", TargetID->present);
        r = ngap_send_error_indication2(source_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }
    targetRANNodeID = TargetID->choice.targetRANNodeID;
    if (!targetRANNodeID) {
        ogs_error("No targetRANNodeID");
        r = ngap_send_error_indication2(source_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    globalRANNodeID = &targetRANNodeID->globalRANNodeID;
    if (globalRANNodeID->present != NGAP_GlobalRANNodeID_PR_globalGNB_ID) {
        ogs_error("Not implemented globalRANNodeID[%d]",
                globalRANNodeID->present);
        r = ngap_send_error_indication2(source_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    globalGNB_ID = globalRANNodeID->choice.globalGNB_ID;
    if (!globalGNB_ID) {
        ogs_error("No globalGNB_ID");
        r = ngap_send_error_indication2(source_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_ngap_GNB_ID_to_uint32(&globalGNB_ID->gNB_ID, &target_gnb_id);
    target_gnb = amf_gnb_find_by_gnb_id(target_gnb_id);
    if (!target_gnb) {
        ogs_error("Handover required : cannot find target gNB-id[0x%x]",
                target_gnb_id);
        r = ngap_send_error_indication2(source_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!PDUSessionList) {
        ogs_error("No PDUSessionList");
        r = ngap_send_error_indication2(source_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!SourceToTarget_TransparentContainer) {
        ogs_error("No SourceToTarget_TransparentContainer");
        r = ngap_send_error_indication2(source_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!SECURITY_CONTEXT_IS_VALID(amf_ue)) {
        ogs_error("No Security Context");
        r = ngap_send_error_indication2(source_ue,
                NGAP_Cause_PR_nas, NGAP_CauseNas_authentication_failure);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    target_ue = ran_ue_cycle(source_ue->target_ue);
    if (target_ue) {
    /*
     * Issue #3014
     *
     * 1. HandoverRequired
     * 2. HandoverRequest
     * 3. HandoverFailure
     * 4. UEContextReleaseCommand
     * 5. HandoverPreparationFailure
     *
     * If UEContextReleaseComplete is not received,
     * the Source-UE will have the Target-UE.
     *
     * 6. HandoverRequired
     *
     * There may be cases where the Source UE has a Target UE
     * from a previous HandoverRequired process. In this case,
     * it is recommended to force the deletion of the Target UE information
     * when receiving a new HandoverRequired.
     *
     * 7. HandoverRequest
     * 8. HandoverFailure
     * 9. UEContextReleaseCommand
     * 10. UEContextReleaseComplete
     * 11. HandoverPreparationFailure
     *
     * ... Crashed ...
     */
        ogs_warn("DELETE the previously used TARGET in SOURCE");
        ogs_warn("    Source : RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] ",
            (long long)source_ue->ran_ue_ngap_id,
            (long long)source_ue->amf_ue_ngap_id);
        ogs_warn("    Target : RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] ",
            (long long)target_ue->ran_ue_ngap_id,
            (long long)target_ue->amf_ue_ngap_id);
        source_ue_deassociate_target_ue(target_ue);
        ran_ue_remove(target_ue);
    }

    /* Target UE */
    target_ue = ran_ue_add(target_gnb, INVALID_UE_NGAP_ID);
    if (target_ue == NULL) {
        r = ngap_send_error_indication2(source_ue, NGAP_Cause_PR_misc,
                NGAP_CauseMisc_control_processing_overload);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    /* Source UE - Target UE associated */
    source_ue_associate_target_ue(source_ue, target_ue);

    /* Context Transfer */
    target_ue->ue_context_requested = source_ue->ue_context_requested;
    target_ue->initial_context_setup_request_sent =
            source_ue->initial_context_setup_request_sent;
    target_ue->initial_context_setup_response_received =
            source_ue->initial_context_setup_response_received;

    target_ue->psimask.activated = source_ue->psimask.activated;

    ogs_debug("    Target : RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] ",
        (long long)target_ue->ran_ue_ngap_id,
        (long long)target_ue->amf_ue_ngap_id);

    /* Store HandoverType */
    amf_ue->handover.type = *HandoverType;

    /* Store Cause */
    amf_ue->handover.group = Cause->present;
    amf_ue->handover.cause = (int)Cause->choice.radioNetwork;

    /* Update Security Context (NextHop) */
    amf_ue->nhcc++;
    ogs_kdf_nh_gnb(amf_ue->kamf, amf_ue->nh, amf_ue->nh);

    /* Store Container */
    OGS_ASN_STORE_DATA(&amf_ue->handover.container,
            SourceToTarget_TransparentContainer);

    for (i = 0; i < PDUSessionList->list.count; i++) {
        amf_sess_t *sess = NULL;
        PDUSessionItem = (NGAP_PDUSessionResourceItemHORqd_t *)
            PDUSessionList->list.array[i];

        if (!PDUSessionItem) {
            ogs_error("No PDUSessionResourceItemHORqd");
            r = ngap_send_error_indication2(source_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        transfer = &PDUSessionItem->handoverRequiredTransfer;
        if (!transfer) {
            ogs_error("No handoverRequiredTransfer");
            r = ngap_send_error_indication2(source_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        if (PDUSessionItem->pDUSessionID ==
                OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
            ogs_error("PDU Session Identity is unassigned");
            r = ngap_send_error_indication2(source_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        sess = amf_sess_find_by_psi(amf_ue, PDUSessionItem->pDUSessionID);
        if (!sess) {
            ogs_error("Cannot find PDU Session ID [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            r = ngap_send_error_indication2(source_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        if (!SESSION_CONTEXT_IN_SMF(sess)) {
            ogs_error("Session Context is not in SMF [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            r = ngap_send_error_indication2(source_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        memset(&param, 0, sizeof(param));
        param.n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
        ogs_assert(param.n2smbuf);
        param.n2SmInfoType = OpenAPI_n2_sm_info_type_HANDOVER_REQUIRED;
        ogs_pkbuf_put_data(param.n2smbuf, transfer->buf, transfer->size);

        param.hoState = OpenAPI_ho_state_PREPARING;
        param.TargetID = TargetID;

        r = amf_sess_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                amf_nsmf_pdusession_build_update_sm_context,
                source_ue, sess,
                AMF_UPDATE_SM_CONTEXT_HANDOVER_REQUIRED, &param);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

        ogs_pkbuf_free(param.n2smbuf);
    }
}

void ngap_handle_handover_request_ack(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, r;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *source_ue = NULL, *target_ue = NULL;
    uint64_t amf_ue_ngap_id;
    amf_nsmf_pdusession_sm_context_param_t param;

    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_HandoverRequestAcknowledge_t *HandoverRequestAcknowledge = NULL;

    NGAP_HandoverRequestAcknowledgeIEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceAdmittedList_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceAdmittedItem_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    NGAP_TargetToSource_TransparentContainer_t
        *TargetToSource_TransparentContainer = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

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
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_PDUSessionResourceAdmittedList:
            PDUSessionList = &ie->value.choice.PDUSessionResourceAdmittedList;
            break;
        case NGAP_ProtocolIE_ID_id_TargetToSource_TransparentContainer:
            TargetToSource_TransparentContainer =
                &ie->value.choice.TargetToSource_TransparentContainer;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!RAN_UE_NGAP_ID) {
        ogs_error("No RAN_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    target_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!target_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, (uint64_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!RAN_UE_NGAP_ID) {
        ogs_error("No RAN_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    target_ue->ran_ue_ngap_id = *RAN_UE_NGAP_ID;

    source_ue = target_ue->source_ue;
    if (!source_ue) {
        ogs_error("Cannot find Source-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, &target_ue->ran_ue_ngap_id, &target_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_inconsistent_remote_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }
    amf_ue = target_ue->amf_ue;
    if (!amf_ue) {
        ogs_error("Cannot find AMF-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, &target_ue->ran_ue_ngap_id, &target_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_debug("    Source : RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] ",
        (long long)source_ue->ran_ue_ngap_id,
        (long long)source_ue->amf_ue_ngap_id);
    ogs_debug("    Target : RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] ",
        (long long)target_ue->ran_ue_ngap_id,
        (long long)target_ue->amf_ue_ngap_id);

    if (!PDUSessionList) {
        ogs_error("No PDUSessionList");
        r = ngap_send_error_indication2(target_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (!TargetToSource_TransparentContainer) {
        ogs_error("No TargetToSource_TransparentContainer");
        r = ngap_send_error_indication2(target_ue,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    /* Store Container */
    OGS_ASN_STORE_DATA(&amf_ue->handover.container,
            TargetToSource_TransparentContainer);

    for (i = 0; i < PDUSessionList->list.count; i++) {
        amf_sess_t *sess = NULL;
        PDUSessionItem = (NGAP_PDUSessionResourceAdmittedItem_t *)
            PDUSessionList->list.array[i];

        if (!PDUSessionItem) {
            ogs_error("No PDUSessionResourceAdmittedItem");
            r = ngap_send_error_indication2(target_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        transfer = &PDUSessionItem->handoverRequestAcknowledgeTransfer;
        if (!transfer) {
            ogs_error("No handoverRequestAcknowledgeTransfer");
            r = ngap_send_error_indication2(target_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        if (PDUSessionItem->pDUSessionID ==
                OGS_NAS_PDU_SESSION_IDENTITY_UNASSIGNED) {
            ogs_error("PDU Session Identity is unassigned");
            r = ngap_send_error_indication2(target_ue,
                    NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        sess = amf_sess_find_by_psi(amf_ue, PDUSessionItem->pDUSessionID);
        if (!sess) {
            ogs_error("Cannot find PDU Session ID [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            r = ngap_send_error_indication2(target_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        if (!SESSION_CONTEXT_IN_SMF(sess)) {
            ogs_error("Session Context is not in SMF [%d]",
                    (int)PDUSessionItem->pDUSessionID);
            r = ngap_send_error_indication2(target_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_unknown_PDU_session_ID);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        memset(&param, 0, sizeof(param));
        param.n2smbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
        ogs_assert(param.n2smbuf);
        param.n2SmInfoType = OpenAPI_n2_sm_info_type_HANDOVER_REQ_ACK;
        ogs_pkbuf_put_data(param.n2smbuf, transfer->buf, transfer->size);

        param.hoState = OpenAPI_ho_state_PREPARED;

        r = amf_sess_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                amf_nsmf_pdusession_build_update_sm_context,
                target_ue, sess,
                AMF_UPDATE_SM_CONTEXT_HANDOVER_REQ_ACK, &param);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

        ogs_pkbuf_free(param.n2smbuf);
    }
}

void ngap_handle_handover_failure(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, r;

    ran_ue_t *source_ue = NULL, *target_ue = NULL;
    uint64_t amf_ue_ngap_id;

    NGAP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    NGAP_HandoverFailure_t *HandoverFailure = NULL;

    NGAP_HandoverFailureIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_assert(message);
    unsuccessfulOutcome = message->choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    HandoverFailure =
        &unsuccessfulOutcome->value.choice.HandoverFailure;
    ogs_assert(HandoverFailure);

    ogs_debug("HandoverFailure");

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
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    target_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!target_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, NULL, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    source_ue = target_ue->source_ue;
    if (!source_ue) {
        ogs_error("Cannot find Source-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, &target_ue->ran_ue_ngap_id, &target_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_inconsistent_remote_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_debug("    Source : RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] ",
        (long long)source_ue->ran_ue_ngap_id,
        (long long)source_ue->amf_ue_ngap_id);
    ogs_debug("    Target : RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] ",
        (long long)target_ue->ran_ue_ngap_id,
        (long long)target_ue->amf_ue_ngap_id);

    if (!Cause) {
        ogs_error("No Cause");
        r = ngap_send_error_indication(
                gnb, &target_ue->ran_ue_ngap_id, &target_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }
    ogs_debug("    Cause[Group:%d Cause:%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

    r = ngap_send_handover_preparation_failure(source_ue, Cause);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    r = ngap_send_ran_ue_context_release_command(target_ue,
            NGAP_Cause_PR_radioNetwork, NGAP_CauseRadioNetwork_ho_failure_in_target_5GC_ngran_node_or_target_system,
            NGAP_UE_CTX_REL_NG_HANDOVER_FAILURE, 0);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);
}

void ngap_handle_handover_cancel(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, r;

    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;
    ran_ue_t *source_ue = NULL, *target_ue = NULL;
    uint64_t amf_ue_ngap_id;
    amf_nsmf_pdusession_sm_context_param_t param;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_HandoverCancel_t *HandoverCancel = NULL;

    NGAP_HandoverCancelIEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    HandoverCancel = &initiatingMessage->value.choice.HandoverCancel;
    ogs_assert(HandoverCancel);

    ogs_debug("HandoverCancel");

    for (i = 0; i < HandoverCancel->protocolIEs.list.count; i++) {
        ie = HandoverCancel->protocolIEs.list.array[i];
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
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    source_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!source_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, (uint64_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    target_ue = source_ue->target_ue;
    if (!target_ue) {
        ogs_error("Cannot find Source-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, &source_ue->ran_ue_ngap_id, &source_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_inconsistent_remote_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }
    amf_ue = source_ue->amf_ue;
    if (!amf_ue) {
        ogs_error("Cannot find AMF-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, &source_ue->ran_ue_ngap_id, &source_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_debug("    Source : RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] ",
        (long long)source_ue->ran_ue_ngap_id,
        (long long)source_ue->amf_ue_ngap_id);
    ogs_debug("    Target : RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] ",
        (long long)target_ue->ran_ue_ngap_id,
        (long long)target_ue->amf_ue_ngap_id);

    if (!Cause) {
        ogs_error("No Cause");
        r = ngap_send_error_indication(
                gnb, &source_ue->ran_ue_ngap_id, &source_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }
    ogs_debug("    Cause[Group:%d Cause:%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        if (!SESSION_CONTEXT_IN_SMF(sess)) {
            ogs_error("Session Context is not in SMF [%d]", sess->psi);
            r = ngap_send_error_indication2(source_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_handover_cancelled);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        memset(&param, 0, sizeof(param));
        param.hoState = OpenAPI_ho_state_CANCELLED;
        param.ngApCause.group = Cause->present;
        param.ngApCause.value = (int)Cause->choice.radioNetwork;

        r = amf_sess_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                amf_nsmf_pdusession_build_update_sm_context,
                source_ue, sess, AMF_UPDATE_SM_CONTEXT_HANDOVER_CANCEL, &param);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    }
}

void ngap_handle_uplink_ran_status_transfer(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, r;

    amf_ue_t *amf_ue = NULL;
    ran_ue_t *source_ue = NULL, *target_ue = NULL;
    uint64_t amf_ue_ngap_id;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UplinkRANStatusTransfer_t *UplinkRANStatusTransfer = NULL;

    NGAP_UplinkRANStatusTransferIEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RANStatusTransfer_TransparentContainer_t
        *RANStatusTransfer_TransparentContainer = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    UplinkRANStatusTransfer =
        &initiatingMessage->value.choice.UplinkRANStatusTransfer;
    ogs_assert(UplinkRANStatusTransfer);

    ogs_debug("UplinkRANStatusTransfer");

    for (i = 0; i < UplinkRANStatusTransfer->protocolIEs.list.count; i++) {
        ie = UplinkRANStatusTransfer->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_RANStatusTransfer_TransparentContainer:
            RANStatusTransfer_TransparentContainer =
                &ie->value.choice.RANStatusTransfer_TransparentContainer;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    source_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!source_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, (uint64_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    target_ue = source_ue->target_ue;
    if (!target_ue) {
        ogs_error("Cannot find Source-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, &source_ue->ran_ue_ngap_id, &source_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_inconsistent_remote_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }
    amf_ue = source_ue->amf_ue;
    if (!amf_ue) {
        ogs_error("Cannot find AMF-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, &source_ue->ran_ue_ngap_id, &source_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_debug("    Source : RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] ",
        (long long)source_ue->ran_ue_ngap_id,
        (long long)source_ue->amf_ue_ngap_id);
    ogs_debug("    Target : RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] ",
        (long long)target_ue->ran_ue_ngap_id,
        (long long)target_ue->amf_ue_ngap_id);

    r = ngap_send_downlink_ran_status_transfer(
            target_ue, RANStatusTransfer_TransparentContainer);
    ogs_expect(r == OGS_OK);
    /* ogs_asn_copy_ie() could be failed from received packet.
     * So we should not use ogs_assert(r != OGS_ERROR).*/
}

void ngap_handle_handover_notification(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, r;

    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;
    ran_ue_t *source_ue = NULL, *target_ue = NULL;
    uint64_t amf_ue_ngap_id;
    amf_nsmf_pdusession_sm_context_param_t param;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_HandoverNotify_t *HandoverNotify = NULL;

    NGAP_HandoverNotifyIEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_UserLocationInformation_t *UserLocationInformation = NULL;
    NGAP_UserLocationInformationNR_t *UserLocationInformationNR = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    HandoverNotify = &initiatingMessage->value.choice.HandoverNotify;
    ogs_assert(HandoverNotify);

    ogs_debug("HandoverNotify");

    for (i = 0; i < HandoverNotify->protocolIEs.list.count; i++) {
        ie = HandoverNotify->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_UserLocationInformation:
            UserLocationInformation = &ie->value.choice.UserLocationInformation;
            break;
        default:
            break;
        }
    }

    ogs_debug("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!AMF_UE_NGAP_ID) {
        ogs_error("No AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
        ogs_error("Invalid AMF_UE_NGAP_ID");
        r = ngap_send_error_indication(gnb, (uint64_t *)RAN_UE_NGAP_ID, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    target_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
    if (!target_ue) {
        ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, (uint64_t *)RAN_UE_NGAP_ID, &amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    source_ue = target_ue->source_ue;
    if (!source_ue) {
        ogs_error("Cannot find Source-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, &target_ue->ran_ue_ngap_id, &target_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_inconsistent_remote_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }
    amf_ue = target_ue->amf_ue;
    if (!amf_ue) {
        ogs_error("Cannot find AMF-UE Context [%lld]",
                (long long)amf_ue_ngap_id);
        r = ngap_send_error_indication(
                gnb, &target_ue->ran_ue_ngap_id, &target_ue->amf_ue_ngap_id,
                NGAP_Cause_PR_radioNetwork,
                NGAP_CauseRadioNetwork_unknown_local_UE_NGAP_ID);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    amf_ue_associate_ran_ue(amf_ue, target_ue);

    if (!UserLocationInformation) {
        ogs_error("No UserLocationInformation");
        r = ngap_send_error_indication(gnb, &target_ue->ran_ue_ngap_id, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    if (UserLocationInformation->present !=
            NGAP_UserLocationInformation_PR_userLocationInformationNR) {
        ogs_error("Not implemented UserLocationInformation[%d]",
                UserLocationInformation->present);
        r = ngap_send_error_indication(gnb, &target_ue->ran_ue_ngap_id, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_unspecified);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    UserLocationInformationNR =
        UserLocationInformation->choice.userLocationInformationNR;
    ogs_assert(UserLocationInformationNR);
    ogs_ngap_ASN_to_nr_cgi(
            &UserLocationInformationNR->nR_CGI, &target_ue->saved.nr_cgi);
    ogs_ngap_ASN_to_5gs_tai(
            &UserLocationInformationNR->tAI, &target_ue->saved.nr_tai);

    ogs_debug("    Source : RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] ",
        (long long)source_ue->ran_ue_ngap_id,
        (long long)source_ue->amf_ue_ngap_id);
    ogs_debug("    Source : TAC[%d] CellID[0x%llx]",
        source_ue->saved.nr_tai.tac.v,
        (long long)source_ue->saved.nr_cgi.cell_id);
    ogs_debug("    Target : RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld] ",
        (long long)target_ue->ran_ue_ngap_id,
        (long long)target_ue->amf_ue_ngap_id);
    ogs_debug("    Target : TAC[%d] CellID[0x%llx]",
        target_ue->saved.nr_tai.tac.v,
        (long long)target_ue->saved.nr_cgi.cell_id);

    /* Copy Stream-No/TAI/ECGI from ran_ue */
    amf_ue->gnb_ostream_id = target_ue->gnb_ostream_id;
    memcpy(&amf_ue->nr_tai, &target_ue->saved.nr_tai, sizeof(ogs_5gs_tai_t));
    memcpy(&amf_ue->nr_cgi, &target_ue->saved.nr_cgi, sizeof(ogs_nr_cgi_t));

    r = ngap_send_ran_ue_context_release_command(source_ue,
            NGAP_Cause_PR_radioNetwork,
            NGAP_CauseRadioNetwork_successful_handover,
            NGAP_UE_CTX_REL_NG_HANDOVER_COMPLETE,
            ogs_local_conf()->time.handover.duration);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        if (!SESSION_CONTEXT_IN_SMF(sess)) {
            ogs_error("Session Context is not in SMF [%d]", sess->psi);
            r = ngap_send_error_indication2(source_ue,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_partial_handover);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        memset(&param, 0, sizeof(param));
        param.hoState = OpenAPI_ho_state_COMPLETED;

        r = amf_sess_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NSMF_PDUSESSION, NULL,
                amf_nsmf_pdusession_build_update_sm_context,
                source_ue, sess, AMF_UPDATE_SM_CONTEXT_HANDOVER_NOTIFY, &param);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    }
}

void ngap_handle_ran_configuration_update(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, j, k, r;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_RANConfigurationUpdate_t *RANConfigurationUpdate = NULL;

    NGAP_RANConfigurationUpdateIEs_t *ie = NULL;
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
    RANConfigurationUpdate = &initiatingMessage->value.choice.RANConfigurationUpdate;
    ogs_assert(RANConfigurationUpdate);

    ogs_debug("RANConfigurationUpdate");

    for (i = 0; i < RANConfigurationUpdate->protocolIEs.list.count; i++) {
        ie = RANConfigurationUpdate->protocolIEs.list.array[i];
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

    if (GlobalRANNodeID) {
        globalGNB_ID = GlobalRANNodeID->choice.globalGNB_ID;
        if (!globalGNB_ID) {
            ogs_error("No globalGNB_ID");
            group = NGAP_Cause_PR_protocol;
            cause = NGAP_CauseProtocol_semantic_error;
            r = ngap_send_ran_configuration_update_failure(gnb, group, cause);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        ogs_ngap_GNB_ID_to_uint32(&globalGNB_ID->gNB_ID, &gnb_id);
        ogs_debug("    IP[%s] GNB_ID[0x%x]",
                OGS_ADDR(gnb->sctp.addr, buf), gnb_id);

        amf_gnb_set_gnb_id(gnb, gnb_id);
    }

    if (SupportedTAList) {
        /* Parse Supported TA */
        for (i = 0, gnb->num_of_supported_ta_list = 0;
                i < SupportedTAList->list.count &&
                gnb->num_of_supported_ta_list < OGS_MAX_NUM_OF_TAI;
                    i++) {
            NGAP_SupportedTAItem_t *SupportedTAItem = NULL;

            if (gnb->num_of_supported_ta_list >=
                    OGS_ARRAY_SIZE(gnb->supported_ta_list)) {
                ogs_error("OVERFLOW GNB->num_of_supported_ta_list "
                        "[%d:%d:%d]",
                        gnb->num_of_supported_ta_list,
                        OGS_MAX_NUM_OF_SUPPORTED_TA,
                        (int)OGS_ARRAY_SIZE(gnb->supported_ta_list));
                break;
            }

            SupportedTAItem = (NGAP_SupportedTAItem_t *)
                    SupportedTAList->list.array[i];
            if (!SupportedTAItem) {
                ogs_error("No SupportedTAItem");
                group = NGAP_Cause_PR_protocol;
                cause = NGAP_CauseProtocol_semantic_error;
                r = ngap_send_ran_configuration_update_failure(
                        gnb, group, cause);
                ogs_expect(r == OGS_OK);
                ogs_assert(r != OGS_ERROR);
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

                if (gnb->supported_ta_list[i].num_of_bplmn_list >=
                        OGS_ARRAY_SIZE(gnb->supported_ta_list[i].bplmn_list)) {
                    ogs_error("OVERFLOW GNB->supported_ta_list."
                            "num_of_bplm_list [%d:%d:%d]",
                            gnb->supported_ta_list[i].num_of_bplmn_list,
                            OGS_MAX_NUM_OF_BPLMN,
                            (int)OGS_ARRAY_SIZE(
                                gnb->supported_ta_list[i].bplmn_list));
                    break;
                }

                BroadcastPLMNItem = (NGAP_BroadcastPLMNItem_t *)
                        SupportedTAItem->broadcastPLMNList.list.array[j];
                if (!BroadcastPLMNItem) {
                    ogs_error("No BroadcastPLMNItem");
                    group = NGAP_Cause_PR_protocol;
                    cause = NGAP_CauseProtocol_semantic_error;
                    r = ngap_send_ran_configuration_update_failure(
                            gnb, group, cause);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
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
                            OGS_MAX_NUM_OF_SLICE_SUPPORT;
                                k++) {
                    NGAP_SliceSupportItem_t *SliceSupportItem = NULL;
                    NGAP_S_NSSAI_t *s_NSSAI = NULL;

                    if (gnb->supported_ta_list[i].
                            bplmn_list[j].num_of_s_nssai >=
                            OGS_ARRAY_SIZE(gnb->supported_ta_list[i].
                                bplmn_list[j].s_nssai)) {
                        ogs_error("OVERFLOW GNB->num_of_supported_ta_list."
                                "bplmn_list.num_of_s_nssai"
                                "[%d:%d:%d]",
                                gnb->supported_ta_list[i].bplmn_list[j].
                                    num_of_s_nssai,
                                OGS_MAX_NUM_OF_SLICE_SUPPORT,
                                (int)OGS_ARRAY_SIZE(gnb->supported_ta_list[i].
                                    bplmn_list[j].s_nssai));
                        break;
                    }

                    SliceSupportItem = (NGAP_SliceSupportItem_t *)
                        BroadcastPLMNItem->tAISliceSupportList.list.array[k];
                    if (!SliceSupportItem) {
                        ogs_error("No SliceSupportItem");
                        group = NGAP_Cause_PR_protocol;
                        cause = NGAP_CauseProtocol_semantic_error;
                        r = ngap_send_ran_configuration_update_failure(
                                gnb, group, cause);
                        ogs_expect(r == OGS_OK);
                        ogs_assert(r != OGS_ERROR);
                        return;
                    }

                    s_NSSAI = &SliceSupportItem->s_NSSAI;
                    ogs_assert(s_NSSAI);

                    ogs_asn_OCTET_STRING_to_uint8(&s_NSSAI->sST,
                        &gnb->supported_ta_list[i].
                            bplmn_list[j].s_nssai[k].sst);
                    if (!s_NSSAI->sD) {
                        gnb->supported_ta_list[i].bplmn_list[j].
                            s_nssai[k].sd.v = OGS_S_NSSAI_NO_SD_VALUE;
                    } else {
                        ogs_asn_OCTET_STRING_to_uint24(s_NSSAI->sD,
                        &gnb->supported_ta_list[i].bplmn_list[j].s_nssai[k].sd);
                    }

                    ogs_debug("    S_NSSAI[SST:%d SD:0x%x]",
                        gnb->supported_ta_list[i].bplmn_list[j].s_nssai[k].sst,
                        gnb->supported_ta_list[i].bplmn_list[j].
                            s_nssai[k].sd.v);

                    gnb->supported_ta_list[i].bplmn_list[j].num_of_s_nssai++;
                }

                gnb->supported_ta_list[i].num_of_bplmn_list++;
            }

            gnb->num_of_supported_ta_list++;
        }

        if (gnb->num_of_supported_ta_list == 0) {
            ogs_warn("RANConfigurationUpdate failure:");
            ogs_warn("    No supported TA exist in request");
            group = NGAP_Cause_PR_protocol;
            cause =
                NGAP_CauseProtocol_message_not_compatible_with_receiver_state;

            r = ngap_send_ran_configuration_update_failure(gnb, group, cause);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        if (!served_tai_is_found(gnb)) {
            ogs_warn("RANConfigurationUpdate failure:");
            ogs_warn("    Cannot find Served TAI. "
                        "Check 'amf.tai' configuration");
            group = NGAP_Cause_PR_misc;
            cause = NGAP_CauseMisc_unknown_PLMN_or_SNPN;

            r = ngap_send_ran_configuration_update_failure(gnb, group, cause);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }

        if (!s_nssai_is_found(gnb)) {
            ogs_warn("RANConfigurationUpdate failure:");
            ogs_warn("    Cannot find S_NSSAI. "
                        "Check 'amf.plmn_support.s_nssai' configuration");
            group = NGAP_Cause_PR_radioNetwork;
            cause = NGAP_CauseRadioNetwork_slice_not_supported;

            r = ngap_send_ran_configuration_update_failure(gnb, group, cause);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
            return;
        }
    }

    if (PagingDRX)
        ogs_debug("    PagingDRX[%ld]", *PagingDRX);

    ogs_expect(OGS_OK == ngap_send_ran_configuration_update_ack(gnb));
}

void ngap_handle_ng_reset(
        amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i, r, old_xact_count = 0, new_xact_count = 0;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_NGReset_t *NGReset = NULL;

    NGAP_NGResetIEs_t *ie = NULL;
    NGAP_Cause_t *Cause = NULL;
    NGAP_ResetType_t *ResetType = NULL;
    NGAP_UE_associatedLogicalNG_connectionList_t *partOfNG_Interface = NULL;

    ran_ue_t *iter = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    NGReset = &initiatingMessage->value.choice.NGReset;
    ogs_assert(NGReset);

    ogs_warn("NGReset");

    for (i = 0; i < NGReset->protocolIEs.list.count; i++) {
        ie = NGReset->protocolIEs.list.array[i];
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
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (!Cause) {
        ogs_error("No Cause");
        r = ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    ogs_warn("    Cause[Group:%d Cause:%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

    if (!ResetType) {
        ogs_error("No ResetType");
        r = ngap_send_error_indication(gnb, NULL, NULL,
                NGAP_Cause_PR_protocol, NGAP_CauseProtocol_semantic_error);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        return;
    }

    switch (ResetType->present) {
    case NGAP_ResetType_PR_nG_Interface:
        ogs_warn("    NGAP_ResetType_PR_nG_Interface");

        amf_sbi_send_deactivate_all_ue_in_gnb(
                gnb, AMF_REMOVE_S1_CONTEXT_BY_RESET_ALL);

    /*
     * TS38.413
     *
     * 8.7.4.2.1 NG Reset initiated by the AMF
     * At reception of the NG RESET message the NG-RAN node shall release all
     * allocated resources on NG and Uu related to the UE association(s)
     * indicated explicitly or implicitly in the NG RESET message and
     * remove the indicated UE contexts including NGAP ID.
     *
     * After the NG-RAN node has released all assigned NG resources and
     * the UE NGAP IDs for all indicated UE associations which can be used
     * for new UE-associated logical NG-connections over the NG interface,
     * the NG-RAN node shall respond with the NG RESET ACKNOWLEDGE message.
     *
     * The NG-RAN node does not need to wait for the release of radio resources
     * to be completed before returning the NG RESET ACKNOWLEDGE message.
     *
     * 8.7.4.2.2 NG Reset initiated by the NG-RAN node
     *
     * At reception of the NG RESET message the AMF shall release
     * all allocated resources on NG related to the UE association(s)
     * indicated explicitly or implicitly in the NG RESET message and
     * remove the NGAP ID for the indicated UE associations.
     *
     * After the AMF has released all assigned NG resources and
     * the UE NGAP IDs for all indicated UE associations which can be used
     * for new UE-associated logical NG-connections over the NG interface,
     * the AMF shall respond with the NG RESET ACKNOWLEDGE message.
     */
        if (ogs_list_count(&gnb->ran_ue_list) == 0) {
            r = ngap_send_ng_reset_ack(gnb, NULL);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }

        break;

    case NGAP_ResetType_PR_partOfNG_Interface:
        ogs_warn("    NGAP_ResetType_PR_partOfNG_Interface");

        partOfNG_Interface = ResetType->choice.partOfNG_Interface;
        ogs_assert(partOfNG_Interface);

        if (gnb->ng_reset_ack)
            ogs_pkbuf_free(gnb->ng_reset_ack);

        gnb->ng_reset_ack = ogs_ngap_build_ng_reset_ack(partOfNG_Interface);
        if (!gnb->ng_reset_ack) {
            ogs_error("ogs_ngap_build_ng_reset_ack() failed");
            return;
        }

        for (i = 0; i < partOfNG_Interface->list.count; i++) {
            NGAP_UE_associatedLogicalNG_connectionItem_t *item = NULL;
            uint64_t amf_ue_ngap_id;

            ran_ue_t *ran_ue = NULL;
            amf_ue_t *amf_ue = NULL;

            item = (NGAP_UE_associatedLogicalNG_connectionItem_t *)
                        partOfNG_Interface->list.array[i];
            if (!item) {
                ogs_error("No ResetType");
                continue;
            }

            if (item->aMF_UE_NGAP_ID) {
                if (asn_INTEGER2uint64(item->aMF_UE_NGAP_ID,
                            &amf_ue_ngap_id) != 0) {
                    ogs_error("Invalid AMF_UE_NGAP_ID");
                    continue;
                }

                ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);

                if (!ran_ue) {
                    ogs_error("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                            (long long)amf_ue_ngap_id);
                    continue;
                }

            } else if (item->rAN_UE_NGAP_ID) {

                ran_ue = ran_ue_find_by_ran_ue_ngap_id(
                            gnb, *item->rAN_UE_NGAP_ID);

                if (!ran_ue) {
                    ogs_error("No RAN UE Context : RAN_UE_NGAP_ID[%d]",
                            (int)*item->rAN_UE_NGAP_ID);
                    continue;
                }
            } else {
                ogs_error("No UE NGAP ID");
                continue;
            }

            ogs_assert(ran_ue);

            /* RAN_UE Context where PartOfNG_interface was requested */
            ran_ue->part_of_ng_reset_requested = true;

            amf_ue = ran_ue->amf_ue;
            /*
             * Issues #1928
             *
             * By ran_ue_deassociate(amf_ue->ran_ue),
             * 'ran_ue->amf_ue' could be NULL.
             *
             * As such, we should not use the ogs_assert() like the following.
             * ogs_assert(ran_ue->amf_ue);
             */
            if (amf_ue) {
                old_xact_count = amf_sess_xact_count(amf_ue);

                amf_sbi_send_deactivate_all_sessions(
                    ran_ue, amf_ue, AMF_REMOVE_S1_CONTEXT_BY_RESET_PARTIAL,
                    NGAP_Cause_PR_radioNetwork,
                    NGAP_CauseRadioNetwork_failure_in_radio_interface_procedure);

                new_xact_count = amf_sess_xact_count(amf_ue);
            } else {
                ogs_warn("UE(amf-ue) context has already been removed");
                ogs_warn("   AMF_UE_NGAP_ID[%lld] RAN_UE_NGAP_ID[%lld]",
                            (long long)ran_ue->amf_ue_ngap_id,
                            (long long)ran_ue->ran_ue_ngap_id);
            }

            if (old_xact_count == new_xact_count) ran_ue_remove(ran_ue);
        }

        ogs_list_for_each(&gnb->ran_ue_list, iter) {
            if (iter->part_of_ng_reset_requested == true) {
                /* The GNB_UE context
                 * where PartOfNG_interface was requested
                 * still remains */
                return;
            }
        }

        /* All GNB_UE context
         * where PartOfNG_interface was requested
         * REMOVED */
        ogs_assert(gnb->ng_reset_ack);
        r = ngap_send_to_gnb(gnb, gnb->ng_reset_ack, NGAP_NON_UE_SIGNALLING);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

        /* Clear NG-Reset Ack Buffer */
        gnb->ng_reset_ack = NULL;
        break;
    default:
        ogs_warn("Invalid ResetType[%d]", ResetType->present);
        break;
    }
}

void ngap_handle_error_indication(amf_gnb_t *gnb, ogs_ngap_message_t *message)
{
    int i;
    char buf[OGS_ADDRSTRLEN];
    uint64_t amf_ue_ngap_id;

    ran_ue_t *ran_ue = NULL;

    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_ErrorIndication_t *ErrorIndication = NULL;

    NGAP_ErrorIndicationIEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    ogs_assert(gnb);
    ogs_assert(gnb->sctp.sock);

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    ErrorIndication = &initiatingMessage->value.choice.ErrorIndication;
    ogs_assert(ErrorIndication);

    ogs_warn("ErrorIndication");

    for (i = 0; i < ErrorIndication->protocolIEs.list.count; i++) {
        ie = ErrorIndication->protocolIEs.list.array[i];
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

    ogs_warn("    IP[%s] RAN_ID[%d]",
            OGS_ADDR(gnb->sctp.addr, buf), gnb->gnb_id);

    if (AMF_UE_NGAP_ID) {

        if (asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id) != 0) {
            ogs_warn("Invalid AMF_UE_NGAP_ID");
        }

        ran_ue = ran_ue_find_by_amf_ue_ngap_id(amf_ue_ngap_id);
        if (!ran_ue)
            ogs_warn("No RAN UE Context : AMF_UE_NGAP_ID[%lld]",
                    (long long)amf_ue_ngap_id);
        else
            ogs_warn("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
                    (long long)ran_ue->ran_ue_ngap_id,
                    (long long)ran_ue->amf_ue_ngap_id);

    } else if (RAN_UE_NGAP_ID) {
        ran_ue = ran_ue_find_by_ran_ue_ngap_id(gnb, *RAN_UE_NGAP_ID);
        if (!ran_ue)
            ogs_warn("No RAN UE Context : RAN_UE_NGAP_ID[%d]",
                    (int)*RAN_UE_NGAP_ID);
        else
            ogs_warn("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
                    (long long)ran_ue->ran_ue_ngap_id,
                    (long long)ran_ue->amf_ue_ngap_id);
    }

    if (Cause) {
        ogs_warn("    Cause[Group:%d Cause:%d]",
                Cause->present, (int)Cause->choice.radioNetwork);
    }
}
