/*
 * Copyright (C) 2019-2024 by Sukchan Lee <acetcom@gmail.com>
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

ogs_sbi_request_t *nssf_nnssf_nsselection_build_get(
        nssf_home_t *home, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;
    nssf_nnssf_nsselection_param_t *param = data;

    ogs_assert(param);
    ogs_assert(home);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_GET;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNSSF_NSSELECTION;
    message.h.api.version = (char *)OGS_SBI_API_V2;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_NETWORK_SLICE_INFORMATION;

    message.param.nf_id = NF_INSTANCE_ID(ogs_sbi_self()->nf_instance);
    if (!message.param.nf_id) {
        ogs_error("No nf-id");
        goto end;
    }
    message.param.nf_type = NF_INSTANCE_TYPE(ogs_sbi_self()->nf_instance);
    if (!message.param.nf_type) {
        ogs_error("No nf-type");
        goto end;
    }

    message.param.slice_info_for_pdu_session_presence =
        param->slice_info_for_pdu_session.presence;

    if (!message.param.slice_info_for_pdu_session_presence) {
        ogs_error("No sliceInfoForPDUSession");
        goto end;
    }

    if (!param->slice_info_for_pdu_session.snssai) {
        ogs_error("No sNssai");
        goto end;
    }

    message.param.snssai_presence = true;
    memcpy(&message.param.s_nssai,
            param->slice_info_for_pdu_session.snssai,
            sizeof(message.param.s_nssai));

    message.param.roaming_indication =
        param->slice_info_for_pdu_session.roaming_indication;

    if (!message.param.roaming_indication) {
        ogs_error("No roamingIndication");
        goto end;
    }

    if (param->tai) {
        message.param.tai_presence = true;
        memcpy(&message.param.tai, param->tai, sizeof(message.param.tai));
    }

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:

    return request;
}
