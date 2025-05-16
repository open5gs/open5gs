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

#include "test-app.h"

abts_suite * test_ue_context_transfer(abts_suite *suite);
abts_suite * test_ue_context_transfer_error_case(abts_suite *suite);

struct testlist {
    abts_suite *(*func)(abts_suite *suite);
};

const struct testlist alltests[] = {
    {test_ue_context_transfer},
    {NULL},
};

const struct testlist alltests_error[] = {
    {test_ue_context_transfer_error_case},
    {NULL},
};

static void terminate(void)
{
    ogs_msleep(50);

    test_child_terminate();
    app_terminate();

    test_5gc_final();
    ogs_app_terminate();
}

static void initialize(const char *const argv[])
{
    int rv;

    rv = ogs_app_initialize(NULL, NULL, argv);
    ogs_assert(rv == OGS_OK);
    test_5gc_init();

    rv = app_initialize(argv);
    ogs_assert(rv == OGS_OK);
}

int main(int argc, const char *const argv[])
{
    int i, j, rv;
    abts_suite *suite = NULL;

    struct test_config {
        const struct testlist *tests;
        const char *config;
    };

    /*
     * Tests run the app with different configs:
     * - alltests runs positive test scenarios with scp included in 5g core
     * - alltests_error runs error case scenario with configuration "no_scp: true"
     */
    struct test_config test_configs[] = {
        {alltests, "transfer.yaml"},
        {alltests_error, "transfer-error-case.yaml"}
    };

    int num_configs = sizeof(test_configs) / sizeof(test_configs[0]);

    for (j = 0; j < num_configs; j++) {
        test_app_run(argc, argv, test_configs[j].config, initialize);

        for (i = 0; test_configs[j].tests[i].func; i++) {
            suite = test_configs[j].tests[i].func(suite);
        }

        rv = abts_report(suite);
        if (rv != OGS_OK)
            ogs_error("abts_report error for config: %s", test_configs[j].config);

        suite = NULL;
        terminate();
    }

    return rv;
}
