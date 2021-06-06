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
    char *strerror = NULL;
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_header_t header;
    ogs_sbi_response_t *response = NULL;

#if 0
#if 0
    const char *tmp[1] = { "de8ca9df474091fe4e9263c5daa907e9" };
    /* PFCP test */
    const char *tmp[1] = { "cc3766b98a8031a7286a68c7f577ed2e" };
#endif
    /* Xn-Handover */
    const char *tmp[1] = { "5ca0df8c9bb8dbcf3c2a7dd448da1369" };

#if 0
    /* ISSUE-482 */
    const char *tmp[2] = {
        "3a1fa0f51fe50f324f8522b220fc62a2",
        "cc5539bf72824c879e47e73efc885021"
    };
#endif
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
            if (recvmsg->res_status != OGS_SBI_HTTP_STATUS_OK) {
                strerror = ogs_msprintf("[%s] HTTP response error [%d]",
                        udm_ue->suci, recvmsg->res_status);
                ogs_assert(strerror);

                if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_NOT_FOUND)
                    ogs_warn("%s", strerror);
                else
                    ogs_error("%s", strerror);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(
                        stream, recvmsg->res_status, recvmsg, strerror, NULL));
                ogs_free(strerror);
                return false;
            }

            AuthenticationSubscription = recvmsg->AuthenticationSubscription;
            if (!AuthenticationSubscription) {
                ogs_error("[%s] No AuthenticationSubscription", udm_ue->suci);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                        recvmsg, "No AuthenticationSubscription",
                        udm_ue->suci));
                return false;
            }

            if (AuthenticationSubscription->authentication_method !=
                    OpenAPI_auth_method_5G_AKA) {
                ogs_error("[%s] Not supported Auth Method [%d]",
                        udm_ue->suci,
                        AuthenticationSubscription->authentication_method);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_FORBIDDEN,
                        recvmsg, "Not supported Auth Method", udm_ue->suci));
                return false;

            }

            if (!AuthenticationSubscription->enc_permanent_key) {
                ogs_error("[%s] No encPermanentKey", udm_ue->suci);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                        recvmsg, "No encPermanentKey", udm_ue->suci));
                return false;
            }
            if (!AuthenticationSubscription->enc_opc_key) {
                ogs_error("[%s] No encPermanentKey", udm_ue->suci);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    recvmsg, "No encPermanentKey", udm_ue->suci));
                return false;
            }
            if (!AuthenticationSubscription->authentication_management_field) {
                ogs_error("[%s] No authenticationManagementField",
                        udm_ue->suci);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    recvmsg, "No authenticationManagementField", udm_ue->suci));
            return false;
        }
        if (!AuthenticationSubscription->sequence_number) {
            ogs_error("[%s] No SequenceNumber", udm_ue->suci);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    recvmsg, "No SequenceNumber", udm_ue->suci));
                return false;
            }
            if (!AuthenticationSubscription->sequence_number->sqn) {
                ogs_error("[%s] No SequenceNumber.sqn", udm_ue->suci);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    recvmsg, "No SequenceNumber.sqn", udm_ue->suci));
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
            if (recvmsg->res_status != OGS_SBI_HTTP_STATUS_OK &&
                recvmsg->res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                strerror = ogs_msprintf("[%s] HTTP response error [%d]",
                        udm_ue->suci, recvmsg->res_status);
                ogs_assert(strerror);

                ogs_error("%s", strerror);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(
                        stream, recvmsg->res_status, recvmsg, strerror, NULL));
                ogs_free(strerror);
                return false;
            }

            memset(&AuthenticationInfoResult,
                    0, sizeof(AuthenticationInfoResult));

            AuthenticationInfoResult.auth_type = udm_ue->auth_type;

            ogs_random(udm_ue->rand, OGS_RAND_LEN);
#if 0
            OGS_HEX(tmp[step], strlen(tmp[step]), udm_ue->rand);
#if 0
            if (step == 0) step = 1; /* For supporting authentication failure */
            else step = 0;
#endif
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
            ogs_assert(true == ogs_sbi_server_send_response(stream, response));

            break;

        DEFAULT
            ogs_error("Invalid HTTP method [%s]", recvmsg->h.method);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_FORBIDDEN, recvmsg,
                    "Invalid HTTP method", recvmsg->h.method));
            return false;
        END
        break;

    CASE(OGS_SBI_RESOURCE_NAME_AUTHENTICATION_STATUS)
        OpenAPI_auth_event_t *AuthEvent = NULL;

        if (recvmsg->res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT) {
            strerror = ogs_msprintf("[%s] HTTP response error [%d]",
                    udm_ue->suci, recvmsg->res_status);
            ogs_assert(strerror);

            ogs_error("%s", strerror);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, recvmsg->res_status, recvmsg, strerror, NULL));
            ogs_free(strerror);
            return false;
        }

        AuthEvent = udm_ue->auth_event;
        if (!AuthEvent) {
            ogs_error("[%s] No AuthEvent", udm_ue->suci);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No AuthEvent", udm_ue->suci));
            return false;
        }

        if (!AuthEvent->nf_instance_id) {
            ogs_error("[%s] No nfInstanceId", udm_ue->suci);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No nfInstanceId", udm_ue->suci));
            return false;
        }

        if (!AuthEvent->success) {
            ogs_error("[%s] No success", udm_ue->suci);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No success", udm_ue->suci));
            return false;
        }

        if (!AuthEvent->time_stamp) {
            ogs_error("[%s] No timeStamp", udm_ue->suci);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No timeStamp", udm_ue->suci));
            return false;
        }

        if (!AuthEvent->auth_type) {
            ogs_error("[%s] No authType", udm_ue->suci);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No authType", udm_ue->suci));
            return false;
        }

        if (!AuthEvent->serving_network_name) {
            ogs_error("[%s] No servingNetworkName", udm_ue->suci);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No servingNetworkName", udm_ue->suci));
            return false;
        }

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
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));

        ogs_free(sendmsg.http.location);
        OpenAPI_auth_event_free(sendmsg.AuthEvent);
        break;

    DEFAULT
        strerror = ogs_msprintf("[%s] Invalid resource name [%s]",
                udm_ue->supi, recvmsg->h.resource.component[3]);
        ogs_assert(strerror);

        ogs_error("%s", strerror);
        ogs_assert(true ==
            ogs_sbi_server_send_error(
                stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, strerror, NULL));
        ogs_free(strerror);
        return false;
    END

    return true;
}

bool udm_nudr_dr_handle_subscription_context(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    char *strerror = NULL;
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

    if (recvmsg->res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT) {
        ogs_error("[%s] HTTP response error [%d]",
            udm_ue->supi, recvmsg->res_status);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, recvmsg->res_status,
                NULL, "HTTP response error", udm_ue->supi));
        return false;
    }

    SWITCH(recvmsg->h.resource.component[3])
    CASE(OGS_SBI_RESOURCE_NAME_AMF_3GPP_ACCESS)
        Amf3GppAccessRegistration = udm_ue->amf_3gpp_access_registration;
        OpenAPI_guami_t *Guami = NULL;

        if (!Amf3GppAccessRegistration) {
            ogs_error("[%s] No Amf3GppAccessRegistration", udm_ue->supi);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No Amf3GppAccessRegistration", udm_ue->supi));
            return false;
        }

        if (!Amf3GppAccessRegistration->amf_instance_id) {
            ogs_error("[%s] No amfInstanceId", udm_ue->supi);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No amfInstanceId", udm_ue->supi));
            return false;
        }

        if (!Amf3GppAccessRegistration->dereg_callback_uri) {
            ogs_error("[%s] No dregCallbackUri", udm_ue->supi);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No dregCallbackUri", udm_ue->supi));
            return false;
        }

        Guami = Amf3GppAccessRegistration->guami;
        if (!Guami) {
            ogs_error("[%s] No Guami", udm_ue->supi);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No Guami", udm_ue->supi));
            return false;
        }

        if (!Guami->amf_id) {
            ogs_error("[%s] No Guami.AmfId", udm_ue->supi);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No Guami.AmfId", udm_ue->supi));
            return false;
        }

        if (!Guami->plmn_id) {
            ogs_error("[%s] No PlmnId", udm_ue->supi);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No PlmnId", udm_ue->supi));
            return false;
        }

        if (!Guami->plmn_id->mnc) {
            ogs_error("[%s] No PlmnId.Mnc", udm_ue->supi);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No PlmnId.Mnc", udm_ue->supi));
            return false;
        }

        if (!Guami->plmn_id->mcc) {
            ogs_error("[%s] No PlmnId.Mcc", udm_ue->supi);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No PlmnId.Mcc", udm_ue->supi));
            return false;
        }

        if (!Amf3GppAccessRegistration->rat_type) {
            ogs_error("[%s] No RatType", udm_ue->supi);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No RatType", udm_ue->supi));
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
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));

        ogs_free(sendmsg.http.location);
        OpenAPI_amf3_gpp_access_registration_free(
                sendmsg.Amf3GppAccessRegistration);
        break;

    DEFAULT
        strerror = ogs_msprintf("[%s] Invalid resource name [%s]",
                udm_ue->supi, recvmsg->h.resource.component[3]);
        ogs_assert(strerror);

        ogs_error("%s", strerror);
        ogs_assert(true ==
            ogs_sbi_server_send_error(
                stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, strerror, NULL));
        ogs_free(strerror);
        return false;
    END

    return true;
}

bool udm_nudr_dr_handle_subscription_provisioned(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    char *strerror = NULL;
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
        OpenAPI_access_and_mobility_subscription_data_t
            *AccessAndMobilitySubscriptionData = NULL;

        AccessAndMobilitySubscriptionData =
            recvmsg->AccessAndMobilitySubscriptionData;
        if (!AccessAndMobilitySubscriptionData) {
            ogs_error("[%s] No AccessAndMobilitySubscriptionData",
                    udm_ue->supi);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No AccessAndMobilitySubscriptionData",
                    udm_ue->supi));
            return false;
        }

        memset(&sendmsg, 0, sizeof(sendmsg));

        sendmsg.AccessAndMobilitySubscriptionData =
            OpenAPI_access_and_mobility_subscription_data_copy(
                sendmsg.AccessAndMobilitySubscriptionData,
                    recvmsg->AccessAndMobilitySubscriptionData);

        response = ogs_sbi_build_response(&sendmsg, recvmsg->res_status);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));

        OpenAPI_access_and_mobility_subscription_data_free(
                sendmsg.AccessAndMobilitySubscriptionData);
        break;

    CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECTION_SUBSCRIPTION_DATA)
        OpenAPI_smf_selection_subscription_data_t *SmfSelectionSubscriptionData;

        SmfSelectionSubscriptionData = recvmsg->SmfSelectionSubscriptionData;
        if (!SmfSelectionSubscriptionData) {
            ogs_error("[%s] No SmfSelectionSubscriptionData", udm_ue->supi);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No SmfSelectionSubscriptionData",
                    udm_ue->supi));
            return false;
        }

        memset(&SmfSelectionSubscriptionData, 0,
                sizeof(SmfSelectionSubscriptionData));

        memset(&sendmsg, 0, sizeof(sendmsg));

        sendmsg.SmfSelectionSubscriptionData =
            OpenAPI_smf_selection_subscription_data_copy(
                sendmsg.SmfSelectionSubscriptionData,
                    recvmsg->SmfSelectionSubscriptionData);

        response = ogs_sbi_build_response(&sendmsg, recvmsg->res_status);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));

        OpenAPI_smf_selection_subscription_data_free(
                sendmsg.SmfSelectionSubscriptionData);
        break;

    CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
        OpenAPI_session_management_subscription_data_t *
                SessionManagementSubscriptionData = NULL;

        SessionManagementSubscriptionData =
            recvmsg->SessionManagementSubscriptionData;
        if (!SessionManagementSubscriptionData) {
            ogs_error("[%s] No SessionManagementSubscriptionData",
                    udm_ue->supi);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No SessionManagementSubscriptionData",
                    udm_ue->supi));
            return false;
        }

        memset(&sendmsg, 0, sizeof(sendmsg));

        sendmsg.SessionManagementSubscriptionData =
            OpenAPI_session_management_subscription_data_copy(
                sendmsg.SessionManagementSubscriptionData,
                    recvmsg->SessionManagementSubscriptionData);

        response = ogs_sbi_build_response(&sendmsg, recvmsg->res_status);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));

        OpenAPI_session_management_subscription_data_free(
                sendmsg.SessionManagementSubscriptionData);

        break;

    DEFAULT
        strerror = ogs_msprintf("[%s] Invalid resource name [%s]",
                udm_ue->supi, recvmsg->h.resource.component[3]);
        ogs_assert(strerror);

        ogs_error("%s", strerror);
        ogs_assert(true ==
            ogs_sbi_server_send_error(
                stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, strerror, NULL));
        ogs_free(strerror);
        return false;
    END

    return true;
}
