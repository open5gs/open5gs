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

#include "microhttpd.h"

#if MHD_VERSION >= 0x00097001
typedef enum MHD_Result _MHD_Result;
#else
typedef int _MHD_Result;
#endif

static void server_init(int num_of_session_pool);
static void server_final(void);

static void server_start(ogs_sbi_server_t *server,
        int (*cb)(ogs_sbi_request_t *request, void *data));
static void server_stop(ogs_sbi_server_t *server);

static void server_send_response(
        ogs_sbi_stream_t *stream, ogs_sbi_response_t *response);

static ogs_sbi_server_t *server_from_stream(ogs_sbi_stream_t *stream);

const ogs_sbi_server_actions_t ogs_mhd_server_actions = {
    server_init,
    server_final,

    server_start,
    server_stop,

    server_send_response,
    server_from_stream,
};

static void run(short when, ogs_socket_t fd, void *data);
static void notify_connection(void *cls,
        struct MHD_Connection *connection,
        void **socket_context,
        enum MHD_ConnectionNotificationCode toe);
static _MHD_Result access_handler(
        void *cls,
        struct MHD_Connection *connection,
        const char *url,
        const char *method,
        const char *version,
        const char *upload_data,
        size_t *upload_data_size,
        void **con_cls);
static void notify_completed(
        void *cls,
        struct MHD_Connection *connection,
        void **con_cls,
        enum MHD_RequestTerminationCode toe);

static void session_timer_expired(void *data);

typedef struct ogs_sbi_session_s {
    ogs_lnode_t             lnode;

    struct MHD_Connection   *connection;

    ogs_sbi_request_t       *request;
    ogs_sbi_server_t        *server;

    /*
     * If the HTTP client closes the socket without sending an HTTP response,
     * there is no way to detect the socket disconnection after MHD_run().
     *
     * In this case, the poll instance cannot be removed, so the CPU load
     * of the program using MHD is 100%. POLLIN (POLLRDHUP) is still created,
     * poll's callback function is still called.
     *
     * To solve this problem, we've created the timer to check
     * whether the user does not use the HTTP response. When the timer expires,
     * an assertion occurs and terminates the program.
     *
     * To avoid the above timer expiration, the user of HTTP server(MHD)
     * should send an HTTP response if an HTTP client(CURL) is requested.
     */
    ogs_timer_t             *timer;

    void *data;
} ogs_sbi_session_t;

static OGS_POOL(session_pool, ogs_sbi_session_t);

static void server_init(int num_of_session_pool)
{
    ogs_pool_init(&session_pool, num_of_session_pool);
}

static void server_final(void)
{
    ogs_pool_final(&session_pool);
}

static ogs_sbi_session_t *session_add(ogs_sbi_server_t *server,
        ogs_sbi_request_t *request, struct MHD_Connection *connection)
{
    ogs_sbi_session_t *sbi_sess = NULL;

    ogs_assert(server);
    ogs_assert(request);
    ogs_assert(connection);

    ogs_pool_alloc(&session_pool, &sbi_sess);
    ogs_assert(sbi_sess);
    memset(sbi_sess, 0, sizeof(ogs_sbi_session_t));

    sbi_sess->server = server;
    sbi_sess->request = request;
    sbi_sess->connection = connection;

    sbi_sess->timer = ogs_timer_add(
            ogs_app()->timer_mgr, session_timer_expired, sbi_sess);
    ogs_assert(sbi_sess->timer);

    /* If User does not send HTTP response within deadline,
     * Open5GS will assert this program. */
    ogs_timer_start(sbi_sess->timer,
            ogs_app()->time.message.sbi.connection_deadline);

    ogs_list_add(&server->session_list, sbi_sess);

    return sbi_sess;
}

static void session_remove(ogs_sbi_session_t *sbi_sess)
{
    struct MHD_Connection *connection;
    ogs_sbi_server_t *server = NULL;

    ogs_assert(sbi_sess);
    server = sbi_sess->server;
    ogs_assert(server);

    ogs_list_remove(&server->session_list, sbi_sess);

    ogs_assert(sbi_sess->timer);
    ogs_timer_delete(sbi_sess->timer);

    connection = sbi_sess->connection;
    ogs_assert(connection);

    MHD_resume_connection(connection);

    ogs_pool_free(&session_pool, sbi_sess);
}

static void session_timer_expired(void *data)
{
    ogs_sbi_session_t *sbi_sess = data;

    ogs_assert(sbi_sess);

    ogs_fatal("An HTTP request was received, "
                "but the HTTP response is missing.");
    ogs_fatal("Please send the related pcap files for this case.");

    session_remove(sbi_sess);

    ogs_assert_if_reached();
}

static void session_remove_all(ogs_sbi_server_t *server)
{
    ogs_sbi_session_t *sbi_sess = NULL, *next_sbi_sess = NULL;

    ogs_assert(server);

    ogs_list_for_each_safe(&server->session_list, next_sbi_sess, sbi_sess)
        session_remove(sbi_sess);
}

static void server_start(ogs_sbi_server_t *server,
        int (*cb)(ogs_sbi_request_t *request, void *data))
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sockaddr_t *addr = NULL;
    char *hostname = NULL;

#if MHD_VERSION >= 0x00095300
    unsigned int mhd_flags = MHD_USE_ERROR_LOG;
#else
    unsigned int mhd_flags = MHD_USE_DEBUG;
#endif
    const union MHD_DaemonInfo *mhd_info = NULL;
#define MAX_NUM_OF_MHD_OPTION_ITEM 8
    struct MHD_OptionItem mhd_ops[MAX_NUM_OF_MHD_OPTION_ITEM];
    int index = 0;

    ogs_assert(server);

#if MHD_VERSION >= 0x00095300
    mhd_flags |= MHD_ALLOW_SUSPEND_RESUME;
#elif MHD_VERSION >= 0x00093400
    mhd_flags |= MHD_USE_SUSPEND_RESUME;
#else
    mhd_flags |= MHD_USE_PIPE_FOR_SHUTDOWN;
#endif

    /* Setup callback function */
    server->cb = cb;

    mhd_ops[index].option = MHD_OPTION_NOTIFY_COMPLETED;
    mhd_ops[index].value = (intptr_t)notify_completed;
    mhd_ops[index].ptr_value = server;
    index++;

    mhd_ops[index].option = MHD_OPTION_NOTIFY_CONNECTION;
    mhd_ops[index].value = (intptr_t)&notify_connection;
    mhd_ops[index].ptr_value = NULL;
    index++;

    addr = server->node.addr;
    ogs_assert(addr);
    if (addr->ogs_sa_family == AF_INET6)
        mhd_flags |= MHD_USE_IPv6;
    mhd_ops[index].option = MHD_OPTION_SOCK_ADDR;
    mhd_ops[index].value = 0;
    mhd_ops[index].ptr_value = (void *)&addr->sa;
    index++;

    mhd_ops[index].option = MHD_OPTION_END;
    mhd_ops[index].value = 0;
    mhd_ops[index].ptr_value = NULL;
    index++;

    server->mhd = MHD_start_daemon(
                mhd_flags,
                0,
                NULL, NULL,
                access_handler, server,
                MHD_OPTION_ARRAY, mhd_ops,
                MHD_OPTION_END);
    if (!server->mhd) {
        ogs_error("Cannot start SBI server");
        return;
    }

    /* Setup poll for server listening socket */
    mhd_info = MHD_get_daemon_info(server->mhd, MHD_DAEMON_INFO_LISTEN_FD);
    ogs_assert(mhd_info);

    server->node.poll = ogs_pollset_add(ogs_app()->pollset,
            OGS_POLLIN, mhd_info->listen_fd, run, server->mhd);
    ogs_assert(server->node.poll);

    hostname = ogs_gethostname(addr);
    if (hostname)
        ogs_info("mhd_server() [%s]:%d", hostname, OGS_PORT(addr));
    else
        ogs_info("mhd_server() [%s]:%d", OGS_ADDR(addr, buf), OGS_PORT(addr));
}

static void server_stop(ogs_sbi_server_t *server)
{
    ogs_assert(server);

    if (server->node.poll)
        ogs_pollset_remove(server->node.poll);

    session_remove_all(server);

    if (server->mhd) {
        MHD_stop_daemon(server->mhd);
        server->mhd = NULL;
    }
}

static void server_send_response(
        ogs_sbi_stream_t *stream, ogs_sbi_response_t *response)
{
    int ret;
    int status;

    struct MHD_Connection *connection = NULL;
    struct MHD_Response *mhd_response;
    struct MHD_Daemon *mhd_daemon = NULL;
    const union MHD_ConnectionInfo *mhd_info = NULL;
    MHD_socket mhd_socket = INVALID_SOCKET;

    ogs_hash_index_t *hi;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_session_t *sbi_sess = NULL;

    sbi_sess = (ogs_sbi_session_t *)stream;
    ogs_assert(sbi_sess);
    ogs_assert(response);

    connection = sbi_sess->connection;
    ogs_assert(connection);

    mhd_info = MHD_get_connection_info(
            connection, MHD_CONNECTION_INFO_DAEMON);
    ogs_assert(mhd_info);
    mhd_daemon = mhd_info->daemon;
    ogs_assert(mhd_daemon);

    mhd_info = MHD_get_connection_info(
            connection, MHD_CONNECTION_INFO_CONNECTION_FD);
    ogs_assert(mhd_info);
    mhd_socket = mhd_info->connect_fd;
    ogs_assert(mhd_socket != INVALID_SOCKET);

    if (response->http.content) {
        mhd_response = MHD_create_response_from_buffer(
                response->http.content_length, response->http.content,
                MHD_RESPMEM_PERSISTENT);
        ogs_assert(mhd_response);
    } else {
        mhd_response = MHD_create_response_from_buffer(
                0, NULL, MHD_RESPMEM_PERSISTENT);
        ogs_assert(mhd_response);
    }

    for (hi = ogs_hash_first(response->http.headers);
            hi; hi = ogs_hash_next(hi)) {
        const char *key = ogs_hash_this_key(hi);
        char *val = ogs_hash_this_val(hi);
        MHD_add_response_header(mhd_response, key, val);
    }

    status = response->status;
    request = sbi_sess->request;
    ogs_assert(request);

    ogs_sbi_response_free(response);
    session_remove(sbi_sess);

    request->poll.write = ogs_pollset_add(ogs_app()->pollset,
                    OGS_POLLOUT, mhd_socket, run, mhd_daemon);
    ogs_assert(request->poll.write);

    ret = MHD_queue_response(connection, status, mhd_response);
    if (ret != MHD_YES) {
        ogs_fatal("MHD_queue_response_error [%d]", ret);
        ogs_assert_if_reached();
    }
    MHD_destroy_response(mhd_response);
}

static void run(short when, ogs_socket_t fd, void *data)
{
    struct MHD_Daemon *mhd_daemon = data;

    ogs_assert(mhd_daemon);
    MHD_run(mhd_daemon);
}

static void notify_connection(void *cls,
        struct MHD_Connection *connection,
        void **socket_context,
        enum MHD_ConnectionNotificationCode toe)
{
    struct MHD_Daemon *mhd_daemon = NULL;
    MHD_socket mhd_socket = INVALID_SOCKET;

    const union MHD_ConnectionInfo *mhd_info = NULL;
    struct {
        ogs_poll_t *read;
    } poll;

    switch (toe) {
        case MHD_CONNECTION_NOTIFY_STARTED:
            mhd_info = MHD_get_connection_info(
                    connection, MHD_CONNECTION_INFO_DAEMON);
            ogs_assert(mhd_info);
            mhd_daemon = mhd_info->daemon;
            ogs_assert(mhd_daemon);

            mhd_info = MHD_get_connection_info(
                    connection, MHD_CONNECTION_INFO_CONNECTION_FD);
            ogs_assert(mhd_info);
            mhd_socket = mhd_info->connect_fd;
            ogs_assert(mhd_socket != INVALID_SOCKET);

            poll.read = ogs_pollset_add(ogs_app()->pollset,
                    OGS_POLLIN, mhd_socket, run, mhd_daemon);
            ogs_assert(poll.read);
            *socket_context = poll.read;
            break;
        case MHD_CONNECTION_NOTIFY_CLOSED:
            poll.read = ogs_pollset_cycle(ogs_app()->pollset, *socket_context);
            if (poll.read)
                ogs_pollset_remove(poll.read);
            break;
    }
}

static int get_values(ogs_hash_t *hash,
        enum MHD_ValueKind kind, const char *key, const char *value)
{
    ogs_assert(hash);

    if (!key || !value)
        return MHD_YES;     //  Ignore connection value if invalid!

    ogs_sbi_header_set(hash, key, value);

    return MHD_YES;
}

static _MHD_Result access_handler(
        void *cls,
        struct MHD_Connection *connection,
        const char *url,
        const char *method,
        const char *version,
        const char *upload_data,
        size_t *upload_data_size,
        void **con_cls)
{
    ogs_sbi_server_t *server = NULL;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_session_t *sbi_sess = NULL;

    server = cls;
    ogs_assert(server);

    request = *con_cls;

    if (request && request->suspended) {
        ogs_error("Suspended Request");
        return MHD_YES;
    }

    if (!request) {
        request = ogs_sbi_request_new();
        ogs_assert(request);
        *con_cls = request;

        ogs_assert(request->http.params);
        MHD_get_connection_values(connection, MHD_GET_ARGUMENT_KIND,
                (MHD_KeyValueIterator)get_values, request->http.params);

        ogs_assert(request->http.headers);
        MHD_get_connection_values(connection, MHD_HEADER_KIND,
                (MHD_KeyValueIterator)get_values, request->http.headers);

        request->h.method = ogs_strdup(method);
        request->h.uri = ogs_strdup(url);

        if (ogs_sbi_header_get(request->http.headers, "Content-Length") ||
            ogs_sbi_header_get(request->http.headers, "Transfer-Encoding")) {

            // FIXME : check if POST_DATA is on MHD_POSTDATA_KIND

            return MHD_YES;
        }

        goto suspend;
    }

    if (*upload_data_size != 0) {
        size_t offset = 0;

        if (request->http.content == NULL) {
            request->http.content_length = *upload_data_size;
            request->http.content =
                (char*)ogs_malloc(request->http.content_length + 1);
            ogs_assert(request->http.content);
        } else {
            offset = request->http.content_length;
            if ((request->http.content_length +
                        *upload_data_size) > OGS_HUGE_LEN) {
                ogs_error("Overflow : Content-Length[%d], upload_data_size[%d]",
                            (int)request->http.content_length,
                            (int)*upload_data_size);
                *upload_data_size = 0;
                return MHD_YES;
            }
            request->http.content_length += *upload_data_size;
            request->http.content = (char *)ogs_realloc(
                    request->http.content, request->http.content_length + 1);
            ogs_assert(request->http.content);
        }

        memcpy(request->http.content + offset, upload_data, *upload_data_size);
        request->http.content[request->http.content_length] = '\0';
        *upload_data_size = 0;

        return MHD_YES;
    }

suspend:
    MHD_suspend_connection(connection);
    request->suspended = true;

    sbi_sess = session_add(server, request, connection);
    ogs_assert(sbi_sess);

    if (server->cb) {
        if (server->cb(request, sbi_sess) != OGS_OK) {
            ogs_warn("server callback error");
            ogs_sbi_server_send_error((ogs_sbi_stream_t *)sbi_sess,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                    "server callback error", NULL);

            return MHD_YES;
        }
    } else {
        ogs_fatal("server callback is not registered");
        ogs_assert_if_reached();
    }

    return MHD_YES;
}

static void notify_completed(
        void *cls,
        struct MHD_Connection *connection,
        void **con_cls,
        enum MHD_RequestTerminationCode toe)
{
    ogs_sbi_request_t *request = *con_cls;
    ogs_poll_t *poll = NULL;

    ogs_assert(request);
    poll = ogs_pollset_cycle(ogs_app()->pollset, request->poll.write);
    if (poll)
        ogs_pollset_remove(poll);

    ogs_sbi_request_free(request);
}

static ogs_sbi_server_t *server_from_stream(ogs_sbi_stream_t *stream)
{
    ogs_sbi_session_t *sbi_sess = (ogs_sbi_session_t *)stream;

    ogs_assert(sbi_sess);
    ogs_assert(sbi_sess->server);

    return sbi_sess->server;
}
