/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

static int sepp_discover_handler(
        int status, ogs_sbi_response_t *response, void *data);

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
    ogs_pool_id_t xact_id = 0;
    ogs_sbi_object_t *sbi_object = NULL;
    ogs_sbi_service_type_e service_type = OGS_SBI_SERVICE_TYPE_NULL;
    ogs_sbi_discovery_option_t *discovery_option = NULL;
    OpenAPI_nf_type_e target_nf_type = OpenAPI_nf_type_NULL;
    OpenAPI_nf_type_e requester_nf_type = OpenAPI_nf_type_NULL;

    ogs_hash_index_t *hi = NULL;
    char *producer_id = NULL;

    xact_id = OGS_POINTER_TO_UINT(data);
    ogs_assert(xact_id >= OGS_MIN_POOL_ID && xact_id <= OGS_MAX_POOL_ID);

    xact = ogs_sbi_xact_find_by_id(xact_id);
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
    ogs_assert(target_nf_type);
    requester_nf_type = xact->requester_nf_type;
    ogs_assert(requester_nf_type);

    discovery_option = xact->discovery_option;

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

            switch (target_nf_type) {
            case OpenAPI_nf_type_SMF:
                if (discovery_option &&
                    discovery_option->num_of_snssais && discovery_option->dnn &&
                    discovery_option->tai_presence == true) {
    /*
     * If we assume that SMF is executed first and then AMF is executed,
     * AMF will not have SMF information, so it needs to discover SMF
     * through NFDiscovery instead of the subscription notification.
     *
     * Let's assume that in smfInfo, TAC is set to 1 and 2, and two SMFs are
     * executed. In this case, TAI will be added to the discovery option and
     * will be performed during the NFDiscovery process.
     *
     * If the first SMF is discovered with TAC 1 in conjunction with SCP,
     * AMF will remember this SMF through Producer-Id. However, if the second
     * SMF with TAC 2 is discovered, the previously discovered SMF with TAC 1
     * will be selected.
     *
     * Therefore, to avoid such a situation, we reflect the contents of
     * the discovery option in NFProfile. For SMF, we record the s_nssai, dnn,
     * and tai information in the ogs_sbi_smf_info_t structure, which is created
     * as ogs_sbi_nf_info_t. Then, when we try to find the second SMF
     * with TAC 2, we compare these values in the amf_sbi_select_nf() function,
     * allowing us to discover a new SMF.
     */
                    ogs_sbi_nf_info_t *nf_info = NULL;
                    ogs_sbi_smf_info_t *smf_info = NULL;

                    nf_info = ogs_sbi_nf_info_add(
                            &nf_instance->nf_info_list, OpenAPI_nf_type_SMF);
                    ogs_assert(nf_info);

                    smf_info = &nf_info->smf;
                    ogs_assert(smf_info);

                    smf_info->slice[0].dnn[0] =
                        ogs_strdup(discovery_option->dnn);
                    ogs_assert(smf_info->slice[0].dnn[0]);
                    smf_info->slice[0].num_of_dnn++;

                    memcpy(&smf_info->slice[0].s_nssai,
                            &discovery_option->snssais[0],
                            sizeof(ogs_s_nssai_t));
                    smf_info->num_of_slice++;

                    memcpy(&smf_info->nr_tai[0],
                            &discovery_option->tai, sizeof(ogs_5gs_tai_t));
                    smf_info->num_of_nr_tai++;
                }
                break;
            default:
                break;
            }

            ogs_sbi_nf_fsm_init(nf_instance);

            ogs_info("[%s] (SCP-discover) NF registered [%s]",
                    nf_instance->nf_type ?
                        OpenAPI_nf_type_ToString(nf_instance->nf_type) : "NULL",
                    nf_instance->id);
        } else {
            ogs_warn("[%s] (SCP-discover) NF has already been added [%s]",
                    OpenAPI_nf_type_ToString(nf_instance->nf_type),
                    nf_instance->id);
            if (!OGS_FSM_CHECK(&nf_instance->sm, ogs_sbi_nf_state_registered)) {
                ogs_error("[%s] (SCP-discover) NF invalid state [%s]",
                        OpenAPI_nf_type_ToString(nf_instance->nf_type),
                        nf_instance->id);
            }
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
    if (ogs_sbi_self()->client_delegated_config.nrf.disc ==
            OGS_SBI_CLIENT_DELEGATED_AUTO) {
        scp_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);
    } else if (ogs_sbi_self()->client_delegated_config.nrf.disc ==
            OGS_SBI_CLIENT_DELEGATED_YES) {
        scp_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);
        ogs_assert(scp_client);
    }

    /* Target NF-Instance */
    nf_instance = OGS_SBI_GET_NF_INSTANCE(
            sbi_object->service_type_array[service_type]);
    ogs_debug("OGS_SBI_GET_NF_INSTANCE [nf_instance:%p,service_name:%s]",
            nf_instance, ogs_sbi_service_type_to_name(service_type));
    if (!nf_instance) {
        nf_instance = ogs_sbi_nf_instance_find_by_discovery_param(
                        target_nf_type, requester_nf_type, discovery_option);
        ogs_debug("ogs_sbi_nf_instance_find_by_discovery_param() "
                "[nf_instance:%p,service_name:%s]",
                nf_instance, ogs_sbi_service_type_to_name(service_type));
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
        char *fqdn = NULL;
        uint16_t fqdn_port = 0;
        ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

        rc = ogs_sbi_getaddr_from_uri(
                &scheme, &fqdn, &fqdn_port, &addr, &addr6, request->h.uri);
        if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
            ogs_error("Invalid URL [%s]", request->h.uri);
            return OGS_ERROR;
        }
        client = ogs_sbi_client_find(scheme, fqdn, fqdn_port, addr, addr6);

        ogs_free(fqdn);
        ogs_freeaddrinfo(addr);
        ogs_freeaddrinfo(addr6);

        if (!client) {
            ogs_fatal("No Client : [%s]", request->h.uri);
            ogs_assert_if_reached();
        }
    }

    if (scp_client) {
        /*************************
         * INDIRECT COMMUNICATION
         *************************/
        build_default_discovery_parameter(
            request, service_type, discovery_option);

        if (client) {
            /*
             * If `client` instance is available,
             * 3gpp-Sbi-Target-apiRoot is added to HTTP header.
             */
            apiroot = ogs_sbi_client_apiroot(client);
            ogs_assert(apiroot);

            ogs_debug("apiroot [%s]", apiroot);
            ogs_sbi_header_set(request->http.headers,
                    OGS_SBI_CUSTOM_TARGET_APIROOT, apiroot);

            ogs_free(apiroot);

            rc = ogs_sbi_client_send_via_scp_or_sepp(
                    scp_client, ogs_sbi_client_handler, request,
                    OGS_UINT_TO_POINTER(xact->id));
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
                ogs_debug("target_nf_instance_id [%s]",
                        discovery_option->target_nf_instance_id);
                ogs_sbi_header_set(request->http.headers,
                        OGS_SBI_CUSTOM_DISCOVERY_TARGET_NF_INSTANCE_ID,
                        discovery_option->target_nf_instance_id);
            } else if (nf_instance && nf_instance->id) {
                ogs_sbi_header_set(request->http.headers,
                        OGS_SBI_CUSTOM_DISCOVERY_TARGET_NF_INSTANCE_ID,
                        nf_instance->id);
                ogs_debug("nf_instance->id [%s]", nf_instance->id);
            }

            if (discovery_option && discovery_option->num_of_snssais) {
                bool rc = false;
                char *v = ogs_sbi_discovery_option_build_snssais(
                        discovery_option);
                ogs_expect(v);

                if (v) {
                    char *encoded = ogs_sbi_url_encode(v);
                    ogs_expect(encoded);
                    ogs_debug("snssai [%s]", v);

                    if (encoded) {
            /*
             * In http.params, the CURL library automatically encodes the URL.
             * http.headers implements open5gs to directly encode URLs.
             *
             * Since it is http.headers,
             * we need to encode `v` using ogs_sbi_url_encode();
             */
                        ogs_sbi_header_set(request->http.headers,
                                OGS_SBI_CUSTOM_DISCOVERY_SNSSAIS, encoded);
                        ogs_free(encoded);

                        rc = true;
                    }
                    ogs_free(v);
                }

                if (rc == false)
                    ogs_error("build failed: snssais(%d)[SST:%d SD:0x%x]",
                                discovery_option->num_of_snssais,
                                discovery_option->snssais[0].sst,
                                discovery_option->snssais[0].sd.v);
            }

            if (discovery_option && discovery_option->dnn) {
                ogs_debug("dnn [%s]", discovery_option->dnn);
                ogs_sbi_header_set(request->http.headers,
                        OGS_SBI_CUSTOM_DISCOVERY_DNN, discovery_option->dnn);
            }

            if (discovery_option && discovery_option->tai_presence) {
                bool rc = false;
                char *v = ogs_sbi_discovery_option_build_tai(discovery_option);
                ogs_expect(v);


                if (v) {
                    char *encoded = ogs_sbi_url_encode(v);
                    ogs_expect(encoded);
                    ogs_debug("tai [%s]", v);

                    if (encoded) {
                        ogs_sbi_header_set(request->http.headers,
                                OGS_SBI_CUSTOM_DISCOVERY_TAI, encoded);
                        ogs_free(encoded);

                        rc = true;
                    }
                    ogs_free(v);
                }

                if (rc == false)
                    ogs_error("build failed: tai[PLMN_ID:%06x,TAC:%d]",
                                ogs_plmn_id_hexdump(
                                    &discovery_option->tai.plmn_id),
                                discovery_option->tai.tac.v);
            }

            if (discovery_option && discovery_option->guami_presence) {
                bool rc = false;
                char *v = ogs_sbi_discovery_option_build_guami(discovery_option);
                ogs_expect(v);

                if (v) {
                    char *encoded = ogs_sbi_url_encode(v);
                    ogs_expect(encoded);
                    ogs_debug("guami [%s]", v);

                    if (encoded) {
                        ogs_sbi_header_set(request->http.headers,
                                OGS_SBI_CUSTOM_DISCOVERY_GUAMI, encoded);
                        ogs_free(encoded);

                        rc = true;
                    }
                    ogs_free(v);
                }

                if (rc == false)
                    ogs_error("build failed: guami[PLMN_ID:%06x,AMF_ID:%x]",
                                ogs_plmn_id_hexdump(
                                    &discovery_option->guami.plmn_id),
                                ogs_amf_id_hexdump(
                                    &discovery_option->guami.amf_id));
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

            rc = ogs_sbi_client_send_via_scp_or_sepp(
                    scp_client, client_discover_cb, request,
                    OGS_UINT_TO_POINTER(xact->id));
            ogs_expect(rc == true);
            return (rc == true) ? OGS_OK : OGS_ERROR;
        }

    } else if (client) {
        /***********************
         * DIRECT COMMUNICATION
         ***********************/

        /* If `client` instance is available, use direct communication */
        rc = ogs_sbi_send_request_to_client(
                client, ogs_sbi_client_handler, request,
                OGS_UINT_TO_POINTER(xact->id));
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
                client, ogs_sbi_client_handler, request,
                OGS_UINT_TO_POINTER(xact->id));
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
            ogs_sbi_xact_remove(xact);
            return false;
        }
    } else {

        /*********************************************************
         *
         * DEPRECATED
         *
         ********************************************************/

        ogs_fatal("[%s] %s", request->h.method, request->h.uri);
        ogs_assert_if_reached();

#if 0
        bool rc;
        OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
        char *fqdn = NULL;
        uint16_t fqdn_port = 0;
        ogs_sockaddr_t *addr = NULL, *addr6 = NULL;
        char buf[OGS_ADDRSTRLEN];

        rc = ogs_sbi_getaddr_from_uri(
                &scheme, &fqdn, &fqdn_port, &addr, &addr6, request->h.uri);
        if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
            ogs_error("[%s:%s] Invalid URL [%s]",
                    OpenAPI_nf_type_ToString(nf_instance->nf_type),
                    nf_instance->id, request->h.uri);
            return false;
        }
        client = ogs_sbi_client_find(scheme, fqdn, fqdn_port, addr, addr6);
        if (!client) {
            ogs_error("[%s:%s] Cannot find client [%s:%d]",
                    OpenAPI_nf_type_ToString(nf_instance->nf_type),
                    nf_instance->id,
                    OGS_ADDR(addr, buf), OGS_PORT(addr));
            ogs_free(fqdn);
            ogs_freeaddrinfo(addr);
            ogs_freeaddrinfo(addr6);
            return false;
        }

        ogs_free(fqdn);
        ogs_freeaddrinfo(addr);
        ogs_freeaddrinfo(addr6);
#endif
    }

    if (client->fqdn && ogs_sbi_fqdn_in_vplmn(client->fqdn) == true) {
        ogs_sbi_client_t *sepp_client = NULL, *nrf_client = NULL;

        /* Visited Network requires SEPP */
        sepp_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->sepp_instance);
        nrf_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->nrf_instance);

        if (!sepp_client && !nrf_client) {

            ogs_error("No SEPP(%p) and NRF(%p) [%s]",
                    sepp_client, nrf_client, client->fqdn);

            ogs_sbi_xact_remove(xact);
            return false;

        } else if (!sepp_client) {

            ogs_sbi_request_t *nrf_request = NULL;

            xact->target_apiroot = ogs_sbi_client_apiroot(client);
            if (!xact->target_apiroot) {
                ogs_error("ogs_strdup(xact->target_apiroot) failed");
                ogs_sbi_xact_remove(xact);
                return false;
            }

            nrf_request = ogs_nnrf_disc_build_discover(
                        OpenAPI_nf_type_SEPP, xact->requester_nf_type, NULL);
            if (!nrf_request) {
                ogs_error("ogs_nnrf_disc_build_discover() failed");
                ogs_sbi_xact_remove(xact);
                return false;
            }

            rc = ogs_sbi_client_send_request(
                    nrf_client, sepp_discover_handler, nrf_request,
                    OGS_UINT_TO_POINTER(xact->id));
            if (rc == false) {
                ogs_error("ogs_sbi_client_send_request() failed");
                ogs_sbi_xact_remove(xact);
            }

            ogs_sbi_request_free(nrf_request);

            return rc;
        }
    }

    rc = ogs_sbi_send_request_to_client(
            client, ogs_sbi_client_handler, request,
            OGS_UINT_TO_POINTER(xact->id));
    if (rc == false) {
        ogs_error("ogs_sbi_send_request_to_client() failed");
        ogs_sbi_xact_remove(xact);
    }

    return rc;
}

bool ogs_sbi_send_request_to_client(
        ogs_sbi_client_t *client, ogs_sbi_client_cb_f client_cb,
        ogs_sbi_request_t *request, void *data)
{
    bool rc;
    ogs_sbi_client_t *scp_client = NULL, *sepp_client = NULL;
    ogs_sbi_client_t *scp_or_sepp = NULL;
    char *apiroot = NULL;

    /*
     * If the HTTP2 Server's EndPoint is known,
     * 3gpp-Sbi-Target-apiRoot should always be included in the HTTP2 Request.
     */
    ogs_assert(client);
    ogs_assert(request);

    scp_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);
    sepp_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->sepp_instance);

    if (scp_client && scp_client != client) {

        /*************************
         * INDIRECT COMMUNICATION
         *************************/
        scp_or_sepp = scp_client;

    } else if (client->fqdn && ogs_sbi_fqdn_in_vplmn(client->fqdn) == true) {

        /***************************
         * SEPP for Visited Network
         ***************************/
        if (sepp_client && sepp_client != client) {
            scp_or_sepp = sepp_client;
        } else {
            ogs_error("No SEPP [%s]", client->fqdn);
            return false;
        }
    }

    if (scp_or_sepp) {

        /* Added 3gpp-Sbi-Target-apiRoot to HTTP header */
        apiroot = ogs_sbi_client_apiroot(client);
        ogs_assert(apiroot);

        ogs_sbi_header_set(request->http.headers,
                OGS_SBI_CUSTOM_TARGET_APIROOT, apiroot);

        ogs_free(apiroot);

        rc = ogs_sbi_client_send_via_scp_or_sepp(
                scp_or_sepp, client_cb, request, data);
        ogs_expect(rc == true);

    } else {

        /***********************
         * DIRECT COMMUNICATION
         ***********************/

        /* Direct communication since `client' instance is always available */
        rc = ogs_sbi_client_send_request(
                client, client_cb, request, data);
        ogs_expect(rc == true);

    }

    return rc;
}

bool ogs_sbi_send_request_to_nrf(
        ogs_sbi_service_type_e nrf_service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_client_cb_f client_cb,
        ogs_sbi_request_t *request, void *data)
{
    bool rc;
    ogs_sbi_client_t *nrf_client = NULL, *scp_client = NULL;
    ogs_sbi_client_delegated_mode_e mode = OGS_SBI_CLIENT_DELEGATED_AUTO;

    ogs_assert(nrf_service_type);
    ogs_assert(request);

    scp_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);
    nrf_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->nrf_instance);

    /* Decide which delegated mode to use */
    if (nrf_service_type == OGS_SBI_SERVICE_TYPE_NNRF_NFM)
        mode = ogs_sbi_self()->client_delegated_config.nrf.nfm;
    else if (nrf_service_type == OGS_SBI_SERVICE_TYPE_NNRF_DISC)
        mode = ogs_sbi_self()->client_delegated_config.nrf.disc;
    /* else if it's some other Nnrf service, fallback to AUTO or keep default */

    switch (mode) {
    case OGS_SBI_CLIENT_DELEGATED_NO:
        /* NO => Direct communication (NRF must exist) */
        if (!nrf_client) {
            ogs_fatal("[No-NRF] Cannot send request [%s:%s:%s]",
                ogs_sbi_service_type_to_name(nrf_service_type),
                request->h.service.name, request->h.api.version);
            ogs_assert_if_reached();
            return false;
        }
        /* Send directly to NRF */
        rc = ogs_sbi_client_send_request(nrf_client, client_cb,
                                         request, data);
        ogs_expect(rc == true);
        break;

    case OGS_SBI_CLIENT_DELEGATED_YES:
        /* YES => Indirect communication (SCP must exist) */
        if (!scp_client) {
            ogs_fatal("[No-SCP] Cannot send request [%s:%s:%s]",
                ogs_sbi_service_type_to_name(nrf_service_type),
                request->h.service.name, request->h.api.version);
            ogs_assert_if_reached();
            return false;
        }
        /* Indirect via SCP, build discovery parameter if needed */
        build_default_discovery_parameter(request, nrf_service_type,
                                          discovery_option);
        rc = ogs_sbi_client_send_via_scp_or_sepp(scp_client, client_cb,
                                                 request, data);
        ogs_expect(rc == true);
        break;

    case OGS_SBI_CLIENT_DELEGATED_AUTO:
    default:
        /*
         * AUTO => If SCP is present, use it; otherwise direct.
         */
        if (scp_client) {
            build_default_discovery_parameter(request, nrf_service_type,
                                              discovery_option);
            rc = ogs_sbi_client_send_via_scp_or_sepp(scp_client, client_cb,
                                                     request, data);
            ogs_expect(rc == true);
        } else if (nrf_client) {
            rc = ogs_sbi_client_send_request(nrf_client, client_cb,
                                             request, data);
            ogs_expect(rc == true);
        } else {
            ogs_fatal("[No-NRF:No-SCP] Cannot send request [%s:%s:%s]",
                ogs_sbi_service_type_to_name(nrf_service_type),
                request->h.service.name, request->h.api.version);
            ogs_assert_if_reached();
            return false;
        }
        break;
    }

    return rc;
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

static int sepp_discover_handler(
        int status, ogs_sbi_response_t *response, void *data)
{
    int rv;
    char *strerror = NULL;
    ogs_sbi_message_t message;

    ogs_sbi_xact_t *xact = NULL;
    ogs_pool_id_t xact_id = 0;

    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *scp_client = NULL, *sepp_client = NULL;

    xact_id = OGS_POINTER_TO_UINT(data);
    ogs_assert(xact_id >= OGS_MIN_POOL_ID && xact_id <= OGS_MAX_POOL_ID);

    xact = ogs_sbi_xact_find_by_id(xact_id);
    if (!xact) {
        ogs_error("SBI transaction has already been removed");
        if (response)
            ogs_sbi_response_free(response);

        return OGS_ERROR;
    }

    if (status != OGS_OK) {
        ogs_log_message(
                status == OGS_DONE ? OGS_LOG_DEBUG : OGS_LOG_WARN, 0,
                "sepp_discover_handler() failed [%d]", status);

        if (response)
            ogs_sbi_response_free(response);

        ogs_sbi_xact_remove(xact);

        return OGS_ERROR;
    }

    ogs_assert(response);
    request = xact->request;
    ogs_assert(request);

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

    /*****************************
     * Check if SEPP is discovered
     *****************************/
    sepp_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->sepp_instance);
    if (!sepp_client) {
        strerror = ogs_msprintf("No SEPP");
        goto cleanup;
    }

    /* Added 3gpp-Sbi-Target-apiRoot to HTTP header */
    ogs_sbi_header_set(request->http.headers,
            OGS_SBI_CUSTOM_TARGET_APIROOT, xact->target_apiroot);

    /**********************************************************************
     * SCP should be checked considering 'discovery.delegated:no' situation
     **********************************************************************/
    scp_client = NF_INSTANCE_CLIENT(ogs_sbi_self()->scp_instance);

    if (false == ogs_sbi_client_send_via_scp_or_sepp(
                scp_client ? scp_client : sepp_client,
                ogs_sbi_client_handler, request,
                OGS_UINT_TO_POINTER(xact->id))) {
        strerror = ogs_msprintf("ogs_sbi_client_send_via_scp_or_sepp() failed");
        goto cleanup;
    }

    ogs_sbi_response_free(response);
    ogs_sbi_message_free(&message);

    return OGS_OK;

cleanup:
    ogs_assert(strerror);
    ogs_error("%s", strerror);

    ogs_free(strerror);

    ogs_sbi_xact_remove(xact);

    ogs_sbi_response_free(response);
    ogs_sbi_message_free(&message);

    return OGS_ERROR;
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
        if (discovery_option->num_of_service_names) {
            bool rc = false;

            /* send array items separated by a comma */
            char *v = ogs_sbi_discovery_option_build_service_names(
                        discovery_option);
            ogs_expect(v);

            if (v) {
                char *encoded = ogs_sbi_url_encode(v);
                ogs_expect(encoded);

                if (encoded) {
            /*
             * In http.params, the CURL library automatically encodes the URL.
             * http.headers implements open5gs to directly encode URLs.
             *
             * Since it is http.headers,
             * we need to encode `v` using ogs_sbi_url_encode();
             */
                    ogs_sbi_header_set(request->http.headers,
                            OGS_SBI_CUSTOM_DISCOVERY_SERVICE_NAMES, encoded);
                    ogs_free(encoded);

                    rc = true;
                }
                ogs_free(v);

            }

            if (rc == false)
                ogs_warn("invalid service names failed[%d:%s]",
                            discovery_option->num_of_service_names,
                            discovery_option->service_names[0]);
        }

        if (discovery_option->num_of_target_plmn_list) {
            char *v = ogs_sbi_discovery_option_build_plmn_list(
                    discovery_option->target_plmn_list,
                    discovery_option->num_of_target_plmn_list);
            if (v) {
                ogs_sbi_header_set(request->http.headers,
                        OGS_SBI_CUSTOM_DISCOVERY_TARGET_PLMN_LIST, v);
                ogs_free(v);
            } else {
                ogs_warn("invalid target-plmn-list failed[%d:%06x]",
                            discovery_option->num_of_target_plmn_list,
                            ogs_plmn_id_hexdump(
                                &discovery_option->target_plmn_list[0]));
            }
        }

        if (discovery_option->num_of_requester_plmn_list) {
            char *v = ogs_sbi_discovery_option_build_plmn_list(
                    discovery_option->requester_plmn_list,
                    discovery_option->num_of_requester_plmn_list);
            if (v) {
                ogs_sbi_header_set(request->http.headers,
                        OGS_SBI_CUSTOM_DISCOVERY_REQUESTER_PLMN_LIST, v);
                ogs_free(v);
            } else {
                ogs_warn("invalid target-plmn-list failed[%d:%06x]",
                            discovery_option->num_of_requester_plmn_list,
                            ogs_plmn_id_hexdump(
                                &discovery_option->requester_plmn_list[0]));
            }
        }
    }

    if (local_discovery_option)
        ogs_sbi_discovery_option_free(local_discovery_option);
}
