/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "test-app.h"

abts_suite *test_4g_proxy(abts_suite *suite);

const struct testlist {
    abts_suite *(*func)(abts_suite *suite);
} alltests[] = {
    {test_4g_proxy},
    // TODO Test FE/BE separation
    // TODO Test Handling of ModifyBearer
    {NULL},
};

static ogs_thread_t *hss_thread = NULL;
static ogs_thread_t *pcrf_thread = NULL;
static ogs_thread_t *smf_thread = NULL;
static ogs_thread_t *upf_thread = NULL;
static ogs_thread_t *sgwu_thread = NULL;
static ogs_thread_t *proxy_thread = NULL;

static void initialize(const char *const argv[])
{
    int rv;

    rv = ogs_app_initialize(NULL, NULL, argv);
    ogs_assert(rv == OGS_OK);

    ogs_gtp_context_init(OGS_MAX_NUM_OF_GTPU_RESOURCE);
    ogs_gtp_xact_init();

    ogs_log_install_domain(&__ogs_dbi_domain, "dbi", OGS_LOG_ERROR);
    ogs_log_install_domain(&__ogs_gtp_domain, "gtp", OGS_LOG_ERROR);
    ogs_assert(ogs_dbi_init(ogs_app()->db_uri) == OGS_OK);

    hss_thread = test_child_create("hss", argv);
    pcrf_thread = test_child_create("pcrf", argv);
    smf_thread = test_child_create("smf", argv);
    upf_thread = test_child_create("upf", argv);
    sgwu_thread = test_child_create("sgwu", argv);
    proxy_thread = test_child_create_ex("gtp-proxy", "", argv);
}

static void terminate(void)
{
    ogs_msleep(50);

    test_child_terminate();

    ogs_gtp_xact_final();
    ogs_gtp_context_final();
    test_5gc_final();

    ogs_thread_destroy(proxy_thread);
    ogs_thread_destroy(sgwu_thread);
    ogs_thread_destroy(upf_thread);
    ogs_thread_destroy(smf_thread);
    ogs_thread_destroy(pcrf_thread);
    ogs_thread_destroy(hss_thread);
}

int main(int argc, const char *const argv[])
{
    int i;
    abts_suite *suite = NULL;

    atexit(terminate);

    const char *argv_out[OGS_ARG_MAX];
    for (i = 0; argv[i] && i < OGS_ARG_MAX-3; i++) {
        argv_out[i] = argv[i];
    }
    argv_out[i++] = "-c";
    argv_out[i++] = CONFIG_FILE;
    argv_out[i] = NULL;

    test_app_run(argc + 2, argv_out, NULL, initialize);

    for (i = 0; alltests[i].func; i++)
        suite = alltests[i].func(suite);

    return abts_report(suite);
}
