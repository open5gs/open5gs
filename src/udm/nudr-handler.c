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

#include "nudr-handler.h"

bool udm_nudr_dr_handle_subscription_authentication(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_header_t header;
    ogs_sbi_response_t *response = NULL;

#if 0
    const char *tmp[1] = "de8ca9df474091fe4e9263c5daa907e9";
    const char *tmp[1] = "cc3766b98a8031a7286a68c7f577ed2e"; /* For test */

    /* ISSUE-482 */
    const char *tmp[2] = {
        "3a1fa0f51fe50f324f8522b220fc62a2",
        "cc5539bf72824c879e47e73efc885021"
    };
    static int step = 0;
#endif

    uint8_t autn[OGS_AUTN_LEN];
    uint8_t ik[OGS_KEY_LEN];
    uint8_t ck[OGS_KEY_LEN];
    uint8_t ak[OGS_AK_LEN];
    uint8_t xres[OGS_MAX_RES_LEN];
    size_t xres_len = 8;
    uint8_t xres_star[OGS_MAX_RES_LEN];
    uint8_t kausf[OGS_SHA256_DIGEST_SIZE];

    char rand_string[OGS_KEYSTRLEN(OGS_RAND_LEN)];
    char autn_string[OGS_KEYSTRLEN(OGS_AUTN_LEN)];
    char kausf_string[OGS_KEYSTRLEN(OGS_SHA256_DIGEST_SIZE)];
    char xres_star_string[OGS_KEYSTRLEN(OGS_MAX_RES_LEN)];

    OpenAPI_authentication_subscription_t *AuthenticationSubscription = NULL;
    OpenAPI_authentication_info_result_t AuthenticationInfoResult;
    OpenAPI_authentication_vector_t AuthenticationVector;

    ogs_assert(udm_ue);
    ogs_assert(stream);
    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    ogs_assert(recvmsg);

    SWITCH(recvmsg->h.resource.component[3])
    CASE(OGS_SBI_RESOURCE_NAME_AUTHENTICATION_SUBSCRIPTION)
        SWITCH(recvmsg->h.method)
        CASE(OGS_SBI_HTTP_METHOD_GET)

            AuthenticationSubscription = recvmsg->AuthenticationSubscription;
            if (!AuthenticationSubscription) {
                ogs_error("[%s] No AuthenticationSubscription", udm_ue->suci);
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                        recvmsg, "No AuthenticationSubscription", udm_ue->suci);
                return false;
            }

            if (AuthenticationSubscription->authentication_method !=
                    OpenAPI_auth_method_5G_AKA) {
                ogs_error("[%s] Not supported Auth Method [%d]",
                        udm_ue->suci,
                        AuthenticationSubscription->authentication_method);
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_FORBIDDEN,
                        recvmsg, "Not supported Auth Method", udm_ue->suci);
                return false;

            }

            if (!AuthenticationSubscription->enc_permanent_key) {
                ogs_error("[%s] No encPermanentKey", udm_ue->suci);
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                        recvmsg, "No encPermanentKey", udm_ue->suci);
                return false;
            }
            if (!AuthenticationSubscription->enc_opc_key) {
                ogs_error("[%s] No encPermanentKey", udm_ue->suci);
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    recvmsg, "No encPermanentKey", udm_ue->suci);
                return false;
            }
            if (!AuthenticationSubscription->authentication_management_field) {
                ogs_error("[%s] No authenticationManagementField",
                        udm_ue->suci);
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    recvmsg, "No authenticationManagementField", udm_ue->suci);
                return false;
            }
            if (!AuthenticationSubscription->sequence_number) {
                ogs_error("[%s] No SequenceNumber", udm_ue->suci);
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    recvmsg, "No SequenceNumber", udm_ue->suci);
                return false;
            }
            if (!AuthenticationSubscription->sequence_number->sqn) {
                ogs_error("[%s] No SequenceNumber.sqn", udm_ue->suci);
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    recvmsg, "No SequenceNumber.sqn", udm_ue->suci);
                return false;
            }

            udm_ue->auth_type = OpenAPI_auth_type_5G_AKA;

            ogs_ascii_to_hex(
                AuthenticationSubscription->enc_opc_key,
                strlen(AuthenticationSubscription->enc_opc_key),
                udm_ue->opc, sizeof(udm_ue->opc));
            ogs_ascii_to_hex(
                AuthenticationSubscription->authentication_management_field,
                strlen(AuthenticationSubscription->
                    authentication_management_field),
                udm_ue->amf, sizeof(udm_ue->amf));
            ogs_ascii_to_hex(
                AuthenticationSubscription->enc_permanent_key,
                strlen(AuthenticationSubscription->enc_permanent_key),
                udm_ue->k, sizeof(udm_ue->k));
            ogs_ascii_to_hex(
                AuthenticationSubscription->sequence_number->sqn,
                strlen(AuthenticationSubscription->sequence_number->sqn),
                udm_ue->sqn, sizeof(udm_ue->sqn));

        CASE(OGS_SBI_HTTP_METHOD_PATCH)

            memset(&AuthenticationInfoResult,
                    0, sizeof(AuthenticationInfoResult));

            AuthenticationInfoResult.auth_type = udm_ue->auth_type;

            ogs_random(udm_ue->rand, OGS_RAND_LEN);
#if 0
            OGS_HEX(tmp[step], strlen(tmp[step]), udm_ue->rand);
            if (step == 0) step = 1; /* For supporting authentication failure */
            else step = 0;
#endif

            milenage_generate(udm_ue->opc, udm_ue->amf, udm_ue->k, udm_ue->sqn,
                    udm_ue->rand, autn, ik, ck, ak, xres, &xres_len);

            ogs_assert(udm_ue->serving_network_name);

            /* TS33.501 Annex A.2 : Kausf derviation function */
            ogs_kdf_kausf(
                    ck, ik,
                    udm_ue->serving_network_name, autn,
                    kausf);

            /* TS33.501 Annex A.4 : RES* and XRES* derivation function */
            ogs_kdf_xres_star(
                    ck, ik,
                    udm_ue->serving_network_name, udm_ue->rand, xres, xres_len,
                    xres_star);

            memset(&AuthenticationVector, 0, sizeof(AuthenticationVector));
            AuthenticationVector.av_type = OpenAPI_av_type_5G_HE_AKA;

            ogs_hex_to_ascii(udm_ue->rand, sizeof(udm_ue->rand),
                    rand_string, sizeof(rand_string));
            AuthenticationVector.rand = rand_string;
            ogs_hex_to_ascii(xres_star, sizeof(xres_star),
                    xres_star_string, sizeof(xres_star_string));
            AuthenticationVector.xres_star = xres_star_string;
            ogs_hex_to_ascii(autn, sizeof(autn),
                    autn_string, sizeof(autn_string));
            AuthenticationVector.autn = autn_string;
            ogs_hex_to_ascii(kausf, sizeof(kausf),
                    kausf_string, sizeof(kausf_string));
            AuthenticationVector.kausf = kausf_string;

            AuthenticationInfoResult.authentication_vector =
                &AuthenticationVector;

            memset(&sendmsg, 0, sizeof(sendmsg));

            ogs_assert(AuthenticationInfoResult.auth_type);
            sendmsg.AuthenticationInfoResult = &AuthenticationInfoResult;

            response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
            ogs_assert(response);
            ogs_sbi_server_send_response(stream, response);

            return true;

        DEFAULT
            ogs_error("Invalid HTTP method [%s]", recvmsg->h.method);
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_FORBIDDEN, recvmsg,
                    "Invalid HTTP method", recvmsg->h.method);
        END
        break;

    CASE(OGS_SBI_RESOURCE_NAME_AUTHENTICATION_STATUS)
        memset(&sendmsg, 0, sizeof(sendmsg));

        memset(&header, 0, sizeof(header));
        header.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDM_UEAU;
        header.api.version = (char *)OGS_SBI_API_V1;
        header.resource.component[0] = udm_ue->supi;
        header.resource.component[1] =
            (char *)OGS_SBI_RESOURCE_NAME_AUTH_EVENTS;
        header.resource.component[2] = udm_ue->ctx_id;

        sendmsg.http.location = ogs_sbi_server_uri(server, &header);
        sendmsg.AuthEvent = OpenAPI_auth_event_copy(
                sendmsg.AuthEvent, udm_ue->auth_event);

        response = ogs_sbi_build_response(&sendmsg,
                OGS_SBI_HTTP_STATUS_CREATED);
        ogs_assert(response);
        ogs_sbi_server_send_response(stream, response);

        ogs_free(sendmsg.http.location);
        OpenAPI_auth_event_free(sendmsg.AuthEvent);

        return true;

    DEFAULT
        ogs_error("Invalid resource name [%s]",
                recvmsg->h.resource.component[3]);
    END

    return false;
}

bool udm_nudr_dr_handle_subscription_context(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_header_t header;
    ogs_sbi_response_t *response = NULL;

    int status;

    OpenAPI_amf3_gpp_access_registration_t *Amf3GppAccessRegistration = NULL;

    ogs_assert(udm_ue);
    ogs_assert(stream);
    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    ogs_assert(recvmsg);

    SWITCH(recvmsg->h.resource.component[3])
    CASE(OGS_SBI_RESOURCE_NAME_AMF_3GPP_ACCESS)
        Amf3GppAccessRegistration = udm_ue->amf_3gpp_access_registration;
        if (!Amf3GppAccessRegistration) {
            ogs_error("[%s] No Amf3GppAccessRegistration", udm_ue->supi);
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No Amf3GppAccessRegistration", udm_ue->supi);
            return false;
        }

        if (!Amf3GppAccessRegistration->amf_instance_id) {
            ogs_error("[%s] No amfInstanceId", udm_ue->supi);
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No amfInstanceId", udm_ue->supi);
            return false;
        }

        memset(&sendmsg, 0, sizeof(sendmsg));

        memset(&header, 0, sizeof(header));
        header.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDM_UECM;
        header.api.version = (char *)OGS_SBI_API_V1;
        header.resource.component[0] = udm_ue->supi;
        header.resource.component[1] =
            (char *)OGS_SBI_RESOURCE_NAME_REGISTRATIONS;
        header.resource.component[2] =
            (char *)OGS_SBI_RESOURCE_NAME_AMF_3GPP_ACCESS;

        if (udm_ue->amf_instance_id &&
            strcmp(udm_ue->amf_instance_id,
                Amf3GppAccessRegistration->amf_instance_id) == 0)
            status = OGS_SBI_HTTP_STATUS_OK;
        else
            status = OGS_SBI_HTTP_STATUS_CREATED;

        if (status == OGS_SBI_HTTP_STATUS_CREATED)
            sendmsg.http.location = ogs_sbi_server_uri(server, &header);

        sendmsg.Amf3GppAccessRegistration =
            OpenAPI_amf3_gpp_access_registration_copy(
                sendmsg.Amf3GppAccessRegistration,
                    udm_ue->amf_3gpp_access_registration);

        response = ogs_sbi_build_response(&sendmsg, status);
        ogs_assert(response);
        ogs_sbi_server_send_response(stream, response);

        ogs_free(sendmsg.http.location);
        OpenAPI_amf3_gpp_access_registration_free(
                sendmsg.Amf3GppAccessRegistration);
        return true;

    DEFAULT
        ogs_error("Invalid resource name [%s]",
                recvmsg->h.resource.component[3]);
    END

    return false;
}

bool udm_nudr_dr_handle_subscription_provisioned(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(udm_ue);
    ogs_assert(stream);
    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    ogs_assert(recvmsg);

    SWITCH(recvmsg->h.resource.component[4])
    CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
        memset(&sendmsg, 0, sizeof(sendmsg));

        if (recvmsg->AccessAndMobilitySubscriptionData)
            sendmsg.AccessAndMobilitySubscriptionData =
                OpenAPI_access_and_mobility_subscription_data_copy(
                    sendmsg.AccessAndMobilitySubscriptionData,
                        recvmsg->AccessAndMobilitySubscriptionData);

        response = ogs_sbi_build_response(&sendmsg, recvmsg->res_status);
        ogs_assert(response);
        ogs_sbi_server_send_response(stream, response);

        OpenAPI_access_and_mobility_subscription_data_free(
                sendmsg.AccessAndMobilitySubscriptionData);
        break;

    CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA)
        memset(&sendmsg, 0, sizeof(sendmsg));

        response = ogs_sbi_build_response(&sendmsg, recvmsg->res_status);
        ogs_assert(response);
        ogs_sbi_server_send_response(stream, response);

        break;

    CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
        memset(&sendmsg, 0, sizeof(sendmsg));

        if (recvmsg->SessionManagementSubscriptionData)
            sendmsg.SessionManagementSubscriptionData =
                OpenAPI_session_management_subscription_data_copy(
                    sendmsg.SessionManagementSubscriptionData,
                        recvmsg->SessionManagementSubscriptionData);

        response = ogs_sbi_build_response(&sendmsg, recvmsg->res_status);
        ogs_assert(response);
        ogs_sbi_server_send_response(stream, response);

        OpenAPI_session_management_subscription_data_free(
                sendmsg.SessionManagementSubscriptionData);

        break;

    DEFAULT
        ogs_error("Invalid resource name [%s]",
                recvmsg->h.resource.component[3]);
        return false;
    END

    return true;
}
