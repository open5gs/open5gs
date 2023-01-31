/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
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

static void build_default_discovery_parameter(
        ogs_sbi_request_t *request,
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option);

int ogs_sbi_server_handler(ogs_sbi_request_t *request, void *data)
{
    ogs_event_t *e = NULL;
    int rv;

    ogs_assert(request);
    ogs_assert(data);

    e = ogs_event_new(OGS_EVENT_SBI_SERVER);
    ogs_assert(e);

    e->sbi.request = request;
    e->sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_sbi_request_free(request);
        ogs_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_sbi_client_handler(
        int status, ogs_sbi_response_t *response, void *data)
{
    ogs_event_t *e = NULL;
    int rv;

    if (status != OGS_OK) {
        ogs_log_message(
                status == OGS_DONE ? OGS_LOG_DEBUG : OGS_LOG_WARN, 0,
                "ogs_sbi_client_handler() failed [%d]", status);
        return OGS_ERROR;
    }

    ogs_assert(response);

    e = ogs_event_new(OGS_EVENT_SBI_CLIENT);
    ogs_assert(e);
    e->sbi.response = response;
    e->sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_sbi_response_free(response);
        ogs_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int client_discover_cb(
        int status, ogs_sbi_response_t *response, void *data)
{
    int rv;
    ogs_event_t *e = NULL;

    ogs_sbi_xact_t *xact = NULL;
    ogs_sbi_object_t *sbi_object = NULL;
    ogs_sbi_service_type_e service_type = OGS_SBI_SERVICE_TYPE_NULL;
    OpenAPI_nf_type_e target_nf_type = OpenAPI_nf_type_NULL;
    OpenAPI_nf_type_e requester_nf_type = OpenAPI_nf_type_NULL;

    ogs_hash_index_t *hi = NULL;
    char *producer_id = NULL;

    xact = data;
    ogs_assert(xact);

    xact = ogs_sbi_xact_cycle(xact);
    if (!xact) {
        ogs_error("SBI transaction has already been removed");
        if (response)
            ogs_sbi_response_free(response);
        return OGS_ERROR;
    }

    sbi_object = xact->sbi_object;
    ogs_assert(sbi_object);
    service_type = xact->service_type;
    ogs_assert(service_type);
    target_nf_type = ogs_sbi_service_type_to_nf_type(service_type);
    requester_nf_type = xact->requester_nf_type;
    ogs_assert(requester_nf_type);

    if (status != OGS_OK) {
        ogs_log_message(
                status == OGS_DONE ? OGS_LOG_DEBUG : OGS_LOG_WARN, 0,
                "ogs_sbi_client_handler() failed [%d]", status);
        ogs_sbi_xact_remove(xact);
        return OGS_ERROR;
    }

    ogs_assert(response);

    /* Check if 3gpp-Sbi-Producer-Id in HTTP2 Header */
    for (hi = ogs_hash_first(response->http.headers);
            hi; hi = ogs_hash_next(hi)) {
        if (!ogs_strcasecmp(ogs_hash_this_key(hi), OGS_SBI_CUSTOM_PRODUCER_ID)) {
            producer_id = ogs_hash_this_val(hi);
            break;
        }
    }

    /* Added newly discovered NF Instance */
    if (producer_id) {
        ogs_sbi_nf_instance_t *nf_instance =
            ogs_sbi_nf_instance_find(producer_id);
        if (!nf_instance) {
            nf_instance = ogs_sbi_nf_instance_add();
            ogs_assert(nf_instance);

            ogs_sbi_nf_instance_set_id(nf_instance, producer_id);
            ogs_sbi_nf_instance_set_type(nf_instance, target_nf_type);
        }

        OGS_SBI_SETUP_NF_INSTANCE(
                sbi_object->service_type_array[service_type], nf_instance);
    }

    e = ogs_event_new(OGS_EVENT_SBI_CLIENT);
    ogs_assert(e);
    e->sbi.response = response;
    e->sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_sbi_response_free(response);
        ogs_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ogs_sbi_discover_and_send(ogs_sbi_xact_t *xact)
{
    bool rc;
    ogs_sbi_client_t *client = NULL, *scp_client = NULL;
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_sbi_object_t *sbi_object = NULL;
    ogs_sbi_service_type_e service_type = OGS_SBI_SERVICE_TYPE_NULL;
    ogs_sbi_discovery_option_t *discovery_option = NULL;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_nf_type_e target_nf_type = OpenAPI_nf_type_NULL;
    OpenAPI_nf_type_e requester_nf_type = OpenAPI_nf_type_NULL;
    char *apiroot = NULL;

    sbi_object = xact->sbi_object;
    ogs_assert(sbi_object);
    service_type = xact->service_type;
    ogs_assert(service_type);
    target_nf_type = ogs_sbi_service_type_to_nf_type(service_type);
    ogs_assert(target_nf_type);
    requester_nf_type = xact->requester_nf_type;
    ogs_assert(requester_nf_type);
    request = xact->request;
    ogs_assert(request);

    discovery_option = xact->discovery_option;

    /* SCP Availability */
    if (ogs_sbi_self()->discovery_config.delegated ==
            OGS_SBI_DISCOVERY_DELEGATED_AUTO) {
        scp_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);
    } else if (ogs_sbi_self()->discovery_config.delegated ==
            OGS_SBI_DISCOVERY_DELEGATED_YES) {
        scp_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);
        ogs_assert(scp_client);
    }

    /* Target NF-Instance */
    nf_instance = sbi_object->service_type_array[service_type].nf_instance;
    if (!nf_instance) {
        nf_instance = ogs_sbi_nf_instance_find_by_discovery_param(
                        target_nf_type, requester_nf_type, discovery_option);
        if (nf_instance)
            OGS_SBI_SETUP_NF_INSTANCE(
                    sbi_object->service_type_array[service_type], nf_instance);
    }

    /* Target Client */
    if (request->h.uri == NULL) {
        if (nf_instance) {
            client = ogs_sbi_client_find_by_service_name(nf_instance,
                    request->h.service.name, request->h.api.version);
        }
    } else {
        OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
        ogs_sockaddr_t *addr = NULL;

        rc = ogs_sbi_getaddr_from_uri(&scheme, &addr, request->h.uri);
        if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
            ogs_error("Invalid URL [%s]", request->h.uri);
            return OGS_ERROR;
        }
        client = ogs_sbi_client_find(scheme, addr);
        ogs_freeaddrinfo(addr);
    }

    if (scp_client) {
        /*************************
         * INDIRECT COMMUNICATION
         *************************/
        build_default_discovery_parameter(
            request, service_type, discovery_option);

        if (client) {
            /*
             * If `client` instance is avaiable,
             * 3gpp-Sbi-Target-apiRoot is added to HTTP header.
             */
            apiroot = ogs_sbi_client_apiroot(client);
            ogs_assert(apiroot);

            ogs_sbi_header_set(request->http.headers,
                    OGS_SBI_CUSTOM_TARGET_APIROOT, apiroot);

            ogs_free(apiroot);

            rc = ogs_sbi_client_send_via_scp(
                    scp_client, ogs_sbi_client_handler, request, xact);
            ogs_expect(rc == true);
            return (rc == true) ? OGS_OK : OGS_ERROR;

        } else {
            /*
             * If no `client` instance,
             *
             * Discovery-*** is added to HTTP header.
             */
            if (discovery_option &&
                discovery_option->target_nf_instance_id) {
                ogs_sbi_header_set(request->http.headers,
                        OGS_SBI_CUSTOM_DISCOVERY_TARGET_NF_INSTANCE_ID,
                        discovery_option->target_nf_instance_id);
            } else if (nf_instance && nf_instance->id) {
                ogs_sbi_header_set(request->http.headers,
                        OGS_SBI_CUSTOM_DISCOVERY_TARGET_NF_INSTANCE_ID,
                        nf_instance->id);
            }

            if (discovery_option &&
                discovery_option->requester_features) {
                char *v = ogs_uint64_to_string(
                        discovery_option->requester_features);
                if (!v) {
                    ogs_error("ogs_uint64_to_string[0x%llx] failed",
                            (long long)discovery_option->requester_features);
                    return OGS_ERROR;
                }

                ogs_sbi_header_set(request->http.headers,
                        OGS_SBI_CUSTOM_DISCOVERY_REQUESTER_FEATURES, v);
                ogs_free(v);
            }

            rc = ogs_sbi_client_send_via_scp(
                    scp_client, client_discover_cb, request, xact);
            ogs_expect(rc == true);
            return (rc == true) ? OGS_OK : OGS_ERROR;
        }

    } else if (client) {
        /***********************
         * DIRECT COMMUNICATION
         ***********************/

        /* If `client` instance is available, use direct communication */
        rc = ogs_sbi_client_send_request(
                client, ogs_sbi_client_handler, request, xact);
        ogs_expect(rc == true);
        return (rc == true) ? OGS_OK : OGS_ERROR;

    } else {
        /**********************************************
         * No SCP and Client, Use NRF for NF-Discovery
         **********************************************/
        return ogs_sbi_discover_only(xact);
    }

    return OGS_OK;
}

int ogs_sbi_discover_only(ogs_sbi_xact_t *xact)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_sbi_object_t *sbi_object = NULL;
    ogs_sbi_service_type_e service_type = OGS_SBI_SERVICE_TYPE_NULL;
    ogs_sbi_discovery_option_t *discovery_option = NULL;

    OpenAPI_nf_type_e target_nf_type = OpenAPI_nf_type_NULL;
    OpenAPI_nf_type_e requester_nf_type = OpenAPI_nf_type_NULL;

    sbi_object = xact->sbi_object;
    ogs_assert(sbi_object);
    service_type = xact->service_type;
    ogs_assert(service_type);
    target_nf_type = ogs_sbi_service_type_to_nf_type(service_type);
    ogs_assert(target_nf_type);
    requester_nf_type = xact->requester_nf_type;
    ogs_assert(requester_nf_type);

    discovery_option = xact->discovery_option;

    /* NRF NF-Instance */
    nf_instance = ogs_sbi_self()->nrf_instance;
    if (nf_instance) {
        bool rc;
        ogs_sbi_client_t *client = NULL;
        ogs_sbi_request_t *request = NULL;

        ogs_warn("Try to discover [%s]",
                    ogs_sbi_service_type_to_name(service_type));

        client = NF_INSTANCE_CLIENT(nf_instance);
        if (!client) {
            ogs_error("No Client");
            return OGS_NOTFOUND;
        }

        request = ogs_nnrf_disc_build_discover(
                    target_nf_type, requester_nf_type, discovery_option);
        if (!request) {
            ogs_error("ogs_nnrf_disc_build_discover() failed");
            return OGS_ERROR;
        }

        rc = ogs_sbi_client_send_request(
                client, ogs_sbi_client_handler, request, xact);
        ogs_expect(rc == true);

        ogs_sbi_request_free(request);

        return (rc == true) ? OGS_OK : OGS_ERROR;
    }

    ogs_error("Cannot discover [%s]",
                ogs_sbi_service_type_to_name(service_type));

    return OGS_NOTFOUND;
}

bool ogs_sbi_send_request_to_nf_instance(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact)
{
    bool rc;
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
        bool rc;
        OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
        ogs_sockaddr_t *addr = NULL;
        char buf[OGS_ADDRSTRLEN];

        rc = ogs_sbi_getaddr_from_uri(&scheme, &addr, request->h.uri);
        if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
            ogs_error("[%s:%s] Invalid URL [%s]",
                    OpenAPI_nf_type_ToString(nf_instance->nf_type),
                    nf_instance->id, request->h.uri);
            return false;
        }
        client = ogs_sbi_client_find(scheme, addr);
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

    rc = ogs_sbi_send_request_to_client(
            client, ogs_sbi_client_handler, request, xact);
    ogs_expect(rc == true);

    return rc;
}

bool ogs_sbi_send_request_to_client(
        ogs_sbi_client_t *client, ogs_sbi_client_cb_f client_cb,
        ogs_sbi_request_t *request, void *data)
{
    bool rc;
    ogs_sbi_client_t *scp_client = NULL;
    char *apiroot = NULL;

    /*
     * If the HTTP2 Server's EndPoint is known,
     * 3gpp-Sbi-Target-apiRoot should always be included in the HTTP2 Request.
     */
    ogs_assert(client);
    ogs_assert(request);

    scp_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);

    if (scp_client && scp_client != client) {

        /*************************
         * INDIRECT COMMUNICATION
         *************************/

        /* Added 3gpp-Sbi-Target-apiRoot to HTTP header */
        apiroot = ogs_sbi_client_apiroot(client);
        ogs_assert(apiroot);

        ogs_sbi_header_set(request->http.headers,
                OGS_SBI_CUSTOM_TARGET_APIROOT, apiroot);

        ogs_free(apiroot);

        rc = ogs_sbi_client_send_via_scp(
                scp_client, client_cb, request, data);
        ogs_expect(rc == true);

    } else {

        /***********************
         * DIRECT COMMUNICATION
         ***********************/

        /* Direct communication since `client' instance is always avaiable */
        rc = ogs_sbi_client_send_request(
                client, client_cb, request, data);
        ogs_expect(rc == true);

    }

    return rc;
}

bool ogs_sbi_send_notification_request(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *request, void *data)
{
    bool rc;
    ogs_sbi_client_t *client = NULL, *scp_client = NULL;
    OpenAPI_nf_type_e target_nf_type = OpenAPI_nf_type_NULL;

    ogs_assert(service_type);
    target_nf_type = ogs_sbi_service_type_to_nf_type(service_type);
    ogs_assert(target_nf_type);
    ogs_assert(request);

    scp_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);
    if (target_nf_type == OpenAPI_nf_type_NRF)
        client = NF_INSTANCE_CLIENT(ogs_sbi_self()->nrf_instance);
    else {
        ogs_fatal("Not implemented[%s]",
                ogs_sbi_service_type_to_name(service_type));
        ogs_assert_if_reached();
    }

    if (scp_client) {
        /*************************
         * INDIRECT COMMUNICATION
         *************************/
        build_default_discovery_parameter(
            request, service_type, discovery_option);

        rc = ogs_sbi_client_send_via_scp(
                scp_client, ogs_sbi_client_handler, request, data);
        ogs_expect(rc == true);

    } else if (client) {

        /***********************
         * DIRECT COMMUNICATION
         ***********************/

        /* NRF is avaiable */
        rc = ogs_sbi_client_send_request(
                client, ogs_sbi_client_handler, request, data);
        ogs_expect(rc == true);


    } else {
        ogs_fatal("[%s:%s] Cannot send request [%s:%s:%s]",
                client ? "CLIENT" : "No-CLIENT",
                scp_client ? "SCP" : "No-SCP",
                ogs_sbi_service_type_to_name(service_type),
                request->h.service.name, request->h.api.version);
        rc = false;
        ogs_assert_if_reached();
    }

    return true;
}

bool ogs_sbi_send_response(ogs_sbi_stream_t *stream, int status)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(stream);

    memset(&sendmsg, 0, sizeof(sendmsg));

    response = ogs_sbi_build_response(&sendmsg, status);
    if (!response) {
        ogs_error("ogs_sbi_build_response() failed");
        return false;
    }

    return ogs_sbi_server_send_response(stream, response);
}

static void build_default_discovery_parameter(
        ogs_sbi_request_t *request,
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option)
{
    OpenAPI_nf_type_e target_nf_type = OpenAPI_nf_type_NULL;
    OpenAPI_nf_type_e requester_nf_type = OpenAPI_nf_type_NULL;
    ogs_sbi_discovery_option_t *local_discovery_option = NULL;

    ogs_assert(service_type);
    target_nf_type = ogs_sbi_service_type_to_nf_type(service_type);
    ogs_assert(target_nf_type);
    requester_nf_type = NF_INSTANCE_TYPE(ogs_sbi_self()->nf_instance);
    ogs_assert(requester_nf_type);

    /*
     * Insert one service-name in the discovery option
     * in the function below.
     *
     * - ogs_sbi_xact_add()
     * - ogs_sbi_send_notification_request()
     */
    if (!discovery_option) {
        local_discovery_option = ogs_sbi_discovery_option_new();
        ogs_assert(local_discovery_option);

        discovery_option = local_discovery_option;
    }

    if (!discovery_option->num_of_service_names) {
        ogs_sbi_discovery_option_add_service_names(
                discovery_option,
                (char *)ogs_sbi_service_type_to_name(service_type));
    }

    ogs_sbi_header_set(request->http.headers,
            OGS_SBI_CUSTOM_DISCOVERY_TARGET_NF_TYPE,
            OpenAPI_nf_type_ToString(target_nf_type));

    /* Instead of Discovery-requester-nf-type,
     * Open5GS uses User-Agent for requester-nf-type */

    if (discovery_option) {
        if (discovery_option->requester_nf_instance_id) {
            ogs_sbi_header_set(request->http.headers,
                    OGS_SBI_CUSTOM_DISCOVERY_REQUESTER_NF_INSTANCE_ID,
                    discovery_option->requester_nf_instance_id);
        }
        if (ogs_sbi_self()->discovery_config.
                no_service_names == false &&
            discovery_option->num_of_service_names) {

            /* send array items separated by a comma */
            char *v = ogs_sbi_discovery_option_build_service_names(
                        discovery_option);
            if (v) {
                ogs_sbi_header_set(request->http.headers,
                        OGS_SBI_CUSTOM_DISCOVERY_SERVICE_NAMES, v);
                ogs_free(v);
            } else {
                ogs_warn("invalid service names failed[%d:%s]",
                            discovery_option->num_of_service_names,
                            discovery_option->service_names[0]);
            }
        }
    }

    if (local_discovery_option)
        ogs_sbi_discovery_option_free(local_discovery_option);
}
