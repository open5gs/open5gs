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

#include "ogs-sbi.h"
#include "ogs-app.h"

bool ogs_sbi_send_request(ogs_sbi_nf_instance_t *nf_instance,
        ogs_sbi_client_cb_f client_cb, ogs_sbi_xact_t *xact)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;
    ogs_sbi_object_t *sbi_object = NULL;

    ogs_assert(xact);
    sbi_object = xact->sbi_object;
    ogs_assert(sbi_object);
    request = xact->request;
    ogs_assert(request);

    ogs_assert(nf_instance);

    if (request->h.uri == NULL) {
        client = ogs_sbi_client_find_by_service_name(nf_instance,
                request->h.service.name, request->h.api.version);
        if (!client) {
            ogs_error("[%s:%s] Cannot find client [%s:%s]",
                    OpenAPI_nf_type_ToString(nf_instance->nf_type),
                    nf_instance->id,
                    request->h.service.name, request->h.api.version);
            return false;
        }
    } else {
        ogs_sockaddr_t *addr = NULL;
        char buf[OGS_ADDRSTRLEN];

        addr = ogs_sbi_getaddr_from_uri(request->h.uri);
        if (!addr) {
            ogs_error("[%s:%s] Invalid URL [%s]",
                    OpenAPI_nf_type_ToString(nf_instance->nf_type),
                    nf_instance->id, request->h.uri);
            return false;
        }
        client = ogs_sbi_client_find(addr);
        if (!client) {
            ogs_error("[%s:%s] Cannot find client [%s:%d]",
                    OpenAPI_nf_type_ToString(nf_instance->nf_type),
                    nf_instance->id,
                    OGS_ADDR(addr, buf), OGS_PORT(addr));
            ogs_freeaddrinfo(addr);
            return false;
        }
        ogs_freeaddrinfo(addr);
    }

    /* ogs_sbi_xact_remove() will call ogs_sbi_request_free()
     * As such, we'll use ogs_sbi_client_send_reqmem_persistent() */
    ogs_expect_or_return_val(true ==
        ogs_sbi_scp_send_reqmem_persistent(
            client, client_cb, request, xact), false);

    return true;
}

bool ogs_sbi_discover_and_send(
        ogs_sbi_object_t *sbi_object,
        OpenAPI_nf_type_e target_nf_type,
        OpenAPI_nf_type_e requester_nf_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_client_cb_f client_cb, void *data)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_assert(sbi_object);
    ogs_assert(target_nf_type);
    ogs_assert(requester_nf_type);
    ogs_assert(client_cb);

    /* Target NF-Instance */
    nf_instance = OGS_SBI_NF_INSTANCE(sbi_object, target_nf_type);
    if (!nf_instance) {
        ogs_assert(target_nf_type != OpenAPI_nf_type_NRF);
        ogs_sbi_select_nf(sbi_object, target_nf_type, discovery_option);
        nf_instance = OGS_SBI_NF_INSTANCE(sbi_object, target_nf_type);
    }

    if (nf_instance) {
        return ogs_sbi_send_request(nf_instance, client_cb, data);
    }

    /* NRF NF-Instance */
    nf_instance = OGS_SBI_NF_INSTANCE(sbi_object, OpenAPI_nf_type_NRF);
    if (!nf_instance) {
        ogs_sbi_select_nf(sbi_object, OpenAPI_nf_type_NRF, NULL);
        nf_instance = OGS_SBI_NF_INSTANCE(sbi_object, OpenAPI_nf_type_NRF);
    }

    if (nf_instance) {
        ogs_warn("Try to discover [%s]",
                    OpenAPI_nf_type_ToString(target_nf_type));
        return ogs_nnrf_disc_send_nf_discover(
                nf_instance,
                target_nf_type, requester_nf_type, discovery_option,
                data);
    }

    ogs_error("Cannot discover [%s]", OpenAPI_nf_type_ToString(target_nf_type));

    return false;
}

bool ogs_nnrf_nfm_send_nf_register(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_request_t *(*build)(void))
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(nf_instance);
    client = nf_instance->client;
    ogs_assert(client);
    ogs_assert(build);

    request = (*build)();
    ogs_expect_or_return_val(request, false);

    return ogs_sbi_scp_send_request(client, client->cb, request, nf_instance);
}

bool ogs_nnrf_nfm_send_nf_update(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(nf_instance);
    client = nf_instance->client;
    ogs_assert(client);

    request = ogs_nnrf_nfm_build_update();
    ogs_expect_or_return_val(request, false);

    return ogs_sbi_scp_send_request(client, client->cb, request, nf_instance);
}

bool ogs_nnrf_nfm_send_nf_de_register(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(nf_instance);
    client = nf_instance->client;
    ogs_assert(client);

    request = ogs_nnrf_nfm_build_de_register();
    ogs_expect_or_return_val(request, false);

    return ogs_sbi_scp_send_request(client, client->cb, request, nf_instance);
}

bool ogs_nnrf_nfm_send_nf_profile_retrieve(ogs_sbi_nf_instance_t *nf_instance,
        char *nf_instance_id, void *data)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(nf_instance);
    client = nf_instance->client;
    ogs_assert(client);
    ogs_assert(nf_instance_id);

    request = ogs_nnrf_nfm_build_profile_retrieve(nf_instance_id);
    ogs_expect_or_return_val(request, false);

    return ogs_sbi_client_send_request(
            client, client->cb, request, data);
}

bool ogs_nnrf_nfm_send_nf_status_subscribe(ogs_sbi_client_t *client,
        OpenAPI_nf_type_e req_nf_type, char *req_nf_instance_id,
        OpenAPI_nf_type_e subscr_cond_nf_type)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_subscription_t *subscription = NULL;

    ogs_assert(client);

    subscription = ogs_sbi_subscription_add();
    ogs_assert(subscription);

    OGS_SBI_SETUP_CLIENT(subscription, client);
    subscription->req_nf_type = req_nf_type;
    if (req_nf_instance_id) {
        subscription->req_nf_instance_id = ogs_strdup(req_nf_instance_id);
        ogs_expect_or_return_val(req_nf_instance_id, false);
    }
    subscription->subscr_cond.nf_type = subscr_cond_nf_type;

    request = ogs_nnrf_nfm_build_status_subscribe(subscription);
    ogs_expect_or_return_val(request, false);

    return ogs_sbi_scp_send_request(
            client, client->cb, request, subscription);
}

bool ogs_nnrf_nfm_send_nf_status_unsubscribe(
        ogs_sbi_subscription_t *subscription)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(subscription);

    request = ogs_nnrf_nfm_build_status_unsubscribe(subscription);
    ogs_expect_or_return_val(request, false);

    client = subscription->client;
    ogs_assert(client);

    return ogs_sbi_scp_send_request(
            client, client->cb, request, subscription);
}

bool ogs_nnrf_disc_send_nf_discover(
        ogs_sbi_nf_instance_t *nf_instance,
        OpenAPI_nf_type_e target_nf_type,
        OpenAPI_nf_type_e requester_nf_type,
        ogs_sbi_discovery_option_t *discovery_option,
        void *data)
{
    ogs_sbi_client_t *client = NULL;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(nf_instance);
    ogs_assert(target_nf_type);
    ogs_assert(requester_nf_type);

    request = ogs_nnrf_disc_build_discover(
                target_nf_type, requester_nf_type, discovery_option);
    ogs_expect_or_return_val(request, false);

    client = nf_instance->client;
    ogs_assert(client);

    return ogs_sbi_client_send_request(client, client->cb, request, data);
}

bool ogs_sbi_send_response(ogs_sbi_stream_t *stream, int status)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(stream);

    memset(&sendmsg, 0, sizeof(sendmsg));

    response = ogs_sbi_build_response(&sendmsg, status);
    ogs_expect_or_return_val(response, false);
    return ogs_sbi_server_send_response(stream, response);
}
