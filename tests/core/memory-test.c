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

static void test1_func(abts_case *tc, void *data)
{
    char *ptr = ogs_malloc(256);
    ABTS_PTR_NOTNULL(tc, ptr);
    ogs_free(ptr);
}

static void test2_func(abts_case *tc, void *data)
{
    char *ptr = ogs_calloc(2, 10);
    int i;
    for (i = 0; i < 2*10; i++)
    {
        ABTS_INT_EQUAL(tc, 0, ptr[i]);
    }
    ABTS_PTR_NOTNULL(tc, ptr);
    ogs_free(ptr);
}

static void test3_func(abts_case *tc, void *data)
{
    char *ptr = ogs_realloc(0, 10);
    ABTS_PTR_NOTNULL(tc, ptr);
    ogs_free(ptr);

    ptr = ogs_malloc(20);
    ABTS_PTR_NOTNULL(tc, ptr);
    ptr = ogs_realloc(ptr, 0);
}

static void test4_func(abts_case *tc, void *data)
{
    char *p, *q;

    p = ogs_malloc(10);
    ABTS_PTR_NOTNULL(tc, p);
    memset(p, 1, 10);

    q = ogs_realloc(p, 128 - sizeof(ogs_pkbuf_t *) - 1);
    ABTS_TRUE(tc, p == q);

    p = ogs_realloc(q, 128 - sizeof(ogs_pkbuf_t *));
    ABTS_TRUE(tc, p == q);

    q = ogs_realloc(p, 128 - sizeof(ogs_pkbuf_t *) + 1);
    ABTS_TRUE(tc, p != q);
    ABTS_TRUE(tc, memcmp(p, q, 10) == 0);
    ogs_free(q);
}

abts_suite *test_memory(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test2_func, NULL);
    abts_run_test(suite, test3_func, NULL);
    abts_run_test(suite, test4_func, NULL);

    return suite;
}
