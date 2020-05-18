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

#include "nnrf-build.h"

OpenAPI_nf_profile_t *ogs_sbi_nnrf_build_nf_profile(
        ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_nf_service_t *nf_service = NULL;

    OpenAPI_nf_profile_t *NFProfile = NULL;
    OpenAPI_list_t *Ipv4AddrList = NULL;
    OpenAPI_list_t *Ipv6AddrList = NULL;
    OpenAPI_list_t *NFServiceList = NULL;

    int i = 0;
    int fqdn_len;
    char fqdn[OGS_MAX_FQDN_LEN+1];

    ogs_assert(nf_instance);

    NFProfile = ogs_calloc(1, sizeof(*NFProfile));
    ogs_assert(NFProfile);

	NFProfile->nf_instance_id = nf_instance->id;
	NFProfile->nf_type = nf_instance->nf_type;
	NFProfile->nf_status = nf_instance->nf_status;

    if (strlen(nf_instance->fqdn)) {
        memset(fqdn, 0, sizeof(fqdn));
        fqdn_len = ogs_fqdn_build(fqdn,
                nf_instance->fqdn, strlen(nf_instance->fqdn));
        NFProfile->fqdn = ogs_memdup(fqdn, fqdn_len);
    }

    Ipv4AddrList = OpenAPI_list_create();
    ogs_assert(Ipv4AddrList);
    Ipv6AddrList = OpenAPI_list_create();
    ogs_assert(Ipv6AddrList);

    for (i = 0; i < nf_instance->num_of_ipv4; i++) {
        if (nf_instance->ipv4[i])
            OpenAPI_list_add(Ipv4AddrList, ogs_ipstrdup(nf_instance->ipv4[i]));
    }
    for (i = 0; i < nf_instance->num_of_ipv6; i++) {
        if (nf_instance->ipv6[i])
            OpenAPI_list_add(Ipv6AddrList, ogs_ipstrdup(nf_instance->ipv6[i]));
    }

    if (Ipv4AddrList->count)
        NFProfile->ipv4_addresses = Ipv4AddrList;
    else
        OpenAPI_list_free(Ipv4AddrList);
    if (Ipv6AddrList->count)
        NFProfile->ipv6_addresses = Ipv6AddrList;
    else
        OpenAPI_list_free(Ipv6AddrList);

    NFServiceList = OpenAPI_list_create();
    ogs_assert(NFServiceList);

    ogs_list_for_each(&nf_instance->nf_service_list, nf_service) {
        OpenAPI_nf_service_t *NFService = NULL;
        OpenAPI_list_t *VersionList = NULL;
        OpenAPI_list_t *IpEndPointList = NULL;

        NFService = ogs_calloc(1, sizeof(*NFService));
        ogs_assert(NFService);
        NFService->service_instance_id = ogs_strdup(nf_service->id);
        NFService->service_name = ogs_strdup(nf_service->name);

        VersionList = OpenAPI_list_create();
        ogs_assert(VersionList);

        for (i = 0; i < nf_service->num_of_version; i++) {
            OpenAPI_nf_service_version_t *NFServiceVersion = NULL;

            NFServiceVersion = ogs_calloc(1, sizeof(*NFServiceVersion));
            ogs_assert(NFServiceVersion);
            if (nf_service->versions[i].in_uri)
                NFServiceVersion->api_version_in_uri =
                    ogs_strdup(nf_service->versions[i].in_uri);
            if (nf_service->versions[i].full)
                NFServiceVersion->api_full_version =
                    ogs_strdup(nf_service->versions[i].full);
            if (nf_service->versions[i].expiry)
                NFServiceVersion->expiry =
                    ogs_strdup(nf_service->versions[i].expiry);

            OpenAPI_list_add(VersionList, NFServiceVersion);
        }

        ogs_assert(VersionList->count);
        NFService->versions = VersionList;

        NFService->scheme = nf_service->scheme;
        NFService->nf_service_status = nf_service->status;

        if (strlen(nf_service->fqdn)) {
            memset(fqdn, 0, sizeof(fqdn));
            fqdn_len = ogs_fqdn_build(fqdn,
                    nf_service->fqdn, strlen(nf_service->fqdn));
            NFService->fqdn = ogs_memdup(fqdn, fqdn_len);
        }

        IpEndPointList = OpenAPI_list_create();
        ogs_assert(IpEndPointList);

        for (i = 0; i < nf_service->num_of_addr; i++) {
            ogs_sockaddr_t *ipv4 = NULL;
            ogs_sockaddr_t *ipv6 = NULL;

            OpenAPI_ip_end_point_t *IpEndPoint = NULL;

            ipv4 = nf_service->addr[i].ipv4;
            ipv6 = nf_service->addr[i].ipv6;

            if (ipv4 || ipv6) {
                IpEndPoint = ogs_calloc(1, sizeof(*IpEndPoint));
                ogs_assert(IpEndPoint);
                if (ipv4) IpEndPoint->ipv4_address = ogs_ipstrdup(ipv4);
                if (ipv6) IpEndPoint->ipv6_address = ogs_ipstrdup(ipv6);
                IpEndPoint->port = nf_service->addr[i].port;
                OpenAPI_list_add(IpEndPointList, IpEndPoint);
            }
        }

        if (IpEndPointList->count)
            NFService->ip_end_points = IpEndPointList;
        else
            OpenAPI_list_free(IpEndPointList);

        OpenAPI_list_add(NFServiceList, NFService);
    }

    if (NFServiceList->count)
        NFProfile->nf_services = NFServiceList;
    else
        OpenAPI_list_free(NFServiceList);

    return NFProfile;
}

void ogs_sbi_nnrf_free_nf_profile(OpenAPI_nf_profile_t *NFProfile)
{
    OpenAPI_lnode_t *node = NULL;

    ogs_assert(NFProfile);

    OpenAPI_list_for_each(NFProfile->ipv4_addresses, node)
        ogs_free(node->data);
    OpenAPI_list_free(NFProfile->ipv4_addresses);
    OpenAPI_list_for_each(NFProfile->ipv6_addresses, node)
        ogs_free(node->data);
    OpenAPI_list_free(NFProfile->ipv6_addresses);

    OpenAPI_list_for_each(NFProfile->nf_services, node) {
        OpenAPI_lnode_t *node2;
        OpenAPI_nf_service_t *NFService = node->data;
        ogs_assert(NFService);

        ogs_free(NFService->service_instance_id);
        ogs_free(NFService->service_name);

        OpenAPI_list_for_each(NFService->versions, node2) {
            OpenAPI_nf_service_version_t *NFServiceVersion = node2->data;
            ogs_assert(NFServiceVersion);
            ogs_free(NFServiceVersion->api_version_in_uri);
            ogs_free(NFServiceVersion->api_full_version);
            if (NFServiceVersion->expiry)
                ogs_free(NFServiceVersion->expiry);
            ogs_free(NFServiceVersion);
        }
        OpenAPI_list_free(NFService->versions);

        OpenAPI_list_for_each(NFService->ip_end_points, node2) {
            OpenAPI_ip_end_point_t *IpEndPoint = node2->data;
            ogs_assert(IpEndPoint);
            if (IpEndPoint->ipv4_address)
                ogs_free(IpEndPoint->ipv4_address);
            if (IpEndPoint->ipv6_address)
                ogs_free(IpEndPoint->ipv6_address);
            ogs_free(IpEndPoint);
        }
        OpenAPI_list_free(NFService->ip_end_points);

        if (NFService->fqdn)
            ogs_free(NFService->fqdn);

        ogs_free(NFService);
    }
    OpenAPI_list_free(NFProfile->nf_services);

    if (NFProfile->fqdn)
        ogs_free(NFProfile->fqdn);

    ogs_free(NFProfile);
}
