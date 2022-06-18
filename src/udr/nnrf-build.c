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

#include "nnrf-build.h"

ogs_sbi_request_t *udr_nnrf_nfm_build_register(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_nf_profile_t *NFProfile = NULL;

    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);
    ogs_assert(nf_instance->id);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PUT;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;
    message.h.resource.component[1] = nf_instance->id;

    message.http.content_encoding = (char*)ogs_sbi_self()->content_encoding;

    NFProfile = ogs_nnrf_nfm_build_nf_profile();
    ogs_expect_or_return_val(NFProfile, NULL);

    message.NFProfile = NFProfile;

    request = ogs_sbi_build_request(&message);

    ogs_sbi_nnrf_free_nf_profile(NFProfile);

    return request;
}
