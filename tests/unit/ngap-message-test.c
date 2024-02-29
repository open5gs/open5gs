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

#include "ogs-ngap.h"
#include "core/abts.h"

static void ngap_message_test1(abts_case *tc, void *data)
{
    ogs_pkbuf_t *pkbuf = NULL;
    NGAP_NGAP_PDU_t pdu;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_NGResetAcknowledge_t *NGResetAcknowledge = NULL;

    NGAP_NGResetAcknowledgeIEs_t *ie = NULL;
    NGAP_UE_associatedLogicalNG_connectionList_t
        *UE_associatedLogicalNG_connectionList = NULL;

    ogs_ngap_message_t message, *struct_ptr = NULL;
    size_t struct_size;
    asn_dec_rval_t dec_ret = {0};

    uint64_t ran_ue_ngap_id;
    uint64_t amf_ue_ngap_id;

    memset(&pdu, 0, sizeof (NGAP_NGAP_PDU_t));
    pdu.present = NGAP_NGAP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome = CALLOC(1, sizeof(NGAP_SuccessfulOutcome_t));

    successfulOutcome = pdu.choice.successfulOutcome;
    successfulOutcome->procedureCode = NGAP_ProcedureCode_id_NGReset;
    successfulOutcome->criticality = NGAP_Criticality_reject;
    successfulOutcome->value.present =
        NGAP_SuccessfulOutcome__value_PR_NGResetAcknowledge;

    NGResetAcknowledge = &successfulOutcome->value.choice.NGResetAcknowledge;

    ie = CALLOC(1, sizeof(NGAP_NGResetAcknowledgeIEs_t));
    ASN_SEQUENCE_ADD(&NGResetAcknowledge->protocolIEs, ie);

    ie->id = NGAP_ProtocolIE_ID_id_UE_associatedLogicalNG_connectionList;
    ie->criticality = NGAP_Criticality_reject;
    ie->value.present = NGAP_NGResetAcknowledgeIEs__value_PR_UE_associatedLogicalNG_connectionList;

    UE_associatedLogicalNG_connectionList =
        &ie->value.choice.UE_associatedLogicalNG_connectionList;

    ran_ue_ngap_id = 1;
    amf_ue_ngap_id = 2;

    ogs_ngap_build_part_of_ng_interface(UE_associatedLogicalNG_connectionList,
            &ran_ue_ngap_id, &amf_ue_ngap_id);

    pkbuf = ogs_ngap_encode(&pdu);
    ogs_assert(pkbuf);

    struct_ptr = &message;
    struct_size = sizeof(ogs_ngap_message_t);

    memset(struct_ptr, 0, struct_size);
    dec_ret = aper_decode(NULL, &asn_DEF_NGAP_NGAP_PDU, (void **)&struct_ptr,
            pkbuf->data, pkbuf->len, 0, 0);
    ABTS_INT_EQUAL(tc, 0, dec_ret.code);
    ABTS_INT_EQUAL(tc, 128, dec_ret.consumed);

    ogs_ngap_free(&message);
    ogs_pkbuf_free(pkbuf);
}

static void ngap_message_test2(abts_case *tc, void *data)
{
    /* NGReset */
    const char *payload = "0014001300000200 0f400200c0005800 06400160010001";

    ogs_ngap_message_t message;
    ogs_pkbuf_t *pkbuf;
    int result;
    char hexbuf[OGS_HUGE_LEN];

    ogs_ngap_message_t *struct_ptr = NULL;
    size_t struct_size;
    asn_dec_rval_t dec_ret = {0};

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
            ogs_hex_from_string(payload, hexbuf, sizeof(hexbuf)), 23);

    struct_ptr = &message;
    struct_size = sizeof(ogs_ngap_message_t);

    memset(struct_ptr, 0, struct_size);
    dec_ret = aper_decode(NULL, &asn_DEF_NGAP_NGAP_PDU, (void **)&struct_ptr,
            pkbuf->data, pkbuf->len, 0, 0);
    ABTS_INT_EQUAL(tc, 0, dec_ret.code);
    ABTS_INT_EQUAL(tc, 184, dec_ret.consumed);

    ogs_ngap_free(&message);
    ogs_pkbuf_free(pkbuf);
}

static void ngap_message_test3(abts_case *tc, void *data)
{
    /* NGReset */
    const char *payload =
        "0015004200000500 1b00090009f10728 000800000052400b 0400354720674e42"
        "2d43550066000d00 000000010009f107 0000000800154001 0001114009403035"
        "484c41423032";

    ogs_ngap_message_t message;
    ogs_pkbuf_t *pkbuf;
    int result;
    char hexbuf[OGS_HUGE_LEN];

    ogs_ngap_message_t *struct_ptr = NULL;
    size_t struct_size;
    asn_dec_rval_t dec_ret = {0};

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
            ogs_hex_from_string(payload, hexbuf, sizeof(hexbuf)), 70);

    struct_ptr = &message;
    struct_size = sizeof(ogs_ngap_message_t);

    memset(struct_ptr, 0, struct_size);
    dec_ret = aper_decode(NULL, &asn_DEF_NGAP_NGAP_PDU, (void **)&struct_ptr,
            pkbuf->data, pkbuf->len, 0, 0);
    ABTS_INT_EQUAL(tc, 0, dec_ret.code);
    ABTS_INT_EQUAL(tc, 560, dec_ret.consumed);

    ogs_ngap_free(&message);
    ogs_pkbuf_free(pkbuf);
}

static void ngap_message_test4(abts_case *tc, void *data)
{
    /* NGReset */
    const char *payload =
        "0015003f00000500 1b00080045f01000 0000040052400903 0035484c41423032"
        "0066000d00000062 280045f010000000 0800154001400111 4009203035484c41"
        "423032";

    ogs_ngap_message_t message;
    ogs_pkbuf_t *pkbuf;
    int result;
    char hexbuf[OGS_HUGE_LEN];

    ogs_ngap_message_t *struct_ptr = NULL;
    size_t struct_size;
    asn_dec_rval_t dec_ret = {0};

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
            ogs_hex_from_string(payload, hexbuf, sizeof(hexbuf)), 67);

    struct_ptr = &message;
    struct_size = sizeof(ogs_ngap_message_t);

    memset(struct_ptr, 0, struct_size);
    dec_ret = aper_decode(NULL, &asn_DEF_NGAP_NGAP_PDU, (void **)&struct_ptr,
            pkbuf->data, pkbuf->len, 0, 0);
    ABTS_INT_EQUAL(tc, 0, dec_ret.code);
    ABTS_INT_EQUAL(tc, 536, dec_ret.consumed);

    ogs_ngap_free(&message);
    ogs_pkbuf_free(pkbuf);
}

static ogs_pkbuf_t *build_uplink_nas_transport(
        uint64_t ran_ue_ngap_id, uint64_t amf_ue_ngap_id, ogs_pkbuf_t *gmmbuf)
{
    const char *payload =
        "7e005c00 0d0199f9 07f0ff00 00000020"
        "3190";
    char hexbuf[OGS_HUGE_LEN];

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

    ogs_nr_cgi_t nr_cgi;
    ogs_5gs_tai_t nr_tai;

    ogs_assert(gmmbuf);

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

    asn_uint642INTEGER(AMF_UE_NGAP_ID, amf_ue_ngap_id);
    *RAN_UE_NGAP_ID = ran_ue_ngap_id;

    NAS_PDU->size = gmmbuf->len;
    NAS_PDU->buf = CALLOC(NAS_PDU->size, sizeof(uint8_t));
    memcpy(NAS_PDU->buf, gmmbuf->data, NAS_PDU->size);
    ogs_pkbuf_free(gmmbuf);

    userLocationInformationNR =
            CALLOC(1, sizeof(NGAP_UserLocationInformationNR_t));

    nR_CGI = &userLocationInformationNR->nR_CGI;
    ogs_ngap_nr_cgi_to_ASN(&nr_cgi, nR_CGI);

    tAI = &userLocationInformationNR->tAI;
    ogs_ngap_5gs_tai_to_ASN(&nr_tai, tAI);

    UserLocationInformation->present =
        NGAP_UserLocationInformation_PR_userLocationInformationNR;
    UserLocationInformation->choice.userLocationInformationNR =
        userLocationInformationNR;

    return ogs_ngap_encode(&pdu);
}

static void ngap_message_test5_issues2934(abts_case *tc, void *data)
{
    const char *payload =
        "7e005c00 0d0199f9 07f0ff00 00000020"
        "3190";
    ogs_pkbuf_t *gmmbuf = NULL;
    ogs_pkbuf_t *ngapbuf = NULL;
    char hexbuf[OGS_HUGE_LEN];

    ogs_ngap_message_t message, *struct_ptr = NULL;
    size_t struct_size;
    asn_dec_rval_t dec_ret = {0};

    int i;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_DownlinkNASTransport_t *DownlinkNASTransport = NULL;

    NGAP_DownlinkNASTransport_IEs_t *ie = NULL;
    NGAP_AMF_UE_NGAP_ID_t *AMF_UE_NGAP_ID = NULL;
    NGAP_RAN_UE_NGAP_ID_t *RAN_UE_NGAP_ID = NULL;
    NGAP_NAS_PDU_t *NAS_PDU = NULL;

    uint64_t amf_ue_ngap_id;

#define NGAPBUF_SIZE 18

    gmmbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(gmmbuf);
    ogs_pkbuf_put_data(gmmbuf,
            ogs_hex_from_string(payload, hexbuf, sizeof(hexbuf)), NGAPBUF_SIZE);

    ngapbuf = build_uplink_nas_transport(1, 0xffffffff, gmmbuf);
    ABTS_PTR_NOTNULL(tc, ngapbuf);
    ogs_pkbuf_free(ngapbuf);

    gmmbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(gmmbuf);
    ogs_pkbuf_put_data(gmmbuf,
            ogs_hex_from_string(payload, hexbuf, sizeof(hexbuf)), NGAPBUF_SIZE);

    ngapbuf = build_uplink_nas_transport(1, 0xffffffffff, gmmbuf);
    ABTS_PTR_NOTNULL(tc, ngapbuf);

    struct_ptr = &message;
    struct_size = sizeof(ogs_ngap_message_t);

    memset(struct_ptr, 0, struct_size);
    dec_ret = aper_decode(NULL, &asn_DEF_NGAP_NGAP_PDU, (void **)&struct_ptr,
            ngapbuf->data, ngapbuf->len, 0, 0);
    ABTS_INT_EQUAL(tc, 0, dec_ret.code);
    ABTS_INT_EQUAL(tc, ngapbuf->len * 8, dec_ret.consumed);

    initiatingMessage = message.choice.initiatingMessage;
    DownlinkNASTransport =
        &initiatingMessage->value.choice.DownlinkNASTransport;

    for (i = 0; i < DownlinkNASTransport->protocolIEs.list.count; i++) {
        ie = DownlinkNASTransport->protocolIEs.list.array[i];
        switch (ie->id) {
        case NGAP_ProtocolIE_ID_id_AMF_UE_NGAP_ID:
            AMF_UE_NGAP_ID = &ie->value.choice.AMF_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_RAN_UE_NGAP_ID:
            RAN_UE_NGAP_ID = &ie->value.choice.RAN_UE_NGAP_ID;
            break;
        case NGAP_ProtocolIE_ID_id_NAS_PDU:
            NAS_PDU = &ie->value.choice.NAS_PDU;
            break;
        default:
            break;
        }
    }

    ABTS_PTR_NOTNULL(tc, AMF_UE_NGAP_ID);
    asn_INTEGER2uint64(AMF_UE_NGAP_ID, &amf_ue_ngap_id);
    ABTS_TRUE(tc, 0xffffffffff == amf_ue_ngap_id);
    ABTS_PTR_NOTNULL(tc, RAN_UE_NGAP_ID);
    ABTS_INT_EQUAL(tc, 1, *RAN_UE_NGAP_ID);
    ABTS_PTR_NOTNULL(tc, NAS_PDU);

    ogs_ngap_free(&message);

    ogs_pkbuf_free(ngapbuf);
}

abts_suite *test_ngap_message(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    ogs_log_install_domain(&__ogs_ngap_domain, "ngap", OGS_LOG_ERROR);

    abts_run_test(suite, ngap_message_test1, NULL);
    abts_run_test(suite, ngap_message_test2, NULL);
    abts_run_test(suite, ngap_message_test3, NULL);
    abts_run_test(suite, ngap_message_test4, NULL);
    abts_run_test(suite, ngap_message_test5_issues2934, NULL);

    return suite;
}
