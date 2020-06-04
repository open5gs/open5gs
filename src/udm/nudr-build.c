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

#include "nudr-build.h"

ogs_sbi_request_t *udm_nudr_dr_build_query(udm_ue_t *udm_ue)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(udm_ue);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_GET;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDR_DR;
    message.h.api.version = (char *)OGS_SBI_API_VERSION;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SUBSCRIPTION_DATA;
    message.h.resource.component[1] = udm_ue->supi;
    message.h.resource.component[2] =
        (char *)OGS_SBI_RESOURCE_NAME_AUTHENTICATION_DATA;
    message.h.resource.component[3] =
        (char *)OGS_SBI_RESOURCE_NAME_AUTHENTICATION_SUBSCRIPTION;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    return request;
}

ogs_sbi_request_t *udm_nudr_dr_build_update(udm_ue_t *udm_ue)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_auth_event_t *AuthEvent = NULL;

    ogs_assert(udm_ue);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PUT;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDR_DR;
    message.h.api.version = (char *)OGS_SBI_API_VERSION;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SUBSCRIPTION_DATA;
    message.h.resource.component[1] = udm_ue->supi;
    message.h.resource.component[2] =
        (char *)OGS_SBI_RESOURCE_NAME_AUTHENTICATION_DATA;
    message.h.resource.component[3] =
        (char *)OGS_SBI_RESOURCE_NAME_AUTHENTICATION_STATUS;

    AuthEvent = ogs_calloc(1, sizeof(*AuthEvent));
    ogs_assert(AuthEvent);

    AuthEvent->nf_instance_id = udm_ue->ausf_instance_id;
    AuthEvent->success = udm_ue->auth_success;
    AuthEvent->auth_type = udm_ue->auth_type;
    AuthEvent->serving_network_name = udm_ue->serving_network_name;
    AuthEvent->time_stamp = udm_ue->auth_timestamp;

    message.AuthEvent = AuthEvent;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    ogs_free(AuthEvent);

    return request;
}
