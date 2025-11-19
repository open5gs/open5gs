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

#include "nudm-handler.h"

static const char *links_member_name(OpenAPI_auth_type_e auth_type)
{
    if (auth_type == OpenAPI_auth_type_5G_AKA ||
        auth_type == OpenAPI_auth_type_EAP_AKA_PRIME) {
        return OGS_SBI_RESOURCE_NAME_5G_AKA;
    } else if (auth_type == OpenAPI_auth_type_EAP_TLS) {
        return OGS_SBI_RESOURCE_NAME_EAP_SESSION;
    }

    ogs_assert_if_reached();
    return NULL;
}

bool ausf_nudm_ueau_handle_get(ausf_ue_t *ausf_ue,
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_header_t header;
    ogs_sbi_response_t *response = NULL;

    char hxres_star_string[OGS_KEYSTRLEN(OGS_MAX_RES_LEN)];

    OpenAPI_authentication_info_result_t *AuthenticationInfoResult = NULL;
    OpenAPI_authentication_vector_t *AuthenticationVector = NULL;
    OpenAPI_ue_authentication_ctx_t UeAuthenticationCtx;
    OpenAPI_ue_authentication_ctx_5g_auth_data_t AV5G_AKA;
    OpenAPI_map_t *LinksValueScheme = NULL;
    OpenAPI_links_value_schema_t LinksValueSchemeValue;

    ogs_assert(ausf_ue);
    ogs_assert(stream);
    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    ogs_assert(recvmsg);

    AuthenticationInfoResult = recvmsg->AuthenticationInfoResult;
    if (!AuthenticationInfoResult) {
        ogs_error("[%s] No AuthenticationInfoResult", ausf_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No AuthenticationInfoResult", ausf_ue->suci,
                NULL));
        return false;
    }

    /* See TS29.509 6.1.7.3 Application Errors */
    if (AuthenticationInfoResult->auth_type !=
            OpenAPI_auth_type_5G_AKA) {
        ogs_error("[%s] Not supported Auth Method [%d]",
            ausf_ue->suci, AuthenticationInfoResult->auth_type);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_NOT_IMPLEMENTED,
                recvmsg, "Not supported Auth Method", ausf_ue->suci,
                NULL));
        return false;
    }

    AuthenticationVector =
        AuthenticationInfoResult->authentication_vector;
    if (!AuthenticationVector) {
        ogs_error("[%s] No AuthenticationVector", ausf_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No AuthenticationVector", ausf_ue->suci, NULL));
        return false;
    }

    if (AuthenticationVector->av_type != OpenAPI_av_type_5G_HE_AKA) {
        ogs_error("[%s] Not supported Auth Method [%d]",
            ausf_ue->suci, AuthenticationVector->av_type);
        /*
         * TS29.509
         * 5.2.2.2.2 5G AKA 
         *
         * On failure or redirection, one of the HTTP status code
         * listed in table 6.1.7.3-1 shall be returned with the message
         * body containing a ProblemDetails structure with the "cause"
         * attribute set to one of the application error listed in
         * Table 6.1.7.3-1.
         * Application Error: AUTHENTICATION_REJECTED
         * HTTP status code: 403 Forbidden 
         * Description: The user cannot be authenticated with this
         * authentication method e.g. only SIM data available 
         */
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_FORBIDDEN,
                recvmsg, "Not supported Auth Method", ausf_ue->suci,
                "AUTHENTICATION_REJECTED"));
        return false;
    }

    if (!AuthenticationVector->rand) {
        ogs_error("[%s] No AuthenticationVector.rand", ausf_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No AuthenticationVector.rand", ausf_ue->suci,
                NULL));
        return false;
    }

    if (!AuthenticationVector->xres_star) {
        ogs_error("[%s] No AuthenticationVector.xresStar",
                ausf_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No AuthenticationVector.xresStar", ausf_ue->suci,
                NULL));
        return false;
    }

    if (!AuthenticationVector->autn) {
        ogs_error("[%s] No AuthenticationVector.autn", ausf_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No AuthenticationVector.autn", ausf_ue->suci,
                NULL));
        return false;
    }

    if (!AuthenticationVector->kausf) {
        ogs_error("[%s] No AuthenticationVector.kausf", ausf_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No AuthenticationVector.kausf", ausf_ue->suci,
                NULL));
        return false;
    }

    if (!AuthenticationInfoResult->supi) {
        ogs_error("[%s] No AuthenticationVector.supi", ausf_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No AuthenticationVector.supi", ausf_ue->suci,
                NULL));
        return false;
    }

    /* SUPI */
    if (ausf_ue->supi) {
        ogs_hash_set(ausf_self()->supi_hash,
                ausf_ue->supi, strlen(ausf_ue->supi), NULL);
        ogs_free(ausf_ue->supi);
    }
    ausf_ue->supi = ogs_strdup(AuthenticationInfoResult->supi);
    ogs_assert(ausf_ue->supi);
    ogs_hash_set(ausf_self()->supi_hash,
            ausf_ue->supi, strlen(ausf_ue->supi), ausf_ue);

    ausf_ue->auth_type = AuthenticationInfoResult->auth_type;

    ogs_ascii_to_hex(
        AuthenticationVector->rand,
        strlen(AuthenticationVector->rand),
        ausf_ue->rand, sizeof(ausf_ue->rand));
    ogs_ascii_to_hex(
        AuthenticationVector->xres_star,
        strlen(AuthenticationVector->xres_star),
        ausf_ue->xres_star, sizeof(ausf_ue->xres_star));
    ogs_ascii_to_hex(
        AuthenticationVector->kausf,
        strlen(AuthenticationVector->kausf),
        ausf_ue->kausf, sizeof(ausf_ue->kausf));

    memset(&UeAuthenticationCtx, 0, sizeof(UeAuthenticationCtx));

    UeAuthenticationCtx.auth_type = ausf_ue->auth_type;

    memset(&AV5G_AKA, 0, sizeof(AV5G_AKA));
    AV5G_AKA.rand = AuthenticationVector->rand;
    AV5G_AKA.autn = AuthenticationVector->autn;

    ogs_kdf_hxres_star(ausf_ue->rand, ausf_ue->xres_star,
            ausf_ue->hxres_star);
    ogs_hex_to_ascii(ausf_ue->hxres_star, sizeof(ausf_ue->hxres_star),
            hxres_star_string, sizeof(hxres_star_string));
    AV5G_AKA.hxres_star = hxres_star_string;

    UeAuthenticationCtx._5g_auth_data = &AV5G_AKA;

    memset(&LinksValueSchemeValue, 0, sizeof(LinksValueSchemeValue));

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NAUSF_AUTH;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] =
            (char *)OGS_SBI_RESOURCE_NAME_UE_AUTHENTICATIONS;
    header.resource.component[1] = ausf_ue->ctx_id;
    header.resource.component[2] =
            (char *)OGS_SBI_RESOURCE_NAME_5G_AKA_CONFIRMATION;
    LinksValueSchemeValue.href = ogs_sbi_server_uri(server, &header);
    LinksValueScheme = OpenAPI_map_create(
            (char *)links_member_name(UeAuthenticationCtx.auth_type),
            &LinksValueSchemeValue);
    ogs_assert(LinksValueScheme);

    UeAuthenticationCtx._links = OpenAPI_list_create();
    ogs_assert(UeAuthenticationCtx._links);
    OpenAPI_list_add(UeAuthenticationCtx._links, LinksValueScheme);

    memset(&sendmsg, 0, sizeof(sendmsg));

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NAUSF_AUTH;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] =
            (char *)OGS_SBI_RESOURCE_NAME_UE_AUTHENTICATIONS;
    header.resource.component[1] = ausf_ue->ctx_id;

    sendmsg.http.location = ogs_sbi_server_uri(server, &header);
    sendmsg.http.content_type = (char *)OGS_SBI_CONTENT_3GPPHAL_TYPE;

    sendmsg.UeAuthenticationCtx = &UeAuthenticationCtx;

    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_CREATED);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    OpenAPI_list_free(UeAuthenticationCtx._links);
    OpenAPI_map_free(LinksValueScheme);

    ogs_free(LinksValueSchemeValue.href);
    ogs_free(sendmsg.http.location);

    return true;
}

bool ausf_nudm_ueau_handle_auth_removal_ind(ausf_ue_t *ausf_ue,
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(ausf_ue);
    ogs_assert(stream);

    memset(&sendmsg, 0, sizeof(sendmsg));
    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    return true;
}

bool ausf_nudm_ueau_handle_result_confirmation_inform(ausf_ue_t *ausf_ue,
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    char kseaf_string[OGS_KEYSTRLEN(OGS_SHA256_DIGEST_SIZE)];

    OpenAPI_confirmation_data_response_t ConfirmationDataResponse;
    OpenAPI_auth_event_t *AuthEvent = NULL;

    bool rc;
    ogs_sbi_client_t *client = NULL;
    OpenAPI_uri_scheme_e scheme = OpenAPI_uri_scheme_NULL;
    char *fqdn = NULL;
    uint16_t fqdn_port = 0;
    ogs_sockaddr_t *addr = NULL, *addr6 = NULL;

    ogs_assert(ausf_ue);
    ogs_assert(stream);

    ogs_assert(recvmsg);

    AuthEvent = recvmsg->AuthEvent;
    if (!AuthEvent) {
        ogs_error("[%s] No AuthEvent", ausf_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No AuthEvent", ausf_ue->suci, NULL));
        return false;
    }

    if (!recvmsg->http.location) {
        ogs_error("[%s] No Location", ausf_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No Location", ausf_ue->suci, NULL));
        return false;
    }

    rc = ogs_sbi_getaddr_from_uri(
            &scheme, &fqdn, &fqdn_port, &addr, &addr6, recvmsg->http.location);
    if (rc == false || scheme == OpenAPI_uri_scheme_NULL) {
        ogs_error("[%s] Invalid URI [%s]",
                ausf_ue->suci, recvmsg->http.location);

        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "Invalid URI", ausf_ue->suci, NULL));

        return false;
    }

    client = ogs_sbi_client_find(scheme, fqdn, fqdn_port, addr, addr6);
    if (!client) {
        ogs_debug("[%s] ogs_sbi_client_add()", ausf_ue->suci);
        client = ogs_sbi_client_add(scheme, fqdn, fqdn_port, addr, addr6);
        ogs_assert(client);
    }

    OGS_SBI_SETUP_CLIENT(&ausf_ue->auth_event, client);

    ogs_free(fqdn);
    ogs_freeaddrinfo(addr);
    ogs_freeaddrinfo(addr6);

    AUTH_EVENT_STORE(ausf_ue, recvmsg->http.location);

    memset(&ConfirmationDataResponse, 0, sizeof(ConfirmationDataResponse));

    if (AuthEvent->success == true)
        ausf_ue->auth_result = OpenAPI_auth_result_AUTHENTICATION_SUCCESS;
    else
        ausf_ue->auth_result = OpenAPI_auth_result_AUTHENTICATION_FAILURE;

    ConfirmationDataResponse.auth_result = ausf_ue->auth_result;
    ConfirmationDataResponse.supi = ausf_ue->supi;

    ogs_kdf_kseaf(ausf_ue->serving_network_name,
            ausf_ue->kausf, ausf_ue->kseaf);
    ogs_hex_to_ascii(ausf_ue->kseaf, sizeof(ausf_ue->kseaf),
            kseaf_string, sizeof(kseaf_string));
    ConfirmationDataResponse.kseaf = kseaf_string;

    memset(&sendmsg, 0, sizeof(sendmsg));

    sendmsg.ConfirmationDataResponse = &ConfirmationDataResponse;

    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    return true;
}
