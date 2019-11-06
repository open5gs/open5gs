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

static void test_now(abts_case *tc, void *data)
{
    struct timeval tv;
    time_t timediff;
    time_t current;
    time_t os_now;

    ogs_gettimeofday(&tv);
    current = tv.tv_sec;
    time(&os_now);

    timediff = os_now - current;
    ABTS_ASSERT(tc, "ogs_gettimeofday() and time() do not agree",
            (timediff > -2) && (timediff < 2));
}

#define STR_SIZE 100

static void test_strftime(abts_case *tc, void *data)
{
    struct tm tm;
    char str[STR_SIZE+1];
    time_t now = (time_t)1542100786;

    ogs_gmtime(now, &tm);
    strftime(str, sizeof str, "%Y/%m/%d %H:%M:%S", &tm);
    ABTS_STR_EQUAL(tc, "2018/11/13 09:19:46", str);

    ogs_localtime(now, &tm);
    strftime(str, sizeof str, "%Y/%m/%d %H:%M:%S", &tm);
#if 0 /* FIXME */
    ABTS_STR_EQUAL(tc, "2018/11/13 18:19:46", str);
#endif
}

abts_suite *test_time(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_now, NULL);
    abts_run_test(suite, test_strftime, NULL);

    return suite;
}
