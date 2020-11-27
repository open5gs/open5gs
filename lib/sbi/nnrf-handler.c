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

#include "ogs-sbi.h"

bool ogs_sbi_nnrf_handle_nf_profile(ogs_sbi_nf_instance_t *nf_instance,
        OpenAPI_nf_profile_t *NFProfile,
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    int rv;

    OpenAPI_lnode_t *node;

    ogs_assert(nf_instance);
    ogs_assert(NFProfile);

    if (!NFProfile) {
        ogs_error("No NFProfile");
        if (stream)
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        message, "No NFProfile", NULL);
        return false;
    }

    if (!NFProfile->nf_instance_id) {
        ogs_error("No NFProfile.NFInstanceId");
        if (stream)
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        message, "NFProfile", "No NFInstanceId");
        return false;
    }

    if (!NFProfile->nf_type) {
        ogs_error("No NFProfile.NFType");
        if (stream)
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        message, "NFProfile", "No NFType");
        return false;
    }

    if (!NFProfile->nf_status) {
        ogs_error("No NFProfile.NFStatus");
        if (stream)
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "NFProfile", "No NFStatus");
        return false;
    }

    ogs_sbi_nf_instance_clear(nf_instance);

    nf_instance->nf_type = NFProfile->nf_type;
    nf_instance->nf_status = NFProfile->nf_status;
    nf_instance->time.heartbeat_interval = NFProfile->heart_beat_timer;

    if (NFProfile->fqdn)
        ogs_fqdn_parse(nf_instance->fqdn,
                NFProfile->fqdn, strlen(NFProfile->fqdn));

    /* Only one time handles RegisterNFInstance operation */
    OpenAPI_list_for_each(NFProfile->ipv4_addresses, node) {
        ogs_sockaddr_t *addr = NULL;

        if (!node->data) continue;

        if (nf_instance->num_of_ipv4 < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {

            rv = ogs_getaddrinfo(&addr, AF_UNSPEC,
                    node->data, OGS_SBI_HTTPS_PORT, 0);
            if (rv != OGS_OK) continue;

            nf_instance->ipv4[nf_instance->num_of_ipv4] = addr;
            nf_instance->num_of_ipv4++;
        }
    }
    OpenAPI_list_for_each(NFProfile->ipv6_addresses, node) {
        ogs_sockaddr_t *addr = NULL;

        if (!node->data) continue;

        if (nf_instance->num_of_ipv6 < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {

            rv = ogs_getaddrinfo(&addr, AF_UNSPEC,
                    node->data, OGS_SBI_HTTPS_PORT, 0);
            if (rv != OGS_OK) continue;

            nf_instance->ipv6[nf_instance->num_of_ipv6] = addr;
            nf_instance->num_of_ipv6++;
        }
    }

    OpenAPI_list_for_each(NFProfile->nf_services, node) {
        OpenAPI_nf_service_t *NFService = node->data;
        OpenAPI_list_t *VersionList = NULL;
        OpenAPI_list_t *IpEndPointList = NULL;
        OpenAPI_lnode_t *node2 = NULL;

        ogs_sbi_nf_service_t *nf_service = NULL;

        if (!NFService) continue;
        if (!NFService->service_instance_id) continue;
        if (!NFService->service_name) continue;

        VersionList = NFService->versions;
        IpEndPointList = NFService->ip_end_points;

        nf_service = ogs_sbi_nf_service_find(nf_instance,
                NFService->service_name);
        if (!nf_service) {
            nf_service = ogs_sbi_nf_service_add(nf_instance,
                NFService->service_instance_id,
                NFService->service_name, NFService->scheme);
            ogs_assert(nf_service);
        }

        ogs_sbi_nf_service_clear(nf_service);

        OpenAPI_list_for_each(VersionList, node2) {
            OpenAPI_nf_service_version_t *NFServiceVersion = node2->data;

            if (!NFServiceVersion) continue;

            ogs_sbi_nf_service_add_version(nf_service,
                        NFServiceVersion->api_version_in_uri,
                        NFServiceVersion->api_full_version,
                        NFServiceVersion->expiry);
        }

        if (NFService->fqdn)
            ogs_fqdn_parse(nf_service->fqdn,
                    NFService->fqdn, strlen(NFService->fqdn));

        OpenAPI_list_for_each(IpEndPointList, node2) {
            OpenAPI_ip_end_point_t *IpEndPoint = node2->data;
            ogs_sockaddr_t *addr = NULL, *addr6 = NULL;
            int port = 0;

            if (!IpEndPoint) continue;

            if (nf_service->num_of_addr < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {
                port = IpEndPoint->port;
                if (!port) {
                    if (nf_service->scheme == OpenAPI_uri_scheme_http)
                        port = OGS_SBI_HTTP_PORT;
                    else if (nf_service->scheme == OpenAPI_uri_scheme_https)
                        port = OGS_SBI_HTTPS_PORT;
                    else
                        continue;
                }

                if (IpEndPoint->ipv4_address) {
                    rv = ogs_getaddrinfo(&addr, AF_UNSPEC,
                            IpEndPoint->ipv4_address, port, 0);
                    if (rv != OGS_OK) continue;
                }
                if (IpEndPoint->ipv6_address) {
                    rv = ogs_getaddrinfo(&addr6, AF_UNSPEC,
                            IpEndPoint->ipv6_address, port, 0);
                    if (rv != OGS_OK) continue;
                }

                if (addr || addr6) {
                    nf_service->addr[nf_service->num_of_addr].
                        port = port;
                    nf_service->addr[nf_service->num_of_addr].
                        ipv4 = addr;
                    nf_service->addr[nf_service->num_of_addr].
                        ipv6 = addr6;
                    nf_service->num_of_addr++;
                }
            }
        }
    }

    return true;
}
