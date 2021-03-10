/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "nnssf-build.h"

ogs_sbi_request_t *amf_nnssf_nsselection_build_get(
        amf_sess_t *sess, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(sess);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_GET;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNSSF_NSSELECTION;
    message.h.api.version = (char *)OGS_SBI_API_V2;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_NETWORK_SLICE_INFORMATION;

    message.param.nf_id = ogs_sbi_self()->nf_instance_id;
    message.param.nf_type = amf_self()->nf_type;

    message.param.slice_info_request_for_pdu_session_presence = true;
    message.param.roaming_indication = OpenAPI_roaming_indication_NON_ROAMING;
    memcpy(&message.param.s_nssai, &sess->s_nssai,
            sizeof(message.param.s_nssai));

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    return request;
}
