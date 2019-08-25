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

#include "ogs-core.h"
#include "core/abts.h"

#include "base/context.h"

#include "app-init.h"
#include "test-app.h"

abts_suite *test_mnc3(abts_suite *suite);

const struct testlist {
    abts_suite *(*func)(abts_suite *suite);
} alltests[] = {
    {test_mnc3},
    {NULL},
};

static void terminate(void)
{
    ogs_msleep(50);

    epc_child_terminate();
    app_terminate();

    base_finalize();
    ogs_core_finalize();
}

static void initialize(char **argv)
{
    int rv;

    ogs_core_initialize();
    base_initialize();

    rv = app_initialize(argv);
    ogs_assert(rv == OGS_OK);
}

int main(int argc, char **argv)
{
    int i;
    abts_suite *suite = NULL;

    atexit(terminate);
    test_main(argc, argv, "sample-simple.conf", initialize);

    for (i = 0; alltests[i].func; i++)
        suite = alltests[i].func(suite);

    return abts_report(suite);
}
