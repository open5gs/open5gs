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
#include "ogs-app.h"

OpenAPI_nf_profile_t *ogs_nnrf_nfm_build_nf_profile(
        ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_nf_service_t *nf_service = NULL;

    OpenAPI_nf_profile_t *NFProfile = NULL;
    OpenAPI_list_t *Ipv4AddrList = NULL;
    OpenAPI_list_t *Ipv6AddrList = NULL;
    OpenAPI_list_t *AllowedNfTypeList = NULL;
    OpenAPI_list_t *NFServiceList = NULL;

    int i = 0;
#if SBI_FQDN_WITH_ONE_OCTET_LENGTH
    int fqdn_len;
    char fqdn[OGS_MAX_FQDN_LEN];
#endif

    char *ipstr = NULL;

    ogs_assert(nf_instance);

    NFProfile = ogs_calloc(1, sizeof(*NFProfile));
    ogs_expect_or_return_val(NFProfile, NULL);

	NFProfile->nf_instance_id = nf_instance->id;
	NFProfile->nf_type = nf_instance->nf_type;
	NFProfile->nf_status = nf_instance->nf_status;

    ogs_trace("[%s] ogs_nnrf_nfm_build_nf_profile()", nf_instance->id);

    ogs_trace("NF-Type[%s] NF-Status[%s] IPv4[%d] IPv6[%d]",
                OpenAPI_nf_type_ToString(nf_instance->nf_type),
                OpenAPI_nf_status_ToString(nf_instance->nf_status),
                nf_instance->num_of_ipv4, nf_instance->num_of_ipv6);

    if (nf_instance->time.heartbeat_interval) {
        NFProfile->is_heart_beat_timer = true;
        NFProfile->heart_beat_timer = nf_instance->time.heartbeat_interval;
    }
    NFProfile->is_nf_profile_changes_support_ind = true;
    NFProfile->nf_profile_changes_support_ind = true;

    if (nf_instance->fqdn) {
#if SBI_FQDN_WITH_ONE_OCTET_LENGTH
        memset(fqdn, 0, sizeof(fqdn));
        fqdn_len = ogs_fqdn_build(fqdn,
                nf_instance->fqdn, strlen(nf_instance->fqdn));
        NFProfile->fqdn = ogs_memdup(fqdn, fqdn_len+1);
        ogs_expect_or_return_val(NFProfile->fqdn, NULL);
        NFProfile->fqdn[fqdn_len] = 0;

        ogs_debug("NFInstance-FQDN[%s]", nf_instance->fqdn);
        ogs_log_hexdump(OGS_LOG_DEBUG,
                (unsigned char *)NFProfile->fqdn, fqdn_len);
#else
        NFProfile->fqdn = ogs_strdup(nf_instance->fqdn);
#endif
    }

    NFProfile->is_priority = true;
    NFProfile->priority = nf_instance->priority;
    NFProfile->is_capacity = true;
    NFProfile->capacity = nf_instance->capacity;
    NFProfile->is_load = true;
    NFProfile->load = nf_instance->load;

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
            ipstr = ogs_ipstrdup(nf_instance->ipv4[i]);
            ogs_expect_or_return_val(ipstr, NULL);
            OpenAPI_list_add(Ipv4AddrList, ipstr);
        }
    }
    for (i = 0; i < nf_instance->num_of_ipv6; i++) {
        if (nf_instance->ipv6[i]) {
            ogs_trace("IPv6 [family:%d, port:%d]",
                    nf_instance->ipv6[i]->ogs_sa_family,
                    nf_instance->ipv6[i]->ogs_sin_port);
            ogs_assert(nf_instance->ipv6[i]->ogs_sa_family == AF_INET6);
            ipstr = ogs_ipstrdup(nf_instance->ipv6[i]);
            ogs_expect_or_return_val(ipstr, NULL);
            OpenAPI_list_add(Ipv6AddrList, ipstr);
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

    AllowedNfTypeList = OpenAPI_list_create();
    ogs_assert(AllowedNfTypeList);

    for (i = 0; i < nf_instance->num_of_allowed_nf_type; i++) {
        OpenAPI_list_add(AllowedNfTypeList,
                (void *)(uintptr_t)nf_instance->allowed_nf_types[i]);
    }

    if (AllowedNfTypeList->count)
        NFProfile->allowed_nf_types = AllowedNfTypeList;
    else
        OpenAPI_list_free(AllowedNfTypeList);

    NFServiceList = OpenAPI_list_create();
    ogs_assert(NFServiceList);

    ogs_list_for_each(&nf_instance->nf_service_list, nf_service) {
        OpenAPI_nf_service_t *NFService = NULL;
        OpenAPI_list_t *VersionList = NULL;
        OpenAPI_list_t *IpEndPointList = NULL;
        OpenAPI_list_t *AllowedNfTypeList = NULL;

        NFService = ogs_calloc(1, sizeof(*NFService));
        ogs_expect_or_return_val(NFService, NULL);
        NFService->service_instance_id = ogs_strdup(nf_service->id);
        ogs_expect_or_return_val(NFService->service_instance_id, NULL);
        NFService->service_name = ogs_strdup(nf_service->name);
        ogs_expect_or_return_val(NFService->service_name, NULL);

        VersionList = OpenAPI_list_create();
        ogs_assert(VersionList);

        for (i = 0; i < nf_service->num_of_version; i++) {
            OpenAPI_nf_service_version_t *NFServiceVersion = NULL;

            NFServiceVersion = ogs_calloc(1, sizeof(*NFServiceVersion));
            ogs_expect_or_return_val(NFServiceVersion, NULL);
            if (nf_service->versions[i].in_uri) {
                NFServiceVersion->api_version_in_uri =
                    ogs_strdup(nf_service->versions[i].in_uri);
                ogs_expect_or_return_val(
                    NFServiceVersion->api_version_in_uri, NULL);
            }
            if (nf_service->versions[i].full) {
                NFServiceVersion->api_full_version =
                    ogs_strdup(nf_service->versions[i].full);
                ogs_expect_or_return_val(
                    NFServiceVersion->api_full_version, NULL);
            }
            if (nf_service->versions[i].expiry) {
                NFServiceVersion->expiry =
                    ogs_strdup(nf_service->versions[i].expiry);
                ogs_expect_or_return_val(
                    NFServiceVersion->expiry, NULL);
            }

            OpenAPI_list_add(VersionList, NFServiceVersion);
        }

        ogs_assert(VersionList->count);
        NFService->versions = VersionList;

        NFService->scheme = nf_service->scheme;
        NFService->nf_service_status = nf_service->status;

        if (nf_service->fqdn) {
#if SBI_FQDN_WITH_ONE_OCTET_LENGTH
            memset(fqdn, 0, sizeof(fqdn));
            fqdn_len = ogs_fqdn_build(fqdn,
                    nf_service->fqdn, strlen(nf_service->fqdn));
            NFService->fqdn = ogs_memdup(fqdn, fqdn_len+1);
            ogs_expect_or_return_val(NFService->fqdn, NULL);
            NFService->fqdn[fqdn_len] = 0;

            ogs_debug("NFService-FQDN[%s]", nf_service->fqdn);
            ogs_log_hexdump(OGS_LOG_DEBUG,
                    (unsigned char *)NFService->fqdn, fqdn_len);
#else
            NFService->fqdn = ogs_strdup(nf_service->fqdn);
#endif
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
                ogs_expect_or_return_val(IpEndPoint, NULL);
                if (ipv4) {
                    IpEndPoint->ipv4_address = ogs_ipstrdup(ipv4);
                    ogs_expect_or_return_val(IpEndPoint->ipv4_address, NULL);
                }
                if (ipv6) {
                    IpEndPoint->ipv6_address = ogs_ipstrdup(ipv6);
                    ogs_expect_or_return_val(IpEndPoint->ipv6_address, NULL);
                }
                IpEndPoint->is_port = true;
                IpEndPoint->port = nf_service->addr[i].port;
                OpenAPI_list_add(IpEndPointList, IpEndPoint);
            }
        }

        if (IpEndPointList->count)
            NFService->ip_end_points = IpEndPointList;
        else
            OpenAPI_list_free(IpEndPointList);

        AllowedNfTypeList = OpenAPI_list_create();
        ogs_assert(AllowedNfTypeList);

        for (i = 0; i < nf_service->num_of_allowed_nf_type; i++) {
            OpenAPI_list_add(AllowedNfTypeList,
                    (void *)(uintptr_t)nf_service->allowed_nf_types[i]);
        }

        if (AllowedNfTypeList->count)
            NFService->allowed_nf_types = AllowedNfTypeList;
        else
            OpenAPI_list_free(AllowedNfTypeList);

        NFService->is_priority = true;
        NFService->priority = nf_service->priority;
        NFService->is_capacity = true;
        NFService->capacity = nf_service->capacity;
        NFService->is_load = true;
        NFService->load = nf_service->load;

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

    OpenAPI_list_free(NFProfile->allowed_nf_types);

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

        OpenAPI_list_free(NFService->allowed_nf_types);

        if (NFService->fqdn)
            ogs_free(NFService->fqdn);

        ogs_free(NFService);
    }
    OpenAPI_list_free(NFProfile->nf_services);

    if (NFProfile->fqdn)
        ogs_free(NFProfile->fqdn);

    ogs_free(NFProfile);
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
    item.value = OpenAPI_any_type_create_string(
        OpenAPI_nf_status_ToString(OpenAPI_nf_status_REGISTERED));
    ogs_assert(item.value);

    OpenAPI_list_add(PatchItemList, &item);

    message.PatchItemList = PatchItemList;

    request = ogs_sbi_build_request(&message);

    OpenAPI_list_free(PatchItemList);
    OpenAPI_any_type_free(item.value);

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
    OpenAPI_subscription_data_subscr_cond_t SubscrCond;

    ogs_assert(subscription);
    ogs_assert(subscription->req_nf_type);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS;

    SubscriptionData = ogs_calloc(1, sizeof(*SubscriptionData));
    ogs_expect_or_return_val(SubscriptionData, NULL);

    server = ogs_list_first(&ogs_sbi_self()->server_list);
    ogs_expect_or_return_val(server, NULL);

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] =
            (char *)OGS_SBI_RESOURCE_NAME_NF_STATUS_NOTIFY;
    SubscriptionData->nf_status_notification_uri =
                        ogs_sbi_server_uri(server, &header);
    ogs_expect_or_return_val(
            SubscriptionData->nf_status_notification_uri, NULL);

	SubscriptionData->req_nf_type = subscription->req_nf_type;
    SubscriptionData->req_nf_instance_id = subscription->req_nf_instance_id;

    memset(&SubscrCond, 0, sizeof(SubscrCond));
    if (subscription->subscr_cond.nf_type) {
        SubscrCond.nf_type = subscription->subscr_cond.nf_type;
        SubscriptionData->subscr_cond = &SubscrCond;
    }

    message.SubscriptionData = SubscriptionData;

    request = ogs_sbi_build_request(&message);

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

    return request;
}
