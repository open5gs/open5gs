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

#define SIZE_OF_lt_type1 16

static OGS_LIST(tlist1);

static void test_basic(abts_case *tc, void *data)
{
    int i;
    lt_type1 *iter, *tmp, node[SIZE_OF_lt_type1];

    for (i = 0; i < SIZE_OF_lt_type1; i++)
        node[i].m1 = i;

    ogs_list_init(&tlist1);
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

abts_suite *test_list(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_basic, NULL);

    return suite;
}
