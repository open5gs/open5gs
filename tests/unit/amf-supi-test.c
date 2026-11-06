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

#include "amf/context.h"
#include "core/abts.h"

#define SUPI_1 "imsi-999700000000001"
#define SUPI_2 "imsi-999700000000002"

/*
 * Exercise the production SUPI setter without UE pools or NF startup.
 * Two contexts model a UE whose SUPI is re-assigned while another
 * context still holds it, and that context then frees its SUPI.
 */
static void reassignment_func(abts_case *tc, void *data)
{
    ogs_hash_t *saved_hash = amf_self()->supi_hash;
    ogs_hash_index_t *hi;
    amf_ue_t *ue = ogs_calloc(2, sizeof(*ue));

    ogs_assert(ue);
    amf_self()->supi_hash = ogs_hash_make();
    ogs_assert(amf_self()->supi_hash);

    amf_ue_set_supi(&ue[0], (char *)SUPI_1);
    amf_ue_set_supi(&ue[1], (char *)SUPI_2);
    ABTS_INT_EQUAL(tc, 2, ogs_hash_count(amf_self()->supi_hash));

    /* Re-assignment moves ue[1] to the SUPI ue[0] still holds. */
    amf_ue_set_supi(&ue[1], (char *)SUPI_1);
    ABTS_PTR_EQUAL(tc, &ue[1], amf_ue_find_by_supi((char *)SUPI_1));
    ABTS_PTR_EQUAL(tc, NULL, amf_ue_find_by_supi((char *)SUPI_2));
    ABTS_INT_EQUAL(tc, 1, ogs_hash_count(amf_self()->supi_hash));

    /* The index holds the SUPI itself: its contents and length. */
    hi = ogs_hash_first(amf_self()->supi_hash);
    ABTS_PTR_NOTNULL(tc, hi);
    if (hi) {
        ABTS_STR_EQUAL(tc, SUPI_1, ogs_hash_this_key(hi));
        ABTS_INT_EQUAL(tc, strlen(SUPI_1), ogs_hash_this_key_len(hi));
    }

    /* ue[0] changes and frees its string; the index must not depend on it. */
    ue[0].supi[0] = 'X';
    ABTS_PTR_EQUAL(tc, &ue[1], amf_ue_find_by_supi((char *)SUPI_1));
    ogs_free(ue[0].supi);
    ue[0].supi = NULL;
    ABTS_PTR_EQUAL(tc, &ue[1], amf_ue_find_by_supi((char *)SUPI_1));
    hi = ogs_hash_first(amf_self()->supi_hash);
    ABTS_PTR_NOTNULL(tc, hi);
    if (hi) {
        ABTS_STR_EQUAL(tc, SUPI_1, ogs_hash_this_key(hi));
        ABTS_INT_EQUAL(tc, strlen(SUPI_1), ogs_hash_this_key_len(hi));
    }

    ogs_free(ue[1].supi);
    ogs_hash_destroy(amf_self()->supi_hash);
    amf_self()->supi_hash = saved_hash;
    ogs_free(ue);
}

abts_suite *test_amf_supi(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, reassignment_func, NULL);

    return suite;
}
