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

static ogs_thread_mutex_t lock;
static int x = 0;

#define LOCK_LOOP 10000
#define THREAD_NUM 100

static ogs_thread_t *thread[THREAD_NUM];

static void thread_func(void *data)
{
    int i;

    for (i = 0; i < LOCK_LOOP; i++) {
        ogs_thread_mutex_lock(&lock);
        x++;
        ogs_thread_mutex_unlock(&lock);
    }
}

static void init_thread(abts_case *tc, void *data)
{
    ogs_thread_mutex_init(&lock);
}

static void create_threads(abts_case *tc, void *data)
{
    int i = 0;

    for (i = 0; i < THREAD_NUM; i++) {
        thread[i] = ogs_thread_create(thread_func, NULL);
    }
}

static void delete_threads(abts_case *tc, void *data)
{
    int i = 0;

    for (i = 0; i < THREAD_NUM; i++) {
        ogs_thread_destroy(thread[i]);
    }
}

static void check_locks(abts_case *tc, void *data)
{
    ABTS_INT_EQUAL(tc, THREAD_NUM * LOCK_LOOP, x);
}

static void final_thread(abts_case *tc, void *data)
{
    ogs_thread_mutex_destroy(&lock);
}

abts_suite *test_thread(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, init_thread, NULL);
    abts_run_test(suite, create_threads, NULL);
    abts_run_test(suite, delete_threads, NULL);
    abts_run_test(suite, check_locks, NULL);
    abts_run_test(suite, final_thread, NULL);

    return suite;
}
