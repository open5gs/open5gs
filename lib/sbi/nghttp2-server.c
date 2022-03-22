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

#include "ogs-app.h"
#include "ogs-sbi.h"
#include "yuarel.h"

#include <netinet/tcp.h>
#include <nghttp2/nghttp2.h>

#define USE_SEND_DATA_WITH_NO_COPY 1

static void server_init(int num_of_session_pool, int num_of_stream_pool);
static void server_final(void);

static int server_start(ogs_sbi_server_t *server,
        int (*cb)(ogs_sbi_request_t *request, void *data));
static void server_stop(ogs_sbi_server_t *server);

static bool server_send_response(
        ogs_sbi_stream_t *stream, ogs_sbi_response_t *response);

static ogs_sbi_server_t *server_from_stream(ogs_sbi_stream_t *data);

const ogs_sbi_server_actions_t ogs_nghttp2_server_actions = {
    server_init,
    server_final,

    server_start,
    server_stop,

    server_send_response,
    server_from_stream,
};

struct h2_settings {
    uint32_t max_concurrent_streams;
    bool enable_push;
};

typedef struct ogs_sbi_session_s {
    ogs_lnode_t             lnode;

    ogs_sock_t              *sock;
    ogs_sockaddr_t          *addr;
    struct {
        ogs_poll_t          *read;
        ogs_poll_t          *write;
    } poll;

    nghttp2_session         *session;
    ogs_list_t              write_queue;

    ogs_sbi_server_t        *server;
    ogs_list_t              stream_list;
    int32_t                 last_stream_id;

    struct h2_settings      settings;
} ogs_sbi_session_t;

typedef struct ogs_sbi_stream_s {
    ogs_lnode_t             lnode;

    int32_t                 stream_id;
    ogs_sbi_request_t       *request;

    ogs_sbi_session_t       *session;
} ogs_sbi_stream_t;

static void session_remove(ogs_sbi_session_t *sbi_sess);
static void session_remove_all(ogs_sbi_server_t *server);

static void stream_remove(ogs_sbi_stream_t *stream);

static void accept_handler(short when, ogs_socket_t fd, void *data);
static void recv_handler(short when, ogs_socket_t fd, void *data);

static int session_set_callbacks(ogs_sbi_session_t *sbi_sess);
static int session_send_preface(ogs_sbi_session_t *sbi_sess);
static int session_send(ogs_sbi_session_t *sbi_sess);
static void session_write_to_buffer(
        ogs_sbi_session_t *sbi_sess, ogs_pkbuf_t *pkbuf);

static OGS_POOL(session_pool, ogs_sbi_session_t);
static OGS_POOL(stream_pool, ogs_sbi_stream_t);

static void server_init(int num_of_session_pool, int num_of_stream_pool)
{
    ogs_pool_init(&session_pool, num_of_session_pool);
    ogs_pool_init(&stream_pool, num_of_stream_pool);
}

static void server_final(void)
{
    ogs_pool_final(&stream_pool);
    ogs_pool_final(&session_pool);
}

static int server_start(ogs_sbi_server_t *server,
        int (*cb)(ogs_sbi_request_t *request, void *data))
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sock_t *sock = NULL;
    ogs_sockaddr_t *addr = NULL;
    char *hostname = NULL;

    addr = server->node.addr;
    ogs_assert(addr);

    sock = ogs_tcp_server(addr, server->node.option);
    if (!sock) {
        ogs_error("Cannot start SBI server");
        return OGS_ERROR;
    }

    server->node.sock = sock;

    /* Setup callback function */
    server->cb = cb;

    /* Setup poll for server listening socket */
    server->node.poll = ogs_pollset_add(ogs_app()->pollset,
            OGS_POLLIN, sock->fd, accept_handler, server);
    ogs_assert(server->node.poll);

    hostname = ogs_gethostname(addr);
    if (hostname)
        ogs_info("nghttp2_server() [%s]:%d", hostname, OGS_PORT(addr));
    else
        ogs_info("nghttp2_server() [%s]:%d",
                OGS_ADDR(addr, buf), OGS_PORT(addr));

    return OGS_OK;
}

static void server_stop(ogs_sbi_server_t *server)
{
    ogs_assert(server);

    if (server->node.poll)
        ogs_pollset_remove(server->node.poll);

    if (server->node.sock)
        ogs_sock_destroy(server->node.sock);

    session_remove_all(server);
}

static void add_header(nghttp2_nv *nv, const char *key, const char *value)
{
    nv->name = (uint8_t *)key;
    nv->namelen = strlen(key);
    nv->value = (uint8_t *)value;
    nv->valuelen = strlen(value);
    nv->flags = NGHTTP2_NV_FLAG_NONE;
}

static char status_string[600][4] = {
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "100", "101", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "200", "201", "202", "203", "204", "205", "206", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "300", "301", "302", "303", "304", "305", "306", "307", "308", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "400", "401", "402", "403", "404", "405", "406", "407", "408", "409",
 "410", "411", "412", "413", "414", "415", "416", "417", "", "",
 "", "421", "", "", "", "", "426", "", "428", "429", "", "431", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "451", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "500", "501", "502", "503", "504", "505", "", "", "", "", "", "511", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
 "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""
};

#define DATE_STRLEN 128
static char *get_date_string(char *date)
{
    static const char *const days[] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };
    static const char *const mons[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    ogs_assert(date);

    struct tm tm;
    ogs_gmtime(ogs_time_sec(ogs_time_now()), &tm);

    ogs_snprintf(date, DATE_STRLEN, "%3s, %02u %3s %04u %02u:%02u:%02u GMT",
            days[tm.tm_wday % 7],
            (unsigned int)tm.tm_mday,
            mons[tm.tm_mon % 12],
            (unsigned int)(1900 + tm.tm_year),
            (unsigned int)tm.tm_hour,
            (unsigned int)tm.tm_min,
            (unsigned int)tm.tm_sec);

    return date;
}

static ssize_t response_read_callback(nghttp2_session *session,
                                      int32_t stream_id,
                                      uint8_t *buf, size_t length,
                                      uint32_t *data_flags,
                                      nghttp2_data_source *source,
                                      void *user_data)
{
#if USE_SEND_DATA_WITH_NO_COPY
    int rv;
#endif

    ogs_sbi_response_t *response = NULL;
    ogs_sbi_stream_t *stream = NULL;

    ogs_assert(session);

    stream = nghttp2_session_get_stream_user_data(session, stream_id);
    if (!stream) {
        ogs_error("no stream [%d]", stream_id);
        return NGHTTP2_ERR_TEMPORAL_CALLBACK_FAILURE;
    }

    ogs_assert(source);
    response = source->ptr;
    ogs_assert(response);

    ogs_assert(response->http.content);
    ogs_assert(response->http.content_length);

#if USE_SEND_DATA_WITH_NO_COPY
    *data_flags |= NGHTTP2_DATA_FLAG_NO_COPY;
#else
    memcpy(buf, response->http.content, response->http.content_length);
#endif

    *data_flags |= NGHTTP2_DATA_FLAG_EOF;

#if USE_SEND_DATA_WITH_NO_COPY
    rv = nghttp2_session_get_stream_remote_close(session, stream_id);
    if (rv == 0) {
        ogs_warn("nghttp2_session_get_stream_remote_close() failed");
        nghttp2_submit_rst_stream(
                session, NGHTTP2_FLAG_NONE, stream_id, NGHTTP2_NO_ERROR);
    } else if (rv != 1) {
        ogs_error("nghttp2_session_get_stream_remote_close() failed[%d]", rv);
    }
#endif

    return response->http.content_length;
}

static bool server_send_response(
        ogs_sbi_stream_t *stream, ogs_sbi_response_t *response)
{
    ogs_sbi_session_t *sbi_sess = NULL;
    ogs_sock_t *sock = NULL;
    ogs_socket_t fd = INVALID_SOCKET;

    ogs_hash_index_t *hi;
    nghttp2_nv *nva;
    size_t nvlen;
    int i, rv;
    char datebuf[DATE_STRLEN];
    char srv_version[128];
    char clen[128];

    ogs_assert(stream);
    sbi_sess = stream->session;
    ogs_assert(sbi_sess);
    ogs_assert(sbi_sess->session);
    ogs_assert(response);

    sock = sbi_sess->sock;
    ogs_assert(sock);
    fd = sock->fd;
    ogs_assert(fd != INVALID_SOCKET); /* Check if session is removed */

    nvlen = 3; /* :status && server && date */

    for (hi = ogs_hash_first(response->http.headers);
            hi; hi = ogs_hash_next(hi))
        nvlen++;

    if (response->http.content && response->http.content_length)
        nvlen++;

    nva = ogs_calloc(nvlen, sizeof(nghttp2_nv));
    ogs_expect_or_return_val(nva, false);

    i = 0;

    ogs_expect_or_return_val(response->status < 600, false);
    ogs_assert(strlen(status_string[response->status]) == 3);
    add_header(&nva[i++], ":status", status_string[response->status]);

    ogs_snprintf(srv_version, sizeof(srv_version),
            "Open5GS %s", ogs_app()->version ? ogs_app()->version : "TEST");
    add_header(&nva[i++], "server", srv_version);
    add_header(&nva[i++], "date", get_date_string(datebuf));

    if (response->http.content && response->http.content_length) {
        ogs_snprintf(clen, sizeof(clen),
                "%d", (int)response->http.content_length);
        add_header(&nva[i++], "content-length", clen);
    }

    for (hi = ogs_hash_first(response->http.headers);
            hi; hi = ogs_hash_next(hi)) {
        add_header(&nva[i++], ogs_hash_this_key(hi), ogs_hash_this_val(hi));
    }

    ogs_debug("STATUS [%d]", response->status);

    if (response->http.content && response->http.content_length) {
        nghttp2_data_provider data_prd;

        data_prd.source.ptr = response;
        data_prd.read_callback = response_read_callback;

        ogs_debug("SENDING...: %d", (int)response->http.content_length);
        ogs_debug("%s", response->http.content);

        rv = nghttp2_submit_response(sbi_sess->session,
                stream->stream_id, nva, nvlen, &data_prd);
    } else {
        rv = nghttp2_submit_response(sbi_sess->session,
                stream->stream_id, nva, nvlen, NULL);
    }

    if (rv != OGS_OK) {
        ogs_error("nghttp2_submit_response(%d) failed (%d:%s)",
                    (int)response->http.content_length,
                    rv, nghttp2_strerror(rv));
        nghttp2_submit_rst_stream(
                sbi_sess->session, NGHTTP2_FLAG_NONE, stream->stream_id, rv);
    }

    if (session_send(sbi_sess) != OGS_OK) {
        ogs_error("session_send() failed");
        session_remove(sbi_sess);
    }

    ogs_sbi_response_free(response);
    ogs_free(nva);

    return true;
}

static ogs_sbi_server_t *server_from_stream(ogs_sbi_stream_t *stream)
{
    ogs_sbi_session_t *sbi_sess = NULL;

    ogs_assert(stream);
    sbi_sess = stream->session;
    ogs_assert(sbi_sess);
    ogs_assert(sbi_sess->server);

    return sbi_sess->server;
}

static ogs_sbi_stream_t *stream_add(
        ogs_sbi_session_t *sbi_sess, int32_t stream_id)
{
    ogs_sbi_stream_t *stream = NULL;

    ogs_assert(sbi_sess);

    ogs_pool_alloc(&stream_pool, &stream);
    ogs_expect_or_return_val(stream, NULL);
    memset(stream, 0, sizeof(ogs_sbi_stream_t));

    stream->request = ogs_sbi_request_new();
    ogs_expect_or_return_val(stream->request, NULL);

    stream->stream_id = stream_id;
    sbi_sess->last_stream_id = stream_id;

    stream->session = sbi_sess;

    return stream;
}

static void stream_remove(ogs_sbi_stream_t *stream)
{
    ogs_sbi_session_t *sbi_sess = NULL;

    ogs_assert(stream);
    sbi_sess = stream->session;
    ogs_assert(sbi_sess);

    ogs_list_remove(&sbi_sess->stream_list, stream);

    ogs_assert(stream->request);
    ogs_sbi_request_free(stream->request);

    ogs_pool_free(&stream_pool, stream);
}

static void stream_remove_all(ogs_sbi_session_t *sbi_sess)
{
    ogs_sbi_stream_t *stream = NULL, *next_stream = NULL;

    ogs_assert(sbi_sess);

    ogs_list_for_each_safe(&sbi_sess->stream_list, next_stream, stream)
        stream_remove(stream);
}

static ogs_sbi_session_t *session_add(
        ogs_sbi_server_t *server, ogs_sock_t *sock)
{
    ogs_sbi_session_t *sbi_sess = NULL;

    ogs_assert(server);
    ogs_assert(sock);

    ogs_pool_alloc(&session_pool, &sbi_sess);
    ogs_expect_or_return_val(sbi_sess, NULL);
    memset(sbi_sess, 0, sizeof(ogs_sbi_session_t));

    sbi_sess->server = server;
    sbi_sess->sock = sock;

    sbi_sess->addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));
    ogs_expect_or_return_val(sbi_sess->addr, NULL);
    memcpy(sbi_sess->addr, &sock->remote_addr, sizeof(ogs_sockaddr_t));

    ogs_list_add(&server->session_list, sbi_sess);

    return sbi_sess;
}

static void session_remove(ogs_sbi_session_t *sbi_sess)
{
    ogs_sbi_server_t *server = NULL;
    ogs_pkbuf_t *pkbuf = NULL, *next_pkbuf = NULL;

    ogs_assert(sbi_sess);
    server = sbi_sess->server;
    ogs_assert(server);

    ogs_list_remove(&server->session_list, sbi_sess);

    stream_remove_all(sbi_sess);

    ogs_assert(sbi_sess->poll.read);
    ogs_pollset_remove(sbi_sess->poll.read);

    if (sbi_sess->poll.write)
        ogs_pollset_remove(sbi_sess->poll.write);

    ogs_list_for_each_safe(&sbi_sess->write_queue, next_pkbuf, pkbuf) {
        ogs_list_remove(&sbi_sess->write_queue, pkbuf);
        ogs_pkbuf_free(pkbuf);
    }

    ogs_assert(sbi_sess->addr);
    ogs_free(sbi_sess->addr);

    ogs_assert(sbi_sess->sock);
    ogs_sock_destroy(sbi_sess->sock);

    ogs_pool_free(&session_pool, sbi_sess);
}

static void session_remove_all(ogs_sbi_server_t *server)
{
    ogs_sbi_session_t *sbi_sess = NULL, *next_sbi_sess = NULL;

    ogs_assert(server);

    ogs_list_for_each_safe(&server->session_list, next_sbi_sess, sbi_sess)
        session_remove(sbi_sess);
}

static void accept_handler(short when, ogs_socket_t fd, void *data)
{
    ogs_sbi_server_t *server = data;
    ogs_sbi_session_t *sbi_sess = NULL;
    ogs_sock_t *sock = NULL;
    ogs_sock_t *new = NULL;

    int on;

    ogs_assert(data);
    ogs_assert(fd != INVALID_SOCKET);

    sock = server->node.sock;

    new = ogs_sock_accept(sock);
    if (!new) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno, "accept() failed");
        return;
    }
    ogs_assert(new->fd != INVALID_SOCKET);

    on = 1;
    if (setsockopt(new->fd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on)) != 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "setsockopt() for SCTP_NODELAY failed");
        ogs_sock_destroy(new);
        return;
    }

    sbi_sess = session_add(server, new);
    ogs_assert(sbi_sess);

    sbi_sess->poll.read = ogs_pollset_add(ogs_app()->pollset,
        OGS_POLLIN, new->fd, recv_handler, sbi_sess);
    ogs_assert(sbi_sess->poll.read);

    if (session_set_callbacks(sbi_sess) != OGS_OK ||
        session_send_preface(sbi_sess) != OGS_OK) {
        ogs_error("session_add() failed");
        session_remove(sbi_sess);
    }
}

static void recv_handler(short when, ogs_socket_t fd, void *data)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sockaddr_t *addr = NULL;

    ogs_sbi_session_t *sbi_sess = data;
    ogs_pkbuf_t *pkbuf = NULL;
    ssize_t readlen;
    int n;

    ogs_assert(sbi_sess);
    ogs_assert(fd != INVALID_SOCKET);
    addr = sbi_sess->addr;
    ogs_assert(addr);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);

    n = ogs_recv(fd, pkbuf->data, OGS_MAX_SDU_LEN, 0);
    if (n > 0) {
        ogs_pkbuf_put(pkbuf, n);

        ogs_assert(sbi_sess->session);
        readlen = nghttp2_session_mem_recv(
                sbi_sess->session, pkbuf->data, pkbuf->len);
        if (readlen < 0) {
            ogs_error("nghttp2_session_mem_recv() failed (%d:%s)",
                        (int)readlen, nghttp2_strerror((int)readlen));
            session_remove(sbi_sess);
        }
    } else {
        if (n < 0) {
            if (errno != OGS_ECONNRESET)
                ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                                "lost connection [%s]:%d",
                                OGS_ADDR(addr, buf), OGS_PORT(addr));
        } else if (n == 0) {
            ogs_debug("connection closed [%s]:%d",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));
        }

        session_remove(sbi_sess);
    }

    ogs_pkbuf_free(pkbuf);
}

static int on_frame_recv(nghttp2_session *session,
                         const nghttp2_frame *frame, void *user_data);
static int on_stream_close(nghttp2_session *session, int32_t stream_id,
                           uint32_t error_code, void *user_data);
static int on_header(nghttp2_session *session,
                     const nghttp2_frame *frame,
                     nghttp2_rcbuf *name, nghttp2_rcbuf *value,
                     uint8_t flags, void *user_data);
static int on_data_chunk_recv(nghttp2_session *session, uint8_t flags,
                              int32_t stream_id, const uint8_t *data,
                              size_t len, void *user_data);
static int error_callback(nghttp2_session *session,
                          const char *msg, size_t len, void *user_data);
static int on_invalid_frame_recv(nghttp2_session *session,
                                 const nghttp2_frame *frame,
                                 int error_code, void *user_data);
static int on_invalid_header(nghttp2_session *session,
                             const nghttp2_frame *frame,
                             const uint8_t *name, size_t namelen,
                             const uint8_t *value, size_t valuelen,
                             uint8_t flags, void *user_data);
static int on_begin_frame(nghttp2_session *session,
                          const nghttp2_frame_hd *hd, void *user_data);
static int on_begin_headers(nghttp2_session *session,
                            const nghttp2_frame *frame,
                                     void *user_data);
#if USE_SEND_DATA_WITH_NO_COPY
static int on_send_data(nghttp2_session *session, nghttp2_frame *frame,
                        const uint8_t *framehd, size_t length,
                        nghttp2_data_source *source, void *user_data);
#else
static ssize_t send_callback(nghttp2_session *session, const uint8_t *data,
                             size_t length, int flags, void *user_data);
#endif

static int session_set_callbacks(ogs_sbi_session_t *sbi_sess)
{
    int rv;
    nghttp2_session_callbacks *callbacks = NULL;

    ogs_assert(sbi_sess);

    rv = nghttp2_session_callbacks_new(&callbacks);
    if (rv != 0) {
        ogs_error("nghttp2_session_callbacks_new() failed (%d:%s)",
                    rv, nghttp2_strerror(rv));
        return OGS_ERROR;
    }

    nghttp2_session_callbacks_set_on_frame_recv_callback(
            callbacks, on_frame_recv);

    nghttp2_session_callbacks_set_on_stream_close_callback(
            callbacks, on_stream_close);

    nghttp2_session_callbacks_set_on_header_callback2(callbacks, on_header);

    nghttp2_session_callbacks_set_on_data_chunk_recv_callback(
            callbacks, on_data_chunk_recv);

    nghttp2_session_callbacks_set_error_callback(callbacks, error_callback);

    nghttp2_session_callbacks_set_on_invalid_frame_recv_callback(
            callbacks, on_invalid_frame_recv);

    nghttp2_session_callbacks_set_on_invalid_header_callback(
            callbacks, on_invalid_header);

    nghttp2_session_callbacks_set_on_begin_frame_callback(
            callbacks, on_begin_frame);

    nghttp2_session_callbacks_set_on_begin_headers_callback(
            callbacks, on_begin_headers);

#if USE_SEND_DATA_WITH_NO_COPY
    nghttp2_session_callbacks_set_send_data_callback(callbacks, on_send_data);
#else
    nghttp2_session_callbacks_set_send_callback(callbacks, send_callback);
#endif

    rv = nghttp2_session_server_new(&sbi_sess->session, callbacks, sbi_sess);
    if (rv != 0) {
        ogs_error("nghttp2_session_callbacks_new() failed (%d:%s)",
                    rv, nghttp2_strerror(rv));
        return OGS_ERROR;
    }

    nghttp2_session_callbacks_del(callbacks);

    return OGS_OK;
}

static int on_frame_recv(nghttp2_session *session,
                         const nghttp2_frame *frame, void *user_data)
{
    int rv;
    ogs_sbi_session_t *sbi_sess = user_data;

    ogs_sbi_server_t *server = NULL;
    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(sbi_sess);
    server = sbi_sess->server;
    ogs_assert(server);
    ogs_assert(server->cb);

    ogs_assert(session);
    ogs_assert(frame);

    stream = nghttp2_session_get_stream_user_data(session, frame->hd.stream_id);
    if (!stream) {
        if (frame->hd.type == NGHTTP2_SETTINGS) {
            sbi_sess->settings.max_concurrent_streams =
                nghttp2_session_get_remote_settings(
                    session, NGHTTP2_SETTINGS_MAX_CONCURRENT_STREAMS);
            sbi_sess->settings.enable_push =
                nghttp2_session_get_remote_settings(
                    session, NGHTTP2_SETTINGS_ENABLE_PUSH);
            ogs_debug("MAX_CONCURRENT_STREAMS = %d",
                sbi_sess->settings.max_concurrent_streams);
            ogs_debug("ENABLE_PUSH = %s",
                sbi_sess->settings.enable_push ? "TRUE" : "false");

        } else if (frame->hd.type == NGHTTP2_GOAWAY) {
            rv = nghttp2_submit_goaway(
                 session, NGHTTP2_FLAG_NONE, sbi_sess->last_stream_id,
                 NGHTTP2_NO_ERROR, NULL, 0);
            if (rv != 0) {
                ogs_error("nghttp2_submit_goaway() failed (%d:%s)",
                            rv, nghttp2_strerror(rv));
                return OGS_ERROR;
            }

            session_send(sbi_sess);
        }
        return 0;
    }

    request = stream->request;
    ogs_assert(request);

    switch (frame->hd.type) {
    case NGHTTP2_HEADERS:
        if (frame->headers.cat == NGHTTP2_HCAT_REQUEST) {
            const char *expect100 =
                ogs_sbi_header_get(request->http.headers, OGS_SBI_EXPECT);
            if (expect100 && ogs_strcasecmp(expect100, "100-continue") == 0) {
                nghttp2_nv nva;

                add_header(&nva, ":status", status_string[100]);
                rv = nghttp2_submit_headers(session, NGHTTP2_FLAG_NONE,
                           stream->stream_id, NULL, &nva, 1, NULL);
                if (rv != 0) {
                    ogs_error("nghttp2_submit_headers() failed (%d:%s)",
                            rv, nghttp2_strerror(rv));
                    nghttp2_submit_rst_stream(
                            session, NGHTTP2_FLAG_NONE, stream->stream_id, rv);
                    return 0;
                }
            }
        }
        /* fallthrough */
        OGS_GNUC_FALLTHROUGH;

    case NGHTTP2_DATA:
        /* HEADERS or DATA frame with +END_STREAM flag */
        if (frame->hd.flags & NGHTTP2_FLAG_END_STREAM) {

            ogs_debug("[%s] %s", request->h.method, request->h.uri);

            if (request->http.content_length && request->http.content) {
                ogs_debug("RECEIVED: %d", (int)request->http.content_length);
                ogs_debug("%s", request->http.content);
            }

            if (server->cb(request, stream) != OGS_OK) {
                ogs_warn("server callback error");
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                        "server callback error", NULL));

                return 0;
            }
            break;
        }
    default:
        break;
    }

    return 0;
}

static int on_stream_close(nghttp2_session *session, int32_t stream_id,
                           uint32_t error_code, void *user_data)
{
    ogs_sbi_stream_t *stream = NULL;

    ogs_assert(session);

    stream = nghttp2_session_get_stream_user_data(session, stream_id);
    if (!stream) {
        ogs_error("no stream [%d]", stream_id);
        return NGHTTP2_ERR_TEMPORAL_CALLBACK_FAILURE;
    }

    if (error_code) {
        ogs_error("on_stream_close_callback() failed (%d:%s)",
                    error_code, nghttp2_http2_strerror(error_code));
        nghttp2_submit_rst_stream(
                session, NGHTTP2_FLAG_NONE, stream_id, error_code);
    }

    ogs_debug("STREAM closed [%d]", stream_id);
    stream_remove(stream);
    return 0;
}

static int on_header(nghttp2_session *session, const nghttp2_frame *frame,
                     nghttp2_rcbuf *name, nghttp2_rcbuf *value,
                     uint8_t flags, void *user_data)
{
    ogs_sbi_session_t *sbi_sess = user_data;
    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_request_t *request = NULL;

    const char PATH[] = ":path";
    const char METHOD[] = ":method";

    nghttp2_vec namebuf, valuebuf;
    char *namestr = NULL, *valuestr = NULL;

    ogs_assert(session);
    ogs_assert(frame);

    if (frame->hd.type != NGHTTP2_HEADERS ||
        frame->headers.cat != NGHTTP2_HCAT_REQUEST) {
        return 0;
    }

    stream = nghttp2_session_get_stream_user_data(session, frame->hd.stream_id);
    if (!stream) {
        ogs_error("no stream [%d]", frame->hd.stream_id);
        return NGHTTP2_ERR_TEMPORAL_CALLBACK_FAILURE;
    }

    ogs_assert(sbi_sess);

    request = stream->request;
    ogs_assert(request);

    ogs_assert(name);
    namebuf = nghttp2_rcbuf_get_buf(name);
    ogs_assert(namebuf.base);
    ogs_assert(namebuf.len);

    ogs_assert(value);
    valuebuf = nghttp2_rcbuf_get_buf(value);
    ogs_assert(valuebuf.base);
    ogs_assert(valuebuf.len);

    namestr = ogs_strndup((const char *)namebuf.base, namebuf.len);
    ogs_assert(namestr);

    valuestr = ogs_strndup((const char *)valuebuf.base, valuebuf.len);
    ogs_assert(valuestr);

    if (namebuf.len == sizeof(PATH) - 1 &&
            memcmp(PATH, namebuf.base, namebuf.len) == 0) {
        char *saveptr = NULL, *query;
#define MAX_NUM_OF_PARAM_IN_QUERY 16
        struct yuarel_param params[MAX_NUM_OF_PARAM_IN_QUERY+2];
        int j;

        ogs_assert(request->h.uri == NULL);
        request->h.uri = ogs_sbi_parse_uri(valuestr, "?", &saveptr);
        ogs_assert(request->h.uri);

        memset(params, 0, sizeof(params));

        query = ogs_sbi_parse_uri(NULL, "?", &saveptr);
        if (query && *query && strlen(query))
            yuarel_parse_query(query, '&', params, MAX_NUM_OF_PARAM_IN_QUERY+1);

        j = 0;
        while(params[j].key && params[j].val) {
            ogs_sbi_header_set(request->http.params,
                    params[j].key, params[j].val);
            j++;
        }

        if (j >= MAX_NUM_OF_PARAM_IN_QUERY+1) {
            ogs_fatal("Maximum number(%d) of query params reached",
                    MAX_NUM_OF_PARAM_IN_QUERY);
            ogs_assert_if_reached();
        }

        ogs_free(query);

    } else if (namebuf.len == sizeof(METHOD) - 1 &&
            memcmp(METHOD, namebuf.base, namebuf.len) == 0) {

        ogs_assert(request->h.method == NULL);
        request->h.method = ogs_strdup(valuestr);
        ogs_assert(request->h.method);

    } else {

        ogs_sbi_header_set(request->http.headers, namestr, valuestr);

    }

    ogs_free(namestr);
    ogs_free(valuestr);

    return 0;
}

static int on_data_chunk_recv(nghttp2_session *session, uint8_t flags,
                              int32_t stream_id, const uint8_t *data,
                              size_t len, void *user_data)
{
    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_request_t *request = NULL;

    size_t offset = 0;

    ogs_assert(session);

    stream = nghttp2_session_get_stream_user_data(session, stream_id);
    if (!stream) {
        ogs_error("no stream [%d]", stream_id);
        return 0;
    }

    request = stream->request;
    ogs_assert(request);

    ogs_assert(data);
    ogs_assert(len);

    if (request->http.content == NULL) {
        request->http.content_length = len;
        request->http.content =
            (char*)ogs_malloc(request->http.content_length + 1);
        ogs_assert(request->http.content);
    } else {
        offset = request->http.content_length;
        if ((request->http.content_length + len) > OGS_HUGE_LEN) {
            ogs_error("Overflow : Content-Length[%d], len[%d]",
                        (int)request->http.content_length, (int)len);
            ogs_assert_if_reached();
        }
        request->http.content_length += len;
        request->http.content = (char *)ogs_realloc(
                request->http.content, request->http.content_length + 1);
        ogs_assert(request->http.content);
    }

    memcpy(request->http.content + offset, data, len);
    request->http.content[request->http.content_length] = '\0';

    return 0;
}

static int error_callback(nghttp2_session *session,
                          const char *msg, size_t len, void *user_data)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sockaddr_t *addr = NULL;
    ogs_sbi_session_t *sbi_sess = user_data;

    ogs_assert(sbi_sess);
    addr = sbi_sess->addr;
    ogs_assert(addr);

    ogs_assert(msg);

    ogs_error("[%s]:%d http2 error: %.*s",
            OGS_ADDR(addr, buf), OGS_PORT(addr), (int)len, msg);

    return 0;
}

static int on_invalid_frame_recv(nghttp2_session *session,
                                 const nghttp2_frame *frame,
                                 int error_code, void *user_data)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sockaddr_t *addr = NULL;

    ogs_sbi_session_t *sbi_sess = user_data;

    ogs_assert(sbi_sess);
    addr = sbi_sess->addr;
    ogs_assert(addr);

    ogs_error("[%s]:%d invalid frame (%d:%s)",
            OGS_ADDR(addr, buf), OGS_PORT(addr),
            error_code, nghttp2_strerror(error_code));
    return 0;
}

static int on_invalid_header(nghttp2_session *session,
                             const nghttp2_frame *frame,
                             const uint8_t *name, size_t namelen,
                             const uint8_t *value, size_t valuelen,
                             uint8_t flags, void *user_data)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sockaddr_t *addr = NULL;
    char *namestr = NULL, *valuestr = NULL;

    ogs_sbi_session_t *sbi_sess = user_data;

    ogs_assert(sbi_sess);
    addr = sbi_sess->addr;
    ogs_assert(addr);

    namestr = ogs_strndup((const char *)name, namelen);
    ogs_assert(namestr);

    valuestr = ogs_strndup((const char *)value, valuelen);
    ogs_assert(valuestr);

    ogs_error("[%s]:%d invalid header (%s:%s)",
            OGS_ADDR(addr, buf), OGS_PORT(addr), namestr, valuestr);

    ogs_free(namestr);
    ogs_free(valuestr);

    return 0;
}

static int on_begin_frame(nghttp2_session *session, const nghttp2_frame_hd *hd,
                          void *user_data)
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sockaddr_t *addr = NULL;
    ogs_sbi_session_t *sbi_sess = user_data;

    ogs_assert(sbi_sess);
    addr = sbi_sess->addr;
    ogs_assert(addr);

    ogs_assert(hd);

    if ((hd->type == NGHTTP2_HEADERS) &&
        (hd->stream_id < sbi_sess->last_stream_id)) {
        ogs_error("[%s]:%d invalid stream id(%d) >= last stream id(%d)",
                OGS_ADDR(addr, buf), OGS_PORT(addr),
                hd->stream_id, sbi_sess->last_stream_id);
        return NGHTTP2_ERR_CALLBACK_FAILURE;
    }

    return 0;
}

static int on_begin_headers(nghttp2_session *session,
                            const nghttp2_frame *frame, void *user_data)
{
    ogs_sbi_session_t *sbi_sess = user_data;
    ogs_sbi_stream_t *stream = NULL;

    ogs_assert(sbi_sess);
    ogs_assert(session);
    ogs_assert(frame);

    if (frame->hd.type != NGHTTP2_HEADERS ||
        frame->headers.cat != NGHTTP2_HCAT_REQUEST) {
        return 0;
    }

    stream = stream_add(sbi_sess, frame->hd.stream_id);
    ogs_assert(stream);
    ogs_debug("STREAM added [%d]", frame->hd.stream_id);

    nghttp2_session_set_stream_user_data(session, frame->hd.stream_id, stream);

    return 0;
}

static int session_send_preface(ogs_sbi_session_t *sbi_sess)
{
    int rv;
    nghttp2_settings_entry iv[1] = {
        { NGHTTP2_SETTINGS_MAX_CONCURRENT_STREAMS, 100 }
    };

    ogs_assert(sbi_sess);
    ogs_assert(sbi_sess->session);

    rv = nghttp2_submit_settings(
            sbi_sess->session, NGHTTP2_FLAG_NONE, iv, OGS_ARRAY_SIZE(iv));
    if (rv != 0) {
        ogs_error("nghttp2_submit_settings() failed (%d:%s)",
                    rv, nghttp2_strerror(rv));
        return OGS_ERROR;
    }

    return session_send(sbi_sess);
}

#if USE_SEND_DATA_WITH_NO_COPY
static int on_send_data(nghttp2_session *session, nghttp2_frame *frame,
                        const uint8_t *framehd, size_t length,
                        nghttp2_data_source *source, void *user_data)
{
    ogs_sbi_session_t *sbi_sess = user_data;

    ogs_sbi_response_t *response = NULL;
    ogs_sbi_stream_t *stream = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    size_t padlen = 0;

    ogs_assert(session);
    ogs_assert(frame);

    stream = nghttp2_session_get_stream_user_data(session, frame->hd.stream_id);
    if (!stream) {
        ogs_error("no stream [%d]", frame->hd.stream_id);
        return NGHTTP2_ERR_TEMPORAL_CALLBACK_FAILURE;
    }

    ogs_assert(sbi_sess);

    ogs_assert(source);
    response = source->ptr;
    ogs_assert(response);

    ogs_assert(response->http.content);
    ogs_assert(response->http.content_length);

    ogs_assert(framehd);
    ogs_assert(length);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf, framehd, 9);

    padlen = frame->data.padlen;

    if (padlen > 0) {
        ogs_pkbuf_put_u8(pkbuf, padlen-1);
    }

    ogs_pkbuf_put_data(pkbuf,
            response->http.content, response->http.content_length);

    if (padlen > 0) {
        memset(pkbuf->tail, 0, padlen-1);
        ogs_pkbuf_put(pkbuf, padlen-1);
    }

    session_write_to_buffer(sbi_sess, pkbuf);

    return 0;
}
#else
static ssize_t send_callback(nghttp2_session *session, const uint8_t *data,
                             size_t length, int flags, void *user_data)
{
    ogs_sbi_session_t *sbi_sess = user_data;
    ogs_sock_t *sock = NULL;
    ogs_socket_t fd = INVALID_SOCKET;

    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sbi_sess);
    sock = sbi_sess->sock;
    ogs_assert(sock);
    fd = sock->fd;
    ogs_assert(fd != INVALID_SOCKET);

    ogs_assert(data);
    ogs_assert(length);

    pkbuf = ogs_pkbuf_alloc(NULL, length);
    ogs_assert(pkbuf);
    ogs_pkbuf_put_data(pkbuf, data, length);

    session_write_to_buffer(sbi_sess, pkbuf);

    return length;
}
#endif

static int session_send(ogs_sbi_session_t *sbi_sess)
{
#if USE_SEND_DATA_WITH_NO_COPY
    ogs_pkbuf_t *pkbuf = NULL;
#else
    int rv;
#endif

    ogs_assert(sbi_sess);
    ogs_assert(sbi_sess->session);

#if USE_SEND_DATA_WITH_NO_COPY
    for (;;) {
        const uint8_t *data = NULL;
        ssize_t data_len;

        data_len = nghttp2_session_mem_send(sbi_sess->session, &data);
        if (data_len < 0) {
            ogs_error("nghttp2_session_mem_send() failed (%d:%s)",
                        (int)data_len, nghttp2_strerror((int)data_len));
            return OGS_ERROR;
        }

        if (data_len == 0) {
            break;
        }

        pkbuf = ogs_pkbuf_alloc(NULL, data_len);
        ogs_assert(pkbuf);
        ogs_pkbuf_put_data(pkbuf, data, data_len);

        session_write_to_buffer(sbi_sess, pkbuf);
    }
#else
    rv = nghttp2_session_send(sbi_sess->session);
    if (rv != 0) {
        ogs_error("nghttp_session_send() failed (%d:%s)",
                    rv, nghttp2_strerror(rv));
        return OGS_ERROR;
    }
#endif

    return OGS_OK;
}

static void session_write_callback(short when, ogs_socket_t fd, void *data)
{
    ogs_sbi_session_t *sbi_sess = data;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_assert(sbi_sess);

    if (ogs_list_empty(&sbi_sess->write_queue) == true) {
        ogs_assert(sbi_sess->poll.write);
        ogs_pollset_remove(sbi_sess->poll.write);
        sbi_sess->poll.write = NULL;
        return;
    }

    pkbuf = ogs_list_first(&sbi_sess->write_queue);
    ogs_assert(pkbuf);
    ogs_list_remove(&sbi_sess->write_queue, pkbuf);

    ogs_send(fd, pkbuf->data, pkbuf->len, 0);
    ogs_log_hexdump(OGS_LOG_DEBUG, pkbuf->data, pkbuf->len);

    ogs_pkbuf_free(pkbuf);
}

static void session_write_to_buffer(
        ogs_sbi_session_t *sbi_sess, ogs_pkbuf_t *pkbuf)
{
    ogs_sock_t *sock = NULL;
    ogs_socket_t fd = INVALID_SOCKET;

    ogs_assert(pkbuf);

    ogs_assert(sbi_sess);
    sock = sbi_sess->sock;
    ogs_assert(sock);
    fd = sock->fd;
    ogs_assert(fd != INVALID_SOCKET);

    ogs_list_add(&sbi_sess->write_queue, pkbuf);

    if (!sbi_sess->poll.write) {
        sbi_sess->poll.write = ogs_pollset_add(ogs_app()->pollset,
            OGS_POLLOUT, fd, session_write_callback, sbi_sess);
        ogs_assert(sbi_sess->poll.write);
    }
}
