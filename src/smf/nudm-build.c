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

#include "nudm-build.h"

ogs_sbi_request_t *smf_nudm_sdm_build_get(smf_sess_t *sess, void *data)
{
    smf_ue_t *smf_ue = NULL;
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);
    ogs_assert(smf_ue->supi);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_GET;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDM_SDM;
    message.h.api.version = (char *)OGS_SBI_API_V2;
    message.h.resource.component[0] = smf_ue->supi;
    message.h.resource.component[1] = data;

    message.param.single_nssai_presence = true;
    memcpy(&message.param.single_nssai, &sess->s_nssai,
            sizeof(message.param.single_nssai));

    if (sess->dnn)
        message.param.dnn = sess->dnn;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    return request;
}
