/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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
#include "gtp-path.h"

static test_ue_t test_ue = {
    .imsi = "999707364000060",
    .k_string = "465b5ce8b199b49faa5f0a2ee238a6bc",
    .opc_string = "e8ed289deba952e4283b54e88e6183ca",
};

void test_subscriber_init(void) {
    ogs_bcd_to_buffer(test_ue.imsi, test_ue.imsi_buf, &test_ue.imsi_len);

    bson_t *doc = test_db_new_simple(&test_ue);
    ogs_assert(doc != NULL);
    ogs_assert(OGS_OK == test_db_insert_ue(&test_ue, doc));
    // Note: "doc" is destroyed by test_db_insert_ue
}

void test_subscriber_cleanup(void) {
    ogs_assert(OGS_OK == test_db_remove_ue(&test_ue));
}

static void test_4g_proxy_func(abts_case *tc, void *data)
{
    int rv = OGS_ERROR;
    ogs_socknode_t *sgw_c;
    ogs_socknode_t *sgw_u;
    ogs_gtp_node_t *gnode;
    ogs_pkbuf_t *recvbuf;

    test_sess_t *sess;
    test_bearer_t *bearer;

    ogs_sockaddr_t from;
    char buf[OGS_ADDRSTRLEN];

    test_subscriber_init();

    /* SGW GTPv2-C */
    sgw_c = test_sgw_server(OGS_GTPV2_C_UDP_PORT);
    ABTS_PTR_NOTNULL(tc, sgw_c);

    gnode = test_gtpc_proxy_node_new();
    ogs_assert(gnode);

    ABTS_PTR_NOTNULL(tc, sgw_c->sock);
    rv = ogs_gtp_connect(sgw_c->sock, NULL, gnode);
    ogs_assert(rv == OGS_OK);

    /* SGW GTP-U */
    sgw_u = test_sgw_server(OGS_GTPV1_U_UDP_PORT);
    ABTS_PTR_NOTNULL(tc, sgw_u);

    sess = test_sess_add_by_apn(&test_ue, "internet", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);

    OGS_SETUP_GTP_NODE(sess, gnode);

    /* Giving components some time to establish PFCP associations */
    ogs_msleep(2500);

    /* Send S8 Create Session Request */
    test_s8_send_create_session_request(sess, 0);

    /* Receive S8 Create Session Response */
    recvbuf = test_sgw_read(sgw_c, &from);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ABTS_STR_EQUAL(tc, "127.100.0.3", OGS_ADDR(&from, buf));
    test_s8_recv(sess, recvbuf);

    /* Send GTP-U ICMP Packet */
    bearer = test_bearer_find_by_sess_ebi(sess, 5);
    ogs_assert(bearer);

    rv = test_gtpu_send_ping(sgw_u, bearer, TEST_PING_IPV4);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive GTP-U ICMP Packet */
    recvbuf = test_sgw_read(sgw_u, &from);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ABTS_STR_EQUAL(tc, "127.100.0.3", OGS_ADDR(&from, buf));
    ogs_pkbuf_free(recvbuf);

    /* Send S8 Delete Session Request */
    rv = test_s8_send_delete_session_request(sess);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive S8 Delete Session Response */
    recvbuf = test_sgw_read(sgw_c, &from);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    ABTS_STR_EQUAL(tc, "127.100.0.3", OGS_ADDR(&from, buf));
    test_s8_recv(sess, recvbuf);

    /* SGW diconnect */
    test_sgw_close(sgw_c);
    test_sgw_close(sgw_u);

    test_sess_remove_all(&test_ue);
    ogs_gtp_node_free(gnode);

    test_subscriber_cleanup();
}

abts_suite *test_4g_proxy(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test_4g_proxy_func, NULL);

    return suite;
}
