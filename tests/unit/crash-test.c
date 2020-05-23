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

#include "test-packet.h"

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
    ogs_pkbuf_put_data(enb_pkbuf, 
            OGS_HEX(payload, strlen(payload), hexbuf), 44);

    result = ogs_s1ap_decode(&message, enb_pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);

    test_parse_enb_configuration_transfer(&message);
    ogs_s1ap_free(&message);

    ogs_pkbuf_free(enb_pkbuf);
}

abts_suite *test_crash(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test2_func, NULL);
    abts_run_test(suite, test3_func, NULL);

    return suite;
}
