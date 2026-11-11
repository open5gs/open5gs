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
#ifdef TEST_S10_SGW_RELOCATION
static ogs_thread_t *second_sgwc, *second_sgwu;
#define SOURCE_CONFIG "s10-sgw-source.yaml"
#define TARGET_CONFIG "s10-sgw-target.yaml"
#else
#define SOURCE_CONFIG "s10-source.yaml"
#define TARGET_CONFIG "s10-target.yaml"
#endif

static void terminate(void)
{
    ogs_msleep(50);

    test_child_terminate();
    if (second_mme) ogs_thread_destroy(second_mme);
#ifdef TEST_S10_SGW_RELOCATION
    if (second_sgwc) ogs_thread_destroy(second_sgwc);
    if (second_sgwu) ogs_thread_destroy(second_sgwu);
#endif
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
#ifdef TEST_S10_SGW_RELOCATION
    {
        const char *sgwu_argv[] = {"sgwu", "-c", MESON_BUILD_ROOT "/configs/" TARGET_CONFIG, NULL};
        const char *sgwc_argv[] = {"sgwc", "-c", MESON_BUILD_ROOT "/configs/" TARGET_CONFIG, NULL};
#ifndef TEST_S10_SGW_FAILURE
        second_sgwu = test_child_create("sgwu", 1, sgwu_argv);
#else
        (void)sgwu_argv;
#endif
#ifndef TEST_S10_SGW_TIMEOUT
        second_sgwc = test_child_create("sgwc", 1, sgwc_argv);
#else
        (void)sgwc_argv;
#endif
#ifndef TEST_S10_SGW_FAILURE
        ogs_assert(second_sgwu && second_sgwc);
#endif
    }
#endif
    {
        const char *target_argv[] = {"mme", "-c",
            MESON_BUILD_ROOT "/configs/" TARGET_CONFIG, NULL};
        second_mme = test_child_create("mme", 1, target_argv);
        ogs_assert(second_mme);
        ogs_msleep(5000);
    }
}

abts_suite *test_s10_connected(abts_suite *suite);

int main(int argc, const char *const argv[])
{
    abts_suite *suite = NULL;

    atexit(terminate);
    test_app_run(argc, argv, SOURCE_CONFIG, initialize);

    suite = test_s10_mobility(suite);
    suite = test_s10_connected(suite);

    return abts_report(suite);
}
