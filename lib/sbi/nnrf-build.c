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

OpenAPI_nf_profile_t *ogs_nnrf_nfm_build_nf_profile(
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

    ogs_trace("[%s] ogs_nnrf_nfm_build_nf_profile()", nf_instance->id);

    ogs_trace("NF-Type[%s] NF-Status[%s] IPv4[%d] IPv6[%d]",
                OpenAPI_nf_type_ToString(nf_instance->nf_type),
                OpenAPI_nf_status_ToString(nf_instance->nf_status),
                nf_instance->num_of_ipv4, nf_instance->num_of_ipv6);

    if (strlen(nf_instance->fqdn)) {
        memset(fqdn, 0, sizeof(fqdn));
        fqdn_len = ogs_fqdn_build(fqdn,
                nf_instance->fqdn, strlen(nf_instance->fqdn));
        NFProfile->fqdn = ogs_memdup(fqdn, fqdn_len);

        ogs_trace("FQDN[%s]", nf_instance->fqdn);
    }

    Ipv4AddrList = OpenAPI_list_create();
    ogs_assert(Ipv4AddrList);
    Ipv6AddrList = OpenAPI_list_create();
    ogs_assert(Ipv6AddrList);

    for (i = 0; i < nf_instance->num_of_ipv4; i++) {
        if (nf_instance->ipv4[i]) {
            ogs_trace("IPv4 [family:%d, addr:%x, port:%d]",
                    nf_instance->ipv4[i]->ogs_sa_family,
                    htobe32(nf_instance->ipv4[i]->sin.sin_addr.s_addr),
                    nf_instance->ipv4[i]->ogs_sin_port);
            ogs_assert(nf_instance->ipv4[i]->ogs_sa_family == AF_INET);
            OpenAPI_list_add(Ipv4AddrList, ogs_ipstrdup(nf_instance->ipv4[i]));
        }
    }
    for (i = 0; i < nf_instance->num_of_ipv6; i++) {
        if (nf_instance->ipv6[i]) {
            ogs_trace("IPv6 [family:%d, port:%d]",
                    nf_instance->ipv6[i]->ogs_sa_family,
                    nf_instance->ipv6[i]->ogs_sin_port);
            ogs_assert(nf_instance->ipv6[i]->ogs_sa_family == AF_INET6);
            OpenAPI_list_add(Ipv6AddrList, ogs_ipstrdup(nf_instance->ipv6[i]));
        }
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

ogs_sbi_request_t *ogs_nnrf_nfm_build_register(
        ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    OpenAPI_nf_profile_t *NFProfile = NULL;

    ogs_assert(nf_instance);
    client = nf_instance->client;
    ogs_assert(client);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PUT;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;
    message.h.resource.component[1] = ogs_sbi_self()->nf_instance_id;

    message.http.content_encoding = (char*)ogs_sbi_self()->content_encoding;

    NFProfile = ogs_nnrf_nfm_build_nf_profile(nf_instance);
    ogs_assert(NFProfile);

    NFProfile->heart_beat_timer = nf_instance->time.heartbeat_interval;

    message.NFProfile = NFProfile;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    ogs_sbi_nnrf_free_nf_profile(NFProfile);

    return request;
}

ogs_sbi_request_t *ogs_nnrf_nfm_build_update(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_list_t *PatchItemList;
    OpenAPI_patch_item_t item;

    ogs_assert(nf_instance);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PATCH;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;
    message.h.resource.component[1] = ogs_sbi_self()->nf_instance_id;
    message.http.content_type = (char *)OGS_SBI_CONTENT_PATCH_TYPE;

    PatchItemList = OpenAPI_list_create();
    ogs_assert(PatchItemList);

    memset(&item, 0, sizeof(item));
    item.op = OpenAPI_patch_operation_replace;
    item.path = (char *)"/nfStatus";
    item.value = OpenAPI_nf_status_ToString(OpenAPI_nf_status_REGISTERED);

    OpenAPI_list_add(PatchItemList, &item);

    message.PatchItemList = PatchItemList;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    OpenAPI_list_free(PatchItemList);

    return request;
}

ogs_sbi_request_t *ogs_nnrf_nfm_build_de_register(
        ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(nf_instance);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_DELETE;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;
    message.h.resource.component[1] = nf_instance->id;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    return request;
}

ogs_sbi_request_t *ogs_nnrf_nfm_build_status_subscribe(
        ogs_sbi_subscription_t *subscription)
{
    ogs_sbi_message_t message;
    ogs_sbi_header_t header;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_server_t *server = NULL;

    OpenAPI_subscription_data_t *SubscriptionData = NULL;

    ogs_assert(subscription);
    ogs_assert(subscription->nf_type);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS;

    SubscriptionData = ogs_calloc(1, sizeof(*SubscriptionData));
    ogs_assert(SubscriptionData);

    server = ogs_list_first(&ogs_sbi_self()->server_list);
    ogs_assert(server);

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] =
            (char *)OGS_SBI_RESOURCE_NAME_NF_STATUS_NOTIFY;
    SubscriptionData->nf_status_notification_uri =
                        ogs_sbi_server_uri(server, &header);
    ogs_assert(SubscriptionData->nf_status_notification_uri);

	SubscriptionData->req_nf_type = subscription->nf_type;
    SubscriptionData->req_nf_instance_id = subscription->nf_instance_id;

    message.SubscriptionData = SubscriptionData;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    ogs_free(SubscriptionData->nf_status_notification_uri);
    ogs_free(SubscriptionData);

    return request;
}

ogs_sbi_request_t *ogs_nnrf_nfm_build_status_unsubscribe(
        ogs_sbi_subscription_t *subscription)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(subscription);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_DELETE;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS;
    message.h.resource.component[1] = subscription->id;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    return request;
}

ogs_sbi_request_t *ogs_nnrf_disc_build_discover(
        OpenAPI_nf_type_e target_nf_type, OpenAPI_nf_type_e requester_nf_type)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(target_nf_type);
    ogs_assert(requester_nf_type);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_GET;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_DISC;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;

    message.param.target_nf_type = target_nf_type;
    message.param.requester_nf_type = requester_nf_type;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    return request;
}
