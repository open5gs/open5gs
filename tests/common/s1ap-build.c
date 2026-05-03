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
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_S1Setup;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_S1SetupRequest;

    initiatingMessage->value.choice.S1SetupRequest =
        CALLOC(1, sizeof(*S1SetupRequest));
    S1SetupRequest = initiatingMessage->value.choice.S1SetupRequest;
    ogs_assert(S1SetupRequest);

    S1SetupRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_S1SetupRequest);
    ogs_assert(S1SetupRequest->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(S1SetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Global_ENB_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_S1SetupRequestIEs__value_PR_Global_ENB_ID;

    ie->value.choice.Global_ENB_ID =
        CALLOC(1, sizeof(*Global_ENB_ID));
    Global_ENB_ID = ie->value.choice.Global_ENB_ID;
    ogs_assert(Global_ENB_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(S1SetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_SupportedTAs;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_S1SetupRequestIEs__value_PR_SupportedTAs;

    ie->value.choice.SupportedTAs =
        CALLOC(1, sizeof(*SupportedTAs));
    SupportedTAs = ie->value.choice.SupportedTAs;
    ogs_assert(SupportedTAs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(S1SetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_DefaultPagingDRX;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_S1SetupRequestIEs__value_PR_PagingDRX;

    ie->value.choice.PagingDRX = CALLOC(1, sizeof(*PagingDRX));
    PagingDRX = ie->value.choice.PagingDRX;
    ogs_assert(PagingDRX);

    Global_ENB_ID->eNB_ID =
        CALLOC(1, sizeof(*Global_ENB_ID->eNB_ID));
    ogs_assert(Global_ENB_ID->eNB_ID);
    ogs_s1ap_uint32_to_ENB_ID(present, enb_id, Global_ENB_ID->eNB_ID);
    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_self()->e_tai.plmn_id, OGS_PLMN_ID_LEN,
            &Global_ENB_ID->pLMNidentity);

    SupportedTAs_Item =
        CALLOC(1, sizeof(*SupportedTAs_Item));
    ogs_assert(SupportedTAs_Item);
    ogs_asn_uint16_to_OCTET_STRING(
            test_self()->e_tai.tac, &SupportedTAs_Item->tAC);
    PLMNidentity = CALLOC(1, sizeof(*PLMNidentity));
    ogs_assert(PLMNidentity);
    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_self()->e_tai.plmn_id, OGS_PLMN_ID_LEN, PLMNidentity);
    SupportedTAs_Item->broadcastPLMNs =
        CALLOC(1, sizeof(*SupportedTAs_Item->broadcastPLMNs));
    ogs_assert(SupportedTAs_Item->broadcastPLMNs);
    ASN_SEQUENCE_ADD(SupportedTAs_Item->broadcastPLMNs, PLMNidentity);

    ASN_SEQUENCE_ADD(SupportedTAs, SupportedTAs_Item);

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
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_initialUEMessage;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_InitialUEMessage;

    initiatingMessage->value.choice.InitialUEMessage =
        CALLOC(1, sizeof(*InitialUEMessage));
    InitialUEMessage = initiatingMessage->value.choice.InitialUEMessage;
    ogs_assert(InitialUEMessage);

    InitialUEMessage->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_InitialUEMessage);
    ogs_assert(InitialUEMessage->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialUEMessage->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_InitialUEMessage_IEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    test_ue->enb_ue_s1ap_id++;
    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialUEMessage->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_NAS_PDU;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_InitialUEMessage_IEs__value_PR_NAS_PDU;

    ie->value.choice.NAS_PDU = CALLOC(1, sizeof(*NAS_PDU));
    NAS_PDU = ie->value.choice.NAS_PDU;
    ogs_assert(NAS_PDU);

    NAS_PDU->size = emmbuf->len;
    NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
    ogs_assert(NAS_PDU->buf);
    memcpy(NAS_PDU->buf, emmbuf->data, NAS_PDU->size);
    ogs_pkbuf_free(emmbuf);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialUEMessage->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_TAI;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_InitialUEMessage_IEs__value_PR_TAI;

    ie->value.choice.TAI =
        CALLOC(1, sizeof(*TAI));
    TAI = ie->value.choice.TAI;
    ogs_assert(TAI);

    ogs_asn_uint16_to_OCTET_STRING(test_ue->e_tai.tac, &TAI->tAC);
    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_ue->e_tai.plmn_id, OGS_PLMN_ID_LEN, &TAI->pLMNidentity);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialUEMessage->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_EUTRAN_CGI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_InitialUEMessage_IEs__value_PR_EUTRAN_CGI;

    ie->value.choice.EUTRAN_CGI =
        CALLOC(1, sizeof(*EUTRAN_CGI));
    EUTRAN_CGI = ie->value.choice.EUTRAN_CGI;
    ogs_assert(EUTRAN_CGI);

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

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialUEMessage->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_RRC_Establishment_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialUEMessage_IEs__value_PR_RRC_Establishment_Cause;

    ie->value.choice.RRC_Establishment_Cause = CALLOC(1, sizeof(*RRC_Establishment_Cause));
    RRC_Establishment_Cause = ie->value.choice.RRC_Establishment_Cause;
    ogs_assert(RRC_Establishment_Cause);

    *RRC_Establishment_Cause = cause;

    if (s_tmsi) {
        S1AP_MME_Code_t *mMEC = NULL;
        S1AP_M_TMSI_t *m_TMSI = NULL;
        S1AP_S_TMSI_t *S_TMSI = NULL;

        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(InitialUEMessage->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_S_TMSI;
        ie->criticality = S1AP_Criticality_reject;
        ie->value.present = S1AP_InitialUEMessage_IEs__value_PR_S_TMSI;

        ie->value.choice.S_TMSI =
            CALLOC(1, sizeof(*S_TMSI));
        S_TMSI = ie->value.choice.S_TMSI;
        ogs_assert(S_TMSI);

        mMEC = &S_TMSI->mMEC;
        m_TMSI = &S_TMSI->m_TMSI;

        ogs_asn_uint8_to_OCTET_STRING(test_ue->nas_eps_guti.mme_code, mMEC);
        ogs_asn_uint32_to_OCTET_STRING(test_ue->nas_eps_guti.m_tmsi, m_TMSI);
    }

    if (test_ue->initial_ue_param.gummei_id) {
        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(InitialUEMessage->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_GUMMEI_ID;
        ie->criticality = S1AP_Criticality_reject;
        ie->value.present = S1AP_InitialUEMessage_IEs__value_PR_GUMMEI;

        ie->value.choice.GUMMEI =
            CALLOC(1, sizeof(*GUMMEI));
        GUMMEI = ie->value.choice.GUMMEI;
        ogs_assert(GUMMEI);
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
        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(InitialUEMessage->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_GUMMEIType;
        ie->criticality = S1AP_Criticality_reject;
        ie->value.present = S1AP_InitialUEMessage_IEs__value_PR_GUMMEIType;

        ie->value.choice.GUMMEIType = CALLOC(1, sizeof(*GUMMEIType));
        GUMMEIType = ie->value.choice.GUMMEIType;
        ogs_assert(GUMMEIType);

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
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_uplinkNASTransport;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_UplinkNASTransport;

    initiatingMessage->value.choice.UplinkNASTransport =
        CALLOC(1, sizeof(*UplinkNASTransport));
    UplinkNASTransport = initiatingMessage->value.choice.UplinkNASTransport;
    ogs_assert(UplinkNASTransport);

    UplinkNASTransport->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_UplinkNASTransport);
    ogs_assert(UplinkNASTransport->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UplinkNASTransport_IEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UplinkNASTransport_IEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_NAS_PDU;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_UplinkNASTransport_IEs__value_PR_NAS_PDU;

    ie->value.choice.NAS_PDU = CALLOC(1, sizeof(*NAS_PDU));
    NAS_PDU = ie->value.choice.NAS_PDU;
    ogs_assert(NAS_PDU);

    NAS_PDU->size = nasbuf->len;
    NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
    ogs_assert(NAS_PDU->buf);
    memcpy(NAS_PDU->buf, nasbuf->data, NAS_PDU->size);
    ogs_pkbuf_free(nasbuf);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_EUTRAN_CGI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_UplinkNASTransport_IEs__value_PR_EUTRAN_CGI;

    ie->value.choice.EUTRAN_CGI =
        CALLOC(1, sizeof(*EUTRAN_CGI));
    EUTRAN_CGI = ie->value.choice.EUTRAN_CGI;
    ogs_assert(EUTRAN_CGI);

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

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_TAI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_UplinkNASTransport_IEs__value_PR_TAI;

    ie->value.choice.TAI =
        CALLOC(1, sizeof(*TAI));
    TAI = ie->value.choice.TAI;
    ogs_assert(TAI);

    ogs_asn_uint16_to_OCTET_STRING(test_ue->e_tai.tac, &TAI->tAC);
    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_ue->e_tai.plmn_id, OGS_PLMN_ID_LEN, &TAI->pLMNidentity);

    if (test_ue->nr_cgi.cell_id) {
        ie = CALLOC(1, sizeof(*ie));
        ogs_assert(ie);
        ASN_SEQUENCE_ADD(UplinkNASTransport->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_PSCellInformation;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_UplinkNASTransport_IEs__value_PR_PSCellInformation;

        ie->value.choice.PSCellInformation =
            CALLOC(1, sizeof(*PSCellInformation));
        PSCellInformation = ie->value.choice.PSCellInformation;
        ogs_assert(PSCellInformation);
        PSCellInformation->nCGI =
            CALLOC(1, sizeof(*nCGI));
        ogs_assert(PSCellInformation->nCGI);
        nCGI = PSCellInformation->nCGI;
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

    uint8_t tmp[OGS_HUGE_LEN];
    char *_capability_captured =
        "013001023c"
        "d98000bc000ff06e c4d00141b82c0000 000007d404000ef0 802000016a030000"
        "880000";

    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_UECapabilityInfoIndication;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_UECapabilityInfoIndication;

    initiatingMessage->value.choice.UECapabilityInfoIndication =
        CALLOC(1, sizeof(*UECapabilityInfoIndication));

    UECapabilityInfoIndication = initiatingMessage->value.choice.UECapabilityInfoIndication;
    ogs_assert(UECapabilityInfoIndication);

    UECapabilityInfoIndication->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_UECapabilityInfoIndication);
    ogs_assert(UECapabilityInfoIndication->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UECapabilityInfoIndication->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UECapabilityInfoIndicationIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UECapabilityInfoIndication->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UECapabilityInfoIndicationIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UECapabilityInfoIndication->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_UERadioCapability;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_UECapabilityInfoIndicationIEs__value_PR_UERadioCapability;

    ie->value.choice.UERadioCapability = CALLOC(1, sizeof(*UERadioCapability));
    UERadioCapability = ie->value.choice.UERadioCapability;
    ogs_assert(UERadioCapability);

    ogs_hex_from_string(_capability_captured, tmp, sizeof(tmp));

    UERadioCapability->size = 40;
    UERadioCapability->buf = CALLOC(UERadioCapability->size, sizeof(uint8_t));
    ogs_assert(UERadioCapability->buf);
    memcpy(UERadioCapability->buf, tmp, UERadioCapability->size);

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_initial_context_setup_response(test_ue_t *test_ue)
{
    int rv;

    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    ogs_gtp2_f_teid_t f_teid;
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
        CALLOC(1, sizeof(*successfulOutcome));

    successfulOutcome = pdu.choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    successfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_InitialContextSetup;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_InitialContextSetupResponse;

    successfulOutcome->value.choice.InitialContextSetupResponse =
        CALLOC(1, sizeof(*InitialContextSetupResponse));

    InitialContextSetupResponse = successfulOutcome->value.choice.InitialContextSetupResponse;
    ogs_assert(InitialContextSetupResponse);

    InitialContextSetupResponse->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_InitialContextSetupResponse);
    ogs_assert(InitialContextSetupResponse->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialContextSetupResponseIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialContextSetupResponseIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABSetupListCtxtSURes;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialContextSetupResponseIEs__value_PR_E_RABSetupListCtxtSURes;

    ie->value.choice.E_RABSetupListCtxtSURes =
        CALLOC(1, sizeof(*E_RABSetupListCtxtSURes));
    E_RABSetupListCtxtSURes = ie->value.choice.E_RABSetupListCtxtSURes;
    ogs_assert(E_RABSetupListCtxtSURes);

    ogs_list_for_each(&test_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
        item = CALLOC(1, sizeof(*item));
        ogs_assert(item);
        ASN_SEQUENCE_ADD(E_RABSetupListCtxtSURes, item);

        item->id = S1AP_ProtocolIE_ID_id_E_RABSetupItemCtxtSURes;
        item->criticality = S1AP_Criticality_ignore;
        item->value.present =
            S1AP_E_RABSetupItemCtxtSUResIEs__value_PR_E_RABSetupItemCtxtSURes;

        item->value.choice.E_RABSetupItemCtxtSURes =
            CALLOC(1, sizeof(*e_rab));
        e_rab = item->value.choice.E_RABSetupItemCtxtSURes;
        ogs_assert(e_rab);

        e_rab->e_RAB_ID = bearer->ebi;

        rv = ogs_gtp2_sockaddr_to_f_teid(
                bearer->enb_s1u_addr, bearer->enb_s1u_addr6, &f_teid, &len);
        ogs_assert(rv == OGS_OK);

        rv = ogs_gtp2_f_teid_to_ip(&f_teid, &ip);
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
        CALLOC(1, sizeof(*unsuccessfulOutcome));

    unsuccessfulOutcome = pdu.choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    unsuccessfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_InitialContextSetup;
    unsuccessfulOutcome->criticality = S1AP_Criticality_reject;
    unsuccessfulOutcome->value.present =
        S1AP_UnsuccessfulOutcome__value_PR_InitialContextSetupFailure;

    unsuccessfulOutcome->value.choice.InitialContextSetupFailure =
        CALLOC(1, sizeof(*InitialContextSetupFailure));

    InitialContextSetupFailure = unsuccessfulOutcome->value.choice.InitialContextSetupFailure;
    ogs_assert(InitialContextSetupFailure);

    InitialContextSetupFailure->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_InitialContextSetupFailure);
    ogs_assert(InitialContextSetupFailure->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialContextSetupFailureIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_InitialContextSetupFailureIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(InitialContextSetupFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_InitialContextSetupFailureIEs__value_PR_Cause;

    ie->value.choice.Cause =
        CALLOC(1, sizeof(*Cause));
    Cause = ie->value.choice.Cause;
    ogs_assert(Cause);

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
        CALLOC(1, sizeof(*successfulOutcome));

    successfulOutcome = pdu.choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    successfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_UEContextModification;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_UEContextModificationResponse;

    successfulOutcome->value.choice.UEContextModificationResponse =
        CALLOC(1, sizeof(*UEContextModificationResponse));

    UEContextModificationResponse = successfulOutcome->value.choice.UEContextModificationResponse;
    ogs_assert(UEContextModificationResponse);

    UEContextModificationResponse->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_UEContextModificationResponse);
    ogs_assert(UEContextModificationResponse->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextModificationResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_UEContextModificationResponseIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextModificationResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_UEContextModificationResponseIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

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
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_UEContextReleaseRequest;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_UEContextReleaseRequest;

    initiatingMessage->value.choice.UEContextReleaseRequest =
        CALLOC(1, sizeof(*UEContextReleaseRequest));

    UEContextReleaseRequest = initiatingMessage->value.choice.UEContextReleaseRequest;
    ogs_assert(UEContextReleaseRequest);

    UEContextReleaseRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_UEContextReleaseRequest);
    ogs_assert(UEContextReleaseRequest->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextReleaseRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UEContextReleaseRequest_IEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextReleaseRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UEContextReleaseRequest_IEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextReleaseRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_UEContextReleaseRequest_IEs__value_PR_Cause;

    ie->value.choice.Cause =
        CALLOC(1, sizeof(*Cause));
    Cause = ie->value.choice.Cause;
    ogs_assert(Cause);

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
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(*successfulOutcome));

    successfulOutcome = pdu.choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    successfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_UEContextRelease;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_UEContextReleaseComplete;

    successfulOutcome->value.choice.UEContextReleaseComplete =
        CALLOC(1, sizeof(*UEContextReleaseComplete));

    UEContextReleaseComplete = successfulOutcome->value.choice.UEContextReleaseComplete;
    ogs_assert(UEContextReleaseComplete);

    UEContextReleaseComplete->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_UEContextReleaseComplete);
    ogs_assert(UEContextReleaseComplete->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextReleaseComplete->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_UEContextReleaseComplete_IEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);
    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(UEContextReleaseComplete->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_UEContextReleaseComplete_IEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);
    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_e_rab_setup_response(test_bearer_t *bearer)
{
    int rv;

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

    ogs_gtp2_f_teid_t f_teid;
    ogs_ip_t ip;
    int len;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(*successfulOutcome));

    successfulOutcome = pdu.choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_E_RABSetup;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_E_RABSetupResponse;

    successfulOutcome->value.choice.E_RABSetupResponse =
        CALLOC(1, sizeof(*E_RABSetupResponse));
    E_RABSetupResponse = successfulOutcome->value.choice.E_RABSetupResponse;
    ogs_assert(E_RABSetupResponse);

    E_RABSetupResponse->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_E_RABSetupResponse);
    ogs_assert(E_RABSetupResponse->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABSetupResponseIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABSetupResponseIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABSetupListBearerSURes;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_E_RABSetupResponseIEs__value_PR_E_RABSetupListBearerSURes;

    ie->value.choice.E_RABSetupListBearerSURes =
        CALLOC(1, sizeof(*E_RABSetupListBearerSURes));
    E_RABSetupListBearerSURes = ie->value.choice.E_RABSetupListBearerSURes;
    ogs_assert(E_RABSetupListBearerSURes);

    item = CALLOC(1, sizeof(*item));
    ogs_assert(item);
    ASN_SEQUENCE_ADD(E_RABSetupListBearerSURes, item);

    item->id = S1AP_ProtocolIE_ID_id_E_RABSetupItemBearerSURes;
    item->criticality = S1AP_Criticality_ignore;
    item->value.present =
        S1AP_E_RABSetupItemBearerSUResIEs__value_PR_E_RABSetupItemBearerSURes;

    item->value.choice.E_RABSetupItemBearerSURes =
        CALLOC(1, sizeof(*e_rab));
    e_rab = item->value.choice.E_RABSetupItemBearerSURes;
    ogs_assert(e_rab);

    e_rab->e_RAB_ID = bearer->ebi;

    rv = ogs_gtp2_sockaddr_to_f_teid(
            bearer->enb_s1u_addr, bearer->enb_s1u_addr6, &f_teid, &len);
    ogs_assert(rv == OGS_OK);

    rv = ogs_gtp2_f_teid_to_ip(&f_teid, &ip);
    ogs_assert(rv == OGS_OK);

    rv = ogs_asn_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
    ogs_assert(rv == OGS_OK);
    ogs_asn_uint32_to_OCTET_STRING(bearer->enb_s1u_teid, &e_rab->gTP_TEID);

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_e_rab_failed_setup_response(
        test_bearer_t *bearer, S1AP_Cause_PR group, long cause)
{
    int rv;

    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_SuccessfulOutcome_t *successfulOutcome = NULL;
    S1AP_E_RABSetupResponse_t *E_RABSetupResponse = NULL;

    S1AP_E_RABSetupResponseIEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_E_RABList_t *E_RABList = NULL;

    S1AP_E_RABItemIEs_t *item = NULL;
    S1AP_E_RABItem_t *e_rab = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    test_ue = sess->test_ue;
    ogs_assert(test_ue);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(*successfulOutcome));

    successfulOutcome = pdu.choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_E_RABSetup;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_E_RABSetupResponse;

    successfulOutcome->value.choice.E_RABSetupResponse =
        CALLOC(1, sizeof(*E_RABSetupResponse));
    E_RABSetupResponse = successfulOutcome->value.choice.E_RABSetupResponse;
    ogs_assert(E_RABSetupResponse);

    E_RABSetupResponse->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_E_RABSetupResponse);
    ogs_assert(E_RABSetupResponse->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABSetupResponseIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABSetupResponseIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABSetupResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABFailedToSetupListBearerSURes;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABSetupResponseIEs__value_PR_E_RABList;

    ie->value.choice.E_RABList =
        CALLOC(1, sizeof(*E_RABList));
    E_RABList = ie->value.choice.E_RABList;
    ogs_assert(E_RABList);

    item = CALLOC(1, sizeof(*item));
    ogs_assert(item);
    ASN_SEQUENCE_ADD(E_RABList, item);

    item->id = S1AP_ProtocolIE_ID_id_E_RABItem;
    item->criticality = S1AP_Criticality_reject;
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

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_e_rab_modify_response(test_bearer_t *bearer)
{
    int rv;

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
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(*successfulOutcome));

    successfulOutcome = pdu.choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_E_RABModify;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_E_RABModifyResponse;

    successfulOutcome->value.choice.E_RABModifyResponse =
        CALLOC(1, sizeof(*E_RABModifyResponse));
    E_RABModifyResponse = successfulOutcome->value.choice.E_RABModifyResponse;
    ogs_assert(E_RABModifyResponse);

    E_RABModifyResponse->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_E_RABModifyResponse);
    ogs_assert(E_RABModifyResponse->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABModifyResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABModifyResponseIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABModifyResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABModifyResponseIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABModifyResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABModifyListBearerModRes;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_E_RABModifyResponseIEs__value_PR_E_RABModifyListBearerModRes;

    ie->value.choice.E_RABModifyListBearerModRes =
        CALLOC(1, sizeof(*E_RABModifyListBearerModRes));
    E_RABModifyListBearerModRes = ie->value.choice.E_RABModifyListBearerModRes;
    ogs_assert(E_RABModifyListBearerModRes);

    item = CALLOC(1, sizeof(*item));
    ogs_assert(item);
    ASN_SEQUENCE_ADD(E_RABModifyListBearerModRes, item);

    item->id = S1AP_ProtocolIE_ID_id_E_RABModifyItemBearerModRes;
    item->criticality = S1AP_Criticality_ignore;
    item->value.present = S1AP_E_RABModifyItemBearerModResIEs__value_PR_E_RABModifyItemBearerModRes;

    item->value.choice.E_RABModifyItemBearerModRes =
        CALLOC(1, sizeof(*e_rab));
    e_rab = item->value.choice.E_RABModifyItemBearerModRes;
    ogs_assert(e_rab);

    e_rab->e_RAB_ID = bearer->ebi;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_e_rab_release_response(test_bearer_t *bearer)
{
    int rv;

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
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(*successfulOutcome));

    successfulOutcome = pdu.choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_E_RABRelease;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_E_RABReleaseResponse;

    successfulOutcome->value.choice.E_RABReleaseResponse =
        CALLOC(1, sizeof(*E_RABReleaseResponse));

    E_RABReleaseResponse = successfulOutcome->value.choice.E_RABReleaseResponse;
    ogs_assert(E_RABReleaseResponse);

    E_RABReleaseResponse->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_E_RABReleaseResponse);
    ogs_assert(E_RABReleaseResponse->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABReleaseResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABReleaseResponseIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABReleaseResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_E_RABReleaseResponseIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABReleaseResponse->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABReleaseListBearerRelComp;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_E_RABReleaseResponseIEs__value_PR_E_RABReleaseListBearerRelComp;

    ie->value.choice.E_RABReleaseListBearerRelComp =
        CALLOC(1, sizeof(*E_RABReleaseListBearerRelComp));

    E_RABReleaseListBearerRelComp = ie->value.choice.E_RABReleaseListBearerRelComp;
    ogs_assert(E_RABReleaseListBearerRelComp);

    item = CALLOC(1, sizeof(*item));
    ogs_assert(item);
    ASN_SEQUENCE_ADD(E_RABReleaseListBearerRelComp, item);

    item->id = S1AP_ProtocolIE_ID_id_E_RABReleaseItemBearerRelComp;
    item->criticality = S1AP_Criticality_ignore;
    item->value.present =
        S1AP_E_RABReleaseItemBearerRelCompIEs__value_PR_E_RABReleaseItemBearerRelComp;

    item->value.choice.E_RABReleaseItemBearerRelComp =
        CALLOC(1, sizeof(*e_rab));
    e_rab = item->value.choice.E_RABReleaseItemBearerRelComp;
    ogs_assert(e_rab);

    e_rab->e_RAB_ID = bearer->ebi;

    return ogs_s1ap_encode(&pdu);
}

ogs_pkbuf_t *test_s1ap_build_e_rab_modification_indication(test_ue_t *test_ue)
{
    int rv;

    test_sess_t *sess = NULL;
    test_bearer_t *bearer = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_E_RABModificationIndication_t *E_RABModificationIndication = NULL;

    S1AP_E_RABModificationIndicationIEs_t *ie = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_E_RABToBeModifiedListBearerModInd_t *E_RABToBeModifiedListBearerModInd = NULL;

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_E_RABModificationIndication;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_E_RABModificationIndication;

    initiatingMessage->value.choice.E_RABModificationIndication =
        CALLOC(1, sizeof(*E_RABModificationIndication));

    E_RABModificationIndication = initiatingMessage->value.choice.E_RABModificationIndication;
    ogs_assert(E_RABModificationIndication);

    E_RABModificationIndication->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_E_RABModificationIndication);
    ogs_assert(E_RABModificationIndication->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABModificationIndication->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_E_RABModificationIndicationIEs__value_PR_MME_UE_S1AP_ID;
    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABModificationIndication->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_E_RABModificationIndicationIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(E_RABModificationIndication->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABToBeModifiedListBearerModInd;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_E_RABModificationIndicationIEs__value_PR_E_RABToBeModifiedListBearerModInd;

    ie->value.choice.E_RABToBeModifiedListBearerModInd =
        ogs_asn_calloc_constructed(
                &asn_DEF_S1AP_E_RABToBeModifiedListBearerModInd);
    E_RABToBeModifiedListBearerModInd =
            ie->value.choice.E_RABToBeModifiedListBearerModInd;
    ogs_assert(E_RABToBeModifiedListBearerModInd);

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;
    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ogs_list_for_each(&test_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            S1AP_E_RABToBeModifiedItemBearerModIndIEs_t *item = NULL;
            S1AP_E_RABToBeModifiedItemBearerModInd_t *e_rab = NULL;

            ogs_gtp2_f_teid_t f_teid;
            ogs_ip_t ip;
            int len;

            item = CALLOC(1,
                    sizeof(*item));
            ogs_assert(item);
            ASN_SEQUENCE_ADD(E_RABToBeModifiedListBearerModInd, item);

            item->id = S1AP_ProtocolIE_ID_id_E_RABToBeModifiedItemBearerModInd;
            item->criticality = S1AP_Criticality_reject;
            item->value.present =
                S1AP_E_RABToBeModifiedItemBearerModIndIEs__value_PR_E_RABToBeModifiedItemBearerModInd;

            item->value.choice.E_RABToBeModifiedItemBearerModInd =
                CALLOC(1, sizeof(*e_rab));
            e_rab = item->value.choice.E_RABToBeModifiedItemBearerModInd;
            ogs_assert(e_rab);

            e_rab->e_RAB_ID = bearer->ebi;

            rv = ogs_gtp2_sockaddr_to_f_teid(
                    bearer->enb_s1u_addr, bearer->enb_s1u_addr6, &f_teid, &len);
            ogs_assert(rv == OGS_OK);

            rv = ogs_gtp2_f_teid_to_ip(&f_teid, &ip);
            ogs_assert(rv == OGS_OK);

            rv = ogs_asn_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
            ogs_assert(rv == OGS_OK);
            ogs_asn_uint32_to_OCTET_STRING(
                    bearer->enb_s1u_teid, &e_rab->dL_GTP_TEID);
        }
    }

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
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_PathSwitchRequest;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_PathSwitchRequest;

    initiatingMessage->value.choice.PathSwitchRequest =
        CALLOC(1, sizeof(*PathSwitchRequest));
    PathSwitchRequest = initiatingMessage->value.choice.PathSwitchRequest;
    ogs_assert(PathSwitchRequest);

    PathSwitchRequest->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_PathSwitchRequest);
    ogs_assert(PathSwitchRequest->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_PathSwitchRequestIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABToBeSwitchedDLList;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_PathSwitchRequestIEs__value_PR_E_RABToBeSwitchedDLList;

    ie->value.choice.E_RABToBeSwitchedDLList =
        ogs_asn_calloc_constructed(
                &asn_DEF_S1AP_E_RABToBeSwitchedDLList);
    E_RABToBeSwitchedDLList = ie->value.choice.E_RABToBeSwitchedDLList;
    ogs_assert(E_RABToBeSwitchedDLList);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_SourceMME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_PathSwitchRequestIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_EUTRAN_CGI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_PathSwitchRequestIEs__value_PR_EUTRAN_CGI;

    ie->value.choice.EUTRAN_CGI =
        CALLOC(1, sizeof(*EUTRAN_CGI));
    EUTRAN_CGI = ie->value.choice.EUTRAN_CGI;
    ogs_assert(EUTRAN_CGI);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_TAI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_PathSwitchRequestIEs__value_PR_TAI;

    ie->value.choice.TAI =
        CALLOC(1, sizeof(*TAI));
    TAI = ie->value.choice.TAI;
    ogs_assert(TAI);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(PathSwitchRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_PathSwitchRequestIEs__value_PR_UESecurityCapabilities;

    ie->value.choice.UESecurityCapabilities =
        CALLOC(1, sizeof(*UESecurityCapabilities));
    UESecurityCapabilities = ie->value.choice.UESecurityCapabilities;
    ogs_assert(UESecurityCapabilities);

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;
    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ogs_list_for_each(&test_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            S1AP_E_RABToBeSwitchedDLItemIEs_t *item = NULL;
            S1AP_E_RABToBeSwitchedDLItem_t *e_rab = NULL;

            ogs_gtp2_f_teid_t f_teid;
            ogs_ip_t ip;
            int len;

            item = CALLOC(1, sizeof(*item));
            ogs_assert(item);
            ASN_SEQUENCE_ADD(E_RABToBeSwitchedDLList, item);

            item->id = S1AP_ProtocolIE_ID_id_E_RABToBeSwitchedDLItem;
            item->criticality = S1AP_Criticality_reject;
            item->value.present = S1AP_E_RABToBeSwitchedDLItemIEs__value_PR_E_RABToBeSwitchedDLItem;

            item->value.choice.E_RABToBeSwitchedDLItem =
                CALLOC(1, sizeof(*e_rab));
            e_rab = item->value.choice.E_RABToBeSwitchedDLItem;
            ogs_assert(e_rab);

            e_rab->e_RAB_ID = bearer->ebi;

            rv = ogs_gtp2_sockaddr_to_f_teid(
                    bearer->enb_s1u_addr, bearer->enb_s1u_addr6, &f_teid, &len);
            ogs_assert(rv == OGS_OK);

            rv = ogs_gtp2_f_teid_to_ip(&f_teid, &ip);
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

    uint8_t tmp[OGS_HUGE_LEN];
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
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_HandoverPreparation;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_HandoverRequired;

    initiatingMessage->value.choice.HandoverRequired =
        CALLOC(1, sizeof(*HandoverRequired));
    HandoverRequired = initiatingMessage->value.choice.HandoverRequired;
    ogs_assert(HandoverRequired);

    HandoverRequired->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_HandoverRequired);
    ogs_assert(HandoverRequired->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequired->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverRequiredIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequired->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverRequiredIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequired->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_HandoverType;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverRequiredIEs__value_PR_HandoverType;

    ie->value.choice.HandoverType = CALLOC(1, sizeof(*HandoverType));
    HandoverType = ie->value.choice.HandoverType;
    ogs_assert(HandoverType);

    *HandoverType = handover_type;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequired->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_HandoverRequiredIEs__value_PR_Cause;

    ie->value.choice.Cause =
        CALLOC(1, sizeof(*Cause));
    Cause = ie->value.choice.Cause;
    ogs_assert(Cause);

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequired->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_TargetID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverRequiredIEs__value_PR_TargetID;

    ie->value.choice.TargetID =
        CALLOC(1, sizeof(*TargetID));
    TargetID = ie->value.choice.TargetID;
    ogs_assert(TargetID);

    TargetID->present = S1AP_TargetID_PR_targeteNB_ID;
    targeteNB_ID = CALLOC(1, sizeof(*targeteNB_ID));
    ogs_assert(targeteNB_ID);
    TargetID->choice.targeteNB_ID = targeteNB_ID;

    targeteNB_ID->global_ENB_ID =
        CALLOC(1, sizeof(*global_ENB_ID));
    ogs_assert(targeteNB_ID->global_ENB_ID);
    global_ENB_ID = targeteNB_ID->global_ENB_ID;
    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_ue->e_tai.plmn_id, OGS_PLMN_ID_LEN,
            &global_ENB_ID->pLMNidentity);
    global_ENB_ID->eNB_ID =
        CALLOC(1, sizeof(*global_ENB_ID->eNB_ID));
    ogs_assert(global_ENB_ID->eNB_ID);
    ogs_s1ap_uint32_to_ENB_ID(present, enb_id, global_ENB_ID->eNB_ID);

    targeteNB_ID->selected_TAI =
        CALLOC(1, sizeof(*selected_TAI));
    ogs_assert(targeteNB_ID->selected_TAI);
    selected_TAI = targeteNB_ID->selected_TAI;
    ogs_asn_uint16_to_OCTET_STRING(test_ue->e_tai.tac, &selected_TAI->tAC);
    ogs_s1ap_buffer_to_OCTET_STRING(
            &test_ue->e_tai.plmn_id,
            OGS_PLMN_ID_LEN, &selected_TAI->pLMNidentity);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequired->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Source_ToTarget_TransparentContainer;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_HandoverRequiredIEs__value_PR_Source_ToTarget_TransparentContainer;

    ie->value.choice.Source_ToTarget_TransparentContainer =
        CALLOC(1, sizeof(*Source_ToTarget_TransparentContainer));

    Source_ToTarget_TransparentContainer =
        ie->value.choice.Source_ToTarget_TransparentContainer;
    ogs_assert(Source_ToTarget_TransparentContainer);

    ogs_hex_from_string(_container, tmp, sizeof(tmp));

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

    char hexbuf[OGS_HUGE_LEN];
    const char *payload =
        "00 80810bf900d8af40 00a0339057801f47 88009e81de2c20a4"
        "81de2c404a00ef16 2000010044013f21 2249008093efd243 3914cd2aa0a0142f"
        "f2214d6dfb82c194 0b10080000020040 bbfd55aeab41ad80 8fd50398381c08fd"
        "503983804d037868 baa016423342bc3a 18b58fa084ca833f a17970acfc10e84e"
        "0004f14550d00096 c88900";

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome =
        CALLOC(1, sizeof(*successfulOutcome));

    successfulOutcome = pdu.choice.successfulOutcome;
    ogs_assert(successfulOutcome);
    successfulOutcome->procedureCode = S1AP_ProcedureCode_id_HandoverResourceAllocation;
    successfulOutcome->criticality = S1AP_Criticality_reject;
    successfulOutcome->value.present =
        S1AP_SuccessfulOutcome__value_PR_HandoverRequestAcknowledge;

    successfulOutcome->value.choice.HandoverRequestAcknowledge =
        CALLOC(1, sizeof(*HandoverRequestAcknowledge));

    HandoverRequestAcknowledge = successfulOutcome->value.choice.HandoverRequestAcknowledge;
    ogs_assert(HandoverRequestAcknowledge);

    HandoverRequestAcknowledge->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_HandoverRequestAcknowledge);
    ogs_assert(HandoverRequestAcknowledge->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverRequestAcknowledgeIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverRequestAcknowledgeIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    test_ue->enb_ue_s1ap_id++;
    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABAdmittedList;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverRequestAcknowledgeIEs__value_PR_E_RABAdmittedList;

    ie->value.choice.E_RABAdmittedList =
        ogs_asn_calloc_constructed(&asn_DEF_S1AP_E_RABAdmittedList);
    E_RABAdmittedList = ie->value.choice.E_RABAdmittedList;
    ogs_assert(E_RABAdmittedList);
    ogs_list_for_each(&test_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            S1AP_E_RABAdmittedItemIEs_t *item = NULL;
            S1AP_E_RABAdmittedItem_t *e_rab = NULL;

            ogs_gtp2_f_teid_t f_teid;
            ogs_ip_t ip;
            int len;

            item = CALLOC(1, sizeof(*item));
            ogs_assert(item);
            ASN_SEQUENCE_ADD(E_RABAdmittedList, item);

            item->id = S1AP_ProtocolIE_ID_id_E_RABAdmittedItem;
            item->criticality = S1AP_Criticality_ignore;
            item->value.present =
                S1AP_E_RABAdmittedItemIEs__value_PR_E_RABAdmittedItem;

            item->value.choice.E_RABAdmittedItem =
                CALLOC(1, sizeof(*e_rab));
            e_rab = item->value.choice.E_RABAdmittedItem;
            ogs_assert(e_rab);

            e_rab->e_RAB_ID = bearer->ebi;

            rv = ogs_gtp2_sockaddr_to_f_teid(
                    bearer->enb_s1u_addr, bearer->enb_s1u_addr6, &f_teid, &len);
            ogs_assert(rv == OGS_OK);

            rv = ogs_gtp2_f_teid_to_ip(&f_teid, &ip);
            ogs_assert(rv == OGS_OK);

            rv = ogs_asn_ip_to_BIT_STRING(&ip, &e_rab->transportLayerAddress);
            ogs_assert(rv == OGS_OK);
            ogs_asn_uint32_to_OCTET_STRING(
                    bearer->enb_s1u_teid, &e_rab->gTP_TEID);

            e_rab->dL_transportLayerAddress =
                CALLOC(1, sizeof(*e_rab->dL_transportLayerAddress));
            ogs_assert(e_rab->dL_transportLayerAddress);
            rv = ogs_asn_ip_to_BIT_STRING(&ip, e_rab->dL_transportLayerAddress);
            ogs_assert(rv == OGS_OK);
            e_rab->dL_gTP_TEID = CALLOC(1, sizeof(*e_rab->dL_gTP_TEID));
            ogs_assert(e_rab->dL_gTP_TEID);
            ogs_asn_uint32_to_OCTET_STRING(
                    bearer->enb_s1u_teid+10, e_rab->dL_gTP_TEID);

            e_rab->uL_TransportLayerAddress =
                CALLOC(1, sizeof(*e_rab->uL_TransportLayerAddress));
            ogs_assert(e_rab->uL_TransportLayerAddress);
            rv = ogs_asn_ip_to_BIT_STRING(&ip, e_rab->uL_TransportLayerAddress);
            ogs_assert(rv == OGS_OK);
            e_rab->uL_GTP_TEID = CALLOC(1, sizeof(*e_rab->uL_GTP_TEID));
            ogs_assert(e_rab->uL_GTP_TEID);
            ogs_asn_uint32_to_OCTET_STRING(
                    bearer->enb_s1u_teid+20, e_rab->uL_GTP_TEID);
        }
    }

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverRequestAcknowledge->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Target_ToSource_TransparentContainer;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverRequestAcknowledgeIEs__value_PR_Target_ToSource_TransparentContainer;

    ie->value.choice.Target_ToSource_TransparentContainer =
        CALLOC(1, sizeof(*Target_ToSource_TransparentContainer));

    Target_ToSource_TransparentContainer =
        ie->value.choice.Target_ToSource_TransparentContainer;
    ogs_assert(Target_ToSource_TransparentContainer);

    ogs_s1ap_buffer_to_OCTET_STRING(
            ogs_hex_from_string(payload, hexbuf, sizeof(hexbuf)), 132,
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
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_eNBStatusTransfer;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_ENBStatusTransfer;

    initiatingMessage->value.choice.ENBStatusTransfer =
        CALLOC(1, sizeof(*ENBStatusTransfer));
    ENBStatusTransfer = initiatingMessage->value.choice.ENBStatusTransfer;
    ogs_assert(ENBStatusTransfer);

    ENBStatusTransfer->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_ENBStatusTransfer);
    ogs_assert(ENBStatusTransfer->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(ENBStatusTransfer->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_ENBStatusTransferIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(ENBStatusTransfer->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_ENBStatusTransferIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(ENBStatusTransfer->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_StatusTransfer_TransparentContainer;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_ENBStatusTransferIEs__value_PR_ENB_StatusTransfer_TransparentContainer;

    ie->value.choice.ENB_StatusTransfer_TransparentContainer =
        CALLOC(1, sizeof(*ENB_StatusTransfer_TransparentContainer));

    ENB_StatusTransfer_TransparentContainer =
        ie->value.choice.ENB_StatusTransfer_TransparentContainer;
    ogs_assert(ENB_StatusTransfer_TransparentContainer);

    ENB_StatusTransfer_TransparentContainer->
        bearers_SubjectToStatusTransferList =
            CALLOC(1, sizeof(*Bearers_SubjectToStatusTransferList));
    ogs_assert(ENB_StatusTransfer_TransparentContainer->
        bearers_SubjectToStatusTransferList);

    Bearers_SubjectToStatusTransferList =
        ENB_StatusTransfer_TransparentContainer->
        bearers_SubjectToStatusTransferList;

    ogs_list_for_each(&test_ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            ie2 = CALLOC(
                    1, sizeof(*ie2));
            ogs_assert(ie2);
            ASN_SEQUENCE_ADD(Bearers_SubjectToStatusTransferList, ie2);

            ie2->id =
                S1AP_ProtocolIE_ID_id_Bearers_SubjectToStatusTransfer_Item;
            ie2->criticality = S1AP_Criticality_ignore;
            ie2->value.present =
                S1AP_Bearers_SubjectToStatusTransfer_ItemIEs__value_PR_Bearers_SubjectToStatusTransfer_Item;

            ie2->value.choice.Bearers_SubjectToStatusTransfer_Item =
                CALLOC(1, sizeof(*item));
            item = ie2->value.choice.Bearers_SubjectToStatusTransfer_Item;
            ogs_assert(item);
            item->e_RAB_ID = bearer->ebi;
            item->uL_COUNTvalue =
                CALLOC(1, sizeof(*item->uL_COUNTvalue));
            ogs_assert(item->uL_COUNTvalue);
            item->dL_COUNTvalue =
                CALLOC(1, sizeof(*item->dL_COUNTvalue));
            ogs_assert(item->dL_COUNTvalue);
            item->uL_COUNTvalue->pDCP_SN = 75;
            item->dL_COUNTvalue->pDCP_SN = 17;
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
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_HandoverNotification;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_HandoverNotify;

    initiatingMessage->value.choice.HandoverNotify =
        CALLOC(1, sizeof(*HandoverNotify));
    HandoverNotify = initiatingMessage->value.choice.HandoverNotify;
    ogs_assert(HandoverNotify);

    HandoverNotify->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_HandoverNotify);
    ogs_assert(HandoverNotify->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverNotify->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverNotifyIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverNotify->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverNotifyIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverNotify->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_EUTRAN_CGI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_HandoverNotifyIEs__value_PR_EUTRAN_CGI;

    ie->value.choice.EUTRAN_CGI =
        CALLOC(1, sizeof(*EUTRAN_CGI));
    EUTRAN_CGI = ie->value.choice.EUTRAN_CGI;
    ogs_assert(EUTRAN_CGI);

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

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverNotify->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_TAI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_HandoverNotifyIEs__value_PR_TAI;

    ie->value.choice.TAI =
        CALLOC(1, sizeof(*TAI));
    TAI = ie->value.choice.TAI;
    ogs_assert(TAI);

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
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(*initiatingMessage));

    initiatingMessage = pdu.choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    initiatingMessage->procedureCode = S1AP_ProcedureCode_id_HandoverCancel;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_HandoverCancel;

    initiatingMessage->value.choice.HandoverCancel =
        CALLOC(1, sizeof(*HandoverCancel));
    HandoverCancel = initiatingMessage->value.choice.HandoverCancel;
    ogs_assert(HandoverCancel);

    HandoverCancel->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_HandoverCancel);
    ogs_assert(HandoverCancel->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverCancel->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverCancelIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverCancel->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_HandoverCancelIEs__value_PR_ENB_UE_S1AP_ID;

    ie->value.choice.ENB_UE_S1AP_ID = CALLOC(1, sizeof(*ENB_UE_S1AP_ID));
    ENB_UE_S1AP_ID = ie->value.choice.ENB_UE_S1AP_ID;
    ogs_assert(ENB_UE_S1AP_ID);

    *ENB_UE_S1AP_ID = test_ue->enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverCancel->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_HandoverCancelIEs__value_PR_Cause;

    ie->value.choice.Cause =
        CALLOC(1, sizeof(*Cause));
    Cause = ie->value.choice.Cause;
    ogs_assert(Cause);

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
        CALLOC(1, sizeof(*unsuccessfulOutcome));

    unsuccessfulOutcome = pdu.choice.unsuccessfulOutcome;
    ogs_assert(unsuccessfulOutcome);
    unsuccessfulOutcome->procedureCode =
        S1AP_ProcedureCode_id_HandoverResourceAllocation;
    unsuccessfulOutcome->criticality = S1AP_Criticality_reject;
    unsuccessfulOutcome->value.present =
        S1AP_UnsuccessfulOutcome__value_PR_HandoverFailure;

    unsuccessfulOutcome->value.choice.HandoverFailure =
        CALLOC(1, sizeof(*HandoverFailure));
    HandoverFailure = unsuccessfulOutcome->value.choice.HandoverFailure;
    ogs_assert(HandoverFailure);

    HandoverFailure->protocolIEs =
        ogs_asn_calloc_protocol_ies(&asn_DEF_S1AP_HandoverFailure);
    ogs_assert(HandoverFailure->protocolIEs);

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_HandoverFailureIEs__value_PR_MME_UE_S1AP_ID;

    ie->value.choice.MME_UE_S1AP_ID = CALLOC(1, sizeof(*MME_UE_S1AP_ID));
    MME_UE_S1AP_ID = ie->value.choice.MME_UE_S1AP_ID;
    ogs_assert(MME_UE_S1AP_ID);

    *MME_UE_S1AP_ID = test_ue->mme_ue_s1ap_id;

    ie = CALLOC(1, sizeof(*ie));
    ogs_assert(ie);
    ASN_SEQUENCE_ADD(HandoverFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_HandoverFailureIEs__value_PR_Cause;

    ie->value.choice.Cause =
        CALLOC(1, sizeof(*Cause));
    Cause = ie->value.choice.Cause;
    ogs_assert(Cause);

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    return ogs_s1ap_encode(&pdu);
}

#define TEST_S1AP_MAX_MESSAGE 64

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
    char hexbuf[OGS_HUGE_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
        ogs_hex_from_string(payload[i], hexbuf, sizeof(hexbuf)), len[i]);

    return pkbuf;
}

ogs_pkbuf_t *test_s1ap_build_enb_configuration_update(int i)
{
    ogs_pkbuf_t *pkbuf = NULL;
    const char *payload[TEST_S1AP_MAX_MESSAGE] = {
        "001d002a0000"
        "04003c4003000031 0040000700000040 34f2990089400140 0124400c00004ced"
        "a80000004034f299",
        "",
        "",

        "",
        "",
        "",

        "",
        "",
        "",

    };
    uint16_t len[TEST_S1AP_MAX_MESSAGE] = {
        46,
        0,
        0,

        0,
        0,
        0,

        0,
        0,
        0,
    };
    char hexbuf[OGS_HUGE_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
        ogs_hex_from_string(payload[i], hexbuf, sizeof(hexbuf)), len[i]);

    return pkbuf;
}

ogs_pkbuf_t *test_s1ap_build_malformed_s1_setup_request(int i)
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

    char hexbuf[OGS_HUGE_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
        ogs_hex_from_string(payload[i], hexbuf, sizeof(hexbuf)), len[i]);

    return pkbuf;
}

ogs_pkbuf_t *test_s1ap_build_malformed_enb_status_transfer(int i)
{
    ogs_pkbuf_t *pkbuf = NULL;
    const char *payload[TEST_S1AP_MAX_MESSAGE] = {
        "0018406100000a00b0400e05c000e815b337606bcf9ab42f180043400600bb31"
        "1d84c000f54001300058400480249995009240010000b0400e44d089e4b2a9d1"
        "a7e369b61c000a000840048036d48c000840048020cd4df30040014300084005"
        "c0d27e885a5001120c2b7d9501034007500198b6010101010101010101010101"
        "0101010101010101010101010101010101010101010101010101010101010101"
        "01230101010101010101010101010101010101100101010101fe010101010101"
        "0101010101010101010101010101010101010101010101010101010101010101"
        "0101010101010101010105010101010101010101010101010101010101010101"

        "0101010101010101010101010101010101010101010101010101010101010101"
        "0101016f40020110000240013600004005c0fe6eba000a8000df00d73c084000"
        "084000800d02800000400500000200afffff0504800b0e9c00004005c070217c"
        "db000840048042c1431702400110000101010101010101010101010101010101"
        "0101010101010101010101010101010101010101010101010101010101010101"
        "0101010101010101010101010101010101010101010101010101010101010901"
        "0101010101010101010101010101010101010101010101010101010101010101"
        "0101f80001010101010101010101010101010101010101010101010101010101"

        "0101010101010101010101010101010101010101010101010101010101010101"
        "0101010201010101010101010101010101010101010101010101010101010101"
        "010101011c010101010101010101010101010101010101010101010101010101"
        "010101010101010101010101010101010101010101010101ffffff7f01010101"
        "0101010101010101010101010101010101010101010101010101010101010101"
        "01010101ef010101010101010101010101010101010101010101010101010101"
        "0101010101010101010101010101010101010101010101010101010101020101"
        "0101010101800000000101010101010101010101010101010101010101010101"

        "0101010101010101010101010101010101010101010101010101010101010101"
        "01010101010101010101010101010101010101010101010302bb9a0042407901"
        "00dc721f8001f9f47b0a00054082cc802f48e000004005c098627f8800004005"
        "c06f86cead00084004803d7ca200004005c0db1ec0c10019408278408c010101"
        "0101010101010101010101010101010101000000000101010101010101010101"
        "0101010101010101010101010101010101010101010101010101010101010101"
        "0101010101010101010101010101010101010101010201010101010101010101"
        "0101010101010101010101010101010101010101010101010101010101010101"

        "0101010101010101010101010101010101010101010101010101010101010101"
        "0101010101010101010101010101010101010101010101010101010101010101"
        "010501010101010101010101010101",
        "",
    };

    uint16_t len[TEST_S1AP_MAX_MESSAGE] = {
        1103,
        0,
    };

    char hexbuf[OGS_HUGE_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
        ogs_hex_from_string(payload[i], hexbuf, sizeof(hexbuf)), len[i]);

    return pkbuf;
}

ogs_pkbuf_t *test_s1ap_build_oversized_message(int i)
{
#define OVERSIZE 52170
    ogs_pkbuf_t *pkbuf = NULL;
    const char *payload[TEST_S1AP_MAX_MESSAGE] = {
        "000808080800244080d500000a007740030000ed00754032953382b4d330d07a0cb8f03b189d6b2c3ced65ce2c7ac9c45f738edd8ec89e921dd700eba23ec48ee91c3ae85652000001000052141c50182e0123810f99d6782a05c9d4a48ffe7fc7a0d252141c50182e022300244061d500000a007740030000ed00754032913382b4d330d07a1cb8f03b189d6b2c3ced65ce2c7ac9c45f7371dd8ec89e921dd700eba235c48ee91c3afc564eb37c9776006f4002204f00754032f363d6782a7faad4a48ffe7fc7a0d252141c50182e02238191f2fcc95b330c2b97173272f3822e2050eb13c7aee0281463e5011e0030404500000a004a400100004a400101ad013a4003021199004a4001013a4001000176040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404f4040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040450505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050504d50505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505000020000505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050507f505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050507f5050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050509240010000b0400e44d089e4b2a9400e05c000e838b337606bcf9ab42f180043400600bb311d84c000f54001400008400480505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050504f50505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050455050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050504a505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050ffff7fff5050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050506a505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050ff5050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404ee03040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404170404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404046262626262626262626262626262626262626262626262626262626204040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404e20404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404045050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050504c5050505050505050000a0002400200a0005c40010000024002000000024001340002400130005c400100005c400100005c405a4009005b400448f6d327005b400428210359005b400428f3e848005b40096cfb44465080f8bc41005b40054c04a134ae005b40096c523cc91d80c01ffa005b400428e68e9d005b400428354c58005b400428332ab6005b400100000a400114005c4001005050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050509240010000b0400e44d089e4b2a9400e05c000e838b337606bcf9ab42f180043400600bb311d84c000f54001400008400480505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050504f50505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050504a505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050",
        "",
    };

    uint16_t len[TEST_S1AP_MAX_MESSAGE] = {
        OVERSIZE,
        0,
    };

    char *hexbuf = NULL;

    hexbuf = ogs_calloc(1, OVERSIZE);
    ogs_assert(hexbuf);

    pkbuf = ogs_pkbuf_alloc(NULL, OVERSIZE);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
        ogs_hex_from_string(payload[i], hexbuf, sizeof(hexbuf)), len[i]);

    ogs_free(hexbuf);

    return pkbuf;
}

ogs_pkbuf_t *test_s1ap_build_malformed_enb_direct_information_transfer(int i)
{
    ogs_pkbuf_t *pkbuf = NULL;
    const char *payload[TEST_S1AP_MAX_MESSAGE] = {
        "0025"
        "0110000001000000 1000008200000000 1000010000001000 0082000000001000"
        "00",
        "",
    };

    uint16_t len[TEST_S1AP_MAX_MESSAGE] = {
        35,
        0,
    };

    char hexbuf[OGS_HUGE_LEN];

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
        ogs_hex_from_string(payload[i], hexbuf, sizeof(hexbuf)), len[i]);

    return pkbuf;
}
