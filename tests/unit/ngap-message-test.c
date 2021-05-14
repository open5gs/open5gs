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

    uint32_t ran_ue_ngap_id;
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
    char hexbuf[OGS_MAX_SDU_LEN];

    ogs_ngap_message_t *struct_ptr = NULL;
    size_t struct_size;
    asn_dec_rval_t dec_ret = {0};

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
            OGS_HEX(payload, strlen(payload), hexbuf), 23);

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
    char hexbuf[OGS_MAX_SDU_LEN];

    ogs_ngap_message_t *struct_ptr = NULL;
    size_t struct_size;
    asn_dec_rval_t dec_ret = {0};

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
            OGS_HEX(payload, strlen(payload), hexbuf), 70);

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
    char hexbuf[OGS_MAX_SDU_LEN];

    ogs_ngap_message_t *struct_ptr = NULL;
    size_t struct_size;
    asn_dec_rval_t dec_ret = {0};

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf,
            OGS_HEX(payload, strlen(payload), hexbuf), 67);

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

abts_suite *test_ngap_message(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    ogs_log_install_domain(&__ogs_ngap_domain, "ngap", OGS_LOG_ERROR);

    abts_run_test(suite, ngap_message_test1, NULL);
    abts_run_test(suite, ngap_message_test2, NULL);
    abts_run_test(suite, ngap_message_test3, NULL);
    abts_run_test(suite, ngap_message_test4, NULL);

    return suite;
}
