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

ogs_sbi_request_t *nrf_nnrf_nfm_build_nf_status_notify(
        ogs_sbi_client_t *client, ogs_sbi_subscription_t *subscription,
        OpenAPI_notification_event_type_e event,
        ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_sbi_header_t header;
    ogs_sbi_server_t *server = NULL;

    OpenAPI_notification_data_t *NotificationData = NULL;

    ogs_assert(client);
    ogs_assert(subscription);
    ogs_assert(event);
    ogs_assert(nf_instance);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.uri = subscription->notification_uri;

    message.http.accept = (char *)OGS_SBI_CONTENT_PROBLEM_TYPE;

    NotificationData = ogs_calloc(1, sizeof(*NotificationData));
    ogs_expect_or_return_val(NotificationData, NULL);

    NotificationData->event = event;

    server = ogs_list_first(&ogs_sbi_self()->server_list);
    ogs_expect_or_return_val(server, NULL);

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    header.resource.component[1] = nf_instance->id;

    NotificationData->nf_instance_uri = ogs_sbi_server_uri(server, &header);
    ogs_expect_or_return_val(NotificationData->nf_instance_uri, NULL);

    if (event != OpenAPI_notification_event_type_NF_DEREGISTERED) {
        ogs_expect_or_return_val(nf_instance->nf_profile, NULL);
        NotificationData->nf_profile = nf_instance->nf_profile;
    }

    message.NotificationData = NotificationData;

    request = ogs_sbi_build_request(&message);
    ogs_expect_or_return_val(request, NULL);

    ogs_free(NotificationData->nf_instance_uri);
    ogs_free(NotificationData);

    return request;
}
