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

/* 2002-09-14 12:05:36.186711 -25200 [257 Sat]. */
static ogs_time_t now = 1032030336186711L;

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

static void test_gmtstr(abts_case *tc, void *data)
{
    struct tm tm;
    char *str = NULL;

    char datetime[STR_SIZE];

    ogs_gmtime(ogs_time_sec(now), &tm);
    ogs_strftime(datetime, sizeof datetime, "%Y-%m-%dT%H:%M:%S", &tm);

    str = ogs_msprintf("%s.%06lldZ",
            datetime, (long long)ogs_time_usec(now));
    ogs_assert(str);

    ABTS_STR_EQUAL(tc, "2002-09-14T19:05:36.186711Z", str);

    ogs_free(str);
}

static void test_get_gmt(abts_case *tc, void *data)
{
    int rv;
    struct tm xt;
    ogs_time_t imp;
    int64_t hr_off_64;

    ogs_gmtime(ogs_time_sec(now), &xt);
    rv = ogs_time_from_lt(&imp, &xt, ogs_time_usec(now));
    ABTS_TRUE(tc, rv == OGS_OK);
    hr_off_64 = (int64_t) xt.tm_gmtoff * OGS_USEC_PER_SEC;
    ABTS_TRUE(tc, now + hr_off_64 == imp);
}

static void test_get_lt(abts_case *tc, void *data)
{
    int rv;
    struct tm xt;
    ogs_time_t imp;
    int64_t hr_off_64;

    ogs_localtime(ogs_time_sec(now), &xt);
    rv = ogs_time_from_lt(&imp, &xt, ogs_time_usec(now));
    ABTS_TRUE(tc, rv == OGS_OK);
    hr_off_64 = (int64_t) xt.tm_gmtoff * OGS_USEC_PER_SEC;
    ABTS_TRUE(tc, now + hr_off_64 == imp);
}

static void test_imp_gmt(abts_case *tc, void *data)
{
    int rv;
    struct tm xt;
    ogs_time_t imp;

    ogs_gmtime(ogs_time_sec(now), &xt);
    rv = ogs_time_from_gmt(&imp, &xt, ogs_time_usec(now));
    ABTS_TRUE(tc, rv == OGS_OK);
    ABTS_TRUE(tc, now == imp);
}

abts_suite *test_time(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_now, NULL);
    abts_run_test(suite, test_gmtstr, NULL);
    abts_run_test(suite, test_get_gmt, NULL);
    abts_run_test(suite, test_get_lt, NULL);
    abts_run_test(suite, test_imp_gmt, NULL);

    return suite;
}
