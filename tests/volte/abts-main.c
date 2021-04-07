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

#include "test-fd-path.h"

abts_suite *test_bearer(abts_suite *suite);
abts_suite *test_session(abts_suite *suite);
abts_suite *test_rx(abts_suite *suite);
abts_suite *test_video(abts_suite *suite);
abts_suite *test_cx(abts_suite *suite);

const struct testlist {
    abts_suite *(*func)(abts_suite *suite);
} alltests[] = {
    {test_bearer},
    {test_session},
    {test_rx},
    {test_video},
    {test_cx},
    {NULL},
};

static void terminate(void)
{
    ogs_msleep(50);

    test_child_terminate();
    app_terminate();

    test_fd_final();

    test_epc_final();
    ogs_app_terminate();
}

static void initialize(const char *const argv[])
{
    int rv;

    rv = ogs_app_initialize(NULL, NULL, argv);
    ogs_assert(rv == OGS_OK);
    test_epc_init();

    rv = test_fd_init();
    ogs_assert(rv == OGS_OK);

    rv = app_initialize(argv);
    ogs_assert(rv == OGS_OK);
}

int main(int argc, const char *const argv[])
{
    int i;
    abts_suite *suite = NULL;

    atexit(terminate);
    test_app_run(argc, argv, "volte.yaml", initialize);

    for (i = 0; alltests[i].func; i++)
        suite = alltests[i].func(suite);

    return abts_report(suite);
}
