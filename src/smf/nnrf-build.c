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

ogs_sbi_request_t *smf_nnrf_build_nf_register(
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
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_VERSION;
    message.h.resource.name = (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;
    message.h.resource.id = ogs_sbi_self()->nf_instance_id;

    message.http.content_encoding = (char*)ogs_sbi_self()->content_encoding;

    NFProfile = ogs_sbi_nnrf_build_nf_profile(nf_instance);
    ogs_assert(NFProfile);

    NFProfile->heart_beat_timer = nf_instance->time.heartbeat;

    message.NFProfile = NFProfile;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    ogs_sbi_nnrf_free_nf_profile(NFProfile);

    return request;
}

ogs_sbi_request_t *smf_nnrf_build_nf_update(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_list_t *PatchItemList;
    OpenAPI_patch_item_t item;

    ogs_assert(nf_instance);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PATCH;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_VERSION;
    message.h.resource.name = (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;
    message.h.resource.id = ogs_sbi_self()->nf_instance_id;
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

ogs_sbi_request_t *smf_nnrf_build_nf_de_register(
        ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(nf_instance);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_DELETE;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_VERSION;
    message.h.resource.name = (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;
    message.h.resource.id = nf_instance->id;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    return request;
}

ogs_sbi_request_t *smf_nnrf_build_nf_status_subscribe(
        ogs_sbi_subscription_t *subscription)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_server_t *server = NULL;

    OpenAPI_subscription_data_t *SubscriptionData = NULL;

    ogs_assert(subscription);
    ogs_assert(subscription->nf_type);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_VERSION;
    message.h.resource.name = (char *)OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS;

    SubscriptionData = ogs_calloc(1, sizeof(*SubscriptionData));
    ogs_assert(SubscriptionData);

    server = ogs_list_first(&ogs_sbi_self()->server_list);
    ogs_assert(server);

    SubscriptionData->nf_status_notification_uri = ogs_sbi_server_uri(server,
                OGS_SBI_SERVICE_NAME_NRF_NFM, OGS_SBI_API_VERSION,
                OGS_SBI_RESOURCE_NAME_NF_STATUS_NOTIFY, NULL);
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

ogs_sbi_request_t *smf_nnrf_build_nf_status_unsubscribe(
        ogs_sbi_subscription_t *subscription)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(subscription);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_DELETE;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_VERSION;
    message.h.resource.name = (char *)OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS;
    message.h.resource.id = subscription->id;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    return request;
}

ogs_sbi_request_t *smf_nnrf_build_nf_discover(
        OpenAPI_nf_type_e target_nf_type, OpenAPI_nf_type_e requester_nf_type)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(target_nf_type);
    ogs_assert(requester_nf_type);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_GET;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NRF_DISC;
    message.h.api.version = (char *)OGS_SBI_API_VERSION;
    message.h.resource.name = (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;

    message.param.target_nf_type = target_nf_type;
    message.param.requester_nf_type = requester_nf_type;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    return request;
}
