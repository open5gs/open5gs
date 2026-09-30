/*
 * Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
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

#include "mme/mme-context.h"
#include "core/abts.h"

/*
 * Exercise the production ID setter without sockets, UE pools or NF startup.
 * Two contexts model old and new associations with overlapping lifetimes.
 */
static void replacement_func(abts_case *tc, void *data)
{
    uint32_t id = *(uint32_t *)data;
    ogs_hash_t *saved_hash = mme_self()->enb_id_hash;
    ogs_hash_index_t *hi;
    mme_enb_t *enb = ogs_calloc(2, sizeof(*enb));

    ogs_assert(enb);
    mme_self()->enb_id_hash = ogs_hash_make();
    ogs_assert(mme_self()->enb_id_hash);

    ABTS_INT_EQUAL(tc, OGS_OK, mme_enb_set_enb_id(&enb[0], id));
    ABTS_INT_EQUAL(tc, OGS_OK, mme_enb_set_enb_id(&enb[1], id));
    ABTS_INT_EQUAL(tc, 1, ogs_hash_count(mme_self()->enb_id_hash));
    ABTS_PTR_EQUAL(tc, &enb[1], mme_enb_find_by_enb_id(id));

    /* Both the key pointer and the value must belong to the replacement. */
    hi = ogs_hash_first(mme_self()->enb_id_hash);
    ABTS_PTR_NOTNULL(tc, hi);
    if (hi)
        ABTS_PTR_EQUAL(tc, &enb[1].enb_id, ogs_hash_this_key(hi));

    /*
     * Model reuse of the old context's pool storage without dereferencing
     * freed memory. Checking only the value above would miss a stale key.
     */
    enb[0].enb_id = id ^ 1;
    ABTS_PTR_EQUAL(tc, &enb[1], mme_enb_find_by_enb_id(id));

    ogs_hash_destroy(mme_self()->enb_id_hash);
    mme_self()->enb_id_hash = saved_hash;
    ogs_free(enb);
}

static void reassignment_func(abts_case *tc, void *data)
{
    uint32_t id = *(uint32_t *)data;
    ogs_hash_t *saved_hash = mme_self()->enb_id_hash;
    mme_enb_t *enb = ogs_calloc(2, sizeof(*enb));

    ogs_assert(enb);
    mme_self()->enb_id_hash = ogs_hash_make();
    ogs_assert(mme_self()->enb_id_hash);

    ABTS_INT_EQUAL(tc, OGS_OK, mme_enb_set_enb_id(&enb[0], id));
    ABTS_INT_EQUAL(tc, OGS_OK, mme_enb_set_enb_id(&enb[1], id));

    /*
     * The displaced context changes its ID after the replacement is live.
     * The owner helper deliberately skips the old mapping (and logs it);
     * neither that mapping nor the new assignment may be lost.
     */
    ABTS_INT_EQUAL(tc, OGS_OK, mme_enb_set_enb_id(&enb[0], id ^ 1));
    ABTS_PTR_EQUAL(tc, &enb[1], mme_enb_find_by_enb_id(id));
    ABTS_PTR_EQUAL(tc, &enb[0], mme_enb_find_by_enb_id(id ^ 1));
    ABTS_INT_EQUAL(tc, 2, ogs_hash_count(mme_self()->enb_id_hash));

    /* Repeated setup by the current owner remains valid. */
    ABTS_INT_EQUAL(tc, OGS_OK, mme_enb_set_enb_id(&enb[1], id));
    ABTS_PTR_EQUAL(tc, &enb[1], mme_enb_find_by_enb_id(id));
    ABTS_INT_EQUAL(tc, 2, ogs_hash_count(mme_self()->enb_id_hash));

    /* Changing the current owner's ID must still remove its previous ID. */
    ABTS_INT_EQUAL(tc, OGS_OK, mme_enb_set_enb_id(&enb[1], id ^ 2));
    ABTS_PTR_EQUAL(tc, NULL, mme_enb_find_by_enb_id(id));
    ABTS_PTR_EQUAL(tc, &enb[1], mme_enb_find_by_enb_id(id ^ 2));
    ABTS_PTR_EQUAL(tc, &enb[0], mme_enb_find_by_enb_id(id ^ 1));
    ABTS_INT_EQUAL(tc, 2, ogs_hash_count(mme_self()->enb_id_hash));

    ogs_hash_destroy(mme_self()->enb_id_hash);
    mme_self()->enb_id_hash = saved_hash;
    ogs_free(enb);
}

abts_suite *test_mme_enb_id(abts_suite *suite)
{
    uint32_t id[] = { 0, 0x4001 };
    size_t i;

    suite = ADD_SUITE(suite)

    for (i = 0; i < sizeof(id) / sizeof(id[0]); i++) {
        abts_run_test(suite, replacement_func, &id[i]);
        abts_run_test(suite, reassignment_func, &id[i]);
    }

    return suite;
}
