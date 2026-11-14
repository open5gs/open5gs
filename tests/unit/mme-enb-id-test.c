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

    /* The hash indexes its own copy of the ID, not either context's. */
    hi = ogs_hash_first(mme_self()->enb_id_hash);
    ABTS_PTR_NOTNULL(tc, hi);
    if (hi) {
        ABTS_INT_EQUAL(tc, sizeof(id), ogs_hash_this_key_len(hi));
        ABTS_TRUE(tc, memcmp(ogs_hash_this_key(hi), &id, sizeof(id)) == 0);
    }

    /* Reuse of either context's storage must not break the lookup. */
    enb[0].enb_id = id ^ 1;
    enb[1].enb_id = id ^ 2;
    ABTS_PTR_EQUAL(tc, &enb[1], mme_enb_find_by_enb_id(id));
    hi = ogs_hash_first(mme_self()->enb_id_hash);
    ABTS_PTR_NOTNULL(tc, hi);
    if (hi) {
        ABTS_INT_EQUAL(tc, sizeof(id), ogs_hash_this_key_len(hi));
        ABTS_TRUE(tc, memcmp(ogs_hash_this_key(hi), &id, sizeof(id)) == 0);
    }

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

static void global_identity_func(abts_case *tc, void *data)
{
    static const S1AP_ENB_ID_PR types[] = {
        S1AP_ENB_ID_PR_macroENB_ID, S1AP_ENB_ID_PR_homeENB_ID,
        S1AP_ENB_ID_PR_short_macroENB_ID, S1AP_ENB_ID_PR_long_macroENB_ID
    };
    ogs_list_t saved;
    mme_enb_t *enb = ogs_calloc(6, sizeof(*enb));
    ogs_plmn_id_t first, second;
    S1AP_Global_ENB_ID_t global = {0};
    S1AP_ENB_ID_t id = {0};
    size_t i;
    ogs_assert(enb);
    ogs_list_copy(&saved, &mme_self()->enb_list);
    ogs_list_init(&mme_self()->enb_list);
    ogs_plmn_id_build(&first, 310, 260, 3);
    ogs_plmn_id_build(&second, 310, 26, 2);
    for (i = 0; i < 6; i++) {
        enb[i].enb_id_presence = true;
        enb[i].state.s1_setup_success = true;
        enb[i].enb_id = 0x1234;
        enb[i].enb_type = i < 4 ? types[i] : types[0];
        enb[i].plmn_id = i == 4 ? second : first;
        ogs_list_add(&mme_self()->enb_list, &enb[i]);
    }
    /* A reconnect takes over only its own global identity. */
    ABTS_PTR_EQUAL(tc, &enb[5],
            mme_enb_find_by_global_id(&first, types[0], 0x1234));
    ABTS_PTR_EQUAL(tc, &enb[4],
            mme_enb_find_by_global_id(&second, types[0], 0x1234));
    for (i = 1; i < 4; i++)
        ABTS_PTR_EQUAL(tc, &enb[i],
                mme_enb_find_by_global_id(&first, types[i], 0x1234));
    enb[5].state.s1_setup_success = false;
    ABTS_PTR_EQUAL(tc, &enb[0],
            mme_enb_find_by_global_id(&first, types[0], 0x1234));
    ABTS_PTR_EQUAL(tc, NULL, mme_enb_find_by_global_id(&first, types[0], 0x4321));
    ABTS_PTR_EQUAL(tc, NULL, mme_enb_find_by_global_id(&second, types[1], 0x1234));
    ogs_s1ap_uint32_to_ENB_ID(types[2], 0x1234, &id);
    global.eNB_ID = &id;
    global.pLMNidentity.buf = (uint8_t *)&first;
    global.pLMNidentity.size = sizeof(first);
    ABTS_PTR_EQUAL(tc, &enb[2], mme_enb_find_by_s1ap_global_id(&global));
    global.pLMNidentity.size--;
    ABTS_PTR_EQUAL(tc, NULL, mme_enb_find_by_s1ap_global_id(&global));
    global.pLMNidentity.size++;
    global.pLMNidentity.buf = NULL;
    ABTS_PTR_EQUAL(tc, NULL, mme_enb_find_by_s1ap_global_id(&global));
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1AP_ENB_ID, &id);
    ogs_list_copy(&mme_self()->enb_list, &saved);
    ogs_free(enb);
}

abts_suite *test_mme_enb_id(abts_suite *suite)
{
    uint32_t id[] = { 0, 0x4001 };
    size_t i;

    suite = ADD_SUITE(suite)

    abts_run_test(suite, global_identity_func, NULL);

    for (i = 0; i < sizeof(id) / sizeof(id[0]); i++) {
        abts_run_test(suite, replacement_func, &id[i]);
        abts_run_test(suite, reassignment_func, &id[i]);
    }

    return suite;
}
