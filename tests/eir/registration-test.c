/*
 * Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
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

#include "integration.h"

typedef struct registration_case_s {
    const char *name;
    const char *generic_status;
    const char *specific_status;
    bool other_supi;
    uint8_t reject_cause;
} registration_case_t;

static const registration_case_t registration_cases[] = {
    { "whitelist registration", "WHITELISTED", NULL, false, 0 },
    { "greylist registration", "GREYLISTED", NULL, false, 0 },
    { "blacklist rejects registration", "BLACKLISTED", NULL, false,
        OGS_5GMM_CAUSE_ILLEGAL_ME },
    { "unknown equipment rejects registration", NULL, NULL, false,
        OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED },
    { "specific blacklist overrides generic whitelist",
        "WHITELISTED", "BLACKLISTED", false, OGS_5GMM_CAUSE_ILLEGAL_ME },
    { "specific whitelist overrides generic blacklist",
        "BLACKLISTED", "WHITELISTED", false, 0 },
    { "different SUPI falls back to generic whitelist",
        "WHITELISTED", "BLACKLISTED", true, 0 },
    { "EIR server failure rejects registration", "INVALID", NULL, false,
        OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED },
};

typedef struct registration_fixture_s {
    mongoc_collection_t *collection[3];
    bson_oid_t id[3];
    unsigned int count;
} registration_fixture_t;

/* Refuse existing identities; the test must never replace another fixture. */
static bool identity_unused(abts_case *tc, mongoc_collection_t *collection,
        const char *key, const char *value)
{
    bson_t *query = BCON_NEW(key, BCON_UTF8(value));
    mongoc_cursor_t *cursor;
    const bson_t *document;
    bson_error_t error;
    bool found, failed;

    ogs_assert(query);
    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
    ogs_assert(cursor);
    found = mongoc_cursor_next(cursor, &document);
    failed = mongoc_cursor_error(cursor, &error);
    if (failed) {
        ABTS_FAIL(tc, error.message);
    } else if (found) {
        ABTS_FAIL(tc, "EIR registration fixture identity already exists");
    }
    mongoc_cursor_destroy(cursor);
    bson_destroy(query);
    return !failed && !found;
}

/* Each document is removed by its owned _id, including after a failed step. */
static bool insert_fixture(abts_case *tc, registration_fixture_t *fixture,
        mongoc_collection_t *collection, bson_t *document, const bson_t *opts)
{
    unsigned int index = fixture->count++;
    bson_error_t error;
    bool inserted;

    ogs_assert(document);
    ogs_assert(index < OGS_ARRAY_SIZE(fixture->id));
    fixture->collection[index] = collection;
    bson_oid_init(&fixture->id[index], NULL);
    ogs_assert(BSON_APPEND_OID(document, "_id", &fixture->id[index]));
    inserted = mongoc_collection_insert_one(collection,
            document, opts, NULL, &error);
    bson_destroy(document);
    if (!inserted)
        ABTS_FAIL(tc, error.message);
    return inserted;
}

static void remove_fixtures(abts_case *tc, registration_fixture_t *fixture)
{
    bson_error_t error;

    while (fixture->count) {
        unsigned int index = --fixture->count;
        bson_t *query = BCON_NEW("_id", BCON_OID(&fixture->id[index]));

        ogs_assert(query);
        if (!mongoc_collection_delete_one(fixture->collection[index],
                query, NULL, NULL, &error))
            ABTS_FAIL(tc, error.message);
        bson_destroy(query);
    }
}

static bool seed_registration(abts_case *tc, test_ue_t *ue, const char *pei,
        const registration_case_t *test, registration_fixture_t *fixture)
{
    bson_t *document;
    mongoc_collection_t *subscriber = ogs_mongoc()->collection.subscriber;
    mongoc_collection_t *eir = ogs_mongoc()->collection.eir;

    if (!identity_unused(tc, subscriber, "imsi", ue->imsi) ||
            !identity_unused(tc, eir, "pei", pei))
        return false;

    ogs_hex_from_string(ue->k_string, ue->k, sizeof(ue->k));
    ogs_hex_from_string(ue->opc_string, ue->opc, sizeof(ue->opc));
    document = test_db_new_simple(ue);
    ABTS_PTR_NOTNULL(tc, document);
    if (!document || !insert_fixture(tc, fixture, subscriber, document, NULL))
        return false;

    if (test->generic_status) {
        bson_t *opts = NULL;
        bool inserted;

        document = BCON_NEW("pei", BCON_UTF8(pei),
                "status", BCON_UTF8(test->generic_status));

        /* Exercise AMF failure_action against a real EIR HTTP 500. */
        if (!strcmp(test->generic_status, "INVALID"))
            opts = BCON_NEW("bypassDocumentValidation", BCON_BOOL(true));
        inserted = insert_fixture(tc, fixture, eir, document, opts);
        if (opts)
            bson_destroy(opts);
        if (!inserted)
            return false;
    }
    if (test->specific_status) {
        document = BCON_NEW("pei", BCON_UTF8(pei),
                "supi", BCON_UTF8(test->other_supi ?
                    "imsi-999709999999999" : ue->supi),
                "status", BCON_UTF8(test->specific_status));
        if (!insert_fixture(tc, fixture, eir, document, NULL))
            return false;
    }
    return true;
}

static bool send_ngap(abts_case *tc, ogs_socknode_t *ngap, ogs_pkbuf_t *buffer)
{
    int rv;

    ABTS_PTR_NOTNULL(tc, buffer);
    if (!buffer)
        return false;
    rv = testgnb_ngap_send(ngap, buffer);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    if (rv != OGS_OK)
        ogs_pkbuf_free(buffer);
    return rv == OGS_OK;
}

static bool send_nas(abts_case *tc, ogs_socknode_t *ngap,
        test_ue_t *ue, ogs_pkbuf_t *nas)
{
    ABTS_PTR_NOTNULL(tc, nas);
    if (!nas)
        return false;
    return send_ngap(tc, ngap, testngap_build_uplink_nas_transport(ue, nas));
}

static bool receive_ngap(abts_case *tc, ogs_socknode_t *ngap,
        test_ue_t *ue, int procedure, uint8_t nas_type)
{
    ogs_pkbuf_t *buffer = testgnb_ngap_read(ngap);

    ABTS_PTR_NOTNULL(tc, buffer);
    if (!buffer)
        return false;
    ue->gmm_message_type = 0;
    testngap_recv(ue, buffer);
    ABTS_INT_EQUAL(tc, procedure, ue->ngap_procedure_code);
    if (nas_type)
        ABTS_INT_EQUAL(tc, nas_type, ue->gmm_message_type);
    return !tc->failed;
}

static void registration_case(abts_case *tc, void *data)
{
    const registration_case_t *test = data;
    unsigned int index = test - registration_cases;
    registration_fixture_t fixture = {0};
    ogs_nas_5gs_mobile_identity_suci_t suci;
    test_ue_t *ue = NULL;
    ogs_socknode_t *ngap = NULL;
    ogs_pkbuf_t *nas = NULL, *container = NULL;
    char msin[11], pei[24];

    ogs_debug("EIR registration: %s", test->name);

    memset(&suci, 0, sizeof(suci));
    suci.h.supi_format = OGS_NAS_5GS_SUPI_FORMAT_IMSI;
    suci.h.type = OGS_NAS_5GS_MOBILE_IDENTITY_SUCI;
    suci.routing_indicator2 = suci.routing_indicator3 =
        suci.routing_indicator4 = 0xf;
    suci.protection_scheme_id = OGS_PROTECTION_SCHEME_NULL;
    ogs_snprintf(msin, sizeof(msin), "00004708%02u", index);
    ue = test_ue_add_by_suci(&suci, msin);
    ogs_assert(ue);
    ue->nr_cgi.cell_id = 0x40001;
    ue->nas.registration.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    ue->nas.registration.follow_on_request = 1;
    ue->nas.registration.value = OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL;
    ue->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    ue->opc_string = "e8ed289deba952e4283b54e88e6183ca";
    ue->mobile_identity_imeisv.digit16 = index;
    ogs_snprintf(pei, sizeof(pei), "imeisv-866507040040530%u", index);
    if (!seed_registration(tc, ue, pei, test, &fixture))
        goto cleanup;

    ngap = testngap_client(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap);
    if (!ngap)
        goto cleanup;
    if (!send_ngap(tc, ngap,
                testngap_build_ng_setup_request(0x4708 + index, 22)) ||
            !receive_ngap(tc, ngap, ue, NGAP_ProcedureCode_id_NGSetup, 0))
        goto cleanup;

    /* The first unprotected request contains only cleartext IEs. */
    nas = testgmm_build_registration_request(ue, NULL, false, false);
    ogs_assert(nas);

    /* Carry the additional IEs inside Security Mode Complete's protected
     * NAS message container, as in tests/registration/identity-test.c. */
    ue->registration_request_param.gmm_capability = 1;
    ue->registration_request_param.s1_ue_network_capability = 1;
    ue->registration_request_param.requested_nssai = 1;
    ue->registration_request_param.last_visited_registered_tai = 1;
    ue->registration_request_param.ue_usage_setting = 1;
    container = testgmm_build_registration_request(ue, NULL, false, false);
    ogs_assert(container);
    if (!send_ngap(tc, ngap, testngap_build_initial_ue_message(ue, nas,
                    NGAP_RRCEstablishmentCause_mo_Signalling, false, true)))
        goto cleanup;
    if (!receive_ngap(tc, ngap, ue, NGAP_ProcedureCode_id_DownlinkNASTransport,
                OGS_NAS_5GS_AUTHENTICATION_REQUEST) ||
            !send_nas(tc, ngap, ue,
                testgmm_build_authentication_response(ue)) ||
            !receive_ngap(tc, ngap, ue,
                NGAP_ProcedureCode_id_DownlinkNASTransport,
                OGS_NAS_5GS_SECURITY_MODE_COMMAND))
        goto cleanup;
    nas = testgmm_build_security_mode_complete(ue, container);
    container = NULL;
    if (!send_nas(tc, ngap, ue, nas))
        goto cleanup;

    if (test->reject_cause) {
        if (!receive_ngap(tc, ngap, ue,
                    NGAP_ProcedureCode_id_DownlinkNASTransport,
                    OGS_NAS_5GS_REGISTRATION_REJECT))
            goto cleanup;
        ABTS_INT_EQUAL(tc, test->reject_cause, ue->registration_reject_cause);
    } else {
        if (!receive_ngap(tc, ngap, ue,
                    NGAP_ProcedureCode_id_InitialContextSetup,
                    OGS_NAS_5GS_REGISTRATION_ACCEPT) ||
                !send_ngap(tc, ngap,
                    testngap_build_ue_radio_capability_info_indication(ue)) ||
                !send_ngap(tc, ngap,
                    testngap_build_initial_context_setup_response(ue, false)) ||
                !send_nas(tc, ngap, ue,
                    testgmm_build_registration_complete(ue)) ||
                !receive_ngap(tc, ngap, ue,
                    NGAP_ProcedureCode_id_DownlinkNASTransport,
                    OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND))
            goto cleanup;
        /* Switch-off deregistration releases AMF/UDM/PCF state without a PDU
         * session, so these tests do not require an SMF, UPF or TUN device. */
        if (!send_nas(tc, ngap, ue,
                    testgmm_build_de_registration_request(
                        ue, true, true, false)))
            goto cleanup;
    }
    if (!receive_ngap(tc, ngap, ue,
                NGAP_ProcedureCode_id_UEContextRelease, 0) ||
            !send_ngap(tc, ngap,
                testngap_build_ue_context_release_complete(ue)))
        goto cleanup;

cleanup:
    if (container)
        ogs_pkbuf_free(container);
    if (ngap)
        testgnb_ngap_close(ngap);
    remove_fixtures(tc, &fixture);
    if (ue)
        test_ue_remove(ue);
}

abts_suite *test_eir_registration(abts_suite *suite)
{
    unsigned int i;

    suite = ADD_SUITE(suite)
    for (i = 0; i < OGS_ARRAY_SIZE(registration_cases); i++)
        abts_run_test(suite, registration_case, (void *)&registration_cases[i]);
    return suite;
}
