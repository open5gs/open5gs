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

#include "namf-build.h"

ogs_sbi_request_t *pcf_namf_callback_build_am_policy_control(
        pcf_ue_t *pcf_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_policy_update_t PolicyUpdate;

    ogs_assert(pcf_ue);
    ogs_assert(pcf_ue->notification_uri);

    memset(&PolicyUpdate, 0, sizeof(PolicyUpdate));

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.uri = pcf_ue->notification_uri;

    message.PolicyUpdate = &PolicyUpdate;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    return request;
}
