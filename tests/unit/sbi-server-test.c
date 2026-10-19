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

#include "core/abts.h"

/*
 * Build a private copy of the nghttp2 server backend so the test can drive
 * one HTTP/2 session in memory, without a listening socket or event loop.
 */
#define ogs_nghttp2_server_actions test_nghttp2_server_actions
#include "../../lib/sbi/nghttp2-server.c"

#define TEST_BODY "{\"nfInstanceId\":\"test\"}"
#define OTHER_BODY "{\"cause\":\"OTHER_RESPONSE\"}"

typedef struct test_peer_s {
    ogs_pool_id_t stream_id;
    char body[256];
    size_t body_len;
} test_peer_t;

static test_peer_t peer;

static int test_server_cb(ogs_sbi_request_t *request, void *data)
{
    peer.stream_id = OGS_POINTER_TO_UINT(data);
    return OGS_OK;
}

static int test_client_data(nghttp2_session *session, uint8_t flags,
        int32_t stream_id, const uint8_t *data, size_t len, void *user_data)
{
    test_peer_t *p = user_data;

    ogs_assert(p->body_len + len <= sizeof(p->body));
    memcpy(p->body + p->body_len, data, len);
    p->body_len += len;

    return 0;
}

static void client_to_server(
        nghttp2_session *client, ogs_sbi_session_t *sbi_sess)
{
    for (;;) {
        const uint8_t *data = NULL;
        ssize_t len = nghttp2_session_mem_send(client, &data);

        ogs_assert(len >= 0);
        if (len == 0)
            break;
        ogs_assert(nghttp2_session_mem_recv(
                    sbi_sess->session, data, len) == len);
    }

    /* Same as recv_handler() after nghttp2_session_mem_recv() */
    ogs_assert(session_send(sbi_sess) == OGS_OK);
}

static void server_to_client(
        ogs_sbi_session_t *sbi_sess, nghttp2_session *client)
{
    ogs_pkbuf_t *pkbuf = NULL, *next_pkbuf = NULL;

    ogs_list_for_each_safe(&sbi_sess->write_queue, next_pkbuf, pkbuf) {
        ogs_list_remove(&sbi_sess->write_queue, pkbuf);
        ogs_assert(nghttp2_session_mem_recv(
                    client, pkbuf->data, pkbuf->len) == pkbuf->len);
        ogs_pkbuf_free(pkbuf);
    }
}

static ogs_sbi_response_t *test_response(const char *body)
{
    ogs_sbi_response_t *response = ogs_sbi_response_new();

    ogs_assert(response);
    response->status = OGS_SBI_HTTP_STATUS_OK;
    response->http.content = ogs_strdup(body);
    ogs_assert(response->http.content);
    response->http.content_length = strlen(body);

    return response;
}

/* One server session with an in-memory client and one open request */
typedef struct test_conn_s {
    ogs_pollset_t *saved_pollset;
    uint64_t saved_stream_pool;

    ogs_sbi_server_t server;
    ogs_sbi_session_t *sbi_sess;
    int fds[2];

    nghttp2_session *client;
    int32_t client_stream_id;
} test_conn_t;

static ogs_sbi_stream_t *test_conn_open(test_conn_t *conn, uint32_t window)
{
    ogs_sock_t *sock = NULL;
    nghttp2_session_callbacks *callbacks = NULL;
    nghttp2_settings_entry iv[1] = {
        { NGHTTP2_SETTINGS_INITIAL_WINDOW_SIZE, window }
    };
    nghttp2_nv nva[4];

    memset(&peer, 0, sizeof(peer));
    memset(conn, 0, sizeof(*conn));

    conn->saved_pollset = ogs_app()->pollset;
    conn->saved_stream_pool = ogs_app()->pool.stream;

    server_init(4, 4);
    ogs_app()->pollset = ogs_pollset_create(16);
    ogs_assert(ogs_app()->pollset);
    ogs_app()->pool.stream = 4;

    conn->server.cb = test_server_cb;
    ogs_list_init(&conn->server.session_list);

    ogs_assert(socketpair(AF_UNIX, SOCK_STREAM, 0, conn->fds) == 0);
    sock = ogs_sock_create();
    ogs_assert(sock);
    sock->fd = conn->fds[0];

    conn->sbi_sess = session_add(&conn->server, sock);
    ogs_assert(conn->sbi_sess);
    ogs_assert(session_set_callbacks(conn->sbi_sess) == OGS_OK);
    ogs_assert(session_send_preface(conn->sbi_sess) == OGS_OK);

    ogs_assert(nghttp2_session_callbacks_new(&callbacks) == 0);
    nghttp2_session_callbacks_set_on_data_chunk_recv_callback(
            callbacks, test_client_data);
    ogs_assert(nghttp2_session_client_new(
                &conn->client, callbacks, &peer) == 0);
    nghttp2_session_callbacks_del(callbacks);

    ogs_assert(nghttp2_submit_settings(conn->client,
                NGHTTP2_FLAG_NONE, iv, OGS_ARRAY_SIZE(iv)) == 0);
    add_header(&nva[0], ":method", "GET");
    add_header(&nva[1], ":scheme", "http");
    add_header(&nva[2], ":authority", "localhost");
    add_header(&nva[3], ":path", "/nnrf-nfm/v1/nf-instances");
    conn->client_stream_id = nghttp2_submit_request(
            conn->client, NULL, nva, OGS_ARRAY_SIZE(nva), NULL, NULL);
    ogs_assert(conn->client_stream_id > 0);

    client_to_server(conn->client, conn->sbi_sess);
    server_to_client(conn->sbi_sess, conn->client);

    return stream_find_by_id(peer.stream_id);
}

static void test_conn_close(test_conn_t *conn)
{
    nghttp2_session_del(conn->client);
    session_remove(conn->sbi_sess);
    close(conn->fds[1]);

    ogs_pollset_destroy(ogs_app()->pollset);
    ogs_app()->pollset = conn->saved_pollset;
    ogs_app()->pool.stream = conn->saved_stream_pool;
    server_final();
}

/*
 * The peer controls HTTP/2 flow control. With a zero stream window the
 * response HEADERS go out at once but the DATA frame waits for the peer's
 * WINDOW_UPDATE, after ogs_sbi_server_send_response() has returned and
 * freed the response. The body sent then must still be this response's.
 */
static void deferred_data_func(abts_case *tc, void *data)
{
    test_conn_t conn;
    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_response_t *other = NULL;

    stream = test_conn_open(&conn, 0);
    ABTS_PTR_NOTNULL(tc, stream);
    if (!stream)
        goto out;

    ABTS_TRUE(tc, server_send_response(stream, test_response(TEST_BODY)));
    server_to_client(conn.sbi_sess, conn.client);

    /* The zero window holds the DATA frame back */
    ABTS_INT_EQUAL(tc, 0, peer.body_len);

    /*
     * Another response is built while this stream's DATA is pending.
     * Unless the stream owns the body, the deferred DATA frame reads
     * the freed response, which AddressSanitizer reports.
     */
    other = test_response(OTHER_BODY);

    ogs_assert(nghttp2_submit_window_update(conn.client,
                NGHTTP2_FLAG_NONE, conn.client_stream_id, 1024) == 0);
    client_to_server(conn.client, conn.sbi_sess);
    server_to_client(conn.sbi_sess, conn.client);

    ABTS_INT_EQUAL(tc, strlen(TEST_BODY), peer.body_len);
    ABTS_TRUE(tc, peer.body_len == strlen(TEST_BODY) &&
            memcmp(peer.body, TEST_BODY, peer.body_len) == 0);

    /* END_STREAM went out with the DATA frame, so the stream is gone */
    ABTS_PTR_EQUAL(tc, NULL, stream_find_by_id(peer.stream_id));

out:
    if (other)
        ogs_sbi_response_free(other);

    test_conn_close(&conn);
}

/*
 * A request is answered once. A second response for the same stream is
 * refused, and the first body still goes out when the window opens.
 */
static void second_response_func(abts_case *tc, void *data)
{
    test_conn_t conn;
    ogs_sbi_stream_t *stream = NULL;

    stream = test_conn_open(&conn, 0);
    ABTS_PTR_NOTNULL(tc, stream);
    if (!stream)
        goto out;

    ABTS_TRUE(tc, server_send_response(stream, test_response(TEST_BODY)));
    ABTS_TRUE(tc, server_send_response(stream, test_response(OTHER_BODY)));
    server_to_client(conn.sbi_sess, conn.client);
    ABTS_PTR_EQUAL(tc, stream, stream_find_by_id(peer.stream_id));

    ogs_assert(nghttp2_submit_window_update(conn.client,
                NGHTTP2_FLAG_NONE, conn.client_stream_id, 1024) == 0);
    client_to_server(conn.client, conn.sbi_sess);
    server_to_client(conn.sbi_sess, conn.client);

    ABTS_INT_EQUAL(tc, strlen(TEST_BODY), peer.body_len);
    ABTS_TRUE(tc, peer.body_len == strlen(TEST_BODY) &&
            memcmp(peer.body, TEST_BODY, peer.body_len) == 0);

out:
    test_conn_close(&conn);
}

abts_suite *test_sbi_server(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, deferred_data_func, NULL);
    abts_run_test(suite, second_response_func, NULL);

    return suite;
}
