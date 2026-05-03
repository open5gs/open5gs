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

static void ngap_build_plmn_support_list(NGAP_PLMNSupportList_t *PLMNSupportList)
{
    int i, j;

    ogs_assert(PLMNSupportList);

    for (i = 0; i < amf_self()->num_of_plmn_support; i++) {
        NGAP_PLMNSupportItem_t *NGAP_PLMNSupportItem = NULL;
        NGAP_PLMNIdentity_t *pLMNIdentity = NULL;
        NGAP_SliceSupportList_t *sliceSupportList = NULL;

        /* Skip PLMNs with no S-NSSAIs (invalid NGAP configuration) */
        if (amf_self()->plmn_support[i].num_of_s_nssai == 0) {
            ogs_warn("Skipping PLMN at index %d with no S-NSSAIs", i);
            continue;
        }

        NGAP_PLMNSupportItem = (NGAP_PLMNSupportItem_t *)
                CALLOC(1, sizeof(NGAP_PLMNSupportItem_t));
        pLMNIdentity = &NGAP_PLMNSupportItem->pLMNIdentity;
        sliceSupportList = CALLOC(1, sizeof(*sliceSupportList));
        ogs_assert(sliceSupportList);
        NGAP_PLMNSupportItem->sliceSupportList = sliceSupportList;

        ogs_asn_buffer_to_OCTET_STRING(
                &amf_self()->plmn_support[i].plmn_id,
                OGS_PLMN_ID_LEN, pLMNIdentity);
        for (j = 0; j < amf_self()->plmn_support[i].num_of_s_nssai; j++) {
            NGAP_SliceSupportItem_t *NGAP_SliceSupportItem = NULL;
            NGAP_S_NSSAI_t *s_NSSAI = NULL;
            NGAP_SST_t *sST = NULL;

            NGAP_SliceSupportItem = (NGAP_SliceSupportItem_t *)
                    CALLOC(1, sizeof(NGAP_SliceSupportItem_t));
            s_NSSAI = CALLOC(1, sizeof(*s_NSSAI));
            ogs_assert(s_NSSAI);
            NGAP_SliceSupportItem->s_NSSAI = s_NSSAI;
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
}

ogs_pkbuf_t *ngap_build_ng_setup_response(void)
{
    int i;

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

    NGSetupResponse = CALLOC(1, sizeof(*NGSetupResponse));
    ogs_assert(NGSetupResponse);
    successfulOutcome->value.choice.NGSetupResponse = NGSetupResponse;

    NGSetupResponse->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_NGSetupResponse);
    ogs_assert(NGSetupResponse->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_NGSetupResponseIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(NGSetupResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMFName;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_NGSetupResponseIEs__value_PR_AMFName;

    AMFName = CALLOC(1, sizeof(*AMFName));
    ogs_assert(AMFName);
    ie->value.choice.AMFName = AMFName;

    ie = CALLOC(1, sizeof(NGAP_NGSetupResponseIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(NGSetupResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_ServedGUAMIList;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_NGSetupResponseIEs__value_PR_ServedGUAMIList;

    ServedGUAMIList = CALLOC(1, sizeof(*ServedGUAMIList));
    ogs_assert(ServedGUAMIList);
    ie->value.choice.ServedGUAMIList = ServedGUAMIList;

    ie = CALLOC(1, sizeof(NGAP_NGSetupResponseIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(NGSetupResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RelativeAMFCapacity;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_NGSetupResponseIEs__value_PR_RelativeAMFCapacity;

    RelativeAMFCapacity = CALLOC(1, sizeof(*RelativeAMFCapacity));
    ogs_assert(RelativeAMFCapacity);
    ie->value.choice.RelativeAMFCapacity = RelativeAMFCapacity;

    ie = CALLOC(1, sizeof(NGAP_NGSetupResponseIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(NGSetupResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PLMNSupportList;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_NGSetupResponseIEs__value_PR_PLMNSupportList;

    PLMNSupportList = CALLOC(1, sizeof(*PLMNSupportList));
    ogs_assert(PLMNSupportList);
    ie->value.choice.PLMNSupportList = PLMNSupportList;

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
        gUAMI = CALLOC(1, sizeof(*gUAMI));
        ogs_assert(gUAMI);
        ServedGUAMIItem->gUAMI = gUAMI;
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

    ngap_build_plmn_support_list(PLMNSupportList);

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

    NGSetupFailure = CALLOC(1, sizeof(*NGSetupFailure));
    ogs_assert(NGSetupFailure);
    unsuccessfulOutcome->value.choice.NGSetupFailure = NGSetupFailure;

    NGSetupFailure->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_NGSetupFailure);
    ogs_assert(NGSetupFailure->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_NGSetupFailureIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(NGSetupFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_NGSetupFailureIEs__value_PR_Cause;

    Cause = CALLOC(1, sizeof(*Cause));
    ogs_assert(Cause);
    ie->value.choice.Cause = Cause;
    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    if (time_to_wait > -1) {
        ie = CALLOC(1, sizeof(NGAP_NGSetupFailureIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(NGSetupFailure->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_TimeToWait;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_NGSetupFailureIEs__value_PR_TimeToWait;

        ie->value.choice.TimeToWait = CALLOC(1,
                sizeof(*ie->value.choice.TimeToWait));
        ogs_assert(ie->value.choice.TimeToWait);
        *ie->value.choice.TimeToWait = time_to_wait;
    }

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_amf_configuration_update(void)
{
    int i, num_plmn = 0;
    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_AMFConfigurationUpdate_t *AMFConfigurationUpdate = NULL;

    NGAP_AMFConfigurationUpdateIEs_t *ie = NULL;
    NGAP_PLMNSupportList_t *PLMNSupportList = NULL;

    ogs_debug("AMFConfigurationUpdate");

    /* Count valid PLMNs (those with at least one S-NSSAI) */
    for (i = 0; i < amf_self()->num_of_plmn_support; i++) {
        if (amf_self()->plmn_support[i].num_of_s_nssai > 0) {
            num_plmn++;
        }
    }

    /*
     * Build message even without PLMNs to notify gNBs
     * If no PLMNs, the PLMNSupportList IE is omitted
     */
    if (num_plmn == 0) {
        ogs_warn("Building AMF Configuration Update with no PLMNs (gNBs will be notified)");
    }

    memset(&pdu, 0, sizeof(NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = NGAP_ProcedureCode_id_AMFConfigurationUpdate;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present = NGAP_InitiatingMessage__value_PR_AMFConfigurationUpdate;

    AMFConfigurationUpdate = CALLOC(1, sizeof(*AMFConfigurationUpdate));
    ogs_assert(AMFConfigurationUpdate);
    initiatingMessage->value.choice.AMFConfigurationUpdate = AMFConfigurationUpdate;

    AMFConfigurationUpdate->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_AMFConfigurationUpdate);
    ogs_assert(AMFConfigurationUpdate->protocolIEs);

    /* Only add PLMNSupportList IE if there are PLMNs */
    if (num_plmn > 0) {
        ie = CALLOC(1, sizeof(NGAP_AMFConfigurationUpdateIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(AMFConfigurationUpdate->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_PLMNSupportList;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present =
            NGAP_AMFConfigurationUpdateIEs__value_PR_PLMNSupportList;

        PLMNSupportList = CALLOC(1, sizeof(*PLMNSupportList));
        ogs_assert(PLMNSupportList);
        ie->value.choice.PLMNSupportList = PLMNSupportList;
        ngap_build_plmn_support_list(PLMNSupportList);
    }

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_ran_configuration_update_ack(void)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_RANConfigurationUpdateAcknowledge_t
        *RANConfigurationUpdateAcknowledge = NULL;

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

    RANConfigurationUpdateAcknowledge =
        CALLOC(1, sizeof(*RANConfigurationUpdateAcknowledge));
    ogs_assert(RANConfigurationUpdateAcknowledge);
    successfulOutcome->value.choice.RANConfigurationUpdateAcknowledge =
        RANConfigurationUpdateAcknowledge;

    RANConfigurationUpdateAcknowledge->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_RANConfigurationUpdateAcknowledge);
    ogs_assert(RANConfigurationUpdateAcknowledge->protocolIEs);

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

    RANConfigurationUpdateFailure = CALLOC(1, sizeof(*RANConfigurationUpdateFailure));
    ogs_assert(RANConfigurationUpdateFailure);
    unsuccessfulOutcome->value.choice.RANConfigurationUpdateFailure = RANConfigurationUpdateFailure;

    RANConfigurationUpdateFailure->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_RANConfigurationUpdateFailure);
    ogs_assert(RANConfigurationUpdateFailure->protocolIEs);

    if (time_to_wait > -1) {
        ie = CALLOC(1, sizeof(NGAP_RANConfigurationUpdateFailureIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(RANConfigurationUpdateFailure->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_TimeToWait;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_RANConfigurationUpdateFailureIEs__value_PR_TimeToWait;

        TimeToWait = CALLOC(1, sizeof(*TimeToWait));
        ogs_assert(TimeToWait);
        ie->value.choice.TimeToWait = TimeToWait;
    }

    ie = CALLOC(1, sizeof(NGAP_RANConfigurationUpdateFailureIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(RANConfigurationUpdateFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_RANConfigurationUpdateFailureIEs__value_PR_Cause;

    Cause = CALLOC(1, sizeof(*Cause));
    ogs_assert(Cause);
    ie->value.choice.Cause = Cause;
    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    if (TimeToWait)
        *TimeToWait = time_to_wait;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_downlink_nas_transport(
    ran_ue_t *ran_ue, amf_ue_t *amf_ue,
    ogs_pkbuf_t *gmmbuf, bool ue_ambr, bool allowed_nssai)
{
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
    ogs_assert(amf_ue);

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

    DownlinkNASTransport = CALLOC(1, sizeof(*DownlinkNASTransport));
    ogs_assert(DownlinkNASTransport);
    initiatingMessage->value.choice.DownlinkNASTransport = DownlinkNASTransport;

    DownlinkNASTransport->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_DownlinkNASTransport);
    ogs_assert(DownlinkNASTransport->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_DownlinkNASTransport_IEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(DownlinkNASTransport->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_DownlinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = CALLOC(1, sizeof(*AMF_UE_NGAP_ID));
    ogs_assert(AMF_UE_NGAP_ID);
    ie->value.choice.AMF_UE_NGAP_ID = AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_DownlinkNASTransport_IEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(DownlinkNASTransport->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_DownlinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = CALLOC(1, sizeof(*RAN_UE_NGAP_ID));
    ogs_assert(RAN_UE_NGAP_ID);
    ie->value.choice.RAN_UE_NGAP_ID = RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_DownlinkNASTransport_IEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(DownlinkNASTransport->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_NAS_PDU;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_DownlinkNASTransport_IEs__value_PR_NAS_PDU;

    NAS_PDU = CALLOC(1, sizeof(*NAS_PDU));
    ogs_assert(NAS_PDU);
    ie->value.choice.NAS_PDU = NAS_PDU;

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);

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
    if (ran_ue->ue_ambr_sent == false && ue_ambr &&
        amf_ue->ue_ambr.downlink && amf_ue->ue_ambr.uplink) {
        ogs_assert(amf_ue);

        ie = CALLOC(1, sizeof(NGAP_DownlinkNASTransport_IEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(DownlinkNASTransport->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_DownlinkNASTransport_IEs__value_PR_UEAggregateMaximumBitRate;

        UEAggregateMaximumBitRate = CALLOC(1, sizeof(*UEAggregateMaximumBitRate));
        ogs_assert(UEAggregateMaximumBitRate);
        ie->value.choice.UEAggregateMaximumBitRate = UEAggregateMaximumBitRate;

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
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(DownlinkNASTransport->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_AllowedNSSAI;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present =
            NGAP_DownlinkNASTransport_IEs__value_PR_AllowedNSSAI;

        AllowedNSSAI = CALLOC(1, sizeof(*AllowedNSSAI));
        ogs_assert(AllowedNSSAI);
        ie->value.choice.AllowedNSSAI = AllowedNSSAI;

        ogs_assert(amf_ue->allowed_nssai.num_of_s_nssai);
        for (i = 0; i < amf_ue->allowed_nssai.num_of_s_nssai; i++) {
            NGAP_AllowedNSSAI_Item_t *NGAP_AllowedNSSAI_Item = NULL;
            NGAP_S_NSSAI_t *s_NSSAI = NULL;
            NGAP_SST_t *sST = NULL;

            NGAP_AllowedNSSAI_Item = (NGAP_AllowedNSSAI_Item_t *)
                    CALLOC(1, sizeof(NGAP_AllowedNSSAI_Item_t));
            s_NSSAI = CALLOC(1, sizeof(*s_NSSAI));
            ogs_assert(s_NSSAI);
            NGAP_AllowedNSSAI_Item->s_NSSAI = s_NSSAI;
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
    ran_ue = ran_ue_find_by_id(amf_ue->ran_ue_id);
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

    InitialContextSetupRequest = CALLOC(1, sizeof(*InitialContextSetupRequest));
    ogs_assert(InitialContextSetupRequest);
    initiatingMessage->value.choice.InitialContextSetupRequest = InitialContextSetupRequest;

    InitialContextSetupRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_InitialContextSetupRequest);
    ogs_assert(InitialContextSetupRequest->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = CALLOC(1, sizeof(*AMF_UE_NGAP_ID));
    ogs_assert(AMF_UE_NGAP_ID);
    ie->value.choice.AMF_UE_NGAP_ID = AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = CALLOC(1, sizeof(*RAN_UE_NGAP_ID));
    ogs_assert(RAN_UE_NGAP_ID);
    ie->value.choice.RAN_UE_NGAP_ID = RAN_UE_NGAP_ID;

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
    if (PDU_RES_SETUP_REQ_TRANSFER_NEEDED(amf_ue) == true &&
        amf_ue->ue_ambr.downlink && amf_ue->ue_ambr.uplink) {

        ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present = NGAP_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

        UEAggregateMaximumBitRate = CALLOC(1, sizeof(*UEAggregateMaximumBitRate));
        ogs_assert(UEAggregateMaximumBitRate);
        ie->value.choice.UEAggregateMaximumBitRate = UEAggregateMaximumBitRate;

        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateUL,
                amf_ue->ue_ambr.uplink);
        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateDL,
                amf_ue->ue_ambr.downlink);

        ran_ue->ue_ambr_sent = true;
    }

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_GUAMI;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_InitialContextSetupRequestIEs__value_PR_GUAMI;

    GUAMI = CALLOC(1, sizeof(*GUAMI));
    ogs_assert(GUAMI);
    ie->value.choice.GUAMI = GUAMI;

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        OCTET_STRING_t *transfer = NULL;
        NGAP_S_NSSAI_t *s_NSSAI = NULL;
        NGAP_SST_t *sST = NULL;

        if (!sess->transfer.pdu_session_resource_setup_request) continue;

        if (!PDUSessionList) {
            ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
            ogs_assert(ie);
            ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

            ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtReq;
            ie->criticality = NGAP_Criticality_reject;
            ie->value.present = NGAP_InitialContextSetupRequestIEs__value_PR_PDUSessionResourceSetupListCxtReq;

            PDUSessionList = CALLOC(1, sizeof(*PDUSessionList));
            ogs_assert(PDUSessionList);
            ie->value.choice.PDUSessionResourceSetupListCxtReq = PDUSessionList;
        }

        PDUSessionItem = CALLOC(1,
                sizeof(struct NGAP_PDUSessionResourceSetupItemCxtReq));
        ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

        PDUSessionItem->pDUSessionID = sess->psi;

        s_NSSAI = CALLOC(1, sizeof(*s_NSSAI));
        ogs_assert(s_NSSAI);
        PDUSessionItem->s_NSSAI = s_NSSAI;
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
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AllowedNSSAI;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_AllowedNSSAI;

    AllowedNSSAI = CALLOC(1, sizeof(*AllowedNSSAI));
    ogs_assert(AllowedNSSAI);
    ie->value.choice.AllowedNSSAI = AllowedNSSAI;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities;

    UESecurityCapabilities = CALLOC(1, sizeof(*UESecurityCapabilities));
    ogs_assert(UESecurityCapabilities);
    ie->value.choice.UESecurityCapabilities = UESecurityCapabilities;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SecurityKey;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_SecurityKey;

    SecurityKey = CALLOC(1, sizeof(*SecurityKey));
    ogs_assert(SecurityKey);
    ie->value.choice.SecurityKey = SecurityKey;

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);

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
        s_NSSAI = CALLOC(1, sizeof(*s_NSSAI));
        ogs_assert(s_NSSAI);
        NGAP_AllowedNSSAI_Item->s_NSSAI = s_NSSAI;
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
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UERadioCapability;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_InitialContextSetupRequestIEs__value_PR_UERadioCapability;

        UERadioCapability = CALLOC(1, sizeof(*UERadioCapability));
        ogs_assert(UERadioCapability);
        ie->value.choice.UERadioCapability = UERadioCapability;

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
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_MaskedIMEISV;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_InitialContextSetupRequestIEs__value_PR_MaskedIMEISV;

        MaskedIMEISV = CALLOC(1, sizeof(*MaskedIMEISV));
        ogs_assert(MaskedIMEISV);
        ie->value.choice.MaskedIMEISV = MaskedIMEISV;

        MaskedIMEISV->size = amf_ue->masked_imeisv_len;
        MaskedIMEISV->buf = CALLOC(MaskedIMEISV->size, sizeof(uint8_t));
        MaskedIMEISV->bits_unused = 0;
        memcpy(MaskedIMEISV->buf, amf_ue->masked_imeisv, MaskedIMEISV->size);
    }

    if (gmmbuf) {
        ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_NAS_PDU;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_InitialContextSetupRequestIEs__value_PR_NAS_PDU;

        NAS_PDU = CALLOC(1, sizeof(*NAS_PDU));
        ogs_assert(NAS_PDU);
        ie->value.choice.NAS_PDU = NAS_PDU;

        NAS_PDU->size = gmmbuf->len;
        NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
        memcpy(NAS_PDU->buf, gmmbuf->data, NAS_PDU->size);
        ogs_pkbuf_free(gmmbuf);
    }

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_ue_context_modification_request(amf_ue_t *amf_ue)
{
    ran_ue_t *ran_ue = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UEContextModificationRequest_t *UEContextModificationRequest = NULL;

    NGAP_UEContextModificationRequestIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_UEAggregateMaximumBitRate_t *UEAggregateMaximumBitRate = NULL;
    NGAP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;
    NGAP_SecurityKey_t *SecurityKey = NULL;

    ogs_assert(amf_ue);
    ran_ue = ran_ue_find_by_id(amf_ue->ran_ue_id);
    ogs_assert(ran_ue);

    ogs_debug("UEContextModificationRequest(UE)");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_UEContextModification;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_UEContextModificationRequest;

    UEContextModificationRequest = CALLOC(1, sizeof(*UEContextModificationRequest));
    ogs_assert(UEContextModificationRequest);
    initiatingMessage->value.choice.UEContextModificationRequest = UEContextModificationRequest;

    UEContextModificationRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_UEContextModificationRequest);
    ogs_assert(UEContextModificationRequest->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_UEContextModificationRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextModificationRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_UEContextModificationRequestIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = CALLOC(1, sizeof(*AMF_UE_NGAP_ID));
    ogs_assert(AMF_UE_NGAP_ID);
    ie->value.choice.AMF_UE_NGAP_ID = AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_UEContextModificationRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextModificationRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_UEContextModificationRequestIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = CALLOC(1, sizeof(*RAN_UE_NGAP_ID));
    ogs_assert(RAN_UE_NGAP_ID);
    ie->value.choice.RAN_UE_NGAP_ID = RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_UEContextModificationRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextModificationRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_UEContextModificationRequestIEs__value_PR_UEAggregateMaximumBitRate;

    UEAggregateMaximumBitRate = CALLOC(1, sizeof(*UEAggregateMaximumBitRate));
    ogs_assert(UEAggregateMaximumBitRate);
    ie->value.choice.UEAggregateMaximumBitRate = UEAggregateMaximumBitRate;

    asn_uint642INTEGER(
            &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateUL,
            (amf_ue->ue_ambr.uplink == 0 ||
             amf_ue->ue_ambr.uplink > OGS_MAX_BITRATE_NGAP) ?
                OGS_MAX_BITRATE_NGAP : amf_ue->ue_ambr.uplink);
    asn_uint642INTEGER(
            &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateDL,
            (amf_ue->ue_ambr.downlink == 0 ||
             amf_ue->ue_ambr.downlink > OGS_MAX_BITRATE_NGAP) ?
                OGS_MAX_BITRATE_NGAP : amf_ue->ue_ambr.downlink);

    ran_ue->ue_ambr_sent = true;

    ie = CALLOC(1, sizeof(NGAP_UEContextModificationRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextModificationRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_UEContextModificationRequestIEs__value_PR_UESecurityCapabilities;

    UESecurityCapabilities = CALLOC(1, sizeof(*UESecurityCapabilities));
    ogs_assert(UESecurityCapabilities);
    ie->value.choice.UESecurityCapabilities = UESecurityCapabilities;

    ie = CALLOC(1, sizeof(NGAP_UEContextModificationRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextModificationRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SecurityKey;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_UEContextModificationRequestIEs__value_PR_SecurityKey;

    SecurityKey = CALLOC(1, sizeof(*SecurityKey));
    ogs_assert(SecurityKey);
    ie->value.choice.SecurityKey = SecurityKey;

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;

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

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_sess_build_initial_context_setup_request(
        ran_ue_t *ran_ue, amf_sess_t *sess,
        ogs_pkbuf_t *gmmbuf, ogs_pkbuf_t *n2smbuf)
{
    int i;

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
    amf_ue = amf_ue_find_by_id(sess->amf_ue_id);
    ogs_assert(amf_ue);
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

    InitialContextSetupRequest = CALLOC(1, sizeof(*InitialContextSetupRequest));
    ogs_assert(InitialContextSetupRequest);
    initiatingMessage->value.choice.InitialContextSetupRequest = InitialContextSetupRequest;

    InitialContextSetupRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_InitialContextSetupRequest);
    ogs_assert(InitialContextSetupRequest->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = CALLOC(1, sizeof(*AMF_UE_NGAP_ID));
    ogs_assert(AMF_UE_NGAP_ID);
    ie->value.choice.AMF_UE_NGAP_ID = AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = CALLOC(1, sizeof(*RAN_UE_NGAP_ID));
    ogs_assert(RAN_UE_NGAP_ID);
    ie->value.choice.RAN_UE_NGAP_ID = RAN_UE_NGAP_ID;

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
    if ((gmmbuf || n2smbuf) &&
        amf_ue->ue_ambr.downlink && amf_ue->ue_ambr.uplink) {

        ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present =
            NGAP_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

        UEAggregateMaximumBitRate = CALLOC(1, sizeof(*UEAggregateMaximumBitRate));
        ogs_assert(UEAggregateMaximumBitRate);
        ie->value.choice.UEAggregateMaximumBitRate = UEAggregateMaximumBitRate;

        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateUL,
                amf_ue->ue_ambr.uplink);
        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateDL,
                amf_ue->ue_ambr.downlink);

        ran_ue->ue_ambr_sent = true;
    }

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_GUAMI;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_InitialContextSetupRequestIEs__value_PR_GUAMI;

    GUAMI = CALLOC(1, sizeof(*GUAMI));
    ogs_assert(GUAMI);
    ie->value.choice.GUAMI = GUAMI;

    if (gmmbuf || n2smbuf) {
        NGAP_NAS_PDU_t *nAS_PDU = NULL;
        OCTET_STRING_t *transfer = NULL;
        NGAP_S_NSSAI_t *s_NSSAI = NULL;
        NGAP_SST_t *sST = NULL;

        ogs_assert(n2smbuf);

        ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtReq;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present = NGAP_InitialContextSetupRequestIEs__value_PR_PDUSessionResourceSetupListCxtReq;

        PDUSessionList = CALLOC(1, sizeof(*PDUSessionList));
        ogs_assert(PDUSessionList);
        ie->value.choice.PDUSessionResourceSetupListCxtReq = PDUSessionList;

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

        s_NSSAI = CALLOC(1, sizeof(*s_NSSAI));
        ogs_assert(s_NSSAI);
        PDUSessionItem->s_NSSAI = s_NSSAI;
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
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AllowedNSSAI;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_AllowedNSSAI;

    AllowedNSSAI = CALLOC(1, sizeof(*AllowedNSSAI));
    ogs_assert(AllowedNSSAI);
    ie->value.choice.AllowedNSSAI = AllowedNSSAI;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities;

    UESecurityCapabilities = CALLOC(1, sizeof(*UESecurityCapabilities));
    ogs_assert(UESecurityCapabilities);
    ie->value.choice.UESecurityCapabilities = UESecurityCapabilities;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SecurityKey;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialContextSetupRequestIEs__value_PR_SecurityKey;

    SecurityKey = CALLOC(1, sizeof(*SecurityKey));
    ogs_assert(SecurityKey);
    ie->value.choice.SecurityKey = SecurityKey;

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);

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
        s_NSSAI = CALLOC(1, sizeof(*s_NSSAI));
        ogs_assert(s_NSSAI);
        NGAP_AllowedNSSAI_Item->s_NSSAI = s_NSSAI;
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
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UERadioCapability;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_InitialContextSetupRequestIEs__value_PR_UERadioCapability;

        UERadioCapability = CALLOC(1, sizeof(*UERadioCapability));
        ogs_assert(UERadioCapability);
        ie->value.choice.UERadioCapability = UERadioCapability;

        ogs_assert(UERadioCapability);
        ogs_asn_buffer_to_OCTET_STRING(
                amf_ue->ueRadioCapability.buf, amf_ue->ueRadioCapability.size,
                UERadioCapability);
    }

    if (amf_ue->masked_imeisv_len) {
        ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_MaskedIMEISV;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_InitialContextSetupRequestIEs__value_PR_MaskedIMEISV;

        MaskedIMEISV = CALLOC(1, sizeof(*MaskedIMEISV));
        ogs_assert(MaskedIMEISV);
        ie->value.choice.MaskedIMEISV = MaskedIMEISV;

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

    UEContextReleaseCommand = CALLOC(1, sizeof(*UEContextReleaseCommand));
    ogs_assert(UEContextReleaseCommand);
    initiatingMessage->value.choice.UEContextReleaseCommand = UEContextReleaseCommand;

    UEContextReleaseCommand->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_UEContextReleaseCommand);
    ogs_assert(UEContextReleaseCommand->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_UEContextReleaseCommand_IEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextReleaseCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UE_NGAP_IDs;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_UEContextReleaseCommand_IEs__value_PR_UE_NGAP_IDs;

    UE_NGAP_IDs = CALLOC(1, sizeof(*UE_NGAP_IDs));
    ogs_assert(UE_NGAP_IDs);
    ie->value.choice.UE_NGAP_IDs = UE_NGAP_IDs;

    ie = CALLOC(1, sizeof(NGAP_UEContextReleaseCommand_IEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextReleaseCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_UEContextReleaseCommand_IEs__value_PR_Cause;

    Cause = CALLOC(1, sizeof(*Cause));
    ogs_assert(Cause);
    ie->value.choice.Cause = Cause;

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
    ran_ue = ran_ue_find_by_id(amf_ue->ran_ue_id);
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

    PDUSessionResourceSetupRequest = CALLOC(1, sizeof(*PDUSessionResourceSetupRequest));
    ogs_assert(PDUSessionResourceSetupRequest);
    initiatingMessage->value.choice.PDUSessionResourceSetupRequest = PDUSessionResourceSetupRequest;

    PDUSessionResourceSetupRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_PDUSessionResourceSetupRequest);
    ogs_assert(PDUSessionResourceSetupRequest->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PDUSessionResourceSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PDUSessionResourceSetupRequestIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = CALLOC(1, sizeof(*AMF_UE_NGAP_ID));
    ogs_assert(AMF_UE_NGAP_ID);
    ie->value.choice.AMF_UE_NGAP_ID = AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PDUSessionResourceSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PDUSessionResourceSetupRequestIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = CALLOC(1, sizeof(*RAN_UE_NGAP_ID));
    ogs_assert(RAN_UE_NGAP_ID);
    ie->value.choice.RAN_UE_NGAP_ID = RAN_UE_NGAP_ID;

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;

    if (gmmbuf) {
        ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(PDUSessionResourceSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_NAS_PDU;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present =
            NGAP_PDUSessionResourceSetupRequestIEs__value_PR_NAS_PDU;

        NAS_PDU = CALLOC(1, sizeof(*NAS_PDU));
        ogs_assert(NAS_PDU);
        ie->value.choice.NAS_PDU = NAS_PDU;

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
            ogs_assert(ie);
            ASN_SEQUENCE_ADD(PDUSessionResourceSetupRequest->protocolIEs, ie);

            ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListSUReq;
            ie->criticality = NGAP_Criticality_reject;
            ie->value.present = NGAP_PDUSessionResourceSetupRequestIEs__value_PR_PDUSessionResourceSetupListSUReq;

            PDUSessionList = CALLOC(1, sizeof(*PDUSessionList));
            ogs_assert(PDUSessionList);
            ie->value.choice.PDUSessionResourceSetupListSUReq = PDUSessionList;
        }

        PDUSessionItem =
            CALLOC(1, sizeof(struct NGAP_PDUSessionResourceSetupItemSUReq));
        ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

        PDUSessionItem->pDUSessionID = sess->psi;

        s_NSSAI = CALLOC(1, sizeof(*s_NSSAI));
        ogs_assert(s_NSSAI);
        PDUSessionItem->s_NSSAI = s_NSSAI;
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
    ogs_assert(OGS_ASN_LIST_COUNT(PDUSessionList));

    /*
     * TS 38.413
     * 8.2.1. PDU Session Resource Setup
     * 8.2.1.2. Successful Operation
     *
     * The UE Aggregate Maximum Bit Rate IE should be sent to the NG-RAN node
     * if the AMF has not sent it previously.
     */
    if (ran_ue->ue_ambr_sent == false &&
        PDU_RES_SETUP_REQ_TRANSFER_NEEDED(amf_ue) == true &&
        amf_ue->ue_ambr.downlink && amf_ue->ue_ambr.uplink) {

        ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(PDUSessionResourceSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_PDUSessionResourceSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

        UEAggregateMaximumBitRate = CALLOC(1, sizeof(*UEAggregateMaximumBitRate));
        ogs_assert(UEAggregateMaximumBitRate);
        ie->value.choice.UEAggregateMaximumBitRate = UEAggregateMaximumBitRate;

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
        ran_ue_t *ran_ue, amf_sess_t *sess,
        ogs_pkbuf_t *gmmbuf, ogs_pkbuf_t *n2smbuf)
{
    amf_ue_t *amf_ue = NULL;

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

    amf_ue = amf_ue_find_by_id(sess->amf_ue_id);
    ogs_assert(amf_ue);
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

    PDUSessionResourceSetupRequest = CALLOC(1, sizeof(*PDUSessionResourceSetupRequest));
    ogs_assert(PDUSessionResourceSetupRequest);
    initiatingMessage->value.choice.PDUSessionResourceSetupRequest = PDUSessionResourceSetupRequest;

    PDUSessionResourceSetupRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_PDUSessionResourceSetupRequest);
    ogs_assert(PDUSessionResourceSetupRequest->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PDUSessionResourceSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PDUSessionResourceSetupRequestIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = CALLOC(1, sizeof(*AMF_UE_NGAP_ID));
    ogs_assert(AMF_UE_NGAP_ID);
    ie->value.choice.AMF_UE_NGAP_ID = AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PDUSessionResourceSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PDUSessionResourceSetupRequestIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = CALLOC(1, sizeof(*RAN_UE_NGAP_ID));
    ogs_assert(RAN_UE_NGAP_ID);
    ie->value.choice.RAN_UE_NGAP_ID = RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PDUSessionResourceSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListSUReq;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceSetupRequestIEs__value_PR_PDUSessionResourceSetupListSUReq;

    PDUSessionList = CALLOC(1, sizeof(*PDUSessionList));
    ogs_assert(PDUSessionList);
    ie->value.choice.PDUSessionResourceSetupListSUReq = PDUSessionList;

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);

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

    s_NSSAI = CALLOC(1, sizeof(*s_NSSAI));
    ogs_assert(s_NSSAI);
    PDUSessionItem->s_NSSAI = s_NSSAI;
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
    if (ran_ue->ue_ambr_sent == false &&
        amf_ue->ue_ambr.downlink && amf_ue->ue_ambr.uplink) {

        ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupRequestIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(PDUSessionResourceSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_PDUSessionResourceSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

        UEAggregateMaximumBitRate = CALLOC(1, sizeof(*UEAggregateMaximumBitRate));
        ogs_assert(UEAggregateMaximumBitRate);
        ie->value.choice.UEAggregateMaximumBitRate = UEAggregateMaximumBitRate;

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

    amf_ue = amf_ue_find_by_id(sess->amf_ue_id);
    ogs_assert(amf_ue);
    ran_ue = ran_ue_find_by_id(amf_ue->ran_ue_id);
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

    PDUSessionResourceModifyRequest = CALLOC(1, sizeof(*PDUSessionResourceModifyRequest));
    ogs_assert(PDUSessionResourceModifyRequest);
    initiatingMessage->value.choice.PDUSessionResourceModifyRequest = PDUSessionResourceModifyRequest;

    PDUSessionResourceModifyRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_PDUSessionResourceModifyRequest);
    ogs_assert(PDUSessionResourceModifyRequest->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceModifyRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PDUSessionResourceModifyRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PDUSessionResourceModifyRequestIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = CALLOC(1, sizeof(*AMF_UE_NGAP_ID));
    ogs_assert(AMF_UE_NGAP_ID);
    ie->value.choice.AMF_UE_NGAP_ID = AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceModifyRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PDUSessionResourceModifyRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PDUSessionResourceModifyRequestIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = CALLOC(1, sizeof(*RAN_UE_NGAP_ID));
    ogs_assert(RAN_UE_NGAP_ID);
    ie->value.choice.RAN_UE_NGAP_ID = RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceModifyRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PDUSessionResourceModifyRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceModifyListModReq;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceModifyRequestIEs__value_PR_PDUSessionResourceModifyListModReq;

    PDUSessionList = CALLOC(1, sizeof(*PDUSessionList));
    ogs_assert(PDUSessionList);
    ie->value.choice.PDUSessionResourceModifyListModReq = PDUSessionList;

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);

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

    amf_ue = amf_ue_find_by_id(sess->amf_ue_id);
    ogs_assert(amf_ue);
    ran_ue = ran_ue_find_by_id(amf_ue->ran_ue_id);
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

    PDUSessionResourceReleaseCommand = CALLOC(1, sizeof(*PDUSessionResourceReleaseCommand));
    ogs_assert(PDUSessionResourceReleaseCommand);
    initiatingMessage->value.choice.PDUSessionResourceReleaseCommand = PDUSessionResourceReleaseCommand;

    PDUSessionResourceReleaseCommand->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_PDUSessionResourceReleaseCommand);
    ogs_assert(PDUSessionResourceReleaseCommand->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceReleaseCommandIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PDUSessionResourceReleaseCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PDUSessionResourceReleaseCommandIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = CALLOC(1, sizeof(*AMF_UE_NGAP_ID));
    ogs_assert(AMF_UE_NGAP_ID);
    ie->value.choice.AMF_UE_NGAP_ID = AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceReleaseCommandIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PDUSessionResourceReleaseCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PDUSessionResourceReleaseCommandIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = CALLOC(1, sizeof(*RAN_UE_NGAP_ID));
    ogs_assert(RAN_UE_NGAP_ID);
    ie->value.choice.RAN_UE_NGAP_ID = RAN_UE_NGAP_ID;

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;

    if (gmmbuf) {
        ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceReleaseCommandIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(PDUSessionResourceReleaseCommand->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_NAS_PDU;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_PDUSessionResourceReleaseCommandIEs__value_PR_NAS_PDU;

        NAS_PDU = CALLOC(1, sizeof(*NAS_PDU));
        ogs_assert(NAS_PDU);
        ie->value.choice.NAS_PDU = NAS_PDU;

        NAS_PDU->size = gmmbuf->len;
        NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
        memcpy(NAS_PDU->buf, gmmbuf->data, NAS_PDU->size);
        ogs_pkbuf_free(gmmbuf);
    }

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceReleaseCommandIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PDUSessionResourceReleaseCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceToReleaseListRelCmd;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceReleaseCommandIEs__value_PR_PDUSessionResourceToReleaseListRelCmd;

    PDUSessionList = CALLOC(1, sizeof(*PDUSessionList));
    ogs_assert(PDUSessionList);
    ie->value.choice.PDUSessionResourceToReleaseListRelCmd = PDUSessionList;

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

    Paging = CALLOC(1, sizeof(*Paging));
    ogs_assert(Paging);
    initiatingMessage->value.choice.Paging = Paging;

    Paging->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_Paging);
    ogs_assert(Paging->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_PagingIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(Paging->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UEPagingIdentity;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_PagingIEs__value_PR_UEPagingIdentity;

    UEPagingIdentity = CALLOC(1, sizeof(*UEPagingIdentity));
    ogs_assert(UEPagingIdentity);
    ie->value.choice.UEPagingIdentity = UEPagingIdentity;

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
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(Paging->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_TAIListForPaging;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_PagingIEs__value_PR_TAIListForPaging;

    TAIList = CALLOC(1, sizeof(*TAIList));
    ogs_assert(TAIList);
    ie->value.choice.TAIListForPaging = TAIList;

    TAIItem = CALLOC(1, sizeof(NGAP_TAIListForPagingItem_t));
    ASN_SEQUENCE_ADD(&TAIList->list, TAIItem);

    tAI = CALLOC(1, sizeof(*tAI));
    ogs_assert(tAI);
    TAIItem->tAI = tAI;
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

    DownlinkRANConfigurationTransfer = CALLOC(1, sizeof(*DownlinkRANConfigurationTransfer));
    ogs_assert(DownlinkRANConfigurationTransfer);
    initiatingMessage->value.choice.DownlinkRANConfigurationTransfer = DownlinkRANConfigurationTransfer;

    DownlinkRANConfigurationTransfer->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_DownlinkRANConfigurationTransfer);
    ogs_assert(DownlinkRANConfigurationTransfer->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_DownlinkRANConfigurationTransferIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(DownlinkRANConfigurationTransfer->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SONConfigurationTransferDL;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_DownlinkRANConfigurationTransferIEs__value_PR_SONConfigurationTransfer;

    SONConfigurationTransfer = CALLOC(1, sizeof(*SONConfigurationTransfer));
    ogs_assert(SONConfigurationTransfer);
    ie->value.choice.SONConfigurationTransfer = SONConfigurationTransfer;

    rv = ogs_asn_copy_ie(&asn_DEF_NGAP_SONConfigurationTransfer,
            transfer, SONConfigurationTransfer);
    if (rv != OGS_OK) {
        ogs_error("ogs_asn_copy_ie() failed");
        ogs_asn_free(&asn_DEF_NGAP_NGAP_PDU, &pdu);
        return NULL;
    }

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
    ran_ue = ran_ue_find_by_id(amf_ue->ran_ue_id);
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

    PathSwitchRequestAcknowledge = CALLOC(1, sizeof(*PathSwitchRequestAcknowledge));
    ogs_assert(PathSwitchRequestAcknowledge);
    successfulOutcome->value.choice.PathSwitchRequestAcknowledge = PathSwitchRequestAcknowledge;

    PathSwitchRequestAcknowledge->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_PathSwitchRequestAcknowledge);
    ogs_assert(PathSwitchRequestAcknowledge->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestAcknowledgeIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequestAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PathSwitchRequestAcknowledgeIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = CALLOC(1, sizeof(*AMF_UE_NGAP_ID));
    ogs_assert(AMF_UE_NGAP_ID);
    ie->value.choice.AMF_UE_NGAP_ID = AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestAcknowledgeIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequestAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PathSwitchRequestAcknowledgeIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = CALLOC(1, sizeof(*RAN_UE_NGAP_ID));
    ogs_assert(RAN_UE_NGAP_ID);
    ie->value.choice.RAN_UE_NGAP_ID = RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestAcknowledgeIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequestAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SecurityContext;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PathSwitchRequestAcknowledgeIEs__value_PR_SecurityContext;

    SecurityContext = CALLOC(1, sizeof(*SecurityContext));
    ogs_assert(SecurityContext);
    ie->value.choice.SecurityContext = SecurityContext;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestAcknowledgeIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequestAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSwitchedList;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_PathSwitchRequestAcknowledgeIEs__value_PR_PDUSessionResourceSwitchedList;

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)ran_ue->ran_ue_ngap_id,
            (long long)ran_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;

    SecurityContext->nextHopChainingCount = amf_ue->nhcc;
    SecurityContext->nextHopNH.size = OGS_SHA256_DIGEST_SIZE;
    SecurityContext->nextHopNH.buf =
        CALLOC(SecurityContext->nextHopNH.size, sizeof(uint8_t));
    SecurityContext->nextHopNH.bits_unused = 0;
    memcpy(SecurityContext->nextHopNH.buf,
            amf_ue->nh, SecurityContext->nextHopNH.size);

    PDUSessionResourceSwitchedList = CALLOC(1, sizeof(*PDUSessionResourceSwitchedList));
    ogs_assert(PDUSessionResourceSwitchedList);
    ie->value.choice.PDUSessionResourceSwitchedList = PDUSessionResourceSwitchedList;

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        OCTET_STRING_t *transfer = NULL;
        NGAP_PDUSessionResourceSwitchedItem_t *PDUSessionItem = NULL;

        if (!sess->transfer.path_switch_request_ack) continue;

        if (!PDUSessionResourceSwitchedList) {
            ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestAcknowledgeIEs_t));
            ogs_assert(ie);
            ASN_SEQUENCE_ADD(PathSwitchRequestAcknowledge->protocolIEs, ie);

            ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSwitchedList;
            ie->criticality = NGAP_Criticality_ignore;
            ie->value.present = NGAP_PathSwitchRequestAcknowledgeIEs__value_PR_PDUSessionResourceSwitchedList;

            PDUSessionResourceSwitchedList = CALLOC(1, sizeof(*PDUSessionResourceSwitchedList));
            ogs_assert(PDUSessionResourceSwitchedList);
            ie->value.choice.PDUSessionResourceSwitchedList = PDUSessionResourceSwitchedList;
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
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequestAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AllowedNSSAI;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PathSwitchRequestAcknowledgeIEs__value_PR_AllowedNSSAI;

    AllowedNSSAI = CALLOC(1, sizeof(*AllowedNSSAI));
    ogs_assert(AllowedNSSAI);
    ie->value.choice.AllowedNSSAI = AllowedNSSAI;

    ogs_assert(amf_ue->allowed_nssai.num_of_s_nssai);
    for (i = 0; i < amf_ue->allowed_nssai.num_of_s_nssai; i++) {
        NGAP_AllowedNSSAI_Item_t *NGAP_AllowedNSSAI_Item = NULL;
        NGAP_S_NSSAI_t *s_NSSAI = NULL;
        NGAP_SST_t *sST = NULL;

        NGAP_AllowedNSSAI_Item = (NGAP_AllowedNSSAI_Item_t *)
                CALLOC(1, sizeof(NGAP_AllowedNSSAI_Item_t));
        s_NSSAI = CALLOC(1, sizeof(*s_NSSAI));
        ogs_assert(s_NSSAI);
        NGAP_AllowedNSSAI_Item->s_NSSAI = s_NSSAI;
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
    amf_ue = amf_ue_find_by_id(target_ue->amf_ue_id);
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

    HandoverRequest = CALLOC(1, sizeof(*HandoverRequest));
    ogs_assert(HandoverRequest);
    initiatingMessage->value.choice.HandoverRequest = HandoverRequest;

    HandoverRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_HandoverRequest);
    ogs_assert(HandoverRequest->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = CALLOC(1, sizeof(*AMF_UE_NGAP_ID));
    ogs_assert(AMF_UE_NGAP_ID);
    ie->value.choice.AMF_UE_NGAP_ID = AMF_UE_NGAP_ID;

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)target_ue->ran_ue_ngap_id,
            (long long)target_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, target_ue->amf_ue_ngap_id);

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_HandoverType;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_HandoverType;

    HandoverType = CALLOC(1, sizeof(*HandoverType));
    ogs_assert(HandoverType);
    ie->value.choice.HandoverType = HandoverType;

    ogs_debug("    HandoverType[%d]", (int)amf_ue->handover.type);

    *HandoverType = amf_ue->handover.type;;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_Cause;

    Cause = CALLOC(1, sizeof(*Cause));
    ogs_assert(Cause);
    ie->value.choice.Cause = Cause;

    Cause->present = amf_ue->handover.group;
    Cause->choice.radioNetwork = amf_ue->handover.cause;
    ogs_debug("    Group[%d] Cause[%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

    if (HANDOVER_REQUEST_TRANSFER_NEEDED(amf_ue) == true &&
        amf_ue->ue_ambr.downlink && amf_ue->ue_ambr.uplink) {

        ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present =
            NGAP_HandoverRequestIEs__value_PR_UEAggregateMaximumBitRate;

        UEAggregateMaximumBitRate = CALLOC(1, sizeof(*UEAggregateMaximumBitRate));
        ogs_assert(UEAggregateMaximumBitRate);
        ie->value.choice.UEAggregateMaximumBitRate = UEAggregateMaximumBitRate;

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
    ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_HandoverRequestIEs__value_PR_UESecurityCapabilities;

    UESecurityCapabilities = CALLOC(1, sizeof(*UESecurityCapabilities));
    ogs_assert(UESecurityCapabilities);
    ie->value.choice.UESecurityCapabilities = UESecurityCapabilities;

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
    ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SecurityContext;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_SecurityContext;

    SecurityContext = CALLOC(1, sizeof(*SecurityContext));
    ogs_assert(SecurityContext);
    ie->value.choice.SecurityContext = SecurityContext;

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
            ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

            ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListHOReq;
            ie->criticality = NGAP_Criticality_reject;
            ie->value.present = NGAP_HandoverRequestIEs__value_PR_PDUSessionResourceSetupListHOReq;

            PDUSessionList = CALLOC(1, sizeof(*PDUSessionList));
            ogs_assert(PDUSessionList);
            ie->value.choice.PDUSessionResourceSetupListHOReq = PDUSessionList;
        }

        PDUSessionItem = CALLOC(1,
                sizeof(struct NGAP_PDUSessionResourceSetupItemHOReq));
        ogs_assert(PDUSessionItem);
        ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

        PDUSessionItem->pDUSessionID = sess->psi;

        s_NSSAI = CALLOC(1, sizeof(*s_NSSAI));
        ogs_assert(s_NSSAI);
        PDUSessionItem->s_NSSAI = s_NSSAI;
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
    ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AllowedNSSAI;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_AllowedNSSAI;

    AllowedNSSAI = CALLOC(1, sizeof(*AllowedNSSAI));
    ogs_assert(AllowedNSSAI);
    ie->value.choice.AllowedNSSAI = AllowedNSSAI;

    ogs_assert(amf_ue->allowed_nssai.num_of_s_nssai);
    for (i = 0; i < amf_ue->allowed_nssai.num_of_s_nssai; i++) {
        NGAP_AllowedNSSAI_Item_t *NGAP_AllowedNSSAI_Item = NULL;
        NGAP_S_NSSAI_t *s_NSSAI = NULL;
        NGAP_SST_t *sST = NULL;

        NGAP_AllowedNSSAI_Item = (NGAP_AllowedNSSAI_Item_t *)
                CALLOC(1, sizeof(NGAP_AllowedNSSAI_Item_t));
        s_NSSAI = CALLOC(1, sizeof(*s_NSSAI));
        ogs_assert(s_NSSAI);
        NGAP_AllowedNSSAI_Item->s_NSSAI = s_NSSAI;
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
        ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_MaskedIMEISV;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_HandoverRequestIEs__value_PR_MaskedIMEISV;

        MaskedIMEISV = CALLOC(1, sizeof(*MaskedIMEISV));
        ogs_assert(MaskedIMEISV);
        ie->value.choice.MaskedIMEISV = MaskedIMEISV;

        MaskedIMEISV->size = amf_ue->masked_imeisv_len;
        MaskedIMEISV->buf = CALLOC(MaskedIMEISV->size, sizeof(uint8_t));
        MaskedIMEISV->bits_unused = 0;
        memcpy(MaskedIMEISV->buf, amf_ue->masked_imeisv, MaskedIMEISV->size);
    }

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SourceToTarget_TransparentContainer;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_SourceToTarget_TransparentContainer;

    SourceToTarget_TransparentContainer = CALLOC(1, sizeof(*SourceToTarget_TransparentContainer));
    ogs_assert(SourceToTarget_TransparentContainer);
    ie->value.choice.SourceToTarget_TransparentContainer = SourceToTarget_TransparentContainer;

    ogs_asn_buffer_to_OCTET_STRING(
            amf_ue->handover.container.buf, amf_ue->handover.container.size,
            SourceToTarget_TransparentContainer);

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_GUAMI;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_GUAMI;

    GUAMI = CALLOC(1, sizeof(*GUAMI));
    ogs_assert(GUAMI);
    ie->value.choice.GUAMI = GUAMI;

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

    HandoverPreparationFailure = CALLOC(1, sizeof(*HandoverPreparationFailure));
    ogs_assert(HandoverPreparationFailure);
    unsuccessfulOutcome->value.choice.HandoverPreparationFailure = HandoverPreparationFailure;

    HandoverPreparationFailure->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_HandoverPreparationFailure);
    ogs_assert(HandoverPreparationFailure->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_HandoverPreparationFailureIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverPreparationFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_HandoverPreparationFailureIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = CALLOC(1, sizeof(*AMF_UE_NGAP_ID));
    ogs_assert(AMF_UE_NGAP_ID);
    ie->value.choice.AMF_UE_NGAP_ID = AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverPreparationFailureIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverPreparationFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_HandoverPreparationFailureIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = CALLOC(1, sizeof(*RAN_UE_NGAP_ID));
    ogs_assert(RAN_UE_NGAP_ID);
    ie->value.choice.RAN_UE_NGAP_ID = RAN_UE_NGAP_ID;

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)source_ue->ran_ue_ngap_id,
            (long long)source_ue->amf_ue_ngap_id);
    asn_uint642INTEGER(AMF_UE_NGAP_ID, source_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = source_ue->ran_ue_ngap_id;

    ie = CALLOC(1, sizeof(NGAP_HandoverPreparationFailureIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverPreparationFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_HandoverPreparationFailureIEs__value_PR_Cause;

    Cause = CALLOC(1, sizeof(*Cause));
    ogs_assert(Cause);
    ie->value.choice.Cause = Cause;

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
    amf_ue = amf_ue_find_by_id(source_ue->amf_ue_id);
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

    HandoverCommand = CALLOC(1, sizeof(*HandoverCommand));
    ogs_assert(HandoverCommand);
    successfulOutcome->value.choice.HandoverCommand = HandoverCommand;

    HandoverCommand->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_HandoverCommand);
    ogs_assert(HandoverCommand->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_HandoverCommandIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverCommandIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = CALLOC(1, sizeof(*AMF_UE_NGAP_ID));
    ogs_assert(AMF_UE_NGAP_ID);
    ie->value.choice.AMF_UE_NGAP_ID = AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverCommandIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverCommandIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = CALLOC(1, sizeof(*RAN_UE_NGAP_ID));
    ogs_assert(RAN_UE_NGAP_ID);
    ie->value.choice.RAN_UE_NGAP_ID = RAN_UE_NGAP_ID;

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)source_ue->ran_ue_ngap_id,
            (long long)source_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, source_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = source_ue->ran_ue_ngap_id;

    ie = CALLOC(1, sizeof(NGAP_HandoverCommandIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_HandoverType;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverCommandIEs__value_PR_HandoverType;

    HandoverType = CALLOC(1, sizeof(*HandoverType));
    ogs_assert(HandoverType);
    ie->value.choice.HandoverType = HandoverType;

    ogs_debug("    HandoverType[%d]", (int)amf_ue->handover.type);

    *HandoverType = amf_ue->handover.type;;

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        OCTET_STRING_t *transfer = NULL;

        if (!sess->transfer.handover_command) continue;

        if (!PDUSessionList) {
            ie = CALLOC(1, sizeof(NGAP_HandoverCommandIEs_t));
            ogs_assert(ie);
            ASN_SEQUENCE_ADD(HandoverCommand->protocolIEs, ie);

            ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceHandoverList;
            ie->criticality = NGAP_Criticality_ignore;
            ie->value.present = NGAP_HandoverCommandIEs__value_PR_PDUSessionResourceHandoverList;

            PDUSessionList = CALLOC(1, sizeof(*PDUSessionList));
            ogs_assert(PDUSessionList);
            ie->value.choice.PDUSessionResourceHandoverList = PDUSessionList;
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
    ASN_SEQUENCE_ADD(HandoverCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_TargetToSource_TransparentContainer;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverCommandIEs__value_PR_TargetToSource_TransparentContainer;

    TargetToSource_TransparentContainer = CALLOC(1, sizeof(*TargetToSource_TransparentContainer));
    ogs_assert(TargetToSource_TransparentContainer);
    ie->value.choice.TargetToSource_TransparentContainer = TargetToSource_TransparentContainer;

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

    HandoverCancelAcknowledge = CALLOC(1, sizeof(*HandoverCancelAcknowledge));
    ogs_assert(HandoverCancelAcknowledge);
    successfulOutcome->value.choice.HandoverCancelAcknowledge = HandoverCancelAcknowledge;

    HandoverCancelAcknowledge->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_HandoverCancelAcknowledge);
    ogs_assert(HandoverCancelAcknowledge->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_HandoverCancelAcknowledgeIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverCancelAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_HandoverCancelAcknowledgeIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = CALLOC(1, sizeof(*AMF_UE_NGAP_ID));
    ogs_assert(AMF_UE_NGAP_ID);
    ie->value.choice.AMF_UE_NGAP_ID = AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverCancelAcknowledgeIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverCancelAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_HandoverCancelAcknowledgeIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = CALLOC(1, sizeof(*RAN_UE_NGAP_ID));
    ogs_assert(RAN_UE_NGAP_ID);
    ie->value.choice.RAN_UE_NGAP_ID = RAN_UE_NGAP_ID;

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)source_ue->ran_ue_ngap_id,
            (long long)source_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, source_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = source_ue->ran_ue_ngap_id;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_downlink_ran_status_transfer(
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

    DownlinkRANStatusTransfer = CALLOC(1, sizeof(*DownlinkRANStatusTransfer));
    ogs_assert(DownlinkRANStatusTransfer);
    initiatingMessage->value.choice.DownlinkRANStatusTransfer = DownlinkRANStatusTransfer;

    DownlinkRANStatusTransfer->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_DownlinkRANStatusTransfer);
    ogs_assert(DownlinkRANStatusTransfer->protocolIEs);

    ie = CALLOC(1, sizeof(NGAP_DownlinkRANStatusTransferIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(DownlinkRANStatusTransfer->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_DownlinkRANStatusTransferIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = CALLOC(1, sizeof(*AMF_UE_NGAP_ID));
    ogs_assert(AMF_UE_NGAP_ID);
    ie->value.choice.AMF_UE_NGAP_ID = AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_DownlinkRANStatusTransferIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(DownlinkRANStatusTransfer->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_DownlinkRANStatusTransferIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = CALLOC(1, sizeof(*RAN_UE_NGAP_ID));
    ogs_assert(RAN_UE_NGAP_ID);
    ie->value.choice.RAN_UE_NGAP_ID = RAN_UE_NGAP_ID;

    ogs_debug("    RAN_UE_NGAP_ID[%lld] AMF_UE_NGAP_ID[%lld]",
            (long long)target_ue->ran_ue_ngap_id,
            (long long)target_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, target_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = target_ue->ran_ue_ngap_id;

    ie = CALLOC(1, sizeof(NGAP_DownlinkRANStatusTransferIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(DownlinkRANStatusTransfer->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RANStatusTransfer_TransparentContainer;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_DownlinkRANStatusTransferIEs__value_PR_RANStatusTransfer_TransparentContainer;

    RANStatusTransfer_TransparentContainer = CALLOC(1, sizeof(*RANStatusTransfer_TransparentContainer));
    ogs_assert(RANStatusTransfer_TransparentContainer);
    ie->value.choice.RANStatusTransfer_TransparentContainer = RANStatusTransfer_TransparentContainer;

    rv = ogs_asn_copy_ie(
            &asn_DEF_NGAP_RANStatusTransfer_TransparentContainer,
            transfer, RANStatusTransfer_TransparentContainer);
    if (rv != OGS_OK) {
        ogs_error("ogs_asn_copy_ie() failed");
        ogs_asn_free(&asn_DEF_NGAP_NGAP_PDU, &pdu);
        return NULL;
    }

    return ogs_ngap_encode(&pdu);
}
