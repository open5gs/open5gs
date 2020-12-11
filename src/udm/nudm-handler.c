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
    char *serving_network_name = NULL;
    char *ausf_instance_id = NULL;

    ogs_assert(udm_ue);
    ogs_assert(stream);
    ogs_assert(recvmsg);

    AuthenticationInfoRequest = recvmsg->AuthenticationInfoRequest;
    if (!AuthenticationInfoRequest) {
        ogs_error("[%s] No AuthenticationInfoRequest", udm_ue->suci);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No AuthenticationInfoRequest", udm_ue->suci);
        return false;
    }

    serving_network_name = AuthenticationInfoRequest->serving_network_name;
    if (!AuthenticationInfoRequest) {
        ogs_error("[%s] No servingNetworkName", udm_ue->suci);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No servingNetworkName", udm_ue->suci);
        return false;
    }

    if (udm_ue->serving_network_name)
        ogs_free(udm_ue->serving_network_name);
    udm_ue->serving_network_name = ogs_strdup(serving_network_name);

    ausf_instance_id = AuthenticationInfoRequest->ausf_instance_id;
    if (!AuthenticationInfoRequest) {
        ogs_error("[%s] No ausfInstanceId", udm_ue->suci);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No ausfInstanceId", udm_ue->suci);
        return false;
    }

    if (udm_ue->ausf_instance_id)
        ogs_free(udm_ue->ausf_instance_id);
    udm_ue->ausf_instance_id = ogs_strdup(ausf_instance_id);

    ResynchronizationInfo = AuthenticationInfoRequest->resynchronization_info;
    if (!ResynchronizationInfo) {

        udm_sbi_discover_and_send(OpenAPI_nf_type_UDR, udm_ue, stream, NULL,
                    udm_nudr_dr_build_authentication_subscription);

    } else {
        uint8_t rand[OGS_RAND_LEN];
        uint8_t auts[OGS_AUTS_LEN];
        uint8_t sqn_ms[OGS_SQN_LEN];
        uint8_t mac_s[OGS_MAC_S_LEN];
        uint64_t sqn = 0;

        if (!ResynchronizationInfo->rand) {
            ogs_error("[%s] No RAND", udm_ue->suci);
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No RAND", udm_ue->suci);
            return false;
        }

        if (!ResynchronizationInfo->auts) {
            ogs_error("[%s] No AUTS", udm_ue->suci);
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "No AUTS", udm_ue->suci);
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

            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    recvmsg, "Invalid RAND", udm_ue->suci);
            return false;
        }

        ogs_auc_sqn(udm_ue->opc, udm_ue->k, rand, auts, sqn_ms, mac_s);

        if (memcmp(auts + OGS_SQN_LEN, mac_s, OGS_MAC_S_LEN) != 0) {
            ogs_error("[%s] Re-synch MAC failed", udm_ue->suci);
            ogs_log_print(OGS_LOG_ERROR, "[MAC_S] ");
            ogs_log_hexdump(OGS_LOG_ERROR, mac_s, OGS_MAC_S_LEN);
            ogs_log_hexdump(OGS_LOG_ERROR, auts + OGS_SQN_LEN, OGS_MAC_S_LEN);
            ogs_log_hexdump(OGS_LOG_ERROR, sqn_ms, OGS_SQN_LEN);
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_UNAUTHORIZED,
                    recvmsg, "Re-sync MAC failed", udm_ue->suci);
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

        udm_sbi_discover_and_send(OpenAPI_nf_type_UDR,
                udm_ue, stream, udm_ue->sqn,
                udm_nudr_dr_build_authentication_subscription);
    }

    return true;
}

bool udm_nudm_ueau_handle_result_confirmation_inform(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    OpenAPI_auth_event_t *AuthEvent = NULL;

    ogs_assert(udm_ue);
    ogs_assert(stream);
    ogs_assert(message);

    AuthEvent = message->AuthEvent;
    if (!AuthEvent) {
        ogs_error("[%s] No AuthEvent", udm_ue->suci);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No AuthEvent", udm_ue->suci);
        return false;
    }

    if (!AuthEvent->nf_instance_id) {
        ogs_error("[%s] No nfInstanceId", udm_ue->suci);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No nfInstanceId", udm_ue->suci);
        return false;
    }

    if (!AuthEvent->success) {
        ogs_error("[%s] No success", udm_ue->suci);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No success", udm_ue->suci);
        return false;
    }

    if (!AuthEvent->time_stamp) {
        ogs_error("[%s] No timeStamp", udm_ue->suci);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No timeStamp", udm_ue->suci);
        return false;
    }

    if (!AuthEvent->auth_type) {
        ogs_error("[%s] No authType", udm_ue->suci);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No authType", udm_ue->suci);
        return false;
    }

    if (!AuthEvent->serving_network_name) {
        ogs_error("[%s] No servingNetworkName", udm_ue->suci);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No servingNetworkName", udm_ue->suci);
        return false;
    }

    udm_ue->auth_event = OpenAPI_auth_event_copy(
            udm_ue->auth_event, message->AuthEvent);

    udm_sbi_discover_and_send(OpenAPI_nf_type_UDR, udm_ue, stream, NULL,
            udm_nudr_dr_build_update_authentication_status);

    return true;
}

bool udm_nudm_uecm_handle_registration(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    OpenAPI_amf3_gpp_access_registration_t *Amf3GppAccessRegistration = NULL;
    OpenAPI_guami_t *Guami = NULL;

    ogs_assert(udm_ue);
    ogs_assert(stream);
    ogs_assert(message);

    Amf3GppAccessRegistration = message->Amf3GppAccessRegistration;
    if (!Amf3GppAccessRegistration) {
        ogs_error("[%s] No Amf3GppAccessRegistration", udm_ue->supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No Amf3GppAccessRegistration", udm_ue->supi);
        return false;
    }

    if (!Amf3GppAccessRegistration->dereg_callback_uri) {
        ogs_error("[%s] No dregCallbackUri", udm_ue->supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No dregCallbackUri", udm_ue->supi);
        return false;
    }

    Guami = Amf3GppAccessRegistration->guami;
    if (!Guami) {
        ogs_error("[%s] No Guami", udm_ue->supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No Guami", udm_ue->supi);
        return false;
    }

    if (!Guami->amf_id) {
        ogs_error("[%s] No Guami.AmfId", udm_ue->supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No Guami.AmfId", udm_ue->supi);
        return false;
    }

    if (!Guami->plmn_id) {
        ogs_error("[%s] No PlmnId", udm_ue->supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No PlmnId", udm_ue->supi);
        return false;
    }

    if (!Guami->plmn_id->mnc) {
        ogs_error("[%s] No PlmnId.Mnc", udm_ue->supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No PlmnId.Mnc", udm_ue->supi);
        return false;
    }

    if (!Guami->plmn_id->mcc) {
        ogs_error("[%s] No PlmnId.Mcc", udm_ue->supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No PlmnId.Mcc", udm_ue->supi);
        return false;
    }

    if (!Amf3GppAccessRegistration->rat_type) {
        ogs_error("[%s] No RatType", udm_ue->supi);
        ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "No RatType", udm_ue->supi);
        return false;
    }

    if (udm_ue->dereg_callback_uri)
        ogs_free(udm_ue->dereg_callback_uri);
    udm_ue->dereg_callback_uri = ogs_strdup(
            Amf3GppAccessRegistration->dereg_callback_uri);

    ogs_sbi_parse_guami(&udm_ue->guami, Guami);

    udm_ue->rat_type = Amf3GppAccessRegistration->rat_type;

    udm_ue->amf_3gpp_access_registration =
        OpenAPI_amf3_gpp_access_registration_copy(
            udm_ue->amf_3gpp_access_registration,
                message->Amf3GppAccessRegistration);

    udm_sbi_discover_and_send(OpenAPI_nf_type_UDR, udm_ue, stream, NULL,
            udm_nudr_dr_build_update_amf_context);

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
