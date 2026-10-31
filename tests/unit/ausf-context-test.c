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
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ausf/context.h"
#include "ausf/nudm-handler.h"
#include "core/abts.h"

/* Run the UDM answer handler without an SBI server. */
#define ogs_sbi_server_from_stream(stream) ((ogs_sbi_server_t *)(stream))
#define ogs_sbi_server_uri(server, header) ogs_strdup("http://ausf")
#define ogs_sbi_build_response(msg, status) ((ogs_sbi_response_t *)(msg))
#define ogs_sbi_server_send_response(stream, response) true
#include "ausf/nudm-handler.c"

#define TEST_SUPI "imsi-001010000000001"

/* How many of the index's stored keys read key, with its length. */
static int count_key(ogs_hash_t *hash, const char *key)
{
    ogs_hash_index_t *hi;
    int n = 0;

    for (hi = ogs_hash_first(hash); hi; hi = ogs_hash_next(hi))
        if (ogs_hash_this_key_len(hi) == (int)strlen(key) &&
            !strcmp(ogs_hash_this_key(hi), key))
            n++;

    return n;
}

static void udm_answer(abts_case *tc, ausf_ue_t *ausf_ue, const char *supi)
{
    char stream;
    ogs_sbi_message_t msg;
    OpenAPI_authentication_info_result_t result;
    OpenAPI_authentication_vector_t av;

    memset(&av, 0, sizeof(av));
    av.av_type = OpenAPI_av_type_5G_HE_AKA;
    av.rand = (char *)"00112233445566778899aabbccddeeff";
    av.xres_star = (char *)"00112233445566778899aabbccddeeff";
    av.autn = (char *)"00112233445566778899aabbccddeeff";
    av.kausf = (char *)
        "00112233445566778899aabbccddeeff00112233445566778899aabbccddeeff";

    memset(&result, 0, sizeof(result));
    result.auth_type = OpenAPI_auth_type_5G_AKA;
    result.authentication_vector = &av;
    result.supi = (char *)supi;

    memset(&msg, 0, sizeof(msg));
    msg.AuthenticationInfoResult = &result;

    ABTS_TRUE(tc, ausf_nudm_ueau_handle_get(
                ausf_ue, (ogs_sbi_stream_t *)&stream, &msg));
}

/*
 * A UE that registers again with a new SUCI gets a second AUSF context
 * for the same SUPI. Removing the first one must leave the SUPI index
 * pointing at the second, with a key the index can still read.
 */
static void test_supi_reregistration(abts_case *tc, void *data)
{
    ausf_ue_t *a, *b;
    ogs_pool_id_t b_id;

    a = ausf_ue_add((char *)"suci-0-001-01-0000-1-1-aaaa");
    ABTS_PTR_NOTNULL(tc, a);
    udm_answer(tc, a, TEST_SUPI);

    b = ausf_ue_add((char *)"suci-0-001-01-0000-1-1-bbbb");
    ABTS_PTR_NOTNULL(tc, b);
    b_id = b->id;
    udm_answer(tc, b, TEST_SUPI);

    /* The index holds the SUPI itself: its contents and length. */
    ABTS_INT_EQUAL(tc, 1, count_key(ausf_self()->supi_hash, TEST_SUPI));

    /* a's strings are freed with it; b must still be found by its SUPI. */
    ausf_ue_remove(a);

    b = ausf_ue_find_by_id(b_id);
    ABTS_PTR_EQUAL(tc, b, ausf_ue_find_by_supi((char *)TEST_SUPI));
    ABTS_INT_EQUAL(tc, 1, count_key(ausf_self()->supi_hash, TEST_SUPI));

    ausf_ue_remove(b);
    ABTS_PTR_EQUAL(tc, NULL, ausf_ue_find_by_supi((char *)TEST_SUPI));
}

/*
 * Two requests that name the UE by SUPI before the UDM has answered
 * create two contexts with the same identity in the SUCI index.
 */
static void test_same_identity_contexts(abts_case *tc, void *data)
{
    ausf_ue_t *a, *b;
    ogs_pool_id_t b_id;

    a = ausf_ue_add((char *)TEST_SUPI);
    ABTS_PTR_NOTNULL(tc, a);
    b = ausf_ue_add((char *)TEST_SUPI);
    ABTS_PTR_NOTNULL(tc, b);
    b_id = b->id;

    /* The index holds the SUCI itself: its contents and length. */
    ABTS_INT_EQUAL(tc, 1, count_key(ausf_self()->suci_hash, TEST_SUPI));

    /* a's strings are freed with it; b must still be found by its SUCI. */
    ausf_ue_remove(a);

    b = ausf_ue_find_by_id(b_id);
    ABTS_PTR_EQUAL(tc, b, ausf_ue_find_by_suci((char *)TEST_SUPI));
    ABTS_INT_EQUAL(tc, 1, count_key(ausf_self()->suci_hash, TEST_SUPI));

    ausf_ue_remove(b);
    ABTS_PTR_EQUAL(tc, NULL, ausf_ue_find_by_suci((char *)TEST_SUPI));
}

abts_suite *test_ausf_context(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    ogs_app_context_init();
    ogs_global_conf()->max.ue = 8;
    ausf_context_init();

    abts_run_test(suite, test_supi_reregistration, NULL);
    abts_run_test(suite, test_same_identity_contexts, NULL);

    ausf_context_final();
    ogs_app_context_final();

    return suite;
}
