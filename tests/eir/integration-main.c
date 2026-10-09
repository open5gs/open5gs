/* Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
 * SPDX-License-Identifier: AGPL-3.0-or-later */

#include "integration.h"
#include <curl/curl.h>

const struct testlist {
    abts_suite *(*func)(abts_suite *suite);
} alltests[] = {
    {test_eir_dbi},
    {test_eir_service},
    {test_eir_registration},
    {test_eir_attach},
    {test_guti_epc},
    {test_guti_amf},
    {NULL},
};

static void terminate(void)
{
    ogs_msleep(50);

    test_child_terminate();
    app_terminate();

    test_app_final();
    ogs_app_terminate();
    curl_global_cleanup();
}

static void initialize(const char *const argv[])
{
    int rv;

    rv = ogs_app_initialize(NULL, NULL, argv);
    ogs_assert(rv == OGS_OK);
    test_app_init();

    rv = app_initialize(argv);
    ogs_assert(rv == OGS_OK);
}

int main(int argc, const char *const argv[])
{
    int i;
    abts_suite *suite = NULL;
    CURLcode rv;

    rv = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (rv != CURLE_OK) {
        fprintf(stderr, "EIR integration curl initialization failed: %s\n",
                curl_easy_strerror(rv));
        return EXIT_FAILURE;
    }

    atexit(terminate);
    test_app_run(argc, argv, "sample.yaml", initialize);

    for (i = 0; alltests[i].func; i++)
        suite = alltests[i].func(suite);

    return abts_report(suite);
}
