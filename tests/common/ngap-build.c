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

#include "test-common.h"

static ogs_pkbuf_t *testngap_build_pdu_session_resource_setup_response_trasfer(
        test_bearer_t *qos_flow);

ogs_pkbuf_t *testngap_build_ng_setup_request(uint32_t gnb_id, uint8_t bitsize)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int i, j;
    ogs_plmn_id_t *plmn_id = NULL;
    const char *ran_node_name = "5G gNB-CU";

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_NGSetupRequest_t *NGSetupRequest = NULL;

    NGAP_NGSetupRequestIEs_t *ie = NULL;
    NGAP_GlobalRANNodeID_t *GlobalRANNodeID = NULL;
    NGAP_RANNodeName_t *RANNodeName = NULL;
    NGAP_GlobalGNB_ID_t *globalGNB_ID = NULL;
    NGAP_SupportedTAList_t *SupportedTAList = NULL;
    NGAP_SupportedTAItem_t *SupportedTAItem = NULL;
    NGAP_BroadcastPLMNItem_t *BroadcastPLMNItem = NULL;
    NGAP_SliceSupportItem_t *SliceSupportItem = NULL;
    NGAP_PLMNIdentity_t *pLMNIdentity = NULL;
    NGAP_PagingDRX_t *PagingDRX = NULL;

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = NGAP_ProcedureCode_id_NGSetup;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_NGSetupRequest;

    NGSetupRequest = &initiatingMessage->value.choice.NGSetupRequest;

    globalGNB_ID = CALLOC(1, sizeof(NGAP_GlobalGNB_ID_t));

    plmn_id = &test_self()->plmn_support[0].plmn_id;
    ogs_asn_buffer_to_OCTET_STRING(
            plmn_id, OGS_PLMN_ID_LEN, &globalGNB_ID->pLMNIdentity);

    ogs_ngap_uint32_to_GNB_ID(gnb_id, bitsize, &globalGNB_ID->gNB_ID);

    ie = CALLOC(1, sizeof(NGAP_NGSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&NGSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_GlobalRANNodeID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_NGSetupRequestIEs__value_PR_GlobalRANNodeID;

    GlobalRANNodeID = &ie->value.choice.GlobalRANNodeID;

    ie = CALLOC(1, sizeof(NGAP_NGSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&NGSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RANNodeName;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_NGSetupRequestIEs__value_PR_RANNodeName;

    RANNodeName = &ie->value.choice.RANNodeName;

    ie = CALLOC(1, sizeof(NGAP_NGSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&NGSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SupportedTAList;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_NGSetupRequestIEs__value_PR_SupportedTAList;

    SupportedTAList = &ie->value.choice.SupportedTAList;

    ie = CALLOC(1, sizeof(NGAP_NGSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&NGSetupRequest->protocolIEs, ie);
    
    ie->id = NGAP_ProtocolIE_ID_id_DefaultPagingDRX;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_NGSetupRequestIEs__value_PR_PagingDRX;

    PagingDRX = &ie->value.choice.PagingDRX;

    GlobalRANNodeID->present = NGAP_GlobalRANNodeID_PR_globalGNB_ID;
    GlobalRANNodeID->choice.globalGNB_ID = globalGNB_ID;

    ogs_asn_buffer_to_OCTET_STRING((char*)ran_node_name,
            strlen(ran_node_name), RANNodeName);

    SupportedTAItem = CALLOC(1, sizeof(NGAP_SupportedTAItem_t));
    if (test_self()->nr_served_tai[0].list2.num)
        ogs_asn_uint24_to_OCTET_STRING(
            test_self()->nr_served_tai[0].list2.tai[0].tac,
            &SupportedTAItem->tAC);
    else if (test_self()->nr_served_tai[0].list0.tai[0].num)
        ogs_asn_uint24_to_OCTET_STRING(
            test_self()->nr_served_tai[0].list0.tai[0].tac[0],
                &SupportedTAItem->tAC);
    else
        ogs_assert_if_reached();

    for (i = 0; i < test_self()->num_of_plmn_support; i++) {
        plmn_id = &test_self()->plmn_support[i].plmn_id;

        BroadcastPLMNItem = CALLOC(1, sizeof(NGAP_BroadcastPLMNItem_t));

        ogs_asn_buffer_to_OCTET_STRING(
                plmn_id, OGS_PLMN_ID_LEN, &BroadcastPLMNItem->pLMNIdentity);

        for (j = 0; j < test_self()->plmn_support[i].num_of_s_nssai; j++) {
            ogs_s_nssai_t *s_nssai = &test_self()->plmn_support[i].s_nssai[j];
            ogs_uint24_t sd;

            SliceSupportItem = CALLOC(1, sizeof(NGAP_SliceSupportItem_t));
            ogs_asn_uint8_to_OCTET_STRING(s_nssai->sst,
                    &SliceSupportItem->s_NSSAI.sST);
            sd.v = s_nssai->sd.v;
            if (sd.v == OGS_S_NSSAI_NO_SD_VALUE)
                sd.v = 0x010000;
            SliceSupportItem->s_NSSAI.sD = CALLOC(1, sizeof(NGAP_SD_t));
            ogs_asn_uint24_to_OCTET_STRING(sd, SliceSupportItem->s_NSSAI.sD);

            ASN_SEQUENCE_ADD(&BroadcastPLMNItem->tAISliceSupportList.list,
                            SliceSupportItem);
        }

        ASN_SEQUENCE_ADD(&SupportedTAItem->broadcastPLMNList.list,
                BroadcastPLMNItem);
    }

    ASN_SEQUENCE_ADD(&SupportedTAList->list, SupportedTAItem);

    *PagingDRX = NGAP_PagingDRX_v32;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_initial_ue_message(
        test_ue_t *test_ue, ogs_pkbuf_t *gmmbuf, bool s_tmsi)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int i, j;
    char buf[5];

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_InitialUEMessage_t *InitialUEMessage = NULL;

    NGAP_InitialUEMessage_IEs_t *ie = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;
    NGAP_UserLocationInformation_t *UserLocationInformation = NULL;
    NGAP_UserLocationInformationNR_t *userLocationInformationNR = NULL;
    NGAP_NR_CGI_t *nR_CGI = NULL;
    NGAP_TAI_t *tAI = NULL;
    NGAP_RRCEstablishmentCause_t *RRCEstablishmentCause = NULL;
    NGAP_UEContextRequest_t *UEContextRequest = NULL;
    NGAP_FiveG_S_TMSI_t *FiveG_S_TMSI = NULL;

    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = NGAP_ProcedureCode_id_InitialUEMessage;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_InitialUEMessage;

    InitialUEMessage = &initiatingMessage->value.choice.InitialUEMessage;

    ie = CALLOC(1, sizeof(NGAP_InitialUEMessage_IEs_t));
    ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_InitialUEMessage_IEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_InitialUEMessage_IEs_t));
    ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_NAS_PDU;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_InitialUEMessage_IEs__value_PR_NAS_PDU;

    NAS_PDU = &ie->value.choice.NAS_PDU;

    ie = CALLOC(1, sizeof(NGAP_InitialUEMessage_IEs_t));
    ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UserLocationInformation;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_InitialUEMessage_IEs__value_PR_UserLocationInformation;

    UserLocationInformation = &ie->value.choice.UserLocationInformation;

    ie = CALLOC(1, sizeof(NGAP_InitialUEMessage_IEs_t));
    ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RRCEstablishmentCause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_InitialUEMessage_IEs__value_PR_RRCEstablishmentCause;

    RRCEstablishmentCause = &ie->value.choice.RRCEstablishmentCause;

    test_ue->ran_ue_ngap_id++;
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    NAS_PDU->size = gmmbuf->len;
    NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
    memcpy(NAS_PDU->buf, gmmbuf->data, NAS_PDU->size);
    ogs_pkbuf_free(gmmbuf);

    userLocationInformationNR =
            CALLOC(1, sizeof(NGAP_UserLocationInformationNR_t));

    nR_CGI = &userLocationInformationNR->nR_CGI;
    ogs_ngap_nr_cgi_to_ASN(&test_self()->nr_cgi, nR_CGI);

    tAI = &userLocationInformationNR->tAI;
    ogs_ngap_5gs_tai_to_ASN(&test_self()->nr_tai, tAI);

    UserLocationInformation->present =
        NGAP_UserLocationInformation_PR_userLocationInformationNR;
    UserLocationInformation->choice.userLocationInformationNR =
        userLocationInformationNR;

    *RRCEstablishmentCause = NGAP_RRCEstablishmentCause_mo_Signalling;

    if (s_tmsi) {
        NGAP_AMFSetID_t *aMFSetID = NULL;
        NGAP_AMFPointer_t *aMFPointer = NULL;
        NGAP_FiveG_TMSI_t *fiveG_TMSI = NULL;

        ie = CALLOC(1, sizeof(NGAP_InitialUEMessage_IEs_t));
        ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_FiveG_S_TMSI;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_InitialUEMessage_IEs__value_PR_FiveG_S_TMSI;

        FiveG_S_TMSI = &ie->value.choice.FiveG_S_TMSI;

        aMFSetID = &FiveG_S_TMSI->aMFSetID;
        aMFPointer = &FiveG_S_TMSI->aMFPointer;
        fiveG_TMSI = &FiveG_S_TMSI->fiveG_TMSI;

        ogs_ngap_uint16_to_AMFSetID(
                ogs_amf_set_id(&test_ue->nas_5gs_guti.amf_id), aMFSetID);
        ogs_ngap_uint8_to_AMFPointer(
                ogs_amf_pointer(&test_ue->nas_5gs_guti.amf_id), aMFPointer);
        ogs_asn_uint32_to_OCTET_STRING(
                test_ue->nas_5gs_guti.m_tmsi, fiveG_TMSI);
    }

    ie = CALLOC(1, sizeof(NGAP_InitialUEMessage_IEs_t));
    ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UEContextRequest;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_InitialUEMessage_IEs__value_PR_UEContextRequest;

    UEContextRequest = &ie->value.choice.UEContextRequest;

    *UEContextRequest = NGAP_UEContextRequest_requested;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_uplink_nas_transport(
        test_ue_t *test_ue, ogs_pkbuf_t *gmmbuf)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UplinkNASTransport_t *UplinkNASTransport = NULL;

    NGAP_UplinkNASTransport_IEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;
    NGAP_UserLocationInformation_t *UserLocationInformation = NULL;
    NGAP_UserLocationInformationNR_t *userLocationInformationNR = NULL;
    NGAP_NR_CGI_t *nR_CGI = NULL;
    NGAP_TAI_t *tAI = NULL;

    ogs_assert(gmmbuf);
    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_UplinkNASTransport;
    initiatingMessage->criticality = NGAP_Criticality_ignore;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_UplinkNASTransport;

    UplinkNASTransport =
        &initiatingMessage->value.choice.UplinkNASTransport;

    ie = CALLOC(1, sizeof(NGAP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_UplinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_UplinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_NAS_PDU;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_UplinkNASTransport_IEs__value_PR_NAS_PDU;

    NAS_PDU = &ie->value.choice.NAS_PDU;

    ie = CALLOC(1, sizeof(NGAP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UserLocationInformation;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_UplinkNASTransport_IEs__value_PR_UserLocationInformation;

    UserLocationInformation = &ie->value.choice.UserLocationInformation;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    NAS_PDU->size = gmmbuf->len;
    NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
    memcpy(NAS_PDU->buf, gmmbuf->data, NAS_PDU->size);
    ogs_pkbuf_free(gmmbuf);

    userLocationInformationNR =
            CALLOC(1, sizeof(NGAP_UserLocationInformationNR_t));

    nR_CGI = &userLocationInformationNR->nR_CGI;
    ogs_ngap_nr_cgi_to_ASN(&test_self()->nr_cgi, nR_CGI);

    tAI = &userLocationInformationNR->tAI;
    ogs_ngap_5gs_tai_to_ASN(&test_self()->nr_tai, tAI);

    UserLocationInformation->present =
        NGAP_UserLocationInformation_PR_userLocationInformationNR;
    UserLocationInformation->choice.userLocationInformationNR =
        userLocationInformationNR;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_initial_context_setup_response(
        test_ue_t *test_ue, test_sess_t *sess)
{
    int rv;

    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_InitialContextSetupResponse_t *InitialContextSetupResponse = NULL;

    NGAP_InitialContextSetupResponseIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode =
        NGAP_ProcedureCode_id_InitialContextSetup;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_InitialContextSetupResponse;

    InitialContextSetupResponse =
        &successfulOutcome->value.choice.InitialContextSetupResponse;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_InitialContextSetupResponseIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_InitialContextSetupResponseIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    if (sess) {
        NGAP_PDUSessionResourceSetupListCxtRes_t *PDUSessionList = NULL;
        NGAP_PDUSessionResourceSetupItemCxtRes_t *PDUSessionItem = NULL;
        OCTET_STRING_t *transfer = NULL;
        ogs_pkbuf_t *n2smbuf = NULL;

        test_bearer_t *bearer = NULL;

        ie = CALLOC(1, sizeof(NGAP_InitialContextSetupResponseIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupResponse->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtRes;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present = NGAP_InitialContextSetupResponseIEs__value_PR_PDUSessionResourceSetupListCxtRes;

        PDUSessionList = &ie->value.choice.PDUSessionResourceSetupListCxtRes;

        PDUSessionItem =
            CALLOC(1, sizeof(struct NGAP_PDUSessionResourceSetupItemCxtRes));
        ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

        PDUSessionItem->pDUSessionID = sess->psi;

        bearer = ogs_list_first(&sess->bearer_list);
        ogs_assert(bearer);

        n2smbuf = testngap_build_pdu_session_resource_setup_response_trasfer(
                bearer);
        ogs_assert(n2smbuf);
        transfer = &PDUSessionItem->pDUSessionResourceSetupResponseTransfer;

        transfer->size = n2smbuf->len;
        transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
        memcpy(transfer->buf, n2smbuf->data, transfer->size);
        ogs_pkbuf_free(n2smbuf);
    }

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_initial_context_setup_failure(test_ue_t *test_ue,
        NGAP_Cause_PR group, long cause)
{
    int rv;

    NGAP_NGAP_PDU_t pdu;
    NGAP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    NGAP_InitialContextSetupFailure_t *InitialContextSetupFailure = NULL;

    NGAP_InitialContextSetupFailureIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_unsuccessfulOutcome;
    pdu.choice.unsuccessfulOutcome =
        CALLOC(1, sizeof(NGAP_UnsuccessfulOutcome_t));

    unsuccessfulOutcome = pdu.choice.unsuccessfulOutcome;
    unsuccessfulOutcome->procedureCode =
        NGAP_ProcedureCode_id_InitialContextSetup;
    unsuccessfulOutcome->criticality = NGAP_Criticality_reject;
    unsuccessfulOutcome->value.present =
        NGAP_UnsuccessfulOutcome__value_PR_InitialContextSetupFailure;

    InitialContextSetupFailure =
        &unsuccessfulOutcome->value.choice.InitialContextSetupFailure;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupFailureIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_InitialContextSetupFailureIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupFailureIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_InitialContextSetupFailureIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_InitialContextSetupFailureIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_InitialContextSetupFailureIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_ue_radio_capability_info_indication(
        test_ue_t *test_ue)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UERadioCapabilityInfoIndication_t *UERadioCapabilityInfoIndication =
        NULL;

    NGAP_UERadioCapabilityInfoIndicationIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_UERadioCapability_t *UERadioCapability = NULL;

    uint8_t tmp[OGS_MAX_SDU_LEN];
    char *_capability_captured = "040ca1080f"
        "de1a00000074e5a0 31e000380a03c126 0c80038d80818804 804c0203018022a3"
        "6146b040d0d00012 0098087ad8202020 e1de38720c380020 64f1f570000f001c"
        "0003c002a36266b0 40d0d00012009808 7ad8202020e1de38 720c38002064f1f5"
        "70000f001c0003c0 02a3626eb040d0d0 00120098087ad820 2020e1de38720c38"
        "002064f1f570000f 001c0003c0003403 898000000e480713 4000001c900e1400"
        "0000390088407022 6c70e04089b1c381 0226c70e007b0380 c000a07010140c00"
        "a0341f0000800000 00032a6802080000 4b2ca82000032cb2 800a955d52a9a020"
        "7000b00000004008 0118b10000204060 71112131b1f21252 62728292a4a50123"
        "bb40002000208183 91244cd9f4295327 50a551021ffff21f fff21ffff21ffff2"
        "1ffff21ffff21fff f21ffff21ffff21f fff21ffff21ffff2 1ffff21ffff21fff"
        "f21ffff21fffffe0 dd88600280706900 00000054888af410 0001a47a08000023"
        "fe40000331401c36 980610fffffffff3 5500020045240000 80000412c3804260"
        "9a143c0c00000410 0004104394000000 0000";

    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_UERadioCapabilityInfoIndication;
    initiatingMessage->criticality = NGAP_Criticality_ignore;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_UERadioCapabilityInfoIndication;

    UERadioCapabilityInfoIndication =
        &initiatingMessage->value.choice.UERadioCapabilityInfoIndication;

    ie = CALLOC(1, sizeof(NGAP_UERadioCapabilityInfoIndicationIEs_t));
    ASN_SEQUENCE_ADD(&UERadioCapabilityInfoIndication->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_UERadioCapabilityInfoIndicationIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_UERadioCapabilityInfoIndicationIEs_t));
    ASN_SEQUENCE_ADD(&UERadioCapabilityInfoIndication->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_UERadioCapabilityInfoIndicationIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_UERadioCapabilityInfoIndicationIEs_t));
    ASN_SEQUENCE_ADD(&UERadioCapabilityInfoIndication->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UERadioCapability;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_UERadioCapabilityInfoIndicationIEs__value_PR_UERadioCapability;

    UERadioCapability = &ie->value.choice.UERadioCapability;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    OGS_HEX(_capability_captured, strlen(_capability_captured), tmp),

    UERadioCapability->size = 407;
    UERadioCapability->buf = CALLOC(UERadioCapability->size, sizeof(uint8_t));
    memcpy(UERadioCapability->buf, tmp, UERadioCapability->size);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_ue_context_release_request(test_ue_t *test_ue,
        NGAP_Cause_PR group, long cause, bool pdu_session)
{
    int rv;
    test_sess_t *sess = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UEContextReleaseRequest_t *UEContextReleaseRequest = NULL;

    NGAP_UEContextReleaseRequest_IEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceListCxtRelReq_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceItemCxtRelReq_t *PDUSessionItem = NULL;
    NGAP_Cause_t *Cause = NULL;

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_UEContextReleaseRequest;
    initiatingMessage->criticality = NGAP_Criticality_ignore;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_UEContextReleaseRequest;

    UEContextReleaseRequest =
        &initiatingMessage->value.choice.UEContextReleaseRequest;

    ie = CALLOC(1, sizeof(NGAP_UEContextReleaseRequest_IEs_t));
    ASN_SEQUENCE_ADD(&UEContextReleaseRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_UEContextReleaseRequest_IEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_UEContextReleaseRequest_IEs_t));
    ASN_SEQUENCE_ADD(&UEContextReleaseRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_UEContextReleaseRequest_IEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    if (pdu_session) {
        ie = CALLOC(1, sizeof(NGAP_UEContextReleaseRequest_IEs_t));
        ASN_SEQUENCE_ADD(&UEContextReleaseRequest->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceListCxtRelReq;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present = NGAP_UEContextReleaseRequest_IEs__value_PR_PDUSessionResourceListCxtRelReq;

        PDUSessionList = &ie->value.choice.PDUSessionResourceListCxtRelReq;

        PDUSessionItem =
            CALLOC(1, sizeof(struct NGAP_PDUSessionResourceItemCxtRelReq));
        ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

        sess = ogs_list_first(&test_ue->sess_list);
        ogs_assert(sess);

        PDUSessionItem->pDUSessionID = sess->psi;
    }

    ie = CALLOC(1, sizeof(NGAP_UEContextReleaseRequest_IEs_t));
    ASN_SEQUENCE_ADD(&UEContextReleaseRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_UEContextReleaseRequest_IEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_ue_context_release_complete(test_ue_t *test_ue)
{
    int rv;

    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_UEContextReleaseComplete_t *UEContextReleaseComplete = NULL;

    NGAP_UEContextReleaseComplete_IEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode =
        NGAP_ProcedureCode_id_UEContextRelease;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_UEContextReleaseComplete;

    UEContextReleaseComplete =
        &successfulOutcome->value.choice.UEContextReleaseComplete;

    ie = CALLOC(1, sizeof(NGAP_UEContextReleaseComplete_IEs_t));
    ASN_SEQUENCE_ADD(&UEContextReleaseComplete->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_UEContextReleaseComplete_IEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_UEContextReleaseComplete_IEs_t));
    ASN_SEQUENCE_ADD(&UEContextReleaseComplete->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_UEContextReleaseComplete_IEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_pdu_session_resource_setup_response(
        test_sess_t *sess)
{
    int rv;

    test_ue_t *test_ue = NULL;
    test_bearer_t *qos_flow = NULL;

    ogs_pkbuf_t *n2smbuf = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_PDUSessionResourceSetupResponse_t *PDUSessionResourceSetupResponse;

    NGAP_PDUSessionResourceSetupResponseIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceSetupListSURes_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceSetupItemSURes_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode =
        NGAP_ProcedureCode_id_PDUSessionResourceSetup;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_PDUSessionResourceSetupResponse;

    PDUSessionResourceSetupResponse =
        &successfulOutcome->value.choice.PDUSessionResourceSetupResponse;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceSetupResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PDUSessionResourceSetupResponseIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceSetupResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PDUSessionResourceSetupResponseIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceSetupResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListSURes;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceSetupListSURes;

    PDUSessionList = &ie->value.choice.PDUSessionResourceSetupListSURes;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    PDUSessionItem =
        CALLOC(1, sizeof(struct NGAP_PDUSessionResourceSetupItemSURes));
    ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

    PDUSessionItem->pDUSessionID = sess->psi;

    qos_flow = ogs_list_first(&sess->bearer_list);
    ogs_assert(qos_flow);

    n2smbuf = testngap_build_pdu_session_resource_setup_response_trasfer(
            qos_flow);
    ogs_assert(n2smbuf);
    transfer = &PDUSessionItem->pDUSessionResourceSetupResponseTransfer;

    transfer->size = n2smbuf->len;
    transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
    memcpy(transfer->buf, n2smbuf->data, transfer->size);
    ogs_pkbuf_free(n2smbuf);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_pdu_session_resource_release_response(
        test_sess_t *sess)
{
    int rv;
    test_ue_t *test_ue;
    ogs_pkbuf_t *ngapbuf = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_PDUSessionResourceReleaseResponse_t *PDUSessionResourceReleaseResponse;

    NGAP_PDUSessionResourceReleaseResponseIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceReleasedListRelRes_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceReleasedItemRelRes_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode =
        NGAP_ProcedureCode_id_PDUSessionResourceRelease;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_PDUSessionResourceReleaseResponse;

    PDUSessionResourceReleaseResponse =
        &successfulOutcome->value.choice.PDUSessionResourceReleaseResponse;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceReleaseResponseIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceReleaseResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PDUSessionResourceReleaseResponseIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceReleaseResponseIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceReleaseResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PDUSessionResourceReleaseResponseIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceReleaseResponseIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceReleaseResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceReleasedListRelRes;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_PDUSessionResourceReleaseResponseIEs__value_PR_PDUSessionResourceReleasedListRelRes;

    PDUSessionList = &ie->value.choice.PDUSessionResourceReleasedListRelRes;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    PDUSessionItem =
        CALLOC(1, sizeof(struct NGAP_PDUSessionResourceReleasedItemRelRes));
    ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

    PDUSessionItem->pDUSessionID = sess->psi;

    transfer = &PDUSessionItem->pDUSessionResourceReleaseResponseTransfer;

    transfer->size = 1;
    transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));

    return ogs_ngap_encode(&pdu);
}

static ogs_pkbuf_t *testngap_build_pdu_session_resource_setup_response_trasfer(
        test_bearer_t *qos_flow)
{
    int rv;

    test_sess_t *sess = NULL;

    ogs_gtp_f_teid_t f_teid;
    ogs_ip_t ip;
    int len;

    ogs_assert(qos_flow);
    sess = qos_flow->sess;
    ogs_assert(sess);

    NGAP_PDUSessionResourceSetupResponseTransfer_t message;

    NGAP_QosFlowPerTNLInformation_t *dLQosFlowPerTNLInformation = NULL;

    NGAP_UPTransportLayerInformation_t *uPTransportLayerInformation = NULL;
    NGAP_GTPTunnel_t *gTPTunnel = NULL;

    NGAP_AssociatedQosFlowList_t *associatedQosFlowList = NULL;
    NGAP_AssociatedQosFlowItem_t *associatedQosFlowItem = NULL;

    memset(&message, 0, sizeof(message));

    dLQosFlowPerTNLInformation = &message.dLQosFlowPerTNLInformation;
    uPTransportLayerInformation =
        &dLQosFlowPerTNLInformation->uPTransportLayerInformation;

    gTPTunnel = CALLOC(1, sizeof(struct NGAP_GTPTunnel));
    uPTransportLayerInformation->present =
        NGAP_UPTransportLayerInformation_PR_gTPTunnel;
    uPTransportLayerInformation->choice.gTPTunnel = gTPTunnel;

    ogs_assert(sess->gnb_n3_addr || sess->gnb_n3_addr6);
    rv = ogs_gtp_sockaddr_to_f_teid(
            sess->gnb_n3_addr, sess->gnb_n3_addr6, &f_teid, &len);
    ogs_assert(rv == OGS_OK);

    rv = ogs_gtp_f_teid_to_ip(&f_teid, &ip);
    ogs_assert(rv == OGS_OK);

    ogs_asn_ip_to_BIT_STRING(&ip, &gTPTunnel->transportLayerAddress);
    ogs_asn_uint32_to_OCTET_STRING(sess->gnb_n3_teid, &gTPTunnel->gTP_TEID);

    associatedQosFlowList = &dLQosFlowPerTNLInformation->associatedQosFlowList;
    associatedQosFlowItem =
        CALLOC(1, sizeof(struct NGAP_AssociatedQosFlowItem));
    ASN_SEQUENCE_ADD(&associatedQosFlowList->list, associatedQosFlowItem);

    associatedQosFlowItem->qosFlowIdentifier = qos_flow->qfi;

    return ogs_asn_encode(
            &asn_DEF_NGAP_PDUSessionResourceSetupResponseTransfer, &message);
}
