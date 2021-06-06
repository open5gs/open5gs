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

ogs_sbi_request_t *amf_nnrf_nfm_build_register(
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
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NNRF_NFM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_NF_INSTANCES;
    message.h.resource.component[1] = ogs_sbi_self()->nf_instance_id;

    message.http.content_encoding = (char*)ogs_sbi_self()->content_encoding;

    NFProfile = ogs_nnrf_nfm_build_nf_profile(nf_instance);
    ogs_expect_or_return_val(NFProfile, NULL);

    message.NFProfile = NFProfile;

    request = ogs_sbi_build_request(&message);

    ogs_sbi_nnrf_free_nf_profile(NFProfile);

    return request;
}

ogs_sbi_request_t *amf_nnrf_disc_build_discover(
        char *nrf_id,
        OpenAPI_nf_type_e target_nf_type, OpenAPI_nf_type_e requester_nf_type)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(nrf_id);
    ogs_assert(target_nf_type);
    ogs_assert(requester_nf_type);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_GET;
    message.h.uri = nrf_id;

    message.param.target_nf_type = target_nf_type;
    message.param.requester_nf_type = requester_nf_type;

    request = ogs_sbi_build_request(&message);

    return request;
}
