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
        test_sess_t *sess);
static ogs_pkbuf_t *testngap_build_pdu_session_resource_setup_unsuccessful_trasfer(
        test_sess_t *sess, NGAP_Cause_PR group, long cause);
static ogs_pkbuf_t *testngap_build_pdu_session_resource_modify_response_trasfer(
        test_sess_t *sess);
static ogs_pkbuf_t *testngap_build_qos_flow_resource_release_response_trasfer(
        test_bearer_t *qos_flow);
static ogs_pkbuf_t *testngap_build_path_switch_request_trasfer(
        test_sess_t *sess);
static ogs_pkbuf_t *testngap_build_handover_required_transfer(
        test_sess_t *sess, bool direct);
static ogs_pkbuf_t *testngap_build_handover_request_ack_transfer(
        test_sess_t *sess);

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
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

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

            SliceSupportItem = CALLOC(1, sizeof(NGAP_SliceSupportItem_t));
            ogs_asn_uint8_to_OCTET_STRING(s_nssai->sst,
                    &SliceSupportItem->s_NSSAI.sST);
            if (s_nssai->sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
                SliceSupportItem->s_NSSAI.sD = CALLOC(1, sizeof(NGAP_SD_t));
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

    *PagingDRX = NGAP_PagingDRX_v32;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_ran_configuration_update(bool supported_ta_list)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int i, j;
    ogs_plmn_id_t *plmn_id = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_RANConfigurationUpdate_t *RANConfigurationUpdate = NULL;

    NGAP_RANConfigurationUpdateIEs_t *ie = NULL;
    NGAP_GlobalRANNodeID_t *GlobalRANNodeID = NULL;
    NGAP_RANNodeName_t *RANNodeName = NULL;
    NGAP_GlobalGNB_ID_t *globalGNB_ID = NULL;
    NGAP_SupportedTAList_t *SupportedTAList = NULL;
    NGAP_SupportedTAItem_t *SupportedTAItem = NULL;
    NGAP_BroadcastPLMNItem_t *BroadcastPLMNItem = NULL;
    NGAP_SliceSupportItem_t *SliceSupportItem = NULL;
    NGAP_PLMNIdentity_t *pLMNIdentity = NULL;

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_RANConfigurationUpdate;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_RANConfigurationUpdate;

    RANConfigurationUpdate =
        &initiatingMessage->value.choice.RANConfigurationUpdate;

    if (supported_ta_list == true) {
        ie = CALLOC(1, sizeof(NGAP_RANConfigurationUpdateIEs_t));
        ASN_SEQUENCE_ADD(&RANConfigurationUpdate->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_SupportedTAList;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present =
            NGAP_RANConfigurationUpdateIEs__value_PR_SupportedTAList;

        SupportedTAList = &ie->value.choice.SupportedTAList;

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
                ogs_s_nssai_t *s_nssai =
                    &test_self()->plmn_support[i].s_nssai[j];

                SliceSupportItem = CALLOC(1, sizeof(NGAP_SliceSupportItem_t));
                ogs_asn_uint8_to_OCTET_STRING(s_nssai->sst,
                        &SliceSupportItem->s_NSSAI.sST);
                if (s_nssai->sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
                    SliceSupportItem->s_NSSAI.sD = CALLOC(1, sizeof(NGAP_SD_t));
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
    }

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_initial_ue_message(
        test_ue_t *test_ue, ogs_pkbuf_t *gmmbuf,
        uint8_t cause, bool s_tmsi, bool ue_context_requested)
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
    initiatingMessage->criticality = NGAP_Criticality_ignore;
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
    ogs_ngap_nr_cgi_to_ASN(&test_ue->nr_cgi, nR_CGI);

    tAI = &userLocationInformationNR->tAI;
    ogs_ngap_5gs_tai_to_ASN(&test_ue->nr_tai, tAI);

    UserLocationInformation->present =
        NGAP_UserLocationInformation_PR_userLocationInformationNR;
    UserLocationInformation->choice.userLocationInformationNR =
        userLocationInformationNR;

    *RRCEstablishmentCause = cause;

    if (s_tmsi) {
        NGAP_AMFSetID_t *aMFSetID = NULL;
        NGAP_AMFPointer_t *aMFPointer = NULL;
        NGAP_FiveG_TMSI_t *fiveG_TMSI = NULL;

        ie = CALLOC(1, sizeof(NGAP_InitialUEMessage_IEs_t));
        ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_FiveG_S_TMSI;
        ie->criticality = NGAP_Criticality_reject;
        ie->value.present = NGAP_InitialUEMessage_IEs__value_PR_FiveG_S_TMSI;

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

    if (ue_context_requested) {
        ie = CALLOC(1, sizeof(NGAP_InitialUEMessage_IEs_t));
        ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

        ie->id = NGAP_ProtocolIE_ID_id_UEContextRequest;
        ie->criticality = NGAP_Criticality_ignore;
        ie->value.present =
            NGAP_InitialUEMessage_IEs__value_PR_UEContextRequest;

        UEContextRequest = &ie->value.choice.UEContextRequest;

        *UEContextRequest = NGAP_UEContextRequest_requested;
    }

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
    ogs_ngap_nr_cgi_to_ASN(&test_ue->nr_cgi, nR_CGI);

    tAI = &userLocationInformationNR->tAI;
    ogs_ngap_5gs_tai_to_ASN(&test_ue->nr_tai, tAI);

    UserLocationInformation->present =
        NGAP_UserLocationInformation_PR_userLocationInformationNR;
    UserLocationInformation->choice.userLocationInformationNR =
        userLocationInformationNR;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_initial_context_setup_response(
        test_ue_t *test_ue, bool pdu_session)
{
    int rv;
    test_sess_t *sess = NULL;

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

    if (pdu_session) {
        NGAP_PDUSessionResourceSetupListCxtRes_t *PDUSessionList = NULL;
        NGAP_PDUSessionResourceSetupItemCxtRes_t *PDUSessionItem = NULL;

        ogs_list_for_each(&test_ue->sess_list, sess) {
            OCTET_STRING_t *transfer = NULL;
            ogs_pkbuf_t *n2smbuf = NULL;

            if (!PDUSessionList) {
                ie = CALLOC(1, sizeof(NGAP_InitialContextSetupResponseIEs_t));
                ASN_SEQUENCE_ADD(&InitialContextSetupResponse->protocolIEs, ie);

                ie->id =
                    NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtRes;
                ie->criticality = NGAP_Criticality_ignore;
                ie->value.present = NGAP_InitialContextSetupResponseIEs__value_PR_PDUSessionResourceSetupListCxtRes;

                PDUSessionList =
                    &ie->value.choice.PDUSessionResourceSetupListCxtRes;
            }

            PDUSessionItem = CALLOC(1,
                    sizeof(struct NGAP_PDUSessionResourceSetupItemCxtRes));
            ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

            PDUSessionItem->pDUSessionID = sess->psi;

            n2smbuf =
                testngap_build_pdu_session_resource_setup_response_trasfer(
                    sess);
            ogs_assert(n2smbuf);
            transfer = &PDUSessionItem->pDUSessionResourceSetupResponseTransfer;

            transfer->size = n2smbuf->len;
            transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
            memcpy(transfer->buf, n2smbuf->data, transfer->size);
            ogs_pkbuf_free(n2smbuf);
        }
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
        NGAP_PDUSessionResourceListCxtRelReq_t *PDUSessionList = NULL;
        NGAP_PDUSessionResourceItemCxtRelReq_t *PDUSessionItem = NULL;

        ogs_list_for_each(&test_ue->sess_list, sess) {
            if (!PDUSessionList) {
                ie = CALLOC(1, sizeof(NGAP_UEContextReleaseRequest_IEs_t));
                ASN_SEQUENCE_ADD(&UEContextReleaseRequest->protocolIEs, ie);

                ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceListCxtRelReq;
                ie->criticality = NGAP_Criticality_reject;
                ie->value.present = NGAP_UEContextReleaseRequest_IEs__value_PR_PDUSessionResourceListCxtRelReq;

                PDUSessionList =
                    &ie->value.choice.PDUSessionResourceListCxtRelReq;
            }

            PDUSessionItem =
                CALLOC(1, sizeof(struct NGAP_PDUSessionResourceItemCxtRelReq));
            ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

            PDUSessionItem->pDUSessionID = sess->psi;
        }
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

ogs_pkbuf_t *testngap_ue_build_pdu_session_resource_setup_response(
        test_ue_t *test_ue)
{
    int rv;

    test_sess_t *sess = NULL;

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

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    ogs_list_for_each(&test_ue->sess_list, sess) {
        if (!PDUSessionList) {
            ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceSetupResponseIEs_t));
            ASN_SEQUENCE_ADD(&PDUSessionResourceSetupResponse->protocolIEs, ie);

            ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceSetupListSURes;
            ie->criticality = NGAP_Criticality_reject;
            ie->value.present = NGAP_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceSetupListSURes;

            PDUSessionList = &ie->value.choice.PDUSessionResourceSetupListSURes;
        }

        PDUSessionItem =
            CALLOC(1, sizeof(struct NGAP_PDUSessionResourceSetupItemSURes));
        ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

        PDUSessionItem->pDUSessionID = sess->psi;

        n2smbuf = testngap_build_pdu_session_resource_setup_response_trasfer(
                sess);
        ogs_assert(n2smbuf);
        transfer = &PDUSessionItem->pDUSessionResourceSetupResponseTransfer;

        transfer->size = n2smbuf->len;
        transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
        memcpy(transfer->buf, n2smbuf->data, transfer->size);
        ogs_pkbuf_free(n2smbuf);
    }

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_sess_build_pdu_session_resource_setup_response(
        test_sess_t *sess)
{
    int rv;

    test_ue_t *test_ue = NULL;

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

    n2smbuf = testngap_build_pdu_session_resource_setup_response_trasfer(
            sess);
    ogs_assert(n2smbuf);
    transfer = &PDUSessionItem->pDUSessionResourceSetupResponseTransfer;

    transfer->size = n2smbuf->len;
    transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
    memcpy(transfer->buf, n2smbuf->data, transfer->size);
    ogs_pkbuf_free(n2smbuf);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_sess_build_pdu_session_resource_failed_to_setup(
        test_sess_t *sess, NGAP_Cause_PR group, long cause)
{
    int rv;

    test_ue_t *test_ue = NULL;

    ogs_pkbuf_t *n2smbuf = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_PDUSessionResourceSetupResponse_t *PDUSessionResourceSetupResponse;

    NGAP_PDUSessionResourceSetupResponseIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceFailedToSetupListSURes_t
        *PDUSessionFailedList = NULL;
    NGAP_PDUSessionResourceFailedToSetupItemSURes_t
        *PDUSessionFailedItem = NULL;
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

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListSURes;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceFailedToSetupListSURes;

    PDUSessionFailedList =
        &ie->value.choice.PDUSessionResourceFailedToSetupListSURes;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    PDUSessionFailedItem =
        CALLOC(1, sizeof(struct NGAP_PDUSessionResourceFailedToSetupItemSURes));
    ASN_SEQUENCE_ADD(&PDUSessionFailedList->list, PDUSessionFailedItem);

    PDUSessionFailedItem->pDUSessionID = sess->psi;

    n2smbuf = testngap_build_pdu_session_resource_setup_unsuccessful_trasfer(
            sess, group, cause);
    ogs_assert(n2smbuf);
    transfer =
        &PDUSessionFailedItem->pDUSessionResourceSetupUnsuccessfulTransfer;

    transfer->size = n2smbuf->len;
    transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
    memcpy(transfer->buf, n2smbuf->data, transfer->size);
    ogs_pkbuf_free(n2smbuf);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_sess_build_pdu_session_resource_modify_response(
        test_sess_t *sess)
{
    int rv;

    test_ue_t *test_ue = NULL;

    ogs_pkbuf_t *n2smbuf = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_PDUSessionResourceModifyResponse_t *PDUSessionResourceModifyResponse;

    NGAP_PDUSessionResourceModifyResponseIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceModifyListModRes_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceModifyItemModRes_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode =
        NGAP_ProcedureCode_id_PDUSessionResourceModify;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_PDUSessionResourceModifyResponse;

    PDUSessionResourceModifyResponse =
        &successfulOutcome->value.choice.PDUSessionResourceModifyResponse;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceModifyResponseIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceModifyResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PDUSessionResourceModifyResponseIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceModifyResponseIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceModifyResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PDUSessionResourceModifyResponseIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceModifyResponseIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceModifyResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceModifyListModRes;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceModifyResponseIEs__value_PR_PDUSessionResourceModifyListModRes;

    PDUSessionList = &ie->value.choice.PDUSessionResourceModifyListModRes;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    PDUSessionItem =
        CALLOC(1, sizeof(struct NGAP_PDUSessionResourceModifyItemModRes));
    ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

    PDUSessionItem->pDUSessionID = sess->psi;

    n2smbuf = testngap_build_pdu_session_resource_modify_response_trasfer(sess);
    ogs_assert(n2smbuf);
    transfer = &PDUSessionItem->pDUSessionResourceModifyResponseTransfer;

    transfer->size = n2smbuf->len;
    transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
    memcpy(transfer->buf, n2smbuf->data, transfer->size);
    ogs_pkbuf_free(n2smbuf);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_qos_flow_resource_modify_response(
        test_bearer_t *qos_flow)
{
    int rv;
    test_sess_t *sess = NULL;

    ogs_assert(qos_flow);
    sess = qos_flow->sess;
    ogs_assert(sess);

    ogs_list_init(&sess->qos_flow_to_modify_list);
    ogs_list_add(&sess->qos_flow_to_modify_list, &qos_flow->to_modify_node);

    return testngap_sess_build_pdu_session_resource_modify_response(sess);
}

ogs_pkbuf_t *testngap_build_qos_flow_resource_release_response(
        test_bearer_t *qos_flow)
{
    int rv;

    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;

    ogs_pkbuf_t *n2smbuf = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_PDUSessionResourceModifyResponse_t *PDUSessionResourceModifyResponse;

    NGAP_PDUSessionResourceModifyResponseIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceModifyListModRes_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceModifyItemModRes_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    ogs_assert(qos_flow);
    sess = qos_flow->sess;
    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode =
        NGAP_ProcedureCode_id_PDUSessionResourceModify;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_PDUSessionResourceModifyResponse;

    PDUSessionResourceModifyResponse =
        &successfulOutcome->value.choice.PDUSessionResourceModifyResponse;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceModifyResponseIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceModifyResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PDUSessionResourceModifyResponseIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceModifyResponseIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceModifyResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PDUSessionResourceModifyResponseIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PDUSessionResourceModifyResponseIEs_t));
    ASN_SEQUENCE_ADD(&PDUSessionResourceModifyResponse->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceModifyListModRes;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PDUSessionResourceModifyResponseIEs__value_PR_PDUSessionResourceModifyListModRes;

    PDUSessionList = &ie->value.choice.PDUSessionResourceModifyListModRes;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    PDUSessionItem =
        CALLOC(1, sizeof(struct NGAP_PDUSessionResourceModifyItemModRes));
    ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

    PDUSessionItem->pDUSessionID = sess->psi;

    n2smbuf = testngap_build_qos_flow_resource_release_response_trasfer(
            qos_flow);
    ogs_assert(n2smbuf);
    transfer = &PDUSessionItem->pDUSessionResourceModifyResponseTransfer;

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

ogs_pkbuf_t *testngap_build_uplink_ran_configuration_transfer(
        uint32_t source_gnb_id, uint8_t source_bitsize,
        uint32_t target_gnb_id, uint8_t target_bitsize)
{
    int rv;
    ogs_plmn_id_t *plmn_id = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UplinkRANConfigurationTransfer_t
        *UplinkRANConfigurationTransfer = NULL;

    NGAP_UplinkRANConfigurationTransferIEs_t *ie = NULL;
    NGAP_SONConfigurationTransfer_t *SONConfigurationTransfer = NULL;
    NGAP_SourceRANNodeID_t *sourceRANNodeID = NULL;
    NGAP_GlobalRANNodeID_t *sourceGlobalRANNodeID;
    NGAP_GlobalGNB_ID_t *sourceGlobalGNB_ID;
    NGAP_TAI_t *sourceSelectedTAI;
    NGAP_TargetRANNodeID_t *targetRANNodeID = NULL;
    NGAP_GlobalRANNodeID_t *targetGlobalRANNodeID;
    NGAP_GlobalGNB_ID_t *targetGlobalGNB_ID;
    NGAP_TAI_t *targetSelectedTAI;
    NGAP_SONInformation_t *sONInformation = NULL;
    NGAP_SONInformationRequest_t *sONInformationRequest = NULL;

    ogs_debug("Uplink Configuration Transfer");

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));
    ogs_assert(pdu.choice.initiatingMessage);

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_UplinkRANConfigurationTransfer;
    initiatingMessage->criticality = NGAP_Criticality_ignore;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_UplinkRANConfigurationTransfer;

    UplinkRANConfigurationTransfer =
        &initiatingMessage->value.choice.UplinkRANConfigurationTransfer;

    ie = CALLOC(1, sizeof(NGAP_UplinkRANConfigurationTransferIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&UplinkRANConfigurationTransfer->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SONConfigurationTransferUL;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_UplinkRANConfigurationTransferIEs__value_PR_SONConfigurationTransfer;

    SONConfigurationTransfer = &ie->value.choice.SONConfigurationTransfer;

    plmn_id = &test_self()->plmn_support[0].plmn_id;

    targetRANNodeID = &SONConfigurationTransfer->targetRANNodeID;
    targetGlobalRANNodeID = &targetRANNodeID->globalRANNodeID;
    targetSelectedTAI = &targetRANNodeID->selectedTAI;

    targetGlobalRANNodeID->present = NGAP_GlobalRANNodeID_PR_globalGNB_ID;
    targetGlobalRANNodeID->choice.globalGNB_ID = targetGlobalGNB_ID =
        CALLOC(1, sizeof(*targetGlobalGNB_ID));
    ogs_assert(targetGlobalGNB_ID);

    ogs_asn_buffer_to_OCTET_STRING(
            plmn_id, OGS_PLMN_ID_LEN, &targetGlobalGNB_ID->pLMNIdentity);
    ogs_ngap_uint32_to_GNB_ID(target_gnb_id, target_bitsize,
            &targetGlobalGNB_ID->gNB_ID);
    ogs_ngap_5gs_tai_to_ASN(&test_self()->nr_tai, targetSelectedTAI);

    sourceRANNodeID = &SONConfigurationTransfer->sourceRANNodeID;
    sourceGlobalRANNodeID = &sourceRANNodeID->globalRANNodeID;
    sourceSelectedTAI = &sourceRANNodeID->selectedTAI;

    sourceGlobalRANNodeID->present = NGAP_GlobalRANNodeID_PR_globalGNB_ID;
    sourceGlobalRANNodeID->choice.globalGNB_ID = sourceGlobalGNB_ID =
        CALLOC(1, sizeof(*sourceGlobalGNB_ID));
    ogs_assert(sourceGlobalGNB_ID);

    ogs_asn_buffer_to_OCTET_STRING(
            plmn_id, OGS_PLMN_ID_LEN, &sourceGlobalGNB_ID->pLMNIdentity);
    ogs_ngap_uint32_to_GNB_ID(source_gnb_id, source_bitsize,
            &sourceGlobalGNB_ID->gNB_ID);
    ogs_ngap_5gs_tai_to_ASN(&test_self()->nr_tai, sourceSelectedTAI);

    sONInformation = &SONConfigurationTransfer->sONInformation;

    sONInformation->present = NGAP_SONInformation_PR_sONInformationRequest;
    sONInformationRequest = &sONInformation->choice.sONInformationRequest;

    *sONInformationRequest =
        NGAP_SONInformationRequest_xn_TNL_configuration_info;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_path_switch_request(test_ue_t *test_ue)
{
    int rv;

    test_sess_t *sess = NULL;

    ogs_pkbuf_t *n2smbuf = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_PathSwitchRequest_t *PathSwitchRequest = NULL;

    NGAP_PathSwitchRequestIEs_t *ie = NULL;

    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_UserLocationInformation_t *UserLocationInformation = NULL;
    NGAP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;
    NGAP_PDUSessionResourceToBeSwitchedDLList_t
        *PDUSessionResourceToBeSwitchedDLList = NULL;

    NGAP_PDUSessionResourceToBeSwitchedDLItem_t *PDUSessionItem = NULL;

    NGAP_UserLocationInformationNR_t *userLocationInformationNR = NULL;
    NGAP_NR_CGI_t *nR_CGI = NULL;
    NGAP_TAI_t *tAI = NULL;

    OCTET_STRING_t *transfer = NULL;

    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_PathSwitchRequest;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_PathSwitchRequest;

    PathSwitchRequest =
        &initiatingMessage->value.choice.PathSwitchRequest;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PathSwitchRequestIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SourceAMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_PathSwitchRequestIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UserLocationInformation;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PathSwitchRequestIEs__value_PR_UserLocationInformation;

    UserLocationInformation = &ie->value.choice.UserLocationInformation;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_PathSwitchRequestIEs__value_PR_UESecurityCapabilities;

    UESecurityCapabilities = &ie->value.choice.UESecurityCapabilities;

    ie = CALLOC(1, sizeof(NGAP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceToBeSwitchedDLList;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_PathSwitchRequestIEs__value_PR_PDUSessionResourceToBeSwitchedDLList;

    PDUSessionResourceToBeSwitchedDLList =
        &ie->value.choice.PDUSessionResourceToBeSwitchedDLList;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    userLocationInformationNR =
            CALLOC(1, sizeof(NGAP_UserLocationInformationNR_t));

    nR_CGI = &userLocationInformationNR->nR_CGI;
    ogs_ngap_nr_cgi_to_ASN(&test_ue->nr_cgi, nR_CGI);

    tAI = &userLocationInformationNR->tAI;
    ogs_ngap_5gs_tai_to_ASN(&test_ue->nr_tai, tAI);

    UserLocationInformation->present =
        NGAP_UserLocationInformation_PR_userLocationInformationNR;
    UserLocationInformation->choice.userLocationInformationNR =
        userLocationInformationNR;

    UESecurityCapabilities->nRencryptionAlgorithms.size = 2;
    UESecurityCapabilities->nRencryptionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    nRencryptionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->nRencryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->nRencryptionAlgorithms.buf[0] =
        (test_ue->ue_security_capability.nr_ea << 1);

    UESecurityCapabilities->nRintegrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->nRintegrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    nRintegrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->nRintegrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->nRintegrityProtectionAlgorithms.buf[0] =
        (test_ue->ue_security_capability.nr_ia << 1);

    UESecurityCapabilities->eUTRAencryptionAlgorithms.size = 2;
    UESecurityCapabilities->eUTRAencryptionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    eUTRAencryptionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->eUTRAencryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->eUTRAencryptionAlgorithms.buf[0] =
        (test_ue->ue_security_capability.eutra_ea << 1);

    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                    eUTRAintegrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->eUTRAintegrityProtectionAlgorithms.buf[0] =
        (test_ue->ue_security_capability.eutra_ia << 1);

    ogs_list_for_each(&test_ue->sess_list, sess) {
        PDUSessionItem =
            CALLOC(1, sizeof(NGAP_PDUSessionResourceToBeSwitchedDLItem_t));
        ASN_SEQUENCE_ADD(
            &PDUSessionResourceToBeSwitchedDLList->list, PDUSessionItem);

        PDUSessionItem->pDUSessionID = sess->psi;

        n2smbuf = testngap_build_path_switch_request_trasfer(sess);
        ogs_assert(n2smbuf);
        transfer = &PDUSessionItem->pathSwitchRequestTransfer;

        transfer->size = n2smbuf->len;
        transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
        memcpy(transfer->buf, n2smbuf->data, transfer->size);
        ogs_pkbuf_free(n2smbuf);
    }

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_handover_required(
        test_ue_t *test_ue, NGAP_HandoverType_t handover_type,
        uint32_t gnb_id, uint8_t bitsize,
        NGAP_Cause_PR group, long cause,
        bool direct)
{
    test_sess_t *sess = NULL;

    ogs_pkbuf_t *n2smbuf = NULL;

    ogs_plmn_id_t *plmn_id = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_HandoverRequired_t *HandoverRequired = NULL;

    NGAP_HandoverRequiredIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_HandoverType_t *HandoverType = NULL;
    NGAP_Cause_t *Cause = NULL;
    NGAP_TargetID_t *TargetID = NULL;
    NGAP_TargetRANNodeID_t *targetRANNodeID = NULL;
    NGAP_GlobalRANNodeID_t *globalRANNodeID = NULL;
    NGAP_GlobalGNB_ID_t *globalGNB_ID = NULL;
    NGAP_TAI_t *selectedTAI = NULL;

    NGAP_PDUSessionResourceListHORqd_t *PDUSessionList = NULL;
    NGAP_PDUSessionResourceItemHORqd_t *PDUSessionItem = NULL;
    OCTET_STRING_t *transfer = NULL;

    NGAP_SourceToTarget_TransparentContainer_t
        *SourceToTarget_TransparentContainer = NULL;

    uint8_t tmp[OGS_MAX_SDU_LEN];
    char *_container =
        "40"
        "0300001100000a00 010002f839000102 0120000002f83900 0000001000000a";

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));
    ogs_assert(pdu.choice.initiatingMessage);

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_HandoverPreparation;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_HandoverRequired;

    HandoverRequired = &initiatingMessage->value.choice.HandoverRequired;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequiredIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverRequired->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequiredIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequiredIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverRequired->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequiredIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequiredIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequired->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_HandoverType;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequiredIEs__value_PR_HandoverType;

    HandoverType = &ie->value.choice.HandoverType;

    *HandoverType = handover_type;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequiredIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverRequired->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_HandoverRequiredIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequiredIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverRequired->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_TargetID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequiredIEs__value_PR_TargetID;

    TargetID = &ie->value.choice.TargetID;

    TargetID->present = NGAP_TargetID_PR_targetRANNodeID;
    TargetID->choice.targetRANNodeID = targetRANNodeID =
        CALLOC(1, sizeof(*targetRANNodeID));
    ogs_assert(targetRANNodeID);

    globalRANNodeID = &targetRANNodeID->globalRANNodeID;
    globalRANNodeID->present = NGAP_GlobalRANNodeID_PR_globalGNB_ID;
    globalRANNodeID->choice.globalGNB_ID = globalGNB_ID =
        CALLOC(1, sizeof(*globalGNB_ID));
    ogs_assert(globalGNB_ID);

    plmn_id = &test_self()->plmn_support[0].plmn_id;
    ogs_asn_buffer_to_OCTET_STRING(
            plmn_id, OGS_PLMN_ID_LEN, &globalGNB_ID->pLMNIdentity);

    ogs_ngap_uint32_to_GNB_ID(gnb_id, bitsize, &globalGNB_ID->gNB_ID);

    selectedTAI = &targetRANNodeID->selectedTAI;
    ogs_ngap_5gs_tai_to_ASN(&test_ue->nr_tai, selectedTAI);

    ogs_list_for_each(&test_ue->sess_list, sess) {
        if (!PDUSessionList) {
            ie = CALLOC(1, sizeof(NGAP_HandoverRequiredIEs_t));
            ogs_assert(ie);
            ASN_SEQUENCE_ADD(&HandoverRequired->protocolIEs, ie);

            ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceListHORqd;
            ie->criticality = NGAP_Criticality_reject;
            ie->value.present = NGAP_HandoverRequiredIEs__value_PR_PDUSessionResourceListHORqd;

            PDUSessionList = &ie->value.choice.PDUSessionResourceListHORqd;
        }

        PDUSessionItem = CALLOC(1, sizeof(*PDUSessionItem));
        ogs_assert(PDUSessionItem);
        ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

        PDUSessionItem->pDUSessionID = sess->psi;

        n2smbuf = testngap_build_handover_required_transfer(sess, direct);
        ogs_assert(n2smbuf);
        transfer = &PDUSessionItem->handoverRequiredTransfer;

        transfer->size = n2smbuf->len;
        transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
        memcpy(transfer->buf, n2smbuf->data, transfer->size);
        ogs_pkbuf_free(n2smbuf);
    }

    ie = CALLOC(1, sizeof(NGAP_HandoverRequiredIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverRequired->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_SourceToTarget_TransparentContainer;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequiredIEs__value_PR_SourceToTarget_TransparentContainer;

    SourceToTarget_TransparentContainer =
        &ie->value.choice.SourceToTarget_TransparentContainer;

    OGS_HEX(_container, strlen(_container), tmp),

    SourceToTarget_TransparentContainer->size = 32;
    SourceToTarget_TransparentContainer->buf =
        CALLOC(SourceToTarget_TransparentContainer->size, sizeof(uint8_t));
    memcpy(SourceToTarget_TransparentContainer->buf,
            tmp, SourceToTarget_TransparentContainer->size);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_handover_request_ack(test_ue_t *test_ue)
{
    int rv;
    test_sess_t *sess = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_HandoverRequestAcknowledge_t *HandoverRequestAcknowledge = NULL;

    NGAP_HandoverRequestAcknowledgeIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_PDUSessionResourceAdmittedList_t *PDUSessionList = NULL;
    NGAP_TargetToSource_TransparentContainer_t
        *TargetToSource_TransparentContainer = NULL;

    uint8_t tmp[OGS_MAX_SDU_LEN];
    char *_container =
        "00010000";

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode =
        NGAP_ProcedureCode_id_HandoverResourceAllocation;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_HandoverRequestAcknowledge;

    HandoverRequestAcknowledge =
        &successfulOutcome->value.choice.HandoverRequestAcknowledge;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestAcknowledgeIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_HandoverRequestAcknowledgeIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestAcknowledgeIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_HandoverRequestAcknowledgeIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);

    test_ue->ran_ue_ngap_id++;
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    ogs_list_for_each(&test_ue->sess_list, sess) {
        NGAP_PDUSessionResourceAdmittedItem_t *PDUSessionItem = NULL;
        OCTET_STRING_t *transfer = NULL;
        ogs_pkbuf_t *n2smbuf = NULL;

        if (!PDUSessionList) {
            ie = CALLOC(1, sizeof(NGAP_HandoverRequestAcknowledgeIEs_t));
            ogs_assert(ie);
            ASN_SEQUENCE_ADD(&HandoverRequestAcknowledge->protocolIEs, ie);

            ie->id = NGAP_ProtocolIE_ID_id_PDUSessionResourceAdmittedList;
            ie->criticality = NGAP_Criticality_ignore;
            ie->value.present = NGAP_HandoverRequestAcknowledgeIEs__value_PR_PDUSessionResourceAdmittedList;

            PDUSessionList = &ie->value.choice.PDUSessionResourceAdmittedList;
        }

        PDUSessionItem = CALLOC(1, sizeof(*PDUSessionItem));
        ogs_assert(PDUSessionItem);
        ASN_SEQUENCE_ADD(&PDUSessionList->list, PDUSessionItem);

        PDUSessionItem->pDUSessionID = sess->psi;

        n2smbuf = testngap_build_handover_request_ack_transfer(sess);
        ogs_assert(n2smbuf);
        transfer = &PDUSessionItem->handoverRequestAcknowledgeTransfer;

        transfer->size = n2smbuf->len;
        transfer->buf = CALLOC(transfer->size, sizeof(uint8_t));
        memcpy(transfer->buf, n2smbuf->data, transfer->size);
        ogs_pkbuf_free(n2smbuf);
    }

    ie = CALLOC(1, sizeof(NGAP_HandoverRequestAcknowledgeIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_TargetToSource_TransparentContainer;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverRequestAcknowledgeIEs__value_PR_TargetToSource_TransparentContainer;

    TargetToSource_TransparentContainer =
        &ie->value.choice.TargetToSource_TransparentContainer;

    OGS_HEX(_container, strlen(_container), tmp),

    TargetToSource_TransparentContainer->size = 4;
    TargetToSource_TransparentContainer->buf =
        CALLOC(TargetToSource_TransparentContainer->size, sizeof(uint8_t));
    memcpy(TargetToSource_TransparentContainer->buf,
            tmp, TargetToSource_TransparentContainer->size);

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_uplink_ran_status_transfer(test_ue_t *test_ue)
{
    test_sess_t *sess = NULL;

    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_UplinkRANStatusTransfer_t *UplinkRANStatusTransfer = NULL;

    NGAP_UplinkRANStatusTransferIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_RANStatusTransfer_TransparentContainer_t
        *RANStatusTransfer_TransparentContainer = NULL;
    NGAP_DRBsSubjectToStatusTransferList_t *StatusTransferList = NULL;

    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_UplinkRANStatusTransfer;
    initiatingMessage->criticality = NGAP_Criticality_ignore;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_UplinkRANStatusTransfer;

    UplinkRANStatusTransfer = &initiatingMessage->value.choice.UplinkRANStatusTransfer;

    ie = CALLOC(1, sizeof(NGAP_UplinkRANStatusTransferIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&UplinkRANStatusTransfer->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_UplinkRANStatusTransferIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_UplinkRANStatusTransferIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&UplinkRANStatusTransfer->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present =
        NGAP_UplinkRANStatusTransferIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    ie = CALLOC(1, sizeof(NGAP_UplinkRANStatusTransferIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&UplinkRANStatusTransfer->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RANStatusTransfer_TransparentContainer;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_UplinkRANStatusTransferIEs__value_PR_RANStatusTransfer_TransparentContainer;

    RANStatusTransfer_TransparentContainer =
        &ie->value.choice.RANStatusTransfer_TransparentContainer;

    StatusTransferList = &RANStatusTransfer_TransparentContainer->dRBsSubjectToStatusTransferList;

    ogs_list_for_each(&test_ue->sess_list, sess) {
        NGAP_DRBsSubjectToStatusTransferItem_t *StatusTransferItem = NULL;
        NGAP_DRB_ID_t *dRB_ID = NULL;
        NGAP_DRBStatusUL_t *dRBStatusUL = NULL;
        NGAP_DRBStatusUL12_t *dRBStatusUL12 = NULL;
        NGAP_COUNTValueForPDCP_SN12_t *uL_COUNTValue = NULL;
        NGAP_DRBStatusDL_t *dRBStatusDL = NULL;
        NGAP_DRBStatusDL18_t *dRBStatusDL18 = NULL;
        NGAP_COUNTValueForPDCP_SN18_t *dL_COUNTValue = NULL;

        StatusTransferItem = CALLOC(1, sizeof(*StatusTransferItem));
        ogs_assert(StatusTransferItem);
        ASN_SEQUENCE_ADD(&StatusTransferList->list, StatusTransferItem);

        dRB_ID = &StatusTransferItem->dRB_ID;

        *dRB_ID = sess->psi;

        dRBStatusUL = &StatusTransferItem->dRBStatusUL;
        dRBStatusUL->present = NGAP_DRBStatusUL_PR_dRBStatusUL12;
        dRBStatusUL->choice.dRBStatusUL12 = dRBStatusUL12 =
            CALLOC(1, sizeof(*dRBStatusUL12));
        uL_COUNTValue = &dRBStatusUL12->uL_COUNTValue;
        uL_COUNTValue->pDCP_SN12 = 1;
        uL_COUNTValue->hFN_PDCP_SN12 = 2;

        dRBStatusDL = &StatusTransferItem->dRBStatusDL;
        dRBStatusDL->present = NGAP_DRBStatusDL_PR_dRBStatusDL18;
        dRBStatusDL->choice.dRBStatusDL18 = dRBStatusDL18 =
            CALLOC(1, sizeof(*dRBStatusDL18));
        dL_COUNTValue = &dRBStatusDL18->dL_COUNTValue;
        dL_COUNTValue->pDCP_SN18 = 3;
        dL_COUNTValue->hFN_PDCP_SN18 = 4;
    }

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_handover_notify(test_ue_t *test_ue)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_HandoverNotify_t *HandoverNotify = NULL;

    NGAP_HandoverNotifyIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_UserLocationInformation_t *UserLocationInformation = NULL;
    NGAP_UserLocationInformationNR_t *userLocationInformationNR = NULL;
    NGAP_NR_CGI_t *nR_CGI = NULL;
    NGAP_TAI_t *tAI = NULL;

    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        NGAP_ProcedureCode_id_HandoverNotification;
    initiatingMessage->criticality = NGAP_Criticality_ignore;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_HandoverNotify;

    HandoverNotify = &initiatingMessage->value.choice.HandoverNotify;

    ie = CALLOC(1, sizeof(NGAP_HandoverNotifyIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverNotify->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverNotifyIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverNotifyIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverNotify->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverNotifyIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    ie = CALLOC(1, sizeof(NGAP_HandoverNotifyIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverNotify->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UserLocationInformation;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present =
        NGAP_HandoverNotifyIEs__value_PR_UserLocationInformation;

    UserLocationInformation = &ie->value.choice.UserLocationInformation;

    userLocationInformationNR =
            CALLOC(1, sizeof(NGAP_UserLocationInformationNR_t));
    ogs_assert(userLocationInformationNR);

    nR_CGI = &userLocationInformationNR->nR_CGI;
    ogs_ngap_nr_cgi_to_ASN(&test_ue->nr_cgi, nR_CGI);

    tAI = &userLocationInformationNR->tAI;
    ogs_ngap_5gs_tai_to_ASN(&test_ue->nr_tai, tAI);

    UserLocationInformation->present =
        NGAP_UserLocationInformation_PR_userLocationInformationNR;
    UserLocationInformation->choice.userLocationInformationNR =
        userLocationInformationNR;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_handover_failure(test_ue_t *test_ue,
        NGAP_Cause_PR group, long cause)
{
    int rv;

    NGAP_NGAP_PDU_t pdu;
    NGAP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    NGAP_HandoverFailure_t *HandoverFailure = NULL;

    NGAP_HandoverFailureIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_unsuccessfulOutcome;
    pdu.choice.unsuccessfulOutcome =
        CALLOC(1, sizeof(NGAP_UnsuccessfulOutcome_t));

    unsuccessfulOutcome = pdu.choice.unsuccessfulOutcome;
    unsuccessfulOutcome->procedureCode =
        NGAP_ProcedureCode_id_HandoverResourceAllocation;
    unsuccessfulOutcome->criticality = NGAP_Criticality_reject;
    unsuccessfulOutcome->value.present =
        NGAP_UnsuccessfulOutcome__value_PR_HandoverFailure;

    HandoverFailure = &unsuccessfulOutcome->value.choice.HandoverFailure;

    ie = CALLOC(1, sizeof(NGAP_HandoverFailureIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_HandoverFailureIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverFailureIEs_t));
    ASN_SEQUENCE_ADD(&HandoverFailure->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_HandoverFailureIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    return ogs_ngap_encode(&pdu);
}

ogs_pkbuf_t *testngap_build_handover_cancel(test_ue_t *test_ue,
        NGAP_Cause_PR group, long cause)
{
    NGAP_NGAP_PDU_t pdu;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_HandoverCancel_t *HandoverCancel = NULL;

    NGAP_HandoverCancelIEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_Cause_t *Cause = NULL;

    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = NGAP_ProcedureCode_id_HandoverCancel;
    initiatingMessage->criticality = NGAP_Criticality_reject;
    initiatingMessage->value.present =
        NGAP_InitiatingMessage__value_PR_HandoverCancel;

    HandoverCancel = &initiatingMessage->value.choice.HandoverCancel;

    ie = CALLOC(1, sizeof(NGAP_HandoverCancelIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverCancel->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverCancelIEs__value_PR_AMF_UE_NGAP_ID;

    AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;

    ie = CALLOC(1, sizeof(NGAP_HandoverCancelIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverCancel->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_HandoverCancelIEs__value_PR_RAN_UE_NGAP_ID;

    RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;

    asn_uint642INTEGER(AMF_UE_NGAP_ID, test_ue->amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = test_ue->ran_ue_ngap_id;

    ie = CALLOC(1, sizeof(NGAP_HandoverCancelIEs_t));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(&HandoverCancel->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_Cause;
    ie->criticality = NGAP_Criticality_ignore;
    ie->value.present = NGAP_HandoverCancelIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    return ogs_ngap_encode(&pdu);
}

static ogs_pkbuf_t *testngap_build_pdu_session_resource_setup_response_trasfer(
        test_sess_t *sess)
{
    int rv;
    test_bearer_t *qos_flow = NULL;

    ogs_gtp2_f_teid_t f_teid;
    ogs_ip_t ip;
    int len;

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
    rv = ogs_gtp2_sockaddr_to_f_teid(
            sess->gnb_n3_addr, sess->gnb_n3_addr6, &f_teid, &len);
    ogs_assert(rv == OGS_OK);

    rv = ogs_gtp2_f_teid_to_ip(&f_teid, &ip);
    ogs_assert(rv == OGS_OK);

    ogs_asn_ip_to_BIT_STRING(&ip, &gTPTunnel->transportLayerAddress);
    ogs_asn_uint32_to_OCTET_STRING(sess->gnb_n3_teid, &gTPTunnel->gTP_TEID);

    associatedQosFlowList = &dLQosFlowPerTNLInformation->associatedQosFlowList;

    ogs_list_for_each(&sess->bearer_list, qos_flow) {
        associatedQosFlowItem =
            CALLOC(1, sizeof(struct NGAP_AssociatedQosFlowItem));
        ASN_SEQUENCE_ADD(&associatedQosFlowList->list, associatedQosFlowItem);

        associatedQosFlowItem->qosFlowIdentifier = qos_flow->qfi;
    }

    return ogs_asn_encode(
            &asn_DEF_NGAP_PDUSessionResourceSetupResponseTransfer, &message);
}

static ogs_pkbuf_t *testngap_build_pdu_session_resource_setup_unsuccessful_trasfer(
        test_sess_t *sess, NGAP_Cause_PR group, long cause)
{
    int rv;

    ogs_assert(sess);

    NGAP_PDUSessionResourceSetupUnsuccessfulTransfer_t message;
    NGAP_Cause_t *Cause = NULL;

    memset(&message, 0, sizeof(message));

    Cause = &message.cause;

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    return ogs_asn_encode(
        &asn_DEF_NGAP_PDUSessionResourceSetupUnsuccessfulTransfer, &message);
}

static ogs_pkbuf_t *testngap_build_pdu_session_resource_modify_response_trasfer(
        test_sess_t *sess)
{
    NGAP_PDUSessionResourceModifyResponseTransfer_t message;

    NGAP_QosFlowAddOrModifyResponseList_t *qosFlowAddOrModifyResponseList;
    NGAP_QosFlowAddOrModifyResponseItem_t *qosFlowAddOrModifyResponseItem;

    test_bearer_t *qos_flow = NULL;

    ogs_assert(sess);

    memset(&message, 0, sizeof(message));

    message.qosFlowAddOrModifyResponseList =
        qosFlowAddOrModifyResponseList =
            CALLOC(1, sizeof(struct NGAP_QosFlowAddOrModifyResponseList));

    ogs_list_for_each_entry(
                &sess->qos_flow_to_modify_list, qos_flow, to_modify_node) {

        qosFlowAddOrModifyResponseItem =
            CALLOC(1, sizeof(struct NGAP_QosFlowAddOrModifyResponseItem));
        ASN_SEQUENCE_ADD(&qosFlowAddOrModifyResponseList->list,
                qosFlowAddOrModifyResponseItem);

        qosFlowAddOrModifyResponseItem->qosFlowIdentifier = qos_flow->qfi;
    }

    return ogs_asn_encode(
            &asn_DEF_NGAP_PDUSessionResourceModifyResponseTransfer, &message);
}

static ogs_pkbuf_t *testngap_build_qos_flow_resource_release_response_trasfer(
        test_bearer_t *qos_flow)
{
    ogs_assert(qos_flow);

    NGAP_PDUSessionResourceModifyResponseTransfer_t message;
    memset(&message, 0, sizeof(message));

    return ogs_asn_encode(
            &asn_DEF_NGAP_PDUSessionResourceModifyResponseTransfer, &message);
}

static ogs_pkbuf_t *testngap_build_path_switch_request_trasfer(
        test_sess_t *sess)
{
    int rv;

    test_bearer_t *qos_flow = NULL;

    ogs_gtp2_f_teid_t f_teid;
    ogs_ip_t ip;
    int len;

    ogs_assert(sess);

    NGAP_PathSwitchRequestTransfer_t message;

    NGAP_UPTransportLayerInformation_t *dL_NGU_UP_TNLInformation = NULL;
    NGAP_GTPTunnel_t *gTPTunnel = NULL;

    NGAP_QosFlowAcceptedList_t *qosFlowAcceptedList = NULL;
    NGAP_QosFlowAcceptedItem_t *qosFlowAcceptedItem = NULL;

    memset(&message, 0, sizeof(message));

    dL_NGU_UP_TNLInformation = &message.dL_NGU_UP_TNLInformation;

    gTPTunnel = CALLOC(1, sizeof(struct NGAP_GTPTunnel));
    dL_NGU_UP_TNLInformation->present =
        NGAP_UPTransportLayerInformation_PR_gTPTunnel;
    dL_NGU_UP_TNLInformation->choice.gTPTunnel = gTPTunnel;

    ogs_assert(sess->gnb_n3_addr || sess->gnb_n3_addr6);
    rv = ogs_gtp2_sockaddr_to_f_teid(
            sess->gnb_n3_addr, sess->gnb_n3_addr6, &f_teid, &len);
    ogs_assert(rv == OGS_OK);

    rv = ogs_gtp2_f_teid_to_ip(&f_teid, &ip);
    ogs_assert(rv == OGS_OK);

    ogs_asn_ip_to_BIT_STRING(&ip, &gTPTunnel->transportLayerAddress);
    ogs_asn_uint32_to_OCTET_STRING(sess->gnb_n3_teid, &gTPTunnel->gTP_TEID);

    qosFlowAcceptedList = &message.qosFlowAcceptedList;

    ogs_list_for_each(&sess->bearer_list, qos_flow) {
        qosFlowAcceptedItem =
            CALLOC(1, sizeof(NGAP_QosFlowAcceptedItem_t));
        ASN_SEQUENCE_ADD(&qosFlowAcceptedList->list, qosFlowAcceptedItem);

        qosFlowAcceptedItem->qosFlowIdentifier = qos_flow->qfi;
    }

    return ogs_asn_encode(&asn_DEF_NGAP_PathSwitchRequestTransfer, &message);
}

static ogs_pkbuf_t *testngap_build_handover_required_transfer(
        test_sess_t *sess, bool direct)
{
    int rv;

    test_bearer_t *qos_flow = NULL;

    ogs_gtp2_f_teid_t f_teid;
    ogs_ip_t ip;
    int len;

    ogs_assert(sess);

    NGAP_HandoverRequiredTransfer_t message;

    NGAP_DirectForwardingPathAvailability_t
        *directForwardingPathAvailability = NULL;

    memset(&message, 0, sizeof(message));

    if (direct == true) {
        message.directForwardingPathAvailability =
            directForwardingPathAvailability =
                CALLOC(1, sizeof(*directForwardingPathAvailability));

        *directForwardingPathAvailability =
            NGAP_DirectForwardingPathAvailability_direct_path_available;
    }

    return ogs_asn_encode(&asn_DEF_NGAP_HandoverRequiredTransfer, &message);
}

static ogs_pkbuf_t *testngap_build_handover_request_ack_transfer(
        test_sess_t *sess)
{
    int rv;
    test_bearer_t *qos_flow = NULL;

    ogs_gtp2_f_teid_t f_teid;
    ogs_ip_t ip;
    int len;

    ogs_assert(sess);

    NGAP_HandoverRequestAcknowledgeTransfer_t message;

    NGAP_UPTransportLayerInformation_t *dL_NGU_UP_TNLInformation = NULL;
    NGAP_UPTransportLayerInformation_t *dLForwardingUP_TNLInformation = NULL;
    NGAP_QosFlowListWithDataForwarding_t *qosFlowSetupResponseList = NULL;
    NGAP_QosFlowItemWithDataForwarding_t *qosFlowSetupResponseItem = NULL;
    NGAP_GTPTunnel_t *gTPTunnel = NULL;

    memset(&message, 0, sizeof(message));

    dL_NGU_UP_TNLInformation = &message.dL_NGU_UP_TNLInformation;

    dL_NGU_UP_TNLInformation->present =
        NGAP_UPTransportLayerInformation_PR_gTPTunnel;
    dL_NGU_UP_TNLInformation->choice.gTPTunnel = gTPTunnel =
        CALLOC(1, sizeof(*gTPTunnel));
    ogs_assert(gTPTunnel);

    ogs_assert(sess->gnb_n3_addr || sess->gnb_n3_addr6);
    rv = ogs_gtp2_sockaddr_to_f_teid(
            sess->gnb_n3_addr, sess->gnb_n3_addr6, &f_teid, &len);
    ogs_assert(rv == OGS_OK);

    rv = ogs_gtp2_f_teid_to_ip(&f_teid, &ip);
    ogs_assert(rv == OGS_OK);

    ogs_asn_ip_to_BIT_STRING(&ip, &gTPTunnel->transportLayerAddress);
    ogs_asn_uint32_to_OCTET_STRING(sess->gnb_n3_teid, &gTPTunnel->gTP_TEID);

    if (sess->handover.data_forwarding_not_possible) {
        NGAP_GTPTunnel_t *gTPTunnelForDLForwarding = NULL;
        message.dLForwardingUP_TNLInformation = dLForwardingUP_TNLInformation =
            CALLOC(1, sizeof(*dLForwardingUP_TNLInformation));
        ogs_assert(dLForwardingUP_TNLInformation);

        dLForwardingUP_TNLInformation->present =
            NGAP_UPTransportLayerInformation_PR_gTPTunnel;
        dLForwardingUP_TNLInformation->choice.gTPTunnel =
            gTPTunnelForDLForwarding =
                CALLOC(1, sizeof(*gTPTunnelForDLForwarding));
        ogs_assert(gTPTunnelForDLForwarding);

        ogs_asn_ip_to_BIT_STRING(&ip,
                &gTPTunnelForDLForwarding->transportLayerAddress);
        ogs_asn_uint32_to_OCTET_STRING(sess->gnb_n3_teid+10,
                &gTPTunnelForDLForwarding->gTP_TEID);
    }

    qosFlowSetupResponseList = &message.qosFlowSetupResponseList;

    ogs_list_for_each(&sess->bearer_list, qos_flow) {
        qosFlowSetupResponseItem =
            CALLOC(1, sizeof(struct NGAP_QosFlowItemWithDataForwarding));
        ogs_assert(qosFlowSetupResponseItem);
        ASN_SEQUENCE_ADD(&qosFlowSetupResponseList->list,
                qosFlowSetupResponseItem);

        qosFlowSetupResponseItem->qosFlowIdentifier = qos_flow->qfi;
    }

    return ogs_asn_encode(
            &asn_DEF_NGAP_HandoverRequestAcknowledgeTransfer, &message);
}
