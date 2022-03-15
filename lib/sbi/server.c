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

extern const ogs_sbi_server_actions_t ogs_mhd_server_actions;
extern const ogs_sbi_server_actions_t ogs_nghttp2_server_actions;

ogs_sbi_server_actions_t ogs_sbi_server_actions;
bool ogs_sbi_server_actions_initialized = false;

static OGS_POOL(server_pool, ogs_sbi_server_t);

void ogs_sbi_server_init(int num_of_session_pool, int num_of_stream_pool)
{
    if (ogs_sbi_server_actions_initialized == false) {
#if 1 /* Use HTTP2 */
        ogs_sbi_server_actions = ogs_nghttp2_server_actions;
#else
        ogs_sbi_server_actions = ogs_mhd_server_actions;
#endif
    }

    ogs_sbi_server_actions.init(num_of_session_pool, num_of_stream_pool);

    ogs_list_init(&ogs_sbi_self()->server_list);
    ogs_pool_init(&server_pool, ogs_app()->pool.nf);
}

void ogs_sbi_server_final(void)
{
    ogs_sbi_server_remove_all();

    ogs_pool_final(&server_pool);

    ogs_sbi_server_actions.cleanup();
}

ogs_sbi_server_t *ogs_sbi_server_add(
        ogs_sockaddr_t *addr, ogs_sockopt_t *option)
{
    ogs_sbi_server_t *server = NULL;

    ogs_assert(addr);

    ogs_pool_alloc(&server_pool, &server);
    ogs_assert(server);
    memset(server, 0, sizeof(ogs_sbi_server_t));

    ogs_assert(OGS_OK == ogs_copyaddrinfo(&server->node.addr, addr));
    if (option)
        server->node.option = ogs_memdup(option, sizeof *option);

    ogs_list_add(&ogs_sbi_self()->server_list, server);

    return server;
}

void ogs_sbi_server_remove(ogs_sbi_server_t *server)
{
    ogs_assert(server);

    ogs_list_remove(&ogs_sbi_self()->server_list, server);

    ogs_assert(server->node.addr);
    ogs_freeaddrinfo(server->node.addr);
    if (server->node.option)
        ogs_free(server->node.option);
    if (server->advertise)
        ogs_freeaddrinfo(server->advertise);

    ogs_pool_free(&server_pool, server);
}

void ogs_sbi_server_remove_all(void)
{
    ogs_sbi_server_t *server = NULL, *next_server = NULL;

    ogs_list_for_each_safe(&ogs_sbi_self()->server_list, next_server, server)
        ogs_sbi_server_remove(server);
}

void ogs_sbi_server_set_advertise(
        ogs_sbi_server_t *server, int family, ogs_sockaddr_t *advertise)
{
    ogs_sockaddr_t *addr = NULL;

    ogs_assert(server);
    ogs_assert(advertise);

    ogs_assert(OGS_OK == ogs_copyaddrinfo(&addr, advertise));
    if (family != AF_UNSPEC)
        ogs_filteraddrinfo(&addr, family);

    if (addr)
        server->advertise = addr;
}

int ogs_sbi_server_start_all(
        int (*cb)(ogs_sbi_request_t *request, void *data))
{
    ogs_sbi_server_t *server = NULL, *next_server = NULL;

    ogs_list_for_each_safe(&ogs_sbi_self()->server_list, next_server, server)
        if (ogs_sbi_server_actions.start(server, cb) != OGS_OK)
            return OGS_ERROR;

    return OGS_OK;
}

void ogs_sbi_server_stop_all(void)
{
    ogs_sbi_server_t *server = NULL, *next_server = NULL;

    ogs_list_for_each_safe(&ogs_sbi_self()->server_list, next_server, server)
        ogs_sbi_server_actions.stop(server);
}

bool ogs_sbi_server_send_response(
        ogs_sbi_stream_t *stream, ogs_sbi_response_t *response)
{
    return ogs_sbi_server_actions.send_response(stream, response);
}

bool ogs_sbi_server_send_problem(
        ogs_sbi_stream_t *stream, OpenAPI_problem_details_t *problem)
{
    ogs_sbi_message_t message;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(stream);
    ogs_assert(problem);

    memset(&message, 0, sizeof(message));

    message.http.content_type = (char*)"application/problem+json";
    message.ProblemDetails = problem;

    response = ogs_sbi_build_response(&message, problem->status);
    ogs_assert(response);

    ogs_sbi_server_send_response(stream, response);

    return true;
}

bool ogs_sbi_server_send_error(ogs_sbi_stream_t *stream,
        int status, ogs_sbi_message_t *message,
        const char *title, const char *detail)
{
    OpenAPI_problem_details_t problem;

    ogs_assert(stream);

    memset(&problem, 0, sizeof(problem));

    if (message) {
        problem.type = ogs_msprintf("/%s/%s",
                message->h.service.name, message->h.api.version);
        ogs_expect_or_return_val(problem.type, false);
        if (message->h.resource.component[1])
            problem.instance = ogs_msprintf("/%s/%s",
                    message->h.resource.component[0],
                    message->h.resource.component[1]);
        else
            problem.instance =
                    ogs_msprintf("/%s", message->h.resource.component[0]);
        ogs_expect_or_return_val(problem.instance, NULL);
    }
    if (status) {
        problem.is_status = true;
        problem.status = status;
    }
    problem.title = (char*)title;
    problem.detail = (char*)detail;

    ogs_sbi_server_send_problem(stream, &problem);

    if (problem.type)
        ogs_free(problem.type);
    if (problem.instance)
        ogs_free(problem.instance);

    return true;
}

ogs_sbi_server_t *ogs_sbi_server_from_stream(ogs_sbi_stream_t *stream)
{
    return ogs_sbi_server_actions.from_stream(stream);
}
