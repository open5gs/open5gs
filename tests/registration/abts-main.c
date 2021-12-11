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

abts_suite *test_guti(abts_suite *suite);
abts_suite *test_auth(abts_suite *suite);
abts_suite *test_idle(abts_suite *suite);
abts_suite *test_dereg(abts_suite *suite);
abts_suite *test_paging(abts_suite *suite);
abts_suite *test_identity(abts_suite *suite);
abts_suite *test_gmm_status(abts_suite *suite);
abts_suite *test_ue_context(abts_suite *suite);
abts_suite *test_reset(abts_suite *suite);
abts_suite *test_crash(abts_suite *suite);

const struct testlist {
    abts_suite *(*func)(abts_suite *suite);
} alltests[] = {
    {test_guti},
    {test_auth},
    {test_idle},
    {test_dereg},
    {test_paging},
    {test_identity},
    {test_gmm_status},
    {test_ue_context},
    {test_reset},
#if 0 /* Since there is error LOG, we disabled the following test */
    {test_crash},
#endif
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
    int i;
    abts_suite *suite = NULL;

    atexit(terminate);
    test_app_run(argc, argv, "sample.yaml", initialize);

    for (i = 0; alltests[i].func; i++)
        suite = alltests[i].func(suite);

    return abts_report(suite);
}
