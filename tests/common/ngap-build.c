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

#include "test-ngap.h"

ogs_pkbuf_t *testngap_build_ng_setup_request(uint32_t gnb_id)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int i, j;
    ogs_plmn_id_t *plmn_id = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_NGSetupRequest_t *NGSetupRequest = NULL;

    NGAP_NGSetupRequestIEs_t *ie = NULL;
    NGAP_GlobalRANNodeID_t *GlobalRANNodeID = NULL;
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

    ogs_ngap_uint32_to_GNB_ID(gnb_id, &globalGNB_ID->gNB_ID);

    ie = CALLOC(1, sizeof(NGAP_NGSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&NGSetupRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_GlobalRANNodeID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_NGSetupRequestIEs__value_PR_GlobalRANNodeID;

    GlobalRANNodeID = &ie->value.choice.GlobalRANNodeID;

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

    SupportedTAItem = CALLOC(1, sizeof(NGAP_SupportedTAItem_t));
    if (test_self()->served_tai[0].list2.num)
        ogs_asn_uint24_to_OCTET_STRING(
            test_self()->served_tai[0].list2.tai[0].tac, &SupportedTAItem->tAC);
    else if (test_self()->served_tai[0].list0.tai[0].num)
        ogs_asn_uint24_to_OCTET_STRING(
            test_self()->served_tai[0].list0.tai[0].tac[0],
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

            SliceSupportItem = CALLOC(1, sizeof(NGAP_SliceSupportItem_t));
            ogs_asn_uint8_to_OCTET_STRING(s_nssai->sst,
                    &SliceSupportItem->s_NSSAI.sST);
            if (s_nssai->sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
                SliceSupportItem->s_NSSAI.sD = CALLOC(1, sizeof(ogs_uint24_t));
                ogs_asn_uint24_to_OCTET_STRING(
                        s_nssai->sd, SliceSupportItem->s_NSSAI.sD);
            }

            ASN_SEQUENCE_ADD(&BroadcastPLMNItem->tAISliceSupportList.list,
                            SliceSupportItem);
        }

        ASN_SEQUENCE_ADD(&SupportedTAItem->broadcastPLMNList.list,
                BroadcastPLMNItem);
    }

    ASN_SEQUENCE_ADD(&SupportedTAList->list, SupportedTAItem);

    *PagingDRX = NGAP_PagingDRX_v64;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_initial_ue_message(ogs_pkbuf_t *gmmbuf)
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

    ie = CALLOC(1, sizeof(NGAP_InitialUEMessage_IEs_t));
    ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UEContextRequest;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_InitialUEMessage_IEs__value_PR_UEContextRequest;

    UEContextRequest = &ie->value.choice.UEContextRequest;

    *RAN_UE_NGAP_ID = 1;

    NAS_PDU->size = gmmbuf->len;
    NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
    memcpy(NAS_PDU->buf, gmmbuf->data, NAS_PDU->size);
    ogs_pkbuf_free(gmmbuf);

    userLocationInformationNR =
            CALLOC(1, sizeof(NGAP_UserLocationInformationNR_t));

    nR_CGI = &userLocationInformationNR->nR_CGI;
    ogs_ngap_nr_cgi_to_ASN(&test_self()->cgi, nR_CGI);

    tAI = &userLocationInformationNR->tAI;
    ogs_ngap_5gs_tai_to_ASN(&test_self()->tai, tAI);

    UserLocationInformation->present =
        NGAP_UserLocationInformation_PR_userLocationInformationNR;
    UserLocationInformation->choice.userLocationInformationNR =
        userLocationInformationNR;

    *RRCEstablishmentCause = NGAP_RRCEstablishmentCause_mt_Access;

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
    ie->criticality = NGAP_Criticality_reject;
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
    ogs_ngap_nr_cgi_to_ASN(&test_self()->cgi, nR_CGI);

    tAI = &userLocationInformationNR->tAI;
    ogs_ngap_5gs_tai_to_ASN(&test_self()->tai, tAI);

    UserLocationInformation->present =
        NGAP_UserLocationInformation_PR_userLocationInformationNR;
    UserLocationInformation->choice.userLocationInformationNR =
        userLocationInformationNR;

    return ogs_ngap_encode(&pdu);
}
