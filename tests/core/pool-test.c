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

typedef struct {
    char m1;
    int m2;
} pt_type1;

#define SIZE_OF_TPOOL1  5
#define SIZE_OF_TPOOL2  5

typedef char type_of_tpool1;
typedef pt_type1 type_of_tpool2;

static OGS_POOL(tpool1, type_of_tpool1);
static OGS_POOL(tpool2, type_of_tpool2);

static void test_sub_func1(abts_case *tc, void *data, int start)
{
    int i, j, n;

    type_of_tpool1 *org[SIZE_OF_TPOOL1+1];
    type_of_tpool1 *node[SIZE_OF_TPOOL1+1];

    ogs_pool_init(&tpool1, SIZE_OF_TPOOL1);

    n = ogs_pool_size(&tpool1);
    ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL1, n);
    n = ogs_pool_avail(&tpool1);
    ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL1, n);
    n = ogs_pool_size(&tpool1) - ogs_pool_avail(&tpool1);
    ABTS_INT_EQUAL(tc, 0, n);

    for (i = 0; i < start; i++) {
        ogs_pool_alloc(&tpool1, &node[i]);
        ABTS_PTR_NOTNULL(tc, node[i]);
        n = ogs_pool_avail(&tpool1);
        ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL1 - (i + 1), n);
    }

    for (i = 0; i < start; i++) {
        ogs_pool_free(&tpool1, node[i]);
        n = ogs_pool_avail(&tpool1);
        ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL1 - start + (i + 1), n);
    }

    for (i = 0; i < SIZE_OF_TPOOL1; i++) {
        ogs_pool_alloc(&tpool1, &node[i]);
        org[i] = node[i];
        ABTS_PTR_NOTNULL(tc, node[i]);
        n = ogs_pool_avail(&tpool1);
        ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL1 - (i + 1), n);
    }

    for (i = 0; i < SIZE_OF_TPOOL1; i++) {
        ogs_pool_free(&tpool1, node[i]);
        n = ogs_pool_avail(&tpool1);
        ABTS_INT_EQUAL(tc, i + 1, n);
    }

    n = 0;
    for (i = 0; i < SIZE_OF_TPOOL1; i++) {
        ogs_pool_alloc(&tpool1, &node[i]);
        for (j = 0; j < SIZE_OF_TPOOL1; j++) {
            if (node[i] == org[j])
                n++;
        }
    }

    for (i = 0; i < SIZE_OF_TPOOL1; i++) {
        ogs_pool_free(&tpool1, node[i]);
        n = ogs_pool_avail(&tpool1);
        ABTS_INT_EQUAL(tc, i + 1, n);
    }

    ogs_pool_final(&tpool1);
}

static void test_sub_func2(abts_case *tc, void *data, int start)
{
    int i, j, n;

    type_of_tpool2 *org[SIZE_OF_TPOOL2+1];
    type_of_tpool2 *node[SIZE_OF_TPOOL2+1];

    ogs_pool_init(&tpool2, SIZE_OF_TPOOL2);

    n = ogs_pool_size(&tpool2);
    ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL2, n);
    n = ogs_pool_avail(&tpool2);
    ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL2, n);
    n = ogs_pool_size(&tpool2) - ogs_pool_avail(&tpool2);
    ABTS_INT_EQUAL(tc, 0, n);

    for (i = 0; i < start; i++) {
        ogs_pool_alloc(&tpool2, &node[i]);
        ABTS_PTR_NOTNULL(tc, node[i]);
        n = ogs_pool_avail(&tpool2);
        ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL2 - (i + 1), n);
    }

    for (i = 0; i < start; i++) {
        ogs_pool_free(&tpool2, node[i]);
        n = ogs_pool_avail(&tpool2);
        ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL2 - start + (i + 1), n);
    }

    for (i = 0; i < SIZE_OF_TPOOL2; i++) {
        ogs_pool_alloc(&tpool2, &node[i]);
        org[i] = node[i];
        ABTS_PTR_NOTNULL(tc, node[i]);
        n = ogs_pool_avail(&tpool2);
        ABTS_INT_EQUAL(tc, SIZE_OF_TPOOL2 - (i + 1), n);
    }

    for (i = 0; i < SIZE_OF_TPOOL2; i++) {
        ogs_pool_free(&tpool2, node[i]);
        n = ogs_pool_avail(&tpool2);
        ABTS_INT_EQUAL(tc, i + 1, n);
    }

    n = 0;
    for (i = 0; i < SIZE_OF_TPOOL2; i++) {
        ogs_pool_alloc(&tpool2, &node[i]);
        for (j = 0; j < SIZE_OF_TPOOL2; j++) {
            if (node[i] == org[j])
                n++;
        }
    }

    for (i = 0; i < SIZE_OF_TPOOL2; i++) {
        ogs_pool_free(&tpool2, node[i]);
        n = ogs_pool_avail(&tpool2);
        ABTS_INT_EQUAL(tc, i + 1, n);
    }

    ogs_pool_final(&tpool2);
}

static void test1_func(abts_case *tc, void *data)
{
    int i;

    for (i = 0; i < SIZE_OF_TPOOL1; i++)
        test_sub_func1(tc, data, i);
}

static void test2_func(abts_case *tc, void *data)
{
    int i;

    for (i = 0; i < SIZE_OF_TPOOL2; i++)
        test_sub_func2(tc, data, i);
}

typedef char testnode_t;

static OGS_POOL(testpool, testnode_t);

static void test3_func(abts_case *tc, void *data)
{
    ogs_pool_init(&testpool, 5);
    testnode_t *node[7] = {NULL, };
    int index;

    ogs_pool_alloc(&testpool, &node[0]);
    ABTS_PTR_NOTNULL(tc, &node[0]);
    ogs_pool_alloc(&testpool, &node[1]);
    ABTS_PTR_NOTNULL(tc, &node[1]);
    ogs_pool_alloc(&testpool, &node[2]);
    ABTS_PTR_NOTNULL(tc, &node[2]);
    ogs_pool_alloc(&testpool, &node[3]);
    ABTS_PTR_NOTNULL(tc, &node[3]);
    ogs_pool_alloc(&testpool, &node[4]);
    ABTS_PTR_NOTNULL(tc, &node[4]);

    index = ogs_pool_index(&testpool, node[3]);
    ABTS_PTR_NOTNULL(tc, ogs_pool_find(&testpool, index));
    ogs_pool_free(&testpool, node[3]);
    ABTS_PTR_EQUAL(tc, 0, ogs_pool_find(&testpool, index));
    index = ogs_pool_index(&testpool, node[2]);
    ABTS_PTR_NOTNULL(tc, ogs_pool_find(&testpool, index));
    ogs_pool_free(&testpool, node[2]);
    ABTS_PTR_EQUAL(tc, 0, ogs_pool_find(&testpool, index));

    ogs_pool_alloc(&testpool, &node[5]);
    ABTS_PTR_NOTNULL(tc, &node[5]);
    index = ogs_pool_index(&testpool, node[5]);
    ABTS_PTR_NOTNULL(tc, ogs_pool_find(&testpool, index));
    ogs_pool_alloc(&testpool, &node[6]);
    ABTS_PTR_NOTNULL(tc, &node[6]);
    index = ogs_pool_index(&testpool, node[6]);
    ABTS_PTR_NOTNULL(tc, ogs_pool_find(&testpool, index));

    ABTS_INT_EQUAL(tc, 1, ogs_pool_index(&testpool, node[0]));
    ABTS_INT_EQUAL(tc, 2, ogs_pool_index(&testpool, node[1]));
    ABTS_INT_EQUAL(tc, 3, ogs_pool_index(&testpool, node[2]));
    ABTS_INT_EQUAL(tc, 4, ogs_pool_index(&testpool, node[3]));
    ABTS_INT_EQUAL(tc, 5, ogs_pool_index(&testpool, node[4]));
    ABTS_INT_EQUAL(tc, 4, ogs_pool_index(&testpool, node[5]));
    ABTS_INT_EQUAL(tc, 3, ogs_pool_index(&testpool, node[6]));

    ogs_pool_free(&testpool, node[0]);
    ogs_pool_free(&testpool, node[1]);
    ogs_pool_free(&testpool, node[2]);
    ogs_pool_free(&testpool, node[3]);
    ogs_pool_free(&testpool, node[4]);

    ogs_pool_final(&testpool);
}

abts_suite *test_pool(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test2_func, NULL);
    abts_run_test(suite, test3_func, NULL);

    return suite;
}
