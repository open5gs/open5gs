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
static int discover_handler(
        int status, ogs_sbi_response_t *response, void *data);

static void copy_request(
        ogs_sbi_request_t *target, ogs_sbi_request_t *source,
        bool include_discovery);

int scp_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL, *nrf_instance = NULL;
    ogs_sbi_client_t *nrf_client = NULL, *next_scp = NULL;

    /* Initialize SELF NF instance */
    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);
    ogs_sbi_nf_fsm_init(nf_instance);

    /* Build NF instance information. It will be transmitted to NRF. */
    ogs_sbi_nf_instance_build_default(nf_instance);

    /*
     * If the SCP is running in Model D,
     * it can send NFRegister/NFStatusSubscribe messages to the NRF.
     */
    nrf_instance = ogs_sbi_self()->nrf_instance;
    nrf_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->nrf_instance);

    if (nrf_client) {

        /* Initialize NRF NF Instance */
        if (nrf_instance)
            ogs_sbi_nf_fsm_init(nrf_instance);
    }

    /* Check if Next-SCP's client */
    if (ogs_sbi_self()->discovery_config.delegated ==
            OGS_SBI_DISCOVERY_DELEGATED_AUTO) {
        next_scp = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);
    } else if (ogs_sbi_self()->discovery_config.delegated ==
            OGS_SBI_DISCOVERY_DELEGATED_YES) {
        next_scp = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);
        ogs_assert(next_scp);
    }

    /* If the SCP has an NRF client and does not delegate to Next-SCP */
    if (nrf_client && !next_scp) {

        /* Setup Subscription-Data */
        ogs_sbi_subscription_spec_add(OpenAPI_nf_type_AMF, NULL);
        ogs_sbi_subscription_spec_add(OpenAPI_nf_type_AUSF, NULL);
        ogs_sbi_subscription_spec_add(OpenAPI_nf_type_BSF, NULL);
        ogs_sbi_subscription_spec_add(OpenAPI_nf_type_NSSF, NULL);
        ogs_sbi_subscription_spec_add(OpenAPI_nf_type_PCF, NULL);
        ogs_sbi_subscription_spec_add(OpenAPI_nf_type_SMF, NULL);
        ogs_sbi_subscription_spec_add(OpenAPI_nf_type_UDM, NULL);
        ogs_sbi_subscription_spec_add(OpenAPI_nf_type_UDR, NULL);
    }

    if (ogs_sbi_server_start_all(request_handler) != OGS_OK)
        return OGS_ERROR;

    return OGS_OK;
}

void scp_sbi_close(void)
{
    ogs_sbi_client_stop_all();
    ogs_sbi_server_stop_all();
}

static int request_handler(ogs_sbi_request_t *request, void *data)
{
    int rv;
    ogs_hash_index_t *hi;
    ogs_sbi_client_t *client = NULL, *nrf_client = NULL, *next_scp = NULL;
    ogs_sbi_stream_t *stream = data;

    ogs_sbi_request_t scp_request;
    char *apiroot = NULL, *newuri = NULL;

    OpenAPI_nf_type_e target_nf_type = OpenAPI_nf_type_NULL;
    OpenAPI_nf_type_e requester_nf_type = OpenAPI_nf_type_NULL;
    ogs_sbi_discovery_option_t *discovery_option = NULL;
    ogs_sbi_service_type_e service_type = OGS_SBI_SERVICE_TYPE_NULL;
    bool discovery_presence = false;

    scp_assoc_t *assoc = NULL;
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    struct {
        char *target_apiroot;
        char *callback;
        char *nrf_uri;
    } headers = {
        NULL, NULL, NULL
    };

    scp_event_t *e = NULL;

    ogs_assert(request);
    ogs_assert(request->h.uri);
    ogs_assert(stream);

    /* Check if Next-SCP's client */
    if (ogs_sbi_self()->discovery_config.delegated ==
            OGS_SBI_DISCOVERY_DELEGATED_AUTO) {
        next_scp = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);
    } else if (ogs_sbi_self()->discovery_config.delegated ==
            OGS_SBI_DISCOVERY_DELEGATED_YES) {
        next_scp = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);
        ogs_assert(next_scp);
    }

    discovery_option = ogs_sbi_discovery_option_new();
    ogs_assert(discovery_option);

    /* Extract HTTP Header */
    for (hi = ogs_hash_first(request->http.headers);
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
        if (!strcasecmp(key, OGS_SBI_USER_AGENT)) {
            if (val) requester_nf_type = OpenAPI_nf_type_FromString(val);
        } else if (!strcasecmp(key, OGS_SBI_CUSTOM_TARGET_APIROOT)) {
            headers.target_apiroot = val;
        } else if (!strcasecmp(key, OGS_SBI_CUSTOM_CALLBACK)) {
            headers.callback = val;
        } else if (!strcasecmp(key, OGS_SBI_CUSTOM_NRF_URI)) {
            headers.nrf_uri = val;
        } else if (!strcasecmp(key, OGS_SBI_CUSTOM_DISCOVERY_TARGET_NF_TYPE)) {
            if (val) target_nf_type = OpenAPI_nf_type_FromString(val);
        } else if (!strcasecmp(key,
                    OGS_SBI_CUSTOM_DISCOVERY_REQUESTER_NF_TYPE)) {
            ogs_warn("Use User-Agent instead of Discovery-requester-nf-type");
        } else if (!strcasecmp(key,
                    OGS_SBI_CUSTOM_DISCOVERY_TARGET_NF_INSTANCE_ID)) {
            ogs_sbi_discovery_option_set_target_nf_instance_id(
                    discovery_option, val);
        } else if (!strcasecmp(key,
                    OGS_SBI_CUSTOM_DISCOVERY_REQUESTER_NF_INSTANCE_ID)) {
            ogs_sbi_discovery_option_set_requester_nf_instance_id(
                    discovery_option, val);
        } else if (!strcasecmp(key, OGS_SBI_CUSTOM_DISCOVERY_SERVICE_NAMES)) {
            if (val)
                ogs_sbi_discovery_option_parse_service_names(
                        discovery_option, val);

            /*
             * So, we'll use the first item in service-names list.
             *
             * TS29.500
             * 6.10 Support of Indirect Communication
             * 6.10.3 NF Discovery and Selection for indirect communication
             *        with Delegated Discovery
             * 6.10.3.2 Conveyance of NF Discovery Factors
             *
             * If the NF service consumer includes more than one service name
             * in the 3gpp-Sbi-Discovery-service-names header, the service name
             * corresponding to the service request shall be listed
             * as the first service name in the header.
             *
             * NOTE 3: The SCP can assume that the service request corresponds
             * to the first service name in the header.
             */
            if (discovery_option->num_of_service_names) {
                service_type = ogs_sbi_service_type_from_name(
                                    discovery_option->service_names[0]);
            }
        } else if (!strcasecmp(key,
                    OGS_SBI_CUSTOM_DISCOVERY_REQUESTER_FEATURES)) {
            if (val)
                discovery_option->requester_features =
                    ogs_uint64_from_string(val);
        } else if (!strcasecmp(key, OGS_SBI_SCHEME)) {
            /* ':scheme' will be automatically filled in later */
        } else if (!strcasecmp(key, OGS_SBI_AUTHORITY)) {
            /* ':authority' will be automatically filled in later */
        } else {
        }
    }

    /* Check if Discovery Parameter and Option */
    discovery_presence = false;

    if (!requester_nf_type) {
        ogs_error("[%s] No User-Agent", request->h.uri);

        ogs_sbi_discovery_option_free(discovery_option);
        return OGS_ERROR;
    }

    if (target_nf_type || service_type) {
        if (!target_nf_type || !service_type) {
            ogs_error("[%s] No Mandatory Discovery [%d:%d]",
                request->h.uri, target_nf_type, service_type);

            ogs_sbi_discovery_option_free(discovery_option);
            return OGS_ERROR;
        }

        if (target_nf_type == OpenAPI_nf_type_NRF)
            client = NF_INSTANCE_CLIENT(ogs_sbi_self()->nrf_instance);
        else {
            if (discovery_option && discovery_option->target_nf_instance_id) {
                nf_instance = ogs_sbi_nf_instance_find(
                        discovery_option->target_nf_instance_id);
                if (nf_instance) {
                    client = ogs_sbi_client_find_by_service_type(
                                nf_instance, service_type);
                    if (!client) {
                        ogs_error("[%s:%s] Cannot find client [%s:%s]",
                                OpenAPI_nf_type_ToString(nf_instance->nf_type),
                                nf_instance->id,
                                OpenAPI_nf_type_ToString(target_nf_type),
                                ogs_sbi_service_type_to_name(service_type));
                    }
                }
            }
        }

        discovery_presence = true;
    }

    /************************************
     * Send REQUEST message to the CLIENT
     ************************************/
    if (next_scp || headers.target_apiroot || client) {
        assoc = scp_assoc_add(stream);
        if (!assoc) {
            ogs_error("scp_assoc_add() failed");
            ogs_sbi_discovery_option_free(discovery_option);
            return OGS_ERROR;
        }

        if (next_scp) {
            /* Switch to the Next-SCP's client */
            client = next_scp;

            /* Client ApiRoot */
            apiroot = ogs_sbi_client_apiroot(client);
            ogs_assert(apiroot);

            /* Setup New URI */
            newuri = ogs_msprintf("%s%s", apiroot, request->h.uri);
            ogs_assert(newuri);

            ogs_free(apiroot);

        } else if (headers.target_apiroot) {
            bool rc;
            OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
            ogs_sockaddr_t *addr = NULL;

            /* Find or Add Client Instance */
            rc = ogs_sbi_getaddr_from_uri(
                    &scheme, &addr, headers.target_apiroot);
            if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
                ogs_error("Invalid Target-apiRoot [%s]",
                        headers.target_apiroot);

                ogs_sbi_discovery_option_free(discovery_option);
                scp_assoc_remove(assoc);

                return OGS_ERROR;
            }

            client = ogs_sbi_client_find(scheme, addr);
            if (!client) {
                client = ogs_sbi_client_add(scheme, addr);
                ogs_assert(client);
            }
            OGS_SBI_SETUP_CLIENT(assoc, client);
            ogs_freeaddrinfo(addr);

            /* Setup New URI */
            newuri = ogs_msprintf("%s%s",
                    headers.target_apiroot, request->h.uri);
            ogs_assert(newuri);

        } else if (client) {
            /* Client ApiRoot */
            apiroot = ogs_sbi_client_apiroot(client);
            ogs_assert(apiroot);

            /* Setup New URI */
            newuri = ogs_msprintf("%s%s", apiroot, request->h.uri);
            ogs_assert(newuri);

            ogs_free(apiroot);
        }

        /* Check assocation and client instance */
        ogs_assert(assoc);
        ogs_assert(client);

        /* Copy Request for sending SCP */
        copy_request(&scp_request, request, next_scp ? true : false);
        ogs_assert(scp_request.http.headers);

        /* Setup NEW URI */
        scp_request.h.uri = newuri;
        ogs_assert(scp_request.h.uri);

        /* Send the HTTP Request with New URI and HTTP Headers */
        if (ogs_sbi_client_send_request(
                    client, response_handler, &scp_request, assoc) != true) {
            ogs_error("ogs_sbi_client_send_request() failed");

            ogs_sbi_http_hash_free(scp_request.http.headers);
            ogs_free(scp_request.h.uri);
            ogs_sbi_discovery_option_free(discovery_option);
            scp_assoc_remove(assoc);

            return OGS_ERROR;
        }

        ogs_sbi_http_hash_free(scp_request.http.headers);
        ogs_free(scp_request.h.uri);
        ogs_sbi_discovery_option_free(discovery_option);

        return OGS_OK;
    }

    /*******************************
     * Send DISCOVERY message to NRF
     *******************************/
    if (discovery_presence == true) {
        ogs_sbi_request_t *nrf_request = NULL;

        assoc = scp_assoc_add(stream);
        if (!assoc) {
            ogs_error("scp_assoc_add() failed");
            ogs_sbi_discovery_option_free(discovery_option);
            return OGS_ERROR;
        }

        if (headers.nrf_uri) {
            char *key = NULL;
            char *nnrf_disc = NULL;
            char *nnrf_nfm = NULL;
            char *nnrf_oauth2 = NULL;

            char *tmp = NULL, *p = NULL;
            char *v_start = NULL, *v_end = NULL;

            tmp = ogs_strdup(headers.nrf_uri);
            ogs_assert(tmp);

            for (key = ogs_strtok_r(tmp, ": ", &p);
                    key != NULL; key = ogs_strtok_r(NULL, ": ", &p)) {

                v_start = v_end = NULL;

                while (*p) {
                    if (*p == ';') {
                        if ((v_start && v_end) || !v_start) {
                            p++;
                            break;
                        }
                    } else if (*p == '"') {
                        if (!v_start) v_start = p+1;
                        else if (!v_end) v_end = p;
                    }
                    p++;
                }

                if (v_start && v_end) {
                    SWITCH(key)
                    CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)
                        nnrf_nfm = ogs_strndup(v_start, v_end-v_start);
                        break;
                    CASE(OGS_SBI_SERVICE_NAME_NNRF_DISC)
                        nnrf_disc = ogs_strndup(v_start, v_end-v_start);
                        break;
                    CASE(OGS_SBI_SERVICE_NAME_NNRF_OAUTH2)
                        nnrf_oauth2 = ogs_strndup(v_start, v_end-v_start);
                        break;
                    DEFAULT
                    END
                }
            }

            ogs_free(tmp);

            /* Find or Add Client Instance */
            if (nnrf_disc) {
                bool rc;
                OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
                ogs_sockaddr_t *addr = NULL;

                rc = ogs_sbi_getaddr_from_uri(&scheme, &addr, nnrf_disc);
                if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
                    ogs_error("Invalid nnrf-disc [%s]", nnrf_disc);

                    ogs_sbi_discovery_option_free(discovery_option);
                    scp_assoc_remove(assoc);

                    return OGS_ERROR;
                }

                nrf_client = ogs_sbi_client_find(scheme, addr);
                if (!nrf_client) {
                    nrf_client = ogs_sbi_client_add(scheme, addr);
                    ogs_assert(nrf_client);
                }
                OGS_SBI_SETUP_CLIENT(assoc, nrf_client);
                ogs_freeaddrinfo(addr);
            }

            if (nnrf_nfm) ogs_free(nnrf_nfm);
            if (nnrf_disc) ogs_free(nnrf_disc);
            if (nnrf_oauth2) ogs_free(nnrf_oauth2);
        }

        if (!nrf_client) {
            nrf_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->nrf_instance);
            if (!nrf_client) {
                ogs_error("No NRF");

                ogs_sbi_discovery_option_free(discovery_option);
                scp_assoc_remove(assoc);

                return OGS_ERROR;
            }
        }

        /* Store request and service-type in association context */
        assoc->request = request;
        ogs_assert(assoc->request);
        assoc->service_type = service_type;
        ogs_assert(assoc->service_type);
        assoc->requester_nf_type = requester_nf_type;
        ogs_assert(assoc->requester_nf_type);

        ogs_assert(target_nf_type);
        ogs_assert(discovery_option);

        nrf_request = ogs_nnrf_disc_build_discover(
                    target_nf_type, requester_nf_type, discovery_option);
        if (!nrf_request) {
            ogs_error("ogs_nnrf_disc_build_discover() failed");

            ogs_sbi_discovery_option_free(discovery_option);
            scp_assoc_remove(assoc);

            return OGS_ERROR;
        }

        if (false == ogs_sbi_client_send_request(
                    nrf_client, discover_handler, nrf_request, assoc)) {
            ogs_error("ogs_sbi_client_send_request() failed");

            scp_assoc_remove(assoc);

            ogs_sbi_request_free(nrf_request);
            ogs_sbi_discovery_option_free(discovery_option);

            return OGS_ERROR;
        }

        ogs_sbi_request_free(nrf_request);
        ogs_sbi_discovery_option_free(discovery_option);

        return OGS_OK;
    }

    ogs_sbi_discovery_option_free(discovery_option);

    /***************************************
     * Receive NOTIFICATION message from NRF
     ***************************************/
    ogs_assert(request);
    ogs_assert(data);

    e = scp_event_new(OGS_EVENT_SBI_SERVER);
    ogs_assert(e);

    e->h.sbi.request = request;
    e->h.sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);

        ogs_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int response_handler(
        int status, ogs_sbi_response_t *response, void *data)
{
    scp_assoc_t *assoc = data;
    ogs_sbi_stream_t *stream = NULL;

    ogs_assert(assoc);
    stream = assoc->stream;
    ogs_assert(stream);

    if (status != OGS_OK) {

        ogs_log_message(
                status == OGS_DONE ? OGS_LOG_DEBUG : OGS_LOG_WARN, 0,
                "response_handler() failed [%d]", status);

        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                "response_handler() failed", NULL));

        scp_assoc_remove(assoc);

        return OGS_ERROR;
    }

    ogs_assert(response);

    if (assoc->nf_service_producer) {
        if (assoc->nf_service_producer->id)
            ogs_sbi_header_set(response->http.headers,
                OGS_SBI_CUSTOM_PRODUCER_ID, assoc->nf_service_producer->id);
        else
            ogs_error("No NF-Instance ID");
    }

    ogs_expect(true == ogs_sbi_server_send_response(stream, response));
    scp_assoc_remove(assoc);

    return OGS_OK;
}

static int discover_handler(
        int status, ogs_sbi_response_t *response, void *data)
{
    int rv;
    char *strerror = NULL;
    ogs_sbi_message_t message;

    scp_assoc_t *assoc = data;
    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_service_type_e service_type = OGS_SBI_SERVICE_TYPE_NULL;
    OpenAPI_nf_type_e requester_nf_type = OpenAPI_nf_type_NULL;

    ogs_sbi_request_t scp_request;
    char *apiroot = NULL;

    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_client_t *client = NULL, *next_scp = NULL;

    ogs_assert(assoc);
    stream = assoc->stream;
    ogs_assert(stream);
    request = assoc->request;
    ogs_assert(request);
    service_type = assoc->service_type;
    ogs_assert(service_type);
    requester_nf_type = assoc->requester_nf_type;
    ogs_assert(requester_nf_type);

    if (status != OGS_OK) {

        ogs_log_message(
                status == OGS_DONE ? OGS_LOG_DEBUG : OGS_LOG_WARN, 0,
                "response_handler() failed [%d]", status);

        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                "response_handler() failed", NULL));

        scp_assoc_remove(assoc);

        return OGS_ERROR;
    }

    ogs_assert(response);

    rv = ogs_sbi_parse_response(&message, response);
    if (rv != OGS_OK) {
        strerror = ogs_msprintf("cannot parse HTTP response");
        goto cleanup;
    }

    if (message.res_status != OGS_SBI_HTTP_STATUS_OK) {
        strerror = ogs_msprintf("NF-Discover failed [%d]", message.res_status);
        goto cleanup;
    }

    if (!message.SearchResult) {
        strerror = ogs_msprintf("No SearchResult");
        goto cleanup;
    }

    ogs_nnrf_disc_handle_nf_discover_search_result(message.SearchResult);

    nf_instance = ogs_sbi_nf_instance_find_by_service_type(
                    service_type, requester_nf_type);
    if (!nf_instance) {
        strerror = ogs_msprintf("(NF discover) No NF-Instance [%s:%s]",
                    ogs_sbi_service_type_to_name(service_type),
                    OpenAPI_nf_type_ToString(requester_nf_type));

        goto cleanup;
    }

    client = ogs_sbi_client_find_by_service_type(nf_instance, service_type);
    if (!client) {
        strerror = ogs_msprintf("(NF discover) No client [%s:%s]",
                    ogs_sbi_service_type_to_name(service_type),
                    OpenAPI_nf_type_ToString(requester_nf_type));

        goto cleanup;
    }

    /* Copy Request for sending SCP */
    copy_request(&scp_request, request, false);
    ogs_assert(scp_request.http.headers);

    /* Check if Next-SCP's client */
    next_scp = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);
    if (next_scp) {
        apiroot = ogs_sbi_client_apiroot(client);
        ogs_assert(apiroot);

        ogs_sbi_header_set(scp_request.http.headers,
                OGS_SBI_CUSTOM_TARGET_APIROOT, apiroot);

        ogs_free(apiroot);

        /* Switch to the Next-SCP's client */
        client = next_scp;
    }

    /* Client ApiRoot */
    apiroot = ogs_sbi_client_apiroot(client);
    ogs_assert(apiroot);

    /* Setup New URI */
    scp_request.h.uri = ogs_msprintf("%s%s", apiroot, request->h.uri);
    ogs_assert(scp_request.h.uri);

    ogs_free(apiroot);

    /* Store NF Service Producer */
    assoc->nf_service_producer = nf_instance;
    ogs_assert(assoc->nf_service_producer);

    /* Send the HTTP Request with New URI and HTTP Headers */
    if (ogs_sbi_client_send_request(
                client, response_handler, &scp_request, assoc) != true) {
        ogs_error("ogs_sbi_client_send_request() failed");
        strerror = ogs_msprintf("ogs_sbi_client_send_request() failed");

        ogs_sbi_http_hash_free(scp_request.http.headers);
        ogs_free(scp_request.h.uri);

        goto cleanup;
    }

    ogs_sbi_http_hash_free(scp_request.http.headers);
    ogs_free(scp_request.h.uri);

    ogs_sbi_response_free(response);
    ogs_sbi_message_free(&message);

    return OGS_OK;

cleanup:
    ogs_assert(strerror);
    ogs_error("%s", strerror);

    ogs_assert(true ==
        ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST, NULL, strerror, NULL));

    ogs_free(strerror);

    scp_assoc_remove(assoc);

    ogs_sbi_response_free(response);
    ogs_sbi_message_free(&message);

    return OGS_ERROR;
}

static void copy_request(
        ogs_sbi_request_t *target, ogs_sbi_request_t *source,
        bool next_scp)
{
    ogs_hash_index_t *hi;

    ogs_assert(source);
    ogs_assert(target);

    memset(target, 0, sizeof(*target));

    /* HTTP method/params/content */
    target->h.method = source->h.method;
    target->http.params = source->http.params;
    target->http.content = source->http.content;
    target->http.content_length = source->http.content_length;

    /* HTTP Headers
     *
     * To remove the followings,
     *   Scheme - https
     *   Authority - scp.open5gs.org
     */
    target->http.headers = ogs_hash_make();
    ogs_assert(target->http.headers);

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
        if (next_scp == false &&
            !strcasecmp(key, OGS_SBI_CUSTOM_TARGET_APIROOT)) {
        } else if (next_scp == false &&
            !strncasecmp(key, OGS_SBI_CUSTOM_DISCOVERY_COMMON,
                strlen(OGS_SBI_CUSTOM_DISCOVERY_COMMON))) {
        } else if (!strcasecmp(key, OGS_SBI_SCHEME)) {
        } else if (!strcasecmp(key, OGS_SBI_AUTHORITY)) {
        } else {
            ogs_sbi_header_set(target->http.headers, key, val);
        }
    }
}
