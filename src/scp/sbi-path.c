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

#include "sbi-path.h"

static int request_handler(ogs_sbi_request_t *request, void *data);
static int response_handler(
        int status, ogs_sbi_response_t *response, void *data);

static int client_cb(int status, ogs_sbi_response_t *response, void *data);

int scp_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    /* Add SELF NF instance */
    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);
    ogs_sbi_nf_fsm_init(nf_instance);

    /* Build NF instance information. It will be transmitted to NRF. */
    ogs_sbi_nf_instance_build_default(nf_instance, OpenAPI_nf_type_SCP);

    /* Initialize NRF NF Instance */
    nf_instance = ogs_sbi_self()->nrf_instance;
    if (nf_instance) {
        ogs_sbi_client_t *client = NULL;

        /* Client callback is only used when NF sends to NRF */
        client = nf_instance->client;
        ogs_assert(client);
        client->cb = client_cb;

        /* NFRegister is sent and the response is received
         * by the above client callback. */
        ogs_sbi_nf_fsm_init(nf_instance);
    }

    /* Build Subscription-Data */
    ogs_sbi_subscription_data_build_default(OpenAPI_nf_type_AMF, NULL);
    ogs_sbi_subscription_data_build_default(OpenAPI_nf_type_AUSF, NULL);
    ogs_sbi_subscription_data_build_default(OpenAPI_nf_type_BSF, NULL);
    ogs_sbi_subscription_data_build_default(OpenAPI_nf_type_NSSF, NULL);
    ogs_sbi_subscription_data_build_default(OpenAPI_nf_type_PCF, NULL);
    ogs_sbi_subscription_data_build_default(OpenAPI_nf_type_SMF, NULL);
    ogs_sbi_subscription_data_build_default(OpenAPI_nf_type_UDM, NULL);
    ogs_sbi_subscription_data_build_default(OpenAPI_nf_type_UDR, NULL);

    if (ogs_sbi_server_start_all(request_handler) != OGS_OK)
        return OGS_ERROR;

    return OGS_OK;
}

void scp_sbi_close(void)
{
    ogs_sbi_client_stop_all();
    ogs_sbi_server_stop_all();
}

static int request_handler(ogs_sbi_request_t *source, void *data)
{
    int rv;
    ogs_hash_index_t *hi;
    ogs_sbi_stream_t *stream = data;

    struct {
        char *target_apiroot;
        char *discovery;
        char *accept;
        char *content_type;
        char *content_encoding;
    } headers;

    ogs_assert(source);
    ogs_assert(source->h.uri);
    ogs_assert(stream);

    memset(&headers, 0, sizeof(headers));

    /* Extract HTTP Header */
    for (hi = ogs_hash_first(source->http.headers);
            hi; hi = ogs_hash_next(hi)) {
        char *key = (char *)ogs_hash_this_key(hi);
        char *val = ogs_hash_this_val(hi);

        if (!key || !val) {
            ogs_error("No Key[%s] Value[%s]", key, val);
            continue;
        }

        /*
         * <RFC 2616>
         *  Each header field consists of a name followed by a colon (":")
         *  and the field value. Field names are case-insensitive.
         */
        if (!strcasecmp(key, OGS_SBI_CUSTOM_TARGET_APIROOT)) {
            headers.target_apiroot = val;
        } else if (!strncasecmp(key,
                    OGS_SBI_CUSTOM_DISCOVERY_COMMON,
                    strlen(OGS_SBI_CUSTOM_DISCOVERY_COMMON))) {
            headers.discovery = val;
        } else if (!strcasecmp(key, OGS_SBI_ACCEPT)) {
            headers.accept = val;
        } else if (!strcasecmp(key, OGS_SBI_CONTENT_TYPE)) {
            headers.content_type = val;
        } else if (!strcasecmp(key, OGS_SBI_ACCEPT_ENCODING)) {
            headers.content_encoding = val;
        }
    }

    if (headers.target_apiroot) {
        ogs_sbi_nf_instance_t *scp_instance = NULL;
        ogs_sbi_client_t *client = NULL;
        ogs_sbi_request_t target;

        scp_conn_t *conn = NULL;

        memset(&target, 0, sizeof(target));

        conn = scp_conn_add(stream);
        if (!conn) {
            ogs_error("scp_conn_add() failed");
            ogs_sbi_request_free(source);
            return OGS_ERROR;
        }

        if (ogs_sbi_self()->discovery_config.delegated ==
                OGS_SBI_DISCOVERY_DELEGATED_AUTO) {
            scp_instance = ogs_sbi_self()->scp_instance;
        } else if (ogs_sbi_self()->discovery_config.delegated ==
                OGS_SBI_DISCOVERY_DELEGATED_YES) {
            scp_instance = ogs_sbi_self()->scp_instance;
            ogs_assert(scp_instance);
        }

        /* HTTP Method */
        target.h.method = source->h.method;

        /* HTTP Headers
         *
         * To remove the followings,
         *   Scheme - https
         *   Authority - scp.open5gs.org
         *
         * We regenerate the HTTP Headers.
         */
        target.http.headers = ogs_hash_make();
        ogs_assert(target.http.headers);

        if (headers.content_type)
            ogs_sbi_header_set(target.http.headers,
                    OGS_SBI_CONTENT_TYPE, headers.content_type);
        if (headers.accept)
            ogs_sbi_header_set(target.http.headers,
                    OGS_SBI_ACCEPT, headers.accept);
        if (headers.content_encoding)
            ogs_sbi_header_set(target.http.headers,
                    OGS_SBI_ACCEPT_ENCODING, headers.content_encoding);

        /* HTTP Params */
        target.http.params = source->http.params;

        /* HTTP Content */
        target.http.content = source->http.content;
        target.http.content_length = source->http.content_length;

        if (scp_instance) {
            char *apiroot = NULL;

            if (headers.target_apiroot)
                ogs_sbi_header_set(target.http.headers,
                        OGS_SBI_CUSTOM_TARGET_APIROOT, headers.target_apiroot);

            /* Client Instance */
            client = scp_instance->client;
            ogs_assert(client);

            /* Client ApiRoot */
            apiroot = ogs_sbi_client_apiroot(client);
            ogs_assert(apiroot);

            /* Setup New URI */
            target.h.uri = ogs_msprintf("%s%s", apiroot, source->h.uri);
            ogs_assert(target.h.uri);

            ogs_free(apiroot);

        } else {
            ogs_sockaddr_t *addr = NULL;

            /* Find or Add Client Instance */
            addr = ogs_sbi_getaddr_from_uri(headers.target_apiroot);
            if (!addr) {
                ogs_error("Invalid Target-apiRoot [%s]",
                        headers.target_apiroot);
                ogs_sbi_http_hash_free(target.http.headers);
                ogs_free(target.h.uri);

                ogs_sbi_request_free(source);
                scp_conn_remove(conn);
                return OGS_ERROR;
            }

            client = ogs_sbi_client_find(addr);
            if (!client) {
                client = ogs_sbi_client_add(addr);
                ogs_assert(client);
                client->cb = client_cb;
            }
            OGS_SBI_SETUP_CLIENT(conn, client);

            ogs_freeaddrinfo(addr);

            /* Setup New URI */
            target.h.uri = ogs_msprintf("%s%s",
                                headers.target_apiroot, source->h.uri);
            ogs_assert(target.h.uri);
        }

        /* Send the HTTP Request with New URI and HTTP Headers */
        if (ogs_sbi_client_send_reqmem_persistent(
                    client, response_handler, &target, conn) != true) {
            ogs_error("ogs_sbi_client_send_request() failed");
            ogs_sbi_http_hash_free(target.http.headers);
            ogs_free(target.h.uri);

            ogs_sbi_request_free(source);
            scp_conn_remove(conn);
            return OGS_ERROR;
        }

        ogs_sbi_http_hash_free(target.http.headers);
        ogs_free(target.h.uri);

    } else if (headers.discovery) {
        scp_event_t *e = NULL;

        e = scp_event_new(OGS_EVENT_SBI_SERVER);
        ogs_assert(e);

        e->h.sbi.request = source;
        e->h.sbi.data = stream;

        rv = ogs_queue_push(ogs_app()->queue, e);
        if (rv != OGS_OK) {
            ogs_error("ogs_queue_push() failed:%d", (int)rv);
            ogs_sbi_request_free(source);
            ogs_event_free(e);
            return OGS_ERROR;
        }
    } else {
        scp_event_t *e = NULL;
        int rv;

        ogs_assert(source);
        ogs_assert(data);

        e = scp_event_new(OGS_EVENT_SBI_SERVER);
        ogs_assert(e);

        e->h.sbi.request = source;
        e->h.sbi.data = data;

        rv = ogs_queue_push(ogs_app()->queue, e);
        if (rv != OGS_OK) {
            ogs_error("ogs_queue_push() failed:%d", (int)rv);
            ogs_sbi_request_free(source);
            ogs_event_free(e);
            return OGS_ERROR;
        }
    }

    return OGS_OK;
}

static int response_handler(
        int status, ogs_sbi_response_t *response, void *data)
{
    scp_conn_t *conn = data;
    ogs_sbi_stream_t *stream = NULL;

    ogs_assert(conn);
    stream = conn->stream;
    ogs_assert(stream);

    if (status != OGS_OK) {

        ogs_log_message(
                status == OGS_DONE ? OGS_LOG_DEBUG : OGS_LOG_WARN, 0,
                "response_handler() failed [%d]", status);

        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                "response_handler() failed", NULL));

        scp_conn_remove(conn);

        return OGS_ERROR;
    }

    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));
    scp_conn_remove(conn);

    return OGS_OK;
}

static int client_cb(int status, ogs_sbi_response_t *response, void *data)
{
    scp_event_t *e = NULL;
    int rv;

    if (status != OGS_OK) {
        ogs_log_message(
                status == OGS_DONE ? OGS_LOG_DEBUG : OGS_LOG_WARN, 0,
                "client_cb() failed [%d]", status);
        return OGS_ERROR;
    }

    ogs_assert(response);

    e = scp_event_new(OGS_EVENT_SBI_CLIENT);
    ogs_assert(e);
    e->h.sbi.response = response;
    e->h.sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_sbi_response_free(response);
        ogs_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

bool scp_sbi_send_request(ogs_sbi_nf_instance_t *nf_instance, void *data)
{
    ogs_assert(nf_instance);

    return ogs_sbi_send_request(nf_instance, client_cb, data);
}

bool scp_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(scp_conn_t *conn, void *data),
        scp_conn_t *conn, ogs_sbi_stream_t *stream, void *data)
{
    ogs_sbi_xact_t *xact = NULL;

    ogs_assert(service_type);
    ogs_assert(conn);
    ogs_assert(stream);
    ogs_assert(build);

    xact = ogs_sbi_xact_add(
            &conn->sbi, service_type, discovery_option,
            (ogs_sbi_build_f)build, conn, data);
    if (!xact) {
        ogs_error("scp_sbi_discover_and_send() failed");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", NULL));
        return false;
    }

    xact->assoc_stream = stream;

    if (ogs_sbi_discover_and_send(xact, client_cb) != true) {
        ogs_error("scp_sbi_discover_and_send() failed");
        ogs_sbi_xact_remove(xact);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", NULL));
        return false;
    }

    return true;
}
