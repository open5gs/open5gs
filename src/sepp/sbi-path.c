/*
 * Copyright (C) 2023-2024 by Sukchan Lee <acetcom@gmail.com>
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

static void copy_request(
        ogs_sbi_request_t *target, ogs_sbi_request_t *source,
        bool do_not_remove_custom_header);

int sepp_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    /* Initialize SELF NF instance */
    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);
    ogs_sbi_nf_fsm_init(nf_instance);

    /* Build NF instance information. It will be transmitted to NRF. */
    ogs_sbi_nf_instance_build_default(nf_instance);

    /* Initialize NRF NF Instance */
    nf_instance = ogs_sbi_self()->nrf_instance;
    if (nf_instance)
        ogs_sbi_nf_fsm_init(nf_instance);

    if (ogs_sbi_server_start_all(request_handler) != OGS_OK)
        return OGS_ERROR;

    return OGS_OK;
}

void sepp_sbi_close(void)
{
    ogs_sbi_client_stop_all();
    ogs_sbi_server_stop_all();
}

bool sepp_n32c_handshake_send_security_capability_request(
        sepp_node_t *sepp_node, bool none)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(sepp_node);
    client = sepp_node->client;
    if (!client) {
        ogs_error("No Client");
        return false;
    }

    request = sepp_n32c_handshake_build_security_capability_request(
            sepp_node, none);
    if (!request) {
        ogs_error("sepp_n32c_handshake_build_exchange_capability() failed");
        return false;
    }

    rc = ogs_sbi_client_send_request(
            client, ogs_sbi_client_handler, request, sepp_node);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}

void sepp_n32c_handshake_send_security_capability_response(
        sepp_node_t *sepp_node, ogs_sbi_stream_t *stream)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_sec_negotiate_rsp_data_t SecNegotiateRspData;

    OpenAPI_list_t *PlmnIdList = NULL;
    OpenAPI_plmn_id_t *PlmnId = NULL;

    int i;
    OpenAPI_lnode_t *node = NULL;

    ogs_assert(sepp_self()->sender);
    ogs_assert(sepp_node);
    ogs_assert(stream);

    memset(&SecNegotiateRspData, 0, sizeof(SecNegotiateRspData));
    SecNegotiateRspData.sender = sepp_self()->sender;
    SecNegotiateRspData.selected_sec_capability =
        sepp_node->negotiated_security_scheme;

    if (SecNegotiateRspData.selected_sec_capability !=
            OpenAPI_security_capability_NONE) {
        if (sepp_node->target_apiroot_supported == true) {
            SecNegotiateRspData.is__3_gpp_sbi_target_api_root_supported = true;
            SecNegotiateRspData._3_gpp_sbi_target_api_root_supported = 1;
        }
    }

    PlmnIdList = OpenAPI_list_create();
    ogs_assert(PlmnIdList);

    for (i = 0; i < ogs_local_conf()->num_of_serving_plmn_id; i++) {
        PlmnId = ogs_sbi_build_plmn_id(&ogs_local_conf()->serving_plmn_id[i]);
        ogs_assert(PlmnId);
        OpenAPI_list_add(PlmnIdList, PlmnId);
    }

    if (PlmnIdList->count)
        SecNegotiateRspData.plmn_id_list = PlmnIdList;
    else
        OpenAPI_list_free(PlmnIdList);

    SecNegotiateRspData.supported_features =
        ogs_uint64_to_string(sepp_node->supported_features);
    ogs_assert(SecNegotiateRspData.supported_features);

    memset(&sendmsg, 0, sizeof(sendmsg));
    sendmsg.SecNegotiateRspData = &SecNegotiateRspData;

    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    OpenAPI_list_for_each(SecNegotiateRspData.plmn_id_list, node) {
        PlmnId = node->data;
        if (PlmnId)
            ogs_sbi_free_plmn_id(PlmnId);
    }
    OpenAPI_list_free(SecNegotiateRspData.plmn_id_list);
    if (SecNegotiateRspData.supported_features)
        ogs_free(SecNegotiateRspData.supported_features);
}

static int request_handler(ogs_sbi_request_t *request, void *data)
{
    int rv;
    ogs_hash_index_t *hi;
    ogs_sbi_client_t *client = NULL, *scp_client = NULL;
    ogs_sbi_stream_t *stream = data;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_request_t sepp_request;
    char *apiroot = NULL, *newuri = NULL;

    sepp_assoc_t *assoc = NULL;

    struct {
        char *target_apiroot;
        char *callback;
        char *nrf_uri;
    } headers = {
        NULL, NULL, NULL
    };

    sepp_event_t *e = NULL;

    ogs_assert(request);
    ogs_assert(request->h.uri);

    stream_id = OGS_POINTER_TO_UINT(data);
    ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
            stream_id <= OGS_MAX_POOL_ID);

    stream = ogs_sbi_stream_find_by_id(stream_id);
    if (!stream) {
        ogs_error("STREAM has already been removed [%d]", stream_id);
        return OGS_ERROR;
    }

    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

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
        if (!strcasecmp(key, OGS_SBI_CUSTOM_TARGET_APIROOT)) {
            headers.target_apiroot = val;
        }
    }

    if (headers.target_apiroot) {
        bool rc;
        sepp_node_t *sepp_node = NULL;
        bool do_not_remove_custom_header;

        assoc = sepp_assoc_add(stream_id);
        if (!assoc) {
            ogs_error("sepp_assoc_add() failed");
            return OGS_ERROR;
        }

        do_not_remove_custom_header = true;

        if (ogs_sbi_fqdn_in_vplmn(headers.target_apiroot) == true) {
            uint16_t mcc = 0, mnc = 0;

            if (server->interface) {
                ogs_error("[DROP] Peer SEPP is using "
                        "the wrong interface[%s]", server->interface);
                sepp_assoc_remove(assoc);
                return OGS_ERROR;
            }

            mcc = ogs_plmn_id_mcc_from_fqdn(headers.target_apiroot);
            ogs_assert(mcc);
            mnc = ogs_plmn_id_mnc_from_fqdn(headers.target_apiroot);
            ogs_assert(mnc);

            /*
             * Different PLMN : FROM c-SEPP TO p-SEPP
             */
            sepp_node = sepp_node_find_by_plmn_id(mcc, mnc);
            if (!sepp_node) {
                ogs_error("Cannot find SEPP Peer Node [%s:%d:%d]",
                        headers.target_apiroot, mcc, mnc);
                sepp_assoc_remove(assoc);
                return OGS_ERROR;
            }

            client = NF_INSTANCE_CLIENT(&sepp_node->n32f);
            if (!client) {
                client = NF_INSTANCE_CLIENT(sepp_node);
                if (!client) {
                    ogs_error("No Client in SEPP Peer Node [%s:%d:%d]",
                            headers.target_apiroot, mcc, mnc);
                    sepp_assoc_remove(assoc);
                    return OGS_ERROR;
                }
            }

            /* Client ApiRoot */
            apiroot = ogs_sbi_client_apiroot(client);
            ogs_assert(apiroot);

        } else {
            /*
             * Same PLMN : From p-SEPP to NF via SCP
             */
            OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
            char *fqdn = NULL;
            uint16_t fqdn_port = 0;
            ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

            if (server->interface == NULL) {
                if (ogs_sbi_server_first_by_interface(
                            OGS_SBI_INTERFACE_NAME_SEPP) ||
                    ogs_sbi_server_first_by_interface(
                            OGS_SBI_INTERFACE_NAME_N32F)) {
                    ogs_error("[DROP] Peer SEPP is using "
                            "the wrong interface[%s]", server->interface);
                    sepp_assoc_remove(assoc);
                    return OGS_ERROR;
                }
            } else {
                if (strcmp(server->interface,
                            OGS_SBI_INTERFACE_NAME_SEPP) == 0) {
                    if (ogs_sbi_server_first_by_interface(
                                OGS_SBI_INTERFACE_NAME_N32F)) {
                        ogs_error("[DROP] Peer SEPP is using "
                                "the wrong interface[%s]", server->interface);
                        sepp_assoc_remove(assoc);
                        return OGS_ERROR;
                    }
                }
            }

            /* Find or Add Client Instance */
            rc = ogs_sbi_getaddr_from_uri(
                    &scheme, &fqdn, &fqdn_port, &addr, &addr6,
                    headers.target_apiroot);
            if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
                ogs_error("Invalid Target-apiRoot [%s]",
                        headers.target_apiroot);

                sepp_assoc_remove(assoc);
                return OGS_ERROR;
            }

            client = ogs_sbi_client_find(
                    scheme, fqdn, fqdn_port, addr, addr6);
            if (!client) {
                client = ogs_sbi_client_add(
                        scheme, fqdn, fqdn_port, addr, addr6);
                ogs_assert(client);
            }
            OGS_SBI_SETUP_CLIENT(assoc, client);

            ogs_free(fqdn);
            ogs_freeaddrinfo(addr);
            ogs_freeaddrinfo(addr6);

            /* Get SCP client */
            scp_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);

            /* Client ApiRoot */
            if (scp_client) {
                apiroot = ogs_sbi_client_apiroot(scp_client);
                ogs_assert(apiroot);

                /* Switch to the SCP's client */
                client = scp_client;
            } else {
                apiroot = ogs_sbi_client_apiroot(client);
                ogs_assert(apiroot);

                /* Remove Target-apiRoot */
                do_not_remove_custom_header = false;
            }
        }

        /* Setup New URI */
        newuri = ogs_msprintf("%s%s", apiroot, request->h.uri);
        ogs_assert(newuri);

        ogs_free(apiroot);

        /* Copy Request for SEPP request */
        copy_request(&sepp_request, request, do_not_remove_custom_header);
        ogs_assert(sepp_request.http.headers);

        /* Set New URI to SEPP request */
        sepp_request.h.uri = newuri;
        ogs_assert(sepp_request.h.uri);

        /* Send the HTTP Request with New URI and HTTP Headers */
        if (scp_client) {
            rc = ogs_sbi_client_send_via_scp_or_sepp(
                    scp_client, response_handler, &sepp_request, assoc);
            ogs_expect(rc == true);
        } else {
            rc = ogs_sbi_client_send_request(
                    client, response_handler, &sepp_request, assoc);
            ogs_expect(rc == true);
        }

        if (rc == false) {
            ogs_error("ogs_sbi_send_request_to_client() failed");

            ogs_sbi_http_hash_free(sepp_request.http.headers);
            ogs_free(sepp_request.h.uri);
            sepp_assoc_remove(assoc);

            return OGS_ERROR;
        }

        ogs_sbi_http_hash_free(sepp_request.http.headers);
        ogs_free(sepp_request.h.uri);

        return OGS_OK;
    }

    /***************************************
     * Receive NOTIFICATION message from NRF
     ***************************************/
    ogs_assert(request);
    ogs_assert(data);

    if (server->interface &&
        strcmp(server->interface, OGS_SBI_INTERFACE_NAME_N32F) == 0) {
        ogs_error("[DROP] Peer SEPP is using the wrong interface[%s]",
                server->interface);
        return OGS_ERROR;
    }

    e = sepp_event_new(OGS_EVENT_SBI_SERVER);
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
    sepp_assoc_t *assoc = data;
    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;

    ogs_assert(assoc);

    stream_id = assoc->stream_id;
    ogs_assert(stream_id >= OGS_MIN_POOL_ID && stream_id <= OGS_MAX_POOL_ID);
    stream = ogs_sbi_stream_find_by_id(stream_id);

    if (status != OGS_OK) {

        ogs_log_message(
                status == OGS_DONE ? OGS_LOG_DEBUG : OGS_LOG_WARN, 0,
                "response_handler() failed [%d]", status);

        sepp_assoc_remove(assoc);

        if (stream) {
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                    "response_handler() failed", NULL, NULL));
        } else
            ogs_error("STREAM has already been removed [%d]", stream_id);

        return OGS_ERROR;
    }

    ogs_assert(response);

    sepp_assoc_remove(assoc);

    if (!stream) {
        ogs_error("STREAM has already been removed [%d]", stream_id);
        ogs_sbi_response_free(response);
        return OGS_ERROR;
    }
    ogs_expect(true == ogs_sbi_server_send_response(stream, response));

    return OGS_OK;
}

static void copy_request(
        ogs_sbi_request_t *target, ogs_sbi_request_t *source,
        bool do_not_remove_custom_header)
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
     *   Authority - sepp.open5gs.org
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
        if (do_not_remove_custom_header == false &&
            !strcasecmp(key, OGS_SBI_CUSTOM_TARGET_APIROOT)) {
        } else if (do_not_remove_custom_header == false &&
            !strncasecmp(key, OGS_SBI_CUSTOM_DISCOVERY_COMMON,
                strlen(OGS_SBI_CUSTOM_DISCOVERY_COMMON))) {
        } else if (!strcasecmp(key, OGS_SBI_SCHEME)) {
        } else if (!strcasecmp(key, OGS_SBI_AUTHORITY)) {
        } else {
            ogs_sbi_header_set(target->http.headers, key, val);
        }
    }
}
