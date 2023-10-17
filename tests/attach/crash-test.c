/*
 * Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>
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

static void test1_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;

    s1ap = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap);

    sendbuf = test_s1ap_build_malformed_s1_setup_request(0);
    ABTS_PTR_NOTNULL(tc, sendbuf);

    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    testenb_s1ap_close(s1ap);
}

static void test2_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf = NULL;
    ogs_s1ap_message_t message;
    int i;

    s1ap = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap);

    sendbuf = test_s1ap_build_s1_setup_request(
            S1AP_ENB_ID_PR_macroENB_ID, 0x54f64);
    ABTS_PTR_NOTNULL(tc, sendbuf);

    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    sendbuf = test_s1ap_build_malformed_enb_status_transfer(0);
    ABTS_PTR_NOTNULL(tc, sendbuf);

    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    testenb_s1ap_close(s1ap);
}

#if 0
static void test3_func(abts_case *tc, void *data)
{
    int rv;
    ogs_socknode_t *s1ap;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;

    s1ap = tests1ap_client(AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap);

    sendbuf = test_s1ap_build_oversized_message(0);
    ABTS_PTR_NOTNULL(tc, sendbuf);

    rv = testenb_s1ap_send(s1ap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    recvbuf = testenb_s1ap_read(s1ap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ogs_pkbuf_free(recvbuf);

    testenb_s1ap_close(s1ap);
}
#endif

abts_suite *test_crash(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test2_func, NULL);
#if 0
    abts_run_test(suite, test3_func, NULL);
#endif

    return suite;
}
