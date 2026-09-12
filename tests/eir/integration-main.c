/* Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
 * SPDX-License-Identifier: AGPL-3.0-or-later */

#include "integration.h"
#include <curl/curl.h>

static char *eir_uri;
static char *nrf_uri;

const char *test_eir_sbi_uri(void)
{
    return eir_uri;
}

const char *test_eir_nrf_uri(void)
{
    return nrf_uri;
}

static bool config_member(ogs_yaml_iter_t *parent,
        const char *name, ogs_yaml_iter_t *child)
{
    ogs_yaml_iter_t iter = *parent;

    if (ogs_yaml_iter_type(&iter) != YAML_MAPPING_NODE)
        return false;

    while (ogs_yaml_iter_next(&iter)) {
        if (!strcmp(ogs_yaml_iter_key(&iter), name)) {
            ogs_yaml_iter_recurse(&iter, child);
            return true;
        }
    }
    return false;
}

static char *config_sbi_uri(const char *nf)
{
    ogs_yaml_iter_t node, server, address, port;
    const char *host, *value;
    const char *field = "section";
    const char *reason = "missing or invalid mapping";
    char *end = NULL, *uri;
    unsigned long number;

    ogs_yaml_iter_init(&node, ogs_app()->document);
    if (!config_member(&node, nf, &node))
        goto invalid;
    field = "sbi";
    if (!config_member(&node, "sbi", &node))
        goto invalid;
    field = "sbi.server";
    if (!config_member(&node, "server", &node))
        goto invalid;

    if (ogs_yaml_iter_type(&node) == YAML_SEQUENCE_NODE) {
        reason = "empty server list";
        if (!ogs_yaml_iter_next(&node))
            goto invalid;
        ogs_yaml_iter_recurse(&node, &server);
        reason = "multiple servers are not supported by this test";
        if (ogs_yaml_iter_next(&node))
            goto invalid;
        node = server;
    }

    field = "sbi.server.address";
    reason = "missing or non-scalar value";
    if (!config_member(&node, "address", &address) ||
            ogs_yaml_iter_type(&address) != YAML_SCALAR_NODE)
        goto invalid;
    host = ogs_yaml_iter_value(&address);
    reason = "empty address";
    if (!*host)
        goto invalid;

    field = "sbi.server.port";
    reason = "missing or non-scalar value";
    if (!config_member(&node, "port", &port) ||
            ogs_yaml_iter_type(&port) != YAML_SCALAR_NODE)
        goto invalid;
    value = ogs_yaml_iter_value(&port);
    number = strtoul(value, &end, 10);
    reason = "port must be an integer between 1 and 65535";
    if (!*value || !end || *end || !number || number > 65535)
        goto invalid;

    uri = strchr(host, ':') ?
        ogs_msprintf("http://[%s]:%lu", host, number) :
        ogs_msprintf("http://%s:%lu", host, number);
    if (!uri)
        ogs_error("Cannot allocate EIR integration SBI URI for %s", nf);
    return uri;

invalid:
    ogs_error("Invalid EIR integration config [%s.%s]: %s",
            nf, field, reason);
    return NULL;
}

static void terminate(void)
{
    ogs_msleep(50);

    test_child_terminate();
    app_terminate();
    test_5gc_final();

    ogs_free(eir_uri);
    ogs_free(nrf_uri);
    ogs_app_terminate();
    curl_global_cleanup();
}

static void initialize(const char *const argv[])
{
    int rv;

    rv = ogs_app_initialize(NULL, NULL, argv);
    if (rv != OGS_OK)
        ogs_error("EIR integration application setup failed [error:%d]", rv);
    ogs_assert(rv == OGS_OK);
    test_5gc_init();

    eir_uri = config_sbi_uri("eir");
    nrf_uri = config_sbi_uri("nrf");
    ogs_assert(eir_uri && nrf_uri);

    rv = app_initialize(argv);
    if (rv != OGS_OK)
        ogs_error("EIR integration NF startup failed [error:%d]", rv);
    ogs_assert(rv == OGS_OK);
}

int main(int argc, const char *const argv[])
{
    abts_suite *suite = NULL;
    CURLcode rv;

    rv = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (rv != CURLE_OK) {
        fprintf(stderr, "EIR integration curl initialization failed: %s\n",
                curl_easy_strerror(rv));
        return EXIT_FAILURE;
    }

    atexit(terminate);
    test_app_run(argc, argv, "eir.yaml", initialize);

    /* Discovery confirms that EIR registered its SBI service with the NRF. */
    if (!test_eir_wait_ready()) {
        ogs_error("EIR did not become ready before the integration timeout");
        return EXIT_FAILURE;
    }

    suite = test_eir_dbi(suite);
    suite = test_eir_service(suite);
    suite = test_eir_registration(suite);
    return abts_report(suite);
}
