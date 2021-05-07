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

#include "ngap-build.h"

ogs_pkbuf_t *ngap_build_ng_setup_response(void)
{
    int i, j;

    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_NGSetupResponse_t *NGSetupResponse = NULL;

    NGAP_NGSetupResponseIEs_t *ie = NULL;
    NGAP_AMFName_t *AMFName = NULL;
    NGAP_ServedGUAMIList_t *ServedGUAMIList = NULL;
    NGAP_RelativeAMFCapacity_t *RelativeAMFCapacity = NULL;
    NGAP_PLMNSupportList_t *PLMNSupportList = NULL;

    ogs_debug("NGSetupResponse");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = NGAP_ProcedureCode_id_NGSetup;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_NGSetupResponse;

    NGSetupResponse = &successfulOutcome->value.choice.NGSetupResponse;

    ie = CALLOC(1, sizeof(NGAP_NGSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&NGSetupResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMFName;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_NGSetupResponseIEs__value_PR_AMFName;

    AMFName = &ie->value.choice.AMFName;

    ie = CALLOC(1, sizeof(NGAP_NGSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&NGSetupResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_ServedGUAMIList;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_NGSetupResponseIEs__value_PR_ServedGUAMIList;

    ServedGUAMIList = &ie->value.choice.ServedGUAMIList;

    ie = CALLOC(1, sizeof(NGAP_NGSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&NGSetupResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RelativeAMFCapacity;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_NGSetupResponseIEs__value_PR_RelativeAMFCapacity;

    RelativeAMFCapacity = &ie->value.choice.RelativeAMFCapacity;

    ie = CALLOC(1, sizeof(NGAP_NGSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&NGSetupResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PLMNSupportList;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_NGSetupResponseIEs__value_PR_PLMNSupportList;

    PLMNSupportList = &ie->value.choice.PLMNSupportList;

    ogs_asn_buffer_to_OCTET_STRING((char*)amf_self()->amf_name,
            strlen(amf_self()->amf_name), AMFName);

    for (i = 0; i < amf_self()->num_of_served_guami; i++) {
        NGAP_ServedGUAMIItem_t *ServedGUAMIItem = NULL;
        NGAP_GUAMI_t *gUAMI = NULL;
        NGAP_PLMNIdentity_t *pLMNIdentity = NULL;
        NGAP_AMFRegionID_t *aMFRegionID = NULL;
        NGAP_AMFSetID_t *aMFSetID = NULL;
        NGAP_AMFPointer_t *aMFPointer = NULL;

        ServedGUAMIItem = (NGAP_ServedGUAMIItem_t *)
                CALLOC(1, sizeof(NGAP_ServedGUAMIItem_t));
        gUAMI = &ServedGUAMIItem->gUAMI;
        pLMNIdentity = &gUAMI->pLMNIdentity;
        aMFRegionID = &gUAMI->aMFRegionID;
        aMFSetID = &gUAMI->aMFSetID;
        aMFPointer = &gUAMI->aMFPointer;

        ogs_asn_buffer_to_OCTET_STRING(
                &amf_self()->served_guami[i].plmn_id,
                OGS_PLMN_ID_LEN, pLMNIdentity);
        ogs_ngap_uint8_to_AMFRegionID(
                ogs_amf_region_id(&amf_self()->served_guami[i].amf_id),
                aMFRegionID);
        ogs_ngap_uint16_to_AMFSetID(
                ogs_amf_set_id(&amf_self()->served_guami[i].amf_id),
                aMFSetID);
        ogs_ngap_uint8_to_AMFPointer(
                ogs_amf_pointer(&amf_self()->served_guami[i].amf_id),
                aMFPointer);

        ASN_SEQUENCE_ADD(&ServedGUAMIList->list, ServedGUAMIItem);
    }

    *RelativeAMFCapacity = amf_self()->relative_capacity;

    for (i = 0; i < amf_self()->num_of_plmn_support; i++) {
        NGAP_PLMNSupportItem_t *NGAP_PLMNSupportItem = NULL;
        NGAP_PLMNIdentity_t *pLMNIdentity = NULL;
        NGAP_SliceSupportList_t *sliceSupportList = NULL;

        NGAP_PLMNSupportItem = (NGAP_PLMNSupportItem_t *)
                CALLOC(1, sizeof(NGAP_PLMNSupportItem_t));
        pLMNIdentity = &NGAP_PLMNSupportItem->pLMNIdentity;
        sliceSupportList = &NGAP_PLMNSupportItem->sliceSupportList;

        ogs_asn_buffer_to_OCTET_STRING(
                &amf_self()->plmn_support[i].plmn_id,
                OGS_PLMN_ID_LEN, pLMNIdentity);
        for (j = 0; j < amf_self()->plmn_support[i].num_of_s_nssai; j++) {
            NGAP_SliceSupportItem_t *NGAP_SliceSupportItem = NULL;
            NGAP_S_NSSAI_t *s_NSSAI = NULL;
            NGAP_SST_t *sST = NULL;

            NGAP_SliceSupportItem = (NGAP_SliceSupportItem_t *)
                    CALLOC(1, sizeof(NGAP_SliceSupportItem_t));
            s_NSSAI = &NGAP_SliceSupportItem->s_NSSAI;
            sST = &s_NSSAI->sST;

            ogs_asn_uint8_to_OCTET_STRING(
                amf_self()->plmn_support[i].s_nssai[j].sst, sST);
            if (amf_self()->plmn_support[i].s_nssai[j].sd.v !=
                    OGS_S_NSSAI_NO_SD_VALUE) {
                s_NSSAI->sD = CALLOC(1, sizeof(NGAP_SD_t));
                ogs_asn_uint24_to_OCTET_STRING(
                    amf_self()->plmn_support[i].s_nssai[j].sd, s_NSSAI->sD);
            }

            ASN_SEQUENCE_ADD(&sliceSupportList->list, NGAP_SliceSupportItem);
        }

        ASN_SEQUENCE_ADD(&PLMNSupportList->list, NGAP_PLMNSupportItem);
    }

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_ng_setup_failure(
        NGAP_Cause_PR group, long cause, long time_to_wait)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    NGAP_NGSetupFailure_t *NGSetupFailure = NULL;

    NGAP_NGSetupFailureIEs_t *ie = NULL;
    NGAP_Cause_t *Cause = NULL;
    NGAP_TimeToWait_t *TimeToWait = NULL;

    ogs_debug("    Group[%d] Cause[%d] TimeToWait[%ld]",
            group, (int)cause, time_to_wait);

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_unsuccessfulOutcome;
    pdu.choice.unsuccessfulOutcome =
        CALLOC(1, sizeof(NGAP_UnsuccessfulOutcome_t));

    unsuccessfulOutcome = pdu.choice.unsuccessfulOutcome;
    unsuccessfulOutcome->procedureCode = NGAP_ProcedureCode_id_NGSetup;
    unsuccessfulOutcome->criticality = NGAP_Criticality_reject;
    unsuccessfulOutcome->value.present =
        NGAP_UnsuccessfulOutcome__value_PR_NGSetupFailure;

    NGSetupFailure = &unsuccessfulOutcome->value.choice.NGSetupFailure;

    if (time_to_wait > -1) {
        ie = CALLOC(1, sizeof(NGAP_NGSetupFailureIEs_t));
        ASN_SEQUENCE_ADD(&NGSetupFailure->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_TimeToWait;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_NGSetupFailureIEs__value_PR_TimeToWait;

        TimeToWait = &ie->value.choice.TimeToWait;
    }

    ie = CALLOC(1, sizeof(NGAP_NGSetupFailureIEs_t));
    ASN_SEQUENCE_ADD(&NGSetupFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_NGSetupFailureIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;
    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    if (TimeToWait)
        *TimeToWait = time_to_wait;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_ran_configuration_update_ack(void)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;

    ogs_debug("RANConfigurationUpdateAcknowledge");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode =
        NGAP_ProcedureCode_id_RANConfigurationUpdate;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_RANConfigurationUpdateAcknowledge;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_ran_configuration_update_failure(
        NGAP_Cause_PR group, long cause, long time_to_wait)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    NGAP_RANConfigurationUpdateFailure_t *RANConfigurationUpdateFailure = NULL;

    NGAP_RANConfigurationUpdateFailureIEs_t *ie = NULL;
    NGAP_Cause_t *Cause = NULL;
    NGAP_TimeToWait_t *TimeToWait = NULL;

    ogs_debug("    Group[%d] Cause[%d] TimeToWait[%ld]",
            group, (int)cause, time_to_wait);

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_unsuccessfulOutcome;
    pdu.choice.unsuccessfulOutcome =
        CALLOC(1, sizeof(NGAP_UnsuccessfulOutcome_t));

    unsuccessfulOutcome = pdu.choice.unsuccessfulOutcome;
    unsuccessfulOutcome->procedureCode =
        NGAP_ProcedureCode_id_RANConfigurationUpdate;
    unsuccessfulOutcome->criticality = NGAP_Criticality_reject;
    unsuccessfulOutcome->value.present =
        NGAP_UnsuccessfulOutcome__value_PR_RANConfigurationUpdateFailure;

    RANConfigurationUpdateFailure =
        &unsuccessfulOutcome->value.choice.RANConfigurationUpdateFailure;

    if (time_to_wait > -1) {
        ie = CALLOC(1, sizeof(NGAP_RANConfigurationUpdateFailureIEs_t));
        ASN_SEQUENCE_ADD(&RANConfigurationUpdateFailure->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_TimeToWait;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_RANConfigurationUpdateFailureIEs__value_PR_TimeToWait;

        TimeToWait = &ie->value.choice.TimeToWait;
    }

    ie = CALLOC(1, sizeof(NGAP_RANConfigurationUpdateFailureIEs_t));
    ASN_SEQUENCE_ADD(&RANConfigurationUpdateFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_RANConfigurationUpdateFailureIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;
    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    if (TimeToWait)
        *TimeToWait = time_to_wait;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_downlink_nas_transport(
    ran_ue_t *ran_ue, ogs_pkbuf_t *gmmbuf, bool ue_ambr, bool allowed_nssai)
{
    amf_ue_t *amf_ue = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_DownlinkNASTransport_t *DownlinkNASTransport = NULL;

    NGAP_DownlinkNASTransport_IEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;
    NGAP_UEAggregateMaximumBitRate_t *UEAggregateMaximumBitRate = NULL;
    NGAP_AllowedNSSAI_t *AllowedNSSAI = NULL;

    ogs_assert(gmmbuf);
    ogs_assert(ran_ue);

    amf_ue = ran_ue->amf_ue;

    ogs_debug("DownlinkNASTransport");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_DownlinkNASTransport;
    initiatingMessage->criticality = NGAP_Criticality_ignore;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_DownlinkNASTransport;

    DownlinkNASTransport =
        &initiatingMessage->value.choice.DownlinkNASTransport;

    ie = CALLOC(1, sizeof(NGAP_DownlinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&DownlinkNASTransport->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_DownlinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_DownlinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&DownlinkNASTransport->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_DownlinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_DownlinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&DownlinkNASTransport->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_NAS_PDU;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_DownlinkNASTransport_IEs__value_PR_NAS_PDU;

    NAS_PDU = &ie->value.choice.NAS_PDU;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;

    NAS_PDU->size = gmmbuf->len;
    NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
    memcpy(NAS_PDU->buf, gmmbuf->data, NAS_PDU->size);
    ogs_pkbuf_free(gmmbuf);

    /*
     * TS 38.413
     * 8.6.2 Downlink NAS Transport
     * 8.6.2.1. Successful Operation
     *
     * The UE Aggregate Maximum Bit Rate IE should be sent to the NG-RAN node
     * if the AMF has not sent it previously
     */
    if (ran_ue->ue_ambr_sent == false && ue_ambr) {
        ogs_assert(amf_ue);
        ogs_assert(amf_ue->ue_ambr.downlink && amf_ue->ue_ambr.uplink);

        ie = CALLOC(1, sizeof(NGAP_DownlinkNASTransport_IEs_t));
        ASN_SEQUENCE_ADD(&DownlinkNASTransport->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_DownlinkNASTransport_IEs__value_PR_UEAggregateMaximumBitRate;

        UEAggregateMaximumBitRate = &ie->value.choice.UEAggregateMaximumBitRate;

        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateUL,
                amf_ue->ue_ambr.uplink);
        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateDL,
                amf_ue->ue_ambr.downlink);

        ran_ue->ue_ambr_sent = true;
    }

    if (allowed_nssai) {
        int i;
        ogs_assert(amf_ue);

        ie = CALLOC(1, sizeof(NGAP_DownlinkNASTransport_IEs_t));
        ASN_SEQUENCE_ADD(&DownlinkNASTransport->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_AllowedNSSAI;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present =
            NGAP_DownlinkNASTransport_IEs__value_PR_AllowedNSSAI;

        AllowedNSSAI = &ie->value.choice.AllowedNSSAI;

        ogs_assert(amf_ue->allowed_nssai.num_of_s_nssai);
        for (i = 0; i < amf_ue->allowed_nssai.num_of_s_nssai; i++) {
            NGAP_AllowedNSSAI_Item_t *NGAP_AllowedNSSAI_Item = NULL;
            NGAP_S_NSSAI_t *s_NSSAI = NULL;
            NGAP_SST_t *sST = NULL;

            NGAP_AllowedNSSAI_Item = (NGAP_AllowedNSSAI_Item_t *)
                    CALLOC(1, sizeof(NGAP_AllowedNSSAI_Item_t));
            s_NSSAI = &NGAP_AllowedNSSAI_Item->s_NSSAI;
            sST = &s_NSSAI->sST;

            ogs_asn_uint8_to_OCTET_STRING(
                amf_ue->allowed_nssai.s_nssai[i].sst, sST);
            if (amf_ue->allowed_nssai.s_nssai[i].sd.v !=
                    OGS_S_NSSAI_NO_SD_VALUE) {
                s_NSSAI->sD = CALLOC(1, sizeof(NGAP_SD_t));
                ogs_asn_uint24_to_OCTET_STRING(
                    amf_ue->allowed_nssai.s_nssai[i].sd, s_NSSAI->sD);
            }

            ASN_SEQUENCE_ADD(&AllowedNSSAI->list, NGAP_AllowedNSSAI_Item);
        }
    }

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_ue_build_initial_context_setup_request(
            amf_ue_t *amf_ue, ogs_pkbuf_t *gmmbuf)
{
    int i;

    ran_ue_t *ran_ue = NULL;
    amf_sess_t *sess = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_InitialContextSetupRequest_t *InitialContextSetupRequest = NULL;

    NGAP_InitialContextSetupRequestIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_UEAggregateMaximumBitRate_t *UEAggregateMaximumBitRate = NULL;
    NGAP_PDUSessionResourceSetupListCxtReq_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceSetupItemCxtReq_t *PDUSessionItem = NULL;
    NGAP_GUAMI_t *GUAMI = NULL;
    NGAP_AllowedNSSAI_t *AllowedNSSAI = NULL;
    NGAP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;
    NGAP_SecurityKey_t *SecurityKey = NULL;
    NGAP_MaskedIMEISV_t *MaskedIMEISV = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;

    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

    ogs_debug("InitialContextSetupRequest(UE)");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_InitialContextSetup;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_InitialContextSetupRequest;

    InitialContextSetupRequest =
        &initiatingMessage->value.choice.InitialContextSetupRequest;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    /*
     *
     * 9.2.2. UE Context Management Messages
     * 9.2.2.1. INITIAL CONTEXT SETUP REQUEST
     *
     * <TABLE>
     * if PDUsessionResourceSetup
     *
     * SHOULD NOT CHECK PREVIOUSLY SENT
     */
    if (PDU_RES_SETUP_REQ_TRANSFER_NEEDED(amf_ue) == true) {
        ogs_assert(amf_ue->ue_ambr.downlink && amf_ue->ue_ambr.uplink);

        ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present = NGAP_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

        UEAggregateMaximumBitRate = &ie->value.choice.UEAggregateMaximumBitRate;

        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateUL,
                amf_ue->ue_ambr.uplink);
        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateDL,
                amf_ue->ue_ambr.downlink);

        ran_ue->ue_ambr_sent = true;
    }

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_GUAMI;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_InitialContextSetupRequestIEs__value_PR_GUAMI;

    GUAMI = &ie->value.choice.GUAMI;

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        OCTET_STRING_t *transfer = NULL;
        NGAP_S_NSSAI_t *s_NSSAI = NULL;
        NGAP_SST_t *sST = NULL;

        if (!sess->transfer.pdu_session_resource_setup_request) continue;

        if (!PDUSessionList) {
            ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
            ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

            ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtReq;
            ie->criticality = NGAP_Criticality_reject;
            ie->value.present = NGAP_InitialContextSetupRequestIEs__value_PR_PDUSessionResourceSetupListCxtReq;

            PDUSessionList =
                &ie->value.choice.PDUSessionResourceSetupListCxtReq;
        }

        PDUSessionItem = CALLOC(1,
                sizeof(struct NGAP_PDUSessionResourceSetupItemCxtReq));
        ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

        PDUSessionItem->pDUSessionID = sess->psi;

        s_NSSAI = &PDUSessionItem->s_NSSAI;
        sST = &s_NSSAI->sST;

        ogs_asn_uint8_to_OCTET_STRING(sess->s_nssai.sst, sST);
        if (sess->s_nssai.sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
            s_NSSAI->sD = CALLOC(1, sizeof(NGAP_SD_t));
            ogs_asn_uint24_to_OCTET_STRING(sess->s_nssai.sd, s_NSSAI->sD);
        }

        transfer = &PDUSessionItem->pDUSessionResourceSetupRequestTransfer;
        transfer->size = sess->transfer.pdu_session_resource_setup_request->len;
        transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
        memcpy(transfer->buf,
                sess->transfer.pdu_session_resource_setup_request->data,
                transfer->size);
    }

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AllowedNSSAI;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_AllowedNSSAI;

    AllowedNSSAI = &ie->value.choice.AllowedNSSAI;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities;

    UESecurityCapabilities = &ie->value.choice.UESecurityCapabilities;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SecurityKey;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_SecurityKey;

    SecurityKey = &ie->value.choice.SecurityKey;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;

    ogs_assert(amf_ue->guami);
    ogs_asn_buffer_to_OCTET_STRING(&amf_ue->guami->plmn_id, OGS_PLMN_ID_LEN,
            &GUAMI->pLMNIdentity);
    ogs_ngap_uint8_to_AMFRegionID(ogs_amf_region_id(&amf_ue->guami->amf_id),
            &GUAMI->aMFRegionID);
    ogs_ngap_uint16_to_AMFSetID(ogs_amf_set_id(&amf_ue->guami->amf_id),
            &GUAMI->aMFSetID);
    ogs_ngap_uint8_to_AMFPointer(ogs_amf_pointer(&amf_ue->guami->amf_id),
            &GUAMI->aMFPointer);

    ogs_assert(amf_ue->allowed_nssai.num_of_s_nssai);
    for (i = 0; i < amf_ue->allowed_nssai.num_of_s_nssai; i++) {
        NGAP_AllowedNSSAI_Item_t *NGAP_AllowedNSSAI_Item = NULL;
        NGAP_S_NSSAI_t *s_NSSAI = NULL;
        NGAP_SST_t *sST = NULL;

        NGAP_AllowedNSSAI_Item = (NGAP_AllowedNSSAI_Item_t *)
                CALLOC(1, sizeof(NGAP_AllowedNSSAI_Item_t));
        s_NSSAI = &NGAP_AllowedNSSAI_Item->s_NSSAI;
        sST = &s_NSSAI->sST;

        ogs_asn_uint8_to_OCTET_STRING(
            amf_ue->allowed_nssai.s_nssai[i].sst, sST);
        if (amf_ue->allowed_nssai.s_nssai[i].sd.v !=
                OGS_S_NSSAI_NO_SD_VALUE) {
            s_NSSAI->sD = CALLOC(1, sizeof(NGAP_SD_t));
            ogs_asn_uint24_to_OCTET_STRING(
                amf_ue->allowed_nssai.s_nssai[i].sd, s_NSSAI->sD);
        }

        ASN_SEQUENCE_ADD(&AllowedNSSAI->list, NGAP_AllowedNSSAI_Item);
    }

    UESecurityCapabilities->nRencryptionAlgorithms.size = 2;
    UESecurityCapabilities->nRencryptionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    nRencryptionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->nRencryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->nRencryptionAlgorithms.buf[0] =
        (amf_ue->ue_security_capability.nr_ea << 1);

    UESecurityCapabilities->nRintegrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->nRintegrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    nRintegrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->nRintegrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->nRintegrityProtectionAlgorithms.buf[0] =
        (amf_ue->ue_security_capability.nr_ia << 1);

    UESecurityCapabilities->eUTRAencryptionAlgorithms.size = 2;
    UESecurityCapabilities->eUTRAencryptionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    eUTRAencryptionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->eUTRAencryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->eUTRAencryptionAlgorithms.buf[0] =
        (amf_ue->ue_security_capability.eutra_ea << 1);

    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    eUTRAintegrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.buf[0] =
        (amf_ue->ue_security_capability.eutra_ia << 1);

    SecurityKey->size = OGS_SHA256_DIGEST_SIZE;
    SecurityKey->buf = CALLOC(SecurityKey->size, sizeof(uint8_t));
    SecurityKey->bits_unused = 0;
    memcpy(SecurityKey->buf, amf_ue->kgnb, SecurityKey->size);

    if (amf_ue->ueRadioCapability.buf && amf_ue->ueRadioCapability.size) {
        /* Set UeRadioCapability if exists */
        NGAP_UERadioCapability_t *UERadioCapability = NULL;

        ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UERadioCapability;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_InitialContextSetupRequestIEs__value_PR_UERadioCapability;

        UERadioCapability = &ie->value.choice.UERadioCapability;

        ogs_assert(UERadioCapability);
        ogs_asn_buffer_to_OCTET_STRING(
                amf_ue->ueRadioCapability.buf, amf_ue->ueRadioCapability.size,
                UERadioCapability);
    }

    /* TS23.003 6.2.2 Composition of IMEISV
     *
     * The International Mobile station Equipment Identity and
     * Software Version Number (IMEISV) is composed.
     *
     * TAC(8 digits) - SNR(6 digits) - SVN(2 digits)
     * IMEISV(16 digits) ==> 8bytes
     */
    if (amf_ue->masked_imeisv_len == OGS_MAX_IMEISV_LEN) {
        ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_MaskedIMEISV;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_InitialContextSetupRequestIEs__value_PR_MaskedIMEISV;

        MaskedIMEISV = &ie->value.choice.MaskedIMEISV;

        MaskedIMEISV->size = amf_ue->masked_imeisv_len;
        MaskedIMEISV->buf = CALLOC(MaskedIMEISV->size, sizeof(uint8_t));
        MaskedIMEISV->bits_unused = 0;
        memcpy(MaskedIMEISV->buf, amf_ue->masked_imeisv, MaskedIMEISV->size);
    }

    if (gmmbuf) {
        ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_NAS_PDU;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_InitialContextSetupRequestIEs__value_PR_NAS_PDU;

        NAS_PDU = &ie->value.choice.NAS_PDU;

        NAS_PDU->size = gmmbuf->len;
        NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
        memcpy(NAS_PDU->buf, gmmbuf->data, NAS_PDU->size);
        ogs_pkbuf_free(gmmbuf);
    }

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_sess_build_initial_context_setup_request(
            amf_sess_t *sess, ogs_pkbuf_t *gmmbuf, ogs_pkbuf_t *n2smbuf)
{
    int i;

    ran_ue_t *ran_ue = NULL;
    amf_ue_t *amf_ue = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_InitialContextSetupRequest_t *InitialContextSetupRequest = NULL;

    NGAP_InitialContextSetupRequestIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_UEAggregateMaximumBitRate_t *UEAggregateMaximumBitRate = NULL;
    NGAP_PDUSessionResourceSetupListCxtReq_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceSetupItemCxtReq_t *PDUSessionItem = NULL;
    NGAP_GUAMI_t *GUAMI = NULL;
    NGAP_AllowedNSSAI_t *AllowedNSSAI = NULL;
    NGAP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;
    NGAP_SecurityKey_t *SecurityKey = NULL;
    NGAP_MaskedIMEISV_t *MaskedIMEISV = NULL;

    ogs_assert(sess);
    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

    ogs_debug("InitialContextSetupRequest(Session)");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_InitialContextSetup;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_InitialContextSetupRequest;

    InitialContextSetupRequest =
        &initiatingMessage->value.choice.InitialContextSetupRequest;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    /*
     *
     * 9.2.2. UE Context Management Messages
     * 9.2.2.1. INITIAL CONTEXT SETUP REQUEST
     *
     * <TABLE>
     * if PDUsessionResourceSetup
     *
     * SHOULD NOT CHECK PREVIOUSLY SENT
     */
    if (gmmbuf || n2smbuf) {
        ogs_assert(amf_ue->ue_ambr.downlink && amf_ue->ue_ambr.uplink);

        ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present =
            NGAP_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

        UEAggregateMaximumBitRate = &ie->value.choice.UEAggregateMaximumBitRate;

        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateUL,
                amf_ue->ue_ambr.uplink);
        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateDL,
                amf_ue->ue_ambr.downlink);

        ran_ue->ue_ambr_sent = true;
    }

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_GUAMI;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_InitialContextSetupRequestIEs__value_PR_GUAMI;

    GUAMI = &ie->value.choice.GUAMI;

    if (gmmbuf || n2smbuf) {
        NGAP_NAS_PDU_t *nAS_PDU = NULL;
        OCTET_STRING_t *transfer = NULL;
        NGAP_S_NSSAI_t *s_NSSAI = NULL;
        NGAP_SST_t *sST = NULL;

        ogs_assert(n2smbuf);

        ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtReq;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present = NGAP_InitialContextSetupRequestIEs__value_PR_PDUSessionResourceSetupListCxtReq;

        PDUSessionList = &ie->value.choice.PDUSessionResourceSetupListCxtReq;

        PDUSessionItem = CALLOC(1,
                sizeof(struct NGAP_PDUSessionResourceSetupItemCxtReq));
        ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

        if (gmmbuf) {
            PDUSessionItem->nAS_PDU = nAS_PDU = CALLOC(1, sizeof(*nAS_PDU));
            ogs_assert(nAS_PDU);

            nAS_PDU->size = gmmbuf->len;
            nAS_PDU->buf = CALLOC(nAS_PDU->size, sizeof(uint8_t));
            memcpy(nAS_PDU->buf, gmmbuf->data, nAS_PDU->size);
            ogs_pkbuf_free(gmmbuf);
        }

        PDUSessionItem->pDUSessionID = sess->psi;

        s_NSSAI = &PDUSessionItem->s_NSSAI;
        sST = &s_NSSAI->sST;

        ogs_asn_uint8_to_OCTET_STRING(sess->s_nssai.sst, sST);
        if (sess->s_nssai.sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
            s_NSSAI->sD = CALLOC(1, sizeof(NGAP_SD_t));
            ogs_asn_uint24_to_OCTET_STRING(sess->s_nssai.sd, s_NSSAI->sD);
        }

        transfer = &PDUSessionItem->pDUSessionResourceSetupRequestTransfer;
        transfer->size = n2smbuf->len;
        transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
        memcpy(transfer->buf, n2smbuf->data, transfer->size);
        ogs_pkbuf_free(n2smbuf);
    }

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AllowedNSSAI;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_AllowedNSSAI;

    AllowedNSSAI = &ie->value.choice.AllowedNSSAI;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities;

    UESecurityCapabilities = &ie->value.choice.UESecurityCapabilities;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SecurityKey;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_SecurityKey;

    SecurityKey = &ie->value.choice.SecurityKey;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;

    ogs_assert(amf_ue->guami);
    ogs_asn_buffer_to_OCTET_STRING(&amf_ue->guami->plmn_id, OGS_PLMN_ID_LEN,
            &GUAMI->pLMNIdentity);
    ogs_ngap_uint8_to_AMFRegionID(ogs_amf_region_id(&amf_ue->guami->amf_id),
            &GUAMI->aMFRegionID);
    ogs_ngap_uint16_to_AMFSetID(ogs_amf_set_id(&amf_ue->guami->amf_id),
            &GUAMI->aMFSetID);
    ogs_ngap_uint8_to_AMFPointer(ogs_amf_pointer(&amf_ue->guami->amf_id),
            &GUAMI->aMFPointer);

    ogs_assert(amf_ue->allowed_nssai.num_of_s_nssai);
    for (i = 0; i < amf_ue->allowed_nssai.num_of_s_nssai; i++) {
        NGAP_AllowedNSSAI_Item_t *NGAP_AllowedNSSAI_Item = NULL;
        NGAP_S_NSSAI_t *s_NSSAI = NULL;
        NGAP_SST_t *sST = NULL;

        NGAP_AllowedNSSAI_Item = (NGAP_AllowedNSSAI_Item_t *)
                CALLOC(1, sizeof(NGAP_AllowedNSSAI_Item_t));
        s_NSSAI = &NGAP_AllowedNSSAI_Item->s_NSSAI;
        sST = &s_NSSAI->sST;

        ogs_asn_uint8_to_OCTET_STRING(
            amf_ue->allowed_nssai.s_nssai[i].sst, sST);
        if (amf_ue->allowed_nssai.s_nssai[i].sd.v !=
                OGS_S_NSSAI_NO_SD_VALUE) {
            s_NSSAI->sD = CALLOC(1, sizeof(NGAP_SD_t));
            ogs_asn_uint24_to_OCTET_STRING(
                amf_ue->allowed_nssai.s_nssai[i].sd, s_NSSAI->sD);
        }

        ASN_SEQUENCE_ADD(&AllowedNSSAI->list, NGAP_AllowedNSSAI_Item);
    }

    UESecurityCapabilities->nRencryptionAlgorithms.size = 2;
    UESecurityCapabilities->nRencryptionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    nRencryptionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->nRencryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->nRencryptionAlgorithms.buf[0] =
        (amf_ue->ue_security_capability.nr_ea << 1);

    UESecurityCapabilities->nRintegrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->nRintegrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    nRintegrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->nRintegrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->nRintegrityProtectionAlgorithms.buf[0] =
        (amf_ue->ue_security_capability.nr_ia << 1);

    UESecurityCapabilities->eUTRAencryptionAlgorithms.size = 2;
    UESecurityCapabilities->eUTRAencryptionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    eUTRAencryptionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->eUTRAencryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->eUTRAencryptionAlgorithms.buf[0] =
        (amf_ue->ue_security_capability.eutra_ea << 1);

    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    eUTRAintegrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.buf[0] =
        (amf_ue->ue_security_capability.eutra_ia << 1);

    SecurityKey->size = OGS_SHA256_DIGEST_SIZE;
    SecurityKey->buf = CALLOC(SecurityKey->size, sizeof(uint8_t));
    SecurityKey->bits_unused = 0;
    memcpy(SecurityKey->buf, amf_ue->kgnb, SecurityKey->size);

    if (amf_ue->ueRadioCapability.buf && amf_ue->ueRadioCapability.size) {
        /* Set UeRadioCapability if exists */
        NGAP_UERadioCapability_t *UERadioCapability = NULL;

        ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UERadioCapability;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_InitialContextSetupRequestIEs__value_PR_UERadioCapability;

        UERadioCapability = &ie->value.choice.UERadioCapability;

        ogs_assert(UERadioCapability);
        ogs_asn_buffer_to_OCTET_STRING(
                amf_ue->ueRadioCapability.buf, amf_ue->ueRadioCapability.size,
                UERadioCapability);
    }

    if (amf_ue->masked_imeisv_len) {
        ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_MaskedIMEISV;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_InitialContextSetupRequestIEs__value_PR_MaskedIMEISV;

        MaskedIMEISV = &ie->value.choice.MaskedIMEISV;

        MaskedIMEISV->size = amf_ue->masked_imeisv_len;
        MaskedIMEISV->buf = CALLOC(MaskedIMEISV->size, sizeof(uint8_t));
        MaskedIMEISV->bits_unused = 0;
        memcpy(MaskedIMEISV->buf, amf_ue->masked_imeisv, MaskedIMEISV->size);
    }

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_ue_context_release_command(
    ran_ue_t *ran_ue, NGAP_Cause_PR group, long cause)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UEContextReleaseCommand_t *UEContextReleaseCommand = NULL;

    NGAP_UEContextReleaseCommand_IEs_t *ie = NULL;
    NGAP_UE_NGAP_IDs_t *UE_NGAP_IDs = NULL;
    NGAP_Cause_t *Cause = NULL;

    ogs_assert(ran_ue);

    ogs_debug("UEContextReleaseCommand");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = NGAP_ProcedureCode_id_UEContextRelease;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_UEContextReleaseCommand;

    UEContextReleaseCommand =
        &initiatingMessage->value.choice.UEContextReleaseCommand;

    ie = CALLOC(1, sizeof(NGAP_UEContextReleaseCommand_IEs_t));
    ASN_SEQUENCE_ADD(&UEContextReleaseCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UE_NGAP_IDs;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_UEContextReleaseCommand_IEs__value_PR_UE_NGAP_IDs;

    UE_NGAP_IDs = &ie->value.choice.UE_NGAP_IDs;

    ie = CALLOC(1, sizeof(NGAP_UEContextReleaseCommand_IEs_t));
    ASN_SEQUENCE_ADD(&UEContextReleaseCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_UEContextReleaseCommand_IEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    if (ran_ue->ran_ue_ngap_id == INVALID_UE_NGAP_ID) {
        UE_NGAP_IDs->present = NGAP_UE_NGAP_IDs_PR_aMF_UE_NGAP_ID;
        asn_uint642INTEGER(
                &UE_NGAP_IDs->choice.aMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    } else {
        UE_NGAP_IDs->present = NGAP_UE_NGAP_IDs_PR_uE_NGAP_ID_pair;
        UE_NGAP_IDs->choice.uE_NGAP_ID_pair = 
            CALLOC(1, sizeof(NGAP_UE_NGAP_ID_pair_t));
        asn_uint642INTEGER(
            &UE_NGAP_IDs->choice.uE_NGAP_ID_pair->aMF_UE_NGAP_ID,
            ran_ue->amf_ue_ngap_id);
        UE_NGAP_IDs->choice.uE_NGAP_ID_pair->rAN_UE_NGAP_ID =
            ran_ue->ran_ue_ngap_id;
    }

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_ue_build_pdu_session_resource_setup_request(
    amf_ue_t *amf_ue, ogs_pkbuf_t *gmmbuf)
{
    ran_ue_t *ran_ue = NULL;
    amf_sess_t *sess = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_PDUSessionResourceSetupRequest_t *PDUSessionResourceSetupRequest;

    NGAP_PDUSessionResourceSetupRequestIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;
    NGAP_UEAggregateMaximumBitRate_t *UEAggregateMaximumBitRate = NULL;

    NGAP_PDUSessionResourceSetupListSUReq_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceSetupItemSUReq_t *PDUSessionItem = NULL;

    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

    ogs_debug("PDUSessionResourceSetupRequest(UE)");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_PDUSessionResourceSetup;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_PDUSessionResourceSetupRequest;

    PDUSessionResourceSetupRequest =
        &initiatingMessage->value.choice.PDUSessionResourceSetupRequest;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PDUSessionResourceSetupRequestIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PDUSessionResourceSetupRequestIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;

    if (gmmbuf) {
        ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&PDUSessionResourceSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_NAS_PDU;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present =
            NGAP_PDUSessionResourceSetupRequestIEs__value_PR_NAS_PDU;

        NAS_PDU = &ie->value.choice.NAS_PDU;

        NAS_PDU->size = gmmbuf->len;
        NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
        memcpy(NAS_PDU->buf, gmmbuf->data, NAS_PDU->size);
        ogs_pkbuf_free(gmmbuf);
    }

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        NGAP_S_NSSAI_t *s_NSSAI = NULL;
        NGAP_SST_t *sST = NULL;
        OCTET_STRING_t *transfer = NULL;

        if (!sess->transfer.pdu_session_resource_setup_request) continue;

        if (!PDUSessionList) {
            ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
            ASN_SEQUENCE_ADD(&PDUSessionResourceSetupRequest->protocolIEs, ie);

            ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListSUReq;
            ie->criticality = NGAP_Criticality_reject;
            ie->value.present = NGAP_PDUSessionResourceSetupRequestIEs__value_PR_PDUSessionResourceSetupListSUReq;

            PDUSessionList = &ie->value.choice.PDUSessionResourceSetupListSUReq;
        }

        PDUSessionItem =
            CALLOC(1, sizeof(struct NGAP_PDUSessionResourceSetupItemSUReq));
        ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

        PDUSessionItem->pDUSessionID = sess->psi;

        s_NSSAI = &PDUSessionItem->s_NSSAI;
        sST = &s_NSSAI->sST;
        ogs_asn_uint8_to_OCTET_STRING(sess->s_nssai.sst, sST);
        if (sess->s_nssai.sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
            s_NSSAI->sD = CALLOC(1, sizeof(NGAP_SD_t));
            ogs_asn_uint24_to_OCTET_STRING(sess->s_nssai.sd, s_NSSAI->sD);
        }

        transfer = &PDUSessionItem->pDUSessionResourceSetupRequestTransfer;
        transfer->size = sess->transfer.pdu_session_resource_setup_request->len;
        transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
        memcpy(transfer->buf,
                sess->transfer.pdu_session_resource_setup_request->data,
                transfer->size);
    }

    ogs_assert(PDUSessionList);
    ogs_assert(PDUSessionList->list.count);

    /*
     * TS 38.413
     * 8.2.1. PDU Session Resource Setup
     * 8.2.1.2. Successful Operation
     *
     * The UE Aggregate Maximum Bit Rate IE should be sent to the NG-RAN node
     * if the AMF has not sent it previously.
     */
    if (ran_ue->ue_ambr_sent == false &&
        PDU_RES_SETUP_REQ_TRANSFER_NEEDED(amf_ue) == true) {
        ogs_assert(amf_ue->ue_ambr.downlink && amf_ue->ue_ambr.uplink);

        ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&PDUSessionResourceSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_PDUSessionResourceSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

        UEAggregateMaximumBitRate = &ie->value.choice.UEAggregateMaximumBitRate;

        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateUL,
                amf_ue->ue_ambr.uplink);
        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateDL,
                amf_ue->ue_ambr.downlink);

        ran_ue->ue_ambr_sent = true;
    }

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_sess_build_pdu_session_resource_setup_request(
    amf_sess_t *sess, ogs_pkbuf_t *gmmbuf, ogs_pkbuf_t *n2smbuf)
{
    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_PDUSessionResourceSetupRequest_t *PDUSessionResourceSetupRequest;

    NGAP_PDUSessionResourceSetupRequestIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;

    NGAP_PDUSessionResourceSetupListSUReq_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceSetupItemSUReq_t *PDUSessionItem = NULL;
    NGAP_NAS_PDU_t *pDUSessionNAS_PDU = NULL;
    NGAP_S_NSSAI_t *s_NSSAI = NULL;
    NGAP_SST_t *sST = NULL;
    OCTET_STRING_t *transfer = NULL;

    NGAP_UEAggregateMaximumBitRate_t *UEAggregateMaximumBitRate = NULL;

    ogs_assert(n2smbuf);
    ogs_assert(sess);

    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

    ogs_debug("PDUSessionResourceSetupRequest(Session)");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_PDUSessionResourceSetup;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_PDUSessionResourceSetupRequest;

    PDUSessionResourceSetupRequest =
        &initiatingMessage->value.choice.PDUSessionResourceSetupRequest;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PDUSessionResourceSetupRequestIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PDUSessionResourceSetupRequestIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListSUReq;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceSetupRequestIEs__value_PR_PDUSessionResourceSetupListSUReq;

    PDUSessionList = &ie->value.choice.PDUSessionResourceSetupListSUReq;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;

    PDUSessionItem =
        CALLOC(1, sizeof(struct NGAP_PDUSessionResourceSetupItemSUReq));
    ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

    PDUSessionItem->pDUSessionID = sess->psi;

    if (gmmbuf) {
        PDUSessionItem->pDUSessionNAS_PDU =
            pDUSessionNAS_PDU = CALLOC(1, sizeof(NGAP_NAS_PDU_t));
        pDUSessionNAS_PDU->size = gmmbuf->len;
        pDUSessionNAS_PDU->buf =
            CALLOC(pDUSessionNAS_PDU->size, sizeof(uint8_t));
        memcpy(pDUSessionNAS_PDU->buf, gmmbuf->data, pDUSessionNAS_PDU->size);
        ogs_pkbuf_free(gmmbuf);
    }

    s_NSSAI = &PDUSessionItem->s_NSSAI;
    sST = &s_NSSAI->sST;
    ogs_asn_uint8_to_OCTET_STRING(sess->s_nssai.sst, sST);
    if (sess->s_nssai.sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
        s_NSSAI->sD = CALLOC(1, sizeof(NGAP_SD_t));
        ogs_asn_uint24_to_OCTET_STRING(sess->s_nssai.sd, s_NSSAI->sD);
    }

    transfer = &PDUSessionItem->pDUSessionResourceSetupRequestTransfer;
    transfer->size = n2smbuf->len;
    transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
    memcpy(transfer->buf, n2smbuf->data, transfer->size);
    ogs_pkbuf_free(n2smbuf);

    /*
     * TS 38.413
     * 8.2.1. PDU Session Resource Setup
     * 8.2.1.2. Successful Operation
     *
     * The UE Aggregate Maximum Bit Rate IE should be sent to the NG-RAN node
     * if the AMF has not sent it previously.
     */
    if (ran_ue->ue_ambr_sent == false) {
        ogs_assert(amf_ue->ue_ambr.downlink && amf_ue->ue_ambr.uplink);

        ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&PDUSessionResourceSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_PDUSessionResourceSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

        UEAggregateMaximumBitRate = &ie->value.choice.UEAggregateMaximumBitRate;

        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateUL,
                amf_ue->ue_ambr.uplink);
        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateDL,
                amf_ue->ue_ambr.downlink);

        ran_ue->ue_ambr_sent = true;
    }

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_pdu_session_resource_modify_request(
    amf_sess_t *sess, ogs_pkbuf_t *gmmbuf, ogs_pkbuf_t *n2smbuf)
{
    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_PDUSessionResourceModifyRequest_t *PDUSessionResourceModifyRequest;

    NGAP_PDUSessionResourceModifyRequestIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;

    NGAP_PDUSessionResourceModifyListModReq_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceModifyItemModReq_t *PDUSessionItem = NULL;
    NGAP_NAS_PDU_t *nAS_PDU = NULL;
    OCTET_STRING_t *transfer = NULL;

    ogs_assert(gmmbuf);
    ogs_assert(n2smbuf);
    ogs_assert(sess);

    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

    ogs_debug("PDUSessionResourceModifyRequest");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_PDUSessionResourceModify;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_PDUSessionResourceModifyRequest;

    PDUSessionResourceModifyRequest =
        &initiatingMessage->value.choice.PDUSessionResourceModifyRequest;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceModifyRequestIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceModifyRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PDUSessionResourceModifyRequestIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceModifyRequestIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceModifyRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PDUSessionResourceModifyRequestIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceModifyRequestIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceModifyRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceModifyListModReq;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceModifyRequestIEs__value_PR_PDUSessionResourceModifyListModReq;

    PDUSessionList = &ie->value.choice.PDUSessionResourceModifyListModReq;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;

    PDUSessionItem =
        CALLOC(1, sizeof(struct NGAP_PDUSessionResourceModifyItemModReq));
    ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

    PDUSessionItem->pDUSessionID = sess->psi;

    PDUSessionItem->nAS_PDU = nAS_PDU = CALLOC(1, sizeof(NGAP_NAS_PDU_t));
    nAS_PDU->size = gmmbuf->len;
    nAS_PDU->buf = CALLOC(nAS_PDU->size, sizeof(uint8_t));
    memcpy(nAS_PDU->buf, gmmbuf->data, nAS_PDU->size);
    ogs_pkbuf_free(gmmbuf);

    transfer = &PDUSessionItem->pDUSessionResourceModifyRequestTransfer;
    transfer->size = n2smbuf->len;
    transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
    memcpy(transfer->buf, n2smbuf->data, transfer->size);
    ogs_pkbuf_free(n2smbuf);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_pdu_session_resource_release_command(
    amf_sess_t *sess, ogs_pkbuf_t *gmmbuf, ogs_pkbuf_t *n2smbuf)
{
    amf_ue_t *amf_ue = NULL;
    ran_ue_t *ran_ue = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_PDUSessionResourceReleaseCommand_t *PDUSessionResourceReleaseCommand;

    NGAP_PDUSessionResourceReleaseCommandIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;

    NGAP_PDUSessionResourceToReleaseListRelCmd_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceToReleaseItemRelCmd_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    ogs_assert(n2smbuf);
    ogs_assert(sess);

    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

    ogs_debug("PDUSessionResourceReleaseCommand");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_PDUSessionResourceRelease;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_PDUSessionResourceReleaseCommand;

    PDUSessionResourceReleaseCommand =
        &initiatingMessage->value.choice.PDUSessionResourceReleaseCommand;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceReleaseCommandIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceReleaseCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PDUSessionResourceReleaseCommandIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceReleaseCommandIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceReleaseCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PDUSessionResourceReleaseCommandIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;

    if (gmmbuf) {
        ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceReleaseCommandIEs_t));
        ASN_SEQUENCE_ADD(&PDUSessionResourceReleaseCommand->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_NAS_PDU;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_PDUSessionResourceReleaseCommandIEs__value_PR_NAS_PDU;

        NAS_PDU = &ie->value.choice.NAS_PDU;

        NAS_PDU->size = gmmbuf->len;
        NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
        memcpy(NAS_PDU->buf, gmmbuf->data, NAS_PDU->size);
        ogs_pkbuf_free(gmmbuf);
    }

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceReleaseCommandIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceReleaseCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceToReleaseListRelCmd;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceReleaseCommandIEs__value_PR_PDUSessionResourceToReleaseListRelCmd;

    PDUSessionList = &ie->value.choice.PDUSessionResourceToReleaseListRelCmd;

    PDUSessionItem =
        CALLOC(1, sizeof(struct NGAP_PDUSessionResourceToReleaseItemRelCmd));
    ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

    PDUSessionItem->pDUSessionID = sess->psi;

    transfer = &PDUSessionItem->pDUSessionResourceReleaseCommandTransfer;
    transfer->size = n2smbuf->len;
    transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
    memcpy(transfer->buf, n2smbuf->data, transfer->size);
    ogs_pkbuf_free(n2smbuf);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_paging(amf_ue_t *amf_ue)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_Paging_t *Paging = NULL;

    NGAP_PagingIEs_t *ie = NULL;

    NGAP_UEPagingIdentity_t *UEPagingIdentity = NULL;
    NGAP_FiveG_S_TMSI_t *fiveG_S_TMSI = NULL;
    NGAP_AMFSetID_t *aMFSetID = NULL;
    NGAP_AMFPointer_t *aMFPointer = NULL;
    NGAP_FiveG_TMSI_t *fiveG_TMSI = NULL;
    NGAP_TAIListForPaging_t *TAIList = NULL;
    NGAP_TAIListForPagingItem_t *TAIItem = NULL;
    NGAP_TAI_t *tAI = NULL;

    ogs_assert(amf_ue);
    ogs_debug("Paging");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = NGAP_ProcedureCode_id_Paging;
    initiatingMessage->criticality = NGAP_Criticality_ignore;
    initiatingMessage->value.present = NGAP_InitiatingMessage__value_PR_Paging;

    Paging = &initiatingMessage->value.choice.Paging;

    ie = CALLOC(1, sizeof(NGAP_PagingIEs_t));
    ASN_SEQUENCE_ADD(&Paging->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UEPagingIdentity;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_PagingIEs__value_PR_UEPagingIdentity;

    UEPagingIdentity = &ie->value.choice.UEPagingIdentity;

    UEPagingIdentity->present = NGAP_UEPagingIdentity_PR_fiveG_S_TMSI;
    UEPagingIdentity->choice.fiveG_S_TMSI = fiveG_S_TMSI =
        CALLOC(1, sizeof(NGAP_FiveG_S_TMSI_t));
    ogs_assert(fiveG_S_TMSI);

    aMFSetID = &fiveG_S_TMSI->aMFSetID;
    aMFPointer = &fiveG_S_TMSI->aMFPointer;
    fiveG_TMSI = &fiveG_S_TMSI->fiveG_TMSI;

    ogs_ngap_uint16_to_AMFSetID(
            ogs_amf_set_id(&amf_ue->current.guti.amf_id), aMFSetID);
    ogs_ngap_uint8_to_AMFPointer(
            ogs_amf_pointer(&amf_ue->current.guti.amf_id), aMFPointer);
    ogs_asn_uint32_to_OCTET_STRING(amf_ue->current.guti.m_tmsi, fiveG_TMSI);

    ie = CALLOC(1, sizeof(NGAP_PagingIEs_t));
    ASN_SEQUENCE_ADD(&Paging->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_TAIListForPaging;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_PagingIEs__value_PR_TAIListForPaging;

    TAIList = &ie->value.choice.TAIListForPaging;

    TAIItem = CALLOC(1, sizeof(NGAP_TAIListForPagingItem_t));
    ASN_SEQUENCE_ADD(&TAIList->list, TAIItem);

    tAI = &TAIItem->tAI;
    ogs_ngap_5gs_tai_to_ASN(&amf_ue->nr_tai, tAI);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_downlink_ran_configuration_transfer(
    NGAP_SONConfigurationTransfer_t *transfer)
{
    int rv;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_DownlinkRANConfigurationTransfer_t
        *DownlinkRANConfigurationTransfer = NULL;

    NGAP_DownlinkRANConfigurationTransferIEs_t *ie = NULL;
    NGAP_SONConfigurationTransfer_t *SONConfigurationTransfer = NULL;

    ogs_assert(transfer);
    ogs_debug("DownlinkRANConfigurationTransfer");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));
    ogs_assert(pdu.choice.initiatingMessage);

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_DownlinkRANConfigurationTransfer;
    initiatingMessage->criticality = NGAP_Criticality_ignore;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_DownlinkRANConfigurationTransfer;

    DownlinkRANConfigurationTransfer =
        &initiatingMessage->value.choice.DownlinkRANConfigurationTransfer;

    ie = CALLOC(1, sizeof(NGAP_DownlinkRANConfigurationTransferIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&DownlinkRANConfigurationTransfer->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SONConfigurationTransferDL;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_DownlinkRANConfigurationTransferIEs__value_PR_SONConfigurationTransfer;

    SONConfigurationTransfer = &ie->value.choice.SONConfigurationTransfer;

    rv = ogs_asn_copy_ie(&asn_DEF_NGAP_SONConfigurationTransfer,
            transfer, SONConfigurationTransfer);
    ogs_assert(rv == OGS_OK);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_path_switch_ack(amf_ue_t *amf_ue)
{
    int i;

    ran_ue_t *ran_ue = NULL;
    amf_sess_t *sess = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_PathSwitchRequestAcknowledge_t *PathSwitchRequestAcknowledge = NULL;

    NGAP_PathSwitchRequestAcknowledgeIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_SecurityContext_t *SecurityContext = NULL;
    NGAP_PDUSessionResourceSwitchedList_t *PDUSessionResourceSwitchedList;
    NGAP_AllowedNSSAI_t *AllowedNSSAI = NULL;

    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

    ogs_debug("PathSwitchAcknowledge");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = NGAP_ProcedureCode_id_PathSwitchRequest;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_PathSwitchRequestAcknowledge;

    PathSwitchRequestAcknowledge =
        &successfulOutcome->value.choice.PathSwitchRequestAcknowledge;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequestAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PathSwitchRequestAcknowledgeIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequestAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PathSwitchRequestAcknowledgeIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequestAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SecurityContext;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PathSwitchRequestAcknowledgeIEs__value_PR_SecurityContext;

    SecurityContext = &ie->value.choice.SecurityContext;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequestAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSwitchedList;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_PathSwitchRequestAcknowledgeIEs__value_PR_PDUSessionResourceSwitchedList;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;

    SecurityContext->nextHopChainingCount = amf_ue->nhcc;
    SecurityContext->nextHopNH.size = OGS_SHA256_DIGEST_SIZE;
    SecurityContext->nextHopNH.buf =
        CALLOC(SecurityContext->nextHopNH.size, sizeof(uint8_t));
    SecurityContext->nextHopNH.bits_unused = 0;
    memcpy(SecurityContext->nextHopNH.buf,
            amf_ue->nh, SecurityContext->nextHopNH.size);

    PDUSessionResourceSwitchedList =
        &ie->value.choice.PDUSessionResourceSwitchedList;

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        OCTET_STRING_t *transfer = NULL;
        NGAP_PDUSessionResourceSwitchedItem_t *PDUSessionItem = NULL;

        if (!sess->transfer.path_switch_request_ack) continue;

        if (!PDUSessionResourceSwitchedList) {
            ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestAcknowledgeIEs_t));
            ASN_SEQUENCE_ADD(&PathSwitchRequestAcknowledge->protocolIEs, ie);

            ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSwitchedList;
            ie->criticality = NGAP_Criticality_ignore;
            ie->value.present = NGAP_PathSwitchRequestAcknowledgeIEs__value_PR_PDUSessionResourceSwitchedList;

            PDUSessionResourceSwitchedList =
                &ie->value.choice.PDUSessionResourceSwitchedList;
        }

        PDUSessionItem =
            CALLOC(1, sizeof(NGAP_PDUSessionResourceSwitchedItem_t));
        ASN_SEQUENCE_ADD(&PDUSessionResourceSwitchedList->list, PDUSessionItem);

        PDUSessionItem->pDUSessionID = sess->psi;

        transfer = &PDUSessionItem->pathSwitchRequestAcknowledgeTransfer;
        transfer->size = sess->transfer.path_switch_request_ack->len;
        transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
        memcpy(transfer->buf, sess->transfer.path_switch_request_ack->data,
                transfer->size);
    }

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequestAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AllowedNSSAI;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PathSwitchRequestAcknowledgeIEs__value_PR_AllowedNSSAI;

    AllowedNSSAI = &ie->value.choice.AllowedNSSAI;

    ogs_assert(amf_ue->allowed_nssai.num_of_s_nssai);
    for (i = 0; i < amf_ue->allowed_nssai.num_of_s_nssai; i++) {
        NGAP_AllowedNSSAI_Item_t *NGAP_AllowedNSSAI_Item = NULL;
        NGAP_S_NSSAI_t *s_NSSAI = NULL;
        NGAP_SST_t *sST = NULL;

        NGAP_AllowedNSSAI_Item = (NGAP_AllowedNSSAI_Item_t *)
                CALLOC(1, sizeof(NGAP_AllowedNSSAI_Item_t));
        s_NSSAI = &NGAP_AllowedNSSAI_Item->s_NSSAI;
        sST = &s_NSSAI->sST;

        ogs_asn_uint8_to_OCTET_STRING(
            amf_ue->allowed_nssai.s_nssai[i].sst, sST);
        if (amf_ue->allowed_nssai.s_nssai[i].sd.v !=
                OGS_S_NSSAI_NO_SD_VALUE) {
            s_NSSAI->sD = CALLOC(1, sizeof(NGAP_SD_t));
            ogs_asn_uint24_to_OCTET_STRING(
                amf_ue->allowed_nssai.s_nssai[i].sd, s_NSSAI->sD);
        }

        ASN_SEQUENCE_ADD(&AllowedNSSAI->list, NGAP_AllowedNSSAI_Item);
    }

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_handover_request(ran_ue_t *target_ue)
{
    int i;

    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_HandoverRequest_t *HandoverRequest = NULL;

    NGAP_HandoverRequestIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_HandoverType_t *HandoverType = NULL;
    NGAP_Cause_t *Cause = NULL;
    NGAP_UEAggregateMaximumBitRate_t *UEAggregateMaximumBitRate = NULL;
    NGAP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;
    NGAP_SecurityContext_t *SecurityContext = NULL;
    NGAP_PDUSessionResourceSetupListHOReq_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceSetupItemHOReq_t *PDUSessionItem = NULL;
    NGAP_AllowedNSSAI_t *AllowedNSSAI = NULL;
    NGAP_MaskedIMEISV_t *MaskedIMEISV = NULL;
    NGAP_SourceToTarget_TransparentContainer_t
        *SourceToTarget_TransparentContainer = NULL;
    NGAP_GUAMI_t *GUAMI = NULL;

    ogs_assert(target_ue);
    amf_ue = target_ue->amf_ue;
    ogs_assert(amf_ue);

    ogs_debug("HandoverRequest");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_HandoverResourceAllocation;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_HandoverRequest;

    HandoverRequest = &initiatingMessage->value.choice.HandoverRequest;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            target_ue->ran_ue_ngap_id, (long long)target_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, target_ue->amf_ue_ngap_id);

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_HandoverType;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_HandoverType;

    HandoverType = &ie->value.choice.HandoverType;

    ogs_debug("    HandoverType[%d]", (int)amf_ue->handover.type);

    *HandoverType = amf_ue->handover.type;;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    Cause->present = amf_ue->handover.group;
    Cause->choice.radioNetwork = amf_ue->handover.cause;
    ogs_debug("    Group[%d] Cause[%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

    if (HANDOVER_REQUEST_TRANSFER_NEEDED(amf_ue) == true) {
        ogs_assert(amf_ue->ue_ambr.downlink && amf_ue->ue_ambr.uplink);

        ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present =
            NGAP_HandoverRequestIEs__value_PR_UEAggregateMaximumBitRate;

        UEAggregateMaximumBitRate = &ie->value.choice.UEAggregateMaximumBitRate;

        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateUL,
                amf_ue->ue_ambr.uplink);
        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateDL,
                amf_ue->ue_ambr.downlink);

        target_ue->ue_ambr_sent = true;
    }

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_HandoverRequestIEs__value_PR_UESecurityCapabilities;

    UESecurityCapabilities = &ie->value.choice.UESecurityCapabilities;

    UESecurityCapabilities->nRencryptionAlgorithms.size = 2;
    UESecurityCapabilities->nRencryptionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    nRencryptionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->nRencryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->nRencryptionAlgorithms.buf[0] =
        (amf_ue->ue_security_capability.nr_ea << 1);

    UESecurityCapabilities->nRintegrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->nRintegrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    nRintegrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->nRintegrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->nRintegrityProtectionAlgorithms.buf[0] =
        (amf_ue->ue_security_capability.nr_ia << 1);

    UESecurityCapabilities->eUTRAencryptionAlgorithms.size = 2;
    UESecurityCapabilities->eUTRAencryptionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    eUTRAencryptionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->eUTRAencryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->eUTRAencryptionAlgorithms.buf[0] =
        (amf_ue->ue_security_capability.eutra_ea << 1);

    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    eUTRAintegrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.buf[0] =
        (amf_ue->ue_security_capability.eutra_ia << 1);

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SecurityContext;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_SecurityContext;

    SecurityContext = &ie->value.choice.SecurityContext;

    SecurityContext->nextHopChainingCount = amf_ue->nhcc;
    SecurityContext->nextHopNH.size = OGS_SHA256_DIGEST_SIZE;
    SecurityContext->nextHopNH.buf =
        CALLOC(SecurityContext->nextHopNH.size, sizeof(uint8_t));
    SecurityContext->nextHopNH.bits_unused = 0;
    memcpy(SecurityContext->nextHopNH.buf,
            amf_ue->nh, SecurityContext->nextHopNH.size);

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        OCTET_STRING_t *transfer = NULL;
        NGAP_S_NSSAI_t *s_NSSAI = NULL;
        NGAP_SST_t *sST = NULL;

        if (!sess->transfer.handover_request) continue;

        if (!PDUSessionList) {
            ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
            ogs_assert(ie);
            ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

            ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListHOReq;
            ie->criticality = NGAP_Criticality_reject;
            ie->value.present = NGAP_HandoverRequestIEs__value_PR_PDUSessionResourceSetupListHOReq;

            PDUSessionList = &ie->value.choice.PDUSessionResourceSetupListHOReq;
        }

        PDUSessionItem = CALLOC(1,
                sizeof(struct NGAP_PDUSessionResourceSetupItemHOReq));
        ogs_assert(PDUSessionItem);
        ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

        PDUSessionItem->pDUSessionID = sess->psi;

        s_NSSAI = &PDUSessionItem->s_NSSAI;
        sST = &s_NSSAI->sST;

        ogs_asn_uint8_to_OCTET_STRING(sess->s_nssai.sst, sST);
        if (sess->s_nssai.sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
            s_NSSAI->sD = CALLOC(1, sizeof(NGAP_SD_t));
            ogs_asn_uint24_to_OCTET_STRING(sess->s_nssai.sd, s_NSSAI->sD);
        }

        transfer = &PDUSessionItem->handoverRequestTransfer;
        transfer->size = sess->transfer.handover_request->len;
        transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
        memcpy(transfer->buf,
                sess->transfer.handover_request->data, transfer->size);
    }

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AllowedNSSAI;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_AllowedNSSAI;

    AllowedNSSAI = &ie->value.choice.AllowedNSSAI;

    ogs_assert(amf_ue->allowed_nssai.num_of_s_nssai);
    for (i = 0; i < amf_ue->allowed_nssai.num_of_s_nssai; i++) {
        NGAP_AllowedNSSAI_Item_t *NGAP_AllowedNSSAI_Item = NULL;
        NGAP_S_NSSAI_t *s_NSSAI = NULL;
        NGAP_SST_t *sST = NULL;

        NGAP_AllowedNSSAI_Item = (NGAP_AllowedNSSAI_Item_t *)
                CALLOC(1, sizeof(NGAP_AllowedNSSAI_Item_t));
        s_NSSAI = &NGAP_AllowedNSSAI_Item->s_NSSAI;
        sST = &s_NSSAI->sST;

        ogs_asn_uint8_to_OCTET_STRING(
            amf_ue->allowed_nssai.s_nssai[i].sst, sST);
        if (amf_ue->allowed_nssai.s_nssai[i].sd.v !=
                OGS_S_NSSAI_NO_SD_VALUE) {
            s_NSSAI->sD = CALLOC(1, sizeof(NGAP_SD_t));
            ogs_asn_uint24_to_OCTET_STRING(
                amf_ue->allowed_nssai.s_nssai[i].sd, s_NSSAI->sD);
        }

        ASN_SEQUENCE_ADD(&AllowedNSSAI->list, NGAP_AllowedNSSAI_Item);
    }

    /* TS23.003 6.2.2 Composition of IMEISV
     *
     * The International Mobile station Equipment Identity and
     * Software Version Number (IMEISV) is composed.
     *
     * TAC(8 digits) - SNR(6 digits) - SVN(2 digits)
     * IMEISV(16 digits) ==> 8bytes
     */
    if (amf_ue->masked_imeisv_len == OGS_MAX_IMEISV_LEN) {
        ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_MaskedIMEISV;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_HandoverRequestIEs__value_PR_MaskedIMEISV;

        MaskedIMEISV = &ie->value.choice.MaskedIMEISV;

        MaskedIMEISV->size = amf_ue->masked_imeisv_len;
        MaskedIMEISV->buf = CALLOC(MaskedIMEISV->size, sizeof(uint8_t));
        MaskedIMEISV->bits_unused = 0;
        memcpy(MaskedIMEISV->buf, amf_ue->masked_imeisv, MaskedIMEISV->size);
    }

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SourceToTarget_TransparentContainer;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_SourceToTarget_TransparentContainer;

    SourceToTarget_TransparentContainer =
        &ie->value.choice.SourceToTarget_TransparentContainer;

    ogs_asn_buffer_to_OCTET_STRING(
            amf_ue->handover.container.buf, amf_ue->handover.container.size,
            SourceToTarget_TransparentContainer);

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_GUAMI;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_GUAMI;

    GUAMI = &ie->value.choice.GUAMI;

    ogs_assert(amf_ue->guami);
    ogs_asn_buffer_to_OCTET_STRING(&amf_ue->guami->plmn_id, OGS_PLMN_ID_LEN,
            &GUAMI->pLMNIdentity);
    ogs_ngap_uint8_to_AMFRegionID(ogs_amf_region_id(&amf_ue->guami->amf_id),
            &GUAMI->aMFRegionID);
    ogs_ngap_uint16_to_AMFSetID(ogs_amf_set_id(&amf_ue->guami->amf_id),
            &GUAMI->aMFSetID);
    ogs_ngap_uint8_to_AMFPointer(ogs_amf_pointer(&amf_ue->guami->amf_id),
            &GUAMI->aMFPointer);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_handover_preparation_failure(
    ran_ue_t *source_ue, NGAP_Cause_t *cause)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    NGAP_HandoverPreparationFailure_t *HandoverPreparationFailure = NULL;

    NGAP_HandoverPreparationFailureIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    ogs_assert(source_ue);
    ogs_assert(cause);

    ogs_debug("HandoverPreparationFailure");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_unsuccessfulOutcome;
    pdu.choice.unsuccessfulOutcome =
        CALLOC(1, sizeof(NGAP_UnsuccessfulOutcome_t));

    unsuccessfulOutcome = pdu.choice.unsuccessfulOutcome;
    unsuccessfulOutcome->procedureCode =
        NGAP_ProcedureCode_id_HandoverPreparation;
    unsuccessfulOutcome->criticality = NGAP_Criticality_reject;
    unsuccessfulOutcome->value.present =
        NGAP_UnsuccessfulOutcome__value_PR_HandoverPreparationFailure;

    HandoverPreparationFailure =
        &unsuccessfulOutcome->value.choice.HandoverPreparationFailure;

    ie = CALLOC(1, sizeof(NGAP_HandoverPreparationFailureIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverPreparationFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_HandoverPreparationFailureIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverPreparationFailureIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverPreparationFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_HandoverPreparationFailureIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            source_ue->ran_ue_ngap_id, (long long)source_ue->amf_ue_ngap_id);
    asn_uint642INTEGER(AMF_UE_NGAP_ID, source_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = source_ue->ran_ue_ngap_id;

    ie = CALLOC(1, sizeof(NGAP_HandoverPreparationFailureIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverPreparationFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_HandoverPreparationFailureIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    ogs_debug("    Group[%d] Cause[%d]",
            cause->present, (int)cause->choice.radioNetwork);
    Cause->present = cause->present;
    Cause->choice.radioNetwork = cause->choice.radioNetwork;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_handover_command(ran_ue_t *source_ue)
{
    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_HandoverCommand_t *HandoverCommand = NULL;

    NGAP_HandoverCommandIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_HandoverType_t *HandoverType = NULL;
    NGAP_PDUSessionResourceHandoverList_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceHandoverItem_t *PDUSessionItem = NULL;
    NGAP_TargetToSource_TransparentContainer_t
        *TargetToSource_TransparentContainer = NULL;

    ogs_assert(source_ue);
    amf_ue = source_ue->amf_ue;
    ogs_assert(amf_ue);

    ogs_debug("HandoverCommand");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode =
        NGAP_ProcedureCode_id_HandoverPreparation;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_HandoverCommand;

    HandoverCommand = &successfulOutcome->value.choice.HandoverCommand;

    ie = CALLOC(1, sizeof(NGAP_HandoverCommandIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverCommandIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverCommandIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverCommandIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            source_ue->ran_ue_ngap_id, (long long)source_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, source_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = source_ue->ran_ue_ngap_id;

    ie = CALLOC(1, sizeof(NGAP_HandoverCommandIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_HandoverType;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverCommandIEs__value_PR_HandoverType;

    HandoverType = &ie->value.choice.HandoverType;

    ogs_debug("    HandoverType[%d]", (int)amf_ue->handover.type);

    *HandoverType = amf_ue->handover.type;;

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        OCTET_STRING_t *transfer = NULL;

        if (!sess->transfer.handover_command) continue;

        if (!PDUSessionList) {
            ie = CALLOC(1, sizeof(NGAP_HandoverCommandIEs_t));
            ogs_assert(ie);
            ASN_SEQUENCE_ADD(&HandoverCommand->protocolIEs, ie);

            ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceHandoverList;
            ie->criticality = NGAP_Criticality_ignore;
            ie->value.present = NGAP_HandoverCommandIEs__value_PR_PDUSessionResourceHandoverList;

            PDUSessionList = &ie->value.choice.PDUSessionResourceHandoverList;
        }

        PDUSessionItem = CALLOC(1, sizeof(*PDUSessionItem));
        ogs_assert(PDUSessionItem);
        ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

        PDUSessionItem->pDUSessionID = sess->psi;

        transfer = &PDUSessionItem->handoverCommandTransfer;
        transfer->size = sess->transfer.handover_command->len;
        transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
        memcpy(transfer->buf,
                sess->transfer.handover_command->data, transfer->size);
    }

    ie = CALLOC(1, sizeof(NGAP_HandoverCommandIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_TargetToSource_TransparentContainer;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverCommandIEs__value_PR_TargetToSource_TransparentContainer;

    TargetToSource_TransparentContainer =
        &ie->value.choice.TargetToSource_TransparentContainer;

    ogs_asn_buffer_to_OCTET_STRING(
            amf_ue->handover.container.buf, amf_ue->handover.container.size,
            TargetToSource_TransparentContainer);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_handover_cancel_ack(ran_ue_t *source_ue)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_HandoverCancelAcknowledge_t *HandoverCancelAcknowledge = NULL;

    NGAP_HandoverCancelAcknowledgeIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;

    ogs_assert(source_ue);

    ogs_debug("HandoverCancelAcknowledge");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = NGAP_ProcedureCode_id_HandoverCancel;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_HandoverCancelAcknowledge;

    HandoverCancelAcknowledge =
        &successfulOutcome->value.choice.HandoverCancelAcknowledge;

    ie = CALLOC(1, sizeof(NGAP_HandoverCancelAcknowledgeIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverCancelAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_HandoverCancelAcknowledgeIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverCancelAcknowledgeIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverCancelAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_HandoverCancelAcknowledgeIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            source_ue->ran_ue_ngap_id, (long long)source_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, source_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = source_ue->ran_ue_ngap_id;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_uplink_ran_status_transfer(
    ran_ue_t *target_ue,
    NGAP_RANStatusTransfer_TransparentContainer_t *transfer)
{
    int rv;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_DownlinkRANStatusTransfer_t *DownlinkRANStatusTransfer = NULL;

    NGAP_DownlinkRANStatusTransferIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_RANStatusTransfer_TransparentContainer_t
        *RANStatusTransfer_TransparentContainer = NULL;

    ogs_assert(target_ue);
    ogs_assert(transfer);

    ogs_debug("DownlinkRANStatusTransfer");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_DownlinkRANStatusTransfer;
    initiatingMessage->criticality = NGAP_Criticality_ignore;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_DownlinkRANStatusTransfer;

    DownlinkRANStatusTransfer =
        &initiatingMessage->value.choice.DownlinkRANStatusTransfer;

    ie = CALLOC(1, sizeof(NGAP_DownlinkRANStatusTransferIEs_t));
    ASN_SEQUENCE_ADD(&DownlinkRANStatusTransfer->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_DownlinkRANStatusTransferIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_DownlinkRANStatusTransferIEs_t));
    ASN_SEQUENCE_ADD(&DownlinkRANStatusTransfer->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_DownlinkRANStatusTransferIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            target_ue->ran_ue_ngap_id, (long long)target_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, target_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = target_ue->ran_ue_ngap_id;

    ie = CALLOC(1, sizeof(NGAP_DownlinkRANStatusTransferIEs_t));
    ASN_SEQUENCE_ADD(&DownlinkRANStatusTransfer->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RANStatusTransfer_TransparentContainer;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_DownlinkRANStatusTransferIEs__value_PR_RANStatusTransfer_TransparentContainer;

    RANStatusTransfer_TransparentContainer =
        &ie->value.choice.RANStatusTransfer_TransparentContainer;

    rv = ogs_asn_copy_ie(
            &asn_DEF_NGAP_RANStatusTransfer_TransparentContainer,
            transfer, RANStatusTransfer_TransparentContainer);
    ogs_assert(rv == OGS_OK);

    return ogs_ngap_encode(&pdu);
}
