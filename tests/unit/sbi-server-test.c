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

/* Backend used by ogs_sbi_server_send_error() and friends */
extern ogs_sbi_server_actions_t ogs_sbi_server_actions;

#define TEST_BODY "{\"nfInstanceId\":\"test\"}"
#define OTHER_BODY "{\"cause\":\"OTHER_RESPONSE\"}"
#define LARGE_BODY_LEN (2 * 16384 + 17)

typedef struct test_peer_s {
    ogs_pool_id_t stream_id;
    char body[LARGE_BODY_LEN + 1];
    size_t body_len;
    int status;
    bool end_stream;
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

static int test_client_header(nghttp2_session *session,
        const nghttp2_frame *frame, const uint8_t *name, size_t namelen,
        const uint8_t *value, size_t valuelen, uint8_t flags, void *user_data)
{
    test_peer_t *p = user_data;

    if (namelen == 7 && memcmp(name, ":status", 7) == 0)
        p->status = atoi((const char *)value);

    return 0;
}

static int test_client_frame(nghttp2_session *session,
        const nghttp2_frame *frame, void *user_data)
{
    test_peer_t *p = user_data;

    if ((frame->hd.type == NGHTTP2_HEADERS ||
         frame->hd.type == NGHTTP2_DATA) &&
        (frame->hd.flags & NGHTTP2_FLAG_END_STREAM))
        p->end_stream = true;

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
    ogs_sbi_server_actions_t saved_actions;

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
    conn->saved_actions = ogs_sbi_server_actions;
    ogs_sbi_server_actions = test_nghttp2_server_actions;

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
    nghttp2_session_callbacks_set_on_header_callback(
            callbacks, test_client_header);
    nghttp2_session_callbacks_set_on_frame_recv_callback(
            callbacks, test_client_frame);
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
    ogs_sbi_server_actions = conn->saved_actions;
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
    ogs_log_level_e saved_log_level;

    stream = test_conn_open(&conn, 0);
    ABTS_PTR_NOTNULL(tc, stream);
    if (!stream)
        goto out;

    ABTS_TRUE(tc, server_send_response(stream, test_response(TEST_BODY)));

    /* Suppress the expected error unless debug logging is enabled. */
    saved_log_level = ogs_log_get_domain_level(OGS_LOG_DOMAIN);
    if (saved_log_level >= OGS_LOG_ERROR && saved_log_level < OGS_LOG_DEBUG)
        ogs_log_set_domain_level(OGS_LOG_DOMAIN, OGS_LOG_FATAL);
    ABTS_TRUE(tc, server_send_response(stream, test_response(OTHER_BODY)));
    ogs_log_set_domain_level(OGS_LOG_DOMAIN, saved_log_level);

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

/* The peer opens its 8-byte window between partial DATA frames. */
static void partial_data_func(abts_case *tc, void *data)
{
    test_conn_t conn;
    ogs_sbi_stream_t *stream = NULL;

    stream = test_conn_open(&conn, 8);
    ABTS_PTR_NOTNULL(tc, stream);
    if (!stream)
        goto out;

    ABTS_TRUE(tc, server_send_response(stream, test_response(TEST_BODY)));
    server_to_client(conn.sbi_sess, conn.client);
    ABTS_INT_EQUAL(tc, 8, peer.body_len);
    ABTS_TRUE(tc, memcmp(peer.body, TEST_BODY, 8) == 0);
    ABTS_PTR_EQUAL(tc, stream, stream_find_by_id(peer.stream_id));

    client_to_server(conn.client, conn.sbi_sess);
    server_to_client(conn.sbi_sess, conn.client);
    ABTS_INT_EQUAL(tc, 16, peer.body_len);
    ABTS_TRUE(tc, memcmp(peer.body, TEST_BODY, 16) == 0);
    ABTS_PTR_EQUAL(tc, stream, stream_find_by_id(peer.stream_id));

    client_to_server(conn.client, conn.sbi_sess);
    server_to_client(conn.sbi_sess, conn.client);
    ABTS_INT_EQUAL(tc, strlen(TEST_BODY), peer.body_len);
    ABTS_TRUE(tc, peer.body_len == strlen(TEST_BODY) &&
            memcmp(peer.body, TEST_BODY, peer.body_len) == 0);
    ABTS_PTR_EQUAL(tc, NULL, stream_find_by_id(peer.stream_id));

out:
    test_conn_close(&conn);
}

/* An open window still limits each DATA frame to the default 16 KiB. */
static void large_data_func(abts_case *tc, void *data)
{
    test_conn_t conn;
    ogs_sbi_stream_t *stream = NULL;
    char body[LARGE_BODY_LEN + 1];
    int i;

    for (i = 0; i < LARGE_BODY_LEN; i++)
        body[i] = 'a' + i % 26;
    body[LARGE_BODY_LEN] = '\0';

    stream = test_conn_open(&conn, 65535);
    ABTS_PTR_NOTNULL(tc, stream);
    if (!stream)
        goto out;

    ABTS_TRUE(tc, server_send_response(stream, test_response(body)));
    server_to_client(conn.sbi_sess, conn.client);
    ABTS_INT_EQUAL(tc, LARGE_BODY_LEN, peer.body_len);
    ABTS_TRUE(tc, peer.body_len == LARGE_BODY_LEN &&
            memcmp(peer.body, body, LARGE_BODY_LEN) == 0);
    ABTS_PTR_EQUAL(tc, NULL, stream_find_by_id(peer.stream_id));

out:
    test_conn_close(&conn);
}

/* Pass the client's frames to the server through its socket. */
static void client_to_recv_handler(test_conn_t *conn)
{
    for (;;) {
        const uint8_t *data = NULL;
        ssize_t len = nghttp2_session_mem_send(conn->client, &data);

        ogs_assert(len >= 0);
        if (len == 0)
            break;
        ogs_assert(write(conn->fds[1], data, len) == len);
        recv_handler(OGS_POLLIN, conn->fds[0], conn->sbi_sess);
    }
}

static void exchange(test_conn_t *conn)
{
    int i;

    for (i = 0; i < 64; i++) {
        server_to_client(conn->sbi_sess, conn->client);
        if (!nghttp2_session_want_write(conn->client))
            break;
        client_to_recv_handler(conn);
    }
}

/* A receive-callback error completes and leaves another stream usable. */
static void recv_handler_error_response_func(abts_case *tc, void *data)
{
    test_conn_t conn;
    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id;
    nghttp2_nv nva[4];
    ogs_log_level_e saved_log_level;

    stream = test_conn_open(&conn, 8);
    ABTS_PTR_NOTNULL(tc, stream);
    if (!stream)
        goto out;
    stream_id = peer.stream_id;

    add_header(&nva[0], ":method", "GET");
    add_header(&nva[1], ":scheme", "http");
    add_header(&nva[2], ":authority", "localhost");
    add_header(&nva[3], ":path", "/nnrf-nfm/v1/nf-instances?"
            "a=1&b=2&c=3&d=4&e=5&f=6&g=7&h=8&i=9&j=10&"
            "k=11&l=12&m=13&n=14&o=15&p=16&q=17&r=18");
    ogs_assert(nghttp2_submit_request(conn.client,
                NULL, nva, OGS_ARRAY_SIZE(nva), NULL, NULL) > 0);

    /* Suppress expected rejection errors unless debug logging is enabled. */
    saved_log_level = ogs_log_get_domain_level(OGS_LOG_DOMAIN);
    if (saved_log_level >= OGS_LOG_ERROR && saved_log_level < OGS_LOG_DEBUG)
        ogs_log_set_domain_level(OGS_LOG_DOMAIN, OGS_LOG_FATAL);
    client_to_recv_handler(&conn);
    ogs_log_set_domain_level(OGS_LOG_DOMAIN, saved_log_level);

    exchange(&conn);

    ABTS_PTR_EQUAL(tc, conn.sbi_sess,
            ogs_list_first(&conn.server.session_list));
    ABTS_INT_EQUAL(tc, stream_id, peer.stream_id);
    ABTS_INT_EQUAL(tc, OGS_SBI_HTTP_STATUS_BAD_REQUEST, peer.status);
    ABTS_TRUE(tc, peer.end_stream);
    ABTS_TRUE(tc, peer.body_len > 8 &&
            peer.body[0] == '{' && peer.body[peer.body_len-1] == '}');

    stream = stream_find_by_id(stream_id);
    ABTS_PTR_NOTNULL(tc, stream);
    if (!stream)
        goto out;

    memset(&peer, 0, sizeof(peer));
    ABTS_TRUE(tc, server_send_response(stream, test_response(OTHER_BODY)));
    exchange(&conn);
    ABTS_INT_EQUAL(tc, strlen(OTHER_BODY), peer.body_len);
    ABTS_TRUE(tc, peer.body_len == strlen(OTHER_BODY) &&
            memcmp(peer.body, OTHER_BODY, peer.body_len) == 0);
    ABTS_TRUE(tc, peer.end_stream);

out:
    test_conn_close(&conn);
}

abts_suite *test_sbi_server(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, deferred_data_func, NULL);
    abts_run_test(suite, second_response_func, NULL);
    abts_run_test(suite, partial_data_func, NULL);
    abts_run_test(suite, large_data_func, NULL);
    abts_run_test(suite, recv_handler_error_response_func, NULL);

    return suite;
}
