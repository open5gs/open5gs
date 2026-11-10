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

#include "test-config-private.h"
#include "test-app.h"

abts_suite *test_s10_mobility(abts_suite *suite);
static ogs_thread_t *second_mme;

static void terminate(void)
{
    ogs_msleep(50);

    test_child_terminate();
    if (second_mme) ogs_thread_destroy(second_mme);
    app_terminate();

    test_epc_final();
    ogs_app_terminate();
}

static void initialize(const char *const argv[])
{
    int rv;

    rv = ogs_app_initialize(NULL, NULL, argv);
    ogs_assert(rv == OGS_OK);
    test_epc_init();

    rv = app_initialize(argv);
    ogs_assert(rv == OGS_OK);
    {
        const char *target_argv[] = {"mme", "-c",
            MESON_BUILD_ROOT "/configs/s10-target.yaml", NULL};
        second_mme = test_child_create("mme", 1, target_argv);
        ogs_assert(second_mme);
        ogs_msleep(5000);
    }
}

int main(int argc, const char *const argv[])
{
    abts_suite *suite = NULL;

    atexit(terminate);
    test_app_run(argc, argv, "s10-source.yaml", initialize);

    suite = test_s10_mobility(suite);

    return abts_report(suite);
}
