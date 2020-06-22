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

#include "nausf-build.h"

ogs_sbi_request_t *amf_nausf_auth_build_authenticate(
        amf_ue_t *amf_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_authentication_info_t AuthenticationInfo;
    OpenAPI_resynchronization_info_t ResynchronizationInfo;
    uint8_t *auts = data;

    char rand_string[OGS_KEYSTRLEN(OGS_RAND_LEN)];
    char auts_string[OGS_KEYSTRLEN(OGS_AUTS_LEN)];

    ogs_assert(amf_ue);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NAUSF_AUTH;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_UE_AUTHENTICATIONS;

    message.http.accept = (char *)(OGS_SBI_CONTENT_3GPPHAL_TYPE ","
                                    OGS_SBI_CONTENT_PROBLEM_TYPE);

    memset(&AuthenticationInfo, 0, sizeof(AuthenticationInfo));

    ogs_assert(amf_ue->suci);
    AuthenticationInfo.supi_or_suci = amf_ue->suci;
    AuthenticationInfo.serving_network_name =
        ogs_serving_network_name_from_plmn_id(&amf_ue->tai.plmn_id);

    if (auts) {
        memset(&ResynchronizationInfo, 0, sizeof(ResynchronizationInfo));

        ogs_hex_to_ascii(amf_ue->rand, OGS_RAND_LEN,
                rand_string, sizeof(rand_string));
        ogs_hex_to_ascii(auts, OGS_AUTS_LEN, auts_string, sizeof(auts_string));

        ResynchronizationInfo.rand = rand_string;
        ResynchronizationInfo.auts = auts_string;

        AuthenticationInfo.resynchronization_info = &ResynchronizationInfo;
    }

    message.AuthenticationInfo = &AuthenticationInfo;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    ogs_free(AuthenticationInfo.serving_network_name);

    return request;
}

ogs_sbi_request_t *amf_nausf_auth_build_authenticate_confirmation(
        amf_ue_t *amf_ue, void *data)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    char xres_star_string[OGS_KEYSTRLEN(OGS_MAX_RES_LEN)];

    OpenAPI_confirmation_data_t *ConfirmationData = NULL;

    ogs_assert(amf_ue);
    ogs_assert(amf_ue->confirmation_url_for_5g_aka);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_PUT;
    message.h.uri = amf_ue->confirmation_url_for_5g_aka;

    ConfirmationData = ogs_calloc(1, sizeof(*ConfirmationData));
    ogs_assert(ConfirmationData);

    ogs_hex_to_ascii(amf_ue->xres_star, sizeof(amf_ue->xres_star),
            xres_star_string, sizeof(xres_star_string));

    ConfirmationData->res_star = xres_star_string;

    message.ConfirmationData = ConfirmationData;

    request = ogs_sbi_build_request(&message);
    ogs_assert(request);

    ogs_free(ConfirmationData);

    return request;
}
