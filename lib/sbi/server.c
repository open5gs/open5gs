/*
 * Copyright (C) 2019,2024 by Sukchan Lee <acetcom@gmail.com>
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
        const char *interface,
        OpenAPI_uri_scheme_e scheme, ogs_sockaddr_t *addr,
        ogs_sockopt_t *option)
{
    ogs_sbi_server_t *server = NULL;

    ogs_assert(addr);
    ogs_assert(scheme);

    ogs_pool_id_calloc(&server_pool, &server);
    ogs_assert(server);

    if (interface)
        server->interface = ogs_strdup(interface);

    server->scheme = scheme;

    if (ogs_sbi_self()->tls.server.private_key)
        server->private_key =
            ogs_strdup(ogs_sbi_self()->tls.server.private_key);
    if (ogs_sbi_self()->tls.server.cert)
        server->cert = ogs_strdup(ogs_sbi_self()->tls.server.cert);
    if (ogs_sbi_self()->tls.server.sslkeylog)
        server->sslkeylog = ogs_strdup(ogs_sbi_self()->tls.server.sslkeylog);

    server->verify_client = ogs_sbi_self()->tls.server.verify_client;
    if (ogs_sbi_self()->tls.server.verify_client_cacert)
        server->verify_client_cacert =
            ogs_strdup(ogs_sbi_self()->tls.server.verify_client_cacert);

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

    if (server->interface)
        ogs_free(server->interface);

    ogs_assert(server->node.addr);
    ogs_freeaddrinfo(server->node.addr);
    if (server->node.option)
        ogs_free(server->node.option);
    if (server->advertise)
        ogs_freeaddrinfo(server->advertise);

    if (server->verify_client_cacert)
        ogs_free(server->verify_client_cacert);
    if (server->private_key)
        ogs_free(server->private_key);
    if (server->cert)
        ogs_free(server->cert);
    if (server->sslkeylog)
        ogs_free(server->sslkeylog);

    ogs_pool_id_free(&server_pool, server);
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

void ogs_sbi_server_graceful_shutdown_all(void)
{
    ogs_sbi_server_t *server = NULL, *next_server = NULL;

    ogs_list_for_each_safe(&ogs_sbi_self()->server_list, next_server, server)
        ogs_sbi_server_actions.graceful_shutdown(server);
}

void ogs_sbi_server_stop_all(void)
{
    ogs_sbi_server_t *server = NULL, *next_server = NULL;

    ogs_list_for_each_safe(&ogs_sbi_self()->server_list, next_server, server)
        ogs_sbi_server_actions.stop(server);
}

bool ogs_sbi_server_send_rspmem_persistent(
        ogs_sbi_stream_t *stream, ogs_sbi_response_t *response)
{
    return ogs_sbi_server_actions.send_rspmem_persistent(stream, response);
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
        const char *title, const char *detail, const char *cause)
{
    OpenAPI_problem_details_t problem;

    ogs_assert(stream);

    memset(&problem, 0, sizeof(problem));

    if (message) {
        problem.type = ogs_msprintf("/%s/%s",
                message->h.service.name, message->h.api.version);
        ogs_expect(problem.type);
        if (message->h.resource.component[1])
            problem.instance = ogs_msprintf("/%s/%s",
                    message->h.resource.component[0],
                    message->h.resource.component[1]);
        else
            problem.instance =
                    ogs_msprintf("/%s", message->h.resource.component[0]);
        ogs_expect(problem.instance);
    }
    if (status) {
        problem.is_status = true;
        problem.status = status;
    }
    problem.title = (char*)title;
    problem.detail = (char*)detail;
    problem.cause = (char*)cause;

    ogs_sbi_server_send_problem(stream, &problem);

    if (problem.type)
        ogs_free(problem.type);
    if (problem.instance)
        ogs_free(problem.instance);

    return true;
}

ogs_sbi_server_t *ogs_sbi_server_from_stream(ogs_sbi_stream_t *stream)
{
    return ogs_sbi_server_actions.server_from_stream(stream);
}

ogs_pool_id_t ogs_sbi_id_from_stream(ogs_sbi_stream_t *stream)
{
    return ogs_sbi_server_actions.id_from_stream(stream);
}

void *ogs_sbi_stream_find_by_id(ogs_pool_id_t id)
{
    return ogs_sbi_server_actions.stream_find_by_id(id);
}

int ogs_sbi_server_attach_xact(ogs_sbi_xact_t *xact)
{
    ogs_sbi_stream_t *stream = NULL;

    ogs_assert(xact);

    /*
     * Idempotent. NFs may set xact->assoc_stream_id and reach the
     * SBI path multiple times for retries; attaching twice would
     * corrupt the list, so we skip when already attached. This is
     * not an error in itself — return OK so the caller continues.
     */
    if (xact->to_stream_list) {
        ogs_error("ogs_sbi_server_attach_xact: already attached "
                "[xact:%d,assoc_stream_id:%d,service:%s]",
                (int)xact->id, (int)xact->assoc_stream_id,
                xact->service_type ?
                    ogs_sbi_service_type_to_name(xact->service_type) :
                    "NULL");
        return OGS_OK;
    }

    /*
     * No associated stream is normal for self-initiated transactions
     * (NRF discovery from the NF itself, status notifications, etc.).
     * Silent skip, OK.
     */
    if (xact->assoc_stream_id < OGS_MIN_POOL_ID ||
        xact->assoc_stream_id > OGS_MAX_POOL_ID)
        return OGS_OK;

    /*
     * xact->assoc_stream_id is a "best-effort" linkage: it means
     * "if a response arrives and this stream is still alive,
     * deliver the response there." The stream is not required to
     * be alive at the moment the outbound transaction is sent.
     *
     * NFs are allowed to pass the same stream to discover_and_send()
     * even after that stream has already been used to answer the
     * original peer (and is therefore being closed). The common
     * case is a "respond then start follow-up" pattern, e.g. SMF
     * answering the AMF with HTTP 201 Created and then kicking off
     * Npcf_SMPolicyControl_Create.
     *
     * Two outcomes:
     *
     *   - Stream still alive: attach to its xact_list so the
     *     transaction is cancelled (and its response timer freed)
     *     if the peer later resets the stream. This is the
     *     #4472/#4473 protection path. Returns OGS_OK.
     *
     *   - Stream already gone: nothing to attach. Returns
     *     OGS_ERROR so the caller can log with its own context
     *     (NF name, UE/session id, etc.) and decide what to do.
     *     The caller may legitimately ignore the error and let
     *     the transaction proceed; the response (if any) will be
     *     dropped through the existing "STREAM has already been
     *     removed" path in the response handler.
     */
    stream = ogs_sbi_stream_find_by_id(xact->assoc_stream_id);
    if (!stream) {
        ogs_error("ogs_sbi_server_attach_xact: originating SBI stream "
                "already closed "
                "[xact:%d,assoc_stream_id:%d,service:%s]",
                (int)xact->id, (int)xact->assoc_stream_id,
                xact->service_type ?
                    ogs_sbi_service_type_to_name(xact->service_type) :
                    "NULL");
        return OGS_NOTFOUND;
    }

    ogs_assert(ogs_sbi_server_actions.xact_attach);
    ogs_sbi_server_actions.xact_attach(stream, xact);

    return OGS_OK;
}

void ogs_sbi_server_detach_xact(ogs_sbi_xact_t *xact)
{
    ogs_assert(xact);

    /*
     * Idempotent. No-op when not attached, including transactions
     * that never had an inbound stream (NRF discovery, status
     * notifications) where to_stream_list stays NULL. This is
     * invoked unconditionally from ogs_sbi_xact_remove(), so
     * streamless transactions reach here on every cleanup.
     */
    if (!xact->to_stream_list)
        return;

    ogs_assert(ogs_sbi_server_actions.xact_detach);
    ogs_sbi_server_actions.xact_detach(xact);
}

static ogs_sbi_server_t *ogs_sbi_server_find_by_interface(
        ogs_sbi_server_t *current, const char *interface)
{
    ogs_sbi_server_t *server = NULL;

    server = current ?
        ogs_list_next(current) : ogs_list_first(&ogs_sbi_self()->server_list);

    for (; server; server = ogs_list_next(server)) {
        if (interface == NULL) {
            if (server->interface == NULL)
                return server;
        } else {
            if (server->interface && strcmp(interface, server->interface) == 0)
                return server;
        }
    }

    return NULL;
}

ogs_sbi_server_t *ogs_sbi_server_first(void)
{
    return ogs_sbi_server_find_by_interface(NULL, NULL);
}

ogs_sbi_server_t *ogs_sbi_server_next(ogs_sbi_server_t *current)
{
    ogs_assert(current);
    return ogs_sbi_server_find_by_interface(current, NULL);
}

ogs_sbi_server_t *ogs_sbi_server_first_by_interface(const char *interface)
{
    ogs_assert(interface);
    return ogs_sbi_server_find_by_interface(NULL, interface);
}

ogs_sbi_server_t *ogs_sbi_server_next_by_interface(
        ogs_sbi_server_t *current, const char *interface)
{
    ogs_assert(current);
    ogs_assert(interface);
    return ogs_sbi_server_find_by_interface(current, interface);
}
