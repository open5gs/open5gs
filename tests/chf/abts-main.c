/*
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
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

/*
 * CHF unit test entry point.
 *
 * Tests covered:
 *   chf-quota-test.c    — quota engine defaults, volume math, exhaustion flags
 *   chf-cdr-test.c      — CDR file creation, JSON fields, directory structure,
 *                          the BER/JSON format switch and aggregated filing
 *   chf-cdr-asn1-test.c — the TS 32.298 record and its BER encoding
 *   chf-metrics-test.c  — the metric spec tables, that instrumentation is a
 *                          no-op before the metrics context exists, and the
 *                          init/populate/teardown cycle (METR-5, the only
 *                          case that starts a real metrics context)
 *   chf-sbi-test.c      — Phase 2: lib/sbi SUPI-range routing (ogs_sbi_check_chf_info_supi_range)
 *   chf-sbi-test.c      — Phase 2: lib/sbi SUPI-range routing
 *
 * Run:
 *   build/tests/chf/chf-unit
 *   meson test -C build --suite chf
 *
 * These tests run without a live NRF, MongoDB, or CHF process.
 * They exercise libchf internals directly using the abts framework.
 */

#include "ogs-app.h"
#include "ogs-sbi.h"
#include "chf/context.h"
#include "core/abts.h"

/* __ogs_sbi_domain, ogs_sbi_message_init/final declared in ogs-sbi.h */

abts_suite *test_chf_quota(abts_suite *suite);
abts_suite *test_chf_cdr(abts_suite *suite);
abts_suite *test_chf_cdr_asn1(abts_suite *suite);
abts_suite *test_chf_metrics(abts_suite *suite);
abts_suite *test_chf_sbi(abts_suite *suite);
abts_suite *test_chf_grant(abts_suite *suite);
abts_suite *test_chf_bucket(abts_suite *suite);

const struct testlist {
    abts_suite *(*func)(abts_suite *suite);
} alltests[] = {
    { test_chf_quota },
    { test_chf_cdr   },
    { test_chf_cdr_asn1 },
    { test_chf_metrics },
    { test_chf_sbi   },
    { test_chf_grant },
    { test_chf_bucket },
    { NULL },
};

static void terminate(void)
{
    chf_context_final();

    ogs_sbi_context_final();

    ogs_pkbuf_default_destroy();
    ogs_core_terminate();
}

int main(int argc, const char *const argv[])
{
    int rv, i, opt;
    ogs_getopt_t options;
    struct {
        char *log_level;
        char *domain_mask;
    } optarg;
    const char *argv_out[argc + 3];

    abts_suite *suite = NULL;
    ogs_pkbuf_config_t config;

    rv = abts_main(argc, argv, argv_out);
    if (rv != OGS_OK) return rv;

    memset(&optarg, 0, sizeof(optarg));
    ogs_getopt_init(&options, (char **)argv_out);
    while ((opt = ogs_getopt(&options, "e:m:")) != -1) {
        switch (opt) {
        case 'e': optarg.log_level   = options.optarg; break;
        case 'm': optarg.domain_mask = options.optarg; break;
        default:
            fprintf(stderr, "%s: should not be reached\n", OGS_FUNC);
            return OGS_ERROR;
        }
    }

    ogs_core_initialize();
    ogs_pkbuf_default_init(&config);
    ogs_pkbuf_default_create(&config);

    /*
     * Unit tests run without a full YAML config, so the pool sizes the
     * config parser would derive from global.max are unset.  Set them
     * directly, to test-suitable sizes, before anything allocates from
     * them.
     */
    ogs_app()->pool.sess = 128;
    ogs_app()->pool.nf = 16;
    ogs_app()->pool.nf_service = 16;
    ogs_app()->pool.event = 64;
    ogs_app()->pool.xact = 64;
    ogs_app()->pool.subscription = 16;
    ogs_app()->pool.message = 128;

    /*
     * METR-5 starts a real metrics context, and the prometheus backend
     * sizes its spec pool from this.  ogs_app_config() sets it to 512 in a
     * normal daemon; here it would be 0, and the first ogs_metrics_spec_new()
     * would assert on an empty pool.  512 is upstream's own default, so the
     * CHF's family count cannot outgrow it before the daemon's would.
     */
    ogs_app()->metrics.max_specs = 512;

    ogs_log_install_domain(&__ogs_sbi_domain, "sbi", OGS_LOG_ERROR);

    /*
     * The real SBI context, not just the message pools.
     *
     * The CDR writer records the CHF's own NF instance id as
     * recordingNetworkFunctionID, which TS 32.298 makes mandatory, and
     * that id belongs to the self NF-Instance ogs_sbi_context_init()
     * creates.  Initialising the whole context - which also sets up the
     * message pools this used to do by hand - is what lets the tests
     * exercise the same path the daemon does.
     */
    ogs_sbi_context_init(OpenAPI_nf_type_CHF);

    /* Initialize CHF context — sets quota/CDR defaults via chf_context_prepare() */
    chf_context_init();

    atexit(terminate);

    rv = ogs_log_config_domain(optarg.domain_mask, optarg.log_level);
    if (rv != OGS_OK) return rv;

    for (i = 0; alltests[i].func; i++)
        suite = alltests[i].func(suite);

    return abts_report(suite);
}
