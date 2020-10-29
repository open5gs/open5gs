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

ogs_pkbuf_t *ngap_build_downlink_nas_transport(
    ran_ue_t *ran_ue, ogs_pkbuf_t *gmmbuf)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_DownlinkNASTransport_t *DownlinkNASTransport = NULL;

    NGAP_DownlinkNASTransport_IEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;

    ogs_assert(gmmbuf);
    ogs_assert(ran_ue);

    ogs_debug("DownlinkNASTransport");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

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

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_initial_context_setup_request(
            amf_ue_t *amf_ue, ogs_pkbuf_t *gmmbuf)
{
    int i, j;

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

    ogs_debug("Initial context setup request");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

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

    if (amf_ue->subscribed_ue_ambr.downlink ||
            amf_ue->subscribed_ue_ambr.uplink) {
        ie = CALLOC(1, sizeof(NGAP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present =
            NGAP_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitRate;

        UEAggregateMaximumBitRate = &ie->value.choice.UEAggregateMaximumBitRate;

        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateUL,
                amf_ue->subscribed_ue_ambr.uplink);
        asn_uint642INTEGER(
                &UEAggregateMaximumBitRate->uEAggregateMaximumBitRateDL,
                amf_ue->subscribed_ue_ambr.downlink);
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

        if (!sess->pdu_session_resource_setup_request_transfer) continue;

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
        transfer->size = sess->pdu_session_resource_setup_request_transfer->len;
        transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
        memcpy(transfer->buf,
                sess->pdu_session_resource_setup_request_transfer->data,
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

    for (i = 0; i < amf_self()->num_of_plmn_support; i++) {
        if (memcmp(&amf_ue->tai.plmn_id,
                &amf_self()->plmn_support[i].plmn_id, OGS_PLMN_ID_LEN) != 0)
            continue;
        for (j = 0; j < amf_self()->plmn_support[i].num_of_s_nssai; j++) {
            NGAP_AllowedNSSAI_Item_t *NGAP_AllowedNSSAI_Item = NULL;
            NGAP_S_NSSAI_t *s_NSSAI = NULL;
            NGAP_SST_t *sST = NULL;

            NGAP_AllowedNSSAI_Item = (NGAP_AllowedNSSAI_Item_t *)
                    CALLOC(1, sizeof(NGAP_AllowedNSSAI_Item_t));
            s_NSSAI = &NGAP_AllowedNSSAI_Item->s_NSSAI;
            sST = &s_NSSAI->sST;

            ogs_asn_uint8_to_OCTET_STRING(
                amf_self()->plmn_support[i].s_nssai[j].sst, sST);
            if (amf_self()->plmn_support[i].s_nssai[j].sd.v !=
                    OGS_S_NSSAI_NO_SD_VALUE) {
                s_NSSAI->sD = CALLOC(1, sizeof(NGAP_SD_t));
                ogs_asn_uint24_to_OCTET_STRING(
                    amf_self()->plmn_support[i].s_nssai[j].sd, s_NSSAI->sD);
            }

            ASN_SEQUENCE_ADD(&AllowedNSSAI->list, NGAP_AllowedNSSAI_Item);
        }
    }

    UESecurityCapabilities->nRencryptionAlgorithms.size = 2;
    UESecurityCapabilities->nRencryptionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    nRencryptionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->nRencryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->nRencryptionAlgorithms.buf[0] =
        (amf_ue->ue_security_capability.nea << 1);

    UESecurityCapabilities->nRintegrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->nRintegrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    nRintegrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->nRintegrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->nRintegrityProtectionAlgorithms.buf[0] =
        (amf_ue->ue_security_capability.nia << 1);

    UESecurityCapabilities->eUTRAencryptionAlgorithms.size = 2;
    UESecurityCapabilities->eUTRAencryptionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    eUTRAencryptionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->eUTRAencryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->eUTRAencryptionAlgorithms.buf[0] =
        (amf_ue->ue_security_capability.eps_ea << 1);

    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    eUTRAintegrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.buf[0] =
        (amf_ue->ue_security_capability.eps_ia << 1);

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

#if 0
ogs_pkbuf_t *ngap_build_ue_context_modification_request(amf_ue_t *amf_ue)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UEContextModificationRequest_t *UEContextModificationRequest = NULL;

    NGAP_UEContextModificationRequestIEs_t *ie = NULL;

    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;
    NGAP_SecurityKey_t *SecurityKey = NULL;
    NGAP_CSFallbackIndicator_t *CSFallbackIndicator = NULL;
    NGAP_LAI_t *LAI = NULL;

    ran_ue_t *ran_ue = NULL;

    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

    ogs_debug("UE context modification request");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_UEContextModification;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_UEContextModificationRequest;

    UEContextModificationRequest =
        &initiatingMessage->value.choice.UEContextModificationRequest;

    ie = CALLOC(1, sizeof(NGAP_UEContextModificationRequestIEs_t));
    ASN_SEQUENCE_ADD(&UEContextModificationRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_UEContextModificationRequestIEs__value_PR_AMF_UE_NGAP_ID,

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_UEContextModificationRequestIEs_t));
    ASN_SEQUENCE_ADD(&UEContextModificationRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_UEContextModificationRequestIEs__value_PR_RAN_UE_NGAP_ID,

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    *AMF_UE_NGAP_ID = ran_ue->amf_ue_ngap_id;
    *RAN_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            ran_ue->ran_ue_ngap_id, ran_ue->amf_ue_ngap_id);

    if (amf_ue->nas_eps.type == AMF_EPS_TYPE_EXTENDED_SERVICE_REQUEST &&
        AMF_P_TMSI_IS_AVAILABLE(amf_ue)) {
        ie = CALLOC(1, sizeof(NGAP_UEContextModificationRequestIEs_t));
        ASN_SEQUENCE_ADD(&UEContextModificationRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_CSFallbackIndicator;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present =
            NGAP_UEContextModificationRequestIEs__value_PR_CSFallbackIndicator;

        CSFallbackIndicator = &ie->value.choice.CSFallbackIndicator;
        ogs_assert(CSFallbackIndicator);

        *CSFallbackIndicator = NGAP_CSFallbackIndicator_cs_fallback_required;

        ie = CALLOC(1, sizeof(NGAP_UEContextModificationRequestIEs_t));
        ASN_SEQUENCE_ADD(&UEContextModificationRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_RegisteredLAI;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_UEContextModificationRequestIEs__value_PR_LAI;

        LAI = &ie->value.choice.LAI;
        ogs_assert(LAI);

        ogs_asn_buffer_to_OCTET_STRING(
            &amf_ue->tai.plmn_id, sizeof(ogs_plmn_id_t), &LAI->pLMNidentity);
        ogs_assert(amf_ue->csmap);
        ogs_assert(amf_ue->p_tmsi);
        ogs_asn_uint16_to_OCTET_STRING(amf_ue->csmap->lai.lac, &LAI->lAC);

    } else {
        ie = CALLOC(1, sizeof(NGAP_UEContextModificationRequestIEs_t));
        ASN_SEQUENCE_ADD(&UEContextModificationRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UESecurityCapabilities;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present = NGAP_UEContextModificationRequestIEs__value_PR_UESecurityCapabilities;

        UESecurityCapabilities = &ie->value.choice.UESecurityCapabilities;

        UESecurityCapabilities->encryptionAlgorithms.size = 2;
        UESecurityCapabilities->encryptionAlgorithms.buf = 
            CALLOC(UESecurityCapabilities->encryptionAlgorithms.size,
                        sizeof(uint8_t));
        UESecurityCapabilities->encryptionAlgorithms.bits_unused = 0;
        UESecurityCapabilities->encryptionAlgorithms.buf[0] = 
            (amf_ue->ue_network_capability.eea << 1);

        UESecurityCapabilities->integrityProtectionAlgorithms.size = 2;
        UESecurityCapabilities->integrityProtectionAlgorithms.buf =
            CALLOC(UESecurityCapabilities->
                        integrityProtectionAlgorithms.size, sizeof(uint8_t));
        UESecurityCapabilities->integrityProtectionAlgorithms.bits_unused = 0;
        UESecurityCapabilities->integrityProtectionAlgorithms.buf[0] =
            (amf_ue->ue_network_capability.eia << 1);

        ie = CALLOC(1, sizeof(NGAP_UEContextModificationRequestIEs_t));
        ASN_SEQUENCE_ADD(&UEContextModificationRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_SecurityKey;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present =
            NGAP_UEContextModificationRequestIEs__value_PR_SecurityKey;

        SecurityKey = &ie->value.choice.SecurityKey;

        SecurityKey->size = OGS_SHA256_DIGEST_SIZE;
        SecurityKey->buf = 
            CALLOC(SecurityKey->size, sizeof(uint8_t));
        SecurityKey->bits_unused = 0;
        memcpy(SecurityKey->buf, amf_ue->kgnb, SecurityKey->size);
    }

    return ogs_ngap_encode(&pdu);
}
#endif

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

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

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

ogs_pkbuf_t *ngap_build_pdu_session_resource_setup_request(
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

    ogs_assert(gmmbuf);
    ogs_assert(n2smbuf);
    ogs_assert(sess);

    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

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

    pDUSessionNAS_PDU = CALLOC(1, sizeof(NGAP_NAS_PDU_t));
    PDUSessionItem->pDUSessionNAS_PDU = pDUSessionNAS_PDU;
    pDUSessionNAS_PDU->size = gmmbuf->len;
    pDUSessionNAS_PDU->buf = CALLOC(pDUSessionNAS_PDU->size, sizeof(uint8_t));
    memcpy(pDUSessionNAS_PDU->buf, gmmbuf->data, pDUSessionNAS_PDU->size);
    ogs_pkbuf_free(gmmbuf);

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

    ogs_assert(gmmbuf);
    ogs_assert(n2smbuf);
    ogs_assert(sess);

    amf_ue = sess->amf_ue;
    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

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

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceReleaseCommandIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceReleaseCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_NAS_PDU;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PDUSessionResourceReleaseCommandIEs__value_PR_NAS_PDU;

    NAS_PDU = &ie->value.choice.NAS_PDU;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceReleaseCommandIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceReleaseCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceToReleaseListRelCmd;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceReleaseCommandIEs__value_PR_PDUSessionResourceToReleaseListRelCmd;

    PDUSessionList = &ie->value.choice.PDUSessionResourceToReleaseListRelCmd;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;

    NAS_PDU->size = gmmbuf->len;
    NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
    memcpy(NAS_PDU->buf, gmmbuf->data, NAS_PDU->size);
    ogs_pkbuf_free(gmmbuf);

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

#if 0
ogs_pkbuf_t *ngap_build_paging(
        amf_ue_t *amf_ue, NGAP_CNDomain_t cn_domain)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_Paging_t *Paging = NULL;

    NGAP_PagingIEs_t *ie = NULL;

    NGAP_UEIdentityIndexValue_t *UEIdentityIndexValue = NULL;
    NGAP_UEPagingID_t *UEPagingID = NULL;
    NGAP_CNDomain_t *CNDomain = NULL;
    NGAP_TAIList_t *TAIList = NULL;

    NGAP_TAIItemIEs_t *item = NULL;
    NGAP_TAIItem_t *tai_item = NULL;

    uint16_t index_value;
    uint64_t ue_imsi_value = 0;
    int i = 0;

    ogs_assert(amf_ue);

    ogs_debug("Paging");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = NGAP_ProcedureCode_id_Paging;
    initiatingMessage->criticality = NGAP_Criticality_ignore;
    initiatingMessage->value.present = NGAP_InitiatingMessage__value_PR_Paging;

    Paging = &initiatingMessage->value.choice.Paging;

    ie = CALLOC(1, sizeof(NGAP_PagingIEs_t));
    ASN_SEQUENCE_ADD(&Paging->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UEIdentityIndexValue;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_PagingIEs__value_PR_UEIdentityIndexValue;

    UEIdentityIndexValue = &ie->value.choice.UEIdentityIndexValue;

    ie = CALLOC(1, sizeof(NGAP_PagingIEs_t));
    ASN_SEQUENCE_ADD(&Paging->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UEPagingID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_PagingIEs__value_PR_UEPagingID;

    UEPagingID = &ie->value.choice.UEPagingID;

    ie = CALLOC(1, sizeof(NGAP_PagingIEs_t));
    ASN_SEQUENCE_ADD(&Paging->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_CNDomain;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_PagingIEs__value_PR_CNDomain;

    CNDomain = &ie->value.choice.CNDomain;

    ie = CALLOC(1, sizeof(NGAP_PagingIEs_t));
    ASN_SEQUENCE_ADD(&Paging->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_TAIList;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_PagingIEs__value_PR_TAIList;

    TAIList = &ie->value.choice.TAIList;

    /* Set UE Identity Index value : IMSI mod 4096 */
    UEIdentityIndexValue->size = 2;
    UEIdentityIndexValue->buf = 
        CALLOC(UEIdentityIndexValue->size, sizeof(uint8_t));

    /* Conver string to value */
    for (i = 0; i < strlen(amf_ue->imsi_bcd); i++) {
        ue_imsi_value = ue_imsi_value*10 + (amf_ue->imsi_bcd[i] - '0');
    }

    /* index(10bit) = ue_imsi_value mod 1024 */
    index_value = ue_imsi_value % 1024;
    UEIdentityIndexValue->buf[0] = index_value >> 2;
    UEIdentityIndexValue->buf[1] = (index_value & 0x3f) << 6;
    UEIdentityIndexValue->bits_unused = 6;

    /* Set Paging Identity */
    UEPagingID->present = NGAP_UEPagingID_PR_s_TMSI;
    UEPagingID->choice.s_TMSI = 
        CALLOC(1, sizeof(NGAP_S_TMSI_t));
    ogs_asn_uint8_to_OCTET_STRING(amf_ue->guti.amf_code, 
            &UEPagingID->choice.s_TMSI->mMEC);

    ogs_asn_uint32_to_OCTET_STRING(amf_ue->guti.m_tmsi, 
            &UEPagingID->choice.s_TMSI->m_TMSI);

    ogs_debug("    AMF_CODE[%d] M_TMSI[0x%x]",
            amf_ue->guti.amf_code, amf_ue->guti.m_tmsi);
    ogs_debug("    CN_DOMAIN[%s]",
            cn_domain == NGAP_CNDomain_cs ? "CS" :
                cn_domain == NGAP_CNDomain_ps ? "PS" : "Unknown");

    *CNDomain = cn_domain;

    item = CALLOC(1, sizeof(NGAP_TAIItemIEs_t));
    ASN_SEQUENCE_ADD(&TAIList->list, item);

    item->id = NGAP_ProtocolIE_ID_id_TAIItem;
    item->criticality = NGAP_Criticality_ignore;
    item->value.present = NGAP_TAIItemIEs__value_PR_TAIItem;

    tai_item = &item->value.choice.TAIItem;

    ogs_asn_buffer_to_OCTET_STRING(&amf_ue->tai.plmn_id, sizeof(ogs_plmn_id_t),
            &tai_item->tAI.pLMNidentity);
    ogs_asn_uint16_to_OCTET_STRING(amf_ue->tai.tac, &tai_item->tAI.tAC);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_amf_configuration_transfer(
        NGAP_SONConfigurationTransfer_t *son_configuration_transfer)
{
    int rv;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_AMFConfigurationTransfer_t *AMFConfigurationTransfer = NULL;

    NGAP_AMFConfigurationTransferIEs_t *ie = NULL;
    NGAP_SONConfigurationTransfer_t *SONConfigurationTransfer = NULL;

    ogs_assert(son_configuration_transfer);

    ogs_debug("AMF Configuration Transfer");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_AMFConfigurationTransfer;
    initiatingMessage->criticality = NGAP_Criticality_ignore;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_AMFConfigurationTransfer;

    AMFConfigurationTransfer =
        &initiatingMessage->value.choice.AMFConfigurationTransfer;

    ie = CALLOC(1, sizeof(NGAP_AMFConfigurationTransferIEs_t));
    ASN_SEQUENCE_ADD(&AMFConfigurationTransfer->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SONConfigurationTransferMCT;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_AMFConfigurationTransferIEs__value_PR_SONConfigurationTransfer;

    SONConfigurationTransfer = &ie->value.choice.SONConfigurationTransfer;

    rv = ogs_asn_copy_ie(&asn_DEF_NGAP_SONConfigurationTransfer,
            son_configuration_transfer, SONConfigurationTransfer);
    ogs_assert(rv == OGS_OK);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_path_switch_ack(amf_ue_t *amf_ue)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_PathSwitchRequestAcknowledge_t *PathSwitchRequestAcknowledge = NULL;

    NGAP_PathSwitchRequestAcknowledgeIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_SecurityContext_t *SecurityContext = NULL;

    ran_ue_t *ran_ue = NULL;

    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);

    ogs_debug("Path switch acknowledge");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

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

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;

    SecurityContext->nextHopChainingCount = amf_ue->nhcc;
    SecurityContext->nextHopParameter.size = OGS_SHA256_DIGEST_SIZE;
    SecurityContext->nextHopParameter.buf = 
        CALLOC(SecurityContext->nextHopParameter.size,
        sizeof(uint8_t));
    SecurityContext->nextHopParameter.bits_unused = 0;
    memcpy(SecurityContext->nextHopParameter.buf,
            amf_ue->nh, SecurityContext->nextHopParameter.size);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_path_switch_failure(
    uint32_t ran_ue_ngap_id, uint64_t amf_ue_ngap_id,
    NGAP_Cause_PR group, long cause)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    NGAP_PathSwitchRequestFailure_t *PathSwitchRequestFailure = NULL;

    NGAP_PathSwitchRequestFailureIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    ogs_debug("Path switch failure");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_unsuccessfulOutcome;
    pdu.choice.unsuccessfulOutcome =
        CALLOC(1, sizeof(NGAP_UnsuccessfulOutcome_t));

    unsuccessfulOutcome = pdu.choice.unsuccessfulOutcome;
    unsuccessfulOutcome->procedureCode =
        NGAP_ProcedureCode_id_PathSwitchRequest;
    unsuccessfulOutcome->criticality = NGAP_Criticality_reject;
    unsuccessfulOutcome->value.present =
        NGAP_UnsuccessfulOutcome__value_PR_PathSwitchRequestFailure;

    PathSwitchRequestFailure =
        &unsuccessfulOutcome->value.choice.PathSwitchRequestFailure;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestFailureIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequestFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PathSwitchRequestFailureIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestFailureIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequestFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PathSwitchRequestFailureIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestFailureIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequestFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_PathSwitchRequestFailureIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue_ngap_id, (long long)amf_ue_ngap_id);
    ogs_debug("    Group[%d] Cause[%d]", group, (int)cause);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue_ngap_id;
    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_handover_command(ran_ue_t *source_ue)
{
    int rv;

    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_HandoverCommand_t *HandoverCommand = NULL;

    NGAP_HandoverCommandIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_HandoverType_t *HandoverType = NULL;
    NGAP_E_RABSubjecttoDataForwardingList_t
        *E_RABSubjecttoDataForwardingList = NULL;
    NGAP_Target_ToSource_TransparentContainer_t
        *Target_ToSource_TransparentContainer = NULL;

    amf_ue_t *amf_ue = NULL;
    amf_sess_t *sess = NULL;
    amf_bearer_t *bearer = NULL;

    ogs_assert(source_ue);
    amf_ue = source_ue->amf_ue;

    ogs_debug("Handover command");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode =
        NGAP_ProcedureCode_id_HandoverPreparation;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_HandoverCommand;

    HandoverCommand = &successfulOutcome->value.choice.HandoverCommand;
    ogs_assert(HandoverCommand);

    ie = CALLOC(1, sizeof(NGAP_HandoverCommandIEs_t));
    ASN_SEQUENCE_ADD(&HandoverCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverCommandIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverCommandIEs_t));
    ASN_SEQUENCE_ADD(&HandoverCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverCommandIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverCommandIEs_t));
    ASN_SEQUENCE_ADD(&HandoverCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_HandoverType;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverCommandIEs__value_PR_HandoverType;

    HandoverType = &ie->value.choice.HandoverType;

    *AMF_UE_NGAP_ID = source_ue->amf_ue_ngap_id;
    *RAN_UE_NGAP_ID = source_ue->ran_ue_ngap_id;
    *HandoverType = source_ue->handover_type;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            source_ue->ran_ue_ngap_id, source_ue->amf_ue_ngap_id);

    sess = amf_sess_first(amf_ue);
    while (sess) {
        bearer = amf_bearer_first(sess);
        while (bearer) {
            NGAP_E_RABDataForwardingItem_t *e_rab = NULL;

            if (AMF_HAVE_SGW_DL_INDIRECT_TUNNEL(bearer) ||
                AMF_HAVE_SGW_UL_INDIRECT_TUNNEL(bearer)) {
                NGAP_E_RABDataForwardingItemIEs_t *item = NULL;

                if (E_RABSubjecttoDataForwardingList == NULL) {
                    ie = CALLOC(1, sizeof(NGAP_HandoverCommandIEs_t));
                    ogs_assert(ie);
                    ASN_SEQUENCE_ADD(&HandoverCommand->protocolIEs, ie);

                    ie->id =
                        NGAP_ProtocolIE_ID_id_E_RABSubjecttoDataForwardingList;
                    ie->criticality = NGAP_Criticality_ignore;
                    ie->value.present =
                        NGAP_HandoverCommandIEs__value_PR_E_RABSubjecttoDataForwardingList;

                    E_RABSubjecttoDataForwardingList =
                        &ie->value.choice.E_RABSubjecttoDataForwardingList;
                }
                ogs_assert(E_RABSubjecttoDataForwardingList);

                item = CALLOC(
                        1, sizeof(NGAP_E_RABDataForwardingItemIEs_t));
                ogs_assert(item);
                ASN_SEQUENCE_ADD(&E_RABSubjecttoDataForwardingList->list, item);

                item->id = NGAP_ProtocolIE_ID_id_E_RABDataForwardingItem;
                item->criticality = NGAP_Criticality_ignore;
                item->value.present =
                    NGAP_E_RABDataForwardingItemIEs__value_PR_E_RABDataForwardingItem;

                e_rab = &item->value.choice.E_RABDataForwardingItem;
                ogs_assert(e_rab);

                e_rab->e_RAB_ID = bearer->ebi;
            }

            if (AMF_HAVE_SGW_DL_INDIRECT_TUNNEL(bearer)) {
                ogs_assert(e_rab);
                e_rab->dL_transportLayerAddress =
                    (NGAP_TransportLayerAddress_t *)
                    CALLOC(1, sizeof(NGAP_TransportLayerAddress_t));
                rv = ogs_asn_ip_to_BIT_STRING(
                        &bearer->sgw_dl_ip, e_rab->dL_transportLayerAddress);
                ogs_assert(rv == OGS_OK);

                e_rab->dL_gTP_TEID = (NGAP_GTP_TEID_t *)
                    CALLOC(1, sizeof(NGAP_GTP_TEID_t));
                ogs_asn_uint32_to_OCTET_STRING(
                        bearer->sgw_dl_teid, e_rab->dL_gTP_TEID);
                ogs_debug("    SGW-DL-TEID[%d]", bearer->sgw_dl_teid);
            }

            if (AMF_HAVE_SGW_UL_INDIRECT_TUNNEL(bearer)) {
                ogs_assert(e_rab);
                e_rab->uL_TransportLayerAddress =
                    (NGAP_TransportLayerAddress_t *)
                    CALLOC(1, sizeof(NGAP_TransportLayerAddress_t));
                rv = ogs_asn_ip_to_BIT_STRING(
                    &bearer->sgw_ul_ip, e_rab->uL_TransportLayerAddress);
                ogs_assert(rv == OGS_OK);

                e_rab->uL_GTP_TEID = (NGAP_GTP_TEID_t *)
                    CALLOC(1, sizeof(NGAP_GTP_TEID_t));
                ogs_asn_uint32_to_OCTET_STRING(
                        bearer->sgw_ul_teid, e_rab->uL_GTP_TEID);
                ogs_debug("    SGW-UL-TEID[%d]", bearer->sgw_dl_teid);
            }

            bearer = amf_bearer_next(bearer);
        }
        sess = amf_sess_next(sess);
    }

    ie = CALLOC(1, sizeof(NGAP_HandoverCommandIEs_t));
    ASN_SEQUENCE_ADD(&HandoverCommand->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Target_ToSource_TransparentContainer;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_HandoverCommandIEs__value_PR_Target_ToSource_TransparentContainer;

    Target_ToSource_TransparentContainer =
        &ie->value.choice.Target_ToSource_TransparentContainer;

    ogs_asn_buffer_to_OCTET_STRING(amf_ue->container.buf,
            amf_ue->container.size, Target_ToSource_TransparentContainer);

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

    ogs_debug("Handover preparation failure");

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
    ASN_SEQUENCE_ADD(&HandoverPreparationFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_HandoverPreparationFailureIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverPreparationFailureIEs_t));
    ASN_SEQUENCE_ADD(&HandoverPreparationFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_HandoverPreparationFailureIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverPreparationFailureIEs_t));
    ASN_SEQUENCE_ADD(&HandoverPreparationFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_HandoverPreparationFailureIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            source_ue->ran_ue_ngap_id, source_ue->amf_ue_ngap_id);
    ogs_debug("    Group[%d] Cause[%d]",
            cause->present, (int)cause->choice.radioNetwork);

    *AMF_UE_NGAP_ID = source_ue->amf_ue_ngap_id;
    *RAN_UE_NGAP_ID = source_ue->ran_ue_ngap_id;
    Cause->present = cause->present;
    Cause->choice.radioNetwork = cause->choice.radioNetwork;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_handover_request(
        amf_ue_t *amf_ue, ran_ue_t *target_ue,
        NGAP_RAN_UE_NGAP_ID_t *ran_ue_ngap_id,
        NGAP_AMF_UE_NGAP_ID_t *amf_ue_ngap_id,
        NGAP_HandoverType_t *handovertype,
        NGAP_Cause_t *cause,
        NGAP_SourceToTarget_TransparentContainer_t
            *source_totarget_transparentContainer)
{
    int rv;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_HandoverRequest_t *HandoverRequest = NULL;

    NGAP_HandoverRequestIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_HandoverType_t *HandoverType = NULL;
    NGAP_Cause_t *Cause = NULL;
    NGAP_UEAggregateMaximumBitrate_t *UEAggregateMaximumBitrate = NULL;
    NGAP_E_RABToBeSetupListHOReq_t *E_RABToBeSetupListHOReq = NULL;
    NGAP_Source_ToTarget_TransparentContainer_t
        *Source_ToTarget_TransparentContainer = NULL;
    NGAP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;
    NGAP_SecurityContext_t *SecurityContext = NULL;

    amf_sess_t *sess = NULL;
    amf_bearer_t *bearer = NULL;
    ogs_diam_s6a_subscription_data_t *subscription_data = NULL;

    ogs_assert(handovertype);
    ogs_assert(cause);
    ogs_assert(source_totarget_transparentContainer);

    ogs_assert(target_ue);
    ogs_assert(amf_ue);
    subscription_data = &amf_ue->subscription_data;
    ogs_assert(subscription_data);

    ogs_debug("Handover request");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_HandoverResourceAllocation;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_HandoverRequest;

    HandoverRequest = &initiatingMessage->value.choice.HandoverRequest;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_HandoverType;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_HandoverType;

    HandoverType = &ie->value.choice.HandoverType;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_HandoverRequestIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_uEaggregateMaximumBitrate;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_HandoverRequestIEs__value_PR_UEAggregateMaximumBitrate;

    UEAggregateMaximumBitrate = &ie->value.choice.UEAggregateMaximumBitrate;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_E_RABToBeSetupListHOReq;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_HandoverRequestIEs__value_PR_E_RABToBeSetupListHOReq;

    E_RABToBeSetupListHOReq = &ie->value.choice.E_RABToBeSetupListHOReq;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Source_ToTarget_TransparentContainer;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_HandoverRequestIEs__value_PR_Source_ToTarget_TransparentContainer;

    Source_ToTarget_TransparentContainer =
        &ie->value.choice.Source_ToTarget_TransparentContainer;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_HandoverRequestIEs__value_PR_UESecurityCapabilities;

    UESecurityCapabilities = &ie->value.choice.UESecurityCapabilities;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SecurityContext;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_HandoverRequestIEs__value_PR_SecurityContext;

    SecurityContext = &ie->value.choice.SecurityContext;

    *AMF_UE_NGAP_ID = target_ue->amf_ue_ngap_id;
    *HandoverType = *handovertype;
    Cause->present = cause->present;
    Cause->choice.radioNetwork = cause->choice.radioNetwork;

    asn_uint642INTEGER(
            &UEAggregateMaximumBitrate->uEaggregateMaximumBitRateUL,
            subscription_data->ambr.uplink);
    asn_uint642INTEGER(
            &UEAggregateMaximumBitrate->uEaggregateMaximumBitRateDL,
            subscription_data->ambr.downlink);

    sess = amf_sess_first(amf_ue);
    while (sess) {
        bearer = amf_bearer_first(sess);
        while (bearer) {
            NGAP_E_RABToBeSetupItemHOReqIEs_t *item = NULL;
            NGAP_E_RABToBeSetupItemHOReq_t *e_rab = NULL;
            NGAP_GBR_QosInformation_t *gbrQosInformation = NULL;

            item = CALLOC(1, sizeof(NGAP_E_RABToBeSetupItemHOReqIEs_t));
            ASN_SEQUENCE_ADD(&E_RABToBeSetupListHOReq->list, item);

            item->id = NGAP_ProtocolIE_ID_id_E_RABToBeSetupItemHOReq;
            item->criticality = NGAP_Criticality_reject;
            item->value.present =
            NGAP_E_RABToBeSetupItemHOReqIEs__value_PR_E_RABToBeSetupItemHOReq;

            e_rab = &item->value.choice.E_RABToBeSetupItemHOReq;

            e_rab->e_RAB_ID = bearer->ebi;
            e_rab->e_RABlevelQosParameters.qCI = bearer->qos.qci;

            e_rab->e_RABlevelQosParameters.allocationRetentionPriority.
                priorityLevel = bearer->qos.arp.priority_level;
            e_rab->e_RABlevelQosParameters.allocationRetentionPriority.
                pre_emptionCapability =
                    !(bearer->qos.arp.pre_emption_capability);
            e_rab->e_RABlevelQosParameters.allocationRetentionPriority.
                pre_emptionVulnerability =
                    !(bearer->qos.arp.pre_emption_vulnerability);

            if (bearer->qos.mbr.downlink || bearer->qos.mbr.uplink ||
                bearer->qos.gbr.downlink || bearer->qos.gbr.uplink) {
                if (bearer->qos.mbr.downlink == 0)
                    bearer->qos.mbr.downlink = MAX_BIT_RATE;
                if (bearer->qos.mbr.uplink == 0)
                    bearer->qos.mbr.uplink = MAX_BIT_RATE;
                if (bearer->qos.gbr.downlink == 0)
                    bearer->qos.gbr.downlink = MAX_BIT_RATE;
                if (bearer->qos.gbr.uplink == 0)
                    bearer->qos.gbr.uplink = MAX_BIT_RATE;

                gbrQosInformation = 
                        CALLOC(1, sizeof(struct NGAP_GBR_QosInformation));
                asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateDL,
                        bearer->qos.mbr.downlink);
                asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateUL,
                        bearer->qos.mbr.uplink);
                asn_uint642INTEGER(&gbrQosInformation->
                        e_RAB_GuaranteedBitrateDL, bearer->qos.gbr.downlink);
                asn_uint642INTEGER(&gbrQosInformation->
                        e_RAB_GuaranteedBitrateUL, bearer->qos.gbr.uplink);
                e_rab->e_RABlevelQosParameters.gbrQosInformation =
                        gbrQosInformation;
            }

            rv = ogs_asn_ip_to_BIT_STRING(
                    &bearer->sgw_s1u_ip, &e_rab->transportLayerAddress);
            ogs_assert(rv == OGS_OK);
            ogs_asn_uint32_to_OCTET_STRING(
                    bearer->sgw_s1u_teid, &e_rab->gTP_TEID);
            ogs_debug("    SGW-NGU-TEID[%d]", bearer->sgw_s1u_teid);

            bearer = amf_bearer_next(bearer);
        }
        sess = amf_sess_next(sess);
    }

    ogs_asn_buffer_to_OCTET_STRING(
            source_totarget_transparentContainer->buf, 
            source_totarget_transparentContainer->size, 
            Source_ToTarget_TransparentContainer);

    UESecurityCapabilities->encryptionAlgorithms.size = 2;
    UESecurityCapabilities->encryptionAlgorithms.buf = 
        CALLOC(UESecurityCapabilities->encryptionAlgorithms.size, 
                    sizeof(uint8_t));
    UESecurityCapabilities->encryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->encryptionAlgorithms.buf[0] = 
        (amf_ue->ue_network_capability.eea << 1);

    UESecurityCapabilities->integrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                        integrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->integrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf[0] =
        (amf_ue->ue_network_capability.eia << 1);

    SecurityContext->nextHopChainingCount = amf_ue->nhcc;
    SecurityContext->nextHopParameter.size = OGS_SHA256_DIGEST_SIZE;
    SecurityContext->nextHopParameter.buf = 
        CALLOC(SecurityContext->nextHopParameter.size,
        sizeof(uint8_t));
    SecurityContext->nextHopParameter.bits_unused = 0;
    memcpy(SecurityContext->nextHopParameter.buf,
            amf_ue->nh, SecurityContext->nextHopParameter.size);

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

    ogs_debug("Handover cancel acknowledge");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = NGAP_ProcedureCode_id_HandoverCancel;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_HandoverCancelAcknowledge;

    HandoverCancelAcknowledge =
        &successfulOutcome->value.choice.HandoverCancelAcknowledge;

    ie = CALLOC(1, sizeof(NGAP_HandoverCancelAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&HandoverCancelAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_HandoverCancelAcknowledgeIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverCancelAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&HandoverCancelAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_HandoverCancelAcknowledgeIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    *AMF_UE_NGAP_ID = source_ue->amf_ue_ngap_id;
    *RAN_UE_NGAP_ID = source_ue->ran_ue_ngap_id;

    ogs_debug("    Source : RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            source_ue->ran_ue_ngap_id, source_ue->amf_ue_ngap_id);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_amf_status_transfer(
        ran_ue_t *target_ue,
        NGAP_RANStatusTransfer_TransparentContainer_t
            *gnb_statustransfer_transparentContainer)
{
    int rv;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_AMFStatusTransfer_t *AMFStatusTransfer = NULL;

    NGAP_AMFStatusTransferIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_RANStatusTransfer_TransparentContainer_t
        *RAN_StatusTransfer_TransparentContainer = NULL;

    ogs_assert(target_ue);
    ogs_assert(gnb_statustransfer_transparentContainer);
    
    ogs_debug("AMF status transfer");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = NGAP_ProcedureCode_id_AMFStatusTransfer;
    initiatingMessage->criticality = NGAP_Criticality_ignore;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_AMFStatusTransfer;

    AMFStatusTransfer = &initiatingMessage->value.choice.AMFStatusTransfer;

    ie = CALLOC(1, sizeof(NGAP_AMFStatusTransferIEs_t));
    ASN_SEQUENCE_ADD(&AMFStatusTransfer->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_AMFStatusTransferIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_AMFStatusTransferIEs_t));
    ASN_SEQUENCE_ADD(&AMFStatusTransfer->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_AMFStatusTransferIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_AMFStatusTransferIEs_t));
    ASN_SEQUENCE_ADD(&AMFStatusTransfer->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_StatusTransfer_TransparentContainer;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
    NGAP_AMFStatusTransferIEs__value_PR_RAN_StatusTransfer_TransparentContainer;

    RAN_StatusTransfer_TransparentContainer =
        &ie->value.choice.RAN_StatusTransfer_TransparentContainer;

    *AMF_UE_NGAP_ID = target_ue->amf_ue_ngap_id;
    *RAN_UE_NGAP_ID = target_ue->ran_ue_ngap_id;

    ogs_debug("    Target : RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%d]",
            target_ue->ran_ue_ngap_id, target_ue->amf_ue_ngap_id);

    rv = ogs_asn_copy_ie(
            &asn_DEF_NGAP_RAN_StatusTransfer_TransparentContainer,
            gnb_statustransfer_transparentContainer,
            RAN_StatusTransfer_TransparentContainer);
    ogs_assert(rv == OGS_OK);

    return ogs_ngap_encode(&pdu);
}
#endif

ogs_pkbuf_t *ngap_build_error_indication(
        uint32_t *ran_ue_ngap_id,
        uint64_t *amf_ue_ngap_id,
        NGAP_Cause_PR group, long cause)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_ErrorIndication_t *ErrorIndication = NULL;

    NGAP_ErrorIndicationIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    ogs_debug("Error Indication");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = NGAP_ProcedureCode_id_ErrorIndication;
    initiatingMessage->criticality = NGAP_Criticality_ignore;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_ErrorIndication;

    ErrorIndication = &initiatingMessage->value.choice.ErrorIndication;

    if (amf_ue_ngap_id) {
        ie = CALLOC(1, sizeof(NGAP_ErrorIndicationIEs_t));
        ASN_SEQUENCE_ADD(&ErrorIndication->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_ErrorIndicationIEs__value_PR_AMF_UE_NGAP_ID;

        AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

        asn_uint642INTEGER(AMF_UE_NGAP_ID, *amf_ue_ngap_id);
        ogs_debug("    AMF_UE_NGAP_ID[%lld]", (long long)*amf_ue_ngap_id);
    }

    if (ran_ue_ngap_id) {
        ie = CALLOC(1, sizeof(NGAP_ErrorIndicationIEs_t));
        ASN_SEQUENCE_ADD(&ErrorIndication->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_ErrorIndicationIEs__value_PR_RAN_UE_NGAP_ID;

        RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

        *RAN_UE_NGAP_ID = *ran_ue_ngap_id;
        ogs_debug("    RAN_UE_NGAP_ID[%d]", (int)*ran_ue_ngap_id);
    }

    ie = CALLOC(1, sizeof(NGAP_ErrorIndicationIEs_t));
    ASN_SEQUENCE_ADD(&ErrorIndication->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_ErrorIndicationIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    ogs_debug("    Group[%d] Cause[%d]",
            Cause->present, (int)Cause->choice.radioNetwork);

    return ogs_ngap_encode(&pdu);
}



ogs_pkbuf_t *ngap_build_path_switch_ack(amf_ue_t *amf_ue)
{
    ran_ue_t *ran_ue = NULL;
    amf_sess_t *sess = NULL;
	
    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;	
	NGAP_PathSwitchRequestAcknowledge_t	 *PathSwitchRequestAcknowledge = NULL;
	
    NGAP_PathSwitchRequestAcknowledgeIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
	NGAP_SecurityContext_t	*SecurityContext = NULL;
	NGAP_PDUSessionResourceSwitchedList_t *PDUSessionResourceSwitchedList;
	
    ogs_assert(amf_ue);
    ran_ue = ran_ue_cycle(amf_ue->ran_ue);
    ogs_assert(ran_ue);
	
    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = NGAP_ProcedureCode_id_PathSwitchRequest;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_PathSwitchRequestAcknowledge;

    PathSwitchRequestAcknowledge = &successfulOutcome->value.choice.PathSwitchRequestAcknowledge;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequestAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PathSwitchRequestAcknowledgeIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequestAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
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
    ie->value.present =
        NGAP_PathSwitchRequestAcknowledgeIEs__value_PR_PDUSessionResourceSwitchedList;

    PDUSessionResourceSwitchedList = &ie->value.choice.PDUSessionResourceSwitchedList;
	
    ogs_list_for_each(&amf_ue->sess_list, sess) {
        OCTET_STRING_t *transfer = NULL;
		NGAP_PDUSessionResourceSwitchedItem_t *PDUSessionItem = NULL;

        if (!sess->pdu_session_resource_setup_request_transfer) continue;

        if (!PDUSessionResourceSwitchedList) {
		    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestAcknowledgeIEs_t));
		    ASN_SEQUENCE_ADD(&PathSwitchRequestAcknowledge->protocolIEs, ie);

		    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSwitchedList;
		    ie->criticality = NGAP_Criticality_ignore;
		    ie->value.present =
		        NGAP_PathSwitchRequestAcknowledgeIEs__value_PR_PDUSessionResourceSwitchedList;

		    PDUSessionResourceSwitchedList = &ie->value.choice.PDUSessionResourceSwitchedList;
        }

        PDUSessionItem = CALLOC(1,
                sizeof(NGAP_PDUSessionResourceSwitchedItem_t));
        ASN_SEQUENCE_ADD(&PDUSessionResourceSwitchedList->list, PDUSessionItem);

        PDUSessionItem->pDUSessionID = sess->psi;

        transfer = &PDUSessionItem->pathSwitchRequestAcknowledgeTransfer;
        transfer->size = sess->pdu_session_resource_setup_request_transfer->len;
        transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
        memcpy(transfer->buf,
                sess->pdu_session_resource_setup_request_transfer->data,
                transfer->size);
    }
	
    ogs_debug("    RAN_UE_NGAP_ID[%d] AMF_UE_NGAP_ID[%lld]",
            ran_ue->ran_ue_ngap_id, (long long)ran_ue->amf_ue_ngap_id);

    asn_uint642INTEGER(AMF_UE_NGAP_ID, ran_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue->ran_ue_ngap_id;
	
    SecurityContext->nextHopChainingCount = amf_ue->nhcc;
    SecurityContext->nextHopNH.size = OGS_SHA256_DIGEST_SIZE;
    SecurityContext->nextHopNH.buf = 
        CALLOC(SecurityContext->nextHopNH.size,
        sizeof(uint8_t));
    SecurityContext->nextHopNH.bits_unused = 0;
    memcpy(SecurityContext->nextHopNH.buf,
            amf_ue->nh, SecurityContext->nextHopNH.size);
	
    return ogs_ngap_encode(&pdu);
}


#if 0
ogs_pkbuf_t *ngap_build_s1_reset(
        NGAP_Cause_PR group, long cause,
        NGAP_UE_associatedLogicalNG_connectionList_t *partOfNG_Interface)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_Reset_t *Reset = NULL;

    NGAP_ResetIEs_t *ie = NULL;
    NGAP_Cause_t *Cause = NULL;
    NGAP_ResetType_t *ResetType = NULL;

    ogs_debug("Reset");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = NGAP_ProcedureCode_id_Reset;
    initiatingMessage->criticality = NGAP_Criticality_ignore;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_Reset;

    Reset = &initiatingMessage->value.choice.Reset;

    ie = CALLOC(1, sizeof(NGAP_ResetIEs_t));
    ASN_SEQUENCE_ADD(&Reset->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_ResetIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    ie = CALLOC(1, sizeof(NGAP_ResetIEs_t));
    ASN_SEQUENCE_ADD(&Reset->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_ResetType;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_ResetIEs__value_PR_ResetType;

    ResetType = &ie->value.choice.ResetType;

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    ogs_debug("    Group[%d] Cause[%d] partOfNG_Interface[%p]",
        Cause->present, (int)Cause->choice.radioNetwork, partOfNG_Interface);

    if (partOfNG_Interface) {
        ResetType->present = NGAP_ResetType_PR_partOfNG_Interface;
        ResetType->choice.partOfNG_Interface = partOfNG_Interface;
    } else {
        ResetType->present = NGAP_ResetType_PR_s1_Interface;
        ResetType->choice.s1_Interface = NGAP_ResetAll_reset_all;
    }

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *ngap_build_s1_reset_partial(
        NGAP_Cause_PR group, long cause,
        NGAP_AMF_UE_NGAP_ID_t *amf_ue_ngap_id,
        NGAP_RAN_UE_NGAP_ID_t *ran_ue_ngap_id)
{
    NGAP_UE_associatedLogicalNG_ConnectionListRes_t *partOfNG_Interface = NULL;
    NGAP_UE_associatedLogicalNG_ConnectionItemRes_t *ie2 = NULL;
    NGAP_UE_associatedLogicalNG_ConnectionItem_t *item = NULL;

    partOfNG_Interface = CALLOC(1,
            sizeof(NGAP_UE_associatedLogicalNG_ConnectionListRes_t));
    ogs_assert(partOfNG_Interface);

    ie2 = CALLOC(1,
            sizeof(NGAP_UE_associatedLogicalNG_ConnectionItemRes_t));
    ASN_SEQUENCE_ADD(&partOfNG_Interface->list, ie2);

    ie2->id = NGAP_ProtocolIE_ID_id_UE_associatedLogicalNG_ConnectionItem;
    ie2->criticality = NGAP_Criticality_reject;
    ie2->value.present = NGAP_UE_associatedLogicalNG_ConnectionItemRes__value_PR_UE_associatedLogicalNG_ConnectionItem;

    item = &ie2->value.choice.UE_associatedLogicalNG_ConnectionItem;
    item->aMF_UE_NGAP_ID = amf_ue_ngap_id;
    item->rAN_UE_NGAP_ID = ran_ue_ngap_id;

    return ngap_build_s1_reset(group, cause, partOfNG_Interface);
}

ogs_pkbuf_t *ngap_build_s1_reset_ack(
        NGAP_UE_associatedLogicalNG_connectionList_t *partOfNG_Interface)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_ResetAcknowledge_t *ResetAcknowledge = NULL;

    NGAP_ResetAcknowledgeIEs_t *ie = NULL;

    ogs_debug("Reset acknowledge");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = NGAP_ProcedureCode_id_Reset;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_ResetAcknowledge;

    ResetAcknowledge = &successfulOutcome->value.choice.ResetAcknowledge;

    if (partOfNG_Interface && partOfNG_Interface->list.count) {
        int i = 0;
        NGAP_UE_associatedLogicalNG_ConnectionListResAck_t *list = NULL;

        ie = CALLOC(1, sizeof(NGAP_ResetAcknowledgeIEs_t));
        ASN_SEQUENCE_ADD(&ResetAcknowledge->protocolIEs, ie);

        ie->id =
            NGAP_ProtocolIE_ID_id_UE_associatedLogicalNG_ConnectionListResAck;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_ResetAcknowledgeIEs__value_PR_UE_associatedLogicalNG_ConnectionListResAck;

        list = &ie->value.choice.UE_associatedLogicalNG_ConnectionListResAck;

        for (i = 0; i < partOfNG_Interface->list.count; i++) {
            NGAP_UE_associatedLogicalNG_ConnectionItemRes_t *ie1 = NULL;
            NGAP_UE_associatedLogicalNG_ConnectionItem_t *item1 = NULL;

            NGAP_UE_associatedLogicalNG_ConnectionItemResAck_t *ie2 = NULL;
            NGAP_UE_associatedLogicalNG_ConnectionItem_t *item2 = NULL;

            ie1 = (NGAP_UE_associatedLogicalNG_ConnectionItemRes_t *)
                partOfNG_Interface->list.array[i];
            ogs_assert(ie1);

            item1 = &ie1->value.choice.UE_associatedLogicalNG_ConnectionItem;
            ogs_assert(item1);

            if (item1->aMF_UE_NGAP_ID == NULL &&
                    item1->rAN_UE_NGAP_ID == NULL) {
                ogs_warn("No AMF_UE_NGAP_ID & RAN_UE_NGAP_ID");
                continue;
            }

            ie2 = CALLOC(1,
                    sizeof(NGAP_UE_associatedLogicalNG_ConnectionItemResAck_t));
            ogs_assert(ie2);
            ASN_SEQUENCE_ADD(&list->list, ie2);

            ie2->id =
                NGAP_ProtocolIE_ID_id_UE_associatedLogicalNG_ConnectionItem;
            ie2->criticality = NGAP_Criticality_ignore;
            ie2->value.present = NGAP_UE_associatedLogicalNG_ConnectionItemResAck__value_PR_UE_associatedLogicalNG_ConnectionItem;

            item2 = &ie2->value.choice.UE_associatedLogicalNG_ConnectionItem;
            ogs_assert(item2);

            if (item1->aMF_UE_NGAP_ID) {
                item2->aMF_UE_NGAP_ID = CALLOC(1,
                        sizeof(NGAP_AMF_UE_NGAP_ID_t));
                ogs_assert(item2->aMF_UE_NGAP_ID);
                *item2->aMF_UE_NGAP_ID = *item1->aMF_UE_NGAP_ID;
            }

            if (item1->rAN_UE_NGAP_ID) {
                item2->rAN_UE_NGAP_ID = CALLOC(1,
                        sizeof(NGAP_RAN_UE_NGAP_ID_t));
                ogs_assert(item2->rAN_UE_NGAP_ID);
                *item2->rAN_UE_NGAP_ID = *item1->rAN_UE_NGAP_ID;
            }

            ogs_debug("    AMF_UE_NGAP_ID[%d] RAN_UE_NGAP_ID[%d]",
                item2->aMF_UE_NGAP_ID ? (int)*item2->aMF_UE_NGAP_ID : -1,
                item2->rAN_UE_NGAP_ID ? (int)*item2->rAN_UE_NGAP_ID : -1);
        }
    }

    return ogs_ngap_encode(&pdu);
}
#endif
