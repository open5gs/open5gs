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
    ogs_list_t list;
    int m1;
} lt_type1;

int lttype1_compare(lt_type1 *pnode1, lt_type1 *pnode2)
{
    if (pnode1->m1 == pnode2->m1)
        return 0;
    else if (pnode1->m1 < pnode2->m1)
        return -1;
    else
        return 1;
}

#define SIZE_OF_lt_type1 16

static OGS_LIST(tlist1);

static void list_test1(abts_case *tc, void *data)
{
    int i;
    lt_type1 *iter, *tmp, node[SIZE_OF_lt_type1];

    for (i = 0; i < SIZE_OF_lt_type1; i++)
        node[i].m1 = i;

    ABTS_TRUE(tc, ogs_list_empty(&tlist1));
    ABTS_INT_EQUAL(tc, 0, ogs_list_count(&tlist1));

    for (i = 0; i < 1; i++)
        ogs_list_add(&tlist1, &node[i]);
    ABTS_INT_EQUAL(tc, 1, ogs_list_count(&tlist1));

    i = 0;
    ogs_list_for_each(&tlist1, iter)
        ABTS_PTR_EQUAL(tc, &node[i++], iter);

    for (i = 1; i < 3; i++)
        ogs_list_add(&tlist1, &node[i]);
    ABTS_INT_EQUAL(tc, 3, ogs_list_count(&tlist1));

    i = 0;
    ogs_list_for_each(&tlist1, iter)
        ABTS_PTR_EQUAL(tc, &node[i++], iter);

    ogs_list_remove(&tlist1, &node[1]);
    ABTS_INT_EQUAL(tc, 2, ogs_list_count(&tlist1));
    ABTS_TRUE(tc, !ogs_list_empty(&tlist1));

    ogs_list_for_each_safe(&tlist1, tmp, iter)
        ogs_list_remove(&tlist1, iter);
    ABTS_INT_EQUAL(tc, 0, ogs_list_count(&tlist1));
    ABTS_TRUE(tc, ogs_list_empty(&tlist1));
}

static void list_test2(abts_case *tc, void *data)
{
    int i;

    lt_type1 *iter, node[SIZE_OF_lt_type1];

    for (i = 0; i < SIZE_OF_lt_type1; i++)
        node[i].m1 = i;

    /* Initialize head of list */
    ogs_list_init(&tlist1);

    /* Check list is empty */
    ABTS_TRUE(tc, ogs_list_empty(&tlist1));

    /* Confirm that any node can't be get */
    iter = ogs_list_first(&tlist1);
    ABTS_PTR_EQUAL(tc, NULL, iter);

    /* Add a node */
    ogs_list_prepend(&tlist1, &node[0]);

    /* Iterate from the first. And check the pointers */
    iter = ogs_list_first(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, NULL, iter);

    /* Iterate from the last. And check the pointers */
    iter = ogs_list_last(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = ogs_list_prev(iter);
    ABTS_PTR_EQUAL(tc, NULL, iter);

    /* Add two nodes */
    ogs_list_prepend(&tlist1, &node[1]);
    ogs_list_prepend(&tlist1, &node[2]);

    /* Iterate from the first. And check the pointers */
    iter = ogs_list_first(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[2], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[1], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, NULL, iter);

    /* Iterate from the last. And check the pointers */
    iter = ogs_list_last(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = ogs_list_prev(iter);
    ABTS_PTR_EQUAL(tc, &node[1], iter);
    iter = ogs_list_prev(iter);
    ABTS_PTR_EQUAL(tc, &node[2], iter);
    iter = ogs_list_prev(iter);
    ABTS_PTR_EQUAL(tc, NULL, iter);

    /* Remove all nodes */
    ogs_list_remove(&tlist1, &node[2]);
    ogs_list_remove(&tlist1, &node[1]);
    ogs_list_remove(&tlist1, &node[0]);

    /* Check list is empty */
    ABTS_TRUE(tc, ogs_list_empty(&tlist1));
}

static void list_test3(abts_case *tc, void *data)
{
    int i;

    lt_type1 *iter, node[SIZE_OF_lt_type1];

    for (i = 0; i < SIZE_OF_lt_type1; i++)
        node[i].m1 = i;

    /* Initialize head of list */
    ogs_list_init(&tlist1);

    /* Add three nodes */
    for (i = 0; i < 3; i++)
        ogs_list_add(&tlist1, &node[i]);

    /* Iterate from the first. And check the pointers */
    i = 0; iter = ogs_list_first(&tlist1);
    while (iter) {
        ABTS_PTR_EQUAL(tc, &node[i++], iter);
        iter = ogs_list_next(iter);
    }
    ABTS_INT_EQUAL(tc, 3, i);

    /* Iterate from the last. And check the pointers */
    i = 0; iter = ogs_list_last(&tlist1);
    while (iter) {
        ABTS_PTR_EQUAL(tc, &node[3 - (++i)], iter);
        iter = ogs_list_prev(iter);
    }
    ABTS_INT_EQUAL(tc, 3, i);

    /* Insert three nodes by ogs_list_insert_prev() */
    ogs_list_insert_prev(&tlist1, &node[0], &node[3]);
    ogs_list_insert_prev(&tlist1, &node[1], &node[4]);
    ogs_list_insert_prev(&tlist1, &node[2], &node[5]);

    /* Iterate from the first. And check the pointers */
    iter = ogs_list_first(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[3], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[4], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[1], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[5], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[2], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, NULL, iter);

    /* Remove three nodes inserted by ogs_list_insert_prev() */
    ogs_list_remove(&tlist1, &node[3]);
    ogs_list_remove(&tlist1, &node[4]);
    ogs_list_remove(&tlist1, &node[5]);

    /* Iterate from the first. And check the pointers */
    iter = ogs_list_first(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[1], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[2], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, NULL, iter);
}

static void list_test4(abts_case *tc, void *data)
{
    int i;

    lt_type1 *iter, node[SIZE_OF_lt_type1];

    for (i = 0; i < SIZE_OF_lt_type1; i++)
        node[i].m1 = i;

    /* Initialize head of list */
    ogs_list_init(&tlist1);

    /* Add three nodes */
    for (i = 0; i < 3; i++)
        ogs_list_add(&tlist1, &node[i]);

    /* Iterate from the first. And check the pointers */
    i = 0; iter = ogs_list_first(&tlist1);
    while (iter) {
        ABTS_PTR_EQUAL(tc, &node[i++], iter);
        iter = ogs_list_next(iter);
    }
    ABTS_INT_EQUAL(tc, 3, i);

    /* Iterate from the last. And check the pointers */
    i = 0; iter = ogs_list_last(&tlist1);
    while (iter) {
        ABTS_PTR_EQUAL(tc, &node[3 - (++i)], iter);
        iter = ogs_list_prev(iter);
    }
    ABTS_INT_EQUAL(tc, 3, i);

    /* Insert three nodes by ogs_list_insert_next() */
    ogs_list_insert_next(&tlist1, &node[0], &node[3]);
    ogs_list_insert_next(&tlist1, &node[1], &node[4]);
    ogs_list_insert_next(&tlist1, &node[2], &node[5]);

    /* Iterate from the first. And check the pointers */
    iter = ogs_list_first(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[3], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[1], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[4], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[2], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[5], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, NULL, iter);

    /* Remove three nodes inserted by ogs_list_insert_next() */
    ogs_list_remove(&tlist1, &node[3]);
    ogs_list_remove(&tlist1, &node[4]);
    ogs_list_remove(&tlist1, &node[5]);

    /* Iterate from the first. And check the pointers */
    iter = ogs_list_first(&tlist1);
    ABTS_PTR_EQUAL(tc, &node[0], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[1], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, &node[2], iter);
    iter = ogs_list_next(iter);
    ABTS_PTR_EQUAL(tc, NULL, iter);
}

static void list_test5(abts_case *tc, void *data)
{
    int i, j;

    /* List up posssible oders with four nodes */
    int od[24][4] = {
        {0,1,2,3}, {0,1,3,2}, {0,2,1,3}, {0,2,3,1}, {0,3,1,2}, {0,3,2,1},
        {1,0,2,3}, {1,0,3,2}, {1,2,0,3}, {1,2,3,0}, {1,3,0,2}, {1,3,2,0},
        {2,1,0,3}, {2,1,3,0}, {2,0,1,3}, {2,0,3,1}, {2,3,1,0}, {2,3,0,1},
        {3,1,2,0}, {3,1,0,2}, {3,2,1,0}, {3,2,0,1}, {3,0,1,2}, {3,0,2,1}
    };

    lt_type1 *iter, node[SIZE_OF_lt_type1];

    for (i = 0; i < SIZE_OF_lt_type1; i++)
        node[i].m1 = i;

    for (i = 0; i < 24; i++) {
        /* Initialize head of list */
        ogs_list_init(&tlist1);

        /* Add four nodes using predefined orders */
        for (j = 0; j < 4; j++)
            ogs_list_insert_sorted(&tlist1, &node[od[i][j]], &lttype1_compare);

        /* Iterate from the first. And check the SORTED */
        j = 0; iter = ogs_list_first(&tlist1);
        while (iter) {
            ABTS_INT_EQUAL(tc, iter->m1, j++);
            iter = ogs_list_next(iter);
        }
    }
}

abts_suite *test_list(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, list_test1, NULL);
    abts_run_test(suite, list_test2, NULL);
    abts_run_test(suite, list_test3, NULL);
    abts_run_test(suite, list_test4, NULL);
    abts_run_test(suite, list_test5, NULL);

    return suite;
}
