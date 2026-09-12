/*
 * Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

#include "integration.h"
#include <curl/curl.h>

#define SERVICE_PEI "imeisv-4901542032375199"
#define SERVICE_SUPI "imsi-001010000029999"
#define EQUIPMENT_PATH "/n5g-eir-eic/v1/equipment-status"
#define DISCOVERY_PATH "/nnrf-disc/v1/nf-instances?target-nf-type=5G_EIR" \
    "&requester-nf-type=AMF&service-names=n5g-eir-eic"

typedef struct http_result_s {
    char body[65536];
    size_t length;
    char content_type[128];
    long status;
    long version;
    CURLcode error;
    const char *stage;
} http_result_t;

static size_t receive_body(char *data, size_t size, size_t count, void *arg)
{
    http_result_t *result = arg;
    size_t bytes;

    if (size && count > (sizeof(result->body) - 1 - result->length) / size) {
        ogs_debug("EIR HTTP response exceeds the test buffer [limit:%zu]",
                sizeof(result->body) - 1);
        return 0;
    }
    bytes = size * count;
    memcpy(result->body + result->length, data, bytes);
    result->length += bytes;
    result->body[result->length] = '\0';
    return bytes;
}

static bool http_request(const char *base, const char *path,
        const char *method, http_result_t *result)
{
    CURL *curl;
    CURLcode rv;
    char *url, *content_type = NULL;

    memset(result, 0, sizeof(*result));
    result->stage = "client initialization";
    curl = curl_easy_init();
    if (!curl) {
        result->error = CURLE_FAILED_INIT;
        ogs_debug("Cannot initialize EIR test HTTP client");
        return false;
    }
    url = ogs_msprintf("%s%s", base, path);
    ogs_assert(url);

    /* Exercise the real HTTP/2 listener; do not fall back to HTTP/1.1. */
    result->stage = "HTTP/2 setup";
    rv = curl_easy_setopt(curl, CURLOPT_HTTP_VERSION,
            CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE);
    if (rv != CURLE_OK) goto done;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "AMF");
    curl_easy_setopt(curl, CURLOPT_PROXY, "");
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 500L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 2000L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receive_body);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
    result->stage = "request";
    rv = curl_easy_perform(curl);
    if (rv == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &result->status);
        curl_easy_getinfo(curl, CURLINFO_HTTP_VERSION, &result->version);
        curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
        if (content_type)
            ogs_cpystrn(result->content_type, content_type,
                    sizeof(result->content_type));
    }
done:
    result->error = rv;
    if (rv != CURLE_OK)
        ogs_debug("EIR HTTP %s failed [%s:%s]", method,
                result->stage, curl_easy_strerror(rv));
    else if (result->version != CURL_HTTP_VERSION_2_0)
        ogs_debug("EIR test requires HTTP/2 [version:%ld]", result->version);
    ogs_free(url);
    curl_easy_cleanup(curl);
    return rv == CURLE_OK && result->version == CURL_HTTP_VERSION_2_0;
}

static void log_http_failure(const char *name, const http_result_t *result)
{
    ogs_error("EIR HTTP/2 test [%s] failed [%s:%s,http:%ld,version:%ld]",
            name, result->stage, curl_easy_strerror(result->error),
            result->status, result->version);
}

static bool string_is(const cJSON *object, const char *key, const char *value)
{
    const cJSON *item = cJSON_GetObjectItemCaseSensitive(object, key);
    return cJSON_IsString(item) && !strcmp(item->valuestring, value);
}

static bool has_eir_service(const cJSON *profile)
{
    const cJSON *services, *service;

    services = cJSON_GetObjectItemCaseSensitive(profile, "nfServices");
    if (!services)
        services = cJSON_GetObjectItemCaseSensitive(profile, "nfServiceList");
    cJSON_ArrayForEach(service, services) {
        if (string_is(service, "serviceName", "n5g-eir-eic") &&
                string_is(service, "nfServiceStatus", "REGISTERED"))
            return true;
    }
    return false;
}

static bool eir_registered(const http_result_t *result)
{
    cJSON *document, *profiles, *profile;
    bool found = false;

    if (result->status != 200) {
        ogs_debug("EIR NRF discovery returned HTTP status [%ld]",
                result->status);
        return false;
    }
    document = cJSON_Parse(result->body);
    if (!document) {
        ogs_debug("EIR NRF discovery returned invalid JSON");
        return false;
    }
    profiles = cJSON_GetObjectItemCaseSensitive(document, "nfInstances");
    cJSON_ArrayForEach(profile, profiles) {
        if (string_is(profile, "nfType", "5G_EIR") &&
                string_is(profile, "nfStatus", "REGISTERED") &&
                has_eir_service(profile)) {
            found = true;
            break;
        }
    }
    cJSON_Delete(document);
    if (!found)
        ogs_debug("NRF discovery has no registered 5G-EIR service yet");
    return found;
}

bool test_eir_wait_ready(void)
{
    http_result_t result;
    ogs_time_t deadline = ogs_get_monotonic_time() + ogs_time_from_sec(20);

    do {
        if (http_request(test_eir_nrf_uri(), DISCOVERY_PATH, "GET", &result) &&
                eir_registered(&result) &&
                http_request(test_eir_sbi_uri(),
                    EQUIPMENT_PATH "?pei=" SERVICE_PEI, "GET", &result) &&
                (result.status == 200 || result.status == 404))
            return true;
        ogs_msleep(100);
    } while (ogs_get_monotonic_time() < deadline);
    return false;
}

static void discovery(abts_case *tc, void *data)
{
    http_result_t result;
    bool received = http_request(test_eir_nrf_uri(),
            DISCOVERY_PATH, "GET", &result);
    bool registered;

    ABTS_TRUE(tc, received);
    if (!received) {
        log_http_failure("NRF discovery", &result);
        return;
    }
    ABTS_INT_EQUAL(tc, 200, result.status);
    registered = eir_registered(&result);
    if (!registered)
        ogs_error("EIR discovery test found no registered service [http:%ld]",
                result.status);
    ABTS_TRUE(tc, registered);
}

typedef struct service_case_s {
    const char *name;
    const char *path;
    const char *method;
    const char *record_status; /* NULL: no fixture */
    int http_status;
    const char *equipment_status;
} service_case_t;

static const service_case_t cases[] = {
    {"whitelisted", EQUIPMENT_PATH "?pei=" SERVICE_PEI, "GET",
        "WHITELISTED", 200, "WHITELISTED"},
    {"blacklisted", EQUIPMENT_PATH "?pei=" SERVICE_PEI, "GET",
        "BLACKLISTED", 200, "BLACKLISTED"},
    {"greylisted", EQUIPMENT_PATH "?pei=" SERVICE_PEI, "GET",
        "GREYLISTED", 200, "GREYLISTED"},
    {"generic record with SUPI", EQUIPMENT_PATH "?pei=" SERVICE_PEI
        "&supi=" SERVICE_SUPI, "GET", "WHITELISTED", 200, "WHITELISTED"},
    {"unknown equipment", EQUIPMENT_PATH "?pei=" SERVICE_PEI, "GET",
        NULL, 404, NULL},
    {"missing PEI", EQUIPMENT_PATH, "GET", NULL, 400, NULL},
    {"invalid PEI", EQUIPMENT_PATH "?pei=imeisv-invalid", "GET",
        NULL, 400, NULL},
    {"invalid SUPI", EQUIPMENT_PATH "?pei=" SERVICE_PEI "&supi=imsi-x",
        "GET", "WHITELISTED", 400, NULL},
    {"IMSI below EIR minimum", EQUIPMENT_PATH "?pei=" SERVICE_PEI
        "&supi=imsi-00101", "GET", "WHITELISTED", 400, NULL},
    {"IMSI at EIR minimum", EQUIPMENT_PATH "?pei=" SERVICE_PEI
        "&supi=imsi-001010", "GET", "WHITELISTED", 200, "WHITELISTED"},
    {"invalid database status", EQUIPMENT_PATH "?pei=" SERVICE_PEI,
        "GET", "INVALID", 500, NULL},
    {"unsupported method", EQUIPMENT_PATH "?pei=" SERVICE_PEI,
        "POST", NULL, 403, NULL},
    {"unsupported version", "/n5g-eir-eic/v2/equipment-status?pei=" SERVICE_PEI,
        "GET", NULL, 400, NULL},
    {"unknown resource", "/n5g-eir-eic/v1/unknown?pei=" SERVICE_PEI,
        "GET", NULL, 400, NULL},
};

static void service_request(abts_case *tc, void *data)
{
    const service_case_t *test = data;
    mongoc_collection_t *collection = ogs_mongoc()->collection.eir;
    bson_t query, record, owned;
    bson_t *insert_opts = NULL;
    bson_oid_t oid;
    bson_error_t error;
    int64_t count;
    bool inserted = false, received;
    http_result_t result;
    const char *expected_content_type;
    cJSON *document, *status;

    ogs_debug("EIR HTTP/2: %s", test->name);
    bson_init(&query);
    BSON_APPEND_UTF8(&query, "pei", SERVICE_PEI);
#if MONGOC_CHECK_VERSION(1, 11, 0)
    count = mongoc_collection_count_documents(
            collection, &query, NULL, NULL, NULL, &error);
#else
    count = mongoc_collection_count(
            collection, MONGOC_QUERY_NONE, &query, 0, 0, NULL, &error);
#endif
    bson_destroy(&query);
    if (count != 0) {
        ABTS_FAIL(tc, count < 0 ? error.message :
                "EIR HTTP fixture PEI already exists; existing records kept");
        return;
    }

    bson_oid_init(&oid, NULL);
    bson_init(&owned);
    BSON_APPEND_OID(&owned, "_id", &oid);
    if (test->record_status) {
        bson_init(&record);
        BSON_APPEND_OID(&record, "_id", &oid);
        BSON_APPEND_UTF8(&record, "pei", SERVICE_PEI);
        BSON_APPEND_UTF8(&record, "status", test->record_status);
        /* Insert the malformed fixture even when the shared collection has
         * a validator. The exception applies only to this test-owned document;
         * normal fixtures still use the collection's validation rules. */
        if (!strcmp(test->record_status, "INVALID")) {
            insert_opts = BCON_NEW("bypassDocumentValidation", BCON_BOOL(true));
            ogs_assert(insert_opts);
        }
        inserted = mongoc_collection_insert_one(collection,
                &record, insert_opts, NULL, &error);
        if (insert_opts)
            bson_destroy(insert_opts);
        bson_destroy(&record);
        if (!inserted) {
            ABTS_FAIL(tc, error.message);
            goto cleanup;
        }
    }

    received = http_request(test_eir_sbi_uri(),
            test->path, test->method, &result);
    ABTS_TRUE(tc, received);
    if (!received) {
        log_http_failure(test->name, &result);
        goto cleanup;
    }
    if (result.status != test->http_status)
        ogs_error("EIR HTTP/2 [%s] status mismatch [expected:%d,actual:%ld]",
                test->name, test->http_status, result.status);
    ABTS_INT_EQUAL(tc, test->http_status, result.status);
    expected_content_type = test->http_status == 200 ?
        OGS_SBI_CONTENT_JSON_TYPE : OGS_SBI_CONTENT_PROBLEM_TYPE;
    if (strncmp(result.content_type,
                expected_content_type, strlen(expected_content_type)))
        ogs_error("EIR HTTP/2 [%s] content type mismatch "
                "[expected:%s,actual:%s]", test->name,
                expected_content_type, result.content_type);
    ABTS_TRUE(tc, !strncmp(result.content_type,
                expected_content_type, strlen(expected_content_type)));
    document = cJSON_Parse(result.body);
    if (!document)
        ogs_error("EIR HTTP/2 [%s] returned invalid JSON", test->name);
    ABTS_PTR_NOTNULL(tc, document);
    if (document) {
        status = cJSON_GetObjectItemCaseSensitive(document, "status");
        if (test->equipment_status) {
            ABTS_TRUE(tc, cJSON_IsString(status));
            if (cJSON_IsString(status))
                ABTS_STR_EQUAL(tc, test->equipment_status, status->valuestring);
        } else {
            ABTS_TRUE(tc, cJSON_IsNumber(status));
            if (cJSON_IsNumber(status))
                ABTS_INT_EQUAL(tc, test->http_status, status->valueint);
            if (test->http_status == 404)
                ABTS_TRUE(tc, string_is(document,
                            "cause", "ERROR_EQUIPMENT_UNKNOWN"));
        }
        cJSON_Delete(document);
    }
cleanup:
    /* Delete only this case's generated _id, including uncertain inserts. */
    if (test->record_status &&
            !mongoc_collection_delete_one(collection,
                &owned, NULL, NULL, &error))
        ABTS_FAIL(tc, error.message);
    bson_destroy(&owned);
}

abts_suite *test_eir_service(abts_suite *suite)
{
    size_t i;

    suite = ADD_SUITE(suite)
    abts_run_test(suite, discovery, NULL);
    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
        abts_run_test(suite, service_request, (void *)&cases[i]);
    return suite;
}
