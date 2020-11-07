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

ogs_pkbuf_t *test_s1ap_build_s1_setup_request(
        S1AP_ENB_ID_PR present, uint32_t enb_id)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_S1SetupRequest_t *S1SetupRequest = NULL;

    S1AP_S1SetupRequestIEs_t *ie = NULL;
    S1AP_Global_ENB_ID_t *Global_ENB_ID = NULL;
    S1AP_SupportedTAs_t *SupportedTAs = NULL;
    S1AP_SupportedTAs_Item_t *SupportedTAs_Item = NULL;
    S1AP_PLMNidentity_t *PLMNidentity = NULL;
    S1AP_PagingDRX_t *PagingDRX = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_S1Setup;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_S1SetupRequest;

    S1SetupRequest = &initiatingMessage->value.choice.S1SetupRequest;

    ie = CALLOC(1, sizeof(S1AP_S1SetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&S1SetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Global_ENB_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_S1SetupRequestIEs__value_PR_Global_ENB_ID;

    Global_ENB_ID = &ie->value.choice.Global_ENB_ID;

    ie = CALLOC(1, sizeof(S1AP_S1SetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&S1SetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_SupportedTAs;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_S1SetupRequestIEs__value_PR_SupportedTAs;

    SupportedTAs = &ie->value.choice.SupportedTAs;

    ie = CALLOC(1, sizeof(S1AP_S1SetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&S1SetupRequest->protocolIEs, ie);
    
    ie->id = S1AP_ProtocolIE_ID_id_DefaultPagingDRX;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_S1SetupRequestIEs__value_PR_PagingDRX;

    PagingDRX = &ie->value.choice.PagingDRX;

    ogs_s1ap_uint32_to_ENB_ID(present, enb_id, &Global_ENB_ID->eNB_ID);
    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_self()->e_tai.plmn_id, OGS_PLMN_ID_LEN,
            &Global_ENB_ID->pLMNidentity);

    SupportedTAs_Item = (S1AP_SupportedTAs_Item_t *)
        CALLOC(1, sizeof(S1AP_SupportedTAs_Item_t));
    ogs_asn_uint16_to_OCTET_STRING(
            test_self()->e_tai.tac, &SupportedTAs_Item->tAC);
    PLMNidentity = (S1AP_PLMNidentity_t *)
        CALLOC(1, sizeof(S1AP_PLMNidentity_t));
    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_self()->e_tai.plmn_id, OGS_PLMN_ID_LEN, PLMNidentity);
    ASN_SEQUENCE_ADD(&SupportedTAs_Item->broadcastPLMNs.list, PLMNidentity);

    ASN_SEQUENCE_ADD(&SupportedTAs->list, SupportedTAs_Item);

    *PagingDRX = S1AP_PagingDRX_v64;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_initial_ue_message(
        test_ue_t *test_ue, ogs_pkbuf_t *emmbuf, uint8_t cause, bool s_tmsi)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int i, j;
    char buf[5];

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_InitialUEMessage_t *InitialUEMessage = NULL;

    S1AP_InitialUEMessage_IEs_t *ie = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_NAS_PDU_t *NAS_PDU = NULL;
    S1AP_TAI_t *TAI = NULL;
    S1AP_EUTRAN_CGI_t *EUTRAN_CGI = NULL;
    S1AP_RRC_Establishment_Cause_t *RRC_Establishment_Cause = NULL;
    S1AP_GUMMEI_t *GUMMEI = NULL;
    S1AP_GUMMEIType_t *GUMMEIType = NULL;

    ogs_assert(test_ue);
    ogs_assert(emmbuf);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_initialUEMessage;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_InitialUEMessage;

    InitialUEMessage = &initiatingMessage->value.choice.InitialUEMessage;

    ie = CALLOC(1, sizeof(S1AP_InitialUEMessage_IEs_t));
    ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_InitialUEMessage_IEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    test_ue->enb_ue_s1ap_id++;
    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_InitialUEMessage_IEs_t));
    ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_NAS_PDU;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_InitialUEMessage_IEs__value_PR_NAS_PDU;

    NAS_PDU = &ie->value.choice.NAS_PDU;

    NAS_PDU->size = emmbuf->len;
    NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
    memcpy(NAS_PDU->buf, emmbuf->data, NAS_PDU->size);
    ogs_pkbuf_free(emmbuf);

    ie = CALLOC(1, sizeof(S1AP_InitialUEMessage_IEs_t));
    ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_TAI;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_InitialUEMessage_IEs__value_PR_TAI;

    TAI = &ie->value.choice.TAI;

    ogs_asn_uint16_to_OCTET_STRING(test_ue->e_tai.tac, &TAI->tAC);
    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_ue->e_tai.plmn_id, OGS_PLMN_ID_LEN, &TAI->pLMNidentity);

    ie = CALLOC(1, sizeof(S1AP_InitialUEMessage_IEs_t));
    ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_EUTRAN_CGI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_InitialUEMessage_IEs__value_PR_EUTRAN_CGI;

    EUTRAN_CGI = &ie->value.choice.EUTRAN_CGI;

    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_ue->e_cgi.plmn_id,
            OGS_PLMN_ID_LEN, &EUTRAN_CGI->pLMNidentity);
    EUTRAN_CGI->cell_ID.size = 4;
    EUTRAN_CGI->cell_ID.buf =  CALLOC(
         EUTRAN_CGI->cell_ID.size, sizeof(uint8_t));
    ogs_assert(EUTRAN_CGI->cell_ID.buf);
    EUTRAN_CGI->cell_ID.buf[0] = (test_ue->e_cgi.cell_id >> 24);
    EUTRAN_CGI->cell_ID.buf[1] = (test_ue->e_cgi.cell_id >> 16);
    EUTRAN_CGI->cell_ID.buf[2] = (test_ue->e_cgi.cell_id >> 8);
    EUTRAN_CGI->cell_ID.buf[3] = (test_ue->e_cgi.cell_id);
    EUTRAN_CGI->cell_ID.bits_unused = 4;

    ie = CALLOC(1, sizeof(S1AP_InitialUEMessage_IEs_t));
    ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_RRC_Establishment_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialUEMessage_IEs__value_PR_RRC_Establishment_Cause;

    RRC_Establishment_Cause = &ie->value.choice.RRC_Establishment_Cause;

    *RRC_Establishment_Cause = cause;

    if (s_tmsi) {
        S1AP_MME_Code_t *mMEC = NULL;
        S1AP_M_TMSI_t *m_TMSI = NULL;
        S1AP_S_TMSI_t *S_TMSI = NULL;

        ie = CALLOC(1, sizeof(S1AP_InitialUEMessage_IEs_t));
        ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_S_TMSI;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present = S1AP_InitialUEMessage_IEs__value_PR_S_TMSI;

        S_TMSI = &ie->value.choice.S_TMSI;

        mMEC = &S_TMSI->mMEC;
        m_TMSI = &S_TMSI->m_TMSI;

        ogs_asn_uint8_to_OCTET_STRING(test_ue->nas_eps_guti.mme_code, mMEC);
        ogs_asn_uint32_to_OCTET_STRING(test_ue->nas_eps_guti.m_tmsi, m_TMSI);
    }

    if (test_ue->initial_ue_param.gummei_id) {
        ie = CALLOC(1, sizeof(S1AP_InitialUEMessage_IEs_t));
        ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_GUMMEI_ID;
        ie->criticality = S1AP_Criticality_reject;
        ie->value.present = S1AP_InitialUEMessage_IEs__value_PR_GUMMEI;

        GUMMEI = &ie->value.choice.GUMMEI;
        ogs_s1ap_buffer_to_OCTET_STRING(
                &test_ue->e_cgi.plmn_id,
                OGS_PLMN_ID_LEN, &GUMMEI->pLMN_Identity);
        ogs_asn_uint16_to_OCTET_STRING(
                test_ue->initial_ue_param.gummei.mme_gid,
                &GUMMEI->mME_Group_ID);
        ogs_asn_uint8_to_OCTET_STRING(
                test_ue->initial_ue_param.gummei.mme_code,
                &GUMMEI->mME_Code);
    }

    if (test_ue->initial_ue_param.gummei_type) {
        ie = CALLOC(1, sizeof(S1AP_InitialUEMessage_IEs_t));
        ASN_SEQUENCE_ADD(&InitialUEMessage->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_GUMMEIType;
        ie->criticality = S1AP_Criticality_reject;
        ie->value.present = S1AP_InitialUEMessage_IEs__value_PR_GUMMEIType;

        GUMMEIType = &ie->value.choice.GUMMEIType;

        *GUMMEIType = test_ue->initial_ue_param.gummei.type;
    }

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_uplink_nas_transport(
        test_ue_t *test_ue, ogs_pkbuf_t *nasbuf)
{
    char buf[5];

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_UplinkNASTransport_t *UplinkNASTransport = NULL;

    S1AP_UplinkNASTransport_IEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_NAS_PDU_t *NAS_PDU = NULL;
    S1AP_TAI_t *TAI = NULL;
    S1AP_EUTRAN_CGI_t *EUTRAN_CGI = NULL;
    S1AP_PSCellInformation_t *PSCellInformation = NULL;
    S1AP_NR_CGI_t *nCGI = NULL;

    ogs_assert(test_ue);
    ogs_assert(nasbuf);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_uplinkNASTransport;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_UplinkNASTransport;

    UplinkNASTransport = &initiatingMessage->value.choice.UplinkNASTransport;

    ie = CALLOC(1, sizeof(S1AP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UplinkNASTransport_IEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UplinkNASTransport_IEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_NAS_PDU;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_UplinkNASTransport_IEs__value_PR_NAS_PDU;

    NAS_PDU = &ie->value.choice.NAS_PDU;

    NAS_PDU->size = nasbuf->len;
    NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
    memcpy(NAS_PDU->buf, nasbuf->data, NAS_PDU->size);
    ogs_pkbuf_free(nasbuf);

    ie = CALLOC(1, sizeof(S1AP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_EUTRAN_CGI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_UplinkNASTransport_IEs__value_PR_EUTRAN_CGI;

    EUTRAN_CGI = &ie->value.choice.EUTRAN_CGI;

    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_ue->e_cgi.plmn_id,
            OGS_PLMN_ID_LEN, &EUTRAN_CGI->pLMNidentity);
    EUTRAN_CGI->cell_ID.size = 4;
    EUTRAN_CGI->cell_ID.buf =  CALLOC(
         EUTRAN_CGI->cell_ID.size, sizeof(uint8_t));
    ogs_assert(EUTRAN_CGI->cell_ID.buf);
    EUTRAN_CGI->cell_ID.buf[0] = (test_ue->e_cgi.cell_id >> 24);
    EUTRAN_CGI->cell_ID.buf[1] = (test_ue->e_cgi.cell_id >> 16);
    EUTRAN_CGI->cell_ID.buf[2] = (test_ue->e_cgi.cell_id >> 8);
    EUTRAN_CGI->cell_ID.buf[3] = (test_ue->e_cgi.cell_id);
    EUTRAN_CGI->cell_ID.bits_unused = 4;

    ie = CALLOC(1, sizeof(S1AP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_TAI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_UplinkNASTransport_IEs__value_PR_TAI;

    TAI = &ie->value.choice.TAI;

    ogs_asn_uint16_to_OCTET_STRING(test_ue->e_tai.tac, &TAI->tAC);
    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_ue->e_tai.plmn_id, OGS_PLMN_ID_LEN, &TAI->pLMNidentity);

    if (test_ue->nr_cgi.cell_id) {
        ie = CALLOC(1, sizeof(S1AP_UplinkNASTransport_IEs_t));
        ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_PSCellInformation;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_UplinkNASTransport_IEs__value_PR_PSCellInformation;

        PSCellInformation = &ie->value.choice.PSCellInformation;
        nCGI = &PSCellInformation->nCGI;
        ogs_s1ap_buffer_to_OCTET_STRING(
                &test_ue->e_cgi.plmn_id, OGS_PLMN_ID_LEN, &nCGI->pLMNIdentity);

        ogs_uint64_to_buffer((test_ue->nr_cgi.cell_id << 4), 5, buf);
        ogs_asn_buffer_to_BIT_STRING(buf, 5, 4, &nCGI->nRCellIdentity);
    }

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *tests1ap_build_ue_radio_capability_info_indication(
        test_ue_t *test_ue)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_UECapabilityInfoIndication_t *UECapabilityInfoIndication =
        NULL;

    S1AP_UECapabilityInfoIndicationIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_UERadioCapability_t *UERadioCapability = NULL;

    uint8_t tmp[OGS_MAX_SDU_LEN];
    char *_capability_captured =
        "013001023c"
        "d98000bc000ff06e c4d00141b82c0000 000007d404000ef0 802000016a030000"
        "880000";

    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_UECapabilityInfoIndication;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_UECapabilityInfoIndication;

    UECapabilityInfoIndication =
        &initiatingMessage->value.choice.UECapabilityInfoIndication;

    ie = CALLOC(1, sizeof(S1AP_UECapabilityInfoIndicationIEs_t));
    ASN_SEQUENCE_ADD(&UECapabilityInfoIndication->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UECapabilityInfoIndicationIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_UECapabilityInfoIndicationIEs_t));
    ASN_SEQUENCE_ADD(&UECapabilityInfoIndication->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UECapabilityInfoIndicationIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_UECapabilityInfoIndicationIEs_t));
    ASN_SEQUENCE_ADD(&UECapabilityInfoIndication->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_UERadioCapability;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_UECapabilityInfoIndicationIEs__value_PR_UERadioCapability;

    UERadioCapability = &ie->value.choice.UERadioCapability;

    OGS_HEX(_capability_captured, strlen(_capability_captured), tmp),

    UERadioCapability->size = 40;
    UERadioCapability->buf = CALLOC(UERadioCapability->size, sizeof(uint8_t));
    memcpy(UERadioCapability->buf, tmp, UERadioCapability->size);

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_initial_context_setup_response(test_ue_t *test_ue)
{
    int rv;

    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    ogs_gtp_f_teid_t f_teid;
    ogs_ip_t ip;
    int len;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_InitialContextSetupResponse_t *InitialContextSetupResponse = NULL;

    S1AP_InitialContextSetupResponseIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABSetupListCtxtSURes_t *E_RABSetupListCtxtSURes = NULL;

    S1AP_E_RABSetupItemCtxtSUResIEs_t *item = NULL;
    S1AP_E_RABSetupItemCtxtSURes_t *e_rab = NULL;

    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(S1AP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_InitialContextSetup;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_InitialContextSetupResponse;

    InitialContextSetupResponse =
        &successfulOutcome->value.choice.InitialContextSetupResponse;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialContextSetupResponseIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialContextSetupResponseIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABSetupListCtxtSURes;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialContextSetupResponseIEs__value_PR_E_RABSetupListCtxtSURes;

    E_RABSetupListCtxtSURes = &ie->value.choice.E_RABSetupListCtxtSURes;

    ogs_list_for_each(&test_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
        item = CALLOC(1, sizeof(S1AP_E_RABSetupItemCtxtSUResIEs_t));
        ASN_SEQUENCE_ADD(&E_RABSetupListCtxtSURes->list, item);

        item->id = S1AP_ProtocolIE_ID_id_E_RABSetupItemCtxtSURes;
        item->criticality = S1AP_Criticality_ignore;
        item->value.present =
            S1AP_E_RABSetupItemCtxtSUResIEs__value_PR_E_RABSetupItemCtxtSURes;

        e_rab = &item->value.choice.E_RABSetupItemCtxtSURes;

        e_rab->e_RAB_ID = bearer->ebi;

        rv = ogs_gtp_sockaddr_to_f_teid(
                bearer->enb_s1u_addr, bearer->enb_s1u_addr6, &f_teid, &len);
        ogs_assert(rv == OGS_OK);

        rv = ogs_gtp_f_teid_to_ip(&f_teid, &ip);
        ogs_assert(rv == OGS_OK);

        rv = ogs_asn_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
        ogs_assert(rv == OGS_OK);
        ogs_asn_uint32_to_OCTET_STRING(bearer->enb_s1u_teid, &e_rab->gTP_TEID);
        }
    }

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_initial_context_setup_failure(test_ue_t *test_ue,
        S1AP_Cause_PR group, long cause)
{
    int rv;

    S1AP_S1AP_PDU_t pdu;
    S1AP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    S1AP_InitialContextSetupFailure_t *InitialContextSetupFailure = NULL;

    S1AP_InitialContextSetupFailureIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_Cause_t *Cause = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_unsuccessfulOutcome;
    pdu.choice.unsuccessfulOutcome =
        CALLOC(1, sizeof(S1AP_UnsuccessfulOutcome_t));

    unsuccessfulOutcome = pdu.choice.unsuccessfulOutcome;
    unsuccessfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_InitialContextSetup;
    unsuccessfulOutcome->criticality = S1AP_Criticality_reject;
    unsuccessfulOutcome->value.present =
        S1AP_UnsuccessfulOutcome__value_PR_InitialContextSetupFailure;

    InitialContextSetupFailure =
        &unsuccessfulOutcome->value.choice.InitialContextSetupFailure;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupFailureIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialContextSetupFailureIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupFailureIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialContextSetupFailureIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupFailureIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_InitialContextSetupFailureIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_ue_context_modification_response(
        test_ue_t *test_ue)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_UEContextModificationResponse_t *UEContextModificationResponse = NULL;

    S1AP_UEContextModificationResponseIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(S1AP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_UEContextModification;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_UEContextModificationResponse;

    UEContextModificationResponse =
        &successfulOutcome->value.choice.UEContextModificationResponse;

    ie = CALLOC(1, sizeof(S1AP_UEContextModificationResponseIEs_t));
    ASN_SEQUENCE_ADD(&UEContextModificationResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_UEContextModificationResponseIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_UEContextModificationResponseIEs_t));
    ASN_SEQUENCE_ADD(&UEContextModificationResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_UEContextModificationResponseIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_ue_context_release_request(test_ue_t *test_ue,
        S1AP_Cause_PR group, long cause)
{
    int rv;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_UEContextReleaseRequest_t *UEContextReleaseRequest = NULL;

    S1AP_UEContextReleaseRequest_IEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_Cause_t *Cause = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_UEContextReleaseRequest;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_UEContextReleaseRequest;

    UEContextReleaseRequest =
        &initiatingMessage->value.choice.UEContextReleaseRequest;

    ie = CALLOC(1, sizeof(S1AP_UEContextReleaseRequest_IEs_t));
    ASN_SEQUENCE_ADD(&UEContextReleaseRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UEContextReleaseRequest_IEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_UEContextReleaseRequest_IEs_t));
    ASN_SEQUENCE_ADD(&UEContextReleaseRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UEContextReleaseRequest_IEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_UEContextReleaseRequest_IEs_t));
    ASN_SEQUENCE_ADD(&UEContextReleaseRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_UEContextReleaseRequest_IEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_ue_context_release_complete(test_ue_t *test_ue)
{
    int rv;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_UEContextReleaseComplete_t *UEContextReleaseComplete = NULL;

    S1AP_UEContextReleaseComplete_IEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(S1AP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_UEContextRelease;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_UEContextReleaseComplete;

    UEContextReleaseComplete =
        &successfulOutcome->value.choice.UEContextReleaseComplete;

    ie = CALLOC(1, sizeof(S1AP_UEContextReleaseComplete_IEs_t));
    ASN_SEQUENCE_ADD(&UEContextReleaseComplete->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_UEContextReleaseComplete_IEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;
    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_UEContextReleaseComplete_IEs_t));
    ASN_SEQUENCE_ADD(&UEContextReleaseComplete->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_UEContextReleaseComplete_IEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;
    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_e_rab_setup_response(test_bearer_t *bearer)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;

    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_E_RABSetupResponse_t *E_RABSetupResponse = NULL;

    S1AP_E_RABSetupResponseIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABSetupListBearerSURes_t *E_RABSetupListBearerSURes = NULL;

    S1AP_E_RABSetupItemBearerSUResIEs_t *item = NULL;
    S1AP_E_RABSetupItemBearerSURes_t *e_rab = NULL;

    ogs_gtp_f_teid_t f_teid;
    ogs_ip_t ip;
    int len;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(S1AP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_E_RABSetup;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_E_RABSetupResponse;

    E_RABSetupResponse = &successfulOutcome->value.choice.E_RABSetupResponse;

    ie = CALLOC(1, sizeof(S1AP_E_RABSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&E_RABSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABSetupResponseIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_E_RABSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&E_RABSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABSetupResponseIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_E_RABSetupResponseIEs_t));
    ASN_SEQUENCE_ADD(&E_RABSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABSetupListBearerSURes;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_E_RABSetupResponseIEs__value_PR_E_RABSetupListBearerSURes;

    E_RABSetupListBearerSURes = &ie->value.choice.E_RABSetupListBearerSURes;

    item = CALLOC(1, sizeof(S1AP_E_RABSetupItemBearerSUResIEs_t));
    ASN_SEQUENCE_ADD(&E_RABSetupListBearerSURes->list, item);

    item->id = S1AP_ProtocolIE_ID_id_E_RABSetupItemBearerSURes;
    item->criticality = S1AP_Criticality_ignore;
    item->value.present =
        S1AP_E_RABSetupItemBearerSUResIEs__value_PR_E_RABSetupItemBearerSURes;

    e_rab = &item->value.choice.E_RABSetupItemBearerSURes;

    e_rab->e_RAB_ID = bearer->ebi;

    rv = ogs_gtp_sockaddr_to_f_teid(
            bearer->enb_s1u_addr, bearer->enb_s1u_addr6, &f_teid, &len);
    ogs_assert(rv == OGS_OK);

    rv = ogs_gtp_f_teid_to_ip(&f_teid, &ip);
    ogs_assert(rv == OGS_OK);

    rv = ogs_asn_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
    ogs_assert(rv == OGS_OK);
    ogs_asn_uint32_to_OCTET_STRING(bearer->enb_s1u_teid, &e_rab->gTP_TEID);

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_e_rab_modify_response(test_bearer_t *bearer)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;

    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_E_RABModifyResponse_t *E_RABModifyResponse = NULL;

    S1AP_E_RABModifyResponseIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABModifyListBearerModRes_t *E_RABModifyListBearerModRes = NULL;

    S1AP_E_RABModifyItemBearerModResIEs_t *item = NULL;
    S1AP_E_RABModifyItemBearerModRes_t *e_rab = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(S1AP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_E_RABModify;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_E_RABModifyResponse;

    E_RABModifyResponse = &successfulOutcome->value.choice.E_RABModifyResponse;

    ie = CALLOC(1, sizeof(S1AP_E_RABModifyResponseIEs_t));
    ASN_SEQUENCE_ADD(&E_RABModifyResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABModifyResponseIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_E_RABModifyResponseIEs_t));
    ASN_SEQUENCE_ADD(&E_RABModifyResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABModifyResponseIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_E_RABModifyResponseIEs_t));
    ASN_SEQUENCE_ADD(&E_RABModifyResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABModifyListBearerModRes;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_E_RABModifyResponseIEs__value_PR_E_RABModifyListBearerModRes;

    E_RABModifyListBearerModRes = &ie->value.choice.E_RABModifyListBearerModRes;

    item = CALLOC(1, sizeof(S1AP_E_RABModifyItemBearerModResIEs_t));
    ASN_SEQUENCE_ADD(&E_RABModifyListBearerModRes->list, item);

    item->id = S1AP_ProtocolIE_ID_id_E_RABModifyItemBearerModRes;
    item->criticality = S1AP_Criticality_ignore;
    item->value.present = S1AP_E_RABModifyItemBearerModResIEs__value_PR_E_RABModifyItemBearerModRes;

    e_rab = &item->value.choice.E_RABModifyItemBearerModRes;

    e_rab->e_RAB_ID = bearer->ebi;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_e_rab_release_response(test_bearer_t *bearer)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;

    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_E_RABReleaseResponse_t *E_RABReleaseResponse = NULL;

    S1AP_E_RABReleaseResponseIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABReleaseListBearerRelComp_t *E_RABReleaseListBearerRelComp = NULL;

    S1AP_E_RABReleaseItemBearerRelCompIEs_t *item = NULL;
    S1AP_E_RABReleaseItemBearerRelComp_t *e_rab = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(S1AP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_E_RABRelease;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_E_RABReleaseResponse;

    E_RABReleaseResponse =
        &successfulOutcome->value.choice.E_RABReleaseResponse;

    ie = CALLOC(1, sizeof(S1AP_E_RABReleaseResponseIEs_t));
    ASN_SEQUENCE_ADD(&E_RABReleaseResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABReleaseResponseIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_E_RABReleaseResponseIEs_t));
    ASN_SEQUENCE_ADD(&E_RABReleaseResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABReleaseResponseIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_E_RABReleaseResponseIEs_t));
    ASN_SEQUENCE_ADD(&E_RABReleaseResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABReleaseListBearerRelComp;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_E_RABReleaseResponseIEs__value_PR_E_RABReleaseListBearerRelComp;

    E_RABReleaseListBearerRelComp =
        &ie->value.choice.E_RABReleaseListBearerRelComp;

    item = CALLOC(1, sizeof(S1AP_E_RABReleaseItemBearerRelCompIEs_t));
    ASN_SEQUENCE_ADD(&E_RABReleaseListBearerRelComp->list, item);

    item->id = S1AP_ProtocolIE_ID_id_E_RABReleaseItemBearerRelComp;
    item->criticality = S1AP_Criticality_ignore;
    item->value.present =
        S1AP_E_RABReleaseItemBearerRelCompIEs__value_PR_E_RABReleaseItemBearerRelComp;

    e_rab = &item->value.choice.E_RABReleaseItemBearerRelComp;

    e_rab->e_RAB_ID = bearer->ebi;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_path_switch_request(test_ue_t *test_ue)
{
    int rv;

    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_PathSwitchRequest_t *PathSwitchRequest = NULL;

    S1AP_PathSwitchRequestIEs_t *ie = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABToBeSwitchedDLList_t *E_RABToBeSwitchedDLList = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_EUTRAN_CGI_t *EUTRAN_CGI = NULL;
    S1AP_TAI_t *TAI = NULL;
    S1AP_UESecurityCapabilities_t *UESecurityCapabilities = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_PathSwitchRequest;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_PathSwitchRequest;

    PathSwitchRequest = &initiatingMessage->value.choice.PathSwitchRequest;

    ie = CALLOC(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_PathSwitchRequestIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABToBeSwitchedDLList;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_PathSwitchRequestIEs__value_PR_E_RABToBeSwitchedDLList;

    E_RABToBeSwitchedDLList = &ie->value.choice.E_RABToBeSwitchedDLList;

    ie = CALLOC(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_SourceMME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_PathSwitchRequestIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_EUTRAN_CGI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_PathSwitchRequestIEs__value_PR_EUTRAN_CGI;

    EUTRAN_CGI = &ie->value.choice.EUTRAN_CGI;

    ie = CALLOC(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_TAI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_PathSwitchRequestIEs__value_PR_TAI;

    TAI = &ie->value.choice.TAI;

    ie = CALLOC(1, sizeof(S1AP_PathSwitchRequestIEs_t));
    ASN_SEQUENCE_ADD(&PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_PathSwitchRequestIEs__value_PR_UESecurityCapabilities;

    UESecurityCapabilities = &ie->value.choice.UESecurityCapabilities;

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;
    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ogs_list_for_each(&test_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            S1AP_E_RABToBeSwitchedDLItemIEs_t *item = NULL;
            S1AP_E_RABToBeSwitchedDLItem_t *e_rab = NULL;

            ogs_sockaddr_t *addr = NULL;
            ogs_gtp_f_teid_t f_teid;
            ogs_ip_t ip;
            int len;

            item = CALLOC(1, sizeof(S1AP_E_RABToBeSwitchedDLItemIEs_t));
            ASN_SEQUENCE_ADD(&E_RABToBeSwitchedDLList->list, item);

            item->id = S1AP_ProtocolIE_ID_id_E_RABToBeSwitchedDLItem;
            item->criticality = S1AP_Criticality_reject;
            item->value.present = S1AP_E_RABToBeSwitchedDLItemIEs__value_PR_E_RABToBeSwitchedDLItem;

            e_rab = &item->value.choice.E_RABToBeSwitchedDLItem;

            e_rab->e_RAB_ID = bearer->ebi;

            rv = ogs_gtp_sockaddr_to_f_teid(
                    bearer->enb_s1u_addr, bearer->enb_s1u_addr6, &f_teid, &len);
            ogs_assert(rv == OGS_OK);

            rv = ogs_gtp_f_teid_to_ip(&f_teid, &ip);
            ogs_assert(rv == OGS_OK);

            rv = ogs_asn_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
            ogs_assert(rv == OGS_OK);
            ogs_asn_uint32_to_OCTET_STRING(
                    bearer->enb_s1u_teid, &e_rab->gTP_TEID);
        }
    }

    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_ue->e_cgi.plmn_id,
            OGS_PLMN_ID_LEN, &EUTRAN_CGI->pLMNidentity);
    EUTRAN_CGI->cell_ID.size = 4;
    EUTRAN_CGI->cell_ID.buf =  CALLOC(
         EUTRAN_CGI->cell_ID.size, sizeof(uint8_t));
    ogs_assert(EUTRAN_CGI->cell_ID.buf);
    EUTRAN_CGI->cell_ID.buf[0] = (test_ue->e_cgi.cell_id >> 24);
    EUTRAN_CGI->cell_ID.buf[1] = (test_ue->e_cgi.cell_id >> 16);
    EUTRAN_CGI->cell_ID.buf[2] = (test_ue->e_cgi.cell_id >> 8);
    EUTRAN_CGI->cell_ID.buf[3] = (test_ue->e_cgi.cell_id);
    EUTRAN_CGI->cell_ID.bits_unused = 4;

    ogs_asn_uint16_to_OCTET_STRING(test_ue->e_tai.tac, &TAI->tAC);
    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_ue->e_tai.plmn_id, OGS_PLMN_ID_LEN, &TAI->pLMNidentity);

    UESecurityCapabilities->encryptionAlgorithms.size = 2;
    UESecurityCapabilities->encryptionAlgorithms.buf = 
        CALLOC(UESecurityCapabilities->encryptionAlgorithms.size, 
                    sizeof(uint8_t));
    UESecurityCapabilities->encryptionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->encryptionAlgorithms.buf[0] = (0x70 << 1);

    UESecurityCapabilities->integrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                        integrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->integrityProtectionAlgorithms.bits_unused = 0;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf[0] = (0x70 << 1);

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_handover_required(
        test_ue_t *test_ue, S1AP_HandoverType_t handover_type,
        S1AP_ENB_ID_PR present, uint32_t enb_id,
        S1AP_Cause_PR group, long cause)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_HandoverRequired_t *HandoverRequired = NULL;

    S1AP_HandoverRequiredIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_HandoverType_t *HandoverType = NULL;
    S1AP_Cause_t *Cause = NULL;
    S1AP_TargetID_t *TargetID = NULL;
    S1AP_TargeteNB_ID_t *targeteNB_ID = NULL;
    S1AP_Global_ENB_ID_t *global_ENB_ID = NULL;
    S1AP_TAI_t *selected_TAI = NULL;
    S1AP_Source_ToTarget_TransparentContainer_t
        *Source_ToTarget_TransparentContainer = NULL;

    uint8_t tmp[OGS_MAX_SDU_LEN];
    char *_container =
        "4080ab0f1014"
        "c59800018000bf06 ec4d00100302c000 0000000015a80014 06720af00348f100"
        "13d03bc58414903b c58809401de2c210 044013f212249010 127dad808fd50398"
        "381c08fd50398380 5f5037868baa010c 846fb057874162d6 2e821f42f2e159f8"
        "21d09c0009e28aa1 a0012d888026d060 0040800806607200 3e415250d4621020"
        "81843423c1350022 14d6dfb0a0b06502 c402000004008177 faab4677235334a0"
        "e867213e80403000 01004e4002450000 4e400246000000f1 1000043a000000f1"
        "10001f20a0000022";

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_HandoverPreparation;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_HandoverRequired;

    HandoverRequired = &initiatingMessage->value.choice.HandoverRequired;

    ie = CALLOC(1, sizeof(S1AP_HandoverRequiredIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequired->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverRequiredIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_HandoverRequiredIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequired->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverRequiredIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_HandoverRequiredIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequired->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_HandoverType;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverRequiredIEs__value_PR_HandoverType;

    HandoverType = &ie->value.choice.HandoverType;

    *HandoverType = handover_type;

    ie = CALLOC(1, sizeof(S1AP_HandoverRequiredIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequired->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_HandoverRequiredIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    ie = CALLOC(1, sizeof(S1AP_HandoverRequiredIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequired->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_TargetID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverRequiredIEs__value_PR_TargetID;

    TargetID = &ie->value.choice.TargetID;

    TargetID->present = S1AP_TargetID_PR_targeteNB_ID;
    targeteNB_ID = CALLOC(1, sizeof(S1AP_TargeteNB_ID_t));
    TargetID->choice.targeteNB_ID = targeteNB_ID;

    global_ENB_ID = &targeteNB_ID->global_ENB_ID;
    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_ue->e_tai.plmn_id, OGS_PLMN_ID_LEN,
            &global_ENB_ID->pLMNidentity);
    ogs_s1ap_uint32_to_ENB_ID(present, enb_id, &global_ENB_ID->eNB_ID);

    selected_TAI = &targeteNB_ID->selected_TAI;
    ogs_asn_uint16_to_OCTET_STRING(test_ue->e_tai.tac, &selected_TAI->tAC);
    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_ue->e_tai.plmn_id,
            OGS_PLMN_ID_LEN, &selected_TAI->pLMNidentity);

    ie = CALLOC(1, sizeof(S1AP_HandoverRequiredIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequired->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Source_ToTarget_TransparentContainer;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverRequiredIEs__value_PR_Source_ToTarget_TransparentContainer;

    Source_ToTarget_TransparentContainer =
        &ie->value.choice.Source_ToTarget_TransparentContainer;

    OGS_HEX(_container, strlen(_container), tmp),

    Source_ToTarget_TransparentContainer->size = 206;
    Source_ToTarget_TransparentContainer->buf =
        CALLOC(Source_ToTarget_TransparentContainer->size, sizeof(uint8_t));
    memcpy(Source_ToTarget_TransparentContainer->buf,
            tmp, Source_ToTarget_TransparentContainer->size);

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_handover_request_ack(test_ue_t *test_ue)
{
    int rv;

    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_HandoverRequestAcknowledge_t *HandoverRequestAcknowledge = NULL;

    S1AP_HandoverRequestAcknowledgeIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABAdmittedList_t *E_RABAdmittedList = NULL;
    S1AP_Target_ToSource_TransparentContainer_t
        *Target_ToSource_TransparentContainer = NULL;

    char hexbuf[OGS_MAX_SDU_LEN];
    const char *payload =
        "00 80810bf900d8af40 00a0339057801f47 88009e81de2c20a4"
        "81de2c404a00ef16 2000010044013f21 2249008093efd243 3914cd2aa0a0142f"
        "f2214d6dfb82c194 0b10080000020040 bbfd55aeab41ad80 8fd50398381c08fd"
        "503983804d037868 baa016423342bc3a 18b58fa084ca833f a17970acfc10e84e"
        "0004f14550d00096 c88900";

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(S1AP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_HandoverResourceAllocation;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_HandoverRequestAcknowledge;

    HandoverRequestAcknowledge =
        &successfulOutcome->value.choice.HandoverRequestAcknowledge;

    ie = CALLOC(1, sizeof(S1AP_HandoverRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverRequestAcknowledgeIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_HandoverRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverRequestAcknowledgeIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    test_ue->enb_ue_s1ap_id++;
    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_HandoverRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABAdmittedList;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverRequestAcknowledgeIEs__value_PR_E_RABAdmittedList;

    E_RABAdmittedList = &ie->value.choice.E_RABAdmittedList;
    ogs_list_for_each(&test_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            S1AP_E_RABAdmittedItemIEs_t *item = NULL;
            S1AP_E_RABAdmittedItem_t *e_rab = NULL;

            ogs_gtp_f_teid_t f_teid;
            ogs_ip_t ip;
            int len;

            item = CALLOC(1, sizeof(S1AP_E_RABAdmittedItemIEs_t));
            ASN_SEQUENCE_ADD(&E_RABAdmittedList->list, item);

            item->id = S1AP_ProtocolIE_ID_id_E_RABAdmittedItem;
            item->criticality = S1AP_Criticality_ignore;
            item->value.present =
                S1AP_E_RABAdmittedItemIEs__value_PR_E_RABAdmittedItem;

            e_rab = &item->value.choice.E_RABAdmittedItem;

            e_rab->e_RAB_ID = bearer->ebi;

            rv = ogs_gtp_sockaddr_to_f_teid(
                    bearer->enb_s1u_addr, bearer->enb_s1u_addr6, &f_teid, &len);
            ogs_assert(rv == OGS_OK);

            rv = ogs_gtp_f_teid_to_ip(&f_teid, &ip);
            ogs_assert(rv == OGS_OK);

            rv = ogs_asn_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
            ogs_assert(rv == OGS_OK);
            ogs_asn_uint32_to_OCTET_STRING(
                    bearer->enb_s1u_teid, &e_rab->gTP_TEID);

            e_rab->dL_transportLayerAddress =
                (S1AP_TransportLayerAddress_t *)
                CALLOC(1, sizeof(S1AP_TransportLayerAddress_t));
            rv = ogs_asn_ip_to_BIT_STRING(&ip, e_rab->dL_transportLayerAddress);
            ogs_assert(rv == OGS_OK);
            e_rab->dL_gTP_TEID = (S1AP_GTP_TEID_t *)
                CALLOC(1, sizeof(S1AP_GTP_TEID_t));
            ogs_asn_uint32_to_OCTET_STRING(
                    bearer->enb_s1u_teid+10, e_rab->dL_gTP_TEID);

            e_rab->uL_TransportLayerAddress =
                (S1AP_TransportLayerAddress_t *)
                CALLOC(1, sizeof(S1AP_TransportLayerAddress_t));
            rv = ogs_asn_ip_to_BIT_STRING(&ip, e_rab->uL_TransportLayerAddress);
            ogs_assert(rv == OGS_OK);
            e_rab->uL_GTP_TEID = (S1AP_GTP_TEID_t *)
                CALLOC(1, sizeof(S1AP_GTP_TEID_t));
            ogs_asn_uint32_to_OCTET_STRING(
                    bearer->enb_s1u_teid+20, e_rab->uL_GTP_TEID);
        }
    }

    ie = CALLOC(1, sizeof(S1AP_HandoverRequestAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Target_ToSource_TransparentContainer;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_HandoverRequestAcknowledgeIEs__value_PR_Target_ToSource_TransparentContainer;

    Target_ToSource_TransparentContainer =
        &ie->value.choice.Target_ToSource_TransparentContainer;

    ogs_s1ap_buffer_to_OCTET_STRING(
            OGS_HEX(payload, strlen(payload), hexbuf), 132,
            Target_ToSource_TransparentContainer);

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_enb_status_transfer(test_ue_t *test_ue)
{
    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_ENBStatusTransfer_t *ENBStatusTransfer =
        NULL;

    S1AP_ENBStatusTransferIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_ENB_StatusTransfer_TransparentContainer_t
        *ENB_StatusTransfer_TransparentContainer = NULL;
	S1AP_Bearers_SubjectToStatusTransferList_t
        *Bearers_SubjectToStatusTransferList = NULL;
	S1AP_Bearers_SubjectToStatusTransfer_ItemIEs_t *ie2 = NULL;
	S1AP_Bearers_SubjectToStatusTransfer_Item_t *item = NULL;

    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_eNBStatusTransfer;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_ENBStatusTransfer;

    ENBStatusTransfer = &initiatingMessage->value.choice.ENBStatusTransfer;

    ie = CALLOC(1, sizeof(S1AP_ENBStatusTransferIEs_t));
    ASN_SEQUENCE_ADD(&ENBStatusTransfer->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_ENBStatusTransferIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_ENBStatusTransferIEs_t));
    ASN_SEQUENCE_ADD(&ENBStatusTransfer->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_ENBStatusTransferIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_ENBStatusTransferIEs_t));
    ASN_SEQUENCE_ADD(&ENBStatusTransfer->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_StatusTransfer_TransparentContainer;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_ENBStatusTransferIEs__value_PR_ENB_StatusTransfer_TransparentContainer;

    ENB_StatusTransfer_TransparentContainer =
        &ie->value.choice.ENB_StatusTransfer_TransparentContainer;

    Bearers_SubjectToStatusTransferList =
        &ENB_StatusTransfer_TransparentContainer->
        bearers_SubjectToStatusTransferList;

    ogs_list_for_each(&test_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            ie2 = CALLOC(
                    1, sizeof(S1AP_Bearers_SubjectToStatusTransfer_ItemIEs_t));
            ASN_SEQUENCE_ADD(&Bearers_SubjectToStatusTransferList->list, ie2);

            ie2->id =
                S1AP_ProtocolIE_ID_id_Bearers_SubjectToStatusTransfer_Item;
            ie2->criticality = S1AP_Criticality_ignore;
            ie2->value.present = S1AP_Bearers_SubjectToStatusTransfer_ItemIEs__value_PR_Bearers_SubjectToStatusTransfer_Item;

            item = &ie2->value.choice.Bearers_SubjectToStatusTransfer_Item;
            item->e_RAB_ID = bearer->ebi;
            item->uL_COUNTvalue.pDCP_SN = 75;
            item->dL_COUNTvalue.pDCP_SN = 17;
        }
    }

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_handover_notify(test_ue_t *test_ue)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_HandoverNotify_t *HandoverNotify = NULL;

    S1AP_HandoverNotifyIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_EUTRAN_CGI_t *EUTRAN_CGI = NULL;
    S1AP_TAI_t *TAI = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_HandoverNotification;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_HandoverNotify;

    HandoverNotify = &initiatingMessage->value.choice.HandoverNotify;

    ie = CALLOC(1, sizeof(S1AP_HandoverNotifyIEs_t));
    ASN_SEQUENCE_ADD(&HandoverNotify->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverNotifyIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_HandoverNotifyIEs_t));
    ASN_SEQUENCE_ADD(&HandoverNotify->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverNotifyIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_HandoverNotifyIEs_t));
    ASN_SEQUENCE_ADD(&HandoverNotify->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_EUTRAN_CGI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_HandoverNotifyIEs__value_PR_EUTRAN_CGI;

    EUTRAN_CGI = &ie->value.choice.EUTRAN_CGI;

    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_ue->e_cgi.plmn_id,
            OGS_PLMN_ID_LEN, &EUTRAN_CGI->pLMNidentity);
    EUTRAN_CGI->cell_ID.size = 4;
    EUTRAN_CGI->cell_ID.buf =  CALLOC(
         EUTRAN_CGI->cell_ID.size, sizeof(uint8_t));
    ogs_assert(EUTRAN_CGI->cell_ID.buf);
    EUTRAN_CGI->cell_ID.buf[0] = (test_ue->e_cgi.cell_id >> 24);
    EUTRAN_CGI->cell_ID.buf[1] = (test_ue->e_cgi.cell_id >> 16);
    EUTRAN_CGI->cell_ID.buf[2] = (test_ue->e_cgi.cell_id >> 8);
    EUTRAN_CGI->cell_ID.buf[3] = (test_ue->e_cgi.cell_id);
    EUTRAN_CGI->cell_ID.bits_unused = 4;

    ie = CALLOC(1, sizeof(S1AP_HandoverNotifyIEs_t));
    ASN_SEQUENCE_ADD(&HandoverNotify->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_TAI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_HandoverNotifyIEs__value_PR_TAI;

    TAI = &ie->value.choice.TAI;

    ogs_asn_uint16_to_OCTET_STRING(test_ue->e_tai.tac, &TAI->tAC);
    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_ue->e_tai.plmn_id, OGS_PLMN_ID_LEN, &TAI->pLMNidentity);


    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_handover_cancel(test_ue_t *test_ue,
        S1AP_Cause_PR group, long cause)
{
    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_HandoverCancel_t *HandoverCancel = NULL;

    S1AP_HandoverCancelIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_Cause_t *Cause = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_HandoverCancel;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_HandoverCancel;

    HandoverCancel = &initiatingMessage->value.choice.HandoverCancel;

    ie = CALLOC(1, sizeof(S1AP_HandoverCancelIEs_t));
    ASN_SEQUENCE_ADD(&HandoverCancel->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverCancelIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_HandoverCancelIEs_t));
    ASN_SEQUENCE_ADD(&HandoverCancel->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverCancelIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_HandoverCancelIEs_t));
    ASN_SEQUENCE_ADD(&HandoverCancel->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_HandoverCancelIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_handover_failure(test_ue_t *test_ue,
        S1AP_Cause_PR group, long cause)
{
    int rv;

    S1AP_S1AP_PDU_t pdu;
    S1AP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    S1AP_HandoverFailure_t *HandoverFailure = NULL;

    S1AP_HandoverFailureIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_Cause_t *Cause = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_unsuccessfulOutcome;
    pdu.choice.unsuccessfulOutcome =
        CALLOC(1, sizeof(S1AP_UnsuccessfulOutcome_t));

    unsuccessfulOutcome = pdu.choice.unsuccessfulOutcome;
    unsuccessfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_HandoverResourceAllocation;
    unsuccessfulOutcome->criticality = S1AP_Criticality_reject;
    unsuccessfulOutcome->value.present =
        S1AP_UnsuccessfulOutcome__value_PR_HandoverFailure;

    HandoverFailure = &unsuccessfulOutcome->value.choice.HandoverFailure;

    ie = CALLOC(1, sizeof(S1AP_HandoverFailureIEs_t));
    ASN_SEQUENCE_ADD(&HandoverFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverFailureIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_HandoverFailureIEs_t));
    ASN_SEQUENCE_ADD(&HandoverFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_HandoverFailureIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    return ogs_s1ap_encode(&pdu);
}

#define TEST_S1AP_MAX_MESSAGE 64

ogs_pkbuf_t *test_s1ap_build_invalid_packet(int i)
{
    ogs_pkbuf_t *pkbuf = NULL;
    const char *payload[TEST_S1AP_MAX_MESSAGE] = {
        "0011002100000300 3b40080062f22400 0001700040000700 00004062f224002c"
        "00030a0100",
        "",
    };

    uint16_t len[TEST_S1AP_MAX_MESSAGE] = {
        37,
        0,
    };

    char hexbuf[OGS_MAX_SDU_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return pkbuf;
}

ogs_pkbuf_t *test_s1ap_build_enb_configuration_transfer(int i)
{
    ogs_pkbuf_t *pkbuf = NULL;
    const char *payload[TEST_S1AP_MAX_MESSAGE] = {
        "0028"
        "403b000001008140 3440049699000004 3004969900020004 969900001f200496"
        "9900020000000098 401341f0ac110e02 0000009940070200 f8ac110e02",
        "0028"
        "403b000001008140 344004969900001f 2004969900020004 9699000004300496"
        "9900020000000098 401341f0ac110e02 0000009940070200 f8ac110e02",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    uint16_t len[TEST_S1AP_MAX_MESSAGE] = {
        63,
        63,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[OGS_MAX_SDU_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
        OGS_HEX(payload[i], strlen(payload[i]), hexbuf), len[i]);

    return pkbuf;
}
