/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-s1ap.h"
#include "ogs-crypt.h"
#include "core/abts.h"

static void test1_func(abts_case *tc, void *data)
{
    int rv;

    ogs_pkbuf_t *s1apbuf = NULL;

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

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = 
        CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_InitialContextSetup;
    initiatingMessage->criticality = S1AP_Criticality_reject;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_InitialContextSetupRequest;

    InitialContextSetupRequest =
        &initiatingMessage->value.choice.InitialContextSetupRequest;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_MME_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_MME_UE_S1AP_ID;

    MME_UE_S1AP_ID = &ie->value.choice.MME_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_uEaggregateMaximumBitrate;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_UEAggregateMaximumBitrate;

    UEAggregateMaximumBitrate = &ie->value.choice.UEAggregateMaximumBitrate;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_E_RABToBeSetupListCtxtSUReq;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
    S1AP_InitialContextSetupRequestIEs__value_PR_E_RABToBeSetupListCtxtSUReq;

    E_RABToBeSetupListCtxtSUReq = &ie->value.choice.E_RABToBeSetupListCtxtSUReq;

    *MME_UE_S1AP_ID = 1;
    *ENB_UE_S1AP_ID = 1;

    asn_uint642INTEGER(
            &UEAggregateMaximumBitrate->uEaggregateMaximumBitRateUL, 1);
    asn_uint642INTEGER(
            &UEAggregateMaximumBitrate->uEaggregateMaximumBitRateDL, 1);

    {
        S1AP_E_RABToBeSetupItemCtxtSUReqIEs_t *item = NULL;
        S1AP_E_RABToBeSetupItemCtxtSUReq_t *e_rab = NULL;
        S1AP_GBR_QosInformation_t *gbrQosInformation = NULL;
        S1AP_NAS_PDU_t *nasPdu = NULL;
        ogs_ip_t sgw_s1u_ip;
        uint32_t sgw_s1u_teid = 1;

        item = CALLOC(
                1, sizeof(S1AP_E_RABToBeSetupItemCtxtSUReqIEs_t));
        ASN_SEQUENCE_ADD(&E_RABToBeSetupListCtxtSUReq->list, item);

        item->id = S1AP_ProtocolIE_ID_id_E_RABToBeSetupItemCtxtSUReq;
        item->criticality = S1AP_Criticality_reject;
        item->value.present = S1AP_E_RABToBeSetupItemCtxtSUReqIEs__value_PR_E_RABToBeSetupItemCtxtSUReq;

        e_rab = &item->value.choice.E_RABToBeSetupItemCtxtSUReq;

        e_rab->e_RAB_ID = 5;
        e_rab->e_RABlevelQoSParameters.qCI = 1;

        e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
            priorityLevel = 10;
        e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
            pre_emptionCapability = 1;
        e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
            pre_emptionVulnerability = 1;

        gbrQosInformation = 
                CALLOC(1, sizeof(struct S1AP_GBR_QosInformation));
        asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateDL, 1);
        asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateUL, 1);
        asn_uint642INTEGER(&gbrQosInformation->e_RAB_GuaranteedBitrateDL, 1);
        asn_uint642INTEGER(&gbrQosInformation->e_RAB_GuaranteedBitrateUL, 1);
        e_rab->e_RABlevelQoSParameters.gbrQosInformation =
                gbrQosInformation;

        memset(&sgw_s1u_ip, 0, sizeof(sgw_s1u_ip));
        sgw_s1u_ip.ipv4 = 1;
        sgw_s1u_ip.ipv6 = 0;
        rv = ogs_asn_ip_to_BIT_STRING(
                &sgw_s1u_ip, &e_rab->transportLayerAddress);
        ogs_asn_uint32_to_OCTET_STRING(sgw_s1u_teid, &e_rab->gTP_TEID);
    }

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_UESecurityCapabilities;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_UESecurityCapabilities;

    UESecurityCapabilities = &ie->value.choice.UESecurityCapabilities;

    UESecurityCapabilities->encryptionAlgorithms.size = 2;
    UESecurityCapabilities->encryptionAlgorithms.buf = 
        CALLOC(UESecurityCapabilities->encryptionAlgorithms.size, 
                    sizeof(uint8_t));
    UESecurityCapabilities->encryptionAlgorithms.bits_unused = 0;

    UESecurityCapabilities->integrityProtectionAlgorithms.size = 2;
    UESecurityCapabilities->integrityProtectionAlgorithms.buf =
        CALLOC(UESecurityCapabilities->
                        integrityProtectionAlgorithms.size, sizeof(uint8_t));
    UESecurityCapabilities->integrityProtectionAlgorithms.bits_unused = 0;

    ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
    ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_SecurityKey;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_InitialContextSetupRequestIEs__value_PR_SecurityKey;

    SecurityKey = &ie->value.choice.SecurityKey;

    SecurityKey->size = OGS_SHA256_DIGEST_SIZE;
    SecurityKey->buf = 
        CALLOC(SecurityKey->size, sizeof(uint8_t));
    SecurityKey->bits_unused = 0;

        /* Set CS-Fallback */
    {
        S1AP_CSFallbackIndicator_t *CSFallbackIndicator = NULL;
        S1AP_LAI_t *LAI = NULL;
        ogs_plmn_id_t plmn_id;
        uint16_t lac = 1;

        ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_CSFallbackIndicator;
        ie->criticality = S1AP_Criticality_reject;
        ie->value.present =
            S1AP_InitialContextSetupRequestIEs__value_PR_CSFallbackIndicator;

        CSFallbackIndicator = &ie->value.choice.CSFallbackIndicator;
        ogs_assert(CSFallbackIndicator);

        *CSFallbackIndicator = S1AP_CSFallbackIndicator_cs_fallback_required;

        ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_RegisteredLAI;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_InitialContextSetupRequestIEs__value_PR_LAI;

        LAI = &ie->value.choice.LAI;
        ogs_assert(LAI);

        ogs_s1ap_buffer_to_OCTET_STRING(&plmn_id, sizeof(ogs_plmn_id_t),
                &LAI->pLMNidentity);
        ogs_asn_uint16_to_OCTET_STRING(lac, &LAI->lAC);

    }
    
    {
        char buf[1024];
        int size = 540;
        /* Set UeRadioCapability if exists */
        S1AP_UERadioCapability_t *UERadioCapability = NULL;

        ie = CALLOC(1, sizeof(S1AP_InitialContextSetupRequestIEs_t));
        ASN_SEQUENCE_ADD(&InitialContextSetupRequest->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_id_UERadioCapability;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present =
            S1AP_InitialContextSetupRequestIEs__value_PR_UERadioCapability;

        UERadioCapability = &ie->value.choice.UERadioCapability;

        ogs_s1ap_buffer_to_OCTET_STRING(buf, size, UERadioCapability);
    }

    s1apbuf = ogs_s1ap_encode(&pdu);
    ogs_pkbuf_free(s1apbuf);

    ABTS_INT_EQUAL(tc, OGS_OK, rv);
}

static int test_build_mme_configuration_transfer(
        S1AP_SONConfigurationTransfer_t *son_configuration_transfer)
{
    int rv;
    ogs_pkbuf_t *s1apbuf;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_MMEConfigurationTransfer_t *MMEConfigurationTransfer = NULL;

    S1AP_MMEConfigurationTransferIEs_t *ie = NULL;
    S1AP_SONConfigurationTransfer_t *SONConfigurationTransfer = NULL;

    ogs_assert(son_configuration_transfer);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = 
        CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

    initiatingMessage = pdu.choice.initiatingMessage;
    initiatingMessage->procedureCode =
        S1AP_ProcedureCode_id_MMEConfigurationTransfer;
    initiatingMessage->criticality = S1AP_Criticality_ignore;
    initiatingMessage->value.present =
        S1AP_InitiatingMessage__value_PR_MMEConfigurationTransfer;

    MMEConfigurationTransfer =
        &initiatingMessage->value.choice.MMEConfigurationTransfer;

    ie = CALLOC(1, sizeof(S1AP_MMEConfigurationTransferIEs_t));
    ASN_SEQUENCE_ADD(&MMEConfigurationTransfer->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_SONConfigurationTransferMCT;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present =
        S1AP_MMEConfigurationTransferIEs__value_PR_SONConfigurationTransfer;

    SONConfigurationTransfer = &ie->value.choice.SONConfigurationTransfer;

    rv = ogs_asn_copy_ie(&asn_DEF_S1AP_SONConfigurationTransfer,
            son_configuration_transfer, SONConfigurationTransfer);
    ogs_assert(rv == OGS_OK);

    s1apbuf = ogs_s1ap_encode(&pdu);
    ogs_pkbuf_free(s1apbuf);

    return OGS_OK;
}

static void test_parse_enb_configuration_transfer(
        ogs_s1ap_message_t *message)
{
    int rv;
    char buf[OGS_ADDRSTRLEN];
    int i;

    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_ENBConfigurationTransfer_t *ENBConfigurationTransfer = NULL;

    S1AP_ENBConfigurationTransferIEs_t *ie = NULL;
    S1AP_SONConfigurationTransfer_t *SONConfigurationTransfer = NULL;

    ogs_assert(message);
    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    ENBConfigurationTransfer =
        &initiatingMessage->value.choice.ENBConfigurationTransfer;
    ogs_assert(ENBConfigurationTransfer);

    for (i = 0; i < ENBConfigurationTransfer->protocolIEs.list.count; i++) {
        ie = ENBConfigurationTransfer->protocolIEs.list.array[i];
        switch (ie->id) {
        case S1AP_ProtocolIE_ID_id_SONConfigurationTransferECT:
            SONConfigurationTransfer =
                &ie->value.choice.SONConfigurationTransfer;
            break;
        default:
            break;
        }
    }

    if (SONConfigurationTransfer) {
        test_build_mme_configuration_transfer(SONConfigurationTransfer);
    }
}

static void test2_func(abts_case *tc, void *data)
{
    const char *payload = 
        "0028"
        "4022000001008140 1b0009f124000000 1009f12458ac0009 f1240000002009f1"
        "2458ac00";

    ogs_s1ap_message_t message;
    ogs_pkbuf_t *enb_pkbuf;
    int result;
    char hexbuf[OGS_MAX_SDU_LEN];

    enb_pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(enb_pkbuf);
    ogs_pkbuf_put_data(enb_pkbuf, 
            OGS_HEX(payload, strlen(payload), hexbuf), 38);

    result = ogs_s1ap_decode(&message, enb_pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);

    test_parse_enb_configuration_transfer(&message);
    ogs_s1ap_free(&message);

    ogs_pkbuf_free(enb_pkbuf);
}

static void test3_func(abts_case *tc, void *data)
{
    const char *payload = 
        "0028"
        "4028000001008140 210009f124000000 2009f12458ac0009 f1240000001009f1"
        "2458ac500f80c0a8 683b";

    S1AP_SONConfigurationTransfer_t SONConfigurationTransfer;
    ogs_s1ap_message_t message;
    ogs_pkbuf_t *enb_pkbuf;
    int result;
    char hexbuf[OGS_MAX_SDU_LEN];

    enb_pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(enb_pkbuf);
    ogs_pkbuf_put_data(enb_pkbuf, 
            OGS_HEX(payload, strlen(payload), hexbuf), 44);

    result = ogs_s1ap_decode(&message, enb_pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);

    test_parse_enb_configuration_transfer(&message);
    ogs_s1ap_free(&message);

    ogs_pkbuf_free(enb_pkbuf);
}

static void test4_func(abts_case *tc, void *data)
{
    const char *payload =
        "000d"
        "4037000005000000 0200640008000200 01001a000e0d277a 6f1f5b0107430003"
        "5200c20064400800 32f5400020001000 4340060032f54000 01";

    ogs_s1ap_message_t message;
    ogs_pkbuf_t *enb_pkbuf;
    int result;
    char hexbuf[OGS_MAX_SDU_LEN];

    enb_pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(enb_pkbuf);
    ogs_pkbuf_put_data(enb_pkbuf,
            OGS_HEX(payload, strlen(payload), hexbuf), 59);

    result = ogs_s1ap_decode(&message, enb_pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);

    ogs_s1ap_free(&message);
    ogs_pkbuf_free(enb_pkbuf);
}

static void test5_func(abts_case *tc, void *data)
{
    const char *payload =
        "000d404400000600 0000020001000800 020001001a000e0d 2728e58e6d010743"
        "00035200c2006440 080009f107000200 10004340060009f1 0700010120400900"
        "09f1070012345020";

    ogs_s1ap_message_t message;
    ogs_pkbuf_t *enb_pkbuf;
    int result;
    char hexbuf[OGS_MAX_SDU_LEN];

    enb_pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(enb_pkbuf);
    ogs_pkbuf_put_data(enb_pkbuf,
            OGS_HEX(payload, strlen(payload), hexbuf), 72);

    result = ogs_s1ap_decode(&message, enb_pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);

    ogs_s1ap_free(&message);
    ogs_pkbuf_free(enb_pkbuf);
}

static ogs_pkbuf_t *test_build_uplink_nas_transport(
        uint32_t mme_ue_s1ap_id, uint32_t enb_ue_s1ap_id, ogs_pkbuf_t *emmbuf)
{
    ogs_pkbuf_t *s1apbuf = NULL;

    S1AP_S1AP_PDU_t pdu;
    S1AP_InitiatingMessage_t *initiatingMessage = NULL;
    S1AP_UplinkNASTransport_t *UplinkNASTransport = NULL;

    S1AP_UplinkNASTransport_IEs_t *ie = NULL;
    S1AP_MME_UE_S1AP_ID_t *MME_UE_S1AP_ID = NULL;
    S1AP_ENB_UE_S1AP_ID_t *ENB_UE_S1AP_ID = NULL;
    S1AP_NAS_PDU_t *NAS_PDU = NULL;
    S1AP_EUTRAN_CGI_t *EUTRAN_CGI = NULL;
    S1AP_TAI_t *TAI = NULL;
    S1AP_PSCellInformation_t *PSCellInformation = NULL;
    S1AP_NR_CGI_t *nCGI = NULL;

    ogs_plmn_id_t plmn_id;
    uint32_t e_cell_id; /* 28 bit */
    uint64_t nr_cell_id; /* 36 bit */
    uint16_t tac;

    ogs_assert(emmbuf);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage =
        CALLOC(1, sizeof(S1AP_InitiatingMessage_t));

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

    ie = CALLOC(1, sizeof(S1AP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_eNB_UE_S1AP_ID;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present =
        S1AP_UplinkNASTransport_IEs__value_PR_ENB_UE_S1AP_ID;

    ENB_UE_S1AP_ID = &ie->value.choice.ENB_UE_S1AP_ID;

    ie = CALLOC(1, sizeof(S1AP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_NAS_PDU;
    ie->criticality = S1AP_Criticality_reject;
    ie->value.present = S1AP_UplinkNASTransport_IEs__value_PR_NAS_PDU;

    NAS_PDU = &ie->value.choice.NAS_PDU;

    NAS_PDU->size = emmbuf->len;
    NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
    memcpy(NAS_PDU->buf, emmbuf->data, NAS_PDU->size);
    ogs_pkbuf_free(emmbuf);

    *MME_UE_S1AP_ID = mme_ue_s1ap_id;
    *ENB_UE_S1AP_ID = enb_ue_s1ap_id;

    ie = CALLOC(1, sizeof(S1AP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_EUTRAN_CGI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_UplinkNASTransport_IEs__value_PR_EUTRAN_CGI;

    EUTRAN_CGI = &ie->value.choice.EUTRAN_CGI;

    ogs_plmn_id_build(&plmn_id, 901, 70, 2);

    ogs_s1ap_buffer_to_OCTET_STRING(
            &plmn_id, OGS_PLMN_ID_LEN, &EUTRAN_CGI->pLMNidentity);
    EUTRAN_CGI->cell_ID.size = 4;
    EUTRAN_CGI->cell_ID.buf =  CALLOC(
         EUTRAN_CGI->cell_ID.size, sizeof(uint8_t));
    ogs_assert(EUTRAN_CGI->cell_ID.buf);

    e_cell_id = 0x20010;

    EUTRAN_CGI->cell_ID.buf[0] = (e_cell_id >> 24);
    EUTRAN_CGI->cell_ID.buf[1] = (e_cell_id >> 16);
    EUTRAN_CGI->cell_ID.buf[2] = (e_cell_id >> 8);
    EUTRAN_CGI->cell_ID.buf[3] = (e_cell_id);
    EUTRAN_CGI->cell_ID.bits_unused = 4;

    ie = CALLOC(1, sizeof(S1AP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_TAI;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_UplinkNASTransport_IEs__value_PR_TAI;

    TAI = &ie->value.choice.TAI;

    tac = 1;
    ogs_asn_uint16_to_OCTET_STRING(tac, &TAI->tAC);
    ogs_s1ap_buffer_to_OCTET_STRING(
            &plmn_id, OGS_PLMN_ID_LEN, &TAI->pLMNidentity);

    ie = CALLOC(1, sizeof(S1AP_UplinkNASTransport_IEs_t));
    ASN_SEQUENCE_ADD(&UplinkNASTransport->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_id_PSCellInformation;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1AP_UplinkNASTransport_IEs__value_PR_PSCellInformation;

    PSCellInformation = &ie->value.choice.PSCellInformation;
    nCGI = &PSCellInformation->nCGI;

    ogs_s1ap_buffer_to_OCTET_STRING(
            &plmn_id, OGS_PLMN_ID_LEN, &nCGI->pLMNIdentity);

    nr_cell_id = 0x0012345020;
    nCGI->nRCellIdentity.size = 5;
    nCGI->nRCellIdentity.buf =  CALLOC(
         nCGI->nRCellIdentity.size, sizeof(uint8_t));
    ogs_assert(nCGI->nRCellIdentity.buf);
    nCGI->nRCellIdentity.buf[0] = (nr_cell_id >> 32);
    nCGI->nRCellIdentity.buf[1] = (nr_cell_id >> 24);
    nCGI->nRCellIdentity.buf[2] = (nr_cell_id >> 16);
    nCGI->nRCellIdentity.buf[3] = (nr_cell_id >> 8);
    nCGI->nRCellIdentity.buf[4] = (nr_cell_id);
    nCGI->nRCellIdentity.bits_unused = 4;

    return ogs_s1ap_encode(&pdu);
}

static void test6_func(abts_case *tc, void *data)
{
    const char *nas_payload = "2728 e58e6d0107430003 5200c2";

    ogs_s1ap_message_t message;
    int result;

    ogs_pkbuf_t *s1apbuf = NULL;
    ogs_pkbuf_t *emmbuf = NULL;
    char hexbuf[OGS_MAX_SDU_LEN];

    emmbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(emmbuf);
    ogs_pkbuf_put_data(emmbuf,
            OGS_HEX(nas_payload, strlen(nas_payload), hexbuf), 13);

    s1apbuf = test_build_uplink_nas_transport(1, 1, emmbuf);
    ABTS_PTR_NOTNULL(tc, s1apbuf);

    result = ogs_s1ap_decode(&message, s1apbuf);
    ABTS_INT_EQUAL(tc, 0, result);

    ogs_s1ap_free(&message);
    ogs_pkbuf_free(s1apbuf);
}

abts_suite *test_crash(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test2_func, NULL);
    abts_run_test(suite, test3_func, NULL);
    abts_run_test(suite, test4_func, NULL);
    abts_run_test(suite, test5_func, NULL);
    abts_run_test(suite, test6_func, NULL);

    return suite;
}
