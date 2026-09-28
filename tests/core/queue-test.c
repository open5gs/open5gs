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

#define NUMBER_CONSUMERS    3
#define CONSUMER_ACTIVITY   4
#define NUMBER_PRODUCERS    4
#define PRODUCER_ACTIVITY   5
#define QUEUE_SIZE          100

static ogs_queue_t *queue;

static void consumer(void *data)
{
    long sleeprate;
    abts_case *tc = data;
    int rv;
    void *v;

    sleeprate = 1000000/CONSUMER_ACTIVITY;
    ogs_usleep((ogs_random32() % 4) * 1000000); /* sleep random seconds */

    while (1) {
        rv = ogs_queue_pop(queue, &v);

        if (rv == OGS_ERROR)
            continue;

        if (rv == OGS_DONE)
            break;

        ABTS_TRUE(tc, v == NULL);
        ABTS_TRUE(tc, rv == OGS_OK);

        ogs_usleep(sleeprate); /* sleep this long to acheive our rate */
    }
}

static void producer(void *data)
{
    long sleeprate;
    abts_case *tc = data;
    int rv;

    sleeprate = 1000000/PRODUCER_ACTIVITY;
    ogs_usleep((ogs_random32() % 4) * 1000000); /* sleep random seconds */

    while (1)
    {
        rv = ogs_queue_push(queue, NULL);

        if (rv == OGS_ERROR)
            continue;

        if (rv == OGS_DONE)
            break;

        ABTS_TRUE(tc, rv == OGS_OK);

        ogs_usleep(sleeprate); /* sleep this long to acheive our rate */
    }
}

static void test_queue_producer_consumer(abts_case *tc, void *data)
{
    unsigned int i;
    int rv;
    ogs_thread_t *consumer_thread[NUMBER_CONSUMERS];
    ogs_thread_t *producer_thread[NUMBER_PRODUCERS];

    queue = ogs_queue_create(QUEUE_SIZE);
    ABTS_PTR_NOTNULL(tc, queue);

    for (i = 0; i < NUMBER_CONSUMERS; i++) {
        consumer_thread[i] = ogs_thread_create(consumer, tc);
        ABTS_PTR_NOTNULL(tc, consumer_thread[i]);
    }

    for (i = 0; i < NUMBER_PRODUCERS; i++) {
        producer_thread[i] = ogs_thread_create(producer, tc);
        ABTS_PTR_NOTNULL(tc, producer_thread[i]);
    }

    ogs_msleep(3000); /* sleep 3 seconds */

    rv = ogs_queue_term(queue);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    for (i = 0; i < NUMBER_CONSUMERS; i++) {
        ogs_thread_destroy(consumer_thread[i]);
    }

    for (i = 0; i < NUMBER_PRODUCERS; i++) {
        ogs_thread_destroy(producer_thread[i]);
    }

    ogs_queue_destroy(queue);
}

static void test_queue_timeout(abts_case *tc, void *data)
{
    ogs_queue_t *q;
    int rv;
    ogs_time_t start;
    unsigned int i;
    void *value;

    q = ogs_queue_create(5);
    ABTS_PTR_NOTNULL(tc, q);

    for (i = 0; i < 2; ++i) {
        rv = ogs_queue_timedpush(q, NULL, ogs_time_from_msec(1));
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }
    for (i = 0; i < 3; ++i) {
        rv = ogs_queue_trypush(q, NULL);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    start = ogs_get_monotonic_time();
    rv = ogs_queue_timedpush(q, NULL, ogs_time_from_msec(1));
    ABTS_INT_EQUAL(tc, OGS_TIMEUP, rv);
    ABTS_TRUE(tc, ogs_get_monotonic_time() - start >= ogs_time_from_msec(1));

    rv = ogs_queue_trypush(q, NULL);
    ABTS_INT_EQUAL(tc, OGS_RETRY, rv);

    for (i = 0; i < 2; ++i) {
        rv = ogs_queue_timedpop(q, &value, ogs_time_from_msec(1));
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }
    for (i = 0; i < 3; ++i) {
        rv = ogs_queue_trypop(q, &value);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);
    }

    start = ogs_get_monotonic_time();
    rv = ogs_queue_timedpop(q, &value, ogs_time_from_sec(1));
    ABTS_INT_EQUAL(tc, OGS_TIMEUP, rv);
    ABTS_TRUE(tc, ogs_get_monotonic_time() - start >= ogs_time_from_msec(1));

    rv = ogs_queue_trypop(q, &value);
    ABTS_INT_EQUAL(tc, OGS_RETRY, rv);

    rv = ogs_queue_term(q);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_queue_destroy(q);
}

typedef struct {
    abts_case *tc;
    unsigned int order;
    unsigned int *next;
} queue_cleanup_item_t;

static void queue_cleanup(void *data)
{
    queue_cleanup_item_t *item = data;

    ABTS_INT_EQUAL(item->tc, item->order, *item->next);
    (*item->next)++;
}

static void test_queue_drain(abts_case *tc, void *data)
{
    ogs_queue_t *q = ogs_queue_create(3);
    queue_cleanup_item_t items[4];
    unsigned int i, next = 1;
    void *value = NULL;

    ABTS_PTR_NOTNULL(tc, q);
    for (i = 0; i < 4; i++) {
        items[i].tc = tc;
        items[i].order = i;
        items[i].next = &next;
    }

    for (i = 0; i < 3; i++)
        ABTS_INT_EQUAL(tc, OGS_OK, ogs_queue_trypush(q, &items[i]));
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_queue_trypop(q, &value));
    ABTS_TRUE(tc, value == &items[0]);
    /* Wrap around before termination, leaving three queued items. */
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_queue_trypush(q, &items[3]));
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_queue_term(q));

    ABTS_INT_EQUAL(tc, OGS_DONE, ogs_queue_trypop(q, &value));
    ABTS_INT_EQUAL(tc, OGS_DONE, ogs_queue_push(q, &items[0]));
    ABTS_INT_EQUAL(tc, OGS_DONE, ogs_queue_trypush(q, &items[0]));
    ABTS_INT_EQUAL(tc, OGS_DONE,
            ogs_queue_timedpush(q, &items[0], ogs_time_from_msec(1)));

    ogs_queue_drain(q, queue_cleanup);
    ABTS_INT_EQUAL(tc, 4, next);
    ABTS_INT_EQUAL(tc, 0, ogs_queue_size(q));
    ogs_queue_drain(q, queue_cleanup);
    ABTS_INT_EQUAL(tc, 4, next);
    ABTS_INT_EQUAL(tc, OGS_DONE, ogs_queue_trypush(q, &items[0]));
    ogs_queue_destroy(q);
}

abts_suite *test_queue(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_queue_producer_consumer, NULL);
    abts_run_test(suite, test_queue_timeout, NULL);
    abts_run_test(suite, test_queue_drain, NULL);

    return suite;
}
