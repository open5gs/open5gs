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

#define TEST_TIMER_NUM          200
#define TEST_TIMER_PRECISION    20000
#define TEST_DURATION           400000

static uint8_t expire_check[TEST_DURATION/TEST_TIMER_PRECISION];
static ogs_time_t timer_duration[] = { 500000, 50000, 200000, 90000, 800000 };

void test_expire_func_1(void *data)
{
    int index = (uintptr_t)data;

    expire_check[index] = TRUE;
}

void test_expire_func_2(void *data)
{
    int index = (uintptr_t)data;

    expire_check[index]++;
}

/* basic timer Test */
static void test1_func(abts_case *tc, void *data)
{
    int n = 0;
    ogs_pollset_t *pollset = NULL;
    ogs_timer_mgr_t *timer = NULL;
    ogs_timer_t *timer_array[TEST_TIMER_NUM];

    memset(expire_check, 0, TEST_DURATION/TEST_TIMER_PRECISION);

    timer = ogs_timer_mgr_create(512);
    pollset = ogs_pollset_create(512);
    ogs_assert(timer);
    for(n = 0; n < sizeof(timer_duration)/sizeof(ogs_time_t); n++) {
        timer_array[n] = ogs_timer_add(
                timer, test_expire_func_1, (void*)(uintptr_t)n);
        ogs_assert(timer_array[n]);
        ogs_timer_start(timer_array[n], timer_duration[n]);
    }

    ogs_pollset_poll(pollset, ogs_timer_mgr_next(timer));
    ogs_timer_mgr_expire(timer);

    ABTS_INT_EQUAL(tc, 0, expire_check[0]);
    ABTS_INT_EQUAL(tc, 1, expire_check[1]);
    ABTS_INT_EQUAL(tc, 0, expire_check[2]);
    ABTS_INT_EQUAL(tc, 0, expire_check[3]);
    ABTS_INT_EQUAL(tc, 0, expire_check[4]);

    ogs_pollset_poll(pollset, ogs_timer_mgr_next(timer));
    ogs_timer_mgr_expire(timer);

    ABTS_INT_EQUAL(tc, 0, expire_check[0]);
    ABTS_INT_EQUAL(tc, 1, expire_check[1]);
    ABTS_INT_EQUAL(tc, 0, expire_check[2]);
    ABTS_INT_EQUAL(tc, 1, expire_check[3]);
    ABTS_INT_EQUAL(tc, 0, expire_check[4]);

    ogs_pollset_poll(pollset, ogs_timer_mgr_next(timer));
    ogs_timer_mgr_expire(timer);

    ABTS_INT_EQUAL(tc, 0, expire_check[0]);
    ABTS_INT_EQUAL(tc, 1, expire_check[1]);
    ABTS_INT_EQUAL(tc, 1, expire_check[2]);
    ABTS_INT_EQUAL(tc, 1, expire_check[3]);
    ABTS_INT_EQUAL(tc, 0, expire_check[4]);

    ogs_pollset_poll(pollset, ogs_timer_mgr_next(timer));
    ogs_timer_mgr_expire(timer);

    ABTS_INT_EQUAL(tc, 1, expire_check[0]);
    ABTS_INT_EQUAL(tc, 1, expire_check[1]);
    ABTS_INT_EQUAL(tc, 1, expire_check[2]);
    ABTS_INT_EQUAL(tc, 1, expire_check[3]);
    ABTS_INT_EQUAL(tc, 0, expire_check[4]);

    ogs_pollset_poll(pollset, ogs_timer_mgr_next(timer));
    ogs_timer_mgr_expire(timer);
    ABTS_INT_EQUAL(tc, OGS_INFINITE_TIME, ogs_timer_mgr_next(timer));

    ABTS_INT_EQUAL(tc, 1, expire_check[0]);
    ABTS_INT_EQUAL(tc, 1, expire_check[1]);
    ABTS_INT_EQUAL(tc, 1, expire_check[2]);
    ABTS_INT_EQUAL(tc, 1, expire_check[3]);
    ABTS_INT_EQUAL(tc, 1, expire_check[4]);

    for(n = 0; n < sizeof(timer_duration)/sizeof(ogs_time_t); n++)
        ogs_timer_delete(timer_array[n]);

    ogs_timer_mgr_destroy(timer);
    ogs_pollset_destroy(pollset);
}

static void test2_func(abts_case *tc, void *data)
{
    int n = 0;
    ogs_timer_mgr_t *timer = NULL;
    ogs_timer_t *timer_array[TEST_TIMER_NUM];
    int duration;
    int tm_num[TEST_DURATION/TEST_TIMER_PRECISION];
    int tm_idx;

    memset(expire_check, 0, TEST_DURATION/TEST_TIMER_PRECISION);
    memset(tm_num, 0, sizeof(int)*(TEST_DURATION/TEST_TIMER_PRECISION));

    timer = ogs_timer_mgr_create(512);
    ogs_assert(timer);

    for(n = 0; n < TEST_TIMER_NUM; n++) {
        duration = (ogs_random32() % (TEST_DURATION/TEST_TIMER_PRECISION))
            * TEST_TIMER_PRECISION;

        tm_idx = duration/TEST_TIMER_PRECISION;
        tm_num[tm_idx]++;
        duration += (TEST_TIMER_PRECISION >> 1);

        timer_array[n] = ogs_timer_add(
                timer, test_expire_func_2, (void*)(uintptr_t)tm_idx);
        ogs_assert(timer_array[n]);
        ogs_timer_start(timer_array[n], duration);
    }

    for(n = 0; n < TEST_DURATION/TEST_TIMER_PRECISION; n++) {
        ogs_usleep(TEST_TIMER_PRECISION);
        ogs_timer_mgr_expire(timer);
        ABTS_INT_EQUAL(tc, tm_num[n], expire_check[n]);
    }

    for(n = 0; n < TEST_TIMER_NUM; n++)
        ogs_timer_delete(timer_array[n]);

    ogs_timer_mgr_destroy(timer);
}

static void test3_func(abts_case *tc, void *data)
{
    int n = 0;
    ogs_timer_mgr_t *timer = NULL;
    ogs_timer_t *timer_array[TEST_TIMER_NUM];
    int id_duration[TEST_TIMER_NUM];
    int duration;
    int tm_num[TEST_DURATION/TEST_TIMER_PRECISION];
    int tm_idx, tm_check_id;

    memset(expire_check, 0, TEST_DURATION/TEST_TIMER_PRECISION);
    memset(tm_num, 0, sizeof(int)*(TEST_DURATION/TEST_TIMER_PRECISION));

    timer = ogs_timer_mgr_create(512);
    ogs_assert(timer);

    for(n = 0; n < TEST_TIMER_NUM; n++) {
        duration = (ogs_random32() % (TEST_DURATION/TEST_TIMER_PRECISION))
            * TEST_TIMER_PRECISION;

        tm_idx = duration/TEST_TIMER_PRECISION;
        tm_num[tm_idx]++;
        id_duration[n] = duration;
        duration += (TEST_TIMER_PRECISION >> 1);

        timer_array[n] = ogs_timer_add(
                timer, test_expire_func_2, (void*)(uintptr_t)tm_idx);
        ogs_assert(timer_array[n]);
        ogs_timer_start(timer_array[n], duration);
    }

    for(n = 0; n < TEST_TIMER_NUM / 10; n++) {
        tm_idx = n*10 + ogs_random32()%10;
        tm_check_id = id_duration[tm_idx]/TEST_TIMER_PRECISION;
        tm_num[tm_check_id]--;
        ogs_timer_delete(timer_array[tm_idx]);
        timer_array[tm_idx] = NULL;
    }

    for(n = 0; n < TEST_DURATION/TEST_TIMER_PRECISION; n++) {
        ogs_usleep(TEST_TIMER_PRECISION);
        ogs_timer_mgr_expire(timer);
        ABTS_INT_EQUAL(tc, tm_num[n], expire_check[n]);
    }

    for(n = 0; n < TEST_TIMER_NUM; n++) {
        if (timer_array[n])
            ogs_timer_delete(timer_array[n]);
    }

    ogs_timer_mgr_destroy(timer);
}

abts_suite *test_timer(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test2_func, NULL);
    abts_run_test(suite, test3_func, NULL);

    return suite;
}
