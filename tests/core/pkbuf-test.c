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
    ogs_pkbuf_t *pkbuf = NULL;
    unsigned char *tmp = NULL;
         
    pkbuf = ogs_pkbuf_alloc(NULL, 100);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    ogs_pkbuf_reserve(pkbuf, 50);
    tmp = ogs_pkbuf_push(pkbuf, 50);
    ABTS_PTR_NOTNULL(tc, tmp);
    tmp = ogs_pkbuf_pull(pkbuf, 50);
    ABTS_PTR_NOTNULL(tc, tmp);
    tmp = ogs_pkbuf_put(pkbuf, 50);
    ABTS_PTR_NOTNULL(tc, tmp);

    ogs_pkbuf_free(pkbuf);
}

static void test2_func(abts_case *tc, void *data)
{
    ogs_pkbuf_t *pkbuf = NULL, *p2 = NULL, *p3 = NULL;
    unsigned char *tmp = NULL;
         
    pkbuf = ogs_pkbuf_alloc(NULL, 100);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    ogs_pkbuf_reserve(pkbuf, 50);
    tmp = ogs_pkbuf_push(pkbuf, 50);
    ABTS_PTR_NOTNULL(tc, tmp);
    p2 = ogs_pkbuf_copy(pkbuf);
    ABTS_PTR_NOTNULL(tc, p2);
    ABTS_INT_EQUAL(tc, 50, p2->len);
    ABTS_TRUE(tc, p2->head == p2->data);
    ABTS_INT_EQUAL(tc, 50, (p2->tail-p2->data));
    ABTS_INT_EQUAL(tc, 50, (p2->end-p2->tail));
    tmp = ogs_pkbuf_pull(pkbuf, 50);
    ABTS_PTR_NOTNULL(tc, tmp);
    p3 = ogs_pkbuf_copy(pkbuf);
    ABTS_PTR_NOTNULL(tc, p3);
    ABTS_INT_EQUAL(tc, 0, p3->len);
    ABTS_TRUE(tc, p3->tail == p3->data);
    ABTS_INT_EQUAL(tc, 50, (p3->tail-p3->head));
    ABTS_INT_EQUAL(tc, 50, (p3->end-p3->tail));
    tmp = ogs_pkbuf_put(pkbuf, 50);
    ABTS_PTR_NOTNULL(tc, tmp);

    ogs_pkbuf_free(pkbuf);
    ogs_pkbuf_free(p2);
    ogs_pkbuf_free(p3);
}

abts_suite *test_pkbuf(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test2_func, NULL);

    return suite;
}
