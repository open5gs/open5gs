/*
 * Copyright (C) 2019-2026 by Sukchan Lee <acetcom@gmail.com>
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

#include "mme-context.h"

#include "mme-sm.h"
#include "s1ap-build.h"
#include "S1AP_E-RABToBeSwitchedULList.h"
#include "S1AP_E-RABSubjecttoDataForwardingList.h"
#include "S1AP_E-RABToBeSetupListHOReq.h"

ogs_pkbuf_t *s1ap_build_setup_rsp(void)
{
    int i, j;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_S1SetupResponse_t *S1SetupResponse = NULL;

    S1AP_S1SetupResponseIEs_t *ie = NULL;
    S1AP_ServedGUMMEIs_t *ServedGUMMEIs = NULL;
    S1AP_RelativeMMECapacity_t *RelativeMMECapacity = NULL;
    S1AP_MMEname_t *MMEname = NULL;

    ogs_debug("S1SetupResponse");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(*successfulOutcome));

    successfulOutcome = pdu.choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_S1Setup;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_S1SetupResponse;

    successfulOutcome->value.choice.S1SetupResponse =
        CALLOC(1, sizeof(*S1SetupResponse));
    S1SetupResponse = successfulOutcome->value.choice.S1SetupResponse;
    ogs_assert(S1SetupResponse);

    S1SetupResponse->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_S1SetupResponse);
    ogs_assert(S1SetupResponse->protocolIEs);

    if (mme_self()->mme_name) {
        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(S1SetupResponse->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_MMEname;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present = S1AP_S1SetupResponseIEs__value_PR_MMEname;

        ie->value.choice.MMEname = CALLOC(1, sizeof(*MMEname));
        MMEname = ie->value.choice.MMEname;
        ogs_assert(MMEname);
    }

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(S1SetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_ServedGUMMEIs;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_S1SetupResponseIEs__value_PR_ServedGUMMEIs;

    ie->value.choice.ServedGUMMEIs =
        CALLOC(1, sizeof(*ServedGUMMEIs));
    ServedGUMMEIs = ie->value.choice.ServedGUMMEIs;
    ogs_assert(ServedGUMMEIs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(S1SetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_RelativeMMECapacity;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_S1SetupResponseIEs__value_PR_RelativeMMECapacity;

    ie->value.choice.RelativeMMECapacity = CALLOC(1, sizeof(*RelativeMMECapacity));
    RelativeMMECapacity = ie->value.choice.RelativeMMECapacity;
    ogs_assert(RelativeMMECapacity);

    if (MMEname) {
        ogs_asn_buffer_to_OCTET_STRING((char*)mme_self()->mme_name,
                strlen(mme_self()->mme_name), MMEname);
    }

    for (i = 0; i < mme_self()->num_of_served_gummei; i++) {
        S1AP_ServedGUMMEIsItem_t *ServedGUMMEIsItem = NULL;
        ServedGUMMEIsItem =
            CALLOC(1, sizeof(*ServedGUMMEIsItem));
        ogs_assert(ServedGUMMEIsItem);
        ServedGUMMEIsItem->servedPLMNs =
            CALLOC(1, sizeof(*ServedGUMMEIsItem->servedPLMNs));
        ogs_assert(ServedGUMMEIsItem->servedPLMNs);
        ServedGUMMEIsItem->servedGroupIDs =
            CALLOC(1, sizeof(*ServedGUMMEIsItem->servedGroupIDs));
        ogs_assert(ServedGUMMEIsItem->servedGroupIDs);
        ServedGUMMEIsItem->servedMMECs =
            CALLOC(1, sizeof(*ServedGUMMEIsItem->servedMMECs));
        ogs_assert(ServedGUMMEIsItem->servedMMECs);

        served_gummei_t *served_gummei = &mme_self()->served_gummei[i];
        for (j = 0; j < served_gummei->num_of_plmn_id; j++) {
            S1AP_PLMNidentity_t *PLMNidentity = NULL;
            PLMNidentity = CALLOC(1, sizeof(*PLMNidentity));
            ogs_assert(PLMNidentity);
            ogs_s1ap_buffer_to_OCTET_STRING(
                    &served_gummei->plmn_id[j], OGS_PLMN_ID_LEN, PLMNidentity);
            ASN_SEQUENCE_ADD(
                    ServedGUMMEIsItem->servedPLMNs, PLMNidentity);
            ogs_debug("    PLMN_ID[MCC:%d MNC:%d]",
                ogs_plmn_id_mcc(&served_gummei->plmn_id[j]),
                ogs_plmn_id_mnc(&served_gummei->plmn_id[j]));
        }

        for (j = 0; j < served_gummei->num_of_mme_gid; j++) {
            S1AP_MME_Group_ID_t *MME_Group_ID = NULL;
            MME_Group_ID = CALLOC(1, sizeof(*MME_Group_ID));
            ogs_assert(MME_Group_ID);
            ogs_asn_uint16_to_OCTET_STRING(
                    served_gummei->mme_gid[j], MME_Group_ID);
            ASN_SEQUENCE_ADD(
                    ServedGUMMEIsItem->servedGroupIDs, MME_Group_ID);
            ogs_debug("    MME Group[%d]", served_gummei->mme_gid[j]);
        }

        for (j = 0; j < served_gummei->num_of_mme_code; j++) {
            S1AP_MME_Code_t *MME_Code = NULL ;
            MME_Code = CALLOC(1, sizeof(*MME_Code));
            ogs_assert(MME_Code);
            ogs_asn_uint8_to_OCTET_STRING(
                    served_gummei->mme_code[j], MME_Code);
            ASN_SEQUENCE_ADD(ServedGUMMEIsItem->servedMMECs, MME_Code);
            ogs_debug("    MME Code[%d]", served_gummei->mme_code[j]);
        }
        ASN_SEQUENCE_ADD(ServedGUMMEIs, ServedGUMMEIsItem);
    }

    *RelativeMMECapacity = mme_self()->relative_capacity;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_setup_failure(
        S1AP_Cause_PR group, long cause, long time_to_wait)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    S1AP_S1SetupFailure_t *S1SetupFailure = NULL;

    S1AP_S1SetupFailureIEs_t *ie = NULL;
    S1AP_Cause_t *Cause = NULL;
    S1AP_TimeToWait_t *TimeToWait = NULL;

    ogs_debug("S1SetupFailure");

    ogs_debug("    Group[%d] Cause[%d] TimeToWait[%ld]",
            group, (int)cause, time_to_wait);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_unsuccessfulOutcome;
    pdu.choice.unsuccessfulOutcome =
        CALLOC(1, sizeof(*unsuccessfulOutcome));

    unsuccessfulOutcome = pdu.choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    unsuccessfulOutcome->procedureCode = S1AP_ProcedureCode_id_S1Setup;
    unsuccessfulOutcome->criticality = S1AP_Criticality_reject;
    unsuccessfulOutcome->value.present =
        S1AP_UnsuccessfulOutcome__value_PR_S1SetupFailure;

    unsuccessfulOutcome->value.choice.S1SetupFailure =
        CALLOC(1, sizeof(*S1SetupFailure));
    S1SetupFailure = unsuccessfulOutcome->value.choice.S1SetupFailure;
    ogs_assert(S1SetupFailure);

    S1SetupFailure->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_S1SetupFailure);
    ogs_assert(S1SetupFailure->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(S1SetupFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_S1SetupFailureIEs__value_PR_Cause;

    ie->value.choice.Cause =
        CALLOC(1, sizeof(*Cause));
    Cause = ie->value.choice.Cause;
    ogs_assert(Cause);

    if (time_to_wait > -1) {
        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(S1SetupFailure->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_TimeToWait;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present = S1AP_S1SetupFailureIEs__value_PR_TimeToWait;

        ie->value.choice.TimeToWait = CALLOC(1, sizeof(*TimeToWait));
        TimeToWait = ie->value.choice.TimeToWait;
        ogs_assert(TimeToWait);
    }

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    if (TimeToWait)
        *TimeToWait = time_to_wait;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_enb_configuration_update_ack(void)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;

    S1AP_ENBConfigurationUpdateAcknowledge_t
        *ENBConfigurationUpdateAcknowledge = NULL;

    ogs_debug("ENBConfigurationUpdateAcknowledge");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(*successfulOutcome));

    successfulOutcome = pdu.choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    successfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_ENBConfigurationUpdate;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_ENBConfigurationUpdateAcknowledge;

    successfulOutcome->value.choice.ENBConfigurationUpdateAcknowledge =
        CALLOC(1, sizeof(*ENBConfigurationUpdateAcknowledge));
    ENBConfigurationUpdateAcknowledge =
        successfulOutcome->value.choice.ENBConfigurationUpdateAcknowledge;
    ogs_assert(ENBConfigurationUpdateAcknowledge);

    ENBConfigurationUpdateAcknowledge->protocolIEs =
        ogs_asn_calloc_protocol_ies(
                &asn_DEF_S1AP_ENBConfigurationUpdateAcknowledge);
    ogs_assert(ENBConfigurationUpdateAcknowledge->protocolIEs);

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_enb_configuration_update_failure(
    S1AP_Cause_PR group, long cause, long time_to_wait)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    S1AP_ENBConfigurationUpdateFailure_t *ENBConfigurationUpdateFailure = NULL;

    S1AP_ENBConfigurationUpdateFailureIEs_t *ie = NULL;
    S1AP_Cause_t *Cause = NULL;
    S1AP_TimeToWait_t *TimeToWait = NULL;

    ogs_debug("ENBConfigurationUpdateFailure");

    ogs_debug("    Group[%d] Cause[%d] TimeToWait[%ld]",
            group, (int)cause, time_to_wait);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_unsuccessfulOutcome;
    pdu.choice.unsuccessfulOutcome =
        CALLOC(1, sizeof(*unsuccessfulOutcome));

    unsuccessfulOutcome = pdu.choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    unsuccessfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_ENBConfigurationUpdate;
    unsuccessfulOutcome->criticality = S1AP_Criticality_reject;
    unsuccessfulOutcome->value.present =
        S1AP_UnsuccessfulOutcome__value_PR_ENBConfigurationUpdateFailure;

    unsuccessfulOutcome->value.choice.ENBConfigurationUpdateFailure =
        CALLOC(1, sizeof(*ENBConfigurationUpdateFailure));
    ENBConfigurationUpdateFailure = unsuccessfulOutcome->value.choice.ENBConfigurationUpdateFailure;
    ogs_assert(ENBConfigurationUpdateFailure);

    ENBConfigurationUpdateFailure->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_ENBConfigurationUpdateFailure);
    ogs_assert(ENBConfigurationUpdateFailure->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(ENBConfigurationUpdateFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_ENBConfigurationUpdateFailureIEs__value_PR_Cause;

    ie->value.choice.Cause =
        CALLOC(1, sizeof(*Cause));
    Cause = ie->value.choice.Cause;
    ogs_assert(Cause);

    if (time_to_wait > -1) {
        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(ENBConfigurationUpdateFailure->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_TimeToWait;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_ENBConfigurationUpdateFailureIEs__value_PR_TimeToWait;

        ie->value.choice.TimeToWait = CALLOC(1, sizeof(*TimeToWait));
        TimeToWait = ie->value.choice.TimeToWait;
        ogs_assert(TimeToWait);
    }

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    if (TimeToWait)
        *TimeToWait = time_to_wait;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_downlink_nas_transport(
    enb_ue_t *enb_ue, ogs_pkbuf_t *emmbuf)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_DownlinkNASTransport_t *DownlinkNASTransport = NULL;

    S1AP_DownlinkNASTransport_IEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_NAS_PDU_t *NAS_PDU = NULL;

    ogs_assert(emmbuf);
    ogs_assert(enb_ue);

    ogs_debug("DownlinkNASTransport");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_downlinkNASTransport;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_DownlinkNASTransport;

    initiatingMessage->value.choice.DownlinkNASTransport =
        CALLOC(1, sizeof(*DownlinkNASTransport));
    DownlinkNASTransport = initiatingMessage->value.choice.DownlinkNASTransport;
    ogs_assert(DownlinkNASTransport);

    DownlinkNASTransport->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_DownlinkNASTransport);
    ogs_assert(DownlinkNASTransport->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(DownlinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_DownlinkNASTransport_IEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(DownlinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_DownlinkNASTransport_IEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(DownlinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_NAS_PDU;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_DownlinkNASTransport_IEs__value_PR_NAS_PDU;

    ie->value.choice.NAS_PDU = CALLOC(1, sizeof(*NAS_PDU));
    NAS_PDU = ie->value.choice.NAS_PDU;
    ogs_assert(NAS_PDU);

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    *MME_UE_S1AP_ID = enb_ue->mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

    NAS_PDU->size = emmbuf->len;
    NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
    ogs_assert(NAS_PDU->buf);
    memcpy(NAS_PDU->buf, emmbuf->data, NAS_PDU->size);
    ogs_pkbuf_free(emmbuf);

    return ogs_s1ap_encode(&pdu);
}

static void fill_e_rab_to_be_setup(
        S1AP_E_RABToBeSetupItemCtxtSUReq_t *e_rab, mme_bearer_t *bearer)
{
    int rv;
    S1AP_GBR_QosInformation_t *gbrQosInformation = NULL;

    ogs_assert(e_rab);
    ogs_assert(bearer);

    e_rab->e_RAB_ID = bearer->ebi;
    e_rab->e_RABlevelQoSParameters =
        CALLOC(1, sizeof(*e_rab->e_RABlevelQoSParameters));
    ogs_assert(e_rab->e_RABlevelQoSParameters);
    e_rab->e_RABlevelQoSParameters->qCI = bearer->qos.index;
    e_rab->e_RABlevelQoSParameters->allocationRetentionPriority =
        CALLOC(1, sizeof(*e_rab->e_RABlevelQoSParameters->allocationRetentionPriority));
    ogs_assert(e_rab->e_RABlevelQoSParameters->allocationRetentionPriority);

    ogs_debug("    EBI[%d] QCI[%d] SGW-S1U-TEID[%d]",
            bearer->ebi, bearer->qos.index, bearer->sgw_s1u_teid);
    ogs_debug("    ARP[%d:%d:%d]",
            bearer->qos.arp.priority_level,
            bearer->qos.arp.pre_emption_capability,
            bearer->qos.arp.pre_emption_vulnerability);

    e_rab->e_RABlevelQoSParameters->allocationRetentionPriority->
        priorityLevel = bearer->qos.arp.priority_level;
    e_rab->e_RABlevelQoSParameters->allocationRetentionPriority->
        pre_emptionCapability =
            !(bearer->qos.arp.pre_emption_capability);
    e_rab->e_RABlevelQoSParameters->allocationRetentionPriority->
        pre_emptionVulnerability =
            !(bearer->qos.arp.pre_emption_vulnerability);

    if (bearer->qos.mbr.downlink || bearer->qos.mbr.uplink ||
        bearer->qos.gbr.downlink || bearer->qos.gbr.uplink) {
        if (bearer->qos.mbr.downlink && bearer->qos.mbr.uplink &&
            bearer->qos.gbr.downlink && bearer->qos.gbr.uplink) {

            ogs_debug("    MBR[DL:%lld,UL:%lld]",
                (long long)bearer->qos.mbr.downlink,
                (long long)bearer->qos.mbr.uplink);
            ogs_debug("    GBR[DL:%lld,UL:%lld]",
                (long long)bearer->qos.gbr.downlink,
                (long long)bearer->qos.gbr.uplink);

            ogs_assert(bearer->qos.mbr.downlink <= OGS_MAX_BITRATE_S1AP);
            ogs_assert(bearer->qos.mbr.uplink <= OGS_MAX_BITRATE_S1AP);
            ogs_assert(bearer->qos.gbr.downlink <= OGS_MAX_BITRATE_S1AP);
            ogs_assert(bearer->qos.gbr.uplink <= OGS_MAX_BITRATE_S1AP);

            gbrQosInformation =
                    CALLOC(1, sizeof(*gbrQosInformation));
            ogs_assert(gbrQosInformation);
            asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateDL,
                    bearer->qos.mbr.downlink);
            asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateUL,
                    bearer->qos.mbr.uplink);
            asn_uint642INTEGER(&gbrQosInformation->
                    e_RAB_GuaranteedBitrateDL, bearer->qos.gbr.downlink);
            asn_uint642INTEGER(&gbrQosInformation->
                    e_RAB_GuaranteedBitrateUL, bearer->qos.gbr.uplink);
            e_rab->e_RABlevelQoSParameters->gbrQosInformation =
                    gbrQosInformation;

        } else {
            ogs_error("Missing one or more MBR/GBR parameters; "
                    "defaulting to Non-GBR flow ");
            ogs_error("    MBR[DL:%lld,UL:%lld]",
                (long long)bearer->qos.mbr.downlink,
                (long long)bearer->qos.mbr.uplink);
            ogs_error("    GBR[DL:%lld,UL:%lld]",
                (long long)bearer->qos.gbr.downlink,
                (long long)bearer->qos.gbr.uplink);
        }
    }

    rv = ogs_asn_ip_to_BIT_STRING(
            &bearer->sgw_s1u_ip, &e_rab->transportLayerAddress);
    ogs_assert(rv == OGS_OK);
    ogs_asn_uint32_to_OCTET_STRING(
            bearer->sgw_s1u_teid, &e_rab->gTP_TEID);
}

ogs_pkbuf_t *s1ap_build_initial_context_setup_request(
            mme_ue_t *mme_ue, ogs_pkbuf_t *emmbuf)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_InitialContextSetupRequest_t *InitialContextSetupRequest = NULL;

    S1AP_InitialContextSetupRequestIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_UEAggregateMaximumBitrate_t *UEAggregateMaximumBitrate = NULL;
    S1AP_E_RABToBeSetupListCtxtSUReq_t *E_RABToBeSetupListCtxtSUReq = NULL;
    S1AP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;
    S1AP_SecurityKey_t *SecurityKey = NULL;
    S1AP_Masked_IMEISV_t *Masked_IMEISV = NULL;
    S1AP_NRUESecurityCapabilities_t *NRUESecurityCapabilities = NULL;

    enb_ue_t *enb_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    ogs_assert(mme_ue);
    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    ogs_assert(enb_ue);

    ogs_debug("InitialContextSetupRequest");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_InitialContextSetup;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_InitialContextSetupRequest;

    initiatingMessage->value.choice.InitialContextSetupRequest =
        CALLOC(1, sizeof(*InitialContextSetupRequest));
    InitialContextSetupRequest = initiatingMessage->value.choice.InitialContextSetupRequest;
    ogs_assert(InitialContextSetupRequest);

    InitialContextSetupRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_InitialContextSetupRequest);
    ogs_assert(InitialContextSetupRequest->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_uEaggregateMaximumBitrate;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitrate;

    ie->value.choice.UEAggregateMaximumBitrate =
        CALLOC(1, sizeof(*UEAggregateMaximumBitrate));
    UEAggregateMaximumBitrate = ie->value.choice.UEAggregateMaximumBitrate;
    ogs_assert(UEAggregateMaximumBitrate);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABToBeSetupListCtxtSUReq;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
    S1AP_InitialContextSetupRequestIEs__value_PR_E_RABToBeSetupListCtxtSUReq;

    ie->value.choice.E_RABToBeSetupListCtxtSUReq =
        CALLOC(1, sizeof(*E_RABToBeSetupListCtxtSUReq));
    E_RABToBeSetupListCtxtSUReq = ie->value.choice.E_RABToBeSetupListCtxtSUReq;
    ogs_assert(E_RABToBeSetupListCtxtSUReq);

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    *MME_UE_S1AP_ID = enb_ue->mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

    ogs_debug("    AMBR[DL:%lld,UL:%lld]",
        (long long)mme_ue->ambr.downlink, (long long)mme_ue->ambr.uplink);

    asn_uint642INTEGER(
            &UEAggregateMaximumBitrate->uEaggregateMaximumBitRateUL,
            mme_ue->ambr.uplink);
    asn_uint642INTEGER(
            &UEAggregateMaximumBitrate->uEaggregateMaximumBitRateDL,
            mme_ue->ambr.downlink);

    S1AP_E_RABToBeSetupItemCtxtSUReqIEs_t *item = NULL;
    S1AP_E_RABToBeSetupItemCtxtSUReq_t *e_rab = NULL;
    S1AP_NAS_PDU_t *nasPdu = NULL;

    if (mme_ue->nas_eps.type == MME_EPS_TYPE_ATTACH_REQUEST) {
        /*
         * For Attach Request,
         * Delete Session Request/Response removes ALL session/bearers.
         *
         * Since all bearers are INACTIVE,
         * we should not check the bearer activation.
         */
        sess = ogs_list_first(&mme_ue->sess_list);
        /*
         * Issue #3072 : Only first Bearer should be included.
         */
        if (sess)
            bearer = ogs_list_first(&sess->bearer_list);

        if (sess && bearer) {
            item = CALLOC(1, sizeof(*item));
            ogs_assert(item);
            ASN_SEQUENCE_ADD(E_RABToBeSetupListCtxtSUReq, item);

            item->id = S1AP_ProtocolIE_ID_id_E_RABToBeSetupItemCtxtSUReq;
            item->criticality = S1AP_Criticality_reject;
            item->value.present =
                S1AP_E_RABToBeSetupItemCtxtSUReqIEs__value_PR_E_RABToBeSetupItemCtxtSUReq;

            item->value.choice.E_RABToBeSetupItemCtxtSUReq =
                CALLOC(1, sizeof(*e_rab));
            e_rab = item->value.choice.E_RABToBeSetupItemCtxtSUReq;
            ogs_assert(e_rab);

            fill_e_rab_to_be_setup(e_rab, bearer);

            if (emmbuf && emmbuf->len) {
                ogs_debug("    NASPdu[%p:%d]", emmbuf, emmbuf->len);

                nasPdu = CALLOC(1, sizeof(*nasPdu));
                ogs_assert(nasPdu);
                nasPdu->size = emmbuf->len;
                nasPdu->buf = CALLOC(nasPdu->size, sizeof(uint8_t));
                ogs_assert(nasPdu->buf);
                memcpy(nasPdu->buf, emmbuf->data, nasPdu->size);
                e_rab->nAS_PDU = nasPdu;
                ogs_pkbuf_free(emmbuf);

                ogs_log_hexdump(OGS_LOG_DEBUG, nasPdu->buf, nasPdu->size);

                /* Since Tracking area update accept is used only once,
                 * set emmbuf to NULL as shown below */
                emmbuf = NULL;
            }
        }
    } else {
        /*
         * For Service Request/TAU Request/Extended Service Request,
         * Only the active EPS bearer can be included.
         *
         * If MME received Create Bearer Request and
         * if MME does not receive Activate EPS Bearer Context Accept,
         * We should not include the INACTIVE bearer.
         *
         * For example,
         * 1. SGW->MME : Create Bearer Request
         * 2. MME->UE : S1 Paging
         * 3. UE->MME : Service Request
         * 4. MME->UE : Initial Context Setup Request
         *              (We should not include INACTIVE BEARER)
         * 5. UE->MME : Initial Context Setup Response
         * 6. MME->UE : Activate dedicated EPS Bearer Context Request
         * 7. UE->MME : Activate dedicated EPS Bearer Context Accept
         * 8. MME->SGW : Create Bearer Response
         */
        ogs_list_for_each(&mme_ue->sess_list, sess) {
            ogs_list_for_each(&sess->bearer_list, bearer) {
                if (!OGS_FSM_CHECK(&bearer->sm, esm_state_active)) {
                    ogs_warn("No active EPS bearer [%d]", bearer->ebi);
                    ogs_warn("    IMSI[%s] NAS-EPS Type[%d] "
                            "ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
                            mme_ue->imsi_bcd, mme_ue->nas_eps.type,
                            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);
                    ogs_warn("[EBI-TRACK]     EBI[%d] ESM[%s] AGE[%lldms] "
                            "bitmap[0x%04x]",
                            bearer->ebi,
                            mme_ebi_track_esm_state_name(bearer),
                            bearer->trace_created ? (long long)
                                ogs_time_to_msec(ogs_time_now() -
                                    bearer->trace_created) : -1LL,
                            mme_ue->ebi_bitmap);
                    continue;
                }

                item = CALLOC(1, sizeof(*item));
                ogs_assert(item);
                ASN_SEQUENCE_ADD(E_RABToBeSetupListCtxtSUReq, item);

                item->id = S1AP_ProtocolIE_ID_id_E_RABToBeSetupItemCtxtSUReq;
                item->criticality = S1AP_Criticality_reject;
                item->value.present =
                    S1AP_E_RABToBeSetupItemCtxtSUReqIEs__value_PR_E_RABToBeSetupItemCtxtSUReq;

                item->value.choice.E_RABToBeSetupItemCtxtSUReq =
                    CALLOC(1, sizeof(*e_rab));
                e_rab = item->value.choice.E_RABToBeSetupItemCtxtSUReq;
                ogs_assert(e_rab);

                fill_e_rab_to_be_setup(e_rab, bearer);

                if (emmbuf && emmbuf->len) {
                    ogs_debug("    NASPdu[%p:%d]", emmbuf, emmbuf->len);

                    nasPdu = CALLOC(1, sizeof(*nasPdu));
                    ogs_assert(nasPdu);
                    nasPdu->size = emmbuf->len;
                    nasPdu->buf = CALLOC(nasPdu->size, sizeof(uint8_t));
                    ogs_assert(nasPdu->buf);
                    memcpy(nasPdu->buf, emmbuf->data, nasPdu->size);
                    e_rab->nAS_PDU = nasPdu;
                    ogs_pkbuf_free(emmbuf);

                    ogs_log_hexdump(OGS_LOG_DEBUG, nasPdu->buf, nasPdu->size);

                    /* Since Tracking area update accept is used only once,
                     * set emmbuf to NULL as shown below */
                    emmbuf = NULL;
                }
            }
        }
    }

    if (emmbuf && emmbuf->len) {
        ogs_error("NAS message without session/bearer");
        ogs_pkbuf_free(emmbuf);
        emmbuf = NULL;
    }

    if (!OGS_ASN_LIST_COUNT(E_RABToBeSetupListCtxtSUReq)) {
        ogs_list_for_each(&mme_ue->sess_list, sess) {
            ogs_error("    APN[%s]",
                    sess->session ? sess->session->name : "Unknown");
            ogs_list_for_each(&sess->bearer_list, bearer) {
                if (OGS_FSM_CHECK(&bearer->sm, esm_state_inactive))
                    ogs_error("    IN-ACTIVE");
                else if (OGS_FSM_CHECK(&bearer->sm, esm_state_active))
                    ogs_error("    ACTIVE");
                else if (OGS_FSM_CHECK(&bearer->sm,
                            esm_state_pdn_will_disconnect))
                    ogs_error("    PDN will disconnect");
                else
                    ogs_error("    OTHER STATE");

                ogs_error("    EBI[%d] QCI[%d] SGW-S1U-TEID[%d]",
                        bearer->ebi, bearer->qos.index, bearer->sgw_s1u_teid);
            }
        }
        ogs_error("Before ogs_s1ap_free()");
        ogs_s1ap_free(&pdu);
        ogs_error("After ogs_s1ap_free()");
        return NULL;
    }

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities;

    ogs_debug("    UESecurityCapabilities");
    ie->value.choice.UESecurityCapabilities =
        CALLOC(1, sizeof(*UESecurityCapabilities));
    UESecurityCapabilities = ie->value.choice.UESecurityCapabilities;
    ogs_assert(UESecurityCapabilities);

    UESecurityCapabilities->encryptionAlgorithms.size = 2;
    UESecurityCapabilities->encryptionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->encryptionAlgorithms.size,
                    sizeof(uint8_t));
    UESecurityCapabilities->encryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->encryptionAlgorithms.buf[0] =
        (mme_ue->ue_network_capability.eea << 1);

    ogs_log_hexdump(OGS_LOG_DEBUG,
            UESecurityCapabilities->encryptionAlgorithms.buf,
            UESecurityCapabilities->encryptionAlgorithms.size);

    UESecurityCapabilities->integrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                        integrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->integrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf[0] =
        (mme_ue->ue_network_capability.eia << 1);

    ogs_log_hexdump(OGS_LOG_DEBUG,
            UESecurityCapabilities->integrityProtectionAlgorithms.buf,
            UESecurityCapabilities->integrityProtectionAlgorithms.size);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_SecurityKey;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_SecurityKey;

    ogs_debug("    SecurityKey");
    ie->value.choice.SecurityKey = CALLOC(1, sizeof(*SecurityKey));
    SecurityKey = ie->value.choice.SecurityKey;
    ogs_assert(SecurityKey);

    SecurityKey->size = OGS_SHA256_DIGEST_SIZE;
    SecurityKey->buf =
        CALLOC(SecurityKey->size, sizeof(uint8_t));
    SecurityKey->bits_unused = 0;
    memcpy(SecurityKey->buf, mme_ue->kenb, SecurityKey->size);

    ogs_log_hexdump(OGS_LOG_DEBUG, SecurityKey->buf, SecurityKey->size);

    if (mme_ue->nas_eps.type == MME_EPS_TYPE_EXTENDED_SERVICE_REQUEST &&
        MME_CURRENT_P_TMSI_IS_AVAILABLE(mme_ue)) {

        /* Set CS-Fallback */
        S1AP_CSFallbackIndicator_t *CSFallbackIndicator = NULL;
        S1AP_LAI_t *LAI = NULL;

        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_CSFallbackIndicator;
        ie->criticality = S1AP_Criticality_reject;
        ie->value.present =
            S1AP_InitialContextSetupRequestIEs__value_PR_CSFallbackIndicator;

        ogs_debug("    CSFallbackIndicator");
        ie->value.choice.CSFallbackIndicator = CALLOC(1, sizeof(*CSFallbackIndicator));
        CSFallbackIndicator = ie->value.choice.CSFallbackIndicator;
        ogs_assert(CSFallbackIndicator);

        *CSFallbackIndicator = S1AP_CSFallbackIndicator_cs_fallback_required;

        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_RegisteredLAI;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_InitialContextSetupRequestIEs__value_PR_LAI;

        ogs_debug("    LAI");
        ie->value.choice.LAI =
            CALLOC(1, sizeof(*LAI));
        LAI = ie->value.choice.LAI;
        ogs_assert(LAI);

        ogs_s1ap_buffer_to_OCTET_STRING(
            &mme_ue->tai.plmn_id, sizeof(ogs_plmn_id_t), &LAI->pLMNidentity);
        ogs_assert(mme_ue->csmap);
        ogs_assert(mme_ue->current.p_tmsi);
        ogs_asn_uint16_to_OCTET_STRING(mme_ue->csmap->lai.lac, &LAI->lAC);

    }

    if (mme_ue->ueRadioCapability.buf && mme_ue->ueRadioCapability.size) {
        /* Set UeRadioCapability if exists */
        S1AP_UERadioCapability_t *UERadioCapability = NULL;

        ogs_debug("    UERadioCapability[%p:%d]",
                    mme_ue->ueRadioCapability.buf,
                    (int)mme_ue->ueRadioCapability.size);

        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_UERadioCapability;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_InitialContextSetupRequestIEs__value_PR_UERadioCapability;

        ie->value.choice.UERadioCapability = CALLOC(1, sizeof(*UERadioCapability));
        UERadioCapability = ie->value.choice.UERadioCapability;

        ogs_assert(UERadioCapability);
        ogs_s1ap_buffer_to_OCTET_STRING(
                mme_ue->ueRadioCapability.buf, mme_ue->ueRadioCapability.size,
                UERadioCapability);

        ogs_log_hexdump(OGS_LOG_DEBUG,
                UERadioCapability->buf, UERadioCapability->size);
    }

    /* TS23.003 6.2.2 Composition of IMEISV
     *
     * The International Mobile station Equipment Identity and
     * Software Version Number (IMEISV) is composed.
     *
     * TAC(8 digits) - SNR(6 digits) - SVN(2 digits)
     * IMEISV(16 digits) ==> 8bytes
     */
    if (mme_ue->imeisv_len == OGS_MAX_IMEISV_LEN) {
        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_Masked_IMEISV;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_InitialContextSetupRequestIEs__value_PR_Masked_IMEISV;

        ogs_debug("    Masked_IMEISV");
        ie->value.choice.Masked_IMEISV = CALLOC(1, sizeof(*Masked_IMEISV));
        Masked_IMEISV = ie->value.choice.Masked_IMEISV;
        ogs_assert(Masked_IMEISV);

        Masked_IMEISV->size = mme_ue->masked_imeisv_len;
        Masked_IMEISV->buf = CALLOC(Masked_IMEISV->size, sizeof(uint8_t));
        ogs_assert(Masked_IMEISV->buf);
        Masked_IMEISV->bits_unused = 0;
        memcpy(Masked_IMEISV->buf, mme_ue->masked_imeisv, Masked_IMEISV->size);

        ogs_log_hexdump(OGS_LOG_DEBUG, Masked_IMEISV->buf, Masked_IMEISV->size);
    }

    if (mme_ue->ue_additional_security_capability.length) {
        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_NRUESecurityCapabilities;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present = S1AP_InitialContextSetupRequestIEs__value_PR_NRUESecurityCapabilities;

        ogs_debug("    NRUESecurityCapabilities");
        ie->value.choice.NRUESecurityCapabilities =
            CALLOC(1, sizeof(*NRUESecurityCapabilities));
        NRUESecurityCapabilities = ie->value.choice.NRUESecurityCapabilities;
        ogs_assert(NRUESecurityCapabilities);

        NRUESecurityCapabilities->nRencryptionAlgorithms.size = 2;
        NRUESecurityCapabilities->nRencryptionAlgorithms.buf =
            CALLOC(NRUESecurityCapabilities->nRencryptionAlgorithms.size,
                        sizeof(uint8_t));
        NRUESecurityCapabilities->nRencryptionAlgorithms.bits_unused = 0;
        NRUESecurityCapabilities->nRencryptionAlgorithms.buf[0] =
            (mme_ue->ue_additional_security_capability.nea << 1);

        ogs_log_hexdump(OGS_LOG_DEBUG,
            NRUESecurityCapabilities->nRencryptionAlgorithms.buf,
            NRUESecurityCapabilities->nRencryptionAlgorithms.size);

        NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.size = 2;
        NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.buf =
            CALLOC(NRUESecurityCapabilities->
                    nRintegrityProtectionAlgorithms.size, sizeof(uint8_t));
        NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.
            bits_unused = 0;
        NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.buf[0] =
            (mme_ue->ue_additional_security_capability.nia << 1);

        ogs_log_hexdump(OGS_LOG_DEBUG,
            NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.buf,
            NRUESecurityCapabilities->nRintegrityProtectionAlgorithms.size);
    }

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_ue_context_modification_request(mme_ue_t *mme_ue)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_UEContextModificationRequest_t *UEContextModificationRequest = NULL;

    S1AP_UEContextModificationRequestIEs_t *ie = NULL;

    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;
    S1AP_SecurityKey_t *SecurityKey = NULL;
    S1AP_CSFallbackIndicator_t *CSFallbackIndicator = NULL;
    S1AP_LAI_t *LAI = NULL;

    enb_ue_t *enb_ue = NULL;

    ogs_assert(mme_ue);
    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    ogs_assert(enb_ue);

    ogs_debug("UEContextModificationRequest");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_UEContextModification;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_UEContextModificationRequest;

    initiatingMessage->value.choice.UEContextModificationRequest =
        CALLOC(1, sizeof(*UEContextModificationRequest));
    UEContextModificationRequest = initiatingMessage->value.choice.UEContextModificationRequest;
    ogs_assert(UEContextModificationRequest);

    UEContextModificationRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_UEContextModificationRequest);
    ogs_assert(UEContextModificationRequest->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextModificationRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UEContextModificationRequestIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextModificationRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UEContextModificationRequestIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    *MME_UE_S1AP_ID = enb_ue->mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    if (mme_ue->nas_eps.type == MME_EPS_TYPE_EXTENDED_SERVICE_REQUEST &&
        MME_CURRENT_P_TMSI_IS_AVAILABLE(mme_ue)) {
        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(UEContextModificationRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_CSFallbackIndicator;
        ie->criticality = S1AP_Criticality_reject;
        ie->value.present =
            S1AP_UEContextModificationRequestIEs__value_PR_CSFallbackIndicator;

        ie->value.choice.CSFallbackIndicator = CALLOC(1, sizeof(*CSFallbackIndicator));
        CSFallbackIndicator = ie->value.choice.CSFallbackIndicator;
        ogs_assert(CSFallbackIndicator);

        *CSFallbackIndicator = S1AP_CSFallbackIndicator_cs_fallback_required;

        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(UEContextModificationRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_RegisteredLAI;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_UEContextModificationRequestIEs__value_PR_LAI;

        ie->value.choice.LAI =
            CALLOC(1, sizeof(*LAI));
        LAI = ie->value.choice.LAI;
        ogs_assert(LAI);

        ogs_s1ap_buffer_to_OCTET_STRING(
            &mme_ue->tai.plmn_id, sizeof(ogs_plmn_id_t), &LAI->pLMNidentity);
        ogs_assert(mme_ue->csmap);
        ogs_assert(mme_ue->current.p_tmsi);
        ogs_asn_uint16_to_OCTET_STRING(mme_ue->csmap->lai.lac, &LAI->lAC);

    } else {
        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(UEContextModificationRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_UESecurityCapabilities;
        ie->criticality = S1AP_Criticality_reject;
        ie->value.present = S1AP_UEContextModificationRequestIEs__value_PR_UESecurityCapabilities;

        ie->value.choice.UESecurityCapabilities =
            CALLOC(1, sizeof(*UESecurityCapabilities));
        UESecurityCapabilities = ie->value.choice.UESecurityCapabilities;
        ogs_assert(UESecurityCapabilities);

        UESecurityCapabilities->encryptionAlgorithms.size = 2;
        UESecurityCapabilities->encryptionAlgorithms.buf =
            CALLOC(UESecurityCapabilities->encryptionAlgorithms.size,
                        sizeof(uint8_t));
        UESecurityCapabilities->encryptionAlgorithms.bits_unused = 0;
        UESecurityCapabilities->encryptionAlgorithms.buf[0] =
            (mme_ue->ue_network_capability.eea << 1);

        UESecurityCapabilities->integrityProtectionAlgorithms.size = 2;
        UESecurityCapabilities->integrityProtectionAlgorithms.buf =
            CALLOC(UESecurityCapabilities->
                        integrityProtectionAlgorithms.size, sizeof(uint8_t));
        UESecurityCapabilities->integrityProtectionAlgorithms.bits_unused = 0;
        UESecurityCapabilities->integrityProtectionAlgorithms.buf[0] =
            (mme_ue->ue_network_capability.eia << 1);

        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(UEContextModificationRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_SecurityKey;
        ie->criticality = S1AP_Criticality_reject;
        ie->value.present =
            S1AP_UEContextModificationRequestIEs__value_PR_SecurityKey;

        ie->value.choice.SecurityKey = CALLOC(1, sizeof(*SecurityKey));
        SecurityKey = ie->value.choice.SecurityKey;
        ogs_assert(SecurityKey);

        SecurityKey->size = OGS_SHA256_DIGEST_SIZE;
        SecurityKey->buf =
            CALLOC(SecurityKey->size, sizeof(uint8_t));
        SecurityKey->bits_unused = 0;
        memcpy(SecurityKey->buf, mme_ue->kenb, SecurityKey->size);
    }

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_ue_context_release_command(
    enb_ue_t *enb_ue, S1AP_Cause_PR group, long cause)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_UEContextReleaseCommand_t *UEContextReleaseCommand = NULL;

    S1AP_UEContextReleaseCommand_IEs_t *ie = NULL;
    S1AP_UE_S1AP_IDs_t *UE_S1AP_IDs = NULL;
    S1AP_Cause_t *Cause = NULL;

    ogs_assert(enb_ue);

    if (enb_ue->mme_ue_s1ap_id == 0) {
        ogs_error("invalid mme ue s1ap id");
        return NULL;
    }

    ogs_debug("UEContextReleaseCommand");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_UEContextRelease;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_UEContextReleaseCommand;

    initiatingMessage->value.choice.UEContextReleaseCommand =
        CALLOC(1, sizeof(*UEContextReleaseCommand));
    UEContextReleaseCommand = initiatingMessage->value.choice.UEContextReleaseCommand;
    ogs_assert(UEContextReleaseCommand);

    UEContextReleaseCommand->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_UEContextReleaseCommand);
    ogs_assert(UEContextReleaseCommand->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextReleaseCommand->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_UE_S1AP_IDs;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_UEContextReleaseCommand_IEs__value_PR_UE_S1AP_IDs;

    ie->value.choice.UE_S1AP_IDs =
        CALLOC(1, sizeof(*UE_S1AP_IDs));
    UE_S1AP_IDs = ie->value.choice.UE_S1AP_IDs;
    ogs_assert(UE_S1AP_IDs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextReleaseCommand->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_UEContextReleaseCommand_IEs__value_PR_Cause;

    ie->value.choice.Cause =
        CALLOC(1, sizeof(*Cause));
    Cause = ie->value.choice.Cause;
    ogs_assert(Cause);

    if (enb_ue->enb_ue_s1ap_id == INVALID_UE_S1AP_ID) {
        UE_S1AP_IDs->present = S1AP_UE_S1AP_IDs_PR_mME_UE_S1AP_ID;
        UE_S1AP_IDs->choice.mME_UE_S1AP_ID = enb_ue->mme_ue_s1ap_id;
    } else {
        UE_S1AP_IDs->present = S1AP_UE_S1AP_IDs_PR_uE_S1AP_ID_pair;
        UE_S1AP_IDs->choice.uE_S1AP_ID_pair =
            CALLOC(1, sizeof(*UE_S1AP_IDs->choice.uE_S1AP_ID_pair));
        ogs_assert(UE_S1AP_IDs->choice.uE_S1AP_ID_pair);
        UE_S1AP_IDs->choice.uE_S1AP_ID_pair->mME_UE_S1AP_ID =
            enb_ue->mme_ue_s1ap_id;
        UE_S1AP_IDs->choice.uE_S1AP_ID_pair->eNB_UE_S1AP_ID =
            enb_ue->enb_ue_s1ap_id;
    }

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    return ogs_s1ap_encode(&pdu);
}


ogs_pkbuf_t *s1ap_build_e_rab_setup_request(
            mme_bearer_t *bearer, ogs_pkbuf_t *esmbuf)
{
    int rv;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_E_RABSetupRequest_t *E_RABSetupRequest = NULL;

    S1AP_E_RABSetupRequestIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABToBeSetupListBearerSUReq_t *E_RABToBeSetupListBearerSUReq = NULL;

    S1AP_E_RABToBeSetupItemBearerSUReqIEs_t *item = NULL;
    S1AP_E_RABToBeSetupItemBearerSUReq_t *e_rab = NULL;
    S1AP_GBR_QosInformation_t *gbrQosInformation = NULL;
    S1AP_NAS_PDU_t *nasPdu = NULL;

    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(esmbuf);
    ogs_assert(bearer);

    mme_ue = mme_ue_find_by_id(bearer->mme_ue_id);
    ogs_assert(mme_ue);
    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    ogs_assert(enb_ue);

    ogs_debug("E-RABSetupRequest");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_E_RABSetup;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_E_RABSetupRequest;

    initiatingMessage->value.choice.E_RABSetupRequest =
        CALLOC(1, sizeof(*E_RABSetupRequest));
    E_RABSetupRequest = initiatingMessage->value.choice.E_RABSetupRequest;
    ogs_assert(E_RABSetupRequest);

    E_RABSetupRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_E_RABSetupRequest);
    ogs_assert(E_RABSetupRequest->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_E_RABSetupRequestIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_E_RABSetupRequestIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABToBeSetupListBearerSUReq;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_E_RABSetupRequestIEs__value_PR_E_RABToBeSetupListBearerSUReq;

    ie->value.choice.E_RABToBeSetupListBearerSUReq =
        CALLOC(1, sizeof(*E_RABToBeSetupListBearerSUReq));
    E_RABToBeSetupListBearerSUReq = ie->value.choice.E_RABToBeSetupListBearerSUReq;
    ogs_assert(E_RABToBeSetupListBearerSUReq);

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    *MME_UE_S1AP_ID = enb_ue->mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

    item = CALLOC(1, sizeof(*item));
    ogs_assert(item);
    ASN_SEQUENCE_ADD(E_RABToBeSetupListBearerSUReq, item);

    item->id = S1AP_ProtocolIE_ID_id_E_RABToBeSetupItemBearerSUReq;
    item->criticality = S1AP_Criticality_reject;
    item->value.present =
        S1AP_E_RABToBeSetupItemBearerSUReqIEs__value_PR_E_RABToBeSetupItemBearerSUReq;

    item->value.choice.E_RABToBeSetupItemBearerSUReq =
        CALLOC(1, sizeof(*e_rab));
    e_rab = item->value.choice.E_RABToBeSetupItemBearerSUReq;
    ogs_assert(e_rab);

    e_rab->e_RAB_ID = bearer->ebi;
    e_rab->e_RABlevelQoSParameters =
        CALLOC(1, sizeof(*e_rab->e_RABlevelQoSParameters));
    ogs_assert(e_rab->e_RABlevelQoSParameters);
    e_rab->e_RABlevelQoSParameters->qCI = bearer->qos.index;
    e_rab->e_RABlevelQoSParameters->allocationRetentionPriority =
        CALLOC(1, sizeof(*e_rab->e_RABlevelQoSParameters->allocationRetentionPriority));
    ogs_assert(e_rab->e_RABlevelQoSParameters->allocationRetentionPriority);

    ogs_debug("    EBI[%d] QCI[%d]", bearer->ebi, bearer->qos.index);

    e_rab->e_RABlevelQoSParameters->allocationRetentionPriority->
        priorityLevel = bearer->qos.arp.priority_level;
    e_rab->e_RABlevelQoSParameters->allocationRetentionPriority->
        pre_emptionCapability = !(bearer->qos.arp.pre_emption_capability);
    e_rab->e_RABlevelQoSParameters->allocationRetentionPriority->
        pre_emptionVulnerability = !(bearer->qos.arp.pre_emption_vulnerability);

    if (bearer->qos.mbr.downlink || bearer->qos.mbr.uplink ||
        bearer->qos.gbr.downlink || bearer->qos.gbr.uplink) {
        if (bearer->qos.mbr.downlink && bearer->qos.mbr.uplink &&
            bearer->qos.gbr.downlink && bearer->qos.gbr.uplink) {

            ogs_assert(bearer->qos.mbr.downlink <= OGS_MAX_BITRATE_S1AP);
            ogs_assert(bearer->qos.mbr.uplink <= OGS_MAX_BITRATE_S1AP);
            ogs_assert(bearer->qos.gbr.downlink <= OGS_MAX_BITRATE_S1AP);
            ogs_assert(bearer->qos.gbr.uplink <= OGS_MAX_BITRATE_S1AP);

            gbrQosInformation = CALLOC(1, sizeof(*gbrQosInformation));
            ogs_assert(gbrQosInformation);
            asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateDL,
                    bearer->qos.mbr.downlink);
            asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateUL,
                    bearer->qos.mbr.uplink);
            asn_uint642INTEGER(&gbrQosInformation->e_RAB_GuaranteedBitrateDL,
                    bearer->qos.gbr.downlink);
            asn_uint642INTEGER(&gbrQosInformation->e_RAB_GuaranteedBitrateUL,
                    bearer->qos.gbr.uplink);
            e_rab->e_RABlevelQoSParameters->gbrQosInformation =
                gbrQosInformation;

        } else {
            ogs_error("Missing one or more MBR/GBR parameters; "
                    "defaulting to Non-GBR flow ");
            ogs_error("    MBR[DL:%lld,UL:%lld]",
                (long long)bearer->qos.mbr.downlink,
                (long long)bearer->qos.mbr.uplink);
            ogs_error("    GBR[DL:%lld,UL:%lld]",
                (long long)bearer->qos.gbr.downlink,
                (long long)bearer->qos.gbr.uplink);
        }
    }

    rv = ogs_asn_ip_to_BIT_STRING(
            &bearer->sgw_s1u_ip, &e_rab->transportLayerAddress);
    ogs_assert(rv == OGS_OK);
    ogs_asn_uint32_to_OCTET_STRING(bearer->sgw_s1u_teid, &e_rab->gTP_TEID);
    ogs_debug("    SGW-S1U-TEID[%d]", bearer->sgw_s1u_teid);

    nasPdu = &e_rab->nAS_PDU;
    nasPdu->size = esmbuf->len;
    nasPdu->buf = CALLOC(nasPdu->size, sizeof(uint8_t));
    ogs_assert(nasPdu->buf);
    memcpy(nasPdu->buf, esmbuf->data, nasPdu->size);
    ogs_pkbuf_free(esmbuf);

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_e_rab_modify_request(
            mme_bearer_t *bearer, ogs_pkbuf_t *esmbuf)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_E_RABModifyRequest_t *E_RABModifyRequest = NULL;

    S1AP_E_RABModifyRequestIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABToBeModifiedListBearerModReq_t
        *E_RABToBeModifiedListBearerModReq = NULL;

    S1AP_E_RABToBeModifiedItemBearerModReqIEs_t *item = NULL;
    S1AP_E_RABToBeModifiedItemBearerModReq_t *e_rab = NULL;
    S1AP_GBR_QosInformation_t *gbrQosInformation = NULL;
    S1AP_NAS_PDU_t *nasPdu = NULL;

    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(esmbuf);
    ogs_assert(bearer);

    mme_ue = mme_ue_find_by_id(bearer->mme_ue_id);
    ogs_assert(mme_ue);
    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    ogs_assert(enb_ue);

    ogs_debug("E-RABModifyRequest");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_E_RABModify;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_E_RABModifyRequest;

    initiatingMessage->value.choice.E_RABModifyRequest =
        CALLOC(1, sizeof(*E_RABModifyRequest));
    E_RABModifyRequest = initiatingMessage->value.choice.E_RABModifyRequest;
    ogs_assert(E_RABModifyRequest);

    E_RABModifyRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_E_RABModifyRequest);
    ogs_assert(E_RABModifyRequest->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABModifyRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_E_RABModifyRequestIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABModifyRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_E_RABModifyRequestIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABModifyRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABToBeModifiedListBearerModReq;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_E_RABModifyRequestIEs__value_PR_E_RABToBeModifiedListBearerModReq;

    ie->value.choice.E_RABToBeModifiedListBearerModReq =
        CALLOC(1, sizeof(*E_RABToBeModifiedListBearerModReq));
    E_RABToBeModifiedListBearerModReq = ie->value.choice.E_RABToBeModifiedListBearerModReq;
    ogs_assert(E_RABToBeModifiedListBearerModReq);

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    *MME_UE_S1AP_ID = enb_ue->mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

    item = CALLOC(1, sizeof(*item));
    ogs_assert(item);
    ASN_SEQUENCE_ADD(E_RABToBeModifiedListBearerModReq, item);

    item->id = S1AP_ProtocolIE_ID_id_E_RABToBeModifiedItemBearerModReq;
    item->criticality = S1AP_Criticality_reject;
    item->value.present =
        S1AP_E_RABToBeModifiedItemBearerModReqIEs__value_PR_E_RABToBeModifiedItemBearerModReq;

    item->value.choice.E_RABToBeModifiedItemBearerModReq =
        CALLOC(1, sizeof(*e_rab));
    e_rab = item->value.choice.E_RABToBeModifiedItemBearerModReq;
    ogs_assert(e_rab);

    e_rab->e_RAB_ID = bearer->ebi;
    e_rab->e_RABLevelQoSParameters =
        CALLOC(1, sizeof(*e_rab->e_RABLevelQoSParameters));
    ogs_assert(e_rab->e_RABLevelQoSParameters);
    e_rab->e_RABLevelQoSParameters->qCI = bearer->qos.index;
    e_rab->e_RABLevelQoSParameters->allocationRetentionPriority =
        CALLOC(1, sizeof(*e_rab->e_RABLevelQoSParameters->allocationRetentionPriority));
    ogs_assert(e_rab->e_RABLevelQoSParameters->allocationRetentionPriority);

    ogs_debug("    EBI[%d] QCI[%d]", bearer->ebi, bearer->qos.index);

    e_rab->e_RABLevelQoSParameters->allocationRetentionPriority->
        priorityLevel = bearer->qos.arp.priority_level;
    e_rab->e_RABLevelQoSParameters->allocationRetentionPriority->
        pre_emptionCapability = !(bearer->qos.arp.pre_emption_capability);
    e_rab->e_RABLevelQoSParameters->allocationRetentionPriority->
        pre_emptionVulnerability = !(bearer->qos.arp.pre_emption_vulnerability);

    if (bearer->qos.mbr.downlink || bearer->qos.mbr.uplink ||
        bearer->qos.gbr.downlink || bearer->qos.gbr.uplink) {
        ogs_assert(bearer->qos.mbr.downlink);
        ogs_assert(bearer->qos.mbr.uplink);
        ogs_assert(bearer->qos.gbr.downlink);
        ogs_assert(bearer->qos.gbr.uplink);
        ogs_assert(bearer->qos.mbr.downlink <= OGS_MAX_BITRATE_S1AP);
        ogs_assert(bearer->qos.mbr.uplink <= OGS_MAX_BITRATE_S1AP);
        ogs_assert(bearer->qos.gbr.downlink <= OGS_MAX_BITRATE_S1AP);
        ogs_assert(bearer->qos.gbr.uplink <= OGS_MAX_BITRATE_S1AP);

        gbrQosInformation =
                CALLOC(1, sizeof(*gbrQosInformation));
        ogs_assert(gbrQosInformation);
        asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateDL,
                bearer->qos.mbr.downlink);
        asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateUL,
                bearer->qos.mbr.uplink);
        asn_uint642INTEGER(&gbrQosInformation->e_RAB_GuaranteedBitrateDL,
                bearer->qos.gbr.downlink);
        asn_uint642INTEGER(&gbrQosInformation->e_RAB_GuaranteedBitrateUL,
                bearer->qos.gbr.uplink);
        e_rab->e_RABLevelQoSParameters->gbrQosInformation = gbrQosInformation;
    }

    nasPdu = &e_rab->nAS_PDU;
    nasPdu->size = esmbuf->len;
    nasPdu->buf = CALLOC(nasPdu->size, sizeof(uint8_t));
    ogs_assert(nasPdu->buf);
    memcpy(nasPdu->buf, esmbuf->data, nasPdu->size);
    ogs_pkbuf_free(esmbuf);

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_e_rab_release_command(
        mme_bearer_t *bearer, ogs_pkbuf_t *esmbuf,
        S1AP_Cause_PR group, long cause)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_E_RABReleaseCommand_t *E_RABReleaseCommand = NULL;

    S1AP_E_RABReleaseCommandIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_UEAggregateMaximumBitrate_t *UEAggregateMaximumBitrate = NULL;
    S1AP_E_RABList_t *E_RABList = NULL;
    S1AP_NAS_PDU_t *nasPdu = NULL;

    S1AP_E_RABItemIEs_t *item = NULL;
    S1AP_E_RABItem_t *e_rab = NULL;

    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(esmbuf);
    ogs_assert(bearer);

    mme_ue = mme_ue_find_by_id(bearer->mme_ue_id);
    ogs_assert(mme_ue);
    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    ogs_assert(enb_ue);

    ogs_debug("E-RABReleaseCommand");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_E_RABRelease;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_E_RABReleaseCommand;

    initiatingMessage->value.choice.E_RABReleaseCommand =
        CALLOC(1, sizeof(*E_RABReleaseCommand));
    E_RABReleaseCommand = initiatingMessage->value.choice.E_RABReleaseCommand;
    ogs_assert(E_RABReleaseCommand);

    E_RABReleaseCommand->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_E_RABReleaseCommand);
    ogs_assert(E_RABReleaseCommand->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABReleaseCommand->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_E_RABReleaseCommandIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABReleaseCommand->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_E_RABReleaseCommandIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

#if 0 /* It's redundant */
    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABReleaseCommand->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_uEaggregateMaximumBitrate;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_E_RABReleaseCommandIEs__value_PR_UEAggregateMaximumBitrate;

    ie->value.choice.UEAggregateMaximumBitrate =
        CALLOC(1, sizeof(*UEAggregateMaximumBitrate));
    UEAggregateMaximumBitrate = ie->value.choice.UEAggregateMaximumBitrate;
    ogs_assert(UEAggregateMaximumBitrate);
#endif

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABReleaseCommand->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABToBeReleasedList;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABReleaseCommandIEs__value_PR_E_RABList;

    ie->value.choice.E_RABList =
        CALLOC(1, sizeof(*E_RABList));
    E_RABList = ie->value.choice.E_RABList;
    ogs_assert(E_RABList);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABReleaseCommand->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_NAS_PDU;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABReleaseCommandIEs__value_PR_NAS_PDU;

    ie->value.choice.NAS_PDU = CALLOC(1, sizeof(*nasPdu));
    nasPdu = ie->value.choice.NAS_PDU;
    ogs_assert(nasPdu);

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    *MME_UE_S1AP_ID = enb_ue->mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

    if (UEAggregateMaximumBitrate) {
        asn_uint642INTEGER(
                &UEAggregateMaximumBitrate->uEaggregateMaximumBitRateUL,
                mme_ue->ambr.uplink);
        asn_uint642INTEGER(
                &UEAggregateMaximumBitrate->uEaggregateMaximumBitRateDL,
                mme_ue->ambr.downlink);
    }

    item = CALLOC(1, sizeof(*item));
    ogs_assert(item);
    ASN_SEQUENCE_ADD(E_RABList, item);

    item->id = S1AP_ProtocolIE_ID_id_E_RABItem;
    item->criticality = S1AP_Criticality_ignore;
    item->value.present = S1AP_E_RABItemIEs__value_PR_E_RABItem;

    item->value.choice.E_RABItem =
        CALLOC(1, sizeof(*e_rab));
    e_rab = item->value.choice.E_RABItem;
    ogs_assert(e_rab);

    e_rab->e_RAB_ID = bearer->ebi;
    e_rab->cause = CALLOC(1, sizeof(*e_rab->cause));
    ogs_assert(e_rab->cause);
    e_rab->cause->present = group;
    e_rab->cause->choice.radioNetwork = cause;

    ogs_debug("    EBI[%d] Gruop[%d] Cause[%d]",
            bearer->ebi, group, (int)cause);

    nasPdu->size = esmbuf->len;
    nasPdu->buf = CALLOC(nasPdu->size, sizeof(uint8_t));
    ogs_assert(nasPdu->buf);
    memcpy(nasPdu->buf, esmbuf->data, nasPdu->size);
    ogs_pkbuf_free(esmbuf);

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_e_rab_modification_confirm(mme_ue_t *mme_ue)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_E_RABModificationConfirm_t *E_RABModificationConfirm = NULL;

    S1AP_E_RABModificationConfirmIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABModifyListBearerModConf_t *E_RABModifyListBearerModConf = NULL;

    enb_ue_t *enb_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    ogs_assert(mme_ue);
    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    ogs_assert(enb_ue);

    ogs_debug("E-RABModificationConfirm");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(*successfulOutcome));

    successfulOutcome = pdu.choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    successfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_E_RABModificationIndication;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_E_RABModificationConfirm;

    successfulOutcome->value.choice.E_RABModificationConfirm =
        CALLOC(1, sizeof(*E_RABModificationConfirm));
    E_RABModificationConfirm = successfulOutcome->value.choice.E_RABModificationConfirm;
    ogs_assert(E_RABModificationConfirm);

    E_RABModificationConfirm->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_E_RABModificationConfirm);
    ogs_assert(E_RABModificationConfirm->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABModificationConfirm->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_E_RABModificationConfirmIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABModificationConfirm->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_E_RABModificationConfirmIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABModificationConfirm->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABModifyListBearerModConf;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_E_RABModificationConfirmIEs__value_PR_E_RABModifyListBearerModConf;

    ie->value.choice.E_RABModifyListBearerModConf =
        CALLOC(1, sizeof(*E_RABModifyListBearerModConf));
    E_RABModifyListBearerModConf = ie->value.choice.E_RABModifyListBearerModConf;
    ogs_assert(E_RABModifyListBearerModConf);

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    *MME_UE_S1AP_ID = enb_ue->mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

    ogs_list_for_each(&mme_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            S1AP_E_RABModifyItemBearerModConfIEs_t *item = NULL;
            S1AP_E_RABModifyItemBearerModConf_t *e_rab = NULL;

            item = CALLOC(1, sizeof(*item));
            ogs_assert(item);
            ASN_SEQUENCE_ADD(E_RABModifyListBearerModConf, item);

            item->id = S1AP_ProtocolIE_ID_id_E_RABModifyItemBearerModConf;
            item->criticality = S1AP_Criticality_ignore;
            item->value.present =
                S1AP_E_RABModifyItemBearerModConfIEs__value_PR_E_RABModifyItemBearerModConf;

            item->value.choice.E_RABModifyItemBearerModConf =
                CALLOC(1, sizeof(*e_rab));
            e_rab = item->value.choice.E_RABModifyItemBearerModConf;
            ogs_assert(e_rab);

            e_rab->e_RAB_ID = bearer->ebi;

            ogs_debug("    EBI[%d]", bearer->ebi);
        }
    }

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_paging(
        mme_ue_t *mme_ue, S1AP_CNDomain_t cn_domain)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_Paging_t *Paging = NULL;

    S1AP_PagingIEs_t *ie = NULL;

    S1AP_UEIdentityIndexValue_t *UEIdentityIndexValue = NULL;
    S1AP_UEPagingID_t *UEPagingID = NULL;
    S1AP_CNDomain_t *CNDomain = NULL;
    S1AP_TAIList_t *TAIList = NULL;

    S1AP_TAIItemIEs_t *item = NULL;
    S1AP_TAIItem_t *tai_item = NULL;

    uint16_t index_value;
    uint64_t ue_imsi_value = 0;
    int i = 0;

    ogs_assert(mme_ue);

    ogs_debug("Paging");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_Paging;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present = S1AP_InitiatingMessage__value_PR_Paging;

    initiatingMessage->value.choice.Paging =
        CALLOC(1, sizeof(*Paging));
    Paging = initiatingMessage->value.choice.Paging;
    ogs_assert(Paging);

    Paging->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_Paging);
    ogs_assert(Paging->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(Paging->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_UEIdentityIndexValue;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_PagingIEs__value_PR_UEIdentityIndexValue;

    ie->value.choice.UEIdentityIndexValue = CALLOC(1, sizeof(*UEIdentityIndexValue));
    UEIdentityIndexValue = ie->value.choice.UEIdentityIndexValue;
    ogs_assert(UEIdentityIndexValue);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(Paging->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_UEPagingID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_PagingIEs__value_PR_UEPagingID;

    ie->value.choice.UEPagingID =
        CALLOC(1, sizeof(*UEPagingID));
    UEPagingID = ie->value.choice.UEPagingID;
    ogs_assert(UEPagingID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(Paging->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_CNDomain;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_PagingIEs__value_PR_CNDomain;

    ie->value.choice.CNDomain = CALLOC(1, sizeof(*CNDomain));
    CNDomain = ie->value.choice.CNDomain;
    ogs_assert(CNDomain);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(Paging->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_TAIList;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_PagingIEs__value_PR_TAIList;

    ie->value.choice.TAIList =
        CALLOC(1, sizeof(*TAIList));
    TAIList = ie->value.choice.TAIList;
    ogs_assert(TAIList);

    /* Set UE Identity Index value : IMSI mod 4096 */
    UEIdentityIndexValue->size = 2;
    UEIdentityIndexValue->buf =
        CALLOC(UEIdentityIndexValue->size, sizeof(uint8_t));

    /* Conver string to value */
    for (i = 0; i < strlen(mme_ue->imsi_bcd); i++) {
        ue_imsi_value = ue_imsi_value*10 + (mme_ue->imsi_bcd[i] - '0');
    }

    /* index(10bit) = ue_imsi_value mod 1024 */
    index_value = ue_imsi_value % 1024;
    UEIdentityIndexValue->buf[0] = index_value >> 2;
    UEIdentityIndexValue->buf[1] = (index_value & 0x3f) << 6;
    UEIdentityIndexValue->bits_unused = 6;

    /* Set Paging Identity */
    UEPagingID->present = S1AP_UEPagingID_PR_s_TMSI;
    UEPagingID->choice.s_TMSI =
        CALLOC(1, sizeof(*UEPagingID->choice.s_TMSI));
    ogs_assert(UEPagingID->choice.s_TMSI);
    ogs_asn_uint8_to_OCTET_STRING(mme_ue->current.guti.mme_code,
            &UEPagingID->choice.s_TMSI->mMEC);

    ogs_asn_uint32_to_OCTET_STRING(mme_ue->current.guti.m_tmsi,
            &UEPagingID->choice.s_TMSI->m_TMSI);

    ogs_debug("    MME_CODE[%d] M_TMSI[0x%x]",
            mme_ue->current.guti.mme_code, mme_ue->current.guti.m_tmsi);
    ogs_debug("    CN_DOMAIN[%s]",
            cn_domain == S1AP_CNDomain_cs ? "CS" :
                cn_domain == S1AP_CNDomain_ps ? "PS" : "Unknown");

    *CNDomain = cn_domain;

    item = CALLOC(1, sizeof(*item));
    ogs_assert(item);
    ASN_SEQUENCE_ADD(TAIList, item);

    item->id = S1AP_ProtocolIE_ID_id_TAIItem;
    item->criticality = S1AP_Criticality_ignore;
    item->value.present = S1AP_TAIItemIEs__value_PR_TAIItem;

    item->value.choice.TAIItem =
        CALLOC(1, sizeof(*tai_item));
    tai_item = item->value.choice.TAIItem;
    ogs_assert(tai_item);
    tai_item->tAI = CALLOC(1, sizeof(*tai_item->tAI));
    ogs_assert(tai_item->tAI);

    ogs_s1ap_buffer_to_OCTET_STRING(&mme_ue->tai.plmn_id, sizeof(ogs_plmn_id_t),
            &tai_item->tAI->pLMNidentity);
    ogs_asn_uint16_to_OCTET_STRING(mme_ue->tai.tac, &tai_item->tAI->tAC);

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_mme_configuration_transfer(
        S1AP_SONConfigurationTransfer_t *son_configuration_transfer)
{
    int rv;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_MMEConfigurationTransfer_t *MMEConfigurationTransfer = NULL;

    S1AP_MMEConfigurationTransferIEs_t *ie = NULL;
    S1AP_SONConfigurationTransfer_t *SONConfigurationTransfer = NULL;

    ogs_assert(son_configuration_transfer);

    ogs_debug("MMEConfigurationTransfer");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_MMEConfigurationTransfer;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_MMEConfigurationTransfer;

    initiatingMessage->value.choice.MMEConfigurationTransfer =
        CALLOC(1, sizeof(*MMEConfigurationTransfer));
    MMEConfigurationTransfer = initiatingMessage->value.choice.MMEConfigurationTransfer;
    ogs_assert(MMEConfigurationTransfer);

    MMEConfigurationTransfer->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_MMEConfigurationTransfer);
    ogs_assert(MMEConfigurationTransfer->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(MMEConfigurationTransfer->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_SONConfigurationTransferMCT;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_MMEConfigurationTransferIEs__value_PR_SONConfigurationTransfer;

    ie->value.choice.SONConfigurationTransfer =
        CALLOC(1, sizeof(*SONConfigurationTransfer));
    SONConfigurationTransfer = ie->value.choice.SONConfigurationTransfer;
    ogs_assert(SONConfigurationTransfer);

    rv = ogs_asn_copy_ie(&asn_DEF_S1AP_SONConfigurationTransfer,
            son_configuration_transfer, SONConfigurationTransfer);
    if (rv != OGS_OK) {
        ogs_error("ogs_asn_copy_ie() failed");
        ogs_asn_free(&asn_DEF_S1AP_S1AP_PDU, &pdu);
        return NULL;
    }

    return ogs_s1ap_encode(&pdu);
}

/* 3GPP TS 36.413 8.14 MME Direct Information Transfer */
ogs_pkbuf_t *s1ap_build_direct_information_transfer(const uint8_t *buf, size_t buf_len)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_MMEDirectInformationTransfer_t *MMEDirectInformationTransfer = NULL;

    S1AP_MMEDirectInformationTransferIEs_t *ie = NULL;
    S1AP_Inter_SystemInformationTransferType_t *Inter_SystemInformationTransferType = NULL;
    S1AP_RIMTransfer_t *rIMTransfer = NULL;

    ogs_assert(buf);
    ogs_assert(buf_len > 0);

    ogs_debug("MMEDirectInformationTransfer");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_MMEDirectInformationTransfer;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_MMEDirectInformationTransfer;

    initiatingMessage->value.choice.MMEDirectInformationTransfer =
        CALLOC(1, sizeof(*MMEDirectInformationTransfer));
    MMEDirectInformationTransfer = initiatingMessage->value.choice.MMEDirectInformationTransfer;
    ogs_assert(MMEDirectInformationTransfer);

    MMEDirectInformationTransfer->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_MMEDirectInformationTransfer);
    ogs_assert(MMEDirectInformationTransfer->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(MMEDirectInformationTransfer->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Inter_SystemInformationTransferTypeMDT;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_MMEDirectInformationTransferIEs__value_PR_Inter_SystemInformationTransferType;

    ie->value.choice.Inter_SystemInformationTransferType =
        CALLOC(1, sizeof(*Inter_SystemInformationTransferType));
    Inter_SystemInformationTransferType =
        ie->value.choice.Inter_SystemInformationTransferType;
    ogs_assert(Inter_SystemInformationTransferType);
    Inter_SystemInformationTransferType->present =
        S1AP_Inter_SystemInformationTransferType_PR_rIMTransfer;

    Inter_SystemInformationTransferType->choice.rIMTransfer =
        CALLOC(1, sizeof(*rIMTransfer));
    rIMTransfer = Inter_SystemInformationTransferType->choice.rIMTransfer;
    ogs_assert(rIMTransfer);

    rIMTransfer->rIMInformation.size = buf_len;
    rIMTransfer->rIMInformation.buf = CALLOC(buf_len, sizeof(uint8_t));
    ogs_assert(rIMTransfer->rIMInformation.buf);
    memcpy(rIMTransfer->rIMInformation.buf, buf, buf_len);

    /*
     * "The RIM Routing Address IE shall not be present since the eNB is
     * the final destination node":
     */
    rIMTransfer->rIMRoutingAddress = NULL;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_path_switch_ack(
        mme_ue_t *mme_ue, bool e_rab_to_switched_in_uplink_list)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_PathSwitchRequestAcknowledge_t *PathSwitchRequestAcknowledge = NULL;

    S1AP_PathSwitchRequestAcknowledgeIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    struct S1AP_E_RABToBeSwitchedULList	*E_RABToBeSwitchedULList = NULL;
    S1AP_SecurityContext_t *SecurityContext = NULL;
    S1AP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;
    bool send_ue_security_capability = false;

    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(mme_ue);
    enb_ue = enb_ue_find_by_id(mme_ue->enb_ue_id);
    ogs_assert(enb_ue);

    ogs_debug("PathSwitchAcknowledge");

    send_ue_security_capability =
        mme_ue->send_ue_security_capability_in_path_switch_ack;
    mme_ue->send_ue_security_capability_in_path_switch_ack = false;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(*successfulOutcome));

    successfulOutcome = pdu.choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_PathSwitchRequest;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_PathSwitchRequestAcknowledge;

    successfulOutcome->value.choice.PathSwitchRequestAcknowledge =
        CALLOC(1, sizeof(*PathSwitchRequestAcknowledge));
    PathSwitchRequestAcknowledge = successfulOutcome->value.choice.PathSwitchRequestAcknowledge;
    ogs_assert(PathSwitchRequestAcknowledge);

    PathSwitchRequestAcknowledge->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_PathSwitchRequestAcknowledge);
    ogs_assert(PathSwitchRequestAcknowledge->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_PathSwitchRequestAcknowledgeIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_PathSwitchRequestAcknowledgeIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    if (e_rab_to_switched_in_uplink_list == true) {
        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(PathSwitchRequestAcknowledge->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_E_RABToBeSwitchedULList;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_PathSwitchRequestAcknowledgeIEs__value_PR_E_RABToBeSwitchedULList;

        ie->value.choice.E_RABToBeSwitchedULList =
            ogs_asn_calloc_constructed(&asn_DEF_S1AP_E_RABToBeSwitchedULList);
        ogs_assert(ie->value.choice.E_RABToBeSwitchedULList);
        E_RABToBeSwitchedULList = ie->value.choice.E_RABToBeSwitchedULList;
    }

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_SecurityContext;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_PathSwitchRequestAcknowledgeIEs__value_PR_SecurityContext;

    ie->value.choice.SecurityContext =
        CALLOC(1, sizeof(*SecurityContext));
    SecurityContext = ie->value.choice.SecurityContext;
    ogs_assert(SecurityContext);

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    *MME_UE_S1AP_ID = enb_ue->mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

    if (e_rab_to_switched_in_uplink_list == true) {
        ogs_list_for_each(&mme_ue->sess_list, sess) {
            ogs_list_for_each(&sess->bearer_list, bearer) {
                S1AP_E_RABToBeSwitchedULItemIEs_t *item = NULL;
                S1AP_E_RABToBeSwitchedULItem_t *e_rab = NULL;

                item = CALLOC(1, sizeof(*item));
                ogs_assert(item);
                ASN_SEQUENCE_ADD(E_RABToBeSwitchedULList, item);

                item->id = S1AP_ProtocolIE_ID_id_E_RABToBeSwitchedULItem;
                item->criticality = S1AP_Criticality_ignore;
                item->value.present =
                    S1AP_E_RABToBeSwitchedULItemIEs__value_PR_E_RABToBeSwitchedULItem;

                item->value.choice.E_RABToBeSwitchedULItem =
                    CALLOC(1, sizeof(*e_rab));
                e_rab = item->value.choice.E_RABToBeSwitchedULItem;
                ogs_assert(e_rab);

                e_rab->e_RAB_ID = bearer->ebi;

                ogs_assert(OGS_OK == ogs_asn_ip_to_BIT_STRING(
                        &bearer->sgw_s1u_ip, &e_rab->transportLayerAddress));
                ogs_asn_uint32_to_OCTET_STRING(
                        bearer->sgw_s1u_teid, &e_rab->gTP_TEID);
            }
        }
    }

    SecurityContext->nextHopChainingCount = mme_ue->nhcc;
    SecurityContext->nextHopParameter.size = OGS_SHA256_DIGEST_SIZE;
    SecurityContext->nextHopParameter.buf =
        CALLOC(SecurityContext->nextHopParameter.size,
        sizeof(uint8_t));
    SecurityContext->nextHopParameter.bits_unused = 0;
    memcpy(SecurityContext->nextHopParameter.buf,
            mme_ue->nh, SecurityContext->nextHopParameter.size);

    if (send_ue_security_capability) {
        ie = CALLOC(1, sizeof(S1AP_PathSwitchRequestAcknowledgeIEs_t));
        ASN_SEQUENCE_ADD(&PathSwitchRequestAcknowledge->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_UESecurityCapabilities;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_PathSwitchRequestAcknowledgeIEs__value_PR_UESecurityCapabilities;

        ie->value.choice.UESecurityCapabilities =
            CALLOC(1, sizeof(*UESecurityCapabilities));
        UESecurityCapabilities = ie->value.choice.UESecurityCapabilities;
        ogs_assert(UESecurityCapabilities);

        UESecurityCapabilities->encryptionAlgorithms.size = 2;
        UESecurityCapabilities->encryptionAlgorithms.buf =
            CALLOC(UESecurityCapabilities->encryptionAlgorithms.size,
                        sizeof(uint8_t));
        UESecurityCapabilities->encryptionAlgorithms.bits_unused = 0;
        UESecurityCapabilities->encryptionAlgorithms.buf[0] =
            (mme_ue->ue_network_capability.eea << 1);

        UESecurityCapabilities->integrityProtectionAlgorithms.size = 2;
        UESecurityCapabilities->integrityProtectionAlgorithms.buf =
            CALLOC(UESecurityCapabilities->
                            integrityProtectionAlgorithms.size,
                        sizeof(uint8_t));
        UESecurityCapabilities->integrityProtectionAlgorithms.bits_unused = 0;
        UESecurityCapabilities->integrityProtectionAlgorithms.buf[0] =
            (mme_ue->ue_network_capability.eia << 1);
    }

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_path_switch_failure(
    uint32_t enb_ue_s1ap_id, uint32_t mme_ue_s1ap_id,
    S1AP_Cause_PR group, long cause)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    S1AP_PathSwitchRequestFailure_t *PathSwitchRequestFailure = NULL;

    S1AP_PathSwitchRequestFailureIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_Cause_t *Cause = NULL;

    ogs_debug("PathSwitchFailure");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_unsuccessfulOutcome;
    pdu.choice.unsuccessfulOutcome =
        CALLOC(1, sizeof(*unsuccessfulOutcome));

    unsuccessfulOutcome = pdu.choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    unsuccessfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_PathSwitchRequest;
    unsuccessfulOutcome->criticality = S1AP_Criticality_reject;
    unsuccessfulOutcome->value.present =
        S1AP_UnsuccessfulOutcome__value_PR_PathSwitchRequestFailure;

    unsuccessfulOutcome->value.choice.PathSwitchRequestFailure =
        CALLOC(1, sizeof(*PathSwitchRequestFailure));
    PathSwitchRequestFailure = unsuccessfulOutcome->value.choice.PathSwitchRequestFailure;
    ogs_assert(PathSwitchRequestFailure);

    PathSwitchRequestFailure->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_PathSwitchRequestFailure);
    ogs_assert(PathSwitchRequestFailure->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequestFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_PathSwitchRequestFailureIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequestFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_PathSwitchRequestFailureIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequestFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_PathSwitchRequestFailureIEs__value_PR_Cause;

    ie->value.choice.Cause =
        CALLOC(1, sizeof(*Cause));
    Cause = ie->value.choice.Cause;
    ogs_assert(Cause);

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            enb_ue_s1ap_id, mme_ue_s1ap_id);
    ogs_debug("    Group[%d] Cause[%d]", group, (int)cause);

    *MME_UE_S1AP_ID = mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue_s1ap_id;
    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_handover_command(enb_ue_t *source_ue)
{
    int rv;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_HandoverCommand_t *HandoverCommand = NULL;

    S1AP_HandoverCommandIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_HandoverType_t *HandoverType = NULL;
    struct S1AP_E_RABSubjecttoDataForwardingList
        *E_RABSubjecttoDataForwardingList = NULL;
    S1AP_Target_ToSource_TransparentContainer_t
        *Target_ToSource_TransparentContainer = NULL;

    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    ogs_assert(source_ue);
    mme_ue = mme_ue_find_by_id(source_ue->mme_ue_id);
    ogs_assert(mme_ue);

    ogs_debug("HandoverCommand");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(*successfulOutcome));

    successfulOutcome = pdu.choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    successfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_HandoverPreparation;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_HandoverCommand;

    successfulOutcome->value.choice.HandoverCommand =
        CALLOC(1, sizeof(*HandoverCommand));
    HandoverCommand = successfulOutcome->value.choice.HandoverCommand;
    ogs_assert(HandoverCommand);

    HandoverCommand->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_HandoverCommand);
    ogs_assert(HandoverCommand->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverCommand->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverCommandIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverCommand->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverCommandIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverCommand->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_HandoverType;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverCommandIEs__value_PR_HandoverType;

    ie->value.choice.HandoverType = CALLOC(1, sizeof(*HandoverType));
    HandoverType = ie->value.choice.HandoverType;
    ogs_assert(HandoverType);

    *MME_UE_S1AP_ID = source_ue->mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = source_ue->enb_ue_s1ap_id;
    *HandoverType = source_ue->handover_type;

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);

    ogs_list_for_each(&mme_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {

            S1AP_E_RABDataForwardingItem_t *e_rab = NULL;

            if (MME_HAVE_SGW_DL_INDIRECT_TUNNEL(bearer) ||
                MME_HAVE_SGW_UL_INDIRECT_TUNNEL(bearer)) {
                S1AP_E_RABDataForwardingItemIEs_t *item = NULL;

                if (E_RABSubjecttoDataForwardingList == NULL) {
                    ie = CALLOC(1, sizeof(*ie));
                    ogs_assert(ie);
                    ASN_SEQUENCE_ADD(HandoverCommand->protocolIEs, ie);

                    ie->id =
                        S1AP_ProtocolIE_ID_id_E_RABSubjecttoDataForwardingList;
                    ie->criticality = S1AP_Criticality_ignore;
                    ie->value.present =
                        S1AP_HandoverCommandIEs__value_PR_E_RABSubjecttoDataForwardingList;

                    ie->value.choice.E_RABSubjecttoDataForwardingList =
                        ogs_asn_calloc_constructed(
                                &asn_DEF_S1AP_E_RABSubjecttoDataForwardingList);
                    ogs_assert(
                        ie->value.choice.E_RABSubjecttoDataForwardingList);
                    E_RABSubjecttoDataForwardingList =
                            ie->value.choice.E_RABSubjecttoDataForwardingList;
                }
                ogs_assert(E_RABSubjecttoDataForwardingList);

                item = CALLOC(
                        1, sizeof(*item));
                ogs_assert(item);
                ASN_SEQUENCE_ADD(E_RABSubjecttoDataForwardingList, item);

                item->id = S1AP_ProtocolIE_ID_id_E_RABDataForwardingItem;
                item->criticality = S1AP_Criticality_ignore;
                item->value.present =
                    S1AP_E_RABDataForwardingItemIEs__value_PR_E_RABDataForwardingItem;

                item->value.choice.E_RABDataForwardingItem =
                    CALLOC(1, sizeof(*e_rab));
                e_rab = item->value.choice.E_RABDataForwardingItem;
                ogs_assert(e_rab);

                e_rab->e_RAB_ID = bearer->ebi;
            }

            if (MME_HAVE_SGW_DL_INDIRECT_TUNNEL(bearer)) {
                ogs_assert(e_rab);
                e_rab->dL_transportLayerAddress =
                    CALLOC(1, sizeof(*e_rab->dL_transportLayerAddress));
                ogs_assert(e_rab->dL_transportLayerAddress);
                rv = ogs_asn_ip_to_BIT_STRING(
                        &bearer->sgw_dl_ip, e_rab->dL_transportLayerAddress);
                ogs_assert(rv == OGS_OK);

                e_rab->dL_gTP_TEID = CALLOC(1, sizeof(*e_rab->dL_gTP_TEID));
                ogs_assert(e_rab->dL_gTP_TEID);
                ogs_asn_uint32_to_OCTET_STRING(
                        bearer->sgw_dl_teid, e_rab->dL_gTP_TEID);
                ogs_debug("    SGW-DL-TEID[%d]", bearer->sgw_dl_teid);
            }

            if (MME_HAVE_SGW_UL_INDIRECT_TUNNEL(bearer)) {
                ogs_assert(e_rab);
                e_rab->uL_TransportLayerAddress =
                    CALLOC(1, sizeof(*e_rab->uL_TransportLayerAddress));
                ogs_assert(e_rab->uL_TransportLayerAddress);
                rv = ogs_asn_ip_to_BIT_STRING(
                    &bearer->sgw_ul_ip, e_rab->uL_TransportLayerAddress);
                ogs_assert(rv == OGS_OK);

                e_rab->uL_GTP_TEID = CALLOC(1, sizeof(*e_rab->uL_GTP_TEID));
                ogs_assert(e_rab->uL_GTP_TEID);
                ogs_asn_uint32_to_OCTET_STRING(
                        bearer->sgw_ul_teid, e_rab->uL_GTP_TEID);
                ogs_debug("    SGW-UL-TEID[%d]", bearer->sgw_dl_teid);
            }
        }
    }

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverCommand->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Target_ToSource_TransparentContainer;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_HandoverCommandIEs__value_PR_Target_ToSource_TransparentContainer;

    ie->value.choice.Target_ToSource_TransparentContainer =
        CALLOC(1, sizeof(*Target_ToSource_TransparentContainer));
    Target_ToSource_TransparentContainer =
        ie->value.choice.Target_ToSource_TransparentContainer;
    ogs_assert(Target_ToSource_TransparentContainer);

    ogs_s1ap_buffer_to_OCTET_STRING(mme_ue->container.buf,
            mme_ue->container.size, Target_ToSource_TransparentContainer);

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_handover_preparation_failure(
        enb_ue_t *source_ue, S1AP_Cause_PR group, long cause)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    S1AP_HandoverPreparationFailure_t *HandoverPreparationFailure = NULL;

    S1AP_HandoverPreparationFailureIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_Cause_t *Cause = NULL;

    ogs_assert(source_ue);
    ogs_assert(group);

    ogs_debug("HandoverPreparationFailure");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_unsuccessfulOutcome;
    pdu.choice.unsuccessfulOutcome =
        CALLOC(1, sizeof(*unsuccessfulOutcome));

    unsuccessfulOutcome = pdu.choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    unsuccessfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_HandoverPreparation;
    unsuccessfulOutcome->criticality = S1AP_Criticality_reject;
    unsuccessfulOutcome->value.present =
        S1AP_UnsuccessfulOutcome__value_PR_HandoverPreparationFailure;

    unsuccessfulOutcome->value.choice.HandoverPreparationFailure =
        CALLOC(1, sizeof(*HandoverPreparationFailure));
    HandoverPreparationFailure = unsuccessfulOutcome->value.choice.HandoverPreparationFailure;
    ogs_assert(HandoverPreparationFailure);

    HandoverPreparationFailure->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_HandoverPreparationFailure);
    ogs_assert(HandoverPreparationFailure->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverPreparationFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverPreparationFailureIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverPreparationFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverPreparationFailureIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverPreparationFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_HandoverPreparationFailureIEs__value_PR_Cause;

    ie->value.choice.Cause =
        CALLOC(1, sizeof(*Cause));
    Cause = ie->value.choice.Cause;
    ogs_assert(Cause);

    ogs_debug("    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);
    ogs_debug("    Group[%d] Cause[%d]", group, (int)cause);

    *MME_UE_S1AP_ID = source_ue->mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = source_ue->enb_ue_s1ap_id;
    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_handover_request(
    enb_ue_t *target_ue, S1AP_HandoverType_t *handovertype, S1AP_Cause_t *cause,
    S1AP_Source_ToTarget_TransparentContainer_t
        *source_totarget_transparentContainer)
{
    int rv;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_HandoverRequest_t *HandoverRequest = NULL;

    S1AP_HandoverRequestIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_HandoverType_t *HandoverType = NULL;
    S1AP_Cause_t *Cause = NULL;
    S1AP_UEAggregateMaximumBitrate_t *UEAggregateMaximumBitrate = NULL;
    struct S1AP_E_RABToBeSetupListHOReq *E_RABToBeSetupListHOReq = NULL;
    S1AP_Source_ToTarget_TransparentContainer_t
        *Source_ToTarget_TransparentContainer = NULL;
    S1AP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;
    S1AP_SecurityContext_t *SecurityContext = NULL;
    S1AP_Masked_IMEISV_t *Masked_IMEISV = NULL;

    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    ogs_assert(handovertype);
    ogs_assert(cause);
    ogs_assert(source_totarget_transparentContainer);

    ogs_assert(target_ue);
    mme_ue = mme_ue_find_by_id(target_ue->mme_ue_id);
    ogs_assert(mme_ue);

    ogs_debug("HandoverRequest");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_HandoverResourceAllocation;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_HandoverRequest;

    initiatingMessage->value.choice.HandoverRequest =
        CALLOC(1, sizeof(*HandoverRequest));
    HandoverRequest = initiatingMessage->value.choice.HandoverRequest;
    ogs_assert(HandoverRequest);

    HandoverRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_HandoverRequest);
    ogs_assert(HandoverRequest->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverRequestIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_HandoverType;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverRequestIEs__value_PR_HandoverType;

    ie->value.choice.HandoverType = CALLOC(1, sizeof(*HandoverType));
    HandoverType = ie->value.choice.HandoverType;
    ogs_assert(HandoverType);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_HandoverRequestIEs__value_PR_Cause;

    ie->value.choice.Cause =
        CALLOC(1, sizeof(*Cause));
    Cause = ie->value.choice.Cause;
    ogs_assert(Cause);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_uEaggregateMaximumBitrate;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_HandoverRequestIEs__value_PR_UEAggregateMaximumBitrate;

    ie->value.choice.UEAggregateMaximumBitrate =
        CALLOC(1, sizeof(*UEAggregateMaximumBitrate));
    UEAggregateMaximumBitrate = ie->value.choice.UEAggregateMaximumBitrate;
    ogs_assert(UEAggregateMaximumBitrate);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABToBeSetupListHOReq;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_HandoverRequestIEs__value_PR_E_RABToBeSetupListHOReq;

    ie->value.choice.E_RABToBeSetupListHOReq =
        ogs_asn_calloc_constructed(&asn_DEF_S1AP_E_RABToBeSetupListHOReq);

    ogs_assert(ie->value.choice.E_RABToBeSetupListHOReq);
    E_RABToBeSetupListHOReq = ie->value.choice.E_RABToBeSetupListHOReq;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Source_ToTarget_TransparentContainer;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_HandoverRequestIEs__value_PR_Source_ToTarget_TransparentContainer;

    ie->value.choice.Source_ToTarget_TransparentContainer =
        CALLOC(1, sizeof(*Source_ToTarget_TransparentContainer));
    Source_ToTarget_TransparentContainer =
        ie->value.choice.Source_ToTarget_TransparentContainer;
    ogs_assert(Source_ToTarget_TransparentContainer);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_HandoverRequestIEs__value_PR_UESecurityCapabilities;

    ie->value.choice.UESecurityCapabilities =
        CALLOC(1, sizeof(*UESecurityCapabilities));
    UESecurityCapabilities = ie->value.choice.UESecurityCapabilities;
    ogs_assert(UESecurityCapabilities);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_SecurityContext;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverRequestIEs__value_PR_SecurityContext;

    ie->value.choice.SecurityContext =
        CALLOC(1, sizeof(*SecurityContext));
    SecurityContext = ie->value.choice.SecurityContext;
    ogs_assert(SecurityContext);

    *MME_UE_S1AP_ID = target_ue->mme_ue_s1ap_id;
    *HandoverType = *handovertype;
    Cause->present = cause->present;
    Cause->choice.radioNetwork = cause->choice.radioNetwork;

    asn_uint642INTEGER(
            &UEAggregateMaximumBitrate->uEaggregateMaximumBitRateUL,
            mme_ue->ambr.uplink);
    asn_uint642INTEGER(
            &UEAggregateMaximumBitrate->uEaggregateMaximumBitRateDL,
            mme_ue->ambr.downlink);

    ogs_list_for_each(&mme_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {

            S1AP_E_RABToBeSetupItemHOReqIEs_t *item = NULL;
            S1AP_E_RABToBeSetupItemHOReq_t *e_rab = NULL;
            S1AP_GBR_QosInformation_t *gbrQosInformation = NULL;

            if (!OGS_FSM_CHECK(&bearer->sm, esm_state_active)) {
                ogs_warn("No active EPS bearer [%d]", bearer->ebi);
                ogs_warn("    IMSI[%s] NAS-EPS Type[%d] "
                        "ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
                        mme_ue->imsi_bcd, mme_ue->nas_eps.type,
                        target_ue->enb_ue_s1ap_id, target_ue->mme_ue_s1ap_id);
                continue;
            }

            item = CALLOC(1, sizeof(*item));
            ogs_assert(item);
            ASN_SEQUENCE_ADD(E_RABToBeSetupListHOReq, item);

            item->id = S1AP_ProtocolIE_ID_id_E_RABToBeSetupItemHOReq;
            item->criticality = S1AP_Criticality_reject;
            item->value.present =
            S1AP_E_RABToBeSetupItemHOReqIEs__value_PR_E_RABToBeSetupItemHOReq;

            item->value.choice.E_RABToBeSetupItemHOReq =
                CALLOC(1, sizeof(*e_rab));
            e_rab = item->value.choice.E_RABToBeSetupItemHOReq;
            ogs_assert(e_rab);

            e_rab->e_RAB_ID = bearer->ebi;
            e_rab->e_RABlevelQosParameters =
                CALLOC(1, sizeof(*e_rab->e_RABlevelQosParameters));
            ogs_assert(e_rab->e_RABlevelQosParameters);
            e_rab->e_RABlevelQosParameters->qCI = bearer->qos.index;
            e_rab->e_RABlevelQosParameters->allocationRetentionPriority =
                CALLOC(1, sizeof(*e_rab->e_RABlevelQosParameters->allocationRetentionPriority));
            ogs_assert(e_rab->e_RABlevelQosParameters->allocationRetentionPriority);

            e_rab->e_RABlevelQosParameters->allocationRetentionPriority->
                priorityLevel = bearer->qos.arp.priority_level;
            e_rab->e_RABlevelQosParameters->allocationRetentionPriority->
                pre_emptionCapability =
                    !(bearer->qos.arp.pre_emption_capability);
            e_rab->e_RABlevelQosParameters->allocationRetentionPriority->
                pre_emptionVulnerability =
                    !(bearer->qos.arp.pre_emption_vulnerability);

            if (bearer->qos.mbr.downlink || bearer->qos.mbr.uplink ||
                bearer->qos.gbr.downlink || bearer->qos.gbr.uplink) {
                ogs_assert(bearer->qos.mbr.downlink);
                ogs_assert(bearer->qos.mbr.uplink);
                ogs_assert(bearer->qos.gbr.downlink);
                ogs_assert(bearer->qos.gbr.uplink);
                ogs_assert(bearer->qos.mbr.downlink <= OGS_MAX_BITRATE_S1AP);
                ogs_assert(bearer->qos.mbr.uplink <= OGS_MAX_BITRATE_S1AP);
                ogs_assert(bearer->qos.gbr.downlink <= OGS_MAX_BITRATE_S1AP);
                ogs_assert(bearer->qos.gbr.uplink <= OGS_MAX_BITRATE_S1AP);

                gbrQosInformation =
                        CALLOC(1, sizeof(*gbrQosInformation));
                ogs_assert(gbrQosInformation);
                asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateDL,
                        bearer->qos.mbr.downlink);
                asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateUL,
                        bearer->qos.mbr.uplink);
                asn_uint642INTEGER(&gbrQosInformation->
                        e_RAB_GuaranteedBitrateDL, bearer->qos.gbr.downlink);
                asn_uint642INTEGER(&gbrQosInformation->
                        e_RAB_GuaranteedBitrateUL, bearer->qos.gbr.uplink);
                e_rab->e_RABlevelQosParameters->gbrQosInformation =
                        gbrQosInformation;
            }

            rv = ogs_asn_ip_to_BIT_STRING(
                    &bearer->sgw_s1u_ip, &e_rab->transportLayerAddress);
            ogs_assert(rv == OGS_OK);
            ogs_asn_uint32_to_OCTET_STRING(
                    bearer->sgw_s1u_teid, &e_rab->gTP_TEID);
            ogs_debug("    SGW-S1U-TEID[%d]", bearer->sgw_s1u_teid);
        }
    }

    if (!OGS_ASN_LIST_COUNT(E_RABToBeSetupListHOReq)) {
        ogs_list_for_each(&mme_ue->sess_list, sess) {
            ogs_error("    APN[%s]",
                    sess->session ? sess->session->name : "Unknown");
            ogs_list_for_each(&sess->bearer_list, bearer) {
                if (OGS_FSM_CHECK(&bearer->sm, esm_state_inactive))
                    ogs_error("    IN-ACTIVE");
                else if (OGS_FSM_CHECK(&bearer->sm, esm_state_active))
                    ogs_error("    ACTIVE");
                else if (OGS_FSM_CHECK(&bearer->sm,
                            esm_state_pdn_will_disconnect))
                    ogs_error("    PDN will disconnect");
                else
                    ogs_error("    OTHER STATE");

                ogs_error("    EBI[%d] QCI[%d] SGW-S1U-TEID[%d]",
                        bearer->ebi, bearer->qos.index, bearer->sgw_s1u_teid);
            }
        }
        ogs_error("Before ogs_s1ap_free()");
        ogs_s1ap_free(&pdu);
        ogs_error("After ogs_s1ap_free()");
        return NULL;
    }

    ogs_s1ap_buffer_to_OCTET_STRING(
            source_totarget_transparentContainer->buf,
            source_totarget_transparentContainer->size,
            Source_ToTarget_TransparentContainer);

    UESecurityCapabilities->encryptionAlgorithms.size = 2;
    UESecurityCapabilities->encryptionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->encryptionAlgorithms.size,
                    sizeof(uint8_t));
    UESecurityCapabilities->encryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->encryptionAlgorithms.buf[0] =
        (mme_ue->ue_network_capability.eea << 1);

    UESecurityCapabilities->integrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                        integrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->integrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf[0] =
        (mme_ue->ue_network_capability.eia << 1);

    SecurityContext->nextHopChainingCount = mme_ue->nhcc;
    SecurityContext->nextHopParameter.size = OGS_SHA256_DIGEST_SIZE;
    SecurityContext->nextHopParameter.buf =
        CALLOC(SecurityContext->nextHopParameter.size,
        sizeof(uint8_t));
    SecurityContext->nextHopParameter.bits_unused = 0;
    memcpy(SecurityContext->nextHopParameter.buf,
            mme_ue->nh, SecurityContext->nextHopParameter.size);

    /* TS23.003 6.2.2 Composition of IMEISV
     *
     * The International Mobile station Equipment Identity and
     * Software Version Number (IMEISV) is composed.
     *
     * TAC(8 digits) - SNR(6 digits) - SVN(2 digits)
     * IMEISV(16 digits) ==> 8bytes
     */
    if (mme_ue->imeisv_len == OGS_MAX_IMEISV_LEN) {
        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_Masked_IMEISV;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present = S1AP_HandoverRequestIEs__value_PR_Masked_IMEISV;

        ie->value.choice.Masked_IMEISV = CALLOC(1, sizeof(*Masked_IMEISV));
        Masked_IMEISV = ie->value.choice.Masked_IMEISV;
        ogs_assert(Masked_IMEISV);

        Masked_IMEISV->size = mme_ue->masked_imeisv_len;
        Masked_IMEISV->buf = CALLOC(Masked_IMEISV->size, sizeof(uint8_t));
        ogs_assert(Masked_IMEISV->buf);
        Masked_IMEISV->bits_unused = 0;
        memcpy(Masked_IMEISV->buf, mme_ue->masked_imeisv, Masked_IMEISV->size);
    }

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_handover_cancel_ack(enb_ue_t *source_ue)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_HandoverCancelAcknowledge_t *HandoverCancelAcknowledge = NULL;

    S1AP_HandoverCancelAcknowledgeIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;

    ogs_assert(source_ue);

    ogs_debug("HandoverCancelAcknowledge");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(*successfulOutcome));

    successfulOutcome = pdu.choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_HandoverCancel;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_HandoverCancelAcknowledge;

    successfulOutcome->value.choice.HandoverCancelAcknowledge =
        CALLOC(1, sizeof(*HandoverCancelAcknowledge));
    HandoverCancelAcknowledge = successfulOutcome->value.choice.HandoverCancelAcknowledge;
    ogs_assert(HandoverCancelAcknowledge);

    HandoverCancelAcknowledge->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_HandoverCancelAcknowledge);
    ogs_assert(HandoverCancelAcknowledge->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverCancelAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverCancelAcknowledgeIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverCancelAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverCancelAcknowledgeIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    *MME_UE_S1AP_ID = source_ue->mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = source_ue->enb_ue_s1ap_id;

    ogs_debug("    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_mme_status_transfer(
        enb_ue_t *target_ue,
        S1AP_ENB_StatusTransfer_TransparentContainer_t
            *enb_statustransfer_transparentContainer)
{
    int rv;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_MMEStatusTransfer_t *MMEStatusTransfer = NULL;

    S1AP_MMEStatusTransferIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_ENB_StatusTransfer_TransparentContainer_t
        *ENB_StatusTransfer_TransparentContainer = NULL;

    ogs_assert(target_ue);
    ogs_assert(enb_statustransfer_transparentContainer);

    ogs_debug("MMEStatusTransfer");

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_MMEStatusTransfer;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_MMEStatusTransfer;

    initiatingMessage->value.choice.MMEStatusTransfer =
        CALLOC(1, sizeof(*MMEStatusTransfer));
    MMEStatusTransfer = initiatingMessage->value.choice.MMEStatusTransfer;
    ogs_assert(MMEStatusTransfer);

    MMEStatusTransfer->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_MMEStatusTransfer);
    ogs_assert(MMEStatusTransfer->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(MMEStatusTransfer->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_MMEStatusTransferIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(MMEStatusTransfer->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_MMEStatusTransferIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(MMEStatusTransfer->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_StatusTransfer_TransparentContainer;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_MMEStatusTransferIEs__value_PR_ENB_StatusTransfer_TransparentContainer;

    ie->value.choice.ENB_StatusTransfer_TransparentContainer =
        CALLOC(1, sizeof(*ENB_StatusTransfer_TransparentContainer));
    ENB_StatusTransfer_TransparentContainer =
        ie->value.choice.ENB_StatusTransfer_TransparentContainer;
    ogs_assert(ENB_StatusTransfer_TransparentContainer);

    *MME_UE_S1AP_ID = target_ue->mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = target_ue->enb_ue_s1ap_id;

    ogs_debug("    Target : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]",
            target_ue->enb_ue_s1ap_id, target_ue->mme_ue_s1ap_id);

    rv = ogs_asn_copy_ie(
            &asn_DEF_S1AP_ENB_StatusTransfer_TransparentContainer,
            enb_statustransfer_transparentContainer,
            ENB_StatusTransfer_TransparentContainer);
    if (rv != OGS_OK) {
        ogs_error("ogs_asn_copy_ie() failed");
        ogs_asn_free(&asn_DEF_S1AP_S1AP_PDU, &pdu);
        return NULL;
    }

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_write_replace_warning_request(sbc_pws_data_t *sbc_pws)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_WriteReplaceWarningRequest_t *WriteReplaceWarningRequest = NULL;

    S1AP_WriteReplaceWarningRequestIEs_t *ie = NULL;
    S1AP_MessageIdentifier_t *MessageIdentifier = NULL;
    S1AP_SerialNumber_t *SerialNumber = NULL;
    S1AP_RepetitionPeriod_t *RepetitionPeriod = NULL;
    S1AP_NumberofBroadcastRequest_t *NumberofBroadcastRequest = NULL;
    S1AP_DataCodingScheme_t *DataCodingScheme = NULL;
    S1AP_WarningMessageContents_t *WarningMessageContents = NULL;

    ogs_debug("WriteReplaceWarningRequest");

    ogs_assert(sbc_pws);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_WriteReplaceWarning;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_WriteReplaceWarningRequest;

    initiatingMessage->value.choice.WriteReplaceWarningRequest =
        CALLOC(1, sizeof(*WriteReplaceWarningRequest));
    WriteReplaceWarningRequest = initiatingMessage->value.choice.WriteReplaceWarningRequest;
    ogs_assert(WriteReplaceWarningRequest);

    WriteReplaceWarningRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_WriteReplaceWarningRequest);
    ogs_assert(WriteReplaceWarningRequest->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(WriteReplaceWarningRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MessageIdentifier;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_WriteReplaceWarningRequestIEs__value_PR_MessageIdentifier;

    ie->value.choice.MessageIdentifier = CALLOC(1, sizeof(*MessageIdentifier));
    MessageIdentifier = ie->value.choice.MessageIdentifier;
    ogs_assert(MessageIdentifier);

    MessageIdentifier->size = (16 / 8);
    MessageIdentifier->buf =
        CALLOC(MessageIdentifier->size, sizeof(uint8_t));
    MessageIdentifier->bits_unused = 0;
    MessageIdentifier->buf[0] = (sbc_pws->message_id >> 8) & 0xFF;
    MessageIdentifier->buf[1] = sbc_pws->message_id & 0xFF;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(WriteReplaceWarningRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_SerialNumber;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_WriteReplaceWarningRequestIEs__value_PR_SerialNumber;

    ie->value.choice.SerialNumber = CALLOC(1, sizeof(*SerialNumber));
    SerialNumber = ie->value.choice.SerialNumber;
    ogs_assert(SerialNumber);

    SerialNumber->size = (16 / 8);
    SerialNumber->buf =
        CALLOC(SerialNumber->size, sizeof(uint8_t));
    SerialNumber->bits_unused = 0;
    SerialNumber->buf[0] = (sbc_pws->serial_number >> 8) & 0xFF;
    SerialNumber->buf[1] = sbc_pws->serial_number & 0xFF;

    /* TODO: optional Warning Area List */

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(WriteReplaceWarningRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_RepetitionPeriod;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_WriteReplaceWarningRequestIEs__value_PR_RepetitionPeriod;

    ie->value.choice.RepetitionPeriod = CALLOC(1, sizeof(*RepetitionPeriod));
    RepetitionPeriod = ie->value.choice.RepetitionPeriod;
    ogs_assert(RepetitionPeriod);

    *RepetitionPeriod = sbc_pws->repetition_period;

    /* TODO: optional Extended Repetition Period */

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(WriteReplaceWarningRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_NumberofBroadcastRequest;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_WriteReplaceWarningRequestIEs__value_PR_NumberofBroadcastRequest;

    ie->value.choice.NumberofBroadcastRequest = CALLOC(1, sizeof(*NumberofBroadcastRequest));
    NumberofBroadcastRequest = ie->value.choice.NumberofBroadcastRequest;
    ogs_assert(NumberofBroadcastRequest);

    *NumberofBroadcastRequest = sbc_pws->number_of_broadcast;

    /* TODO: optional Warnging Type */

    /* TODO: optional Warning Security Information */

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(WriteReplaceWarningRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_DataCodingScheme;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_WriteReplaceWarningRequestIEs__value_PR_DataCodingScheme;

    ie->value.choice.DataCodingScheme = CALLOC(1, sizeof(*DataCodingScheme));
    DataCodingScheme = ie->value.choice.DataCodingScheme;
    ogs_assert(DataCodingScheme);

    DataCodingScheme->size = (8 / 8);
    DataCodingScheme->buf =
        CALLOC(DataCodingScheme->size, sizeof(uint8_t));
    DataCodingScheme->bits_unused = 0;
    DataCodingScheme->buf[0] = sbc_pws->data_coding_scheme & 0xFF;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(WriteReplaceWarningRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_WarningMessageContents;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_WriteReplaceWarningRequestIEs__value_PR_WarningMessageContents;

    ie->value.choice.WarningMessageContents = CALLOC(1, sizeof(*WarningMessageContents));
    WarningMessageContents = ie->value.choice.WarningMessageContents;
    ogs_assert(WarningMessageContents);

    WarningMessageContents->size = sbc_pws->message_length;;
    WarningMessageContents->buf =
        CALLOC(WarningMessageContents->size, sizeof(uint8_t));
    memcpy(WarningMessageContents->buf,
            sbc_pws->message_contents, WarningMessageContents->size);

    /* TODO: optional Concurrent Warning Message Indicator */

    ogs_debug("    Message[%02x,%02x] Serial[%02x,%02x] "
            "Repetition[%d] NumBroadcast[%d]",
        MessageIdentifier->buf[0], MessageIdentifier->buf[1],
        SerialNumber->buf[0], SerialNumber->buf[1],
        (int)*RepetitionPeriod, (int)*NumberofBroadcastRequest);

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *s1ap_build_kill_request(sbc_pws_data_t *sbc_pws)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_KillRequest_t *KillRequest = NULL;

    S1AP_KillRequestIEs_t *ie = NULL;
    S1AP_MessageIdentifier_t *MessageIdentifier = NULL;
    S1AP_SerialNumber_t *SerialNumber = NULL;

    ogs_debug("KillRequest");

    ogs_assert(sbc_pws);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_Kill;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_KillRequest;

    initiatingMessage->value.choice.KillRequest =
        CALLOC(1, sizeof(*KillRequest));
    KillRequest = initiatingMessage->value.choice.KillRequest;
    ogs_assert(KillRequest);

    KillRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_KillRequest);
    ogs_assert(KillRequest->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(KillRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MessageIdentifier;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_KillRequestIEs__value_PR_MessageIdentifier;

    ie->value.choice.MessageIdentifier = CALLOC(1, sizeof(*MessageIdentifier));
    MessageIdentifier = ie->value.choice.MessageIdentifier;
    ogs_assert(MessageIdentifier);

    MessageIdentifier->size = (16 / 8);
    MessageIdentifier->buf =
        CALLOC(MessageIdentifier->size, sizeof(uint8_t));
    MessageIdentifier->bits_unused = 0;
    MessageIdentifier->buf[0] = (sbc_pws->message_id >> 8) & 0xFF;
    MessageIdentifier->buf[1] = sbc_pws->message_id & 0xFF;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(KillRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_SerialNumber;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_KillRequestIEs__value_PR_SerialNumber;

    ie->value.choice.SerialNumber = CALLOC(1, sizeof(*SerialNumber));
    SerialNumber = ie->value.choice.SerialNumber;
    ogs_assert(SerialNumber);

    SerialNumber->size = (16 / 8);
    SerialNumber->buf =
        CALLOC(SerialNumber->size, sizeof(uint8_t));
    SerialNumber->bits_unused = 0;
    SerialNumber->buf[0] = (sbc_pws->serial_number >> 8) & 0xFF;
    SerialNumber->buf[1] = sbc_pws->serial_number & 0xFF;

    /* TODO: optional Warning Area List */

    ogs_debug("    Message[%02x,%02x] Serial[%02x,%02x]",
            MessageIdentifier->buf[0], MessageIdentifier->buf[1],
            SerialNumber->buf[0], SerialNumber->buf[1]);

    return ogs_s1ap_encode(&pdu);
}
