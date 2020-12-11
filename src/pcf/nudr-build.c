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

ogs_sbi_request_t *pcf_nudr_dr_build_query_am_data(
        pcf_ue_t *pcf_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(pcf_ue);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_GET;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDR_DR;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_POLICY_DATA;
    message.h.resource.component[1] = (char *)OGS_SBI_RESOURCE_NAME_UES;
    message.h.resource.component[2] = pcf_ue->supi;
    message.h.resource.component[3] = (char *)OGS_SBI_RESOURCE_NAME_AM_DATA;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    return request;
}

ogs_sbi_request_t *pcf_nudr_dr_build_query_sm_data(
        pcf_sess_t *sess, void *data)
{
    pcf_ue_t *pcf_ue = NULL;

    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(sess);
    pcf_ue = sess->pcf_ue;
    ogs_assert(pcf_ue);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_GET;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDR_DR;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_POLICY_DATA;
    message.h.resource.component[1] = (char *)OGS_SBI_RESOURCE_NAME_UES;
    message.h.resource.component[2] = pcf_ue->supi;
    message.h.resource.component[3] = (char *)OGS_SBI_RESOURCE_NAME_SM_DATA;

    message.param.snssai_presence = true;
    memcpy(&message.param.snssai, &sess->s_nssai, sizeof(message.param.snssai));

    if (sess->dnn)
        message.param.dnn = sess->dnn;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    return request;
}
