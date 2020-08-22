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

#include "test-common.h"

#define NUM_OF_TEST_DUPLICATED_ENB 4

static void s1setup_test1(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *node[NUM_OF_TEST_DUPLICATED_ENB];
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf = NULL;
    ogs_s1ap_message_t message;
    int i;

    for (i = 0; i < NUM_OF_TEST_DUPLICATED_ENB; i++) {
        node[i] = tests1ap_client(AF_INET);
        ABTS_PTR_NOTNULL(tc, node[i]);
    }

    for (i = 0; i < NUM_OF_TEST_DUPLICATED_ENB; i++) {
        sendbuf = test_s1ap_build_s1_setup_request(
                S1AP_ENB_ID_PR_macroENB_ID, 0x54f64);
        ABTS_PTR_NOTNULL(tc, sendbuf);

        rv = testenb_s1ap_send(node[i], sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        recvbuf = testenb_s1ap_read(node[i]);
        ABTS_PTR_NOTNULL(tc, recvbuf);

        rv = ogs_s1ap_decode(&message, recvbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        ogs_s1ap_free(&message);
        ogs_pkbuf_free(recvbuf);
    }

    for (i = 0; i < NUM_OF_TEST_DUPLICATED_ENB; i++) {
        testenb_s1ap_close(node[i]);
    }

    ogs_pkbuf_free(recvbuf);
}

#define NUM_OF_TEST_ENB 4

static void s1setup_test2(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *node[NUM_OF_TEST_ENB];
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    ogs_s1ap_message_t message;
    int i;

    for (i = 0; i < NUM_OF_TEST_ENB; i++) {
        node[i] = tests1ap_client(AF_INET);
        ABTS_PTR_NOTNULL(tc, node[i]);
    }

    for (i = 0; i < NUM_OF_TEST_ENB; i++) {
        sendbuf = test_s1ap_build_s1_setup_request(
                S1AP_ENB_ID_PR_macroENB_ID, 0x54f64+i);
        ABTS_PTR_NOTNULL(tc, sendbuf);

        rv = testenb_s1ap_send(node[i], sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        recvbuf = testenb_s1ap_read(node[i]);
        ABTS_PTR_NOTNULL(tc, recvbuf);

        rv = ogs_s1ap_decode(&message, recvbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        ogs_s1ap_free(&message);
        ogs_pkbuf_free(recvbuf);
    }

    for (i = 0; i < NUM_OF_TEST_ENB; i++) {
        testenb_s1ap_close(node[i]);
    }

    ogs_pkbuf_free(recvbuf);
}

static void s1setup_test3(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;

    s1ap = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap);

    sendbuf = test_s1ap_build_invalid_packet(0);
    ABTS_PTR_NOTNULL(tc, sendbuf);

    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    testenb_s1ap_close(s1ap);
}

abts_suite *test_s1setup(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, s1setup_test1, NULL);
    abts_run_test(suite, s1setup_test2, NULL);
    abts_run_test(suite, s1setup_test3, NULL);

    return suite;
}
