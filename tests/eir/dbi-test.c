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
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "integration.h"

#define TEST_EIR_PEI "imeisv-4901542032375186"
#define TEST_EIR_IMEI "imei-490154203237518"
#define TEST_EIR_SUPI "imsi-001010123456789"
#define TEST_EIR_OTHER_SUPI "imsi-001010987654321"

#define GENERIC(status) \
    "{\"pei\":\"" TEST_EIR_PEI "\",\"status\":\"" status "\"}"
#define SPECIFIC(status) \
    "{\"pei\":\"" TEST_EIR_PEI "\",\"supi\":\"" TEST_EIR_SUPI \
    "\",\"status\":\"" status "\"}"
#define NULL_SUPI(status) \
    "{\"pei\":\"" TEST_EIR_PEI "\",\"supi\":null,\"status\":\"" status "\"}"


typedef struct lookup_case_s {
    const char *name;
    const char *documents[2];
    const char *pei;
    const char *supi;
    int result;
    ogs_dbi_eir_status_t status;
    const char *matched_supi;
} lookup_case_t;

static const lookup_case_t lookup_cases[] = {
    {
        "generic whitelist", {GENERIC("WHITELISTED")},
        TEST_EIR_PEI, NULL, OGS_OK, OGS_DBI_EIR_STATUS_WHITELISTED,
        NULL
    }, {
        "generic blacklist", {GENERIC("BLACKLISTED")},
        TEST_EIR_PEI, NULL, OGS_OK, OGS_DBI_EIR_STATUS_BLACKLISTED,
        NULL
    }, {
        "generic greylist", {GENERIC("GREYLISTED")},
        TEST_EIR_PEI, TEST_EIR_SUPI, OGS_OK, OGS_DBI_EIR_STATUS_GREYLISTED,
        NULL
    }, {
        "IMEI lookup",
        {"{\"pei\":\"" TEST_EIR_IMEI "\",\"status\":\"WHITELISTED\"}"},
        TEST_EIR_IMEI, NULL, OGS_OK, OGS_DBI_EIR_STATUS_WHITELISTED,
        NULL
    }, {
        "pair blacklist overrides generic whitelist",
        {GENERIC("WHITELISTED"), SPECIFIC("BLACKLISTED")},
        TEST_EIR_PEI, TEST_EIR_SUPI, OGS_OK, OGS_DBI_EIR_STATUS_BLACKLISTED,
        TEST_EIR_SUPI
    }, {
        "pair whitelist overrides generic blacklist",
        {GENERIC("BLACKLISTED"), SPECIFIC("WHITELISTED")},
        TEST_EIR_PEI, TEST_EIR_SUPI, OGS_OK, OGS_DBI_EIR_STATUS_WHITELISTED,
        TEST_EIR_SUPI
    }, {
        "pair-only exact match", {SPECIFIC("GREYLISTED")},
        TEST_EIR_PEI, TEST_EIR_SUPI, OGS_OK, OGS_DBI_EIR_STATUS_GREYLISTED,
        TEST_EIR_SUPI
    }, {
        "different SUPI uses generic record",
        {SPECIFIC("BLACKLISTED"), GENERIC("WHITELISTED")},
        TEST_EIR_PEI, TEST_EIR_OTHER_SUPI, OGS_OK,
        OGS_DBI_EIR_STATUS_WHITELISTED, NULL
    }, {
        "omitted SUPI ignores pair",
        {GENERIC("GREYLISTED"), SPECIFIC("BLACKLISTED")},
        TEST_EIR_PEI, NULL, OGS_OK, OGS_DBI_EIR_STATUS_GREYLISTED,
        NULL
    }, {
        "null SUPI is generic", {NULL_SUPI("BLACKLISTED")},
        TEST_EIR_PEI, NULL, OGS_OK, OGS_DBI_EIR_STATUS_BLACKLISTED,
        NULL
    }, {
        "null SUPI fallback", {NULL_SUPI("WHITELISTED")},
        TEST_EIR_PEI, TEST_EIR_SUPI, OGS_OK, OGS_DBI_EIR_STATUS_WHITELISTED,
        NULL
    }, {
        "unknown equipment", {NULL},
        TEST_EIR_PEI, TEST_EIR_SUPI, OGS_NOTFOUND,
        OGS_DBI_EIR_STATUS_UNKNOWN, NULL
    }, {
        "unrelated PEI", {GENERIC("WHITELISTED")},
        TEST_EIR_IMEI, TEST_EIR_SUPI, OGS_NOTFOUND,
        OGS_DBI_EIR_STATUS_UNKNOWN, NULL
    }, {
        "pair-only other SUPI", {SPECIFIC("WHITELISTED")},
        TEST_EIR_PEI, TEST_EIR_OTHER_SUPI, OGS_NOTFOUND,
        OGS_DBI_EIR_STATUS_UNKNOWN, NULL
    }, {
        "pair-only omitted SUPI", {SPECIFIC("WHITELISTED")},
        TEST_EIR_PEI, NULL, OGS_NOTFOUND,
        OGS_DBI_EIR_STATUS_UNKNOWN, NULL
    }, {
        "invalid status is a database error", {GENERIC("BOGUS")},
        TEST_EIR_PEI, NULL, OGS_ERROR,
        OGS_DBI_EIR_STATUS_UNKNOWN, NULL
    }, {
        "empty status is a database error", {GENERIC("")},
        TEST_EIR_PEI, NULL, OGS_ERROR,
        OGS_DBI_EIR_STATUS_UNKNOWN, NULL
    }, {
        "missing status is a database error",
        {"{\"pei\":\"" TEST_EIR_PEI "\"}"},
        TEST_EIR_PEI, NULL, OGS_ERROR,
        OGS_DBI_EIR_STATUS_UNKNOWN, NULL
    }, {
        "numeric status is a database error",
        {"{\"pei\":\"" TEST_EIR_PEI "\",\"status\":1}"},
        TEST_EIR_PEI, NULL, OGS_ERROR,
        OGS_DBI_EIR_STATUS_UNKNOWN, NULL
    }, {
        "null status is a database error",
        {"{\"pei\":\"" TEST_EIR_PEI "\",\"status\":null}"},
        TEST_EIR_PEI, NULL, OGS_ERROR,
        OGS_DBI_EIR_STATUS_UNKNOWN, NULL
    }, {
        "SUPI array match is a database error",
        {"{\"pei\":\"" TEST_EIR_PEI "\",\"supi\":[\"" TEST_EIR_SUPI
         "\"],\"status\":\"WHITELISTED\"}"},
        TEST_EIR_PEI, TEST_EIR_SUPI, OGS_ERROR,
        OGS_DBI_EIR_STATUS_UNKNOWN, NULL
    }, {
        "null SUPI array is not a generic record",
        {"{\"pei\":\"" TEST_EIR_PEI
         "\",\"supi\":[null],\"status\":\"WHITELISTED\"}"},
        TEST_EIR_PEI, NULL, OGS_ERROR,
        OGS_DBI_EIR_STATUS_UNKNOWN, NULL
    }, {
        "PEI array match is a database error",
        {"{\"pei\":[\"" TEST_EIR_PEI
         "\"],\"status\":\"WHITELISTED\"}"},
        TEST_EIR_PEI, NULL, OGS_ERROR,
        OGS_DBI_EIR_STATUS_UNKNOWN, NULL
    }
};

typedef struct fixture_s {
    bson_oid_t ids[3];
    unsigned int count;
} fixture_t;

static bool fixture_begin(abts_case *tc)
{
    bson_error_t error;
    bson_t *query = BCON_NEW("pei", "{", "$in", "[",
            BCON_UTF8(TEST_EIR_PEI), BCON_UTF8(TEST_EIR_IMEI), "]", "}");
    int64_t count;

#if MONGOC_CHECK_VERSION(1, 11, 0)
    count = mongoc_collection_count_documents(
            ogs_mongoc()->collection.eir, query, NULL, NULL, NULL, &error);
#else
    count = mongoc_collection_count(ogs_mongoc()->collection.eir,
            MONGOC_QUERY_NONE, query, 0, 0, NULL, &error);
#endif

    bson_destroy(query);
    if (count != 0)
        ABTS_FAIL(tc, count < 0 ? error.message :
                "EIR DB fixture PEI already exists; existing records kept");
    return count == 0;
}

static bool fixture_write(abts_case *tc, fixture_t *fixture,
        const char *json, const bson_t *opts, int expected_error)
{
    bson_error_t error;
    bson_t *document = bson_new_from_json((const uint8_t *)json, -1, &error);
    unsigned int index = fixture->count++;
    bool inserted;

    ogs_assert(index < OGS_ARRAY_SIZE(fixture->ids));
    bson_oid_init(&fixture->ids[index], NULL);
    if (!document) {
        ABTS_FAIL(tc, error.message);
        return false;
    }
    BSON_APPEND_OID(document, "_id", &fixture->ids[index]);
    inserted = mongoc_collection_insert_one(ogs_mongoc()->collection.eir,
            document, opts, NULL, &error);
    bson_destroy(document);

    if (!inserted && !expected_error) {
        ABTS_FAIL(tc, error.message);
    } else {
        ABTS_INT_EQUAL(tc, expected_error, inserted ? 0 : error.code);
    }
    return !tc->failed;
}

static bool fixture_insert(abts_case *tc, fixture_t *fixture,
        const char *json, const bson_t *opts)
{
    return fixture_write(tc, fixture, json, opts, 0);
}

static void fixture_end(abts_case *tc, fixture_t *fixture)
{
    bson_error_t error;

    /* Remove only documents created by this fixture. */
    while (fixture->count) {
        bson_t *query = BCON_NEW("_id",
                BCON_OID(&fixture->ids[--fixture->count]));

        if (!mongoc_collection_delete_one(ogs_mongoc()->collection.eir,
                    query, NULL, NULL, &error))
            ABTS_FAIL(tc, error.message);
        bson_destroy(query);
    }
}

static void equipment_lookup(abts_case *tc, void *data)
{
    const lookup_case_t *test = data;
    fixture_t fixture = {0};
    ogs_dbi_eir_record_t record;
    bson_t *opts = NULL;
    unsigned int i;
    int rv;

    ogs_debug("EIR DB: %s", test->name);
    if (!fixture_begin(tc))
        return;
    /* Exercise runtime validation even if an operator installed a validator. */
    if (test->result == OGS_ERROR)
        opts = BCON_NEW("bypassDocumentValidation", BCON_BOOL(true));
    for (i = 0; i < OGS_ARRAY_SIZE(test->documents); i++) {
        if (test->documents[i] &&
                !fixture_insert(tc, &fixture, test->documents[i], opts))
            goto cleanup;
    }

    memset(&record, 0, sizeof(record));
    record.status = OGS_DBI_EIR_STATUS_BLACKLISTED;
    rv = ogs_dbi_eir_check_equipment(test->supi, test->pei, &record);
    ABTS_INT_EQUAL(tc, test->result, rv);
    ABTS_INT_EQUAL(tc, test->status, record.status);
    if (rv == OGS_OK) {
        ABTS_STR_EQUAL(tc, test->pei, record.pei);
        ABTS_STR_EQUAL(tc, test->matched_supi, record.supi);
    } else {
        ABTS_PTR_EQUAL(tc, NULL, record.pei);
        ABTS_PTR_EQUAL(tc, NULL, record.supi);
    }
    ogs_dbi_eir_record_free(&record);
    ABTS_PTR_EQUAL(tc, NULL, record.pei);
    ABTS_PTR_EQUAL(tc, NULL, record.supi);
    ABTS_INT_EQUAL(tc, OGS_DBI_EIR_STATUS_UNKNOWN, record.status);
    ogs_dbi_eir_record_free(&record);

cleanup:
    if (opts)
        bson_destroy(opts);
    fixture_end(tc, &fixture);
}

static void invalid_pair_does_not_fall_back(abts_case *tc, void *data)
{
    fixture_t fixture = {0};
    ogs_dbi_eir_record_t record;
    bson_t *opts;
    bool inserted;
    int rv;

    if (!fixture_begin(tc))
        return;
    if (!fixture_insert(tc, &fixture, GENERIC("WHITELISTED"), NULL))
        goto cleanup;
    opts = BCON_NEW("bypassDocumentValidation", BCON_BOOL(true));
    inserted = fixture_insert(tc, &fixture, SPECIFIC("INVALID"), opts);
    bson_destroy(opts);
    if (!inserted)
        goto cleanup;

    /* Simulate legacy/corrupt data without changing collection validation. */
    rv = ogs_dbi_eir_check_equipment(TEST_EIR_SUPI, TEST_EIR_PEI, &record);
    ABTS_INT_EQUAL(tc, OGS_ERROR, rv);
    ABTS_INT_EQUAL(tc, OGS_DBI_EIR_STATUS_UNKNOWN, record.status);
    ABTS_PTR_EQUAL(tc, NULL, record.pei);
    ABTS_PTR_EQUAL(tc, NULL, record.supi);
    ogs_dbi_eir_record_free(&record);

cleanup:
    fixture_end(tc, &fixture);
}

enum {
    EIR_DUPLICATE_KEY = 11000,
    EIR_DOCUMENT_VALIDATION_FAILURE = 121
};

typedef struct write_case_s {
    const char *documents[2];
    int error;
} write_case_t;

static const write_case_t write_cases[] = {
    {{GENERIC("WHITELISTED"), GENERIC("BLACKLISTED")}, EIR_DUPLICATE_KEY},
    {{GENERIC("WHITELISTED"), NULL_SUPI("BLACKLISTED")},
        EIR_DUPLICATE_KEY},
    {{NULL_SUPI("WHITELISTED"), GENERIC("BLACKLISTED")},
        EIR_DUPLICATE_KEY},
    {{SPECIFIC("WHITELISTED"), SPECIFIC("BLACKLISTED")}, EIR_DUPLICATE_KEY},
    {{GENERIC("WHITELISTED"), SPECIFIC("BLACKLISTED")}, 0},
    {{SPECIFIC("WHITELISTED"),
      "{\"pei\":\"" TEST_EIR_PEI "\",\"supi\":\"" TEST_EIR_OTHER_SUPI
      "\",\"status\":\"BLACKLISTED\"}"}, 0},
    {{NULL, GENERIC("INVALID")}, EIR_DOCUMENT_VALIDATION_FAILURE},
    {{NULL, "{\"pei\":\"" TEST_EIR_PEI "\"}"}, EIR_DOCUMENT_VALIDATION_FAILURE},
    {{NULL, "{\"pei\":\"" TEST_EIR_PEI "\",\"status\":1}"},
        EIR_DOCUMENT_VALIDATION_FAILURE},
    {{NULL, "{\"pei\":\"" TEST_EIR_PEI "\",\"status\":null}"},
        EIR_DOCUMENT_VALIDATION_FAILURE},
    {{NULL, "{\"status\":\"WHITELISTED\"}"}, EIR_DOCUMENT_VALIDATION_FAILURE},
    {{NULL, "{\"pei\":1,\"status\":\"WHITELISTED\"}"},
        EIR_DOCUMENT_VALIDATION_FAILURE},
    {{NULL, "{\"pei\":[\"" TEST_EIR_PEI
      "\"],\"status\":\"WHITELISTED\"}"}, EIR_DOCUMENT_VALIDATION_FAILURE},
    {{NULL, "{\"pei\":\"" TEST_EIR_PEI "\",\"supi\":1,"
      "\"status\":\"WHITELISTED\"}"}, EIR_DOCUMENT_VALIDATION_FAILURE},
    {{NULL, "{\"pei\":\"" TEST_EIR_PEI "\",\"supi\":[\"" TEST_EIR_SUPI
      "\"],\"status\":\"WHITELISTED\"}"}, EIR_DOCUMENT_VALIDATION_FAILURE}
};

static void database_constraints(abts_case *tc, void *data)
{
    const write_case_t *test = data;
    fixture_t fixture = {0};

    if (!fixture_begin(tc))
        return;
    if (test->documents[0] && !fixture_insert(tc, &fixture,
                test->documents[0], NULL))
        goto cleanup;

    /* Normal writes must enforce the startup index and validator. */
    fixture_write(tc, &fixture, test->documents[1], NULL, test->error);

cleanup:
    fixture_end(tc, &fixture);
}

static void repeated_database_init(abts_case *tc, void *data)
{
    fixture_t fixture = {0};

    if (!fixture_begin(tc))
        return;
    if (!fixture_insert(tc, &fixture, GENERIC("WHITELISTED"), NULL))
        goto cleanup;

    ABTS_INT_EQUAL(tc, OGS_OK, ogs_dbi_eir_init());
    /* Reinitialization must retain data and both write constraints. */
    fixture_write(tc, &fixture, NULL_SUPI("BLACKLISTED"), NULL,
            EIR_DUPLICATE_KEY);
    fixture_write(tc, &fixture, SPECIFIC("INVALID"), NULL,
            EIR_DOCUMENT_VALIDATION_FAILURE);

cleanup:
    fixture_end(tc, &fixture);
}

abts_suite *test_eir_dbi(abts_suite *suite)
{
    unsigned int i;

    suite = ADD_SUITE(suite)
    for (i = 0; i < OGS_ARRAY_SIZE(write_cases); i++)
        abts_run_test(suite, database_constraints, (void *)&write_cases[i]);
    abts_run_test(suite, repeated_database_init, NULL);
    for (i = 0; i < OGS_ARRAY_SIZE(lookup_cases); i++)
        abts_run_test(suite, equipment_lookup, (void *)&lookup_cases[i]);
    abts_run_test(suite, invalid_pair_does_not_fall_back, NULL);
    return suite;
}
