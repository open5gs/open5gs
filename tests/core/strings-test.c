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

static void test_slprintf(abts_case *tc, void *data)
{
    char str[8];
    char *last = str + 8;
    char *buffer = str;

    buffer = ogs_slprintf(buffer, last, "1234");
    ABTS_TRUE(tc, strcmp(str, "1234") == 0);
    buffer = ogs_slprintf(buffer, last, "56");
    ABTS_TRUE(tc, strcmp(str, "123456") == 0);
    buffer = ogs_slprintf(buffer, last, "7");
    ABTS_TRUE(tc, strcmp(str, "1234567") == 0);
    buffer = ogs_slprintf(buffer, last, "8");
    ABTS_TRUE(tc, strcmp(str, "1234567") == 0);
    buffer = ogs_slprintf(buffer, last, "9");
    ABTS_TRUE(tc, strcmp(str, "1234567") == 0);
    buffer = ogs_slprintf(buffer, last, "10");
    ABTS_TRUE(tc, strcmp(str, "1234567") == 0);
}

abts_suite *test_strings(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_slprintf, NULL);

    return suite;
}
