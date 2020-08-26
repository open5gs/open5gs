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

typedef struct ogs_sbi_session_s {
    ogs_lnode_t             lnode;

    struct MHD_Connection   *connection;

    ogs_sbi_request_t       *request;
    ogs_sbi_server_t        *server;

    /*
     * The HTTP server(MHD) should send an HTTP response
     * if an HTTP client(CURL) is requested.
     *
     * If the HTTP client closes the socket without sending an HTTP response,
     * the CPU load of a program using MHD is 100%. This is because
     * POLLIN(POLLRDHUP) is generated. So, the callback function of poll
     * continues to be called.
     *
     * I've created the timer to check whether the user does not use
     * the HTTP response. When the timer expires, an assertion occurs and
     * terminates the program.
     */
    ogs_timer_t             *timer;

    void *data;
} ogs_sbi_session_t;

static OGS_POOL(server_pool, ogs_sbi_server_t);
static OGS_POOL(session_pool, ogs_sbi_session_t);

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

void ogs_sbi_server_init(int num_of_session_pool)
{
    ogs_list_init(&ogs_sbi_self()->server_list);
    ogs_pool_init(&server_pool, ogs_app()->pool.nf);

    ogs_pool_init(&session_pool, num_of_session_pool);
}
void ogs_sbi_server_final(void)
{
    ogs_sbi_server_remove_all();

    ogs_pool_final(&server_pool);
    ogs_pool_final(&session_pool);
}

static ogs_sbi_session_t *session_add(ogs_sbi_server_t *server,
        ogs_sbi_request_t *request, struct MHD_Connection *connection)
{
    ogs_sbi_session_t *session = NULL;

    ogs_assert(server);
    ogs_assert(request);
    ogs_assert(connection);

    ogs_pool_alloc(&session_pool, &session);
    ogs_assert(session);
    memset(session, 0, sizeof(ogs_sbi_session_t));

    session->server = server;
    session->request = request;
    session->connection = connection;

    session->timer = ogs_timer_add(
            ogs_app()->timer_mgr, session_timer_expired, session);
    ogs_assert(session->timer);

    /* If User does not send http response within deadline,
     * Open5GS will assert this program. */
    ogs_timer_start(session->timer,
            ogs_app()->time.message.sbi.connection_deadline);

    ogs_list_add(&server->suspended_session_list, session);

    return session;
}

static void session_remove(ogs_sbi_session_t *session)
{
    struct MHD_Connection *connection;
    ogs_sbi_server_t *server = NULL;

    ogs_assert(session);
    server = session->server;
    ogs_assert(server);

    ogs_list_remove(&server->suspended_session_list, session);

    ogs_assert(session->timer);
    ogs_timer_delete(session->timer);

    connection = session->connection;
    ogs_assert(connection);

    MHD_resume_connection(connection);

    ogs_pool_free(&session_pool, session);
}

static void session_timer_expired(void *data)
{
    ogs_sbi_session_t *session = NULL;

    session = data;
    ogs_assert(session);

    ogs_fatal("An HTTP request was received, "
                "but the HTTP response is missing.");
    ogs_fatal("Please send the related pcap files for this case.");

    session_remove(session);

    ogs_assert_if_reached();
}

static void session_remove_all(ogs_sbi_server_t *server)
{
    ogs_sbi_session_t *session = NULL, *next_session = NULL;

    ogs_assert(server);

    ogs_list_for_each_safe(
            &server->suspended_session_list, next_session, session)
        session_remove(session);
}

ogs_sbi_server_t *ogs_sbi_server_add(ogs_sockaddr_t *addr)
{
    ogs_sbi_server_t *server = NULL;

    ogs_assert(addr);

    ogs_pool_alloc(&server_pool, &server);
    ogs_assert(server);
    memset(server, 0, sizeof(ogs_sbi_server_t));

    ogs_list_init(&server->suspended_session_list);
    ogs_copyaddrinfo(&server->addr, addr);

    ogs_list_add(&ogs_sbi_self()->server_list, server);

    return server;
}

void ogs_sbi_server_remove(ogs_sbi_server_t *server)
{
    ogs_assert(server);

    ogs_list_remove(&ogs_sbi_self()->server_list, server);

    ogs_sbi_server_stop(server);

    ogs_assert(server->addr);
    ogs_freeaddrinfo(server->addr);

    ogs_pool_free(&server_pool, server);
}

void ogs_sbi_server_remove_all(void)
{
    ogs_sbi_server_t *server = NULL, *next_server = NULL;

    ogs_list_for_each_safe(&ogs_sbi_self()->server_list, next_server, server)
        ogs_sbi_server_remove(server);
}

void ogs_sbi_server_start(ogs_sbi_server_t *server, int (*cb)(
            ogs_sbi_server_t *server, ogs_sbi_session_t *session,
            ogs_sbi_request_t *request))
{
    char buf[OGS_ADDRSTRLEN];
    ogs_sockaddr_t *addr = NULL;

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

    addr = server->addr;
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

    server->poll = ogs_pollset_add(ogs_app()->pollset,
            OGS_POLLIN, mhd_info->listen_fd, run, server->mhd);
    ogs_assert(server->poll);

    if (addr) {
        char *hostname = ogs_gethostname(addr);
        if (hostname)
            ogs_info("sbi_server() [%s]:%d",
                    hostname, OGS_PORT(addr));
        else
            ogs_info("sbi_server() [%s]:%d",
                    OGS_ADDR(addr, buf), OGS_PORT(addr));
    } else
        ogs_info("sbi_server() [any]:any");
}

void ogs_sbi_server_start_all(int (*cb)(
            ogs_sbi_server_t *server, ogs_sbi_session_t *session,
            ogs_sbi_request_t *request))
{
    ogs_sbi_server_t *server = NULL, *next_server = NULL;

    ogs_list_for_each_safe(&ogs_sbi_self()->server_list, next_server, server)
        ogs_sbi_server_start(server, cb);
}

void ogs_sbi_server_stop(ogs_sbi_server_t *server)
{
    ogs_assert(server);

    if (server->poll) {
        ogs_pollset_remove(server->poll);
        server->poll = NULL;
    }

    session_remove_all(server);

    if (server->mhd) {
        MHD_stop_daemon(server->mhd);
        server->mhd = NULL;
    }
}

void ogs_sbi_server_stop_all(void)
{
    ogs_sbi_server_t *server = NULL, *next_server = NULL;

    ogs_list_for_each_safe(&ogs_sbi_self()->server_list, next_server, server)
        ogs_sbi_server_stop(server);
}

void ogs_sbi_server_send_response(
        ogs_sbi_session_t *session, ogs_sbi_response_t *response)
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

    ogs_assert(response);

    ogs_assert(session);
    connection = session->connection;
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
    request = session->request;
    ogs_assert(request);

    ogs_sbi_response_free(response);
    session_remove(session);

    request->poll = ogs_pollset_add(ogs_app()->pollset,
                    OGS_POLLOUT, mhd_socket, run, mhd_daemon);
    ogs_assert(request->poll);

    ret = MHD_queue_response(connection, status, mhd_response);
    if (ret != MHD_YES) {
        ogs_fatal("MHD_queue_response_error [%d]", ret);
        ogs_assert_if_reached();
    }
    MHD_destroy_response(mhd_response);
}

void ogs_sbi_server_send_problem(
        ogs_sbi_session_t *session, OpenAPI_problem_details_t *problem)
{
    ogs_sbi_message_t message;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(session);
    ogs_assert(problem);

    memset(&message, 0, sizeof(message));

    message.http.content_type = (char*)"application/problem+json";
    message.ProblemDetails = problem;

    response = ogs_sbi_build_response(&message, problem->status);
    ogs_assert(response);

    ogs_sbi_server_send_response(session, response);
}

void ogs_sbi_server_send_error(ogs_sbi_session_t *session,
        int status, ogs_sbi_message_t *message,
        const char *title, const char *detail)
{
    OpenAPI_problem_details_t problem;

    ogs_assert(session);

    memset(&problem, 0, sizeof(problem));

    if (message) {
        problem.type = ogs_msprintf("/%s/%s",
                message->h.service.name, message->h.api.version);
        if (message->h.resource.component[1])
            problem.instance = ogs_msprintf("/%s/%s",
                    message->h.resource.component[0],
                    message->h.resource.component[1]);
        else
            problem.instance =
                    ogs_msprintf("/%s", message->h.resource.component[0]);
    }
    problem.status = status;
    problem.title = (char*)title;
    problem.detail = (char*)detail;

    ogs_sbi_server_send_problem(session, &problem);

    if (problem.type)
        ogs_free(problem.type);
    if (problem.instance)
        ogs_free(problem.instance);
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
    ogs_poll_t *poll = NULL;

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

            poll = ogs_pollset_add(ogs_app()->pollset,
                    OGS_POLLIN, mhd_socket, run, mhd_daemon);
            ogs_assert(poll);
            *socket_context = poll;
            break;
        case MHD_CONNECTION_NOTIFY_CLOSED:
            poll = *socket_context;
            ogs_pollset_remove(poll);
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
    ogs_sbi_session_t *session = NULL;

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

    session = session_add(server, request, connection);
    ogs_assert(session);

    if (server->cb) {
        if (server->cb(server, session, request) != OGS_OK) {
            ogs_warn("server callback error");
            ogs_sbi_server_send_error(session,
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
    poll = request->poll;
    if (poll)
        ogs_pollset_remove(poll);

    ogs_sbi_request_free(request);
}

void ogs_sbi_session_set_data(ogs_sbi_session_t *session, void *data)
{
    ogs_assert(session);
    ogs_assert(data);

    session->data = data;
}

void *ogs_sbi_session_get_data(ogs_sbi_session_t *session)
{
    ogs_assert(session);

    return session->data;
}

ogs_sbi_server_t *ogs_sbi_session_get_server(ogs_sbi_session_t *session)
{
    ogs_assert(session);
    ogs_assert(session->server);

    return session->server;
}
