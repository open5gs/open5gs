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

#include "nsmf-build.h"

ogs_sbi_request_t *pcf_nsmf_callback_build_smpolicycontrol_update(
        pcf_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_header_t header;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_server_t *server = NULL;

    OpenAPI_sm_policy_notification_t SmPolicyNotification;
    OpenAPI_sm_policy_decision_t *SmPolicyDecision;

    ogs_assert(sess);
    ogs_assert(sess->sm_policy_id);
    ogs_assert(sess->notification_uri);

    server = ogs_list_first(&ogs_sbi_self()->server_list);
    ogs_assert(server);

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_SM_POLICIES;
    header.resource.component[1] = sess->sm_policy_id;
    header.resource.component[2] = (char *)OGS_SBI_RESOURCE_NAME_UPDATE;

    memset(&SmPolicyNotification, 0, sizeof(SmPolicyNotification));

    SmPolicyNotification.resource_uri = ogs_sbi_server_uri(server, &header);
    ogs_assert(SmPolicyNotification.resource_uri);

    SmPolicyDecision = data;
    ogs_assert(SmPolicyDecision);

    SmPolicyNotification.sm_policy_decision = SmPolicyDecision;

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.uri = ogs_msprintf("%s/%s",
            sess->notification_uri, OGS_SBI_RESOURCE_NAME_UPDATE);
    ogs_assert(message.h.uri);

    message.SmPolicyNotification = &SmPolicyNotification;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    ogs_free(SmPolicyNotification.resource_uri);
    ogs_free(message.h.uri);

    return request;
}

ogs_sbi_request_t *pcf_nsmf_callback_build_smpolicycontrol_terminate(
        pcf_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_header_t header;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(sess);
    ogs_assert(sess->sm_policy_id);
    ogs_assert(sess->notification_uri);

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_SM_POLICIES;
    header.resource.component[1] = sess->sm_policy_id;
    header.resource.component[2] = (char *)OGS_SBI_RESOURCE_NAME_UPDATE;

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.uri = ogs_msprintf("%s/%s",
            sess->notification_uri, OGS_SBI_RESOURCE_NAME_TERMINATE);
    ogs_assert(message.h.uri);

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    ogs_free(message.h.uri);

    return request;
}
