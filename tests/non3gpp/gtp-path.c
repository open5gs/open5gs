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
#include "s2b-build.h"
#include "s2b-handler.h"

ogs_socknode_t *test_epdg_server(uint16_t port)
{
    int rv;
    ogs_sockaddr_t *addr = NULL;
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;

    ogs_assert(port);

#define TEST_EPDG_IPV4          "127.0.0.5"

    rv = ogs_getaddrinfo(&addr, AF_UNSPEC, TEST_EPDG_IPV4, port, 0);
    ogs_assert(rv == OGS_OK);

    node = ogs_socknode_new(addr);
    ogs_assert(node);

    sock = ogs_udp_server(node->addr, NULL);
    ogs_assert(sock);

    node->sock = sock;

    return node;
}

void test_epdg_close(ogs_socknode_t *node)
{
    ogs_socknode_free(node);
}

ogs_pkbuf_t *test_epdg_read(ogs_socknode_t *node)
{
    int rc = 0;
    ogs_sockaddr_t from;
    ogs_pkbuf_t *recvbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(recvbuf);
    ogs_pkbuf_put(recvbuf, OGS_MAX_SDU_LEN);

    ogs_assert(node);
    ogs_assert(node->sock);

    while (1) {
        rc = ogs_recvfrom(
                node->sock->fd, recvbuf->data, recvbuf->len, 0, &from);
        if (rc <= 0) {
            ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                    "ogs_recvfrom() failed");
        }
        break;
    }
    recvbuf->len = rc;

    return recvbuf;
}

void test_s2b_recv(test_sess_t *sess, ogs_pkbuf_t *pkbuf)
{
    int rv;
    ogs_gtp2_message_t gtp_message;
    ogs_gtp_xact_t *xact = NULL;

    ogs_assert(sess);
    ogs_assert(sess->gnode);
    ogs_assert(pkbuf);

    if (ogs_gtp2_parse_msg(&gtp_message, pkbuf) != OGS_OK) {
        ogs_error("ogs_gtp2_parse_msg() failed");
        ogs_pkbuf_free(pkbuf);
        return;
    }

    rv = ogs_gtp_xact_receive(sess->gnode, &gtp_message.h, &xact);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_receive() failed");
        ogs_pkbuf_free(pkbuf);
        return;
    }

    switch (gtp_message.h.type) {
        case OGS_GTP2_CREATE_SESSION_RESPONSE_TYPE:
            test_s2b_handle_create_session_response(
                xact, sess, &gtp_message.create_session_response);
            break;
        case OGS_GTP2_DELETE_SESSION_RESPONSE_TYPE:
            test_s2b_handle_delete_session_response(
                xact, sess, &gtp_message.delete_session_response);
            break;
        case OGS_GTP2_CREATE_BEARER_REQUEST_TYPE:
            test_s2b_handle_create_bearer_request(
                xact, sess, &gtp_message.create_bearer_request);
            break;
        case OGS_GTP2_DELETE_BEARER_REQUEST_TYPE:
            test_s2b_handle_delete_bearer_request(
                xact, sess, &gtp_message.delete_bearer_request);
            break;
        default:
            ogs_error("Not implemented(type:%d)", gtp_message.h.type);
            break;
    }

    ogs_pkbuf_free(pkbuf);
}

int test_s2b_send_create_session_request(test_sess_t *sess, bool handover_ind)
{
    int rv;
    ogs_gtp2_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_xact_t *xact = NULL;

    ogs_assert(sess);

    memset(&h, 0, sizeof(ogs_gtp2_header_t));
    h.type = OGS_GTP2_CREATE_SESSION_REQUEST_TYPE;
    h.teid = sess->smf_s2b_c_teid;

    pkbuf = test_s2b_build_create_session_request(h.type, sess, handover_ind);
    if (!pkbuf) {
        ogs_error("test_s2b_build_create_session_request() failed");
        return OGS_ERROR;
    }

    xact = ogs_gtp_xact_local_create(sess->gnode, &h, pkbuf, NULL, sess);
    if (!xact) {
        ogs_error("ogs_gtp_xact_local_create() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int test_s2b_send_delete_session_request(test_sess_t *sess)
{
    int rv;
    ogs_gtp2_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_gtp_xact_t *xact = NULL;

    ogs_assert(sess);

    memset(&h, 0, sizeof(ogs_gtp2_header_t));
    h.type = OGS_GTP2_DELETE_SESSION_REQUEST_TYPE;
    h.teid = sess->smf_s2b_c_teid;

    pkbuf = test_s2b_build_delete_session_request(h.type, sess);
    if (!pkbuf) {
        ogs_error("test_s2b_build_delete_session_request() failed");
        return OGS_ERROR;
    }

    xact = ogs_gtp_xact_local_create(sess->gnode, &h, pkbuf, NULL, sess);
    if (!xact) {
        ogs_error("ogs_gtp_xact_local_create() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int test_s2b_send_create_bearer_response(
        test_bearer_t *bearer, ogs_gtp_xact_t *xact)
{
    int rv;
    ogs_gtp2_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;
    test_sess_t *sess = NULL;

    ogs_assert(xact);
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    memset(&h, 0, sizeof(ogs_gtp2_header_t));
    h.type = OGS_GTP2_CREATE_BEARER_RESPONSE_TYPE;
    h.teid = sess->smf_s2b_c_teid;

    pkbuf = test_s2b_build_create_bearer_response(h.type, bearer);
    if (!pkbuf) {
        ogs_error("test_s2b_build_create_bearer_response() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int test_s2b_send_delete_bearer_response(
        test_bearer_t *bearer, ogs_gtp_xact_t *xact)
{
    int rv;
    ogs_gtp2_header_t h;
    ogs_pkbuf_t *pkbuf = NULL;
    test_sess_t *sess = NULL;

    ogs_assert(xact);
    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    memset(&h, 0, sizeof(ogs_gtp2_header_t));
    h.type = OGS_GTP2_DELETE_BEARER_RESPONSE_TYPE;
    h.teid = sess->smf_s2b_c_teid;

    pkbuf = test_s2b_build_delete_bearer_response(h.type, bearer);
    if (!pkbuf) {
        ogs_error("test_s2b_build_delete_bearer_response() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_update_tx(xact, &h, pkbuf);
    if (rv != OGS_OK) {
        ogs_error("ogs_gtp_xact_update_tx() failed");
        return OGS_ERROR;
    }

    rv = ogs_gtp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}
