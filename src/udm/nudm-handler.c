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

#include "sbi-path.h"
#include "nnrf-handler.h"
#include "nudm-handler.h"

bool udm_nudm_ueau_handle_get(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    OpenAPI_authentication_info_request_t *AuthenticationInfoRequest = NULL;
    OpenAPI_resynchronization_info_t *ResynchronizationInfo = NULL;
    int r;

    ogs_assert(udm_ue);
    ogs_assert(stream);
    ogs_assert(recvmsg);

    AuthenticationInfoRequest = recvmsg->AuthenticationInfoRequest;
    if (!AuthenticationInfoRequest) {
        ogs_error("[%s] No AuthenticationInfoRequest", udm_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No AuthenticationInfoRequest", udm_ue->suci,
                NULL));
        return false;
    }

    if (!AuthenticationInfoRequest->serving_network_name) {
        ogs_error("[%s] No servingNetworkName", udm_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No servingNetworkName", udm_ue->suci, NULL));
        return false;
    }

    if (!AuthenticationInfoRequest->ausf_instance_id) {
        ogs_error("[%s] No ausfInstanceId", udm_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No ausfInstanceId", udm_ue->suci, NULL));
        return false;
    }

    if (udm_ue->serving_network_name)
        ogs_free(udm_ue->serving_network_name);
    udm_ue->serving_network_name =
        ogs_strdup(AuthenticationInfoRequest->serving_network_name);
    ogs_assert(udm_ue->serving_network_name);

    if (udm_ue->ausf_instance_id)
        ogs_free(udm_ue->ausf_instance_id);
    udm_ue->ausf_instance_id =
        ogs_strdup(AuthenticationInfoRequest->ausf_instance_id);
    ogs_assert(udm_ue->ausf_instance_id);

    ResynchronizationInfo = AuthenticationInfoRequest->resynchronization_info;
    if (!ResynchronizationInfo) {

        r = udm_ue_sbi_discover_and_send(OGS_SBI_SERVICE_TYPE_NUDR_DR, NULL,
                udm_nudr_dr_build_authentication_subscription,
                udm_ue, stream, NULL);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);

    } else {
        uint8_t rand[OGS_RAND_LEN];
        uint8_t auts[OGS_AUTS_LEN];
        uint8_t sqn_ms[OGS_SQN_LEN];
        uint8_t mac_s[OGS_MAC_S_LEN];
        uint64_t sqn = 0;

        if (!ResynchronizationInfo->rand) {
            ogs_error("[%s] No RAND", udm_ue->suci);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No RAND", udm_ue->suci, NULL));
            return false;
        }

        if (!ResynchronizationInfo->auts) {
            ogs_error("[%s] No AUTS", udm_ue->suci);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No AUTS", udm_ue->suci, NULL));
            return false;
        }

        ogs_ascii_to_hex(
            ResynchronizationInfo->rand, strlen(ResynchronizationInfo->rand),
            rand, sizeof(rand));
        ogs_ascii_to_hex(
            ResynchronizationInfo->auts, strlen(ResynchronizationInfo->auts),
            auts, sizeof(auts));

        if (memcmp(udm_ue->rand, rand, OGS_RAND_LEN) != 0) {
            ogs_error("[%s] Invalid RAND", udm_ue->suci);
            ogs_log_hexdump(OGS_LOG_ERROR, udm_ue->rand, sizeof(udm_ue)->rand);
            ogs_log_hexdump(OGS_LOG_ERROR, rand, sizeof(rand));

            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "Invalid RAND", udm_ue->suci, NULL));
            return false;
        }

        ogs_auc_sqn(udm_ue->opc, udm_ue->k, rand, auts, sqn_ms, mac_s);

        if (memcmp(auts + OGS_SQN_LEN, mac_s, OGS_MAC_S_LEN) != 0) {
            ogs_error("[%s] Re-synch MAC failed", udm_ue->suci);
            ogs_log_print(OGS_LOG_ERROR, "[MAC_S] ");
            ogs_log_hexdump(OGS_LOG_ERROR, mac_s, OGS_MAC_S_LEN);
            ogs_log_hexdump(OGS_LOG_ERROR, auts + OGS_SQN_LEN, OGS_MAC_S_LEN);
            ogs_log_hexdump(OGS_LOG_ERROR, sqn_ms, OGS_SQN_LEN);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_UNAUTHORIZED,
                    recvmsg, "Re-sync MAC failed", udm_ue->suci, NULL));
            return false;

        }

        sqn = ogs_buffer_to_uint64(sqn_ms, OGS_SQN_LEN);

        /* 33.102 C.3.4 Guide : IND + 1
         *
         * General rule: index values IND used in the array scheme,
         * according to Annex C.1.2, shall be allocated cyclically
         * within its range 0, ... , a-1. This means that the index value IND
         * used with the previously generated authentication vector is stored
         * in SQN HE , and the next authentication vector shall use index
         * value IND +1 mod a.
         *
         * In future releases there may be additional information
         * about the requesting node identity. If this information is
         * available it is recommended to use it in the following way:
         *
         * - If the new request comes from the same serving node
         *   as the previous request, then the index value used for
         *   the new request shall be the same as was used for
         *   the previous request.
         */
        sqn = (sqn + 32 + 1) & OGS_MAX_SQN;

        ogs_uint64_to_buffer(sqn, OGS_SQN_LEN, udm_ue->sqn);

        r = udm_ue_sbi_discover_and_send(OGS_SBI_SERVICE_TYPE_NUDR_DR, NULL,
                udm_nudr_dr_build_authentication_subscription,
                udm_ue, stream, udm_ue->sqn);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
    }

    return true;
}

bool udm_nudm_ueau_handle_result_confirmation_inform(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    OpenAPI_auth_event_t *AuthEvent = NULL;
    int r;

    ogs_assert(udm_ue);
    ogs_assert(stream);
    ogs_assert(message);

    AuthEvent = message->AuthEvent;
    if (!AuthEvent) {
        ogs_error("[%s] No AuthEvent", udm_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No AuthEvent", udm_ue->suci, NULL));
        return false;
    }

    if (!AuthEvent->nf_instance_id) {
        ogs_error("[%s] No nfInstanceId", udm_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No nfInstanceId", udm_ue->suci, NULL));
        return false;
    }

    if (!AuthEvent->success) {
        ogs_error("[%s] No success", udm_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No success", udm_ue->suci, NULL));
        return false;
    }

    if (!AuthEvent->time_stamp) {
        ogs_error("[%s] No timeStamp", udm_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No timeStamp", udm_ue->suci, NULL));
        return false;
    }

    if (!AuthEvent->auth_type) {
        ogs_error("[%s] No authType", udm_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No authType", udm_ue->suci, NULL));
        return false;
    }

    if (!AuthEvent->serving_network_name) {
        ogs_error("[%s] No servingNetworkName", udm_ue->suci);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No servingNetworkName", udm_ue->suci, NULL));
        return false;
    }

    udm_ue->auth_event = OpenAPI_auth_event_copy(
            udm_ue->auth_event, message->AuthEvent);

    r = udm_ue_sbi_discover_and_send(OGS_SBI_SERVICE_TYPE_NUDR_DR, NULL,
            udm_nudr_dr_build_update_authentication_status,
            udm_ue, stream, NULL);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    return true;
}

bool udm_nudm_uecm_handle_amf_registration(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    OpenAPI_amf3_gpp_access_registration_t *Amf3GppAccessRegistration = NULL;
    OpenAPI_guami_t *Guami = NULL;
    int r;

    ogs_assert(udm_ue);
    ogs_assert(stream);
    ogs_assert(message);

    Amf3GppAccessRegistration = message->Amf3GppAccessRegistration;
    if (!Amf3GppAccessRegistration) {
        ogs_error("[%s] No Amf3GppAccessRegistration", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No Amf3GppAccessRegistration", udm_ue->supi,
                NULL));
        return false;
    }

    if (!Amf3GppAccessRegistration->amf_instance_id) {
        ogs_error("[%s] No amfInstanceId", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No amfInstanceId", udm_ue->supi, NULL));
        return false;
    }

    if (!Amf3GppAccessRegistration->dereg_callback_uri) {
        ogs_error("[%s] No dregCallbackUri", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No dregCallbackUri", udm_ue->supi, NULL));
        return false;
    }

    Guami = Amf3GppAccessRegistration->guami;
    if (!Guami) {
        ogs_error("[%s] No Guami", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No Guami", udm_ue->supi, NULL));
        return false;
    }

    if (!Guami->amf_id) {
        ogs_error("[%s] No Guami.AmfId", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No Guami.AmfId", udm_ue->supi, NULL));
        return false;
    }

    if (!Guami->plmn_id) {
        ogs_error("[%s] No PlmnId", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No PlmnId", udm_ue->supi, NULL));
        return false;
    }

    if (!Guami->plmn_id->mnc) {
        ogs_error("[%s] No PlmnId.Mnc", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No PlmnId.Mnc", udm_ue->supi, NULL));
        return false;
    }

    if (!Guami->plmn_id->mcc) {
        ogs_error("[%s] No PlmnId.Mcc", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No PlmnId.Mcc", udm_ue->supi, NULL));
        return false;
    }

    if (!Amf3GppAccessRegistration->rat_type) {
        ogs_error("[%s] No RatType", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No RatType", udm_ue->supi, NULL));
        return false;
    }

    if (udm_ue->dereg_callback_uri)
        ogs_free(udm_ue->dereg_callback_uri);
    udm_ue->dereg_callback_uri = ogs_strdup(
            Amf3GppAccessRegistration->dereg_callback_uri);
    ogs_assert(udm_ue->dereg_callback_uri);

    ogs_sbi_parse_guami(&udm_ue->guami, Guami);

    udm_ue->rat_type = Amf3GppAccessRegistration->rat_type;

    udm_ue->amf_3gpp_access_registration =
        OpenAPI_amf3_gpp_access_registration_copy(
            udm_ue->amf_3gpp_access_registration,
                message->Amf3GppAccessRegistration);

    r = udm_ue_sbi_discover_and_send(OGS_SBI_SERVICE_TYPE_NUDR_DR, NULL,
            udm_nudr_dr_build_update_amf_context, udm_ue, stream, NULL);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    return true;
}

bool udm_nudm_uecm_handle_amf_registration_update(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    OpenAPI_amf3_gpp_access_registration_modification_t
        *Amf3GppAccessRegistrationModification = NULL;
    OpenAPI_guami_t *Guami = NULL;
    ogs_guami_t recv_guami;
    OpenAPI_list_t *PatchItemList = NULL;
    OpenAPI_patch_item_t item;
    int r;

    ogs_assert(udm_ue);
    ogs_assert(stream);
    ogs_assert(message);

    Amf3GppAccessRegistrationModification = message->Amf3GppAccessRegistrationModification;
    if (!Amf3GppAccessRegistrationModification) {
        ogs_error("[%s] No Amf3GppAccessRegistrationModification", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No Amf3GppAccessRegistrationModification", udm_ue->supi,
                NULL));
        return false;
    }

    Guami = Amf3GppAccessRegistrationModification->guami;
    if (!Guami) {
        ogs_error("[%s] No Guami", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No Guami", udm_ue->supi, NULL));
        return false;
    }

    if (!Guami->amf_id) {
        ogs_error("[%s] No Guami.AmfId", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No Guami.AmfId", udm_ue->supi, NULL));
        return false;
    }

    if (!Guami->plmn_id) {
        ogs_error("[%s] No PlmnId", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No PlmnId", udm_ue->supi, NULL));
        return false;
    }

    if (!Guami->plmn_id->mnc) {
        ogs_error("[%s] No PlmnId.Mnc", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No PlmnId.Mnc", udm_ue->supi, NULL));
        return false;
    }

    if (!Guami->plmn_id->mcc) {
        ogs_error("[%s] No PlmnId.Mcc", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No PlmnId.Mcc", udm_ue->supi, NULL));
        return false;
    }

    /* TS 29.503: 5.3.2.4.2 AMF deregistration for 3GPP access
     * 2a. The UDM shall check whether the received GUAMI matches the stored
     * GUAMI. If so, the UDM shall set the PurgeFlag. The UDM responds with
     * "204 No Content".
     * 2b. Otherwise the UDM responds with "403 Forbidden". */
    ogs_sbi_parse_guami(&recv_guami, Guami);
    if (memcmp(&recv_guami, &udm_ue->guami, sizeof(recv_guami)) != 0) {
        ogs_error("[%s] Guami mismatch", udm_ue->supi);
        /*
         * TS29.503
         * 6.2.7.3 Application Errors
         *
         * Protocol and application errors common to several 5GC SBI API
         * specifications for which the NF shall include in the HTTP
         * response a payload body ("ProblemDetails" data structure or
         * application specific error data structure) with the "cause"
         * attribute indicating corresponding error are listed in table
         * 5.2.7.2-1.
         * Application Error: INVALID_GUAMI
         * HTTP status code: 403 Forbidden
         * Description: The AMF is not allowed to modify the registration
         * information stored in the UDM, as it is not the registered AMF.  
         */
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_FORBIDDEN,
                message, "Guami mismatch", udm_ue->supi,
                "INVALID_GUAMI"));
        return false;
    }

    if (Amf3GppAccessRegistrationModification->is_purge_flag) {
        ogs_assert(udm_ue->amf_3gpp_access_registration);
        udm_ue->amf_3gpp_access_registration->is_purge_flag =
                Amf3GppAccessRegistrationModification->is_purge_flag;
        udm_ue->amf_3gpp_access_registration->purge_flag =
                Amf3GppAccessRegistrationModification->purge_flag;
    }

    PatchItemList = OpenAPI_list_create();
    ogs_assert(PatchItemList);

    if (Amf3GppAccessRegistrationModification->is_purge_flag) {
        memset(&item, 0, sizeof(item));
        item.op = OpenAPI_patch_operation_replace;
        item.path = (char *)"PurgeFlag";
        item.value = OpenAPI_any_type_create_bool(
                Amf3GppAccessRegistrationModification->purge_flag);
        ogs_assert(item.value);

        OpenAPI_list_add(PatchItemList, &item);
    }

    r = udm_ue_sbi_discover_and_send(OGS_SBI_SERVICE_TYPE_NUDR_DR, NULL,
            udm_nudr_dr_build_patch_amf_context,
            udm_ue, stream, PatchItemList);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    return true;
}

bool udm_nudm_uecm_handle_amf_registration_get(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(udm_ue);
    ogs_assert(stream);
    ogs_assert(recvmsg);

    SWITCH(recvmsg->h.resource.component[1])
    CASE(OGS_SBI_RESOURCE_NAME_REGISTRATIONS)
        if (udm_ue->amf_3gpp_access_registration == NULL) {
            ogs_error("Invalid UE Identifier [%s]",
                    udm_ue->suci);
            return false;
        }
        memset(&sendmsg, 0, sizeof(sendmsg));
        sendmsg.Amf3GppAccessRegistration =
            OpenAPI_amf3_gpp_access_registration_copy(
                sendmsg.Amf3GppAccessRegistration,
                udm_ue->amf_3gpp_access_registration);
        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
        ogs_assert(response);
        ogs_sbi_server_send_response(stream, response);

        OpenAPI_amf3_gpp_access_registration_free(
                sendmsg.Amf3GppAccessRegistration);
        break;

    DEFAULT
        ogs_error("Invalid resource name [%s]",
                recvmsg->h.resource.component[3]);
        return false;
    END

    return true;
}

bool udm_nudm_uecm_handle_smf_registration(
    udm_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    udm_ue_t *udm_ue = NULL;
    OpenAPI_smf_registration_t *SmfRegistration = NULL;
    int r;

    ogs_assert(stream);
    ogs_assert(message);

    ogs_assert(sess);
    udm_ue = udm_ue_find_by_id(sess->udm_ue_id);
    ogs_assert(udm_ue);

    SmfRegistration = message->SmfRegistration;
    if (!SmfRegistration) {
        ogs_error("[%s:%d] No SmfRegistration", udm_ue->supi, sess->psi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No SmfRegistration", udm_ue->supi, NULL));
        return false;
    }

    if (!SmfRegistration->smf_instance_id) {
        ogs_error("[%s:%d] No smfInstanceId", udm_ue->supi, sess->psi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No smfInstanceId", udm_ue->supi, NULL));
        return false;
    }

    if (!SmfRegistration->pdu_session_id) {
        ogs_error("[%s:%d] No pduSessionId", udm_ue->supi, sess->psi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No pduSessionId", udm_ue->supi, NULL));
        return false;
    }

    if (!SmfRegistration->single_nssai) {
        ogs_error("[%s:%d] No singleNssai", udm_ue->supi, sess->psi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No singleNssai", udm_ue->supi, NULL));
        return false;
    }

    if (!SmfRegistration->dnn) {
        ogs_error("[%s:%d] No dnn", udm_ue->supi, sess->psi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No dnn", udm_ue->supi, NULL));
        return false;
    }

    if (!SmfRegistration->plmn_id ||
            !SmfRegistration->plmn_id->mcc || !SmfRegistration->plmn_id->mnc) {
        ogs_error("[%s:%d] No plmnId", udm_ue->supi, sess->psi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No plmnId", udm_ue->supi, NULL));
        return false;
    }

    sess->smf_registration =
        OpenAPI_smf_registration_copy(sess->smf_registration, SmfRegistration);

    r = udm_sess_sbi_discover_and_send(OGS_SBI_SERVICE_TYPE_NUDR_DR, NULL,
            udm_nudr_dr_build_update_smf_context, sess, stream, NULL);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    return true;
}

bool udm_nudm_uecm_handle_smf_deregistration(
    udm_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    udm_ue_t *udm_ue = NULL;
    int r;

    ogs_assert(stream);
    ogs_assert(message);

    ogs_assert(sess);
    udm_ue = udm_ue_find_by_id(sess->udm_ue_id);
    ogs_assert(udm_ue);

    r = udm_sess_sbi_discover_and_send(OGS_SBI_SERVICE_TYPE_NUDR_DR, NULL,
            udm_nudr_dr_build_delete_smf_context, sess, stream, NULL);
    ogs_expect(r == OGS_OK);
    ogs_assert(r != OGS_ERROR);

    return true;
}

bool udm_nudm_sdm_handle_subscription_provisioned(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(udm_ue);
    ogs_assert(stream);
    ogs_assert(recvmsg);

    SWITCH(recvmsg->h.resource.component[1])
    CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA)
        OpenAPI_ue_context_in_smf_data_t UeContextInSmfData;

        memset(&UeContextInSmfData, 0, sizeof(UeContextInSmfData));

        memset(&sendmsg, 0, sizeof(sendmsg));
        sendmsg.UeContextInSmfData = &UeContextInSmfData;

        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
        ogs_assert(response);
        ogs_sbi_server_send_response(stream, response);

        break;

    DEFAULT
        ogs_error("Invalid resource name [%s]",
                recvmsg->h.resource.component[3]);
        return false;
    END

    return true;
}

bool udm_nudm_sdm_handle_subscription_create(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;
    ogs_sbi_server_t *server = NULL;
    ogs_sbi_header_t header;

    OpenAPI_sdm_subscription_t *SDMSubscription = NULL;

    udm_sdm_subscription_t *sdm_subscription = NULL;
    
    ogs_assert(udm_ue);
    ogs_assert(stream);
    ogs_assert(recvmsg);

    SDMSubscription = recvmsg->SDMSubscription;
    if (!SDMSubscription) {
        ogs_error("[%s] No SDMSubscription", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No SDMSubscription", udm_ue->supi, NULL));
        return false;
    }

    if (!SDMSubscription->nf_instance_id) {
        ogs_error("[%s] No nfInstanceId", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No nfInstanceId", udm_ue->supi, NULL));
        return false;
    }

    if (!SDMSubscription->callback_reference) {
        ogs_error("[%s] No callbackReference", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No callbackReference", udm_ue->supi, NULL));
        return false;
    }

    /* Clang scan-build SA: NULL pointer dereference: change && to || in case monitored_resource_uris=NULL. */
    if ((!SDMSubscription->monitored_resource_uris) ||
        (!SDMSubscription->monitored_resource_uris->count)) {
        ogs_error("[%s] No monitoredResourceUris", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No monitoredResourceUris", udm_ue->supi, NULL));
        return false;
    }

    sdm_subscription = udm_sdm_subscription_add(udm_ue);
    ogs_assert(sdm_subscription);

    sdm_subscription->data_change_callback_uri =
        ogs_strdup(SDMSubscription->callback_reference);

    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NUDM_SDM;
    header.api.version = (char *)OGS_SBI_API_V2;
    header.resource.component[0] = udm_ue->supi;
    header.resource.component[1] =
            (char *)OGS_SBI_RESOURCE_NAME_SDM_SUBSCRIPTIONS;
    header.resource.component[2] = sdm_subscription->id;

    memset(&sendmsg, 0, sizeof(sendmsg));
    sendmsg.http.location = ogs_sbi_server_uri(server, &header);

    sendmsg.SDMSubscription = OpenAPI_sdm_subscription_copy(
            sendmsg.SDMSubscription, SDMSubscription);

    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_CREATED);
    ogs_assert(response);
    ogs_sbi_server_send_response(stream, response);

    ogs_free(sendmsg.http.location);
    OpenAPI_sdm_subscription_free(sendmsg.SDMSubscription);

    return true;
}

bool udm_nudm_sdm_handle_subscription_delete(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;
    udm_sdm_subscription_t *sdm_subscription;

    ogs_assert(udm_ue);
    ogs_assert(stream);
    ogs_assert(recvmsg);

    if (!recvmsg->h.resource.component[2]) {
        ogs_error("[%s] No subscriptionID", udm_ue->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No subscriptionID", udm_ue->supi, NULL));
        return false;
    }
    sdm_subscription = udm_sdm_subscription_find_by_id(
            recvmsg->h.resource.component[2]);

    if (sdm_subscription) {
        udm_sdm_subscription_remove(sdm_subscription);
    } else {
        ogs_error("Subscription to be deleted does not exist [%s]", 
                recvmsg->h.resource.component[2]);
        ogs_assert(true ==
            ogs_sbi_server_send_error(
                stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
                recvmsg, "Subscription Not found", recvmsg->h.method,
                NULL));
        return false;
    }

    memset(&sendmsg, 0, sizeof(sendmsg));
    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
    ogs_assert(response);
    ogs_sbi_server_send_response(stream, response);

    return true;
}
